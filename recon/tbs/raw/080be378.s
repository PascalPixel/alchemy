.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080030f8, 0x080030f8
	.set sub_08009080, 0x08009080
	.set sub_08009088, 0x08009088
	.set sub_08015118, 0x08015118
	.set sub_08015120, 0x08015120
	.set sub_080151c8, 0x080151c8
	.set sub_08077000, 0x08077000
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_08077018, 0x08077018
	.set sub_08077078, 0x08077078
	.set sub_08077080, 0x08077080
	.set sub_080770c0, 0x080770c0
	.set sub_08077118, 0x08077118
	.set sub_08077128, 0x08077128
	.set sub_08077160, 0x08077160
	.set sub_08077170, 0x08077170
	.set sub_08077178, 0x08077178
	.set sub_080771a0, 0x080771a0
	.set sub_080771b0, 0x080771b0
	.set sub_080771c0, 0x080771c0
	.set sub_080771c8, 0x080771c8
	.set sub_080771e0, 0x080771e0
	.set sub_080771e8, 0x080771e8
	.set sub_08077208, 0x08077208
	.set sub_08077210, 0x08077210
	.set sub_080772b8, 0x080772b8
	.set sub_080772f8, 0x080772f8
	.set sub_080b7dd0, 0x080b7dd0
	.set sub_080b9a44, 0x080b9a44
	.set sub_080bb65c, 0x080bb65c
	.set sub_080bb8d8, 0x080bb8d8
	.set sub_080bb938, 0x080bb938
	.set sub_080bbabc, 0x080bbabc
	.set sub_080bd3c8, 0x080bd3c8
	.set sub_080bd808, 0x080bd808
	.set sub_080bdfec, 0x080bdfec
	.set sub_080be02c, 0x080be02c
	.set sub_080be070, 0x080be070
	.set sub_080be0b4, 0x080be0b4
	.set sub_080be18c, 0x080be18c
	.set sub_080be244, 0x080be244
	.set sub_080c10e8, 0x080c10e8
	.set sub_080c1798, 0x080c1798
	.set sub_080c23e8, 0x080c23e8
	.set sub_080f9010, 0x080f9010
	.global FunctionHead_080be378
	.global Func_080be378
	.thumb_func
