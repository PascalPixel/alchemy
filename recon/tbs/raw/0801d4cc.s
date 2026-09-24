.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_0801d108, 0x0801d108
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_080164ac, 0x080164ac
	.set sub_080164d4, 0x080164d4
	.set sub_0801ccc0, 0x0801ccc0
	.set sub_0801d014, 0x0801d014
	.set sub_0801d0f0, 0x0801d0f0
	.set sub_0801e74c, 0x0801e74c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_08021620, 0x08021620
	.set sub_080216b4, 0x080216b4
	.set sub_080216e8, 0x080216e8
	.set sub_080a1038, 0x080a1038
	.set sub_080b0030, 0x080b0030
	.set sub_080f9010, 0x080f9010
	.global Menu_RunWorkspaceOptions
	.global Func_0801d4cc
	.thumb_func
Menu_RunWorkspaceOptions:
Func_0801d4cc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #1
	movs	r1, #0
	sub	sp, #20
	mov	fp, r0
	mov	r9, r1
	bl	sub_0801d014
	ldr	r3, [pc, #172]
	ldr	r7, [r3, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #2
	movs	r2, #28
	movs	r3, #3
	movs	r0, #1
	bl	sub_080162d4
	str	r0, [sp, #16]
	bl	sub_0801d108
	movs	r3, #48
	mov	r8, r0
	negs	r3, r3
	movs	r2, #64
	mov	r1, r8
	movs	r0, #7
	bl	sub_08021620
	str	r0, [sp, #12]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [pc, #128]
	ldr	r3, [pc, #128]
	adds	r2, r7, r2
	adds	r3, r7, r3
	str	r2, [sp, #8]
	str	r3, [sp, #4]
.L_0801d526:
	mov	r0, fp
	cmp	r0, #0
	bne.n	.L_0801d52e
	b.n	.L_0801d766
.L_0801d52e:
	movs	r1, #0
	mov	r0, r9
	mov	fp, r1
	adds	r0, #5
	movs	r1, #5
	bl	sub_080022fc
	ldr	r5, [pc, #92]
	movs	r2, #179
	mov	r9, r0
	lsls	r2, r2, #3
	add	r2, r9
	adds	r3, r7, #1
	add	r5, r9
	ldrsb	r1, [r3, r2]
	ldrsb	r0, [r7, r5]
	adds	r0, r0, r1
	bl	sub_080022fc
	ldr	r2, [pc, #76]
	strb	r0, [r7, r5]
	adds	r3, r7, r2
	mov	r0, r9
	strh	r0, [r3, #0]
	ldr	r3, [pc, #72]
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #48]
	cmp	r3, #0
	beq.n	.L_0801d570
	movs	r1, #179
	lsls	r1, r1, #3
	adds	r3, r7, r1
	strb	r2, [r3, #0]
.L_0801d570:
	ldr	r6, [pc, #56]
	movs	r5, #0
.L_0801d574:
	ldr	r0, [r6, r7]
	movs	r3, #251
	strb	r3, [r0, #15]
	bl	sub_080a1038
	ldr	r3, [r6, r7]
	ldr	r0, [pc, #44]
	ldrb	r1, [r3, #14]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #0
	cmp	r5, r3
	beq.n	.L_0801d5b6
	b.n	.L_0801d5b4
	.4byte 0x00000000
	.4byte 0x03001ea0
	.4byte 0x00000594
	.4byte 0x00000595
	.4byte 0x00000574
	.4byte 0x03001ca0
	.4byte 0x000005ec
	.2byte 0x0596
	.2byte 0x0000
.L_0801d5b4:
	movs	r2, #1
.L_0801d5b6:
	ldr	r3, [pc, #808]
	ldrsb	r0, [r3, r5]
	adds	r5, #1
	bl	sub_080216e8
	adds	r6, #4
	cmp	r5, #2
	ble.n	.L_0801d574
	movs	r6, #191
	movs	r5, #0
	lsls	r6, r6, #3
.L_0801d5cc:
	ldr	r0, [r6, r7]
	movs	r3, #251
	strb	r3, [r0, #15]
	bl	sub_080a1038
	ldr	r3, [r6, r7]
	ldr	r0, [pc, #776]
	ldrb	r1, [r3, #14]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #0
	cmp	r5, r3
	beq.n	.L_0801d5ec
	movs	r2, #1
.L_0801d5ec:
	ldr	r3, [pc, #760]
	ldrsb	r0, [r3, r5]
	adds	r5, #1
	bl	sub_080216e8
	adds	r6, #4
	cmp	r5, #1
	ble.n	.L_0801d5cc
	ldr	r6, [pc, #748]
	movs	r5, #0
.L_0801d600:
	ldr	r0, [r6, r7]
	movs	r3, #251
	strb	r3, [r0, #15]
	bl	sub_080a1038
	ldr	r3, [r6, r7]
	ldrb	r1, [r3, #14]
	movs	r3, #179
	lsls	r3, r3, #3
	adds	r3, r3, r7
	mov	sl, r3
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #0
	cmp	r5, r3
	beq.n	.L_0801d624
	movs	r2, #1
.L_0801d624:
	ldr	r3, [pc, #712]
	ldrsb	r0, [r3, r5]
	adds	r5, #1
	bl	sub_080216e8
	adds	r6, #4
	cmp	r5, #1
	ble.n	.L_0801d600
	mov	r0, r8
	ldrh	r3, [r0, #12]
	ldr	r1, [sp, #8]
	lsls	r3, r3, #3
	adds	r5, r3, #0
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldr	r2, [pc, #688]
	lsls	r0, r3, #4
	subs	r0, r0, r3
	adds	r3, r7, r2
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	lsls	r0, r0, #2
	bl	sub_080022ec
	adds	r5, #140
	adds	r5, r5, r0
	mov	r0, r8
	ldrh	r3, [r0, #14]
	ldr	r1, [pc, #664]
	lsls	r3, r3, #3
	adds	r2, r3, #4
	adds	r0, r7, r1
	movs	r3, #1
	adds	r1, r5, #0
	bl	sub_080b0030
	mov	r2, r8
	ldrh	r3, [r2, #12]
	ldr	r0, [sp, #4]
	lsls	r3, r3, #3
	adds	r5, r3, #0
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	ldr	r1, [pc, #640]
	lsls	r0, r3, #4
	subs	r0, r0, r3
	adds	r3, r7, r1
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	lsls	r0, r0, #2
	bl	sub_080022ec
	mov	r2, r8
	ldrh	r3, [r2, #14]
	lsls	r3, r3, #3
	adds	r2, r3, #0
	adds	r5, #140
	ldr	r3, [pc, #616]
	adds	r5, r5, r0
	adds	r1, r5, #0
	adds	r0, r7, r3
	adds	r2, #20
	movs	r3, #1
	bl	sub_080b0030
	ldr	r0, [pc, #604]
	adds	r3, r7, r0
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r3, [pc, #600]
	adds	r5, r2, r3
	movs	r3, #48
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #160
	movs	r2, #40
	movs	r3, #200
	bl	sub_080164d4
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #160
	movs	r3, #40
	bl	sub_0801e7c0
	ldr	r1, [pc, #532]
	adds	r3, r7, r1
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r3, [pc, #564]
	adds	r5, r2, r3
	movs	r3, #72
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #160
	movs	r2, #64
	movs	r3, #184
	bl	sub_080164d4
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #160
	movs	r3, #64
	bl	sub_0801e7c0
	mov	r3, sl
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r3, [pc, #528]
	adds	r5, r2, r3
	movs	r3, #96
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #160
	movs	r2, #88
	movs	r3, #184
	bl	sub_080164d4
	adds	r0, r5, #0
	movs	r3, #88
	mov	r1, r8
	movs	r2, #160
	bl	sub_0801e7c0
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #4]
	movs	r0, #0
	ldrsb	r0, [r1, r0]
	movs	r1, #0
	ldrsb	r1, [r2, r1]
	bl	sub_0801ccc0
	mov	r0, r8
	ldrh	r3, [r0, #12]
	mov	r1, r9
	ldrh	r2, [r0, #14]
	lsls	r5, r3, #3
	lsls	r3, r1, #1
	add	r3, r9
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r2, r3, #4
	cmp	r1, #0
	bne.n	.L_0801d746
	adds	r2, #8
.L_0801d746:
	ldr	r3, [pc, #460]
	adds	r1, r5, #0
	adds	r0, r7, r3
	movs	r3, #3
	bl	sub_080b0030
	ldr	r0, [sp, #16]
	bl	sub_080164ac
	ldr	r0, [pc, #444]
	ldr	r1, [sp, #16]
	add	r0, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e74c
.L_0801d766:
	ldr	r0, [sp, #12]
	bl	sub_080216b4
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #424]
	ldr	r2, [r1, #0]
	movs	r3, #4
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0801d7b6
	movs	r0, #112
	bl	sub_080f9010
	ldr	r0, [pc, #408]
	adds	r1, r7, r0
	ldrh	r3, [r1, #0]
	movs	r0, #1
	adds	r3, #1
	mov	fp, r0
	movs	r0, #160
	strh	r3, [r1, #0]
	lsls	r0, r0, #11
	lsls	r3, r3, #16
	movs	r2, #0
	cmp	r3, r0
	bls.n	.L_0801d7a0
	strh	r2, [r1, #0]
.L_0801d7a0:
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #384]
	ldrb	r3, [r2, r3]
	ldr	r2, [sp, #8]
	strb	r3, [r2, #0]
	ldr	r2, [pc, #380]
	ldrh	r3, [r1, #0]
	ldr	r0, [sp, #4]
	ldrb	r3, [r2, r3]
	strb	r3, [r0, #0]
	b.n	.L_0801d526
.L_0801d7b6:
	ldr	r2, [r1, #0]
	movs	r3, #9
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0801d8a6
	ldr	r2, [r1, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0801d89a
	ldr	r5, [pc, #352]
	ldr	r2, [r5, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0801d7e8
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	negs	r1, r1
	movs	r2, #1
	add	r9, r1
	mov	fp, r2
	b.n	.L_0801d526
.L_0801d7e8:
	ldr	r2, [r5, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0801d800
	movs	r0, #111
	bl	sub_080f9010
	movs	r3, #1
	add	r9, r3
	mov	fp, r3
	b.n	.L_0801d526
.L_0801d800:
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0801d81e
	movs	r0, #111
	bl	sub_080f9010
	ldr	r2, [pc, #284]
	add	r2, r9
	ldrb	r3, [r7, r2]
	subs	r3, #1
	movs	r0, #1
	strb	r3, [r7, r2]
	mov	fp, r0
.L_0801d81e:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0801d82a
	b.n	.L_0801d526
.L_0801d82a:
	movs	r0, #111
	bl	sub_080f9010
	ldr	r2, [pc, #252]
	add	r2, r9
	ldrb	r3, [r7, r2]
	movs	r1, #1
	adds	r3, #1
	strb	r3, [r7, r2]
	mov	fp, r1
	b.n	.L_0801d526
.L_0801d840:
	ldr	r0, [sp, #16]
	movs	r1, #2
	bl	sub_08016418
	mov	r0, r8
	movs	r1, #2
	bl	sub_08016418
	cmp	r5, #0
	bne.n	.L_0801d8b0
	ldr	r0, [pc, #216]
	ldr	r2, [pc, #220]
	adds	r3, r7, r0
	ldr	r0, [pc, #220]
	ldrb	r1, [r3, #0]
	adds	r3, r2, r0
	strb	r1, [r3, #0]
	ldr	r1, [pc, #216]
	adds	r3, r7, r1
	ldrb	r1, [r3, #0]
	adds	r0, #1
	adds	r3, r2, r0
	strb	r1, [r3, #0]
	ldr	r1, [pc, #148]
	adds	r3, r7, r1
	ldrb	r1, [r3, #0]
	adds	r0, #6
	adds	r3, r2, r0
	strb	r1, [r3, #0]
	ldr	r1, [pc, #104]
	adds	r3, r7, r1
	ldrb	r1, [r3, #0]
	subs	r0, #2
	adds	r3, r2, r0
	strb	r1, [r3, #0]
	movs	r1, #179
	lsls	r1, r1, #3
	adds	r3, r7, r1
	ldrb	r1, [r3, #0]
	ldr	r3, [pc, #176]
	adds	r2, r2, r3
	ldr	r3, [pc, #176]
	strb	r1, [r2, #0]
	strb	r1, [r3, #0]
	b.n	.L_0801d8c2
.L_0801d89a:
	movs	r5, #1
	movs	r0, #113
	negs	r5, r5
	bl	sub_080f9010
	b.n	.L_0801d840
.L_0801d8a6:
	movs	r0, #112
	movs	r5, #0
	bl	sub_080f9010
	b.n	.L_0801d840
.L_0801d8b0:
	ldr	r3, [pc, #128]
	ldr	r0, [pc, #132]
	ldr	r1, [pc, #144]
	adds	r2, r3, r0
	adds	r3, r3, r1
	ldrb	r0, [r2, #0]
	ldrb	r1, [r3, #0]
	bl	sub_0801ccc0
.L_0801d8c2:
	bl	sub_0801d0f0
	movs	r0, #1
	bl	sub_080030f8
	adds	r0, r5, #0
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x080367c9
	.4byte 0x00000597
	.4byte 0x080367cc
	.4byte 0x00000604
	.4byte 0x080367ce
	.4byte 0x00000599
	.4byte 0x000005b4
	.4byte 0x0000059a
	.4byte 0x000005c4
	.4byte 0x00000596
	.4byte 0x00000c0a
	.4byte 0x00000c10
	.4byte 0x00000c13
	.4byte 0x000005a4
	.4byte 0x00000c15
	.4byte 0x03001c94
	.4byte 0x0000057e
	.4byte 0x080367d0
	.4byte 0x080367d6
	.4byte 0x03001b04
	.4byte 0x00000594
	.4byte 0x02000240
	.4byte 0x00000205
	.4byte 0x00000595
	.4byte 0x0000022a
	.4byte 0x03001d08
	.4byte 0x00000206
