.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014cc0, 0x08014cc0
	.set sub_08016ca4, 0x08016ca4
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_08038038, 0x08038038
	.set sub_08038048, 0x08038048
	.set sub_080380c8, 0x080380c8
	.set sub_080380f8, 0x080380f8
	.set sub_08038140, 0x08038140
	.set sub_08038368, 0x08038368
	.set sub_08038390, 0x08038390
	.set sub_080ad020, 0x080ad020
	.set sub_080ad048, 0x080ad048
	.set sub_080ad100, 0x080ad100
	.set sub_080ad1e0, 0x080ad1e0
	.set sub_080ad1e8, 0x080ad1e8
	.set sub_080c8088, 0x080c8088
	.set sub_080c84d8, 0x080c84d8
	.set sub_080c85c8, 0x080c85c8
	.set sub_080f8028, 0x080f8028
	.set sub_080f8048, 0x080f8048
	.set sub_08108948, 0x08108948
	.set sub_08108aa8, 0x08108aa8
	.set sub_08108b34, 0x08108b34
	.set sub_08108b70, 0x08108b70
	.set sub_08109ad8, 0x08109ad8
	.set sub_0810a2c8, 0x0810a2c8
	.set sub_0810a66c, 0x0810a66c
	.set sub_0810a670, 0x0810a670
	.set sub_0810a6b8, 0x0810a6b8
	.set sub_0810a6f8, 0x0810a6f8
	.set sub_0810bdf4, 0x0810bdf4
	.set sub_0810be3c, 0x0810be3c
	.set sub_081c0048, 0x081c0048
	.global Func_081080a8
	.thumb_func
Func_081080a8:
.L_081080a8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r7, #156
	mov	r8, r3
	movs	r1, #0
	lsls	r7, r7, #2
	mov	sl, r1
	movs	r6, #0
	add	r7, r8
.L_081080c6:
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #6
	add	r3, r8
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	cmp	r5, #3
	beq.n	.L_081080e4
	cmp	r5, #4
	beq.n	.L_081080e4
	adds	r0, r6, #0
	bl	sub_080ad1e0
	cmp	r5, r0
	bne.n	.L_081080f8
