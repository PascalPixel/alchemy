@ Why assembly: the ARM loop uses svc 0x00030000, a software
@ interrupt, then branches back to the same svc instruction.
	.include "games/tbs/include/edition.inc"
.syntax unified
	.section .data,"aw",%progbits
	.arm

	.global IrqHandlers
IrqHandlers:
	.4byte 0	@ VBlank
	.4byte 0	@ HBlank
	.4byte 0	@ VCount
	.4byte 0	@ Timer0
	.4byte 0	@ Timer1
	.4byte 0	@ Timer2
	.4byte 0	@ Timer3
	.4byte 0	@ Serial
	.4byte 0	@ Dma0
	.4byte 0	@ Dma1
	.ifndef TBS_EDITION_EN
	.if EDITION_INTERNATIONAL
	.global syHalt
syHalt:
	.endif
	.endif
	.4byte 0	@ Dma2
	.4byte 0	@ Dma3
	.4byte 0	@ Keypad
	.4byte 0	@ GamePak

	.section .rodata.irq_head,"a",%progbits
	.balign 4
	.global DefaultIrqHandlers
DefaultIrqHandlers:
	.rept 10
	.4byte syIrqIgnore
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
	.4byte syIrqIgnore, syIrqIgnore
	.endif
	.else
	.section .rodata.irq_middle,"a",%progbits
	.4byte syIrqIgnore, syIrqIgnore
	.endif
	.section .rodata.irq_tail,"a",%progbits
	.4byte syInputHandleKeyInterrupt, syIrqIgnore
