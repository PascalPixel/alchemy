.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020030, 0x08020030
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_08038060, 0x08038060
	.set sub_080380c8, 0x080380c8
	.set sub_080380d8, 0x080380d8
	.set sub_080380f8, 0x080380f8
	.set sub_08038120, 0x08038120
	.set sub_08038140, 0x08038140
	.set sub_08038370, 0x08038370
	.set sub_080ad008, 0x080ad008
	.set sub_080ad010, 0x080ad010
	.set sub_080ad0d0, 0x080ad0d0
	.set sub_080ad1d8, 0x080ad1d8
	.set sub_080c8088, 0x080c8088
	.set sub_080f8058, 0x080f8058
	.set sub_080f8060, 0x080f8060
	.set sub_080f8068, 0x080f8068
	.set sub_08108148, 0x08108148
	.set sub_0810824c, 0x0810824c
	.set sub_08108660, 0x08108660
	.set sub_08108690, 0x08108690
	.set sub_08108aa8, 0x08108aa8
	.set sub_08108af0, 0x08108af0
	.set sub_08109188, 0x08109188
	.set sub_0810a864, 0x0810a864
	.set sub_0810a898, 0x0810a898
	.set sub_0810a8ec, 0x0810a8ec
	.set sub_0810a9ac, 0x0810a9ac
	.set sub_0810a9fc, 0x0810a9fc
	.set sub_0810b04c, 0x0810b04c
	.set sub_0810b1b4, 0x0810b1b4
	.set sub_081c0010, 0x081c0010
	.global Func_0810aa80
	.thumb_func
Func_0810aa80:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #0
	sub	sp, #8
	mov	r8, r1
	adds	r5, r0, #0
	mov	sl, r1
	bl	sub_08108148
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r2, #129
	lsls	r2, r2, #3
	adds	r2, #255
	adds	r3, r7, r2
	mov	r1, r8
	strb	r1, [r3, #0]
	adds	r0, r5, #0
	bl	sub_080c8088
	ldr	r3, [r0, #80]
	movs	r1, #128
	ldr	r3, [r3, #40]
	lsls	r1, r1, #3
	ldrh	r2, [r3, #0]
	adds	r1, #250
	adds	r3, r7, r1
	strh	r2, [r3, #0]
	movs	r1, #0
	ldrh	r0, [r3, #0]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080380f8
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_0810ab0e
	movs	r0, #5
	negs	r0, r0
	movs	r5, #2
	movs	r1, #0
	movs	r2, #5
	movs	r3, #5
	str	r5, [sp, #0]
	bl	sub_08038010
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_0810ab0e
	movs	r1, #0
	movs	r2, #5
	movs	r3, #5
	movs	r0, #0
	str	r5, [sp, #0]
	bl	sub_08038010
	movs	r3, #4
	negs	r3, r3
	mov	r8, r0
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	mov	r3, r8
	bl	sub_080380d8
.L_0810ab0e:
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #236
	adds	r3, r7, r2
	movs	r1, #128
	ldrh	r0, [r3, #0]
	movs	r6, #0
	lsls	r1, r1, #23
	mov	r2, r8
	movs	r3, #0
	str	r6, [sp, #0]
	bl	sub_080380c8
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r5, #5]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #220
	strb	r6, [r5, #4]
	movs	r1, #32
	adds	r6, r7, r3
	negs	r1, r1
	adds	r0, r6, #0
	movs	r2, #112
	bl	sub_08108aa8
	str	r5, [r6, #0]
	ldr	r0, [pc, #152]
	bl	sub_0810a9ac
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #11
	movs	r2, #12
	movs	r3, #4
	movs	r0, #16
	bl	sub_08038010
	str	r0, [r7, #12]
	bl	sub_08109188
	movs	r1, #129
	lsls	r1, r1, #3
	adds	r1, #255
	adds	r6, r7, r1
	b.n	.L_0810aba4
.L_0810ab6c:
	ldr	r5, [pc, #116]
	adds	r0, r5, #0
	bl	sub_0810a9ac
	bl	sub_0810a8ec
	cmp	r0, #0
	bne.n	.L_0810ab84
	adds	r0, r5, #1
	bl	sub_0810a9ac
	b.n	.L_0810ab88
.L_0810ab84:
	bl	.L_0810abf0
.L_0810ab88:
	movs	r2, #128
	lsls	r2, r2, #3
	movs	r3, #0
	adds	r2, #220
	movs	r1, #32
	adds	r0, r7, r2
	strb	r3, [r6, #0]
	negs	r1, r1
	movs	r2, #112
	bl	sub_08108aa8
	ldr	r0, [pc, #72]
	bl	sub_0810a9ac
.L_0810aba4:
	mov	r0, sl
	bl	sub_08038370
	movs	r1, #1
	mov	sl, r0
	mov	r3, sl
	negs	r1, r1
	strb	r3, [r6, #0]
	cmp	sl, r1
	bne.n	.L_0810ab6c
	ldr	r0, [pc, #48]
	bl	sub_0810a9ac
	ldr	r0, [r7, #12]
	movs	r1, #2
	bl	sub_08038018
	mov	r0, r8
	movs	r1, #2
	bl	sub_08038018
	bl	sub_0810824c
	movs	r0, #0
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000012d2
	.4byte 0x000012d5
	.4byte 0x000012d3
	.2byte 0x12d4
	.2byte 0x0000
.L_0810abf0:
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
	sub	sp, #16
	ldr	r7, [r3, #0]
	movs	r6, #0
	movs	r0, #0
	movs	r2, #129
	str	r0, [sp, #12]
	str	r6, [sp, #4]
	lsls	r2, r2, #3
	adds	r2, #255
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r1, #1
	ldr	r0, [pc, #624]
	mov	r9, r1
	mov	sl, r3
	bl	sub_0810a9ac
	movs	r5, #2
	movs	r1, #12
	movs	r2, #13
	movs	r3, #3
	movs	r0, #1
	str	r5, [sp, #0]
	bl	sub_08038010
	mov	fp, r0
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r0, #220
	adds	r3, r7, r0
	ldr	r2, [r3, #0]
	movs	r1, #160
	lsls	r1, r1, #3
	movs	r3, #4
	adds	r1, #5
	strb	r3, [r2, #5]
	adds	r3, r7, r1
	mov	r2, r9
	strb	r2, [r3, #0]
	mov	r0, fp
	movs	r1, #2
	movs	r2, #0
	bl	sub_080f8060
	movs	r0, #1
	movs	r1, #16
	movs	r2, #23
	movs	r3, #3
	str	r5, [sp, #0]
	bl	sub_08038010
	mov	r8, r6
	str	r0, [sp, #8]
	str	r6, [sp, #12]
	b.n	.L_0810ac7c
.L_0810ac76:
	ldr	r3, [sp, #12]
	adds	r3, #1
	str	r3, [sp, #12]
.L_0810ac7c:
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #4
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [sp, #12]
	cmp	r2, r3
	bge.n	.L_0810aca8
	movs	r1, #153
	lsls	r1, r1, #3
	lsls	r3, r2, #1
	adds	r3, r3, r1
	adds	r2, r7, #2
	ldrsh	r0, [r2, r3]
	mov	r1, sl
	mov	r8, r0
	bl	sub_0810a898
	cmp	r0, #0
	beq.n	.L_0810ac76
.L_0810aca8:
	movs	r2, #2
	mov	r9, r2
.L_0810acac:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq.n	.L_0810acf8
	movs	r0, #0
	str	r0, [sp, #4]
	ldr	r0, [pc, #476]
	bl	sub_0810a9ac
	ldr	r2, [sp, #4]
	movs	r1, #2
	mov	r9, r1
	str	r2, [sp, #12]
	b.n	.L_0810accc
.L_0810acc6:
	ldr	r3, [sp, #12]
	adds	r3, #1
	str	r3, [sp, #12]
.L_0810accc:
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #4
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [sp, #12]
	cmp	r2, r3
	bge.n	.L_0810acf8
	movs	r1, #153
	lsls	r1, r1, #3
	lsls	r3, r2, #1
	adds	r3, r3, r1
	adds	r2, r7, #2
	ldrsh	r0, [r2, r3]
	mov	r1, sl
	mov	r8, r0
	bl	sub_0810a898
	cmp	r0, #0
	beq.n	.L_0810acc6
.L_0810acf8:
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L_0810ad68
	ldr	r4, [sp, #12]
	movs	r3, #153
	lsls	r3, r3, #3
	lsls	r2, r4, #1
	adds	r2, r2, r3
	adds	r3, r7, #2
	ldrsh	r0, [r3, r2]
	adds	r3, r4, #0
	mov	r8, r0
	cmp	r4, #0
	bge.n	.L_0810ad16
	adds	r3, r4, #3
.L_0810ad16:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r4, r3
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #3
	mov	r0, fp
	movs	r2, #0
	subs	r1, #12
	bl	sub_08108af0
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #5
	adds	r2, r7, r3
	mov	r0, r9
	movs	r3, #3
	strb	r3, [r2, #0]
	cmp	r0, #2
	bne.n	.L_0810ad54
	ldr	r0, [sp, #12]
	cmp	r0, #0
	bge.n	.L_0810ad46
	adds	r0, #3
.L_0810ad46:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	bl	sub_080f8058
	movs	r0, #1
	bl	sub_08013560
.L_0810ad54:
	ldr	r1, [sp, #12]
	mov	r0, fp
	bl	.L_0810af24
	mov	r1, r8
	ldr	r0, [sp, #8]
	bl	sub_0810b04c
	movs	r1, #0
	mov	r9, r1
.L_0810ad68:
	ldr	r1, [pc, #300]
	movs	r3, #1
	ldr	r2, [r1, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0810ae3a
	movs	r0, #1
	bl	sub_08013560
	mov	r1, sl
	mov	r0, r8
	bl	sub_0810a864
	mov	r1, sl
	adds	r5, r0, #0
	mov	r0, r8
	bl	sub_0810a898
	cmp	r0, #0
	bne.n	.L_0810ad98
	movs	r0, #113
	bl	sub_081c0010
	b.n	.L_0810acac
.L_0810ad98:
	mov	r0, fp
	bl	sub_08038060
	mov	r0, r8
	movs	r1, #1
	bl	sub_08038120
	adds	r0, r5, #0
	movs	r1, #5
	bl	sub_08038120
	ldr	r6, [pc, #236]
	adds	r0, r6, #0
	bl	sub_0810a9ac
	movs	r0, #0
	bl	sub_08108660
	cmp	r0, #0
	beq.n	.L_0810adcc
	adds	r0, r6, #2
	bl	sub_0810a9fc
	movs	r2, #1
	str	r2, [sp, #4]
	b.n	.L_0810acac
.L_0810adcc:
	ldr	r3, [pc, #208]
	ldr	r3, [r3, #16]
	cmp	r5, r3
	bls.n	.L_0810ade6
	movs	r0, #113
	bl	sub_081c0010
	adds	r0, r6, #1
	bl	sub_0810a9fc
	movs	r3, #1
	str	r3, [sp, #4]
	b.n	.L_0810acac
.L_0810ade6:
	movs	r1, #1
	mov	r0, r8
	bl	sub_08038120
	adds	r0, r6, #3
	bl	sub_0810a9ac
	bl	sub_08038140
	mov	r0, r8
	mov	r1, sl
	bl	.L_0810aea4
	ldr	r3, [sp, #12]
	adds	r0, r3, #0
	cmp	r3, #0
	bge.n	.L_0810ae0a
	adds	r0, r3, #3
.L_0810ae0a:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	subs	r0, r3, r0
	bl	sub_0810b1b4
	negs	r0, r5
	bl	sub_080ad1d8
	bl	sub_08109188
	mov	r0, r8
	movs	r1, #1
	bl	sub_08038120
	adds	r0, r6, #4
	bl	sub_0810a9ac
	bl	sub_0810a8ec
	cmp	r0, #0
	beq.n	.L_0810ae6a
	movs	r0, #1
	str	r0, [sp, #4]
	b.n	.L_0810acac
.L_0810ae3a:
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810ae4c
	movs	r0, #113
	bl	sub_081c0010
	b.n	.L_0810ae6a
.L_0810ae4c:
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #4
	adds	r3, r7, r1
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	add	r0, sp, #12
	movs	r2, #4
	bl	sub_08108690
	mov	r9, r0
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_0810acac
.L_0810ae6a:
	bl	sub_080f8068
	movs	r1, #2
	ldr	r0, [sp, #8]
	bl	sub_08038018
	mov	r0, fp
	movs	r1, #2
	bl	sub_08038018
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #0
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x000012d7
	.4byte 0x03001150
	.4byte 0x000012d8
	.2byte 0x0240
	.2byte 0x0200
.L_0810aea4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	adds	r5, r1, #0
	bl	sub_08016ca4
	adds	r2, r0, #0
	cmp	r5, #0
	bne.n	.L_0810aec4
	ldrh	r3, [r2, #52]
	adds	r0, r7, #0
	strh	r3, [r2, #56]
	bl	sub_080ad0d0
	b.n	.L_0810af1e
.L_0810aec4:
	cmp	r5, #1
	bne.n	.L_0810aece
	movs	r3, #50
	adds	r3, #255
	b.n	.L_0810aed6
.L_0810aece:
	cmp	r5, #2
	bne.n	.L_0810aede
	movs	r3, #160
	lsls	r3, r3, #1
.L_0810aed6:
	adds	r2, r2, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	b.n	.L_0810af1e
.L_0810aede:
	cmp	r5, #3
	bne.n	.L_0810af1e
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r5, r2, #0
	mov	r8, r3
	movs	r6, #14
	adds	r5, #216
.L_0810aeee:
	ldrh	r2, [r5, #0]
	mov	r3, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810af16
	ldrh	r0, [r5, #0]
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810af16
	ldrh	r2, [r5, #0]
	mov	r3, r8
	eors	r3, r2
	strh	r3, [r5, #0]
	adds	r0, r7, #0
	bl	sub_080ad008
.L_0810af16:
	subs	r6, #1
	adds	r5, #2
	cmp	r6, #0
	bge.n	.L_0810aeee
.L_0810af1e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_0810af24:
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
	sub	sp, #20
	mov	r8, r3
	movs	r3, #129
	lsls	r3, r3, #3
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r9, r1
	str	r3, [sp, #16]
	mov	r3, r9
	adds	r6, r0, #0
	cmp	r3, #0
	bge.n	.L_0810af5a
	adds	r3, #3
.L_0810af5a:
	asrs	r3, r3, #2
	str	r3, [sp, #12]
	lsls	r3, r3, #2
	mov	fp, r3
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #4
	add	r3, r8
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	cmp	r6, #0
	beq.n	.L_0810b03c
	adds	r0, r6, #0
	bl	sub_08038060
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L_0810afa4
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #238
	add	r3, r8
	ldrh	r0, [r3, #0]
	movs	r3, #12
	negs	r3, r3
	movs	r1, #128
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #88
	lsls	r1, r1, #23
	bl	sub_080380c8
	movs	r2, #0
	movs	r3, #17
	strb	r2, [r0, #4]
	strb	r3, [r0, #5]
	strh	r2, [r0, #12]
.L_0810afa4:
	mov	r3, fp
	adds	r3, #4
	cmp	r3, r5
	bge.n	.L_0810afcc
	movs	r3, #158
	lsls	r3, r3, #3
	add	r3, r8
	movs	r1, #128
	ldrh	r0, [r3, #0]
	movs	r5, #0
	movs	r3, #88
	lsls	r1, r1, #23
	adds	r2, r6, #0
	str	r5, [sp, #0]
	bl	sub_080380c8
	movs	r3, #15
	strb	r5, [r0, #4]
	strb	r3, [r0, #5]
	strh	r5, [r0, #12]
.L_0810afcc:
	movs	r2, #0
	mov	sl, r2
	mov	r1, r8
	ldr	r2, [sp, #12]
	adds	r1, #248
	mov	r3, r8
	adds	r3, #2
	str	r1, [sp, #4]
	movs	r1, #153
	str	r3, [sp, #8]
	movs	r7, #156
	lsls	r3, r2, #3
	lsls	r1, r1, #3
	lsls	r7, r7, #1
	adds	r6, r3, r1
.L_0810afea:
	ldr	r2, [sp, #8]
	mov	r3, fp
	ldrsh	r5, [r2, r6]
	ldr	r1, [sp, #4]
	add	r3, sl
	ldmia	r1!, {r0}
	adds	r2, r1, #0
	str	r2, [sp, #4]
	cmp	r0, #0
	beq.n	.L_0810b02e
	cmp	r3, r9
	bne.n	.L_0810b00a
	movs	r1, #30
	bl	sub_08020030
	b.n	.L_0810b010
.L_0810b00a:
	movs	r1, #1
	bl	sub_08020030
.L_0810b010:
	movs	r3, #128
	lsls	r3, r3, #9
	mov	r2, r8
	str	r3, [r7, r2]
	adds	r0, r5, #0
	ldr	r1, [sp, #16]
	bl	sub_0810a898
	cmp	r0, #0
	bne.n	.L_0810b02e
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	mov	r1, r8
	str	r3, [r7, r1]
.L_0810b02e:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r7, #4
	adds	r6, #2
	cmp	r3, #3
	ble.n	.L_0810afea
.L_0810b03c:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
