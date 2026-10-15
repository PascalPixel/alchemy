@ 合成ルーチンへ書き込まれる命令の雛形。関数ではない：r1 が読み出し元、
@ r9 が書き込み先、r5/r8/sl は作業用で、呼び出し規約を持たない。
.syntax unified
.text
	.arm
	.global BattleEffect_CopyReverse
	.global BattleEffect_CopyForward
	.global BattleEffect_Copy8Forward
	.global BattleEffect_Copy8Reverse
	.global BattleEffect_TransparentForward
	.global BattleEffect_TransparentReverse
	.global BattleEffect_Transparent8Forward
	.global BattleEffect_Transparent8Reverse
	.global BattleEffect_MaxForward
	.global BattleEffect_MaxReverse
	.global BattleEffect_Max4Forward
	.global BattleEffect_Max4Reverse
	.global BattleEffect_AdditiveForward
	.global BattleEffect_AdditiveReverse
	.global BattleEffect_Mask
BattleEffect_CopyReverse:
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
BattleEffect_CopyForward:
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
BattleEffect_TransparentForward:
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9]
	add	r9, r9, #1
BattleEffect_TransparentReverse:
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9]
	add	r9, r9, #1
BattleEffect_Transparent8Forward:
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
BattleEffect_Transparent8Reverse:
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
BattleEffect_MaxForward:
	ldrb	r5, [r1], #1
	ldrb	r8, [r9], #1
	cmp	r5, r8
	strbgt	r5, [r9, #-1]
BattleEffect_MaxReverse:
	ldrb	r5, [r1], #-1
	ldrb	r8, [r9], #1
	cmp	r5, r8
	strbgt	r5, [r9, #-1]
BattleEffect_Max4Forward:
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
BattleEffect_Max4Reverse:
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
BattleEffect_AdditiveForward:
	ldrb	r5, [r1], #1
	ldrb	r8, [r9], #1
	add	r5, r5, r8
	cmp	r5, #63
	movcs	r5, #63
	strb	r5, [r9, #-1]
BattleEffect_AdditiveReverse:
	ldrb	r5, [r1], #-1
	ldrb	r8, [r9], #1
	add	r5, r5, r8
	cmp	r5, #63
	movcs	r5, #63
	strb	r5, [r9, #-1]
BattleEffect_Mask:
	mov	sl, #64
	orr	sl, sl, sl, lsl #8
	orr	sl, sl, sl, lsl #16
