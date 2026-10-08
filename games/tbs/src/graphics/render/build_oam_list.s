.syntax unified
	.arm
	.global Render_BuildOamListKernel
Render_BuildOamListKernel:
	push	{r5, r6, r7, r8, r9, lr}
	ldr	r2, [pc, #196]
	mov	ip, #64
	mov	r1, #128
	mov	r9, r0
.L_08001ddc:
	ldmdb	r2!, {r3, r4, r5, r6}
	movs	r6, r6
	blne	.L_08001e20
	movs	r6, r5
	blne	.L_08001e20
	movs	r6, r4
	blne	.L_08001e20
	movs	r6, r3
	blne	.L_08001e20
	subs	ip, ip, #1
	bne	.L_08001ddc
	mov	r2, #192
	orr	r2, r2, #0xc00000
	lsl	r1, r1, #3
	ldr	r4, [pc, #128]
	add	lr, pc, #52
	bx	r4
.L_08001e20:
	ldm	r6, {r6, r7, r8}
	stmia	r0!, {r7, r8}
	subs	r1, r1, #1
	beq	.L_08001e54
	cmp	r6, #0
	bxeq	lr
	ldm	r6, {r6, r7, r8}
	stmia	r0!, {r7, r8}
	subs	r1, r1, #1
	beq	.L_08001e54
	cmp	r6, #0
	bxeq	lr
	b	.L_08001e20
.L_08001e54:
	add	r0, r9, #6
	ldr	r2, [pc, #64]
	ldrb	r4, [r2]
	ldr	r1, [pc, #60]
	cmp	r4, #0
	beq	.L_08001e90
.L_08001e6c:
	ldm	r1!, {r2, r3}
	strh	r2, [r0], #32
	lsr	r2, r2, #16
	strh	r2, [r0, #-24]
	strh	r3, [r0, #-16]
	lsr	r3, r3, #16
	strh	r3, [r0, #-8]
	subs	r4, r4, #1
	bne	.L_08001e6c
.L_08001e90:
	pop	{r5, r6, r7, r8, r9, lr}
	bx	lr
	.4byte	gFrameTick
	.4byte	IwramFillWords
	.4byte	gObjAffineCount
	.4byte	gObjAffineMatrices
	.global LoadedRuntime_Size
	.set LoadedRuntime_Size, . - Render_BuildOamListKernel
