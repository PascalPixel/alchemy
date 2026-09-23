@ Four colour-buffer kernels that save a backup copy as they go: 3/4 scaling
@ and halving that write back only non-zero words, and partial darken and
@ brighten that skip unchanged words, each run from the stack by its loader
@ in COLOR_KERNEL_LOADERS.S. Three save their registers without lr
@ (push {r5, r6} or {r5-r9}) and the partial darken pops its return address
@ into ip for bx ip; GCC saves lr with any register and returns through lr.
@ After each 4-byte Thumb entry (bx pc) the bodies equal the other four
@ kernels of TBS GRAPHICS/PALETTE/COLOR_KERNELS.S (0x0800203c..0x08002298)
@ byte for byte.
@ Its four 4-byte Thumb entries, which are TLA's own, carry no credit.
@ credit: library — TBS/TLA common ARM colour-buffer kernels copied to the stack
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08001d10
	.global ColorBuffer_ScaleNonzeroThreeQuartersKernel
	.thumb_func
ColorBuffer_ScaleNonzeroThreeQuartersKernel:
.L_08001d10:
AlchemyUncredited_08001d10:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001d10:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08001d28:
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
	bne	.L_08001d28
	pop	{r5, r6}
	bx	lr
	.thumb
	.global ColorBuffer_HalveNonzeroKernel
	.thumb_func
ColorBuffer_HalveNonzeroKernel:
.L_08001d70:
AlchemyUncredited_08001d70:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001d70:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08001d88:
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
	bne	.L_08001d88
	pop	{r5, r6}
	bx	lr
	.thumb
	.global ColorBuffer_DarkenPartialKernel
	.thumb_func
ColorBuffer_DarkenPartialKernel:
.L_08001dd0:
AlchemyUncredited_08001dd0:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001dd0:
	rsb	ip, r1, #0x80000000
	add	ip, ip, #0x800000
	add	ip, ip, #0x8000
	add	ip, ip, #128
	mov	r1, #0x100
	add	r1, r1, #1
	orr	r1, r1, r1, lsl #16
	push	{r5, r6, r7, r8, lr}
	add	lr, r0, r3
.L_08001df8:
	ldm	r0!, {r4, r5, r6, r7}
	stmia	r2!, {r4, r5, r6, r7}
	cmp	r4, #0
	bne	.L_08001e2c
	cmp	r5, #0
	bne	.L_08001e48
	cmp	r6, #0
	bne	.L_08001e64
	cmp	r7, #0
	bne	.L_08001e80
	cmp	r0, lr
	bne	.L_08001df8
	b	.L_08001e9c
.L_08001e2c:
	add	r8, r4, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-16]
	cmp	r5, #0
	beq	.L_08001e5c
.L_08001e48:
	add	r8, r5, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-12]
.L_08001e5c:
	cmp	r6, #0
	beq	.L_08001e78
.L_08001e64:
	add	r8, r6, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-8]
.L_08001e78:
	cmp	r7, #0
	beq	.L_08001e94
.L_08001e80:
	add	r8, r7, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-4]
.L_08001e94:
	cmp	r0, lr
	bne	.L_08001df8
.L_08001e9c:
	pop	{r5, r6, r7, r8, ip}
	bx	ip
	.thumb
	.global ColorBuffer_BrightenPartialKernel
	.thumb_func
ColorBuffer_BrightenPartialKernel:
.L_08001ea4:
AlchemyUncredited_08001ea4:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001ea4:
	push	{r5, r6, r7, r8, r9}
	mov	r8, #64
	orr	r8, r8, r8, lsl #8
	orr	r8, r8, r8, lsl #16
	sub	ip, r8, r8, lsr #6
	add	r3, r0, r3
.L_08001ec0:
	ldm	r0!, {r4, r5, r6, r7}
	stmia	r2!, {r4, r5, r6, r7}
	cmp	r4, ip
	bne	.L_08001ef4
	cmp	r5, ip
	bne	.L_08001f14
	cmp	r6, ip
	bne	.L_08001f34
	cmp	r7, ip
	bne	.L_08001f54
	cmp	r0, r3
	bne	.L_08001ec0
	b	.L_08001f74
.L_08001ef4:
	add	r4, r4, r1
	and	r9, r4, r8
	bic	r4, r4, r9
	sub	r9, r9, r9, lsr #6
	orr	r4, r4, r9
	str	r4, [r0, #-16]
	cmp	r5, ip
	beq	.L_08001f2c
.L_08001f14:
	add	r5, r5, r1
	and	r9, r5, r8
	bic	r5, r5, r9
	sub	r9, r9, r9, lsr #6
	orr	r5, r5, r9
	str	r5, [r0, #-12]
.L_08001f2c:
	cmp	r6, ip
	beq	.L_08001f4c
.L_08001f34:
	add	r6, r6, r1
	and	r9, r6, r8
	bic	r6, r6, r9
	sub	r9, r9, r9, lsr #6
	orr	r6, r6, r9
	str	r6, [r0, #-8]
.L_08001f4c:
	cmp	r7, ip
	beq	.L_08001f6c
.L_08001f54:
	add	r7, r7, r1
	and	r9, r7, r8
	bic	r7, r7, r9
	sub	r9, r9, r9, lsr #6
	orr	r7, r7, r9
	str	r7, [r0, #-4]
.L_08001f6c:
	cmp	r0, r3
	bne	.L_08001ec0
.L_08001f74:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
