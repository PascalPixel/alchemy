@ Instruction templates written into the compositing routine, not functions: r1
@ is the source, r9 the destination, and r5/r8/sl scratch; no calling convention.
.syntax unified
.text
	.arm
	.global BattleFx_CopyReverse
	.global BattleFx_CopyForward
	.global BattleEffect_Copy8Forward
	.global BattleEffect_Copy8Reverse
	.global BattleFx_TransparentForward
	.global BattleFx_TransparentReverse
	.global BattleFx_Transparent8Forward
	.global BattleFx_Transparent8Reverse
	.global BattleFx_MaxForward
	.global BattleFx_MaxReverse
	.global BattleFx_Max4Forward
	.global BattleFx_Max4Reverse
	.global BattleFx_AdditiveForward
	.global BattleFx_AdditiveReverse
	.global BattleFx_Mask
BattleFx_CopyReverse:
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
BattleFx_CopyForward:
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
BattleEffect_Copy8Forward:
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
BattleEffect_Copy8Reverse:
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
BattleFx_TransparentForward:
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9]
	add	r9, r9, #1
BattleFx_TransparentReverse:
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9]
	add	r9, r9, #1
BattleFx_Transparent8Forward:
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
BattleFx_Transparent8Reverse:
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
BattleFx_MaxForward:
	ldrb	r5, [r1], #1
	ldrb	r8, [r9], #1
	cmp	r5, r8
	strbgt	r5, [r9, #-1]
BattleFx_MaxReverse:
	ldrb	r5, [r1], #-1
	ldrb	r8, [r9], #1
	cmp	r5, r8
	strbgt	r5, [r9, #-1]
BattleFx_Max4Forward:
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
BattleFx_Max4Reverse:
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
BattleFx_AdditiveForward:
	ldrb	r5, [r1], #1
	ldrb	r8, [r9], #1
	add	r5, r5, r8
	cmp	r5, #63
	movcs	r5, #63
	strb	r5, [r9, #-1]
BattleFx_AdditiveReverse:
	ldrb	r5, [r1], #-1
	ldrb	r8, [r9], #1
	add	r5, r5, r8
	cmp	r5, #63
	movcs	r5, #63
	strb	r5, [r9, #-1]
BattleFx_Mask:
	mov	sl, #64
	orr	sl, sl, sl, lsl #8
	orr	sl, sl, sl, lsl #16
