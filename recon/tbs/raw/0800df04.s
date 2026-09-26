.syntax unified
	.thumb
	.set sub_08004458, 0x08004458
	.set sub_0800447c, 0x0800447c
	.set sub_080044d0, 0x080044d0
	.set sub_0800d14c, 0x0800d14c
	.set sub_0800d924, 0x0800d924
	.set sub_080120dc, 0x080120dc
	.global ScriptObject_WanderNearHome
	.global Func_0800df04
	.thumb_func
ScriptObject_WanderNearHome:
Func_0800df04:
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
	ldmia	r3!, {r1}
	sub	sp, #52
	str	r1, [sp, #24]
	ldmia	r3!, {r2}
	str	r2, [sp, #20]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bge.n	.L_0800df34
	ldr	r4, [pc, #732]
	adds	r3, r3, r4
.L_0800df34:
	asrs	r3, r3, #16
	adds	r1, r3, #0
	muls	r1, r3
	movs	r2, #0
	movs	r0, #6
	ldrsh	r5, [r6, r0]
	mov	sl, r2
	ldr	r2, [r6, #8]
	str	r3, [sp, #16]
	str	r5, [sp, #12]
	str	r1, [sp, #16]
	cmp	r2, #0
	bge.n	.L_0800df52
	ldr	r3, [pc, #704]
	adds	r2, r2, r3
.L_0800df52:
	adds	r4, r6, #0
	adds	r4, #100
	str	r4, [sp, #8]
	movs	r5, #0
	ldrsh	r3, [r4, r5]
	asrs	r2, r2, #16
	subs	r2, r2, r3
	mov	fp, r2
	ldr	r2, [r6, #16]
	cmp	r2, #0
	bge.n	.L_0800df6c
	ldr	r0, [pc, #676]
	adds	r2, r2, r0
.L_0800df6c:
	adds	r1, r6, #0
	adds	r1, #102
	str	r1, [sp, #4]
	movs	r4, #0
	ldrsh	r3, [r1, r4]
	asrs	r2, r2, #16
	subs	r2, r2, r3
	mov	r9, r2
	mov	r5, fp
	mov	r0, r9
	mov	r3, fp
	muls	r3, r5
	mov	r2, r9
	muls	r2, r0
	ldr	r1, [sp, #16]
	adds	r3, r3, r2
	cmp	r3, r1
	ble.n	.L_0800df92
	b.n	.L_0800e146
.L_0800df92:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #7
	ble.n	.L_0800df9e
	b.n	.L_0800e146
.L_0800df9e:
	bl	sub_08004458
	ldr	r3, [pc, #624]
	ldr	r1, [sp, #20]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	ldr	r4, [sp, #24]
	adds	r4, r4, r0
	mov	r8, r4
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	ldr	r3, [r6, #8]
	add	r7, sp, #40
	ldr	r1, [sp, #12]
	str	r3, [r7, #0]
	lsls	r2, r1, #16
	ldr	r3, [r6, #12]
	lsrs	r5, r5, #2
	lsrs	r2, r2, #16
	lsrs	r0, r0, #2
	str	r3, [r7, #4]
	adds	r2, r2, r5
	subs	r2, r2, r0
	ldr	r3, [r6, #16]
	lsls	r2, r2, #16
	lsrs	r4, r2, #16
	movs	r0, #128
	adds	r1, r4, #0
	str	r3, [r7, #8]
	lsls	r0, r0, #12
	adds	r2, r7, #0
	str	r4, [sp, #0]
	bl	sub_0800447c
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_0800d924
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_0800df92
	ldr	r3, [r6, #8]
	str	r3, [r7, #0]
	ldr	r3, [r6, #12]
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	adds	r1, r4, #0
	str	r3, [r7, #8]
	mov	r0, r8
	adds	r2, r7, #0
	bl	sub_0800447c
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_080120dc
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_0800df92
	ldr	r3, [r6, #8]
	add	r5, sp, #28
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	movs	r2, #128
	ldr	r3, [r6, #16]
	lsls	r2, r2, #12
	add	r8, r2
	adds	r1, r4, #0
	str	r3, [r5, #8]
	mov	r0, r8
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_0800df92
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r1, r4, r3
	mov	r0, r8
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_0800df92
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r0, [pc, #412]
	ldr	r3, [r6, #16]
	adds	r1, r4, r0
	str	r3, [r5, #8]
	mov	r0, r8
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_0800e098
	b.n	.L_0800df92
.L_0800e098:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	movs	r2, #128
	ldr	r3, [r6, #16]
	lsls	r2, r2, #7
	adds	r1, r4, r2
	str	r3, [r5, #8]
	mov	r0, r8
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_0800e0c2
	b.n	.L_0800df92
.L_0800e0c2:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	ldr	r3, [pc, #332]
	mov	r0, r8
	adds	r1, r4, r3
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	beq.n	.L_0800e0e8
	b.n	.L_0800df92
.L_0800e0e8:
	ldr	r1, [r7, #0]
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_0800e0f4
	ldr	r4, [pc, #284]
	adds	r2, r1, r4
.L_0800e0f4:
	ldr	r0, [sp, #8]
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	asrs	r2, r2, #16
	ldr	r4, [r7, #8]
	subs	r2, r2, r3
	mov	fp, r2
	adds	r2, r4, #0
	cmp	r4, #0
	bge.n	.L_0800e10c
	ldr	r3, [pc, #260]
	adds	r2, r4, r3
.L_0800e10c:
	ldr	r0, [sp, #4]
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	asrs	r2, r2, #16
	subs	r2, r2, r3
	mov	r9, r2
	mov	r5, r9
	mov	r2, fp
	mov	r3, fp
	muls	r3, r2
	mov	r2, r9
	muls	r2, r5
	ldr	r0, [sp, #16]
	adds	r3, r3, r2
	cmp	r3, r0
	ble.n	.L_0800e12e
	b.n	.L_0800df92
.L_0800e12e:
	adds	r0, r6, #0
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r2, #2
	orrs	r2, r3
	strb	r2, [r0, #0]
	ldr	r2, [r7, #4]
	adds	r0, r6, #0
	adds	r3, r4, #0
	bl	sub_0800d14c
	b.n	.L_0800e1f6
.L_0800e146:
	movs	r1, #0
	mov	sl, r1
	mov	r0, r9
	mov	r1, fp
	bl	sub_080044d0
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r0, r2
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	str	r0, [sp, #12]
.L_0800e15e:
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	cmp	r4, #7
	bgt.n	.L_0800e1f6
	bl	sub_08004458
	ldr	r3, [pc, #164]
	ldr	r1, [sp, #20]
	mov	ip, pc
	bx	r3
	ldr	r5, [sp, #24]
	adds	r5, r5, r0
	bl	sub_08004458
	mov	r8, r5
	adds	r5, r0, #0
	bl	sub_08004458
	ldr	r1, [sp, #12]
	lsls	r2, r1, #16
	ldr	r3, [r6, #8]
	lsrs	r5, r5, #2
	lsrs	r2, r2, #16
	adds	r2, r2, r5
	add	r5, sp, #40
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	lsrs	r0, r0, #2
	str	r3, [r5, #4]
	subs	r2, r2, r0
	ldr	r3, [r6, #16]
	lsls	r2, r2, #16
	lsrs	r7, r2, #16
	movs	r0, #128
	lsls	r0, r0, #12
	adds	r1, r7, #0
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0800d924
	cmp	r0, #0
	bne.n	.L_0800e15e
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	mov	r0, r8
	adds	r1, r7, #0
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080120dc
	cmp	r0, #0
	bne.n	.L_0800e15e
	adds	r1, r6, #0
	adds	r1, #89
	ldrb	r2, [r1, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r6, #0
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	bl	sub_0800d14c
.L_0800e1f6:
	ldrh	r3, [r6, #4]
	adds	r3, #4
	movs	r0, #1
	strh	r3, [r6, #4]
	add	sp, #52
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
	.4byte 0xffffc000
