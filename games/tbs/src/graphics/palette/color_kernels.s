@ C2: all three kernels save callee-saved r5/r6 (and further registers in
@ BrightenKernel) without saving lr.
.syntax unified
	.arm
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
.L_halve_nonzero_end:
	.global ColorBuffer_BackupAndBrightenCodeSize
	.set ColorBuffer_BackupAndBrightenCodeSize, ColorBuffer_ScaleNonzeroThreeQuartersKernel - ColorBuffer_BrightenKernel
	.global ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize
	.set ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize, ColorBuffer_HalveNonzeroKernel - ColorBuffer_ScaleNonzeroThreeQuartersKernel
	.global ColorBuffer_BackupAndHalveNonzeroCodeSize
	.set ColorBuffer_BackupAndHalveNonzeroCodeSize, .L_halve_nonzero_end - ColorBuffer_HalveNonzeroKernel
