@ Colour-buffer kernels that fade a buffer while saving a backup copy: each
@ stores every original block to the backup buffer (stmia r1! or r2!) before
@ writing it scaled by 3/4, halved or darkened. Each is ARM code behind a
@ 4-byte Thumb entry (bx pc) that its loader in COLOR_FADE_LOADERS.S copies
@ to the stack, and each moves eight data words per ldm/stm pair, where
@ GCC 2.96's block moves take at most four; the scaling and halving kernels
@ also save r5-sl without lr, where GCC saves lr with any register. They are
@ TLA's own revisions of TBS GRAPHICS/PALETTE/COLOR_BACKUP_KERNELS.S
@ (0x08001ea8), sharing only runs of 16 and 32 bytes with it.
@ Its three 4-byte Thumb entries, which are TLA's own, carry no credit.
@ credit: handwritten — ARM colour-buffer fade kernels with backup, copied to the stack
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08001a14
	.global ColorBuffer_ScaleThreeQuartersKernel
	.thumb_func
ColorBuffer_ScaleThreeQuartersKernel:
.L_08001a14:
AlchemyUncredited_08001a14:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001a14:
	push	{r5, r6, r7, r8, r9, sl}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08001a2c:
	ldm	r0, {r3, r4, r5, r6, r7, r8, r9, sl}
	stmia	r1!, {r3, r4, r5, r6, r7, r8, r9, sl}
	adds	r3, r3, r3, lsl #1
	andne	r3, ip, r3, lsr #2
	adds	r4, r4, r4, lsl #1
	andne	r4, ip, r4, lsr #2
	adds	r5, r5, r5, lsl #1
	andne	r5, ip, r5, lsr #2
	adds	r6, r6, r6, lsl #1
	andne	r6, ip, r6, lsr #2
	adds	r7, r7, r7, lsl #1
	andne	r7, ip, r7, lsr #2
	adds	r8, r8, r8, lsl #1
	andne	r8, ip, r8, lsr #2
	adds	r9, r9, r9, lsl #1
	andne	r9, ip, r9, lsr #2
	adds	sl, sl, sl, lsl #1
	andne	sl, ip, sl, lsr #2
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, sl}
	ldm	r0, {r3, r4, r5, r6, r7, r8, r9, sl}
	stmia	r1!, {r3, r4, r5, r6, r7, r8, r9, sl}
	adds	r3, r3, r3, lsl #1
	andne	r3, ip, r3, lsr #2
	adds	r4, r4, r4, lsl #1
	andne	r4, ip, r4, lsr #2
	adds	r5, r5, r5, lsl #1
	andne	r5, ip, r5, lsr #2
	adds	r6, r6, r6, lsl #1
	andne	r6, ip, r6, lsr #2
	adds	r7, r7, r7, lsl #1
	andne	r7, ip, r7, lsr #2
	adds	r8, r8, r8, lsl #1
	andne	r8, ip, r8, lsr #2
	adds	r9, r9, r9, lsl #1
	andne	r9, ip, r9, lsr #2
	adds	sl, sl, sl, lsl #1
	andne	sl, ip, sl, lsr #2
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, sl}
	cmp	r0, r2
	bne	.L_08001a2c
	pop	{r5, r6, r7, r8, r9, sl}
	bx	lr
	.thumb
	.global ColorBuffer_HalveKernel
	.thumb_func
ColorBuffer_HalveKernel:
.L_08001ad4:
AlchemyUncredited_08001ad4:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001ad4:
	push	{r5, r6, r7, r8, r9, sl}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08001aec:
	ldm	r0, {r3, r4, r5, r6, r7, r8, r9, sl}
	stmia	r1!, {r3, r4, r5, r6, r7, r8, r9, sl}
	and	r3, ip, r3, lsr #1
	and	r4, ip, r4, lsr #1
	and	r5, ip, r5, lsr #1
	and	r6, ip, r6, lsr #1
	and	r7, ip, r7, lsr #1
	and	r8, ip, r8, lsr #1
	and	r9, ip, r9, lsr #1
	and	sl, ip, sl, lsr #1
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, sl}
	ldm	r0, {r3, r4, r5, r6, r7, r8, r9, sl}
	stmia	r1!, {r3, r4, r5, r6, r7, r8, r9, sl}
	and	r3, ip, r3, lsr #1
	and	r4, ip, r4, lsr #1
	and	r5, ip, r5, lsr #1
	and	r6, ip, r6, lsr #1
	and	r7, ip, r7, lsr #1
	and	r8, ip, r8, lsr #1
	and	r9, ip, r9, lsr #1
	and	sl, ip, sl, lsr #1
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, sl}
	cmp	r0, r2
	bne	.L_08001aec
	pop	{r5, r6, r7, r8, r9, sl}
	bx	lr
	.thumb
	.global ColorBuffer_DarkenKernel
	.thumb_func
ColorBuffer_DarkenKernel:
.L_08001b54:
AlchemyUncredited_08001b54:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001b54:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	ip, [pc, #160]
	rsb	r1, r1, ip, lsl #7
	add	r3, r0, r3
.L_08001b68:
	ldm	r0, {r4, r5, r6, r7, r8, r9, sl, fp}
	stmia	r2!, {r4, r5, r6, r7, r8, r9, sl, fp}
	add	r4, r4, r1
	and	lr, ip, r4, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	r4, r4, lr
	add	r5, r5, r1
	and	lr, ip, r5, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	r5, r5, lr
	add	r6, r6, r1
	and	lr, ip, r6, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	r6, r6, lr
	add	r7, r7, r1
	and	lr, ip, r7, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	r7, r7, lr
	add	r8, r8, r1
	and	lr, ip, r8, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	r8, r8, lr
	add	r9, r9, r1
	and	lr, ip, r9, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	r9, r9, lr
	add	sl, sl, r1
	and	lr, ip, sl, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	sl, sl, lr
	add	fp, fp, r1
	and	lr, ip, fp, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	fp, fp, lr
	stmia	r0!, {r4, r5, r6, r7, r8, r9, sl, fp}
	cmp	r0, r3
	bne	.L_08001b68
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
	.4byte	0x01010101
