.syntax unified
	.section .data,"aw",%progbits
	.arm
	.global gIrqHandlers
gIrqHandlers:
	.4byte	0x00000000, 0x00000000, 0x00000000, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000
	.4byte	0, 0, 0, 0, 0x00000000, 0x00000000
