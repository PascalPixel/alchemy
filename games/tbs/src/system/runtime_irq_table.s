@ Why assembly: the ARM loop uses svc 0x00030000, a software
@ interrupt, then branches back to the same svc instruction.
	.include "games/tbs/include/game/ed_asm.h"
.syntax unified
	.section .data,"aw",%progbits
	.arm

	.global gIrqHandlers
gIrqHandlers:
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
	.global Iwram_Halt
Iwram_Halt:
	.endif
	.endif
	.4byte 0	@ Dma2
	.4byte 0	@ Dma3
	.4byte 0	@ Keypad
	.4byte 0	@ GamePak

	.section .rodata.irq_head,"a",%progbits
	.balign 4
	.global gDefaultIrqHandlers
gDefaultIrqHandlers:
	.rept 10
	.4byte Irq_Ignore
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
	.4byte Irq_Ignore, Irq_Ignore
	.endif
	.else
	.section .rodata.irq_middle,"a",%progbits
	.4byte Irq_Ignore, Irq_Ignore
	.endif
	.section .rodata.irq_tail,"a",%progbits
	.4byte Input_HandleKeyInterrupt, Irq_Ignore
