.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_08013560, 0x08013560
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08038060, 0x08038060
	.set sub_08038080, 0x08038080
	.set sub_080380d8, 0x080380d8
	.set sub_08038260, 0x08038260
	.set sub_08038268, 0x08038268
	.set sub_080ad0f8, 0x080ad0f8
	.set sub_080ad100, 0x080ad100
	.set sub_080ad110, 0x080ad110
	.set sub_080ad250, 0x080ad250
	.set sub_080f811c, 0x080f811c
	.set sub_080f88c4, 0x080f88c4
	.set sub_080f88d0, 0x080f88d0
	.set sub_080f8a44, 0x080f8a44
	.set sub_080f8ab4, 0x080f8ab4
	.set sub_080f9108, 0x080f9108
	.set sub_080fa3d4, 0x080fa3d4
	.set sub_080facd8, 0x080facd8
	.set sub_080fd6b0, 0x080fd6b0
	.set sub_080fd6f0, 0x080fd6f0
	.set sub_080fe638, 0x080fe638
	.set sub_080ff1f4, 0x080ff1f4
	.set sub_080ff27c, 0x080ff27c
	.set sub_080ff2e8, 0x080ff2e8
	.set sub_080ff370, 0x080ff370
	.set sub_081005e4, 0x081005e4
	.set sub_08104c00, 0x08104c00
	.set sub_08104d5c, 0x08104d5c
	.set sub_08104ef8, 0x08104ef8
	.set sub_08104fe0, 0x08104fe0
	.set sub_08105580, 0x08105580
	.set sub_081c0010, 0x081c0010
	.global Func_080fe894
	.thumb_func
