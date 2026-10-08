.syntax unified
	.arm
	.global ColorBuffer_ScaleThreeQuartersKernel
ColorBuffer_ScaleThreeQuartersKernel:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08001ebc:
	ldm	r0, {r3, r4, r5, r6}
	stmia	r1!, {r3, r4, r5, r6}
	adds	r3, r3, r3, lsl #1
	andne	r3, ip, r3, lsr #2
	adds	r4, r4, r4, lsl #1
	andne	r4, ip, r4, lsr #2
	adds	r5, r5, r5, lsl #1
	andne	r5, ip, r5, lsr #2
	adds	r6, r6, r6, lsl #1
	andne	r6, ip, r6, lsr #2
	stmia	r0!, {r3, r4, r5, r6}
	cmp	r0, r2
	bne	.L_08001ebc
	pop	{r5, r6}
	bx	lr
	.global ColorBuffer_HalveKernel
ColorBuffer_HalveKernel:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08001f0c:
	ldm	r0, {r3, r4, r5, r6}
	stmia	r1!, {r3, r4, r5, r6}
	and	r3, ip, r3, lsr #1
	and	r4, ip, r4, lsr #1
	and	r5, ip, r5, lsr #1
	and	r6, ip, r6, lsr #1
	stmia	r0!, {r3, r4, r5, r6}
	cmp	r0, r2
	bne	.L_08001f0c
	pop	{r5, r6}
	bx	lr
.L_halve_end:
	.global ColorBuffer_BackupAndScaleThreeQuartersCodeSize
	.set ColorBuffer_BackupAndScaleThreeQuartersCodeSize, ColorBuffer_HalveKernel - ColorBuffer_ScaleThreeQuartersKernel
	.global ColorBuffer_BackupAndHalveCodeSize
	.set ColorBuffer_BackupAndHalveCodeSize, .L_halve_end - ColorBuffer_HalveKernel
