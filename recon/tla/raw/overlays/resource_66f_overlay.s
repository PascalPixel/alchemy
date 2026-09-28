.syntax unified
	.thumb
	push	{lr}
	movs	r0, #18
	movs	r1, #70
	bl 0x02009144
	pop	{pc}
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x91d4
	.2byte 0x0200
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	movs	r0, #0
	bx	lr
	.global Func_02000050
	.thumb_func
Func_02000050:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9204
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r2, r0, #0
	adds	r5, r2, #0
	adds	r5, #98
	ldrb	r3, [r5, #0]
	movs	r0, #63
	adds	r1, r2, #0
	ands	r0, r3
	adds	r1, #85
	movs	r3, #3
	ldr	r6, [r2, #80]
	strb	r3, [r1, #0]
	cmp	r0, #0
	bne.n	.L_0200007a
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r2, #40]
.L_0200007a:
	cmp	r0, #16
	bne.n	.L_02000084
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #40]
.L_02000084:
	cmp	r0, #24
	bne.n	.L_0200008e
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r2, #40]
.L_0200008e:
	lsls	r0, r0, #12
	bl 0x02009024
	cmp	r0, #0
	bge.n	.L_0200009a
	adds	r0, #63
.L_0200009a:
	asrs	r3, r0, #6
	strh	r3, [r6, #18]
	ldrb	r3, [r5, #0]
	movs	r0, #1
	adds	r3, #1
	strb	r3, [r5, #0]
	negs	r0, r0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.global Func_020000ac
	.thumb_func
Func_020000ac:
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_020000c4
	ldr	r0, [pc, #24]
	b.n	.L_020000d0
.L_020000c4:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020000ce
	ldr	r0, [pc, #24]
	b.n	.L_020000d0
.L_020000ce:
	ldr	r0, [pc, #24]
.L_020000d0:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000006b
	.4byte 0x020095f0
	.4byte 0x0000006c
	.4byte 0x020098c8
	.2byte 0x9294
	.2byte 0x0200
	push	{lr}
	movs	r0, #11
	sub	sp, #8
	bl 0x02009094
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #35
	adds	r0, #85
	strb	r3, [r2, #0]
	strb	r3, [r0, #0]
	movs	r2, #32
	movs	r3, #22
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	movs	r0, #24
	bl 0x0200906c
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009034
	add	sp, #8
	pop	{pc}
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #34
	str	r3, [sp, #4]
	movs	r5, #28
	movs	r0, #30
	movs	r1, #35
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200906c
	movs	r3, #33
	str	r3, [sp, #4]
	movs	r1, #34
	movs	r2, #1
	movs	r3, #1
	movs	r0, #30
	str	r5, [sp, #0]
	bl 0x0200906c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x02009034
	add	sp, #8
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	bl 0x02009094
	adds	r5, r0, #0
	adds	r7, r5, #0
	adds	r7, #85
	movs	r3, #3
	movs	r6, #60
	strb	r3, [r7, #0]
	b.n	.L_02000172
.L_02000170:
	subs	r6, #1
.L_02000172:
	cmp	r6, #0
	beq.n	.L_02000182
	movs	r0, #1
	bl 0x0200901c
	ldr	r3, [r5, #40]
	cmp	r3, #0
	bne.n	.L_02000170
.L_02000182:
	movs	r0, #10
	bl 0x0200901c
	movs	r3, #0
	strb	r3, [r7, #0]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	cmp	r1, #15
	bne.n	.L_0200019e
	movs	r0, #15
	movs	r1, #1
	bl 0x02009114
.L_0200019e:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #136
	mov	r8, r1
	cmp	r1, #14
	bne.n	.L_0200024c
	movs	r0, #14
	bl 0x02009094
	adds	r7, r0, #0
	movs	r0, #146
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_02000244
	ldr	r3, [r7, #8]
	asrs	r6, r3, #20
	cmp	r6, #47
	bne.n	.L_02000244
	ldr	r3, [r7, #16]
	asrs	r5, r3, #20
	cmp	r5, #28
	bne.n	.L_02000244
	movs	r0, #14
	bl 0x0200815c
	movs	r3, #1
	movs	r2, #1
	movs	r1, #28
	movs	r0, #49
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	bl 0x0200906c
	movs	r0, #146
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009034
	movs	r0, #14
	movs	r1, #2
	bl 0x02009114
	movs	r0, #10
	bl 0x02009094
	adds	r5, r0, #0
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #27
	bne.n	.L_02000218
	movs	r0, #2
	adds	r0, #255
	bl 0x0200919c
	movs	r3, #220
	b.n	.L_02000228
.L_02000218:
	subs	r3, #28
	cmp	r3, #1
	bhi.n	.L_0200022c
	movs	r0, #129
	lsls	r0, r0, #1
	bl 0x0200919c
	movs	r3, #236
.L_02000228:
	lsls	r3, r3, #17
	str	r3, [r5, #16]
.L_0200022c:
	ldr	r2, [r7, #16]
	ldr	r3, [r5, #16]
	cmp	r2, r3
	bge.n	.L_02000238
	movs	r0, #129
	b.n	.L_0200023a
.L_02000238:
	movs	r0, #130
.L_0200023a:
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009034
	b.n	.L_0200024c
.L_02000244:
	mov	r0, r8
	movs	r1, #1
	bl 0x02009114
.L_0200024c:
	mov	r2, r8
	cmp	r2, #15
	beq.n	.L_02000254
	b.n	.L_0200035a
.L_02000254:
	movs	r0, #15
	bl 0x02009094
	adds	r6, r0, #0
	movs	r0, #137
	lsls	r0, r0, #2
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_0200035a
	ldr	r3, [r6, #8]
	asrs	r3, r3, #20
	cmp	r3, #53
	bne.n	.L_0200035a
	ldr	r3, [r6, #16]
	asrs	r3, r3, #20
	cmp	r3, #36
	bne.n	.L_0200035a
	movs	r0, #15
	bl 0x0200815c
	movs	r1, #2
	movs	r0, #15
	bl 0x02009114
	movs	r0, #9
	bl 0x02009094
	adds	r7, r0, #0
	ldr	r1, [r6, #8]
	ldr	r3, [r7, #8]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L_020002a2
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_020002ac
	b.n	.L_020002e8
.L_020002a2:
	movs	r2, #128
	subs	r3, r3, r1
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_020002e8
.L_020002ac:
	adds	r3, r7, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	adds	r0, r7, #0
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #8]
	bl 0x02009054
	adds	r0, r7, #0
	bl 0x0200905c
	ldr	r1, [pc, #148]
	adds	r0, r6, #0
	bl 0x02009044
	adds	r0, r7, #0
	bl 0x0200904c
	movs	r0, #128
	str	r5, [r7, #8]
	str	r5, [r7, #16]
	lsls	r0, r0, #2
	bl 0x02009034
	b.n	.L_0200035a
.L_020002e8:
	add	r0, sp, #8
	adds	r1, r7, #0
	movs	r2, #128
	ldr	r5, [pc, #120]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2009
	bl 0x02009194
	movs	r1, #4
	movs	r2, #0
	movs	r0, #9
	bl 0x020090ec
	movs	r0, #30
	bl 0x0200907c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #9
	bl 0x0200911c
	movs	r2, #0
	movs	r1, #15
	movs	r0, #9
	bl 0x020090fc
	movs	r0, #60
	bl 0x0200907c
	movs	r1, #4
	movs	r0, #9
	bl 0x020090e4
	movs	r0, #20
	bl 0x0200907c
.L_02000334:
	movs	r2, #128
	add	r1, sp, #8
	adds	r0, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2335
	movs	r2, #36
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #53
	movs	r1, #35
	movs	r2, #1
	movs	r3, #1
	bl 0x0200906c
	movs	r0, #137
	lsls	r0, r0, #2
	bl 0x02009034
.L_0200035a:
	add	sp, #136
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02009404
	.2byte 0x0730
	.2byte 0x0300
	push	{lr}
	bl 0x02009084
	movs	r0, #0
	bl 0x02009164
	movs	r1, #191
	movs	r2, #192
	movs	r0, #17
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x020090ac
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r2, #190
	lsls	r2, r2, #1
	adds	r1, #234
	movs	r0, #16
	bl 0x020090b4
	movs	r0, #17
	bl 0x02009094
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r0, #17
	bl 0x020090cc
	movs	r0, #17
	bl 0x02009094
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_02000420
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_02000420
	movs	r0, #10
	bl 0x020090a4
	movs	r1, #191
	movs	r2, #194
	movs	r0, #10
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x020090b4
	movs	r1, #191
	movs	r2, #188
	movs	r0, #17
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x020090ac
	movs	r1, #191
	movs	r2, #190
	lsls	r2, r2, #1
	movs	r0, #10
	lsls	r1, r1, #2
	bl 0x020090c4
	movs	r0, #10
	movs	r1, #4
	bl 0x020090dc
	movs	r1, #2
	movs	r0, #10
	adds	r1, #255
	bl 0x02009124
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009034
.L_02000420:
	bl 0x0200908c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #130
	bl 0x02009034
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x0200914c
	pop	{pc}
	push	{lr}
	movs	r0, #18
	sub	sp, #8
	bl 0x02009094
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #35
	adds	r0, #85
	strb	r3, [r2, #0]
	strb	r3, [r0, #0]
	movs	r2, #16
	movs	r3, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	movs	r0, #6
	bl 0x0200906c
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x02009034
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #19
	sub	sp, #8
	bl 0x02009094
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #35
	adds	r0, #85
	strb	r3, [r2, #0]
	strb	r3, [r0, #0]
	movs	r2, #30
	movs	r3, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	movs	r0, #8
	bl 0x0200906c
	movs	r0, #137
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009034
	add	sp, #8
	pop	{pc}
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r3, r5, #0
	subs	r3, #16
	sub	sp, #8
	cmp	r3, #1
	bhi.n	.L_020004d2
	movs	r3, #20
	movs	r2, #33
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #120
	movs	r1, #33
	movs	r2, #8
	movs	r3, #2
	bl 0x0200906c
.L_020004d2:
	adds	r3, r5, #0
	subs	r3, #14
	cmp	r3, #1
	bhi.n	.L_020004ee
	movs	r3, #8
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #112
	movs	r1, #15
	movs	r2, #16
	movs	r3, #4
	bl 0x0200906c
.L_020004ee:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	sub	sp, #8
	adds	r5, r1, #0
	adds	r6, r2, #0
	bl 0x02009094
	ldr	r2, [r0, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200906c
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	adds	r7, r1, #0
	sub	sp, #8
	cmp	r7, #20
	bne.n	.L_02000566
	movs	r0, #20
	bl 0x02009094
	adds	r5, r0, #0
	movs	r0, #253
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_02000566
	ldr	r3, [r5, #8]
	asrs	r5, r3, #20
	cmp	r5, #4
	bne.n	.L_02000566
	movs	r0, #20
	bl 0x0200815c
	movs	r3, #28
	str	r3, [sp, #4]
	movs	r0, #3
	movs	r1, #28
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200906c
	movs	r0, #253
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02009034
.L_02000566:
	cmp	r7, #13
	bne.n	.L_020005b0
	movs	r0, #13
	bl 0x02009094
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #232
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_020005b0
	ldr	r3, [r5, #8]
	asrs	r6, r3, #20
	cmp	r6, #28
	bne.n	.L_020005b0
	ldr	r3, [r5, #16]
	asrs	r5, r3, #20
	cmp	r5, #30
	bne.n	.L_020005b0
	movs	r0, #13
	bl 0x0200815c
	movs	r0, #32
	movs	r1, #30
	movs	r2, #1
	movs	r3, #2
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200906c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #232
	bl 0x02009034
.L_020005b0:
	adds	r3, r7, #0
	subs	r3, #14
	cmp	r3, #1
	bhi.n	.L_020005cc
	movs	r0, #14
	movs	r1, #8
	movs	r2, #17
	bl 0x020084f4
	movs	r0, #15
	movs	r1, #8
	movs	r2, #17
	bl 0x020084f4
.L_020005cc:
	adds	r3, r7, #0
	subs	r3, #16
	cmp	r3, #1
	bhi.n	.L_020005e8
	movs	r0, #16
	movs	r1, #22
	movs	r2, #33
	bl 0x020084f4
	movs	r0, #17
	movs	r1, #22
	movs	r2, #33
	bl 0x020084f4
.L_020005e8:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.global Func_020005ec
	.thumb_func
Func_020005ec:
	push	{lr}
	ldr	r3, [pc, #44]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02000604
	ldr	r0, [pc, #32]
	b.n	.L_0200061a
.L_02000604:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_0200060e
	ldr	r0, [pc, #32]
	b.n	.L_0200061a
.L_0200060e:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_02000618
	ldr	r0, [pc, #28]
	b.n	.L_0200061a
.L_02000618:
	ldr	r0, [pc, #28]
.L_0200061a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000006b
	.4byte 0x02009ae4
	.4byte 0x0000006c
	.4byte 0x02009c58
	.4byte 0x0000006d
	.4byte 0x02009db4
	.2byte 0x9ad8
	.2byte 0x0200
	.global Func_0200063c
	.thumb_func
Func_0200063c:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #129
	adds	r3, r3, r1
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	sub	sp, #8
	bl 0x02009184
	bl 0x02009094
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	ldr	r7, [pc, #748]
	orrs	r3, r2
	movs	r2, #240
	lsls	r2, r2, #1
	strb	r3, [r0, #0]
	adds	r3, r7, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #736]
	movs	r6, #0
	cmp	r2, r3
	beq.n	.L_0200067a
	b.n	.L_020007d6
.L_0200067a:
	movs	r0, #18
	movs	r1, #1
	bl 0x02009114
	movs	r1, #1
	movs	r0, #19
	bl 0x02009114
	movs	r0, #19
	bl 0x02009094
	movs	r1, #0
	bl 0x02009074
	movs	r0, #19
	bl 0x02009094
	adds	r0, #89
	strb	r6, [r0, #0]
	movs	r0, #19
	bl 0x02009094
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #12
	bl 0x02009094
	adds	r0, #89
	strb	r6, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200902c
	cmp	r0, #0
	beq.n	.L_020006ee
	movs	r3, #34
	movs	r5, #28
	str	r3, [sp, #4]
	movs	r0, #30
	movs	r1, #35
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200906c
	movs	r3, #33
	str	r3, [sp, #4]
	movs	r0, #30
	movs	r1, #34
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200906c
.L_020006ee:
	movs	r0, #145
.L_020006f0:
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200902c
	cmp	r0, #0
	beq.n	.L_02000710
	movs	r3, #22
	movs	r2, #32
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #24
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	bl 0x0200906c
.L_02000710:
	movs	r0, #146
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200902c
	cmp	r0, #0
	beq.n	.L_02000734
	movs	r3, #47
	movs	r2, #28
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #49
	movs	r1, #28
	movs	r2, #1
	movs	r3, #1
	bl 0x0200906c
	b.n	.L_0200073c
.L_02000734:
	movs	r0, #14
	movs	r1, #1
	bl 0x02009114
.L_0200073c:
	movs	r0, #137
	lsls	r0, r0, #2
	bl 0x0200902c
	cmp	r0, #0
	beq.n	.L_0200075c
	movs	r3, #53
	movs	r2, #36
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #53
	movs	r1, #35
	movs	r2, #1
	movs	r3, #1
	bl 0x0200906c
.L_0200075c:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200902c
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_0200079c
	movs	r0, #16
	bl 0x02009094
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #17
	bl 0x02009094
	adds	r1, r0, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #224
	lsls	r3, r3, #16
	str	r3, [r0, #12]
	str	r3, [r0, #20]
	adds	r0, #89
	strb	r5, [r0, #0]
	b.n	.L_020007a6
.L_0200079c:
	movs	r1, #2
	movs	r0, #10
	adds	r1, #255
	bl 0x02009124
.L_020007a6:
	movs	r0, #17
	movs	r1, #231
	bl 0x0200918c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #130
	bl 0x0200902c
	cmp	r0, #0
	beq.n	.L_020007c8
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x020090d4
	b.n	.L_0200094a
.L_020007c8:
	movs	r0, #13
	bl 0x02009094
	movs	r3, #160
	lsls	r3, r3, #9
	str	r3, [r0, #28]
	b.n	.L_0200094a
.L_020007d6:
	ldr	r3, [pc, #384]
	cmp	r2, r3
	beq.n	.L_020007de
	b.n	.L_0200094a
.L_020007de:
	movs	r0, #8
	movs	r1, #1
	bl 0x02009114
	movs	r0, #9
	movs	r1, #1
	bl 0x02009114
	bl 0x02009174
	movs	r1, #144
	lsls	r1, r1, #4
	movs	r0, #0
	adds	r1, #232
	movs	r2, #8
	movs	r3, #9
	bl 0x0200917c
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x0200902c
	cmp	r0, #0
	beq.n	.L_02000822
	movs	r3, #4
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #6
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	bl 0x0200906c
.L_02000822:
	movs	r0, #137
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200902c
	cmp	r0, #0
	beq.n	.L_02000844
	movs	r3, #10
	movs	r2, #30
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #8
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	bl 0x0200906c
.L_02000844:
	movs	r0, #253
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200902c
	cmp	r0, #0
	beq.n	.L_02000890
	movs	r0, #20
	bl 0x02009094
	movs	r1, #144
	movs	r2, #228
	adds	r5, r0, #0
	lsls	r1, r1, #15
	movs	r0, #20
	lsls	r2, r2, #17
	bl 0x020090d4
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r0, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x02009064
	movs	r3, #4
	movs	r2, #28
	str	r0, [r5, #20]
	str	r0, [r5, #12]
	movs	r1, #28
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #3
	movs	r2, #1
	movs	r3, #1
	bl 0x0200906c
.L_02000890:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #232
	bl 0x0200902c
	cmp	r0, #0
	beq.n	.L_020008dc
	movs	r0, #13
	bl 0x02009094
	movs	r1, #228
	movs	r2, #244
	adds	r5, r0, #0
	lsls	r1, r1, #17
	movs	r0, #13
	lsls	r2, r2, #17
	bl 0x020090d4
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r0, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x02009064
	movs	r3, #28
	movs	r2, #30
	str	r0, [r5, #20]
	str	r0, [r5, #12]
	movs	r1, #30
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #32
	movs	r2, #1
	movs	r3, #2
	bl 0x0200906c
.L_020008dc:
	movs	r3, #112
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #8
	movs	r1, #15
	movs	r2, #16
	movs	r3, #4
	bl 0x0200906c
	movs	r3, #120
	movs	r2, #33
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #2
	movs	r0, #20
	movs	r1, #33
	movs	r2, #8
	bl 0x0200906c
	movs	r0, #14
	movs	r1, #8
	movs	r2, #17
	bl 0x020084f4
	movs	r0, #15
	movs	r1, #8
	movs	r2, #17
	bl 0x020084f4
	movs	r0, #16
	movs	r1, #22
	movs	r2, #33
	bl 0x020084f4
	movs	r0, #17
	movs	r1, #22
	movs	r2, #33
	bl 0x020084f4
	movs	r0, #10
	adds	r0, #255
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_0200094a
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #8
	bne.n	.L_0200094a
	bl 0x02008af0
.L_0200094a:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0000006b
	.2byte 0x006c
	.2byte 0x0000
	.global Func_0200095c
	.thumb_func
Func_0200095c:
	movs	r0, #0
	bx	lr
	push	{lr}
	bl 0x02009084
	movs	r0, #0
	bl 0x02009164
	ldr	r0, [pc, #344]
	bl 0x02009104
	movs	r1, #156
	movs	r2, #152
	lsls	r2, r2, #17
	movs	r0, #27
	lsls	r1, r1, #17
	bl 0x020090d4
	movs	r0, #27
	movs	r1, #0
	bl 0x0200910c
	movs	r0, #128
	movs	r1, #1
	movs	r2, #196
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200912c
	bl 0x02009134
	movs	r0, #11
	bl 0x02009194
	movs	r0, #11
	movs	r1, #27
	movs	r2, #0
	bl 0x020090fc
	movs	r1, #132
	movs	r2, #156
	movs	r0, #27
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x020090c4
	movs	r1, #128
	movs	r2, #200
	lsls	r2, r2, #1
	movs	r0, #27
	lsls	r1, r1, #1
	bl 0x020090c4
	movs	r0, #11
	movs	r1, #3
	bl 0x020090e4
	movs	r0, #11
	movs	r1, #0
	bl 0x0200910c
	movs	r0, #11
	movs	r1, #1
	bl 0x020090f4
	movs	r0, #11
	movs	r1, #0
	bl 0x0200910c
	movs	r0, #27
	movs	r1, #4
	bl 0x020090e4
	movs	r0, #27
	movs	r1, #0
	bl 0x0200910c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200910c
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200911c
	movs	r0, #11
	movs	r1, #0
	bl 0x0200910c
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #27
	bl 0x0200911c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200910c
	movs	r0, #11
	movs	r1, #3
	bl 0x020090e4
	movs	r0, #11
	movs	r1, #0
	bl 0x0200910c
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #27
	bl 0x0200911c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200910c
	movs	r1, #132
	movs	r2, #156
	movs	r0, #27
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x020090c4
	movs	r1, #180
	movs	r2, #156
	lsls	r2, r2, #1
	movs	r0, #27
	lsls	r1, r1, #1
	bl 0x020090bc
	movs	r0, #11
	movs	r1, #3
	bl 0x020090e4
	movs	r0, #11
	movs	r1, #0
	bl 0x0200910c
	ldr	r3, [pc, #80]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #11
	bl 0x0200916c
	movs	r0, #30
	bl 0x0200901c
	movs	r0, #11
	movs	r1, #0
	bl 0x0200910c
	movs	r1, #2
	movs	r0, #11
	bl 0x020090f4
	movs	r0, #60
	bl 0x0200907c
	movs	r2, #0
	movs	r0, #27
	movs	r1, #0
	bl 0x020090d4
	ldr	r1, [pc, #28]
	movs	r0, #11
.L_02000ab4:
	bl 0x0200909c
	bl 0x0200908c
	movs	r0, #136
.L_02000abe:
	lsls	r0, r0, #4
	bl 0x02009034
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00001d64
	.4byte 0x02000240
	.2byte 0x97f8
	.2byte 0x0200
	push	{r5, lr}
	bl 0x02009094
.L_02000ada:
	movs	r1, #1
	adds	r5, r0, #0
	bl 0x0200903c
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #48]
	pop	{r5, pc}
	push	{r5, lr}
	ldr	r3, [pc, #636]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r5, [r3, #0]
	bl 0x02009084
	movs	r0, #0
	bl 0x02009164
	movs	r1, #248
	movs	r2, #236
	movs	r0, #23
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x020090d4
	movs	r1, #132
	movs	r2, #236
	movs	r0, #24
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x020090d4
	movs	r1, #235
	movs	r0, #25
	lsls	r1, r1, #16
	ldr	r2, [pc, #584]
	bl 0x020090d4
	movs	r1, #232
	movs	r2, #248
	movs	r0, #26
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x020090d4
	movs	r0, #27
	ldr	r1, [pc, #568]
	ldr	r2, [pc, #560]
	bl 0x020090d4
	movs	r1, #140
	movs	r2, #248
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #28
	bl 0x020090d4
	adds	r0, r5, #0
	bl 0x02008ad4
	movs	r0, #23
	bl 0x02008ad4
	movs	r0, #24
	bl 0x02008ad4
	movs	r0, #25
	bl 0x02008ad4
	movs	r0, #26
	bl 0x02008ad4
	movs	r0, #27
	bl 0x02008ad4
	movs	r0, #28
	bl 0x02008ad4
	bl 0x02009154
	bl 0x0200915c
	ldr	r0, [pc, #500]
	bl 0x02009104
	movs	r1, #4
	movs	r2, #30
	adds	r1, #255
	movs	r0, #27
	bl 0x0200911c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200910c
	movs	r2, #0
	movs	r1, #25
	movs	r0, #27
	bl 0x020090fc
	movs	r0, #10
	bl 0x0200907c
	movs	r0, #25
	movs	r1, #3
	bl 0x020090e4
	adds	r1, r5, #0
	movs	r2, #0
	movs	r0, #27
	bl 0x020090fc
	movs	r0, #10
	bl 0x0200907c
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #27
	bl 0x0200911c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200910c
	movs	r0, #27
	movs	r1, #3
	bl 0x020090e4
	movs	r0, #27
	movs	r1, #0
	bl 0x0200910c
	movs	r0, #25
	movs	r1, #23
	movs	r2, #0
	bl 0x020090fc
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200911c
	movs	r0, #25
	movs	r1, #0
	bl 0x0200910c
	movs	r2, #134
	movs	r1, #248
	lsls	r2, r2, #2
	movs	r0, #23
	bl 0x020090bc
	movs	r0, #1
	bl 0x0200907c
	movs	r1, #132
	movs	r2, #134
	lsls	r2, r2, #2
	movs	r0, #24
	lsls	r1, r1, #1
	bl 0x020090bc
	adds	r1, r5, #0
	movs	r0, #25
	bl 0x0200916c
	adds	r1, r5, #0
	movs	r0, #26
	bl 0x0200916c
	adds	r1, r5, #0
	movs	r0, #27
	bl 0x0200916c
	adds	r1, r5, #0
	movs	r0, #28
	bl 0x0200916c
	movs	r0, #2
	bl 0x0200907c
	adds	r0, r5, #0
	bl 0x02009094
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #128
	movs	r2, #138
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	adds	r0, r5, #0
	bl 0x020090c4
	movs	r0, #1
	bl 0x0200907c
	adds	r0, r5, #0
	bl 0x02009094
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	movs	r0, #23
	bl 0x020090dc
	movs	r1, #1
	movs	r0, #24
	bl 0x020090dc
	movs	r0, #20
	bl 0x0200907c
	movs	r1, #4
	movs	r0, #23
	bl 0x020090dc
	movs	r0, #3
	bl 0x0200907c
	movs	r1, #4
	movs	r0, #24
	bl 0x020090e4
	movs	r0, #20
	bl 0x0200907c
	movs	r2, #244
	movs	r1, #248
	lsls	r2, r2, #1
	movs	r0, #23
	bl 0x020090bc
	movs	r0, #2
	bl 0x0200907c
	movs	r1, #132
	movs	r2, #244
	movs	r0, #24
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x020090c4
	movs	r2, #220
	movs	r0, #23
	movs	r1, #248
	lsls	r2, r2, #1
	bl 0x020090bc
	movs	r1, #132
	movs	r2, #220
	movs	r0, #24
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x020090bc
	movs	r2, #228
	movs	r0, #25
	movs	r1, #232
	lsls	r2, r2, #1
	bl 0x020090bc
	movs	r2, #236
	movs	r0, #26
	movs	r1, #232
	lsls	r2, r2, #1
	bl 0x020090bc
	movs	r1, #140
	movs	r2, #228
	movs	r0, #27
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x020090bc
	movs	r1, #140
	movs	r2, #236
	movs	r0, #28
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x020090c4
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x020090d4
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x020090d4
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x020090d4
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x020090d4
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x020090d4
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x020090d4
	bl 0x0200908c
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02009034
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x01e30000
	.4byte 0x01150000
	.2byte 0x1d71
	.2byte 0x0000
	push	{lr}
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200902c
	ldr	r3, [pc, #24]
	cmp	r0, #0
	beq.n	.L_02000d9a
	adds	r0, r3, #0
	movs	r1, #9
	bl 0x0200913c
	b.n	.L_02000da2
.L_02000d9a:
	adds	r0, r3, #0
	movs	r1, #8
	bl 0x0200913c
.L_02000da2:
	pop	{pc}
	.2byte 0x006c
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #116]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r6, [r3, #0]
	adds	r5, r0, #0
	bl 0x02009084
	movs	r0, #0
	bl 0x02009164
	adds	r0, r5, #0
	bl 0x02009194
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #1
	movs	r2, #30
	bl 0x0200911c
	movs	r2, #20
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl 0x020090fc
	ldr	r0, [pc, #68]
	bl 0x02009104
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200910c
	movs	r1, #2
	adds	r0, r6, #0
	adds	r1, #255
	bl 0x02009124
	adds	r0, r5, #0
	movs	r1, #4
	bl 0x020090dc
	movs	r0, #30
	bl 0x0200907c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200910c
.L_02000e0a:
	movs	r1, #128
	adds	r0, r6, #0
	lsls	r1, r1, #1
	bl 0x02009124
.L_02000e14:
	bl 0x02008d80
	bl 0x0200908c
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1d6f
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #116]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r6, [r3, #0]
	adds	r5, r0, #0
	bl 0x02009084
	movs	r0, #0
	bl 0x02009164
	adds	r0, r5, #0
	bl 0x020090a4
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #1
	movs	r2, #30
	bl 0x0200911c
	movs	r2, #20
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl 0x020090fc
	ldr	r0, [pc, #68]
	bl 0x02009104
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200910c
	movs	r1, #2
	adds	r0, r6, #0
	adds	r1, #255
	bl 0x02009124
	adds	r0, r5, #0
	movs	r1, #4
	bl 0x020090dc
	movs	r0, #30
	bl 0x0200907c
	movs	r0, #8
	movs	r1, #0
	bl 0x0200910c
	movs	r1, #128
	adds	r0, r6, #0
	lsls	r1, r1, #1
	bl 0x02009124
	bl 0x02008d80
	bl 0x0200908c
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1d78
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009094
	adds	r5, r0, #0
	movs	r0, #14
	bl 0x02009094
	ldr	r3, [r5, #16]
	ldr	r1, [r0, #16]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L_02000ed4
	movs	r3, #128
	lsls	r3, r3, #13
	cmp	r2, r3
	blt.n	.L_02000ede
	b.n	.L_02000ee6
.L_02000ed4:
	movs	r2, #128
	subs	r3, r3, r1
	lsls	r2, r2, #13
	cmp	r3, r2
	bge.n	.L_02000ee6
.L_02000ede:
	ldr	r2, [r0, #8]
	ldr	r3, [r5, #8]
	cmp	r2, r3
	bgt.n	.L_02000eec
.L_02000ee6:
	movs	r0, #10
	bl 0x02008da8
.L_02000eec:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009094
	adds	r5, r0, #0
	movs	r0, #15
	bl 0x02009094
	ldr	r3, [r5, #16]
	ldr	r1, [r0, #16]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L_02000f20
	movs	r3, #128
	lsls	r3, r3, #13
	cmp	r2, r3
	blt.n	.L_02000f2a
	b.n	.L_02000f32
.L_02000f20:
	movs	r2, #128
	subs	r3, r3, r1
	lsls	r2, r2, #13
	cmp	r3, r2
	bge.n	.L_02000f32
.L_02000f2a:
	ldr	r2, [r0, #8]
	ldr	r3, [r5, #8]
	cmp	r2, r3
	blt.n	.L_02000f38
.L_02000f32:
	movs	r0, #10
	bl 0x02008da8
.L_02000f38:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #11
	bl 0x02008da8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #11
	bl 0x02008da8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #3
	beq.n	.L_02000f70
	movs	r0, #12
	bl 0x02008da8
.L_02000f70:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #12
	bl 0x02008da8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #8
	bl 0x02008da8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #8
	bl 0x02008da8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_02000fc0
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_02000fc0
	movs	r0, #10
	bl 0x02008e28
.L_02000fc0:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_02000fd8
	movs	r0, #9
	bl 0x02008da8
.L_02000fd8:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_02000ff0
	movs	r0, #9
	bl 0x02008da8
.L_02000ff0:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_02001018
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200902c
	cmp	r0, #0
	bne.n	.L_02001018
	movs	r0, #10
	bl 0x02008e28
.L_02001018:
	pop	{pc}
	.2byte 0x0000
	.section .rodata,"a",%progbits
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00180000
	.4byte 0x00000011
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
	.4byte 0x0000006b
	.4byte 0x1011a002
	.4byte 0xffffffff
	.4byte 0x1020206c
	.4byte 0xffffffff
	.4byte 0x1030306c
	.4byte 0xffffffff
	.4byte 0x1040406c
	.4byte 0xffffffff
	.4byte 0x1050106d
	.4byte 0xffffffff
	.4byte 0x1060206d
	.4byte 0xffffffff
	.4byte 0x1070306d
	.4byte 0xffffffff
	.4byte 0x0000006c
	.4byte 0x10119002
	.4byte 0xffffffff
	.4byte 0x1020206b
	.4byte 0xffffffff
	.4byte 0x1030306b
	.4byte 0xffffffff
	.4byte 0x1040406b
	.4byte 0xffffffff
	.4byte 0x0000006d
	.4byte 0x1010506b
	.4byte 0xffffffff
	.4byte 0x1020606b
	.4byte 0xffffffff
	.4byte 0x1030706b
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000005a
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000005a
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000005a
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000005a
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x02008059
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00014000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00004000
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00030000
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00020000
	.4byte 0x00000004
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00020000
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00020000
	.4byte 0x00000004
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00020000
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00020000
	.4byte 0x00000004
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00030000
	.4byte 0x00000004
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00020000
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00020000
	.4byte 0x00000004
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00020000
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00020000
	.4byte 0x00000004
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00020000
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x08ff0053
	.4byte 0x020092ac
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0x08ff0053
	.4byte 0x02009334
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x0002c000
	.4byte 0x08ff00bd
	.4byte 0x02009410
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x0002c000
	.4byte 0xffff012b
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x0002c000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x02009284
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0002c000
	.4byte 0xffff011f
	.4byte 0x02009284
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0002c000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x02dc0000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0002c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02dc0000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0002c000
	.4byte 0x007600f6
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00004000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000005a
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000005a
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000005a
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000005a
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0002c000
	.4byte 0x08ff0053
	.4byte 0x02009728
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0x08ff0053
	.4byte 0x020097f8
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0x08ff0053
	.4byte 0x020092ac
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff00fa
	.4byte 0x02009284
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0002c000
	.4byte 0xffff011f
	.4byte 0x02009284
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0002c000
	.4byte 0xffff011f
	.4byte 0x02009284
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0102c000
	.4byte 0xffff011f
	.4byte 0x02009284
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0102c000
	.4byte 0xffff011f
	.4byte 0x02009284
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x0102c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x02009284
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x0002c000
	.4byte 0xffff0131
	.4byte 0x020091a4
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00028000
	.4byte 0xffff0131
	.4byte 0x020091a4
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x01020000
	.4byte 0xffff0053
	.4byte 0x020092ac
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023f00
	.4byte 0xffff0053
	.4byte 0x02009334
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024100
	.4byte 0xffff0053
	.4byte 0x020092ac
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0053
	.4byte 0x02009334
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0053
	.4byte 0x020092ac
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff0053
	.4byte 0x02009334
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
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
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x0000ce01
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x0000ce01
	.4byte 0x18820006
	.4byte 0x00000006
	.4byte 0x0000ce01
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x08ff0014
	.4byte 0x02008f85
	.4byte 0x00000002
	.4byte 0x08ff0015
	.4byte 0x02008f91
	.4byte 0x00000002
	.4byte 0x08ff0016
	.4byte 0x02008f9d
	.4byte 0x00000002
	.4byte 0x08ff0017
	.4byte 0x02008fc5
	.4byte 0x00000002
	.4byte 0x08ff0018
	.4byte 0x02008fdd
	.4byte 0x00000002
	.4byte 0x08ff0019
	.4byte 0x02008ff5
	.4byte 0x00008602
	.4byte 0xffff0021
	.4byte 0x02008439
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008191
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x020081a1
	.4byte 0x00000000
	.4byte 0x1202000a
	.4byte 0x00001d7b
	.4byte 0x00008d15
	.4byte 0x1202000a
	.4byte 0x00001d7d
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001d7e
	.4byte 0x00000000
	.4byte 0x1200000f
	.4byte 0x00001d7a
	.4byte 0x00008d15
	.4byte 0x1200000f
	.4byte 0x00001d7c
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008039
	.4byte 0x00001815
	.4byte 0x0221000b
	.4byte 0x020080ed
	.4byte 0x00000c15
	.4byte 0x0222000c
	.4byte 0x02008125
	.4byte 0x00004e15
	.4byte 0xffff000d
	.4byte 0x02008429
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x020081a1
	.4byte 0x10008c15
	.4byte 0x0200000f
	.4byte 0x02008191
	.4byte 0x00008c15
	.4byte 0x0200000f
	.4byte 0x020081a1
	.4byte 0x00008715
	.4byte 0x0202c810
	.4byte 0x0200836d
	.4byte 0x00008715
	.4byte 0x02020810
	.4byte 0x0200836d
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
	.4byte 0x00000002
	.4byte 0x08ff001e
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0x08ff001f
	.4byte 0x02008ef5
	.4byte 0x00000002
	.4byte 0x08ff0020
	.4byte 0x02008f41
	.4byte 0x00000002
	.4byte 0x08ff0021
	.4byte 0x02008f4d
	.4byte 0x00000002
	.4byte 0x08800022
	.4byte 0x02008961
	.4byte 0x00000002
	.4byte 0x08ff0023
	.4byte 0x02008f59
	.4byte 0x00000002
	.4byte 0x08ff0024
	.4byte 0x02008f79
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x020084b1
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200851d
	.4byte 0x00008515
	.4byte 0x09e80008
	.4byte 0x00000000
	.4byte 0x00001815
	.4byte 0x02100012
	.4byte 0x02008441
	.4byte 0x00001815
	.4byte 0x02110013
	.4byte 0x02008479
	.4byte 0x10009a15
	.4byte 0xffff0015
	.4byte 0x00000000
	.4byte 0x10009a15
	.4byte 0xffff0016
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x0200851d
	.4byte 0x00008c15
	.4byte 0x08e70014
	.4byte 0x0200851d
	.4byte 0x10008c15
	.4byte 0xffff000e
	.4byte 0x020084b1
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte 0x020084b1
	.4byte 0x10008c15
	.4byte 0xffff0010
	.4byte 0x020084b1
	.4byte 0x10008c15
	.4byte 0xffff0011
	.4byte 0x020084b1
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x0200851d
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x0200851d
	.4byte 0x00008c15
	.4byte 0xffff0010
	.4byte 0x0200851d
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte 0x0200851d
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
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
