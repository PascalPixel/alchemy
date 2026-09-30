.syntax unified
	.thumb
	.section .rodata.x020085f8,"a",%progbits
	.global gTitleEntrances
gTitleEntrances:
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x40000000
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTitleExits
gTitleExits:
	.4byte 0x000001ff
	.global gTitlePlacements
gTitlePlacements:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTitleEvents
gTitleEvents:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTitleVramBlock
gTitleVramBlock:
	.2byte 0xffff
	.section .bss,"aw",%nobits
	.space 58
	.global gTitleRevealFrame
gTitleRevealFrame:
	.space 20
	.global gTitleSprites
gTitleSprites:
	.space 216
