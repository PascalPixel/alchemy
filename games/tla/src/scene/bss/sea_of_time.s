.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.space 0x00000002
	.global gSeaOfTime_RippleLines
gSeaOfTime_RippleLines:
	.space 0x00000140
	.global gSeaOfTime_SavedColors
gSeaOfTime_SavedColors:
