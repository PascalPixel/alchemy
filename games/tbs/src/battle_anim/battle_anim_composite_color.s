@ The colour fragments BattleAnim_BuildBlitter (parts/blitter.c) builds canvas
@ blitters from, as it does the others in battle_anim_composite.s: each
@ adds picture pixels (r1) to the canvas (r9) with sl as the cap mask, forward
@ or reverse.
@
@ Why assembly:
@ BattleAnim_BuildBlitter copies these 14-word fragments into generated
@ ARM routines. r1/r5/r8/r9 are shared state and sl is the mask; r5/r8/r9
@ change without callee saves, and neither fragment has a return.
.syntax unified
.text
	.arm
	.global BattleAnim_ColorForward
	.global BattleAnim_ColorReverse
BattleAnim_ColorForward:
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
BattleAnim_ColorReverse:
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
