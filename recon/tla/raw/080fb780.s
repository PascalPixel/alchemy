.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08016ce4, 0x08016ce4
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_08038070, 0x08038070
	.set sub_08038080, 0x08038080
	.set sub_08038098, 0x08038098
	.set sub_080380b0, 0x080380b0
	.set sub_08038120, 0x08038120
	.set sub_08038260, 0x08038260
	.set sub_08038270, 0x08038270
	.set sub_080383e8, 0x080383e8
	.set sub_080ad010, 0x080ad010
	.set sub_080f9108, 0x080f9108
	.set sub_080f92ac, 0x080f92ac
	.set sub_080f93a4, 0x080f93a4
	.global Func_080fb780
	.thumb_func
Func_080fb780:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r5, #13
	mov	r8, r3
	movs	r3, #1
	mov	sl, r3
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r3, [r3, #0]
	sub	sp, #4
	strb	r5, [r3, #5]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #30
	movs	r3, #10
	mov	r9, r0
	movs	r0, #0
	bl	sub_08038010
	adds	r7, r0, #0
	ldr	r0, [pc, #232]
	bl	sub_08014644
	movs	r3, #184
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	movs	r6, #0
	strb	r5, [r3, #5]
	bl	sub_080f92ac
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_080fb7fe
.L_080fb7d8:
	ldr	r3, [r1, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fb7e8
	movs	r3, #1
	subs	r6, #1
	mov	sl, r3
.L_080fb7e8:
	ldr	r3, [r1, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fb7f8
	movs	r3, #1
	adds	r6, #1
	mov	sl, r3
.L_080fb7f8:
	movs	r0, #1
	bl	sub_08013560
.L_080fb7fe:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fb840
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_080fb826
	movs	r3, #0
	adds	r0, r6, #5
	movs	r1, #5
	mov	sl, r3
	bl	sub_08002064
	mov	r1, r9
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	.L_080fb8ac
.L_080fb826:
	ldr	r1, [pc, #128]
	movs	r2, #1
	ldr	r3, [r1, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080fb840
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fb7d8
	movs	r6, #1
	negs	r6, r6
.L_080fb840:
	adds	r0, r7, #0
	bl	sub_08038260
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08038018
	mov	r3, r8
	ldr	r0, [r3, #16]
	bl	sub_08038260
	mov	r3, r8
	ldr	r0, [r3, #16]
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r2, #3
	movs	r3, #12
	movs	r1, #0
	bl	sub_08038070
	movs	r0, #14
	bl	sub_080f9108
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl	sub_080145a8
	movs	r3, #184
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #1
	strb	r3, [r2, #5]
	movs	r0, #13
	movs	r1, #0
	movs	r2, #17
	movs	r3, #10
	bl	sub_080383e8
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x08104da9
	.2byte 0x1150
	.2byte 0x0300
.L_080fb8ac:
	push	{lr}
	movs	r2, #1
	negs	r2, r2
	bl	.L_080fb8b8
	pop	{pc}
.L_080fb8b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	sub	sp, #12
	movs	r5, #0
	ands	r0, r1
	mov	fp, r1
	str	r5, [sp, #8]
	bl	sub_080ad010
	ldrb	r3, [r0, #2]
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_080fb990
	ldr	r3, [r0, #8]
	ldr	r2, [pc, #796]
	movs	r1, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080fb910
	mov	r2, sl
	mov	r9, r1
	adds	r2, #24
.L_080fb8f6:
	mov	r0, r9
	cmp	r0, #3
	bgt.n	.L_080fb912
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080fb910
	mov	r0, sl
	ldrb	r3, [r0, #12]
	movs	r0, #1
	adds	r2, #4
	add	r9, r0
	cmp	r3, #3
	bne.n	.L_080fb8f6
.L_080fb910:
	movs	r1, #1
.L_080fb912:
	cmp	r1, #1
	bne.n	.L_080fb924
	ldr	r0, [pc, #752]
	mov	r1, r8
	movs	r2, #8
	movs	r3, #0
	bl	sub_08038080
	movs	r5, #1
.L_080fb924:
	mov	r2, sl
	movs	r1, #8
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	beq.n	.L_080fb95a
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	lsls	r6, r5, #3
	adds	r3, r6, #0
	ldr	r0, [pc, #724]
	mov	r1, r8
	movs	r2, #0
	bl	sub_08038080
	mov	r0, sl
	movs	r3, #8
	ldrsh	r7, [r0, r3]
	movs	r1, #3
	adds	r0, r7, #0
	mov	r2, r8
	movs	r3, #72
	str	r6, [sp, #0]
	bl	.L_080fbd34
	adds	r5, #1
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
.L_080fb95a:
	mov	r1, sl
	movs	r3, #10
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_080fb990
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	lsls	r6, r5, #3
	ldr	r0, [pc, #676]
	mov	r1, r8
	movs	r2, #0
	adds	r3, r6, #0
	bl	sub_08038080
	mov	r2, sl
	movs	r7, #10
	ldrsb	r7, [r2, r7]
	movs	r1, #3
	adds	r0, r7, #0
	mov	r2, r8
	movs	r3, #72
	str	r6, [sp, #0]
	bl	.L_080fbd34
	adds	r5, #1
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
.L_080fb990:
	movs	r3, #0
	mov	r9, r3
.L_080fb994:
	mov	r0, r9
	lsls	r1, r0, #2
	adds	r2, r1, #0
	adds	r2, #24
	mov	r0, sl
	ldrb	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L_080fb9a6
	b.n	.L_080fbb2a
.L_080fb9a6:
	adds	r3, r0, r2
	movs	r7, #1
	ldrsb	r7, [r3, r7]
	ldrb	r3, [r3, #0]
	cmp	r3, #27
	bls.n	.L_080fb9b4
	b.n	.L_080fbb20
.L_080fb9b4:
	ldr	r2, [pc, #604]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080fbb20
	.4byte 0x080fba2c
	.4byte 0x080fba2c
	.4byte 0x080fba2c
	.4byte 0x080fba2c
	.4byte 0x080fba2c
	.4byte 0x080fba2c
	.4byte 0x080fba9e
	.4byte 0x080fba9e
	.4byte 0x080fba9e
	.4byte 0x080fba9e
	.4byte 0x080fba9e
	.4byte 0x080fba9e
	.4byte 0x080fba9e
	.4byte 0x080fba9e
	.4byte 0x080fba42
	.4byte 0x080fba42
	.4byte 0x080fba42
	.4byte 0x080fba42
	.4byte 0x080fba42
	.4byte 0x080fba42
	.4byte 0x080fba42
	.4byte 0x080fba42
	.4byte 0x080fbb06
	.4byte 0x080fbb20
	.4byte 0x080fbb06
	.4byte 0x080fba2c
	.2byte 0xbb06
	.2byte 0x080f
	adds	r3, r1, #0
	adds	r3, #24
	mov	r1, sl
	ldrb	r0, [r1, r3]
	ldr	r3, [pc, #480]
	lsls	r6, r5, #24
	asrs	r5, r6, #21
	adds	r0, r0, r3
	mov	r1, r8
	movs	r2, #0
	b.n	.L_080fba88
	adds	r4, r1, #0
	mov	r3, sl
	adds	r4, #24
	ldrb	r2, [r3, r4]
	adds	r3, r2, #0
	subs	r3, #15
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_080fba58
	adds	r1, r2, #0
	subs	r1, #12
.L_080fba58:
	asrs	r1, r1, #2
	lsls	r1, r1, #2
	subs	r1, r3, r1
	lsls	r1, r1, #24
	lsls	r6, r5, #24
	asrs	r5, r6, #24
	movs	r3, #2
	lsrs	r1, r1, #24
	str	r3, [sp, #0]
	adds	r1, #1
	adds	r3, r5, #0
	mov	r0, r8
	movs	r2, #0
	str	r4, [sp, #4]
	bl	sub_08038270
	ldr	r4, [sp, #4]
	mov	r1, sl
	ldrb	r0, [r1, r4]
	ldr	r3, [pc, #408]
	lsls	r5, r5, #3
	adds	r0, r0, r3
	mov	r1, r8
	movs	r2, #8
.L_080fba88:
	adds	r3, r5, #0
	bl	sub_08038080
	adds	r0, r7, #0
	movs	r1, #3
	mov	r2, r8
	movs	r3, #72
	str	r5, [sp, #0]
	bl	.L_080fbd34
	b.n	.L_080fbb22
	adds	r3, r1, #0
	adds	r3, #24
	mov	r2, sl
	ldrb	r0, [r2, r3]
	ldr	r3, [pc, #368]
	lsls	r6, r5, #24
	asrs	r5, r6, #21
	adds	r0, r0, r3
	mov	r1, r8
	movs	r2, #0
	adds	r3, r5, #0
	bl	sub_08038080
	cmp	r7, #9
	ble.n	.L_080fbadc
	movs	r0, #1
	movs	r1, #1
	mov	r2, r8
	movs	r3, #72
	str	r5, [sp, #0]
	bl	sub_080380b0
	ldr	r0, [pc, #336]
	mov	r1, r8
	movs	r2, #80
	adds	r3, r5, #0
	bl	sub_08038098
	adds	r0, r7, #0
	subs	r0, #10
	b.n	.L_080fbaf8
.L_080fbadc:
	movs	r0, #0
	movs	r1, #1
	mov	r2, r8
	movs	r3, #72
	str	r5, [sp, #0]
	bl	sub_080380b0
	ldr	r0, [pc, #304]
	mov	r1, r8
	movs	r2, #80
	adds	r3, r5, #0
	bl	sub_08038098
	adds	r0, r7, #0
.L_080fbaf8:
	movs	r1, #1
	mov	r2, r8
	movs	r3, #88
	str	r5, [sp, #0]
	bl	sub_080380b0
	b.n	.L_080fbb22
	adds	r3, r1, #0
	adds	r3, #24
	mov	r1, sl
	ldrb	r0, [r1, r3]
	ldr	r3, [pc, #264]
	lsls	r6, r5, #24
	adds	r0, r0, r3
	mov	r1, r8
	asrs	r3, r6, #21
	movs	r2, #0
	bl	sub_08038080
	b.n	.L_080fbb22
.L_080fbb20:
	lsls	r6, r5, #24
.L_080fbb22:
	movs	r2, #128
	lsls	r2, r2, #17
	adds	r3, r6, r2
	lsrs	r5, r3, #24
.L_080fbb2a:
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #3
	bgt.n	.L_080fbb36
	b.n	.L_080fb994
.L_080fbb36:
	mov	r1, sl
	ldrb	r2, [r1, #3]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fbb56
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	lsls	r3, r5, #3
	ldr	r0, [pc, #216]
	mov	r1, r8
	movs	r2, #0
	bl	sub_08038080
	adds	r5, #1
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
.L_080fbb56:
	mov	r3, sl
	ldrb	r2, [r3, #12]
	adds	r3, r2, #0
	cmp	r3, #3
	bne.n	.L_080fbb7e
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	ldr	r0, [pc, #188]
	lsls	r3, r5, #3
	mov	r1, r8
	movs	r2, #0
	bl	sub_08038080
	movs	r0, #1
	str	r0, [sp, #8]
	mov	r1, sl
	adds	r5, #1
	ldrb	r2, [r1, #12]
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
.L_080fbb7e:
	adds	r3, r2, #0
	cmp	r3, #4
	beq.n	.L_080fbc60
	cmp	r3, #0
	beq.n	.L_080fbc60
	ldr	r2, [sp, #8]
	cmp	r2, #0
	bne.n	.L_080fbba4
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	lsls	r3, r5, #3
	ldr	r0, [pc, #144]
	mov	r1, r8
	movs	r2, #16
	bl	sub_08038080
	adds	r5, #1
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
.L_080fbba4:
	mov	r0, sl
	ldrb	r3, [r0, #12]
	cmp	r3, #1
	beq.n	.L_080fbbb6
	cmp	r3, #1
	ble.n	.L_080fbc60
	cmp	r3, #2
	beq.n	.L_080fbbcc
	b.n	.L_080fbc60
.L_080fbbb6:
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	lsls	r3, r5, #3
	adds	r5, #1
	ldr	r0, [pc, #108]
	mov	r1, r8
	movs	r2, #0
	lsls	r5, r5, #24
	bl	sub_08038080
	b.n	.L_080fbc5e
.L_080fbbcc:
	movs	r3, #128
	lsls	r3, r3, #3
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080fbc34
	lsls	r5, r5, #24
	ldr	r6, [pc, #84]
	asrs	r5, r5, #24
	lsls	r3, r5, #3
	adds	r5, #1
	lsls	r5, r5, #24
	adds	r0, r6, #0
	mov	r1, r8
	movs	r2, #0
	asrs	r5, r5, #24
	bl	sub_08038080
	adds	r6, #1
	lsls	r3, r5, #3
	adds	r5, #1
	adds	r0, r6, #0
	mov	r1, r8
	movs	r2, #0
	lsls	r5, r5, #24
	bl	sub_08038080
	b.n	.L_080fbc5e
	.4byte 0x00ffffff
	.4byte 0x0000109d
	.4byte 0x00001026
	.4byte 0x00001027
	.4byte 0x080fb9bc
	.4byte 0x0000106b
	.4byte 0x08105968
	.4byte 0x000010a6
	.4byte 0x00001095
	.4byte 0x0000109e
	.4byte 0x00001093
	.2byte 0x10a3
	.2byte 0x0000
.L_080fbc34:
	lsls	r5, r5, #24
	ldr	r6, [pc, #232]
	asrs	r5, r5, #24
	lsls	r3, r5, #3
	adds	r5, #1
	lsls	r5, r5, #24
	adds	r0, r6, #0
	mov	r1, r8
	movs	r2, #0
	asrs	r5, r5, #24
	adds	r6, #1
	bl	sub_08038080
	lsls	r3, r5, #3
	adds	r0, r6, #0
	mov	r1, r8
	movs	r2, #0
	bl	sub_08038080
	adds	r5, #1
	lsls	r5, r5, #24
.L_080fbc5e:
	lsrs	r5, r5, #24
.L_080fbc60:
	mov	r3, sl
	ldrb	r2, [r3, #3]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fbcc4
	lsls	r6, r5, #24
	asrs	r3, r6, #24
	cmp	r3, #0
	beq.n	.L_080fbc78
	adds	r3, #1
	lsls	r6, r3, #24
.L_080fbc78:
	ldr	r0, [pc, #168]
	asrs	r5, r6, #24
	lsls	r3, r5, #3
	mov	r1, r8
	movs	r2, #16
	mov	r9, r0
	bl	sub_08038080
	movs	r0, #248
	adds	r5, #1
	lsls	r0, r0, #8
	mov	r1, fp
	lsls	r5, r5, #24
	ands	r0, r1
	lsrs	r5, r5, #24
	cmp	r0, #0
	bge.n	.L_080fbca2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r2, #255
	adds	r0, r0, r2
.L_080fbca2:
	asrs	r7, r0, #11
	adds	r0, r7, #1
	movs	r1, #5
	lsls	r5, r5, #24
	bl	sub_08038120
	asrs	r5, r5, #24
	mov	r0, r9
	lsls	r3, r5, #3
	adds	r0, #1
	mov	r1, r8
	movs	r2, #0
	bl	sub_08038080
	adds	r5, #1
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
.L_080fbcc4:
	movs	r1, #0
	cmp	r5, #0
	bne.n	.L_080fbd12
	mov	r3, sl
	ldrb	r2, [r3, #3]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fbce4
	mov	r1, r8
	ldr	r0, [pc, #76]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	movs	r1, #1
.L_080fbce4:
	cmp	r1, #0
	bne.n	.L_080fbd12
	mov	r0, sl
	ldrb	r2, [r0, #3]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fbd02
	mov	r1, r8
	ldr	r0, [pc, #52]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	movs	r1, #1
.L_080fbd02:
	cmp	r1, #0
	bne.n	.L_080fbd12
	ldr	r0, [pc, #40]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
.L_080fbd12:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x000010a1
	.4byte 0x0000109f
	.4byte 0x00001099
	.4byte 0x0000109a
	.2byte 0x109c
	.2byte 0x0000
.L_080fbd34:
	push	{r5, r6, r7, lr}
	sub	sp, #4
	adds	r6, r3, #0
	ldr	r3, [sp, #20]
	adds	r5, r0, #0
	str	r3, [sp, #0]
	movs	r1, #3
	adds	r3, r6, #0
	adds	r7, r2, #0
	bl	sub_080380b0
	movs	r2, #1
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_080fbd54
	negs	r3, r5
.L_080fbd54:
	cmp	r3, #9
	ble.n	.L_080fbd5a
	movs	r2, #2
.L_080fbd5a:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_080fbd62
	negs	r3, r5
.L_080fbd62:
	cmp	r3, #99
	ble.n	.L_080fbd68
	movs	r2, #3
.L_080fbd68:
	cmp	r5, #0
	ble.n	.L_080fbd7e
	lsls	r2, r2, #3
	subs	r2, r6, r2
	ldr	r0, [pc, #32]
	adds	r2, #16
	adds	r1, r7, #0
	ldr	r3, [sp, #20]
	bl	sub_08038098
	b.n	.L_080fbd8e
.L_080fbd7e:
	lsls	r2, r2, #3
	subs	r2, r6, r2
	ldr	r0, [pc, #20]
	adds	r2, #16
	adds	r1, r7, #0
	ldr	r3, [sp, #20]
	bl	sub_08038098
.L_080fbd8e:
	add	sp, #4
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0810596c
	.2byte 0x5970
	.2byte 0x0810
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #4
	ldr	r0, [r3, #36]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #13
	movs	r2, #5
	movs	r3, #17
	bl	sub_080f93a4
	add	sp, #4
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #4
	ldr	r0, [r3, #36]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #13
	movs	r2, #3
	movs	r3, #17
	bl	sub_080f93a4
	add	sp, #4
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #4
	adds	r3, #240
	ldr	r0, [r3, #0]
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r1, #13
	movs	r2, #0
	movs	r3, #17
	bl	sub_080f93a4
	add	sp, #4
	pop	{pc}
	.align 2, 0
