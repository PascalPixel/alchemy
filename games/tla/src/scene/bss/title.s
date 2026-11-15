.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.space 0x00000036
	.global gTitleRevealFrame
gTitleRevealFrame:
	.space 0x00000014
	.global gTitleSprites
gTitleSprites:
