@ Five colour-buffer kernels: saturating brighten, in-place 3/4 scaling and
@ halving of non-zero colours, and partial darken and brighten that skip
@ unchanged words. Each loader (0x0800543c, 0x0800562c, 0x0800567c, 0x080055d8,
@ 0x08005584) copies exactly its kernel onto the stack and calls it there.
@ They are ARM-state code built from per-lane masks and unrolled conditional
@ stores; four save r5-r6 or r5-r9 without lr and the partial darken pops its
@ return address into ip for bx ip, which GCC never does. Every TBS edition has
@ these 736 bytes, and every TLA edition has all five bodies byte for byte, each
@ behind a 4-byte Thumb entry, with brighten linked apart from the other four:
@ TLA GRAPHICS/COLOR_BRIGHTEN_KERNEL.S (0x08001c08) and COLOR_KERNELS.S
@ (0x08001d10).
@ credit: library — TBS/TLA common ARM colour-buffer kernels copied to the stack
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08001fb8
	.global ColorBuffer_BrightenKernel
ColorBuffer_BrightenKernel:
	push	{r5, r6, r7, r8, r9}
	mov	r8, #64
	orr	r8, r8, r8, lsl #8
	orr	r8, r8, r8, lsl #16
	sub	ip, r8, r8, lsr #6
	add	r3, r0, r3
.L_08001fd0:
	ldm	r0, {r4, r5, r6, r7}
	stmia	r2!, {r4, r5, r6, r7}
	add	r4, r4, r1
	and	r9, r4, r8
	bic	r4, r4, r9
	sub	r9, r9, r9, lsr #6
	orr	r4, r4, r9
	add	r5, r5, r1
	and	r9, r5, r8
	bic	r5, r5, r9
	sub	r9, r9, r9, lsr #6
	orr	r5, r5, r9
	add	r6, r6, r1
	and	r9, r6, r8
	bic	r6, r6, r9
	sub	r9, r9, r9, lsr #6
	orr	r6, r6, r9
	add	r7, r7, r1
	and	r9, r7, r8
	bic	r7, r7, r9
	sub	r9, r9, r9, lsr #6
	orr	r7, r7, r9
	stmia	r0!, {r4, r5, r6, r7}
	cmp	r0, r3
	bne	.L_08001fd0
	pop	{r5, r6, r7, r8, r9}
	bx	lr
	.global ColorBuffer_ScaleNonzeroThreeQuartersKernel
ColorBuffer_ScaleNonzeroThreeQuartersKernel:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08002050:
	ldm	r0!, {r3, r4, r5, r6}
	stmia	r1!, {r3, r4, r5, r6}
	adds	r3, r3, r3, lsl #1
	andne	r3, ip, r3, lsr #2
	strne	r3, [r0, #-16]
	adds	r4, r4, r4, lsl #1
	andne	r4, ip, r4, lsr #2
	strne	r4, [r0, #-12]
	adds	r5, r5, r5, lsl #1
	andne	r5, ip, r5, lsr #2
	strne	r5, [r0, #-8]
	adds	r6, r6, r6, lsl #1
	andne	r6, ip, r6, lsr #2
	strne	r6, [r0, #-4]
	cmp	r0, r2
	bne	.L_08002050
	pop	{r5, r6}
	bx	lr
	.global ColorBuffer_HalveNonzeroKernel
ColorBuffer_HalveNonzeroKernel:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_080020ac:
	ldm	r0!, {r3, r4, r5, r6}
	stmia	r1!, {r3, r4, r5, r6}
	cmp	r3, #0
	andne	r3, ip, r3, lsr #1
	strne	r3, [r0, #-16]
	cmp	r4, #0
	andne	r4, ip, r4, lsr #1
	strne	r4, [r0, #-12]
	cmp	r5, #0
	andne	r5, ip, r5, lsr #1
	strne	r5, [r0, #-8]
	cmp	r6, #0
	andne	r6, ip, r6, lsr #1
	strne	r6, [r0, #-4]
	cmp	r0, r2
	bne	.L_080020ac
	pop	{r5, r6}
	bx	lr
	.global ColorBuffer_DarkenPartialKernel
ColorBuffer_DarkenPartialKernel:
	rsb	ip, r1, #0x80000000
	add	ip, ip, #0x800000
	add	ip, ip, #0x8000
	add	ip, ip, #128
	mov	r1, #0x100
	add	r1, r1, #1
	orr	r1, r1, r1, lsl #16
	push	{r5, r6, r7, r8, lr}
	add	lr, r0, r3
.L_08002118:
	ldm	r0!, {r4, r5, r6, r7}
	stmia	r2!, {r4, r5, r6, r7}
	cmp	r4, #0
	bne	.L_0800214c
	cmp	r5, #0
	bne	.L_08002168
	cmp	r6, #0
	bne	.L_08002184
	cmp	r7, #0
	bne	.L_080021a0
	cmp	r0, lr
	bne	.L_08002118
	b	.L_080021bc
.L_0800214c:
	add	r8, r4, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-16]
	cmp	r5, #0
	beq	.L_0800217c
.L_08002168:
	add	r8, r5, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-12]
.L_0800217c:
	cmp	r6, #0
	beq	.L_08002198
.L_08002184:
	add	r8, r6, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-8]
.L_08002198:
	cmp	r7, #0
	beq	.L_080021b4
.L_080021a0:
	add	r8, r7, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-4]
.L_080021b4:
	cmp	r0, lr
	bne	.L_08002118
.L_080021bc:
	pop	{r5, r6, r7, r8, ip}
	bx	ip
	.global ColorBuffer_BrightenPartialKernel
ColorBuffer_BrightenPartialKernel:
	push	{r5, r6, r7, r8, r9}
	mov	r8, #64
	orr	r8, r8, r8, lsl #8
	orr	r8, r8, r8, lsl #16
	sub	ip, r8, r8, lsr #6
	add	r3, r0, r3
.L_080021dc:
	ldm	r0!, {r4, r5, r6, r7}
	stmia	r2!, {r4, r5, r6, r7}
	cmp	r4, ip
	bne	.L_08002210
	cmp	r5, ip
	bne	.L_08002230
	cmp	r6, ip
	bne	.L_08002250
	cmp	r7, ip
	bne	.L_08002270
	cmp	r0, r3
	bne	.L_080021dc
	b	.L_08002290
.L_08002210:
	add	r4, r4, r1
	and	r9, r4, r8
	bic	r4, r4, r9
	sub	r9, r9, r9, lsr #6
	orr	r4, r4, r9
	str	r4, [r0, #-16]
	cmp	r5, ip
	beq	.L_08002248
.L_08002230:
	add	r5, r5, r1
	and	r9, r5, r8
	bic	r5, r5, r9
	sub	r9, r9, r9, lsr #6
	orr	r5, r5, r9
	str	r5, [r0, #-12]
.L_08002248:
	cmp	r6, ip
	beq	.L_08002268
.L_08002250:
	add	r6, r6, r1
	and	r9, r6, r8
	bic	r6, r6, r9
	sub	r9, r9, r9, lsr #6
	orr	r6, r6, r9
	str	r6, [r0, #-8]
.L_08002268:
	cmp	r7, ip
	beq	.L_08002288
.L_08002270:
	add	r7, r7, r1
	and	r9, r7, r8
	bic	r7, r7, r9
	sub	r9, r9, r9, lsr #6
	orr	r7, r7, r9
	str	r7, [r0, #-4]
.L_08002288:
	cmp	r0, r3
	bne	.L_080021dc
.L_08002290:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
