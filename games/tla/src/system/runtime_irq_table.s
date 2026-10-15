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
	.global Runtime_IrqHandlers
Runtime_IrqHandlers:
	.4byte Runtime_IgnoreInterrupt	@ VBlank
	.4byte Runtime_IgnoreInterrupt	@ HBlank
	.4byte Runtime_IgnoreInterrupt	@ VCount
	.4byte Runtime_IgnoreInterrupt	@ Timer0
	.4byte Runtime_IgnoreInterrupt	@ Timer1
	.4byte Runtime_IgnoreInterrupt	@ Timer2
	.4byte Runtime_IgnoreInterrupt	@ Timer3
	.4byte Runtime_IgnoreInterrupt	@ Serial
	.4byte Runtime_IgnoreInterrupt	@ Dma0
	.4byte Runtime_IgnoreInterrupt	@ Dma1
	.4byte Runtime_IgnoreInterrupt	@ Dma2
	.4byte Runtime_IgnoreInterrupt	@ Dma3
	.4byte Input_HandleKeyInterrupt	@ Keypad
	.4byte Runtime_IgnoreInterrupt	@ GamePak
