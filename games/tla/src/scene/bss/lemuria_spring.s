.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global gLemuriaSpring_Toss
gLemuriaSpring_Toss:
	.space 0x00000050
	.global gLemuriaSpring_TossChoice
gLemuriaSpring_TossChoice:
	.space 0x00000010
	.global gLemuriaSpring_Obstacles
gLemuriaSpring_Obstacles:
	.space 0x00000060
	.global gLemuriaSpring_TossFrame
gLemuriaSpring_TossFrame:
	.space 0x00000004
	.global gLemuriaSpring_TossDone
gLemuriaSpring_TossDone:
	.space 0x00000004
	.global gLemuriaSpring_TossResult
gLemuriaSpring_TossResult:
