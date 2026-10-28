@ The pieces BattleEffect_BuildBlitter (battle_effect/blitter.c) builds canvas blitters
@ from.
@ None of this code runs where it is: the builder copies its words into a
@ heap block, one routine per canvas size, mirroring, clip and mode.
@
@ Each pixel fragment combines picture pixels, read through r1, into the
@ canvas, written through r9: Copy writes them, Transparent skips zeros, Max
@ keeps the brighter of the two, Additive adds them a pixel at a time, capped
@ at 63. Forward fragments read the picture left to right and Reverse ones
@ right to left; canvas rows stay in tile order. The 8 variants do eight
@ pixels a pass. For whole cells the additive mode uses the colour fragments
@ (blitter_color_fragments.s), which add four pixels a word at a time:
@ BattleEffect_Mask puts 0x40 in every byte of sl, and a byte that reaches it
@ becomes 63. gBlitterTemplate is the template the builder walks: each
@ word it rewrites is marked, and battle_effect/blitter.c describes its blocks.
@
@ Why assembly:
@ r11 (fp) as an ordinary register: it holds a row counter
@ (rsb fp, ip, #16; mov lr, fp), not a frame.
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
	.global gBlitterTemplate
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
gBlitterTemplate:
	@ Block 1: prologue (see battle_effect/blitter.c)
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	add	r4, sp, #32
	ldm	r4, {r4, r5}
	add	r6, r1, r4	@ [3] slot: written when mirrored across only
	sub	r1, r6, #1	@ [4] slot: written when mirrored across only
	lsl	r6, r4, #1	@ [5] slot: written when mirrored across only
	sub	r6, r5, #1	@ [6] slot: written when mirrored down only
	mla	r1, r6, r4, r1	@ [7] slot: written when mirrored down only
	rsb	r6, r4, #0	@ [8] slot: written when mirrored down only
	lsl	r6, r6, #1	@ [9] slot: written when mirrored down only
	mla	r1, r4, r5, r1	@ [10] slot: written when mirrored both ways
	sub	r1, r1, #1	@ [11] slot: written when mirrored both ways
	mov	r6, #0	@ [12] slot: written when mirrored both ways
	mov	r6, #0	@ [13] slot: written when not mirrored
	@ Block 2: vertical clip (see battle_effect/blitter.c)
	rsbs	ip, r3, #0	@ [14] slot: vertical clip only
	addpl	r5, r5, r3	@ [15] slot: vertical clip only
	mlapl	r1, r3, r4, r1	@ [16] slot: vertical clip, mirrored down
	mlapl	r1, ip, r4, r1	@ [17] slot: vertical clip, not mirrored down
	movpl	r3, #0	@ [18] slot: vertical clip only
	add	ip, r3, r5	@ [19] slot: vertical clip only
	cmp	ip, #0	@ [20] slot: vertical clip; patched: + cell height
	rsbgt	r5, r3, #0	@ [21] slot: vertical clip; patched: + cell height
	@ Block 3: columns (see battle_effect/blitter.c)
	cmp	r5, #0
	ble .Lclip_x	@ [23] patched: branch to the exit when no rows remain
	and	r2, r2, #0	@ [24] slot: unclipped only; patched: + width mask
.Lclip_x:
	rsbs	ip, r2, #0	@ [25] slot: horizontal clip only
	addpl	r4, r4, r2	@ [26] slot: horizontal clip only
	addpl	r6, r6, r2	@ [27] slot: horizontal clip, mirrored across
	addpl	r1, r1, r2	@ [28] slot: horizontal clip, mirrored across
	subpl	r6, r6, r2	@ [29] slot: horizontal clip, not mirrored
	subpl	r1, r1, r2	@ [30] slot: horizontal clip, not mirrored
	movpl	r2, #0	@ [31] slot: horizontal clip only
	add	ip, r2, r4	@ [32] slot: horizontal clip only
	subs	ip, ip, #0	@ [33] slot: horizontal clip; patched: + width bit operand
	subgt	r6, r6, ip	@ [34] slot: horizontal clip, mirrored across
	addgt	r6, r6, ip	@ [35] slot: horizontal clip, not mirrored
	rsbgt	r4, r2, #0	@ [36] slot: horizontal clip; patched: + width bit operand
	cmp	r4, #0
	ble .Llead_count	@ [38] patched: branch to the exit when no columns remain
	ands	r7, r2, #7
.Llead_count:
	beq .Lrow_setup
	rsb	r7, r7, #8
	subs	r4, r4, r7
	addlt	r7, r4, r7
	movlt	r4, #0
.Lrow_setup:
	lsr	ip, r2, #3
	rsb	fp, ip, #16	@ [46] slot: unclipped only
	and	r2, r2, #7
	add	r2, r2, ip, lsl #6
	add	r0, r0, r2
	lsl	r3, r3, #3
	add	r2, r3, r5, lsl #3
	@ Block 4: row loop (see battle_effect/blitter.c)
	and	ip, r3, #56
	and	lr, r3, #0, 26	@ [53] patched: + rows per cell, less one
	orr	ip, ip, lr	@ [54] patched: + width shift
	add	r9, r0, ip
	mov	lr, fp	@ [56] slot: unclipped only
	cmp	r7, #0
	beq .Llead	@ [58] patched: branch past an empty run
	sub	ip, r1, r7	@ [59] slot: mirrored across
.Llead:
	add	ip, r1, r7	@ [60] slot: not mirrored
	ldrb	r5, [r1], #-1	@ [61] slot: replaced by the mode's one-pixel fragment
	ldrb	r5, [r1], #1	@ [62] slot: replaced by the mode's one-pixel fragment
	cmp	r5, #0	@ [63] slot: replaced by the mode's one-pixel fragment
	strb	r5, [r9], #1	@ [64] slot: replaced by the mode's one-pixel fragment
	cmp	r1, ip
	bne .Lblocks	@ [66] patched: branch back to the partial run
	add	r9, r9, #56
.Lblocks:
	subs	lr, lr, #1	@ [68] slot: unclipped only
	subeq	r9, r9, #1024	@ [69] slot: unclipped only
	lsrs	ip, r4, #3
	beq .Lblock	@ [71] patched: branch past an empty run
	ldrb	r5, [r1], #-1	@ [72] slot: replaced by the mode's 8-pixel fragment
.Lblock:
	ldrb	r5, [r1], #1	@ [73] slot: replaced by the mode's 8-pixel fragment
	cmp	r5, #0	@ [74] slot: replaced by the mode's 8-pixel fragment
	strb	r5, [r9], #1	@ [75] slot: replaced by the mode's 8-pixel fragment
	add	r9, r9, #56
	subs	lr, lr, #1	@ [77] slot: unclipped only
	subeq	r9, r9, #1024	@ [78] slot: unclipped only
	subs	ip, ip, #1
	bne .Ltail_count	@ [80] patched: branch back to the full-cell loop
	ands	ip, r4, #7
.Ltail_count:
	beq .Ltail	@ [82] patched: branch past an empty run
	sub	ip, r1, ip	@ [83] slot: mirrored across
.Ltail:
	add	ip, r1, ip	@ [84] slot: not mirrored
	ldrb	r5, [r1], #-1	@ [85] slot: replaced by the mode's one-pixel fragment
	ldrb	r5, [r1], #1	@ [86] slot: replaced by the mode's one-pixel fragment
	cmp	r5, #0	@ [87] slot: replaced by the mode's one-pixel fragment
	strb	r5, [r9], #1	@ [88] slot: replaced by the mode's one-pixel fragment
	cmp	r1, ip
	bne .Lrow_end	@ [90] patched: branch back to the partial run
	add	r1, r1, r6
.Lrow_end:
	add	r3, r3, #8
	cmp	r3, r2
	bne .Lreturn	@ [94] patched: branch back to the row loop
	@ Block 5: exit (see battle_effect/blitter.c)
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
.Lreturn:
	bx	lr
