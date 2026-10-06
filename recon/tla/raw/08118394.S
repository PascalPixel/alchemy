.syntax unified
	.thumb
	.global ColorBuffer_ScaleKernel
	.thumb_func
ColorBuffer_ScaleKernel:
	.global ColorBuffer_ScaleKernelCode
ColorBuffer_ScaleKernelCode:
	bx	pc
	.2byte	0x0200
	.arm
	push	{r5, r6, r7, r8, lr}
	mov	r5, r0
	mov	r0, r1
	asr	lr, r2, #8
	cmp	lr, #0x100
	movge	lr, #0x100
	mov	r6, #0x3e0
	mov	r7, #0x7c00
	mov	r8, #31
	cmp	r3, #0
	ble	.L_08118404
	mov	r4, r3
.L_081183c8:
	ldrh	r3, [r5], #2
	and	r1, r3, r8
	mul	r1, lr, r1
	and	r2, r3, r6
	mul	r2, lr, r2
	and	ip, r3, r7
	mul	r3, lr, ip
	subs	r4, r4, #1
	and	r1, r8, r1, lsr #8
	and	r2, r6, r2, lsr #8
	and	ip, r7, r3, lsr #8
	orr	r3, r1, r2
	orr	r3, r3, ip
	strh	r3, [r0], #2
	bne	.L_081183c8
.L_08118404:
	mov	r0, #0
	pop	{r5, r6, r7, r8, lr}
	bx	lr
