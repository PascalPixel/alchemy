@ Map-drawing kernel, copied and run as one 616-byte unit (size and source
@ {0x268, 0x08021070} in its loader's literal pool at 0x0802dd64). Behind a
@ 4-byte Thumb entry (bx pc) the ARM body is hand-unrolled: r4 packs two loop
@ counters and a 0x38 mask (subs #0x1000000; bpl, sub #0x10000; tst), lookups
@ use pre-indexed write-back, and r0 is pushed and popped around the inner
@ loop (stmfd sp!, {r0} ... ldmfd sp!, {r0}), where GCC's fixed ARM frame
@ would use a stack slot. It is TLA's own revision of TBS
@ GRAPHICS/TILE/DRAW_MAP.S (0x08009e7c): the first 32 bytes after the entry
@ match it.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: handwritten — relocated ARM map-draw kernel (TLA revision of TBS DRAW_MAP)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08021070
	.global Render_DrawMap
	.thumb_func
Render_DrawMap:
AlchemyUncredited_08021070:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08021070:
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
