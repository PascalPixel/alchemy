@ Why assembly: r11/fp is a tile-data address (add fp, r3, r7, lsr #26), not a
@ frame pointer. r4 (0xffa00038) is at once two packed loop counters and an
@ AND mask. subs r4, r4, #0x1000000 / bpl counts the inner loop in its top
@ byte, reset by add r4, r4, #0x1e000000; sub r4, r4, #0x10000 with
@ tst r4, #0x70000 and tst r4, #0xff0000 / bne counts the rows in bits
@ 16-23; and every and sl, r4, r8, lsr #20 uses the same register as the
@ mask, of which only the low 0x38 can meet the 12 shifted bits. A compiler
@ keeps two counters and a constant mask as separate values.
@ Its Thumb entry's bx pc switches to the ARM code that follows, a mode
@ switch inside a function; the halfword after bx pc is never run.
.syntax unified
	.thumb
	.global Render_DrawMapCode
Render_DrawMapCode:
	.global Render_DrawMap
	.thumb_func
Render_DrawMap:
	bx	pc
AlchemyUncredited_08021072:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08021072:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	mov	r4, #0xa00000
	orr	r4, r4, #56
	orr	r4, r4, #0xff000000
.L_08021084:
	ldrsh	r6, [r0, #4]
	ldr	r7, [r0, #8]
	ldr	r8, [r0, #12]
	ldrsh	r5, [r0], #32
	stmfd	sp!, {r0}
	mov	r0, #0x1000
	lsl	r7, r7, #15
	lsl	r8, r8, #15
	add	r4, r4, #0x1e000000
.L_080210a8:
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	ip, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	ip, #0
	ldrbne	ip, [sl, ip, lsl #6]
	cmp	ip, #0
	ldrbeq	ip, [fp, -r0]
	subeq	sl, sl, #0x4000
	ldrbeq	ip, [sl, ip, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	r9, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	r9, #0
	ldrbne	r9, [sl, r9, lsl #6]
	cmp	r9, #0
	ldrbeq	r9, [fp, -r0]
	subeq	sl, sl, #0x4000
	ldrbeq	r9, [sl, r9, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	orr	r9, ip, r9, lsl #8
	strh	r9, [r1], #2
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	ip, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	ip, #0
	ldrbne	ip, [sl, ip, lsl #6]
	cmp	ip, #0
	ldrbeq	ip, [fp, -r0]
	subeq	sl, sl, #0x4000
	ldrbeq	ip, [sl, ip, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	r9, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	r9, #0
	ldrbne	r9, [sl, r9, lsl #6]
	cmp	r9, #0
	ldrbeq	r9, [fp, -r0]
	subeq	sl, sl, #0x4000
	ldrbeq	r9, [sl, r9, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	orr	r9, ip, r9, lsl #8
	strh	r9, [r1], #2
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	ip, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	ip, #0
	ldrbne	ip, [sl, ip, lsl #6]
	cmp	ip, #0
	ldrbeq	ip, [fp, -r0]
	subeq	sl, sl, #0x4000
	ldrbeq	ip, [sl, ip, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	r9, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	r9, #0
	ldrbne	r9, [sl, r9, lsl #6]
	cmp	r9, #0
	ldrbeq	r9, [fp, -r0]
	subeq	sl, sl, #0x4000
	ldrbeq	r9, [sl, r9, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	orr	r9, ip, r9, lsl #8
	strh	r9, [r1], #2
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	ip, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	ip, #0
	ldrbne	ip, [sl, ip, lsl #6]
	cmp	ip, #0
	ldrbeq	ip, [fp, -r0]
	subeq	sl, sl, #0x4000
	ldrbeq	ip, [sl, ip, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	r9, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	r9, #0
	ldrbne	r9, [sl, r9, lsl #6]
	cmp	r9, #0
	ldrbeq	r9, [fp, -r0]
	subeq	sl, sl, #0x4000
	ldrbeq	r9, [sl, r9, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	orr	r9, ip, r9, lsl #8
	strh	r9, [r1], #58
	subs	r4, r4, #0x1000000
	bpl	.L_080210a8
	sub	r1, r1, #0x780
	add	r1, r1, #8
	sub	r4, r4, #0x10000
	tst	r4, #0x70000
	addeq	r1, r1, #0x7c0
	ldmfd	sp!, {r0}
	tst	r4, #0xff0000
	bne	.L_08021084
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
	.global Render_DrawMapCodeSize
	.set Render_DrawMapCodeSize, . - Render_DrawMapCode
