.syntax unified
	.thumb
	.global MusicTrack_SetLfoSpeedFromCommand
	.thumb_func
MusicTrack_SetLfoSpeedFromCommand:
	mov	ip, lr
	bl	MusicTrack_ReadCommandByte
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_081c1e80
	bl	Func_081c1e48
.L_081c1e80:
	bx	ip
AlchemyUncredited_081c1e82:
	.2byte	0x0000
	.global MusicTrack_SetModulationFromCommand
	.thumb_func
MusicTrack_SetModulationFromCommand:
AlchemyUncreditedEnd_081c1e82:
	mov	ip, lr
	bl	MusicTrack_ReadCommandByte
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L_081c1e94
	bl	Func_081c1e48
.L_081c1e94:
	bx	ip
AlchemyUncredited_081c1e96:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1e96:
