.syntax unified
	.thumb
	.global MusicTrack_ClearModulationState
	.global Func_080fa1ac
	.thumb_func
MusicTrack_ClearModulationState:
Func_080fa1ac:
	movs	r2, #0
	strb	r2, [r1, #22]
	strb	r2, [r1, #26]
	ldrb	r2, [r1, #24]
	cmp	r2, #0
	bne.n	.L_080fa1bc
	movs	r2, #12
	b.n	.L_080fa1be
.L_080fa1bc:
	movs	r2, #3
.L_080fa1be:
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	lr
	.2byte 0x0000
