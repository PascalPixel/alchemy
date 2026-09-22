.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f0, 0x080072f0
	.set sub_08007300, 0x08007300
	.set sub_08007308, 0x08007308
	.set sub_08015268, 0x08015268
	.set sub_0808ba1c, 0x0808ba1c
	.global Overlay_0808f52c
Overlay_0808f52c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #612]
	ldr	r6, [r3, #0]
	ldr	r3, [r3, #92]
	sub	sp, #24
	ldr	r0, [pc, #608]
	str	r3, [sp, #20]
	adds	r4, r3, r0
	movs	r2, #0
	ldrsb	r2, [r4, r2]
	cmp	r2, #0
	beq.n	.L_0808f5fc
	ldr	r5, [pc, #596]
	adds	r1, r3, r5
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r0, [r1, #0]
	cmp	r3, r2
	blt.n	.L_0808f5c4
	movs	r3, #0
	strb	r3, [r4, #0]
	ldr	r7, [sp, #20]
	ldr	r0, [pc, #580]
	adds	r3, r7, r0
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	cmp	r2, #0
	bne.n	.L_0808f5b8
	ldr	r1, [pc, #572]
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #64
	bne.n	.L_0808f588
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #556]
	b.n	.L_0808f590
.L_0808f588:
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #552]
.L_0808f590:
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r0, [pc, #548]
	bl	sub_08004278
	ldr	r0, [pc, #548]
	bl	sub_08004278
	ldr	r2, [pc, #544]
	ldr	r3, [pc, #548]
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldr	r3, [pc, #544]
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	bl	.L_0808fe1e
.L_0808f5b8:
	ldr	r5, [sp, #20]
	movs	r7, #165
	lsls	r7, r7, #3
	adds	r3, r5, r7
	strh	r2, [r3, #0]
	b.n	.L_0808f5fc
.L_0808f5c4:
	ldr	r2, [sp, #20]
	ldr	r5, [pc, #488]
	adds	r3, r2, r5
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r7, [sp, #20]
	ldr	r3, [pc, #508]
	adds	r5, r7, r3
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	subs	r2, r2, r3
	adds	r3, r0, #1
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r0, r3, #0
	muls	r0, r2
	movs	r1, #0
	ldrsb	r1, [r4, r1]
	ldr	r3, [pc, #488]
	bl	sub_080072f0
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	ldr	r5, [pc, #480]
	adds	r3, r3, r0
	adds	r2, r7, r5
	strh	r3, [r2, #0]
.L_0808f5fc:
	ldr	r7, [sp, #20]
	ldr	r0, [pc, #476]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #5
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r4, r7, r3
	adds	r0, r4, #4
	str	r4, [sp, #0]
	bl	sub_08015268
	movs	r1, #165
	lsls	r1, r1, #3
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	ldr	r4, [sp, #0]
	cmp	r3, #77
	bls.n	.L_0808f62e
	bl	.L_0808fe10
.L_0808f62e:
	ldr	r2, [pc, #432]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0808f770
	.4byte 0x0808f818
	.4byte 0x0808f8e4
	.4byte 0x0808f958
	.4byte 0x0808f9ea
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fa52
	.4byte 0x0808fb3c
	.4byte 0x0808fc32
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.4byte 0x0808fe10
	.2byte 0xfd0e
	.2byte 0x0808
	ldr	r3, [pc, #36]
	strh	r3, [r4, #0]
	ldr	r3, [pc, #36]
	adds	r4, #2
	strh	r3, [r4, #0]
	ldr	r2, [sp, #20]
	ldr	r5, [pc, #88]
	adds	r3, r2, r5
	ldrh	r5, [r3, #0]
	movs	r2, #32
	adds	r3, r5, #0
	ands	r3, r2
	adds	r4, #2
	cmp	r3, #0
	beq.n	.L_0808f7e4
	movs	r3, #31
	ands	r3, r5
	subs	r5, r2, r3
	b.n	.L_0808f7e8
	movs	r0, r0
	.4byte 0x00007f7f
	.4byte 0x00000001
	.4byte 0x03001e70
	.4byte 0x0000053c
	.4byte 0x0000053d
	.4byte 0x0000053e
	.4byte 0x0000053b
	.4byte 0x000081ff
	.4byte 0x00009fff
	.4byte 0x0808f499
	.4byte 0x0808f52d
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x0000053a
	.4byte 0x03000380
	.4byte 0x0000052a
	.4byte 0x00000539
	.2byte 0xf638
	.2byte 0x0808
.L_0808f7e4:
	movs	r3, #31
	ands	r5, r3
.L_0808f7e8:
	ldr	r3, [pc, #284]
	ldrb	r5, [r3, r5]
	movs	r7, #0
	movs	r3, #241
	mov	r8, r7
	subs	r6, r3, r5
.L_0808f7f4:
	str	r4, [sp, #0]
	bl	sub_08004458
	adds	r3, r6, #0
	muls	r3, r0
	movs	r0, #1
	lsrs	r3, r3, #16
	lsls	r2, r3, #8
	ldr	r4, [sp, #0]
	adds	r3, r3, r5
	add	r8, r0
	orrs	r2, r3
	mov	r1, r8
	strh	r2, [r4, #0]
	adds	r4, #4
	cmp	r1, #159
	bls.n	.L_0808f7f4
	b.n	.L_0808fe10
	ldr	r2, [sp, #20]
	ldr	r5, [pc, #240]
	adds	r3, r2, r5
	ldrh	r5, [r3, #0]
	movs	r3, #31
	ands	r3, r5
	lsls	r2, r3, #3
	subs	r2, r2, r3
	ldr	r3, [pc, #228]
	lsls	r2, r2, #2
	adds	r2, r2, r3
	movs	r3, #32
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0808f83e
	ldrh	r3, [r2, #0]
	strh	r3, [r4, #0]
	ldrh	r3, [r2, #2]
	b.n	.L_0808f844
.L_0808f83e:
	ldrh	r3, [r2, #2]
	strh	r3, [r4, #0]
	ldrh	r3, [r2, #0]
.L_0808f844:
	adds	r4, #2
	strh	r3, [r4, #0]
	adds	r4, #2
	movs	r7, #0
	adds	r2, #4
	mov	r8, r7
	mov	sl, r2
.L_0808f852:
	mov	r2, sl
	ldrh	r7, [r2, #0]
	ldrh	r0, [r2, #2]
	cmp	r7, #0
	beq.n	.L_0808f8d4
	cmp	r0, #0
	beq.n	.L_0808f876
	movs	r1, #0
	mov	r9, r1
	cmp	r9, r7
	bge.n	.L_0808f8d4
.L_0808f868:
	movs	r2, #1
	add	r9, r2
	strh	r0, [r4, #0]
	adds	r4, #4
	cmp	r9, r7
	blt.n	.L_0808f868
	b.n	.L_0808f8d4
.L_0808f876:
	ldrb	r3, [r2, #4]
	ldrb	r6, [r2, #6]
	mov	fp, r3
	ldrb	r0, [r2, #7]
	ldrb	r3, [r2, #5]
	cmp	r7, #0
	beq.n	.L_0808f8d4
	mov	r5, fp
	subs	r5, r3, r5
	subs	r0, r0, r6
	str	r5, [sp, #16]
	str	r0, [sp, #12]
	movs	r2, #0
	movs	r3, #0
	mov	r9, r7
.L_0808f894:
	adds	r0, r3, #0
	adds	r1, r7, #0
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_080022ec
	ldr	r2, [sp, #8]
	adds	r5, r0, #0
	adds	r1, r7, #0
	adds	r0, r2, #0
	bl	sub_080022ec
	add	r5, fp
	adds	r0, r6, r0
	ldr	r4, [sp, #0]
	lsls	r5, r5, #8
	adds	r5, r5, r0
	strh	r5, [r4, #0]
	movs	r5, #1
	ldr	r0, [sp, #12]
	ldr	r2, [sp, #8]
	negs	r5, r5
	ldr	r3, [sp, #4]
	ldr	r1, [sp, #16]
	add	r9, r5
	adds	r2, r2, r0
	mov	r0, r9
	adds	r4, #4
	adds	r3, r3, r1
	cmp	r0, #0
	bne.n	.L_0808f894
.L_0808f8d4:
	movs	r2, #1
	add	r8, r2
	movs	r1, #8
	mov	r3, r8
	add	sl, r1
	cmp	r3, #2
	bls.n	.L_0808f852
	b.n	.L_0808fe10
	ldr	r5, [sp, #20]
	ldr	r7, [pc, #36]
	adds	r3, r5, r7
	ldrh	r3, [r3, #0]
	subs	r5, r3, #1
	movs	r3, #32
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0808f914
	ldr	r3, [pc, #8]
	strh	r3, [r4, #0]
	ldr	r3, [pc, #8]
	b.n	.L_0808f91a
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x00007f7f
	.4byte 0x0809e8ac
	.4byte 0x0000052a
	.2byte 0xf840
	.2byte 0x0809
.L_0808f914:
	ldr	r3, [pc, #40]
	strh	r3, [r4, #0]
	ldr	r3, [pc, #40]
.L_0808f91a:
	adds	r4, #2
	strh	r3, [r4, #0]
	adds	r4, #2
	movs	r3, #31
	ands	r5, r3
	movs	r0, #0
	mov	r8, r0
	lsls	r5, r5, #4
.L_0808f92a:
	str	r4, [sp, #0]
	bl	sub_08004458
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	adds	r0, r5, r0
	ldr	r4, [sp, #0]
	cmp	r0, #255
	bls.n	.L_0808f948
	movs	r0, #255
	b.n	.L_0808f948
	.4byte 0x00007f7f
	.2byte 0x0001
	.2byte 0x0000
.L_0808f948:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	strh	r0, [r4, #0]
	adds	r4, #4
	cmp	r2, #159
	bls.n	.L_0808f92a
	b.n	.L_0808fe10
	ldr	r5, [sp, #20]
	ldr	r7, [pc, #60]
	ldr	r2, [pc, #48]
	adds	r3, r5, r7
	ldrh	r5, [r3, #0]
	ldr	r3, [pc, #48]
	strh	r2, [r4, #0]
	adds	r4, #2
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r5, #32
	bls.n	.L_0808f97c
	strh	r3, [r4, #0]
	adds	r4, #2
	movs	r3, #64
	strh	r2, [r4, #0]
	subs	r5, r3, r5
	adds	r4, #2
.L_0808f97c:
	lsls	r3, r5, #2
	adds	r5, r3, r5
	adds	r3, r5, #0
	muls	r3, r5
	movs	r0, #0
	lsls	r3, r3, #16
	ldr	r7, [pc, #16]
	mov	fp, r3
	mov	r8, r0
	b.n	.L_0808f9a0
	.4byte 0x00007f7f
	.4byte 0x00000001
	.4byte 0x0000052a
	.2byte 0x01d8
	.2byte 0x0300
.L_0808f9a0:
	mov	r5, r8
	subs	r5, #80
	adds	r0, r5, #0
	muls	r0, r5
	mov	r1, fp
	lsls	r0, r0, #16
	str	r4, [sp, #0]
	subs	r0, r1, r0
	bl	sub_08007300
	movs	r3, #120
	asrs	r0, r0, #8
	subs	r6, r3, r0
	ldr	r4, [sp, #0]
	adds	r0, #120
	cmp	r6, #0
	bge.n	.L_0808f9c4
	movs	r6, #0
.L_0808f9c4:
	cmp	r0, #0
	bge.n	.L_0808f9ca
	movs	r0, #0
.L_0808f9ca:
	cmp	r6, #240
	ble.n	.L_0808f9d0
	movs	r6, #240
.L_0808f9d0:
	cmp	r0, #240
	ble.n	.L_0808f9d6
	movs	r0, #240
.L_0808f9d6:
	lsls	r3, r6, #8
	movs	r2, #1
	adds	r3, r3, r0
	add	r8, r2
	strh	r3, [r4, #0]
	mov	r3, r8
	adds	r4, #4
	cmp	r3, #159
	bls.n	.L_0808f9a0
	b.n	.L_0808fe10
	ldr	r5, [sp, #20]
	ldr	r7, [pc, #28]
	adds	r3, r5, r7
	ldrh	r5, [r3, #0]
	movs	r3, #32
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0808fa10
	ldr	r3, [pc, #8]
	strh	r3, [r4, #0]
	ldr	r3, [pc, #8]
	b.n	.L_0808fa16
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x00007f7f
	.2byte 0x052a
	.2byte 0x0000
.L_0808fa10:
	ldr	r3, [pc, #36]
	strh	r3, [r4, #0]
	ldr	r3, [pc, #36]
.L_0808fa16:
	adds	r4, #2
	strh	r3, [r4, #0]
	adds	r4, #2
	movs	r3, #31
	ands	r3, r5
	lsls	r2, r3, #4
	subs	r2, r2, r3
	lsls	r2, r2, #4
	lsrs	r5, r2, #5
	movs	r3, #240
	subs	r3, r3, r5
	movs	r0, #0
	lsls	r3, r3, #8
	mov	r8, r0
	adds	r3, #240
	b.n	.L_0808fa40
	movs	r0, r0
	.4byte 0x00007f7f
	.2byte 0x0001
	.2byte 0x0000
.L_0808fa40:
	movs	r1, #2
	add	r8, r1
	mov	r2, r8
	strh	r5, [r4, #0]
	strh	r3, [r4, #4]
	adds	r4, #8
	cmp	r2, #159
	bls.n	.L_0808fa40
	b.n	.L_0808fe10
	adds	r2, r6, #0
	adds	r2, #228
	ldr	r3, [pc, #812]
	ldr	r5, [r2, #0]
	ldr	r6, [r2, #4]
	ands	r5, r3
	ands	r6, r3
	movs	r7, #250
	ldr	r3, [pc, #804]
	lsls	r7, r7, #1
	adds	r3, r3, r7
	ldr	r0, [r3, #0]
	str	r4, [sp, #0]
	bl	sub_0808ba1c
	ldr	r3, [r0, #8]
	subs	r3, r3, r5
	ldr	r4, [sp, #0]
	cmp	r3, #0
	bge.n	.L_0808fa7e
	ldr	r1, [pc, #784]
	adds	r3, r3, r1
.L_0808fa7e:
	ldr	r2, [r0, #12]
	asrs	r7, r3, #16
	ldr	r3, [r0, #16]
	subs	r3, r3, r2
	subs	r0, r3, r6
	cmp	r0, #0
	bge.n	.L_0808fa90
	ldr	r2, [pc, #764]
	adds	r0, r0, r2
.L_0808fa90:
	asrs	r3, r0, #16
	subs	r3, #16
	ldr	r5, [sp, #20]
	mov	sl, r3
	ldr	r0, [pc, #756]
	ldr	r3, [pc, #760]
	ldr	r3, [r3, #0]
	adds	r3, r5, r0
	ldrh	r3, [r3, #0]
	ldr	r1, [pc, #756]
	strh	r3, [r4, #0]
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r4, #2
	ldr	r2, [pc, #748]
	strh	r3, [r4, #0]
	adds	r3, r5, r2
	ldrh	r5, [r3, #0]
	movs	r2, #32
	adds	r3, r5, #0
	ands	r3, r2
	adds	r4, #2
	cmp	r3, #0
	bne.n	.L_0808fac8
	movs	r3, #31
	ands	r3, r5
	subs	r5, r2, r3
	b.n	.L_0808facc
.L_0808fac8:
	movs	r3, #31
	ands	r5, r3
.L_0808facc:
	ldr	r3, [pc, #708]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0808fada
	movs	r5, #0
.L_0808fada:
	lsls	r3, r5, #2
	adds	r5, r3, r5
	adds	r3, r5, #0
	muls	r3, r5
	ldr	r5, [pc, #700]
	lsls	r3, r3, #16
	mov	fp, r3
	movs	r3, #0
	mov	r8, r3
	mov	r9, r5
.L_0808faee:
	mov	r0, r8
	mov	r1, sl
	subs	r5, r0, r1
	adds	r3, r5, #0
	muls	r3, r5
	lsls	r0, r3, #1
	adds	r0, r0, r3
	mov	r2, fp
	lsls	r0, r0, #15
	str	r4, [sp, #0]
	subs	r0, r2, r0
	bl	sub_08007308
	asrs	r0, r0, #8
	subs	r6, r7, r0
	ldr	r4, [sp, #0]
	adds	r0, r7, r0
	cmp	r6, #0
	bge.n	.L_0808fb16
	movs	r6, #0
.L_0808fb16:
	cmp	r0, #0
	bge.n	.L_0808fb1c
	movs	r0, #0
.L_0808fb1c:
	cmp	r6, #240
	ble.n	.L_0808fb22
	movs	r6, #240
.L_0808fb22:
	cmp	r0, #240
	ble.n	.L_0808fb28
	movs	r0, #240
.L_0808fb28:
	lsls	r3, r6, #8
	adds	r3, r3, r0
	strh	r3, [r4, #0]
	movs	r3, #1
	add	r8, r3
	mov	r5, r8
	adds	r4, #4
	cmp	r5, #159
	bls.n	.L_0808faee
	b.n	.L_0808fe10
	adds	r1, r6, #0
	adds	r1, #228
	ldr	r3, [pc, #576]
	ldr	r2, [r1, #0]
	ldr	r7, [sp, #20]
	ldr	r1, [r1, #4]
	ldr	r0, [pc, #600]
	ands	r2, r3
	ands	r1, r3
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_0808fb5c
	ldr	r2, [pc, #560]
	adds	r3, r3, r2
.L_0808fb5c:
	ldr	r5, [sp, #20]
	movs	r0, #166
	lsls	r0, r0, #3
	asrs	r7, r3, #16
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	subs	r1, r3, r1
	cmp	r1, #0
	bge.n	.L_0808fb72
	ldr	r2, [pc, #540]
	adds	r1, r1, r2
.L_0808fb72:
	asrs	r3, r1, #16
	subs	r3, #16
	lsls	r2, r3, #1
	mov	sl, r3
	ldr	r3, [pc, #536]
	ldr	r3, [r3, #0]
	ldr	r5, [sp, #20]
	ldr	r0, [pc, #532]
	subs	r3, r3, r2
	mov	r9, r3
	adds	r3, r5, r0
	ldrh	r3, [r3, #0]
	ldr	r1, [pc, #516]
	strh	r3, [r4, #0]
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r4, #2
	ldr	r2, [pc, #516]
	strh	r3, [r4, #0]
	adds	r3, r5, r2
	ldrh	r5, [r3, #0]
	movs	r2, #32
	adds	r3, r5, #0
	ands	r3, r2
	adds	r4, #2
	cmp	r3, #0
	beq.n	.L_0808fbb0
	movs	r3, #31
	ands	r3, r5
	subs	r5, r2, r3
	b.n	.L_0808fbb4
.L_0808fbb0:
	movs	r3, #31
	ands	r5, r3
.L_0808fbb4:
	lsls	r3, r5, #2
	adds	r5, r3, r5
	adds	r3, r5, #0
	muls	r3, r5
	lsls	r3, r3, #16
	mov	fp, r3
	movs	r3, #0
	mov	r8, r3
.L_0808fbc4:
	mov	r0, r8
	mov	r1, sl
	subs	r5, r0, r1
	adds	r3, r5, #0
	muls	r3, r5
	lsls	r0, r3, #1
	adds	r0, r0, r3
	mov	r2, fp
	lsls	r0, r0, #15
	str	r4, [sp, #0]
	subs	r0, r2, r0
	ldr	r3, [pc, #452]
	bl	sub_080072f0
	asrs	r0, r0, #8
	subs	r6, r7, r0
	adds	r0, r7, r0
	ldr	r4, [sp, #0]
	cmp	r6, r0
	bge.n	.L_0808fc02
	movs	r3, #31
	mov	r5, r9
	ldr	r1, [pc, #436]
	ands	r3, r5
	ldrsb	r3, [r1, r3]
	subs	r6, r6, r3
	adds	r0, r0, r3
	cmp	r6, r0
	blt.n	.L_0808fc02
	movs	r6, #240
	movs	r0, #240
.L_0808fc02:
	cmp	r6, #0
	bge.n	.L_0808fc08
	movs	r6, #0
.L_0808fc08:
	cmp	r0, #0
	bge.n	.L_0808fc0e
	movs	r0, #0
.L_0808fc0e:
	cmp	r6, #240
	ble.n	.L_0808fc14
	movs	r6, #240
.L_0808fc14:
	cmp	r0, #240
	ble.n	.L_0808fc1a
	movs	r0, #240
.L_0808fc1a:
	lsls	r3, r6, #8
	adds	r3, r3, r0
	strh	r3, [r4, #0]
	movs	r3, #1
	add	r8, r3
	movs	r2, #2
	mov	r5, r8
	adds	r4, #4
	add	r9, r2
	cmp	r5, #159
	bls.n	.L_0808fbc4
	b.n	.L_0808fe10
	adds	r2, r6, #0
	adds	r2, #228
	ldr	r3, [pc, #332]
	ldr	r5, [r2, #0]
	ldr	r6, [r2, #4]
	ands	r5, r3
	ands	r6, r3
	movs	r7, #250
	ldr	r3, [pc, #324]
	lsls	r7, r7, #1
	adds	r3, r3, r7
	ldr	r0, [r3, #0]
	str	r4, [sp, #0]
	bl	sub_0808ba1c
	ldr	r3, [r0, #8]
	subs	r3, r3, r5
	ldr	r4, [sp, #0]
	cmp	r3, #0
	bge.n	.L_0808fc5e
	ldr	r1, [pc, #304]
	adds	r3, r3, r1
.L_0808fc5e:
	ldr	r2, [r0, #12]
	asrs	r7, r3, #16
	ldr	r3, [r0, #16]
	subs	r3, r3, r2
	subs	r0, r3, r6
	cmp	r0, #0
	bge.n	.L_0808fc70
	ldr	r2, [pc, #284]
	adds	r0, r0, r2
.L_0808fc70:
	asrs	r3, r0, #16
	subs	r3, #16
	ldr	r5, [sp, #20]
	mov	sl, r3
	ldr	r0, [pc, #284]
	ldr	r3, [pc, #280]
	ldr	r3, [r3, #0]
	adds	r3, r5, r0
	ldrh	r3, [r3, #0]
	ldr	r1, [pc, #268]
	strh	r3, [r4, #0]
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r4, #2
	ldr	r2, [pc, #268]
	strh	r3, [r4, #0]
	adds	r3, r5, r2
	ldrh	r5, [r3, #0]
	movs	r2, #32
	adds	r3, r5, #0
	ands	r3, r2
	adds	r4, #2
	cmp	r3, #0
	beq.n	.L_0808fca8
	movs	r3, #31
	ands	r3, r5
	subs	r5, r2, r3
	b.n	.L_0808fcac
.L_0808fca8:
	movs	r3, #31
	ands	r5, r3
.L_0808fcac:
	lsls	r3, r5, #2
	adds	r5, r3, r5
	adds	r3, r5, #0
	muls	r3, r5
	ldr	r5, [pc, #232]
	lsls	r3, r3, #16
	mov	fp, r3
	movs	r3, #0
	mov	r8, r3
	mov	r9, r5
.L_0808fcc0:
	mov	r0, r8
	mov	r1, sl
	subs	r5, r0, r1
	adds	r3, r5, #0
	muls	r3, r5
	lsls	r0, r3, #1
	adds	r0, r0, r3
	mov	r2, fp
	lsls	r0, r0, #15
	str	r4, [sp, #0]
	subs	r0, r2, r0
	bl	sub_08007308
	asrs	r0, r0, #8
	subs	r6, r7, r0
	ldr	r4, [sp, #0]
	adds	r0, r7, r0
	cmp	r6, #0
	bge.n	.L_0808fce8
	movs	r6, #0
.L_0808fce8:
	cmp	r0, #0
	bge.n	.L_0808fcee
	movs	r0, #0
.L_0808fcee:
	cmp	r6, #240
	ble.n	.L_0808fcf4
	movs	r6, #240
.L_0808fcf4:
	cmp	r0, #240
	ble.n	.L_0808fcfa
	movs	r0, #240
.L_0808fcfa:
	lsls	r3, r6, #8
	adds	r3, r3, r0
	strh	r3, [r4, #0]
	movs	r3, #1
	add	r8, r3
	mov	r5, r8
	adds	r4, #4
	cmp	r5, #159
	bls.n	.L_0808fcc0
	b.n	.L_0808fe10
	adds	r2, r6, #0
	adds	r2, #228
	ldr	r3, [pc, #112]
	ldr	r5, [r2, #0]
	ldr	r6, [r2, #4]
	ands	r5, r3
	ands	r6, r3
	movs	r7, #250
	ldr	r3, [pc, #104]
	lsls	r7, r7, #1
	adds	r3, r3, r7
	ldr	r0, [r3, #0]
	str	r4, [sp, #0]
	bl	sub_0808ba1c
	ldr	r3, [r0, #8]
	subs	r3, r3, r5
	ldr	r4, [sp, #0]
	cmp	r3, #0
	bge.n	.L_0808fd3a
	ldr	r1, [pc, #84]
	adds	r3, r3, r1
.L_0808fd3a:
	ldr	r2, [r0, #12]
	asrs	r7, r3, #16
	ldr	r3, [r0, #16]
	subs	r3, r3, r2
	subs	r0, r3, r6
	cmp	r0, #0
	bge.n	.L_0808fd4c
	ldr	r2, [pc, #64]
	adds	r0, r0, r2
.L_0808fd4c:
	asrs	r3, r0, #16
	subs	r3, #8
	ldr	r5, [sp, #20]
	mov	sl, r3
	ldr	r0, [pc, #64]
	ldr	r3, [pc, #60]
	ldr	r3, [r3, #0]
	adds	r3, r5, r0
	ldrh	r3, [r3, #0]
	ldr	r1, [pc, #48]
	strh	r3, [r4, #0]
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r4, #2
	ldr	r2, [pc, #48]
	strh	r3, [r4, #0]
	adds	r3, r5, r2
	ldrh	r5, [r3, #0]
	movs	r2, #32
	adds	r3, r5, #0
	ands	r3, r2
	adds	r4, #2
	cmp	r3, #0
	beq.n	.L_0808fdac
	movs	r3, #31
	ands	r3, r5
	subs	r5, r2, r3
	b.n	.L_0808fdb0
	.4byte 0xffff0000
	.4byte 0x02000240
	.4byte 0x0000ffff
	.4byte 0x00000536
	.4byte 0x03001e40
	.4byte 0x00000534
	.4byte 0x0000052a
	.4byte 0x030001d8
	.4byte 0x0000052c
	.2byte 0xe8ce
	.2byte 0x0809
.L_0808fdac:
	movs	r3, #31
	ands	r5, r3
.L_0808fdb0:
	lsls	r3, r5, #2
	adds	r5, r3, r5
	adds	r3, r5, #0
	muls	r3, r5
	ldr	r5, [pc, #116]
	lsls	r3, r3, #16
	mov	fp, r3
	movs	r3, #0
	mov	r8, r3
	mov	r9, r5
.L_0808fdc4:
	mov	r0, r8
	mov	r1, sl
	subs	r5, r0, r1
	adds	r3, r5, #0
	muls	r3, r5
	lsls	r0, r3, #1
	adds	r0, r0, r3
	mov	r2, fp
	lsls	r0, r0, #14
	str	r4, [sp, #0]
	subs	r0, r2, r0
	bl	sub_08007308
	asrs	r0, r0, #8
	subs	r6, r7, r0
	ldr	r4, [sp, #0]
	adds	r0, r7, r0
	cmp	r6, #0
	bge.n	.L_0808fdec
	movs	r6, #0
.L_0808fdec:
	cmp	r0, #0
	bge.n	.L_0808fdf2
	movs	r0, #0
.L_0808fdf2:
	cmp	r6, #240
	ble.n	.L_0808fdf8
	movs	r6, #240
.L_0808fdf8:
	cmp	r0, #240
	ble.n	.L_0808fdfe
	movs	r0, #240
.L_0808fdfe:
	lsls	r3, r6, #8
	adds	r3, r3, r0
	strh	r3, [r4, #0]
	movs	r3, #1
	add	r8, r3
	mov	r5, r8
	adds	r4, #4
	cmp	r5, #159
	bls.n	.L_0808fdc4
.L_0808fe10:
	ldr	r7, [sp, #20]
	ldr	r0, [pc, #32]
	adds	r3, r7, r0
	ldrb	r2, [r3, #0]
	movs	r1, #1
	eors	r2, r1
	strb	r2, [r3, #0]
.L_0808fe1e:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
