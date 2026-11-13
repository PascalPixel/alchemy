.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gLemuria_RippleBuffer
gLemuria_RippleBuffer:
	.space 0x00000010
	.global gLemuria_RippleLines
gLemuria_RippleLines:
