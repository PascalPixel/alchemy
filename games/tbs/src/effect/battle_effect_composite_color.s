@ C2: colburst.c copies these 14-word compositor fragments into generated
@ ARM routines. r1/r5/r8/r9 are shared state and sl is the mask; r5/r8/r9
@ change without callee saves, and neither fragment has a return.
.syntax unified
.text
	.arm
	.global BattleEffect_ColorForward
	.global BattleEffect_ColorReverse
BattleEffect_ColorForward:
	ldr	r8, [r9]
	ldrb	r5, [r1], #1
	add	r8, r8, r5
	ldrb	r5, [r1], #1
	add	r8, r8, r5, lsl #8
	ldrb	r5, [r1], #1
	add	r8, r8, r5, lsl #16
	ldrb	r5, [r1], #1
	add	r8, r8, r5, lsl #24
	and	r5, r8, sl
	bic	r8, r8, sl
	sub	r5, r5, r5, lsr #6
	orr	r8, r8, r5
	str	r8, [r9], #4
BattleEffect_ColorReverse:
	ldr	r8, [r9]
	ldrb	r5, [r1], #-1
	add	r8, r8, r5
	ldrb	r5, [r1], #-1
	add	r8, r8, r5, lsl #8
	ldrb	r5, [r1], #-1
	add	r8, r8, r5, lsl #16
	ldrb	r5, [r1], #-1
	add	r8, r8, r5, lsl #24
	and	r5, r8, sl
	bic	r8, r8, sl
	sub	r5, r5, r5, lsr #6
	orr	r8, r8, r5
	str	r8, [r9], #4
