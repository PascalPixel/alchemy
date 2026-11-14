.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.balign 16
	.global gPsynergyTest_LeaderRings
gPsynergyTest_LeaderRings:
	.space 0x000000b0
	.global gPsynergyTest_TargetRings
gPsynergyTest_TargetRings:
