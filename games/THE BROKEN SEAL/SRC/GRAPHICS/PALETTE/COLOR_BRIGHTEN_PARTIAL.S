.syntax unified
	.arm
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
ColorBuffer_KernelsEnd:
	.global ColorBuffer_BackupAndBrightenPartialCodeSize
	.set ColorBuffer_BackupAndBrightenPartialCodeSize, ColorBuffer_KernelsEnd - ColorBuffer_BrightenPartialKernel
