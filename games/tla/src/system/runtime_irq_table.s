.syntax unified
	.section .data,"aw",%progbits
	.arm
	.global gIrqHandlers
gIrqHandlers:
	.4byte	0x00000000, 0x00000000, 0x00000000, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000
	.4byte	0, 0, 0, 0, 0x00000000, 0x00000000

@ The handlers the runtime copies into gIrqHandlers: only the keypad
@ interrupt has one of its own.
	.section .rodata.irq_handlers,"a",%progbits
	.balign 4
	.global gRuntime_IrqHandlers
gRuntime_IrqHandlers:
	.4byte Irq_Ignore	@ VBlank
	.4byte Irq_Ignore	@ HBlank
	.4byte Irq_Ignore	@ VCount
	.4byte Irq_Ignore	@ Timer0
	.4byte Irq_Ignore	@ Timer1
	.4byte Irq_Ignore	@ Timer2
	.4byte Irq_Ignore	@ Timer3
	.4byte Irq_Ignore	@ Serial
	.4byte Irq_Ignore	@ Dma0
	.4byte Irq_Ignore	@ Dma1
	.4byte Irq_Ignore	@ Dma2
	.4byte Irq_Ignore	@ Dma3
	.4byte Input_HandleKeyInterrupt	@ Keypad
	.4byte Irq_Ignore	@ GamePak
