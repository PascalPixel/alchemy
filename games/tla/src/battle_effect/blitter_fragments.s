@ Instruction placements written into the compositing function; they are not
@ functions themselves.
@
@ Why assembly:
@ btEffectBuildBlitter (summon_effects/moloch.c) copies these fragments into
@ generated ARM functions. r1 is the source, r9 the destination and r5/r8/sl
@ scratch, changed without callee saves, and no fragment has a return: a
@ linkage no procedure-call standard produces.
.syntax unified
.text
	.arm
	.global BattleEffectCopyReverse
	.global BattleEffectCopyForward
	.global BattleEffectCopy8Forward
	.global BattleEffectCopy8Reverse
	.global BattleEffectTransparentForward
	.global BattleEffectTransparentReverse
	.global BattleEffectTransparent8Forward
	.global BattleEffectTransparent8Reverse
	.global BattleEffectMaxForward
	.global BattleEffectMaxReverse
	.global BattleEffectMax4Forward
	.global BattleEffectMax4Reverse
	.global BattleEffectAdditiveForward
	.global BattleEffectAdditiveReverse
	.global BattleEffectMask
BattleEffectCopyReverse:
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
BattleEffectCopyForward:
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
BattleEffectCopy8Forward:
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
BattleEffectCopy8Reverse:
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
BattleEffectTransparentForward:
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9]
	add	r9, r9, #1
BattleEffectTransparentReverse:
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9]
	add	r9, r9, #1
BattleEffectTransparent8Forward:
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #1]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #2]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #3]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #4]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #5]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #6]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #7]
	add	r9, r9, #8
BattleEffectTransparent8Reverse:
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #1]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #2]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #3]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #4]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #5]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #6]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #7]
	add	r9, r9, #8
BattleEffectMaxForward:
	ldrb	r5, [r1], #1
	ldrb	r8, [r9], #1
	cmp	r5, r8
	strbgt	r5, [r9, #-1]
BattleEffectMaxReverse:
	ldrb	r5, [r1], #-1
	ldrb	r8, [r9], #1
	cmp	r5, r8
	strbgt	r5, [r9, #-1]
BattleEffectMax4Forward:
	ldrb	r5, [r1], #1
	ldr	r8, [r9], #4
	and	sl, r8, #255
	cmp	r5, sl
	strbgt	r5, [r9, #-4]
	ldrb	r5, [r1], #1
	and	sl, r8, #65280
	cmp	r5, sl, lsr #8
	strbgt	r5, [r9, #-3]
	ldrb	r5, [r1], #1
	and	sl, r8, #16711680
	cmp	r5, sl, lsr #16
	strbgt	r5, [r9, #-2]
	ldrb	r5, [r1], #1
	cmp	r5, r8, lsr #24
	strbgt	r5, [r9, #-1]
BattleEffectMax4Reverse:
	ldrb	r5, [r1], #-1
	ldr	r8, [r9], #4
	and	sl, r8, #255
	cmp	r5, sl
	strbgt	r5, [r9, #-4]
	ldrb	r5, [r1], #-1
	and	sl, r8, #65280
	cmp	r5, sl, lsr #8
	strbgt	r5, [r9, #-3]
	ldrb	r5, [r1], #-1
	and	sl, r8, #16711680
	cmp	r5, sl, lsr #16
	strbgt	r5, [r9, #-2]
	ldrb	r5, [r1], #-1
	cmp	r5, r8, lsr #24
	strbgt	r5, [r9, #-1]
BattleEffectAdditiveForward:
	ldrb	r5, [r1], #1
	ldrb	r8, [r9], #1
	add	r5, r5, r8
	cmp	r5, #63
	movcs	r5, #63
	strb	r5, [r9, #-1]
BattleEffectAdditiveReverse:
	ldrb	r5, [r1], #-1
	ldrb	r8, [r9], #1
	add	r5, r5, r8
	cmp	r5, #63
	movcs	r5, #63
	strb	r5, [r9, #-1]
BattleEffectMask:
	mov	sl, #64
	orr	sl, sl, sl, lsl #8
	orr	sl, sl, sl, lsl #16
