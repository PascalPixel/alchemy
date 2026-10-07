.syntax unified
	.thumb
	.global MusicTrack_ClearModulationStateLeftover
	.thumb_func
MusicTrack_ClearModulationStateLeftover:
	movs	r2, #0
	strb	r2, [r1, #22]
	strb	r2, [r1, #26]
	ldrb	r2, [r1, #24]
	cmp	r2, #0
	bne.n	.L_081c03e8
	movs	r2, #12
	b.n	.L_081c03ea
.L_081c03e8:
	movs	r2, #3
.L_081c03ea:
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	lr
AlchemyUncredited_081c03f2:
	.2byte	0x0000
AlchemyUncreditedEnd_081c03f2:
