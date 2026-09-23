.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080048f4, 0x080048f4
	.set sub_080058ac, 0x080058ac
	.set sub_08005c08, 0x08005c08
	.set sub_08006910, 0x08006910
	.set sub_080069c8, 0x080069c8
	.global SaveState_InitializeWorkspace
	.global Func_080056cc
	.thumb_func
SaveState_InitializeWorkspace:
Func_080056cc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #136
	lsls	r1, r1, #5
	movs	r0, #51
	sub	sp, #24
	bl	sub_080048f4
	movs	r3, #0
	mov	fp, r0
	add	r0, sp, #4
	str	r3, [r0, #0]
	mov	r1, fp
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #44]
	movs	r0, #2
	bl	sub_080069c8
	movs	r7, #0
	b.n	.L_0800570c
.L_08005704:
	movs	r0, #1
	bl	sub_080030f8
	adds	r7, #1
.L_0800570c:
	cmp	r7, #7
	bhi.n	.L_0800571c
	bl	sub_08006910
	lsls	r0, r0, #16
	cmp	r0, #0
	bne.n	.L_08005704
	b.n	.L_0800572c
.L_0800571c:
	movs	r0, #1
	b.n	.L_080057fc
	.4byte 0x040000d4
	.4byte 0x85000440
	.2byte 0x00f4
	.2byte 0x0300
.L_0800572c:
	mov	r2, fp
	movs	r3, #8
	adds	r2, #64
	add	r3, sp
	mov	r6, fp
	str	r2, [sp, #0]
	mov	r8, r3
	movs	r2, #32
	movs	r3, #16
	adds	r2, r2, r6
	adds	r3, r3, r6
	movs	r7, #0
	mov	r9, r2
	mov	sl, r3
.L_08005748:
	movs	r3, #0
	strb	r3, [r6, #0]
	mov	r2, sl
	movs	r3, #16
	strb	r3, [r2, #0]
	ldr	r3, [pc, #52]
	mov	r2, r9
	strh	r3, [r2, #0]
	adds	r0, r7, #0
	bl	sub_080058ac
	ldr	r3, [pc, #44]
	adds	r5, r0, #0
	add	r1, sp, #8
	ldr	r0, [sp, #0]
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_0800576c:
	ldr	r2, [pc, #28]
	ldr	r3, [r2, #8]
	movs	r2, #128
	lsls	r2, r2, #24
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0800576c
	mov	r0, r8
	ldr	r1, [pc, #20]
	movs	r2, #7
	bl	sub_08005c08
	b.n	.L_08005798
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x040000d4
	.4byte 0x84000004
	.2byte 0x79b0
	.2byte 0x0800
.L_08005798:
	cmp	r0, #0
	bne.n	.L_080057ea
	mov	r2, r8
	ldrh	r3, [r2, #10]
	mov	r2, r9
	strh	r3, [r2, #0]
	mov	r3, r8
	ldrb	r2, [r3, #7]
	adds	r1, r2, #0
	cmp	r1, #15
	bhi.n	.L_080057ea
	cmp	r5, #0
	bne.n	.L_080057ea
	movs	r3, #1
	strb	r3, [r6, #0]
	mov	r3, sl
	strb	r2, [r3, #0]
	cmp	r5, r7
	bcs.n	.L_080057ea
	mov	ip, r1
	mov	r1, fp
	adds	r0, r1, #0
	movs	r4, #0
	adds	r0, #32
.L_080057c8:
	ldrb	r3, [r1, #16]
	cmp	r3, ip
	bne.n	.L_080057e0
	mov	r3, r8
	ldrh	r3, [r3, #10]
	ldrh	r2, [r0, #0]
	mov	lr, r3
	cmp	r2, lr
	bcs.n	.L_080057de
	strb	r4, [r1, #0]
	b.n	.L_080057e0
.L_080057de:
	strb	r4, [r6, #0]
.L_080057e0:
	adds	r5, #1
	adds	r1, #1
	adds	r0, #2
	cmp	r5, r7
	bcc.n	.L_080057c8
.L_080057ea:
	movs	r2, #2
	movs	r3, #1
	adds	r7, #1
	adds	r6, #1
	add	r9, r2
	add	sl, r3
	cmp	r7, #15
	bls.n	.L_08005748
	movs	r0, #0
.L_080057fc:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
