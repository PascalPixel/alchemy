@ The colour fragments btEffectBuildBlitter (battle_effect_blitter.c) builds canvas
@ blitters from, as it does the others in blitter_fragments.s: each
@ adds picture pixels (r1) to the canvas (r9) with sl as the cap mask, forward
@ or reverse.
@
@ Why assembly:
@ btEffectBuildBlitter copies these 14-word fragments into generated
@ ARM functions. r1/r5/r8/r9 are shared state and sl is the mask; r5/r8/r9
@ change without callee saves, and neither fragment has a return.
.syntax unified
.text
	.arm
	.global BattleEffectColorForward
	.global BattleEffectColorReverse
BattleEffectColorForward:
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
BattleEffectColorReverse:
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
