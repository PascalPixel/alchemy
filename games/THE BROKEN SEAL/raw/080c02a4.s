.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_0800307c, 0x0800307c
	.set sub_080030f8, 0x080030f8
	.set sub_0800387c, 0x0800387c
	.set sub_0800393c, 0x0800393c
	.set sub_080039fc, 0x080039fc
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_080048b0, 0x080048b0
	.set sub_08015128, 0x08015128
	.set sub_08077008, 0x08077008
	.set sub_080b595c, 0x080b595c
	.set sub_080b6c08, 0x080b6c08
	.set sub_080b7b6c, 0x080b7b6c
	.set sub_080b7dd0, 0x080b7dd0
	.set sub_080c0cec, 0x080c0cec
	.set sub_080c0f98, 0x080c0f98
	.set sub_080c1724, 0x080c1724
	.set sub_080c9028, 0x080c9028
	.global Overlay_080c02a4
Overlay_080c02a4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #148
	str	r0, [sp, #0]
	ldr	r6, [pc, #200]
	adds	r5, r1, #0
	movs	r0, #42
	movs	r1, #4
	ldr	r7, [r6, #0]
	bl	sub_080048b0
	ldr	r1, [pc, #188]
	mov	fp, r0
	cmp	r5, r1
	bne.n	.L_080c02ce
	b.n	.L_080c04c8
.L_080c02ce:
	ldr	r2, [pc, #184]
	mov	ip, r2
	ldr	r3, [pc, #184]
	mov	r0, ip
	ldr	r1, [pc, #184]
	ldr	r2, [pc, #184]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, ip
	adds	r0, #32
	adds	r1, #32
	ldr	r2, [pc, #172]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, ip
	adds	r0, #64
	adds	r1, #32
	ldr	r2, [pc, #160]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, ip
	adds	r0, #96
	adds	r1, #32
	ldr	r2, [pc, #148]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, ip
	adds	r0, #128
	adds	r1, #32
	ldr	r2, [pc, #136]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, ip
	adds	r0, #160
	adds	r1, #32
	ldr	r2, [pc, #124]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, ip
	adds	r0, #192
	adds	r1, #32
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	subs	r3, #212
	movs	r2, #1
	strh	r2, [r3, #0]
	ldr	r3, [pc, #104]
	add	r4, sp, #144
	movs	r5, #0
	str	r2, [r7, #12]
	str	r2, [r7, #8]
	str	r5, [r7, #16]
	adds	r0, r4, #0
	str	r3, [r4, #0]
	subs	r1, #224
	ldr	r3, [pc, #76]
	ldr	r2, [pc, #88]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	str	r5, [r4, #0]
	adds	r0, r4, #0
	ldr	r1, [pc, #84]
	ldr	r2, [pc, #76]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #80]
	ldr	r3, [pc, #32]
	ldr	r1, [pc, #80]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #32]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r3, #2
	str	r3, [r7, #8]
	ldr	r2, [pc, #68]
	movs	r6, #0
.L_080c036a:
	ldr	r1, [pc, #68]
	cmp	r6, #20
	bls.n	.L_080c0372
	ldr	r1, [pc, #64]
.L_080c0372:
	movs	r3, #0
	b.n	.L_080c03b8
	movs	r0, r0
	.4byte 0x00000c04
	.4byte 0x00000002
	.4byte 0x03001f00
	.4byte 0x0000015b
	.4byte 0x080c5b30
	.4byte 0x040000d4
	.4byte 0x06005020
	.4byte 0x84000008
	.4byte 0x33333333
	.4byte 0x85000008
	.4byte 0x06005100
	.4byte 0x0400000a
	.4byte 0x04000008
	.4byte 0x06006000
	.4byte 0x0000f080
	.2byte 0xf088
	.2byte 0x0000
.L_080c03b8:
	adds	r3, #1
	strh	r1, [r2, #0]
	adds	r2, #2
	cmp	r3, #31
	bls.n	.L_080c03b8
	adds	r6, #1
	cmp	r6, #31
	bls.n	.L_080c036a
	ldr	r6, [pc, #88]
	movs	r3, #0
	movs	r5, #32
	mov	r9, r3
	movs	r3, #8
	strh	r5, [r6, #2]
	strh	r5, [r6, #6]
	strh	r3, [r6, #4]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #52]
	ldr	r3, [pc, #68]
	ldr	r2, [pc, #52]
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #36]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, #2
	movs	r0, #128
	strh	r3, [r2, #0]
	lsls	r0, r0, #19
	ldr	r1, [pc, #44]
	bl	sub_0800387c
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #180
	b.n	.L_080c0434
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0x00000088
	.4byte 0x00003537
	.4byte 0x00003f21
	.4byte 0x03001ad0
	.4byte 0x04000040
	.4byte 0x04000048
	.2byte 0x7741
	.2byte 0x0000
.L_080c0434:
	bl	sub_080c0cec
	ldr	r3, [pc, #120]
	mov	r2, fp
	mov	r1, r9
	mov	sl, r3
	str	r1, [r2, #0]
	movs	r1, #200
	lsls	r1, r1, #4
	mov	r0, sl
	bl	sub_080041d8
	ldr	r1, [pc, #104]
	mov	r8, r1
	movs	r1, #144
	lsls	r1, r1, #3
	mov	r0, r8
	bl	sub_080041d8
	ldr	r2, [pc, #96]
	movs	r1, #32
	movs	r0, #2
	bl	sub_0800307c
	strh	r5, [r6, #2]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [pc, #80]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	ldr	r5, [pc, #76]
	bl	sub_08015128
	movs	r0, #20
	bl	sub_080030f8
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_080039fc
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_0800393c
	ldr	r0, [sp, #0]
	bl	sub_080b595c
	mov	r0, sl
	bl	sub_08004278
	mov	r0, r8
	bl	sub_08004278
	mov	r2, r9
	strh	r2, [r6, #2]
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl	sub_0800307c
	b.n	.L_080c069c
	movs	r0, r0
	.4byte 0x080c01bd
	.4byte 0x080c0229
	.4byte 0x080c0299
	.4byte 0x03001e74
	.2byte 0x0008
	.2byte 0x0400
.L_080c04c8:
	adds	r3, r6, #0
	subs	r3, #140
	ldr	r3, [r3, #0]
	mov	sl, r3
	movs	r3, #1
	str	r3, [r7, #12]
	movs	r3, #0
	str	r3, [r7, #16]
	add	r1, sp, #32
	movs	r0, #3
	bl	sub_080b6c08
	movs	r6, #0
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L_080c0516
.L_080c04e8:
	adds	r5, r6, #0
	adds	r5, #120
	cmp	r6, #7
	bgt.n	.L_080c04f2
	adds	r5, r6, #0
.L_080c04f2:
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_08077008
	movs	r3, #148
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldrb	r3, [r0, #0]
	cmp	r3, #148
	beq.n	.L_080c0510
	ldr	r3, [pc, #152]
	str	r3, [r7, #24]
.L_080c0510:
	adds	r6, #1
	cmp	r6, r8
	bne.n	.L_080c04e8
.L_080c0516:
	ldr	r1, [pc, #148]
	ldr	r0, [pc, #148]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_080c0544
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	ldr	r2, [pc, #128]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_080c0544:
	strh	r4, [r0, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r3, #201
	ldr	r2, [pc, #104]
	lsls	r3, r3, #3
	add	r6, sp, #96
	add	r3, sl
	movs	r1, #0
	mov	r8, r1
	strh	r2, [r3, #0]
	adds	r1, r6, #0
	movs	r0, #2
	add	r5, sp, #60
	bl	sub_080b6c08
	ldr	r2, [pc, #60]
	str	r0, [r5, #20]
	mov	sl, r2
	lsls	r0, r0, #1
	mov	r3, sl
	adds	r0, #36
	strh	r3, [r5, r0]
	movs	r1, #0
	adds	r0, r6, #0
	bl	sub_080b7b6c
	adds	r0, r5, #0
	bl	sub_080c9028
	movs	r3, #100
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_080c0cec
	mov	r1, r8
	mov	r2, fp
	str	r1, [r2, #0]
	movs	r0, #2
	ldr	r2, [pc, #36]
	movs	r1, #32
	bl	sub_0800307c
	movs	r0, #1
	b.n	.L_080c05c0
	movs	r0, r0
	.4byte 0x000000ff
	.4byte 0x0000b333
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x00006041
	.4byte 0x00000021
	.2byte 0x0299
	.2byte 0x080c
.L_080c05c0:
	bl	sub_080030f8
	movs	r0, #20
	bl	sub_080030f8
	ldr	r3, [pc, #88]
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #88]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	sub_08015128
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_080039fc
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_0800393c
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	add	r3, sp, #4
	mov	r8, r3
	mov	r1, r8
	movs	r0, #3
	bl	sub_080b6c08
	adds	r7, r0, #0
	lsls	r3, r7, #1
	mov	r2, sl
	mov	r1, r8
	strh	r2, [r1, r3]
	mov	r0, r8
	movs	r1, #0
	bl	sub_080b7b6c
	movs	r0, #1
	mov	r1, r8
	bl	sub_080b6c08
	adds	r7, r0, #0
	movs	r6, #0
	cmp	r7, #0
	beq.n	.L_080c0640
	mov	r5, r8
	b.n	.L_080c0630
	.4byte 0x00003f40
	.4byte 0x03001e74
	.4byte 0x04000008
	.2byte 0x0050
	.2byte 0x0400
.L_080c0630:
	ldrh	r0, [r5, #0]
	movs	r1, #1
	adds	r6, #1
	adds	r5, #2
	bl	sub_080c0f98
	cmp	r6, r7
	bne.n	.L_080c0630
.L_080c0640:
	ldr	r3, [pc, #40]
	ldr	r5, [pc, #36]
	movs	r6, #0
	mov	sl, r3
.L_080c0648:
	adds	r3, r6, #0
	orrs	r3, r5
	mov	r1, sl
	strh	r3, [r1, #0]
	movs	r0, #1
	adds	r6, #1
	bl	sub_080030f8
	cmp	r6, #16
	bne.n	.L_080c0648
	movs	r6, #0
	cmp	r7, #0
	beq.n	.L_080c0680
	mov	r5, r8
	b.n	.L_080c0670
	movs	r0, r0
	.4byte 0x00001000
	.2byte 0x0052
	.2byte 0x0400
.L_080c0670:
	ldrh	r0, [r5, #0]
	movs	r1, #0
	adds	r6, #1
	adds	r5, #2
	bl	sub_080c0f98
	cmp	r6, r7
	bne.n	.L_080c0670
.L_080c0680:
	ldr	r0, [sp, #0]
	bl	sub_080b595c
	ldr	r2, [pc, #88]
	movs	r3, #0
	strh	r3, [r2, #2]
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl	sub_0800307c
.L_080c069c:
	ldr	r6, [pc, #68]
	ldr	r5, [pc, #56]
	movs	r1, #0
	movs	r2, #0
	movs	r0, #2
	bl	sub_0800307c
	strh	r5, [r6, #0]
	movs	r0, #1
	bl	sub_080030f8
	strh	r5, [r6, #0]
	ldr	r1, [pc, #48]
	ldr	r3, [pc, #52]
	ldrh	r2, [r1, #0]
	ands	r3, r2
	ldr	r2, [pc, #32]
	strh	r3, [r1, #0]
	movs	r3, #8
	strh	r3, [r2, #4]
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #42
	bl	sub_08002dd8
	add	sp, #148
	b.n	.L_080c06f0
	movs	r0, r0
	.4byte 0x00001f83
	.4byte 0x00001541
	.4byte 0x03001ad0
	.4byte 0x0400000a
	.4byte 0x04000008
	.2byte 0xfffd
	.2byte 0x0000
.L_080c06f0:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	push	{r5, r6, lr}
	ldr	r3, [pc, #84]
	ldr	r0, [r3, #0]
	ldr	r3, [pc, #84]
	sub	sp, #4
	adds	r4, r1, #0
	adds	r5, r0, r3
	mov	r6, sp
	ldr	r3, [pc, #76]
	ldrh	r2, [r3, #0]
	str	r2, [r6, #0]
	strh	r3, [r3, #0]
	cmp	r4, #0
	bne.n	.L_080c072a
	ldr	r3, [pc, #68]
	adds	r0, r5, #0
	ldr	r1, [pc, #68]
	ldr	r2, [pc, #72]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_080c074a
.L_080c072a:
	ldr	r3, [pc, #68]
	adds	r0, r0, r3
	lsls	r3, r4, #4
	adds	r3, r3, r4
	lsls	r3, r3, #4
	adds	r3, r3, r4
	movs	r2, #128
	lsls	r3, r3, #2
	lsls	r2, r2, #9
	subs	r2, r2, r3
	str	r2, [r0, #0]
	ldr	r1, [pc, #36]
	adds	r0, r5, #0
	movs	r3, #128
	bl	sub_080c1724
.L_080c074a:
	ldr	r2, [r6, #0]
	ldr	r3, [pc, #16]
	add	sp, #4
	strh	r2, [r3, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001e74
	.4byte 0x00000544
	.4byte 0x04000208
	.4byte 0x040000d4
	.4byte 0x050000c0
	.4byte 0x80000080
	.4byte 0x00000644
