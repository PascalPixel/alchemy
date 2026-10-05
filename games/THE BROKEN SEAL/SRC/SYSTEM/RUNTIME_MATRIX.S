.syntax unified
	.section .data,"aw",%progbits
	.arm

	.global gTransform
gTransform:
	.4byte 0x00010000, 0, 0, 0
	.4byte 0x00010000, 0, 0, 0
	.4byte 0x00010000, 0, 0, 0
