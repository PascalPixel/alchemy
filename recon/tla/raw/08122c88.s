.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_08002096, 0x08002096
	.set sub_08013c58, 0x08013c58
	.set sub_08013d0c, 0x08013d0c
	.set sub_08014128, 0x08014128
	.set sub_080143f8, 0x080143f8
	.set sub_080145a8, 0x080145a8
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020030, 0x08020030
	.set sub_08020090, 0x08020090
	.set sub_080200f0, 0x080200f0
	.set sub_08038048, 0x08038048
	.set sub_08038118, 0x08038118
	.set sub_08038120, 0x08038120
	.set sub_08038130, 0x08038130
	.set sub_080381d0, 0x080381d0
	.set sub_080382a0, 0x080382a0
	.set sub_0811a484, 0x0811a484
	.set sub_0811b4d8, 0x0811b4d8
	.set sub_0811b6a4, 0x0811b6a4
	.set sub_0811bc64, 0x0811bc64
	.set sub_0811bdb0, 0x0811bdb0
	.set sub_0811be3c, 0x0811be3c
	.set sub_0811f030, 0x0811f030
	.set sub_0811f3b8, 0x0811f3b8
	.set sub_0811fe3c, 0x0811fe3c
	.set sub_08120178, 0x08120178
	.set sub_081201b4, 0x081201b4
	.set sub_08120454, 0x08120454
	.set sub_081280a0, 0x081280a0
	.set sub_0812824c, 0x0812824c
	.set sub_081c0010, 0x081c0010
	.global Func_08122c88
	.thumb_func