FunctionHead_080be378:
Func_080be378:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	mov	r3, sp
	add	r2, sp, #32
	adds	r3, #44
	str	r0, [r2, #0]
	str	r3, [sp, #8]
	str	r1, [r3, #0]
	ldr	r3, [r2, #0]
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	mov	sl, r2
	bl	sub_08077008
	mov	r1, sp
	adds	r1, #28
	str	r1, [sp, #12]
	ldr	r3, [pc, #912]
	ldr	r3, [r3, #0]
	add	r7, sp, #36
	mov	r2, sl
	str	r3, [r7, #0]
	ldr	r3, [r2, #0]
	str	r0, [r1, #0]
	movs	r4, #10
	ldrsh	r0, [r3, r4]
	bl	sub_080b9a44
	str	r0, [sp, #40]
	bl	sub_080bdfec
	mov	r4, sl
	ldr	r0, [sp, #8]
	ldr	r3, [r4, #0]
	ldr	r1, [r0, #0]
	ldrh	r3, [r3, #0]
	movs	r2, #0
	movs	r5, #4
	strb	r3, [r1, #0]
	str	r2, [r1, #96]
	strb	r2, [r1, #1]
	str	r2, [r1, #88]
	str	r2, [r1, #92]
	str	r5, [r1, #80]
	bl	sub_08015118
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #0]
	movs	r1, #56
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_080be3f0
	bl	.L_080bec5c
.L_080be3f0:
	ldr	r3, [pc, #840]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080be46a
	ldr	r0, [pc, #836]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080be46a
	ldr	r1, [pc, #832]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080be46a
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r5
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_080be420
	movs	r3, #0
	mov	r8, r3
.L_080be420:
	movs	r6, #100
	b.n	.L_080be448
.L_080be424:
	cmp	r5, #254
	beq.n	.L_080be446
	movs	r1, #192
	adds	r0, r5, #0
	lsls	r1, r1, #24
	bl	sub_08077118
	cmp	r0, #0
	bne.n	.L_080be446
	adds	r1, r5, #0
	movs	r0, #8
	bl	sub_080bbabc
	movs	r0, #9
	adds	r1, r5, #0
	bl	sub_080bbabc
.L_080be446:
	adds	r6, #2
.L_080be448:
	mov	r4, r8
	cmp	r4, #0
	beq.n	.L_080be456
	ldr	r3, [r7, #0]
	adds	r3, #2
	ldrsh	r5, [r3, r6]
	b.n	.L_080be45e
.L_080be456:
	ldr	r2, [r7, #0]
	adds	r3, r6, #0
	subs	r3, #12
	ldrsh	r5, [r2, r3]
.L_080be45e:
	cmp	r5, #255
	bne.n	.L_080be424
	bl	sub_080bb938
	bl	.L_080bec5c
.L_080be46a:
	bl	sub_08015118
	ldr	r3, [sp, #12]
	ldr	r4, [pc, #724]
	ldr	r2, [r3, #0]
	adds	r1, r2, r4
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080be498
	movs	r3, #0
	mov	r0, sl
	strb	r3, [r1, #0]
	ldr	r3, [r0, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #700]
	bl	sub_080151c8
	bl	.L_080bec8a
.L_080be498:
	movs	r4, #158
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080be4bc
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #668]
	bl	sub_080151c8
	bl	.L_080bec8a
.L_080be4bc:
	ldr	r4, [pc, #660]
	adds	r3, r2, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080be4dc
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #640]
	bl	sub_080151c8
	b.n	.L_080bec8a
.L_080be4dc:
	movs	r4, #152
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080be51a
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #6
	ldrsh	r3, [r3, r1]
	cmp	r3, #3
	beq.n	.L_080be51a
	bl	sub_080771a0
	movs	r3, #3
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_080be51a
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r1, #1
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl	sub_08015120
	ldr	r0, [pc, #584]
	bl	sub_080151c8
	b.n	.L_080bec8a
.L_080be51a:
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #6
	ldrsh	r3, [r3, r1]
	cmp	r3, #8
	bne.n	.L_080be528
	b.n	.L_080bec5c
.L_080be528:
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #0]
	movs	r2, #1
	mov	fp, r2
	movs	r1, #0
	adds	r3, #44
	movs	r2, #13
.L_080be536:
	subs	r2, #1
	strb	r1, [r3, #0]
	adds	r3, #1
	cmp	r2, #0
	bge.n	.L_080be536
	ldr	r0, [sp, #8]
	movs	r2, #1
	ldr	r3, [r0, #0]
	negs	r2, r2
	adds	r1, r2, #0
	adds	r3, #58
	movs	r2, #13
.L_080be54e:
	subs	r2, #1
	strb	r1, [r3, #0]
	adds	r3, #1
	cmp	r2, #0
	bge.n	.L_080be54e
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r0, #6
	ldrsh	r3, [r3, r0]
	cmp	r3, #99
	bls.n	.L_080be568
	bl	.L_080bee00
.L_080be568:
	ldr	r2, [pc, #500]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080be76c
	.4byte 0x080be7d0
	.4byte 0x080be888
	.4byte 0x080be96e
	.4byte 0x080be984
	.4byte 0x080beb08
	.4byte 0x080becea
	.4byte 0x080be96e
	.4byte 0x080bec5c
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.4byte 0x080bee00
	.2byte 0xe700
	.2byte 0x080b
	mov	r1, sl
	ldr	r3, [r1, #0]
	ldrh	r3, [r3, #0]
	movs	r2, #224
	lsls	r0, r3, #16
	lsls	r2, r2, #11
	cmp	r0, r2
	bhi.n	.L_080be718
	ldr	r0, [pc, #80]
	bl	sub_080151c8
	b.n	.L_080be726
.L_080be718:
	asrs	r0, r0, #16
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #68]
	bl	sub_080151c8
.L_080be726:
	bl	sub_080bb65c
	ldr	r3, [sp, #8]
	ldr	r2, [r3, #0]
	movs	r3, #7
	str	r3, [r2, #84]
	bl	.L_080bf1d4
	movs	r0, r0
	subs	r4, r6, #1
	lsls	r0, r0, #12
	subs	r4, r2, #5
	lsls	r0, r0, #12
	lsls	r5, r5, #5
	movs	r0, r0
	subs	r0, r5, r3
	lsls	r0, r0, #12
	lsls	r5, r0, #5
	movs	r0, r0
	lsrs	r0, r0, #2
	movs	r0, r0
	lsrs	r0, r3, #1
	movs	r0, r0
	lsls	r3, r7, #4
	movs	r0, r0
	lsrs	r7, r2, #1
	movs	r0, r0
	lsrs	r1, r3, #1
	movs	r0, r0
	b.n	sub_080be244
	.2byte 0x080b
	.4byte 0x00000843
	.2byte 0x0846
	.2byte 0x0000
	ldr	r4, [sp, #12]
	ldr	r0, [r4, #0]
	bl	sub_08077160
	mov	fp, r0
	add	r0, sp, #48
	mov	r9, r0
	mov	r0, fp
	bl	sub_080be18c
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_080be78c
	bl	.L_080bf1d6
.L_080be78c:
	mov	r2, fp
	cmp	r2, #1
	bne.n	.L_080be794
	b.n	.L_080bee08
.L_080be794:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	ldr	r2, [sp, #12]
	movs	r1, #1
	ldr	r0, [r2, #0]
	bl	sub_08077078
	movs	r1, #2
	bl	sub_08015120
	ldr	r5, [pc, #744]
	adds	r0, r5, #0
	bl	sub_080151c8
	adds	r5, #1
	bl	sub_080bb8d8
	mov	r0, fp
	movs	r1, #4
	bl	sub_08015120
	adds	r0, r5, #0
.L_080be7ca:
	bl	sub_080151c8
	b.n	.L_080bee00
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #8
	ldrsh	r0, [r3, r1]
	mov	fp, r0
	bl	sub_08077080
	add	r2, sp, #48
	adds	r6, r0, #0
	mov	r9, r2
	mov	r0, fp
	bl	sub_080be18c
	movs	r3, #1
	negs	r3, r3
	movs	r5, #1
	cmp	r0, r3
	bne.n	.L_080be7f8
	bl	.L_080bf1d6
.L_080be7f8:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	movs	r1, #4
	mov	r0, fp
	bl	sub_08015120
	ldr	r0, [pc, #656]
	bl	sub_080151c8
	ldr	r2, [sp, #12]
	ldr	r1, [r2, #0]
	movs	r3, #58
	ldrsh	r2, [r1, r3]
	ldrb	r3, [r6, #9]
	cmp	r2, r3
	bge.n	.L_080be82c
	ldr	r4, [sp, #8]
	ldr	r2, [r4, #0]
	movs	r3, #2
	str	r3, [r2, #92]
	movs	r5, #0
.L_080be82c:
	ldr	r0, [pc, #628]
	adds	r3, r1, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080be840
	ldr	r1, [sp, #8]
	ldr	r2, [r1, #0]
	movs	r3, #1
	str	r3, [r2, #92]
	movs	r5, #0
.L_080be840:
	cmp	r5, #0
	bne.n	.L_080be846
	b.n	.L_080bee00
.L_080be846:
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #0]
	movs	r5, #0
	str	r5, [r3, #92]
	ldr	r3, [sp, #12]
	ldr	r1, [r3, #0]
	ldrb	r2, [r6, #9]
	ldrh	r3, [r1, #58]
	mov	r4, sl
	subs	r3, r3, r2
	strh	r3, [r1, #58]
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_08077128
	ldr	r2, [sp, #12]
	ldr	r1, [r2, #0]
	movs	r4, #58
	ldrsh	r3, [r1, r4]
	cmp	r3, #0
	bge.n	.L_080be874
	strh	r5, [r1, #58]
.L_080be874:
	movs	r0, #58
	ldrsh	r2, [r1, r0]
	movs	r4, #54
	ldrsh	r3, [r1, r4]
	ldrh	r0, [r1, #54]
	cmp	r2, r3
	bgt.n	.L_080be884
	b.n	.L_080bee00
.L_080be884:
	strh	r0, [r1, #58]
	b.n	.L_080bee00
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #8
	ldrsh	r2, [r3, r1]
	cmp	r2, #0
	bge.n	.L_080be8a6
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #520]
	bl	sub_080151c8
	b.n	.L_080bec8a
.L_080be8a6:
	ldr	r4, [sp, #12]
	lsls	r2, r2, #1
	ldr	r3, [r4, #0]
	adds	r2, #216
	ldrh	r0, [r3, r2]
	bl	sub_08077018
	adds	r5, r0, #0
	ldrh	r0, [r5, #40]
	mov	fp, r0
	cmp	r0, #0
	beq.n	.L_080be8dc
	ldr	r1, [sp, #12]
	mov	r3, sl
	ldr	r2, [r1, #0]
	ldr	r1, [r3, #0]
	movs	r4, #8
	ldrsh	r3, [r1, r4]
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r2, [r2, r3]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080be908
	b.n	.L_080be8e0
.L_080be8dc:
	mov	r0, sl
	ldr	r1, [r0, #0]
.L_080be8e0:
	movs	r2, #0
	ldrsh	r0, [r1, r2]
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #448]
	bl	sub_080151c8
	ldr	r4, [sp, #12]
	ldr	r0, [pc, #444]
	ldr	r3, [r4, #0]
	adds	r2, r3, r0
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_080be902
	b.n	.L_080bec8a
.L_080be902:
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_080bec8a
.L_080be908:
	add	r1, sp, #48
	mov	r9, r1
	mov	r0, fp
	bl	sub_080be18c
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_080be91e
	bl	.L_080bf1d6
.L_080be91e:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	ldr	r3, [sp, #12]
	mov	r4, sl
	ldr	r2, [r3, #0]
	ldr	r3, [r4, #0]
	movs	r0, #8
	ldrsh	r3, [r3, r0]
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r0, [r2, r3]
	movs	r1, #2
	bl	sub_08015120
	ldrb	r3, [r5, #12]
	cmp	r3, #2
	beq.n	.L_080be94e
	cmp	r3, #0
	bne.n	.L_080be96a
.L_080be94e:
	ldrb	r0, [r5, #2]
	cmp	r0, #3
	beq.n	.L_080be966
	cmp	r0, #3
	bgt.n	.L_080be95e
	cmp	r0, #1
	beq.n	.L_080be966
	b.n	.L_080be96a
.L_080be95e:
	cmp	r0, #8
	bgt.n	.L_080be96a
	cmp	r0, #6
	blt.n	.L_080be96a
.L_080be966:
	ldr	r0, [pc, #332]
	b.n	.L_080be7ca
.L_080be96a:
	ldr	r0, [pc, #332]
	b.n	.L_080be7ca
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_08015120
	ldr	r0, [pc, #300]
	bl	sub_080151c8
	b.n	.L_080bec8a
	mov	r4, sl
	ldr	r3, [r4, #0]
	add	r2, sp, #48
	movs	r1, #8
	ldrsh	r0, [r3, r1]
	mov	r9, r2
	mov	fp, r0
	bl	sub_080be18c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_080be9a2
	bl	.L_080bf1d6
.L_080be9a2:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	mov	r0, fp
	movs	r1, #4
	bl	sub_08015120
	mov	r0, fp
	bl	sub_08077080
	ldrb	r2, [r0, #1]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #6
	bne.n	.L_080be9cc
	ldr	r0, [pc, #240]
	b.n	.L_080be9ce
.L_080be9cc:
	ldr	r0, [pc, #240]
.L_080be9ce:
	movs	r3, #244
	lsls	r3, r3, #1
	cmp	fp, r3
	beq.n	.L_080bea84
	cmp	fp, r3
	bgt.n	.L_080bea12
	ldr	r2, [pc, #232]
	cmp	fp, r2
	bgt.n	.L_080bea00
	subs	r3, #52
	cmp	fp, r3
	bgt.n	.L_080bea78
	mov	r4, fp
	cmp	r4, #224
	beq.n	.L_080bea64
	cmp	r4, #224
	bge.n	.L_080be9f2
	b.n	.L_080be7ca
.L_080be9f2:
	movs	r1, #217
	lsls	r1, r1, #1
	cmp	fp, r1
	bgt.n	.L_080be9fc
	b.n	.L_080be7ca
.L_080be9fc:
	ldr	r0, [pc, #200]
	b.n	.L_080be7ca
.L_080bea00:
	movs	r2, #222
	lsls	r2, r2, #1
	cmp	fp, r2
	ble.n	.L_080bea7c
	movs	r3, #236
	lsls	r3, r3, #1
	cmp	fp, r3
	beq.n	.L_080bea80
	b.n	.L_080be7ca
.L_080bea12:
	movs	r3, #250
	lsls	r3, r3, #1
	cmp	fp, r3
	beq.n	.L_080bea68
	cmp	fp, r3
	bgt.n	.L_080bea40
	subs	r3, #6
	cmp	fp, r3
	beq.n	.L_080bea74
	cmp	fp, r3
	bgt.n	.L_080bea32
	movs	r4, #246
	lsls	r4, r4, #1
	cmp	fp, r4
	beq.n	.L_080bea88
	b.n	.L_080be7ca
.L_080bea32:
	ldr	r1, [pc, #152]
	cmp	fp, r1
	beq.n	.L_080bea8c
	ldr	r2, [pc, #148]
	cmp	fp, r2
	beq.n	.L_080bea70
	b.n	.L_080be7ca
.L_080bea40:
	ldr	r3, [pc, #144]
	cmp	fp, r3
	beq.n	.L_080bea90
	cmp	fp, r3
	bgt.n	.L_080bea52
	subs	r3, #2
	cmp	fp, r3
	beq.n	.L_080bea6c
	b.n	.L_080be7ca
.L_080bea52:
	movs	r4, #252
	lsls	r4, r4, #1
	cmp	fp, r4
	beq.n	.L_080bea94
	movs	r1, #254
	lsls	r1, r1, #1
	cmp	fp, r1
	beq.n	.L_080bea98
	b.n	.L_080be7ca
.L_080bea64:
	ldr	r0, [pc, #56]
	b.n	.L_080be7ca
.L_080bea68:
	ldr	r0, [pc, #108]
	b.n	.L_080be7ca
.L_080bea6c:
	ldr	r0, [pc, #108]
	b.n	.L_080be7ca
.L_080bea70:
	ldr	r0, [pc, #108]
	b.n	.L_080be7ca
.L_080bea74:
	ldr	r0, [pc, #108]
	b.n	.L_080be7ca
.L_080bea78:
	ldr	r0, [pc, #108]
	b.n	.L_080be7ca
.L_080bea7c:
	ldr	r0, [pc, #64]
	b.n	.L_080be7ca
.L_080bea80:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
.L_080bea84:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
.L_080bea88:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
.L_080bea8c:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
.L_080bea90:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
.L_080bea94:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
.L_080bea98:
	ldr	r0, [pc, #104]
	b.n	.L_080be7ca
	.4byte 0x00000819
	.4byte 0x0000083e
	.4byte 0x0000013d
	.4byte 0x0000081b
	.4byte 0x00000816
	.4byte 0x0000012b
	.4byte 0x00000818
	.4byte 0x00000817
	.4byte 0x000008f1
	.4byte 0x000008f0
	.4byte 0x000001b9
	.4byte 0x000008f2
	.4byte 0x000001ef
	.4byte 0x000001f3
	.4byte 0x000001f7
	.4byte 0x000008f7
	.4byte 0x000008f8
	.4byte 0x000008f9
	.4byte 0x000008fa
	.4byte 0x000008fb
	.4byte 0x000008fc
	.4byte 0x000008fd
	.4byte 0x000008ff
	.4byte 0x000008fe
	.4byte 0x00000900
	.4byte 0x00000901
	.2byte 0x0902
	.2byte 0x0000
	mov	r2, sl
	ldr	r3, [r2, #0]
	ldrh	r3, [r3, #8]
	ldr	r6, [pc, #52]
	lsls	r0, r3, #16
	movs	r5, #255
	asrs	r0, r0, #24
	adds	r1, r5, #0
	ands	r1, r3
	ands	r0, r6
	bl	sub_080771e8
	mov	r4, sl
	ldr	r3, [r4, #0]
	mov	fp, r0
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldrh	r3, [r3, #8]
	lsls	r1, r3, #16
	asrs	r1, r1, #24
	adds	r2, r5, #0
	ands	r1, r6
	ands	r2, r3
	bl	sub_08077208
	cmp	r0, #0
	beq.n	.L_080beb40
	b.n	.L_080bec90
.L_080beb40:
	b.n	.L_080beb48
	movs	r0, r0
	.2byte 0x000f
	.2byte 0x0000
.L_080beb48:
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	ldrh	r3, [r3, #8]
	lsls	r1, r3, #16
	asrs	r1, r1, #24
	adds	r2, r5, #0
	ands	r1, r6
	ands	r2, r3
	bl	sub_08077210
	cmp	r0, #0
	bne.n	.L_080beb66
	b.n	.L_080bec62
.L_080beb66:
	mov	r0, fp
	bl	sub_08077080
	movs	r1, #0
	movs	r0, #0
	bl	sub_080c10e8
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldrh	r3, [r3, #8]
	lsls	r1, r3, #16
	asrs	r1, r1, #24
	adds	r2, r5, #0
	ands	r2, r3
	ands	r1, r6
	bl	sub_080771b0
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	ldrh	r3, [r3, #8]
	lsls	r1, r3, #16
	asrs	r1, r1, #24
	adds	r2, r5, #0
	ands	r1, r6
	ands	r2, r3
	bl	sub_080771c0
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_08077010
	bl	sub_080bdfec
	movs	r0, #30
	bl	sub_080bd808
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r0, #0
	movs	r4, #0
	ldrsh	r1, [r3, r4]
	bl	sub_080bbabc
	mov	r0, sl
	ldr	r3, [r0, #0]
	ldrh	r2, [r3, #8]
	lsls	r3, r2, #16
	asrs	r3, r3, #24
	ands	r3, r6
	lsls	r1, r3, #2
	adds	r1, r1, r3
	adds	r3, r5, #0
	ands	r3, r2
	lsls	r1, r1, #2
	movs	r2, #150
	lsls	r2, r2, #1
	adds	r1, r1, r3
	adds	r1, r1, r2
	movs	r0, #3
	bl	sub_080bbabc
	movs	r1, #175
	movs	r0, #14
	bl	sub_080bbabc
	movs	r1, #0
	movs	r0, #10
	bl	sub_080bbabc
	ldr	r1, [pc, #856]
	movs	r0, #4
	bl	sub_080bbabc
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	movs	r0, #11
	bl	sub_080bbabc
	movs	r0, #212
	bl	sub_080f9010
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_080b7dd0
	movs	r1, #3
	ldr	r0, [r0, #0]
	bl	sub_08009080
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_080b7dd0
	movs	r1, #32
	ldr	r0, [r0, #0]
	bl	sub_08009088
	mov	r2, sl
	ldr	r3, [r2, #0]
	ldrh	r1, [r3, #8]
	lsls	r1, r1, #16
	asrs	r1, r1, #24
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	ands	r1, r6
	movs	r2, #3
	movs	r3, #0
	bl	sub_080c1798
	bl	sub_080be02c
.L_080bec5c:
	movs	r0, #2
	negs	r0, r0
	b.n	.L_080bf1d6
.L_080bec62:
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	movs	r1, #4
	mov	r0, fp
	bl	sub_08015120
	movs	r0, #114
	bl	sub_080f9010
	ldr	r0, [pc, #732]
	bl	sub_080151c8
	movs	r0, #60
	bl	sub_080030f8
.L_080bec8a:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080bf1d6
.L_080bec90:
	add	r2, sp, #48
	mov	r9, r2
	mov	r0, fp
	bl	sub_080be18c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_080beca4
	b.n	.L_080bf1d6
.L_080beca4:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldrh	r3, [r3, #8]
	lsls	r1, r3, #16
	adds	r2, r5, #0
	asrs	r1, r1, #24
	ands	r2, r3
	ands	r1, r6
	bl	sub_080771c8
	mov	r0, fp
	bl	sub_08077080
	mov	r2, sl
	ldr	r3, [r2, #0]
	adds	r5, r0, #0
	movs	r1, #1
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl	sub_08015120
	mov	r0, fp
	movs	r1, #4
	bl	sub_08015120
	ldr	r0, [pc, #644]
	bl	sub_080151c8
	ldr	r0, [sp, #8]
	ldrb	r3, [r5, #2]
	ldr	r2, [r0, #0]
	str	r3, [r2, #80]
	b.n	.L_080bee00
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r2, #8
	ldrsh	r0, [r3, r2]
	bl	sub_080771e0
	mov	r4, sl
	movs	r2, #24
	ldr	r3, [r4, #0]
	add	r2, sp
	mov	r8, r2
	mov	r9, r0
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	mov	r1, r8
	bl	sub_080be0b4
	mov	r4, sl
	ldr	r3, [r4, #0]
	ldrh	r3, [r3, #0]
	movs	r0, #0
	cmp	r3, #7
	bls.n	.L_080bed1a
	movs	r0, #1
.L_080bed1a:
	bl	sub_08077000
	adds	r0, #8
	str	r0, [sp, #4]
	mov	r1, r9
	adds	r1, #4
	mov	r0, r8
	ldrb	r2, [r0, #0]
	ldrb	r3, [r1, #0]
	movs	r7, #0
	cmp	r2, r3
	bcc.n	.L_080bed56
	movs	r5, #4
	mov	r6, r8
	movs	r4, #4
.L_080bed38:
	mov	r2, r9
	ldrb	r3, [r2, r5]
	adds	r7, #1
	strb	r3, [r0, #0]
	adds	r4, #1
	adds	r0, #1
	cmp	r7, #3
	bgt.n	.L_080bed56
	adds	r6, #1
	adds	r1, #1
	ldrb	r2, [r6, #0]
	ldrb	r3, [r1, #0]
	adds	r5, r4, #0
	cmp	r2, r3
	bcs.n	.L_080bed38
.L_080bed56:
	mov	r3, r9
	ldrh	r3, [r3, #0]
	add	r4, sp, #48
	mov	fp, r3
	mov	r9, r4
	mov	r0, fp
	bl	sub_080be18c
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	bne.n	.L_080bed70
	b.n	.L_080bf1d6
.L_080bed70:
	cmp	r7, #4
	beq.n	.L_080bed94
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08015120
	mov	r0, fp
	movs	r1, #4
	bl	sub_08015120
	ldr	r0, [pc, #472]
	bl	sub_080151c8
	adds	r0, r5, #0
	b.n	.L_080bf1d6
.L_080bed94:
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r1, #1
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl	sub_08015120
	movs	r1, #4
	mov	r0, fp
	bl	sub_08015120
	ldr	r0, [pc, #444]
	bl	sub_080151c8
	movs	r1, #128
	ldr	r0, [sp, #4]
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_080bee00
	mov	r9, r5
	adds	r5, r0, #0
.L_080bedc4:
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, r9
	bne.n	.L_080bedee
	ldrb	r0, [r5, #2]
	bl	sub_080be070
	cmp	r0, #0
	beq.n	.L_080bedee
	ldrb	r1, [r5, #0]
	mov	r3, r8
	ldrb	r2, [r3, r1]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080bedee
	movs	r3, #254
	strb	r3, [r5, #3]
	adds	r3, r2, #0
	adds	r3, #255
	mov	r4, r8
	strb	r3, [r4, r1]
.L_080bedee:
	ldr	r0, [sp, #4]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	adds	r7, #1
	adds	r5, #4
	cmp	r7, r3
	bne.n	.L_080bedc4
.L_080bee00:
	mov	r2, fp
	cmp	r2, #1
	beq.n	.L_080bee08
	b.n	.L_080befb4
.L_080bee08:
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #0]
	ldrb	r0, [r3, #2]
	bl	sub_08077008
	adds	r6, r0, #0
	ldr	r0, [sp, #8]
	ldr	r2, [r0, #0]
	mov	r1, sl
	movs	r3, #1
	str	r3, [r2, #76]
	ldr	r3, [r1, #0]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_08077170
	ldr	r3, [sp, #8]
	ldr	r1, [r3, #0]
	movs	r3, #2
	str	r0, [r1, #80]
	str	r3, [r1, #84]
	ldr	r4, [sp, #12]
	ldr	r0, [pc, #308]
	ldr	r2, [r4, #0]
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080bee58
	movs	r1, #148
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r0, [r3, #0]
	bl	sub_080c23e8
	ldr	r3, [sp, #8]
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	orrs	r3, r0
	b.n	.L_080beea6
.L_080bee58:
	movs	r3, #0
	movs	r4, #148
	str	r3, [r1, #88]
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #5
	bhi.n	.L_080beea8
	ldr	r2, [pc, #260]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080beea0
	.4byte 0x080bee88
	.4byte 0x080bee90
	.4byte 0x080bee98
	.4byte 0x080beea8
	.2byte 0xeea0
	.2byte 0x080b
	ldr	r1, [sp, #8]
	ldr	r3, [pc, #232]
	ldr	r2, [r1, #0]
	b.n	.L_080beea6
	ldr	r3, [sp, #8]
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #224]
	b.n	.L_080beea6
	ldr	r4, [sp, #8]
	ldr	r3, [pc, #220]
	ldr	r2, [r4, #0]
	b.n	.L_080beea6
	ldr	r0, [sp, #8]
	ldr	r3, [pc, #208]
	ldr	r2, [r0, #0]
.L_080beea6:
	str	r3, [r2, #88]
.L_080beea8:
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_08015120
	ldr	r0, [pc, #196]
	bl	sub_080151c8
	b.n	.L_080beef4
.L_080beebe:
	ldr	r4, [sp, #12]
	movs	r0, #156
	ldr	r3, [r4, #0]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080beee0
	bl	sub_080771a0
	movs	r3, #255
	ands	r0, r3
	cmp	r0, #152
	bgt.n	.L_080beee0
	ldr	r1, [sp, #8]
	ldr	r3, [r1, #0]
	strb	r5, [r3, #30]
.L_080beee0:
	bl	sub_080771a0
	movs	r3, #31
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_080bef28
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #0]
	strb	r0, [r3, #30]
	b.n	.L_080bef28
.L_080beef4:
	movs	r4, #56
	ldrsh	r3, [r6, r4]
	cmp	r3, #0
	beq.n	.L_080bef28
	movs	r0, #158
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080bef28
	ldr	r1, [pc, #116]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080bef28
	ldr	r2, [pc, #112]
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080bef28
	movs	r4, #157
	lsls	r4, r4, #1
	adds	r3, r6, r4
	ldrb	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L_080beebe
.L_080bef28:
	movs	r0, #183
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080bef3c
	ldr	r0, [sp, #8]
	ldr	r2, [r0, #0]
	movs	r3, #0
	strb	r3, [r2, #30]
.L_080bef3c:
	movs	r1, #56
	ldrsh	r3, [r6, r1]
	cmp	r3, #0
	bne.n	.L_080bef46
	b.n	.L_080bf1a8
.L_080bef46:
	bl	sub_080771a0
	movs	r3, #31
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_080bef88
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #0]
	b.n	.L_080befac
	.4byte 0x00000897
	.4byte 0x0000085b
	.4byte 0x0000083f
	.4byte 0x00000842
	.4byte 0x00000841
	.4byte 0x00000129
	.4byte 0x080bee70
	.4byte 0x00004001
	.4byte 0x00004004
	.4byte 0x00000814
	.4byte 0x0000013b
	.2byte 0x0145
	.2byte 0x0000
.L_080bef88:
	ldr	r3, [sp, #12]
	ldr	r0, [r3, #0]
	bl	sub_080772f8
	movs	r1, #200
	lsls	r0, r0, #16
	bl	sub_080022ec
	adds	r5, r0, #0
	bl	sub_080771a0
	ldr	r3, [pc, #584]
	ands	r0, r3
	cmp	r5, r0
	bgt.n	.L_080befa8
	b.n	.L_080bf1a8
.L_080befa8:
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #0]
.L_080befac:
	movs	r2, #1
	adds	r3, #44
	strb	r2, [r3, #0]
	b.n	.L_080bf1a8
.L_080befb4:
	mov	r0, fp
	bl	sub_08077080
	adds	r7, r0, #0
	ldr	r0, [sp, #8]
	ldrb	r2, [r7, #2]
	ldr	r3, [r0, #0]
	mov	r1, fp
	str	r2, [r3, #80]
	movs	r2, #0
	str	r2, [r3, #88]
	str	r1, [r3, #76]
	ldrb	r3, [r7, #3]
	adds	r2, r3, #0
	cmp	r2, #65
	beq.n	.L_080beff2
	cmp	r2, #41
	beq.n	.L_080befe8
	cmp	r2, #42
	beq.n	.L_080befe8
	cmp	r2, #43
	beq.n	.L_080befe8
	cmp	r2, #44
	beq.n	.L_080befe8
	cmp	r2, #68
	bne.n	.L_080bf044
.L_080befe8:
	adds	r2, r3, #0
	cmp	r2, #65
	beq.n	.L_080beff2
	cmp	r2, #68
	bne.n	.L_080beff6
.L_080beff2:
	movs	r5, #153
	b.n	.L_080bf002
.L_080beff6:
	cmp	r2, #41
	beq.n	.L_080bf000
	movs	r5, #64
	cmp	r2, #43
	bne.n	.L_080bf002
.L_080bf000:
	movs	r5, #32
.L_080bf002:
	cmp	r3, #65
	beq.n	.L_080bf010
	cmp	r3, #41
	beq.n	.L_080bf010
	movs	r6, #2
	cmp	r3, #42
	bne.n	.L_080bf012
.L_080bf010:
	movs	r6, #1
.L_080bf012:
	bl	sub_080771a0
	movs	r3, #255
	ands	r0, r3
	cmp	r0, r5
	bge.n	.L_080bf0f8
	ldr	r3, [sp, #8]
	ldr	r2, [r3, #0]
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	movs	r0, #0
	cmp	r0, r3
	bge.n	.L_080bf0f8
	adds	r1, r2, #0
	adds	r2, #30
.L_080bf030:
	ldrb	r3, [r2, #0]
	adds	r3, r3, r6
	strb	r3, [r2, #0]
	movs	r3, #1
	ldrsb	r3, [r1, r3]
	adds	r0, #1
	adds	r2, #1
	cmp	r0, r3
	blt.n	.L_080bf030
	b.n	.L_080bf0f8
.L_080bf044:
	adds	r3, #220
	movs	r4, #128
	lsls	r3, r3, #24
	lsls	r4, r4, #19
	cmp	r3, r4
	bhi.n	.L_080bf0b4
	ldrb	r3, [r7, #3]
	subs	r3, #36
	cmp	r3, #4
	bhi.n	.L_080bf084
	ldr	r2, [pc, #400]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080bf074
	.4byte 0x080bf078
	.4byte 0x080bf07c
	.4byte 0x080bf080
	.2byte 0xf084
	.2byte 0x080b
	movs	r5, #63
	b.n	.L_080bf086
	movs	r5, #31
	b.n	.L_080bf086
	movs	r5, #15
	b.n	.L_080bf086
	movs	r5, #7
	b.n	.L_080bf086
.L_080bf084:
	movs	r5, #3
.L_080bf086:
	bl	sub_080771a0
	ands	r0, r5
	cmp	r0, #0
	bne.n	.L_080bf0f8
	ldr	r1, [sp, #8]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	movs	r0, #0
	cmp	r0, r3
	bge.n	.L_080bf0f8
	adds	r1, r2, #0
	movs	r4, #2
	adds	r2, #44
.L_080bf0a4:
	strb	r4, [r2, #0]
	movs	r3, #1
	ldrsb	r3, [r1, r3]
	adds	r0, #1
	adds	r2, #1
	cmp	r0, r3
	blt.n	.L_080bf0a4
	b.n	.L_080bf0f8
.L_080bf0b4:
	mov	r2, fp
	cmp	r2, #178
	bne.n	.L_080bf0f8
	ldr	r5, [sp, #8]
	ldr	r3, [r5, #0]
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r6, #0
	cmp	r6, r3
	bge.n	.L_080bf0f8
.L_080bf0ca:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldr	r3, [r5, #0]
	adds	r3, #2
	ldrb	r1, [r3, r6]
	ldrb	r2, [r7, #2]
	ldrb	r3, [r7, #3]
	movs	r4, #100
	str	r4, [sp, #0]
	bl	sub_08077178
	ldr	r1, [r5, #0]
	adds	r2, r6, #0
	adds	r3, r1, #2
	adds	r2, #56
	strb	r0, [r3, r2]
	movs	r3, #1
	ldrsb	r3, [r1, r3]
	adds	r6, #1
	cmp	r6, r3
	blt.n	.L_080bf0ca
.L_080bf0f8:
	ldr	r2, [pc, #244]
	cmp	fp, r2
	bhi.n	.L_080bf11e
	ldr	r3, [sp, #8]
	ldr	r2, [pc, #240]
	ldr	r1, [r3, #0]
	mov	r4, fp
	lsls	r3, r4, #2
	ldr	r2, [r2, r3]
	movs	r3, #30
	ldrsb	r3, [r1, r3]
	str	r2, [r1, #88]
	cmp	r3, #1
	ble.n	.L_080bf11e
	lsls	r3, r3, #12
	ldr	r0, [pc, #224]
	adds	r3, r2, r3
	adds	r3, r3, r0
	str	r3, [r1, #88]
.L_080bf11e:
	ldr	r1, [pc, #220]
	cmp	fp, r1
	bhi.n	.L_080bf138
	ldr	r1, [pc, #216]
	mov	r2, fp
	ldrb	r3, [r1, r2]
	cmp	r3, #0
	beq.n	.L_080bf138
	ldr	r3, [sp, #8]
	mov	r4, fp
	ldr	r2, [r3, #0]
	ldrb	r3, [r1, r4]
	b.n	.L_080bf16c
.L_080bf138:
	mov	r0, fp
	bl	sub_080bd3c8
	cmp	r0, #0
	beq.n	.L_080bf14a
	ldr	r0, [sp, #8]
	ldr	r2, [r0, #0]
	movs	r3, #3
	b.n	.L_080bf16c
.L_080bf14a:
	ldr	r1, [sp, #8]
	ldr	r2, [r1, #0]
	ldr	r3, [r2, #88]
	cmp	r3, #0
	beq.n	.L_080bf16a
	ldr	r4, [sp, #12]
	ldr	r0, [pc, #172]
	ldr	r3, [r4, #0]
	adds	r3, r3, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080bf166
	movs	r3, #8
	b.n	.L_080bf16c
.L_080bf166:
	movs	r3, #3
	b.n	.L_080bf16c
.L_080bf16a:
	movs	r3, #1
.L_080bf16c:
	str	r3, [r2, #84]
	ldrb	r0, [r7, #3]
	bl	sub_080772b8
	cmp	r0, #0
	beq.n	.L_080bf186
	ldr	r1, [sp, #8]
	ldr	r3, [r1, #0]
	movs	r1, #128
	ldr	r2, [r3, #88]
	lsls	r1, r1, #9
	orrs	r2, r1
	str	r2, [r3, #88]
.L_080bf186:
	mov	r2, fp
	cmp	r2, #178
	bne.n	.L_080bf1a8
	ldr	r3, [sp, #8]
	ldr	r1, [r3, #0]
	adds	r3, r1, #0
	adds	r3, #58
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080bf1a8
	ldr	r3, [r1, #88]
	movs	r2, #128
	lsls	r2, r2, #5
	orrs	r3, r2
	str	r3, [r1, #88]
.L_080bf1a8:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r0, #6
	ldrsh	r3, [r3, r0]
	cmp	r3, #2
	bne.n	.L_080bf1c6
	ldr	r1, [sp, #8]
	ldr	r2, [r1, #0]
	ldr	r3, [r2, #84]
	cmp	r3, #5
	beq.n	.L_080bf1c6
	cmp	r3, #9
	beq.n	.L_080bf1c6
	movs	r3, #4
	str	r3, [r2, #84]
.L_080bf1c6:
	ldr	r2, [sp, #8]
	mov	r4, sl
	ldr	r3, [r2, #0]
	ldr	r2, [r4, #0]
	ldrh	r2, [r2, #6]
	adds	r3, #72
	strh	r2, [r3, #0]
.L_080bf1d4:
	movs	r0, #0
.L_080bf1d6:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0000ffff
	.4byte 0x080bf060
	.4byte 0x00000206
	.4byte 0x080c2da0
	.4byte 0xfffff000
	.4byte 0x00000205
	.4byte 0x080c2b98
	.4byte 0x00000129
