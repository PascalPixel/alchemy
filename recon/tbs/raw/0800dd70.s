.syntax unified
	.thumb
	.set sub_08004458, 0x08004458
	.set sub_0800447c, 0x0800447c
	.set sub_0800d14c, 0x0800d14c
	.set sub_0800d924, 0x0800d924
	.set sub_080120dc, 0x080120dc
	.global Func_0800dd70
	.thumb_func
Func_0800dd70:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	movs	r0, #4
	ldrsh	r2, [r6, r0]
	ldr	r3, [r6, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r3, #4
	ldmia	r3!, {r2}
	sub	sp, #32
	str	r2, [sp, #4]
	ldmia	r3!, {r5}
	ldr	r3, [r3, #0]
	mov	fp, r5
	cmp	r3, #0
	bge.n	.L_0800dda0
	ldr	r0, [pc, #344]
	adds	r3, r3, r0
.L_0800dda0:
	asrs	r3, r3, #16
	adds	r5, r3, #0
	muls	r5, r3
	str	r3, [sp, #0]
	movs	r2, #0
	str	r5, [sp, #0]
	mov	r9, r2
.L_0800ddae:
	movs	r0, #1
	add	r9, r0
	mov	r2, r9
	cmp	r2, #7
	ble.n	.L_0800ddba
	b.n	.L_0800debe
.L_0800ddba:
	ldr	r3, [r6, #8]
	add	r7, sp, #20
	str	r3, [r7, #0]
	ldr	r3, [r6, #12]
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	str	r3, [r7, #8]
	bl	sub_08004458
	ldr	r3, [pc, #300]
	mov	r1, fp
	mov	ip, pc
	bx	r3
	ldr	r3, [sp, #4]
	adds	r3, r3, r0
	mov	r8, r3
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	ldrh	r3, [r6, #6]
	lsrs	r5, r5, #2
	lsrs	r0, r0, #2
	adds	r3, r3, r5
	subs	r3, r3, r0
	mov	sl, r3
	mov	r0, r8
	mov	r1, sl
	adds	r2, r7, #0
	bl	sub_0800447c
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_0800d924
	cmp	r0, #0
	bne.n	.L_0800ddae
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800ddae
	movs	r5, #128
	ldr	r3, [r6, #8]
	lsls	r5, r5, #12
	add	r8, r5
	add	r5, sp, #8
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	mov	r0, r8
	str	r3, [r5, #8]
	mov	r1, sl
	adds	r2, r5, #0
	bl	sub_0800447c
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	movs	r1, #128
	ldr	r3, [r6, #16]
	lsls	r1, r1, #6
	add	r1, sl
	mov	r0, r8
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800ddae
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r1, [pc, #160]
	ldr	r3, [r6, #16]
	add	r1, sl
	mov	r0, r8
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800ddae
	ldr	r3, [r7, #0]
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_0800de86
	ldr	r0, [pc, #116]
	adds	r3, r3, r0
.L_0800de86:
	adds	r2, r6, #0
	adds	r2, #100
	movs	r5, #0
	ldrsh	r2, [r2, r5]
	asrs	r3, r3, #16
	subs	r0, r3, r2
	ldr	r2, [r7, #8]
	adds	r4, r2, #0
	cmp	r2, #0
	bge.n	.L_0800de9e
	ldr	r3, [pc, #92]
	adds	r2, r2, r3
.L_0800de9e:
	adds	r3, r6, #0
	adds	r3, #102
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	asrs	r2, r2, #16
	subs	r2, r2, r3
	adds	r3, r0, #0
	muls	r3, r0
	adds	r0, r2, #0
	muls	r0, r2
	adds	r2, r0, #0
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	cmp	r3, r2
	ble.n	.L_0800ded4
	b.n	.L_0800ddae
.L_0800debe:
	ldrh	r3, [r6, #6]
	movs	r5, #128
	lsls	r5, r5, #8
	adds	r3, r3, r5
	adds	r2, r6, #0
	strh	r3, [r6, #6]
	adds	r2, #94
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #0
	b.n	.L_0800dee6
.L_0800ded4:
	adds	r0, r6, #0
	adds	r3, r4, #0
	ldr	r2, [r7, #4]
	bl	sub_0800d14c
	ldrh	r3, [r6, #4]
	adds	r3, #4
	strh	r3, [r6, #4]
	movs	r0, #1
.L_0800dee6:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0000ffff
	.4byte 0x03000118
	.4byte 0xffffe000
