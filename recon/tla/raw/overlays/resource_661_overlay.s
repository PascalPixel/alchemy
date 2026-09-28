.syntax unified
	.thumb
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_0200007c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200007c
	ldr	r1, [r5, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r4, r3
	adds	r3, r2, #0
	lsls	r4, r4, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_0200007c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x6c426883
	.4byte 0x189b6d01
	.4byte 0x6c826083
	.4byte 0x189b68c3
	.4byte 0x6cc260c3
	.4byte 0x189b6903
	.4byte 0x6b026103
	.4byte 0x189b6983
	.4byte 0x6b426183
	.4byte 0x189b69c3
	.4byte 0x306461c3
	.4byte 0x88028a4b
	.4byte 0x824b189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r3
	ldr	r3, [pc, #420]
	sub	sp, #4
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl 0x0200c058
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02000100
	cmp	r7, #0
	beq.n	.L_02000100
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02000108
.L_02000100:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02000108:
	mov	r3, sl
	bl 0x0200bfc0
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02000116
	b.n	.L_02000262
.L_02000116:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200bfa8
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200bfb8
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200c008
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	adds	r0, r6, #0
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x02008038
	movs	r2, #100
	adds	r2, r2, r6
	mov	r9, r2
	mov	r3, r9
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #256]
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000262
	cmp	r7, #0
	beq.n	.L_02000262
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000198
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200c0c0
.L_02000198:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
.L_020001a0:
	cmp	r3, #0
	beq.n	.L_020001b8
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x02008038
.L_020001b8:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020001cc
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_020001cc:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000212
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020001fa
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200bf08
.L_020001ee:
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_0200020c
.L_020001fa:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200bf08
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_0200020c:
	bl 0x0200bf08
	str	r0, [r6, #52]
.L_02000212:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200022e
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200bfa8
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200bfb8
.L_0200022e:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000240
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02000240:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000252
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02000252:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000262
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02000262:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c5c8
	.4byte 0x02008081
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #19
	movs	r1, #68
	bl 0x0200c148
	pop	{pc}
	.global Func_0200028c
	.thumb_func
Func_0200028c:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_020002a4
	ldr	r0, [pc, #12]
	b.n	.L_020002a6
.L_020002a4:
	ldr	r0, [pc, #12]
.L_020002a6:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000004d
	.4byte 0x0200c840
	.2byte 0xc810
	.2byte 0x0200
	.global Func_020002b8
	.thumb_func
Func_020002b8:
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020002d0
	ldr	r0, [pc, #20]
	b.n	.L_020002da
.L_020002d0:
	ldr	r3, [pc, #20]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_020002da
	ldr	r0, [pc, #16]
.L_020002da:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000004c
	.4byte 0x0200c8b8
	.4byte 0x0000004d
	.2byte 0xc8d8
	.2byte 0x0200
	.global Func_020002f0
	.thumb_func
Func_020002f0:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xc908
	.2byte 0x0200
	.global Func_020002f8
	.thumb_func
Func_020002f8:
	push	{lr}
	ldr	r3, [pc, #112]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_02000310
	ldr	r0, [pc, #100]
	b.n	.L_02000368
.L_02000310:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_0200032a
	movs	r0, #143
	lsls	r0, r0, #4
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_02000326
	ldr	r0, [pc, #88]
	b.n	.L_02000368
.L_02000326:
	ldr	r0, [pc, #88]
	b.n	.L_02000368
.L_0200032a:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_02000334
	ldr	r0, [pc, #84]
	b.n	.L_02000368
.L_02000334:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_0200033e
	ldr	r0, [pc, #84]
	b.n	.L_02000368
.L_0200033e:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02000348
	ldr	r0, [pc, #80]
	b.n	.L_02000368
.L_02000348:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02000352
	ldr	r0, [pc, #80]
	b.n	.L_02000368
.L_02000352:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_0200035c
	ldr	r0, [pc, #76]
	b.n	.L_02000368
.L_0200035c:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02000366
	ldr	r0, [pc, #76]
	b.n	.L_02000368
.L_02000366:
	ldr	r0, [pc, #76]
.L_02000368:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000043
	.4byte 0x0200c9dc
	.4byte 0x00000044
	.4byte 0x0200cb44
	.4byte 0x0200ca84
	.4byte 0x00000045
	.4byte 0x0200cc04
	.4byte 0x00000046
	.4byte 0x0200cd3c
	.4byte 0x00000047
	.4byte 0x0200ce44
	.4byte 0x0000004a
	.4byte 0x0200cfdc
	.4byte 0x0000004c
	.4byte 0x0200d06c
	.4byte 0x0000004e
	.4byte 0x0200d0b4
	.2byte 0xc9c4
	.2byte 0x0200
	push	{lr}
	movs	r2, #192
	lsls	r2, r2, #2
	movs	r1, #64
	adds	r2, #2
	bl 0x0200c178
	pop	{pc}
	.global Func_020003c8
	.thumb_func
Func_020003c8:
	push	{lr}
	ldr	r3, [pc, #136]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #128]
	cmp	r2, r3
	bne.n	.L_020003e0
	ldr	r0, [pc, #124]
	b.n	.L_02000450
.L_020003e0:
	ldr	r3, [pc, #124]
	cmp	r2, r3
	bne.n	.L_020003ea
	ldr	r0, [pc, #124]
	b.n	.L_02000450
.L_020003ea:
	ldr	r3, [pc, #124]
	cmp	r2, r3
	bne.n	.L_020003f4
	ldr	r0, [pc, #120]
	b.n	.L_02000450
.L_020003f4:
	ldr	r3, [pc, #120]
	cmp	r2, r3
	bne.n	.L_020003fe
	ldr	r0, [pc, #120]
	b.n	.L_02000450
.L_020003fe:
	ldr	r3, [pc, #120]
	cmp	r2, r3
	bne.n	.L_02000408
	ldr	r0, [pc, #116]
	b.n	.L_02000450
.L_02000408:
	ldr	r3, [pc, #116]
	cmp	r2, r3
	bne.n	.L_02000412
	ldr	r0, [pc, #116]
	b.n	.L_02000450
.L_02000412:
	ldr	r3, [pc, #116]
	cmp	r2, r3
	bne.n	.L_0200041c
	ldr	r0, [pc, #112]
	b.n	.L_02000450
.L_0200041c:
	ldr	r3, [pc, #112]
	cmp	r2, r3
	bne.n	.L_02000426
	ldr	r0, [pc, #112]
	b.n	.L_02000450
.L_02000426:
	ldr	r3, [pc, #112]
	cmp	r2, r3
	bne.n	.L_02000430
	ldr	r0, [pc, #108]
	b.n	.L_02000450
.L_02000430:
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L_0200043a
	ldr	r0, [pc, #108]
	b.n	.L_02000450
.L_0200043a:
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L_02000444
	ldr	r0, [pc, #104]
	b.n	.L_02000450
.L_02000444:
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_0200044e
	ldr	r0, [pc, #104]
	b.n	.L_02000450
.L_0200044e:
	ldr	r0, [pc, #104]
.L_02000450:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000043
	.4byte 0x0200d198
	.4byte 0x00000044
	.4byte 0x0200d21c
	.4byte 0x00000045
	.4byte 0x0200d3fc
	.4byte 0x00000046
	.4byte 0x0200d4e0
	.4byte 0x00000047
	.4byte 0x0200d594
	.4byte 0x00000048
	.4byte 0x0200d630
	.4byte 0x00000049
	.4byte 0x0200d678
	.4byte 0x0000004a
	.4byte 0x0200d6b4
	.4byte 0x0000004b
	.4byte 0x0200d750
	.4byte 0x0000004c
	.4byte 0x0200d78c
	.4byte 0x0000004d
	.4byte 0x0200d7bc
	.4byte 0x0000004e
	.4byte 0x0200d7e0
	.2byte 0xd18c
	.2byte 0x0200
	push	{lr}
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
	ldr	r0, [pc, #16]
	bl 0x0200c0c8
	movs	r1, #0
	movs	r0, #8
	bl 0x0200c0d8
	bl 0x0200c048
	pop	{pc}
	.2byte 0x1a96
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #104]
	bl 0x0200c0c8
	movs	r0, #12
	movs	r1, #4
	bl 0x0200c0a0
	movs	r0, #12
	movs	r1, #0
	bl 0x0200c0d0
	movs	r1, #128
	movs	r2, #20
	movs	r0, #13
	lsls	r1, r1, #8
	bl 0x0200c0e0
	movs	r0, #13
	movs	r1, #4
	bl 0x0200c0a0
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c0d0
	movs	r0, #13
	movs	r1, #0
	movs	r2, #20
	bl 0x0200c0e0
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200c0e0
	movs	r0, #13
	movs	r1, #0
	movs	r2, #20
	bl 0x0200c0e0
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c0e0
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c0d0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x1a9c
	.2byte 0x0000
	push	{lr}
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
	movs	r0, #242
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200bf90
	cmp	r0, #0
	bne.n	.L_0200056e
	bl 0x020084e0
.L_0200056e:
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #12
	bl 0x0200c0b8
	ldr	r0, [pc, #32]
	bl 0x0200c0c8
	movs	r1, #0
	movs	r0, #12
	bl 0x0200c0d0
	movs	r0, #242
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200bf98
	bl 0x0200c048
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1a9f
	.2byte 0x0000
	push	{lr}
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
	movs	r0, #242
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200bf90
	cmp	r0, #0
	bne.n	.L_020005c6
	bl 0x020084e0
.L_020005c6:
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #13
	bl 0x0200c0b8
	ldr	r0, [pc, #32]
	bl 0x0200c0c8
	movs	r1, #0
	movs	r0, #13
	bl 0x0200c0d0
	movs	r0, #242
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200bf98
	bl 0x0200c048
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1aa0
	.2byte 0x0000
	push	{lr}
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200c028
	bl 0x0200c048
	pop	{pc}
	.2byte 0x0000
	.2byte 0x1abe
	.2byte 0x0000
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #8]
	adds	r1, r3, #0
	bl 0x0200b88c
	pop	{pc}
	.2byte 0xc768
	.2byte 0x0200
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #8]
	adds	r1, r3, #0
	bl 0x0200b88c
	pop	{pc}
	.2byte 0xc78e
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #24]
	adds	r1, r5, #0
	bl 0x0200b88c
	cmp	r5, #1
	bne.n	.L_02000666
	ldr	r2, [pc, #16]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_02000666
	movs	r3, #210
	str	r3, [r2, #0]
.L_02000666:
	pop	{r5, pc}
	.4byte 0x0200c7d2
	.2byte 0xc808
	.2byte 0x0200
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #8]
	adds	r1, r3, #0
	bl 0x0200b88c
	pop	{pc}
	.2byte 0xc800
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #24]
	adds	r1, r5, #0
	bl 0x0200b88c
	cmp	r5, #1
	bne.n	.L_020006a2
	ldr	r2, [pc, #16]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_020006a2
	movs	r3, #210
	str	r3, [r2, #0]
.L_020006a2:
	pop	{r5, pc}
	.4byte 0x0200c7ec
	.2byte 0xc808
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #8
	str	r3, [sp, #4]
	ldr	r3, [pc, #244]
	movs	r2, #240
	lsls	r2, r2, #1
	movs	r1, #0
	adds	r3, r3, r2
	mov	fp, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #232]
	cmp	r2, r3
	bne.n	.L_020006e0
	ldr	r2, [pc, #232]
	mov	fp, r2
	b.n	.L_020006f6
.L_020006e0:
	ldr	r3, [pc, #228]
	cmp	r2, r3
	bne.n	.L_020006ec
	ldr	r3, [pc, #228]
	mov	fp, r3
	b.n	.L_020006f6
.L_020006ec:
	ldr	r3, [pc, #224]
	cmp	r2, r3
	bne.n	.L_020006f6
	ldr	r1, [pc, #224]
	mov	fp, r1
.L_020006f6:
	mov	r2, fp
	cmp	r2, #0
	beq.n	.L_020007ac
	movs	r1, #255
	ldrh	r3, [r2, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	beq.n	.L_020007a2
.L_02000708:
	mov	r2, fp
	ldrh	r0, [r2, #0]
	bl 0x0200c058
	movs	r3, #4
	add	fp, r3
	mov	r1, fp
	adds	r7, r0, #0
	ldrh	r0, [r1, #0]
	bl 0x0200bf90
	mov	r9, r0
	cmp	r0, #0
	bne.n	.L_02000790
	adds	r3, r7, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	movs	r1, #156
	mov	r8, r3
	mov	r2, r8
	lsls	r3, r3, #3
	subs	r3, r3, r2
	ldr	r2, [sp, #4]
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r6, [r2, r3]
	ldr	r3, [pc, #152]
	ldr	r1, [pc, #152]
	adds	r5, r6, r3
	asrs	r5, r5, #2
	adds	r0, r7, #0
	adds	r5, r5, r1
	bl 0x0200c1e0
	ldr	r1, [r7, #8]
	mov	sl, r0
	ldr	r2, [r7, #16]
	mov	r0, r8
	bl 0x0200c020
	str	r0, [sp, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	mov	r0, r8
	bl 0x0200bff8
	mov	r1, sl
	lsls	r3, r1, #2
	adds	r6, r6, r3
	adds	r3, r7, #0
	mov	r1, r9
	adds	r3, #85
	strb	r1, [r3, #0]
	strb	r1, [r6, #2]
	ldrb	r1, [r6, #3]
	movs	r3, #128
	orrs	r3, r1
	adds	r2, r0, #0
	strb	r3, [r6, #3]
	asrs	r2, r2, #19
	adds	r2, #4
	mov	r0, r8
	ldr	r1, [sp, #0]
	bl 0x0200c1e8
	add	r5, sl
	strb	r0, [r5, #0]
.L_02000790:
	movs	r2, #2
	add	fp, r2
	mov	r1, fp
	movs	r2, #255
	ldrh	r3, [r1, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02000708
.L_020007a2:
	movs	r0, #134
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bf98
.L_020007ac:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000045
	.4byte 0x0200c78e
	.4byte 0x00000046
	.4byte 0x0200c7d2
	.4byte 0x00000047
	.4byte 0x0200c7ec
	.4byte 0xfdff0000
	.2byte 0x4000
	.2byte 0x0202
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #8
	str	r3, [sp, #4]
	ldr	r3, [pc, #248]
	movs	r2, #240
	lsls	r2, r2, #1
	movs	r1, #0
	adds	r3, r3, r2
	mov	fp, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #236]
	cmp	r2, r3
	bne.n	.L_02000814
	ldr	r2, [pc, #236]
	mov	fp, r2
	b.n	.L_0200082a
.L_02000814:
	ldr	r3, [pc, #232]
	cmp	r2, r3
	bne.n	.L_02000820
	ldr	r3, [pc, #232]
	mov	fp, r3
	b.n	.L_0200082a
.L_02000820:
	ldr	r3, [pc, #228]
	cmp	r2, r3
	bne.n	.L_0200082a
	ldr	r1, [pc, #228]
	mov	fp, r1
.L_0200082a:
	mov	r2, fp
	cmp	r2, #0
	beq.n	.L_020008e4
	movs	r1, #255
	ldrh	r3, [r2, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	beq.n	.L_020008da
.L_0200083c:
	mov	r2, fp
	ldrh	r0, [r2, #0]
	bl 0x0200c058
	movs	r3, #4
	add	fp, r3
	mov	r1, fp
	adds	r7, r0, #0
	ldrh	r0, [r1, #0]
	bl 0x0200bf90
	str	r0, [sp, #0]
	cmp	r0, #0
	bne.n	.L_020008c8
	adds	r3, r7, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	movs	r1, #156
	mov	r8, r3
	mov	r2, r8
	lsls	r3, r3, #3
	subs	r3, r3, r2
	ldr	r2, [sp, #4]
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r6, [r2, r3]
	ldr	r3, [pc, #156]
	ldr	r1, [pc, #156]
	adds	r5, r6, r3
	asrs	r5, r5, #2
	adds	r0, r7, #0
	adds	r5, r5, r1
	bl 0x0200c1e0
	ldr	r1, [r7, #8]
	mov	sl, r0
	ldr	r2, [r7, #16]
	mov	r0, r8
	bl 0x0200c020
	ldr	r1, [r7, #8]
	mov	r9, r0
	ldr	r2, [r7, #16]
	mov	r0, r8
	bl 0x0200bff8
	mov	r1, sl
	lsls	r3, r1, #2
	mov	r1, sp
	ldrb	r1, [r1, #0]
	adds	r6, r6, r3
	adds	r3, r7, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	ldrb	r1, [r6, #3]
	movs	r3, #255
	adds	r2, r0, #0
	strb	r3, [r6, #2]
	movs	r3, #127
	ands	r3, r1
	asrs	r2, r2, #19
	strb	r3, [r6, #3]
	subs	r2, #4
	mov	r0, r8
	mov	r1, r9
	bl 0x0200c1e8
	add	r5, sl
	strb	r0, [r5, #0]
.L_020008c8:
	movs	r2, #2
	add	fp, r2
	mov	r1, fp
	movs	r2, #255
	ldrh	r3, [r1, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_0200083c
.L_020008da:
	movs	r0, #134
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bfa0
.L_020008e4:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000045
	.4byte 0x0200c78e
	.4byte 0x00000046
	.4byte 0x0200c7d2
	.4byte 0x00000047
	.4byte 0x0200c7ec
	.4byte 0xfdff0000
	.2byte 0x4000
	.2byte 0x0202
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r5, r0, #0
	ldr	r7, [r3, #32]
	b.n	.L_0200094e
.L_02000924:
	ldrh	r0, [r5, #0]
	bl 0x0200c058
	adds	r6, r0, #0
	ldrh	r0, [r5, #4]
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_0200094c
	adds	r0, r6, #0
	bl 0x0200c1e0
	movs	r2, #212
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	movs	r2, #255
	strb	r2, [r3, #2]
.L_0200094c:
	adds	r5, #6
.L_0200094e:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02000924
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r5, r0, #0
	ldr	r6, [r3, #32]
	b.n	.L_02000984
.L_02000968:
	ldrh	r0, [r5, #0]
	bl 0x0200c058
	bl 0x0200c1e0
	movs	r2, #212
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	movs	r2, #232
	adds	r5, #6
	strb	r2, [r3, #2]
.L_02000984:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02000968
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	ldr	r5, [pc, #84]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	bl 0x0200c058
	movs	r2, #240
	movs	r3, #2
	lsls	r2, r2, #1
	adds	r0, #34
	strb	r3, [r0, #0]
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_020009c2
	ldr	r0, [pc, #56]
	bl 0x02008918
	b.n	.L_020009ea
.L_020009c2:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_020009d0
	ldr	r0, [pc, #48]
	bl 0x02008918
	b.n	.L_020009ea
.L_020009d0:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_020009de
	ldr	r0, [pc, #44]
	bl 0x02008918
	b.n	.L_020009ea
.L_020009de:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_020009ea
	ldr	r0, [pc, #36]
	bl 0x02008918
.L_020009ea:
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x00000043
	.4byte 0x0200c768
	.4byte 0x00000045
	.4byte 0x0200c78e
	.4byte 0x00000046
	.4byte 0x0200c7d2
	.4byte 0x00000047
	.2byte 0xc7ec
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #84]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	bl 0x0200c058
	movs	r2, #240
	movs	r3, #0
	lsls	r2, r2, #1
	adds	r0, #34
	strb	r3, [r0, #0]
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000a3e
	ldr	r0, [pc, #56]
	bl 0x0200895c
	b.n	.L_02000a66
.L_02000a3e:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000a4c
	ldr	r0, [pc, #48]
	bl 0x0200895c
	b.n	.L_02000a66
.L_02000a4c:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02000a5a
	ldr	r0, [pc, #44]
	bl 0x0200895c
	b.n	.L_02000a66
.L_02000a5a:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02000a66
	ldr	r0, [pc, #36]
	bl 0x0200895c
.L_02000a66:
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x00000043
	.4byte 0x0200c768
	.4byte 0x00000045
	.4byte 0x0200c78e
	.4byte 0x00000046
	.4byte 0x0200c7d2
	.4byte 0x00000047
	.2byte 0xc7ec
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
	ldr	r5, [pc, #88]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
.L_02000aa4:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02000ac2
	movs	r0, #16
	movs	r1, #1
	bl 0x0200c110
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	movs	r1, #16
	bl 0x0200c1c0
	b.n	.L_02000ae6
.L_02000ac2:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000ae6
	movs	r0, #21
	movs	r1, #1
	bl 0x0200c110
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	movs	r1, #21
	bl 0x0200c1c0
	b.n	.L_02000ae6
.L_02000ae0:
	movs	r0, #1
	bl 0x0200bf20
.L_02000ae6:
	ldr	r3, [pc, #24]
	ldr	r3, [r3, #0]
	cmp	r3, #210
	beq.n	.L_02000ae0
	bl 0x0200c048
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x00000046
	.4byte 0x00000047
	.2byte 0xc808
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200bf08
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02000b34
	adds	r3, #15
.L_02000b34:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c058
	ldr	r3, [r0, #80]
	ldr	r4, [r6, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r4, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r4, #9]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #148]
	sub	sp, #68
	add	r7, sp, #28
	str	r3, [r7, #36]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r7, #8]
	str	r3, [r7, #12]
	mov	sl, r2
	movs	r2, #0
	mov	fp, r0
	mov	r9, r1
	mov	r8, r2
.L_02000bae:
	mov	r3, r8
	lsls	r6, r3, #14
	adds	r0, r6, #0
	bl 0x0200bf48
	add	r5, sp, #16
	movs	r3, #0
	str	r0, [r5, #0]
	adds	r0, r6, #0
	str	r3, [r5, #4]
	bl 0x0200bf40
	ldr	r3, [r5, #0]
	str	r0, [r5, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r5, #0]
	bl 0x0200bf38
	ldr	r3, [r5, #0]
	ldr	r2, [pc, #84]
	lsls	r0, r0, #17
	lsrs	r0, r0, #16
	adds	r3, r3, r0
	adds	r3, r3, r2
	str	r3, [r5, #0]
	bl 0x0200bf38
	ldr	r2, [r5, #8]
	ldr	r3, [pc, #68]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r2, r2, r0
	adds	r2, r2, r3
	str	r2, [r5, #8]
	ldr	r3, [r5, #0]
	ldr	r1, [r5, #4]
	str	r2, [sp, #4]
	movs	r2, #128
	lsls	r2, r2, #17
	adds	r2, #1
	str	r1, [sp, #0]
	str	r2, [sp, #8]
	mov	r0, fp
	mov	r2, sl
	mov	r1, r9
	str	r7, [sp, #12]
	bl 0x020080b8
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #16
	bls.n	.L_02000bae
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02008b05
	.4byte 0xffff0000
	.2byte 0x8000
	.2byte 0xffff
	.2byte 0xb520
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
	bl 0x0200a1d4
	ldr	r5, [pc, #248]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #16
	bl 0x0200c1c0
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #232]
	adds	r1, #204
	bl 0x0200c118
	movs	r0, #150
	movs	r1, #1
	movs	r2, #248
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200c120
	movs	r0, #16
	movs	r1, #254
	movs	r2, #248
	bl 0x0200c080
	movs	r1, #176
	lsls	r1, r1, #1
	movs	r2, #240
	movs	r0, #16
	bl 0x0200c080
	ldr	r0, [pc, #188]
	bl 0x0200bf30
	movs	r0, #10
	bl 0x0200c038
	ldr	r0, [pc, #180]
	bl 0x0200bf30
	movs	r0, #1
	bl 0x0200bf20
	movs	r1, #0
	movs	r2, #0
	movs	r0, #17
	bl 0x0200c090
	movs	r0, #40
	bl 0x0200c038
	movs	r0, #176
	movs	r2, #248
	lsls	r2, r2, #16
	movs	r1, #0
	lsls	r0, r0, #17
	bl 0x02008b84
	movs	r0, #176
	bl 0x0200c1f8
	movs	r0, #16
.L_02000cc6:
	bl 0x0200c058
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r0, #16
	bl 0x0200c058
	movs	r1, #0
	bl 0x0200c0c0
	movs	r0, #16
	bl 0x0200c058
	movs	r1, #1
	bl 0x0200c008
	movs	r1, #190
	lsls	r1, r1, #1
	movs	r2, #248
	movs	r0, #16
	bl 0x0200c080
	ldr	r0, [r5, #0]
	bl 0x0200c070
	movs	r0, #241
	bl 0x0200c1f8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #16
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c060
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #248
	movs	r0, #16
	bl 0x0200c088
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200c1f8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #16
	bl 0x0200c090
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #137
	bl 0x0200bf98
	bl 0x0200c048
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x00026666
	.4byte 0x0200a181
	.2byte 0x9c5d
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
	bl 0x0200a1d4
	ldr	r5, [pc, #260]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #21
	bl 0x0200c1c0
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #244]
	adds	r1, #204
	bl 0x0200c118
	movs	r0, #246
	movs	r1, #1
	movs	r2, #154
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #18
	bl 0x0200c120
	movs	r2, #162
	movs	r0, #21
	movs	r1, #248
	lsls	r2, r2, #2
	bl 0x0200c080
	movs	r2, #144
	movs	r1, #248
	lsls	r2, r2, #2
	movs	r0, #21
	bl 0x0200c080
	ldr	r0, [pc, #200]
	bl 0x0200bf30
	movs	r0, #10
	bl 0x0200c038
	ldr	r0, [pc, #192]
	bl 0x0200bf30
	movs	r0, #1
	bl 0x0200bf20
	movs	r1, #0
	movs	r2, #0
	movs	r0, #22
	bl 0x0200c090
	movs	r0, #20
	bl 0x0200c038
	movs	r0, #248
	movs	r2, #144
	lsls	r2, r2, #18
	movs	r1, #0
	lsls	r0, r0, #16
	bl 0x02008b84
	movs	r0, #176
	bl 0x0200c1f8
	movs	r0, #21
	bl 0x0200c058
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r0, #21
	bl 0x0200c058
	movs	r1, #0
	bl 0x0200c0c0
	movs	r0, #21
	bl 0x0200c058
	movs	r1, #1
	bl 0x0200c008
	movs	r1, #128
	movs	r2, #140
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	movs	r0, #21
	bl 0x0200c080
	movs	r0, #20
	bl 0x0200c038
	ldr	r0, [r5, #0]
	bl 0x0200c070
	movs	r0, #241
	bl 0x0200c1f8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #21
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c060
	movs	r2, #248
	movs	r1, #248
	lsls	r2, r2, #1
	movs	r0, #21
	bl 0x0200c088
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200c1f8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #21
	bl 0x0200c090
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #138
	bl 0x0200bf98
	bl 0x0200c048
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00026666
	.4byte 0x0200a181
	.2byte 0x9c5d
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
	bl 0x0200a1d4
	ldr	r5, [pc, #140]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #21
	bl 0x0200c1c0
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #124]
	adds	r1, #204
	bl 0x0200c118
	movs	r0, #232
	movs	r1, #1
	movs	r2, #164
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c120
	movs	r2, #164
	movs	r0, #21
	movs	r1, #232
	lsls	r2, r2, #1
	bl 0x0200c080
	movs	r0, #232
	movs	r1, #1
	movs	r2, #200
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200c120
	movs	r2, #140
	movs	r0, #21
	movs	r1, #232
	lsls	r2, r2, #1
	bl 0x0200c080
	movs	r1, #232
	movs	r2, #168
	movs	r0, #21
	bl 0x0200c080
	ldr	r0, [pc, #52]
	bl 0x0200bf30
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #21
	bl 0x0200c0e0
	ldr	r0, [r5, #0]
	bl 0x0200c070
	movs	r0, #20
	bl 0x0200c038
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #139
	bl 0x0200bf98
	bl 0x0200c048
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x00026666
	.2byte 0xa181
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #0
	sub	sp, #8
	mov	sl, r3
.L_02000f32:
	ldr	r7, [pc, #140]
	movs	r3, #0
	mov	r8, r3
.L_02000f38:
	bl 0x0200bf38
	ldrh	r5, [r7, #0]
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r5, r5, r0
	bl 0x0200bf38
	adds	r6, r7, #2
	ldrh	r3, [r6, #0]
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r3, r3, r0
	subs	r5, #2
	lsls	r5, r5, #16
	subs	r3, #2
	movs	r0, #30
	adds	r1, r5, #0
	lsls	r3, r3, #16
	adds	r0, #255
	movs	r2, #0
	bl 0x0200bfc0
	adds	r5, r0, #0
	adds	r7, #4
	cmp	r5, #0
	beq.n	.L_02000f90
	movs	r3, #144
	lsls	r3, r3, #9
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #52]
	movs	r1, #0
	bl 0x0200c008
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bfa8
	adds	r0, r5, #0
	ldr	r1, [pc, #56]
	bl 0x0200bfb8
.L_02000f90:
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	cmp	r3, #14
	bls.n	.L_02000f38
	movs	r3, #1
	add	sl, r3
	mov	r3, sl
	cmp	r3, #1
	bls.n	.L_02000f32
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #74
	movs	r1, #71
	movs	r2, #77
	movs	r3, #9
	bl 0x0200bff0
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0200c2b4
	.2byte 0xd828
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #660]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c058
	adds	r6, r0, #0
	movs	r0, #21
	bl 0x0200c058
	adds	r7, r0, #0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #139
	bl 0x0200bf90
	cmp	r0, #0
	bne.n	.L_02000ff6
	b.n	.L_0200125e
.L_02000ff6:
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
.L_02001000:
	ldr	r5, [pc, #612]
	ldr	r1, [pc, #616]
	ldr	r2, [r5, #0]
	movs	r0, #1
	ldr	r3, [r2, #24]
	adds	r3, r3, r1
	str	r3, [r2, #24]
	ldr	r3, [r2, #28]
	adds	r3, r3, r1
	str	r3, [r2, #28]
	bl 0x0200bf20
	ldr	r3, [r5, #0]
	movs	r2, #204
	ldr	r3, [r3, #24]
	lsls	r2, r2, #8
	adds	r2, #203
	cmp	r3, r2
	bgt.n	.L_02001000
.L_02001026:
	ldr	r5, [pc, #576]
	movs	r1, #200
	ldr	r2, [r5, #0]
	lsls	r1, r1, #5
	ldr	r3, [r2, #24]
	adds	r1, #153
	adds	r3, r3, r1
	str	r3, [r2, #24]
	ldr	r3, [r2, #28]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r2, #28]
	bl 0x0200bf20
	ldr	r3, [r5, #0]
	movs	r2, #230
	ldr	r3, [r3, #24]
	lsls	r2, r2, #9
	adds	r2, #204
	cmp	r3, r2
	ble.n	.L_02001026
.L_02001050:
	ldr	r5, [pc, #532]
	ldr	r1, [pc, #540]
	ldr	r2, [r5, #0]
	movs	r0, #1
	ldr	r3, [r2, #24]
	adds	r3, r3, r1
	str	r3, [r2, #24]
	ldr	r3, [r2, #28]
	adds	r3, r3, r1
	str	r3, [r2, #28]
	bl 0x0200bf20
	ldr	r3, [r5, #0]
	movs	r2, #204
	ldr	r3, [r3, #24]
	lsls	r2, r2, #6
	adds	r2, #50
	cmp	r3, r2
	bgt.n	.L_02001050
	movs	r1, #0
	movs	r2, #0
	movs	r0, #22
	bl 0x0200c090
	movs	r0, #20
	bl 0x0200c038
	movs	r0, #232
	movs	r1, #1
	movs	r2, #196
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200c120
	ldr	r5, [pc, #456]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	movs	r2, #0
	bl 0x0200c100
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c060
	ldr	r0, [r5, #0]
	movs	r1, #232
	movs	r2, #164
	bl 0x0200c088
	movs	r0, #20
	bl 0x0200c038
	movs	r2, #128
	lsls	r2, r2, #7
	mov	r8, r2
	movs	r1, #232
	movs	r2, #184
	mov	r3, r8
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	movs	r0, #21
	bl 0x0200c098
	movs	r0, #1
	bl 0x0200bf20
	movs	r0, #232
	movs	r2, #184
	lsls	r2, r2, #16
	movs	r1, #0
	lsls	r0, r0, #16
	bl 0x02008b84
	movs	r0, #176
	bl 0x0200c1f8
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r0, #21
	movs	r1, #2
	bl 0x0200c0f0
	movs	r0, #21
	bl 0x0200c058
	movs	r1, #0
	bl 0x0200c0c0
	movs	r0, #21
	bl 0x0200c058
	movs	r1, #1
	bl 0x0200c008
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #21
	ldr	r1, [pc, #316]
	adds	r2, #204
	bl 0x0200c060
	movs	r0, #21
	movs	r1, #216
	movs	r2, #184
	bl 0x0200c080
	movs	r2, #0
	ldr	r0, [r5, #0]
	mov	r1, r8
	bl 0x0200c0e0
	movs	r1, #0
	movs	r0, #21
	bl 0x0200c0e8
	movs	r0, #20
	bl 0x0200c038
	bl 0x0200aca0
	movs	r0, #136
	bl 0x0200c1f8
	bl 0x02008f24
	ldr	r0, [r5, #0]
	bl 0x0200c058
	movs	r1, #0
	bl 0x0200c008
	movs	r0, #20
	bl 0x0200c038
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200c108
	ldr	r0, [r5, #0]
	movs	r1, #27
	bl 0x0200c0a0
	movs	r0, #30
	bl 0x0200c038
	ldr	r0, [r5, #0]
	movs	r1, #28
	bl 0x0200c0a0
	movs	r0, #10
	bl 0x0200c038
	movs	r0, #204
	bl 0x0200c1f8
	movs	r5, #0
.L_020011ae:
	ldr	r3, [r6, #24]
	ldr	r2, [pc, #196]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	adds	r5, #1
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r2, [pc, #184]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	bl 0x0200bf20
	cmp	r5, #13
	bls.n	.L_020011ae
	ldr	r3, [pc, #144]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c058
	movs	r1, #15
	bl 0x0200c0c0
	movs	r0, #153
	bl 0x0200c1f8
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r7, #40]
	movs	r0, #21
	bl 0x0200c058
	movs	r1, #0
	bl 0x0200c008
	movs	r1, #232
	movs	r2, #168
	movs	r0, #21
	bl 0x0200c080
	movs	r0, #3
	bl 0x0200c038
	movs	r0, #204
	bl 0x0200c1f8
	movs	r5, #0
.L_02001214:
	ldr	r3, [r7, #24]
	ldr	r2, [pc, #104]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r7, #24]
	ldr	r3, [r7, #28]
	adds	r5, #1
	adds	r3, r3, r2
	str	r3, [r7, #28]
	ldr	r2, [pc, #92]
	ldr	r3, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #12]
	bl 0x0200bf20
	cmp	r5, #5
	bls.n	.L_02001214
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200c160
	bl 0x0200c168
	movs	r0, #4
	bl 0x0200c138
.L_0200125e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c80c
	.4byte 0xffffeb86
	.4byte 0xfffff5c3
	.4byte 0x00019999
	.4byte 0xfffffc00
	.4byte 0xfffe0000
	.4byte 0xfffff800
	.2byte 0x0000
	.2byte 0xfff9
	.2byte 0xb520
	sub	sp, #8
	cmp	r0, #1
	bne.n	.L_020012d0
	movs	r0, #9
	bl 0x0200c058
	movs	r3, #3
	adds	r5, r0, #0
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r3, #11
	movs	r1, #39
	movs	r2, #13
	bl 0x0200bff0
	movs	r1, #232
	movs	r2, #200
	movs	r0, #9
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200c090
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200bff8
	str	r0, [r5, #12]
	str	r0, [r5, #20]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #237
	bl 0x0200bf98
.L_020012d0:
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x30080100
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #272]
.L_020012e4:
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r2, r2, r3
	ldr	r0, [r2, #0]
	sub	sp, #8
	mov	r8, r2
	bl 0x0200c058
	adds	r5, r0, #0
	movs	r0, #64
	bl 0x0200c058
	ldr	r3, [r5, #8]
	asrs	r7, r3, #20
.L_02001300:
	ldr	r3, [r5, #16]
	asrs	r6, r3, #20
	cmp	r0, #0
	beq.n	.L_02001316
	movs	r1, #154
	movs	r2, #222
	movs	r0, #64
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200c090
.L_02001316:
	movs	r3, #6
	str	r3, [sp, #4]
	movs	r5, #5
	movs	r0, #82
	movs	r1, #82
	movs	r2, #15
	movs	r3, #40
	str	r5, [sp, #0]
	bl 0x0200bff0
	movs	r3, #15
	movs	r2, #40
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #82
	movs	r1, #82
	movs	r2, #5
	movs	r3, #6
	bl 0x0200c000
	movs	r0, #88
	movs	r1, #82
	movs	r2, #36
	movs	r3, #25
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bff0
	movs	r3, #36
	movs	r2, #25
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #88
	movs	r1, #82
	movs	r2, #5
	movs	r3, #5
	bl 0x0200c000
	cmp	r7, #15
	bne.n	.L_0200137e
	cmp	r6, #41
	bne.n	.L_0200136e
	movs	r0, #14
	b.n	.L_020013ac
.L_0200136e:
	cmp	r6, #43
	bne.n	.L_020013ec
	movs	r0, #16
	bl 0x020092d4
	adds	r5, r0, #0
	movs	r0, #43
	b.n	.L_020013b4
.L_0200137e:
	cmp	r7, #16
	bne.n	.L_020013a2
	cmp	r6, #40
	bne.n	.L_02001392
	movs	r0, #16
	bl 0x020092d4
	adds	r5, r0, #0
	movs	r0, #39
	b.n	.L_020013b4
.L_02001392:
	cmp	r6, #41
	bne.n	.L_020013ec
.L_02001396:
	movs	r0, #16
	bl 0x020092d4
	adds	r5, r0, #0
	movs	r0, #42
	b.n	.L_020013b4
.L_020013a2:
	cmp	r7, #18
.L_020013a4:
	bne.n	.L_020013ec
	cmp	r6, #40
	bne.n	.L_020013ca
	movs	r0, #18
.L_020013ac:
	bl 0x020092d4
	adds	r5, r0, #0
.L_020013b2:
	movs	r0, #41
.L_020013b4:
	bl 0x020092d4
	lsls	r5, r5, #16
	adds	r2, r0, #0
	mov	r3, r8
	ldr	r0, [r3, #0]
.L_020013c0:
	lsls	r2, r2, #16
	adds	r1, r5, #0
	bl 0x0200c090
	b.n	.L_020013ec
.L_020013ca:
	cmp	r6, #43
	bne.n	.L_020013ec
	movs	r0, #17
	bl 0x020092d4
	adds	r5, r0, #0
	movs	r0, #43
	bl 0x020092d4
	lsls	r5, r5, #16
	adds	r2, r0, #0
	mov	r3, r8
	ldr	r0, [r3, #0]
	lsls	r2, r2, #16
	adds	r1, r5, #0
	bl 0x0200c090
.L_020013ec:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #276]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r2, r2, r3
	ldr	r0, [r2, #0]
	sub	sp, #8
	mov	r8, r2
	bl 0x0200c058
	ldr	r3, [r0, #8]
	asrs	r7, r3, #20
	ldr	r3, [r0, #16]
	movs	r0, #64
	asrs	r6, r3, #20
	bl 0x0200c058
	cmp	r0, #0
	beq.n	.L_0200142c
	movs	r0, #64
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
.L_0200142c:
	movs	r3, #6
	str	r3, [sp, #4]
	movs	r5, #5
	movs	r0, #82
	movs	r1, #89
	movs	r2, #15
	movs	r3, #40
	str	r5, [sp, #0]
	bl 0x0200bff0
	movs	r3, #15
	movs	r2, #40
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #82
	movs	r1, #89
	movs	r2, #5
	movs	r3, #6
	bl 0x0200c000
	movs	r0, #88
	movs	r1, #89
	movs	r2, #36
	movs	r3, #25
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bff0
	movs	r3, #36
	movs	r2, #25
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #88
	movs	r1, #89
	movs	r2, #5
	movs	r3, #5
	bl 0x0200c000
	cmp	r7, #15
	bne.n	.L_020014a0
	cmp	r6, #44
	bne.n	.L_020014a0
	movs	r0, #15
	bl 0x020092d4
	adds	r5, r0, #0
	movs	r0, #43
	bl 0x020092d4
	lsls	r5, r5, #16
	adds	r2, r0, #0
	mov	r3, r8
	ldr	r0, [r3, #0]
	lsls	r2, r2, #16
	adds	r1, r5, #0
	bl 0x0200c090
	b.n	.L_0200150a
.L_020014a0:
	cmp	r7, #17
	bne.n	.L_020014b4
	cmp	r6, #44
	bne.n	.L_020014b4
	movs	r0, #17
	bl 0x020092d4
	adds	r6, r0, #0
	movs	r0, #43
	b.n	.L_020014c6
.L_020014b4:
	cmp	r7, #19
	bne.n	.L_0200150a
	cmp	r6, #40
	bne.n	.L_020014e2
	movs	r0, #19
	bl 0x020092d4
	adds	r6, r0, #0
	movs	r0, #41
.L_020014c6:
	bl 0x020092d4
	ldr	r5, [pc, #72]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r2, r0, #0
	adds	r5, r5, r3
	lsls	r6, r6, #16
	ldr	r0, [r5, #0]
	lsls	r2, r2, #16
	adds	r1, r6, #0
	bl 0x0200c090
	b.n	.L_0200150a
.L_020014e2:
	cmp	r6, #42
	bne.n	.L_0200150a
	movs	r0, #18
	bl 0x020092d4
	adds	r6, r0, #0
	movs	r0, #42
	bl 0x020092d4
	ldr	r5, [pc, #28]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r2, r0, #0
	adds	r5, r5, r3
	lsls	r6, r6, #16
	ldr	r0, [r5, #0]
	lsls	r2, r2, #16
	adds	r1, r6, #0
	bl 0x0200c090
.L_0200150a:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #64
	bl 0x0200c058
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001576
	ldr	r3, [pc, #80]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r7, r3, r1
	ldr	r0, [r7, #0]
	bl 0x0200c058
	movs	r1, #144
	movs	r2, #240
	adds	r6, r0, #0
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #64
	bl 0x0200c090
	ldr	r2, [r6, #8]
	ldr	r3, [r5, #8]
	movs	r1, #128
	lsls	r1, r1, #12
	adds	r2, r2, r1
	adds	r3, r3, r1
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02001576
	ldr	r2, [r6, #16]
	ldr	r3, [r5, #16]
	adds	r2, r2, r1
	adds	r3, r3, r1
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02001576
	movs	r1, #144
	movs	r2, #128
	ldr	r0, [r7, #0]
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200c090
.L_02001576:
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #64
	bl 0x0200c058
	cmp	r0, #0
	beq.n	.L_02001592
	movs	r0, #64
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
.L_02001592:
	pop	{pc}
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #72
	movs	r3, #32
	bl 0x0200bff0
	movs	r3, #32
	str	r3, [sp, #4]
	movs	r5, #8
	movs	r0, #64
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200c000
	movs	r3, #96
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200c000
	add	sp, #8
	pop	{r5, pc}
	push	{lr}
	cmp	r0, #1
	bne.n	.L_020015e8
	bl 0x02009594
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #141
	bl 0x0200bf98
.L_020015e8:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #44]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_0200160a
	movs	r1, #11
	movs	r2, #2
	bl 0x02009650
	b.n	.L_02001618
.L_0200160a:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02001618
	movs	r1, #9
	movs	r2, #7
	bl 0x02009650
.L_02001618:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000004a
	.2byte 0x004e
	.2byte 0x0000
	push	{lr}
	movs	r0, #1
	bl 0x020095ec
	movs	r0, #135
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bf98
	pop	{pc}
	push	{lr}
	movs	r0, #0
	bl 0x020095ec
	movs	r0, #135
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bfa0
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r2, #0
	movs	r5, #0
	mov	r8, r0
	adds	r7, r1, #0
	cmp	r5, r6
	bcs.n	.L_02001688
.L_02001662:
	adds	r0, r7, r5
	bl 0x0200c058
	mov	r3, r8
	adds	r0, #35
	adds	r1, r5, #1
	cmp	r3, #0
	beq.n	.L_0200167a
	ldrb	r2, [r0, #0]
	movs	r3, #239
	ands	r3, r2
	b.n	.L_02001680
.L_0200167a:
	ldrb	r2, [r0, #0]
	movs	r3, #16
	orrs	r3, r2
.L_02001680:
	strb	r3, [r0, #0]
	adds	r5, r1, #0
	cmp	r5, r6
	bcc.n	.L_02001662
.L_02001688:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
	ldr	r0, [pc, #1016]
	bl 0x0200c0c8
	movs	r0, #9
	bl 0x0200c070
	movs	r0, #1
	bl 0x0200bf20
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #9
	bl 0x0200c100
	movs	r2, #20
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c0e0
	movs	r1, #0
	movs	r0, #9
	bl 0x0200c0d0
	movs	r0, #8
	bl 0x0200c070
	movs	r0, #1
	bl 0x0200bf20
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0e0
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0e0
	movs	r1, #208
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c0e0
	movs	r1, #208
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c0e0
	movs	r1, #208
	movs	r0, #13
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c0e0
	movs	r1, #208
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c0e0
	ldr	r6, [pc, #892]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200c100
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200c0e0
	bl 0x0200c130
	movs	r1, #204
	movs	r3, #0
	adds	r0, #85
	lsls	r1, r1, #6
	strb	r3, [r0, #0]
	adds	r1, #51
	ldr	r0, [pc, #848]
	bl 0x0200c118
	movs	r0, #212
	movs	r2, #212
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r0, r0, #16
	movs	r1, #0
	bl 0x0200c120
	bl 0x0200c128
	movs	r0, #12
	movs	r1, #0
	bl 0x0200c0d0
	movs	r2, #10
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c0e0
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c0d0
	movs	r1, #176
	movs	r0, #13
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c0e0
	movs	r1, #160
	movs	r0, #14
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200c0e0
	movs	r2, #40
	movs	r0, #14
	movs	r1, #4
	bl 0x0200c0b0
	movs	r0, #13
	movs	r1, #3
	bl 0x0200c0a8
	movs	r0, #14
	movs	r1, #3
	bl 0x0200c0a8
	movs	r0, #8
	movs	r1, #14
	bl 0x0200c1c0
	movs	r0, #9
	movs	r1, #14
	bl 0x0200c1c0
	movs	r0, #10
	movs	r1, #14
	bl 0x0200c1c0
	movs	r0, #11
	movs	r1, #14
	bl 0x0200c1c0
	movs	r0, #12
	movs	r1, #14
	bl 0x0200c1c0
	movs	r0, #13
	movs	r1, #14
	bl 0x0200c1c0
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #13
	lsls	r1, r1, #9
	bl 0x0200c060
	ldr	r1, [pc, #688]
	movs	r0, #14
	bl 0x0200c068
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200c118
	movs	r0, #240
	movs	r1, #1
	movs	r2, #130
	lsls	r2, r2, #17
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200c120
	bl 0x0200c128
	movs	r0, #8
	bl 0x0200c070
	movs	r0, #9
	bl 0x0200c070
	movs	r0, #10
	bl 0x0200c070
	movs	r0, #11
	bl 0x0200c070
	movs	r0, #12
	bl 0x0200c070
	movs	r0, #13
	bl 0x0200c070
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #608]
	adds	r1, #204
	bl 0x0200c118
	movs	r0, #212
	movs	r1, #1
	movs	r2, #212
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #16
	bl 0x0200c120
	bl 0x0200c128
	movs	r0, #20
	bl 0x0200c038
	movs	r1, #208
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c0e0
	movs	r1, #160
	movs	r2, #20
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200c0e0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c0d0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200c0a8
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c0d0
	movs	r0, #10
	movs	r1, #4
	movs	r2, #0
	bl 0x0200c0b0
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c0e0
	movs	r0, #10
	movs	r1, #4
	bl 0x0200c0a0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c0d0
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c0e0
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c0e0
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #8
	bl 0x0200c100
	movs	r1, #129
	movs	r2, #80
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200c100
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c0d0
	movs	r1, #208
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c0e0
	movs	r1, #192
	movs	r0, #10
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c0e0
	movs	r1, #208
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c0e0
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0e0
	movs	r1, #128
	movs	r2, #20
	movs	r0, #13
	lsls	r1, r1, #8
	bl 0x0200c0e0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200c0a0
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c0a0
	movs	r0, #10
	movs	r1, #3
	bl 0x0200c0a0
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c0a0
	movs	r0, #12
	movs	r1, #3
	bl 0x0200c0a0
	movs	r0, #13
	movs	r1, #3
	bl 0x0200c0a8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #312]
	adds	r2, #204
	bl 0x0200c060
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #9
	ldr	r1, [pc, #296]
	adds	r2, #204
	bl 0x0200c060
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #10
	ldr	r1, [pc, #284]
	adds	r2, #204
	bl 0x0200c060
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #11
	ldr	r1, [pc, #268]
	adds	r2, #204
	bl 0x0200c060
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #256]
	adds	r2, #204
	bl 0x0200c060
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #13
	ldr	r1, [pc, #240]
	bl 0x0200c060
	ldr	r0, [r6, #0]
	movs	r1, #13
	bl 0x0200c1c0
	ldr	r5, [pc, #236]
	movs	r0, #13
	adds	r1, r5, #0
	bl 0x0200c068
	movs	r0, #10
	bl 0x0200c038
	adds	r1, r5, #0
	movs	r0, #12
	bl 0x0200c068
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	adds	r1, #204
	bl 0x0200c118
	movs	r0, #240
	movs	r1, #1
	movs	r2, #140
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200c120
	adds	r1, r5, #0
	movs	r0, #8
	bl 0x0200c068
	movs	r0, #10
	bl 0x0200c038
	adds	r1, r5, #0
	movs	r0, #9
	bl 0x0200c068
	ldr	r5, [pc, #160]
	movs	r0, #10
	adds	r1, r5, #0
	bl 0x0200c068
	movs	r0, #20
	bl 0x0200c038
	adds	r1, r5, #0
	movs	r0, #11
	bl 0x0200c078
	ldr	r0, [r6, #0]
	bl 0x0200c070
	movs	r0, #1
	bl 0x0200bf20
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r6, #176
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	lsls	r6, r6, #8
	movs	r1, #130
	movs	r2, #242
	adds	r3, r6, #0
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200c098
	movs	r3, #128
	lsls	r3, r3, #8
	movs	r0, #9
	ldr	r1, [pc, #32]
	ldr	r2, [pc, #36]
	b.n	.L_02001abc
	.2byte 0x0000
	.4byte 0x00001aa9
	.4byte 0x02000240
	.4byte 0x00019999
	.4byte 0x0200c68c
	.4byte 0x00026666
	.4byte 0x0200c6dc
	.4byte 0x0200c718
	.4byte 0x02150000
	.2byte 0x0000
	.2byte 0x010f
.L_02001abc:
	bl 0x0200c098
	movs	r3, #192
	movs	r2, #232
	lsls	r3, r3, #6
	movs	r0, #10
	ldr	r1, [pc, #152]
	lsls	r2, r2, #16
	bl 0x0200c098
	movs	r3, #160
	movs	r2, #248
	lsls	r3, r3, #7
	movs	r0, #11
	ldr	r1, [pc, #140]
	lsls	r2, r2, #16
	movs	r5, #208
	bl 0x0200c098
	lsls	r5, r5, #8
	movs	r1, #231
	movs	r2, #170
	adds	r3, r5, #0
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200c098
	movs	r1, #241
	movs	r2, #170
	adds	r3, r5, #0
	movs	r0, #13
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200c098
	movs	r2, #160
	lsls	r2, r2, #17
	adds	r3, r6, #0
	movs	r0, #14
	ldr	r1, [pc, #92]
	bl 0x0200c098
	movs	r0, #8
	movs	r1, #1
	bl 0x0200c0a0
	movs	r0, #9
	movs	r1, #1
	bl 0x0200c0a0
	movs	r0, #10
	movs	r1, #1
	bl 0x0200c0a0
	movs	r0, #11
	movs	r1, #1
	bl 0x0200c0a0
	movs	r0, #12
	movs	r1, #1
	bl 0x0200c0a0
	movs	r0, #13
	movs	r1, #1
	bl 0x0200c0a0
	movs	r1, #1
	movs	r0, #14
	bl 0x0200c0a0
	movs	r0, #143
	lsls	r0, r0, #4
	bl 0x0200bf98
	movs	r0, #242
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200bf98
	bl 0x0200c048
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x01b30000
	.4byte 0x01d30000
	.2byte 0x0000
	.2byte 0x01e9
	push	{r5, r6, r7, lr}
	movs	r0, #234
	movs	r1, #232
	movs	r2, #128
	movs	r3, #200
	adds	r0, #255
	lsls	r1, r1, #16
	lsls	r2, r2, #14
	lsls	r3, r3, #16
	bl 0x0200bfc0
	adds	r6, r0, #0
	movs	r7, #0
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L_02001be8
	ldr	r5, [r6, #80]
	movs	r3, #33
	ldrb	r2, [r5, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r5, #9]
	adds	r3, r6, #0
	adds	r3, #85
	adds	r2, r6, #0
	strb	r7, [r3, #0]
	adds	r2, #92
	movs	r3, #1
	movs	r1, #193
	strb	r3, [r2, #0]
	lsls	r1, r1, #3
	strb	r7, [r5, #26]
	strb	r7, [r5, #27]
	movs	r0, #68
	bl 0x0200bf50
	adds	r7, r0, #0
	movs	r0, #209
	bl 0x0200c030
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r2, r7, r3
	movs	r1, #128
	ldrb	r0, [r5, #16]
	bl 0x0200bf78
	movs	r0, #68
	bl 0x0200bf58
	adds	r0, r6, #0
.L_02001be8:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
	bl 0x0200c158
	bl 0x0200c168
	movs	r0, #20
	bl 0x0200c038
	ldr	r3, [pc, #80]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	movs	r1, #28
	ldr	r0, [r6, #0]
	bl 0x0200c0a0
	bl 0x02009b70
	movs	r1, #0
	adds	r5, r0, #0
	movs	r0, #209
	bl 0x0200c050
	cmp	r5, #0
	beq.n	.L_02001c40
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x0200bfd0
	movs	r0, #1
	bl 0x0200bf20
	adds	r0, r5, #0
	bl 0x0200bfc8
.L_02001c40:
	ldr	r0, [r6, #0]
	movs	r1, #1
	bl 0x0200c0a0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #140
	bl 0x0200bf98
	bl 0x0200c048
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	ldr	r3, [pc, #72]
	movs	r1, #181
	ldr	r3, [r3, #0]
	lsls	r1, r1, #1
	adds	r2, r2, r1
	strh	r3, [r2, #0]
	ldr	r3, [pc, #64]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02001cae
	ldr	r3, [pc, #60]
.L_02001c7a:
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02001c92
	movs	r0, #16
	bl 0x0200c058
	b.n	.L_02001c9e
.L_02001c92:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02001cae
	movs	r0, #21
	bl 0x0200c058
.L_02001c9e:
	ldr	r3, [pc, #20]
	ldr	r2, [r3, #0]
	ldr	r3, [r0, #8]
	str	r3, [r2, #8]
	ldr	r3, [r0, #16]
	str	r3, [r2, #16]
	ldrh	r3, [r0, #6]
	strh	r3, [r2, #6]
.L_02001cae:
	pop	{pc}
	.4byte 0x0200c808
	.4byte 0x0200c80c
	.4byte 0x02000240
	.4byte 0x00000046
	.2byte 0x0047
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r5, [pc, #368]
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	bl 0x0200c058
	movs	r1, #0
	bl 0x0200c008
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r5, r5, r3
	ldrb	r3, [r5, #0]
	cmp	r3, #5
	beq.n	.L_02001dce
	ldr	r3, [pc, #332]
	ldr	r5, [r3, #0]
	movs	r3, #1
	ands	r5, r3
	cmp	r5, #0
	bne.n	.L_02001dce
	movs	r0, #30
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	adds	r0, #255
	bl 0x0200bfc0
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02001dce
	adds	r3, r6, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r3, #4
	strb	r5, [r3, #0]
	movs	r1, #0
	bl 0x0200c008
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200bfa8
	ldr	r1, [pc, #280]
	adds	r0, r6, #0
	bl 0x0200bfb8
	ldr	r1, [r6, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldr	r5, [pc, #260]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001d88
	bl 0x0200bf38
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r0, r0, #14
	bl 0x0200bf48
	add	r0, r8
	str	r0, [r6, #68]
	bl 0x0200bf38
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r0, r0, #14
	bl 0x0200bf40
	add	r0, sl
	str	r0, [r6, #76]
	movs	r2, #2
	ldr	r3, [r5, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001db0
	ldr	r3, [r6, #68]
	mov	r2, sl
	add	r3, r8
	str	r3, [r6, #68]
	adds	r3, r0, r2
	str	r3, [r6, #76]
	b.n	.L_02001db0
.L_02001d88:
	bl 0x0200bf38
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r0, r0, #14
	bl 0x0200bf48
	asrs	r0, r0, #1
	add	r0, r8
	str	r0, [r6, #68]
	bl 0x0200bf38
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r0, r0, #14
	bl 0x0200bf40
	asrs	r0, r0, #1
	add	r0, sl
	str	r0, [r6, #76]
.L_02001db0:
	movs	r5, #0
	str	r5, [r6, #72]
	bl 0x0200bf38
	ldr	r3, [pc, #148]
	lsls	r0, r0, #9
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r6, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	ldr	r3, [pc, #140]
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	str	r3, [r6, #108]
.L_02001dce:
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_02001e06
	ldr	r3, [r7, #8]
	movs	r2, #240
	add	r3, r8
	str	r3, [r7, #8]
	ldr	r3, [pc, #108]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02001e36
	ldr	r1, [r7, #16]
	asrs	r3, r1, #16
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_02001df2
	adds	r2, #15
.L_02001df2:
	asrs	r2, r2, #4
	lsls	r2, r2, #4
	subs	r2, r3, r2
	movs	r3, #8
	subs	r3, r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #3
	adds	r3, r1, r3
	str	r3, [r7, #16]
	b.n	.L_02001e36
.L_02001e06:
	ldr	r3, [r7, #16]
	movs	r2, #240
	add	r3, sl
	str	r3, [r7, #16]
	ldr	r3, [pc, #60]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02001e36
	ldr	r1, [r7, #8]
	asrs	r3, r1, #16
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_02001e24
	adds	r2, #15
.L_02001e24:
	asrs	r2, r2, #4
	lsls	r2, r2, #4
	subs	r2, r3, r2
	movs	r3, #8
	subs	r3, r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #3
	adds	r3, r1, r3
	str	r3, [r7, #8]
.L_02001e36:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0300122c
	.4byte 0x0200c2f0
	.4byte 0x03001150
	.4byte 0xffffff00
	.2byte 0x8b05
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #408]
	movs	r2, #133
	mov	r9, r1
	movs	r0, #192
	lsls	r2, r2, #2
	lsls	r0, r0, #18
	add	r2, r9
	ldr	r5, [r0, #108]
	mov	r8, r0
	ldr	r0, [r2, #0]
	mov	fp, r2
	bl 0x0200c058
	adds	r6, r0, #0
	movs	r0, #179
	lsls	r0, r0, #1
	adds	r3, r5, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r7, #192
	lsls	r7, r7, #9
	cmp	r3, #0
	bne.n	.L_02001eb0
	movs	r2, #173
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	bne.n	.L_02001eb0
	movs	r1, #175
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02001ebe
.L_02001eb0:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200bfb0
	bl 0x0200c010
	b.n	.L_02001ff4
.L_02001ebe:
	mov	r3, r8
	adds	r0, r6, #0
	ldr	r5, [r3, #32]
	bl 0x0200c1e0
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bge.n	.L_02001ed2
	ldr	r1, [pc, #308]
	adds	r3, r3, r1
.L_02001ed2:
	ldr	r2, [r6, #16]
	movs	r1, #128
	lsls	r1, r1, #11
	asrs	r4, r3, #20
	adds	r3, r2, r1
	cmp	r3, #0
	bge.n	.L_02001ee4
	ldr	r1, [pc, #292]
	adds	r3, r2, r1
.L_02001ee4:
	movs	r1, #156
	lsls	r1, r1, #1
	adds	r2, r5, r1
	asrs	r3, r3, #20
	ldr	r2, [r2, #0]
	lsls	r3, r3, #7
	adds	r3, r4, r3
	lsls	r1, r0, #2
	lsls	r3, r3, #2
	adds	r0, r2, r1
	adds	r2, r2, r3
	mov	r8, r2
	movs	r2, #184
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	mov	sl, r0
	adds	r5, r3, r1
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200bfb0
	bl 0x0200c018
	ldrb	r3, [r5, #2]
	movs	r0, #128
	adds	r3, #246
	lsls	r3, r3, #24
	lsls	r0, r0, #19
	cmp	r3, r0
	bhi.n	.L_02001f42
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #5
	bne.n	.L_02001f34
	movs	r7, #0
	b.n	.L_02001f42
.L_02001f34:
	mov	r1, fp
	ldr	r0, [r1, #0]
	bl 0x0200c058
	movs	r1, #1
	bl 0x0200c008
.L_02001f42:
	mov	r2, r8
	ldrb	r3, [r2, #2]
	cmp	r3, #255
	bne.n	.L_02001f4c
	movs	r7, #0
.L_02001f4c:
	mov	r3, sl
	ldrb	r2, [r3, #2]
	cmp	r2, #11
	beq.n	.L_02001f7a
	cmp	r2, #11
	bgt.n	.L_02001f5e
	cmp	r2, #10
	beq.n	.L_02001f68
	b.n	.L_02001fac
.L_02001f5e:
	cmp	r2, #12
	beq.n	.L_02001f8a
	cmp	r2, #13
	beq.n	.L_02001f9c
	b.n	.L_02001fac
.L_02001f68:
	ldr	r3, [pc, #160]
	movs	r2, #64
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001fac
	negs	r2, r7
	asrs	r2, r2, #2
	b.n	.L_02001fc8
.L_02001f7a:
	ldr	r3, [pc, #144]
	movs	r2, #128
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001fac
	asrs	r2, r7, #2
	b.n	.L_02001fc8
.L_02001f8a:
	ldr	r3, [pc, #128]
	movs	r2, #32
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001fac
	negs	r1, r7
	asrs	r1, r1, #2
	b.n	.L_02001fe0
.L_02001f9c:
	ldr	r3, [pc, #108]
	movs	r2, #16
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001fac
	asrs	r1, r7, #2
	b.n	.L_02001fe0
.L_02001fac:
	ldrb	r0, [r5, #2]
	cmp	r0, #11
	beq.n	.L_02001fd2
	cmp	r0, #11
	bgt.n	.L_02001fbc
	cmp	r0, #10
	beq.n	.L_02001fc6
	b.n	.L_02001ff4
.L_02001fbc:
	cmp	r0, #12
	beq.n	.L_02001fde
	cmp	r0, #13
	beq.n	.L_02001fea
	b.n	.L_02001ff4
.L_02001fc6:
	negs	r2, r7
.L_02001fc8:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009cc4
	b.n	.L_02001ff4
.L_02001fd2:
	adds	r0, r6, #0
	movs	r1, #0
	adds	r2, r7, #0
	bl 0x02009cc4
	b.n	.L_02001ff4
.L_02001fde:
	negs	r1, r7
.L_02001fe0:
	adds	r0, r6, #0
	movs	r2, #0
	bl 0x02009cc4
	b.n	.L_02001ff4
.L_02001fea:
	adds	r0, r6, #0
	adds	r1, r7, #0
	movs	r2, #0
	bl 0x02009cc4
.L_02001ff4:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x000fffff
	.4byte 0x0013ffff
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, lr}
	ldr	r6, [pc, #136]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r6, r1
	ldr	r0, [r3, #0]
	bl 0x0200c058
	adds	r5, r0, #0
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #13
	cmp	r3, r2
	ble.n	.L_02002038
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r6, r3
	movs	r3, #1
	b.n	.L_02002042
.L_02002038:
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r2, r6, r1
	movs	r3, #0
.L_02002042:
	strb	r3, [r2, #0]
	ldr	r6, [pc, #84]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02002098
	ldr	r3, [r5, #80]
	movs	r0, #18
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x0200c0f0
	ldr	r3, [r5, #8]
	movs	r2, #236
	lsls	r2, r2, #17
	cmp	r3, r2
	ble.n	.L_02002098
	ldr	r3, [r5, #16]
	movs	r1, #244
	lsls	r1, r1, #17
	cmp	r3, r1
	ble.n	.L_02002098
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #40]
	cmp	r3, r2
	ble.n	.L_0200208c
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r6, r3
	movs	r3, #1
	b.n	.L_02002096
.L_0200208c:
	movs	r1, #226
	lsls	r1, r1, #1
.L_02002090:
	adds	r1, #255
	adds	r2, r6, r1
	movs	r3, #0
.L_02002096:
	strb	r3, [r2, #0]
.L_02002098:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000045
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb520
	ldr	r5, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200c058
	ldr	r2, [pc, #36]
	ldr	r3, [r0, #12]
	cmp	r3, r2
	ble.n	.L_020020cc
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r5, r3
	movs	r3, #1
	b.n	.L_020020d6
.L_020020cc:
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r5, r3
	movs	r3, #0
.L_020020d6:
	strb	r3, [r2, #0]
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb5e0
	ldr	r7, [pc, #84]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl 0x0200c058
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	adds	r6, r0, #0
	bl 0x0200c1e0
	movs	r3, #156
	lsls	r3, r3, #1
	adds	r5, r5, r3
	ldr	r3, [r5, #0]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldrb	r3, [r3, #2]
	cmp	r3, #20
	bne.n	.L_0200212a
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #13
	cmp	r3, r2
	ble.n	.L_0200212a
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r7, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_02002138
.L_0200212a:
	ldr	r3, [pc, #16]
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	movs	r2, #0
	strb	r2, [r3, #0]
.L_02002138:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #56]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c058
	ldr	r3, [r0, #8]
	asrs	r2, r3, #20
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r2, #12
	bls.n	.L_02002172
	cmp	r3, #10
	bls.n	.L_02002172
	cmp	r2, #15
	bhi.n	.L_02002172
	cmp	r3, #13
	bhi.n	.L_02002172
	movs	r0, #8
	adds	r0, #255
	bl 0x0200bf98
	b.n	.L_0200217a
.L_02002172:
	movs	r0, #8
	adds	r0, #255
	bl 0x0200bfa0
.L_0200217a:
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020021c4
	ldr	r3, [pc, #48]
	movs	r1, #5
	ldr	r0, [r3, #0]
	bl 0x0200bf18
	cmp	r0, #0
	bne.n	.L_020021c4
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #5
	bne.n	.L_020021bc
	ldr	r3, [pc, #24]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_020021c4
.L_020021bc:
	movs	r0, #149
	lsls	r0, r0, #2
	bl 0x0200c1f8
.L_020021c4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0x02000240
	.4byte 0x03001150
	.4byte 0x049b23c0
	.4byte 0x22b56edb
	.4byte 0x189b0052
	.4byte 0x801a2200
	.4byte 0x601a4b01
	.4byte 0x00004770
	.2byte 0xc808
	.2byte 0x0200
	push	{lr}
	ldr	r2, [r0, #56]
	movs	r3, #128
	lsls	r3, r3, #24
	cmp	r2, r3
	bne.n	.L_02002204
	ldr	r3, [r0, #64]
	movs	r0, #1
	cmp	r3, r2
	beq.n	.L_02002206
.L_02002204:
	movs	r0, #0
.L_02002206:
	pop	{pc}
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r5, #0
	adds	r6, #98
	ldrb	r3, [r6, #0]
	cmp	r3, #9
	bhi.n	.L_0200230c
	ldr	r2, [pc, #248]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200a248
	.4byte 0x0200a25c
	.4byte 0x0200a26a
	.4byte 0x0200a27e
	.4byte 0x0200a28c
	.4byte 0x0200a2a0
	.4byte 0x0200a2ae
	.4byte 0x0200a2c2
	.4byte 0x0200a2ea
	.4byte 0x0200a2fe
	.4byte 0x23c8218c
	.4byte 0x1c28041b
	.4byte 0x22000449
	.4byte 0xfec4f001
	.4byte 0xe0562301
	.4byte 0xf7ff1c28
	.4byte 0x2800ffc7
	.4byte 0x2302d052
	.4byte 0x218ce04f
	.4byte 0x045b2384
	.4byte 0x04491c28
	.4byte 0xf0012200
	.4byte 0x2303feb3
	.4byte 0x1c28e045
	.4byte 0xffb6f7ff
	.4byte 0xd0412800
	.4byte 0xe03e2304
	.4byte 0x238421b8
	.4byte 0x1c28045b
	.4byte 0x22000409
	.4byte 0xfea2f001
	.4byte 0xe0342305
	.4byte 0xf7ff1c28
	.4byte 0x2800ffa5
	.4byte 0x2306d030
	.4byte 0x21b8e02d
	.4byte 0x041b23f8
	.4byte 0x04091c28
	.4byte 0xf0012200
	.4byte 0x2307fe91
	.4byte 0x1c28e023
	.4byte 0xff94f7ff
	.4byte 0xd01f2800
	.4byte 0x00402082
	.4byte 0xf00130ff
	.4byte 0x2800fe5d
	.4byte 0x4a0ed005
	.4byte 0x66eb2300
	.4byte 0x601323c8
	.4byte 0x2308e012
	.4byte 0x21b8e00f
	.4byte 0x041b23c8
	.4byte 0x04091c28
	.4byte 0xf0012200
	.4byte 0x2309fe73
	.4byte 0x1c28e005
	.4byte 0xff76f7ff
	.4byte 0xd0012800
	.2byte 0x2300
	.2byte 0x7033
.L_0200230c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200a220
	.2byte 0xc808
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #16
	bl 0x0200c058
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r6, r0, #0
	ldr	r0, [pc, #144]
	bl 0x0200bf28
	movs	r0, #17
	ldr	r5, [pc, #140]
	bl 0x0200c058
	adds	r1, r0, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	str	r0, [r5, #0]
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	movs	r0, #17
	bl 0x0200c0f8
	ldr	r3, [r5, #0]
	movs	r5, #3
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r0, #17
	movs	r1, #3
	bl 0x0200c0f0
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #16
	ldr	r1, [pc, #84]
	bl 0x0200c060
	adds	r0, r6, #0
	movs	r1, #15
	bl 0x0200c0c0
	movs	r1, #3
	movs	r0, #16
	bl 0x0200c0f0
	movs	r0, #16
	bl 0x0200c0f8
	movs	r0, #16
	bl 0x0200c058
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bf90
	cmp	r0, #0
	bne.n	.L_020023a6
	adds	r3, r6, #0
	adds	r3, #98
	strb	r0, [r3, #0]
	ldr	r3, [pc, #32]
	str	r3, [r6, #108]
.L_020023a6:
	bl 0x0200a1d4
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #24]
	bl 0x0200bf28
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200a181
	.4byte 0x0200c80c
	.4byte 0x00019999
	.4byte 0x0200a209
	.2byte 0x9c5d
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r5, #0
	adds	r6, #98
	ldrb	r3, [r6, #0]
	cmp	r3, #39
	bls.n	.L_020023dc
	b.n	.L_02002696
.L_020023dc:
	ldr	r2, [pc, #696]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200a484
	.4byte 0x0200a498
	.4byte 0x0200a4a8
	.4byte 0x0200a4bc
	.4byte 0x0200a4cc
	.4byte 0x0200a4e0
	.4byte 0x0200a508
	.4byte 0x0200a51c
	.4byte 0x0200a52c
	.4byte 0x0200a540
	.4byte 0x0200a550
	.4byte 0x0200a564
	.4byte 0x0200a574
	.4byte 0x0200a588
	.4byte 0x0200a598
	.4byte 0x0200a5ac
	.4byte 0x0200a5ba
	.4byte 0x0200a5ce
	.4byte 0x0200a5e8
	.4byte 0x0200a5fc
	.4byte 0x0200a696
	.4byte 0x0200a696
	.4byte 0x0200a696
	.4byte 0x0200a696
	.4byte 0x0200a696
	.4byte 0x0200a696
	.4byte 0x0200a696
	.4byte 0x0200a696
	.4byte 0x0200a696
	.4byte 0x0200a696
	.4byte 0x0200a696
	.4byte 0x0200a696
	.4byte 0x0200a60a
	.4byte 0x0200a622
	.4byte 0x0200a630
	.4byte 0x0200a644
	.4byte 0x0200a652
	.4byte 0x0200a666
	.4byte 0x0200a674
	.4byte 0x0200a688
	.4byte 0x23a6219c
	.4byte 0x1c28049b
	.4byte 0x22000449
	.4byte 0xfda6f001
	.4byte 0xe0fd2301
	.4byte 0xf7ff1c28
	.4byte 0x2800fea9
	.4byte 0xe0f8d100
	.4byte 0xe0f52302
	.4byte 0x23b6219c
	.4byte 0x1c28049b
	.4byte 0x22000449
	.4byte 0xfd94f001
	.4byte 0xe0eb2303
	.4byte 0xf7ff1c28
	.4byte 0x2800fe97
	.4byte 0xe0e6d100
	.4byte 0xe0e32304
	.4byte 0x23b621f6
	.4byte 0x1c28049b
	.4byte 0x22000409
	.4byte 0xfd82f001
	.4byte 0xe0d92305
	.4byte 0xf7ff1c28
	.4byte 0x2800fe85
	.4byte 0xe0d4d100
	.4byte 0x00802080
	.4byte 0xfd4ef001
	.4byte 0xd0052800
	.4byte 0x23004a68
	.4byte 0x23c966eb
	.4byte 0xe0c86013
	.4byte 0xe0c52306
	.4byte 0x23b621b8
	.4byte 0x1c28049b
	.4byte 0x22000409
	.4byte 0xfd64f001
	.4byte 0xe0bb2307
	.4byte 0xf7ff1c28
	.4byte 0x2800fe67
	.4byte 0xe0b6d100
	.4byte 0xe0b32308
	.4byte 0x23b221b8
	.4byte 0x1c28049b
	.4byte 0x22000409
	.4byte 0xfd52f001
	.4byte 0xe0a92309
	.4byte 0xf7ff1c28
	.4byte 0x2800fe55
	.4byte 0xe0a4d100
	.4byte 0xe0a1230a
	.4byte 0x23b2218c
	.4byte 0x1c28049b
	.4byte 0x22000449
	.4byte 0xfd40f001
	.4byte 0xe097230b
	.4byte 0xf7ff1c28
	.4byte 0x2800fe43
	.4byte 0xe092d100
	.4byte 0xe08f230c
	.4byte 0x23be218c
	.4byte 0x1c28049b
	.4byte 0x22000449
	.4byte 0xfd2ef001
	.4byte 0xe085230d
	.4byte 0xf7ff1c28
	.4byte 0x2800fe31
	.4byte 0xe080d100
	.4byte 0xe07d230e
	.4byte 0x23be21e8
	.4byte 0x1c28049b
	.4byte 0x22000409
	.4byte 0xfd1cf001
	.4byte 0xe073230f
	.4byte 0xf7ff1c28
	.4byte 0x2800fe1f
	.4byte 0x2310d06f
	.4byte 0x21e8e06c
	.4byte 0x049b23ba
	.4byte 0x04091c28
	.4byte 0xf0012200
	.4byte 0x2311fd0b
	.4byte 0x1c28e062
	.4byte 0xfe0ef7ff
	.4byte 0xd05e2800
	.4byte 0x00802080
	.4byte 0xfcd8f001
	.4byte 0xd1562800
	.4byte 0xe0552312
	.4byte 0x23a621e8
	.4byte 0x1c28049b
	.4byte 0x22000409
	.4byte 0xfcf4f001
	.4byte 0xe04b2313
	.4byte 0xf7ff1c28
	.4byte 0x2800fdf7
	.4byte 0x2300d047
	.4byte 0xf7ffe044
	.4byte 0x2194fde3
	.4byte 0x049b23ba
	.4byte 0x04491c28
	.4byte 0xf0012200
	.4byte 0x2321fce1
	.4byte 0x1c28e038
	.4byte 0xfde4f7ff
	.4byte 0xd0342800
	.4byte 0xe0312322
	.4byte 0x23be2194
	.4byte 0x1c28049b
	.4byte 0x22000449
	.4byte 0xfcd0f001
	.4byte 0xe0272323
	.4byte 0xf7ff1c28
	.4byte 0x2800fdd3
	.4byte 0x2324d023
	.4byte 0x21e8e020
	.4byte 0x049b23be
	.4byte 0x04091c28
	.4byte 0xf0012200
	.4byte 0x2325fcbf
	.4byte 0x1c28e016
	.4byte 0xfdc2f7ff
	.4byte 0xd0122800
	.4byte 0xe00f2326
	.4byte 0x23ba21e8
	.4byte 0x1c28049b
	.4byte 0x22000409
	.4byte 0xfcaef001
	.4byte 0xe0052327
	.4byte 0xf7ff1c28
	.4byte 0x2800fdb1
	.4byte 0x2320d001
	.2byte 0x7033
.L_02002696:
	pop	{r5, r6, pc}
	.4byte 0x0200a3e4
	.2byte 0xc808
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #21
	bl 0x0200c058
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r6, r0, #0
.L_020026ae:
	ldr	r0, [pc, #160]
	bl 0x0200bf28
	movs	r0, #22
	ldr	r5, [pc, #156]
	bl 0x0200c058
	adds	r1, r0, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	str	r0, [r5, #0]
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	movs	r0, #22
	bl 0x0200c0f8
	ldr	r3, [r5, #0]
	movs	r5, #3
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r0, #22
	movs	r1, #3
	bl 0x0200c0f0
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #21
	ldr	r1, [pc, #100]
	bl 0x0200c060
	adds	r0, r6, #0
	movs	r1, #15
	bl 0x0200c0c0
	movs	r1, #3
	movs	r0, #21
	bl 0x0200c0f0
	movs	r0, #21
	bl 0x0200c0f8
	movs	r0, #21
	bl 0x0200c058
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bf90
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_02002740
	movs	r1, #232
	movs	r2, #166
	movs	r3, #0
	movs	r0, #21
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200c098
	adds	r3, r6, #0
	adds	r3, #98
	strb	r5, [r3, #0]
	ldr	r3, [pc, #28]
	str	r3, [r6, #108]
.L_02002740:
	bl 0x0200a1d4
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl 0x0200bf28
	pop	{r5, r6, pc}
	.4byte 0x0200a181
	.4byte 0x0200c80c
	.4byte 0x00019999
	.4byte 0x0200a3cd
	.2byte 0x9c5d
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r5, #0
	adds	r6, #98
	ldrb	r3, [r6, #0]
	cmp	r3, #55
	bls.n	.L_02002774
	b.n	.L_02002b80
.L_02002774:
	ldr	r2, [pc, #884]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200a85c
	.4byte 0x0200a870
	.4byte 0x0200a880
	.4byte 0x0200a894
	.4byte 0x0200a8a4
	.4byte 0x0200a8b8
	.4byte 0x0200a8da
	.4byte 0x0200a8ee
	.4byte 0x0200a8fe
	.4byte 0x0200a912
	.4byte 0x0200a922
	.4byte 0x0200a936
	.4byte 0x0200a956
	.4byte 0x0200a96a
	.4byte 0x0200a97a
	.4byte 0x0200a98e
	.4byte 0x0200a9b8
	.4byte 0x0200a9cc
	.4byte 0x0200a9dc
	.4byte 0x0200a9f0
	.4byte 0x0200aa00
	.4byte 0x0200aa14
	.4byte 0x0200aa36
	.4byte 0x0200aa4a
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200aa5a
	.4byte 0x0200aa72
	.4byte 0x0200aa82
	.4byte 0x0200aa9a
	.4byte 0x0200aaa8
	.4byte 0x0200aabc
	.4byte 0x0200aaca
	.4byte 0x0200aade
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200ab80
	.4byte 0x0200aaf4
	.4byte 0x0200ab0c
	.4byte 0x0200ab1a
	.4byte 0x0200ab2e
	.4byte 0x0200ab3c
	.4byte 0x0200ab50
	.4byte 0x0200ab5e
	.4byte 0x0200ab72
	.4byte 0x2384219c
	.4byte 0x1c28045b
	.4byte 0x22000449
	.4byte 0xfbbaf001
	.4byte 0xe1862301
	.4byte 0xf7ff1c28
	.4byte 0x2800fcbd
	.4byte 0xe181d100
	.4byte 0xe17e2302
	.4byte 0x23ac219c
	.4byte 0x1c28045b
	.4byte 0x22000449
	.4byte 0xfba8f001
	.4byte 0xe1742303
	.4byte 0xf7ff1c28
	.4byte 0x2800fcab
	.4byte 0xe16fd100
	.4byte 0xe16c2304
	.4byte 0x23ac2194
	.4byte 0x1c28045b
	.4byte 0x22000449
	.4byte 0xfb96f001
	.4byte 0xe1622305
	.4byte 0xf7ff1c28
	.4byte 0x2800fc99
	.4byte 0xe15dd100
	.4byte 0x00802080
	.4byte 0xf0013002
	.4byte 0x2800fb61
	.4byte 0x2320d001
	.4byte 0x2306e153
	.4byte 0x21a8e151
	.4byte 0x045b23ac
	.4byte 0x04091c28
	.4byte 0xf0012200
	.4byte 0x2307fb7b
	.4byte 0x1c28e147
	.4byte 0xfc7ef7ff
	.4byte 0xd1002800
	.4byte 0x2308e142
	.4byte 0x21a8e13f
	.4byte 0x045b2394
	.4byte 0x04091c28
	.4byte 0xf0012200
	.4byte 0x2309fb69
	.4byte 0x1c28e135
	.4byte 0xfc6cf7ff
	.4byte 0xd1002800
	.4byte 0x230ae130
	.4byte 0x21b8e12d
	.4byte 0x045b2394
	.4byte 0x04091c28
	.4byte 0xf0012200
	.4byte 0x230bfb57
	.4byte 0x1c28e123
	.4byte 0xfc5af7ff
	.4byte 0xd1002800
	.4byte 0x2081e11e
	.4byte 0x30ff0040
	.4byte 0xfb22f001
	.4byte 0xd0002800
	.4byte 0x230ce114
	.4byte 0x218ce113
	.4byte 0x045b2394
	.4byte 0x04491c28
	.4byte 0xf0012200
	.4byte 0x230dfb3d
	.4byte 0x1c28e109
	.4byte 0xfc40f7ff
	.4byte 0xd1002800
	.4byte 0x230ee104
	.4byte 0x218ce101
	.4byte 0x045b23a4
	.4byte 0x04491c28
	.4byte 0xf0012200
	.4byte 0x230ffb2b
	.4byte 0x1c28e0f7
	.4byte 0xfc2ef7ff
	.4byte 0xd1002800
	.4byte 0x2080e0f2
	.4byte 0x30020080
	.4byte 0xfaf6f001
	.4byte 0xd0052800
	.4byte 0x23004a51
	.4byte 0x23ca66eb
	.4byte 0xe0e56013
	.4byte 0xe0e22310
	.4byte 0x23b4218c
	.4byte 0x1c28045b
	.4byte 0x22000449
	.4byte 0xfb0cf001
	.4byte 0xe0d82311
	.4byte 0xf7ff1c28
	.4byte 0x2800fc0f
	.4byte 0xe0d3d100
	.4byte 0xe0d02312
	.4byte 0x23b421c8
	.4byte 0x1c28045b
	.4byte 0x22000409
	.4byte 0xfafaf001
	.4byte 0xe0c62313
	.4byte 0xf7ff1c28
	.4byte 0x2800fbfd
	.4byte 0xe0c1d100
	.4byte 0xe0be2314
	.4byte 0x239c21c8
	.4byte 0x1c28045b
	.4byte 0x22000409
	.4byte 0xfae8f001
	.4byte 0xe0b42315
	.4byte 0xf7ff1c28
	.4byte 0x2800fbeb
	.4byte 0xe0afd100
	.4byte 0x00402081
	.4byte 0xf00130ff
	.4byte 0x2800fab3
	.4byte 0x2322d001
	.4byte 0x2316e0a5
	.4byte 0x21c8e0a3
	.4byte 0x045b2384
	.4byte 0x04091c28
	.4byte 0xf0012200
	.4byte 0x2317facd
	.4byte 0x1c28e099
	.4byte 0xfbd0f7ff
	.4byte 0xd1002800
	.4byte 0x2300e094
	.4byte 0xf7ffe091
	.4byte 0x2194fbbb
	.4byte 0x045b239c
	.4byte 0x04491c28
	.4byte 0xf0012200
	.4byte 0x2321fab9
	.4byte 0x1c28e085
	.4byte 0xfbbcf7ff
	.4byte 0xd1002800
	.4byte 0x2322e080
	.4byte 0xf7ffe07d
	.4byte 0x21a4fba7
	.4byte 0x045b239c
	.4byte 0x04491c28
	.4byte 0xf0012200
	.4byte 0x2323faa5
	.4byte 0x1c28e071
	.4byte 0xfba8f7ff
	.4byte 0xd06d2800
	.4byte 0xe06a2324
	.4byte 0x23bc21a4
	.4byte 0x1c28045b
	.4byte 0x22000449
	.4byte 0xfa94f001
	.4byte 0xe0602325
	.4byte 0xf7ff1c28
	.4byte 0x2800fb97
	.4byte 0x2326d05c
	.4byte 0x2194e059
	.4byte 0x045b23bc
	.4byte 0x04491c28
	.4byte 0xf0012200
	.4byte 0x2327fa83
	.4byte 0x1c28e04f
	.4byte 0xfb86f7ff
	.4byte 0xd04b2800
	.4byte 0xe0482320
	.4byte 0x0200a77c
	.4byte 0x0200c808
	.4byte 0xfb6ef7ff
	.4byte 0x23ac21b8
	.4byte 0x1c28045b
	.4byte 0x22000409
	.4byte 0xfa6cf001
	.4byte 0xe0382331
	.4byte 0xf7ff1c28
	.4byte 0x2800fb6f
	.4byte 0x2332d034
	.4byte 0x21a8e031
	.4byte 0x045b23ac
	.4byte 0x04091c28
	.4byte 0xf0012200
	.4byte 0x2333fa5b
	.4byte 0x1c28e027
	.4byte 0xfb5ef7ff
	.4byte 0xd0232800
	.4byte 0xe0202334
	.4byte 0x239421a8
	.4byte 0x1c28045b
	.4byte 0x22000409
	.4byte 0xfa4af001
	.4byte 0xe0162335
	.4byte 0xf7ff1c28
	.4byte 0x2800fb4d
	.4byte 0x2336d012
	.4byte 0x21b8e00f
	.4byte 0x045b2394
	.4byte 0x04091c28
	.4byte 0xf0012200
	.4byte 0x2337fa39
	.4byte 0x1c28e005
	.4byte 0xfb3cf7ff
	.4byte 0xd0012800
	.2byte 0x2330
	.2byte 0x7033
.L_02002b80:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #21
	bl 0x0200c058
	adds	r6, r0, #0
	movs	r0, #22
	ldr	r5, [pc, #196]
	bl 0x0200c058
	adds	r1, r0, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	str	r0, [r5, #0]
	movs	r0, #22
	bl 0x0200c0f8
	movs	r0, #22
	movs	r1, #3
	bl 0x0200c0f0
	ldr	r3, [r5, #0]
	movs	r2, #204
	adds	r3, #85
	movs	r5, #3
	lsls	r2, r2, #8
	strb	r5, [r3, #0]
	adds	r2, #204
	movs	r0, #21
	ldr	r1, [pc, #140]
	bl 0x0200c060
	adds	r0, r6, #0
	movs	r1, #15
	bl 0x0200c0c0
	movs	r1, #3
	movs	r0, #21
	bl 0x0200c0f0
	movs	r0, #21
	bl 0x0200c0f8
	movs	r0, #21
	bl 0x0200c058
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #139
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_02002c14
	movs	r3, #128
	movs	r1, #232
	movs	r2, #168
	lsls	r3, r3, #7
	movs	r0, #21
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200c098
	b.n	.L_02002c46
.L_02002c14:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #68]
.L_02002c1a:
	bl 0x0200bf28
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bf90
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_02002c46
	movs	r1, #200
.L_02002c2e:
	movs	r2, #132
	movs	r3, #0
	movs	r0, #21
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200c098
	adds	r3, r6, #0
	adds	r3, #98
	strb	r5, [r3, #0]
	ldr	r3, [pc, #32]
	str	r3, [r6, #108]
.L_02002c46:
	bl 0x0200a1d4
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #24]
	bl 0x0200bf28
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200c80c
	.4byte 0x00019999
	.4byte 0x0200a181
	.4byte 0x0200a765
	.2byte 0x9c5d
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r2, #196
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	sub	sp, #8
	cmp	r3, #0
	beq.n	.L_02002c9a
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #73
	movs	r1, #76
	movs	r2, #78
	movs	r3, #11
	bl 0x0200bff0
.L_02002c9a:
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #136
	movs	r1, #1
	bl 0x0200c190
	ldr	r5, [pc, #48]
	movs	r1, #144
	adds	r0, r5, #0
	lsls	r1, r1, #3
	bl 0x0200bf28
	movs	r1, #1
	negs	r1, r1
	movs	r0, #21
	bl 0x0200c198
	bl 0x0200c1b0
	movs	r0, #1
	bl 0x0200c188
	bl 0x0200c1a0
	adds	r0, r5, #0
	bl 0x0200bf30
	bl 0x0200c1a8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xac6d
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200c0f8
	adds	r0, r5, #0
	bl 0x0200c058
	movs	r1, #15
	bl 0x0200c0c0
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200c0f0
	adds	r0, r5, #0
	bl 0x0200c058
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_02002d32
	movs	r0, #135
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bf90
	cmp	r0, #0
	bne.n	.L_02002d3c
	bl 0x0200963c
	b.n	.L_02002d3c
.L_02002d32:
	movs	r0, #135
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bf98
.L_02002d3c:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #11
	bl 0x0200ace0
	movs	r0, #12
	bl 0x0200ace0
	bl 0x0200ad10
	pop	{pc}
	push	{r5, lr}
	movs	r5, #0
.L_02002d58:
	adds	r0, r5, #0
	adds	r0, #9
	adds	r5, #1
	bl 0x0200ace0
	cmp	r5, #6
	bls.n	.L_02002d58
	bl 0x0200ad10
	pop	{r5, pc}
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	adds	r6, r0, #0
	mov	r8, r1
	mov	sl, r2
	mov	r9, r3
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	ldr	r5, [pc, #120]
	strb	r3, [r2, #0]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c060
	mov	r2, sl
	ldr	r0, [r5, #0]
	mov	r1, r8
	bl 0x0200c088
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200c0e8
	ldr	r0, [r5, #0]
	bl 0x0200c058
	movs	r1, #0
	bl 0x0200c008
	ldr	r0, [r5, #0]
	movs	r1, #13
	bl 0x0200c0a0
	ldr	r2, [r6, #12]
	ldr	r3, [pc, #60]
	ldr	r1, [r6, #8]
	adds	r2, r2, r3
	adds	r0, r6, #0
	ldr	r3, [r6, #16]
	bl 0x0200bfe0
	adds	r0, r6, #0
	bl 0x0200bfe8
	movs	r1, #10
	ldr	r0, [r5, #0]
	bl 0x0200c0a0
	movs	r0, #123
	bl 0x0200c1f8
	bl 0x0200c160
	bl 0x0200c168
	mov	r0, r9
	bl 0x0200c138
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff6
	.2byte 0xb500
.L_02002e16:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #141
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_02002e8e
	ldr	r3, [pc, #104]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c058
	ldr	r3, [r0, #8]
	asrs	r1, r3, #20
	ldr	r3, [r0, #16]
	asrs	r4, r3, #20
	cmp	r1, #8
	bne.n	.L_02002e60
	cmp	r4, #31
	bne.n	.L_02002e4e
	ldr	r3, [pc, #80]
	movs	r2, #128
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002e8e
.L_02002e4e:
	cmp	r4, #33
	bne.n	.L_02002e82
	ldr	r3, [pc, #64]
	movs	r2, #64
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02002e82
	b.n	.L_02002e8e
.L_02002e60:
	cmp	r4, #32
	bne.n	.L_02002e8e
	cmp	r1, #7
	bne.n	.L_02002e74
	ldr	r3, [pc, #40]
	movs	r2, #16
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002e8e
.L_02002e74:
	cmp	r1, #9
	bne.n	.L_02002e82
	ldr	r3, [pc, #24]
	ldr	r3, [r3, #0]
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_02002e8e
.L_02002e82:
	movs	r2, #129
	lsls	r2, r2, #2
	movs	r1, #136
	movs	r3, #5
	bl 0x0200ad6c
.L_02002e8e:
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c058
	movs	r1, #204
	movs	r2, #216
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r3, #2
	bl 0x0200ad6c
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #440]
	movs	r5, #0
	ldr	r0, [r1, #0]
	bl 0x0200c058
	adds	r6, r0, #0
	movs	r0, #8
	bl 0x0200c058
	mov	r8, r0
	bl 0x0200c040
	movs	r0, #0
	bl 0x0200c1b8
	movs	r0, #78
	bl 0x0200c1f8
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	bl 0x0200c120
	movs	r0, #232
	movs	r1, #1
	movs	r2, #184
	movs	r3, #0
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #16
	bl 0x0200c120
	bl 0x0200bfd8
	movs	r0, #1
	bl 0x0200bf20
	ldr	r2, [pc, #356]
	adds	r7, r6, #0
	ldr	r0, [r2, #0]
	bl 0x0200c058
	movs	r1, #0
	bl 0x0200c008
	movs	r3, #130
	movs	r1, #128
	lsls	r3, r3, #16
.L_02002f32:
	lsls	r1, r1, #8
	adds	r7, #85
	strb	r5, [r7, #0]
	movs	r0, #8
	str	r3, [r6, #12]
	str	r1, [r6, #72]
	str	r5, [r6, #68]
	mov	r9, r3
	mov	sl, r1
	bl 0x0200c058
	movs	r1, #0
	bl 0x0200c008
	movs	r3, #128
	movs	r1, #232
	movs	r2, #148
	lsls	r3, r3, #7
	movs	r0, #8
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200c098
	movs	r2, #85
	add	r2, r8
	strb	r5, [r2, #0]
	mov	r1, r8
	mov	r3, r9
	mov	fp, r2
	mov	r2, sl
	movs	r0, #1
	str	r3, [r1, #12]
	str	r2, [r1, #72]
	str	r5, [r1, #68]
	bl 0x0200bf20
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #130
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	adds	r2, #255
	str	r2, [r3, #0]
	bl 0x0200c158
	movs	r5, #3
	bl 0x0200c168
	movs	r0, #204
	bl 0x0200c1f8
	ldr	r2, [pc, #228]
	strb	r5, [r7, #0]
	ldr	r0, [r2, #0]
	bl 0x0200c058
	movs	r1, #1
	bl 0x0200c008
	movs	r0, #24
	bl 0x0200c038
	ldr	r2, [r6, #16]
	ldr	r1, [r6, #12]
	ldr	r0, [r6, #8]
	bl 0x02008b84
	movs	r0, #188
	bl 0x0200c1f8
	ldr	r3, [pc, #188]
	movs	r1, #2
	ldr	r0, [r3, #0]
	adds	r1, #255
	bl 0x0200c108
	ldr	r1, [pc, #176]
	ldr	r0, [r1, #0]
	movs	r1, #49
	bl 0x0200c0a8
	movs	r0, #204
	bl 0x0200c1f8
	mov	r2, fp
	strb	r5, [r2, #0]
	movs	r0, #8
	bl 0x0200c058
	movs	r1, #1
	bl 0x0200c008
	movs	r0, #24
	bl 0x0200c038
	mov	r3, r8
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	ldr	r2, [r3, #16]
	bl 0x02008b84
	movs	r0, #188
	bl 0x0200c1f8
	ldr	r1, [pc, #120]
	movs	r2, #0
	ldr	r0, [r1, #0]
	movs	r1, #192
	lsls	r1, r1, #8
	bl 0x0200c0e0
	movs	r0, #40
	bl 0x0200c038
	movs	r0, #147
	bl 0x0200c1f8
	movs	r1, #3
	movs	r0, #8
	bl 0x0200c0a0
	movs	r0, #20
	bl 0x0200c038
	movs	r3, #128
	mov	r2, r8
	lsls	r3, r3, #12
	str	r3, [r2, #40]
	movs	r3, #230
	lsls	r3, r3, #8
	adds	r3, #102
	str	r3, [r2, #72]
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #64]
	adds	r2, #204
	bl 0x0200c060
	movs	r0, #8
	movs	r1, #232
	movs	r2, #184
	bl 0x0200c080
	ldr	r3, [pc, #48]
.L_0200305a:
	movs	r1, #166
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r2, r3, r1
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r0, [pc, #40]
	movs	r1, #2
	bl 0x0200c150
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c140
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000454
	.4byte 0x00019999
	.4byte 0x02000240
	.2byte 0x004c
	.2byte 0x0000
	.global Func_02003094
	.thumb_func
Func_02003094:
	push	{lr}
	bl 0x0200a1d4
	ldr	r2, [pc, #152]
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [pc, #148]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #140]
	cmp	r2, r3
	bne.n	.L_020030b8
	bl 0x0200b16c
	b.n	.L_0200312e
.L_020030b8:
	ldr	r3, [pc, #132]
	cmp	r2, r3
	bne.n	.L_020030c4
	bl 0x0200b1b4
	b.n	.L_0200312e
.L_020030c4:
	ldr	r3, [pc, #124]
	cmp	r2, r3
	bne.n	.L_020030d0
	bl 0x0200b2f4
	b.n	.L_0200312e
.L_020030d0:
	ldr	r3, [pc, #116]
	cmp	r2, r3
	bne.n	.L_020030dc
	bl 0x0200b364
	b.n	.L_0200312e
.L_020030dc:
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L_020030e8
	bl 0x0200b404
	b.n	.L_0200312e
.L_020030e8:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_020030f4
	bl 0x0200b5b8
	b.n	.L_0200312e
.L_020030f4:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02003100
	bl 0x0200b5d8
	b.n	.L_0200312e
.L_02003100:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_0200310c
	bl 0x0200b6d0
	b.n	.L_0200312e
.L_0200310c:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02003118
	bl 0x0200b71c
	b.n	.L_0200312e
.L_02003118:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02003124
	bl 0x0200b7d4
	b.n	.L_0200312e
.L_02003124:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_0200312e
	bl 0x0200b7e8
.L_0200312e:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200c80c
	.4byte 0x02000240
	.4byte 0x00000043
	.4byte 0x00000044
	.4byte 0x00000045
	.4byte 0x00000046
	.4byte 0x00000047
	.4byte 0x00000048
	.4byte 0x0000004a
	.4byte 0x0000004b
	.4byte 0x0000004c
	.4byte 0x0000004d
	.2byte 0x004e
	.2byte 0x0000
	.global Func_02003168
	.thumb_func
Func_02003168:
	movs	r0, #0
	bx	lr
	push	{lr}
	ldr	r0, [pc, #60]
	bl 0x0200b820
	movs	r0, #0
	bl 0x0200c180
	ldr	r3, [pc, #52]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	bne.n	.L_020031a8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #41
	bl 0x0200bf98
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #42
	bl 0x0200bfa0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #43
	bl 0x0200bfa0
.L_020031a8:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200c768
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #179
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_0200320c
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	b.n	.L_0200328c
.L_0200320c:
	movs	r0, #143
	lsls	r0, r0, #4
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_0200328c
	movs	r6, #176
	lsls	r6, r6, #8
	movs	r1, #130
	movs	r2, #242
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	adds	r3, r6, #0
	bl 0x0200c098
	movs	r3, #128
	lsls	r3, r3, #8
	movs	r0, #9
	ldr	r1, [pc, #120]
	ldr	r2, [pc, #120]
	bl 0x0200c098
	movs	r3, #192
	movs	r2, #232
	lsls	r3, r3, #6
	movs	r0, #10
	ldr	r1, [pc, #112]
	lsls	r2, r2, #16
	bl 0x0200c098
	movs	r3, #160
	movs	r2, #248
	lsls	r3, r3, #7
	movs	r0, #11
	ldr	r1, [pc, #100]
	lsls	r2, r2, #16
	movs	r5, #208
	bl 0x0200c098
	lsls	r5, r5, #8
	movs	r1, #231
	movs	r2, #170
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	adds	r3, r5, #0
	bl 0x0200c098
	movs	r1, #241
	movs	r2, #170
	movs	r0, #13
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	adds	r3, r5, #0
	bl 0x0200c098
	movs	r2, #160
	movs	r0, #14
	ldr	r1, [pc, #56]
	lsls	r2, r2, #17
	adds	r3, r6, #0
	bl 0x0200c098
.L_0200328c:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #141
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_0200329e
	bl 0x02009594
.L_0200329e:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #28]
	bl 0x0200bf28
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02150000
	.4byte 0x010f0000
	.4byte 0x01b30000
	.4byte 0x01d30000
	.4byte 0x01e90000
	.2byte 0xa011
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #40]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c058
	ldr	r3, [r0, #80]
	movs	r0, #13
	ldrb	r5, [r3, #9]
	lsls	r5, r5, #28
	lsrs	r5, r5, #30
	adds	r1, r5, #0
	bl 0x0200c0f0
	movs	r0, #14
	adds	r1, r5, #0
	bl 0x0200c0f0
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #52]
	bl 0x0200b820
	movs	r0, #134
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_02003314
	bl 0x020086ac
	movs	r0, #10
	bl 0x0200bf20
.L_02003314:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl 0x0200bf28
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200bf28
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200c78e
	.4byte 0x0200a011
	.2byte 0xb2c5
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200c058
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #3
	adds	r0, r5, #0
	bl 0x0200c0f0
	adds	r0, r5, #0
	bl 0x0200c0f8
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200c0a0
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #0
	bl 0x0200c180
	ldr	r3, [pc, #136]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #3
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_020033a0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #137
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_0200339c
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
	b.n	.L_020033a0
.L_0200339c:
	bl 0x0200a318
.L_020033a0:
	ldr	r0, [pc, #88]
	bl 0x0200b820
	movs	r0, #134
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_020033be
	bl 0x020086ac
	movs	r0, #10
	bl 0x0200bf20
.L_020033be:
	movs	r0, #12
	bl 0x0200b338
	movs	r0, #13
	bl 0x0200b338
	movs	r0, #14
	bl 0x0200b338
	movs	r0, #15
	bl 0x0200b338
	ldr	r3, [pc, #32]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #1
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_020033f6
	movs	r1, #144
	ldr	r0, [pc, #16]
	lsls	r1, r1, #3
	bl 0x0200bf28
.L_020033f6:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0200c7d2
	.2byte 0xa0a9
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	sub	sp, #8
	bl 0x0200c180
	ldr	r0, [pc, #416]
	bl 0x0200b820
	movs	r0, #11
	bl 0x0200b338
	movs	r0, #12
	bl 0x0200b338
	movs	r0, #13
	bl 0x0200b338
	movs	r0, #14
	bl 0x0200b338
	movs	r0, #15
	bl 0x0200b338
	movs	r0, #16
	bl 0x0200b338
	movs	r0, #17
	bl 0x0200b338
	movs	r0, #18
	bl 0x0200b338
	movs	r0, #19
	bl 0x0200b338
	movs	r0, #20
	bl 0x0200b338
	ldr	r3, [pc, #352]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #1
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_0200348c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #137
	bl 0x0200bf90
	cmp	r0, #0
	bne.n	.L_02003476
	b.n	.L_020035aa
.L_02003476:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #138
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_02003486
	b.n	.L_020035aa
.L_02003486:
	bl 0x0200a6a0
	b.n	.L_020035aa
.L_0200348c:
	movs	r1, #200
	movs	r2, #248
	movs	r0, #11
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200c090
	movs	r1, #168
	movs	r2, #140
	movs	r0, #12
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200c090
	movs	r1, #152
	movs	r2, #172
	movs	r0, #13
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200c090
	movs	r1, #184
	movs	r2, #180
	movs	r0, #14
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200c090
	movs	r1, #164
	movs	r2, #132
	movs	r0, #15
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200c090
	movs	r1, #148
	movs	r2, #148
	movs	r0, #16
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200c090
	movs	r1, #172
	movs	r2, #156
	movs	r0, #17
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200c090
	movs	r1, #156
	movs	r2, #180
	movs	r0, #18
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200c090
	movs	r1, #140
	movs	r2, #188
	movs	r0, #19
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200c090
	movs	r1, #164
	movs	r2, #196
	movs	r0, #20
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200c090
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #140
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_02003598
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #237
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_0200355c
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #68
	movs	r1, #77
	movs	r2, #13
	movs	r3, #9
	bl 0x0200bff0
	movs	r3, #13
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #68
	movs	r1, #77
	movs	r2, #3
	movs	r3, #3
	bl 0x0200c000
	b.n	.L_02003582
.L_0200355c:
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #74
	movs	r1, #71
	movs	r2, #77
	movs	r3, #9
	bl 0x0200bff0
	movs	r3, #13
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #74
	movs	r1, #71
	movs	r2, #3
	movs	r3, #3
	bl 0x0200c000
.L_02003582:
	movs	r3, #13
	movs	r2, #73
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #63
	movs	r1, #77
	movs	r2, #3
	movs	r3, #3
	bl 0x0200c000
	b.n	.L_020035aa
.L_02003598:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #138
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_020035aa
	bl 0x0200ab84
.L_020035aa:
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200c7ec
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	bl 0x0200c180
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200bf90
	cmp	r0, #0
	bne.n	.L_020035d6
	movs	r0, #64
	movs	r1, #0
	bl 0x0200c170
.L_020035d6:
	pop	{pc}
	push	{r5, lr}
	movs	r0, #64
	bl 0x0200c058
	adds	r5, r0, #0
	movs	r0, #137
	bl 0x0200c1c8
	bl 0x0200c1d0
	movs	r1, #131
	movs	r0, #0
	lsls	r1, r1, #2
	movs	r2, #8
	movs	r3, #9
	bl 0x0200c1d8
	cmp	r5, #0
	beq.n	.L_02003608
	movs	r0, #64
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
.L_02003608:
	movs	r0, #10
	bl 0x0200c058
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r5, r0, #0
	adds	r3, #51
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r3, #8
	strh	r3, [r5, #32]
	bl 0x0200ad40
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #148]
	bl 0x0200bf28
	ldr	r5, [pc, #144]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_0200365c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #41
	bl 0x0200bfa0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #42
	bl 0x0200bf98
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #43
	bl 0x0200bfa0
	b.n	.L_020036ba
.L_0200365c:
	cmp	r3, #4
	bne.n	.L_020036ba
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #41
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_02003684
	ldr	r2, [pc, #84]
	movs	r1, #152
	lsls	r1, r1, #2
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #98
	adds	r2, r5, r3
	movs	r3, #1
	b.n	.L_020036b8
.L_02003684:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #42
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_02003696
	ldr	r2, [pc, #52]
	b.n	.L_020036a6
.L_02003696:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #43
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_020036ba
	ldr	r2, [pc, #36]
.L_020036a6:
	movs	r1, #152
	lsls	r1, r1, #2
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #98
	adds	r2, r5, r3
	movs	r3, #2
.L_020036b8:
	strh	r3, [r2, #0]
.L_020036ba:
	pop	{r5, pc}
	.4byte 0x02009e59
	.4byte 0x02000240
	.4byte 0x00000043
	.4byte 0x0000004a
	.2byte 0x004b
	.2byte 0x0000
	push	{lr}
	movs	r0, #64
	bl 0x0200c058
	cmp	r0, #0
	beq.n	.L_020036e6
	movs	r0, #64
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c090
.L_020036e6:
	ldr	r3, [pc, #48]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_02003714
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #41
	bl 0x0200bfa0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #42
	bl 0x0200bfa0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #43
	bl 0x0200bf98
.L_02003714:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #162
	adds	r2, #93
	str	r2, [r3, #0]
	lsls	r0, r0, #1
	sub	sp, #8
	bl 0x0200bf98
	ldr	r3, [pc, #148]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	bne.n	.L_0200375c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #140
	bl 0x0200bf90
	cmp	r0, #0
	bne.n	.L_0200375c
	bl 0x0200aebc
	b.n	.L_0200377e
.L_0200375c:
	ldr	r3, [pc, #112]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_0200377e
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #140
	bl 0x0200bf90
	cmp	r0, #0
	bne.n	.L_0200377e
	bl 0x02009bec
.L_0200377e:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #237
	bl 0x0200bf90
.L_02003788:
	cmp	r0, #0
	beq.n	.L_020037ac
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #39
	movs	r2, #13
	movs	r3, #11
	bl 0x0200bff0
	movs	r1, #232
	movs	r2, #200
	movs	r0, #9
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200c090
.L_020037ac:
	movs	r0, #0
	movs	r1, #240
	bl 0x0200bda0
	movs	r3, #64
	str	r3, [sp, #0]
	movs	r1, #9
	movs	r2, #0
	movs	r3, #0
	movs	r0, #0
	bl 0x0200be3c
	movs	r0, #10
	bl 0x0200bf20
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x049b23c0
	.4byte 0x22d66edb
	.4byte 0x189b0052
	.4byte 0x601a3258
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	movs	r0, #137
	bl 0x0200c1c8
	ldr	r0, [pc, #20]
	bl 0x0200b820
	bl 0x0200ad54
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl 0x0200bf28
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200c800
	.2byte 0x9e59
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	sub	sp, #4
	cmp	r3, r2
	beq.n	.L_02003884
	adds	r7, r0, #0
.L_02003836:
	ldrh	r3, [r7, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200c058
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #89
	movs	r2, #2
	ldrsh	r6, [r7, r2]
	ldrb	r2, [r1, #0]
	movs	r3, #4
	ldrsh	r4, [r7, r3]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #0
	str	r4, [sp, #0]
	bl 0x0200c008
	ldr	r4, [sp, #0]
	lsls	r6, r6, #16
	lsls	r4, r4, #16
	lsrs	r4, r4, #16
	lsrs	r6, r6, #16
	adds	r5, #34
	ldrb	r3, [r5, #0]
	adds	r2, r4, #0
	mov	r0, r8
	adds	r1, r6, #0
	adds	r7, #6
	bl 0x0200b910
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02003836
.L_02003884:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r5, r0, #0
	mov	r9, r1
	mov	sl, r2
	movs	r1, #255
	ldr	r2, [r3, #0]
	b.n	.L_020038f8
.L_020038a8:
	ldrh	r3, [r5, #0]
	movs	r1, #26
	ldrsh	r7, [r2, r1]
	cmp	r7, r3
	bne.n	.L_020038f4
	adds	r0, r7, #0
	bl 0x0200c058
	adds	r5, #2
	ldrh	r2, [r5, #0]
	mov	r3, sl
	adds	r6, r0, #0
	mov	r8, r2
	ldrh	r5, [r5, #2]
	cmp	r3, #7
	bgt.n	.L_020038d0
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #64]
	adds	r3, r3, r1
	str	r3, [r6, #28]
.L_020038d0:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_02003902
	adds	r0, r5, #0
	bl 0x0200bf98
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	adds	r0, r7, #0
	mov	r1, r8
	adds	r2, r5, #0
	bl 0x0200b910
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #28]
	b.n	.L_02003902
.L_020038f4:
	adds	r5, #6
	movs	r1, #255
.L_020038f8:
	ldrh	r3, [r5, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_020038a8
.L_02003902:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	adds	r5, r3, #0
	mov	r8, r2
	adds	r6, r1, #0
	bl 0x0200c058
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r5, #3
	subs	r3, r3, r5
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r5, [r2, r3]
	adds	r7, r0, #0
	bl 0x0200c1e0
	lsls	r0, r0, #2
	adds	r5, r5, r0
	mov	r0, r8
	bl 0x0200bf90
	cmp	r0, #0
	beq.n	.L_02003984
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #0
	strb	r3, [r5, #2]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	cmp	r6, #1
	beq.n	.L_02003970
	cmp	r6, #1
	bcc.n	.L_02003966
	cmp	r6, #2
	beq.n	.L_0200397a
	b.n	.L_020039b2
.L_02003966:
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200bfa8
	b.n	.L_020039b2
.L_02003970:
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x0200bfa8
	b.n	.L_020039b2
.L_0200397a:
	adds	r0, r7, #0
	movs	r1, #6
	bl 0x0200bfa8
	b.n	.L_020039b2
.L_02003984:
	movs	r3, #255
	strb	r3, [r5, #2]
	cmp	r6, #1
	beq.n	.L_020039a0
	cmp	r6, #1
	bcc.n	.L_02003996
	cmp	r6, #2
	beq.n	.L_020039aa
	b.n	.L_020039b2
.L_02003996:
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200bfa8
	b.n	.L_020039b2
.L_020039a0:
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x0200bfa8
	b.n	.L_020039b2
.L_020039aa:
	adds	r0, r7, #0
	movs	r1, #5
	bl 0x0200bfa8
.L_020039b2:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_020039b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #420]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	mov	r9, r0
	movs	r0, #158
	mov	r4, r9
	lsls	r0, r0, #1
	movs	r3, #2
	ldrsh	r2, [r4, r3]
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	sub	sp, #4
	str	r3, [sp, #0]
	movs	r4, #156
	lsls	r4, r4, #1
	adds	r3, r1, r4
	ldr	r1, [pc, #388]
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	mov	r5, r9
	movs	r0, #0
	adds	r1, r1, r2
	adds	r5, #4
	mov	fp, r3
	mov	sl, r0
	mov	r8, r1
.L_020039fc:
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	cmp	r0, #0
	bne.n	.L_02003a06
	b.n	.L_02003b4c
.L_02003a06:
	bl 0x0200c058
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	cmp	r3, #0
	bne.n	.L_02003a1a
	ldr	r3, [r7, #16]
	cmp	r3, #0
	bne.n	.L_02003a1a
	b.n	.L_02003b4c
.L_02003a1a:
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #8
	ldrsh	r3, [r5, r4]
	cmp	r2, r3
	bne.n	.L_02003a30
	movs	r0, #206
	bl 0x0200c1f8
	movs	r3, #4
	strh	r3, [r5, #18]
.L_02003a30:
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	movs	r1, #10
	ldrsh	r3, [r5, r1]
	cmp	r2, r3
	bne.n	.L_02003a4c
	movs	r0, #140
	adds	r0, #255
	bl 0x0200c1f8
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #248
	strh	r3, [r5, #18]
.L_02003a4c:
	movs	r4, #18
	ldrsh	r3, [r5, r4]
	ldrh	r2, [r5, #18]
	cmp	r3, #0
	beq.n	.L_02003a86
	ldrh	r3, [r5, #4]
	movs	r0, #0
	adds	r3, r3, r2
	movs	r4, #12
	ldrsh	r2, [r5, r4]
	strh	r3, [r5, #4]
.L_02003a62:
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	ldrh	r1, [r5, #12]
	cmp	r3, r2
	blt.n	.L_02003a72
	strh	r1, [r5, #4]
	strh	r0, [r5, #18]
	strh	r0, [r5, #22]
.L_02003a72:
	movs	r1, #4
	ldrsh	r2, [r5, r1]
	movs	r4, #14
	ldrsh	r3, [r5, r4]
	ldrh	r1, [r5, #14]
	cmp	r2, r3
	bgt.n	.L_02003a86
	strh	r1, [r5, #4]
	strh	r0, [r5, #18]
	strh	r0, [r5, #22]
.L_02003a86:
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200bf90
	cmp	r0, #0
	bne.n	.L_02003aa6
	ldrh	r3, [r5, #2]
	movs	r1, #6
	ldrsh	r2, [r5, r1]
	adds	r3, #1
	strh	r3, [r5, #2]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	blt.n	.L_02003aa6
	strh	r0, [r5, #2]
.L_02003aa6:
	ldr	r3, [r7, #16]
	ldr	r2, [r7, #8]
	asrs	r3, r3, #20
	asrs	r2, r2, #20
	lsls	r3, r3, #7
	adds	r1, r2, r3
	mov	r2, r9
	ldrb	r3, [r2, #2]
	ldr	r4, [sp, #0]
	add	r3, sl
	strb	r3, [r4, r1]
	movs	r0, #14
	ldrsh	r2, [r7, r0]
	movs	r4, #4
	ldrsh	r3, [r5, r4]
	adds	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_02003acc
	adds	r2, #7
.L_02003acc:
	asrs	r3, r2, #3
	lsls	r3, r3, #8
	mov	r0, r8
	str	r3, [r0, #0]
	mov	r2, fp
	lsls	r3, r1, #2
	adds	r1, r2, r3
	movs	r4, #18
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	beq.n	.L_02003ae6
	movs	r3, #0
	b.n	.L_02003aec
.L_02003ae6:
	ldrb	r2, [r1, #3]
	movs	r3, #128
	orrs	r3, r2
.L_02003aec:
	strb	r3, [r1, #3]
	movs	r0, #4
	ldrsh	r3, [r5, r0]
	cmp	r3, #0
	beq.n	.L_02003afc
	ldrb	r2, [r1, #3]
	movs	r3, #16
	orrs	r3, r2
.L_02003afc:
	strb	r3, [r1, #3]
	ldr	r6, [r5, #24]
	cmp	r6, #0
	beq.n	.L_02003b4c
	movs	r1, #4
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	bne.n	.L_02003b14
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #100]
	adds	r3, r3, r2
	b.n	.L_02003b42
.L_02003b14:
	ldrh	r0, [r5, #22]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r0, r0, r3
	strh	r0, [r5, #22]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	bl 0x0200bf40
	adds	r1, r0, #0
	movs	r0, #128
	ldr	r3, [pc, #76]
	lsls	r0, r0, #10
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2104
	ldrsh	r2, [r5, r1]
	ldr	r3, [r7, #12]
	ldr	r4, [pc, #64]
	lsls	r2, r2, #16
	adds	r0, r0, r4
	adds	r3, r3, r2
.L_02003b40:
	adds	r3, r3, r0
.L_02003b42:
	str	r3, [r6, #12]
	ldr	r3, [r7, #8]
	str	r3, [r6, #8]
	ldr	r3, [r7, #16]
	str	r3, [r6, #16]
.L_02003b4c:
	movs	r3, #1
	add	sl, r3
	movs	r2, #4
	mov	r4, sl
	add	r8, r2
	adds	r5, #28
	cmp	r4, #15
	bgt.n	.L_02003b5e
	b.n	.L_020039fc
.L_02003b5e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200254c
	.4byte 0x0202c000
	.4byte 0xfff00000
	.4byte 0x0300021c
	.2byte 0x0000
	.2byte 0xfff2
	.2byte 0xb5e0
	mov	r7, fp
.L_02003b84:
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #500]
	sub	sp, #36
	adds	r0, r2, #4
	str	r0, [sp, #32]
	ldr	r1, [pc, #496]
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	lsls	r3, r3, #2
	adds	r3, r3, r1
	ldrh	r3, [r3, #2]
	movs	r1, #226
	lsrs	r3, r3, #5
	str	r3, [sp, #24]
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldr	r2, [r2, #0]
	movs	r3, #192
	mov	r9, r2
	movs	r2, #0
	str	r2, [sp, #16]
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r3, [r2, #0]
	str	r3, [sp, #12]
	ldr	r0, [sp, #12]
	ldr	r3, [pc, #452]
	ands	r0, r3
	str	r0, [sp, #12]
	ldr	r2, [r2, #4]
	ands	r2, r3
	str	r2, [sp, #8]
	ldr	r3, [r1, #0]
	movs	r1, #15
	ldr	r3, [r3, #4]
	str	r1, [sp, #28]
	str	r3, [sp, #4]
.L_02003bda:
	ldr	r3, [sp, #32]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	bne.n	.L_02003be6
	b.n	.L_02003d64
.L_02003be6:
	movs	r1, #4
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02003bf0
	b.n	.L_02003d64
.L_02003bf0:
	bl 0x0200c058
	ldr	r3, [pc, #408]
	movs	r1, #12
	mov	sl, r0
	ldr	r0, [r3, #0]
	bl 0x0200bf18
	movs	r1, #3
	bl 0x0200bf10
	lsls	r0, r0, #3
	adds	r0, #32
	str	r0, [sp, #20]
	ldr	r1, [sp, #32]
	movs	r2, #0
	movs	r0, #4
	ldrsh	r3, [r1, r0]
	mov	fp, r2
	cmp	fp, r3
	bge.n	.L_02003cb4
.L_02003c1a:
	ldr	r2, [sp, #16]
	cmp	r2, #79
	bgt.n	.L_02003ca6
	mov	r0, sl
	ldr	r3, [r0, #8]
	ldr	r1, [sp, #12]
	subs	r7, r3, r1
	mov	r3, fp
	lsls	r2, r3, #16
	ldr	r3, [r0, #12]
	ldr	r0, [sp, #4]
	adds	r3, r3, r2
	mov	r1, sl
	subs	r0, r3, r0
	ldr	r2, [sp, #8]
	ldr	r3, [r1, #16]
	mov	r8, r0
	ldr	r0, [sp, #4]
	subs	r3, r3, r2
	subs	r5, r3, r0
	mov	r1, r8
	subs	r6, r5, r1
	asrs	r3, r6, #16
	adds	r6, r3, #0
	adds	r3, r1, r5
	asrs	r3, r3, #16
	asrs	r2, r7, #16
	adds	r1, r3, #0
	movs	r3, #167
	adds	r7, r2, #0
	lsls	r3, r3, #1
	adds	r2, #7
	subs	r7, #8
	subs	r6, #16
	adds	r1, #58
	cmp	r2, r3
	bhi.n	.L_02003ca6
	movs	r0, #16
	negs	r0, r0
	cmp	r6, r0
	ble.n	.L_02003ca6
	cmp	r6, #239
	bgt.n	.L_02003ca6
	adds	r3, #177
	ands	r7, r3
	movs	r3, #255
	mov	r4, r9
	ands	r6, r3
	movs	r3, #0
	stmia	r4!, {r3}
	lsls	r3, r7, #16
	orrs	r6, r3
	ldr	r3, [pc, #272]
	orrs	r6, r3
	stmia	r4!, {r6}
	ldr	r2, [sp, #24]
	ldr	r0, [sp, #20]
	adds	r3, r2, r0
	movs	r2, #128
	lsls	r2, r2, #4
.L_02003c92:
	orrs	r3, r2
	mov	r0, r9
	str	r3, [r4, #0]
	bl 0x0200bf88
	ldr	r2, [sp, #16]
.L_02003c9e:
	movs	r1, #12
	adds	r2, #1
	str	r2, [sp, #16]
	add	r9, r1
.L_02003ca6:
	ldr	r1, [sp, #32]
	movs	r3, #16
.L_02003caa:
	add	fp, r3
	movs	r0, #4
	ldrsh	r3, [r1, r0]
	cmp	fp, r3
	blt.n	.L_02003c1a
.L_02003cb4:
	ldr	r2, [sp, #16]
.L_02003cb6:
	cmp	r2, #79
	bgt.n	.L_02003d64
	mov	r0, sl
	ldr	r3, [r0, #8]
	ldr	r1, [sp, #12]
	ldr	r0, [sp, #32]
	subs	r7, r3, r1
	movs	r3, #4
	ldrsh	r2, [r0, r3]
	mov	r1, sl
	ldr	r3, [r1, #12]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	ldr	r2, [sp, #4]
	ldr	r0, [sp, #8]
	subs	r2, r3, r2
	ldr	r3, [r1, #16]
	ldr	r1, [sp, #4]
	subs	r3, r3, r0
	subs	r5, r3, r1
	ldr	r3, [sp, #32]
	subs	r6, r5, r2
	mov	r8, r2
	movs	r2, #22
	ldrsh	r0, [r3, r2]
	bl 0x0200bf40
	ldr	r3, [pc, #168]
	adds	r1, r0, #0
	ldr	r0, [pc, #168]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4929
	adds	r0, r6, r0
	mov	r2, r8
	asrs	r7, r7, #16
	adds	r0, r0, r1
	adds	r3, r2, r5
	mov	sl, r7
	asrs	r0, r0, #16
	asrs	r3, r3, #16
	adds	r1, r3, #0
	subs	r6, r0, #4
	mov	r3, sl
	movs	r0, #167
	adds	r3, #7
	lsls	r0, r0, #1
	subs	r7, #8
	adds	r1, #58
	cmp	r3, r0
	bhi.n	.L_02003d64
	movs	r2, #16
	negs	r2, r2
	cmp	r6, r2
	ble.n	.L_02003d64
	cmp	r6, #239
	bgt.n	.L_02003d64
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r7, r3
	movs	r3, #255
	mov	r4, r9
	ands	r6, r3
	movs	r3, #0
	stmia	r4!, {r3}
	lsls	r3, r7, #16
	orrs	r6, r3
	ldr	r3, [pc, #84]
	orrs	r6, r3
	stmia	r4!, {r6}
	ldr	r0, [sp, #24]
	ldr	r2, [sp, #20]
	adds	r3, r0, r2
	movs	r2, #128
	lsls	r2, r2, #4
	subs	r3, #32
	orrs	r3, r2
	mov	r0, r9
	str	r3, [r4, #0]
	bl 0x0200bf88
	ldr	r0, [sp, #16]
	movs	r3, #12
	adds	r0, #1
	str	r0, [sp, #16]
	add	r9, r3
.L_02003d64:
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #32]
	subs	r1, #1
	adds	r2, #28
	str	r1, [sp, #28]
	str	r2, [sp, #32]
	cmp	r1, #0
	blt.n	.L_02003d76
	b.n	.L_02003bda
.L_02003d76:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200254c
	.4byte 0x020036e0
	.4byte 0xffff0000
	.4byte 0x0300122c
	.4byte 0x40002000
	.4byte 0x0300021c
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb5e0
	movs	r0, #10
	adds	r0, #255
	adds	r7, r1, #0
	ldr	r6, [pc, #124]
	bl 0x0200bf90
	cmp	r0, #0
	bne.n	0x0200bdbe
	movs	r1, #228
	ldr	r3, [pc, #116]
	adds	r0, r6, #0
	lsls	r1, r1, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2080
	lsls	r0, r0, #4
	bl 0x0200bf60
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #100]
	bl 0x0200bf70
	bl 0x0200bf80
	movs	r1, #128
	strh	r0, [r6, #0]
	lsls	r0, r0, #16
	adds	r2, r5, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200bf78
	adds	r0, r5, #0
	bl 0x0200bf68
	movs	r0, #240
	lsls	r0, r0, #2
	bl 0x0200bf60
	movs	r2, #226
	lsls	r2, r2, #1
	movs	r1, #128
	adds	r3, r6, r2
	lsls	r1, r1, #3
	str	r0, [r3, #0]
	adds	r1, #141
	strh	r7, [r6, #2]
.L_02003e02:
	ldr	r0, [pc, #48]
	bl 0x0200bf28
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200bf28
	bl 0x0200c1f0
	bl 0x0200c058
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.4byte 0x03000258
	.4byte 0x0200c314
	.4byte 0x0200b9b9
	.2byte 0xbb81
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r9, r2
	mov	sl, r3
	ldr	r2, [pc, #132]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	mov	r8, r1
	lsls	r3, r3, #2
	adds	r3, r3, r2
	mov	r0, r8
	ldr	r7, [sp, #28]
	adds	r5, r3, #4
	bl 0x0200c058
	adds	r6, r0, #0
	mov	r0, r8
	bl 0x0200c058
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200c008
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bf90
	cmp	r0, #0
	bne.n	.L_02003ed4
	cmp	r7, sl
	bge.n	.L_02003e86
	mov	ip, sl
	mov	sl, r7
	mov	r7, ip
.L_02003e86:
	mov	r3, r8
	strh	r3, [r5, #0]
	mov	r3, r9
	strh	r3, [r5, #2]
	movs	r3, #180
	lsls	r3, r3, #1
	strh	r3, [r5, #6]
	movs	r3, #60
	strh	r3, [r5, #8]
	movs	r3, #240
	strh	r3, [r5, #10]
	mov	r3, sl
	ldr	r2, [pc, #44]
	strh	r3, [r5, #14]
	movs	r3, #1
	strh	r3, [r5, #16]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r2, r6, #0
	adds	r2, #89
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r1, r6, #0
	adds	r1, #35
.L_02003eb8:
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strh	r0, [r5, #4]
	strh	r7, [r5, #12]
	strh	r0, [r5, #18]
	strh	r0, [r5, #22]
	strh	r0, [r5, #20]
	strb	r3, [r1, #0]
	b.n	.L_02003ed4
	.4byte 0x00000000
	.2byte 0x254c
	.2byte 0x0200
.L_02003ed4:
	movs	r0, #128
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	lsls	r0, r0, #8
	bl 0x0200bfc0
	adds	r1, r0, #0
	adds	r2, r1, #0
	movs	r3, #0
	adds	r2, #89
	strb	r3, [r2, #0]
	subs	r2, #4
	strb	r3, [r2, #0]
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r0, #0]
	str	r1, [r5, #24]
	adds	r0, r5, #0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.section .rodata,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x009800e0
	.4byte 0x009800e8
	.4byte 0x009800f0
	.4byte 0x00a000d8
	.4byte 0x00a000e0
	.4byte 0x00a000e8
	.4byte 0x00a000f0
	.4byte 0x00a000f8
	.4byte 0x00a800d8
	.4byte 0x00a800e0
	.4byte 0x00a800e8
	.4byte 0x00a800f0
	.4byte 0x00a800f8
	.4byte 0x00b000d8
	.4byte 0x00b000f8
	.4byte 0x00000000
	.4byte 0x00000019
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0xc13c0100
	.4byte 0xb9d2cf52
	.4byte 0x13465bb3
	.4byte 0x5afce9e8
	.4byte 0xafd1ba81
	.4byte 0x9a39eb47
	.4byte 0x7445b435
	.4byte 0x2ff63444
	.4byte 0x74759d7d
	.4byte 0x581ed0e8
	.4byte 0x499ece98
	.4byte 0x66cc9bb5
	.4byte 0x28a91658
	.4byte 0xd6468754
	.4byte 0xb8fc3ca3
	.4byte 0x0394251c
	.4byte 0xa3921e4e
	.4byte 0xed0a619d
	.4byte 0x290915e5
	.4byte 0xd0d9ce84
	.4byte 0xcb0f067c
	.4byte 0x3a788051
	.4byte 0x4d0804f7
	.4byte 0xe32e6224
	.4byte 0x26d300f1
	.4byte 0xc780ce20
	.4byte 0x883c5604
	.4byte 0x991f8fbc
	.4byte 0x64fd4ba6
	.4byte 0x5026593a
	.4byte 0x42e4728b
	.4byte 0x4d0a3424
	.4byte 0xbe74d99b
	.4byte 0xe6f939cd
	.4byte 0xb126f91e
	.4byte 0xf32807ec
	.4byte 0x7c84cbcd
	.4byte 0xdc069263
	.4byte 0x984841e2
	.4byte 0x998e2689
	.4byte 0xd98df5e0
	.4byte 0x7ce0c6f9
	.4byte 0x70784b43
	.4byte 0xf7f6bc36
	.4byte 0xc7b95e4d
	.4byte 0xe7719be3
	.4byte 0x17a9ad75
	.4byte 0x586f8f25
	.4byte 0x75fcd4d3
	.4byte 0x08068958
	.4byte 0x9c9a2b93
	.4byte 0xfbcfa216
	.4byte 0x7e458240
	.4byte 0x86f9f834
	.4byte 0xfc221ebe
	.4byte 0xef811f5e
	.4byte 0x813ce478
	.4byte 0x242f015d
	.4byte 0xc6de6c1d
	.4byte 0x6faf3beb
	.4byte 0x5013be3c
	.4byte 0xf8f0a8bc
	.4byte 0x78c181c6
	.4byte 0xcf386e27
	.4byte 0xde5c3c49
	.4byte 0x6677cfc8
	.4byte 0x2a0df303
	.4byte 0x8f8df3f1
	.4byte 0xdf1e77c1
	.4byte 0x3e78c038
	.4byte 0x3de7e014
	.4byte 0x638c3f8f
	.4byte 0x4481687e
	.4byte 0x401077ce
	.4byte 0xf831f014
	.4byte 0x36800cc6
	.4byte 0x7d8b83c0
	.4byte 0x80cfbf02
	.4byte 0xe1c3400a
	.4byte 0xb40d23c6
	.4byte 0xde213ce2
	.4byte 0x9e3c2f19
	.4byte 0xe39df5f3
	.4byte 0xa8c6f9ae
	.4byte 0x8291fe62
	.4byte 0x80556f9c
	.4byte 0x88e0f404
	.4byte 0x38c0bf22
	.4byte 0xe8dadf3f
	.4byte 0xe01df023
	.4byte 0x102cef93
	.4byte 0x12c8cc41
	.4byte 0xbe40f187
	.4byte 0x39f09e71
	.4byte 0xb7dfcef8
	.4byte 0x0b10cf5e
	.4byte 0x3e012227
	.4byte 0x0e4fbaf0
	.4byte 0x344d6c12
	.4byte 0xc30f8e72
	.4byte 0xf8225e77
	.4byte 0x3bebd6fb
	.4byte 0xbdc9e3ef
	.4byte 0x3cf1a54a
	.4byte 0x323dcfe0
	.4byte 0x859f5350
	.4byte 0x38339cab
	.4byte 0xf5e0510f
	.4byte 0x3410af12
	.4byte 0x1f8088f5
	.4byte 0x9fbaf811
	.4byte 0xf19223df
	.4byte 0x607f04a3
	.4byte 0xadf82f92
	.4byte 0x04abd09e
	.4byte 0xc14f8168
	.4byte 0xaf1efc75
	.4byte 0xf82f19f3
	.4byte 0x301f82fa
	.4byte 0xd7cc2978
	.4byte 0xe0bebc73
	.4byte 0xc17dfdfd
	.4byte 0x033e31e3
	.4byte 0x1f45f3f4
	.4byte 0x18ebc7bf
	.4byte 0x043e08c6
	.4byte 0x60fe70ff
	.4byte 0x904fe2be
	.4byte 0xbaf1735e
	.4byte 0xbb803205
	.4byte 0xa18451f8
	.4byte 0xc00d1f80
	.4byte 0xf0ba1e1e
	.4byte 0x9f9af819
	.4byte 0xc17d79e3
	.4byte 0x1f7008cf
	.4byte 0x2df20be7
	.4byte 0x31f1147f
	.4byte 0xc04f8120
	.4byte 0x32147c17
	.4byte 0x5e1eefe1
	.4byte 0x7c2640a1
	.4byte 0x181fd39d
	.4byte 0xefe15ebc
	.4byte 0x67053ea8
	.4byte 0x8e7809f0
	.4byte 0xd1fc1bc7
	.4byte 0xfc0bd7c0
	.4byte 0x4aebc0dd
	.4byte 0x39f48a5e
	.4byte 0xc10f8eb8
	.4byte 0xbe147c63
	.4byte 0x3e1defe0
	.4byte 0x49f24f08
	.4byte 0x00b382f9
	.4byte 0xf82f891f
	.4byte 0xe3240ab9
	.4byte 0x0c3ea7e7
	.4byte 0x2009f2ef
	.4byte 0xf907af8a
	.4byte 0xc688dc18
	.4byte 0x1e07e087
	.4byte 0xe754707f
	.4byte 0x3f13b043
	.4byte 0xb851f3b3
	.4byte 0x09008f99
	.4byte 0xebeb483c
	.4byte 0x4b053e49
	.4byte 0xf3f2c632
	.4byte 0x3e026724
	.4byte 0xa3f2ef0e
	.4byte 0xf09a5f2b
	.4byte 0x00003efe
	.4byte 0x0200c200
	.4byte 0x0200c23c
	.4byte 0x0200c278
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000008
	.4byte 0x00090200
	.4byte 0x02010000
	.4byte 0x0000000a
	.4byte 0x000b0202
	.4byte 0x02030000
	.4byte 0x0000000c
	.4byte 0x000d0204
	.4byte 0x02050000
	.4byte 0x0008ffff
	.4byte 0x02000000
	.4byte 0x00000009
	.4byte 0x000a0201
	.4byte 0x02020000
	.4byte 0x0000000b
	.4byte 0x000c0203
	.4byte 0x02040000
	.4byte 0x0000000d
	.4byte 0x000e0205
	.4byte 0x02060000
	.4byte 0x0000000f
	.4byte 0x00100207
	.4byte 0x02080000
	.4byte 0x00000011
	.4byte 0x00120209
	.4byte 0x020a0000
	.4byte 0x0008ffff
	.4byte 0x02000000
	.4byte 0x00000009
	.4byte 0x000a0201
	.4byte 0x02020000
	.4byte 0x0000000b
	.4byte 0xffff0203
	.4byte 0x00000008
	.4byte 0x00090200
	.4byte 0x02010000
	.4byte 0x0000000a
	.4byte 0xffff0202
	.4byte 0x00000008
	.4byte 0xffff0200
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000198
	.4byte 0xc00001e0
	.4byte 0x01200000
	.4byte 0x02100178
	.4byte 0x00000218
	.4byte 0xffff0002
	.4byte 0x00000198
	.4byte 0x400001c8
	.4byte 0x01200000
	.4byte 0x02100178
	.4byte 0x00000218
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0x400001a8
	.4byte 0x00000000
	.4byte 0x01c00010
	.4byte 0x000001f0
	.4byte 0xffff0004
	.4byte 0x00000178
	.4byte 0x40000058
	.4byte 0x00000000
	.4byte 0x01c00010
	.4byte 0x000001f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x002c00d8
	.4byte 0x00f800b8
	.4byte 0x00d8004c
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00180070
	.4byte 0x008001a0
	.4byte 0x01b00028
	.4byte 0x0003ffff
	.4byte 0x00180170
	.4byte 0x01800050
	.4byte 0x00600028
	.4byte 0x0004ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000043
	.4byte 0x00111002
	.4byte 0x00201044
	.4byte 0x00000044
	.4byte 0x00102043
	.4byte 0x00201048
	.4byte 0x0030104d
	.4byte 0x00401045
	.4byte 0x0050304d
	.4byte 0x00000045
	.4byte 0x00104044
	.4byte 0x00201046
	.4byte 0x00000046
	.4byte 0x00102045
	.4byte 0x00203046
	.4byte 0x00302046
	.4byte 0x00401047
	.4byte 0x00000047
	.4byte 0x00104046
	.4byte 0x00203047
	.4byte 0x00302047
	.4byte 0x0040104c
	.4byte 0x00000048
	.4byte 0x00102044
	.4byte 0x0020104e
	.4byte 0x00000049
	.4byte 0x0010204e
	.4byte 0x0020104a
	.4byte 0x0000004a
	.4byte 0x00102049
	.4byte 0x00212002
	.4byte 0x0030104b
	.4byte 0x0040104f
	.4byte 0x0000004b
	.4byte 0x0010304a
	.4byte 0x00213002
	.4byte 0x0000004c
	.4byte 0x00104047
	.4byte 0x0000004d
	.4byte 0x00103044
	.4byte 0x0020404d
	.4byte 0x00305044
	.4byte 0x0040204d
	.4byte 0x0000004e
	.4byte 0x00102048
	.4byte 0x00201049
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0058
	.4byte 0x0200c5d4
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0000d000
	.4byte 0xffff0058
	.4byte 0x0200c630
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000d000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00023000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00a20000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00020000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00020000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x00e60000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00020000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x00e60000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0058
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00f20000
	.4byte 0x0001b000
	.4byte 0xffff0058
	.4byte 0x00000001
	.4byte 0x02150000
	.4byte 0x00000000
	.4byte 0x010f0000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01b30000
	.4byte 0x00000000
	.4byte 0x01430000
	.4byte 0x00013000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x01d30000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00025000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01ce0000
	.4byte 0x00000000
	.4byte 0x01540000
	.4byte 0x0001d000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x01e20000
	.4byte 0x00000000
	.4byte 0x01540000
	.4byte 0x0001d000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x01e90000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x0001b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0x007400f6
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00fb
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00fb
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0162
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x03000000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte 0x02008621
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte 0x02008621
	.4byte 0x50008615
	.4byte 0x0202000a
	.4byte 0x02008621
	.4byte 0x50008615
	.4byte 0x0203000b
	.4byte 0x02008621
	.4byte 0x50008615
	.4byte 0x0204000c
	.4byte 0x02008621
	.4byte 0x50008615
	.4byte 0x0205000d
	.4byte 0x02008621
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008995
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02008a11
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000202
	.4byte 0xffff0005
	.4byte 0x0200ae15
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008995
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02008a11
	.4byte 0x50008805
	.4byte 0x088d000a
	.4byte 0x020095d5
	.4byte 0x00000000
	.4byte 0x08f00008
	.4byte 0x020084bd
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001ab0
	.4byte 0x00008d15
	.4byte 0x08f00008
	.4byte 0x00001aa2
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001ab7
	.4byte 0x00000000
	.4byte 0x08f00009
	.4byte 0x00001a99
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001ab1
	.4byte 0x00008d15
	.4byte 0x08f00009
	.4byte 0x00001aa3
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001ab8
	.4byte 0x00000000
	.4byte 0x08f0000a
	.4byte 0x00001a9a
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001ab2
	.4byte 0x00008d15
	.4byte 0x08f0000a
	.4byte 0x00001aa4
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001ab9
	.4byte 0x00000000
	.4byte 0x08f0000b
	.4byte 0x00001a9b
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001ab3
	.4byte 0x00008d15
	.4byte 0x08f0000b
	.4byte 0x00001aa5
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001aba
	.4byte 0x00000000
	.4byte 0x08f0000c
	.4byte 0x02008551
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001ab4
	.4byte 0x00008d15
	.4byte 0x088f040c
	.4byte 0x02008551
	.4byte 0x00008d15
	.4byte 0x08f0000c
	.4byte 0x00001aa6
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001abb
	.4byte 0x00000000
	.4byte 0x08f0000d
	.4byte 0x020085a9
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001ab5
	.4byte 0x00008d15
	.4byte 0x088f040d
	.4byte 0x020085a9
	.4byte 0x00008d15
	.4byte 0x08f0000d
	.4byte 0x00001aa7
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001abc
	.4byte 0x00000000
	.4byte 0x08f0000e
	.4byte 0x00001aa1
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001ab6
	.4byte 0x00008d15
	.4byte 0x08f0000e
	.4byte 0x00001aa8
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001abd
	.4byte 0x00000002
	.4byte 0x08f0000f
	.4byte 0x02009691
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte 0x02008635
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte 0x02008635
	.4byte 0x50008615
	.4byte 0x0202000a
	.4byte 0x02008635
	.4byte 0x50008615
	.4byte 0x0203000b
	.4byte 0x02008635
	.4byte 0x50008615
	.4byte 0x0204000c
	.4byte 0x02008635
	.4byte 0x50008615
	.4byte 0x0205000d
	.4byte 0x02008635
	.4byte 0x50008615
	.4byte 0x0206000e
	.4byte 0x02008635
	.4byte 0x50008615
	.4byte 0x0207000f
	.4byte 0x02008635
	.4byte 0x50008615
	.4byte 0x02080010
	.4byte 0x02008635
	.4byte 0x50008615
	.4byte 0x02090011
	.4byte 0x02008635
	.4byte 0x50008615
	.4byte 0x020a0012
	.4byte 0x02008635
	.4byte 0x00000002
	.4byte 0x020b000b
	.4byte 0x020086ad
	.4byte 0x00000002
	.4byte 0x120b000a
	.4byte 0x020087e1
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008995
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02008a11
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008281
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte 0x02008649
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte 0x02008649
	.4byte 0x50008615
	.4byte 0x0202000a
	.4byte 0x02008649
	.4byte 0x50008615
	.4byte 0x0203000b
	.4byte 0x02008649
	.4byte 0x00000002
	.4byte 0x020b000b
	.4byte 0x020086ad
	.4byte 0x00000002
	.4byte 0x120b000a
	.4byte 0x020087e1
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008995
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02008a11
	.4byte 0x00000006
	.4byte 0xffff00d2
	.4byte 0x02008a8d
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x02008c35
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte 0x02008685
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte 0x02008685
	.4byte 0x50008615
	.4byte 0x0202000a
	.4byte 0x02008685
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008995
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02008a11
	.4byte 0x00000006
	.4byte 0x088b00d2
	.4byte 0x02008a8d
	.4byte 0x00000006
	.4byte 0xffff00c9
	.4byte 0x02008d51
	.4byte 0x00000006
	.4byte 0xffff00ca
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0x088c000a
	.4byte 0x02008fc9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008995
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02008a11
	.4byte 0x50008805
	.4byte 0x0302000c
	.4byte 0x020083b9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008995
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02008a11
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00008515
	.4byte 0x020c0008
	.4byte 0x00000000
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008995
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02008a11
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x020092dd
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020093f9
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008601
	.4byte 0x00000002
	.4byte 0x120d0007
	.4byte 0x0200963d
	.4byte 0x00000002
	.4byte 0x020d0008
	.4byte 0x02009629
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02009519
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200957d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008995
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02008a11
	.4byte 0x50008805
	.4byte 0x08ed000a
	.4byte 0x02009289
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200ae99
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte 0x02008671
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008995
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02008a11
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00ba0000
	.4byte 0x00000001
	.4byte 0x00000026
