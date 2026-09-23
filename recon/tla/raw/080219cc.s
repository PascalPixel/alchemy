.syntax unified
	.thumb
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_0801401c, 0x0801401c
	.set sub_080140d8, 0x080140d8
	.set sub_08014128, 0x08014128
	.set sub_080142d4, 0x080142d4
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d78, 0x08014d78
	.set sub_0801587c, 0x0801587c
	.set sub_08021940, 0x08021940
	.set sub_08021e62, 0x08021e62
	.set sub_08022acc, 0x08022acc
	.set sub_081c0010, 0x081c0010
	.global Func_080219cc
	.thumb_func
Func_080219cc:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldrb	r2, [r5, #27]
	movs	r6, #0
	cmp	r6, r2
	bge.n	.L_08021a7e
.L_080219d8:
	lsls	r3, r6, #2
	adds	r3, #40
	ldr	r4, [r5, r3]
	cmp	r4, #0
	beq.n	.L_08021a78
	ldr	r3, [r4, #16]
	cmp	r3, #0
	beq.n	.L_08021a78
.L_080219e8:
	ldrb	r3, [r4, #20]
	ldr	r1, [r4, #16]
	adds	r2, r3, #1
	strb	r2, [r4, #20]
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	ldrb	r0, [r1, r3]
	adds	r3, r2, #1
	strb	r3, [r4, #20]
	lsls	r2, r2, #24
	adds	r3, r0, #0
	lsrs	r2, r2, #24
	subs	r3, #239
	ldrb	r1, [r1, r2]
	cmp	r3, #16
	bhi.n	.L_08021a66
	ldr	r2, [pc, #116]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08021a6a
	.4byte 0x08021a54
	.4byte 0x08021a6a
	.4byte 0x08021a5c
	.4byte 0x08021a58
	.4byte 0x080219e8
	.4byte 0x080219e8
	.4byte 0x080219e8
	.4byte 0x080219e8
	.4byte 0x080219e8
	.4byte 0x080219e8
	.4byte 0x080219e8
	.4byte 0x08021a66
	.4byte 0x080219e8
	.4byte 0x08021a6a
	.4byte 0x08021a6a
	.2byte 0x1a60
	.2byte 0x0802
	strb	r1, [r4, #4]
	b.n	.L_080219e8
	strb	r1, [r5, #22]
	b.n	.L_080219e8
	strb	r1, [r5, #23]
	b.n	.L_080219e8
	movs	r3, #255
	strb	r3, [r4, #23]
	b.n	.L_080219e8
.L_08021a66:
	strb	r0, [r4, #23]
	b.n	.L_080219e8
	ldrb	r3, [r4, #20]
	ldrb	r2, [r5, #27]
	adds	r3, #254
	strb	r3, [r4, #20]
	movs	r3, #1
	strb	r3, [r5, #25]
	strh	r3, [r4, #2]
.L_08021a78:
	adds	r6, #1
	cmp	r6, r2
	blt.n	.L_080219d8
.L_08021a7e:
	pop	{r5, r6, pc}
	.2byte 0x1a10
	.2byte 0x0802
.L_08021a84:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	adds	r7, r0, #0
	movs	r0, #0
	str	r0, [sp, #24]
	movs	r5, #192
	lsls	r5, r5, #18
	mov	sl, r1
	ldr	r1, [r5, #24]
	movs	r2, #1
	str	r1, [sp, #16]
	str	r2, [sp, #8]
	ldr	r3, [r5, #80]
	str	r3, [sp, #12]
	cmp	r3, #0
	bne.n	.L_08021ada
	ldr	r1, [pc, #844]
	movs	r0, #80
	bl	sub_08014cc0
	ldr	r2, [pc, #840]
	adds	r1, r0, #0
	ldr	r0, [pc, #840]
	movs	r4, #132
	subs	r2, r2, r0
	movs	r3, #128
	lsls	r4, r4, #24
	lsrs	r2, r2, #2
	lsls	r3, r3, #19
	adds	r3, #212
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [r5, #80]
	movs	r4, #0
	str	r5, [sp, #12]
	str	r4, [sp, #8]
.L_08021ada:
	ldrb	r2, [r7, #27]
	ldr	r1, [sp, #24]
	movs	r0, #0
	mov	r8, r0
	cmp	r1, r2
	blt.n	.L_08021ae8
	b.n	.L_08021cae
.L_08021ae8:
	mov	r4, r8
	lsls	r3, r4, #2
	adds	r3, #40
	ldr	r6, [r7, r3]
	cmp	r6, #0
	bne.n	.L_08021af6
	b.n	.L_08021ca4
.L_08021af6:
	ldr	r3, [r6, #16]
	cmp	r3, #0
	bne.n	.L_08021afe
	b.n	.L_08021ca4
.L_08021afe:
	movs	r0, #2
	ldrsh	r3, [r6, r0]
	ldrh	r2, [r6, #2]
	cmp	r3, #0
	bgt.n	.L_08021bce
	ldrb	r3, [r6, #20]
	ldr	r1, [r6, #16]
	adds	r2, r3, #1
	strb	r2, [r6, #20]
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	ldrb	r0, [r1, r3]
	adds	r3, r2, #1
	strb	r3, [r6, #20]
	lsls	r2, r2, #24
	adds	r3, r0, #0
	lsrs	r2, r2, #24
	subs	r3, #239
	ldrb	r5, [r1, r2]
	cmp	r3, #16
	bhi.n	.L_08021bc2
	ldr	r2, [pc, #736]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08021bb0
	.4byte 0x08021b96
	.4byte 0x08021b8c
	.4byte 0x08021b9e
	.4byte 0x08021b9a
	.4byte 0x08021afe
	.4byte 0x08021ba6
	.4byte 0x08021afe
	.4byte 0x08021afe
	.4byte 0x08021afe
	.4byte 0x08021afe
	.4byte 0x08021afe
	.4byte 0x08021bc2
	.4byte 0x08021b84
	.4byte 0x08021b80
	.4byte 0x08021b74
	.2byte 0x1ba2
	.2byte 0x0802
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_08022acc
	strb	r5, [r7, #24]
	b.n	.L_08021afe
	strb	r5, [r6, #20]
	b.n	.L_08021afe
	adds	r0, r5, #0
	bl	sub_081c0010
	b.n	.L_08021afe
	ldrb	r3, [r6, #20]
	ldrb	r0, [r6, #23]
	adds	r3, #254
	strb	r3, [r6, #20]
	b.n	.L_08021bd6
	strb	r5, [r6, #4]
	b.n	.L_08021afe
	strb	r5, [r7, #22]
	b.n	.L_08021afe
	strb	r5, [r7, #23]
	b.n	.L_08021afe
	movs	r3, #255
	strb	r3, [r6, #23]
	ldrh	r3, [r6, #2]
	lsls	r2, r5, #4
	adds	r3, r3, r2
	strh	r3, [r6, #2]
	b.n	.L_08021afe
	movs	r3, #255
	strb	r3, [r6, #23]
	movs	r3, #0
	str	r3, [r6, #16]
	ldrb	r3, [r7, #27]
	movs	r0, #255
	adds	r3, #255
	strb	r3, [r7, #27]
	b.n	.L_08021bd6
.L_08021bc2:
	ldrh	r3, [r6, #2]
	lsls	r2, r5, #4
	adds	r3, r3, r2
	strb	r0, [r6, #23]
	strh	r3, [r6, #2]
	b.n	.L_08021afe
.L_08021bce:
	ldrb	r3, [r6, #21]
	ldrb	r0, [r6, #23]
	subs	r3, r2, r3
	strh	r3, [r6, #2]
.L_08021bd6:
	ldrb	r3, [r6, #4]
	subs	r3, #1
	cmp	r3, #21
	bhi.n	.L_08021c7e
	ldr	r2, [pc, #560]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08021c40
	.4byte 0x08021c48
	.4byte 0x08021c58
	.4byte 0x08021c7e
	.4byte 0x08021c62
	.4byte 0x08021c7e
	.4byte 0x08021c7e
	.4byte 0x08021c6c
	.4byte 0x08021c7e
	.4byte 0x08021c7e
	.4byte 0x08021c7e
	.4byte 0x08021c7e
	.4byte 0x08021c7e
	.4byte 0x08021c7e
	.4byte 0x08021c7e
	.4byte 0x08021c7e
	.4byte 0x08021c7e
	.4byte 0x08021c7e
	.4byte 0x08021c7e
	.4byte 0x08021c48
	.4byte 0x08021c7e
	.2byte 0x1c50
	.2byte 0x0802
	mov	r1, sl
	ldr	r3, [pc, #464]
	lsrs	r2, r1, #13
	b.n	.L_08021c76
	ldr	r3, [pc, #460]
	mov	r4, sl
	lsrs	r2, r4, #13
	b.n	.L_08021c76
	mov	r1, sl
	ldr	r3, [pc, #456]
	lsrs	r2, r1, #13
	b.n	.L_08021c76
	ldr	r3, [pc, #452]
	mov	r4, sl
	lsrs	r2, r4, #12
	movs	r1, #15
	b.n	.L_08021c78
	mov	r1, sl
	ldr	r3, [pc, #444]
	lsrs	r2, r1, #10
	movs	r1, #63
	b.n	.L_08021c78
	movs	r2, #128
	lsls	r2, r2, #5
	ldr	r3, [pc, #436]
	add	r2, sl
	lsrs	r2, r2, #13
.L_08021c76:
	movs	r1, #7
.L_08021c78:
	ands	r2, r1
	ldrb	r2, [r3, r2]
	b.n	.L_08021c80
.L_08021c7e:
	movs	r2, #0
.L_08021c80:
	movs	r3, #7
	ands	r3, r2
	adds	r0, r0, r3
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_08021c96
	lsrs	r3, r2, #7
	cmp	r3, #0
	beq.n	.L_08021c96
	movs	r4, #1
	str	r4, [sp, #24]
.L_08021c96:
	ldrb	r3, [r6, #22]
	cmp	r3, r0
	beq.n	.L_08021ca2
	movs	r3, #1
	strb	r0, [r6, #22]
	strb	r3, [r7, #25]
.L_08021ca2:
	ldrb	r2, [r7, #27]
.L_08021ca4:
	movs	r0, #1
	add	r8, r0
	cmp	r8, r2
	bge.n	.L_08021cae
	b.n	.L_08021ae8
.L_08021cae:
	ldrb	r2, [r7, #26]
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08021cd4
	mov	r2, sl
	ldr	r1, [pc, #348]
	lsrs	r3, r2, #13
	movs	r2, #7
	ands	r3, r2
	ldrb	r2, [r1, r3]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08021cce
	b.n	.L_08021fe0
.L_08021cce:
	movs	r3, #1
	str	r3, [sp, #24]
	b.n	.L_08021fe0
.L_08021cd4:
	ldrb	r3, [r7, #25]
	cmp	r3, #0
	bne.n	.L_08021cdc
	b.n	.L_08021fe0
.L_08021cdc:
	ldrb	r2, [r7, #20]
	ldrb	r3, [r7, #21]
	adds	r4, r3, #0
	muls	r4, r2
	adds	r0, r4, #0
	str	r4, [sp, #20]
	bl	sub_08014d78
	ldr	r3, [pc, #316]
	ldr	r1, [sp, #20]
	mov	r9, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x7efb
	movs	r0, #1
	negs	r0, r0
	subs	r3, #1
	mov	sl, r0
	mov	r8, r3
	cmp	r3, #0
	blt.n	.L_08021d98
	lsls	r3, r3, #2
	adds	r3, r3, r7
	adds	r3, #40
	movs	r1, #0
	mov	lr, r3
	mov	fp, r1
.L_08021d12:
	mov	r2, lr
	ldr	r6, [r2, #0]
	movs	r3, #4
	negs	r3, r3
	add	lr, r3
	cmp	r6, #0
	beq.n	.L_08021d8c
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq.n	.L_08021d8c
	ldrb	r3, [r6, #22]
	cmp	r3, #255
	beq.n	.L_08021d8c
	ldrb	r0, [r6, #6]
	cmp	r0, #3
	bhi.n	.L_08021d8c
	lsls	r0, r0, #8
	mov	r4, r8
	mov	r5, sl
	orrs	r0, r4
	cmp	r5, #0
	blt.n	.L_08021d72
	add	r6, sp, #28
	lsls	r2, r5, #1
	ldrh	r3, [r6, r2]
	cmp	r3, r0
	bls.n	.L_08021d7a
	mov	r1, fp
	strh	r3, [r6, r1]
	adds	r3, r2, r6
	mov	ip, r6
	adds	r4, r3, #2
	adds	r1, r2, #0
.L_08021d54:
	subs	r5, #1
	subs	r1, #2
	cmp	r5, #0
	blt.n	.L_08021d7e
	adds	r3, r1, #0
	mov	r2, ip
	ldrh	r2, [r3, r2]
	str	r2, [sp, #4]
	cmp	r2, r0
	bls.n	.L_08021d80
	add	r3, sp, #4
	ldrh	r3, [r3, #0]
	subs	r4, #2
	strh	r3, [r4, #0]
	b.n	.L_08021d54
.L_08021d72:
	mov	r4, sl
	add	r6, sp, #28
	lsls	r3, r4, #1
	b.n	.L_08021d80
.L_08021d7a:
	adds	r3, r2, #0
	b.n	.L_08021d80
.L_08021d7e:
	lsls	r3, r5, #1
.L_08021d80:
	adds	r3, #2
	strh	r0, [r6, r3]
	movs	r1, #1
	movs	r0, #2
	add	fp, r0
	add	sl, r1
.L_08021d8c:
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_08021d12
.L_08021d98:
	movs	r4, #1
	movs	r0, #0
	add	sl, r4
	mov	r8, r0
	cmp	r8, sl
	bge.n	.L_08021e6a
.L_08021da4:
	mov	r1, r8
	lsls	r3, r1, #1
	add	r2, sp, #36
	adds	r3, r3, r2
	subs	r3, #8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #2
	adds	r3, #40
	ldr	r6, [r7, r3]
	ldrb	r3, [r6, #7]
	cmp	r3, #1
	bne.n	.L_08021dcc
	ldrb	r3, [r6, #22]
	ldr	r2, [r6, #8]
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	mov	r1, r9
	bl	sub_0801587c
	b.n	sub_08021e62
.L_08021dcc:
	cmp	r3, #3
	bne.n	.L_08021e50
	ldrb	r3, [r6, #5]
	cmp	r3, #0
	beq.n	.L_08021e30
	movs	r0, #128
	lsls	r0, r0, #3
	bl	sub_08014d78
	ldrb	r3, [r6, #22]
	ldr	r2, [r6, #8]
	adds	r5, r0, #0
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	adds	r1, r5, #0
	bl	sub_08021940
	ldrb	r2, [r6, #5]
	mov	r1, r9
	ldr	r3, [sp, #12]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c28
	bl	sub_08013164
	b.n	sub_08021e62
	.4byte 0x000002c8
	.4byte 0x08020f90
	.4byte 0x08020da8
	.4byte 0x08021b30
	.4byte 0x08021be8
	.4byte 0x0802eac4
	.4byte 0x0802eadc
	.4byte 0x0802ead4
	.4byte 0x0802eae4
	.4byte 0x0802eb04
	.4byte 0x0802eb44
	.2byte 0x0258
	.2byte 0x0300
.L_08021e30:
	ldrb	r3, [r6, #22]
	ldr	r2, [r6, #8]
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	mov	r1, r9
	ldr	r3, [pc, #276]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2800
	beq.n	sub_08021e62
	mov	r1, r9
	movs	r2, #0
	ldr	r4, [sp, #12]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe008
.L_08021e50:
	ldrb	r3, [r6, #22]
	ldr	r2, [r6, #8]
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	mov	r1, r9
	ldrb	r2, [r6, #5]
	ldr	r3, [sp, #12]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2401
	add	r8, r4
	cmp	r8, sl
	blt.n	.L_08021da4
.L_08021e6a:
	ldrb	r2, [r7, #26]
	movs	r3, #14
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08021e76
	b.n	.L_08021f9a
.L_08021e76:
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08021eb8
	ldr	r5, [pc, #212]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #192]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldrb	r0, [r7, #20]
	ldr	r2, [sp, #16]
	ldrb	r1, [r7, #21]
	ldrb	r3, [r2, #6]
	ldrb	r2, [r2, #7]
	str	r2, [sp, #0]
	mov	r2, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c30
	bl	sub_08013164
	ldrb	r2, [r7, #26]
.L_08021eb8:
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08021f5c
	ldrb	r3, [r7, #17]
	ldrb	r5, [r7, #21]
	lsrs	r0, r3, #2
	ldrb	r6, [r7, #20]
	cmp	r0, r5
	ble.n	.L_08021ece
	adds	r0, r5, #0
.L_08021ece:
	adds	r3, r5, #0
	muls	r3, r6
	mov	r4, r9
	adds	r2, r4, r3
	adds	r3, r6, #0
	subs	r2, #4
	cmp	r3, #0
	beq.n	.L_08021ef2
	movs	r1, #0
	mov	r8, r3
.L_08021ee2:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r4, r8
	str	r1, [r2, #0]
	subs	r2, #4
	cmp	r4, #0
	bne.n	.L_08021ee2
.L_08021ef2:
	adds	r4, r0, #0
	muls	r4, r6
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L_08021efe
	adds	r3, r4, #3
.L_08021efe:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r1, r2, r3
	subs	r3, r5, r0
	subs	r3, #4
	muls	r3, r6
	cmp	r3, #0
	bge.n	.L_08021f10
	adds	r3, #3
.L_08021f10:
	asrs	r3, r3, #2
	cmp	r3, #0
	ble.n	.L_08021f2c
	mov	r8, r3
.L_08021f18:
	ldr	r3, [r1, #0]
	movs	r0, #1
	negs	r0, r0
	add	r8, r0
	str	r3, [r2, #0]
	mov	r3, r8
	subs	r1, #4
	subs	r2, #4
	cmp	r3, #0
	bne.n	.L_08021f18
.L_08021f2c:
	adds	r3, r4, #0
	cmp	r3, #0
	bge.n	.L_08021f34
	adds	r3, #3
.L_08021f34:
	movs	r4, #0
	asrs	r3, r3, #2
	mov	r8, r4
	cmp	r8, r3
	bge.n	.L_08021f9a
	movs	r1, #0
.L_08021f40:
	movs	r0, #1
	add	r8, r0
	str	r1, [r2, #0]
	subs	r2, #4
	cmp	r8, r3
	blt.n	.L_08021f40
	b.n	.L_08021f9a
	movs	r0, r0
	.4byte 0x03000694
	.4byte 0x000000f4
	.2byte 0x1824
	.2byte 0x0802
.L_08021f5c:
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08021f9a
	ldrb	r3, [r7, #17]
	ldrb	r2, [r7, #21]
	lsrs	r3, r3, #2
	ldrb	r0, [r7, #20]
	ldr	r4, [pc, #132]
	cmp	r3, r2
	bls.n	.L_08021f74
	adds	r3, r2, #0
.L_08021f74:
	adds	r1, r3, #0
	muls	r1, r0
	subs	r3, r2, r3
	muls	r3, r0
	mov	r0, r9
	adds	r2, r0, r3
	cmp	r1, #0
	beq.n	.L_08021f9a
	mov	r8, r1
.L_08021f86:
	ldrb	r3, [r2, #0]
	movs	r1, #1
	ldrb	r3, [r4, r3]
	negs	r1, r1
	add	r8, r1
	strb	r3, [r2, #0]
	mov	r3, r8
	adds	r2, #1
	cmp	r3, #0
	bne.n	.L_08021f86
.L_08021f9a:
	ldr	r1, [sp, #20]
	movs	r2, #0
	ldrb	r0, [r7, #16]
	bl	sub_080142d4
	ldr	r4, [pc, #80]
	adds	r5, r0, #0
	movs	r0, #192
	lsls	r3, r5, #5
	lsls	r0, r0, #18
	adds	r3, r3, r4
	ldrb	r1, [r7, #20]
	ldrb	r2, [r7, #21]
	ldr	r4, [r0, #84]
	mov	r0, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4b0c
	ldrh	r2, [r7, #8]
	ands	r5, r3
	ldr	r3, [pc, #56]
	ands	r3, r2
	orrs	r3, r5
	strh	r3, [r7, #8]
	movs	r3, #0
	strb	r3, [r7, #25]
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #20]
	ldrh	r3, [r0, #0]
	adds	r2, r0, #0
	adds	r3, r3, r1
	strh	r3, [r2, #0]
	mov	r0, r9
	bl	sub_08013164
.L_08021fe0:
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne.n	.L_08022000
	movs	r0, #80
	bl	sub_0801314c
	b.n	.L_08022000
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x080209a8
	.4byte 0x06010000
	.2byte 0xfc00
	.2byte 0xffff
.L_08022000:
	ldr	r0, [sp, #24]
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	mov	r8, r2
	ldrb	r2, [r6, #20]
	sub	sp, #4
	lsrs	r0, r2, #1
	ldrb	r2, [r6, #21]
	mov	r9, r3
	mov	ip, r2
	lsrs	r4, r2, #1
	movs	r2, #8
	mov	fp, r2
	movs	r2, #4
	ldr	r3, [sp, #40]
	str	r2, [sp, #0]
	movs	r2, #1
	mov	sl, r2
	ldmia	r3!, {r2}
	ldr	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	cmp	r2, r3
	bgt.n	.L_0802204e
	cmp	r5, r3
	ble.n	.L_0802205e
.L_0802204e:
	movs	r3, #3
	mov	sl, r3
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #16
	lsls	r0, r0, #1
	lsls	r4, r4, #1
	mov	fp, r2
.L_0802205e:
	asrs	r1, r1, #16
	subs	r7, r1, r0
	mov	r2, r8
	mov	r0, r9
	mov	lr, r1
	subs	r1, r0, r2
	movs	r2, #23
	ldrsb	r2, [r6, r2]
	mov	r0, ip
	lsrs	r3, r0, #1
	subs	r3, r3, r2
	muls	r3, r5
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
	asrs	r1, r1, #16
	subs	r1, r1, r4
	asrs	r3, r3, #16
	ldrb	r2, [r6, #5]
	subs	r5, r1, r3
	movs	r1, #4
	negs	r1, r1
	mov	ip, sl
	adds	r3, r1, #0
	mov	r0, ip
	ands	r3, r2
	ldr	r2, [pc, #64]
	orrs	r3, r0
	ldr	r4, [pc, #56]
	ldrh	r0, [r6, #6]
	strb	r3, [r6, #5]
	adds	r3, r2, #0
	ands	r7, r4
	ands	r3, r0
	orrs	r3, r7
	strh	r3, [r6, #6]
	mov	r0, fp
	mov	r3, lr
	subs	r7, r3, r0
	ldr	r3, [sp, #36]
	mov	r0, r9
	subs	r3, r0, r3
	ldr	r0, [sp, #0]
	strb	r5, [r6, #4]
	asrs	r3, r3, #16
	subs	r5, r3, r0
	adds	r3, r6, #0
	adds	r3, #28
	ldrb	r0, [r3, #5]
	ands	r7, r4
	ands	r1, r0
	mov	r0, ip
	orrs	r1, r0
	strb	r1, [r3, #5]
	ldrh	r1, [r3, #6]
	strb	r5, [r3, #4]
	b.n	.L_080220dc
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
.L_080220dc:
	ands	r2, r1
	orrs	r2, r7
	strh	r2, [r3, #6]
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	ldrb	r0, [r7, #20]
	sub	sp, #64
	lsrs	r0, r0, #1
	str	r0, [sp, #24]
	ldrb	r0, [r7, #21]
	lsrs	r0, r0, #1
	str	r0, [sp, #20]
	adds	r0, r7, #0
	ldmia	r2!, {r6}
	ldr	r5, [r2, #0]
	ldmia	r1!, {r2}
	str	r2, [sp, #12]
	ldmia	r1!, {r4}
	str	r4, [sp, #8]
	ldmia	r1!, {r2}
	str	r2, [sp, #4]
	ldr	r1, [r1, #0]
	mov	r8, r1
	adds	r1, r3, #0
	bl	.L_08021a84
	mov	ip, r0
	cmp	r0, #0
	bne.n	.L_0802214c
	movs	r3, #128
	lsls	r3, r3, #9
	cmp	r6, r3
	bne.n	.L_0802214c
	cmp	r5, r6
	bne.n	.L_0802214c
	ldrh	r2, [r7, #18]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_0802214e
	movs	r4, #0
	str	r4, [sp, #16]
	str	r4, [sp, #28]
	b.n	.L_08022196
.L_0802214c:
	ldrh	r2, [r7, #18]
.L_0802214e:
	movs	r1, #1
	str	r1, [sp, #16]
	add	r0, sp, #32
	ldr	r3, [r0, #4]
	ldr	r4, [pc, #168]
	ands	r3, r4
	orrs	r3, r2
	str	r3, [r0, #4]
	ldr	r1, [sp, #32]
	lsls	r3, r6, #8
	lsrs	r3, r3, #16
	ands	r1, r4
	movs	r2, #255
	orrs	r1, r3
	lsls	r2, r2, #8
	lsls	r3, r5, #8
	adds	r2, #255
	lsrs	r3, r3, #16
	ands	r1, r2
	lsls	r3, r3, #16
	orrs	r1, r3
	mov	r2, ip
	str	r1, [sp, #32]
	cmp	r2, #0
	beq.n	.L_08022190
	ldrh	r3, [r0, #0]
	adds	r2, r4, #0
	negs	r3, r3
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	ands	r2, r1
	orrs	r2, r3
	str	r2, [sp, #32]
.L_08022190:
	bl	sub_0801401c
	str	r0, [sp, #28]
.L_08022196:
	movs	r3, #128
	lsls	r3, r3, #9
	cmp	r6, r3
	bgt.n	.L_080221a2
	cmp	r5, r3
	ble.n	.L_080221b2
.L_080221a2:
	ldr	r4, [sp, #24]
	ldr	r1, [sp, #20]
	movs	r3, #3
	lsls	r4, r4, #1
	lsls	r1, r1, #1
	str	r3, [sp, #16]
	str	r4, [sp, #24]
	str	r1, [sp, #20]
.L_080221b2:
	ldr	r2, [sp, #4]
	mov	r4, r8
	subs	r3, r2, r4
	ldrb	r2, [r7, #26]
	asrs	r3, r3, #16
	subs	r4, r3, #4
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08022208
	cmp	r4, #159
	bgt.n	.L_08022208
	adds	r0, r7, #0
	adds	r0, #28
	ldrb	r2, [r0, #5]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r0, #5]
	ldr	r1, [sp, #12]
	ldr	r3, [pc, #32]
	asrs	r2, r1, #16
	subs	r2, #8
	ldrh	r1, [r0, #6]
	ands	r2, r3
	ldr	r3, [pc, #28]
	strb	r4, [r0, #4]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #6]
	ldr	r1, [sp, #4]
	add	r1, r8
	asrs	r1, r1, #16
	adds	r1, #60
	bl	sub_080140d8
	b.n	.L_08022208
	.4byte 0x000001ff
	.4byte 0xffff0000
	.2byte 0xfe00
	.2byte 0xffff
.L_08022208:
	.2byte 0x2e00
	bne.n	.L_0802220e
	b.n	.L_0802230a
.L_0802220e:
	cmp	r5, #0
	bne.n	.L_08022214
	b.n	.L_0802230a
.L_08022214:
	ldrh	r3, [r7, #18]
	cmp	r3, #0
	bne.n	.L_08022234
	movs	r3, #22
	ldrsb	r3, [r7, r3]
	adds	r2, r6, #0
	muls	r2, r3
	ldrb	r3, [r7, #21]
	mov	fp, r2
	movs	r2, #23
	ldrsb	r2, [r7, r2]
	lsrs	r3, r3, #1
	subs	r3, r3, r2
	adds	r0, r5, #0
	muls	r0, r3
	b.n	.L_08022288
.L_08022234:
	ldrh	r0, [r7, #18]
	bl	sub_08002096
	str	r0, [sp, #0]
	ldrh	r0, [r7, #18]
	bl	sub_08002090
	movs	r3, #22
	ldrsb	r3, [r7, r3]
	movs	r2, #23
	ldrsb	r2, [r7, r2]
	adds	r4, r6, #0
	muls	r4, r3
	ldrb	r3, [r7, #21]
	mov	r9, r4
	lsrs	r3, r3, #1
	subs	r3, r3, r2
	adds	r1, r5, #0
	muls	r1, r3
	ldr	r5, [pc, #156]
	mov	r8, r1
	mov	r1, r9
	mov	sl, r0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4641
	adds	r6, r0, #0
	ldr	r0, [sp, #0]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4641
	adds	r6, r6, r0
	mov	r0, sl
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x46b3
	mov	r1, r9
	adds	r6, r0, #0
	ldr	r0, [sp, #0]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1a30
.L_08022288:
	ldr	r3, [sp, #12]
	ldr	r4, [sp, #24]
	movs	r1, #255
	asrs	r2, r3, #16
	lsls	r1, r1, #8
	subs	r2, r2, r4
	adds	r1, #255
	mov	r4, fp
	adds	r3, r4, r1
	asrs	r3, r3, #16
	adds	r6, r2, r3
	ldr	r4, [sp, #8]
	ldr	r2, [sp, #4]
	adds	r1, r0, r1
	subs	r3, r2, r4
	ldr	r2, [sp, #20]
	asrs	r3, r3, #16
	subs	r3, r3, r2
	asrs	r1, r1, #16
	subs	r4, r3, r1
	cmp	r6, #239
	bgt.n	.L_0802230a
	cmp	r4, #159
	bgt.n	.L_0802230a
	ldr	r3, [pc, #56]
	adds	r0, r7, #0
	ldrh	r2, [r0, #6]
	ands	r6, r3
	ldr	r3, [pc, #56]
	strb	r4, [r0, #4]
	ands	r3, r2
	orrs	r3, r6
	strh	r3, [r0, #6]
	ldrb	r2, [r0, #5]
	ldr	r4, [sp, #16]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	orrs	r3, r4
	strb	r3, [r0, #5]
	ldr	r1, [sp, #28]
	movs	r3, #31
	ands	r1, r3
	str	r1, [sp, #28]
	movs	r3, #63
	ldrb	r2, [r0, #7]
	negs	r3, r3
	lsls	r1, r1, #1
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #7]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #4]
	b.n	.L_08022300
	.4byte 0x000001ff
	.4byte 0x0300021c
	.2byte 0xfe00
	.2byte 0xffff
.L_08022300:
	adds	r1, r2, r3
	asrs	r1, r1, #16
	adds	r1, #62
	bl	sub_080140d8
.L_0802230a:
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	ldrb	r0, [r7, #20]
	sub	sp, #64
	lsrs	r0, r0, #1
	str	r0, [sp, #24]
	movs	r4, #4
	ldrb	r0, [r7, #21]
	mov	fp, r4
	lsrs	r0, r0, #1
	str	r0, [sp, #20]
	movs	r0, #8
	str	r0, [sp, #16]
	ldmia	r1!, {r0}
	ldmia	r2!, {r6}
	ldr	r5, [r2, #0]
	str	r0, [sp, #8]
	adds	r0, r7, #0
	ldmia	r1!, {r2}
	str	r2, [sp, #4]
	ldmia	r1!, {r4}
	str	r4, [sp, #0]
	ldr	r1, [r1, #0]
	mov	r8, r1
	adds	r1, r3, #0
	bl	.L_08021a84
	mov	ip, r0
	cmp	r0, #0
	bne.n	.L_0802237c
	movs	r0, #128
	lsls	r0, r0, #9
	cmp	r6, r0
	bne.n	.L_0802237c
	cmp	r5, r6
	bne.n	.L_0802237c
	ldrh	r2, [r7, #18]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_0802237e
	movs	r1, #0
	str	r1, [sp, #12]
	str	r1, [sp, #28]
	b.n	.L_080223c6
.L_0802237c:
	ldrh	r2, [r7, #18]
.L_0802237e:
	movs	r3, #1
	str	r3, [sp, #12]
	add	r0, sp, #32
	ldr	r3, [r0, #4]
	ldr	r4, [pc, #176]
	ands	r3, r4
	orrs	r3, r2
	str	r3, [r0, #4]
	ldr	r1, [sp, #32]
	lsls	r3, r6, #8
	lsrs	r3, r3, #16
	ands	r1, r4
	movs	r2, #255
	orrs	r1, r3
	lsls	r2, r2, #8
	lsls	r3, r5, #8
	adds	r2, #255
	lsrs	r3, r3, #16
	ands	r1, r2
	lsls	r3, r3, #16
	orrs	r1, r3
	mov	r2, ip
	str	r1, [sp, #32]
	cmp	r2, #0
	beq.n	.L_080223c0
	ldrh	r3, [r0, #0]
	adds	r2, r4, #0
	negs	r3, r3
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	ands	r2, r1
	orrs	r2, r3
	str	r2, [sp, #32]
.L_080223c0:
	bl	sub_0801401c
	str	r0, [sp, #28]
.L_080223c6:
	movs	r3, #128
	lsls	r3, r3, #9
	cmp	r6, r3
	bgt.n	.L_080223d2
	cmp	r5, r3
	ble.n	.L_080223ea
.L_080223d2:
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #20]
	movs	r3, #3
	lsls	r4, r4, #1
	lsls	r0, r0, #1
	movs	r1, #16
	str	r3, [sp, #12]
	str	r4, [sp, #24]
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	movs	r2, #8
	mov	fp, r2
.L_080223ea:
	ldr	r4, [sp, #0]
	mov	r0, r8
	subs	r3, r4, r0
	ldrb	r2, [r7, #26]
	asrs	r3, r3, #16
	mov	r1, fp
	subs	r4, r3, r1
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08022440
	cmp	r4, #159
	bgt.n	.L_08022440
	adds	r0, r7, #0
	adds	r0, #28
	ldrb	r2, [r0, #5]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r0, #5]
	ldr	r3, [sp, #8]
	ldr	r1, [sp, #16]
	asrs	r2, r3, #16
	ldr	r3, [pc, #24]
	subs	r2, r2, r1
	ands	r2, r3
	ldrh	r1, [r0, #6]
	ldr	r3, [pc, #24]
	strb	r4, [r0, #4]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #6]
	ldr	r1, [sp, #96]
	bl	sub_08014128
	b.n	.L_08022440
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xffff0000
	.2byte 0xfe00
	.2byte 0xffff
.L_08022440:
	.2byte 0x2e00
	bne.n	.L_08022446
	b.n	.L_0802253c
.L_08022446:
	cmp	r5, #0
	bne.n	.L_0802244c
	b.n	.L_0802253c
.L_0802244c:
	ldrh	r3, [r7, #18]
	cmp	r3, #0
	bne.n	.L_0802246c
	movs	r3, #22
	ldrsb	r3, [r7, r3]
	adds	r2, r6, #0
	muls	r2, r3
	ldrb	r3, [r7, #21]
	mov	r9, r2
	movs	r2, #23
	ldrsb	r2, [r7, r2]
	lsrs	r3, r3, #1
	subs	r3, r3, r2
	adds	r0, r5, #0
	muls	r0, r3
	b.n	.L_080224c2
.L_0802246c:
	ldrh	r0, [r7, #18]
	bl	sub_08002096
	mov	sl, r0
	ldrh	r0, [r7, #18]
	bl	sub_08002090
	movs	r3, #22
	ldrsb	r3, [r7, r3]
	mov	r8, r0
	adds	r4, r6, #0
	muls	r4, r3
	str	r4, [sp, #16]
	adds	r1, r4, #0
	ldrb	r3, [r7, #21]
	movs	r2, #23
	ldrsb	r2, [r7, r2]
	lsrs	r3, r3, #1
	subs	r3, r3, r2
	adds	r0, r5, #0
	muls	r0, r3
	ldr	r5, [pc, #152]
	mov	fp, r0
	mov	r0, r8
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4659
	adds	r6, r0, #0
	mov	r0, sl
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4659
	adds	r6, r6, r0
	mov	r0, r8
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x46b1
	ldr	r1, [sp, #16]
	adds	r6, r0, #0
	mov	r0, sl
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1a30
.L_080224c2:
	ldr	r1, [sp, #8]
	ldr	r3, [sp, #24]
	asrs	r2, r1, #16
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	mov	r4, r9
	subs	r2, r2, r3
	adds	r3, r4, r1
	asrs	r3, r3, #16
	adds	r6, r2, r3
	ldr	r4, [sp, #4]
	ldr	r2, [sp, #0]
	adds	r1, r0, r1
	subs	r3, r2, r4
	ldr	r2, [sp, #20]
	asrs	r3, r3, #16
	subs	r3, r3, r2
	asrs	r1, r1, #16
	subs	r4, r3, r1
	cmp	r6, #239
	bgt.n	.L_0802253c
	cmp	r4, #159
	bgt.n	.L_0802253c
	ldr	r3, [pc, #56]
	adds	r0, r7, #0
	ldrh	r2, [r0, #6]
	ands	r6, r3
	ldr	r3, [pc, #56]
	strb	r4, [r0, #4]
	ands	r3, r2
	orrs	r3, r6
	strh	r3, [r0, #6]
	ldrb	r2, [r0, #5]
	ldr	r4, [sp, #12]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	orrs	r3, r4
	strb	r3, [r0, #5]
	ldr	r1, [sp, #28]
	movs	r3, #31
	ands	r1, r3
	str	r1, [sp, #28]
	movs	r3, #63
	ldrb	r2, [r0, #7]
	negs	r3, r3
	lsls	r1, r1, #1
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #7]
	ldr	r1, [sp, #96]
	b.n	.L_08022538
	.4byte 0x000001ff
	.4byte 0x0300021c
	.2byte 0xfe00
	.2byte 0xffff
.L_08022538:
	bl	sub_08014128
.L_0802253c:
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
