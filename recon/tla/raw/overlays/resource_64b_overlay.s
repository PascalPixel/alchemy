.syntax unified
	.thumb
	push	{r5, r6, r7, lr}
	ldr	r2, [pc, #92]
	movs	r1, #128
	movs	r3, #192
	lsls	r1, r1, #2
	lsls	r3, r3, #18
	adds	r1, #18
	ldr	r6, [r3, #108]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_02000094
	subs	r1, #122
	movs	r3, #150
	lsls	r3, r3, #2
	adds	r5, r6, r1
	adds	r7, r2, r3
	ldr	r3, [r5, #0]
	movs	r1, #10
	lsls	r0, r3, #3
	adds	r0, r0, r3
	bl 0x0200ca58
	ldr	r3, [r7, #0]
	cmp	r3, r0
	blt.n	.L_02000094
	bl 0x0200ca80
	movs	r3, #128
	lsls	r3, r3, #8
	cmp	r0, r3
	bcs.n	.L_02000090
	movs	r0, #128
	lsls	r0, r0, #4
	movs	r1, #8
	adds	r0, #10
	bl 0x0200cd50
	movs	r1, #202
	lsls	r1, r1, #1
	adds	r2, r6, r1
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_02000094
.L_02000090:
	ldr	r3, [r5, #0]
	str	r3, [r7, #0]
.L_02000094:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r2, [pc, #92]
	movs	r1, #128
	movs	r3, #192
	lsls	r1, r1, #2
	lsls	r3, r3, #18
	adds	r1, #18
	ldr	r6, [r3, #108]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_020000f8
	subs	r1, #122
	movs	r3, #150
	lsls	r3, r3, #2
	adds	r5, r6, r1
	adds	r7, r2, r3
	ldr	r3, [r5, #0]
	movs	r1, #10
	lsls	r0, r3, #3
	adds	r0, r0, r3
	bl 0x0200ca58
	ldr	r3, [r7, #0]
	cmp	r3, r0
	blt.n	.L_020000f8
	bl 0x0200ca80
	movs	r3, #128
	lsls	r3, r3, #8
	cmp	r0, r3
	bcs.n	.L_020000f4
	movs	r0, #128
	lsls	r0, r0, #4
	movs	r1, #28
	adds	r0, #11
	bl 0x0200cd50
	movs	r1, #202
	lsls	r1, r1, #1
	adds	r2, r6, r1
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_020000f8
.L_020000f4:
	ldr	r3, [r5, #0]
	str	r3, [r7, #0]
.L_020000f8:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r2, [pc, #92]
	movs	r1, #128
	movs	r3, #192
	lsls	r1, r1, #2
	lsls	r3, r3, #18
	adds	r1, #18
	ldr	r6, [r3, #108]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_0200015c
	subs	r1, #122
	movs	r3, #150
	lsls	r3, r3, #2
	adds	r5, r6, r1
	adds	r7, r2, r3
	ldr	r3, [r5, #0]
	movs	r1, #10
	lsls	r0, r3, #3
	adds	r0, r0, r3
	bl 0x0200ca58
	ldr	r3, [r7, #0]
	cmp	r3, r0
	blt.n	.L_0200015c
	bl 0x0200ca80
	movs	r3, #128
	lsls	r3, r3, #8
	cmp	r0, r3
	bcs.n	.L_02000158
	movs	r0, #128
	lsls	r0, r0, #4
	movs	r1, #31
	adds	r0, #12
	bl 0x0200cd50
	movs	r1, #202
	lsls	r1, r1, #1
	adds	r2, r6, r1
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_0200015c
.L_02000158:
	ldr	r3, [r5, #0]
	str	r3, [r7, #0]
.L_0200015c:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r2, [pc, #92]
	movs	r1, #128
	movs	r3, #192
	lsls	r1, r1, #2
	lsls	r3, r3, #18
	adds	r1, #18
	ldr	r6, [r3, #108]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_020001c0
	subs	r1, #122
	movs	r3, #150
	lsls	r3, r3, #2
	adds	r5, r6, r1
	adds	r7, r2, r3
	ldr	r3, [r5, #0]
	movs	r1, #10
	lsls	r0, r3, #3
	adds	r0, r0, r3
	bl 0x0200ca58
	ldr	r3, [r7, #0]
	cmp	r3, r0
	blt.n	.L_020001c0
	bl 0x0200ca80
	movs	r3, #128
	lsls	r3, r3, #8
	cmp	r0, r3
	bcs.n	.L_020001bc
	movs	r0, #128
	lsls	r0, r0, #4
	movs	r1, #72
	adds	r0, #13
	bl 0x0200cd50
	movs	r1, #202
	lsls	r1, r1, #1
	adds	r2, r6, r1
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_020001c0
.L_020001bc:
	ldr	r3, [r5, #0]
	str	r3, [r7, #0]
.L_020001c0:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r2, [pc, #92]
	movs	r1, #128
	movs	r3, #192
	lsls	r1, r1, #2
	lsls	r3, r3, #18
	adds	r1, #18
	ldr	r6, [r3, #108]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_02000224
	subs	r1, #122
	movs	r3, #150
	lsls	r3, r3, #2
	adds	r5, r6, r1
	adds	r7, r2, r3
	ldr	r3, [r5, #0]
	movs	r1, #10
	lsls	r0, r3, #3
	adds	r0, r0, r3
	bl 0x0200ca58
	ldr	r3, [r7, #0]
	cmp	r3, r0
	blt.n	.L_02000224
	bl 0x0200ca80
	movs	r3, #128
	lsls	r3, r3, #8
	cmp	r0, r3
	bcs.n	.L_02000220
	movs	r0, #128
	lsls	r0, r0, #4
	movs	r1, #13
	adds	r0, #14
	bl 0x0200cd50
	movs	r1, #202
	lsls	r1, r1, #1
	adds	r2, r6, r1
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_02000224
.L_02000220:
	ldr	r3, [r5, #0]
	str	r3, [r7, #0]
.L_02000224:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r2, [pc, #92]
	movs	r1, #128
	movs	r3, #192
	lsls	r1, r1, #2
	lsls	r3, r3, #18
	adds	r1, #18
	ldr	r6, [r3, #108]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_02000288
	subs	r1, #122
	movs	r3, #150
	lsls	r3, r3, #2
	adds	r5, r6, r1
	adds	r7, r2, r3
	ldr	r3, [r5, #0]
	movs	r1, #10
	lsls	r0, r3, #3
	adds	r0, r0, r3
	bl 0x0200ca58
	ldr	r3, [r7, #0]
	cmp	r3, r0
	blt.n	.L_02000288
	bl 0x0200ca80
	movs	r3, #128
	lsls	r3, r3, #8
	cmp	r0, r3
	bcs.n	.L_02000284
	movs	r0, #226
	lsls	r0, r0, #3
	movs	r1, #53
	adds	r0, #255
	bl 0x0200cd50
	movs	r1, #202
	lsls	r1, r1, #1
	adds	r2, r6, r1
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_02000288
.L_02000284:
	ldr	r3, [r5, #0]
	str	r3, [r7, #0]
.L_02000288:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.global Func_02000290
	.thumb_func
Func_02000290:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd990
	.2byte 0x0200
	.global Func_02000298
	.thumb_func
Func_02000298:
	movs	r0, #0
	bx	lr
	.global Func_0200029c
	.thumb_func
Func_0200029c:
	ldr	r0, [pc, #0]
	bx	lr
	.4byte 0x0200e1d0
	.4byte 0x049b23c0
	.4byte 0x228f6a1b
	.4byte 0x189b0052
	.4byte 0x881b6d01
	.4byte 0x824b4a02
	.4byte 0x2001768a
	.4byte 0x00004770
	.2byte 0x0000
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #116]
	movs	r2, #2
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002dc
	movs	r1, #10
	bl 0x0200cb80
	b.n	.L_020002e4
.L_020002dc:
	adds	r0, r6, #0
	movs	r1, #7
	bl 0x0200cb80
.L_020002e4:
	adds	r3, r6, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200033a
	ldr	r3, [pc, #76]
	adds	r5, r6, #0
	str	r3, [r6, #8]
	adds	r5, #100
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	lsls	r0, r0, #3
	bl 0x0200ca88
	movs	r1, #128
	ldr	r3, [pc, #60]
	lsls	r1, r1, #11
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b0e
	movs	r4, #128
	lsls	r4, r4, #13
	adds	r0, r0, r4
	str	r0, [r6, #12]
	str	r3, [r6, #16]
	adds	r0, r4, #0
	movs	r2, #0
	ldrsh	r1, [r5, r2]
	adds	r2, r6, #0
	adds	r2, #8
	bl 0x0200ca90
	ldrh	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r3, r3, r2
	strh	r3, [r6, #6]
	movs	r2, #128
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #3
	adds	r3, r3, r2
	strh	r3, [r5, #0]
.L_0200033a:
	pop	{r5, r6, pc}
	.4byte 0x0300122c
	.4byte 0x27880000
	.4byte 0x0300021c
	.2byte 0x0000
	.2byte 0x20a4
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #55
	bl 0x0200caf8
	cmp	r0, #0
	bne.n	.L_02000380
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200caf8
	cmp	r0, #0
	bne.n	.L_02000380
	ldr	r3, [pc, #28]
	str	r3, [r5, #108]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	adds	r3, #15
	strh	r0, [r3, #0]
	adds	r3, #2
	strh	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #24]
	str	r3, [r5, #28]
.L_02000380:
	movs	r0, #0
	pop	{r5, pc}
	.4byte 0x020082c5
	.4byte 0x049b23c0
	.4byte 0x228f6a1b
	.4byte 0x189b0052
	.4byte 0x881b6d01
	.4byte 0x824b4a02
	.4byte 0x2001768a
	.4byte 0x00004770
	.2byte 0x0000
	.2byte 0x0000
	push	{lr}
	movs	r1, #0
	bl 0x0200cb70
	movs	r0, #0
	pop	{pc}
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200caf8
	cmp	r0, #0
	bne.n	.L_020003c8
	movs	r0, #1
	b.n	.L_020003f0
.L_020003c8:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200cb70
	ldr	r1, [r5, #8]
	ldr	r0, [pc, #32]
	movs	r3, #128
	lsls	r3, r3, #5
	str	r3, [r5, #52]
	str	r3, [r5, #48]
	ldr	r3, [r5, #16]
	adds	r1, r1, r0
	movs	r0, #128
	lsls	r0, r0, #12
	adds	r3, r3, r0
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	bl 0x0200cb50
	movs	r0, #0
.L_020003f0:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffe2
	.global Func_020003f8
	.thumb_func
Func_020003f8:
	.2byte 0xb500
	ldr	r3, [pc, #68]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #68
	beq.n	.L_02000426
	cmp	r3, #68
	bgt.n	.L_02000418
	cmp	r3, #64
	beq.n	.L_0200041e
	cmp	r3, #65
	beq.n	.L_02000422
	b.n	.L_0200043c
.L_02000418:
	cmp	r3, #77
	beq.n	.L_02000438
	b.n	.L_0200043c
.L_0200041e:
	ldr	r0, [pc, #36]
	b.n	.L_0200043e
.L_02000422:
	ldr	r0, [pc, #36]
	b.n	.L_0200043e
.L_02000426:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #222
	bl 0x0200caf8
	cmp	r0, #0
	bne.n	.L_02000438
	ldr	r0, [pc, #20]
	b.n	.L_0200043e
.L_02000438:
	ldr	r0, [pc, #20]
	b.n	.L_0200043e
.L_0200043c:
	ldr	r0, [pc, #20]
.L_0200043e:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0200e500
	.4byte 0x0200e598
	.4byte 0x0200e700
	.4byte 0x0200e790
	.2byte 0xe338
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r1, #128
	ldr	r3, [pc, #160]
	lsls	r1, r1, #2
	adds	r1, #18
	adds	r3, r3, r1
	movs	r2, #8
	movs	r0, #72
	strb	r2, [r3, #0]
	adds	r0, #255
	bl 0x0200caf8
	cmp	r0, #0
	beq.n	.L_0200048e
	bl 0x0200cda8
	movs	r0, #153
	movs	r1, #4
	bl 0x0200cba8
	ldr	r0, [pc, #128]
	movs	r1, #4
	bl 0x0200cba0
.L_0200048e:
	bl 0x0200ce10
	bl 0x0200cc40
	cmp	r0, #0
	beq.n	.L_0200049e
	bl 0x0200cb28
.L_0200049e:
	bl 0x0200bbf0
	movs	r0, #72
	adds	r0, #255
	bl 0x0200caf8
	cmp	r0, #0
	beq.n	.L_020004ba
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #59
	adds	r2, r5, r3
	movs	r3, #0
	b.n	.L_020004c4
.L_020004ba:
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #59
	adds	r2, r5, r1
	movs	r3, #1
.L_020004c4:
	strb	r3, [r2, #0]
	movs	r0, #72
	adds	r0, #255
	bl 0x0200caf8
	cmp	r0, #0
	beq.n	.L_020004da
	bl 0x0200cbb0
	bl 0x0200cdb0
.L_020004da:
	bl 0x0200c9bc
	movs	r3, #173
	lsls	r3, r3, #1
	movs	r1, #175
	adds	r2, r5, r3
	lsls	r1, r1, #1
	movs	r3, #0
	strh	r3, [r2, #0]
	adds	r2, r5, r1
	subs	r1, #2
	strh	r3, [r2, #0]
	adds	r2, r5, r1
	adds	r1, #8
	strh	r3, [r2, #0]
	adds	r2, r5, r1
	adds	r1, #2
	strh	r3, [r2, #0]
	adds	r2, r5, r1
	strh	r3, [r2, #0]
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x0dc2
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #96]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #18
	adds	r3, r3, r1
	movs	r2, #7
	strb	r2, [r3, #0]
	bl 0x0200bc3c
	bl 0x0200ca1c
	movs	r0, #8
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200cb70
	bl 0x0200cbd0
	movs	r6, #0
	adds	r7, r0, #0
	movs	r5, #0
	cmp	r6, r7
	bge.n	.L_02000558
.L_0200053e:
	ldr	r2, [pc, #48]
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldrb	r0, [r2, r3]
	bl 0x0200caf0
	movs	r2, #58
	ldrsh	r3, [r0, r2]
	adds	r5, #1
	adds	r6, r6, r3
	cmp	r5, r7
	blt.n	.L_0200053e
.L_02000558:
	cmp	r6, #0
	bne.n	.L_0200056c
	ldr	r3, [pc, #16]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200cbc8
.L_0200056c:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #222
	mov	r9, r3
	bl 0x0200caf8
	cmp	r0, #0
	bne.n	.L_02000596
	b.n	.L_020006c2
.L_02000596:
	ldr	r1, [pc, #308]
	movs	r3, #133
	mov	r8, r1
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r0, [r3, #0]
	mov	sl, r3
	bl 0x0200cd68
	mov	r1, sl
	adds	r5, r0, #0
	ldr	r0, [r1, #0]
	bl 0x0200cc40
	adds	r6, r0, #0
	movs	r0, #8
	bl 0x0200cc40
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	ldr	r1, [r6, #8]
	ldr	r2, [r7, #16]
	subs	r1, r1, r3
	ldr	r3, [r6, #16]
	asrs	r1, r1, #16
	subs	r3, r3, r2
	asrs	r3, r3, #16
	adds	r2, r1, #0
	muls	r2, r1
	adds	r1, r3, #0
	muls	r1, r3
	adds	r3, r1, #0
	adds	r2, r2, r3
	cmp	r5, #8
	beq.n	.L_020005e0
	cmp	r2, #255
	bgt.n	.L_020006c2
.L_020005e0:
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	add	r2, r8
	movs	r3, #7
	strb	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	add	r2, r8
	movs	r3, #1
	movs	r0, #187
	strh	r3, [r2, #0]
	lsls	r0, r0, #1
	bl 0x0200cb00
	ldr	r3, [r7, #8]
	movs	r2, #158
	lsls	r2, r2, #2
	add	r2, r8
	str	r3, [r2, #0]
	ldr	r3, [r7, #16]
	movs	r2, #159
	lsls	r2, r2, #2
	add	r2, r8
	str	r3, [r2, #0]
	movs	r2, #160
	ldrh	r3, [r7, #6]
	lsls	r2, r2, #2
	add	r2, r8
	str	r3, [r2, #0]
	movs	r0, #78
	.2byte 0xf004
	.2byte 0xfc01
	movs	r1, #6
	adds	r0, r6, #0
	bl 0x0200cb18
	movs	r0, #6
	bl 0x0200ca68
	movs	r0, #152
	bl 0x0200ce30
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200cb70
	movs	r1, #7
	adds	r0, r6, #0
	bl 0x0200cb18
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r6, #48]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r6, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	str	r3, [r6, #20]
	adds	r0, r6, #0
	bl 0x0200cb48
	ldr	r3, [r7, #16]
	movs	r2, #0
	ldr	r1, [r7, #8]
	adds	r0, r6, #0
	bl 0x0200cb50
	adds	r0, r6, #0
	bl 0x0200cb58
	movs	r1, #6
	adds	r0, r6, #0
	bl 0x0200cb18
	movs	r5, #0
	movs	r0, #6
	bl 0x0200ca68
	str	r5, [r6, #20]
	mov	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cc88
	.2byte 0xf004
	.2byte 0xfb94
	.2byte 0xf004
	.2byte 0xfb8e
	bl 0x0200cc28
	movs	r3, #174
	lsls	r3, r3, #1
	add	r3, r9
	strh	r5, [r3, #0]
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	movs	r0, #128
	strh	r5, [r3, #0]
	lsls	r0, r0, #9
	movs	r1, #30
	bl 0x0200cdf8
.L_020006c2:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #164]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #16
	bl 0x0200cc40
	mov	sl, r0
	movs	r0, #8
	.2byte 0xf004
	.2byte 0xfaa5
	movs	r3, #192
	adds	r6, r0, #0
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r5, r6, #0
	adds	r5, #8
	adds	r0, r5, #0
	str	r3, [sp, #0]
	bl 0x0200cb98
	ldr	r3, [r5, #0]
	add	r7, sp, #4
	str	r3, [r7, #0]
	movs	r1, #0
	ldr	r3, [r6, #12]
	mov	r8, r0
	str	r3, [r7, #4]
	movs	r0, #192
	ldr	r3, [r6, #16]
	adds	r2, r7, #0
	str	r3, [r7, #8]
	mov	fp, r1
	lsls	r0, r0, #13
	ldrh	r1, [r6, #6]
	bl 0x0200ca90
	ldr	r3, [r7, #0]
	movs	r2, #0
	mov	r9, r2
	cmp	r3, #0
	bge.n	.L_02000738
	ldr	r0, [pc, #80]
	adds	r3, r3, r0
.L_02000738:
	asrs	r2, r3, #21
	ldr	r3, [r7, #8]
	movs	r1, #31
	ands	r2, r1
	cmp	r3, #0
	bge.n	.L_02000748
	ldr	r0, [pc, #64]
	adds	r3, r3, r0
.L_02000748:
	asrs	r3, r3, #21
	ands	r3, r1
	lsls	r3, r3, #5
	ldr	r1, [pc, #60]
	adds	r3, r2, r3
	lsls	r3, r3, #2
	adds	r2, r3, r1
	mov	r3, r8
	cmp	r3, #12
	ble.n	.L_02000762
	movs	r0, #1
	mov	r9, r0
	b.n	.L_02000802
.L_02000762:
	mov	r3, r8
	subs	r3, #1
	cmp	r3, #2
	bhi.n	.L_0200076e
	movs	r1, #1
	mov	fp, r1
.L_0200076e:
	ldrb	r2, [r2, #3]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000790
	mov	r2, fp
	cmp	r2, #0
	beq.n	.L_02000790
	movs	r3, #1
	b.n	.L_02000800
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x001fffff
	.2byte 0x0000
	.2byte 0x0202
.L_02000790:
	ldr	r3, [r7, #0]
	movs	r5, #192
	lsls	r5, r5, #11
	adds	r3, r3, r5
	str	r3, [r7, #0]
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl 0x0200cb88
	ldr	r2, [pc, #208]
	ldr	r3, [r7, #0]
	mov	r8, r2
	mov	r1, r9
	orrs	r1, r0
	add	r3, r8
	mov	r9, r1
	str	r3, [r7, #0]
	adds	r1, r7, #0
	adds	r0, r6, #0
	.2byte 0xf004
	.2byte 0xf9e7
	mov	r3, r9
	orrs	r3, r0
	mov	r9, r3
	ldr	r3, [r7, #0]
	adds	r1, r7, #0
	adds	r3, r3, r5
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r0, r6, #0
	adds	r3, r3, r5
	str	r3, [r7, #4]
	bl 0x0200cb88
	ldr	r3, [r7, #4]
	mov	r1, r9
	orrs	r1, r0
	add	r3, r8
	mov	r9, r1
	str	r3, [r7, #4]
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl 0x0200cb88
	ldr	r3, [r7, #4]
	mov	r2, r9
	adds	r3, r3, r5
	orrs	r2, r0
	str	r3, [r7, #4]
	adds	r0, r6, #0
	adds	r1, r7, #0
	mov	r9, r2
	bl 0x0200cb88
	mov	r3, r9
	orrs	r3, r0
.L_02000800:
	mov	r9, r3
.L_02000802:
	mov	r0, r9
	cmp	r0, #0
	beq.n	.L_0200080a
	b.n	.L_0200090a
.L_0200080a:
	movs	r0, #78
	bl 0x0200ce30
	.2byte 0xf004
	.2byte 0xfa06
	movs	r0, #0
	bl 0x0200cdb8
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #10
	bl 0x0200cdf8
	ldr	r5, [pc, #80]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #18
	adds	r2, r5, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r0, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r5, r2
	mov	r1, r9
	mov	fp, r0
	movs	r0, #187
	strh	r1, [r3, #0]
	lsls	r0, r0, #1
	.2byte 0xf004
	.2byte 0xf95f
	movs	r3, #158
	lsls	r3, r3, #2
	adds	r2, r5, r3
	ldr	r3, [r6, #8]
	movs	r0, #159
	str	r3, [r2, #0]
	lsls	r0, r0, #2
	ldr	r3, [r6, #16]
	adds	r2, r5, r0
	str	r3, [r2, #0]
	movs	r1, #160
	ldrh	r3, [r6, #6]
	lsls	r1, r1, #2
	adds	r5, r5, r1
	str	r3, [r5, #0]
	mov	r7, sl
	ldr	r3, [r6, #8]
	b.n	.L_0200087c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xfff40000
	.2byte 0x0240
	.2byte 0x0200
.L_0200087c:
	movs	r5, #128
	mov	r0, sl
	adds	r7, #85
	lsls	r5, r5, #11
	ldrb	r2, [r7, #0]
	str	r3, [r0, #8]
	str	r5, [r0, #12]
	mov	r1, fp
	ldr	r3, [r6, #16]
	mov	r8, r2
	str	r3, [r0, #16]
	strb	r1, [r7, #0]
	movs	r1, #0
	.2byte 0xf004
	.2byte 0xf96b
	movs	r1, #6
	mov	r0, sl
	bl 0x0200cb18
	movs	r0, #6
	bl 0x0200ca68
	movs	r0, #152
	bl 0x0200ce30
	movs	r1, #7
	mov	r0, sl
	bl 0x0200cb18
	movs	r3, #192
	mov	r2, sl
	lsls	r3, r3, #10
	str	r3, [r2, #48]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #52]
	mov	r3, r8
	str	r5, [r2, #40]
	mov	r0, sl
	strb	r3, [r7, #0]
	.2byte 0xf004
	.2byte 0xf93c
	add	r3, sp, #4
	movs	r2, #0
	ldr	r1, [r3, #0]
	mov	r0, sl
	ldr	r3, [r3, #8]
	.2byte 0xf004
	.2byte 0xf939
	mov	r0, sl
	.2byte 0xf004
	.2byte 0xf93a
	movs	r1, #6
	mov	r0, sl
	.2byte 0xf004
	.2byte 0xf916
	movs	r0, #6
	.2byte 0xf004
	.2byte 0xf8bb
	bl 0x0200cc28
	bl 0x0200cdc8
	bl 0x0200cdc0
	ldr	r0, [sp, #0]
	movs	r1, #174
	lsls	r1, r1, #1
	adds	r3, r0, r1
	mov	r2, r9
	strh	r2, [r3, #0]
.L_0200090a:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #8
	ldr	r7, [r3, #108]
	bl 0x0200cc40
	adds	r6, r0, #0
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	ldr	r5, [pc, #92]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #18
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #8
	bne.n	.L_02000946
	bl 0x0200850c
.L_02000946:
	bl 0x0200cd98
	bl 0x0200cda0
	movs	r3, #158
	lsls	r3, r3, #2
	adds	r2, r5, r3
	ldr	r3, [r6, #8]
	movs	r1, #159
	str	r3, [r2, #0]
	lsls	r1, r1, #2
	ldr	r3, [r6, #16]
	adds	r2, r5, r1
	str	r3, [r2, #0]
	movs	r2, #160
	lsls	r2, r2, #2
	subs	r1, #6
	adds	r3, r5, r2
	movs	r2, #0
	str	r2, [r3, #0]
	movs	r0, #187
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	lsls	r0, r0, #1
	bl 0x0200cb08
	movs	r0, #123
	bl 0x0200ce30
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl 0x0200cd40
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200caf8
	cmp	r0, #0
	bne.n	.L_020009a6
	b.n	.L_02000bac
.L_020009a6:
	movs	r0, #157
	lsls	r0, r0, #4
	bl 0x0200caf8
	cmp	r0, #0
	beq.n	.L_020009b4
	b.n	.L_02000bac
.L_020009b4:
	ldr	r3, [pc, #540]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	bl 0x0200cc40
	ldr	r6, [r0, #8]
	ldr	r5, [r0, #16]
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #179
	bl 0x0200caf8
	asrs	r6, r6, #20
	asrs	r5, r5, #20
	lsls	r6, r6, #4
	lsls	r5, r5, #4
	cmp	r0, #0
	beq.n	.L_02000a8c
	movs	r0, #179
	lsls	r0, r0, #9
	adds	r0, #102
	movs	r1, #6
	bl 0x0200cdf8
	ldr	r1, [r7, #0]
	movs	r0, #5
	bl 0x0200cc98
	movs	r0, #1
	.2byte 0xf004
	.2byte 0xf834
	adds	r1, r6, #0
	adds	r2, r5, #0
	adds	r1, #24
	subs	r2, #12
	movs	r0, #5
	bl 0x0200cc78
	movs	r1, #192
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200ccf8
	movs	r1, #224
	lsls	r1, r1, #8
	ldr	r0, [r7, #0]
	bl 0x0200cd00
	.2byte 0xf004
	.2byte 0xf9c0
	ldr	r0, [pc, #428]
	bl 0x0200ccd8
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	bl 0x0200cdb0
	movs	r0, #5
	movs	r1, #2
	bl 0x0200cca0
	ldr	r0, [r7, #0]
	bl 0x0200cc40
	cmp	r0, #0
	beq.n	.L_02000a5a
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200cc60
.L_02000a5a:
	movs	r0, #5
	bl 0x0200cc80
	movs	r1, #0
	movs	r2, #0
	movs	r0, #5
	bl 0x0200cc88
	movs	r0, #1
	bl 0x0200ca68
	movs	r1, #224
	lsls	r1, r1, #5
	movs	r2, #128
	ldr	r0, [r7, #0]
	adds	r1, #144
	lsls	r2, r2, #6
	bl 0x0200cc78
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #6
	bl 0x0200cdf8
	b.n	.L_02000ba0
.L_02000a8c:
	movs	r0, #179
	lsls	r0, r0, #9
	adds	r0, #102
	movs	r1, #6
	bl 0x0200cdf8
	ldr	r1, [r7, #0]
	movs	r0, #5
	bl 0x0200cc98
	movs	r0, #1
	bl 0x0200ca68
	subs	r5, #12
	adds	r1, r6, #0
	adds	r1, #24
	movs	r0, #5
	adds	r2, r5, #0
	bl 0x0200cc78
	movs	r1, #192
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200ccf8
	movs	r1, #224
	lsls	r1, r1, #8
	ldr	r0, [r7, #0]
	.2byte 0xf004
	.2byte 0xf91b
	bl 0x0200cda8
	ldr	r0, [pc, #268]
	bl 0x0200ccd8
	movs	r0, #5
	movs	r1, #0
	.2byte 0xf004
	.2byte 0xf90a
	bl 0x0200cdb0
	movs	r1, #128
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200ccf8
	movs	r1, #129
	ldr	r0, [r7, #0]
	lsls	r1, r1, #1
	bl 0x0200cd10
	adds	r1, r6, #0
	adds	r2, r5, #0
	ldr	r0, [r7, #0]
	adds	r1, #12
	bl 0x0200cc78
	ldr	r0, [r7, #0]
	movs	r1, #0
	bl 0x0200cd00
	movs	r0, #5
	movs	r1, #4
	bl 0x0200cca0
	bl 0x0200cda8
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #5
	bl 0x0200cd08
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #5
	movs	r1, #3
	.2byte 0xf004
	.2byte 0xf8b3
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	bl 0x0200cdb0
	movs	r0, #5
	movs	r1, #2
	bl 0x0200cca0
	ldr	r0, [r7, #0]
	bl 0x0200cc40
	cmp	r0, #0
	beq.n	.L_02000b66
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	.2byte 0xf004
	.2byte 0xf87d
.L_02000b66:
	movs	r0, #5
	bl 0x0200cc80
	movs	r1, #0
	movs	r2, #0
	movs	r0, #5
	bl 0x0200cc88
	movs	r0, #1
	bl 0x0200ca68
	movs	r1, #224
	lsls	r1, r1, #5
	movs	r2, #128
	adds	r1, #144
	ldr	r0, [r7, #0]
	lsls	r2, r2, #6
	bl 0x0200cc78
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #179
	bl 0x0200cb00
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #6
	bl 0x0200cdf8
.L_02000ba0:
	movs	r0, #8
	bl 0x0200ca68
	.2byte 0xf004
	.2byte 0xf83f
	b.n	.L_02000bd0
.L_02000bac:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	.2byte 0xf004
	.2byte 0xf835
	movs	r0, #0
	bl 0x0200cdb8
	movs	r0, #123
	bl 0x0200ce30
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r5, r5, r2
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl 0x0200cd40
.L_02000bd0:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002b54
	.2byte 0x2b50
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #12
	bl 0x0200ca80
	lsls	r0, r0, #4
	lsrs	r5, r0, #16
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	ldr	r0, [pc, #268]
	movs	r1, #1
	bl 0x0200cba0
	cmp	r5, #0
	bne.n	.L_02000cba
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #79
	mov	r8, r3
	mov	sl, r2
	bl 0x0200ce10
	.2byte 0xf004
	.2byte 0xf812
	ldr	r3, [pc, #240]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200caf0
	adds	r5, r0, #0
	bl 0x0200ca80
	ldrb	r3, [r5, #15]
	movs	r2, #128
	muls	r3, r0
	lsls	r2, r2, #8
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	adds	r2, #1
	ldr	r0, [pc, #208]
	movs	r1, #1
	adds	r7, r3, r2
	bl 0x0200cba0
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r0, #128
	ldr	r3, [r6, #12]
	lsls	r0, r0, #13
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	ldrh	r1, [r6, #6]
	bl 0x0200ca90
	movs	r0, #79
	bl 0x0200cc40
	adds	r6, r0, #0
	ldr	r1, [r5, #0]
	cmp	r6, #0
	bne.n	.L_02000c90
	movs	r0, #234
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r0, #255
	bl 0x0200cb30
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02000d00
	mov	r2, sl
	lsls	r3, r2, #2
	adds	r3, #20
	mov	r2, r8
	str	r6, [r2, r3]
	b.n	.L_02000c9a
.L_02000c90:
	str	r1, [r6, #8]
	ldr	r3, [r5, #4]
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
.L_02000c9a:
	movs	r0, #79
	adds	r1, r7, #0
	bl 0x0200ce18
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #52
	add	r3, r8
	movs	r0, #128
	strh	r7, [r3, #0]
	lsls	r0, r0, #5
	mov	r3, sl
	orrs	r0, r3
	bl 0x0200cd70
	b.n	.L_02000cfc
.L_02000cba:
	cmp	r5, #1
	bne.n	.L_02000cf0
	movs	r0, #192
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200cb78
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	negs	r0, r0
	adds	r2, #102
	.2byte 0xf003
	.2byte 0xff4c
	movs	r0, #30
	bl 0x0200cc18
	ldr	r0, [pc, #48]
	movs	r1, #1
	bl 0x0200cba0
	b.n	.L_02000cfc
.L_02000cf0:
	ldr	r3, [pc, #36]
	lsrs	r0, r5, #1
	adds	r0, r0, r3
	movs	r1, #1
	bl 0x0200cba0
.L_02000cfc:
	bl 0x0200cc28
.L_02000d00:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00000de4
	.4byte 0x02000240
	.4byte 0x00000de6
	.2byte 0x0de7
	.2byte 0x0000
	push	{lr}
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	movs	r1, #204
	movs	r2, #1
	movs	r0, #19
	lsls	r1, r1, #1
	negs	r2, r2
	bl 0x0200cc38
	cmp	r0, #0
	bne.n	.L_02000d44
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #119
	bl 0x0200cb08
.L_02000d44:
	bl 0x0200cc28
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r1, #204
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200ce18
	movs	r0, #19
	bl 0x0200cc40
	ldr	r1, [r0, #80]
	movs	r2, #1
	ldrb	r3, [r1, #17]
	movs	r0, #19
	orrs	r3, r2
	strb	r3, [r1, #17]
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200cb70
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r0, #19
	ldr	r1, [pc, #20]
	ldr	r2, [pc, #20]
	bl 0x0200cc90
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #119
	bl 0x0200cb00
	pop	{pc}
	.2byte 0x0000
	.4byte 0x27b80000
	.2byte 0x0000
	.2byte 0x25b0
	push	{lr}
	movs	r1, #63
	movs	r2, #62
	bl 0x0200b654
	pop	{pc}
	push	{lr}
	movs	r1, #34
	movs	r2, #33
	bl 0x0200b654
	pop	{pc}
	push	{lr}
	movs	r1, #3
	movs	r2, #2
	bl 0x0200b654
	pop	{pc}
	.global Func_02000dbc
	.thumb_func
Func_02000dbc:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe7c0
	.2byte 0x0200
	.global Func_02000dc4
	.thumb_func
Func_02000dc4:
	push	{r5, r6, r7, lr}
	movs	r0, #10
	adds	r0, #255
	bl 0x0200caf8
	cmp	r0, #0
	bne.n	.L_02000e24
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #222
	bl 0x0200caf8
	cmp	r0, #0
	beq.n	.L_02000e24
	ldr	r5, [pc, #932]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	cmp	r6, #0
	bne.n	.L_02000e36
	movs	r3, #158
	lsls	r3, r3, #2
	adds	r7, r5, r3
	ldr	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_02000e24
	movs	r0, #8
	bl 0x0200cc40
	ldr	r3, [r7, #0]
	movs	r2, #159
	str	r3, [r0, #8]
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	adds	r2, #4
	str	r3, [r0, #16]
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	strh	r3, [r0, #6]
	adds	r3, r0, #0
	adds	r3, #100
	adds	r0, #102
	strh	r6, [r3, #0]
	strh	r6, [r0, #0]
.L_02000e24:
	ldr	r3, [pc, #864]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02000e40
.L_02000e36:
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	.2byte 0xf003
	.2byte 0xffdc
.L_02000e40:
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200cb00
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #214
.L_02000e50:
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #128
	lsls	r3, r3, #3
	str	r3, [r2, #0]
	movs	r3, #218
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #16
	str	r3, [r2, #0]
	movs	r0, #1
	bl 0x0200ca68
	movs	r0, #128
	movs	r1, #128
	lsls	r1, r1, #9
	lsls	r0, r0, #12
	.2byte 0xf003
	.2byte 0xff55
	movs	r0, #48
	adds	r0, #255
	bl 0x0200cb08
	bl 0x0200b960
	movs	r0, #129
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200caf8
	cmp	r0, #0
	beq.n	.L_02000ea0
	movs	r2, #168
	movs	r3, #144
	movs	r0, #0
	movs	r1, #240
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	bl 0x0200cb60
.L_02000ea0:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200caf8
	cmp	r0, #0
	beq.n	.L_02000eca
	movs	r3, #128
	movs	r0, #0
	movs	r1, #224
	movs	r2, #208
	lsls	r3, r3, #1
	bl 0x0200cb60
	movs	r3, #128
	movs	r0, #16
	movs	r1, #224
	movs	r2, #224
	lsls	r3, r3, #1
	bl 0x0200cb60
.L_02000eca:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #119
	bl 0x0200caf8
	cmp	r0, #0
	beq.n	.L_02000edc
	bl 0x02008d4c
.L_02000edc:
	ldr	r1, [pc, #680]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #2
	cmp	r3, #97
	bls.n	.L_02000ef0
	b.n	.L_020011ee
.L_02000ef0:
	ldr	r2, [pc, #664]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200910c
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x0200909c
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x02009080
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020090ca
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020090fa
	.4byte 0x02009100
	.4byte 0x020091ee
	.4byte 0x02009124
	.4byte 0x0200912a
	.4byte 0x02009130
	.4byte 0x02009106
	.4byte 0x020091ee
	.4byte 0x02009136
	.4byte 0x0200909c
	.4byte 0x0200909c
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020090be
	.4byte 0x020090c4
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091ee
	.4byte 0x020091a4
	.4byte 0x020091de
	.4byte 0x01002090
	.4byte 0xf0033027
	.4byte 0x2800fd37
	.4byte 0xe0aed100
	.4byte 0x01002090
	.4byte 0xf00330ab
	.4byte 0xe0a8fd33
	.4byte 0x010020a0
	.4byte 0xf0033076
	.4byte 0x2800fd29
	.4byte 0xe0a0d100
	.4byte 0x22f14b36
	.4byte 0x189b0052
	.4byte 0x5e982200
	.4byte 0xf900f000
	.4byte 0xf001e097
	.4byte 0xe094fc71
	.4byte 0xf9e4f002
	.4byte 0x2090e091
	.4byte 0x300a0100
	.4byte 0xfd12f003
	.4byte 0xd0002800
	.4byte 0x2096e089
	.4byte 0x30ff0100
	.4byte 0xfd0af003
	.4byte 0xd1002800
	.4byte 0x2090e081
	.4byte 0x300a0100
	.4byte 0xfd06f003
	.4byte 0xfdb2f000
	.4byte 0xf001e079
	.4byte 0xe076facd
	.4byte 0xfcd6f001
	.4byte 0xf003e073
	.4byte 0xe070fd7b
	.4byte 0xf0032037
	.4byte 0x2800fcf3
	.4byte 0x491ed16b
	.4byte 0xf0032009
	.4byte 0xf000fd99
	.4byte 0xe064fe89
	.4byte 0xfe6cf001
	.4byte 0xf001e061
	.4byte 0xe05efea5
	.4byte 0xffecf001
	.4byte 0x2002e05b
	.4byte 0xfe04f002
	.4byte 0xf0032007
	.4byte 0x210ffd7f
	.4byte 0xfdc4f003
	.4byte 0x49134a12
	.4byte 0xf0032007
	.4byte 0x2001fd9b
	.4byte 0xfc88f003
	.4byte 0xf0034810
	.4byte 0x2100fdbd
	.4byte 0xf0032007
	.4byte 0x2014fdc5
	.4byte 0xfd56f003
	.4byte 0x21002007
	.4byte 0xfdbef003
	.4byte 0xfe10f003
	.4byte 0xfe12f003
	.4byte 0x21094808
	.4byte 0xfddaf003
	.4byte 0x0000e033
	.4byte 0x02000240
	.4byte 0x02008ef8
	.4byte 0x0200e32c
	.4byte 0x1c840000
	.4byte 0x1c6d0000
	.4byte 0x00002b4e
	.4byte 0x00000109
	.4byte 0x00922280
	.4byte 0x20803276
	.4byte 0x0100188b
	.4byte 0x801a2201
	.4byte 0xf00330de
	.4byte 0x20fcfca3
	.4byte 0x30ff00c0
	.4byte 0xfc9ef003
	.4byte 0x30ff2064
	.4byte 0xfc9af003
	.4byte 0x30ff201e
	.4byte 0xfc96f003
	.4byte 0x213d4807
	.4byte 0xfdaef003
	.4byte 0x2064e007
	.4byte 0xf00330ff
	.4byte 0x201efc8d
	.4byte 0xf00330ff
	.2byte 0xfc89
.L_020011ee:
	movs	r0, #0
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0002
	.2byte 0x0000
	.global Func_020011f8
	.thumb_func
Func_020011f8:
	movs	r0, #0
	bx	lr
	push	{r5, lr}
	.2byte 0xf003
	.2byte 0xfdd7
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #6
	.2byte 0xf003
	.2byte 0xfdf6
	.2byte 0xf003
	.2byte 0xfdf8
	.2byte 0xf003
	.2byte 0xfdca
	movs	r1, #2
	movs	r0, #9
	.2byte 0xf003
	.2byte 0xfd4e
	movs	r0, #118
	adds	r0, #255
	.2byte 0xf003
	.2byte 0xfc6a
	ldr	r3, [pc, #132]
	ldr	r5, [pc, #136]
	subs	r5, r5, r3
	muls	r0, r5
	ldr	r3, [pc, #132]
	adds	r0, r0, r3
	.2byte 0xf003
	.2byte 0xfd52
	movs	r1, #0
	movs	r0, #9
	.2byte 0xf003
	.2byte 0xfd5a
	movs	r0, #30
	.2byte 0xf003
	.2byte 0xfceb
	movs	r0, #111
	.2byte 0xf003
	.2byte 0xfdf4
	movs	r1, #2
	movs	r0, #0
	.2byte 0xf003
	.2byte 0xfcb8
	movs	r0, #112
	adds	r0, #255
	.2byte 0xf003
	.2byte 0xfc58
	movs	r0, #114
	adds	r0, #255
	.2byte 0xf003
	.2byte 0xfc54
	.2byte 0xf003
	.2byte 0xfde2
	movs	r2, #30
	movs	r1, #4
	movs	r0, #9
	bl 0x0200ccb0
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	ldr	r3, [pc, #64]
	muls	r0, r5
	adds	r0, r0, r3
	.2byte 0xf003
	.2byte 0xfd2c
	movs	r1, #0
	movs	r0, #9
	.2byte 0xf003
	.2byte 0xfd34
	movs	r0, #112
	adds	r0, #255
	.2byte 0xf003
	.2byte 0xfc3c
	movs	r0, #114
	adds	r0, #255
	.2byte 0xf003
	.2byte 0xfc34
	.2byte 0xf003
	.2byte 0xfdc6
	movs	r0, #30
	.2byte 0xf003
	.2byte 0xfcbb
	movs	r0, #9
	movs	r1, #5
	.2byte 0xf003
	.2byte 0xfd4f
	pop	{r5, pc}
	.4byte 0x00002fd2
	.4byte 0x00003007
	.4byte 0x00002ffb
	.2byte 0x2ffc
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r5, r0, #0
	.2byte 0xf003
	.2byte 0xfcae
	movs	r0, #0
	.2byte 0xf003
	.2byte 0xfd77
	cmp	r5, #9
	beq.n	.L_020012d0
	b.n	.L_020015c0
.L_020012d0:
	movs	r6, #192
	lsls	r6, r6, #8
	adds	r3, r6, #0
	ldr	r1, [pc, #724]
	ldr	r2, [pc, #724]
	movs	r0, #20
	.2byte 0xf003
	.2byte 0xfcd8
	.2byte 0xf003
	.2byte 0xfd56
	.2byte 0xf003
	.2byte 0xfd5c
	.2byte 0xf003
	.2byte 0xfd5e
	ldr	r0, [pc, #708]
	.2byte 0xf003
	.2byte 0xfcf3
	movs	r2, #5
	movs	r0, #20
	movs	r1, #0
	.2byte 0xf003
	.2byte 0xfcf6
	.2byte 0xf003
	.2byte 0xfd58
	movs	r0, #198
	lsls	r0, r0, #9
	adds	r0, #204
	movs	r1, #16
	.2byte 0xf003
	.2byte 0xfd76
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	.2byte 0xf003
	.2byte 0xfcf8
	movs	r0, #4
	movs	r1, #0
	movs	r2, #32
	bl 0x0200cde8
	adds	r1, r6, #0
	movs	r2, #0
	movs	r0, #4
	.2byte 0xf003
	.2byte 0xfce6
	.2byte 0xf003
	.2byte 0xfd68
	movs	r0, #4
	.2byte 0xf003
	.2byte 0xfca5
	.2byte 0xf003
	.2byte 0xfd37
	adds	r3, r6, #0
	movs	r2, #0
	movs	r1, #16
	movs	r0, #23
	.2byte 0xf003
	.2byte 0xfd49
	movs	r0, #23
	.2byte 0xf003
	.2byte 0xfc86
	adds	r1, r6, #0
	movs	r0, #23
	.2byte 0xf003
	.2byte 0xfcd6
	movs	r5, #128
	movs	r0, #10
	.2byte 0xf003
	.2byte 0xfc5e
	lsls	r5, r5, #7
	movs	r2, #150
	adds	r3, r5, #0
	movs	r0, #20
	ldr	r1, [pc, #580]
	lsls	r2, r2, #22
	.2byte 0xf003
	.2byte 0xfc92
	movs	r2, #150
	adds	r3, r5, #0
	movs	r0, #21
	ldr	r1, [pc, #568]
	lsls	r2, r2, #22
	.2byte 0xf003
	.2byte 0xfc8b
	movs	r2, #150
	adds	r3, r5, #0
	movs	r0, #22
	ldr	r1, [pc, #552]
	lsls	r2, r2, #22
	.2byte 0xf003
	.2byte 0xfc84
	movs	r0, #20
	movs	r1, #0
	movs	r2, #16
	.2byte 0xf003
	.2byte 0xfd27
	movs	r1, #16
	movs	r0, #21
	negs	r1, r1
	movs	r2, #16
	.2byte 0xf003
	.2byte 0xfd21
	movs	r0, #22
	movs	r1, #16
	movs	r2, #16
	.2byte 0xf003
	.2byte 0xfd20
	movs	r1, #192
	movs	r0, #20
	lsls	r1, r1, #6
	movs	r2, #0
	.2byte 0xf003
	.2byte 0xfca2
	movs	r1, #192
	movs	r0, #21
	lsls	r1, r1, #6
	movs	r2, #0
	.2byte 0xf003
	.2byte 0xfc9c
	movs	r1, #192
	movs	r0, #22
	lsls	r1, r1, #6
	movs	r2, #0
	.2byte 0xf003
	.2byte 0xfc96
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #23
	.2byte 0xf003
	.2byte 0xfc98
	movs	r0, #20
	.2byte 0xf003
	.2byte 0xfc51
	movs	r0, #21
	.2byte 0xf003
	.2byte 0xfc4e
	movs	r0, #22
	.2byte 0xf003
	.2byte 0xfc4b
	.2byte 0xf003
	.2byte 0xfcdd
	movs	r2, #5
	movs	r0, #23
	movs	r1, #0
	.2byte 0xf003
	.2byte 0xfc78
	movs	r1, #2
	movs	r0, #20
	.2byte 0xf003
	.2byte 0xfc5c
	movs	r0, #5
	bl 0x0200cc18
	movs	r0, #20
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf003
	.2byte 0xfc6c
	movs	r2, #0
	movs	r1, #23
	movs	r0, #4
	.2byte 0xf003
	.2byte 0xfc57
	movs	r0, #30
	.2byte 0xf003
	.2byte 0xfbfc
	movs	r0, #10
	bl 0x0200cc18
	movs	r0, #21
	movs	r1, #2
	bl 0x0200ccb8
	adds	r1, r6, #0
	movs	r0, #4
	movs	r2, #0
	.2byte 0xf003
	.2byte 0xfc60
	adds	r1, r6, #0
	movs	r2, #0
	movs	r0, #23
	bl 0x0200ccf8
	movs	r0, #5
	.2byte 0xf003
	.2byte 0xfbe8
	movs	r0, #21
	movs	r1, #0
	movs	r2, #5
	bl 0x0200cce8
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #23
	.2byte 0xf003
	.2byte 0xfc55
	movs	r0, #23
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf003
	.2byte 0xfc40
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #22
	.2byte 0xf003
	.2byte 0xfc4a
	movs	r0, #22
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf003
	.2byte 0xfc35
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #23
	.2byte 0xf003
	.2byte 0xfc3f
	movs	r0, #23
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf003
	.2byte 0xfc2a
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #20
	.2byte 0xf003
	.2byte 0xfc34
	movs	r0, #20
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf003
	.2byte 0xfc1f
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #23
	.2byte 0xf003
	.2byte 0xfc29
	movs	r0, #23
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf003
	.2byte 0xfc14
	movs	r0, #21
	movs	r1, #0
	movs	r2, #5
	bl 0x0200cce8
	movs	r1, #23
	movs	r2, #0
	movs	r0, #4
	.2byte 0xf003
	.2byte 0xfbfa
	movs	r0, #30
	bl 0x0200cc18
	movs	r0, #23
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf003
	.2byte 0xfc02
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #22
	.2byte 0xf003
	.2byte 0xfc0c
	adds	r1, r6, #0
	movs	r0, #4
	movs	r2, #0
	.2byte 0xf003
	.2byte 0xfbff
	adds	r1, r6, #0
	movs	r0, #23
	movs	r2, #0
	.2byte 0xf003
	.2byte 0xfbfa
	movs	r0, #22
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf003
	.2byte 0xfbed
	movs	r2, #8
	movs	r0, #4
	movs	r1, #8
	negs	r2, r2
	.2byte 0xf003
	.2byte 0xfc63
	movs	r1, #8
	negs	r1, r1
	movs	r2, #8
	movs	r0, #23
	.2byte 0xf003
	.2byte 0xfc5d
	movs	r0, #4
	.2byte 0xf003
	.2byte 0xfbaa
	movs	r0, #23
	bl 0x0200cc80
	adds	r1, r6, #0
	movs	r0, #4
	movs	r2, #0
	.2byte 0xf003
	.2byte 0xfbde
	adds	r1, r6, #0
	movs	r0, #23
	movs	r2, #0
	.2byte 0xf003
	.2byte 0xfbd9
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #4
	.2byte 0xf003
	.2byte 0xfbdb
	movs	r1, #3
	movs	r0, #20
	.2byte 0xf003
	.2byte 0xfba7
	movs	r0, #10
	.2byte 0xf003
	.2byte 0xfb5c
	movs	r0, #20
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf003
	.2byte 0xfbbf
	ldr	r3, [pc, #76]
	movs	r1, #166
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r3, r1
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r5, [pc, #64]
	movs	r1, #73
	adds	r0, r5, #0
	.2byte 0xf003
	.2byte 0xfbeb
	adds	r0, r5, #0
	movs	r1, #74
	.2byte 0xf003
	.2byte 0xfbeb
	bl 0x0200cdb0
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	.2byte 0xf003
	.2byte 0xfbc4
	movs	r0, #102
	movs	r1, #3
	.2byte 0xf003
	.2byte 0xfbd0
	b.n	.L_02001c54
	.2byte 0x0000
	.4byte 0x27b80000
	.4byte 0x23280000
	.4byte 0x00002fa5
	.4byte 0x02000240
	.2byte 0x0002
	.2byte 0x0000
.L_020015c0:
	cmp	r5, #74
	beq.n	.L_020015c6
	b.n	.L_0200176c
.L_020015c6:
	movs	r0, #160
	lsls	r0, r0, #4
	movs	r5, #128
	lsls	r5, r5, #7
	adds	r0, #118
	.2byte 0xf003
	.2byte 0xfa9a
	movs	r0, #20
	ldr	r1, [pc, #1000]
	ldr	r2, [pc, #1000]
	adds	r3, r5, #0
	.2byte 0xf003
	.2byte 0xfb58
	movs	r0, #21
	ldr	r1, [pc, #996]
	ldr	r2, [pc, #988]
	adds	r3, r5, #0
	.2byte 0xf003
	.2byte 0xfb52
	movs	r0, #22
	ldr	r1, [pc, #988]
	ldr	r2, [pc, #976]
	adds	r3, r5, #0
	.2byte 0xf003
	.2byte 0xfb4c
	movs	r3, #224
	movs	r2, #151
	lsls	r3, r3, #8
	movs	r0, #23
	ldr	r1, [pc, #964]
	lsls	r2, r2, #22
	.2byte 0xf003
	.2byte 0xfb44
	movs	r3, #160
	lsls	r3, r3, #8
	ldr	r2, [pc, #960]
	movs	r0, #4
	ldr	r1, [pc, #940]
	.2byte 0xf003
	.2byte 0xfb3d
	movs	r0, #4
	movs	r1, #0
	.2byte 0xf003
	.2byte 0xfb7d
	movs	r0, #198
	lsls	r0, r0, #9
	adds	r0, #204
	movs	r1, #1
	bl 0x0200cdf8
	movs	r1, #19
	movs	r0, #4
	.2byte 0xf003
	.2byte 0xfb37
	movs	r0, #4
	.2byte 0xf003
	.2byte 0xfb04
	movs	r1, #0
	.2byte 0xf003
	.2byte 0xfa99
	bl 0x0200cd90
	bl 0x0200cda0
	bl 0x0200cda8
	ldr	r0, [pc, #904]
	.2byte 0xf003
	.2byte 0xfb44
	movs	r1, #2
	movs	r0, #4
	.2byte 0xf003
	.2byte 0xfb30
	movs	r0, #5
	.2byte 0xf003
	.2byte 0xfadd
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #20
	bl 0x0200cd08
	movs	r2, #5
	movs	r0, #20
	movs	r1, #0
	bl 0x0200cce8
	adds	r1, r5, #0
	movs	r0, #21
	bl 0x0200cd00
	movs	r0, #10
	.2byte 0xf003
	.2byte 0xfacb
	movs	r2, #5
	movs	r0, #21
	movs	r1, #0
	bl 0x0200cce8
	adds	r1, r5, #0
	movs	r0, #22
	bl 0x0200cd00
	movs	r0, #10
	.2byte 0xf003
	.2byte 0xfabf
	movs	r1, #2
	movs	r0, #22
	bl 0x0200ccb8
	movs	r0, #5
	.2byte 0xf003
	.2byte 0xfab8
	movs	r2, #5
	movs	r0, #22
	movs	r1, #0
	bl 0x0200cce8
	movs	r1, #3
	movs	r0, #21
	bl 0x0200cca8
	movs	r0, #10
	.2byte 0xf003
	.2byte 0xfaac
	adds	r1, r5, #0
	movs	r0, #20
	bl 0x0200cd00
	movs	r0, #10
	.2byte 0xf003
	.2byte 0xfaa5
	movs	r1, #2
	movs	r0, #20
	bl 0x0200ccb8
	movs	r0, #5
	.2byte 0xf003
	.2byte 0xfa9e
	movs	r2, #5
	movs	r0, #20
	movs	r1, #0
	bl 0x0200cce8
	movs	r1, #3
	movs	r0, #20
	bl 0x0200cca8
	movs	r0, #10
	.2byte 0xf003
	.2byte 0xfa92
	movs	r2, #5
	movs	r0, #20
	movs	r1, #0
	bl 0x0200cce8
	movs	r1, #3
	movs	r0, #21
	bl 0x0200cca8
	movs	r0, #10
	.2byte 0xf003
	.2byte 0xfa86
	movs	r0, #21
	movs	r1, #0
	movs	r2, #5
	bl 0x0200cce8
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #22
	bl 0x0200cd08
	movs	r2, #5
	movs	r0, #22
	movs	r1, #0
	bl 0x0200cce8
	movs	r1, #3
	movs	r0, #20
	bl 0x0200cca8
	movs	r0, #10
	bl 0x0200cc18
	movs	r0, #40
	bl 0x0200cc18
	ldr	r2, [pc, #660]
	movs	r1, #242
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #243
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl 0x0200cd38
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #0
	str	r2, [r3, #0]
	b.n	.L_02001c54
.L_0200176c:
	movs	r0, #160
	lsls	r0, r0, #4
	movs	r6, #128
	lsls	r6, r6, #7
	adds	r0, #118
	.2byte 0xf003
	.2byte 0xf9c7
	movs	r0, #20
	ldr	r1, [pc, #576]
	ldr	r2, [pc, #604]
	adds	r3, r6, #0
	bl 0x0200cc90
	movs	r5, #192
	movs	r0, #21
	ldr	r1, [pc, #572]
	ldr	r2, [pc, #588]
	adds	r3, r6, #0
	bl 0x0200cc90
	lsls	r5, r5, #8
	movs	r0, #22
	ldr	r1, [pc, #560]
	ldr	r2, [pc, #576]
	adds	r3, r6, #0
	bl 0x0200cc90
	movs	r0, #23
	ldr	r1, [pc, #568]
	ldr	r2, [pc, #572]
	adds	r3, r5, #0
	bl 0x0200cc90
	movs	r1, #159
	adds	r3, r5, #0
	ldr	r2, [pc, #560]
	movs	r0, #4
	lsls	r1, r1, #22
	bl 0x0200cc90
	movs	r0, #4
	movs	r1, #0
	bl 0x0200cd18
	movs	r0, #192
	movs	r1, #1
	lsls	r0, r0, #9
	bl 0x0200cdf8
	bl 0x0200cd90
	bl 0x0200cda0
	bl 0x0200cda8
	ldr	r0, [pc, #524]
	bl 0x0200ccd8
	movs	r1, #2
	movs	r0, #20
	bl 0x0200ccb8
	movs	r0, #5
	bl 0x0200cc18
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #20
	bl 0x0200cd08
	movs	r0, #20
	movs	r1, #0
	movs	r2, #5
	bl 0x0200cce8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #21
	bl 0x0200cd08
	movs	r0, #21
	movs	r1, #0
	movs	r2, #5
	bl 0x0200cce8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #22
	bl 0x0200cd08
	movs	r2, #5
	movs	r0, #22
	movs	r1, #0
	bl 0x0200cce8
	movs	r1, #2
	movs	r0, #20
	bl 0x0200ccb8
	movs	r0, #5
	bl 0x0200cc18
	movs	r2, #5
	movs	r0, #20
	movs	r1, #0
	bl 0x0200cce8
	movs	r1, #0
	movs	r0, #21
	bl 0x0200cd00
	movs	r0, #10
	bl 0x0200cc18
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #21
	bl 0x0200cd08
	movs	r2, #5
	movs	r0, #21
	movs	r1, #0
	bl 0x0200cce8
	movs	r1, #128
	movs	r0, #20
	lsls	r1, r1, #8
	bl 0x0200cd00
	movs	r1, #3
	movs	r0, #20
	bl 0x0200cca8
	movs	r0, #10
	bl 0x0200cc18
	movs	r2, #5
	movs	r0, #20
	movs	r1, #0
	bl 0x0200cce8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #22
	bl 0x0200cd00
	movs	r0, #10
	bl 0x0200cc18
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #22
	bl 0x0200cd08
	movs	r0, #22
	movs	r1, #0
	movs	r2, #5
	bl 0x0200cce8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #23
	bl 0x0200cd08
	movs	r0, #20
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200ccf8
	movs	r0, #21
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200ccf8
	movs	r0, #22
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200ccf8
	movs	r2, #5
	movs	r0, #23
	movs	r1, #0
	bl 0x0200cce8
	movs	r0, #20
	movs	r1, #3
	bl 0x0200cca0
	movs	r0, #21
	movs	r1, #3
	bl 0x0200cca0
	movs	r1, #3
	movs	r0, #22
	bl 0x0200cca8
	movs	r0, #10
	bl 0x0200cc18
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #23
	bl 0x0200cd08
	movs	r0, #23
	movs	r1, #0
	movs	r2, #5
	bl 0x0200cce8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #20
	bl 0x0200cd08
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #21
	bl 0x0200cd08
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #22
	bl 0x0200cd08
	movs	r0, #40
	bl 0x0200cc18
	movs	r0, #20
	movs	r1, #21
	movs	r2, #0
	bl 0x0200ccc8
	movs	r1, #128
	movs	r0, #22
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200ccf8
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #22
	bl 0x0200cd08
	movs	r1, #129
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #20
	bl 0x0200cd08
	adds	r1, r6, #0
	movs	r0, #20
	bl 0x0200cd00
	movs	r0, #20
	bl 0x0200cc18
	movs	r1, #0
	movs	r0, #20
	bl 0x0200cce0
	movs	r0, #4
	movs	r1, #0
	bl 0x0200cc30
	cmp	r0, #0
	bne.n	.L_020019ec
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #20
	bl 0x0200cd08
	movs	r2, #5
	movs	r0, #20
	movs	r1, #0
	bl 0x0200cce8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001a14
	.4byte 0x27b80000
	.4byte 0x25900000
	.4byte 0x27a80000
	.4byte 0x27c80000
	.4byte 0x25b00000
	.4byte 0x00002fb2
	.4byte 0x02000240
	.4byte 0x25a00000
	.4byte 0x27b00000
	.4byte 0x25d00000
	.2byte 0x2fb9
	.2byte 0x0000
.L_020019ec:
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #20
	bl 0x0200cd08
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	movs	r0, #20
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x0200cce8
.L_02001a14:
	movs	r1, #23
	movs	r2, #0
	movs	r0, #4
	bl 0x0200ccc8
	movs	r0, #40
	bl 0x0200cc18
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200ccf8
	movs	r1, #192
	movs	r0, #23
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200ccf8
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #23
	bl 0x0200cd08
	movs	r2, #5
	movs	r0, #23
	movs	r1, #0
	bl 0x0200cce8
	movs	r0, #21
	movs	r1, #2
	bl 0x0200ccb8
	movs	r1, #192
	movs	r0, #21
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200ccf8
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #22
	bl 0x0200ccf8
	movs	r0, #5
	bl 0x0200cc18
	movs	r2, #5
	movs	r0, #21
	movs	r1, #0
	bl 0x0200cce8
	movs	r0, #22
	movs	r1, #4
	bl 0x0200cca8
	movs	r2, #5
	movs	r0, #22
	movs	r1, #0
	bl 0x0200cce8
	movs	r1, #2
	movs	r0, #20
	bl 0x0200ccb8
	bl 0x02008d4c
	movs	r0, #5
	bl 0x0200cc18
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #20
	bl 0x0200cd08
	movs	r0, #20
	movs	r1, #0
	movs	r2, #5
	bl 0x0200cce8
	movs	r2, #0
	movs	r1, #21
	movs	r0, #20
	bl 0x0200ccc8
	movs	r0, #40
	bl 0x0200cc18
	movs	r0, #21
	movs	r1, #3
	bl 0x0200cca0
	movs	r1, #3
	movs	r0, #20
	bl 0x0200cca8
	movs	r0, #10
	bl 0x0200cc18
	movs	r2, #0
	movs	r1, #22
	movs	r0, #20
	bl 0x0200ccc8
	movs	r0, #40
	bl 0x0200cc18
	movs	r0, #22
	movs	r1, #3
	bl 0x0200cca0
	movs	r1, #3
	movs	r0, #20
	bl 0x0200cca8
	movs	r0, #10
	bl 0x0200cc18
	movs	r0, #20
	movs	r1, #2
	bl 0x0200ccb8
	movs	r1, #128
	movs	r0, #21
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200ccf8
	movs	r1, #128
	movs	r2, #0
	movs	r0, #22
	lsls	r1, r1, #7
	bl 0x0200ccf8
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #20
	bl 0x0200cd00
	movs	r0, #5
	bl 0x0200cc18
	movs	r0, #20
	movs	r1, #0
	movs	r2, #5
	bl 0x0200cce8
	movs	r1, #32
	movs	r0, #20
	negs	r1, r1
	movs	r2, #0
	bl 0x0200cde0
	movs	r1, #32
	movs	r0, #21
	negs	r1, r1
	movs	r2, #0
	bl 0x0200cde0
	movs	r1, #32
	movs	r0, #22
	negs	r1, r1
	movs	r2, #0
	bl 0x0200cde8
	movs	r1, #64
	movs	r2, #64
	movs	r0, #20
	negs	r1, r1
	negs	r2, r2
	bl 0x0200cde0
	movs	r1, #64
	movs	r2, #64
	movs	r0, #21
	negs	r1, r1
	negs	r2, r2
	bl 0x0200cde0
	movs	r1, #64
	movs	r2, #64
	movs	r0, #22
	negs	r1, r1
	negs	r2, r2
	bl 0x0200cde8
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cc88
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cc88
	movs	r2, #0
	movs	r1, #0
	movs	r0, #22
	bl 0x0200cc88
	movs	r0, #10
	bl 0x0200cc18
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #20
	bl 0x0200cd00
	movs	r0, #10
	bl 0x0200cc18
	movs	r0, #23
	movs	r1, #0
	bl 0x0200cd00
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	bl 0x0200cd00
	movs	r1, #3
	movs	r0, #23
	bl 0x0200cca8
	movs	r0, #10
	bl 0x0200cc18
	movs	r2, #5
	movs	r0, #23
	movs	r1, #0
	bl 0x0200cce8
	movs	r0, #4
	movs	r1, #3
	bl 0x0200cca0
	movs	r1, #3
	movs	r0, #23
	bl 0x0200cca8
	movs	r0, #10
	bl 0x0200cc18
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #23
	ldr	r1, [pc, #76]
	adds	r2, #153
	bl 0x0200cc48
	movs	r0, #23
	movs	r1, #2
	bl 0x0200cca0
	movs	r0, #4
	bl 0x0200cc40
	cmp	r0, #0
	beq.n	.L_02001c32
	movs	r2, #10
.L_02001c26:
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #23
	bl 0x0200cc60
.L_02001c32:
	movs	r0, #23
	bl 0x0200cc80
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cc88
	.2byte 0xf003
	.2byte 0xf8b5
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #15
	.2byte 0xf003
	.2byte 0xf8d4
	bl 0x0200cc28
.L_02001c54:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, lr}
	bl 0x0200cc20
	movs	r0, #0
	.2byte 0xf003
	.2byte 0xf8a8
	movs	r0, #16
	bl 0x0200cc40
	movs	r5, #2
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #17
	bl 0x0200cc40
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #18
	bl 0x0200cc40
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r5, #224
	lsls	r5, r5, #8
	adds	r3, r5, #0
	movs	r0, #16
	ldr	r1, [pc, #376]
	ldr	r2, [pc, #380]
	bl 0x0200cc90
	adds	r3, r5, #0
	movs	r0, #17
	ldr	r1, [pc, #372]
	ldr	r2, [pc, #376]
	bl 0x0200cc90
	adds	r3, r5, #0
	movs	r0, #18
	ldr	r1, [pc, #360]
	ldr	r2, [pc, #368]
	bl 0x0200cc90
	.2byte 0xf003
	.2byte 0xf86e
	movs	r1, #204
	movs	r2, #200
	lsls	r1, r1, #6
	lsls	r2, r2, #5
	movs	r0, #16
	adds	r1, #51
	adds	r2, #153
	bl 0x0200cc48
	movs	r1, #204
	movs	r2, #200
	lsls	r1, r1, #6
	lsls	r2, r2, #5
	movs	r0, #17
	adds	r1, #51
	adds	r2, #153
	bl 0x0200cc48
	movs	r1, #204
	movs	r2, #200
	lsls	r1, r1, #6
	lsls	r2, r2, #5
	movs	r0, #18
	adds	r1, #51
	adds	r2, #153
	bl 0x0200cc48
	movs	r0, #16
	movs	r1, #10
	movs	r2, #0
	.2byte 0xf003
	.2byte 0xf876
	movs	r0, #17
	movs	r1, #10
	movs	r2, #0
	.2byte 0xf003
	.2byte 0xf871
	movs	r2, #0
	movs	r0, #18
	movs	r1, #10
	.2byte 0xf003
	.2byte 0xf86c
	.2byte 0xf003
	.2byte 0xf84a
	movs	r1, #6
	ldr	r0, [pc, #272]
	.2byte 0xf003
	.2byte 0xf872
	.2byte 0xf003
	.2byte 0xf874
	.2byte 0xf003
	.2byte 0xf846
	movs	r0, #16
	.2byte 0xf002
	.2byte 0xffaf
	movs	r0, #40
	bl 0x0200cc18
	movs	r1, #3
	movs	r0, #16
	.2byte 0xf002
	.2byte 0xffb8
	ldr	r0, [pc, #240]
	.2byte 0xf002
	.2byte 0xffd1
	movs	r1, #208
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200ccf8
	movs	r1, #208
	movs	r2, #0
	movs	r0, #18
	lsls	r1, r1, #8
	bl 0x0200ccf8
	movs	r0, #16
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #3
	movs	r0, #4
	.2byte 0xf002
	.2byte 0xffa5
	movs	r0, #30
	bl 0x0200cc18
	movs	r0, #16
	movs	r1, #0
	.2byte 0xf002
	.2byte 0xffc2
	movs	r0, #16
	movs	r1, #3
	.2byte 0xf002
	.2byte 0xff96
	movs	r1, #3
	movs	r0, #4
	.2byte 0xf002
	.2byte 0xff92
	movs	r0, #60
	.2byte 0xf002
	.2byte 0xff4b
	movs	r0, #4
	movs	r1, #18
	.2byte 0xf003
	.2byte 0xf833
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #16
	adds	r1, #102
	adds	r2, #51
	.2byte 0xf002
	.2byte 0xff56
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #17
	adds	r1, #102
	adds	r2, #51
	.2byte 0xf002
	.2byte 0xff4d
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	adds	r2, #51
	movs	r0, #18
	adds	r1, #102
	.2byte 0xf002
	.2byte 0xff44
	ldr	r1, [pc, #100]
	movs	r0, #16
	.2byte 0xf002
	.2byte 0xff44
	movs	r0, #40
	bl 0x0200cc18
	ldr	r1, [pc, #92]
	movs	r0, #17
	.2byte 0xf002
	.2byte 0xff3d
	ldr	r1, [pc, #88]
	movs	r0, #18
	bl 0x0200cc50
	movs	r0, #16
	.2byte 0xf002
	.2byte 0xff3a
	movs	r0, #17
	.2byte 0xf002
	.2byte 0xff37
	movs	r0, #18
	.2byte 0xf002
	.2byte 0xff34
	.2byte 0xf002
	.2byte 0xffde
	movs	r0, #30
	.2byte 0xf002
	.2byte 0xff0f
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #6
	bl 0x0200cdf8
	.2byte 0xf002
	.2byte 0xff10
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x22d10000
	.4byte 0x16dd0000
	.4byte 0x22e10000
	.4byte 0x16ed0000
	.4byte 0x16fd0000
	.4byte 0x00013333
	.4byte 0x00002cd8
	.4byte 0x0200ce38
	.4byte 0x0200ce58
	.2byte 0xce88
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #9
	sub	sp, #4
	.2byte 0xf002
	.2byte 0xfefb
	ldr	r3, [pc, #576]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r3, r1
	adds	r6, r0, #0
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	ldr	r2, [pc, #564]
	ldr	r3, [r0, #8]
	ldr	r1, [pc, #564]
	adds	r3, r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	mov	r9, r1
	asrs	r3, r3, #1
	add	r3, r9
	str	r3, [sp, #0]
	ldr	r2, [pc, #552]
	ldr	r3, [r0, #16]
	ldr	r1, [pc, #552]
	adds	r3, r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	mov	r8, r1
	movs	r0, #183
	add	r3, r8
	lsls	r0, r0, #1
	mov	fp, r3
	.2byte 0xf002
	.2byte 0xfe37
	cmp	r0, #0
	beq.n	.L_02001e90
	b.n	.L_02002368
.L_02001e90:
	ldr	r3, [pc, #524]
	movs	r2, #64
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001ea4
	movs	r0, #118
	adds	r0, #255
	.2byte 0xf002
	.2byte 0xfe2e
.L_02001ea4:
	.2byte 0xf002
	.2byte 0xfebc
	movs	r0, #0
	bl 0x0200cdb8
	bl 0x0200cd90
	bl 0x0200cda0
	movs	r0, #0
	.2byte 0xf002
	.2byte 0xfe92
	movs	r0, #1
	.2byte 0xf002
	.2byte 0xfe8f
	movs	r0, #2
	.2byte 0xf002
	.2byte 0xfe8c
	movs	r0, #3
	.2byte 0xf002
	.2byte 0xfe89
	movs	r0, #4
	.2byte 0xf002
	.2byte 0xfe86
	movs	r0, #5
	.2byte 0xf002
	.2byte 0xfe83
	movs	r0, #6
	.2byte 0xf002
	.2byte 0xfe80
	movs	r0, #7
	.2byte 0xf002
	.2byte 0xfe7d
	movs	r0, #4
	.2byte 0xf002
	.2byte 0xfe76
	movs	r0, #5
	bl 0x0200cbd8
	movs	r0, #6
	.2byte 0xf002
	.2byte 0xfe70
	movs	r0, #1
	ldr	r7, [r5, #0]
	.2byte 0xf002
	.2byte 0xfe7c
	movs	r0, #183
	lsls	r0, r0, #1
	.2byte 0xf002
	.2byte 0xfdfc
	.2byte 0xf002
	.2byte 0xfe8a
	movs	r0, #0
	bl 0x0200cdb8
	movs	r1, #0
	movs	r2, #7
	adds	r0, r7, #0
	.2byte 0xf002
	.2byte 0xfe66
	movs	r1, #0
	movs	r2, #7
	adds	r0, r7, #0
	.2byte 0xf002
	.2byte 0xfe65
	bl 0x0200cda8
	movs	r1, #9
	movs	r2, #0
	adds	r0, r7, #0
	bl 0x0200ccc0
	movs	r0, #10
	.2byte 0xf002
	.2byte 0xfe6f
	movs	r1, #2
	adds	r1, #255
	movs	r2, #60
	adds	r0, r7, #0
	bl 0x0200cd08
	adds	r2, r6, #0
	movs	r3, #1
	adds	r2, #102
	strh	r3, [r2, #0]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r0, #9
	bl 0x0200ccc0
	movs	r0, #16
	.2byte 0xf002
	.2byte 0xfd85
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	ldr	r2, [pc, #316]
	ldr	r3, [pc, #316]
	movs	r5, #15
	subs	r3, r3, r2
	muls	r0, r3
	ldr	r3, [pc, #312]
	adds	r0, r0, r3
	bl 0x0200ccd8
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	bl 0x0200cdb0
	ldr	r0, [pc, #296]
	movs	r1, #6
	bl 0x0200cdf8
	bl 0x0200ce00
	bl 0x0200cda8
	movs	r2, #85
	adds	r2, r2, r6
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r6, #72]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	movs	r3, #0
	str	r3, [r6, #40]
	str	r3, [r6, #20]
	mov	sl, r2
	adds	r0, r6, #0
	mov	r1, r9
	movs	r2, #0
	mov	r3, r8
	.2byte 0xf002
	.2byte 0xfdc9
.L_02001fbe:
	ldr	r3, [r6, #24]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #28]
	subs	r5, #1
	.2byte 0xf002
	.2byte 0xfd49
	cmp	r5, #0
	bge.n	.L_02001fbe
	movs	r0, #9
	adds	r1, r7, #0
	movs	r2, #0
	bl 0x0200ccc0
	movs	r2, #0
	movs	r1, #9
	adds	r0, r7, #0
	bl 0x0200ccc0
	movs	r0, #16
	.2byte 0xf002
	.2byte 0xfd3a
	movs	r3, #0
	str	r3, [r6, #108]
	adds	r0, r6, #0
	movs	r1, #0
	.2byte 0xf002
	.2byte 0xfdc0
	movs	r3, #128
	lsls	r3, r3, #9
	movs	r1, #0
	str	r3, [r6, #72]
	movs	r0, #9
	bl 0x0200cce0
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cc30
	cmp	r0, #1
	bne.n	.L_020020b8
	movs	r0, #9
	movs	r1, #2
	movs	r2, #20
	bl 0x0200ccb0
	movs	r1, #2
	movs	r2, #20
	movs	r0, #9
	bl 0x0200ccb0
	movs	r0, #118
	adds	r0, #255
	.2byte 0xf002
	.2byte 0xfd61
	ldr	r2, [pc, #112]
	ldr	r3, [pc, #104]
	ldr	r1, [pc, #120]
	subs	r5, r2, r3
	muls	r0, r5
	mov	r8, r1
	add	r0, r8
	bl 0x0200ccd8
	movs	r1, #0
	movs	r0, #9
	bl 0x0200cce0
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cc30
	cmp	r0, #1
	bne.n	.L_0200206c
	movs	r0, #118
	adds	r0, #255
	.2byte 0xf002
	.2byte 0xfd4a
	muls	r0, r5
	mov	r3, r8
	adds	r3, #1
	b.n	.L_0200207a
.L_0200206c:
	movs	r0, #118
	adds	r0, #255
	.2byte 0xf002
	.2byte 0xfd42
	muls	r0, r5
	mov	r3, r8
	adds	r3, #2
.L_0200207a:
	adds	r0, r0, r3
	bl 0x0200ccd8
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	b.n	.L_020020d8
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xd8780000
	.4byte 0x27880000
	.4byte 0xdf5c0000
	.4byte 0x20a40000
	.4byte 0x03001150
	.4byte 0x00002fd2
	.4byte 0x00003007
	.4byte 0x00002fe1
	.4byte 0x00013333
	.2byte 0x2fe3
	.2byte 0x0000
.L_020020b8:
	movs	r0, #118
	adds	r0, #255
	.2byte 0xf002
	.2byte 0xfd1c
	ldr	r3, [pc, #644]
	ldr	r2, [pc, #648]
	subs	r3, r3, r2
	muls	r0, r3
	ldr	r3, [pc, #644]
	adds	r0, r0, r3
	bl 0x0200ccd8
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
.L_020020d8:
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	adds	r0, r7, #0
	bl 0x0200cd08
	movs	r0, #9
	movs	r1, #2
	movs	r2, #20
	bl 0x0200ccb0
	movs	r1, #2
	movs	r2, #20
	movs	r0, #9
	bl 0x0200ccb0
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	ldr	r2, [pc, #584]
	ldr	r3, [pc, #580]
	movs	r5, #15
	subs	r3, r3, r2
	muls	r0, r3
	ldr	r3, [pc, #584]
	adds	r0, r0, r3
	bl 0x0200ccd8
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	adds	r0, r7, #0
	bl 0x0200cd08
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	adds	r0, r7, #0
	bl 0x0200cd08
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200ccb8
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	movs	r2, #30
	movs	r0, #9
	movs	r1, #2
	bl 0x0200ccb0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	movs	r3, #0
	mov	r2, sl
	strb	r3, [r2, #0]
	movs	r2, #128
	adds	r0, r6, #0
	ldr	r1, [sp, #0]
	lsls	r2, r2, #13
	mov	r3, fp
	bl 0x0200cb50
.L_02002178:
	ldrh	r3, [r6, #6]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r3, r1
	strh	r3, [r6, #6]
	movs	r0, #1
	subs	r5, #1
	bl 0x0200ca68
	cmp	r5, #0
	bge.n	.L_02002178
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200cca0
	movs	r1, #0
	movs	r0, #9
	bl 0x0200ccf0
	movs	r2, #0
	movs	r3, #2
	mov	r1, sl
	strb	r3, [r1, #0]
	str	r2, [r6, #40]
	str	r2, [r6, #20]
	movs	r5, #7
.L_020021ac:
	ldrh	r3, [r6, #6]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	strh	r3, [r6, #6]
	movs	r0, #1
	subs	r5, #1
	bl 0x0200ca68
	cmp	r5, #0
	bge.n	.L_020021ac
	adds	r0, r7, #0
	movs	r1, #22
	bl 0x0200cca0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x0200cd08
	movs	r2, #0
	movs	r0, #9
	adds	r1, r7, #0
	bl 0x0200ccc0
	movs	r0, #9
	movs	r1, #2
	bl 0x0200ccb8
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #9
	movs	r1, #2
	movs	r2, #30
	bl 0x0200ccb0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200cce0
	movs	r6, #0
.L_0200220c:
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cc30
	adds	r5, r0, #0
	cmp	r6, #7
	bne.n	.L_02002254
	cmp	r5, #0
	bne.n	.L_02002292
	movs	r0, #9
	movs	r1, #2
	movs	r2, #20
	bl 0x0200ccb0
	movs	r1, #2
	movs	r2, #20
	movs	r0, #9
	bl 0x0200ccb0
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	ldr	r3, [pc, #268]
	ldr	r2, [pc, #268]
	subs	r3, r3, r2
	muls	r0, r3
	ldr	r3, [pc, #276]
	adds	r0, r0, r3
	bl 0x0200ccd8
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	b.n	.L_020022ce
.L_02002254:
	cmp	r5, #1
	bne.n	.L_02002292
	movs	r0, #9
	movs	r1, #2
	movs	r2, #20
	bl 0x0200ccb0
	movs	r1, #2
	movs	r2, #20
	movs	r0, #9
	bl 0x0200ccb0
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	ldr	r2, [pc, #212]
	ldr	r3, [pc, #208]
	subs	r3, r3, r2
	muls	r0, r3
	ldr	r3, [pc, #220]
	adds	r0, r6, r0
	adds	r0, r0, r3
	bl 0x0200ccd8
	movs	r0, #9
	movs	r1, #0
	bl 0x0200cce0
	adds	r6, #1
	b.n	.L_0200220c
.L_02002292:
	adds	r0, r7, #0
	movs	r1, #22
	bl 0x0200cca0
	movs	r0, #9
	movs	r1, #2
	movs	r2, #20
	bl 0x0200ccb0
	movs	r1, #4
	movs	r2, #20
	movs	r0, #9
	bl 0x0200ccb0
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	ldr	r3, [pc, #144]
	ldr	r2, [pc, #144]
	subs	r3, r3, r2
	muls	r0, r3
	ldr	r3, [pc, #160]
	adds	r0, r0, r3
	bl 0x0200ccd8
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
.L_020022ce:
	movs	r0, #52
	movs	r1, #4
	adds	r0, #255
	bl 0x0200cba8
	movs	r0, #81
	bl 0x0200ce30
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	ldr	r3, [pc, #100]
	ldr	r6, [pc, #92]
	ldr	r5, [pc, #120]
	subs	r6, r6, r3
	muls	r0, r6
	movs	r1, #3
	adds	r0, r0, r5
	bl 0x0200cba0
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	muls	r0, r6
	adds	r5, #1
	adds	r0, r0, r5
	bl 0x0200ccd8
	movs	r0, #9
	movs	r1, #2
	movs	r2, #20
	bl 0x0200ccb0
	movs	r1, #0
	movs	r0, #9
	bl 0x0200cce0
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cc30
	cmp	r0, #1
	bne.n	.L_0200232a
	b.n	.L_020024c8
.L_0200232a:
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	cmp	r0, #0
	beq.n	.L_0200233e
	movs	r0, #17
	bl 0x0200ce30
	b.n	.L_020024be
.L_0200233e:
	movs	r0, #35
	bl 0x0200ce30
	b.n	.L_020024be
	.2byte 0x0000
	.4byte 0x00003007
	.4byte 0x00002fd2
	.4byte 0x00002fe6
	.4byte 0x00002fe7
	.4byte 0x00002ff7
	.4byte 0x00002ff0
	.4byte 0x00002ff8
	.2byte 0x2ff9
	.2byte 0x0000
.L_02002368:
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	bl 0x0200cd90
	bl 0x0200cda0
	movs	r0, #9
	adds	r1, r7, #0
	bl 0x0200cc98
	movs	r3, #160
	lsls	r3, r3, #12
	str	r3, [r6, #40]
	ldr	r1, [sp, #0]
	mov	r3, fp
	movs	r2, #0
	adds	r0, r6, #0
	bl 0x0200cb50
	movs	r0, #30
	bl 0x0200cc18
	bl 0x0200cda8
	movs	r0, #9
	adds	r1, r7, #0
	movs	r2, #0
	bl 0x0200ccc0
	movs	r2, #0
	adds	r0, r7, #0
	movs	r1, #9
	bl 0x0200ccc0
	movs	r1, #22
	adds	r0, r7, #0
	bl 0x0200cca0
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	ldr	r2, [pc, #440]
	ldr	r3, [pc, #440]
	subs	r5, r2, r3
	muls	r0, r5
	ldr	r3, [pc, #440]
	adds	r0, r0, r3
	bl 0x0200ccd8
	movs	r0, #9
	movs	r1, #2
	movs	r2, #20
	bl 0x0200ccb0
	movs	r2, #20
	movs	r0, #9
	movs	r1, #2
	bl 0x0200ccb0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #9
	movs	r1, #2
	bl 0x0200ccb8
	movs	r1, #0
	movs	r0, #9
	bl 0x0200ccf0
	movs	r0, #111
	bl 0x0200ce30
	movs	r1, #2
	movs	r0, #0
	bl 0x0200cbc0
	movs	r0, #112
	adds	r0, #255
	bl 0x0200cb00
	movs	r0, #114
	adds	r0, #255
	bl 0x0200cb08
	bl 0x0200ce28
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	ldr	r3, [pc, #348]
	muls	r0, r5
	adds	r0, r0, r3
	bl 0x0200ccd8
	mov	r3, r8
	movs	r2, #0
	mov	r1, r9
	adds	r0, r6, #0
	bl 0x0200cb50
	movs	r0, #30
	bl 0x0200cc18
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	movs	r2, #0
	movs	r0, #9
	adds	r1, r7, #0
	bl 0x0200ccc0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #9
	bl 0x0200cce0
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cc30
	cmp	r0, #1
	bne.n	.L_020024c8
	adds	r0, r7, #0
	movs	r1, #22
	bl 0x0200cca0
	movs	r1, #2
	movs	r0, #9
	bl 0x0200ccb8
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	ldr	r3, [pc, #256]
	muls	r0, r5
	adds	r0, r0, r3
	bl 0x0200ccd8
	movs	r1, #0
	movs	r0, #9
	bl 0x0200cce0
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cc30
	cmp	r0, #1
	beq.n	.L_020024c8
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	ldr	r3, [sp, #0]
	movs	r0, #9
	asrs	r1, r3, #16
	mov	r3, fp
	asrs	r2, r3, #16
	bl 0x0200cc78
.L_020024be:
	bl 0x020091fc
	bl 0x0200cdb0
	b.n	.L_0200256c
.L_020024c8:
	movs	r1, #22
	adds	r0, r7, #0
	bl 0x0200cca0
	movs	r0, #118
	adds	r0, #255
	bl 0x0200caf8
	ldr	r2, [pc, #164]
	ldr	r3, [pc, #160]
	subs	r3, r3, r2
	muls	r0, r3
	ldr	r3, [pc, #172]
	adds	r0, r0, r3
	bl 0x0200ccd8
	movs	r0, #9
	movs	r1, #2
	movs	r2, #20
	bl 0x0200ccb0
	movs	r2, #20
	movs	r0, #9
	movs	r1, #2
	bl 0x0200ccb0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200cd08
	movs	r1, #0
	movs	r0, #9
	bl 0x0200ccf0
	movs	r0, #112
	adds	r0, #255
	bl 0x0200cb00
	movs	r0, #114
	adds	r0, #255
	bl 0x0200cb00
	bl 0x0200ce28
	movs	r2, #20
	movs	r0, #9
	movs	r1, #2
	bl 0x0200ccb0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	bl 0x0200cdb0
	movs	r1, #0
	movs	r2, #7
	movs	r0, #9
	bl 0x0200cdd0
	bl 0x0200cdc0
	bl 0x0200cc28
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200cb08
	movs	r0, #112
	adds	r0, #255
	bl 0x0200cb08
	movs	r0, #114
	adds	r0, #255
	bl 0x0200cb08
.L_0200256c:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00003007
	.4byte 0x00002fd2
	.4byte 0x00002ffd
	.4byte 0x00002fff
	.4byte 0x00003002
	.2byte 0x3004
	.2byte 0x0000
	push	{r5, r6, lr}
	bl 0x0200ca80
	lsls	r5, r0, #2
	adds	r5, r5, r0
	bl 0x0200ca80
	lsls	r5, r5, #3
	lsls	r3, r0, #4
	lsrs	r5, r5, #16
	movs	r2, #247
	subs	r3, r3, r0
	lsls	r2, r2, #19
	lsls	r5, r5, #16
	adds	r5, r5, r2
	lsls	r3, r3, #1
	ldr	r2, [pc, #84]
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	movs	r0, #30
	adds	r3, r3, r2
	adds	r0, #255
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200cb30
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02002608
	ldr	r5, [r6, #80]
	bl 0x0200ca80
	ldr	r3, [pc, #56]
	lsls	r0, r0, #15
	ldrb	r2, [r5, #9]
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r5, #9]
	adds	r3, r6, #0
	movs	r1, #0
	adds	r3, #85
	strb	r1, [r5, #26]
	strb	r1, [r3, #0]
	str	r0, [r6, #24]
	str	r0, [r6, #28]
	movs	r1, #1
	adds	r0, r6, #0
	bl 0x0200cb18
	ldr	r1, [pc, #16]
	adds	r0, r6, #0
	bl 0x0200cb20
.L_02002608:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x1c5c0000
	.4byte 0x00013333
	.2byte 0xebc8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	adds	r5, r1, #0
	adds	r6, r2, #0
	bl 0x0200cc40
	cmp	r0, #0
	beq.n	.L_0200263e
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	adds	r1, r1, r5
	adds	r2, r2, r6
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	adds	r0, r7, #0
	bl 0x0200cc88
.L_0200263e:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	adds	r5, r2, #0
	adds	r6, r3, #0
	movs	r2, #48
	movs	r3, #224
	mov	sl, r1
	mov	r8, r0
	bl 0x0200cb68
	adds	r1, r5, #0
	adds	r2, r6, #0
	movs	r0, #5
	bl 0x0200a618
	adds	r1, r5, #0
	adds	r2, r6, #0
	movs	r0, #8
	bl 0x0200a618
	adds	r1, r5, #0
	adds	r2, r6, #0
	movs	r0, #9
	bl 0x0200a618
	adds	r1, r5, #0
	adds	r2, r6, #0
	movs	r0, #10
	bl 0x0200a618
	movs	r3, #16
	add	r8, r3
	mov	r0, r8
	mov	r1, sl
	movs	r2, #64
	movs	r3, #224
	bl 0x0200cb68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	push	{lr}
	movs	r0, #1
	bl 0x0200bd44
	ldr	r0, [pc, #528]
	bl 0x0200ccd8
	movs	r0, #8
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #11
	bl 0x0200cb78
	movs	r0, #30
	bl 0x0200cc18
	movs	r1, #0
	movs	r0, #5
	bl 0x0200ccf0
	bl 0x0200cdb0
	movs	r0, #145
	bl 0x0200ce30
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200cd80
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200cd78
	movs	r0, #60
	bl 0x0200cd88
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #448]
	bl 0x0200ca70
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200cb78
	movs	r0, #30
	bl 0x0200cc18
	movs	r0, #224
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200cb78
	movs	r0, #30
	bl 0x0200cc18
	bl 0x0200cda8
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	bl 0x0200cb78
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	bl 0x0200cdb0
	movs	r0, #224
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200cb78
	movs	r1, #1
	ldr	r2, [pc, #352]
	movs	r3, #1
	ldr	r0, [pc, #352]
	negs	r1, r1
	bl 0x0200cd28
	movs	r0, #153
	lsls	r0, r0, #8
	movs	r1, #60
	adds	r0, #153
	bl 0x0200cdf8
	bl 0x0200cd30
	movs	r0, #30
	bl 0x0200cc18
	bl 0x0200cda8
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200ccf0
	bl 0x0200cdb0
	movs	r0, #145
	bl 0x0200ce30
	movs	r2, #0
	movs	r3, #0
	movs	r0, #0
	movs	r1, #128
	bl 0x0200a640
	movs	r0, #5
	movs	r1, #19
	bl 0x0200cca0
	movs	r0, #8
	movs	r1, #5
	bl 0x0200cca0
	movs	r1, #7
	movs	r0, #9
	bl 0x0200cca0
	movs	r0, #4
	bl 0x0200ca68
	movs	r0, #145
	bl 0x0200ce30
	movs	r1, #128
	movs	r2, #32
	movs	r3, #0
	movs	r0, #32
	bl 0x0200a640
	movs	r0, #4
	bl 0x0200ca68
	movs	r1, #128
	movs	r2, #32
	movs	r3, #0
	movs	r0, #64
	bl 0x0200a640
	movs	r0, #4
.L_020027ea:
	bl 0x0200ca68
	movs	r1, #1
	movs	r3, #1
	ldr	r0, [pc, #208]
	negs	r1, r1
	ldr	r2, [pc, #196]
	bl 0x0200cd28
	bl 0x0200cd30
	bl 0x0200cda8
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	bl 0x0200cb78
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	bl 0x0200cdb0
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200cb78
	movs	r1, #128
	movs	r2, #32
	movs	r3, #0
	movs	r0, #96
	bl 0x0200a640
	movs	r0, #20
	bl 0x0200ca68
	movs	r1, #160
	movs	r2, #32
	movs	r3, #0
	movs	r0, #0
	bl 0x0200a640
	movs	r0, #20
	bl 0x0200ca68
	movs	r0, #32
	movs	r1, #160
	movs	r2, #32
	movs	r3, #0
	bl 0x0200a640
	movs	r1, #1
	ldr	r2, [pc, #80]
	movs	r3, #1
	ldr	r0, [pc, #88]
	negs	r1, r1
	bl 0x0200cd28
	bl 0x0200cd30
	bl 0x0200cda8
	movs	r0, #8
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #5
	bl 0x0200ccf0
	movs	r0, #78
	bl 0x0200ce30
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200ce30
	bl 0x0200cd98
	bl 0x0200cda0
	movs	r0, #64
	bl 0x0200cd40
	movs	r0, #120
	bl 0x0200cc18
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0000163c
	.4byte 0x0200a595
	.4byte 0x1d810000
	.4byte 0x07c40000
	.4byte 0x08040000
	.2byte 0x0000
	.2byte 0x0864
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #196]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02002990
	movs	r0, #8
	bl 0x0200cc40
	adds	r6, r0, #0
	ldr	r1, [r6, #8]
	ldr	r0, [pc, #176]
	ldr	r3, [r6, #16]
	adds	r1, r1, r0
	ldr	r0, [pc, #176]
	ldr	r2, [r6, #12]
	adds	r3, r3, r0
	movs	r0, #30
	adds	r0, #255
	bl 0x0200cb30
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02002990
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldr	r7, [r5, #80]
	bl 0x0200ca80
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	cmp	r0, #0
	beq.n	.L_02002930
	bl 0x0200ca80
	lsls	r2, r0, #1
	adds	r2, r2, r0
	lsls	r2, r2, #4
	ldr	r3, [r5, #8]
	lsrs	r2, r2, #16
	lsls	r2, r2, #16
	asrs	r1, r2, #1
	subs	r3, r3, r1
	str	r3, [r5, #8]
	ldr	r3, [r5, #16]
	subs	r3, r3, r2
	b.n	.L_02002946
.L_02002930:
	bl 0x0200ca80
	ldr	r3, [r5, #8]
	lsls	r0, r0, #5
	lsrs	r0, r0, #16
	lsls	r0, r0, #16
	adds	r3, r3, r0
	str	r3, [r5, #8]
	ldr	r3, [r5, #16]
	asrs	r0, r0, #1
	adds	r3, r3, r0
.L_02002946:
	str	r3, [r5, #16]
	movs	r3, #0
	strb	r3, [r7, #26]
	ldrb	r1, [r7, #9]
	ldr	r3, [r6, #80]
	movs	r2, #12
	ldrb	r3, [r3, #9]
	adds	r0, r5, #0
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	adds	r1, r5, #0
	adds	r1, #35
	orrs	r3, r2
	ldrb	r2, [r1, #0]
	strb	r3, [r7, #9]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r6, #0
	adds	r3, #85
	ldrb	r3, [r3, #0]
	adds	r2, r5, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r1, #9
	bl 0x0200ccd0
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200cb18
	ldr	r1, [pc, #20]
	adds	r0, r5, #0
	bl 0x0200cb20
.L_02002990:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0xffe00000
	.4byte 0xfff00000
	.2byte 0xcf10
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #8
	bl 0x0200cc40
	adds	r6, r0, #0
	movs	r0, #60
	bl 0x0200cc18
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	movs	r0, #153
	lsls	r0, r0, #8
	adds	r0, #153
	movs	r1, #1
	bl 0x0200cdf8
	ldr	r3, [pc, #208]
	movs	r1, #1
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	movs	r0, #8
	bl 0x0200cd18
	movs	r0, #1
	bl 0x0200ca68
	ldr	r5, [pc, #192]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	movs	r1, #15
	bl 0x0200ccd0
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200cb70
	movs	r0, #8
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200cb70
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	adds	r2, #51
	movs	r0, #8
	adds	r1, #102
	bl 0x0200cc48
	adds	r6, #100
	movs	r3, #0
	strh	r3, [r6, #0]
	ldr	r1, [pc, #128]
	movs	r0, #8
	bl 0x0200cc50
	movs	r5, #192
	movs	r1, #144
	lsls	r5, r5, #18
	lsls	r1, r1, #3
	ldr	r0, [pc, #116]
	bl 0x0200ca70
	ldr	r3, [r5, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r0, #128
	adds	r3, r3, r2
	lsls	r0, r0, #9
	subs	r2, #172
	movs	r1, #1
	str	r2, [r3, #0]
	adds	r0, #3
	bl 0x0200cd78
	ldr	r2, [r5, #108]
	movs	r6, #218
	movs	r3, #32
	lsls	r6, r6, #1
	str	r3, [r2, r6]
	bl 0x0200cd90
	movs	r0, #120
	bl 0x0200cc18
	movs	r0, #179
	lsls	r0, r0, #9
	movs	r1, #150
	lsls	r1, r1, #1
	adds	r0, #102
	bl 0x0200cdf8
	movs	r0, #135
	lsls	r0, r0, #1
	bl 0x0200cc18
	ldr	r2, [r5, #108]
	movs	r3, #16
	str	r3, [r2, r6]
	ldr	r3, [pc, #20]
	movs	r2, #160
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	bl 0x0200cd98
	bl 0x0200cda0
	movs	r0, #77
	bl 0x0200cd40
	b.n	.L_02002aac
	.4byte 0x00007fff
	.4byte 0x00013333
	.4byte 0x02000240
	.4byte 0x0200ceb8
	.2byte 0xa8cd
	.2byte 0x0200
.L_02002aac:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r0, #10
	bl 0x0200cc40
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r5, r0, #0
	ldr	r7, [r3, #32]
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	movs	r2, #152
	movs	r0, #64
	movs	r1, #176
	lsls	r2, r2, #1
	movs	r3, #240
	bl 0x0200cb68
	movs	r2, #160
	movs	r0, #80
	movs	r1, #176
	lsls	r2, r2, #1
	movs	r3, #240
	bl 0x0200cb68
	ldr	r3, [pc, #708]
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	negs	r1, r1
	movs	r3, #0
	negs	r0, r0
	bl 0x0200cd28
	movs	r0, #1
	bl 0x0200ca68
	ldr	r6, [pc, #680]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r6, r6, r1
	ldr	r0, [r6, #0]
	bl 0x0200cc40
	movs	r1, #15
	bl 0x0200ccd0
	ldr	r0, [r6, #0]
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200cb70
	movs	r0, #10
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200cb70
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #10
	adds	r1, #102
	adds	r2, #51
	bl 0x0200cc48
	adds	r2, r5, #0
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #6
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	ldr	r1, [pc, #608]
	str	r3, [r5, #52]
	str	r3, [r5, #48]
	ldr	r3, [pc, #604]
	bl 0x0200cb50
	bl 0x0200cd90
	bl 0x0200cda0
	bl 0x0200cda8
	ldr	r0, [pc, #592]
	bl 0x0200ccd8
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200ccf0
	ldr	r2, [r5, #12]
	ldr	r3, [pc, #572]
	adds	r0, r5, #0
	ldr	r1, [pc, #572]
	bl 0x0200cb50
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #5
	bl 0x0200ccf0
	movs	r0, #78
	bl 0x0200ce30
	movs	r0, #162
	bl 0x0200ce30
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200ccf0
	movs	r0, #61
	bl 0x0200ce30
	bl 0x0200cdb0
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200cd20
	movs	r1, #1
	ldr	r0, [pc, #468]
	negs	r1, r1
	ldr	r2, [pc, #468]
	movs	r3, #1
	bl 0x0200cd28
	movs	r5, #31
.L_02002bfe:
	movs	r3, #143
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #128
	adds	r3, r3, r1
	strh	r3, [r2, #0]
	movs	r3, #142
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	movs	r0, #10
	adds	r3, #128
	strh	r3, [r2, #0]
	bl 0x0200cc40
	ldr	r1, [pc, #428]
	ldr	r3, [r0, #28]
	subs	r5, #1
	adds	r3, r3, r1
	str	r3, [r0, #28]
	movs	r0, #1
	bl 0x0200ca68
	cmp	r5, #0
	bge.n	.L_02002bfe
	bl 0x0200cda8
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200cb00
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	lsls	r1, r1, #10
	bl 0x0200cb78
	bl 0x0200cdb0
	movs	r0, #153
	lsls	r0, r0, #8
	adds	r0, #153
	movs	r1, #1
	bl 0x0200cdf8
	movs	r3, #143
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r1, #142
	movs	r3, #0
	strh	r3, [r2, #0]
	lsls	r1, r1, #1
	movs	r3, #224
	adds	r2, r7, r1
	lsls	r3, r3, #8
	strh	r3, [r2, #0]
	movs	r0, #1
	bl 0x0200ca68
	movs	r1, #1
	movs	r3, #0
	ldr	r0, [pc, #296]
	negs	r1, r1
	ldr	r2, [pc, #296]
	bl 0x0200cd28
	bl 0x0200cb40
	movs	r2, #238
	movs	r0, #8
	ldr	r1, [pc, #284]
	lsls	r2, r2, #21
	bl 0x0200cc88
	movs	r0, #9
	ldr	r1, [pc, #280]
	ldr	r2, [pc, #280]
	bl 0x0200cc88
	movs	r0, #4
	ldr	r1, [pc, #276]
	ldr	r2, [pc, #280]
	bl 0x0200cc88
	movs	r0, #5
	ldr	r1, [pc, #276]
	ldr	r2, [pc, #276]
	bl 0x0200cc88
	ldr	r1, [pc, #276]
	movs	r0, #6
	ldr	r2, [pc, #276]
	bl 0x0200cc88
	ldr	r5, [pc, #192]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200ccd0
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	movs	r1, #1
	bl 0x0200cb70
	movs	r0, #1
	bl 0x0200ca68
	bl 0x0200cda8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200cd10
	movs	r0, #20
	bl 0x0200cc18
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #9
	movs	r1, #4
	bl 0x0200cca8
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #6
	movs	r1, #6
	movs	r2, #15
	bl 0x0200ccb0
	movs	r0, #6
	movs	r1, #6
	movs	r2, #23
	bl 0x0200ccb0
	movs	r2, #0
	movs	r0, #6
	movs	r1, #4
	bl 0x0200ccc0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #2
	movs	r0, #8
	adds	r1, #255
	bl 0x0200cd10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #128
	movs	r2, #128
	movs	r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200cc48
	movs	r1, #224
	movs	r2, #240
	lsls	r1, r1, #4
	lsls	r2, r2, #5
	adds	r1, #24
	adds	r2, #15
	movs	r0, #8
	bl 0x0200cc70
	movs	r0, #163
	bl 0x0200ce30
	movs	r0, #0
	bl 0x0200c2e0
	movs	r0, #65
	bl 0x0200cd40
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00013333
	.4byte 0x02000240
	.4byte 0x27ff0000
	.4byte 0x1ecc0000
	.4byte 0x0000168b
	.4byte 0x1ebc0000
	.4byte 0x280f0000
	.4byte 0x283f0000
	.4byte 0x1e8c0000
	.4byte 0xfffffd00
	.4byte 0x0e470000
	.4byte 0x1dd00000
	.4byte 0x0e6a0000
	.4byte 0x0e3e0000
	.4byte 0x1dff0000
	.4byte 0x0e550000
	.4byte 0x1df00000
	.4byte 0x0e490000
	.4byte 0x1dc50000
	.4byte 0x0e770000
	.2byte 0x0000
	.2byte 0x1de3
	push	{r5, lr}
	ldr	r5, [pc, #96]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	movs	r3, #0
	str	r3, [r0, #24]
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200cb70
	bl 0x0200cd90
	bl 0x0200cda0
	movs	r0, #20
	bl 0x0200cc18
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200cd20
	movs	r1, #1
	movs	r3, #1
	ldr	r0, [pc, #40]
	negs	r1, r1
	ldr	r2, [pc, #40]
	bl 0x0200cd28
	b.n	.L_02002e50
.L_02002e4a:
	movs	r0, #1
	bl 0x0200ca68
.L_02002e50:
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02002e4a
	ldr	r0, [pc, #24]
	movs	r1, #80
	bl 0x0200cd38
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x2e720000
	.4byte 0x276a0000
	.4byte 0x03001150
	.2byte 0x0061
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #222
	bl 0x0200caf8
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02002e8c
	b.n	.L_02003102
.L_02002e8c:
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	ldr	r5, [pc, #620]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	ldr	r7, [r0, #24]
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	str	r6, [r0, #24]
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200cb70
	bl 0x0200cd90
	bl 0x0200cda0
	bl 0x0200cda8
	ldr	r0, [pc, #576]
	bl 0x0200ccd8
	movs	r0, #40
	bl 0x0200cc18
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200cd08
	movs	r0, #4
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200cd08
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200cd08
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r1, #0
	movs	r0, #7
	bl 0x0200cce0
	movs	r0, #4
	movs	r1, #0
	bl 0x0200cc30
	cmp	r0, #0
	bne.n	.L_02002f70
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200cd08
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x0200cce8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002f98
.L_02002f70:
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200cd08
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #9
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
.L_02002f98:
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200cd08
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200cd08
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200cd08
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200cd08
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200cd08
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200cd08
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200cd08
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cce8
	ldr	r5, [pc, #108]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r5, r3
	movs	r1, #0
	ldr	r0, [r6, #0]
	movs	r2, #0
	bl 0x0200cc88
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r2, r5, r3
	movs	r3, #7
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #118
	adds	r5, r5, r3
	movs	r3, #1
	strh	r3, [r5, #0]
	bl 0x0200cdb0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #222
	bl 0x0200cb00
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cc88
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cc88
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cc88
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cc88
	ldr	r0, [r6, #0]
	bl 0x0200cc40
	str	r7, [r0, #24]
	bl 0x0200cc28
.L_02003102:
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x216b
	.2byte 0x0000
	push	{lr}
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	bl 0x0200cd90
	bl 0x0200cda0
	movs	r1, #164
	movs	r2, #140
	lsls	r1, r1, #6
	lsls	r2, r2, #6
	movs	r0, #8
	adds	r1, #224
	adds	r2, #115
	bl 0x0200cc68
	bl 0x0200cd98
	bl 0x0200cda0
	movs	r0, #66
	bl 0x0200cd40
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #252
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200caf8
	cmp	r0, #0
	bne.n	.L_0200315a
	b.n	.L_0200340a
.L_0200315a:
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #120
	bl 0x0200cb00
	movs	r1, #216
	movs	r2, #132
	lsls	r1, r1, #5
	lsls	r2, r2, #6
	adds	r1, #246
	movs	r0, #8
	adds	r2, #84
	bl 0x0200cc68
	bl 0x0200850c
	movs	r0, #8
	bl 0x0200cc40
	ldr	r1, [r0, #8]
	cmp	r1, #0
	bge.n	.L_02003198
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r1, r1, r3
.L_02003198:
	asrs	r1, r1, #16
	movs	r0, #8
	mov	r8, r1
	bl 0x0200cc40
	ldr	r2, [r0, #16]
	cmp	r2, #0
	bge.n	.L_020031b0
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
.L_020031b0:
	asrs	r7, r2, #16
	movs	r6, #0
.L_020031b4:
	adds	r0, r6, #0
	bl 0x0200cc40
	adds	r5, r6, #1
	ldr	r1, [pc, #592]
	lsls	r2, r6, #2
	cmp	r0, #0
	beq.n	.L_020031ea
	str	r6, [r1, r2]
	adds	r0, r6, #0
	bl 0x0200cc40
	movs	r3, #0
	str	r3, [r0, #24]
	adds	r0, r6, #0
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200cb70
	mov	r3, r8
	adds	r0, r6, #0
	lsls	r1, r3, #16
	lsls	r2, r7, #16
	bl 0x0200cc88
	b.n	.L_020031f0
.L_020031ea:
	movs	r3, #1
	negs	r3, r3
	str	r3, [r1, r2]
.L_020031f0:
	adds	r6, r5, #0
	cmp	r6, #7
	ble.n	.L_020031b4
	movs	r0, #23
	bl 0x0200cc40
	movs	r3, #0
	str	r3, [r0, #24]
	movs	r0, #23
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200cb70
	mov	r3, r8
	lsls	r1, r3, #16
	lsls	r2, r7, #16
	movs	r0, #23
	bl 0x0200cc88
	bl 0x0200cda8
	ldr	r0, [pc, #500]
	bl 0x0200ccd8
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200ce30
	movs	r0, #2
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #129
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200cd08
	movs	r0, #1
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #3
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #0
	bl 0x0200ccf0
	movs	r0, #30
	bl 0x0200cc18
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #23
	bl 0x0200ccf0
	movs	r0, #78
	bl 0x0200ce30
	movs	r0, #50
	bl 0x0200cc18
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #5
	bl 0x0200ccf0
	movs	r0, #30
	bl 0x0200cc18
	movs	r0, #38
	bl 0x0200ce30
	movs	r1, #0
	movs	r0, #6
	bl 0x0200ccf0
	movs	r0, #20
	bl 0x0200cc18
	movs	r1, #0
	movs	r0, #0
	bl 0x0200ccf0
	movs	r0, #20
	bl 0x0200cc18
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #7
	bl 0x0200ccf0
	movs	r0, #20
	bl 0x0200cc18
	movs	r1, #0
	movs	r0, #3
	bl 0x0200ccf0
	movs	r0, #30
	bl 0x0200cc18
	movs	r1, #0
	movs	r0, #2
	bl 0x0200ccf0
	movs	r0, #20
	bl 0x0200cc18
	movs	r0, #1
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #1
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #1
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #3
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #1
	bl 0x0200ccf0
	movs	r0, #40
	bl 0x0200cc18
	movs	r0, #23
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #23
	bl 0x0200ccf0
	movs	r0, #20
	bl 0x0200cc18
	movs	r1, #0
	movs	r0, #23
	bl 0x0200ccf0
	movs	r0, #20
	bl 0x0200cc18
	movs	r1, #0
	movs	r0, #23
	bl 0x0200ccf0
	movs	r0, #20
	bl 0x0200cc18
	movs	r0, #23
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #0
	bl 0x0200ccf0
	movs	r0, #20
	bl 0x0200cc18
	movs	r0, #23
	movs	r1, #0
	bl 0x0200ccf0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #0
	movs	r0, #23
	bl 0x0200ccf0
	movs	r0, #60
	bl 0x0200cc18
	movs	r1, #0
	movs	r0, #6
	bl 0x0200ccf0
	movs	r0, #78
	bl 0x0200ce30
	movs	r1, #0
	movs	r0, #23
	bl 0x0200ccf0
	movs	r0, #40
	bl 0x0200cc18
	movs	r0, #23
	movs	r1, #0
	bl 0x0200ccf0
	bl 0x0200cdc0
	bl 0x0200cdb0
	movs	r6, #0
.L_020033cc:
	ldr	r7, [pc, #64]
	lsls	r5, r6, #2
	ldr	r0, [r7, r5]
	cmp	r0, #0
	blt.n	.L_020033f6
	bl 0x0200cc40
	movs	r1, #1
	bl 0x0200cb70
	ldr	r0, [r7, r5]
	bl 0x0200cc40
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #24]
	ldr	r0, [r7, r5]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cc88
.L_020033f6:
	adds	r6, #1
	cmp	r6, #7
	ble.n	.L_020033cc
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cc88
	bl 0x0200cc28
.L_0200340a:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200ebe8
	.2byte 0x2c61
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200ca80
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bcs.n	.L_02003438
	bl 0x0200ca80
	adds	r1, r0, #0
	lsls	r1, r1, #3
	lsrs	r1, r1, #16
	adds	r0, r5, #0
	bl 0x0200cb80
.L_02003438:
	movs	r0, #0
	pop	{r5, pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #72]
	ldr	r6, [r3, #0]
	movs	r3, #7
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_02003484
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	movs	r0, #209
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200cb30
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02003484
	ldr	r1, [pc, #40]
	bl 0x0200cb20
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200cb18
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200cb70
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #24]
	str	r3, [r5, #28]
.L_02003484:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.2byte 0xcf70
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	ldr	r5, [pc, #312]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	movs	r1, #15
	bl 0x0200ccd0
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200cb70
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	ldr	r3, [pc, #280]
	str	r3, [r0, #108]
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r0, #52]
	ldr	r0, [r5, #0]
	bl 0x0200cc40
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r0, #48]
	movs	r0, #5
	bl 0x0200ca68
	movs	r0, #192
	lsls	r0, r0, #8
	movs	r1, #60
	bl 0x0200cdf8
	bl 0x0200cd90
	movs	r1, #224
	movs	r2, #132
	lsls	r1, r1, #6
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #201
	adds	r2, #129
	bl 0x0200cc68
	movs	r1, #224
	movs	r2, #132
	lsls	r1, r1, #6
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #129
	adds	r2, #214
	bl 0x0200cc68
	movs	r1, #224
	movs	r2, #136
	lsls	r1, r1, #6
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #116
	adds	r2, #62
	bl 0x0200cc68
	movs	r1, #224
	movs	r2, #136
	lsls	r1, r1, #6
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #51
	adds	r2, #124
	bl 0x0200cc68
	movs	r1, #220
	movs	r2, #136
	lsls	r1, r1, #6
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #251
	adds	r2, #184
	bl 0x0200cc68
	movs	r1, #220
	movs	r2, #136
	lsls	r1, r1, #6
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #200
	bl 0x0200cc68
	movs	r1, #220
	movs	r2, #136
	lsls	r1, r1, #6
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #175
	adds	r2, #166
	bl 0x0200cc68
	movs	r1, #220
	movs	r2, #134
	lsls	r1, r1, #6
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #175
	adds	r2, #255
	bl 0x0200cc68
	movs	r1, #220
	movs	r2, #136
	lsls	r1, r1, #6
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r2, #68
	adds	r1, #175
	bl 0x0200cc68
	movs	r0, #160
	lsls	r0, r0, #9
	movs	r1, #120
	bl 0x0200cdf8
	movs	r1, #220
	movs	r2, #136
	lsls	r1, r1, #6
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #195
	adds	r2, #32
	bl 0x0200cc68
	movs	r1, #220
	movs	r2, #136
	lsls	r1, r1, #6
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #177
	adds	r2, #16
	bl 0x0200cc68
	bl 0x0200cd98
	bl 0x0200cda0
	bl 0x0200cc28
	movs	r0, #78
	bl 0x0200cd40
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200b43d
	.4byte 0x049b23c0
	.4byte 0x228f6a1b
	.4byte 0x189b0052
	.4byte 0x881b6d01
	.4byte 0x824b4a02
	.4byte 0x2001768a
	.4byte 0x00004770
	.2byte 0x0000
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r6, r2, #0
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #68]
	lsls	r0, r0, #2
	adds	r0, r0, r2
	ldr	r5, [r3, r0]
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	adds	r7, r1, #0
	bl 0x0200cc40
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	ldr	r2, [r0, #8]
	ldr	r3, [r5, #8]
	cmp	r2, r3
	bge.n	.L_02003636
	movs	r2, #172
	lsls	r2, r2, #1
	adds	r3, r1, r2
	strh	r7, [r3, #0]
	b.n	.L_0200363e
.L_02003636:
	movs	r2, #172
	lsls	r2, r2, #1
	adds	r3, r1, r2
	strh	r6, [r3, #0]
.L_0200363e:
	movs	r0, #123
	bl 0x0200ce30
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xfffffe70
	.4byte 0x0200ec08
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r2, #0
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #68]
	lsls	r0, r0, #2
	adds	r0, r0, r2
	ldr	r5, [r3, r0]
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	adds	r7, r1, #0
	bl 0x0200cc40
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	ldr	r2, [r0, #16]
	ldr	r3, [r5, #16]
	cmp	r2, r3
	bge.n	.L_0200368a
	movs	r2, #172
	lsls	r2, r2, #1
	adds	r3, r1, r2
	strh	r7, [r3, #0]
	b.n	.L_02003692
.L_0200368a:
	movs	r2, #172
	lsls	r2, r2, #1
.L_0200368e:
	adds	r3, r1, r2
	strh	r6, [r3, #0]
.L_02003692:
	movs	r0, #123
	bl 0x0200ce30
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xfffffe70
	.4byte 0x0200ec08
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #24
	bl 0x0200ce08
.L_020036bc:
	mov	r8, r0
	bl 0x0200ce10
	bl 0x0200cc40
	ldr	r3, [r0, #80]
	ldrh	r2, [r0, #32]
	ldr	r3, [r3, #12]
	mov	sl, r0
	adds	r0, r2, #0
	muls	r0, r3
	str	r0, [sp, #20]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r2, #230
	mov	r9, r1
	lsls	r2, r2, #1
	add	r2, r9
	ldr	r3, [r3, #32]
	ldr	r6, [r2, #0]
	movs	r2, #150
	lsls	r2, r2, #4
	adds	r3, r3, r2
	mov	r5, r8
	ldr	r3, [r3, #0]
	ldrb	r2, [r5, #0]
	movs	r5, #128
	lsls	r5, r5, #9
	ldr	r1, [r6, #8]
	ldr	r0, [r6, #16]
	ldr	r4, [pc, #456]
	cmp	r3, r5
	bgt.n	.L_0200371e
	ldr	r3, [pc, #452]
	movs	r5, #160
	lsls	r5, r5, #16
	adds	r3, r1, r3
	adds	r5, r1, r5
	ldr	r1, [pc, #448]
	str	r3, [sp, #12]
	movs	r3, #200
	lsls	r3, r3, #16
	adds	r1, r0, r1
	adds	r3, r0, r3
	str	r5, [sp, #8]
	str	r1, [sp, #4]
	str	r3, [sp, #0]
	b.n	.L_0200373a
.L_0200371e:
	ldr	r5, [pc, #432]
	movs	r3, #240
	adds	r5, r1, r5
	str	r5, [sp, #12]
	lsls	r3, r3, #16
	ldr	r5, [pc, #424]
	adds	r3, r1, r3
	movs	r1, #150
	lsls	r1, r1, #17
	adds	r5, r0, r5
	adds	r1, r0, r1
	str	r3, [sp, #8]
	str	r5, [sp, #4]
	str	r1, [sp, #0]
.L_0200373a:
	movs	r3, #0
	str	r3, [sp, #16]
	adds	r3, r2, #0
	mov	fp, r4
	cmp	r3, #0
	bne.n	.L_02003748
	b.n	.L_020038b6
.L_02003748:
	mov	r5, r8
	ldrb	r3, [r5, #1]
	cmp	r3, #15
	bne.n	.L_02003752
	b.n	.L_0200389a
.L_02003752:
	movs	r1, #4
	ldrsh	r3, [r5, r1]
	mov	r0, fp
	ldr	r6, [r0, #0]
	mov	r0, r8
	lsls	r5, r3, #16
	ldr	r1, [sp, #12]
	movs	r2, #6
	ldrsh	r3, [r0, r2]
	lsls	r7, r3, #16
	cmp	r5, r1
	bgt.n	.L_0200376c
	b.n	.L_0200388a
.L_0200376c:
	ldr	r2, [sp, #8]
	cmp	r5, r2
	blt.n	.L_02003774
	b.n	.L_0200388a
.L_02003774:
	ldr	r3, [sp, #4]
	cmp	r7, r3
	bgt.n	.L_0200377c
	b.n	.L_0200388a
.L_0200377c:
	ldr	r0, [sp, #0]
	cmp	r7, r0
	blt.n	.L_02003784
	b.n	.L_0200388a
.L_02003784:
	mov	r2, r8
	movs	r1, #10
	ldrsh	r0, [r2, r1]
	movs	r3, #1
	negs	r3, r3
	movs	r2, #1
	cmp	r0, r3
	beq.n	.L_020037b4
	movs	r3, #128
	lsls	r3, r3, #5
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_020037a6
	bl 0x0200caf8
	adds	r2, r0, #0
	b.n	.L_020037b4
.L_020037a6:
	bl 0x0200caf8
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	movs	r2, #1
	subs	r2, r2, r3
.L_020037b4:
	cmp	r2, #0
	beq.n	.L_0200388a
	cmp	r6, #0
	bne.n	.L_02003808
	mov	r2, r8
	movs	r1, #2
	ldrsh	r0, [r2, r1]
	adds	r3, r7, #0
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200cb30
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0200389a
	ldr	r1, [pc, #260]
	bl 0x0200cb20
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	mov	r5, r8
	adds	r2, #4
	movs	r3, #1
	strb	r3, [r2, #0]
	ldrh	r3, [r5, #8]
	adds	r0, r6, #0
	strh	r3, [r6, #6]
	movs	r1, #1
	bl 0x0200cb18
	ldr	r5, [r6, #80]
	movs	r1, #192
	ldr	r0, [r5, #12]
	ldr	r3, [pc, #224]
	lsls	r1, r1, #8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x60e8
	mov	r0, fp
	str	r6, [r0, #0]
.L_02003808:
	ldr	r3, [pc, #212]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #118
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200389a
	ldr	r3, [r6, #80]
	mov	r5, sl
	ldr	r0, [r3, #12]
	ldr	r2, [r6, #8]
	ldr	r3, [r5, #8]
	subs	r1, r2, r3
	cmp	r1, #0
	bge.n	.L_0200382c
	subs	r1, r3, r2
.L_0200382c:
	ldrh	r3, [r6, #32]
	muls	r3, r0
	ldr	r0, [sp, #20]
	adds	r2, r0, r3
	mov	r3, sl
	ldr	r0, [r6, #16]
	ldr	r4, [r3, #16]
	subs	r3, r0, r4
	cmp	r3, #0
	blt.n	.L_02003848
	adds	r3, r1, r3
	cmp	r3, r2
	blt.n	.L_02003850
	b.n	.L_0200389a
.L_02003848:
	subs	r3, r4, r0
	adds	r3, r1, r3
	cmp	r3, r2
	bge.n	.L_0200389a
.L_02003850:
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200caf8
	cmp	r0, #0
	bne.n	.L_0200389a
	ldr	r3, [pc, #128]
	movs	r5, #128
	lsls	r5, r5, #2
	adds	r5, #18
	adds	r3, r3, r5
	ldrb	r3, [r3, #0]
	cmp	r3, #9
	bne.n	.L_0200387c
	movs	r2, #179
	movs	r3, #128
	lsls	r2, r2, #1
	lsls	r3, r3, #6
	add	r2, r9
	adds	r3, #139
	strh	r3, [r2, #0]
	b.n	.L_0200389a
.L_0200387c:
	ldr	r2, [sp, #16]
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r2, #100
	add	r3, r9
	strh	r2, [r3, #0]
	b.n	.L_0200389a
.L_0200388a:
	cmp	r6, #0
	beq.n	.L_0200389a
	adds	r0, r6, #0
	bl 0x0200cb38
	movs	r3, #0
	mov	r0, fp
	str	r3, [r0, #0]
.L_0200389a:
	ldr	r1, [sp, #16]
	movs	r2, #4
	adds	r1, #1
	movs	r3, #20
	str	r1, [sp, #16]
	add	fp, r2
	add	r8, r3
	cmp	r1, #127
	bhi.n	.L_020038b6
	mov	r5, r8
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_020038b6
	b.n	.L_02003748
.L_020038b6:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200ec08
	.4byte 0xff600000
	.4byte 0xfed40000
	.4byte 0xff100000
	.4byte 0xfe3e0000
	.4byte 0x0200ebd4
	.4byte 0x0300021c
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #24]
	ldrh	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_02003956
	ldr	r0, [pc, #100]
	ldr	r1, [pc, #100]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02003926
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r0, #0]
	movs	r2, #252
	adds	r3, r3, r0
	lsls	r2, r2, #6
	adds	r3, #4
	adds	r2, #142
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02003926:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02003954
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	adds	r3, #4
	strh	r2, [r0, #0]
	movs	r2, #12
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #84
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02003954:
	strh	r4, [r1, #0]
.L_02003956:
	pop	{pc}
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{lr}
	movs	r1, #128
	ldr	r3, [pc, #20]
	lsls	r1, r1, #2
	ldr	r0, [pc, #20]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2190
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200ca70
	pop	{pc}
	.2byte 0x0000
	.4byte 0x03000258
	.4byte 0x0200ec08
	.2byte 0xb6a9
	.2byte 0x0200
	push	{lr}
	cmp	r0, #0
	bge.n	.L_02003992
	ldr	r2, [pc, #20]
	adds	r0, r0, r2
.L_02003992:
	asrs	r3, r0, #20
	cmp	r1, #0
	bge.n	.L_0200399c
	ldr	r2, [pc, #8]
	adds	r1, r1, r2
.L_0200399c:
	asrs	r0, r1, #20
	lsls	r0, r0, #7
	adds	r0, r3, r0
	pop	{pc}
	.2byte 0xffff
	.2byte 0x000f
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	ldr	r6, [r5, #104]
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	cmp	r0, #31
	ble.n	.L_020039c4
	movs	r0, #0
	b.n	.L_020039ec
.L_020039c4:
	lsls	r0, r0, #10
	bl 0x0200ca88
	movs	r1, #160
	ldr	r3, [pc, #32]
	lsls	r1, r1, #9
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x61a8
	str	r0, [r5, #28]
	movs	r2, #128
	ldr	r3, [r6, #8]
	lsls	r2, r2, #9
	str	r3, [r5, #8]
	ldr	r3, [r5, #12]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r3, [r6, #16]
	str	r3, [r5, #16]
.L_020039ec:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	movs	r1, #192
	ldr	r3, [pc, #128]
	lsls	r1, r1, #9
	ldr	r0, [r7, #24]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2010
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	adds	r0, #255
	bl 0x0200cb30
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02003a50
	ldr	r3, [r7, #20]
	ldr	r1, [pc, #100]
	str	r3, [r5, #20]
	ldr	r6, [r5, #80]
	bl 0x0200cb20
	adds	r3, r5, #0
	adds	r3, #85
	movs	r2, #0
	strb	r2, [r3, #0]
	adds	r3, #15
	strh	r2, [r3, #0]
	str	r7, [r5, #104]
	cmp	r6, #0
	beq.n	.L_02003a50
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200cb10
	ldr	r3, [pc, #56]
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #26]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #9]
.L_02003a50:
	movs	r0, #16
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	adds	r0, #255
	bl 0x0200cb30
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02003aa4
	ldr	r3, [r7, #20]
	ldr	r1, [pc, #24]
	str	r3, [r5, #20]
	ldr	r6, [r5, #80]
	bl 0x0200cb20
	adds	r3, r5, #0
	movs	r2, #0
	adds	r3, #85
	b.n	.L_02003a84
	.4byte 0x00000000
	.4byte 0x0300021c
	.2byte 0xcf90
	.2byte 0x0200
.L_02003a84:
	strb	r2, [r3, #0]
	adds	r3, #15
	strh	r2, [r3, #0]
	adds	r2, r5, #0
	adds	r2, #34
	movs	r3, #1
	str	r7, [r5, #104]
	strb	r3, [r2, #0]
	cmp	r6, #0
	beq.n	.L_02003aa4
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200cb10
	ldr	r3, [pc, #4]
	strb	r3, [r6, #26]
.L_02003aa4:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	movs	r1, #192
	lsls	r1, r1, #9
	ldr	r3, [pc, #120]
	ldr	r0, [r7, #24]
	sub	sp, #12
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x68bb
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	bl 0x0200ca80
	adds	r6, r0, #0
	bl 0x0200ca80
	adds	r1, r0, #0
	lsls	r0, r6, #2
	adds	r0, r0, r6
	adds	r2, r5, #0
	lsls	r0, r0, #2
	bl 0x0200ca90
	movs	r0, #128
	lsls	r0, r0, #2
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r0, #162
	bl 0x0200cb30
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02003b2c
	ldr	r3, [r7, #20]
	ldr	r6, [r5, #80]
	str	r3, [r5, #20]
	ldr	r1, [pc, #44]
	bl 0x0200cb20
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200cb10
	movs	r3, #128
	lsls	r3, r3, #2
	str	r3, [r5, #72]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	cmp	r6, #0
	beq.n	.L_02003b2c
	movs	r3, #0
	strb	r3, [r6, #26]
.L_02003b2c:
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0xcf9c
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #2
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003b50
	movs	r1, #7
	bl 0x0200cb80
	b.n	.L_02003b58
.L_02003b50:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200cb80
.L_02003b58:
	ldr	r3, [pc, #16]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02003b6a
	adds	r0, r5, #0
	bl 0x0200b9f4
.L_02003b6a:
	pop	{r5, pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	ldr	r7, [pc, #56]
	movs	r2, #1
	ldr	r3, [r7, #0]
	adds	r6, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003b98
	ldr	r5, [pc, #44]
	ldr	r2, [pc, #48]
	ldr	r3, [r5, #0]
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	bl 0x0200cb80
	ldr	r3, [r5, #0]
	movs	r2, #3
	adds	r3, #1
	ands	r3, r2
	str	r3, [r5, #0]
.L_02003b98:
	ldr	r3, [r7, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02003ba8
	adds	r0, r6, #0
	bl 0x0200baac
.L_02003ba8:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0x0200ebe4
	.2byte 0xcfc8
	.2byte 0x0200
	push	{lr}
	cmp	r1, #0
	bne.n	.L_02003bd0
	str	r1, [r0, #108]
	movs	r1, #0
	bl 0x0200cb80
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200ce30
	b.n	.L_02003be6
.L_02003bd0:
	cmp	r1, #1
	bne.n	.L_02003bda
	ldr	r3, [pc, #16]
	str	r3, [r0, #108]
	b.n	.L_02003be6
.L_02003bda:
	ldr	r3, [pc, #16]
	str	r3, [r0, #108]
	movs	r0, #36
	adds	r0, #255
	bl 0x0200ce30
.L_02003be6:
	pop	{pc}
	.4byte 0x0200bb39
	.2byte 0xbb71
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x0200ce10
	bl 0x0200cc40
	adds	r5, r0, #0
	movs	r0, #212
	bl 0x0200ce30
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bbb8
	movs	r6, #0
	b.n	.L_02003c16
.L_02003c0e:
	movs	r0, #1
	bl 0x0200ca68
	adds	r6, #1
.L_02003c16:
	cmp	r6, #19
	bgt.n	.L_02003c32
	adds	r0, r5, #0
	adds	r0, #8
	bl 0x0200cb90
	adds	r1, r0, #0
	cmp	r1, #0
	beq.n	.L_02003c0e
	movs	r0, #2
	bl 0x0200ce20
	cmp	r0, #0
	beq.n	.L_02003c0e
.L_02003c32:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200bbb8
	pop	{r5, r6, pc}
	push	{lr}
	bl 0x0200ce10
	bl 0x0200cc40
	movs	r1, #0
	bl 0x0200bbb8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	movs	r1, #252
	lsls	r0, r0, #19
	lsls	r1, r1, #6
	adds	r0, #80
	adds	r1, #65
	bl 0x0200cae8
	ldr	r3, [pc, #36]
	movs	r2, #2
	ldr	r3, [r3, #0]
	ands	r3, r2
	ldr	r2, [pc, #32]
	cmp	r3, #0
	beq.n	.L_02003c90
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #16]
	movs	r0, #128
	lsls	r0, r0, #19
	orrs	r1, r3
	adds	r0, #82
	bl 0x0200cae8
	b.n	.L_02003ca0
	.2byte 0x0000
	.4byte 0x0000000c
	.4byte 0x0300122c
	.2byte 0xee08
	.2byte 0x0200
.L_02003c90:
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #16]
	movs	r0, #128
	lsls	r0, r0, #19
	orrs	r1, r3
	adds	r0, #82
	bl 0x0200cae8
.L_02003ca0:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0010
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #140]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200cc40
	movs	r3, #10
	ldrsh	r5, [r0, r3]
	ldr	r3, [pc, #124]
	movs	r2, #18
	ldrsh	r6, [r0, r2]
	movs	r1, #3
	ldr	r0, [r3, #0]
	bl 0x0200ca60
	cmp	r0, #0
	bne.n	.L_02003d34
	bl 0x0200ca80
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	cmp	r0, #1
	beq.n	.L_02003cf8
	cmp	r0, #1
	bcc.n	.L_02003ce8
	cmp	r0, #2
	beq.n	.L_02003d08
	cmp	r0, #3
	beq.n	.L_02003d20
	b.n	.L_02003d34
.L_02003ce8:
	ldr	r3, [pc, #84]
	lsls	r0, r5, #16
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #9
	lsls	r2, r6, #16
	movs	r1, #1
	b.n	.L_02003d14
.L_02003cf8:
	ldr	r3, [pc, #68]
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r5, #16
	adds	r0, r0, r2
	movs	r1, #1
	lsls	r2, r6, #16
	b.n	.L_02003d14
.L_02003d08:
	movs	r3, #128
	lsls	r3, r3, #9
	lsls	r0, r5, #16
	lsls	r2, r6, #16
	movs	r1, #1
	adds	r0, r0, r3
.L_02003d14:
	adds	r2, r2, r3
	negs	r1, r1
	movs	r3, #1
	bl 0x0200cd28
	b.n	.L_02003d34
.L_02003d20:
	ldr	r3, [pc, #28]
	lsls	r0, r5, #16
	lsls	r2, r6, #16
	movs	r1, #1
	adds	r0, r0, r3
	adds	r2, r2, r3
	negs	r1, r1
	movs	r3, #1
	bl 0x0200cd28
.L_02003d34:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0300122c
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	sl, r0
	movs	r0, #1
	bl 0x0200ca68
	movs	r6, #192
	movs	r0, #10
	adds	r0, #255
	lsls	r6, r6, #18
	bl 0x0200cb08
	ldr	r3, [r6, #108]
	movs	r0, #214
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r5, #0
	str	r5, [r3, #0]
	ldr	r3, [pc, #276]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200cc40
	adds	r0, #84
	strb	r5, [r0, #0]
	movs	r2, #218
	ldr	r3, [r6, #108]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #16
	str	r2, [r3, #0]
	bl 0x0200cd90
	bl 0x0200cda0
	mov	r3, sl
	cmp	r3, #2
	bne.n	.L_02003db4
	movs	r0, #128
	movs	r1, #192
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200cb78
	movs	r0, #141
	bl 0x0200ce30
.L_02003db4:
	bl 0x0200cda8
	movs	r0, #128
	lsls	r0, r0, #7
	bl 0x0200ca98
	adds	r7, r0, #0
	adds	r1, r7, #0
	ldr	r0, [pc, #196]
	bl 0x0200caa8
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r1, r7, r2
	ldr	r0, [pc, #188]
	bl 0x0200caa8
	ldr	r6, [pc, #188]
	ldr	r5, [pc, #188]
	ldrh	r3, [r5, #0]
	adds	r0, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02003e0a
	lsls	r3, r2, #1
	adds	r3, r3, r2
	mov	r1, sl
	adds	r2, #1
	strh	r2, [r6, #0]
	lsls	r2, r1, #5
	ldr	r1, [pc, #168]
	lsls	r3, r3, #2
	adds	r3, r3, r6
	adds	r3, #4
	adds	r2, r2, r1
	stmia	r3!, {r2}
	ldr	r2, [pc, #160]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #24
	adds	r2, #16
	str	r2, [r3, #0]
.L_02003e0a:
	strh	r0, [r5, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02003e30
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r6
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r7}
	strh	r2, [r6, #0]
	ldr	r2, [pc, #120]
	stmia	r3!, {r2}
	ldr	r2, [pc, #120]
	str	r2, [r3, #0]
.L_02003e30:
	strh	r1, [r5, #0]
	ldr	r0, [pc, #120]
	movs	r1, #144
	lsls	r1, r1, #3
	bl 0x0200ca70
	bl 0x0200cc20
	movs	r0, #0
	bl 0x0200cdb8
	movs	r0, #246
	bl 0x0200ce30
	ldr	r2, [pc, #96]
	ldr	r3, [pc, #52]
	mov	r8, r2
	mov	r0, r8
	strh	r3, [r0, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02003ebc
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #210
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #6
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #56]
	stmia	r3!, {r2}
	ldr	r2, [pc, #56]
	str	r2, [r3, #0]
	b.n	.L_02003ebc
	.2byte 0x0000
	.4byte 0x00000e00
	.4byte 0x02000240
	.4byte 0x0200d312
	.4byte 0x0200d058
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x0200cfd8
	.4byte 0x050001c0
	.4byte 0x06001000
	.4byte 0x84000400
	.4byte 0x0200bc51
	.4byte 0x0200ee08
	.4byte 0x06002000
	.2byte 0x0140
	.2byte 0x8400
.L_02003ebc:
	strh	r1, [r5, #0]
	movs	r0, #2
	bl 0x0200cc18
	ldr	r3, [pc, #48]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02003f04
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #186
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #6
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #12]
	stmia	r3!, {r2}
	ldr	r2, [pc, #12]
	str	r2, [r3, #0]
	b.n	.L_02003f04
	.2byte 0x0000
	.4byte 0x00000d00
	.4byte 0x06002000
	.2byte 0x0140
	.2byte 0x8400
.L_02003f04:
	strh	r1, [r5, #0]
	movs	r0, #2
	bl 0x0200cc18
	ldr	r3, [pc, #48]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02003f4c
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #162
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #6
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #12]
	stmia	r3!, {r2}
	ldr	r2, [pc, #12]
	str	r2, [r3, #0]
	b.n	.L_02003f4c
	.2byte 0x0000
	.4byte 0x00000c00
	.4byte 0x06002000
	.2byte 0x0140
	.2byte 0x8400
.L_02003f4c:
	strh	r1, [r5, #0]
	movs	r0, #2
	bl 0x0200cc18
	movs	r1, #176
	lsls	r1, r1, #4
	mov	r2, r8
	strh	r1, [r2, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02003f86
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #138
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #6
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #192]
	stmia	r3!, {r2}
	ldr	r2, [pc, #192]
	str	r2, [r3, #0]
.L_02003f86:
	strh	r1, [r5, #0]
	movs	r0, #2
	bl 0x0200cc18
	movs	r1, #160
	lsls	r1, r1, #4
	mov	fp, r1
	mov	r2, fp
	mov	r3, r8
	strh	r2, [r3, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02003fc4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #228
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #5
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #128]
	stmia	r3!, {r2}
	ldr	r2, [pc, #128]
	str	r2, [r3, #0]
.L_02003fc4:
	strh	r1, [r5, #0]
	movs	r0, #2
	bl 0x0200cc18
	movs	r1, #144
	lsls	r1, r1, #4
	mov	r9, r1
	mov	r2, r9
	mov	r3, r8
	strh	r2, [r3, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02004002
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #180
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #5
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #68]
	stmia	r3!, {r2}
	ldr	r2, [pc, #68]
	str	r2, [r3, #0]
.L_02004002:
	strh	r1, [r5, #0]
	movs	r0, #2
	bl 0x0200cc18
	ldr	r3, [pc, #48]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02004048
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #132
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #5
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #12]
	stmia	r3!, {r2}
	ldr	r2, [pc, #12]
	str	r2, [r3, #0]
	b.n	.L_02004048
	.4byte 0x00000800
	.4byte 0x06002000
	.2byte 0x0140
	.2byte 0x8400
.L_02004048:
	strh	r1, [r5, #0]
	movs	r0, #140
	bl 0x0200cc18
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_0200407a
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #180
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #5
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #340]
	stmia	r3!, {r2}
	ldr	r2, [pc, #340]
	str	r2, [r3, #0]
.L_0200407a:
	strh	r1, [r5, #0]
	movs	r0, #4
	bl 0x0200cc18
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_020040ac
	lsls	r3, r2, #1
.L_02004090:
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #228
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #5
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #288]
	stmia	r3!, {r2}
	ldr	r2, [pc, #288]
	str	r2, [r3, #0]
.L_020040ac:
	strh	r1, [r5, #0]
	movs	r0, #4
	bl 0x0200cc18
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_020040de
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #138
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #6
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #240]
	stmia	r3!, {r2}
	ldr	r2, [pc, #240]
	str	r2, [r3, #0]
.L_020040de:
	strh	r1, [r5, #0]
	movs	r0, #4
	bl 0x0200cc18
	mov	r1, r9
	mov	r2, r8
	strh	r1, [r2, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02004116
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #162
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #6
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #184]
	stmia	r3!, {r2}
	ldr	r2, [pc, #184]
	str	r2, [r3, #0]
.L_02004116:
	strh	r1, [r5, #0]
	movs	r0, #4
	bl 0x0200cc18
	mov	r1, fp
	mov	r2, r8
	strh	r1, [r2, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_0200414e
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #186
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #6
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #128]
	stmia	r3!, {r2}
	ldr	r2, [pc, #128]
	str	r2, [r3, #0]
.L_0200414e:
	strh	r1, [r5, #0]
	movs	r0, #4
	bl 0x0200cc18
	movs	r1, #176
	lsls	r1, r1, #4
.L_0200415a:
	mov	r2, r8
	strh	r1, [r2, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02004188
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #210
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #6
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #68]
	stmia	r3!, {r2}
	ldr	r2, [pc, #68]
	str	r2, [r3, #0]
.L_02004188:
	strh	r1, [r5, #0]
	movs	r0, #4
	bl 0x0200cc18
	ldr	r3, [pc, #48]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_020041d0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #234
	adds	r2, #1
	adds	r3, r3, r6
	lsls	r0, r0, #6
	adds	r3, #4
	strh	r2, [r6, #0]
	adds	r2, r7, r0
	stmia	r3!, {r2}
	ldr	r2, [pc, #12]
	stmia	r3!, {r2}
	ldr	r2, [pc, #12]
	str	r2, [r3, #0]
	b.n	.L_020041d0
	.2byte 0x0000
	.4byte 0x00000c00
	.4byte 0x06002000
	.2byte 0x0140
	.2byte 0x8400
.L_020041d0:
	strh	r1, [r5, #0]
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L_0200425c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200cb78
	movs	r0, #141
	bl 0x0200ce30
	ldr	r3, [pc, #56]
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r0, #4
	bl 0x0200cc18
	ldr	r3, [pc, #48]
	mov	r0, r8
	strh	r3, [r0, #0]
	movs	r0, #4
	bl 0x0200cc18
	ldr	r3, [pc, #40]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r0, #4
	bl 0x0200cc18
	ldr	r3, [pc, #32]
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r0, #45
	bl 0x0200cc18
	ldr	r0, [pc, #24]
	bl 0x0200ca78
	b.n	.L_0200423c
	.2byte 0x0000
	.4byte 0x00000d00
	.4byte 0x00000e00
	.4byte 0x00000f00
	.4byte 0x00001000
	.2byte 0xbc51
	.2byte 0x0200
.L_0200423c:
	bl 0x0200cac8
	movs	r0, #128
	mov	r3, r8
	lsls	r0, r0, #19
	ldrh	r1, [r3, #0]
	adds	r0, #82
	bl 0x0200cae8
	movs	r0, #128
	lsls	r0, r0, #19
	adds	r0, #80
	movs	r1, #0
	bl 0x0200cae8
	b.n	.L_020042c6
.L_0200425c:
	ldr	r3, [pc, #52]
	mov	r0, r8
	strh	r3, [r0, #0]
	movs	r0, #4
	bl 0x0200cc18
	ldr	r3, [pc, #44]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r0, #4
	bl 0x0200cc18
	ldr	r3, [pc, #36]
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r0, #4
	bl 0x0200cc18
	ldr	r3, [pc, #28]
	mov	r0, r8
	strh	r3, [r0, #0]
	movs	r0, #45
	bl 0x0200cc18
	ldr	r0, [pc, #20]
	bl 0x0200ca78
	b.n	.L_020042a8
	.4byte 0x00000d00
	.4byte 0x00000e00
	.4byte 0x00000f00
	.4byte 0x00001000
	.2byte 0xbc51
	.2byte 0x0200
.L_020042a8:
	bl 0x0200cac8
	movs	r0, #128
	mov	r2, r8
	lsls	r0, r0, #19
	ldrh	r1, [r2, #0]
	adds	r0, #82
	bl 0x0200cae8
	movs	r0, #128
	lsls	r0, r0, #19
	adds	r0, #80
	movs	r1, #0
	bl 0x0200cae8
.L_020042c6:
	adds	r0, r7, #0
	bl 0x0200caa0
	movs	r0, #2
	adds	r0, #255
	bl 0x0200cb00
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #128
	lsls	r0, r0, #7
	bl 0x0200ca98
	adds	r7, r0, #0
	adds	r1, r7, #0
	ldr	r0, [pc, #480]
	bl 0x0200caa8
	movs	r1, #128
	lsls	r1, r1, #6
	adds	r1, r1, r7
	ldr	r0, [pc, #472]
	mov	r8, r1
	bl 0x0200caa8
	ldr	r6, [pc, #468]
	ldr	r5, [pc, #468]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02004334
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	ldr	r2, [pc, #448]
	adds	r3, r3, r6
	adds	r3, #4
	stmia	r3!, {r2}
	ldr	r2, [pc, #444]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #24
	adds	r2, #16
	str	r2, [r3, #0]
.L_02004334:
	strh	r1, [r5, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_0200435c
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r6
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r7}
	strh	r2, [r6, #0]
	movs	r2, #192
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	ldr	r2, [pc, #400]
	str	r2, [r3, #0]
.L_0200435c:
	strh	r1, [r5, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_02004384
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r6
	adds	r3, #4
	strh	r2, [r6, #0]
	mov	r2, r8
	stmia	r3!, {r2}
	ldr	r2, [pc, #368]
	stmia	r3!, {r2}
	ldr	r2, [pc, #368]
	str	r2, [r3, #0]
.L_02004384:
	strh	r1, [r5, #0]
	movs	r0, #128
	movs	r1, #252
	lsls	r0, r0, #19
	lsls	r1, r1, #6
	adds	r0, #80
	adds	r1, #65
	bl 0x0200cae8
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #19
	lsls	r1, r1, #5
	adds	r0, #82
	adds	r1, #16
	bl 0x0200cae8
	ldr	r2, [pc, #336]
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r3, #176
	lsls	r3, r3, #1
	strh	r3, [r2, #2]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_020043e0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r6
	strh	r2, [r6, #0]
	movs	r2, #194
	adds	r3, #4
	lsls	r2, r2, #8
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #8
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_020043e0:
	strh	r1, [r5, #0]
	movs	r7, #180
	movs	r6, #153
	lsls	r7, r7, #6
	lsls	r6, r6, #1
	adds	r7, #96
	adds	r6, #255
	movs	r5, #95
.L_020043f0:
	lsls	r3, r6, #4
	cmp	r3, #0
.L_020043f4:
	bge.n	.L_020043f8
	adds	r3, #127
.L_020043f8:
	asrs	r2, r3, #7
	negs	r3, r7
	cmp	r3, #0
	bge.n	.L_02004402
.L_02004400:
	adds	r3, #127
.L_02004402:
	movs	r1, #176
	lsls	r1, r1, #1
	asrs	r3, r3, #7
	adds	r3, r3, r1
	ldr	r1, [pc, #236]
.L_0200440c:
	movs	r0, #1
	mov	r8, r1
	strh	r2, [r1, #0]
	mov	r2, r8
	strh	r3, [r2, #2]
	bl 0x0200ca68
	movs	r3, #176
	lsls	r3, r3, #1
	subs	r5, #1
	adds	r7, r7, r3
	adds	r6, #17
	cmp	r5, #0
	bge.n	.L_020043f0
	movs	r0, #164
	movs	r5, #192
	bl 0x0200ce30
	lsls	r5, r5, #18
	movs	r0, #120
	bl 0x0200ca68
	ldr	r3, [r5, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #128
	movs	r0, #254
	adds	r3, r3, r1
	lsls	r2, r2, #1
	lsls	r0, r0, #7
	str	r2, [r3, #0]
	movs	r1, #0
	adds	r0, #255
	bl 0x0200cd78
	movs	r0, #1
	bl 0x0200cd88
	movs	r0, #128
	lsls	r0, r0, #19
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r0, #82
	bl 0x0200cae8
	movs	r0, #1
	bl 0x0200ca68
	movs	r1, #0
	movs	r0, #0
	bl 0x0200cd78
	movs	r0, #60
	bl 0x0200cd88
	movs	r0, #78
	bl 0x0200ce30
	movs	r0, #60
	bl 0x0200ca68
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200ce30
	movs	r0, #120
	bl 0x0200ca68
	ldr	r2, [r5, #24]
	movs	r3, #1
	strh	r3, [r2, #4]
	movs	r0, #1
	bl 0x0200ca68
	bl 0x0200cad8
	bl 0x0200cac8
	ldr	r3, [pc, #80]
	movs	r1, #147
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r2, r3, r1
	ldrb	r0, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #38
	adds	r3, r3, r2
	ldrb	r1, [r3, #0]
	bl 0x0200cbb8
	movs	r6, #0
	mov	r3, r8
	mov	r1, r8
	strh	r6, [r3, #0]
	strh	r6, [r1, #2]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200d7d0
	.4byte 0x0200d56e
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x0200d54e
	.4byte 0x050001c0
	.4byte 0x84000400
	.4byte 0x06001000
	.4byte 0x84000800
	.4byte 0x03001120
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #2
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02004518
	movs	r1, #7
	bl 0x0200cb80
	b.n	.L_02004520
.L_02004518:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200cb80
.L_02004520:
	ldr	r3, [pc, #16]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02004532
	adds	r0, r5, #0
	bl 0x0200c634
.L_02004532:
	pop	{r5, pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	ldr	r6, [pc, #48]
	adds	r5, r0, #0
	ldr	r0, [r6, #0]
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_02004558
	movs	r1, #6
	lsrs	r0, r0, #1
	bl 0x0200ca60
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl 0x0200cb80
.L_02004558:
	ldr	r3, [r6, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02004568
	adds	r0, r5, #0
	bl 0x0200c634
.L_02004568:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, lr}
	ldr	r3, [pc, #32]
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_02004590
	movs	r1, #6
	lsrs	r0, r0, #1
	bl 0x0200ca60
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl 0x0200cb80
.L_02004590:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	ldr	r6, [r5, #104]
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	cmp	r0, #31
	ble.n	.L_020045b8
	adds	r0, r5, #0
	bl 0x0200cb38
	b.n	.L_020045e2
.L_020045b8:
	lsls	r0, r0, #10
	bl 0x0200ca88
	str	r0, [r5, #24]
	str	r0, [r5, #28]
	movs	r1, #128
	ldr	r3, [r6, #8]
	lsls	r1, r1, #9
	str	r3, [r5, #8]
	ldr	r3, [r5, #12]
	adds	r3, r3, r1
	str	r3, [r5, #12]
	subs	r1, r1, r0
	ldr	r3, [r6, #16]
	lsls	r2, r1, #2
	adds	r2, r2, r1
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r5, #16]
.L_020045e2:
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	ldr	r6, [r5, #104]
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	cmp	r0, #31
	ble.n	.L_02004604
	adds	r0, r5, #0
	bl 0x0200cb38
	b.n	.L_02004630
.L_02004604:
	lsls	r0, r0, #10
	bl 0x0200ca88
	negs	r3, r0
	str	r0, [r5, #24]
	str	r3, [r5, #28]
	movs	r1, #128
	ldr	r3, [r6, #8]
	lsls	r1, r1, #9
	str	r3, [r5, #8]
	ldr	r3, [r5, #12]
	adds	r3, r3, r1
	str	r3, [r5, #12]
	subs	r1, r1, r0
	ldr	r3, [r6, #16]
	lsls	r2, r1, #2
	adds	r2, r2, r1
	subs	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r5, #16]
.L_02004630:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #8
	adds	r6, r0, #0
	mov	r9, r3
	movs	r7, #0
.L_0200464e:
	movs	r0, #16
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #12]
	adds	r0, #255
	bl 0x0200cb30
	mov	r8, sp
	lsls	r3, r7, #2
	mov	r1, r8
	str	r0, [r3, r1]
	cmp	r0, #0
	beq.n	.L_020046f8
	ldr	r3, [r6, #20]
	ldr	r5, [r0, #80]
	str	r3, [r0, #20]
	ldr	r1, [pc, #24]
	adds	r3, r0, #0
	adds	r3, #85
	movs	r2, #0
	strb	r2, [r3, #0]
	adds	r3, #15
	strh	r2, [r3, #0]
	mov	sl, r1
	str	r6, [r0, #104]
	cmp	r5, #0
	beq.n	.L_020046f8
	b.n	.L_0200468c
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_0200468c:
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200cb10
	mov	r2, sl
	strb	r2, [r5, #26]
	ldrb	r0, [r5, #16]
	bl 0x0200cab0
	movs	r3, #248
	lsls	r3, r3, #3
	add	r3, r9
	ldrh	r3, [r3, #0]
	movs	r2, #1
	strb	r3, [r5, #16]
	ldrb	r3, [r5, #17]
	ldr	r1, [pc, #56]
	orrs	r3, r2
	strb	r3, [r5, #17]
	ldrb	r3, [r5, #16]
	ldr	r2, [pc, #52]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r2, [r3, #2]
	ldrh	r3, [r5, #8]
	lsls	r2, r2, #17
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
	movs	r1, #33
	ldrb	r3, [r5, #5]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	movs	r2, #63
	ands	r3, r2
	movs	r1, #64
	orrs	r3, r1
	strb	r3, [r5, #5]
	ldrb	r3, [r5, #7]
	ands	r2, r3
	movs	r3, #128
	orrs	r2, r3
	b.n	.L_020046f0
	.2byte 0x0000
	.4byte 0xfffffc00
	.2byte 0x36e0
	.2byte 0x0200
.L_020046f0:
	ldr	r3, [r5, #40]
	strb	r2, [r5, #7]
	mov	r2, sl
	strb	r2, [r3, #22]
.L_020046f8:
	adds	r7, #1
	cmp	r7, #1
	ble.n	.L_0200464e
	ldr	r2, [sp, #0]
	ldr	r3, [pc, #56]
	ldr	r0, [r2, #80]
	str	r3, [r2, #108]
	ldrb	r1, [r0, #9]
	movs	r2, #13
	negs	r2, r2
	adds	r3, r2, #0
	movs	r4, #4
	ands	r3, r1
	orrs	r3, r4
	strb	r3, [r0, #9]
	mov	r3, r8
	ldr	r1, [r3, #4]
	add	sp, #8
	ldr	r0, [r1, #80]
	ldrb	r3, [r0, #9]
	ands	r2, r3
	ldr	r3, [pc, #28]
	orrs	r2, r4
	str	r3, [r1, #108]
	adds	r1, #35
	movs	r3, #2
	strb	r2, [r0, #9]
	strb	r3, [r1, #0]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200c5e5
	.2byte 0xc599
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	lsls	r3, r0, #4
	subs	r3, r3, r0
	movs	r2, #128
	lsls	r3, r3, #1
	lsls	r2, r2, #8
	adds	r3, r3, r2
	asrs	r3, r3, #16
	adds	r6, r3, #1
	movs	r7, #0
	cmp	r0, #0
	beq.n	.L_0200475e
	movs	r7, #100
.L_0200475e:
	cmp	r6, #15
	bgt.n	.L_02004770
	ldr	r3, [pc, #332]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #1
	bhi.n	.L_02004770
	movs	r7, #200
.L_02004770:
	cmp	r6, #7
	bgt.n	.L_02004782
	ldr	r3, [pc, #312]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #1
	bhi.n	.L_02004782
	movs	r7, #200
.L_02004782:
	ldr	r1, [pc, #304]
	adds	r2, r7, r6
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r2, r3
	bne.n	.L_02004790
	b.n	.L_020048ae
.L_02004790:
	ldr	r5, [pc, #292]
	strh	r2, [r1, #0]
	movs	r1, #128
	ldr	r3, [pc, #292]
	adds	r0, r5, #0
	lsls	r1, r1, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b47
	cmp	r7, #0
	bne.n	.L_020047b2
	ldr	r1, [pc, #284]
	adds	r0, r5, #0
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe075
.L_020047b2:
	cmp	r7, #100
	bne.n	.L_020047c2
	ldr	r1, [pc, #272]
	adds	r0, r5, #0
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe004
.L_020047c2:
	ldr	r1, [pc, #256]
	adds	r0, r5, #0
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2200
	adds	r5, r6, #0
	mov	ip, r2
	cmp	r5, #7
	ble.n	.L_020047e8
	ldr	r3, [pc, #232]
	ldr	r0, [pc, #220]
	ldr	r1, [pc, #240]
	movs	r2, #32
	mov	lr, r3
.L_020047e0:
	.2byte 0xf800
	.2byte 0x2308
	mov	ip, r3
	subs	r5, #8
.L_020047e8:
	cmp	r6, #15
	ble.n	.L_02004800
	ldr	r0, [pc, #224]
	ldr	r1, [pc, #228]
	ldr	r3, [pc, #204]
	movs	r2, #32
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c35
	movs	r0, #16
	mov	ip, r0
	subs	r5, #16
.L_02004800:
	cmp	r6, #23
	ble.n	.L_02004818
	movs	r2, #32
	ldr	r0, [pc, #208]
	ldr	r1, [pc, #208]
	ldr	r3, [pc, #180]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c35
	movs	r2, #24
	mov	ip, r2
	subs	r5, #24
.L_02004818:
	cmp	r6, #30
	ble.n	.L_0200482e
	ldr	r3, [pc, #160]
	ldr	r0, [pc, #192]
	ldr	r1, [pc, #192]
	movs	r2, #32
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2300
	mov	ip, r3
	movs	r5, #0
.L_0200482e:
	cmp	r5, #0
	beq.n	.L_0200489e
	movs	r7, #0
	cmp	r7, r5
	bge.n	.L_0200489e
.L_02004838:
	mov	r3, ip
	cmp	r3, #0
	bge.n	.L_02004840
	adds	r3, #7
.L_02004840:
	asrs	r3, r3, #3
	lsls	r1, r3, #5
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	asrs	r0, r3, #1
	ldr	r2, [pc, #156]
	ldr	r3, [pc, #156]
	adds	r2, r1, r2
	adds	r3, r1, r3
	adds	r6, r2, r0
	adds	r0, r3, r0
	movs	r3, #1
	ands	r3, r7
	movs	r4, #2
	cmp	r3, #0
	bne.n	.L_0200487e
	movs	r4, #1
.L_02004862:
	ldrb	r3, [r0, #0]
	ldrb	r1, [r6, #0]
	movs	r2, #240
	ands	r2, r3
	movs	r3, #15
	ands	r3, r1
	orrs	r2, r3
	adds	r4, #1
	strb	r2, [r0, #0]
	adds	r6, #4
	adds	r0, #4
	cmp	r4, #3
	ble.n	.L_02004862
	b.n	.L_02004898
.L_0200487e:
	ldrb	r3, [r0, #0]
	ldrb	r1, [r6, #0]
	movs	r2, #15
	ands	r2, r3
	movs	r3, #240
	ands	r3, r1
	orrs	r2, r3
	subs	r4, #1
	strb	r2, [r0, #0]
	adds	r6, #4
	adds	r0, #4
	cmp	r4, #0
	bge.n	.L_0200487e
.L_02004898:
	adds	r7, #1
	cmp	r7, r5
	blt.n	.L_02004838
.L_0200489e:
	ldr	r3, [pc, #80]
	movs	r1, #128
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	lsls	r1, r1, #1
	ldr	r2, [pc, #12]
	bl 0x0200cab8
.L_020048ae:
	pop	{r5, r6, r7, pc}
	.4byte 0x0300122c
	.4byte 0x0200ee0c
	.4byte 0x0200ee20
	.4byte 0x03000258
	.4byte 0x03000730
	.4byte 0x0200f020
	.4byte 0x0200efa0
	.4byte 0x0200ef20
	.4byte 0x0200ee40
	.4byte 0x0200ef40
	.4byte 0x0200ee60
	.4byte 0x0200ef60
	.4byte 0x0200ee80
	.4byte 0x0200ef80
	.4byte 0x0200ef24
	.4byte 0x0200ee24
	.2byte 0xee0e
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r0, [pc, #152]
	sub	sp, #16
	str	r0, [sp, #0]
	ldr	r3, [pc, #148]
	ldr	r2, [pc, #152]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r6, r3, #5
	bl 0x0200ce10
	bl 0x0200cc40
	ldr	r3, [pc, #132]
	movs	r2, #3
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02004950
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200caf8
	cmp	r0, #0
	bne.n	.L_02004950
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #11
	cmp	r3, r2
	ble.n	.L_02004950
	ldr	r5, [pc, #100]
	ldrh	r3, [r5, #0]
	subs	r3, #1
	strh	r3, [r5, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bgt.n	.L_02004950
	movs	r0, #1
	bl 0x0200cc08
	ldr	r3, [pc, #60]
	strh	r3, [r5, #0]
.L_02004950:
	bl 0x0200cc10
	bl 0x0200c744
	adds	r0, r7, #0
	add	r1, sp, #4
	adds	r0, #8
	bl 0x0200cae0
	add	r3, sp, #4
	ldr	r0, [sp, #0]
	ldr	r1, [r3, #0]
	ldr	r3, [r3, #4]
	movs	r2, #0
	stmia	r0!, {r2}
	ldr	r5, [pc, #52]
	subs	r1, #16
	lsls	r1, r1, #16
	subs	r3, #32
	orrs	r3, r1
	adds	r4, r0, #0
	orrs	r3, r5
	str	r4, [sp, #0]
	stmia	r0!, {r3}
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r1, r0, #0
	orrs	r6, r3
	b.n	.L_020049a8
	.2byte 0x0000
	.4byte 0x0000000a
	.4byte 0x0200ee14
	.4byte 0x0200ee0e
	.4byte 0x020036e0
	.4byte 0x03001150
	.4byte 0x0200ee10
	.2byte 0x4000
	.2byte 0x8000
.L_020049a8:
	str	r1, [sp, #0]
	str	r6, [r0, #0]
	movs	r1, #255
	ldr	r0, [pc, #8]
	bl 0x0200cad0
	add	sp, #16
	pop	{r5, r6, r7, pc}
	.2byte 0xee14
	.2byte 0x0200
	push	{r5, lr}
	ldr	r1, [pc, #64]
	ldr	r0, [pc, #64]
	bl 0x0200caa8
	ldr	r5, [pc, #64]
	bl 0x0200cac0
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	lsls	r1, r1, #1
	movs	r2, #0
	asrs	r0, r0, #16
	bl 0x0200cab8
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #24]
	movs	r1, #128
	strh	r3, [r2, #0]
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #20]
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	adds	r1, #118
	ldr	r0, [pc, #36]
	bl 0x0200ca70
	b.n	.L_02004a18
	.2byte 0x0000
	.4byte 0xffffffff
	.4byte 0x0000000a
	.4byte 0x0200ef20
	.4byte 0x0200d920
	.4byte 0x0200ee0e
	.4byte 0x0200ee0c
	.4byte 0x0200ee10
	.2byte 0xc8f5
	.2byte 0x0200
.L_02004a18:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r1, [pc, #36]
	ldr	r0, [pc, #36]
	bl 0x0200caa8
	ldr	r3, [pc, #36]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200cab0
	ldr	r2, [pc, #28]
	ldr	r3, [pc, #12]
	ldr	r0, [pc, #28]
	strh	r3, [r2, #0]
	bl 0x0200ca78
	pop	{pc}
	.2byte 0x0000
	.4byte 0xffffffff
	.4byte 0x0200ef20
	.4byte 0x0200d920
	.4byte 0x0200ee0e
	.4byte 0x0200ee0c
	.4byte 0x0200c8f5
	.section .text.x0200ce38,"ax",%progbits
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x23010000
	.4byte 0x16cd0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x22f10000
	.4byte 0x16dd0000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x23010000
	.4byte 0x16cd0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x22f10000
	.4byte 0x16dd0000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x23010000
	.4byte 0x16cd0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.4byte 0x00000005
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0xffffe667
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0x00001999
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000e00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffe80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000030
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x0000002e
	.4byte 0x0200b419
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000078
	.4byte 0x00000000
	.4byte 0x00000026
	.4byte 0x0000002e
	.4byte 0x0200b9a9
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x7c1f7c1f
	.4byte 0x20a21861
	.4byte 0x45643503
	.4byte 0x6a2659c5
	.4byte 0x7ecd7e87
	.4byte 0x7f997f33
	.4byte 0x7c1f7fff
	.4byte 0x00007c1f
	.4byte 0x7c1f7c1f
	.4byte 0x013300ce
	.4byte 0x01b80176
	.4byte 0x023b01fa
	.4byte 0x02de027d
	.4byte 0x337f033f
	.4byte 0x613d7fff
	.4byte 0x00006dff
	.4byte 0x7c1f7c1f
	.4byte 0x2c2b1c09
	.4byte 0x4c713c4e
	.4byte 0x6cb75c94
	.4byte 0x7dbb7cfa
	.4byte 0x7f3d7e7c
	.4byte 0x7c1f7fff
	.4byte 0x00007c1f
	.4byte 0x7c1f7c1f
	.4byte 0x000d000a
	.4byte 0x00150011
	.4byte 0x001d0019
	.4byte 0x211e109d
	.4byte 0x5efe3dfe
	.4byte 0x7c1f7fff
	.4byte 0x00007c1f
	.4byte 0xe8802b01
	.4byte 0xec020200
	.4byte 0x0596020b
	.4byte 0x03280008
	.4byte 0x103c00c4
	.4byte 0xec870201
	.4byte 0x2c400086
	.4byte 0x873fe086
	.4byte 0x2b4000e0
	.4byte 0x4000be06
	.4byte 0x00be0826
	.4byte 0x120d2340
	.4byte 0x60800088
	.4byte 0x0192e091
	.4byte 0xe491e402
	.4byte 0x25400082
	.4byte 0xe0a1e0a0
	.4byte 0xe40201a2
	.4byte 0xa0e4a108
	.4byte 0x234000e4
	.4byte 0x21b1e0b0
	.4byte 0x0201b2e0
	.4byte 0xb0e4b1e4
	.4byte 0x09254000
	.4byte 0xb2e8b1e8
	.4byte 0xb1ec0201
	.4byte 0x00840a01
	.4byte 0xa1e825c0
	.4byte 0x0201a2e8
	.4byte 0x01cfa1ec
	.4byte 0x2740100a
	.4byte 0x020192e8
	.4byte 0x40200601
	.4byte 0x020e202c
	.4byte 0xff4000f7
	.4byte 0x007f4000
	.4byte 0x40000750
	.4byte 0x8000e049
	.4byte 0x0e40106e
	.4byte 0xe2ff0060
	.4byte 0x081b4000
	.4byte 0x628000be
	.4byte 0x019de09c
	.4byte 0xbf41e402
	.4byte 0xab264000
	.4byte 0xade0ace0
	.4byte 0xe4080201
	.4byte 0x50abe4ac
	.4byte 0xe0bb24c0
	.4byte 0xbde021bc
	.4byte 0xbce40201
	.4byte 0x4000bbe4
	.4byte 0xbce80925
	.4byte 0x0201bde8
	.4byte 0x0a01bcec
	.4byte 0x25c00084
	.4byte 0xade8ace8
	.4byte 0xacec0201
	.4byte 0x100a01cf
	.4byte 0x9de82740
	.4byte 0x06010201
	.4byte 0x0f278010
	.4byte 0x4000fa14
	.4byte 0xeb40609f
	.4byte 0x502f4050
	.4byte 0x40003c80
	.4byte 0x0060e04e
	.4byte 0x60aae02e
	.4byte 0x60e02e00
	.4byte 0x60e02e00
	.4byte 0x50e02e00
	.4byte 0xc4e02e80
	.4byte 0x002e0060
	.4byte 0xe08e8880
	.4byte 0xe402018f
	.4byte 0x00e4448e
	.4byte 0xe08c2740
	.4byte 0xe402018d
	.4byte 0x40009a8c
	.4byte 0x018de829
	.4byte 0xec060102
	.4byte 0xe828c000
	.4byte 0x02018f7f
	.4byte 0x00300601
	.4byte 0xff400030
	.4byte 0xb0ce4000
	.4byte 0x40001f40
	.4byte 0x00c0f8af
	.4byte 0x8700c0ff
	.4byte 0x00174400
	.4byte 0x40000780
	.4byte 0x9ae0ba20
	.4byte 0xe4020188
	.4byte 0x4000e4ba
	.4byte 0xaae0b927
	.4byte 0xe4020193
	.4byte 0x294000b9
	.4byte 0x0201aae8
	.4byte 0xec4f0601
	.4byte 0xe828c000
	.4byte 0x0102019a
	.4byte 0x2c401006
	.4byte 0xfe228490
	.4byte 0x40b34000
	.4byte 0x80002b44
	.4byte 0x2f045073
	.4byte 0x00373c00
	.4byte 0xb610a340
	.4byte 0x00888800
	.4byte 0xec822c40
	.4byte 0x2c4000b5
	.4byte 0xbe82e0b5
	.4byte 0x882c0060
	.4byte 0x082c0010
	.4byte 0x264000be
	.4byte 0x4000be08
	.4byte 0x40e18826
	.4byte 0xbe0a2c00
	.4byte 0x83244000
	.4byte 0x00e483e0
	.4byte 0x85112940
	.4byte 0x020193e0
	.4byte 0x00e485e4
	.4byte 0xe8392840
	.4byte 0x01020193
	.4byte 0x2bc00006
	.4byte 0xc05083e8
	.4byte 0xc030f6b9
	.4byte 0xff400028
	.4byte 0x001e3050
	.4byte 0x00e04f40
	.4byte 0x40106e40
	.4byte 0xec22b70e
	.4byte 0x2c4000b6
	.4byte 0x00a6eca7
	.4byte 0x3fa62c40
	.4byte 0x0060a7e0
	.4byte 0x00be082a
	.4byte 0xbe082640
	.4byte 0x08264000
	.4byte 0x4000febe
	.4byte 0x003e0a24
	.4byte 0xfc0a2240
	.4byte 0x0e268000
	.4byte 0x20c04042
	.4byte 0xb3e013b3
	.4byte 0x2c4000e4
	.4byte 0x40a0b3e8
	.4byte 0x1f309035
	.4byte 0xab4000f5
	.4byte 0x00ff0200
	.4byte 0x0060ff02
	.4byte 0x80709820
	.2byte 0x972e
.L_020052be:
	ands	r0, r0
	ldrb	r4, [r5, #28]
	sub	sp, #92
	cmp	r4, #64
	.2byte 0xbe08
	strh	r0, [r0, #0]
	movs	r1, r5
	cmp	r4, #64
.L_020052ce:
	.2byte 0xbe08
	strh	r0, [r2, #2]
	lsrs	r6, r4, #32
	sub	sp, #248
	strh	r0, [r0, #0]
	.2byte 0xec27
	.2byte 0x8000
	str	r0, [r7, r0]
	movs	r2, #192
	.2byte 0xe081
	.2byte 0xe481
	.4byte 0x2c40009f
	.4byte 0x006081e8
	.4byte 0xff0200ff
	.4byte 0x00ff0200
	.4byte 0xa2412102
	.4byte 0xff0200df
	.4byte 0xb77a0200
	.4byte 0x00ff1210
	.4byte 0x0200ff02
	.4byte 0xff0200ff
	.4byte 0x80100200
	.4byte 0xff000000
	.4byte 0x1141387d
	.4byte 0x7ffe6715
	.4byte 0xe6889329
	.2byte 0xaad0
	.2byte 0x65ff
	push	{r1, r4, r6, lr}
	strb	r3, [r7, #11]
.L_02005328:
	.2byte 0xd968
	ldrh	r3, [r1, #50]
.L_0200532c:
	adds	r3, #117
	.2byte 0xfbff
	.2byte 0x82bc
	ldr	r0, [sp, #188]
.L_02005334:
	strh	r1, [r4, #46]
	ldr	r7, [pc, #612]
	stmia	r5!, {r0, r3}
	str	r4, [sp, #492]
	lsrs	r1, r6, #7
	subs	r6, #28
.L_02005340:
	stmia	r0!, {r0, r1, r2, r3, r4, r5, r6, r7}
	asrs	r7, r4, #21
	subs	r1, #148
	.2byte 0xea94
	.2byte 0x99cb
	add	r3, sp, #272
	rors	r4, r3
	ldr	r4, [pc, #724]
.L_02005350:
	adds	r5, r3, #6
	str	r0, [sp, #820]
	str	r5, [sp, #464]
	adds	r3, r7, r0
.L_02005358:
	pop	{r1, r3, r4, r6, r7, pc}
	.2byte 0x465b
	.4byte 0x4519b111
	.4byte 0x630f76cc
	.4byte 0x71176d4e
	.4byte 0x3177ddea
	.4byte 0xbaa666eb
	.4byte 0xc39531cc
	.4byte 0xbc9f60a7
	.4byte 0xb2350a43
	.4byte 0x3521ccf8
	.4byte 0x45c80c78
	.4byte 0x19ccf490
	.4byte 0x44c62e2a
	.4byte 0x4aac5489
	.4byte 0x2ad56a94
	.4byte 0x41d8658a
	.4byte 0x0547ccd1
	.4byte 0x611c0ab3
	.4byte 0x10c0a336
	.4byte 0x2a05033e
	.4byte 0x500a11c0
	.4byte 0x00a11c84
	.4byte 0x892fc845
	.4byte 0x905596e4
	.4byte 0x483bd970
	.4byte 0xa9e670f8
	.4byte 0xa6ab3176
	.4byte 0x5251886a
	.4byte 0x0eebacbd
	.4byte 0x7cb3bf9f
	.4byte 0xa59f8176
	.4byte 0x52c78d30
	.4byte 0x06a7ca38
	.4byte 0x8f9b3306
	.2byte 0xc682
	tst	r0, r7
	adds	r3, #96
.L_020053e6:
	.2byte 0xf847
	.2byte 0x7104
	asrs	r2, r4, #15
	.2byte 0xb370
	bne.n	.L_02005424
	.2byte 0xf139
	.2byte 0x9890
	ldr	r0, [pc, #608]
	str	r4, [r1, #68]
	str	r1, [sp, #376]
	ldmia	r5!, {r1, r3, r6}
.L_020053fc:
	adds	r5, #42
	asrs	r0, r7, #3
	adds	r2, #205
.L_02005402:
	ldmia	r5, {r0, r2, r4, r5}
	strb	r4, [r2, #3]
	cmp	r3, #56
.L_02005408:
	lsls	r2, r6
	lsls	r6, r3, #17
	ldr	r7, [r2, r4]
	ldr	r6, [pc, #960]
	.2byte 0xf524
	.2byte 0x854e
	strh	r2, [r0, #60]
.L_02005416:
	ldrb	r1, [r2, #20]
	.2byte 0xf4fe
	.2byte 0xd3e2
	ldmia	r4!, {r2, r3}
	add	r0, pc, #224
	ldr	r4, [pc, #172]
	.2byte 0xdb03
.L_02005424:
	adds	r2, #192
	.2byte 0xb870
	adds	r3, #152
	cmp	r4, #33
.L_0200542c:
	add	r2, sp, #944
	.2byte 0xfc99
	.2byte 0x0a8d
.L_02005432:
	.2byte 0xb37b
	str	r3, [r0, #124]
	str	r3, [r4, #84]
	ldrh	r6, [r3, #56]
.L_0200543a:
	ldr	r3, [r0, #44]
	stmia	r2!, {r2, r3, r6, r7}
	strh	r0, [r5, r1]
	stmia	r1!, {r0, r3, r5, r7}
	lsls	r3, r0, #19
	movs	r2, #39
	.2byte 0x1d3f
	.2byte 0xf02e
	.2byte 0x1c9a
	lsls	r0, r3
	add	r0, pc, #4
	add	r3, pc, #608
	lsrs	r0, r0, #9
	ldrh	r0, [r1, #16]
	ldrh	r1, [r7, #16]
	lsrs	r7, r1, #26
	strb	r0, [r3, #20]
.L_0200545c:
	lsrs	r0, r4, #13
	lsrs	r6, r4, #15
	strh	r6, [r7, #60]
.L_02005462:
	ldr	r0, [pc, #880]
	str	r0, [r5, #112]
.L_02005466:
	ldrh	r7, [r0, #4]
	ldr	r5, [sp, #708]
	b.n	.L_020052be
	.4byte 0x02e81d9d
	.4byte 0xc10ee0a0
	.4byte 0x7687ba82
	.4byte 0x42227088
	.4byte 0xd267118f
	.4byte 0x40899ccc
	.4byte 0x834aacce
	.4byte 0x1be0706f
	.4byte 0x61b62448
	.4byte 0x6571c787
	.4byte 0x05d814c3
	.4byte 0xc4c327c8
	.4byte 0x7c808af9
	.4byte 0x4e5ea933
	.4byte 0xe7d551d5
	.4byte 0x103e350f
	.4byte 0xe5ce7182
	.4byte 0xe7107013
	.4byte 0x0f87be1e
	.4byte 0x127ccbc6
	.4byte 0xf80c066e
	.4byte 0x65cfa21d
	.4byte 0xb264e43a
	.4byte 0x7989e132
	.4byte 0x29e70e8a
	.4byte 0x942246e6
	.4byte 0x622eb888
	.4byte 0x27e0a182
	.4byte 0x827e2e98
	.4byte 0xfc18fa90
	.4byte 0x041687c4
	.4byte 0xfa0421f0
	.4byte 0xd7b0ebc2
	.4byte 0x3f863edd
	.4byte 0xea6f07c0
	.4byte 0xfe7d8873
	.4byte 0xf03fcf81
	.4byte 0x953e07f9
	.4byte 0xf86b8fc0
	.4byte 0x00ff0e38
	.4byte 0xc3bc61fa
	.4byte 0xfe7cf0ff
	.4byte 0x6cbfe050
	.4byte 0x00737848
	.4byte 0xd7f76b76
	.4byte 0xd001ce01
	.4byte 0x1f780049
	.4byte 0x183c1f85
	.4byte 0xfcfb9bc0
	.4byte 0xe07f9f03
	.4byte 0x3e1df763
	.4byte 0x1fe7c0ff
	.4byte 0x9f03fcf8
	.4byte 0x0ff3e07f
	.4byte 0xcf81fe7c
	.4byte 0x04e1f03f
	.4byte 0x01a0003e
	.4byte 0x08420000
	.4byte 0x18c61084
	.4byte 0x294a2108
	.4byte 0x3def318c
	.4byte 0x4e734631
	.4byte 0x63185ad6
	.4byte 0x77bd6b5a
	.4byte 0x00017fff
	.4byte 0xe009e008
	.4byte 0xe00be00a
	.4byte 0x0de00c00
	.4byte 0x0fe00ee0
	.4byte 0xe01000e0
	.4byte 0xe012e011
	.4byte 0x1410e013
	.4byte 0x040015e0
	.4byte 0x29e02814
	.4byte 0xe02a00e0
	.4byte 0xe02ce02b
	.4byte 0x2e00e02d
	.4byte 0x30e02fe0
	.4byte 0x01e031e0
	.4byte 0xe033e032
	.4byte 0x0035e034
	.4byte 0x06081404
	.4byte 0x00e007e0
	.4byte 0xe0262b84
	.4byte 0x84008827
	.4byte 0x05e0042c
	.4byte 0x242c8400
	.4byte 0x008825e0
	.4byte 0xe0022c84
	.4byte 0x2c840003
	.4byte 0xbd23e022
	.4byte 0x002c8400
	.4byte 0x84000202
	.4byte 0x0040032b
	.4byte 0x04002b84
	.4byte 0x00016202
	.4byte 0x3e032884
	.4byte 0x0021e020
	.4byte 0xff002884
	.4byte 0x8400020a
	.4byte 0x00400b23
	.4byte 0x400b2384
	.4byte 0x0b230810
	.4byte 0x2308103e
	.4byte 0x00800fff
	.4byte 0x800f1f84
	.4byte 0x0d1f8400
	.4byte 0x21081080
	.4byte 0x00033e00
	.4byte 0x00ff1b84
	.4byte 0x84000580
	.4byte 0x073e0019
	.4byte 0x00178400
	.4byte 0x84000980
	.4byte 0x0b3e0015
	.4byte 0xff138400
	.4byte 0x000f8000
	.4byte 0x80000f84
	.4byte 0x0f84000f
	.4byte 0x100d8000
	.4byte 0x3e001108
	.4byte 0x0b840013
	.4byte 0x158000ff
	.4byte 0x00098400
	.4byte 0x8400173e
	.4byte 0x1b800007
	.4byte 0x00038400
	.4byte 0x84001b80
	.4byte 0xe870ff03
	.4byte 0x07040017
	.4byte 0x0017e870
	.4byte 0x80000704
	.4byte 0xff8000ff
	.4byte 0x00ff8000
	.4byte 0x00ffff80
	.4byte 0x6cd0af80
	.4byte 0x0304001b
	.4byte 0x001b6cd0
	.4byte 0xf0d00304
	.4byte 0xd0040f1f
	.4byte 0x0fff1ff0
	.4byte 0x2374e004
	.4byte 0x74e0040b
	.4byte 0xe0040b23
	.4byte 0x040727f8
	.4byte 0x6f1df0d0
	.4byte 0x00140014
	.4byte 0x341df0d0
	.4byte 0xd0001400
	.4byte 0x80001df0
	.4byte 0x19808001
	.4byte 0x0528d0ff
	.4byte 0x8b234000
	.4byte 0x23400084
	.4byte 0x4000840b
	.4byte 0x00840727
	.4byte 0x03ff2b40
	.4byte 0x27400084
	.4byte 0x40004407
	.4byte 0xff02002b
	.4byte 0x00ff0200
	.4byte 0x0200ff02
	.4byte 0x0200c6ff
	.4byte 0xe30200ff
	.4byte 0x8009e008
	.4byte 0x80801a84
	.4byte 0xe03f2801
	.4byte 0x1a848029
	.4byte 0x83018080
	.4byte 0x19840044
	.4byte 0x83018000
	.4byte 0x8400ff44
	.4byte 0x01800019
	.4byte 0x8400c887
	.4byte 0x0d420f09
	.4byte 0x10c88780
	.4byte 0x09ff1d08
	.4byte 0x104c9b04
	.4byte 0x04051d8c
	.4byte 0x84004c9b
	.4byte 0x00800323
	.4byte 0x346f2984
	.4byte 0x84008004
	.4byte 0x54a01429
	.4byte 0x1b102004
	.4byte 0x200354a0
	.4byte 0x3fff1b10
	.4byte 0x1f102010
	.4byte 0x8400400f
	.4byte 0x0b5cb01f
	.4byte 0xb0130810
	.4byte 0x84000b5c
	.4byte 0x8000ff13
	.4byte 0x1b840003
	.4byte 0x00038000
	.4byte 0x64c01b84
	.4byte 0x0b840013
	.4byte 0x001364c0
	.4byte 0x00ff0b84
	.4byte 0x84000b80
	.4byte 0x0b800013
	.4byte 0xc0138400
	.4byte 0x84000fe4
	.4byte 0x0f60c00f
	.4byte 0xff0f8400
	.4byte 0x0f1ff0d0
	.4byte 0x1ff0d084
	.4byte 0xac60840f
	.4byte 0x0384001b
	.4byte 0x001b4000
	.4byte 0xe0ff0384
	.4byte 0x08101b70
	.2byte 0xd003
	.2byte 0x1bec
.L_020057c0:
	lsrs	r0, r2, #32
	strb	r3, [r0, #0]
.L_020057c4:
	movs	r3, #180
	strh	r3, [r1, #32]
.L_020057c8:
	ands	r0, r0
.L_020057ca:
	lsrs	r3, r4, #12
	strh	r4, [r0, #4]
.L_020057ce:
	movs	r0, r0
.L_020057d0:
	movs	r3, #0
	add	r6, sp, #496
.L_020057d4:
	str	r0, [sp, #772]
	ldr	r2, [pc, #136]
.L_020057d8:
	ldr	r2, [r1, r2]
	add	r3, pc, #912
	ldrh	r0, [r3, #30]
	str	r4, [sp, #64]
	b.n	.L_02005b86
	.2byte 0xa01b
	.4byte 0x58a96041
	.4byte 0x8671c264
	.4byte 0x338e90e0
	.4byte 0x86587465
	.4byte 0xc48a1d06
	.4byte 0x22510742
	.4byte 0x523a5582
	.4byte 0x1962521d
	.4byte 0x1228741a
	.4byte 0xab441d0b
	.4byte 0xc66b2d56
	.4byte 0x2a59aab1
	.4byte 0xa0b0ca45
	.4byte 0x10742448
	.4byte 0xcd668d99
	.4byte 0x68e3bdde
	.4byte 0x474883a7
	.4byte 0x2810343b
	.4byte 0x162450e5
	.4byte 0x46230fa4
	.4byte 0x883b0044
	.4byte 0x3c748838
	.4byte 0x60990344
	.4byte 0x99e62287
	.4byte 0xea623933
	.4byte 0x5311d220
	.4byte 0x02c40b87
	.2byte 0x5816
	push	{r1, r3, r4, r5, lr}
	strb	r2, [r5, r5]
.L_02005856:
	add	sp, #16
	ldrh	r3, [r0, #4]
.L_0200585a:
	ldr	r0, [pc, #524]
	strb	r7, [r0, r3]
	strh	r1, [r2, #4]
	strb	r5, [r0, #24]
	stmia	r2!, {r6, r7}
	.2byte 0xbacc
	lsrs	r7, r6, #5
	bge.n	.L_020057c0
	lsrs	r1, r4, #2
	.2byte 0xec05
	.2byte 0xe220
	.4byte 0x580b1120
	.4byte 0x733183a0
	.4byte 0x3acb8e66
	.4byte 0xd65c7488
	.4byte 0x320833e1
	.4byte 0x1bbddee7
	.4byte 0xe220ec01
	.4byte 0x5b91d220
	.4byte 0x90446647
	.4byte 0xef6e160b
	.4byte 0x0edd9dde
	.4byte 0x1d220e22
	.4byte 0x16ec75db
	.4byte 0xa160580b
	.4byte 0x0b10234f
	.4byte 0x7fd16058
	.4byte 0x75fdee82
	.4byte 0xb220e228
	.4byte 0x3f7f4751
	.4byte 0x59f7a807
	.4byte 0x0e012204
	.4byte 0x3ee471f0
	.4byte 0xcf8020c1
	.4byte 0x07f9f03f
	.4byte 0x07c0a33e
	.4byte 0xbb9f7beb
	.4byte 0x91d2a5ff
	.4byte 0x7fe2c7c8
	.4byte 0xfe89963a
	.4byte 0x829f3f30
	.4byte 0xfcf0fa01
	.4byte 0xcf9fd9cf
	.4byte 0x7ec4ffc7
	.4byte 0x06ccfece
	.4byte 0xdaf9ff9c
	.4byte 0xffc24f9f
	.4byte 0x123e7904
	.4byte 0xf851f403
	.4byte 0xf9fd9f9f
	.4byte 0x27cffcec
	.4byte 0x5a3e7f62
	.4byte 0x43f9f3ff
	.4byte 0xf03fcf99
	.4byte 0x003e0521
	.4byte 0x11128100
	.4byte 0x904fffa4
	.4byte 0xd25c1bc8
	.4byte 0x412ef611
	.4byte 0xa7606f00
	.4byte 0x02dfdf7a
	.4byte 0x3fa23ef5
	.4byte 0x3e4ffa81
	.4byte 0xfaad3e81
	.4byte 0xf9bf7e0b
	.4byte 0x904cce26
	.4byte 0x1d8108a4
	.4byte 0xf55f0a3e
	.4byte 0xc0bb5599
	.4byte 0xa7cbe147
	.4byte 0x9f5027f2
	.4byte 0x3b217549
	.4byte 0x52be147c
	.4byte 0x3c48208c
	.4byte 0x0e452098
	.4byte 0x2f851f02
	.4byte 0x0fd423f5
	.4byte 0x47c39f56
	.4byte 0xe54f8be1
	.4byte 0x907ea04f
	.4byte 0x33ea41fa
	.4byte 0xf17c18f9
	.4byte 0x00000001
	.4byte 0xffff0000
	.4byte 0x0000267a
	.4byte 0x40002614
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00002800
	.4byte 0x80001f26
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000027a0
	.4byte 0x4000208a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000027a0
	.4byte 0xc0002030
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00002799
	.4byte 0x4000219a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000028b7
	.4byte 0x400020a2
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x000027ce
	.4byte 0x800022b3
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00002952
	.4byte 0x400022f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x0000298b
	.4byte 0xc0002350
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x000027b8
	.4byte 0x4000259e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x0000281e
	.4byte 0x40002532
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x0000281f
	.4byte 0xc00024d1
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x0000288c
	.4byte 0x8000249a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x000028c8
	.4byte 0x000024b7
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000e
	.4byte 0x00002b75
	.4byte 0x400029c7
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000f
	.4byte 0x00002f26
	.4byte 0x400028e6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0010
	.4byte 0x00002e68
	.4byte 0x4000276a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0011
	.4byte 0x00002ae0
	.4byte 0x800026e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0012
	.4byte 0x00002dd8
	.4byte 0xc000253a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0013
	.4byte 0x00002e86
	.4byte 0x40002601
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0014
	.4byte 0x00002d71
	.4byte 0x400024be
	.4byte 0xffff0000
	.4byte 0xffffffff
	.2byte 0xffff
.L_02005b86:
	movs	r0, r0
	movs	r5, r2
	.2byte 0xffff
	.2byte 0x241c
	movs	r0, r0
	movs	r5, #56
	strh	r0, [r0, #0]
.L_02005b94:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r6, r2
	.2byte 0xffff
	.2byte 0x2470
	movs	r0, r0
	movs	r5, #56
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r7, r2
	.2byte 0xffff
	.2byte 0x23bc
	movs	r0, r0
	movs	r3, #88
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r0, r3
	.2byte 0xffff
	.2byte 0x23bc
	movs	r0, r0
	movs	r3, #28
	.2byte 0xc000
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r1, r3
	.2byte 0xffff
	.2byte 0x23ef
	movs	r0, r0
	movs	r0, #218
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r2, r3
	.2byte 0xffff
	.2byte 0x2364
	movs	r0, r0
	movs	r0, #98
	strh	r0, [r0, #0]
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r3, r3
	.2byte 0xffff
	.2byte 0x2428
	movs	r0, r0
	subs	r6, r1, #4
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r4, r3
	.2byte 0xffff
	.2byte 0x254a
.L_02005c36:
	movs	r0, r0
	movs	r0, #124
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r5, r3
	.2byte 0xffff
	.2byte 0x31e2
	movs	r0, r0
	cmp	r5, #50
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r6, r3
	.2byte 0xffff
	.2byte 0x2dcc
	movs	r0, r0
	.2byte 0x1f80
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r7, r3
	.2byte 0xffff
	.2byte 0x2da0
	movs	r0, r0
	subs	r6, r2, r0
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r0, r4
	.2byte 0xffff
	.2byte 0x34ae
	movs	r0, r0
	adds	r2, r7, #4
	.2byte 0xc000
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r1, r4
	.2byte 0xffff
	.2byte 0x327b
	movs	r0, r0
	movs	r5, #209
	.2byte 0xc000
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r2, r4
	.2byte 0xffff
	.2byte 0x3279
	movs	r0, r0
	movs	r6, #24
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r3, r4
	.2byte 0xffff
	.2byte 0x3188
	movs	r0, r0
	cmp	r0, #57
	strh	r0, [r0, #0]
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r4, r4
	.2byte 0xffff
	.2byte 0x388e
	movs	r0, r0
	movs	r0, #218
	strh	r0, [r0, #0]
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r5, r4
	.2byte 0xffff
	.2byte 0x37aa
	movs	r0, r0
	movs	r2, #24
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_02005d1a:
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r6, r4
	.2byte 0xffff
	.2byte 0x3440
	movs	r0, r0
	asrs	r4, r6, #30
	strh	r0, [r0, #0]
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r7, r4
	.2byte 0xffff
	.2byte 0x348c
	movs	r0, r0
	adds	r4, r7, r0
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r0, r5
	.2byte 0xffff
	.2byte 0x2a97
	movs	r0, r0
	adds	r5, r1, r2
	ands	r0, r0
	movs	r0, r0
.L_02005d5e:
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
.L_02005d66:
	movs	r0, r0
	movs	r1, r5
	.2byte 0xffff
	.2byte 0x2bc1
	movs	r0, r0
	asrs	r1, r3, #31
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r2, r5
	.2byte 0xffff
	.2byte 0x30ad
	movs	r0, r0
	adds	r6, r1, #6
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
.L_02005d96:
	movs	r0, r0
	movs	r3, r5
	.2byte 0xffff
	.2byte 0x2f3a
	movs	r0, r0
	subs	r5, r7, r7
	strh	r0, [r0, #0]
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r4, r5
	.2byte 0xffff
	.2byte 0x32d2
	movs	r0, r0
	lsls	r0, r3, #19
	.2byte 0xc000
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r5, r5
	.2byte 0xffff
	.2byte 0x3320
	movs	r0, r0
	lsls	r2, r0, #17
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r6, r5
	.2byte 0xffff
	.2byte 0x2512
	movs	r0, r0
	cmp	r2, #208
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r7, r5
	.2byte 0xffff
	.2byte 0x16cc
	movs	r0, r0
	movs	r7, #136
	.2byte 0xc000
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
.L_02005e0e:
	movs	r0, r0
	movs	r0, r6
	.2byte 0xffff
	.2byte 0x2186
	movs	r0, r0
	adds	r4, r7, #3
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r1, r6
	.2byte 0xffff
	.2byte 0x18aa
	movs	r0, r0
	adds	r7, r1, r1
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r2, r6
	.2byte 0xffff
	.2byte 0x228f
	movs	r0, r0
	asrs	r5, r2, #8
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
.L_02005e58:
	movs	r3, r6
	.2byte 0xffff
	.2byte 0x230a
	movs	r0, r0
	adds	r4, r1, #5
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r4, r6
	.2byte 0xffff
	.2byte 0x1c58
	movs	r0, r0
	movs	r0, #133
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r5, r6
	.2byte 0xffff
	.2byte 0x1c70
	movs	r0, r0
	subs	r4, r6, #7
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r6, r6
	.2byte 0xffff
	.2byte 0x1e22
	movs	r0, r0
	subs	r0, r2, #1
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
.L_02005eb8:
	movs	r7, r6
	.2byte 0xffff
	.2byte 0x1c6d
	movs	r0, r0
	adds	r2, r4, #2
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r0, r7
.L_02005ed2:
	.2byte 0xffff
	.2byte 0x1cd7
	movs	r0, r0
	asrs	r1, r7, #25
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_02005ee2:
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r1, r7
	.2byte 0xffff
	.2byte 0x1d50
	movs	r0, r0
	asrs	r2, r2, #24
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r2, r7
	.2byte 0xffff
	.2byte 0x1def
	movs	r0, r0
	asrs	r7, r3, #22
.L_02005f0a:
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r3, r7
	.2byte 0xffff
	.2byte 0x2300
	movs	r0, r0
	asrs	r0, r4, #27
	strh	r0, [r0, #0]
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_02005f2a:
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r4, r7
	.2byte 0xffff
	.2byte 0x22e5
	movs	r0, r0
	lsrs	r0, r7, #25
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r5, r7
	.2byte 0xffff
	.2byte 0x22d4
	movs	r0, r0
	lsls	r0, r5, #25
	.2byte 0xc000
	movs	r0, r0
.L_02005f56:
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r6, r7
	.2byte 0xffff
	.2byte 0x2360
	movs	r0, r0
	lsls	r2, r4, #19
	ands	r0, r0
	movs	r0, r0
.L_02005f6e:
	.2byte 0xffff
	.2byte 0xffff
.L_02005f72:
	.2byte 0xffff
	.2byte 0xffff
.L_02005f76:
	movs	r0, r0
	movs	r7, r7
	.2byte 0xffff
	.2byte 0x2360
	movs	r0, r0
	lsls	r7, r2, #18
	.2byte 0xc000
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r0, r0, #1
	.2byte 0xffff
	.2byte 0x07c8
	movs	r0, r0
	adds	r0, r5, #1
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_02005fa2:
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r1, r0, #1
	.2byte 0xffff
	.2byte 0x27ff
	movs	r0, r0
.L_02005fb0:
	subs	r4, r1, #3
	movs	r0, r0
.L_02005fb4:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_02005fba:
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r3, r0, #1
	.2byte 0xffff
	.2byte 0x2d71
	movs	r0, r0
.L_02005fc8:
	movs	r4, #190
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r4, r0, #1
	.2byte 0xffff
	.2byte 0x1c58
	movs	r0, r0
	movs	r0, #133
.L_02005fe2:
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
.L_02005ff0:
	lsls	r5, r0, #1
	.2byte 0xffff
	.2byte 0x2990
	movs	r0, r0
	movs	r3, #115
.L_02005ffa:
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_02006002:
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
.L_02006008:
	lsls	r7, r0, #1
	.2byte 0xffff
	.2byte 0x29e0
.L_0200600e:
	movs	r0, r0
	movs	r3, #115
	movs	r0, r0
	movs	r0, r0
.L_02006016:
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
.L_0200601e:
	movs	r0, r0
	lsls	r0, r1, #1
.L_02006022:
	.2byte 0xffff
	.2byte 0x1c6f
	movs	r0, r0
	adds	r3, r1, #2
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
.L_02006036:
	movs	r0, r0
	lsls	r1, r1, #1
	.2byte 0xffff
	.2byte 0x27b8
	movs	r0, r0
	movs	r5, #158
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r2, r1, #1
.L_02006052:
	.2byte 0xffff
	.2byte 0x27b8
	movs	r0, r0
	movs	r5, #158
	ands	r0, r0
.L_0200605c:
	movs	r0, r0
.L_0200605e:
	.2byte 0xffff
	.2byte 0xffff
.L_02006062:
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
.L_02006068:
	lsls	r5, r1, #1
	.2byte 0xffff
	.2byte 0x2f40
	movs	r0, r0
	movs	r0, #133
.L_02006072:
	movs	r0, r0
	movs	r0, r0
.L_02006076:
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
.L_02006080:
	lsls	r6, r1, #1
	.2byte 0xffff
	.2byte 0x38de
.L_02006086:
	movs	r0, r0
	movs	r1, #9
.L_0200608a:
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_02006092:
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r0, r2, #1
	.2byte 0xffff
	.2byte 0x2dea
	movs	r0, r0
	subs	r6, r3, #5
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_020060aa:
	.2byte 0xffff
	.2byte 0xffff
.L_020060ae:
	movs	r0, r0
	lsls	r1, r2, #1
	.2byte 0xffff
	.2byte 0x2db4
.L_020060b6:
	movs	r0, r0
	subs	r2, r4, #5
	strh	r0, [r0, #0]
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_020060c2:
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r2, r2, #1
	.2byte 0xffff
	.2byte 0x2d4c
	movs	r0, r0
	movs	r7, #220
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_020060da:
	.2byte 0xffff
	.2byte 0xffff
.L_020060de:
	movs	r0, r0
.L_020060e0:
	lsls	r3, r2, #1
	.2byte 0xffff
	.2byte 0x2800
	movs	r0, r0
	movs	r3, #182
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
.L_020060f6:
	movs	r0, r0
	lsls	r4, r2, #1
	.2byte 0xffff
	.2byte 0x233c
	movs	r0, r0
	lsls	r2, r6, #13
.L_02006102:
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r5, r2, #1
	.2byte 0xffff
	.2byte 0x2448
	movs	r0, r0
	movs	r4, #218
	.2byte 0xc000
.L_0200611c:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r6, r2, #1
	.2byte 0xffff
	.2byte 0x2448
.L_0200612e:
	movs	r0, r0
	movs	r5, #168
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r7, r2, #1
	.2byte 0xffff
	.2byte 0x3160
	movs	r0, r0
	asrs	r6, r4, #4
	ands	r0, r0
	movs	r0, r0
.L_0200614e:
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r0, r3, #1
	.2byte 0xffff
	.2byte 0x1ebc
	movs	r0, r0
	movs	r3, #160
	ands	r0, r0
.L_02006164:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
.L_02006170:
	lsls	r1, r3, #1
	.2byte 0xffff
	.2byte 0x2507
.L_02006176:
	movs	r0, r0
	asrs	r4, r0, #15
.L_0200617a:
	ands	r0, r0
.L_0200617c:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r2, r4, #1
	.2byte 0xffff
	.2byte 0x22d4
.L_0200618e:
	movs	r0, r0
.L_02006190:
	lsls	r0, r5, #25
	ands	r0, r0
.L_02006194:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_0200619a:
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	lsls	r3, r4, #1
	.2byte 0xffff
	.2byte 0x281f
.L_020061a6:
	movs	r0, r0
	movs	r5, #68
.L_020061aa:
	ands	r0, r0
.L_020061ac:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0000
.L_020061bc:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020061ca:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r2, r0
	movs	r0, r0
	beq.n	.L_020061dc
	lsls	r4, r0, #16
	movs	r0, #9
	lsls	r0, r2, #16
.L_020061dc:
	add	r0, pc, #168
	lsls	r0, r4, #16
	stmia	r0!, {r0, r3}
	lsls	r0, r2, #19
	str	r6, [r3, r2]
	lsls	r0, r4, #19
	ands	r1, r1
	movs	r0, r2
	strh	r2, [r1, #0]
	asrs	r0, r4, #32
.L_020061f0:
	lsrs	r3, r6, #1
	movs	r0, r0
.L_020061f4:
	str	r0, [sp, #40]
.L_020061f6:
	asrs	r0, r6, #32
	lsrs	r3, r6, #1
.L_020061fa:
	movs	r0, r0
	strh	r5, [r1, #0]
	movs	r0, r4
.L_02006200:
	str	r0, [sp, #52]
	movs	r0, r6
	asrs	r5, r2, #32
	lsls	r0, r0, #1
	adds	r0, #23
	lsls	r0, r2, #1
	asrs	r2, r4, #32
	lsls	r0, r4, #1
	str	r4, [r4, #0]
	lsls	r0, r6, #1
	movs	r0, #43
	lsls	r0, r0, #2
	add	sp, #176
	lsls	r0, r2, #2
	asrs	r5, r6, #32
	lsls	r0, r4, #2
	movs	r0, #53
	lsls	r0, r6, #2
	asrs	r6, r6, #32
	lsls	r0, r0, #3
	movs	r0, #54
	lsls	r0, r2, #3
	str	r7, [r6, #0]
	lsls	r0, r4, #3
	asrs	r5, r7, #32
.L_02006232:
	lsls	r0, r6, #3
	asrs	r2, r2, #1
	lsls	r0, r0, #4
	asrs	r3, r0, #1
	lsls	r0, r2, #4
.L_0200623c:
	movs	r0, #74
	lsls	r0, r4, #4
	movs	r0, #75
	lsls	r0, r6, #4
	str	r0, [sp, #380]
	lsls	r0, r0, #5
	asrs	r6, r4, #1
	lsls	r0, r2, #5
	movs	r0, #102
	lsls	r0, r4, #5
.L_02006250:
	str	r1, [r5, #4]
	lsls	r0, r6, #5
	asrs	r4, r5, #1
	lsls	r0, r2, #6
.L_02006258:
	asrs	r3, r5, #1
	lsls	r0, r4, #6
	asrs	r1, r6, #1
	asrs	r0, r6, #6
	lsrs	r7, r7, #3
	movs	r0, r0
	asrs	r6, r5, #1
	asrs	r0, r6, #6
	.2byte 0xffff
	.2byte 0xffff
	asrs	r3, r0, #2
.L_0200626e:
	lsls	r0, r0, #7
	asrs	r4, r0, #2
	lsls	r0, r2, #7
.L_02006274:
	asrs	r5, r0, #2
.L_02006276:
	lsls	r0, r4, #7
	add	r0, pc, #536
.L_0200627a:
	lsls	r0, r6, #7
	asrs	r7, r0, #2
	lsls	r0, r0, #8
.L_02006280:
	add	r0, pc, #556
	lsls	r0, r2, #8
	sub	sp, #44
.L_02006286:
	lsls	r0, r4, #8
	asrs	r7, r1, #2
	lsls	r0, r6, #8
	asrs	r2, r3, #2
	lsls	r0, r0, #9
	asrs	r6, r3, #2
	lsls	r0, r2, #9
	asrs	r3, r5, #2
.L_02006296:
	lsls	r0, r4, #9
.L_02006298:
	str	r6, [r5, r2]
	lsls	r0, r6, #9
	str	r0, [sp, #728]
	lsls	r0, r0, #10
	asrs	r3, r0, #3
	lsls	r0, r2, #10
.L_020062a4:
	asrs	r5, r0, #3
	lsls	r0, r4, #10
	str	r6, [r0, r3]
	lsls	r0, r6, #10
	adds	r0, #198
	lsls	r0, r0, #11
.L_020062b0:
	asrs	r7, r0, #3
	lsls	r0, r2, #11
	asrs	r6, r1, #3
	lsls	r0, r4, #11
	asrs	r5, r3, #3
	lsls	r0, r6, #11
	asrs	r5, r2, #3
.L_020062be:
	lsls	r0, r0, #12
	asrs	r0, r3, #3
	lsls	r0, r2, #12
	asrs	r3, r3, #3
.L_020062c6:
	lsls	r0, r4, #12
	asrs	r7, r3, #3
	lsls	r0, r6, #12
	asrs	r2, r5, #3
	lsls	r0, r0, #13
.L_020062d0:
	movs	r0, #235
	lsls	r0, r2, #13
	asrs	r4, r5, #3
.L_020062d6:
	lsls	r0, r4, #13
.L_020062d8:
	asrs	r3, r7, #3
	lsls	r0, r6, #13
	asrs	r0, r6, #3
	lsls	r0, r0, #14
	add	r0, pc, #960
	lsls	r0, r2, #14
.L_020062e4:
	asrs	r2, r6, #3
	lsls	r0, r4, #14
.L_020062e8:
	strb	r2, [r1, #4]
	lsls	r0, r6, #14
	asrs	r4, r1, #4
.L_020062ee:
	lsls	r0, r0, #15
	asrs	r4, r1
.L_020062f2:
	lsls	r0, r2, #15
	asrs	r5, r1, #4
	lsls	r0, r4, #15
.L_020062f8:
	adds	r1, #15
	lsls	r0, r6, #15
	asrs	r4, r2, #4
	lsls	r0, r4, #20
	asrs	r3, r2, #4
	lsls	r0, r6, #20
.L_02006304:
	asrs	r7, r2, #4
	lsls	r0, r0, #21
	asrs	r7, r4, #1
.L_0200630a:
	lsls	r0, r2, #21
	movs	r0, #103
.L_0200630e:
	lsls	r0, r4, #21
	asrs	r2, r5, #4
	lsls	r0, r6, #21
	str	r5, [r2, r4]
	lsls	r0, r0, #22
	asrs	r6, r2, #4
	lsls	r0, r2, #22
	lsls	r7, r7, #7
	movs	r0, r0
	movs	r6, r5
.L_02006322:
	movs	r0, r0
	strh	r5, [r4, #20]
.L_02006326:
	lsls	r0, r0, #8
	movs	r1, r2
	movs	r0, r0
	movs	r6, r5
	movs	r0, r0
	strh	r5, [r1, #26]
	lsls	r0, r0, #8
.L_02006334:
	movs	r1, r2
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0xe320
	.2byte 0x0200
	.4byte 0x29900000
	.4byte 0x00000000
	.4byte 0x23730000
	.4byte 0x00020000
	.4byte 0x003700f3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x003800f3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x004c00f4
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x004f00f4
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x007800f6
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x003d00f3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x006500f5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00dd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002a000
	.4byte 0xffff009a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff00dd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x1a7600f0
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x1a7600f1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x1a7600f1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0038
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
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x07b00000
	.4byte 0x00000000
	.4byte 0x1d400000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x07900000
	.4byte 0x00000000
	.4byte 0x1d300000
	.4byte 0x0002c000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x07700000
	.4byte 0x00000000
	.4byte 0x1d400000
	.4byte 0x0002c000
	.4byte 0xffff0008
	.4byte 0x0200e320
	.4byte 0x079a0000
	.4byte 0x00000000
	.4byte 0x1d850000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x02008389
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x020083a9
	.4byte 0x0000002e
	.4byte 0x020083b5
	.4byte 0x00000011
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x2af80000
	.4byte 0x00000000
	.4byte 0x1d4c0000
	.4byte 0x00020000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x27100000
	.4byte 0x00000000
	.4byte 0x1d4c0000
	.4byte 0x00020000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x27100000
	.4byte 0x00000000
	.4byte 0x1f400000
	.4byte 0x00020000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x2af80000
	.4byte 0x00000000
	.4byte 0x1f400000
	.4byte 0x00020000
	.4byte 0xffff01b1
	.4byte 0x0200e578
	.4byte 0x27df0000
	.4byte 0x00000000
	.4byte 0x1edc0000
	.4byte 0x00024000
	.4byte 0xffff0115
	.4byte 0x0200e584
	.4byte 0x291c0000
	.4byte 0x00000000
	.4byte 0x1d1a0000
	.4byte 0x00024000
	.4byte 0xffff0115
	.4byte 0x0200e584
	.4byte 0x29240000
	.4byte 0x00000000
	.4byte 0x1d380000
	.4byte 0x01024000
	.4byte 0xffff0115
	.4byte 0x0200e584
	.4byte 0x292c0000
	.4byte 0x00000000
	.4byte 0x1d560000
	.4byte 0x01024000
	.4byte 0xffff0115
	.4byte 0x0200e584
	.4byte 0x29340000
	.4byte 0x00000000
	.4byte 0x1d740000
	.4byte 0x01024000
	.4byte 0xffff0115
	.4byte 0x0200e584
	.4byte 0x293c0000
	.4byte 0x00000000
	.4byte 0x1d920000
	.4byte 0x01024000
	.4byte 0xffff0115
	.4byte 0x0200e584
	.4byte 0x29440000
	.4byte 0x00000000
	.4byte 0x1db00000
	.4byte 0x01024000
	.4byte 0xffff0115
	.4byte 0x0200e584
	.4byte 0x294c0000
	.4byte 0x00000000
	.4byte 0x1dce0000
	.4byte 0x01024000
	.4byte 0xffff0115
	.4byte 0x0200e584
	.4byte 0x29540000
	.4byte 0x00000000
	.4byte 0x1dec0000
	.4byte 0x01024000
	.4byte 0xffff0008
	.4byte 0x0200e320
	.4byte 0x0dbb0000
	.4byte 0x00000000
	.4byte 0x1dc50000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0200e320
	.4byte 0x29900000
	.4byte 0x00000000
	.4byte 0x23730000
	.4byte 0x00020000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x29360000
	.4byte 0x00000000
	.4byte 0x21ea0000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x29360000
	.4byte 0x00000000
	.4byte 0x24420000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x2a620000
	.4byte 0x00000000
	.4byte 0x21ea0000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x2a620000
	.4byte 0x00000000
	.4byte 0x24420000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff01b1
	.4byte 0x0200e578
	.4byte 0x2f400000
	.4byte 0x00000000
	.4byte 0x20850000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0xffff00fb
	.4byte 0x02008575
	.4byte 0x00000006
	.4byte 0xffff00fc
	.4byte 0x020086d1
	.4byte 0x00000006
	.4byte 0xffff00fd
	.4byte 0x02008459
	.4byte 0x00000006
	.4byte 0xffff00fe
	.4byte 0x0200850d
	.4byte 0x00000002
	.4byte 0x0038005c
	.4byte 0x02008039
	.4byte 0x00000002
	.4byte 0x004c005d
	.4byte 0x0200809d
	.4byte 0x00000002
	.4byte 0x004f005e
	.4byte 0x02008101
	.4byte 0x00000002
	.4byte 0x0078005f
	.4byte 0x02008165
	.4byte 0x00000002
	.4byte 0x003d0060
	.4byte 0x020081c9
	.4byte 0x00000002
	.4byte 0x00650061
	.4byte 0x0200822d
	.4byte 0x00000003
	.4byte 0xffff0463
	.4byte 0x02008be1
	.4byte 0x00000002
	.4byte 0xffff0064
	.4byte 0x02008db1
	.4byte 0x00000001
	.4byte 0xffff0065
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0066
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0067
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0068
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x08de0069
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff006a
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff006b
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff006c
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff006d
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff006e
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff006f
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0070
	.4byte 0x00000013
	.4byte 0x00000001
	.4byte 0xffff0071
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0072
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0073
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0074
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0075
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0xffff0076
	.4byte 0x0000001a
	.4byte 0x00000001
	.4byte 0xffff0077
	.4byte 0x0000001b
	.4byte 0x00000001
	.4byte 0xffff0078
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0xffff0079
	.4byte 0x0000001d
	.4byte 0x00000001
	.4byte 0xffff007a
	.4byte 0x0000001e
	.4byte 0x00000001
	.4byte 0xffff007b
	.4byte 0x0000001f
	.4byte 0x00000001
	.4byte 0xffff007c
	.4byte 0x00000020
	.4byte 0x00000002
	.4byte 0xffff007d
	.4byte 0x02008da5
	.2byte 0x0001
	.2byte 0x0000
.L_02006980:
	lsls	r6, r7, #1
	.2byte 0xffff
	.2byte 0x0023
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r7, r7, #1
	.2byte 0xffff
	.2byte 0x0024
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
.L_02006998:
	lsls	r0, r0, #2
	.2byte 0xffff
	.2byte 0x0025
	movs	r0, r0
.L_020069a0:
	movs	r1, r0
.L_020069a2:
	movs	r0, r0
	lsls	r1, r0, #2
	.2byte 0xffff
	.2byte 0x0026
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r2, r0, #2
	.2byte 0xffff
	.2byte 0x0027
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r3, r0, #2
	.2byte 0xffff
	.2byte 0x0028
	movs	r0, r0
.L_020069c4:
	movs	r1, r0
	movs	r0, r0
	lsls	r4, r0, #2
	.2byte 0xffff
	.2byte 0x0029
.L_020069ce:
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r5, r0, #2
	.2byte 0xffff
	.2byte 0x002a
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r6, r0, #2
.L_020069e2:
	.2byte 0xffff
	.2byte 0x002b
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r0, r1, #2
	.2byte 0xffff
	.2byte 0x002d
	movs	r0, r0
.L_020069f4:
	movs	r1, r0
	movs	r0, r0
	lsls	r1, r1, #2
	.2byte 0xffff
	.2byte 0x002e
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
.L_02006a04:
	lsls	r2, r1, #2
.L_02006a06:
	.2byte 0xffff
	.2byte 0x002f
	movs	r0, r0
.L_02006a0c:
	movs	r1, r0
	movs	r0, r0
.L_02006a10:
	lsls	r3, r1, #2
.L_02006a12:
	.2byte 0xffff
	.2byte 0x0030
.L_02006a16:
	movs	r0, r0
	movs	r1, r0
.L_02006a1a:
	movs	r0, r0
	lsls	r4, r1, #2
.L_02006a1e:
	.2byte 0xffff
	.2byte 0x0031
	movs	r0, r0
.L_02006a24:
	movs	r1, r0
	movs	r0, r0
	lsls	r5, r1, #2
	.2byte 0xffff
	.2byte 0x0032
.L_02006a2e:
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r6, r1, #2
	.2byte 0xffff
	.2byte 0x0033
.L_02006a3a:
	movs	r0, r0
.L_02006a3c:
	movs	r1, r0
	movs	r0, r0
.L_02006a40:
	lsls	r7, r1, #2
.L_02006a42:
	.2byte 0xffff
	.2byte 0x0036
.L_02006a46:
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r0, r2, #2
	.2byte 0xffff
	.2byte 0x0037
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r1, r2, #2
.L_02006a5a:
	.2byte 0xffff
	.2byte 0x0038
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r2, r2, #2
	.2byte 0xffff
	.2byte 0x0039
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r3, r2, #2
	.2byte 0xffff
	.2byte 0x003a
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r4, r2, #2
	.2byte 0xffff
	.2byte 0x003b
	movs	r0, r0
	movs	r2, r0
	movs	r0, r0
.L_02006a88:
	lsls	r6, r2, #2
	.2byte 0xffff
	.2byte 0x8d99
	lsls	r0, r0, #8
	movs	r1, r0
	movs	r0, r0
	lsls	r7, r2, #2
	.2byte 0xffff
	.2byte 0x0052
.L_02006a9a:
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
.L_02006aa0:
	lsls	r0, r3, #2
	.2byte 0xffff
	.2byte 0x0053
.L_02006aa6:
	movs	r0, r0
	movs	r1, r0
.L_02006aaa:
	movs	r0, r0
.L_02006aac:
	lsls	r1, r3, #2
	.2byte 0xffff
	.2byte 0x0054
.L_02006ab2:
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
.L_02006ab8:
	lsls	r2, r3, #2
	.2byte 0xffff
	.2byte 0x0054
.L_02006abe:
	movs	r0, r0
	movs	r1, r0
.L_02006ac2:
	movs	r0, r0
.L_02006ac4:
	lsls	r3, r3, #2
.L_02006ac6:
	.2byte 0xffff
	.2byte 0x0057
	movs	r0, r0
	movs	r1, r0
.L_02006ace:
	movs	r0, r0
	lsls	r4, r3, #2
.L_02006ad2:
	.2byte 0xffff
	.2byte 0x0058
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
.L_02006adc:
	lsls	r5, r3, #2
	.2byte 0xffff
	.2byte 0x0059
	movs	r0, r0
	movs	r1, r0
.L_02006ae6:
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r7, r0
.L_02006af6:
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r2, r1
.L_02006b02:
	.2byte 0xffff
	.2byte 0x000a
	movs	r0, r0
	movs	r1, r0
.L_02006b0a:
	movs	r0, r0
.L_02006b0c:
	movs	r3, r1
	.2byte 0xffff
	.2byte 0x000b
	movs	r0, r0
.L_02006b14:
	movs	r1, r0
.L_02006b16:
	movs	r0, r0
	movs	r1, r2
	.2byte 0xffff
	.2byte 0x0011
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r2, r2
	.2byte 0xffff
	.2byte 0x0012
.L_02006b2a:
	movs	r0, r0
.L_02006b2c:
	movs	r1, r0
	movs	r0, r0
	movs	r2, r5
	.2byte 0xffff
	.2byte 0x002a
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r3, r5
	.2byte 0xffff
	.2byte 0x002b
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r4, r5
	.2byte 0xffff
	.2byte 0x002c
	movs	r0, r0
.L_02006b50:
	movs	r1, r0
	movs	r0, r0
	movs	r5, r5
	.2byte 0xffff
	.2byte 0x002d
.L_02006b5a:
	movs	r0, r0
	movs	r2, r0
	movs	r0, r0
	movs	r4, r6
	.2byte 0xffff
	.2byte 0x8919
	lsls	r0, r0, #8
	movs	r2, r0
.L_02006b6a:
	movs	r0, r0
	movs	r5, r6
	.2byte 0xffff
	.2byte 0x8995
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
.L_02006b78:
	movs	r0, r5
	lsrs	r0, r7, #9
	.2byte 0xb145
.L_02006b7e:
	lsls	r0, r0, #8
	movs	r1, r0
	movs	r0, r0
	movs	r4, r7
	.2byte 0xffff
	.2byte 0x003c
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r7
	.2byte 0xffff
	.2byte 0x003d
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r5, r2, #1
	.2byte 0xffff
	.2byte 0x0055
.L_02006ba2:
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	lsls	r6, r2, #1
	.2byte 0xffff
	.2byte 0x0056
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r3, r2
.L_02006bb6:
	.2byte 0xffff
	.2byte 0x8d1d
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r2
	movs	r0, r0
	movs	r6, r4
	movs	r0, r0
	movs	r6, r5
	movs	r0, r0
	push	{r0, r5, r6, r7, lr}
	lsls	r0, r0, #8
	movs	r1, r2
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
