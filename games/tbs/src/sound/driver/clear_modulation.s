@ C2: MusicPlayer2000 library assembly (public pret m4a_1.s: clear_modM).
@ Credited on library provenance, not on a handwritten-instruction claim:
@ the routine is ordinary Thumb, and it is the only one in this file.
.syntax unified
	.thumb
	.global MusicTrack_ClearModulationState
	.thumb_func
MusicTrack_ClearModulationState:
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
AlchemyUncredited_080fa1c6:
	.2byte	0x0000
AlchemyUncreditedEnd_080fa1c6:
