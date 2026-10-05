.syntax unified
	.thumb

.global Func_080fa1d4
	.thumb_func
Func_080fa1d4:
	.global MusicTrack_SetLfoSpeedFromCommand
	.thumb_func
MusicTrack_SetLfoSpeedFromCommand:
	mov	ip, lr
	bl	Func_080fa1c8
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_080fa1d4_0
	bl	Func_080fa1ac
.L_080fa1d4_0:
	bx	ip
AlchemyUncredited_080fa1e6:
	.balign 4, 0

.global Func_080fa1e8
	.thumb_func
Func_080fa1e8:
AlchemyUncreditedEnd_080fa1e6:
	.global MusicTrack_SetModulationFromCommand
	.thumb_func
MusicTrack_SetModulationFromCommand:
	mov	ip, lr
	bl	Func_080fa1c8
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L_080fa1e8_0
	bl	Func_080fa1ac
.L_080fa1e8_0:
	bx	ip
AlchemyUncredited_080fa1fa:
	.2byte 0
AlchemyUncreditedEnd_080fa1fa:
