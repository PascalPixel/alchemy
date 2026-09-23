.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080048b0, 0x080048b0
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072fc, 0x080072fc
	.set sub_080b5098, 0x080b5098
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e3944, 0x080e3944
	.set sub_080e3980, 0x080e3980
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080cb7f8
	.global Func_080cb7f8
	.thumb_func
Unnamed_080cb7f8:
Func_080cb7f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldr	r1, [pc, #124]
	movs	r0, #39
	sub	sp, #76
	bl	sub_080048b0
	movs	r1, #128
	mov	r9, r0
	lsls	r1, r1, #7
	movs	r0, #40
	bl	sub_080048b0
	ldr	r1, [pc, #108]
	str	r0, [sp, #36]
	movs	r0, #41
	bl	sub_080048b0
	ldr	r3, [pc, #100]
	str	r0, [sp, #24]
	ldr	r5, [pc, #100]
	ldr	r3, [r3, #0]
	add	r5, r9
	str	r3, [sp, #20]
	movs	r0, #0
	str	r6, [r5, #0]
	bl	sub_080cd594
	ldr	r2, [pc, #88]
	movs	r3, #24
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #84]
	movs	r3, #0
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #80]
	ldr	r3, [pc, #48]
	ldr	r6, [pc, #80]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #50
	strh	r3, [r2, #0]
	adds	r0, r6, #0
	mov	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	movs	r3, #0
	ldr	r0, [pc, #60]
	ldr	r1, [sp, #24]
	movs	r2, #0
	bl	sub_080e0524
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_080cb8bc
	cmp	r3, #1
	bgt.n	.L_080cb8b2
	b.n	.L_080cb8ac
	.4byte 0x0000100c
	.4byte 0x00000100
	.4byte 0x0000782c
	.4byte 0x0000060e
	.4byte 0x03001e80
	.4byte 0x00007828
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x04000052
	.4byte 0x00000057
	.2byte 0x0076
	.2byte 0x0000
.L_080cb8ac:
	cmp	r3, #0
	beq.n	.L_080cb8b8
	b.n	.L_080cb8cc
.L_080cb8b2:
	cmp	r3, #2
	beq.n	.L_080cb8c0
	b.n	.L_080cb8cc
.L_080cb8b8:
	ldr	r0, [pc, #8]
	b.n	.L_080cb8ce
.L_080cb8bc:
	adds	r0, r6, #0
	b.n	.L_080cb8ce
.L_080cb8c0:
	ldr	r0, [pc, #4]
	b.n	.L_080cb8ce
	.4byte 0x00000048
	.2byte 0x0047
	.2byte 0x0000
.L_080cb8cc:
	ldr	r0, [pc, #268]
.L_080cb8ce:
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #264]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
	ldr	r5, [pc, #256]
	movs	r1, #0
	mov	r8, r1
.L_080cb8e6:
	movs	r3, #0
	str	r3, [r5, #4]
	bl	sub_08004458
	ldr	r3, [pc, #248]
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08004458
	ldr	r3, [pc, #240]
	mov	r4, r8
	lsls	r2, r4, #1
	ands	r3, r0
	movs	r1, #1
	adds	r3, r3, r2
	add	r8, r1
	str	r3, [r5, #8]
	mov	r2, r8
	negs	r3, r4
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #128
	bne.n	.L_080cb8e6
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #208]
	movs	r3, #75
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #200]
	bl	sub_080041d8
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [pc, #184]
	ldr	r1, [pc, #184]
	adds	r3, #184
	add	r1, r9
	ldr	r3, [r3, #0]
	ldr	r2, [r1, #0]
	str	r3, [sp, #28]
	ldr	r3, [r2, #24]
	adds	r3, #1
	str	r3, [r2, #24]
	cmp	r3, #0
	bgt.n	.L_080cb95a
	movs	r3, #1
	str	r3, [r2, #24]
.L_080cb95a:
	ldr	r2, [r1, #0]
	ldr	r3, [r2, #24]
	cmp	r3, #4
	ble.n	.L_080cb966
	movs	r3, #4
	str	r3, [r2, #24]
.L_080cb966:
	movs	r0, #212
	bl	sub_080f9010
	ldr	r2, [pc, #140]
	ldr	r1, [sp, #20]
	mov	r4, sp
	adds	r4, #64
	adds	r1, #12
	add	r2, r9
	movs	r3, #0
	str	r4, [sp, #8]
	str	r1, [sp, #12]
	str	r2, [sp, #16]
	mov	sl, r3
.L_080cb982:
	ldr	r4, [sp, #16]
	ldr	r3, [r4, #0]
	ldr	r1, [sp, #8]
	ldr	r0, [r3, #8]
	bl	sub_080e3980
	ldr	r1, [sp, #8]
	ldr	r3, [r1, #0]
	movs	r4, #64
	ldr	r2, [pc, #104]
	subs	r3, r4, r3
	lsls	r3, r3, #8
	mov	r1, sl
	str	r3, [r2, #0]
	cmp	r1, #49
	ble.n	.L_080cb9b2
	mov	r3, sl
	lsls	r2, r3, #1
	ldr	r3, [pc, #44]
	subs	r3, r3, r2
	ldr	r2, [pc, #44]
	ldr	r1, [pc, #84]
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_080cb9b2:
	mov	r4, sl
	cmp	r4, #16
	bne.n	.L_080cba08
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #20
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	movs	r3, #0
	bl	sub_080d6888
	b.n	.L_080cba08
	movs	r0, r0
	.4byte 0x00000070
	.4byte 0x00001000
	.4byte 0x00000046
	.4byte 0x03001388
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e50
	.4byte 0x00007828
	.4byte 0x04000028
	.2byte 0x0052
	.2byte 0x0400
.L_080cba08:
	mov	r3, sl
	cmp	r3, #55
	bgt.n	.L_080cbaf4
	lsrs	r3, r3, #31
	add	r3, sl
	asrs	r3, r3, #1
	mov	r8, r3
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L_080cba1e
	adds	r0, #3
.L_080cba1e:
	asrs	r7, r0, #2
	mov	fp, r7
	mov	r4, fp
	lsls	r3, r4, #2
	mov	r1, r8
	movs	r2, #2
	subs	r7, r1, r3
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	sub_080ed408
	ldr	r3, [pc, #444]
	ldr	r4, [sp, #8]
	lsls	r1, r7, #4
	ldr	r6, [r3, #0]
	adds	r1, r1, r7
	ldr	r3, [r4, #4]
	movs	r2, #17
	movs	r4, #64
	lsls	r1, r1, #6
	str	r2, [sp, #0]
	str	r6, [sp, #32]
	add	r1, r9
	subs	r3, #64
	str	r4, [sp, #4]
	ldr	r0, [sp, #36]
	movs	r2, #47
	bl	sub_080072fc
	mov	r0, sl
	cmp	r0, #0
	bge.n	.L_080cba66
	adds	r0, #3
.L_080cba66:
	movs	r1, #3
	asrs	r0, r0, #2
	bl	sub_080022fc
	adds	r7, r0, #0
	lsls	r5, r7, #7
	ldr	r2, [sp, #8]
	adds	r5, r5, r7
	lsls	r5, r5, #3
	movs	r1, #136
	ldr	r3, [r2, #4]
	lsls	r1, r1, #5
	add	r5, r9
	movs	r4, #24
	adds	r5, r5, r1
	movs	r1, #43
	str	r4, [sp, #0]
	str	r1, [sp, #4]
	ldr	r0, [sp, #36]
	subs	r3, #36
	adds	r1, r5, #0
	movs	r2, #40
	bl	sub_080072fc
	movs	r0, #47
	bl	sub_08002dd8
	movs	r2, #2
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	bl	sub_080ed408
	ldr	r3, [pc, #328]
	mov	r4, fp
	ldr	r6, [r3, #0]
	mov	r1, r8
	lsls	r3, r4, #2
	ldr	r2, [sp, #8]
	subs	r7, r1, r3
	lsls	r1, r7, #4
	ldr	r3, [r2, #4]
	adds	r1, r1, r7
	movs	r4, #17
	movs	r2, #64
	lsls	r1, r1, #6
	str	r4, [sp, #0]
	str	r2, [sp, #4]
	str	r6, [sp, #32]
	add	r1, r9
	subs	r3, #64
	ldr	r0, [sp, #36]
	bl	sub_080072fc
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #4]
	movs	r1, #24
	movs	r2, #43
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	subs	r3, #36
	ldr	r0, [sp, #36]
	adds	r1, r5, #0
	movs	r2, #64
	bl	sub_080072fc
	movs	r0, #47
	bl	sub_08002dd8
.L_080cbaf4:
	ldr	r4, [sp, #16]
	ldr	r3, [r4, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	bl	sub_080049ac
	ldr	r1, [sp, #12]
	ldr	r0, [sp, #20]
	bl	sub_080051d8
	ldr	r5, [pc, #240]
	movs	r1, #0
	mov	r8, r1
	add	r7, sp, #40
	add	r6, sp, #52
.L_080cbb14:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080cbb96
	ldr	r0, [r5, #0]
	bl	sub_08002322
	ldr	r3, [r5, #8]
	muls	r3, r0
	asrs	r3, r3, #4
	str	r3, [r7, #0]
	ldr	r0, [r5, #0]
	bl	sub_0800231c
	ldr	r3, [r5, #8]
	muls	r3, r0
	asrs	r3, r3, #4
	negs	r3, r3
	str	r3, [r7, #8]
	ldr	r3, [r5, #4]
	str	r3, [r7, #4]
	movs	r2, #128
	ldr	r3, [r5, #0]
	lsls	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r5, #0]
	movs	r4, #160
	ldr	r3, [r5, #4]
	lsls	r4, r4, #11
	adds	r3, r3, r4
	str	r3, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r3, #64
	str	r3, [r5, #8]
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_080e3944
	ldr	r2, [r6, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	ldr	r4, [sp, #16]
	ldr	r3, [r4, #0]
	mov	r1, r8
	ldr	r3, [r3, #24]
	movs	r0, #1
	ands	r0, r1
	adds	r0, r0, r3
	lsls	r4, r0, #1
	ldr	r1, [pc, #132]
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	ldr	r3, [sp, #24]
	adds	r1, r3, r1
	ldr	r3, [r6, #4]
	subs	r2, r2, r0
	subs	r3, r3, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #36]
	ldr	r4, [sp, #28]
	bl	sub_080072f4
	ldr	r3, [r5, #24]
.L_080cbb96:
	movs	r1, #1
	add	r8, r1
	adds	r3, #1
	mov	r2, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #32
	bne.n	.L_080cbb14
	bl	sub_080cd52c
	ldr	r2, [pc, #88]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	cmp	r4, #56
	beq.n	.L_080cbbc4
	b.n	.L_080cb982
.L_080cbbc4:
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [pc, #60]
	bl	sub_08004278
	bl	sub_080cdbc0
	movs	r0, #41
	bl	sub_08002dd8
	movs	r0, #40
	bl	sub_08002dd8
	movs	r0, #39
	bl	sub_08002dd8
	add	sp, #76
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f0c
	.4byte 0x02010000
	.4byte 0x080ede5c
	.4byte 0x00007824
	.4byte 0x080cd261
