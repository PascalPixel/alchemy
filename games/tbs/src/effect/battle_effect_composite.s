@ The pieces BattleFx_BuildBlitter (effect/blitter.c) builds canvas blitters from.
@ None of this code runs where it is: the builder copies its words into a
@ heap block, one routine per cell size, mirroring, clip and mode.
@
@ Each pixel fragment combines picture pixels, read through r1, into the
@ canvas, written through r9: Copy writes them, Transparent skips zeros, Max
@ keeps the brighter of the two, Additive adds them, capped at 63. Forward
@ fragments read the picture left to right and Reverse ones right to left;
@ canvas rows stay in tile order. The 8 variants do eight pixels a pass. The
@ additive fragments add four pixels a word at a time: BattleEffect_Mask puts
@ 0x40 in every byte of sl, and a byte that reaches it becomes 63.
@ BattleEffect_Composite is the template the builder walks; blitter.c
@ describes its blocks. The colour fragments are in
@ battle_effect_composite_color.s.
@
@ Why assembly:
@ C2: r11/fp holds a row counter (rsb fp, ip, #16; mov lr, fp), not a frame.
@ The preceding entries also share unsaved r5/r9 state with the main body.
.syntax unified
.text
	.arm
	.global BattleEffect_CopyReverse
	.global BattleEffect_CopyForward
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
	.global BattleEffect_Composite
BattleEffect_CopyReverse:
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
BattleEffect_CopyForward:
	ldrb	r5, [r1], #1
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
BattleEffect_Composite:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	add	r4, sp, #32
	ldm	r4, {r4, r5}
	add	r6, r1, r4
	sub	r1, r6, #1
	lsl	r6, r4, #1
	sub	r6, r5, #1
	mla	r1, r6, r4, r1
	rsb	r6, r4, #0
	lsl	r6, r6, #1
	mla	r1, r4, r5, r1
	sub	r1, r1, #1
	mov	r6, #0
	mov	r6, #0
	rsbs	ip, r3, #0
	addpl	r5, r5, r3
	mlapl	r1, r3, r4, r1
	mlapl	r1, ip, r4, r1
	movpl	r3, #0
	add	ip, r3, r5
	cmp	ip, #0
	rsbgt	r5, r3, #0
	cmp	r5, #0
	ble .Lclip_x
	and	r2, r2, #0
.Lclip_x:
	rsbs	ip, r2, #0
	addpl	r4, r4, r2
	addpl	r6, r6, r2
	addpl	r1, r1, r2
	subpl	r6, r6, r2
	subpl	r1, r1, r2
	movpl	r2, #0
	add	ip, r2, r4
	subs	ip, ip, #0
	subgt	r6, r6, ip
	addgt	r6, r6, ip
	rsbgt	r4, r2, #0
	cmp	r4, #0
	ble .Llead_count
	ands	r7, r2, #7
.Llead_count:
	beq .Lrow_setup
	rsb	r7, r7, #8
	subs	r4, r4, r7
	addlt	r7, r4, r7
	movlt	r4, #0
.Lrow_setup:
	lsr	ip, r2, #3
	rsb	fp, ip, #16
	and	r2, r2, #7
	add	r2, r2, ip, lsl #6
	add	r0, r0, r2
	lsl	r3, r3, #3
	add	r2, r3, r5, lsl #3
	and	ip, r3, #56
	and	lr, r3, #0, 26
	orr	ip, ip, lr
	add	r9, r0, ip
	mov	lr, fp
	cmp	r7, #0
	beq .Llead
	sub	ip, r1, r7
.Llead:
	add	ip, r1, r7
	ldrb	r5, [r1], #-1
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strb	r5, [r9], #1
	cmp	r1, ip
	bne .Lblocks
	add	r9, r9, #56
.Lblocks:
	subs	lr, lr, #1
	subeq	r9, r9, #1024
	lsrs	ip, r4, #3
	beq .Lblock
	ldrb	r5, [r1], #-1
.Lblock:
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strb	r5, [r9], #1
	add	r9, r9, #56
	subs	lr, lr, #1
	subeq	r9, r9, #1024
	subs	ip, ip, #1
	bne .Ltail_count
	ands	ip, r4, #7
.Ltail_count:
	beq .Ltail
	sub	ip, r1, ip
.Ltail:
	add	ip, r1, ip
	ldrb	r5, [r1], #-1
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strb	r5, [r9], #1
	cmp	r1, ip
	bne .Lrow_end
	add	r1, r1, r6
.Lrow_end:
	add	r3, r3, #8
	cmp	r3, r2
	bne .Lreturn
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
.Lreturn:
	bx	lr
