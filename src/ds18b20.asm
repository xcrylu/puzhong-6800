; ============================================
; DS18B20温度传感器汇编驱动程序 (Proteus优化版)
; 适用MCU: STC89C52RC
; 时钟频率: 11.0592MHz (机器周期=1.085μs)
; DS18B20 DQ引脚: P3.7
; ============================================

; 使用REG52定义的端口，避免重复定义
;$INCLUDE (REG52.INC)

; ==================== 指令码定义 ====================
SKIP_ROM      EQU 0CCh  ; 跳过ROM指令
CONVERT_TEMP  EQU 044h  ; 温度转换指令
READ_SCRATCH  EQU 0BEh  ; 读取暂存器指令 1011,1110

; ==================== 代码段 ====================
                CSEG    AT 0
                AJMP    _main

                DSEG    AT 30H
temp_low:       DS 1
temp_high:      DS 1
temp_int:       DS 1
temp_frac:      DS 1

                CSEG
; ============================================
; 主程序入口
; ============================================
_main:
                MOV P1, #0FFh       ; P1口初始化

main_loop:
				;ACALL Delay_50ms
				;CLR P3.7				
				;ACALL Delay_50ms
				;SETB P3.7
				;MOV A, #03H
                ;ACALL DS18B20_WriteByte
				;sjmp main_loop

                ACALL DS18B20_Reset
                JC main_loop           ; 初始化失败，重试
				;sjmp main_loop
                
                ; 启动温度转换
                ACALL DS18B20_Reset
                MOV A, #SKIP_ROM
                ACALL DS18B20_WriteByte
                
                MOV A, #CONVERT_TEMP
                ACALL DS18B20_WriteByte
                ;sjmp main_loop
                ; 等待转换完成 (约750ms)
                ACALL Delay_100ms
                ACALL Delay_100ms
                ACALL Delay_100ms
                ACALL Delay_100ms
                ACALL Delay_100ms
                ACALL Delay_100ms
                ACALL Delay_100ms
                ACALL Delay_50ms
                
                ACALL DS18B20_Reset
                JC main_loop
                
                MOV A, #SKIP_ROM
                ACALL DS18B20_WriteByte
                
                MOV A, #READ_SCRATCH
                ACALL DS18B20_WriteByte
                
                ACALL DS18B20_ReadByte   ; 读取温度低字节
                MOV temp_low, A
                ;MOV P1, A
                ACALL DS18B20_ReadByte   ; 读取温度高字节
                MOV temp_high, A
                
				;sjmp main_loop
				
                ACALL Calc_Temp
                
                MOV P1, A
                
                SJMP main_loop        ; 循环读取

; ============================================
; DS18B20复位
; 返回: CY=1失败, CY=0成功
; ============================================
DS18B20_Reset:  SETB P3.7
                nop
                CLR P3.7
                ACALL Delay_480us
                SETB P3.7
                ACALL Delay_60us
                
                MOV C, P3.7
                ACALL Delay_420us
                
                RET

; ============================================
; DS18B20写一位
; 写0: 拉低60μs
; 写1: 拉低<1μs后释放
; ============================================
DS18B20_WriteBit:
                CLR P3.7             ; 拉低启动写时隙
                
                JC WriteBit_1        ; C=1跳转到写1
                
WriteBit_0:
                ACALL Delay_60us     ; 写0: 保持低电平60μs
                SETB P3.7
                RET
                
WriteBit_1:     ; 写1：最短低电平后释放
                SETB P3.7             ; 立即释放总线
                ACALL Delay_60us      ; 保持高电平到时隙结束
                RET

; ============================================
; DS18B20写字节
; ============================================
DS18B20_WriteByte:
                MOV R0, #8           ; 8位计数器
WB_Loop:
                RRC A                ; 右移，低位到CY
                ACALL DS18B20_WriteBit
                DJNZ R0, WB_Loop
                RET

; ============================================
; DS18B20读一位
; 返回: C = 读取的数据位
; ============================================
DS18B20_ReadBit:
                CLR P3.7
                nop
                nop
                SETB P3.7
                nop
                nop
                
                MOV C, P3.7
                
                ACALL Delay_60us
                RET

; ============================================
; DS18B20读一个字节
; 返回: A = 读取的字节
; ============================================
DS18B20_ReadByte:
                MOV R0, #8           ; 8位计数器
                MOV R1, #0           ; 结果寄存器
RB_Loop:
                ACALL DS18B20_ReadBit
                MOV A, R1
                RRC A
                MOV R1, A
                DJNZ R0, RB_Loop
                MOV A, R1
                RET

; ============================================
; 计算温度整数部分
; ============================================
Calc_Temp:
                MOV A, temp_high
                JB ACC.7, CT_Neg
                
                MOV A, temp_low
                SWAP A
                ANL A, #0Fh
                MOV R0, A
                
                MOV A, temp_high
                ANL A, #0Fh
				SWAP A
                ORL A, R0
                SJMP CT_Done
                
CT_Neg:
                MOV A, temp_low
                CPL A
                ADD A, #1
                MOV temp_low, A
                
                MOV A, temp_high
                CPL A
                ADDC A, #0
                MOV temp_high, A
                

                MOV A, temp_low
                MOV R0, A
                ANL A, #0Fh
                mov temp_frac, A
                mov A, R0
                
                MOV R0, A
                SWAP A
                ANL A, #0Fh
                MOV R0, A
                
                MOV A, temp_high
                ANL A, #0Fh
				SWAP A
                ORL A, R0
                mov temp_int,  A
                
                ; 取负
                CPL A
                INC A
                
CT_Done:
                RET

; ============================================
; 延时函数
; ============================================
Delay_1us:      RET                     ; 5周期=5.4μs

Delay_60us:     MOV R2, #25            ; 使用R2，避免与R0冲突
D60_LP:         DJNZ R2, D60_LP
                RET

Delay_480us:    MOV R2, #255           ; 使用R2
D480_LP:        DJNZ R2, D480_LP
                RET

Delay_420us:    MOV R2, #198           ; 使用R2
D420_LP:        DJNZ R2, D420_LP
                RET

; ============================================
; 毫秒级延时函数
; 公式: 总周期 = 3 + R6×(3 + 2×R7) + 2
; R6=100, R7=229: 46603周期=50.57ms
; ============================================
Delay_50ms:
                MOV R6, #100
D50_L1:         MOV R7, #229
D50_L2:         DJNZ R7, D50_L2
                DJNZ R6, D50_L1
                RET

Delay_100ms:
                ACALL Delay_50ms
                ACALL Delay_50ms
                RET

                END