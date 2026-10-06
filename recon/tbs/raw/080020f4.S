.syntax unified
	.arm
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
.L_darken_partial_end:
	.global ColorBuffer_BackupAndDarkenPartialCodeSize
	.set ColorBuffer_BackupAndDarkenPartialCodeSize, .L_darken_partial_end - ColorBuffer_DarkenPartialKernel
