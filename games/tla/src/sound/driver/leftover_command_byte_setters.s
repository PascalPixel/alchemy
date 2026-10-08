.syntax unified
	.thumb
	.global MusicTrack_SetLfoSpeedFromCommandLeftover
	.thumb_func
MusicTrack_SetLfoSpeedFromCommandLeftover:
	mov	ip, lr
	bl	MusicTrack_ReadCommandByteLeftover
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_081c0410
	bl	MusicTrack_ClearModulationStateLeftover
.L_081c0410:
	bx	ip
AlchemyUncredited_081c0412:
	.2byte	0x0000
	.global MusicTrack_SetModulationFromCommandLeftover
	.thumb_func
MusicTrack_SetModulationFromCommandLeftover:
AlchemyUncreditedEnd_081c0412:
	mov	ip, lr
	bl	MusicTrack_ReadCommandByteLeftover
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L_081c0424
	bl	MusicTrack_ClearModulationStateLeftover
.L_081c0424:
	bx	ip
AlchemyUncredited_081c0426:
	.2byte	0x0000
AlchemyUncreditedEnd_081c0426:
