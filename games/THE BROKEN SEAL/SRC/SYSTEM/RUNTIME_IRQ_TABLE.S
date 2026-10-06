	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
.syntax unified
	.section .data,"aw",%progbits
	.arm

IwramIrqVBlank:
	.global gIrqHandlers
gIrqHandlers:
	.4byte 0
IwramIrqHBlank:
	.4byte 0
IwramIrqVCount:
	.4byte 0
IwramIrqTimer0:
	.4byte 0
IwramIrqTimer1:
	.4byte 0
IwramIrqTimer2:
	.4byte 0
IwramIrqTimer3:
	.4byte 0
IwramIrqSerial:
	.4byte 0
IwramIrqDma0:
	.4byte 0
IwramIrqDma1:
	.4byte 0
	.ifndef TBS_EDITION_EN
	.if EDITION_INTERNATIONAL
	.global IwramHalt
IwramHalt:
	.else
IwramIrqDma2:
	.endif
	.else
IwramIrqDma2:
	.endif
	.4byte 0
IwramIrqDma3:
	.4byte 0
IwramIrqKeypad:
	.4byte 0
IwramIrqGamePak:
	.4byte 0

	.section .rodata.irq_head,"a",%progbits
	.balign 4
	.global Runtime_IrqHandlers
Runtime_IrqHandlers:
	.rept 10
	.4byte Runtime_IgnoreInterrupt
	.endr
	.ifndef TBS_EDITION_EN
	.if EDITION_INTERNATIONAL
	.section .text.irq_halt,"ax",%progbits
	.arm
.L_irq_halt:
	svc	0x00030000
	b	.L_irq_halt
	.else
	.section .rodata.irq_middle,"a",%progbits
	.4byte Runtime_IgnoreInterrupt, Runtime_IgnoreInterrupt
	.endif
	.else
	.section .rodata.irq_middle,"a",%progbits
	.4byte Runtime_IgnoreInterrupt, Runtime_IgnoreInterrupt
	.endif
	.section .rodata.irq_tail,"a",%progbits
	.4byte Input_HandleKeyInterrupt, Runtime_IgnoreInterrupt
