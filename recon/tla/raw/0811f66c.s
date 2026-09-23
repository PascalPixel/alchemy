.syntax unified
	.thumb
	.set sub_08014878, 0x08014878
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad008, 0x080ad008
	.set sub_080ad140, 0x080ad140
	.set sub_080ad280, 0x080ad280
	.set sub_0811b4d8, 0x0811b4d8
	.set sub_0811be3c, 0x0811be3c
	.global Func_0811f66c
	.thumb_func
Func_0811f66c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #92
	str	r0, [sp, #28]
	movs	r3, #192
	movs	r0, #0
	lsls	r3, r3, #18
	adds	r7, r1, #0
	ldr	r1, [r3, #36]
	str	r0, [sp, #20]
	mov	r8, r0
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_0811f76e
	movs	r2, #0
	cmp	r3, #2
	beq.n	.L_0811f69c
	cmp	r3, #4
	bne.n	.L_0811f69e
.L_0811f69c:
	movs	r2, #1
.L_0811f69e:
	ldr	r3, [sp, #28]
	cmp	r3, #7
	bls.n	.L_0811f6aa
	cmp	r2, #0
	beq.n	.L_0811f6ae
	b.n	.L_0811f710
.L_0811f6aa:
	cmp	r2, #0
	beq.n	.L_0811f710
.L_0811f6ae:
	movs	r4, #0
	str	r4, [sp, #24]
	movs	r3, #88
	ldrsh	r3, [r1, r3]
	cmp	r3, #255
	beq.n	.L_0811f76e
	mov	r0, r8
	lsls	r3, r0, #1
	add	r2, sp, #92
	adds	r3, r3, r2
	mov	r4, r8
	adds	r0, r3, #0
	lsls	r3, r4, #2
	adds	r3, r3, r2
	ldr	r5, [pc, #32]
	adds	r4, r3, #0
	adds	r1, #88
	subs	r0, #12
	subs	r4, #36
.L_0811f6d4:
	movs	r6, #0
	ldrsh	r2, [r1, r6]
	cmp	r2, #254
	beq.n	.L_0811f6fe
	ldrb	r3, [r7, #0]
	cmp	r3, #4
	bne.n	.L_0811f6f0
	ldr	r3, [sp, #28]
	cmp	r2, r3
	bne.n	.L_0811f6fe
	b.n	.L_0811f6f0
	movs	r0, r0
	.2byte 0x0100
	.2byte 0x0000
.L_0811f6f0:
	stmia	r4!, {r2}
	ldr	r3, [sp, #24]
	movs	r6, #1
	orrs	r3, r5
	strh	r3, [r0, #0]
	add	r8, r6
	adds	r0, #2
.L_0811f6fe:
	ldr	r2, [sp, #24]
	adds	r1, #2
	adds	r2, #1
	str	r2, [sp, #24]
	movs	r6, #0
	ldrsh	r3, [r1, r6]
	cmp	r3, #255
	bne.n	.L_0811f6d4
	b.n	.L_0811f76e
.L_0811f710:
	movs	r0, #0
	str	r0, [sp, #24]
	adds	r2, r1, #2
	movs	r3, #100
	ldrsh	r3, [r2, r3]
	mov	ip, r2
	cmp	r3, #255
	beq.n	.L_0811f76e
	mov	r4, r8
	lsls	r3, r4, #1
	add	r6, sp, #92
	adds	r3, r3, r6
	adds	r1, r3, #0
	lsls	r3, r4, #2
	adds	r3, r3, r6
	ldr	r5, [pc, #28]
	adds	r4, r3, #0
	movs	r0, #100
	subs	r1, #12
	subs	r4, #36
.L_0811f738:
	ldrsh	r2, [r2, r0]
	cmp	r2, #254
	beq.n	.L_0811f75e
	ldrb	r3, [r7, #0]
	cmp	r3, #4
	bne.n	.L_0811f750
	ldr	r6, [sp, #28]
	cmp	r2, r6
	bne.n	.L_0811f75e
	b.n	.L_0811f750
	.2byte 0x0180
	.2byte 0x0000
.L_0811f750:
	stmia	r4!, {r2}
	ldr	r3, [sp, #24]
	movs	r2, #1
	orrs	r3, r5
	strh	r3, [r1, #0]
	add	r8, r2
	adds	r1, #2
.L_0811f75e:
	ldr	r3, [sp, #24]
	adds	r0, #2
	adds	r3, #1
	str	r3, [sp, #24]
	mov	r2, ip
	ldrsh	r3, [r2, r0]
	cmp	r3, #255
	bne.n	.L_0811f738
.L_0811f76e:
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_0811f77a
	movs	r0, #2
	negs	r0, r0
	b.n	.L_0811fe28
.L_0811f77a:
	movs	r1, #0
	str	r1, [sp, #24]
	cmp	r1, r8
	blt.n	.L_0811f784
	b.n	.L_0811fc6c
.L_0811f784:
	add	r2, sp, #56
	mov	sl, r2
.L_0811f788:
	ldr	r3, [sp, #24]
	mov	r4, sl
	lsls	r3, r3, #2
	ldr	r0, [r4, r3]
	mov	r9, r3
	bl	sub_08016ca4
	adds	r5, r0, #0
	ldrb	r0, [r7, #3]
	movs	r6, #0
	cmp	r0, #88
	bls.n	.L_0811f7a2
	b.n	.L_0811fb9c
.L_0811f7a2:
	ldr	r2, [pc, #804]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0811fb9e
	.4byte 0x0811fb9e
	.4byte 0x0811fb9e
	.4byte 0x0811f9c6
	.4byte 0x0811f9da
	.4byte 0x0811fb82
	.4byte 0x0811f910
	.4byte 0x0811f910
	.4byte 0x0811f936
	.4byte 0x0811f936
	.4byte 0x0811f94e
	.4byte 0x0811f94e
	.4byte 0x0811f96a
	.4byte 0x0811f96a
	.4byte 0x0811f98a
	.4byte 0x0811f98a
	.4byte 0x0811f9a6
	.4byte 0x0811f9a6
	.4byte 0x0811fb44
	.4byte 0x0811fb52
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb64
	.4byte 0x0811fb6c
	.4byte 0x0811fb9c
	.4byte 0x0811fb74
	.4byte 0x0811fb9c
	.4byte 0x0811fb3c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fa46
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb90
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb82
	.4byte 0x0811fb82
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fa32
	.4byte 0x0811fa32
	.4byte 0x0811fb9c
	.4byte 0x0811facc
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fa32
	.4byte 0x0811fa32
	.4byte 0x0811fb9c
	.4byte 0x0811fb82
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.4byte 0x0811fb64
	.4byte 0x0811fb9c
	.4byte 0x0811fb9c
	.2byte 0xfb90
	.2byte 0x0811
	movs	r1, #52
	adds	r1, #255
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #1
	cmp	r3, #4
	bgt.n	.L_0811f924
.L_0811f922:
	movs	r6, #1
.L_0811f924:
	movs	r2, #153
	lsls	r2, r2, #1
	adds	r3, r5, r2
.L_0811f92a:
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_0811f932
	b.n	.L_0811fb9e
.L_0811f932:
	adds	r6, #1
	b.n	.L_0811fb9e
	movs	r4, #52
	adds	r4, #255
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r1, #4
	subs	r3, #1
	negs	r1, r1
	cmp	r3, r1
	blt.n	.L_0811f924
	b.n	.L_0811f922
	movs	r4, #54
	adds	r4, #255
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #1
	cmp	r3, #4
	bgt.n	.L_0811f962
	movs	r6, #1
.L_0811f962:
	movs	r1, #154
	lsls	r1, r1, #1
	adds	r3, r5, r1
	b.n	.L_0811f92a
	movs	r2, #54
	adds	r2, #255
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r4, #4
	subs	r3, #1
	negs	r4, r4
	cmp	r3, r4
	blt.n	.L_0811f982
	movs	r6, #1
.L_0811f982:
	movs	r1, #154
	lsls	r1, r1, #1
	adds	r3, r5, r1
	b.n	.L_0811f92a
	movs	r2, #56
	adds	r2, #255
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #1
	cmp	r3, #4
	bgt.n	.L_0811f99e
	movs	r6, #1
.L_0811f99e:
	movs	r4, #155
	lsls	r4, r4, #1
	adds	r3, r5, r4
	b.n	.L_0811f92a
	movs	r1, #56
	adds	r1, #255
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #4
	subs	r3, #1
	negs	r2, r2
	cmp	r3, r2
	blt.n	.L_0811f9be
	movs	r6, #1
.L_0811f9be:
	movs	r4, #155
	lsls	r4, r4, #1
	adds	r3, r5, r4
	b.n	.L_0811f92a
	movs	r1, #50
	adds	r1, #255
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0811f9d8
	b.n	.L_0811fb9e
.L_0811f9d8:
	b.n	.L_0811fb9c
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811f9e8
	movs	r6, #1
.L_0811f9e8:
	movs	r4, #58
	adds	r4, #255
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811f9f6
	adds	r6, #1
.L_0811f9f6:
	movs	r1, #157
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811fa04
	adds	r6, #1
.L_0811fa04:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811fa12
	adds	r6, #1
.L_0811fa12:
	movs	r4, #62
	adds	r4, #255
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811fa20
	adds	r6, #1
.L_0811fa20:
	movs	r1, #66
	adds	r1, #255
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
.L_0811fa28:
	cmp	r3, #0
	bne.n	.L_0811fa2e
	b.n	.L_0811fb9e
.L_0811fa2e:
	adds	r6, #1
	b.n	.L_0811fb9e
	movs	r3, #56
	ldrsh	r2, [r5, r3]
	movs	r4, #52
	ldrsh	r3, [r5, r4]
	ldrh	r1, [r5, #56]
	cmp	r2, r3
	blt.n	.L_0811fa42
	b.n	.L_0811fba0
.L_0811fa42:
	movs	r6, #1
	b.n	.L_0811fba0
	movs	r1, #52
	adds	r1, #255
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_0811fa58
	movs	r6, #1
.L_0811fa58:
	movs	r2, #54
	adds	r2, #255
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_0811fa6a
	adds	r6, #1
.L_0811fa6a:
	movs	r4, #56
	adds	r4, #255
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_0811fa7c
	adds	r6, #1
.L_0811fa7c:
	movs	r1, #150
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_0811fa8e
	adds	r6, #1
.L_0811fa8e:
	movs	r2, #46
	adds	r2, #255
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_0811faa0
	adds	r6, #1
.L_0811faa0:
	movs	r4, #151
	lsls	r4, r4, #1
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_0811fab2
	adds	r6, #1
.L_0811fab2:
	movs	r1, #48
	adds	r1, #255
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_0811fb9e
	adds	r6, #1
	b.n	.L_0811fb9e
	movs	r0, r0
	.2byte 0xf7ac
	.2byte 0x0811
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811fada
	movs	r6, #1
.L_0811fada:
	movs	r4, #58
	adds	r4, #255
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811fae8
	adds	r6, #1
.L_0811fae8:
	movs	r1, #157
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811faf6
	adds	r6, #1
.L_0811faf6:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811fb04
	adds	r6, #1
.L_0811fb04:
	movs	r4, #62
	adds	r4, #255
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811fb12
	adds	r6, #1
.L_0811fb12:
	movs	r1, #66
	adds	r1, #255
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811fb20
	adds	r6, #1
.L_0811fb20:
	movs	r2, #160
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811fb2e
	adds	r6, #1
.L_0811fb2e:
	movs	r4, #50
	adds	r4, #255
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	b.n	.L_0811fa28
	movs	r1, #66
	adds	r1, #255
	adds	r3, r5, r1
	b.n	.L_0811fb7a
	movs	r2, #50
	adds	r2, #255
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	b.n	.L_0811fb7c
	movs	r4, #50
	adds	r4, #255
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	bgt.n	.L_0811fb9e
	b.n	.L_0811fb9c
	movs	r1, #60
	adds	r1, #255
	adds	r3, r5, r1
	b.n	.L_0811fb7a
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r5, r2
	b.n	.L_0811fb7a
	movs	r4, #160
	lsls	r4, r4, #1
	adds	r3, r5, r4
.L_0811fb7a:
	ldrb	r3, [r3, #0]
.L_0811fb7c:
	cmp	r3, #0
	bne.n	.L_0811fb9e
	b.n	.L_0811fb9c
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	ldrh	r1, [r5, #56]
	cmp	r3, #0
	bne.n	.L_0811fbb0
	movs	r6, #100
	b.n	.L_0811fba0
	movs	r4, #44
	adds	r4, #255
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bhi.n	.L_0811fb9e
.L_0811fb9c:
	movs	r6, #1
.L_0811fb9e:
	ldrh	r1, [r5, #56]
.L_0811fba0:
	lsls	r3, r1, #16
	cmp	r3, #0
	bne.n	.L_0811fbb0
	bl	sub_080ad280
	cmp	r0, #0
	bne.n	.L_0811fbb0
	movs	r6, #0
.L_0811fbb0:
	cmp	r6, #0
	bne.n	.L_0811fc40
	ldrb	r2, [r7, #1]
	movs	r3, #15
	ands	r3, r2
	subs	r3, #1
	cmp	r3, #10
	bhi.n	.L_0811fc3c
	ldr	r2, [pc, #628]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0811fc0c
	.4byte 0x0811fc2c
	.4byte 0x0811fbf4
	.4byte 0x0811fbf4
	.4byte 0x0811fbfa
	.4byte 0x0811fbfa
	.4byte 0x0811fc3a
	.4byte 0x0811fbfa
	.4byte 0x0811fc2c
	.4byte 0x0811fc06
	.2byte 0xfc1e
	.2byte 0x0811
	movs	r0, #56
	ldrsh	r3, [r5, r0]
	b.n	.L_0811fc36
	ldrh	r3, [r7, #10]
	cmp	r3, #0
	beq.n	.L_0811fc3c
	movs	r1, #56
	ldrsh	r3, [r5, r1]
	b.n	.L_0811fc36
	movs	r2, #58
	ldrsh	r3, [r5, r2]
	b.n	.L_0811fc36
	movs	r3, #56
	ldrsh	r2, [r5, r3]
	cmp	r2, #0
	beq.n	.L_0811fc3c
	movs	r4, #52
	ldrsh	r3, [r5, r4]
	cmp	r2, r3
	bge.n	.L_0811fc3c
	b.n	.L_0811fc3a
	movs	r0, #58
	ldrsh	r2, [r5, r0]
	movs	r1, #54
	ldrsh	r3, [r5, r1]
	cmp	r2, r3
	bge.n	.L_0811fc3c
	b.n	.L_0811fc3a
	ldrh	r3, [r7, #10]
	cmp	r3, #0
	beq.n	.L_0811fc3c
	movs	r2, #56
	ldrsh	r3, [r5, r2]
.L_0811fc36:
	cmp	r3, #0
	beq.n	.L_0811fc3c
.L_0811fc3a:
	adds	r6, #1
.L_0811fc3c:
	cmp	r6, #0
	beq.n	.L_0811fc60
.L_0811fc40:
	ldr	r3, [sp, #20]
	mov	r4, sl
	mov	r6, r9
	lsls	r1, r3, #2
	ldr	r3, [r4, r6]
	add	r2, sp, #80
	str	r3, [r4, r1]
	ldr	r4, [sp, #24]
	mov	r0, sp
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	adds	r0, #32
	str	r3, [r0, r1]
	ldr	r6, [sp, #20]
	adds	r6, #1
	str	r6, [sp, #20]
.L_0811fc60:
	ldr	r0, [sp, #24]
	adds	r0, #1
	str	r0, [sp, #24]
	cmp	r0, r8
	bge.n	.L_0811fc6c
	b.n	.L_0811f788
.L_0811fc6c:
	ldr	r1, [sp, #20]
	cmp	r1, #0
	bne.n	.L_0811fc78
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811fe28
.L_0811fc78:
	ldrb	r3, [r7, #0]
	mov	r2, sp
	adds	r2, #32
	str	r2, [sp, #12]
	cmp	r3, #1
	beq.n	.L_0811fc86
	b.n	.L_0811fe16
.L_0811fc86:
	ldrb	r3, [r7, #8]
	cmp	r3, #1
	beq.n	.L_0811fc8e
	b.n	.L_0811fe00
.L_0811fc8e:
	ldr	r0, [sp, #28]
	bl	sub_08016ca4
	movs	r4, #165
	lsls	r4, r4, #1
	adds	r3, r0, r4
	ldrh	r0, [r3, #0]
	bl	sub_080ad140
	adds	r0, #43
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	cmp	r3, #2
	bne.n	.L_0811fcac
	b.n	.L_0811fe08
.L_0811fcac:
	ldrb	r2, [r7, #1]
	movs	r3, #15
	ands	r3, r2
	subs	r3, #3
	cmp	r3, #2
	bls.n	.L_0811fcba
	b.n	.L_0811fe10
.L_0811fcba:
	ldr	r0, [sp, #20]
	movs	r1, #1
	movs	r6, #0
	negs	r1, r1
	str	r6, [sp, #24]
	cmp	r6, r0
	bge.n	.L_0811fd88
	mov	r2, sp
	adds	r2, #32
	subs	r0, #1
	str	r2, [sp, #12]
	str	r0, [sp, #8]
.L_0811fcd2:
	ldr	r3, [sp, #24]
	ldr	r4, [sp, #8]
	cmp	r3, r4
	bge.n	.L_0811fd7a
	mov	r6, sp
	lsls	r3, r3, #2
	adds	r6, #56
	adds	r0, r3, r6
	str	r6, [sp, #16]
	ldr	r6, [sp, #24]
	adds	r2, r3, #4
	adds	r5, r3, #0
	ldr	r7, [sp, #16]
	ldr	r3, [sp, #12]
	str	r0, [sp, #4]
	mov	sl, r2
	subs	r4, r4, r6
	mov	r9, r3
	mov	fp, r4
	add	r7, sl
.L_0811fcfa:
	ldr	r2, [sp, #16]
	str	r1, [sp, #0]
	ldr	r0, [r5, r2]
	bl	sub_08016ca4
	mov	r8, r0
	ldr	r0, [r7, #0]
	bl	sub_08016ca4
	adds	r6, r0, #0
	ldr	r0, [sp, #28]
	bl	sub_08016ca4
	movs	r4, #165
	lsls	r4, r4, #1
	adds	r3, r0, r4
	ldrh	r0, [r3, #0]
	bl	sub_080ad140
	adds	r0, #43
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	ldr	r1, [sp, #0]
	cmp	r3, #0
	bne.n	.L_0811fd38
	mov	r2, r8
	movs	r0, #56
	ldrsh	r3, [r2, r0]
	movs	r4, #56
	ldrsh	r0, [r6, r4]
	b.n	.L_0811fd42
.L_0811fd38:
	mov	r2, r8
	movs	r0, #52
	ldrsh	r3, [r2, r0]
	movs	r4, #52
	ldrsh	r0, [r6, r4]
.L_0811fd42:
	cmp	r3, r0
	bge.n	.L_0811fd60
	ldr	r6, [sp, #16]
	ldr	r3, [r7, #0]
	ldr	r2, [r5, r6]
	ldr	r0, [sp, #4]
	mov	r4, sl
	str	r3, [r0, #0]
	str	r2, [r7, #0]
	mov	r3, r9
	ldr	r2, [r3, r5]
	ldr	r3, [r3, r4]
	mov	r6, r9
	str	r3, [r6, r5]
	str	r2, [r6, r4]
.L_0811fd60:
	ldr	r0, [sp, #4]
	movs	r3, #1
	negs	r3, r3
	add	fp, r3
	adds	r0, #4
	movs	r2, #4
	mov	r4, fp
	str	r0, [sp, #4]
	adds	r7, #4
	add	sl, r2
	adds	r5, #4
	cmp	r4, #0
	bne.n	.L_0811fcfa
.L_0811fd7a:
	ldr	r6, [sp, #24]
	ldr	r0, [sp, #20]
	adds	r6, #1
	str	r6, [sp, #24]
	cmp	r6, r0
	blt.n	.L_0811fcd2
	b.n	.L_0811fd8e
.L_0811fd88:
	mov	r2, sp
	adds	r2, #32
	str	r2, [sp, #12]
.L_0811fd8e:
	ldr	r3, [sp, #20]
	cmp	r3, #2
	beq.n	.L_0811fdaa
	cmp	r3, #2
	bgt.n	.L_0811fd9e
	cmp	r3, #1
	beq.n	.L_0811fde2
	b.n	.L_0811fdf4
.L_0811fd9e:
	ldr	r4, [sp, #20]
	cmp	r4, #3
	beq.n	.L_0811fdbe
	cmp	r4, #4
	beq.n	.L_0811fdd2
	b.n	.L_0811fdf4
.L_0811fdaa:
	bl	sub_08014878
	movs	r3, #11
	muls	r3, r0
	lsrs	r3, r3, #16
	movs	r1, #0
	cmp	r3, #5
	bls.n	.L_0811fdf4
.L_0811fdba:
	movs	r1, #1
	b.n	.L_0811fdf4
.L_0811fdbe:
	bl	sub_08014878
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsrs	r1, r3, #16
	cmp	r1, #5
	ble.n	.L_0811fde2
	cmp	r1, #10
	bgt.n	.L_0811fdee
	b.n	.L_0811fdba
.L_0811fdd2:
	bl	sub_08014878
	lsls	r3, r0, #3
	adds	r3, r3, r0
	lsls	r3, r3, #1
	lsrs	r1, r3, #16
	cmp	r1, #5
	bgt.n	.L_0811fde6
.L_0811fde2:
	movs	r1, #0
	b.n	.L_0811fdf4
.L_0811fde6:
	cmp	r1, #10
	ble.n	.L_0811fdba
	cmp	r1, #14
	bgt.n	.L_0811fdf2
.L_0811fdee:
	movs	r1, #2
	b.n	.L_0811fdf4
.L_0811fdf2:
	movs	r1, #3
.L_0811fdf4:
	cmp	r1, #0
	blt.n	.L_0811fe16
	ldr	r6, [sp, #12]
	lsls	r3, r1, #2
	ldr	r0, [r6, r3]
	b.n	.L_0811fe28
.L_0811fe00:
	mov	r0, sp
	adds	r0, #32
	str	r0, [sp, #12]
	b.n	.L_0811fe16
.L_0811fe08:
	mov	r1, sp
	adds	r1, #32
	str	r1, [sp, #12]
	b.n	.L_0811fe16
.L_0811fe10:
	mov	r2, sp
	adds	r2, #32
	str	r2, [sp, #12]
.L_0811fe16:
	bl	sub_08014878
	ldr	r4, [sp, #20]
	ldr	r6, [sp, #12]
	adds	r3, r4, #0
	muls	r3, r0
	lsrs	r3, r3, #16
	lsls	r3, r3, #2
	ldr	r0, [r6, r3]
.L_0811fe28:
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xfbc8
	.2byte 0x0811
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_08016ca4
	movs	r4, #48
	adds	r1, r0, #0
	adds	r4, #255
	movs	r0, #0
	movs	r2, #3
	adds	r3, r1, r4
.L_0811fe50:
	subs	r2, #1
	strb	r0, [r3, #0]
	subs	r3, #1
	cmp	r2, #0
	bge.n	.L_0811fe50
	movs	r0, #50
	adds	r0, #255
	movs	r4, #153
	movs	r3, #0
	adds	r2, r1, r0
	lsls	r4, r4, #1
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	bl	sub_080ad008
	adds	r0, r5, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0811b4d8
	pop	{r5, pc}
	.align 2, 0