Func_080fe894:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	sub	sp, #8
	movs	r2, #28
	ldrsb	r2, [r7, r2]
	movs	r3, #2
	str	r2, [sp, #4]
	mov	r9, r3
	movs	r1, #30
	ldrsb	r1, [r7, r1]
	lsls	r2, r2, #1
	mov	r8, r1
	movs	r1, #0
	str	r1, [sp, #0]
	movs	r1, #135
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	mov	sl, r3
	movs	r3, #129
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrh	r0, [r7, r2]
	bl	sub_08016ca4
	movs	r1, #140
	ldr	r0, [pc, #52]
	lsls	r1, r1, #2
	adds	r3, r7, r1
	movs	r2, #3
	movs	r1, #130
.L_080fe8e4:
	subs	r2, #1
	strh	r1, [r3, #0]
	strh	r0, [r3, #8]
	adds	r1, #32
	adds	r3, #2
	cmp	r2, #0
	bge.n	.L_080fe8e4
	movs	r0, #14
	bl	sub_080f9108
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #12]
	lsls	r1, r1, #19
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_080fe918
	.4byte 0x00000080
	.2byte 0x0200
	.2byte 0x0500
.L_080fe918:
	movs	r2, #128
	lsls	r2, r2, #24
	ldr	r0, [pc, #544]
	adds	r1, #28
	adds	r2, #1
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #24
	ldr	r0, [pc, #536]
	adds	r1, #4
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #24
	ldr	r0, [pc, #524]
	adds	r1, #28
	adds	r2, #1
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #129
	lsls	r2, r2, #2
	mov	fp, r2
	b.n	.L_080feae8
.L_080fe94a:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_080fe9f2
	adds	r5, r7, #0
	adds	r5, #240
	ldr	r0, [r5, #0]
	bl	sub_08038260
	ldr	r6, [pc, #496]
	ldr	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	movs	r0, #55
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080fe97e
	ldr	r1, [r5, #0]
	ldr	r0, [pc, #472]
	movs	r2, #0
	movs	r3, #16
	bl	sub_08038080
.L_080fe97e:
	movs	r2, #0
	movs	r3, #8
	ldr	r1, [r5, #0]
	subs	r0, r6, #3
	bl	sub_08038080
	ldr	r0, [sp, #4]
	mov	r1, r8
	add	r0, r8
	bl	sub_08002064
	str	r0, [sp, #4]
	lsls	r0, r0, #1
	add	r0, fp
	ldrh	r0, [r7, r0]
	bl	sub_08016ca4
	mov	r0, sl
	movs	r1, #3
	adds	r0, #3
	bl	sub_08002064
	ldr	r3, [sp, #4]
	mov	sl, r0
	lsls	r3, r3, #1
	add	r3, fp
	ldrh	r0, [r7, r3]
	mov	r1, sl
	bl	.L_080fee40
	ldr	r3, [sp, #4]
	adds	r0, r7, #0
	lsls	r3, r3, #1
	add	r3, fp
	ldrh	r1, [r7, r3]
	bl	sub_080f88c4
	mov	r1, r9
	cmp	r1, #2
	bne.n	.L_080fe9e4
	ldr	r0, [sp, #4]
	cmp	r0, #0
	bge.n	.L_080fe9d6
	adds	r0, #3
.L_080fe9d6:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	bl	sub_08104ef8
	movs	r0, #1
	bl	sub_08013560
.L_080fe9e4:
	mov	r2, r8
	ldr	r0, [r7, #16]
	ldr	r1, [sp, #4]
	bl	sub_08104d5c
	movs	r2, #0
	mov	r9, r2
.L_080fe9f2:
	ldr	r2, [sp, #4]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080fe9fc
	adds	r3, r2, #3
.L_080fe9fc:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	sub_080f8a44
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #316]
	movs	r3, #1
	ldr	r2, [r1, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fea2e
	movs	r0, #112
	bl	sub_081c0010
	movs	r3, #1
	str	r3, [sp, #0]
	b.n	.L_080feaf6
.L_080fea2e:
	ldr	r2, [r1, #4]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fea46
	movs	r0, #113
	bl	sub_081c0010
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #0]
	b.n	.L_080feaf6
.L_080fea46:
	ldr	r2, [r1, #12]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fea72
	ldr	r0, [sp, #4]
	movs	r1, #1
	bl	.L_080fecfc
	cmp	r0, #0
	beq.n	.L_080feab0
	movs	r0, #112
	bl	sub_081c0010
	ldr	r2, [sp, #4]
	adds	r3, r2, #1
	str	r3, [sp, #4]
	cmp	r3, #0
	bge.n	.L_080fea9c
	adds	r3, r2, #4
	b.n	.L_080fea9c
.L_080fea72:
	ldr	r2, [r1, #12]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080feabe
	ldr	r0, [sp, #4]
	movs	r1, #0
	bl	.L_080fecfc
	cmp	r0, #0
	beq.n	.L_080feab0
	movs	r0, #112
	bl	sub_081c0010
	ldr	r2, [sp, #4]
	subs	r3, r2, #1
	str	r3, [sp, #4]
	cmp	r3, #0
	bge.n	.L_080fea9c
	adds	r3, r2, #2
.L_080fea9c:
	asrs	r0, r3, #2
	lsls	r0, r0, #2
	bl	sub_08104ef8
	ldr	r0, [r7, #16]
	ldr	r1, [sp, #4]
	mov	r2, r8
	bl	sub_08104d5c
	b.n	.L_080feab6
.L_080feab0:
	movs	r0, #114
	bl	sub_081c0010
.L_080feab6:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_080feae8
.L_080feabe:
	ldr	r3, [r1, #4]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080feadc
	movs	r0, #55
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080feadc
	bl	sub_080fe638
	movs	r2, #1
	mov	r9, r2
	b.n	.L_080feae8
.L_080feadc:
	add	r0, sp, #4
	mov	r1, r8
	movs	r2, #4
	bl	sub_08104c00
	mov	r9, r0
.L_080feae8:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080feaf6
	b.n	.L_080fe94a
.L_080feaf6:
	ldr	r3, [sp, #4]
	movs	r2, #129
	strb	r3, [r7, #28]
	ldr	r3, [sp, #4]
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	movs	r1, #128
	str	r3, [r7, #8]
	ldr	r3, [sp, #4]
	lsls	r1, r1, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r2, [r7, r3]
	adds	r1, #22
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	movs	r2, #13
	subs	r1, #154
	strb	r2, [r3, #5]
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	strb	r2, [r3, #5]
	ldr	r0, [sp, #0]
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x050001c8
	.4byte 0x05000200
	.4byte 0x050001e8
	.4byte 0x0000103c
	.4byte 0x00001045
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	sub	sp, #4
	movs	r2, #28
	ldrsb	r2, [r7, r2]
	movs	r3, #2
	str	r2, [sp, #0]
	mov	fp, r3
	movs	r1, #30
	ldrsb	r1, [r7, r1]
	lsls	r2, r2, #1
	mov	sl, r1
	movs	r1, #135
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	adds	r6, r7, #0
	mov	r9, r3
	movs	r3, #129
	lsls	r3, r3, #2
	mov	r8, r3
	add	r2, r8
	adds	r6, #240
	ldrh	r0, [r7, r2]
	bl	sub_08016ca4
	ldr	r0, [r6, #0]
	bl	sub_08038260
	ldr	r5, [pc, #332]
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	sub_08038080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	sub_08038080
	movs	r1, #226
	lsls	r1, r1, #1
	mov	r6, r8
	adds	r5, r7, r1
.L_080febc8:
	mov	r2, fp
	cmp	r2, #0
	beq.n	.L_080fec64
	ldr	r0, [sp, #0]
	mov	r1, sl
	add	r0, sl
	bl	sub_08002064
	str	r0, [sp, #0]
	lsls	r0, r0, #1
	adds	r0, r0, r6
	ldrh	r0, [r7, r0]
	bl	sub_08016ca4
	mov	r0, r9
	movs	r1, #3
	adds	r0, #3
	bl	sub_08002064
	ldr	r3, [sp, #0]
	mov	r9, r0
	lsls	r3, r3, #1
	adds	r3, r3, r6
	ldrh	r0, [r7, r3]
	mov	r1, r9
	bl	.L_080fee40
	ldr	r3, [sp, #0]
	adds	r0, r7, #0
	lsls	r3, r3, #1
	adds	r3, r3, r6
	ldrh	r1, [r7, r3]
	bl	sub_080f88c4
	ldr	r3, [sp, #0]
	lsls	r3, r3, #1
	adds	r3, r3, r6
	ldrh	r0, [r7, r3]
	bl	sub_08016ca4
	movs	r2, #0
	adds	r1, r5, #0
	bl	sub_080fd6f0
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r7, r1
	strb	r0, [r3, #0]
	adds	r0, r5, #0
	bl	sub_080fd6b0
	movs	r2, #8
	movs	r0, #96
	movs	r1, #96
	bl	sub_081005e4
	adds	r0, r5, #0
	bl	sub_080facd8
	mov	r2, fp
	cmp	r2, #2
	bne.n	.L_080fec5a
	ldr	r0, [sp, #0]
	cmp	r0, #0
	bge.n	.L_080fec4c
	adds	r0, #3
.L_080fec4c:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	bl	sub_08104ef8
	movs	r0, #1
	bl	sub_08013560
.L_080fec5a:
	ldr	r0, [r7, #16]
	ldr	r1, [sp, #0]
	mov	r2, sl
	bl	sub_08104d5c
.L_080fec64:
	ldr	r2, [sp, #0]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080fec6e
	adds	r3, r2, #3
.L_080fec6e:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	sub_080f8a44
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #108]
	movs	r2, #1
	ldr	r3, [r1, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fec9e
	movs	r0, #112
	bl	sub_081c0010
	movs	r0, #1
	b.n	.L_080fecc2
.L_080fec9e:
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fecb4
	movs	r0, #113
	bl	sub_081c0010
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080fecc2
.L_080fecb4:
	mov	r0, sp
	mov	r1, sl
	movs	r2, #4
	bl	sub_08104c00
	mov	fp, r0
	b.n	.L_080febc8
.L_080fecc2:
	ldr	r3, [sp, #0]
	movs	r2, #129
	strb	r3, [r7, #28]
	ldr	r3, [sp, #0]
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	movs	r1, #128
	str	r3, [r7, #8]
	ldr	r3, [sp, #0]
	lsls	r1, r1, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r2, [r7, r3]
	adds	r1, #22
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00001136
	.2byte 0x1150
	.2byte 0x0300
.L_080fecfc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #139
	lsls	r2, r2, #1
	mov	r8, r3
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	sub	sp, #56
	cmp	r3, #1
	bls.n	.L_080fed2a
	cmp	r1, #1
	bne.n	.L_080fed2e
	ldrb	r3, [r2, #0]
	subs	r3, #1
	cmp	r0, r3
	bne.n	.L_080fed32
.L_080fed2a:
	movs	r0, #0
	b.n	.L_080fedfa
.L_080fed2e:
	cmp	r0, #0
	beq.n	.L_080fed2a
.L_080fed32:
	mov	r7, sp
	movs	r2, #0
	add	r3, sp, #52
	mov	ip, r7
.L_080fed3a:
	str	r2, [r3, #0]
	subs	r3, #4
	cmp	r3, ip
	bge.n	.L_080fed3a
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	movs	r6, #0
	cmp	r6, r3
	bge.n	.L_080fed70
	movs	r5, #139
	lsls	r5, r5, #1
	movs	r2, #129
	adds	r5, #255
	lsls	r2, r2, #2
	add	r5, r8
	adds	r4, r7, #0
	add	r2, r8
.L_080fed62:
	ldrh	r3, [r2, #0]
	adds	r6, #1
	stmia	r4!, {r3}
	adds	r2, #2
	ldrb	r3, [r5, #0]
	cmp	r6, r3
	blt.n	.L_080fed62
.L_080fed70:
	cmp	r1, #1
	bne.n	.L_080fed7a
	lsls	r3, r0, #2
	adds	r1, r3, #4
	b.n	.L_080fed7e
.L_080fed7a:
	lsls	r3, r0, #2
	subs	r1, r3, #4
.L_080fed7e:
	ldr	r6, [r7, r3]
	ldr	r2, [r7, r1]
	str	r2, [r7, r3]
	str	r6, [r7, r1]
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	movs	r6, #0
	cmp	r6, r3
	bge.n	.L_080fedb8
	movs	r2, #139
	lsls	r2, r2, #1
	adds	r2, #255
	movs	r5, #129
	add	r2, r8
	lsls	r5, r5, #2
	mov	sl, r2
	add	r5, r8
.L_080feda6:
	ldrh	r0, [r5, #0]
	bl	sub_080ad110
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r3
	blt.n	.L_080feda6
.L_080fedb8:
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	movs	r6, #0
	cmp	r6, r3
	bge.n	.L_080fede4
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r8
	mov	sl, r3
	adds	r5, r7, #0
.L_080fedd4:
	ldmia	r5!, {r0}
	bl	sub_080ad0f8
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r6, #1
	cmp	r6, r3
	blt.n	.L_080fedd4
.L_080fede4:
	movs	r0, #129
	lsls	r0, r0, #2
	add	r0, r8
	bl	sub_080ad100
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r8
	strb	r0, [r3, #0]
	movs	r0, #1
.L_080fedfa:
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	adds	r0, r5, #0
	bl	sub_080f88d0
	movs	r1, #2
	movs	r2, #2
	bl	sub_08104fe0
	adds	r3, r5, #0
	movs	r2, #0
	adds	r3, #244
	str	r2, [r5, #44]
	str	r2, [r5, #40]
	str	r2, [r5, #48]
	str	r2, [r5, #36]
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	adds	r2, r5, #0
	adds	r2, #246
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r2, #1
	movs	r3, #2
	strb	r3, [r2, #0]
	pop	{r5, pc}
.L_080fee40:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	adds	r7, r0, #0
	ldr	r5, [r6, #40]
	sub	sp, #8
	movs	r0, #0
	cmp	r5, #0
	bne.n	.L_080fee70
	movs	r3, #15
	adds	r5, r6, #0
	adds	r5, #40
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #5
	movs	r3, #30
	bl	sub_080f811c
	ldr	r5, [r5, #0]
.L_080fee70:
	cmp	r0, #0
	beq.n	.L_080feeb2
	movs	r3, #0
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #0
	adds	r3, r5, #0
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_080380d8
	movs	r2, #184
	lsls	r2, r2, #1
	adds	r3, r6, r2
	str	r0, [r3, #0]
	adds	r2, #172
	movs	r3, #240
	strb	r3, [r0, #15]
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #3
	bne.n	.L_080feea4
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080fa3d4
.L_080feea4:
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	sub_080ff370
	b.n	.L_080feebc
.L_080feeb2:
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	bl	sub_080ff370
.L_080feebc:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #48
	movs	r1, #0
	str	r3, [sp, #36]
	str	r1, [sp, #32]
	adds	r5, r3, #0
	ldr	r2, [r3, #40]
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r0, #0
	adds	r5, #48
	movs	r3, #2
	str	r2, [sp, #12]
	str	r3, [sp, #4]
	movs	r2, #0
	movs	r3, #30
	mov	r8, r0
	adds	r0, r5, #0
	bl	sub_080f811c
	movs	r0, #1
	negs	r0, r0
	ldr	r5, [r5, #0]
	bl	sub_080ad250
	negs	r3, r0
	orrs	r3, r0
	ldr	r4, [sp, #36]
	movs	r0, #128
	lsls	r0, r0, #2
	ldr	r1, [pc, #52]
	lsrs	r3, r3, #31
	adds	r0, #62
	str	r3, [sp, #16]
	movs	r7, #0
	mov	fp, r5
	movs	r2, #3
	adds	r3, r4, r0
.L_080fef20:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_080fef20
	movs	r1, #0
	movs	r0, #10
	str	r1, [sp, #20]
	negs	r0, r0
	movs	r1, #88
	bl	sub_080f8ab4
	movs	r3, #128
	ldr	r2, [sp, #36]
	lsls	r3, r3, #2
	adds	r3, #22
	adds	r3, r2, r3
	str	r3, [sp, #8]
	b.n	.L_080ff1a2
	movs	r0, r0
	.2byte 0x0068
	.2byte 0x0000
.L_080fef4c:
	ldr	r4, [sp, #8]
	ldrb	r0, [r4, #0]
	bl	sub_08016ca4
	ldr	r0, [sp, #8]
	movs	r2, #1
	ldrb	r1, [r0, #0]
	ldr	r0, [sp, #12]
	bl	sub_080ff370
	movs	r1, #40
	ldr	r3, [sp, #8]
	add	r1, sp
	mov	r9, r1
	ldrb	r2, [r3, #0]
	movs	r1, #1
	mov	r0, r9
	bl	sub_08105580
	lsls	r0, r0, #24
	movs	r4, #0
	lsrs	r1, r0, #24
	str	r4, [sp, #24]
	str	r1, [sp, #28]
	cmp	r0, #0
	bne.n	.L_080fefa6
	movs	r2, #1
	str	r2, [sp, #28]
	b.n	.L_080fefaa
.L_080fef86:
	movs	r0, #112
	bl	sub_081c0010
	movs	r3, #1
	str	r3, [sp, #20]
	str	r3, [sp, #32]
	b.n	.L_080ff1a2
.L_080fef94:
	movs	r0, #113
	bl	sub_081c0010
	movs	r0, #1
	movs	r4, #1
	negs	r0, r0
	str	r4, [sp, #20]
	str	r0, [sp, #32]
	b.n	.L_080ff1a2
.L_080fefa6:
	movs	r1, #1
	str	r1, [sp, #24]
.L_080fefaa:
	movs	r2, #1
	mov	sl, r2
	b.n	.L_080ff194
.L_080fefb0:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_080ff066
	ldr	r0, [sp, #28]
	mov	r2, r8
	lsls	r3, r0, #24
	adds	r2, #2
	asrs	r5, r3, #24
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	subs	r2, r2, r3
	movs	r4, #0
	mov	r8, r2
	mov	r0, fp
	mov	sl, r4
	bl	sub_08038260
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_080ff00c
	adds	r0, r7, r5
	adds	r1, r5, #0
	bl	sub_08002064
	ldr	r2, [sp, #24]
	adds	r7, r0, #0
	cmp	r2, #0
	bne.n	.L_080ff032
	ldr	r5, [pc, #412]
	movs	r6, #24
	negs	r6, r6
	adds	r0, r5, #0
	ldr	r1, [sp, #12]
	movs	r2, #0
	adds	r3, r6, #0
	bl	sub_08038080
	subs	r0, r5, #1
	ldr	r1, [sp, #12]
	movs	r2, #80
	adds	r3, r6, #0
	bl	sub_08038080
	b.n	.L_080ff032
.L_080ff00c:
	ldr	r3, [sp, #16]
	cmp	r3, #0
	beq.n	.L_080ff028
	adds	r2, r7, #0
	adds	r2, #8
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080ff020
	adds	r3, r7, #0
	adds	r3, #15
.L_080ff020:
	asrs	r7, r3, #3
	lsls	r3, r7, #3
	subs	r7, r2, r3
	b.n	.L_080ff032
.L_080ff028:
	adds	r0, r7, #7
	movs	r1, #7
	bl	sub_08002064
	adds	r7, r0, #0
.L_080ff032:
	adds	r1, r7, #0
	mov	r2, r9
	movs	r3, #0
	mov	r0, r8
	bl	sub_080ff1f4
	mov	r0, fp
	bl	sub_08038268
	movs	r0, #1
	bl	sub_08013560
	mov	r4, r8
	cmp	r4, #0
	bne.n	.L_080ff05c
	mov	r0, fp
	adds	r1, r7, #0
	mov	r2, r9
	bl	sub_080ff27c
	b.n	.L_080ff066
.L_080ff05c:
	mov	r0, fp
	adds	r1, r7, #0
	ldr	r2, [sp, #16]
	bl	sub_080ff2e8
.L_080ff066:
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_080ff07a
	lsls	r1, r7, #4
	movs	r0, #10
	adds	r1, #88
	negs	r0, r0
	bl	sub_080f8a44
	b.n	.L_080ff094
.L_080ff07a:
	cmp	r7, #3
	bgt.n	.L_080ff08a
	lsls	r1, r7, #3
	adds	r1, #48
	movs	r0, #24
	bl	sub_080f8a44
	b.n	.L_080ff094
.L_080ff08a:
	lsls	r1, r7, #3
	adds	r1, #80
	movs	r0, #48
	bl	sub_080f8a44
.L_080ff094:
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [pc, #244]
	movs	r3, #240
	ldr	r2, [r5, #12]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ff0b2
	mov	r0, r8
	adds	r1, r7, #0
	mov	r2, r9
	movs	r3, #1
	bl	sub_080ff1f4
.L_080ff0b2:
	ldr	r2, [r5, #4]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ff0be
	b.n	.L_080fef86
.L_080ff0be:
	ldr	r2, [r5, #4]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ff0ca
	b.n	.L_080fef94
.L_080ff0ca:
	ldr	r2, [r5, #12]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ff0e0
	movs	r0, #111
	bl	sub_081c0010
	movs	r1, #1
	mov	sl, r1
	subs	r7, #1
.L_080ff0e0:
	ldr	r2, [r5, #12]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ff0f6
	movs	r0, #111
	bl	sub_081c0010
	movs	r2, #1
	mov	sl, r2
	adds	r7, #1
.L_080ff0f6:
	ldr	r2, [r5, #12]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ff10c
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #1
	mov	sl, r3
	add	r8, r3
.L_080ff10c:
	ldr	r2, [r5, #12]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ff126
	movs	r0, #111
	bl	sub_081c0010
	movs	r0, #1
	movs	r4, #1
	negs	r0, r0
	mov	sl, r4
	add	r8, r0
.L_080ff126:
	ldr	r3, [r5, #12]
	movs	r6, #128
	lsls	r6, r6, #1
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_080ff13e
	ldr	r2, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ff194
.L_080ff13e:
	movs	r0, #111
	bl	sub_081c0010
	ldr	r3, [r5, #12]
	ldr	r1, [sp, #36]
	ands	r3, r6
	movs	r0, #28
	ldrsb	r0, [r1, r0]
	cmp	r3, #0
	beq.n	.L_080ff156
	adds	r0, #1
	b.n	.L_080ff158
.L_080ff156:
	subs	r0, #1
.L_080ff158:
	ldr	r2, [sp, #36]
	movs	r4, #139
	lsls	r4, r4, #1
	adds	r4, #255
	adds	r3, r2, r4
	ldrb	r1, [r3, #0]
	adds	r0, r0, r1
	bl	sub_08002064
	movs	r1, #129
	ldr	r4, [sp, #36]
	lsls	r2, r0, #1
	lsls	r1, r1, #2
	adds	r2, r2, r1
	ldrh	r3, [r4, r2]
	str	r3, [r4, #8]
	ldr	r1, [sp, #8]
	ldrh	r3, [r4, r2]
	strb	r3, [r1, #0]
	strb	r0, [r4, #28]
	ldr	r0, [sp, #36]
	ldrh	r1, [r4, r2]
	bl	sub_080f88c4
	b.n	.L_080ff1a2
	movs	r0, r0
	.4byte 0x00001036
	.2byte 0x1150
	.2byte 0x0300
.L_080ff194:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ff1a2
	b.n	.L_080fefb0
.L_080ff1a2:
	ldr	r2, [sp, #20]
	cmp	r2, #0
	bne.n	.L_080ff1b6
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ff1b6
	b.n	.L_080fef4c
.L_080ff1b6:
	ldr	r3, [sp, #36]
	ldr	r0, [r3, #48]
	bl	sub_08038060
	ldr	r0, [sp, #12]
	bl	sub_08038260
	movs	r1, #128
	ldr	r4, [sp, #36]
	lsls	r1, r1, #2
	ldr	r3, [pc, #8]
	adds	r1, #62
	movs	r2, #3
	adds	r0, r4, r1
	b.n	.L_080ff1d8
	.2byte 0xfff0
	.2byte 0xffff
.L_080ff1d8:
	subs	r2, #1
	strh	r3, [r0, #0]
	subs	r0, #2
	cmp	r2, #0
	bge.n	.L_080ff1d8
	ldr	r0, [sp, #32]
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
