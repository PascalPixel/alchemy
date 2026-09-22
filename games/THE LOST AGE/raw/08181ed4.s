.syntax unified
	.thumb
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_08118088, 0x08118088
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e22c, 0x0815e22c
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_081821de, 0x081821de
	.set sub_081823a8, 0x081823a8
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Overlay_08181ed4
Overlay_08181ed4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	str	r0, [sp, #36]
	str	r1, [sp, #32]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #28]
	movs	r0, #1
	ldr	r1, [r3, #96]
	str	r1, [sp, #24]
	ldr	r3, [r3, #100]
	str	r3, [sp, #20]
	bl	sub_081435e0
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L_08181f10
	movs	r2, #128
	ldr	r3, [pc, #4]
	b.n	.L_08181f14
	movs	r0, r0
	.2byte 0x0c10
	.2byte 0x0000
.L_08181f10:
	movs	r2, #128
	ldr	r3, [pc, #60]
.L_08181f14:
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	mov	r3, sp
	adds	r3, #48
	adds	r1, r3, #0
	movs	r0, #0
	str	r3, [sp, #16]
	bl	sub_08144aac
	ldr	r5, [sp, #28]
	movs	r7, #239
	movs	r0, #238
	lsls	r7, r7, #7
	lsls	r0, r0, #7
	adds	r2, r5, r7
	movs	r3, #2
	adds	r0, #132
	str	r3, [r2, #0]
	movs	r1, #200
	adds	r2, r5, r0
	movs	r3, #75
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	movs	r1, #0
	str	r1, [sp, #12]
	b.n	.L_08181f58
	.4byte 0x00001010
	.2byte 0x3001
	.2byte 0x0814
.L_08181f58:
	movs	r2, #0
	ldr	r0, [pc, #832]
	ldr	r1, [sp, #20]
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L_08181f90
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r5, r3
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #808]
	bl	sub_08157cf4
	ldr	r0, [pc, #808]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #800]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe012
.L_08181f90:
	ldr	r5, [sp, #28]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r1, r5, r7
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #780]
	bl	sub_08157cf4
	ldr	r0, [pc, #780]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #760]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9908
	movs	r0, #0
	subs	r1, #1
	str	r1, [sp, #8]
	mov	sl, r0
.L_08181fc0:
	mov	r2, sl
	cmp	r2, #19
	bgt.n	.L_08181fd0
	add	r3, sp, #56
	mov	r9, r3
	mov	r0, r9
	bl	sub_0815e22c
.L_08181fd0:
	mov	r5, sl
	cmp	r5, #0
	bne.n	.L_08181fea
	movs	r0, #144
	bl	sub_081c0010
	ldr	r7, [sp, #28]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r2, r7, r0
	movs	r3, #16
	str	r3, [r2, #0]
.L_08181fea:
	mov	r1, sl
	cmp	r1, #20
	bne.n	.L_08182010
	ldr	r2, [sp, #28]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r3, r2, r5
	movs	r2, #8
	movs	r0, #134
	str	r2, [r3, #0]
	bl	sub_081180e8
	ldr	r1, [sp, #36]
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	movs	r1, #1
	bl	sub_08118088
.L_08182010:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_08182032
	movs	r3, #0
	str	r3, [sp, #12]
	mov	r8, r3
	ldr	r3, [pc, #660]
	movs	r1, #1
	movs	r2, #128
	negs	r1, r1
	lsls	r2, r2, #2
.L_08182026:
	movs	r5, #1
	add	r8, r5
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_08182026
.L_08182032:
	ldr	r7, [sp, #8]
	cmp	r7, #1
	bhi.n	.L_0818206c
	movs	r0, #0
	mov	r8, r0
.L_0818203c:
	mov	r1, sl
	cmp	r1, #7
	ble.n	.L_08182062
	cmp	r1, #19
	bgt.n	.L_0818204e
	ldr	r0, [pc, #624]
	bl	sub_0815f0a0
	b.n	.L_08182062
.L_0818204e:
	mov	r2, sl
	cmp	r2, #31
	bgt.n	.L_0818205c
	ldr	r0, [pc, #612]
	bl	sub_0815f0a0
	b.n	.L_08182062
.L_0818205c:
	ldr	r0, [pc, #608]
	bl	sub_0815f0a0
.L_08182062:
	movs	r3, #1
	add	r8, r3
	mov	r5, r8
	cmp	r5, #3
	bne.n	.L_0818203c
.L_0818206c:
	ldr	r0, [sp, #32]
	movs	r7, #0
	mov	fp, r7
	cmp	r0, #2
	bne.n	.L_0818208e
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L_08182080
	movs	r2, #100
	mov	fp, r2
.L_08182080:
	mov	r3, sl
	subs	r3, #1
	cmp	r3, #22
	bhi.n	.L_081820aa
	movs	r3, #2
	mov	fp, r3
	b.n	.L_081820aa
.L_0818208e:
	mov	r5, sl
	cmp	r5, #0
	bne.n	.L_08182098
	movs	r7, #200
	mov	fp, r7
.L_08182098:
	ldr	r0, [sp, #8]
	cmp	r0, #1
	bhi.n	.L_081820aa
	mov	r3, sl
	subs	r3, #1
	cmp	r3, #22
	bhi.n	.L_081820aa
	movs	r1, #16
	mov	fp, r1
.L_081820aa:
	movs	r2, #0
	mov	r3, fp
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_0818214e
	add	r5, sp, #56
	mov	r9, r5
.L_081820b8:
	ldr	r7, [sp, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r2, r7
	lsls	r3, r2, #3
	ldr	r0, [pc, #508]
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r6, r3, r0
	bl	sub_08014878
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r0
	adds	r7, r3, #0
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	ldr	r1, [pc, #484]
	adds	r3, #255
	ands	r3, r0
	mov	r2, r9
	adds	r5, r3, r1
	ldr	r3, [r2, #0]
	adds	r7, #32
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	movs	r3, #216
	lsls	r3, r3, #15
	str	r3, [r6, #4]
	ldr	r3, [sp, #32]
	cmp	r3, #2
	bne.n	.L_08182114
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r7, #0
	muls	r3, r0
	asrs	r3, r3, #8
	b.n	.L_08182120
.L_08182114:
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r7, #0
	muls	r3, r0
	asrs	r3, r3, #6
.L_08182120:
	str	r3, [r6, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r6, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #48
	str	r3, [r6, #24]
	ldr	r7, [sp, #12]
	movs	r5, #1
	adds	r7, #1
	add	r8, r5
	str	r7, [sp, #12]
	cmp	r8, fp
	bne.n	.L_081820b8
.L_0818214e:
	ldr	r6, [pc, #372]
	movs	r0, #0
	mov	r8, r0
.L_08182154:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	sub_081821de
	subs	r3, #1
	str	r3, [r6, #24]
	ldr	r1, [sp, #32]
	ldr	r2, [pc, #360]
	lsls	r3, r1, #2
	ldr	r2, [r2, r3]
	adds	r0, r6, #0
	movs	r1, #60
	bl	sub_08138086
	ldr	r3, [r6, #4]
	asrs	r2, r3, #16
	mov	ip, r2
	cmp	r2, #120
	ble.n	.L_08182186
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	sub_081821de
.L_08182186:
	ldr	r2, [r6, #0]
	cmp	r2, #0
	blt.n	sub_081821de
	asrs	r7, r2, #16
	cmp	r7, #126
	bgt.n	sub_081821de
	cmp	r3, #0
	blt.n	sub_081821de
	ldr	r3, [r6, #24]
	adds	r2, r3, #0
	subs	r2, #16
	cmp	r2, #0
	bge.n	.L_081821a2
	adds	r2, #7
.L_081821a2:
	ldr	r3, [pc, #300]
	ldr	r0, [sp, #32]
	asrs	r5, r2, #3
	ldrsb	r3, [r3, r0]
	cmp	r5, r3
	bge.n	.L_081821b0
	adds	r5, r3, #0
.L_081821b0:
	ldr	r2, [pc, #288]
	lsls	r4, r5, #1
	subs	r3, r4, #2
	mov	r1, r8
	movs	r0, #1
	ands	r0, r1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	str	r5, [sp, #0]
	adds	r1, r2, r1
	lsrs	r2, r5, #31
	adds	r2, r5, r2
	asrs	r2, r2, #1
	subs	r2, r7, r2
	mov	r7, ip
	subs	r3, r7, r5
	str	r4, [sp, #4]
	ldr	r5, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r5]
	ldr	r0, [sp, #24]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2701
	movs	r0, #128
	add	r8, r7
	lsls	r0, r0, #2
	adds	r6, #28
	cmp	r8, r0
	bne.n	.L_08182154
	mov	r1, sl
	cmp	r1, #39
	ble.n	.L_081821f4
	b.n	.L_08182332
.L_081821f4:
	mov	r0, r8
	bl	sub_08014dac
	adds	r6, r0, #0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #212]
	ldr	r3, [sp, #40]
	movs	r7, #224
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #204]
	lsls	r7, r7, #3
	ands	r3, r2
	movs	r2, #160
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r2, [sp, #28]
	str	r3, [sp, #40]
	adds	r5, r0, #0
	adds	r3, r2, r7
	mov	r0, sl
	add	r7, sp, #40
	movs	r2, #31
	str	r3, [r7, #4]
	lsls	r3, r0, #2
	ands	r3, r2
	strb	r3, [r5, #24]
	lsls	r3, r0, #1
	ands	r3, r2
	strb	r3, [r5, #25]
	bl	sub_08014de4
	ldr	r0, [sp, #56]
	ldr	r1, [pc, #160]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	lsls	r0, r0, #16
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #250
	lsls	r0, r0, #3
	bl	sub_08015024
	ldr	r1, [sp, #32]
	cmp	r1, #0
	bne.n	.L_08182286
	mov	r2, sl
	cmp	r2, #15
	bgt.n	.L_0818226a
	lsls	r0, r2, #10
	bl	sub_08002096
	b.n	.L_08182280
.L_0818226a:
	movs	r0, #128
	mov	r3, sl
	lsls	r0, r0, #9
	cmp	r3, #23
	ble.n	.L_08182280
	ldr	r2, [pc, #108]
	mov	r1, sl
	lsls	r0, r1, #10
	adds	r0, r0, r2
	bl	sub_08002096
.L_08182280:
	movs	r2, #32
	negs	r2, r2
	b.n	.L_081822fc
.L_08182286:
	ldr	r3, [sp, #32]
	cmp	r3, #1
	bne.n	.L_081822e8
	mov	r1, sl
	movs	r2, #32
	lsls	r0, r1, #12
	negs	r2, r2
	cmp	r1, #31
	ble.n	.L_081822fc
	lsls	r2, r1, #3
	b.n	.L_081822f8
	.4byte 0x00000134
	.4byte 0x000000c0
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x000000d2
	.4byte 0x00000184
	.4byte 0x02010018
	.4byte 0x00000154
	.4byte 0x00000150
	.4byte 0x00000152
	.4byte 0x02010000
	.4byte 0xffffc000
	.4byte 0x08199660
	.4byte 0x0819966c
	.4byte 0x08197410
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0xfff00000
	.2byte 0xe000
	.2byte 0xffff
.L_081822e8:
	.2byte 0x2080
	movs	r2, #32
	mov	r3, sl
	lsls	r0, r0, #7
	negs	r2, r2
	cmp	r3, #31
	ble.n	.L_081822fc
	lsls	r2, r3, #3
.L_081822f8:
	movs	r3, #224
	subs	r2, r3, r2
.L_081822fc:
	movs	r3, #7
	str	r3, [r5, #0]
	ldr	r3, [pc, #128]
	str	r2, [r5, #20]
	movs	r1, #224
	lsls	r2, r0, #1
	str	r3, [r5, #8]
	adds	r0, r2, #0
	lsls	r1, r1, #11
	str	r7, [r5, #16]
	str	r6, [r5, #12]
	bl	sub_080151e4
	adds	r1, r6, #0
	movs	r2, #16
	ldr	r0, [pc, #108]
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
	adds	r0, r5, #0
	bl	sub_08013164
	adds	r0, r6, #0
	bl	sub_08013164
.L_08182332:
	movs	r1, #16
	movs	r0, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r7, #240
	ldr	r5, [sp, #28]
	lsls	r7, r7, #7
	adds	r7, #232
	adds	r2, r5, r7
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	cmp	r1, #54
	beq.n	.L_0818235e
	b.n	.L_08181fc0
.L_0818235e:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #32]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08198df8
	.4byte 0x08198c6c
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_081823a8
	pop	{pc}