.L_081080e4:
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_080ad1e8
	cmp	r0, #0
	beq.n	.L_081080f8
	movs	r2, #1
	strh	r6, [r7, #0]
	add	sl, r2
	adds	r7, #2
.L_081080f8:
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r6, #1
	adds	r3, #255
	cmp	r6, r3
	ble.n	.L_081080c6
	mov	r1, sl
	movs	r2, #156
	lsls	r3, r1, #1
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #20]
	mov	r1, r8
	strh	r2, [r1, r3]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #2
	add	r3, r8
	mov	r2, sl
	strh	r2, [r3, #0]
	mov	r0, sl
	b.n	.L_08108128
	.2byte 0x0000
	.2byte 0x0000
.L_08108128:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #220
	adds	r0, r0, r3
	bl	sub_08108948
	pop	{pc}
.L_08108148:
	push	{r5, lr}
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #236
	movs	r0, #220
	sub	sp, #4
	bl	sub_08014cc0
	adds	r5, r0, #0
	bl	sub_080c84d8
	movs	r3, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r5, #0
	ldr	r2, [pc, #188]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #5
	adds	r2, r5, r3
	movs	r3, #12
	strb	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #202
	adds	r0, r5, r2
	bl	sub_080ad100
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #4
	adds	r3, r5, r2
	strb	r0, [r3, #0]
	bl	sub_080f8048
	bl	sub_080143ac
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #236
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #136]
	movs	r1, #128
	bl	sub_08108b34
	bl	sub_080143ac
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #238
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #116]
	movs	r1, #128
	bl	sub_08108b34
	bl	sub_080143ac
	movs	r2, #158
	lsls	r2, r2, #3
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #100]
	movs	r1, #128
	bl	sub_08108b34
	bl	sub_080143ac
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #242
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #84]
	movs	r1, #128
	bl	sub_08108b34
	bl	sub_080143ac
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #246
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #64]
	movs	r1, #128
	bl	sub_08108b34
	bl	sub_080143ac
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #244
	adds	r5, r5, r3
	ldr	r2, [pc, #48]
	strh	r0, [r5, #0]
	movs	r1, #128
	bl	sub_08108b34
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl	sub_080145a8
	bl	sub_0810bdf4
	add	sp, #4
	pop	{r5, pc}
	.4byte 0x8500033b
	.4byte 0x000001fa
	.4byte 0x000001fe
	.4byte 0x000001ff
	.4byte 0x000001fb
	.4byte 0x000001fc
	.4byte 0x000001fd
	.2byte 0x8131
	.2byte 0x0810
.L_0810824c:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	bl	sub_0810be3c
	ldr	r0, [pc, #100]
	bl	sub_08014644
	bl	sub_08038140
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #236
	adds	r3, r5, r2
	ldrh	r0, [r3, #0]
	bl	sub_08014274
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #238
	adds	r3, r5, r2
	ldrh	r0, [r3, #0]
	bl	sub_08014274
	movs	r2, #158
	lsls	r2, r2, #3
	adds	r3, r5, r2
	ldrh	r0, [r3, #0]
	bl	sub_08014274
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #242
	adds	r3, r5, r2
	ldrh	r0, [r3, #0]
	bl	sub_08014274
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #244
	adds	r3, r5, r2
	ldrh	r0, [r3, #0]
	bl	sub_08014274
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #246
	adds	r5, r5, r3
	ldrh	r0, [r5, #0]
	bl	sub_08014274
	movs	r0, #220
	bl	sub_0801314c
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x8131
	.2byte 0x0810
.L_081082c4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r1, #0
	movs	r1, #0
	mov	r8, r0
	sub	sp, #4
	mov	sl, r1
	bl	sub_0810a66c
	cmp	r8, r0
	bge.n	.L_081082e6
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_081082ea
.L_081082e6:
	movs	r3, #0
	mov	r8, r3
.L_081082ea:
	mov	r0, r8
	bl	sub_0810a670
	bl	.L_08108148
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	mov	r0, r8
	ldr	r7, [r3, #0]
	bl	sub_0810a6f8
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #6
	adds	r3, r7, r1
	strb	r0, [r3, #0]
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
	mov	r9, r0
	cmp	r0, #0
	bne.n	.L_08108344
	movs	r3, #2
	movs	r0, #30
	movs	r1, #0
	movs	r2, #2
	str	r3, [sp, #0]
	bl	sub_08038010
	mov	r9, r0
.L_08108344:
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #236
	adds	r3, r7, r2
	movs	r1, #128
	ldrh	r0, [r3, #0]
	lsls	r1, r1, #23
	movs	r3, #0
	mov	r2, r9
	str	r3, [sp, #0]
	bl	sub_080380c8
	movs	r3, #255
	adds	r5, r0, #0
	strb	r3, [r5, #15]
	movs	r3, #1
	strb	r3, [r5, #5]
	ldr	r3, [pc, #32]
	movs	r1, #32
	strb	r3, [r5, #4]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #220
	adds	r6, r7, r3
	adds	r0, r6, #0
	negs	r1, r1
	movs	r2, #112
	bl	sub_08108aa8
	str	r5, [r6, #0]
	ldr	r0, [pc, #8]
	bl	.L_081084f4
	b.n	.L_08108390
	.4byte 0x00000000
	.2byte 0x124c
	.2byte 0x0000
.L_08108390:
	mov	r0, sl
	bl	sub_08038368
	movs	r1, #129
	lsls	r1, r1, #3
	mov	sl, r0
	adds	r1, #255
	adds	r3, r7, r1
	mov	r2, sl
	strb	r2, [r3, #0]
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_081083c4
	movs	r2, #156
	lsls	r2, r2, #2
	adds	r1, r7, r2
	mov	r0, r8
	bl	sub_0810a6b8
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #2
	adds	r3, r7, r1
	strh	r0, [r3, #0]
	ldr	r0, [pc, #132]
	b.n	.L_081083e6
.L_081083c4:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L_081083d6
	ldr	r0, [pc, #128]
	bl	.L_081084f4
	bl	sub_08109ad8
	b.n	.L_0810840e
.L_081083d6:
	mov	r3, sl
	cmp	r3, #2
	bne.n	.L_081083fe
	bl	.L_081080a8
	cmp	r0, #0
	beq.n	.L_081083f0
	ldr	r0, [pc, #104]
.L_081083e6:
	bl	.L_081084f4
	bl	sub_08108b70
	b.n	.L_0810840e
.L_081083f0:
	ldr	r0, [pc, #96]
	bl	.L_081084f4
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_0810840e
.L_081083fe:
	mov	r1, sl
	cmp	r1, #3
	bne.n	.L_08108428
	ldr	r0, [pc, #80]
	bl	.L_081084f4
	bl	sub_0810a2c8
.L_0810840e:
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #220
	movs	r1, #32
	adds	r0, r7, r2
	negs	r1, r1
	movs	r2, #112
	bl	sub_08108aa8
	ldr	r0, [pc, #56]
	bl	.L_081084f4
	b.n	.L_08108390
.L_08108428:
	ldr	r0, [pc, #52]
	bl	.L_081084f4
	mov	r0, r9
	movs	r1, #2
	bl	sub_08038018
	bl	.L_0810824c
	movs	r0, #0
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x00001258
	.4byte 0x0000125a
	.4byte 0x00001269
	.4byte 0x00001268
	.4byte 0x0000126a
	.4byte 0x00001255
	.2byte 0x1256
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #112]
	ldr	r2, [pc, #112]
	movs	r1, #128
	str	r2, [r3, #16]
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r3, r2
	lsls	r1, r1, #3
	movs	r2, #28
	strb	r2, [r3, #0]
	adds	r1, #141
	movs	r0, #1
	bl	sub_080ad020
	adds	r1, r0, #0
	movs	r0, #1
	bl	sub_080ad048
	movs	r1, #195
	lsls	r1, r1, #2
	adds	r1, #255
	movs	r0, #0
	bl	sub_080ad020
	adds	r1, r0, #0
	movs	r0, #0
	bl	sub_080ad048
	movs	r1, #231
	movs	r0, #2
	bl	sub_080ad020
	movs	r0, #3
	bl	sub_08016ca4
	movs	r6, #50
	movs	r5, #1
	adds	r6, #255
	strb	r5, [r0, r6]
	movs	r0, #5
	bl	sub_08016ca4
	strb	r5, [r0, r6]
	movs	r0, #2
	bl	sub_08016ca4
	movs	r3, #160
	lsls	r3, r3, #1
	adds	r0, r0, r3
	strb	r5, [r0, #0]
	movs	r1, #30
	movs	r0, #0
	bl	.L_081082c4
	movs	r0, #0
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x0d40
	.2byte 0x0003
	push	{lr}
	b.n	.L_081084ea
.L_081084e4:
	movs	r0, #1
	bl	sub_08013560
.L_081084ea:
	bl	sub_081c0048
	cmp	r0, #0
	bne.n	.L_081084e4
	pop	{pc}
.L_081084f4:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #250
	adds	r3, r5, r2
	adds	r6, r0, #0
	ldrh	r0, [r3, #0]
	bl	sub_080c85c8
	adds	r7, r0, #0
	bl	sub_08038140
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #6
	adds	r5, r5, r3
	movs	r1, #0
	ldrsb	r1, [r5, r1]
	cmp	r1, #3
	bne.n	.L_0810852c
	ldr	r3, [pc, #68]
	ldr	r2, [pc, #72]
	subs	r3, r3, r2
	adds	r6, r6, r3
.L_0810852c:
	cmp	r1, #2
	bne.n	.L_08108538
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #60]
	subs	r3, r3, r2
	adds	r6, r6, r3
.L_08108538:
	cmp	r1, #0
	bne.n	.L_08108544
	ldr	r3, [pc, #56]
	ldr	r2, [pc, #48]
	subs	r3, r3, r2
	adds	r6, r6, r3
.L_08108544:
	lsls	r3, r7, #16
	movs	r2, #34
	orrs	r3, r2
	adds	r0, r6, #0
	movs	r1, #5
	movs	r2, #0
	bl	sub_08038038
	b.n	.L_0810855c
.L_08108556:
	movs	r0, #1
	bl	sub_08013560
.L_0810855c:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_08108556
	movs	r0, #1
	bl	sub_08013560
	pop	{r5, r6, r7, pc}
	.4byte 0x00001317
	.4byte 0x0000124c
	.4byte 0x00001277
	.2byte 0x12a2
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #220
	adds	r7, r6, r2
	ldr	r3, [r7, #0]
	adds	r2, #30
	ldrb	r3, [r3, #5]
	adds	r5, r0, #0
	mov	sl, r3
	adds	r3, r6, r2
	ldrh	r0, [r3, #0]
	bl	sub_080c85c8
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #6
	adds	r3, r6, r2
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	mov	r8, r0
	cmp	r1, #3
	bne.n	.L_081085c0
	ldr	r3, [pc, #100]
	ldr	r2, [pc, #104]
	subs	r3, r3, r2
	adds	r5, r5, r3
.L_081085c0:
	cmp	r1, #2
	bne.n	.L_081085cc
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #92]
	subs	r3, r3, r2
	adds	r5, r5, r3
.L_081085cc:
	cmp	r1, #0
	bne.n	.L_081085d8
	ldr	r3, [pc, #88]
	ldr	r2, [pc, #80]
	subs	r3, r3, r2
	adds	r5, r5, r3
.L_081085d8:
	ldr	r2, [r7, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	bl	sub_08038140
	mov	r2, r8
	lsls	r3, r2, #16
	movs	r2, #34
	orrs	r3, r2
	adds	r0, r5, #0
	movs	r1, #5
	movs	r2, #0
	bl	sub_08038038
	b.n	.L_081085fc
.L_081085f6:
	movs	r0, #1
	bl	sub_08013560
.L_081085fc:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_081085f6
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #220
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	mov	r2, sl
	strb	r2, [r3, #5]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x00001317
	.4byte 0x0000124c
	.4byte 0x00001277
	.2byte 0x12a2
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #220
	adds	r5, r5, r3
	adds	r7, r0, #0
	ldr	r0, [r5, #0]
	ldrb	r6, [r0, #5]
	bl	sub_080f8028
	adds	r2, r7, #0
	movs	r1, #5
	movs	r0, #7
	bl	sub_08038390
	ldr	r3, [r5, #0]
	adds	r7, r0, #0
	strb	r6, [r3, #5]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