Func_08122c88:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #40
	adds	r2, r3, r1
	movs	r4, #0
	adds	r1, #8
	str	r4, [r2, #0]
	adds	r2, r3, r1
	adds	r1, #4
	str	r4, [r2, #0]
	adds	r2, r3, r1
	str	r0, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #44
	adds	r1, r3, r2
	movs	r2, #2
	str	r2, [r1, #0]
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r1, #125
	adds	r3, r3, r1
	movs	r1, #144
	strb	r4, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl	sub_080145a8
	pop	{pc}
	movs	r0, r0
	.2byte 0x2d11
	.2byte 0x0812
.L_08122cd0:
	push	{lr}
	mov	ip, r3
	mov	r3, r9
	push	{r3}
	mov	r3, ip
	sub	sp, #4
	mov	r3, r9
	str	r3, [sp, #0]
	adds	r2, r0, #0
	ldrb	r3, [r2, #16]
	ldr	r1, [pc, #28]
	lsls	r3, r3, #2
	adds	r3, r3, r1
	ldrh	r0, [r3, #2]
	ldr	r3, [pc, #24]
	ldrb	r1, [r2, #20]
	adds	r0, r0, r3
	ldrb	r3, [r2, #21]
	muls	r1, r3
	ldr	r3, [pc, #20]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb001
	pop	{r3}
	mov	r9, r3
	pop	{pc}
	.4byte 0x020036e0
	.4byte 0x06010000
	.2byte 0x0258
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
	ldr	r3, [r3, #36]
	sub	sp, #32
	movs	r0, #192
	movs	r1, #128
	str	r3, [sp, #8]
	lsls	r0, r0, #3
	lsls	r1, r1, #4
	adds	r0, #228
	adds	r1, #44
	adds	r7, r3, r0
	adds	r3, r3, r1
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08122d40
	b.n	.L_08123494
.L_08122d40:
	movs	r2, #164
	lsls	r2, r2, #1
	adds	r5, r7, r2
	ldr	r3, [r5, #0]
	cmp	r3, #4
	bne.n	.L_08122d4e
	b.n	.L_08123494
.L_08122d4e:
	cmp	r3, #1
	bne.n	.L_08122da8
	ldr	r4, [sp, #8]
	movs	r0, #192
	movs	r3, #160
	lsls	r0, r0, #3
	lsls	r3, r3, #1
	adds	r0, #125
	adds	r6, r7, r3
	adds	r3, r4, r0
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r3, [r6, #0]
	cmp	r3, r2
	bge.n	.L_08122da2
	movs	r1, #162
	lsls	r1, r1, #1
	movs	r4, #166
	movs	r2, #0
	adds	r3, r7, r1
	lsls	r4, r4, #1
	movs	r0, #168
	str	r2, [r3, #0]
	lsls	r0, r0, #1
	adds	r3, r7, r4
	str	r2, [r3, #0]
	adds	r3, r7, r0
	str	r2, [r3, #0]
	ldr	r1, [sp, #8]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #124
	adds	r0, r1, r2
	ldr	r1, [r6, #0]
	bl	sub_08120454
	ldr	r3, [r6, #0]
	adds	r3, #1
	str	r3, [r6, #0]
	movs	r3, #2
	str	r3, [r5, #0]
	b.n	.L_08122d40
.L_08122da2:
	movs	r3, #4
	str	r3, [r5, #0]
	b.n	.L_08122d40
.L_08122da8:
	cmp	r3, #2
	beq.n	.L_08122dae
	b.n	.L_0812302c
.L_08122dae:
	movs	r4, #166
	movs	r0, #162
	lsls	r4, r4, #1
	lsls	r0, r0, #1
	adds	r3, r7, r4
	adds	r2, r7, r0
	ldr	r5, [r3, #0]
	ldr	r3, [r2, #0]
	cmp	r5, r3
	blt.n	.L_08122dc4
	b.n	.L_08123018
.L_08122dc4:
	adds	r6, r5, #0
.L_08122dc6:
	movs	r1, #168
	lsls	r1, r1, #1
	adds	r2, r7, r1
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08122dd8
	subs	r3, #1
	str	r3, [r2, #0]
	b.n	.L_08123494
.L_08122dd8:
	ldrb	r3, [r7, r6]
	cmp	r3, #15
	bls.n	.L_08122de0
	b.n	.L_08122ff0
.L_08122de0:
	ldr	r2, [pc, #648]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08122e42
	.4byte 0x08122e50
	.4byte 0x08122e5e
	.4byte 0x08122e74
	.4byte 0x08122e98
	.4byte 0x08122eb8
	.4byte 0x08122e8a
	.4byte 0x08122ed2
	.4byte 0x08122ee4
	.4byte 0x08122f26
	.4byte 0x08122fa6
	.4byte 0x08122fb6
	.4byte 0x08122ed8
	.4byte 0x08122e34
	.4byte 0x08122e28
	.2byte 0x2fe6
	.2byte 0x0812
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	bl	sub_081c0010
	b.n	.L_08122ff0
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r1, [r7, r3]
	adds	r0, r7, #0
	bl	sub_081201b4
	b.n	.L_08122ff0
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	movs	r1, #1
	bl	sub_08038120
	b.n	.L_08122ff0
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	movs	r1, #5
	bl	sub_08038120
	b.n	.L_08122ff0
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	movs	r1, #2
	bl	sub_08038120
	b.n	.L_08122ff0
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	ands	r0, r3
	movs	r1, #4
	bl	sub_08038120
	b.n	.L_08122ff0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #148
	ldr	r2, [r3, #0]
	movs	r3, #1
	str	r3, [r2, #8]
	b.n	.L_08122ff0
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	cmp	r0, #0
	blt.n	.L_08122ea6
	bl	sub_080381d0
.L_08122ea6:
	movs	r3, #164
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r2, [pc, #444]
	movs	r3, #0
	str	r3, [r2, #28]
	b.n	.L_08122ff0
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	cmp	r0, #0
	blt.n	.L_08122ec6
	bl	sub_080381d0
.L_08122ec6:
	movs	r4, #164
	lsls	r4, r4, #1
	adds	r2, r7, r4
	movs	r3, #13
	str	r3, [r2, #0]
	b.n	.L_08122ff0
	bl	sub_08038118
	b.n	.L_08122ff0
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	bl	sub_08120178
	b.n	.L_08122ff0
	movs	r0, #180
	lsls	r0, r0, #1
	adds	r3, r7, r0
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	cmp	r0, #0
	ble.n	.L_08122ef6
	bl	sub_081c0010
.L_08122ef6:
	movs	r3, #178
	lsls	r3, r3, #1
	adds	r2, r7, r3
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	str	r0, [r2, #0]
	bl	sub_0811be3c
	movs	r1, #5
	ldr	r0, [r0, #0]
	bl	sub_08020090
	movs	r4, #164
	lsls	r4, r4, #1
	movs	r0, #168
	adds	r2, r7, r4
	movs	r3, #10
	lsls	r0, r0, #1
	str	r3, [r2, #0]
	adds	r2, r7, r0
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_08122ff0
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	movs	r1, #178
	lsls	r1, r1, #1
	adds	r5, r7, r1
	str	r0, [r5, #0]
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r1, [r3, #0]
	bl	sub_0812824c
	ldr	r0, [r5, #0]
	bl	sub_0811fe3c
	ldr	r0, [r5, #0]
	bl	sub_08016ca4
	movs	r5, #0
	adds	r6, r0, #0
	b.n	.L_08122f6e
.L_08122f52:
	movs	r4, #149
	lsls	r4, r4, #1
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_08122f66
	movs	r1, #4
	bl	sub_08020030
	b.n	.L_08122f6c
.L_08122f66:
	movs	r1, #5
	bl	sub_08020030
.L_08122f6c:
	adds	r5, #1
.L_08122f6e:
	movs	r0, #178
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	bl	sub_0811be3c
	adds	r1, r5, #0
	ldr	r0, [r0, #0]
	bl	sub_0811bdb0
	cmp	r0, #0
	bne.n	.L_08122f52
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_08122ff0
	movs	r2, #164
	lsls	r2, r2, #1
	movs	r4, #168
	adds	r3, r7, r2
	lsls	r4, r4, #1
	movs	r2, #11
	str	r2, [r3, #0]
	adds	r3, r7, r4
	str	r0, [r3, #0]
	b.n	.L_08122ff0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	sub_08038130
	b.n	.L_08122ff0
	lsls	r5, r6, #2
	adds	r5, #64
	ldr	r0, [r7, r5]
	bl	sub_0811be3c
	adds	r1, r0, #0
	ldr	r0, [r7, r5]
	bl	sub_0811b4d8
	ldr	r0, [r7, r5]
	bl	sub_0811be3c
	adds	r6, r0, #0
	ldr	r0, [r7, r5]
	bl	sub_0811a484
	adds	r1, r0, #0
	ldr	r0, [r6, #0]
	bl	sub_0811f030
	ldr	r0, [r7, r5]
	bl	sub_0811b6a4
	b.n	.L_08122ff0
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	bl	sub_0811f3b8
.L_08122ff0:
	movs	r0, #166
	lsls	r0, r0, #1
	adds	r2, r7, r0
	ldr	r3, [r2, #0]
	movs	r1, #162
	adds	r5, r3, #1
	str	r5, [r2, #0]
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	cmp	r5, r3
	bge.n	.L_08123018
	movs	r2, #164
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	adds	r6, r5, #0
	cmp	r3, #2
	bne.n	.L_08123018
	b.n	.L_08122dc6
.L_08123018:
	movs	r3, #164
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [r2, #0]
	cmp	r3, #2
	beq.n	.L_08123026
	b.n	.L_08122d40
.L_08123026:
	movs	r3, #1
	str	r3, [r2, #0]
	b.n	.L_08122d40
.L_0812302c:
	cmp	r3, #3
	beq.n	.L_08123034
	cmp	r3, #13
	bne.n	.L_08123078
.L_08123034:
	bl	sub_08038048
	cmp	r0, #0
	bne.n	.L_0812303e
	b.n	.L_08123494
.L_0812303e:
	ldr	r3, [r5, #0]
	cmp	r3, #13
	bne.n	.L_08123050
	movs	r4, #168
	movs	r3, #2
	lsls	r4, r4, #1
	str	r3, [r5, #0]
	adds	r2, r7, r4
	b.n	.L_081231c0
.L_08123050:
	movs	r0, #176
	movs	r3, #5
	lsls	r0, r0, #1
	str	r3, [r5, #0]
	adds	r2, r7, r0
	subs	r3, #6
	str	r3, [r2, #0]
	ldr	r3, [pc, #20]
	movs	r1, #168
	ldr	r3, [r3, #0]
	lsls	r1, r1, #1
	adds	r2, r7, r1
	str	r3, [r2, #0]
	b.n	.L_08122d40
	.4byte 0x08122de8
	.4byte 0x03001150
	.2byte 0x1100
	.2byte 0x0300
.L_08123078:
	cmp	r3, #5
	beq.n	.L_0812307e
	b.n	.L_081231d0
.L_0812307e:
	ldr	r4, [pc, #176]
	movs	r2, #170
	ldr	r3, [r4, #0]
	lsls	r2, r2, #1
	adds	r2, r2, r7
	ldr	r1, [pc, #168]
	mov	r9, r2
	lsrs	r3, r3, #2
	movs	r2, #7
	ands	r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r1
	mov	sl, r3
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #148
	ldr	r3, [r3, #0]
	movs	r2, #176
	ldr	r0, [r3, #0]
	ldr	r3, [r3, #4]
	lsls	r2, r2, #1
	str	r3, [sp, #4]
	adds	r6, r7, r2
	ldr	r3, [r6, #0]
	movs	r4, #1
	movs	r1, #0
	negs	r4, r4
	mov	fp, r0
	mov	r8, r1
	cmp	r3, r4
	bne.n	.L_081230c2
	ldr	r0, [sp, #8]
	ldr	r3, [r0, #84]
	str	r3, [r6, #0]
.L_081230c2:
	movs	r5, #128
	lsls	r5, r5, #19
	adds	r5, #74
	bl	sub_08038118
	adds	r0, r5, #0
	movs	r1, #4
	bl	sub_08013d0c
	adds	r0, r5, #0
	movs	r1, #16
	bl	sub_08013c58
	movs	r3, #160
	mov	r1, r9
	lsls	r3, r3, #8
	mov	r2, r8
	str	r3, [r1, #4]
	str	r2, [r1, #8]
	mov	r1, sl
	ldr	r0, [r6, #0]
	bl	sub_080143f8
	ldr	r3, [pc, #52]
	mov	r4, r9
	ands	r0, r3
	mov	r3, r9
	ldrh	r2, [r3, #8]
	ldr	r3, [pc, #60]
	mov	r1, fp
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r4, #8]
	ldr	r4, [sp, #4]
	movs	r0, #12
	ldrsh	r2, [r1, r0]
	ldrh	r3, [r4, #4]
	lsls	r2, r2, #3
	lsrs	r3, r3, #8
	adds	r2, r2, r3
	adds	r2, #4
	ldr	r3, [pc, #20]
	mov	r8, r2
	mov	r0, r8
	mov	r1, r9
	ands	r0, r3
	ldrh	r2, [r1, #6]
	ldr	r3, [pc, #24]
	ands	r3, r2
	b.n	.L_08123140
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x000001ff
	.4byte 0x0300122c
	.4byte 0x0812996c
	.4byte 0xfffffc00
	.2byte 0xfe00
	.2byte 0xffff
.L_08123140:
	orrs	r3, r0
	mov	r2, r9
	strh	r3, [r2, #6]
	ldr	r3, [pc, #816]
	ldr	r0, [r3, #0]
	lsls	r0, r0, #12
	bl	sub_08002096
	cmp	r0, #0
	bge.n	.L_0812315c
	movs	r4, #254
	lsls	r4, r4, #7
	adds	r4, #255
	adds	r0, r0, r4
.L_0812315c:
	mov	r1, fp
	asrs	r2, r0, #15
	movs	r0, #14
	ldrsh	r3, [r1, r0]
	ldr	r4, [sp, #4]
	lsls	r3, r3, #3
	adds	r2, r2, r3
	ldrh	r3, [r4, #6]
	ldr	r1, [pc, #780]
	lsrs	r3, r3, #8
	adds	r3, r3, r2
	adds	r3, #6
	mov	r0, r9
	strb	r3, [r0, #4]
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081231aa
	ldr	r3, [r1, #28]
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_081231aa
	ldr	r3, [pc, #748]
	movs	r4, #168
	lsls	r4, r4, #1
	adds	r2, r7, r4
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	subs	r3, r3, r2
	cmp	r3, #10
	bls.n	.L_081231c6
	ldr	r3, [r1, #0]
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_081231c6
.L_081231aa:
	movs	r0, #111
	bl	sub_081c0010
	movs	r0, #164
	lsls	r0, r0, #1
	movs	r1, #168
	adds	r2, r7, r0
	movs	r3, #2
	lsls	r1, r1, #1
	str	r3, [r2, #0]
	adds	r2, r7, r1
.L_081231c0:
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_08122d40
.L_081231c6:
	mov	r0, r9
	movs	r1, #240
	bl	sub_08014128
	b.n	.L_08123494
.L_081231d0:
	cmp	r3, #10
	bne.n	.L_08123276
	ldr	r3, [pc, #684]
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_081231ea
	movs	r2, #168
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r2, [r3, #0]
	movs	r1, #1
	orrs	r2, r1
	str	r2, [r3, #0]
.L_081231ea:
	movs	r4, #168
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08123254
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0812322a
	add	r0, sp, #28
	movs	r2, #178
	lsls	r2, r2, #1
	mov	r9, r0
	mov	r1, r9
	adds	r5, r7, r2
	movs	r3, #255
	strh	r3, [r1, #0]
	ldr	r0, [r5, #0]
	bl	sub_0811be3c
	adds	r6, r0, #0
	ldr	r0, [r5, #0]
	bl	sub_0811a484
	adds	r1, r0, #0
	ldr	r0, [r6, #0]
	bl	sub_0811f030
	b.n	.L_0812324e
.L_0812322a:
	movs	r3, #28
	movs	r4, #178
	add	r3, sp
	lsls	r4, r4, #1
	mov	r9, r3
	adds	r3, r7, r4
	ldr	r0, [r3, #0]
	mov	r1, r9
	movs	r3, #255
	mov	r2, r9
	strh	r0, [r1, #0]
	strh	r3, [r2, #2]
	bl	sub_0811be3c
	movs	r1, #7
	ldr	r0, [r0, #0]
	bl	sub_0811f030
.L_0812324e:
	mov	r0, r9
	bl	sub_080382a0
.L_08123254:
	movs	r3, #168
	lsls	r3, r3, #1
	adds	r1, r7, r3
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	cmp	r3, #7
	bgt.n	.L_08123266
	b.n	.L_08123494
.L_08123266:
	movs	r4, #164
	lsls	r4, r4, #1
	adds	r3, r7, r4
	movs	r2, #2
	str	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r1, #0]
	b.n	.L_08122d40
.L_08123276:
	cmp	r3, #11
	beq.n	.L_0812327c
	b.n	.L_08122d40
.L_0812327c:
	movs	r0, #168
	lsls	r0, r0, #1
	adds	r5, r7, r0
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_08123290
	movs	r1, #128
	lsls	r1, r1, #3
	cmp	r3, r1
	blt.n	.L_0812338c
.L_08123290:
	movs	r2, #6
	mov	sl, r2
	cmp	r3, #0
	bne.n	.L_081232da
	movs	r4, #182
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_081232da
	movs	r0, #178
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	bl	sub_08016ca4
	movs	r1, #165
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrh	r0, [r3, #0]
	bl	sub_081280a0
	cmp	r0, #0
	blt.n	.L_081232ce
	subs	r0, #1
	cmp	r0, #0
	bge.n	.L_081232c8
	movs	r0, #0
.L_081232c8:
	adds	r0, #146
	bl	sub_081c0010
.L_081232ce:
	movs	r3, #168
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #128
	lsls	r3, r3, #3
	str	r3, [r2, #0]
.L_081232da:
	movs	r4, #168
	lsls	r4, r4, #1
	adds	r2, r7, r4
	movs	r0, #128
	ldr	r3, [r2, #0]
	lsls	r0, r0, #3
	adds	r0, #29
	cmp	r3, r0
	ble.n	.L_081232f0
	movs	r3, #0
	str	r3, [r2, #0]
.L_081232f0:
	cmp	r3, #0
	bne.n	.L_08123316
	movs	r1, #178
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	sub_08016ca4
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrh	r0, [r3, #0]
	bl	sub_081280a0
	cmp	r0, #0
	blt.n	.L_08123316
	adds	r0, #146
	bl	sub_081c0010
.L_08123316:
	movs	r4, #168
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #3
	cmp	r3, r0
	blt.n	.L_0812333e
	ldr	r1, [pc, #352]
	adds	r0, r3, r1
	cmp	r0, #0
	bge.n	.L_08123332
	ldr	r2, [pc, #348]
	adds	r0, r3, r2
.L_08123332:
	asrs	r0, r0, #3
	movs	r1, #5
	bl	sub_08002064
	adds	r0, #1
	mov	sl, r0
.L_0812333e:
	mov	r3, sl
	cmp	r3, #6
	beq.n	.L_08123356
	movs	r4, #168
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	movs	r2, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08123356
	b.n	.L_08123468
.L_08123356:
	movs	r0, #255
	movs	r6, #0
	add	r5, sp, #12
	mov	r8, r0
	b.n	.L_08123372
.L_08123360:
	ldr	r2, [r0, #40]
	mov	r4, r8
	ldrb	r3, [r2, #22]
	mov	r1, sl
	orrs	r3, r4
	stmia	r5!, {r0}
	strb	r1, [r2, #5]
	strb	r3, [r2, #22]
	adds	r6, #1
.L_08123372:
	movs	r0, #178
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	bl	sub_0811be3c
	adds	r1, r6, #0
	ldr	r0, [r0, #0]
	bl	sub_0811bdb0
	cmp	r0, #0
	bne.n	.L_08123360
	b.n	.L_08123468
.L_0812338c:
	cmp	r3, #4
	bne.n	.L_081233a0
	movs	r2, #178
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	sub_0811f3b8
	ldr	r3, [r5, #0]
	b.n	.L_08123490
.L_081233a0:
	cmp	r3, #4
	ble.n	.L_08123490
	movs	r4, #178
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r0, [r3, #0]
	bl	sub_0811be3c
	adds	r6, r0, #0
	adds	r2, r6, #0
	adds	r2, #42
	movs	r3, #1
	strb	r3, [r2, #0]
	add	r5, sp, #12
	movs	r2, #0
	b.n	.L_081233c4
.L_081233c0:
	stmia	r5!, {r0}
	adds	r2, #1
.L_081233c4:
	adds	r1, r2, #0
	ldr	r0, [r6, #0]
	str	r2, [sp, #0]
	bl	sub_0811bdb0
	ldr	r2, [sp, #0]
	cmp	r0, #0
	bne.n	.L_081233c0
	movs	r0, #168
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	movs	r1, #20
	lsls	r3, r3, #2
	negs	r1, r1
	adds	r1, r1, r3
	mov	r8, r1
	cmp	r1, #127
	ble.n	.L_08123426
	cmp	r2, #0
	ble.n	.L_08123404
	add	r6, sp, #12
	adds	r5, r2, #0
.L_081233f2:
	add	r2, sp, #32
	ldmia	r6!, {r0}
	mov	r9, r2
	movs	r1, #0
	subs	r5, #1
	bl	.L_08122cd0
	cmp	r5, #0
	bne.n	.L_081233f2
.L_08123404:
	movs	r4, #178
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r0, [r3, #0]
	bl	sub_0811bc64
	movs	r0, #164
	lsls	r0, r0, #1
	movs	r1, #168
	adds	r2, r7, r0
	movs	r3, #2
	lsls	r1, r1, #1
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_08123494
.L_08123426:
	cmp	r2, #0
	ble.n	.L_08123468
	movs	r4, #19
	movs	r0, #18
	negs	r4, r4
	negs	r0, r0
	adds	r4, r4, r3
	adds	r0, r0, r3
	subs	r3, #17
	mov	fp, r4
	mov	r9, r0
	mov	sl, r3
	adds	r6, r2, #0
	add	r5, sp, #12
.L_08123442:
	ldr	r0, [r5, #0]
	mov	r1, r8
	bl	sub_080200f0
	ldr	r0, [r5, #0]
	mov	r1, fp
	bl	sub_080200f0
	ldr	r0, [r5, #0]
	mov	r1, r9
	bl	sub_080200f0
	subs	r6, #1
	ldmia	r5!, {r0}
	mov	r1, sl
	bl	sub_080200f0
	cmp	r6, #0
	bne.n	.L_08123442
.L_08123468:
	movs	r1, #168
	lsls	r1, r1, #1
	adds	r2, r7, r1
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L_08123494
	movs	r0, r0
	.4byte 0x0300122c
	.4byte 0x03001150
	.4byte 0x03001100
	.4byte 0x030011d8
	.4byte 0xfffffc00
	.2byte 0xfc07
	.2byte 0xffff
.L_08123490:
	adds	r3, #1
	str	r3, [r5, #0]
.L_08123494:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
