.syntax unified
	.thumb

	.global MusicTrack_SetLfoSpeedFromCommand
	.thumb_func
MusicTrack_SetLfoSpeedFromCommand:
	mov	ip, lr
	bl	MusicTrack_ReadCommandByte
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_080fa1d4_0
	bl	MusicTrack_ClearModulationState
.L_080fa1d4_0:
	bx	ip
AlchemyUncredited_080fa1e6:
	.balign 4, 0

AlchemyUncreditedEnd_080fa1e6:
	.global MusicTrack_SetModulationFromCommand
	.thumb_func
MusicTrack_SetModulationFromCommand:
	mov	ip, lr
	bl	MusicTrack_ReadCommandByte
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L_080fa1e8_0
	bl	MusicTrack_ClearModulationState
.L_080fa1e8_0:
	bx	ip
AlchemyUncredited_080fa1fa:
	.2byte 0
AlchemyUncreditedEnd_080fa1fa:
