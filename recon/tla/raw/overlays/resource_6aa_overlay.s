.syntax unified
	.thumb
	push	{lr}
	movs	r1, #0
	bl 0x0200cc40
	movs	r0, #0
	pop	{pc}
	push	{lr}
	movs	r0, #11
	movs	r1, #57
	bl 0x0200cd38
	pop	{pc}
	push	{lr}
	movs	r0, #17
	movs	r1, #36
	bl 0x0200cd38
	pop	{pc}
	.global Func_0200005c
	.thumb_func
Func_0200005c:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd928
	.2byte 0x0200
	.global Func_02000064
	.thumb_func
Func_02000064:
	movs	r0, #0
	bx	lr
	.global Func_02000068
	.thumb_func
Func_02000068:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd958
	.2byte 0x0200
	push	{lr}
	movs	r1, #65
	movs	r0, #0
	bl 0x0200a46c
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.global Func_02000080
	.thumb_func
Func_02000080:
	push	{lr}
	ldr	r3, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02000098
	ldr	r0, [pc, #72]
	b.n	.L_020000d6
.L_02000098:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020000a2
	ldr	r0, [pc, #72]
	b.n	.L_020000d6
.L_020000a2:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020000ac
	ldr	r0, [pc, #68]
	b.n	.L_020000d6
.L_020000ac:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_020000b6
	ldr	r0, [pc, #68]
	b.n	.L_020000d6
.L_020000b6:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_020000c0
	ldr	r0, [pc, #64]
	b.n	.L_020000d6
.L_020000c0:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_020000ca
	ldr	r0, [pc, #64]
	b.n	.L_020000d6
.L_020000ca:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_020000d4
	ldr	r0, [pc, #60]
	b.n	.L_020000d6
.L_020000d4:
	ldr	r0, [pc, #60]
.L_020000d6:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000117
	.4byte 0x0200db0c
	.4byte 0x00000118
	.4byte 0x0200db54
	.4byte 0x00000119
	.4byte 0x0200dbcc
	.4byte 0x0000011a
	.4byte 0x0200dd4c
	.4byte 0x0000011b
	.4byte 0x0200deb4
	.4byte 0x0000011c
	.4byte 0x0200df74
	.4byte 0x0000011d
	.4byte 0x0200e0c4
	.4byte 0x0200daf4
	.4byte 0x049b23c0
	.4byte 0x22d06edb
	.4byte 0x32380112
	.4byte 0x2201189b
	.2byte 0x701a
	.2byte 0x4770
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_0200013c
	movs	r0, #0
	b.n	.L_02000162
.L_0200013c:
	cmp	r0, #2
	bhi.n	.L_02000150
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_02000152
.L_02000150:
	ldr	r4, [pc, #16]
.L_02000152:
	lsls	r3, r2, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
.L_02000162:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r1, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_0200017c
	movs	r0, #0
	b.n	.L_020001a8
.L_0200017c:
	cmp	r0, #2
	bhi.n	.L_02000190
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_02000192
.L_02000190:
	ldr	r4, [pc, #24]
.L_02000192:
	lsls	r3, r2, #7
	adds	r3, r5, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
	asrs	r3, r1, #8
	strb	r3, [r4, #2]
	strb	r1, [r4, #3]
.L_020001a8:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_020001f4
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020001f4
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
.L_020001f4:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	movs	r0, #0
	asrs	r3, r3, #20
	mov	r9, r3
	ldr	r3, [r5, #16]
	mov	r1, r9
	asrs	r3, r3, #20
	mov	sl, r3
	mov	r2, sl
	bl 0x0200812c
	mov	r1, r9
	mov	r2, sl
	mov	r8, r0
	movs	r0, #2
	bl 0x0200812c
	movs	r2, #34
	adds	r2, r2, r5
	adds	r6, r0, #0
	mov	fp, r2
	ldrb	r0, [r2, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x0200cc08
	adds	r3, r5, #0
	adds	r3, #100
	asrs	r7, r0, #19
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_02000268
	ldr	r3, [pc, #104]
	mov	r2, r8
	ands	r6, r3
	movs	r3, #129
	negs	r3, r3
	ands	r2, r3
	ldr	r3, [r5, #20]
	mov	r8, r2
	asrs	r3, r3, #19
	cmp	r3, r7
	beq.n	.L_02000282
	subs	r7, #4
	b.n	.L_02000282
.L_02000268:
	movs	r3, #255
	ands	r6, r3
	lsls	r3, r3, #8
	mov	r2, r8
	orrs	r6, r3
	movs	r3, #128
	orrs	r2, r3
	adds	r0, r5, #0
	movs	r1, #2
	mov	r8, r2
	adds	r7, #4
	bl 0x020081b0
.L_02000282:
	mov	r1, r9
	mov	r2, sl
	mov	r3, r8
	movs	r0, #0
	bl 0x02008168
	mov	r1, r9
	mov	r2, sl
	adds	r3, r6, #0
	movs	r0, #2
	bl 0x02008168
	mov	r3, r9
	mov	r2, sl
	lsls	r0, r3, #20
	mov	r3, fp
	lsls	r1, r2, #20
	ldrb	r2, [r3, #0]
	adds	r3, r7, #0
	bl 0x0200cc80
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x00ff
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_02000330
	adds	r7, r0, #0
.L_020002d0:
	ldrh	r0, [r7, #0]
	bl 0x0200ccb8
	movs	r3, #4
	ldrsh	r2, [r7, r3]
	movs	r1, #0
	mov	r8, r2
	adds	r6, r0, #0
	movs	r3, #2
	ldrsh	r5, [r7, r3]
	bl 0x0200cc40
	mov	r2, r8
	lsls	r0, r2, #16
	lsrs	r0, r0, #16
	bl 0x0200cb90
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r5, r5, r0
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl 0x0200cbc0
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r6, #0
	adds	r1, #89
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r6, #0
	adds	r3, #100
	mov	r2, r8
	strh	r2, [r3, #0]
	adds	r0, r6, #0
	adds	r7, #6
	bl 0x020081f8
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_020002d0
.L_02000330:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
.L_02000348:
	ldr	r3, [r3, #0]
	mov	r9, r0
	movs	r2, #26
	ldrsh	r0, [r3, r2]
	sub	sp, #56
	adds	r6, r1, #0
.L_02000354:
	mov	sl, r3
	bl 0x0200ccb8
	adds	r7, r0, #0
	cmp	r6, #7
	bgt.n	.L_020003cc
	add	r5, sp, #16
	movs	r3, #0
	mov	r8, r3
	str	r3, [r5, #0]
	movs	r3, #24
	adds	r3, #255
	strh	r3, [r5, #24]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #12]
	str	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #20]
	str	r3, [r5, #16]
	bl 0x0200cb30
	ldr	r3, [pc, #60]
	ands	r0, r3
	lsls	r0, r0, #12
	strh	r0, [r5, #32]
	bl 0x0200cb30
	ldr	r1, [r7, #12]
	lsls	r2, r6, #1
	adds	r2, r2, r6
	lsls	r2, r2, #16
	movs	r4, #128
	subs	r1, r1, r2
	lsls	r4, r4, #13
	movs	r3, #15
	ands	r3, r0
	adds	r1, r1, r4
	mov	r4, r8
	ldr	r2, [r7, #16]
	ldr	r0, [r7, #8]
	subs	r3, #8
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	movs	r4, #180
	lsls	r3, r3, #13
	lsls	r4, r4, #15
	str	r4, [sp, #8]
	str	r5, [sp, #12]
	bl 0x0200bb30
	b.n	.L_020003c4
	.2byte 0x0000
	.2byte 0x000f
	.2byte 0x0000
.L_020003c4:
	ldr	r3, [r7, #28]
	ldr	r2, [pc, #64]
	adds	r3, r3, r2
	str	r3, [r7, #28]
.L_020003cc:
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L_020003fa
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #28]
	adds	r3, r7, #0
	adds	r3, #100
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl 0x0200cb98
	mov	r3, sl
	movs	r2, #26
	ldrsh	r0, [r3, r2]
	bl 0x0200ccb8
	bl 0x020081f8
	movs	r3, #0
.L_020003f4:
	str	r3, [r7, #16]
	str	r3, [r7, #12]
	str	r3, [r7, #8]
.L_020003fa:
	add	sp, #56
	pop	{r3, r5, r6}
	mov	r8, r3
.L_02000400:
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb500
	bl 0x02008338
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	bl 0x0200ccb8
	movs	r1, #5
	mov	r9, r0
	mov	fp, r1
.L_0200042c:
	bl 0x0200cb30
.L_02000430:
	adds	r5, r0, #0
	bl 0x0200cb30
	mov	r2, r9
	ldr	r2, [r2, #8]
	lsls	r5, r5, #4
	mov	r8, r2
	add	r8, r5
	lsls	r0, r0, #4
	mov	r3, r8
	subs	r3, r3, r0
	mov	r8, r3
	bl 0x0200cb30
	adds	r6, r0, #0
	bl 0x0200cb30
	adds	r5, r0, #0
	bl 0x0200cb30
	mov	r1, r9
	ldr	r2, [r1, #12]
	ldr	r3, [r1, #16]
	lsls	r6, r6, #3
	adds	r6, r6, r2
	lsls	r5, r5, #4
	movs	r2, #128
	lsls	r0, r0, #4
	lsls	r2, r2, #11
	adds	r3, r3, r5
	subs	r3, r3, r0
	adds	r6, r6, r2
	movs	r0, #70
	adds	r0, #255
	mov	r1, r8
	adds	r2, r6, #0
	bl 0x0200cbd8
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_020004fc
	bl 0x0200cb30
	mov	r8, r0
	bl 0x0200cb30
	adds	r6, r0, #0
	bl 0x0200cb30
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #7
	adds	r0, r7, #0
	ldr	r1, [pc, #120]
	lsrs	r5, r5, #1
	adds	r5, r5, r3
	bl 0x0200cbd0
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cc40
	mov	r1, r8
	movs	r2, #128
	lsls	r2, r2, #10
	lsls	r3, r1, #2
	adds	r3, r3, r2
	str	r3, [r7, #40]
	mov	r0, r8
	bl 0x0200cb48
	ldr	r3, [pc, #88]
	lsls	r6, r6, #3
	adds	r1, r0, #0
	mov	sl, r3
	adds	r0, r6, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x62f8
	mov	r0, r8
	bl 0x0200cb40
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [r7, #52]
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r0, [r7, #36]
	str	r5, [r7, #24]
	str	r5, [r7, #28]
	str	r3, [r7, #68]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200cc60
.L_020004fc:
	movs	r1, #1
	negs	r1, r1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #0
	bge.n	.L_0200042c
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200d394
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, lr}
	movs	r0, #143
	lsls	r0, r0, #2
	sub	sp, #8
	bl 0x0200cde0
	movs	r6, #32
	movs	r5, #6
.L_0200052c:
	movs	r3, #39
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200cc30
	movs	r0, #10
	bl 0x02008414
	subs	r5, #1
	movs	r0, #10
	bl 0x0200cb18
	adds	r6, #3
	cmp	r5, #0
	bge.n	.L_0200052c
	bl 0x0200cdb0
	movs	r0, #20
	bl 0x0200cd30
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	ldr	r5, [pc, #36]
	movs	r1, #1
	adds	r0, r5, #0
	adds	r5, #1
	bl 0x0200cc90
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200cc90
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
	strh	r3, [r2, #0]
	pop	{r5, pc}
	.2byte 0x2e6e
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #54
	bl 0x0200cb90
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0200061c
	bl 0x0200c6b4
	bl 0x0200cca8
	movs	r0, #0
	bl 0x0200cda0
	bl 0x0200cdd0
	ldr	r5, [pc, #140]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200ccb8
	str	r6, [r0, #108]
	ldr	r0, [r5, #0]
	bl 0x0200ccb8
	movs	r1, #0
	bl 0x0200ccf0
	movs	r1, #172
	movs	r2, #180
	ldr	r0, [r5, #0]
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200ccd8
	movs	r0, #172
	movs	r1, #1
	movs	r2, #180
	negs	r1, r1
	lsls	r2, r2, #17
.L_020005e8:
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200cd18
	bl 0x0200cd20
	bl 0x0200cdc8
	movs	r0, #40
	bl 0x0200cca0
	bl 0x0200c718
	bl 0x0200cd90
	movs	r0, #10
	bl 0x0200cb18
	bl 0x0200ccb0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #54
	bl 0x0200cb98
	b.n	.L_02000642
.L_0200061c:
	bl 0x0200c6b4
	bl 0x0200cdd0
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #18
	bl 0x0200cd30
	movs	r0, #40
	bl 0x0200cca0
	bl 0x0200c718
.L_02000642:
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200c6b4
	bl 0x0200cdd0
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #17
	bl 0x0200cd30
	movs	r0, #40
	bl 0x0200cca0
	bl 0x0200c718
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	adds	r5, r1, #0
	cmp	r0, #1
	bne.n	.L_020006d8
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #62
	bl 0x0200cde0
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200cd58
	movs	r0, #60
	bl 0x0200cd60
	movs	r3, #47
	movs	r2, #100
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #47
	movs	r1, #94
	movs	r2, #10
	movs	r3, #6
	bl 0x0200cc30
	movs	r3, #46
	movs	r2, #40
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #110
	movs	r1, #40
	movs	r2, #12
	movs	r3, #4
	bl 0x0200cc28
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ccd8
	movs	r0, #148
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200cb98
.L_020006d8:
	adds	r3, r5, #0
	subs	r3, #97
	cmp	r3, #174
	bhi.n	.L_020006f2
	adds	r0, r5, #0
	movs	r1, #30
	bl 0x0200cb10
	cmp	r0, #0
	bne.n	.L_020006f2
	movs	r0, #18
	bl 0x02008414
.L_020006f2:
	movs	r3, #138
	lsls	r3, r3, #1
	cmp	r5, r3
	bne.n	.L_0200070c
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200cd58
	movs	r0, #20
	bl 0x0200cd60
.L_0200070c:
	add	sp, #8
	pop	{r5, pc}
	push	{lr}
	bl 0x02009a00
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r0, #131
	movs	r3, #0
	strh	r3, [r2, #0]
	lsls	r0, r0, #1
	bl 0x0200cba0
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200cba0
	bl 0x0200cca8
	movs	r0, #0
	bl 0x0200cda0
	movs	r0, #128
	movs	r1, #128
.L_02000748:
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200cd10
	movs	r0, #168
	movs	r2, #166
	lsls	r2, r2, #18
	movs	r3, #1
	movs	r1, #0
	lsls	r0, r0, #16
	bl 0x0200cd18
	bl 0x0200cd20
	movs	r0, #10
	bl 0x0200cb18
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200cd10
	movs	r0, #236
	movs	r2, #166
	movs	r1, #0
	lsls	r2, r2, #18
	movs	r3, #1
	lsls	r0, r0, #17
	bl 0x0200cd18
	bl 0x0200cd20
	movs	r0, #90
	bl 0x0200cca0
	bl 0x0200ccb0
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #8
	movs	r6, #14
	movs	r5, #41
	movs	r0, #14
	movs	r1, #55
	movs	r2, #14
	movs	r3, #3
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200cc28
	movs	r3, #78
	str	r3, [sp, #0]
	mov	r8, r3
	movs	r0, #64
	movs	r1, #64
	movs	r2, #14
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200cc30
	movs	r3, #104
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #14
	movs	r3, #3
	str	r6, [sp, #0]
	bl 0x0200cc30
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #14
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200cc28
	movs	r3, #105
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #14
	movs	r3, #3
	str	r6, [sp, #0]
	bl 0x0200cc28
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #20
	bl 0x0200ccb8
	adds	r5, r0, #0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #71
	bl 0x0200cb90
	cmp	r0, #0
	bne.n	.L_0200085a
	movs	r0, #159
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200cde0
	movs	r0, #153
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200cb98
	adds	r3, r5, #0
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r0, #160
	lsls	r3, r3, #16
	str	r3, [r5, #8]
	adds	r3, r5, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r0, r0, #4
	lsls	r3, r3, #16
	str	r3, [r5, #16]
	adds	r0, #72
	bl 0x0200cb98
	bl 0x02008798
.L_0200085a:
	pop	{r5, pc}
	push	{r5, r6, lr}
	sub	sp, #8
	adds	r6, r1, #0
	cmp	r0, #1
	bne.n	.L_020008ba
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #62
	bl 0x0200cde0
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200cd58
	movs	r0, #60
	bl 0x0200cd60
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r5, #28
	movs	r0, #28
	movs	r1, #92
	movs	r2, #8
	movs	r3, #8
	str	r5, [sp, #0]
	bl 0x0200cc30
	movs	r3, #38
	str	r3, [sp, #4]
	movs	r0, #92
	movs	r1, #38
	movs	r2, #8
	movs	r3, #8
	str	r5, [sp, #0]
	bl 0x0200cc28
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ccd8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #71
	bl 0x0200cb98
.L_020008ba:
	adds	r3, r6, #0
	subs	r3, #97
	cmp	r3, #174
	bhi.n	.L_020008d4
	adds	r0, r6, #0
	movs	r1, #30
	bl 0x0200cb10
	cmp	r0, #0
	bne.n	.L_020008d4
	movs	r0, #20
	bl 0x02008414
.L_020008d4:
	movs	r3, #138
	lsls	r3, r3, #1
	cmp	r6, r3
	bne.n	.L_020008ee
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200cd58
	movs	r0, #20
	bl 0x0200cd60
.L_020008ee:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	cmp	r1, #8
	bne.n	.L_0200092c
	movs	r0, #8
	bl 0x0200ccb8
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #20
	bne.n	.L_0200092c
	movs	r0, #5
	bl 0x0200cca0
	ldr	r3, [r5, #16]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	movs	r0, #149
	adds	r2, r5, #0
	str	r3, [r5, #16]
	adds	r2, #98
	movs	r3, #1
	lsls	r0, r0, #4
	strb	r3, [r2, #0]
	adds	r0, #255
	bl 0x0200cb98
.L_0200092c:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200cb98
	pop	{pc}
	push	{lr}
	bl 0x0200c6b4
	bl 0x0200cdd0
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #14
	bl 0x0200cd30
	movs	r0, #40
	bl 0x0200cca0
	bl 0x0200c718
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	adds	r0, r1, #0
	bl 0x0200ccb8
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #11
	bne.n	.L_02000998
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r5, #52]
	movs	r1, #152
	movs	r3, #198
	ldr	r2, [r5, #12]
	lsls	r1, r1, #16
	lsls	r3, r3, #18
	bl 0x0200cbf8
.L_02000998:
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #10
	bne.n	.L_020009bc
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r5, #52]
	movs	r1, #200
	movs	r3, #198
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	lsls	r1, r1, #16
	lsls	r3, r3, #18
	bl 0x0200cbf8
.L_020009bc:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200cd10
	movs	r0, #176
	movs	r1, #1
	movs	r2, #174
	lsls	r0, r0, #15
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #1
	bl 0x0200cd18
	bl 0x020099c4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r0, #131
	movs	r3, #0
	strh	r3, [r2, #0]
	lsls	r0, r0, #1
	bl 0x0200cba0
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200cba0
	bl 0x0200cca8
	movs	r0, #0
	bl 0x0200cda0
	bl 0x0200cd20
	movs	r0, #120
	bl 0x0200cca0
	bl 0x0200ccb0
	pop	{pc}
	push	{r5, lr}
	movs	r0, #12
	bl 0x0200ccb8
	adds	r5, r0, #0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #77
	bl 0x0200cb90
	cmp	r0, #0
	bne.n	.L_02000a72
	movs	r0, #159
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200cde0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #50
	bl 0x0200cb98
	adds	r3, r5, #0
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r0, #160
	lsls	r3, r3, #16
	str	r3, [r5, #8]
	adds	r3, r5, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r0, r0, #4
	lsls	r3, r3, #16
	str	r3, [r5, #16]
	adds	r0, #76
	bl 0x0200cb98
.L_02000a72:
	pop	{r5, pc}
	push	{lr}
	bl 0x020099e0
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r0, #131
	movs	r3, #0
	strh	r3, [r2, #0]
	lsls	r0, r0, #1
	bl 0x0200cba0
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200cba0
	bl 0x0200cca8
	movs	r0, #0
	bl 0x0200cda0
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200cd10
	movs	r0, #218
	movs	r1, #1
	movs	r2, #168
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200cd18
	bl 0x0200cd20
	movs	r0, #40
	bl 0x0200cca0
	movs	r0, #218
	movs	r1, #1
	movs	r2, #132
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200cd18
	bl 0x0200cd20
	movs	r0, #120
	bl 0x0200cca0
	bl 0x0200ccb0
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x0200c6b4
	bl 0x0200cdd0
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #2
.L_02000b04:
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #14
	bl 0x0200cd30
	movs	r0, #40
	bl 0x0200cca0
	bl 0x0200c718
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #53
	str	r3, [sp, #0]
	movs	r5, #48
	movs	r0, #53
	movs	r1, #30
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200cc20
	movs	r3, #117
	str	r3, [sp, #0]
	movs	r0, #117
	movs	r1, #30
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200cc30
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #53
	str	r3, [sp, #0]
	movs	r5, #48
	movs	r0, #53
	movs	r1, #40
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200cc20
	movs	r3, #117
	str	r3, [sp, #0]
	movs	r0, #117
	movs	r1, #45
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200cc30
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	sub	sp, #8
	adds	r6, r1, #0
	cmp	r0, #1
	bne.n	.L_02000be2
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #62
	bl 0x0200cde0
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200cd58
	movs	r0, #60
	bl 0x0200cd60
	movs	r3, #101
	str	r3, [sp, #4]
	movs	r5, #12
	movs	r0, #12
	movs	r1, #95
	movs	r2, #8
	movs	r3, #6
	str	r5, [sp, #0]
	bl 0x0200cc30
	movs	r3, #40
	str	r3, [sp, #4]
	movs	r0, #76
	movs	r1, #40
	movs	r2, #8
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200cc28
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ccd8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #77
	bl 0x0200cb98
.L_02000be2:
	adds	r3, r6, #0
	subs	r3, #97
	cmp	r3, #174
	bhi.n	.L_02000bfc
	adds	r0, r6, #0
	movs	r1, #30
	bl 0x0200cb10
	cmp	r0, #0
	bne.n	.L_02000bfc
	movs	r0, #12
	bl 0x02008414
.L_02000bfc:
	movs	r3, #138
	lsls	r3, r3, #1
	cmp	r6, r3
	bne.n	.L_02000c16
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200cd58
	movs	r0, #20
	bl 0x0200cd60
.L_02000c16:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	sub	sp, #8
	movs	r6, #53
	movs	r5, #11
	movs	r0, #35
	movs	r1, #11
	movs	r2, #3
	movs	r3, #6
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200cc28
	movs	r3, #117
	str	r3, [sp, #0]
	movs	r0, #35
	movs	r1, #75
	movs	r2, #3
	movs	r3, #6
	str	r5, [sp, #4]
	bl 0x0200cc30
	movs	r3, #75
	str	r3, [sp, #4]
	movs	r1, #75
	movs	r2, #3
	movs	r3, #6
	movs	r0, #35
	str	r6, [sp, #0]
	bl 0x0200cc30
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200cb98
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #26
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #25
	movs	r1, #9
	movs	r2, #11
	movs	r3, #3
	bl 0x0200cc28
	add	sp, #8
	pop	{pc}
	push	{lr}
	bl 0x0200c6b4
	bl 0x0200cdd0
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #12
	bl 0x0200cd30
	movs	r0, #40
	bl 0x0200cca0
	bl 0x0200c718
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200c6b4
	bl 0x0200cdd0
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #13
	bl 0x0200cd30
	movs	r0, #40
	bl 0x0200cca0
	bl 0x0200c718
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
.L_02000ce6:
	cmp	r1, #8
	bne.n	.L_02000d14
	movs	r0, #8
	bl 0x0200ccb8
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #5
	bne.n	.L_02000d14
	movs	r0, #5
	bl 0x0200cca0
	ldr	r3, [r5, #16]
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r0, #160
	adds	r3, r3, r2
	lsls	r0, r0, #4
	str	r3, [r5, #16]
	adds	r0, #78
	bl 0x0200cb98
.L_02000d14:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r3, [pc, #176]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r6, [r2, #0]
	ldr	r2, [r2, #4]
	ands	r6, r3
	ands	r2, r3
	ldr	r3, [r1, #0]
	mov	r8, r2
	ldr	r3, [r3, #4]
	ldr	r2, [pc, #160]
	mov	sl, r3
	ldr	r3, [pc, #160]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	movs	r2, #133
	lsrs	r7, r3, #5
	ldr	r3, [pc, #148]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
	adds	r5, r0, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	asrs	r3, r1, #20
	lsls	r3, r3, #20
	movs	r0, #0
	subs	r6, r3, r6
	bl 0x0200cc08
	mov	r3, sl
	subs	r0, r0, r3
	ldr	r3, [r5, #16]
	mov	r2, r8
	asrs	r3, r3, #20
	lsls	r3, r3, #20
	subs	r3, r3, r2
	mov	r2, sl
	subs	r3, r3, r2
	subs	r4, r3, r0
	asrs	r2, r4, #16
	adds	r0, r0, r3
	asrs	r6, r6, #16
	adds	r4, r2, #0
	asrs	r0, r0, #16
	adds	r3, r6, #0
	movs	r2, #167
	adds	r1, r0, #0
	adds	r3, #15
	lsls	r2, r2, #1
	adds	r4, #16
	adds	r1, #58
	cmp	r3, r2
	bhi.n	.L_02000dd0
	movs	r3, #15
	negs	r3, r3
	cmp	r4, r3
	blt.n	.L_02000dd0
	cmp	r4, #239
	bgt.n	.L_02000dd0
	movs	r3, #128
	lsls	r3, r3, #1
	ldr	r2, [pc, #60]
	adds	r3, #255
	ands	r6, r3
	movs	r3, #255
	ands	r4, r3
	movs	r3, #0
	stmia	r2!, {r3}
	lsls	r3, r6, #16
	orrs	r4, r3
	ldr	r3, [pc, #44]
	ldr	r0, [pc, #40]
	orrs	r4, r3
	stmia	r2!, {r4}
	movs	r3, #128
	lsls	r3, r3, #3
	orrs	r7, r3
	str	r7, [r2, #0]
	bl 0x0200cb80
.L_02000dd0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xffff0000
	.4byte 0x020036e0
	.4byte 0x0200e8a2
	.4byte 0x02000240
	.4byte 0x0200e8a4
	.2byte 0x8800
	.2byte 0x8000
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #1
	bl 0x0200cb58
	adds	r6, r0, #0
	adds	r1, r6, #0
	movs	r2, #63
.L_02000e00:
	ldr	r3, [pc, #44]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bge.n	.L_02000e00
	ldr	r5, [pc, #40]
	bl 0x0200cb78
	strh	r0, [r5, #0]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r2, r6, #0
	ldrh	r0, [r5, #0]
	bl 0x0200cb70
	adds	r0, r6, #0
	bl 0x0200cb60
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl 0x0200cb20
	pop	{r5, r6, pc}
	.4byte 0x11111111
	.4byte 0x0200e8a2
	.2byte 0x8d19
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #140]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r5, r1
	adds	r7, r0, #0
	ldr	r0, [r5, #0]
	bl 0x0200ccb8
	adds	r6, r0, #0
	bl 0x0200cca8
	movs	r0, #0
	bl 0x0200cda0
	ldr	r0, [r5, #0]
	bl 0x0200ccb8
	movs	r1, #0
	bl 0x0200cc40
	movs	r3, #0
	movs	r1, #128
	str	r3, [r6, #108]
	movs	r2, #0
	lsls	r1, r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200cd00
	adds	r0, r6, #0
	movs	r1, #27
	bl 0x0200cbc0
	movs	r1, #48
	adds	r0, r6, #0
	bl 0x0200cbc8
	movs	r0, #30
	bl 0x0200cb18
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	ldr	r2, [pc, #52]
	ldr	r3, [r6, #8]
	asrs	r4, r3, #20
	ldr	r3, [r6, #16]
	asrs	r0, r3, #20
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	cmp	r1, #0
	beq.n	.L_02000ed4
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	movs	r2, #3
	ands	r2, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [r1, r3]
	b.n	.L_02000ed4
	.4byte 0x00000000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0x0201
.L_02000ed4:
	lsls	r3, r0, #7
	adds	r3, r4, r3
	lsls	r3, r3, #2
	movs	r1, #128
	adds	r3, r2, r3
	lsls	r1, r1, #2
	adds	r2, r3, r1
	ldrb	r2, [r2, #3]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000f0a
	movs	r2, #226
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #19
	adds	r2, #255
	adds	r3, #74
	strh	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #52]
	orrs	r3, r2
	strh	r3, [r1, #0]
	bl 0x02008df0
.L_02000f0a:
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #28
	bl 0x0200cce0
	movs	r0, #16
	bl 0x0200cca0
	movs	r5, #0
.L_02000f22:
	cmp	r5, #5
	bne.n	.L_02000f2c
	movs	r0, #204
	bl 0x0200cde0
.L_02000f2c:
	ldr	r3, [r6, #24]
	ldr	r1, [pc, #16]
	ldr	r2, [pc, #16]
	adds	r3, r3, r1
	str	r3, [r6, #24]
	b.n	.L_02000f48
	.4byte 0x00008000
	.4byte 0x02000240
	.4byte 0xfffffc00
	.2byte 0xfd00
	.2byte 0xffff
.L_02000f48:
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #80]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	adds	r5, #1
	bl 0x0200cb18
	cmp	r5, #39
	ble.n	.L_02000f22
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
	movs	r3, #0
	adds	r0, #84
	strb	r3, [r0, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #133
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	adds	r2, #255
	str	r2, [r3, #0]
	bl 0x0200cd70
	bl 0x0200cd78
	adds	r0, r7, #0
	bl 0x0200cd30
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xffff6667
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #146
	lsls	r0, r0, #2
	bl 0x0200cde0
	movs	r0, #126
	adds	r0, #255
	bl 0x0200cb98
	ldr	r1, [pc, #24]
	movs	r0, #11
	bl 0x0200ccc8
	movs	r1, #3
	movs	r0, #0
	bl 0x0200a46c
	movs	r0, #141
	lsls	r0, r0, #2
	bl 0x0200cb98
	pop	{pc}
	.2byte 0xd03c
	.2byte 0x0200
	push	{lr}
	bl 0x0200b564
	bl 0x0200a9a0
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #172]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r2, r2, r3
.L_02000ff0:
	mov	sl, r0
	ldr	r0, [r2, #0]
	mov	r8, r2
	bl 0x0200ccb8
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	ldr	r5, [r6, #16]
	asrs	r7, r3, #20
	movs	r0, #208
	lsls	r0, r0, #2
	adds	r1, r7, #0
	bl 0x0200cbb0
	asrs	r5, r5, #20
.L_0200100e:
	movs	r0, #210
	adds	r1, r5, #0
	lsls	r0, r0, #2
	bl 0x0200cbb0
	bl 0x0200cca8
	movs	r0, #0
	bl 0x0200cda0
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
	movs	r1, #0
	bl 0x0200cc40
	movs	r3, #0
	mov	r2, r8
	movs	r1, #128
	str	r3, [r6, #108]
	ldr	r0, [r2, #0]
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200cd00
	adds	r0, r6, #0
	movs	r1, #27
	bl 0x0200cbc0
	adds	r0, r6, #0
	movs	r1, #48
	bl 0x0200cbc8
	movs	r0, #30
	bl 0x0200cb18
	movs	r3, #128
	ldr	r2, [pc, #52]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	ldrb	r2, [r3, #3]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020010a0
	movs	r2, #226
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #19
	adds	r2, #255
	adds	r3, #74
	strh	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #12]
	orrs	r3, r2
	strh	r3, [r1, #0]
	b.n	.L_0200109c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00008000
	.2byte 0x0240
	.2byte 0x0200
.L_0200109c:
	bl 0x02008df0
.L_020010a0:
	mov	r2, r8
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl 0x0200cce0
	movs	r0, #16
	bl 0x0200cca0
	movs	r7, #0
.L_020010b2:
	cmp	r7, #5
	bne.n	.L_020010bc
	movs	r0, #204
	bl 0x0200cde0
.L_020010bc:
	ldr	r3, [r6, #24]
	ldr	r2, [pc, #92]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [pc, #88]
	ldr	r3, [r6, #28]
	adds	r7, #1
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r2, [pc, #80]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	bl 0x0200cb18
	cmp	r7, #39
	ble.n	.L_020010b2
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
	movs	r3, #0
	adds	r0, #84
	strb	r3, [r0, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200cd70
	bl 0x0200cd78
	movs	r0, #11
	bl 0x0200cd30
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0xfffffd00
	.4byte 0xffff6667
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200cd40
	bl 0x0200b884
	pop	{pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	ldr	r3, [pc, #80]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
	ldr	r3, [r0, #8]
	ldr	r1, [pc, #68]
	asrs	r6, r3, #20
	ldr	r3, [r0, #16]
	asrs	r4, r3, #20
	cmp	r5, #0
	beq.n	.L_02001174
	adds	r3, r0, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	movs	r2, #3
	ands	r2, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r3, r3, #3
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r1, [r5, r3]
.L_02001174:
	lsls	r3, r4, #7
	adds	r3, r6, r3
	lsls	r3, r3, #2
	adds	r1, r1, r3
	ldrb	r2, [r1, #3]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001190
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r0, r1, r3
	bl 0x02008fe0
.L_02001190:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0x0201
	.global Func_0200119c
	.thumb_func
Func_0200119c:
	push	{lr}
	ldr	r3, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_020011b4
	ldr	r0, [pc, #72]
	b.n	.L_020011f2
.L_020011b4:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020011be
	ldr	r0, [pc, #72]
	b.n	.L_020011f2
.L_020011be:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020011c8
	ldr	r0, [pc, #68]
	b.n	.L_020011f2
.L_020011c8:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_020011d2
	ldr	r0, [pc, #68]
	b.n	.L_020011f2
.L_020011d2:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_020011dc
	ldr	r0, [pc, #64]
	b.n	.L_020011f2
.L_020011dc:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_020011e6
	ldr	r0, [pc, #64]
	b.n	.L_020011f2
.L_020011e6:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_020011f0
	ldr	r0, [pc, #60]
	b.n	.L_020011f2
.L_020011f0:
	ldr	r0, [pc, #60]
.L_020011f2:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000117
	.4byte 0x0200e118
	.4byte 0x00000118
	.4byte 0x0200e13c
	.4byte 0x00000119
	.4byte 0x0200e250
	.4byte 0x0000011a
	.4byte 0x0200e3b8
	.4byte 0x0000011b
	.4byte 0x0200e4fc
	.4byte 0x0000011c
	.4byte 0x0200e628
	.4byte 0x0000011d
	.4byte 0x0200e778
	.2byte 0xe10c
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200ccb8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001252
	movs	r1, #0
	bl 0x0200cc40
	adds	r3, r5, #0
	adds	r3, #89
	movs	r2, #0
	strb	r2, [r3, #0]
	subs	r3, #4
	strb	r2, [r3, #0]
.L_02001252:
	pop	{r5, pc}
	push	{r5, lr}
	ldr	r3, [pc, #36]
	ldr	r5, [pc, #36]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #59
	bgt.n	.L_02001284
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_02001298
	movs	r0, #0
	movs	r1, #3
	bl 0x0200a46c
	ldr	r3, [pc, #4]
	b.n	.L_02001296
	.2byte 0x0000
	.4byte 0x00000001
	.4byte 0x0200e8a0
	.2byte 0xe868
	.2byte 0x0200
.L_02001284:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #1
	bne.n	.L_02001298
	movs	r0, #0
	movs	r1, #67
	bl 0x0200a46c
	ldr	r3, [pc, #24]
.L_02001296:
	strh	r3, [r5, #0]
.L_02001298:
	ldr	r2, [pc, #24]
	movs	r1, #150
	ldrh	r3, [r2, #0]
	lsls	r1, r1, #17
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	cmp	r3, r1
	ble.n	.L_020012b8
	ldr	r3, [pc, #4]
	strh	r3, [r2, #0]
	b.n	.L_020012b8
	.4byte 0x00000000
	.2byte 0xe8a0
	.2byte 0x0200
.L_020012b8:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r2, [r3, #0]
	movs	r3, #7
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020012d0
	cmp	r2, #6
	beq.n	.L_020012d8
	b.n	.L_020012de
.L_020012d0:
	movs	r1, #0
	bl 0x0200cc60
	b.n	.L_020012de
.L_020012d8:
	movs	r1, #10
	bl 0x0200cc60
.L_020012de:
	pop	{pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	ldr	r2, [r0, #80]
	movs	r1, #128
	ldrh	r3, [r2, #18]
	lsls	r1, r1, #4
	adds	r3, r3, r1
	adds	r1, r0, #0
	adds	r1, #100
	strh	r3, [r2, #18]
	ldrh	r3, [r1, #0]
	movs	r2, #128
	adds	r3, #1
	strh	r3, [r1, #0]
	lsls	r2, r2, #12
	lsls	r3, r3, #16
	movs	r4, #0
	cmp	r3, r2
	bne.n	.L_0200130e
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r0, #40]
.L_0200130e:
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	cmp	r3, #9
	ble.n	.L_0200132e
	ldr	r3, [r0, #8]
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r0, #8]
	ldr	r2, [pc, #28]
	ldr	r3, [r0, #24]
	adds	r3, r3, r2
	str	r3, [r0, #24]
	ldr	r3, [r0, #28]
	adds	r3, r3, r2
	str	r3, [r0, #28]
.L_0200132e:
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	cmp	r3, #32
	bne.n	.L_0200133c
	str	r4, [r0, #8]
	str	r4, [r0, #16]
	str	r4, [r0, #108]
.L_0200133c:
	pop	{pc}
	.2byte 0x0000
	.2byte 0xf800
	.2byte 0xffff
	push	{lr}
	ldr	r3, [r0, #104]
	ldr	r1, [r0, #8]
	ldr	r3, [r3, #8]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L_0200135c
	movs	r3, #128
	lsls	r3, r3, #14
	cmp	r2, r3
	bgt.n	.L_02001372
	b.n	.L_02001366
.L_0200135c:
	movs	r2, #128
	subs	r3, r3, r1
	lsls	r2, r2, #14
	cmp	r3, r2
	bgt.n	.L_02001372
.L_02001366:
	adds	r3, r0, #0
	adds	r3, #100
	movs	r2, #0
	strh	r2, [r3, #0]
	ldr	r3, [pc, #4]
	str	r3, [r0, #108]
.L_02001372:
	pop	{pc}
	.2byte 0x92e5
	.2byte 0x0200
	push	{lr}
	bl 0x0200a9bc
	cmp	r0, #0
	beq.n	.L_02001388
	movs	r0, #1
	bl 0x0200b878
.L_02001388:
	pop	{pc}
	.2byte 0x0000
	.global Func_0200138c
	.thumb_func
Func_0200138c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #916]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	movs	r0, #214
	mov	r8, r1
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r7, [r3, r2]
	ldr	r2, [pc, #892]
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	lsls	r0, r0, #1
	movs	r2, #129
	adds	r3, r3, r0
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	subs	r0, #154
	sub	sp, #8
	bl 0x0200cb98
	bl 0x0200a270
	ldr	r3, [pc, #864]
	cmp	r8, r3
	bne.n	.L_020013dc
	movs	r0, #1
	bl 0x0200cdd8
	b.n	.L_020018be
.L_020013dc:
	ldr	r3, [pc, #852]
	cmp	r8, r3
	bne.n	.L_0200144c
	movs	r0, #0
	bl 0x0200cd88
	movs	r0, #10
	bl 0x02009234
	movs	r0, #163
	lsls	r0, r0, #4
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_02001420
	movs	r3, #53
	movs	r5, #3
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #41
	movs	r2, #24
	movs	r3, #10
	str	r5, [sp, #0]
	bl 0x0200cc30
	movs	r3, #117
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #105
	movs	r2, #24
	movs	r3, #10
	str	r5, [sp, #0]
	bl 0x0200cc30
.L_02001420:
	ldr	r0, [pc, #788]
	bl 0x020082bc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #62
	bl 0x0200cb90
	cmp	r0, #0
	bne.n	.L_02001436
	b.n	.L_020018be
.L_02001436:
	movs	r3, #39
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #50
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200cc30
	b.n	.L_020018be
.L_0200144c:
	ldr	r3, [pc, #748]
	cmp	r8, r3
	beq.n	.L_02001454
	b.n	.L_02001680
.L_02001454:
	movs	r0, #0
	bl 0x0200cd88
	ldr	r0, [pc, #740]
	ldr	r1, [pc, #740]
	movs	r2, #0
	movs	r3, #88
	bl 0x0200a2e4
	ldr	r0, [pc, #736]
	bl 0x020082bc
	movs	r0, #148
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_020014ae
	movs	r3, #47
	movs	r2, #100
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #47
	movs	r1, #94
	movs	r2, #10
	movs	r3, #6
	bl 0x0200cc30
	movs	r3, #46
	movs	r2, #40
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #110
	movs	r1, #40
	movs	r2, #12
	movs	r3, #4
	bl 0x0200cc28
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ccd8
	b.n	.L_020014e0
.L_020014ae:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #62
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_020014d6
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x0200cb98
	movs	r0, #18
	bl 0x0200ccb8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	b.n	.L_020014e0
.L_020014d6:
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ccd8
.L_020014e0:
	movs	r0, #20
	bl 0x0200ccb8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #20
	bl 0x0200ccf8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #71
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_0200153c
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r5, #28
	movs	r0, #28
	movs	r1, #92
	movs	r2, #8
	movs	r3, #8
	str	r5, [sp, #0]
	bl 0x0200cc30
	movs	r3, #38
	str	r3, [sp, #4]
	movs	r0, #92
	movs	r1, #38
	movs	r2, #8
	movs	r3, #8
	str	r5, [sp, #0]
	bl 0x0200cc28
	bl 0x02008798
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ccd8
	b.n	.L_02001564
.L_0200153c:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #72
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_0200155a
	bl 0x02008798
	movs	r0, #153
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200cb98
	b.n	.L_02001564
.L_0200155a:
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ccd8
.L_02001564:
	movs	r0, #149
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_0200158c
	movs	r1, #164
	movs	r2, #240
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #15
	bl 0x0200ccd8
	movs	r0, #8
	bl 0x0200ccb8
	movs	r3, #1
	adds	r0, #98
	strb	r3, [r0, #0]
.L_0200158c:
	movs	r0, #163
	lsls	r0, r0, #4
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_020015a4
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ccd8
	b.n	.L_020015ae
.L_020015a4:
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ccd8
.L_020015ae:
	cmp	r7, #20
	beq.n	.L_020015b4
	b.n	.L_020018be
.L_020015b4:
	bl 0x0200cca8
	movs	r0, #0
	bl 0x0200cda0
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	bl 0x0200cd18
	movs	r1, #128
	movs	r2, #128
	movs	r0, #0
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200cc50
	ldr	r3, [pc, #328]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ccd8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #18
	bl 0x0200ccd8
	movs	r0, #18
	bl 0x0200ccb8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	bl 0x0200cd68
	bl 0x0200cd78
	movs	r0, #18
	bl 0x0200ccb8
	adds	r3, r0, #0
	adds	r3, #100
	adds	r0, #102
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	movs	r0, #18
	bl 0x0200ccd8
	movs	r0, #159
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200cde0
	movs	r5, #2
.L_0200163c:
	movs	r0, #18
	bl 0x02008414
	subs	r5, #1
	movs	r0, #1
	bl 0x0200cb18
	cmp	r5, #0
	bge.n	.L_0200163c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #62
	bl 0x0200cb98
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200cc50
	movs	r0, #120
	bl 0x0200cca0
	movs	r0, #20
	bl 0x0200cd30
	movs	r0, #10
	adds	r0, #255
	bl 0x0200cb98
	b.n	.L_020018be
.L_02001680:
	ldr	r3, [pc, #200]
	cmp	r8, r3
	bne.n	.L_020016b0
	movs	r0, #0
	bl 0x0200cd88
	cmp	r7, #10
	beq.n	.L_02001694
	cmp	r7, #1
	bne.n	.L_020016a8
.L_02001694:
	ldr	r0, [pc, #184]
	ldr	r1, [pc, #188]
	ldr	r2, [pc, #188]
	movs	r3, #88
	bl 0x0200a2e4
	movs	r0, #0
	movs	r1, #1
	bl 0x0200a48c
.L_020016a8:
	ldr	r0, [pc, #176]
	bl 0x020082bc
	b.n	.L_020018be
.L_020016b0:
	ldr	r3, [pc, #172]
	cmp	r8, r3
	bne.n	.L_02001784
	movs	r0, #0
	bl 0x0200cd88
	movs	r0, #12
	bl 0x0200ccb8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #77
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_0200170c
	movs	r3, #101
	str	r3, [sp, #4]
	movs	r5, #12
.L_020016e0:
	movs	r0, #12
	movs	r1, #95
	movs	r2, #8
	movs	r3, #6
	str	r5, [sp, #0]
	bl 0x0200cc30
	movs	r3, #40
	str	r3, [sp, #4]
	movs	r0, #76
	movs	r1, #40
	movs	r2, #8
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200cc28
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ccd8
	b.n	.L_0200176e
.L_0200170c:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #76
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_02001764
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #50
	bl 0x0200cb98
	b.n	.L_0200176e
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200e878
	.4byte 0x00000117
	.4byte 0x00000118
	.4byte 0x0200d3d8
	.4byte 0x00000119
	.4byte 0x0200d3e6
	.4byte 0x0200d3f8
	.4byte 0x0200d3fe
	.4byte 0x0000011a
	.4byte 0x0200d40c
	.4byte 0x0200d412
	.4byte 0x020099a5
	.4byte 0x0200d418
	.2byte 0x011b
	.2byte 0x0000
.L_02001764:
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ccd8
.L_0200176e:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200cb90
	cmp	r0, #0
	bne.n	.L_0200177e
	b.n	.L_020018be
.L_0200177e:
	bl 0x02008c1c
	b.n	.L_020018be
.L_02001784:
	ldr	r3, [pc, #352]
	cmp	r8, r3
	beq.n	.L_0200178c
	b.n	.L_020018b2
.L_0200178c:
	movs	r0, #0
	bl 0x0200cd88
	cmp	r7, #2
	beq.n	.L_020017a2
	cmp	r7, #4
	beq.n	.L_020017a2
	cmp	r7, #6
	beq.n	.L_020017a2
	cmp	r7, #13
	bne.n	.L_020017b6
.L_020017a2:
	ldr	r0, [pc, #328]
	ldr	r1, [pc, #328]
	ldr	r2, [pc, #332]
	movs	r3, #88
	bl 0x0200a2e4
	movs	r0, #0
	movs	r1, #1
	bl 0x0200a48c
.L_020017b6:
	subs	r3, r7, #7
	cmp	r3, #1
	bls.n	.L_020017c0
	cmp	r7, #11
	bne.n	.L_02001884
.L_020017c0:
	ldr	r0, [pc, #308]
	ldr	r1, [pc, #312]
	ldr	r2, [pc, #312]
	movs	r3, #88
	bl 0x0200a2e4
	movs	r0, #0
	movs	r1, #1
	bl 0x0200a48c
	cmp	r7, #11
	bne.n	.L_020017fa
	ldr	r1, [pc, #296]
	movs	r0, #11
	bl 0x0200ccc8
	movs	r1, #3
	movs	r0, #0
	bl 0x0200a46c
	movs	r1, #144
	ldr	r0, [pc, #284]
	lsls	r1, r1, #3
	bl 0x0200cb20
	movs	r0, #126
	adds	r0, #255
	bl 0x0200cb98
.L_020017fa:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #268]
	bl 0x0200cb20
	movs	r0, #11
	bl 0x0200ccb8
	ldr	r3, [pc, #260]
	str	r3, [r0, #108]
	movs	r0, #1
	bl 0x0200a2d8
	ldr	r0, [pc, #252]
	bl 0x020082bc
	movs	r0, #20
	bl 0x0200ccb8
	adds	r6, r0, #0
	movs	r0, #13
	bl 0x0200ccb8
	ldr	r5, [pc, #236]
	str	r5, [r0, #108]
	movs	r0, #14
	bl 0x0200ccb8
	str	r5, [r0, #108]
	movs	r0, #15
	bl 0x0200ccb8
	str	r5, [r0, #108]
	movs	r0, #16
	bl 0x0200ccb8
	str	r5, [r0, #108]
	movs	r0, #17
	bl 0x0200ccb8
	str	r5, [r0, #108]
	movs	r0, #18
	bl 0x0200ccb8
	str	r5, [r0, #108]
	movs	r0, #13
	bl 0x0200ccb8
	str	r6, [r0, #104]
	movs	r0, #14
	bl 0x0200ccb8
	str	r6, [r0, #104]
	movs	r0, #15
	bl 0x0200ccb8
	str	r6, [r0, #104]
	movs	r0, #16
	bl 0x0200ccb8
	str	r6, [r0, #104]
	movs	r0, #17
	bl 0x0200ccb8
	str	r6, [r0, #104]
	movs	r0, #18
	bl 0x0200ccb8
	str	r6, [r0, #104]
.L_02001884:
	movs	r0, #9
	bl 0x0200ccb8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #78
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_020018be
	movs	r1, #176
	movs	r2, #174
	movs	r0, #8
	lsls	r1, r1, #15
	lsls	r2, r2, #18
	bl 0x0200ccd8
	b.n	.L_020018be
.L_020018b2:
	ldr	r3, [pc, #104]
	cmp	r8, r3
	bne.n	.L_020018be
	movs	r0, #0
	bl 0x0200cd88
.L_020018be:
	ldr	r3, [pc, #40]
	cmp	r8, r3
	bne.n	.L_020018d0
	cmp	r7, #13
	bne.n	.L_020018d0
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200cba0
.L_020018d0:
	ldr	r0, [pc, #76]
	ldr	r1, [pc, #80]
	ldr	r2, [pc, #80]
	ldr	r3, [pc, #84]
	bl 0x0200c5f8
	movs	r0, #0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0000011c
	.4byte 0x0200d438
	.4byte 0x0200d43e
	.4byte 0x0200996d
	.4byte 0x0200d448
	.4byte 0x0200d44e
	.4byte 0x02009989
	.4byte 0x0200d314
	.4byte 0x02009255
	.4byte 0x02009379
	.4byte 0x020092bd
	.4byte 0x0200d454
	.4byte 0x02009345
	.4byte 0x0000011d
	.4byte 0x0200d86c
	.4byte 0x0200d87c
	.4byte 0x0200d8a8
	.2byte 0xd8d4
	.2byte 0x0200
	.global Func_02001930
	.thumb_func
Func_02001930:
	push	{lr}
	movs	r0, #163
	lsls	r0, r0, #4
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_0200195a
	ldr	r3, [pc, #32]
	movs	r2, #253
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #24]
	movs	r1, #160
	subs	r3, r3, r2
	adds	r0, r0, r3
	lsls	r1, r1, #19
	bl 0x0200cc88
.L_0200195a:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000121
	.2byte 0x010e
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #20]
	sub	sp, #4
	str	r3, [sp, #0]
	movs	r1, #19
	movs	r2, #20
	movs	r3, #0
	movs	r0, #10
	bl 0x0200af4c
	add	sp, #4
	pop	{pc}
	.2byte 0xd47c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	sub	sp, #4
	str	r3, [sp, #0]
	movs	r1, #19
	movs	r2, #20
	movs	r3, #0
	movs	r0, #12
	bl 0x0200af4c
	add	sp, #4
	pop	{pc}
	.2byte 0xd4cc
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #24]
	sub	sp, #4
	movs	r3, #128
	str	r2, [sp, #0]
	lsls	r3, r3, #7
	movs	r1, #18
	movs	r2, #20
	movs	r0, #9
	bl 0x0200af4c
	add	sp, #4
	pop	{pc}
	.2byte 0x0000
	.2byte 0xd528
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	sub	sp, #4
	str	r3, [sp, #0]
	movs	r1, #13
	movs	r2, #14
	movs	r3, #0
	movs	r0, #9
	bl 0x0200af4c
	add	sp, #4
	pop	{pc}
	.2byte 0xd578
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #24]
	sub	sp, #4
	movs	r3, #128
	str	r2, [sp, #0]
	lsls	r3, r3, #7
	movs	r1, #13
	movs	r2, #14
	movs	r0, #11
	bl 0x0200af4c
	add	sp, #4
	pop	{pc}
	.2byte 0x0000
	.2byte 0xd5d4
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	sub	sp, #4
	str	r3, [sp, #0]
	movs	r1, #21
	movs	r2, #22
	movs	r3, #0
	movs	r0, #13
	bl 0x0200af4c
	add	sp, #4
	pop	{pc}
	.2byte 0xd644
	.2byte 0x0200
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_02001a32
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001a3c
	b.n	.L_02001a7c
.L_02001a32:
	movs	r2, #128
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001a7c
.L_02001a3c:
	ldr	r4, [r0, #12]
	ldr	r3, [r1, #12]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_02001a50
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001a5a
	b.n	.L_02001a7c
.L_02001a50:
	movs	r2, #128
	subs	r3, r3, r4
.L_02001a54:
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001a7c
.L_02001a5a:
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_02001a6e
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001a78
	b.n	.L_02001a7c
.L_02001a6e:
	movs	r2, #128
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001a7c
.L_02001a78:
	movs	r0, #1
	b.n	.L_02001a7e
.L_02001a7c:
	movs	r0, #0
.L_02001a7e:
	pop	{pc}
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_02001a96
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001aa0
	b.n	.L_02001ad2
.L_02001a96:
	movs	r2, #192
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001ad2
.L_02001aa0:
	ldr	r2, [r1, #12]
	ldr	r3, [r0, #12]
	subs	r3, r3, r2
	ldr	r2, [pc, #48]
	adds	r3, r3, r2
	ldr	r2, [pc, #48]
	cmp	r3, r2
	bhi.n	.L_02001ad2
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_02001ac4
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001ace
	b.n	.L_02001ad2
.L_02001ac4:
	movs	r2, #192
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001ad2
.L_02001ace:
	movs	r0, #1
	b.n	.L_02001ad4
.L_02001ad2:
	movs	r0, #0
.L_02001ad4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0007ffff
	.2byte 0xfffe
	.2byte 0x001f
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #336]
	ldr	r2, [pc, #336]
	mov	sl, r3
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, sl
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #4
	mov	r8, r2
	bl 0x0200ccb8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	ldr	r5, [r6, #68]
	mov	r9, r3
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r4, r0, #0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #16]
	str	r4, [sp, #0]
	bl 0x0200cb08
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bge.n	.L_02001b3c
	adds	r3, #15
.L_02001b3c:
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
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_02001bce
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x02009a1c
	cmp	r0, #0
	beq.n	.L_02001bce
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	r3, sl
	ldrb	r2, [r3, #0]
	cmp	r2, #0
	bne.n	.L_02001bce
	ldr	r1, [r6, #76]
	cmp	r1, #0
	beq.n	.L_02001ba2
	mov	r3, r8
	adds	r3, #104
	strh	r2, [r3, #0]
	mov	r2, r8
	adds	r2, #106
	cmp	r1, #0
	ble.n	.L_02001b9a
	movs	r3, #1
	b.n	.L_02001ba0
.L_02001b9a:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
.L_02001ba0:
	strh	r3, [r2, #0]
.L_02001ba2:
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #0
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r1, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	mov	r2, r8
	movs	r3, #1
	strh	r3, [r2, #4]
	ldrh	r2, [r2, #10]
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	strh	r2, [r3, #0]
.L_02001bce:
	movs	r3, #84
	mov	r2, r8
	ldrh	r0, [r2, r3]
	movs	r7, #0
	cmp	r0, #0
	beq.n	.L_02001c2e
	mov	r5, r8
	adds	r5, #84
.L_02001bde:
	bl 0x0200ccb8
	adds	r4, r0, #0
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_02001c20
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x02009a80
	cmp	r0, #0
	beq.n	.L_02001c20
	ldrh	r1, [r5, #2]
	cmp	r1, #0
	bne.n	.L_02001c1a
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r1, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	b.n	.L_02001c20
.L_02001c1a:
	movs	r3, #1
	mov	r2, r8
	strh	r3, [r2, #6]
.L_02001c20:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #3
	bgt.n	.L_02001c2e
	ldrh	r0, [r5, #0]
	cmp	r0, #0
	bne.n	.L_02001bde
.L_02001c2e:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r3, #4
	strb	r6, [r3, #0]
	movs	r1, #0
	bl 0x0200cc40
	ldr	r1, [pc, #20]
	adds	r0, r5, #0
	bl 0x0200cbd0
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xd6a0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200cbd8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001cb4
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	str	r6, [r5, #76]
	movs	r1, #2
	bl 0x02009c44
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_02001cac
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02001cac:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200cbc0
.L_02001cb4:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x9ae1
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
.L_02001cc6:
	ldr	r3, [pc, #332]
	ldr	r2, [pc, #332]
	mov	sl, r3
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, sl
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #4
	mov	r8, r2
	bl 0x0200ccb8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
.L_02001ce4:
	ldr	r5, [r6, #68]
	mov	r9, r3
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #16]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
.L_02001cf8:
	ldr	r3, [r6, #8]
	adds	r4, r0, #0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #8]
	str	r4, [sp, #0]
	bl 0x0200cb08
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bge.n	.L_02001d18
	adds	r3, #15
.L_02001d18:
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
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_02001da8
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x02009a1c
	cmp	r0, #0
	beq.n	.L_02001da8
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	r3, sl
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02001da8
	ldr	r2, [r6, #76]
	cmp	r2, #0
	beq.n	.L_02001d7c
	mov	r1, r8
	adds	r1, #106
	strh	r3, [r1, #0]
	subs	r1, #2
	cmp	r2, #0
	ble.n	.L_02001d74
	movs	r3, #1
	b.n	.L_02001d7a
.L_02001d74:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
.L_02001d7a:
	strh	r3, [r1, #0]
.L_02001d7c:
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #0
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #68]
	str	r1, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	mov	r2, r8
	movs	r3, #1
	strh	r3, [r2, #4]
	ldrh	r2, [r2, #10]
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	strh	r2, [r3, #0]
.L_02001da8:
	movs	r3, #84
	mov	r2, r8
	ldrh	r0, [r2, r3]
	movs	r7, #0
	cmp	r0, #0
	beq.n	.L_02001e08
	mov	r5, r8
	adds	r5, #84
.L_02001db8:
	bl 0x0200ccb8
	adds	r4, r0, #0
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_02001dfa
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x02009a80
	cmp	r0, #0
	beq.n	.L_02001dfa
	ldrh	r1, [r5, #2]
	cmp	r1, #0
	bne.n	.L_02001df4
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r1, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	b.n	.L_02001dfa
.L_02001df4:
	movs	r3, #1
	mov	r2, r8
	strh	r3, [r2, #6]
.L_02001dfa:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #3
	bgt.n	.L_02001e08
	ldrh	r0, [r5, #0]
	cmp	r0, #0
	bne.n	.L_02001db8
.L_02001e08:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r3, #4
	strb	r6, [r3, #0]
	movs	r3, #3
	ldr	r0, [r5, #80]
	ands	r1, r3
	ldrb	r2, [r0, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	lsls	r1, r1, #2
	orrs	r3, r1
	strb	r3, [r0, #9]
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200cc40
	ldr	r1, [pc, #16]
	adds	r0, r5, #0
	bl 0x0200cbd0
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0xd6a0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200cbd8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001ea2
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	negs	r3, r6
	str	r3, [r5, #76]
	movs	r1, #3
	bl 0x02009e1c
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
.L_02001e8e:
	cmp	r7, #0
	beq.n	.L_02001e9a
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02001e9a:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200cbc0
.L_02001ea2:
	pop	{r5, r6, r7, pc}
	.2byte 0x9ae1
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200cbd8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001ee8
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	str	r6, [r5, #76]
	movs	r1, #3
	bl 0x02009e1c
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_02001ee0
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02001ee0:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200cbc0
.L_02001ee8:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x9cbd
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200cbd8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001f32
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	negs	r3, r6
	str	r3, [r5, #76]
	movs	r1, #3
	bl 0x02009e1c
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_02001f2a
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02001f2a:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200cbc0
.L_02001f32:
	pop	{r5, r6, r7, pc}
	.2byte 0x9cbd
	.2byte 0x0200
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_02001f4e
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001f58
	b.n	.L_02001f88
.L_02001f4e:
	movs	r2, #192
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001f88
.L_02001f58:
	ldr	r2, [r1, #12]
	ldr	r3, [r0, #12]
	subs	r3, r3, r2
	ldr	r2, [pc, #44]
	subs	r3, #1
	cmp	r3, r2
	bhi.n	.L_02001f88
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_02001f7a
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001f84
	b.n	.L_02001f88
.L_02001f7a:
	movs	r2, #128
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001f88
.L_02001f84:
	movs	r0, #1
	b.n	.L_02001f8a
.L_02001f88:
	movs	r0, #0
.L_02001f8a:
	pop	{pc}
	.2byte 0xfffe
	.2byte 0x000f
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #144]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
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
	mov	r8, r0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200cb08
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02001fd4
	adds	r3, #15
.L_02001fd4:
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
	mov	r3, r8
	ldr	r2, [r3, #12]
	ldr	r3, [r3, #20]
	cmp	r2, r3
	bne.n	.L_02002022
	adds	r0, r6, #0
	mov	r1, r8
	bl 0x02009f38
	cmp	r0, #0
	beq.n	.L_02002022
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	movs	r3, #0
	str	r3, [r6, #76]
.L_02002022:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r3, #4
	strb	r6, [r3, #0]
	movs	r1, #0
	bl 0x0200cc40
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200cbc0
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	bl 0x0200cbd0
	adds	r0, r5, #0
	movs	r1, #10
	bl 0x0200ccf0
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xd6a0
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #4]
	adds	r6, r1, #0
	ldr	r1, [r0, #0]
	ldr	r0, [pc, #104]
	adds	r3, r3, r0
	movs	r0, #30
	adds	r0, #255
	bl 0x0200cbd8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020020de
	bl 0x0200cb30
	adds	r3, r0, #0
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #12
	movs	r3, #192
	lsls	r3, r3, #6
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	bl 0x0200cb48
	str	r0, [r5, #68]
	bl 0x0200cb30
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r0, [r5, #72]
	bl 0x0200cb30
	lsls	r0, r0, #17
	lsrs	r0, r0, #16
	adds	r0, r0, r6
	str	r0, [r5, #76]
	bl 0x0200cb30
	ldr	r3, [pc, #36]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r5, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	movs	r1, #2
	adds	r0, r5, #0
	bl 0x0200a02c
	ldr	r3, [pc, #20]
	adds	r0, r5, #0
	str	r3, [r5, #108]
	movs	r1, #1
	bl 0x0200cc48
.L_020020de:
	pop	{r5, r6, pc}
	.4byte 0xfffe0000
	.4byte 0xffff8000
	.2byte 0x9f91
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #364]
	sub	sp, #8
	mov	r8, r0
	movs	r0, #192
	lsls	r0, r0, #18
	ldr	r1, [r0, #32]
	mov	r7, r8
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r3, [r2, #0]
	ldr	r2, [r2, #4]
	mov	r9, r3
	ldr	r3, [pc, #344]
.L_02002114:
	mov	r4, r9
	ands	r4, r3
	ands	r2, r3
	ldr	r3, [r1, #0]
	mov	r9, r4
	ldr	r3, [r3, #4]
	adds	r7, #20
	str	r3, [sp, #4]
	mov	sl, r2
	ldr	r0, [r0, #108]
	str	r0, [sp, #0]
	mov	r0, r8
	movs	r4, #6
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	beq.n	0x0200a146
	movs	r1, #8
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	bne.n	0x0200a146
	ldr	r3, [r0, #16]
	cmp	r3, #0
	beq.n	0x0200a146
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4642
	ldrh	r3, [r2, #6]
	mov	r4, r8
	movs	r2, #0
	mov	r0, r8
	strh	r3, [r4, #8]
	strh	r2, [r0, #6]
	movs	r1, #3
	mov	fp, r1
.L_02002158:
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_02002248
	ldr	r5, [r7, #8]
	cmp	r5, #0
	beq.n	.L_02002248
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200cb90
.L_0200216e:
	ldr	r4, [sp, #0]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r4, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02002182
	movs	r3, #1
	orrs	r0, r3
.L_02002182:
	adds	r6, r5, #0
	adds	r6, #91
	strb	r0, [r6, #0]
	mov	r0, r8
	movs	r4, #14
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	beq.n	.L_0200219c
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200cb90
	strb	r0, [r6, #0]
.L_0200219c:
	ldr	r2, [r5, #8]
	mov	r1, r9
	ldr	r3, [r5, #16]
	subs	r2, r2, r1
	ldr	r1, [r5, #12]
	mov	r4, sl
	subs	r3, r3, r4
	subs	r1, r3, r1
	movs	r0, #6
	ldrsh	r4, [r7, r0]
	asrs	r3, r1, #16
	adds	r1, r3, #0
	asrs	r2, r2, #16
	subs	r1, #8
	cmp	r4, #0
	bne.n	.L_020021d2
	adds	r3, r2, #7
	movs	r2, #167
	lsls	r2, r2, #1
	cmp	r3, r2
	bhi.n	.L_02002248
	movs	r3, #48
	negs	r3, r3
	cmp	r1, r3
	ble.n	.L_02002248
	cmp	r1, #239
	bgt.n	.L_02002248
.L_020021d2:
	movs	r0, #2
	ldrsh	r3, [r7, r0]
	ldrh	r1, [r7, #2]
	cmp	r3, #0
	bgt.n	.L_02002244
	ldrh	r3, [r7, #4]
	movs	r1, #240
	ands	r1, r3
	cmp	r1, #32
	beq.n	.L_02002218
	cmp	r1, #32
	bgt.n	.L_020021f4
	cmp	r1, #0
	beq.n	.L_02002234
	cmp	r1, #16
	beq.n	.L_02002226
	b.n	.L_02002240
.L_020021f4:
	cmp	r1, #48
	beq.n	.L_0200220a
	cmp	r1, #128
	bne.n	.L_02002240
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x0200a06c
	b.n	.L_02002240
.L_0200220a:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x02009e60
	b.n	.L_02002240
.L_02002218:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x02009ef0
	b.n	.L_02002240
.L_02002226:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x02009ea8
	b.n	.L_02002240
.L_02002234:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x02009c74
.L_02002240:
	movs	r3, #8
	b.n	.L_02002246
.L_02002244:
	subs	r3, r1, #1
.L_02002246:
	strh	r3, [r7, #2]
.L_02002248:
	movs	r1, #1
	negs	r1, r1
	add	fp, r1
	mov	r2, fp
	adds	r7, #16
	cmp	r2, #0
	blt.n	.L_02002258
	b.n	.L_02002158
.L_02002258:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb560
	movs	r0, #10
	adds	r0, #255
	ldr	r6, [pc, #68]
	bl 0x0200cb90
	cmp	r0, #0
	bne.n	0x0200a28a
	ldr	r3, [pc, #60]
	adds	r0, r6, #0
	movs	r1, #116
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x206e
	movs	r1, #1
	movs	r2, #0
	movs	r3, #0
	adds	r0, #255
	bl 0x0200cbd8
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #1
	bl 0x0200cbc0
	ldr	r1, [r5, #80]
	movs	r2, #1
	ldrb	r3, [r1, #16]
	str	r5, [r6, #112]
	strh	r3, [r6, #12]
	ldrb	r3, [r1, #17]
	orrs	r3, r2
	strb	r3, [r1, #17]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.2byte 0x0258
	.2byte 0x0300
	push	{r5, lr}
	ldr	r5, [pc, #12]
	ldr	r0, [r5, #112]
	bl 0x0200cbe0
	movs	r3, #0
	str	r3, [r5, #112]
	pop	{r5, pc}
	.4byte 0x0200254c
	.4byte 0x81d84b01
	.4byte 0x00004770
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r1
	ldr	r1, [pc, #280]
	sub	sp, #16
	adds	r6, r0, #0
	movs	r0, #10
	str	r1, [sp, #4]
	adds	r0, #255
	adds	r1, #20
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	mov	r9, r1
	bl 0x0200cb90
	cmp	r0, #0
	bne.n	.L_020023c8
	ldrh	r3, [r6, #0]
	movs	r2, #0
	mov	fp, r2
	mov	sl, r3
	adds	r6, #2
	cmp	r3, #0
	ble.n	.L_02002386
.L_0200231e:
	ldrh	r7, [r6, #0]
	movs	r1, #15
	ands	r1, r7
	movs	r3, #240
	mov	r0, sl
	str	r1, [sp, #0]
	ands	r7, r3
	bl 0x0200ccb8
	adds	r5, r0, #0
	adds	r6, #2
	cmp	r5, #0
	beq.n	.L_02002372
	movs	r1, #0
	bl 0x0200cc40
	adds	r3, r5, #0
	movs	r2, #128
	adds	r3, #98
	movs	r1, #1
	ands	r2, r7
	strb	r1, [r3, #0]
	cmp	r2, #0
	bne.n	.L_02002352
	subs	r3, #9
	strb	r2, [r3, #0]
.L_02002352:
	mov	r2, r9
	mov	r3, r9
	strh	r1, [r2, #0]
	mov	r0, sl
	strh	r7, [r3, #4]
	bl 0x0200ccb8
	mov	r1, r9
	str	r0, [r1, #8]
	ldr	r2, [sp, #0]
	lsls	r3, r2, #16
	str	r3, [r1, #12]
	mov	r3, fp
	strh	r3, [r1, #2]
	movs	r2, #16
	add	r9, r2
.L_02002372:
	movs	r3, #1
	add	fp, r3
	mov	r1, fp
	cmp	r1, #3
	bgt.n	.L_02002386
	ldrh	r2, [r6, #0]
	adds	r6, #2
	mov	sl, r2
	cmp	r2, #0
	bgt.n	.L_0200231e
.L_02002386:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_020023c8
	movs	r1, #0
	ldrh	r2, [r3, #0]
	mov	fp, r1
	ldr	r1, [sp, #4]
	movs	r3, #2
	add	r8, r3
	movs	r3, #84
	strh	r2, [r1, r3]
.L_0200239c:
	cmp	r2, #0
	ble.n	.L_020023c8
	adds	r2, r1, #0
	adds	r2, #84
.L_020023a4:
	mov	r1, r8
.L_020023a6:
	ldrh	r3, [r1, #0]
	movs	r1, #1
	strh	r3, [r2, #2]
	add	fp, r1
	movs	r3, #2
	add	r8, r3
	mov	r3, fp
	adds	r2, #4
	cmp	r3, #3
	bgt.n	.L_020023c8
	mov	r1, r8
	ldrh	r3, [r1, #0]
	movs	r1, #2
	add	r8, r1
	strh	r3, [r2, #0]
	cmp	r3, #0
	bgt.n	.L_020023a4
.L_020023c8:
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #4]
	add	r1, sp, #8
	str	r2, [r3, #16]
	ldrh	r1, [r1, #0]
	ldr	r2, [sp, #4]
	strh	r1, [r2, #10]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #80
	ldrh	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_020023f8
	movs	r3, #192
	movs	r2, #128
	lsls	r3, r3, #4
	lsls	r2, r2, #19
	adds	r3, #8
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #16
	strh	r3, [r1, #0]
.L_020023f8:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl 0x0200cb20
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200254c
	.4byte 0x0200a0ed
	.4byte 0x01004b02
	.4byte 0x231418c0
	.4byte 0x47705ec0
	.4byte 0x0200254c
	.4byte 0x01004b02
	.4byte 0x828118c0
	.4byte 0x00004770
	.4byte 0x0200254c
	.4byte 0x68184b01
	.4byte 0x00004770
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #130
	lsls	r0, r0, #1
	ldr	r5, [pc, #24]
	bl 0x0200cb90
	cmp	r0, #0
	bne.n	.L_02002462
	ldr	r3, [r5, #0]
.L_02002458:
	adds	r3, #1
	str	r3, [r5, #0]
	cmp	r3, r6
	blt.n	.L_02002462
	str	r0, [r5, #0]
.L_02002462:
	ldr	r0, [r5, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.4byte 0x01004b06
	.4byte 0x230f18c0
	.4byte 0x3014400b
	.4byte 0x60c3041b
	.4byte 0x40194b01
	.4byte 0x47708081
	.4byte 0x000000f0
	.4byte 0x0200254c
	.4byte 0x01004b02
	.4byte 0x834118c0
	.4byte 0x00004770
	.4byte 0x0200254c
	.4byte 0x01004b02
	.4byte 0x231818c0
	.4byte 0x47705ec0
	.2byte 0x254c
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #12]
	cmp	r0, #3
	bhi.n	.L_020024ba
	lsls	r3, r0, #2
	adds	r3, #84
	strh	r1, [r2, r3]
.L_020024ba:
	pop	{pc}
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #16
	ldr	r6, [r3, #108]
	bl 0x0200cdc0
	mov	r8, r0
	bl 0x0200ccb8
	bl 0x0200cc98
	movs	r5, #0
	adds	r7, r0, #0
	cmp	r5, r7
.L_020024e4:
	bge.n	.L_02002502
.L_020024e6:
	ldr	r2, [pc, #192]
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldrb	r0, [r2, r3]
	bl 0x0200cb88
	ldrh	r3, [r0, #56]
	lsls	r2, r5, #1
	mov	r1, sp
	adds	r5, #1
	strh	r3, [r1, r2]
	cmp	r5, r7
	blt.n	.L_020024e6
.L_02002502:
	movs	r0, #10
	negs	r0, r0
	movs	r1, #0
	bl 0x0200cda8
	movs	r2, #182
	lsls	r2, r2, #1
	movs	r4, #183
	adds	r3, r6, r2
	lsls	r4, r4, #1
	movs	r2, #0
	strh	r2, [r3, #0]
	movs	r1, #129
	adds	r3, r6, r4
	strh	r2, [r3, #0]
	mov	r0, r8
	lsls	r1, r1, #1
	movs	r5, #0
	bl 0x0200cd08
	cmp	r5, r7
	bge.n	.L_0200259c
.L_0200252e:
	ldr	r1, [pc, #120]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r2, r2, r5
	ldrb	r0, [r1, r2]
	mov	sl, r1
	mov	r8, r2
	bl 0x0200cb88
	movs	r4, #56
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	ble.n	.L_02002556
	movs	r1, #183
	lsls	r1, r1, #1
	adds	r2, r6, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002596
.L_02002556:
	mov	r3, sp
	lsls	r2, r5, #1
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02002596
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r1, r6, r2
	ldrh	r3, [r1, #0]
	movs	r4, #184
	adds	r2, r3, #1
	lsls	r3, r3, #16
	lsls	r4, r4, #1
	asrs	r3, r3, #15
	strh	r2, [r1, #0]
	adds	r3, r3, r4
	mov	r1, sl
	mov	r4, r8
	ldrb	r2, [r1, r4]
	movs	r1, #181
	strh	r2, [r6, r3]
	movs	r3, #255
	lsls	r1, r1, #1
	lsls	r3, r3, #8
	adds	r2, r6, r1
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r3, #50
	adds	r3, #255
	adds	r2, r0, r3
	movs	r3, #0
	strb	r3, [r2, #0]
.L_02002596:
	adds	r5, #1
	cmp	r5, r7
	blt.n	.L_0200252e
.L_0200259c:
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #96]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
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
	bl 0x0200cb08
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_020025ea
	adds	r3, #15
.L_020025ea:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	movs	r1, #128
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	lsls	r1, r1, #5
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r2, [r6, #80]
	ldrh	r3, [r2, #18]
	adds	r3, r3, r1
	strh	r3, [r2, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r2, [r3, #0]
	movs	r3, #7
	ands	r2, r3
	cmp	r2, #0
.L_02002620:
	beq.n	.L_02002628
	cmp	r2, #4
	beq.n	.L_02002630
	b.n	.L_02002636
.L_02002628:
	movs	r1, #10
	bl 0x0200cc60
	b.n	.L_02002636
.L_02002630:
	movs	r1, #0
	bl 0x0200cc60
.L_02002636:
	pop	{pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #484]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	sub	sp, #16
	cmp	r3, #0
	beq.n	.L_0200265e
	b.n	.L_0200281c
.L_0200265e:
	movs	r0, #10
	movs	r1, #0
	negs	r0, r0
	bl 0x0200a4c0
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
	ldr	r2, [pc, #444]
	adds	r6, r0, #0
	str	r2, [sp, #0]
	bl 0x0200cca8
	movs	r0, #0
	bl 0x0200cda0
	ldr	r3, [pc, #432]
	adds	r0, r6, #0
	str	r3, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	movs	r1, #49
	bl 0x0200cbc0
.L_02002696:
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl 0x0200cc18
	ldr	r3, [sp, #0]
	ldr	r1, [sp, #0]
	adds	r3, #104
	adds	r1, #106
	mov	r9, r1
	mov	sl, r3
	add	r1, sp, #4
	cmp	r0, #7
	bne.n	.L_02002712
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	mov	r1, r9
	lsls	r3, r3, #17
	str	r3, [r6, #36]
	movs	r5, #0
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	lsls	r3, r3, #17
	str	r3, [r6, #44]
	movs	r3, #128
	lsls	r3, r3, #5
	str	r3, [r6, #52]
.L_020026d0:
	mov	r0, sl
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [r6, #8]
	lsls	r2, r2, #19
	add	r1, sp, #4
	adds	r3, r3, r2
	str	r3, [r1, #0]
	mov	r0, r9
	ldr	r3, [r6, #12]
	str	r3, [r1, #4]
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [r6, #16]
	lsls	r2, r2, #19
	adds	r3, r3, r2
	str	r3, [r1, #8]
	adds	r0, r6, #0
	bl 0x0200cc38
	cmp	r0, #0
	beq.n	.L_02002704
	movs	r3, #0
	str	r3, [r6, #36]
	str	r3, [r6, #44]
	b.n	.L_02002810
.L_02002704:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200cb18
	cmp	r5, #9
	ble.n	.L_020026d0
	b.n	.L_02002810
.L_02002712:
	mov	r0, sl
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [r6, #8]
	lsls	r2, r2, #19
	adds	r3, r3, r2
	str	r3, [r1, #0]
	mov	r0, r9
	ldr	r3, [r6, #12]
	str	r3, [r1, #4]
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [r6, #16]
	lsls	r2, r2, #19
	adds	r3, r3, r2
	str	r3, [r1, #8]
	adds	r0, r6, #0
	bl 0x0200cc38
	cmp	r0, #0
	bgt.n	.L_02002810
	cmp	r0, #0
	bge.n	.L_0200275c
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	ldr	r3, [pc, #232]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r2, #1
	ldr	r0, [r3, #0]
	movs	r1, #6
	negs	r2, r2
	bl 0x0200cce8
	b.n	.L_02002810
.L_0200275c:
	ldrh	r3, [r6, #32]
	movs	r2, #0
	subs	r3, #2
	mov	fp, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	mov	r8, r2
	adds	r7, r5, #0
	adds	r7, #89
.L_02002770:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_020027a0
	ldrb	r2, [r7, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020027a0
	cmp	r5, r6
	beq.n	.L_020027a0
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	mov	r1, fp
	add	r2, sp, #4
	bl 0x0200cc78
	cmp	r0, #0
	blt.n	.L_020027a0
	movs	r0, #1
	bl 0x0200cb18
	b.n	.L_02002810
.L_020027a0:
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	adds	r7, #128
	adds	r5, #128
	cmp	r0, #63
	ble.n	.L_02002770
	mov	r2, sl
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	ldr	r2, [r6, #8]
	lsls	r3, r3, #17
	adds	r1, r2, r3
	str	r1, [r6, #8]
	mov	r2, r9
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	ldr	r2, [r6, #16]
	ldr	r7, [pc, #116]
	lsls	r3, r3, #17
	ldr	r0, [pc, #116]
	adds	r5, r2, r3
	adds	r3, r1, #0
	ands	r3, r7
	movs	r4, #128
	adds	r2, r3, r0
	lsls	r4, r4, #9
	str	r5, [r6, #16]
	cmp	r2, r4
	ble.n	.L_020027de
	adds	r2, r4, #0
.L_020027de:
	ldr	r0, [pc, #100]
	cmp	r2, r0
	bge.n	.L_020027e6
	adds	r2, r0, #0
.L_020027e6:
	subs	r3, r1, r2
	ldr	r1, [pc, #84]
	str	r3, [r6, #8]
	adds	r3, r5, #0
	ands	r3, r7
	adds	r2, r3, r1
	cmp	r2, r4
	ble.n	.L_020027f8
	adds	r2, r4, #0
.L_020027f8:
	cmp	r2, r0
	bge.n	.L_020027fe
	adds	r2, r0, #0
.L_020027fe:
	subs	r3, r5, r2
	str	r3, [r6, #16]
	ldr	r2, [sp, #0]
	movs	r3, #0
	strh	r3, [r2, #4]
	movs	r0, #1
	bl 0x0200cb18
	b.n	.L_02002696
.L_02002810:
	movs	r3, #0
	str	r3, [r6, #108]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200cc60
.L_0200281c:
	ldr	r0, [sp, #0]
	movs	r3, #0
	strh	r3, [r0, #4]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200254c
	.4byte 0x0200a615
	.4byte 0x000fffff
	.4byte 0xfff80000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	ldr	r3, [pc, #16]
	movs	r2, #4
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02002858
	bl 0x0200a63c
.L_02002858:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r1, #217
	lsls	r1, r1, #1
	adds	r6, r5, r1
	ldrh	r3, [r6, #0]
	sub	sp, #12
	cmp	r3, #0
	bne.n	.L_0200288c
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r5, r2
	adds	r1, #2
	ldr	r0, [r3, #0]
	adds	r3, r5, r1
	ldr	r1, [r3, #0]
	bl 0x0200cd50
	movs	r3, #1
	strh	r3, [r6, #0]
.L_0200288c:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200cba0
	movs	r2, #179
	lsls	r2, r2, #1
	movs	r1, #173
	adds	r3, r5, r2
	lsls	r1, r1, #1
	movs	r2, #0
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	adds	r1, #4
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	subs	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	adds	r1, #8
	strh	r2, [r3, #0]
	movs	r0, #10
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r1, #0
	negs	r0, r0
	bl 0x0200a4c0
	movs	r0, #224
	movs	r1, #224
	lsls	r1, r1, #8
	lsls	r0, r0, #11
	bl 0x0200cd10
	ldr	r3, [pc, #192]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
	adds	r6, r0, #0
	movs	r0, #131
	lsls	r0, r0, #1
	ldr	r7, [pc, #176]
	bl 0x0200cb98
	bl 0x0200cca8
	movs	r0, #0
	bl 0x0200cda0
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200cba0
	ldr	r3, [pc, #156]
	movs	r1, #49
	str	r3, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r0, r6, #0
	bl 0x0200cbc0
	ldr	r3, [r7, #108]
	adds	r3, #100
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002978
.L_02002918:
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	movs	r0, #0
	bl 0x0200cc18
	ldr	r1, [r7, #108]
	cmp	r0, #7
	beq.n	.L_02002938
	ldr	r2, [r1, #112]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldr	r3, [r6, #16]
	ldr	r2, [r1, #120]
	adds	r3, r3, r2
	b.n	.L_02002960
.L_02002938:
	ldr	r3, [r1, #112]
	cmp	r3, #0
	beq.n	.L_0200294c
	ldr	r3, [r6, #8]
	ldr	r2, [pc, #88]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r6, #8]
.L_0200294c:
	ldr	r3, [r7, #108]
	ldr	r3, [r3, #120]
	cmp	r3, #0
	beq.n	.L_02002962
	ldr	r3, [r6, #16]
	ldr	r2, [pc, #68]
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #12
	adds	r3, r3, r1
.L_02002960:
	str	r3, [r6, #16]
.L_02002962:
	movs	r3, #0
	strh	r3, [r7, #4]
	movs	r0, #1
	bl 0x0200cb18
	ldr	r3, [r7, #108]
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02002918
.L_02002978:
	movs	r5, #0
	movs	r0, #30
	bl 0x0200cca0
	adds	r0, r6, #0
	str	r5, [r6, #108]
	movs	r1, #0
	bl 0x0200cc60
	strh	r5, [r7, #4]
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200254c
	.4byte 0x0200a615
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb500
	ldr	r3, [pc, #20]
	movs	r2, #4
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020029b4
	bl 0x0200a860
	movs	r0, #1
	b.n	.L_020029b6
.L_020029b4:
	movs	r0, #0
.L_020029b6:
	pop	{pc}
	.4byte 0x0200254c
	.4byte 0x22044b01
	.4byte 0x47705e98
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #344]
	sub	sp, #4
	ldr	r3, [r1, #112]
	mov	fp, r0
	cmp	r3, #0
	bne.n	.L_020029e4
	b.n	.L_02002b3c
.L_020029e4:
	movs	r2, #0
	str	r2, [sp, #0]
.L_020029e8:
	bl 0x0200cb30
	adds	r5, r0, #0
	bl 0x0200cb30
	mov	r3, fp
	ldr	r3, [r3, #8]
	lsls	r5, r5, #4
	mov	r8, r3
	add	r8, r5
	lsls	r0, r0, #4
	mov	r1, r8
	subs	r1, r1, r0
	mov	r8, r1
	bl 0x0200cb30
	adds	r6, r0, #0
	bl 0x0200cb30
	adds	r5, r0, #0
	bl 0x0200cb30
	mov	r2, fp
	ldr	r3, [r2, #16]
	ldr	r2, [r2, #12]
	lsls	r5, r5, #4
	lsls	r6, r6, #3
	movs	r1, #128
	lsls	r0, r0, #4
	adds	r6, r6, r2
	lsls	r1, r1, #11
	adds	r3, r3, r5
	subs	r3, r3, r0
	adds	r6, r6, r1
	movs	r0, #234
	adds	r0, #255
	mov	r1, r8
	adds	r2, r6, #0
	bl 0x0200cbd8
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02002b1c
	bl 0x0200cb30
	mov	sl, r0
	bl 0x0200cb30
	adds	r6, r0, #0
	bl 0x0200cb30
	adds	r5, r0, #0
	bl 0x0200cb30
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r5, r5, r0
	ldr	r1, [pc, #216]
	adds	r0, r7, #0
	mov	r9, r2
	bl 0x0200cbd0
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cc40
	mov	r1, sl
	movs	r2, #128
	lsls	r2, r2, #10
	lsls	r3, r1, #2
	adds	r3, r3, r2
	str	r3, [r7, #40]
	mov	r0, sl
	bl 0x0200cb48
	ldr	r3, [pc, #184]
	lsls	r6, r6, #3
	mov	r8, r3
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x62f8
	mov	r0, sl
	bl 0x0200cb40
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4a24
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	str	r3, [r7, #72]
	lsrs	r5, r5, #2
	ldr	r3, [r2, #112]
	add	r5, r9
	movs	r6, #0
	mov	r1, r9
	str	r5, [r7, #24]
	str	r5, [r7, #28]
	str	r1, [r7, #68]
	ldr	r5, [r7, #80]
	str	r0, [r7, #36]
	str	r6, [r7, #52]
	ldr	r3, [r3, #80]
	ldrb	r0, [r5, #16]
	mov	r8, r3
	bl 0x0200cb68
	ldrb	r3, [r5, #17]
	ldr	r1, [pc, #100]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r5, #17]
	ldrh	r3, [r1, #12]
	ldr	r0, [r5, #40]
	strb	r3, [r5, #16]
	bl 0x0200cbb8
	str	r6, [r5, #40]
	strb	r6, [r5, #27]
	mov	r2, r8
	ldrb	r3, [r2, #20]
	ldrb	r0, [r5, #5]
	strb	r3, [r5, #20]
	ldrb	r3, [r2, #21]
	strb	r3, [r5, #21]
	ldrb	r1, [r2, #5]
	movs	r2, #63
	adds	r3, r2, #0
	lsrs	r1, r1, #6
	lsls	r1, r1, #6
	ands	r3, r0
	orrs	r3, r1
	strb	r3, [r5, #5]
	mov	r1, r8
	ldrb	r3, [r1, #7]
	ldrb	r1, [r5, #7]
	lsrs	r3, r3, #6
	lsls	r3, r3, #6
	ands	r2, r1
	orrs	r2, r3
	strb	r2, [r5, #7]
	mov	r3, r8
	ldrh	r2, [r3, #8]
	ldr	r1, [pc, #28]
	ldrh	r3, [r5, #8]
	lsls	r2, r2, #22
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
.L_02002b1c:
	ldr	r1, [sp, #0]
	subs	r1, #1
	str	r1, [sp, #0]
	cmp	r1, #0
	blt.n	.L_02002b28
	b.n	.L_020029e8
.L_02002b28:
	b.n	.L_02002b3c
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0x0200254c
	.4byte 0x0200d6d0
	.2byte 0x021c
	.2byte 0x0300
.L_02002b3c:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	str	r3, [sp, #0]
	movs	r3, #1
	mov	fp, r3
.L_02002b68:
	movs	r0, #70
	adds	r0, #255
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #4]
	bl 0x0200cbd8
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02002c0a
	bl 0x0200cb30
	adds	r5, r0, #0
	bl 0x0200cb30
	ldr	r3, [sp, #0]
	lsrs	r5, r5, #4
	adds	r5, r3, r5
	lsrs	r0, r0, #4
	movs	r3, #128
	subs	r5, r5, r0
	lsls	r3, r3, #7
	mov	sl, r3
	adds	r3, r5, #0
	add	r3, sl
	mov	r9, r3
	bl 0x0200cb30
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #3
	mov	r8, r3
	bl 0x0200cb30
	ldr	r1, [pc, #116]
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl 0x0200cbd0
	movs	r1, #0
	adds	r0, r7, #0
	bl 0x0200cc40
	movs	r3, #160
	lsls	r3, r3, #9
	adds	r5, r5, r3
	str	r5, [r7, #40]
	mov	r0, r9
	bl 0x0200cb48
	ldr	r5, [pc, #88]
	adds	r1, r0, #0
	mov	r0, r8
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x62f8
	mov	r0, r9
	bl 0x0200cb40
	adds	r1, r0, #0
	mov	r0, r8
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [r7, #52]
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	lsrs	r6, r6, #1
	str	r3, [r7, #72]
	movs	r3, #128
	add	r6, sl
	lsls	r3, r3, #8
	str	r0, [r7, #36]
	str	r6, [r7, #24]
	str	r6, [r7, #28]
	str	r3, [r7, #68]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200cc60
.L_02002c0a:
	movs	r3, #1
	negs	r3, r3
	add	fp, r3
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_02002b68
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200d714
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200cb90
	adds	r5, #91
	strb	r0, [r5, #0]
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r3, #192
	movs	r0, #100
	lsls	r3, r3, #18
	adds	r0, r0, r5
	ldr	r6, [r3, #108]
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	sub	sp, #56
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_02002c68
	b.n	.L_02002ef4
.L_02002c68:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200cb90
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L_02002c82
	movs	r3, #1
	orrs	r0, r3
.L_02002c82:
	adds	r3, r5, #0
	adds	r3, #91
	strb	r0, [r3, #0]
	add	r7, sp, #44
	ldr	r3, [r5, #8]
	movs	r0, #128
	str	r3, [r7, #0]
	lsls	r0, r0, #12
	ldr	r3, [r5, #12]
	adds	r2, r7, #0
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	str	r3, [r7, #8]
	ldrh	r1, [r5, #6]
	bl 0x0200cb50
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	movs	r0, #0
	bl 0x0200cc18
	cmp	r0, #7
	bne.n	0x0200acf8
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #64]
	str	r3, [r5, #60]
	str	r3, [r5, #56]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #52]
	mov	r0, r8
	movs	r3, #1
	strh	r3, [r0, #0]
	movs	r0, #145
	bl 0x0200cde0
	movs	r0, #160
	lsls	r0, r0, #11
	movs	r2, #128
	adds	r1, r0, #0
	lsls	r2, r2, #9
	bl 0x0200cc50
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200cc50
	ldr	r3, [r5, #104]
	cmp	r3, #0
	beq.n	0x0200acf8
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x23c0
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	ldr	r3, [r5, #8]
	ldr	r1, [r5, #16]
	asrs	r3, r3, #20
	str	r3, [sp, #32]
	movs	r3, #184
	lsls	r3, r3, #1
	ldr	r4, [sp, #32]
	asrs	r1, r1, #20
	adds	r2, r0, r3
	ldr	r2, [r2, #0]
	lsls	r3, r1, #7
	adds	r3, r4, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	str	r2, [sp, #28]
	movs	r4, #212
	lsls	r4, r4, #1
	adds	r2, r0, r4
	ldr	r2, [r2, #0]
	subs	r4, #92
	adds	r2, r2, r3
	str	r2, [sp, #24]
	movs	r2, #164
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldr	r3, [r3, #0]
	asrs	r3, r3, #20
	str	r3, [sp, #20]
	adds	r3, r0, r4
	adds	r4, #52
	ldr	r2, [r3, #0]
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	adds	r4, #4
	asrs	r3, r3, #20
	str	r3, [sp, #16]
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r0, #1
	asrs	r3, r3, #20
	adds	r3, r1, r3
	subs	r3, #2
	asrs	r2, r2, #20
	negs	r0, r0
	str	r3, [sp, #8]
	str	r0, [sp, #40]
	subs	r3, r1, #1
	adds	r1, r1, r2
	subs	r1, #1
	mov	r8, r3
	mov	fp, r1
.L_02002d64:
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #20]
	movs	r4, #1
	adds	r3, r0, r1
	subs	r3, #1
	negs	r4, r4
	mov	r9, r3
	str	r4, [sp, #36]
	adds	r3, r0, r2
	adds	r6, r0, #0
	subs	r3, #1
	subs	r6, #1
	mov	sl, r3
.L_02002d80:
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #36]
	lsls	r3, r4, #7
	adds	r3, r3, r0
	lsls	r3, r3, #2
	ldr	r1, [sp, #28]
	str	r3, [sp, #12]
	adds	r2, r3, r1
	ldrb	r3, [r2, #2]
	cmp	r3, #77
	bne.n	.L_02002de0
	movs	r3, #0
	strb	r3, [r2, #2]
	mov	r2, sl
	mov	r3, fp
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	bl 0x0200cc30
	mov	r4, r8
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	movs	r0, #64
	str	r4, [sp, #4]
	str	r6, [sp, #0]
	bl 0x0200cc28
	movs	r0, #159
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200cde0
	mov	r3, r8
	movs	r4, #128
	lsls	r4, r4, #12
	lsls	r2, r3, #20
	lsls	r0, r6, #20
	adds	r0, r0, r4
	ldr	r1, [r5, #12]
	ldrh	r3, [r5, #6]
	adds	r2, r2, r4
	bl 0x0200ab4c
.L_02002de0:
	ldr	r4, [sp, #12]
	ldr	r0, [sp, #24]
	adds	r2, r4, r0
	ldrb	r3, [r2, #2]
	cmp	r3, #77
	bne.n	.L_02002e34
	movs	r3, #0
	strb	r3, [r2, #2]
	ldr	r2, [sp, #8]
	mov	r1, r9
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #1
	movs	r3, #2
	bl 0x0200cc30
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	movs	r0, #64
	str	r6, [sp, #0]
	bl 0x0200cc28
	movs	r0, #143
	lsls	r0, r0, #2
	bl 0x0200cde0
	mov	r3, r8
	movs	r4, #128
	lsls	r4, r4, #12
	lsls	r2, r3, #20
	lsls	r0, r6, #20
	adds	r0, r0, r4
	ldr	r1, [r5, #12]
	ldrh	r3, [r5, #6]
	adds	r2, r2, r4
	bl 0x0200ab4c
.L_02002e34:
	ldr	r0, [sp, #36]
	movs	r4, #1
	adds	r0, #1
	add	r9, r4
	adds	r6, #1
	add	sl, r4
	str	r0, [sp, #36]
	cmp	r0, #1
	ble.n	.L_02002d80
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #40]
	adds	r1, #1
	adds	r2, #1
	str	r1, [sp, #8]
	add	r8, r4
	add	fp, r4
	str	r2, [sp, #40]
	cmp	r2, #1
	ble.n	.L_02002d64
	ldr	r3, [r5, #24]
	movs	r4, #128
	lsls	r4, r4, #9
	cmp	r3, r4
	bge.n	.L_02002e72
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	str	r3, [r5, #24]
	ldr	r3, [r5, #28]
	adds	r3, r3, r2
	str	r3, [r5, #28]
.L_02002e72:
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	adds	r0, r5, #0
	bl 0x0200cbf8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	ldr	r3, [pc, #176]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	ldr	r6, [pc, #172]
	bl 0x0200ccb8
	ldr	r1, [r5, #8]
	ldr	r3, [r0, #8]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L_02002ea8
	movs	r1, #160
	lsls	r1, r1, #13
	cmp	r2, r1
	blt.n	.L_02002eb2
	b.n	.L_02002f28
.L_02002ea8:
	movs	r2, #160
	subs	r3, r3, r1
	lsls	r2, r2, #13
	cmp	r3, r2
	bge.n	.L_02002f28
.L_02002eb2:
	ldr	r3, [r5, #12]
	ldr	r2, [r0, #12]
	ldr	r4, [pc, #136]
	ldr	r1, [pc, #136]
	subs	r3, r3, r2
	adds	r3, r3, r4
	cmp	r3, r1
	bhi.n	.L_02002f28
	ldr	r3, [r5, #16]
	ldr	r0, [r0, #16]
	subs	r2, r3, r0
	cmp	r2, #0
	blt.n	.L_02002ed6
	movs	r3, #160
	lsls	r3, r3, #13
	cmp	r2, r3
	blt.n	.L_02002ee0
	b.n	.L_02002f28
.L_02002ed6:
	movs	r4, #160
	subs	r3, r0, r3
	lsls	r4, r4, #13
	cmp	r3, r4
	bge.n	.L_02002f28
.L_02002ee0:
	movs	r3, #2
	strh	r3, [r6, #4]
	ldrh	r3, [r6, #10]
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r3, #2
	adds	r2, r7, r0
	str	r5, [r6, #108]
	strh	r3, [r2, #0]
	b.n	.L_02002f28
.L_02002ef4:
	cmp	r3, #1
	bne.n	.L_02002f28
	adds	r3, r5, #0
	adds	r3, #91
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L_02002f1a
	ldr	r2, [pc, #64]
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r5, #24]
	ldr	r3, [r5, #28]
	adds	r3, r3, r2
	str	r3, [r5, #28]
	bl 0x0200a9c8
	b.n	.L_02002f28
.L_02002f1a:
	str	r2, [r5, #16]
	str	r2, [r5, #12]
	str	r2, [r5, #8]
	str	r2, [r5, #44]
	str	r2, [r5, #40]
	str	r2, [r5, #36]
	str	r2, [r5, #108]
.L_02002f28:
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200254c
	.4byte 0x0007ffff
	.4byte 0x001ffffe
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	ldr	r6, [sp, #24]
	adds	r5, r1, #0
	mov	r9, r2
	mov	sl, r3
	bl 0x0200ccb8
	mov	r8, r0
	adds	r0, r5, #0
	bl 0x0200ccb8
	adds	r5, r0, #0
	mov	r0, r8
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	ldr	r1, [r0, #8]
	adds	r0, r5, #0
	bl 0x0200cbe8
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl 0x0200cbd0
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200cc40
	adds	r3, r5, #0
	adds	r3, #85
	movs	r6, #0
	strb	r6, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200cbc0
	adds	r3, r5, #0
	mov	r2, r8
	adds	r3, #100
	str	r2, [r5, #104]
	mov	r0, sl
	strh	r6, [r3, #0]
	adds	r3, #2
	strh	r0, [r3, #0]
	mov	r2, r9
	subs	r3, #4
	strb	r2, [r3, #0]
	ldr	r3, [pc, #12]
	str	r3, [r5, #108]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.2byte 0xac2d
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r0
	mov	r3, fp
	adds	r3, #98
	ldrb	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200ccb8
	mov	r3, fp
	adds	r7, r0, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	ldr	r3, [r7, #80]
	mov	r2, fp
	mov	r9, r3
	ldr	r3, [r2, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r0, #128
	ldr	r3, [r2, #12]
	ldr	r2, [pc, #88]
	adds	r1, r6, #0
	adds	r3, r3, r2
	str	r3, [r5, #4]
	mov	r2, fp
	ldr	r3, [r2, #16]
	lsls	r0, r0, #14
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl 0x0200cb50
	ldr	r3, [pc, #60]
	movs	r2, #0
	mov	sl, r3
	adds	r3, r7, #0
	mov	r8, r2
	adds	r3, #85
	mov	r2, sl
	strh	r6, [r7, #6]
	strb	r2, [r3, #0]
	adds	r0, r7, #0
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	bl 0x0200cbe8
	ldr	r3, [pc, #40]
	mov	r2, sl
	str	r3, [r7, #108]
	adds	r3, r7, #0
	adds	r3, #90
	strb	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r7, #52]
	ldr	r3, [pc, #20]
	mov	r2, r9
	adds	r6, r6, r3
	b.n	.L_02003060
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xfff40000
	.4byte 0x0200ac41
	.2byte 0xc000
	.2byte 0xffff
.L_02003060:
	.2byte 0x4643
	strh	r6, [r2, #18]
	str	r3, [r7, #24]
	str	r3, [r7, #28]
	adds	r3, r7, #0
	mov	r2, r8
	adds	r3, #100
	strh	r2, [r3, #0]
	mov	r2, fp
	ldr	r3, [r2, #104]
	movs	r0, #104
	str	r3, [r7, #104]
	bl 0x0200cde0
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r5, #0
	adds	r7, #100
	movs	r3, #0
	ldrsh	r6, [r7, r3]
	cmp	r6, #0
	bne.n	.L_020030c4
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200cc08
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #9
	str	r0, [r5, #12]
	movs	r1, #1
	adds	r0, r5, #0
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	bl 0x0200cc68
	b.n	.L_0200312c
.L_020030c4:
	cmp	r6, #30
	bgt.n	.L_020030dc
	cmp	r6, #30
	bne.n	.L_0200312c
	movs	r0, #136
	bl 0x0200cde0
	ldr	r0, [r5, #104]
	movs	r1, #2
	bl 0x0200cbc0
	b.n	.L_0200312c
.L_020030dc:
	cmp	r6, #60
	bgt.n	.L_02003104
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldrh	r3, [r7, #0]
	ldr	r0, [r5, #104]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	ldr	r3, [pc, #48]
	asrs	r2, r2, #1
	ands	r2, r3
	lsls	r1, r2, #3
	subs	r1, r1, r2
	bl 0x0200cc60
	b.n	.L_0200312c
.L_02003104:
	movs	r1, #1
	ldr	r0, [r5, #104]
	bl 0x0200cbc0
	movs	r0, #184
	bl 0x0200cde0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200cc08
	movs	r3, #0
	str	r0, [r5, #12]
	str	r3, [r5, #104]
	movs	r0, #0
	b.n	.L_02003134
	.2byte 0x0000
	.2byte 0x0001
	.2byte 0x0000
.L_0200312c:
	ldrh	r3, [r7, #0]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #0]
.L_02003134:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r5, #0
	adds	r7, #100
	movs	r3, #0
	ldrsh	r6, [r7, r3]
	cmp	r6, #0
	bne.n	.L_02003182
	movs	r0, #136
	bl 0x0200cde0
	ldr	r0, [r5, #104]
	movs	r1, #2
	bl 0x0200cbc0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200cc08
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #48]
	movs	r3, #128
	str	r0, [r5, #12]
	lsls	r3, r3, #8
	adds	r0, r5, #0
	movs	r1, #1
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r3, [r5, #52]
	bl 0x0200cc68
	b.n	.L_020031d0
.L_02003182:
	cmp	r6, #32
	bgt.n	.L_020031aa
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldrh	r3, [r7, #0]
	ldr	r0, [r5, #104]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	ldr	r3, [pc, #48]
	asrs	r2, r2, #1
	ands	r2, r3
	lsls	r1, r2, #3
	subs	r1, r1, r2
	bl 0x0200cc60
	b.n	.L_020031d0
.L_020031aa:
	movs	r1, #1
	ldr	r0, [r5, #104]
	bl 0x0200cbc0
	movs	r0, #184
	bl 0x0200cde0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200cc08
	movs	r3, #0
	str	r0, [r5, #12]
	str	r3, [r5, #104]
	movs	r0, #0
	b.n	.L_020031d8
	.2byte 0x0001
	.2byte 0x0000
.L_020031d0:
	ldrh	r3, [r7, #0]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #0]
.L_020031d8:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200afc4
	movs	r3, #0
	str	r3, [r5, #8]
	str	r3, [r5, #12]
	str	r3, [r5, #16]
	str	r3, [r5, #36]
	str	r3, [r5, #40]
	str	r3, [r5, #44]
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #100
	movs	r0, #0
	ldrsh	r1, [r2, r0]
	ldrh	r3, [r2, #0]
	cmp	r1, #0
	beq.n	.L_02003210
	subs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02003276
.L_02003210:
	adds	r3, r5, #0
	adds	r3, #90
	movs	r0, #131
	strb	r1, [r3, #0]
	lsls	r0, r0, #1
	bl 0x0200cb90
	movs	r3, #1
	negs	r3, r3
	cmp	r0, #0
	bne.n	.L_02003236
	ldr	r3, [pc, #80]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #76]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
.L_02003236:
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_02003248
	adds	r0, r5, #0
	movs	r1, #9
	bl 0x0200cbc0
	b.n	.L_02003276
.L_02003248:
	ldrh	r1, [r5, #6]
	movs	r2, #128
	subs	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_0200325a
	adds	r3, r2, #0
.L_0200325a:
	ldr	r2, [pc, #36]
	cmp	r3, r2
	bge.n	.L_02003262
	adds	r3, r2, #0
.L_02003262:
	adds	r3, r1, r3
	adds	r0, r5, #0
	movs	r1, #2
	strh	r3, [r5, #6]
	bl 0x0200cbc0
	adds	r0, r5, #0
	movs	r1, #48
	bl 0x0200cbc8
.L_02003276:
	pop	{r5, pc}
	.4byte 0x03001150
	.4byte 0x0200d758
	.2byte 0xf000
	.2byte 0xffff
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #162
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020032b4
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200cb98
	bl 0x0200cd48
	bl 0x0200cd98
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200cba0
.L_020032b4:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #176]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #40
	bl 0x0200cd80
	adds	r7, r0, #0
.L_020032d8:
	bl 0x0200b284
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r1, [pc, #132]
	ldr	r3, [r7, #8]
	add	r2, sp, #28
	mov	sl, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	mov	r0, sl
	str	r3, [sp, #12]
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [sp, #8]
	str	r3, [r0, #8]
	ldr	r2, [sp, #12]
	str	r3, [sp, #16]
	adds	r3, r7, #0
	adds	r3, #34
	str	r2, [sp, #20]
	str	r3, [sp, #4]
	adds	r1, r2, #0
	ldrb	r0, [r3, #0]
	ldr	r2, [sp, #16]
	bl 0x0200cc10
	str	r0, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r1, [r7, #8]
	ldr	r0, [sp, #16]
	ldr	r3, [pc, #68]
	ldr	r6, [r7, #16]
	subs	r1, r2, r1
	subs	r6, r0, r6
	mov	r8, r3
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x182d
	movs	r0, #128
	lsls	r0, r0, #11
	cmp	r5, r0
	bge.n	.L_02003390
	ldr	r3, [pc, #36]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #36]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
	mov	r9, r2
	lsls	r3, r2, #16
	ldr	r2, [pc, #24]
	cmp	r3, r2
	bne.n	.L_020033bc
	b.n	.L_02003552
	.4byte 0x02000240
	.4byte 0xfff00000
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200d798
	.2byte 0x0000
	.2byte 0xffff
.L_02003390:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl 0x0200cb38
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #128
	ldr	r2, [pc, #16]
	mov	r9, r0
	lsls	r3, r3, #6
	add	r3, r9
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r9, r3
	b.n	.L_020033bc
	.2byte 0xc000
	.2byte 0xffff
.L_020033bc:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200cb50
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl 0x0200cc10
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_0200343e
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	bl 0x0200cc08
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_0200343e
	ldr	r0, [sp, #12]
	mov	r2, sl
	str	r0, [r2, #0]
	ldr	r3, [sp, #8]
	adds	r0, r7, #0
	str	r3, [r2, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r2, r7, #0
	str	r3, [r7, #52]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #8]
	ldr	r2, [r7, #12]
	bl 0x0200cbf8
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200cbc0
	adds	r0, r7, #0
	movs	r1, #48
	bl 0x0200cbc8
	adds	r0, r7, #0
	bl 0x0200cc00
	ldr	r3, [pc, #292]
	str	r3, [r7, #108]
	b.n	.L_020034e8
.L_0200343e:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_02003534
.L_02003452:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl 0x0200cc08
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02003508
	ldrh	r3, [r7, #32]
	movs	r2, #89
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r0, #0
	adds	r2, r2, r5
	mov	sl, r0
	mov	r8, r2
.L_02003480:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_020034aa
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020034aa
	cmp	r5, r7
	beq.n	.L_020034aa
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl 0x0200cc78
	cmp	r0, #0
	bge.n	.L_02003508
.L_020034aa:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02003480
	ldr	r0, [r6, #0]
	movs	r3, #128
	str	r0, [sp, #20]
	lsls	r3, r3, #10
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	str	r2, [sp, #16]
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #52]
	ldr	r3, [r6, #8]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	bl 0x0200cbf8
	adds	r0, r7, #0
	bl 0x0200cc00
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_0200352e
.L_020034e8:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200cb50
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200cc10
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_02003452
.L_02003508:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	ldr	r2, [r7, #12]
	adds	r0, r7, #0
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #16]
	bl 0x0200cbf8
	adds	r0, r7, #0
	bl 0x0200cc00
	movs	r0, #2
	bl 0x0200cb18
	b.n	.L_020032d8
.L_0200352e:
	movs	r0, #10
	bl 0x0200cb18
.L_02003534:
	movs	r3, #0
	str	r3, [r7, #108]
	adds	r1, r7, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200cbc0
.L_02003552:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0xb1f9
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #80]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #40
	bl 0x0200cd80
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	adds	r7, r0, #0
	strh	r3, [r2, #0]
.L_0200358a:
	bl 0x0200b284
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r1, [pc, #32]
	ldr	r3, [r7, #8]
	add	r2, sp, #28
	mov	sl, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	mov	r0, sl
	str	r3, [sp, #12]
	str	r3, [r0, #0]
	b.n	.L_020035d0
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x0200e8b0
	.2byte 0x0000
	.2byte 0xfff0
.L_020035d0:
	.2byte 0x68fb
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [sp, #8]
	str	r3, [r0, #8]
	ldr	r2, [sp, #12]
	str	r3, [sp, #16]
	adds	r3, r7, #0
	adds	r3, #34
	str	r2, [sp, #20]
	str	r3, [sp, #4]
	adds	r1, r2, #0
	ldrb	r0, [r3, #0]
	ldr	r2, [sp, #16]
	bl 0x0200cc10
	str	r0, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r1, [r7, #8]
	ldr	r0, [sp, #16]
	ldr	r3, [pc, #60]
	ldr	r6, [r7, #16]
	subs	r1, r2, r1
	subs	r6, r0, r6
	mov	r8, r3
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x182d
	movs	r0, #128
	lsls	r0, r0, #11
	cmp	r5, r0
	bge.n	.L_0200364c
	ldr	r3, [pc, #28]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #28]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
	mov	r9, r2
	lsls	r3, r2, #16
	ldr	r2, [pc, #16]
	cmp	r3, r2
	bne.n	.L_02003678
	b.n	.L_02003842
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200d798
	.2byte 0x0000
	.2byte 0xffff
.L_0200364c:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl 0x0200cb38
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #128
	ldr	r2, [pc, #16]
	mov	r9, r0
	lsls	r3, r3, #6
	add	r3, r9
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r9, r3
.L_02003672:
	b.n	.L_02003678
	.2byte 0xc000
	.2byte 0xffff
.L_02003678:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200cb50
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl 0x0200cc10
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_020036f2
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	bl 0x0200cc08
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_020036f2
	ldr	r0, [sp, #12]
	mov	r2, sl
	str	r0, [r2, #0]
	ldr	r3, [sp, #8]
	adds	r0, r7, #0
	str	r3, [r2, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r2, r7, #0
	str	r3, [r7, #52]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [r7, #12]
	ldr	r3, [sp, #8]
	bl 0x0200cbf8
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200cbc0
	adds	r0, r7, #0
	movs	r1, #48
	bl 0x0200cbc8
	movs	r5, #0
	b.n	.L_0200371a
.L_020036f2:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
.L_020036fe:
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_02003842
.L_02003706:
	ldr	r3, [pc, #360]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_02003712
	b.n	.L_02003842
.L_02003712:
	movs	r0, #1
	bl 0x0200cb18
	adds	r5, #1
.L_0200371a:
	cmp	r5, #179
	bgt.n	.L_02003728
	adds	r0, r7, #0
	bl 0x0200cc70
	cmp	r0, #0
	beq.n	.L_02003706
.L_02003728:
	ldr	r3, [pc, #328]
	str	r3, [r7, #108]
	b.n	.L_020037f6
.L_0200372e:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl 0x0200cc08
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02003816
	ldr	r3, [pc, #296]
.L_02003748:
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02003842
	ldrh	r3, [r7, #32]
	movs	r2, #89
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r0, #0
	adds	r2, r2, r5
	mov	sl, r0
	mov	r8, r2
.L_02003766:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02003790
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003790
	cmp	r5, r7
	beq.n	.L_02003790
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl 0x0200cc78
	cmp	r0, #0
	bge.n	.L_02003816
.L_02003790:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02003766
	ldr	r0, [r6, #0]
	movs	r3, #128
	str	r0, [sp, #20]
	lsls	r3, r3, #10
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	str	r2, [sp, #16]
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #52]
	movs	r5, #0
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl 0x0200cbf8
	b.n	.L_020037ce
.L_020037c6:
	movs	r0, #1
	bl 0x0200cb18
	adds	r5, #1
.L_020037ce:
	cmp	r5, #179
	bgt.n	.L_020037e6
	adds	r0, r7, #0
	bl 0x0200cc70
	cmp	r0, #0
	bne.n	.L_020037e6
	ldr	r3, [pc, #144]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_020037c6
.L_020037e6:
	ldr	r3, [pc, #136]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02003842
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_0200383c
.L_020037f6:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200cb50
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200cc10
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_0200372e
.L_02003816:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	ldr	r2, [r7, #12]
	adds	r0, r7, #0
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #16]
	bl 0x0200cbf8
	adds	r0, r7, #0
	bl 0x0200cc00
	movs	r0, #2
	bl 0x0200cb18
	b.n	.L_0200358a
.L_0200383c:
	movs	r0, #10
	bl 0x0200cb18
.L_02003842:
	movs	r3, #0
	str	r3, [r7, #108]
	adds	r1, r7, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
.L_0200384c:
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200cbc0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200e8b0
	.4byte 0x0200b1f9
	.4byte 0x80184b01
	.4byte 0x00004770
	.2byte 0xe8b0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #92]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	sub	sp, #32
	bl 0x0200cd80
	adds	r5, r0, #0
	ldrh	r3, [r5, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #60]
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	str	r3, [sp, #16]
.L_020038b6:
	bl 0x0200b284
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #56]
	str	r3, [r5, #64]
	movs	r3, #0
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	ldr	r2, [pc, #28]
	ldr	r3, [r5, #8]
	movs	r1, #128
	lsls	r1, r1, #12
	ands	r3, r2
.L_020038de:
	mov	r9, r1
	add	r6, sp, #20
	add	r3, r9
	str	r3, [r6, #0]
	mov	r8, r3
	b.n	.L_020038f8
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
.L_020038f8:
	.2byte 0x68eb
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	ands	r3, r2
	adds	r7, r3, r1
	mov	r2, r8
	str	r7, [r6, #8]
	str	r2, [sp, #8]
	str	r7, [sp, #4]
	movs	r3, #34
	adds	r3, r3, r5
	ldrb	r0, [r3, #0]
	adds	r1, r2, #0
	adds	r2, r7, #0
	mov	fp, r3
	bl 0x0200cc10
	str	r0, [sp, #12]
	movs	r0, #128
	ldr	r1, [sp, #16]
	lsls	r0, r0, #13
	adds	r2, r6, #0
	bl 0x0200cb50
	mov	r1, fp
	ldrb	r0, [r1, #0]
	ldr	r2, [r6, #8]
	ldr	r1, [r6, #0]
	bl 0x0200cc10
	mov	sl, r0
	cmp	r0, #255
	beq.n	.L_0200398c
	mov	r2, fp
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200cc08
	ldr	r3, [r5, #12]
	subs	r0, r0, r3
	cmp	r0, r9
	bgt.n	.L_0200398c
	ldr	r3, [sp, #8]
	ldr	r2, [pc, #48]
	str	r3, [r6, #0]
	ldr	r1, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r1, [r6, #8]
	str	r3, [r5, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #52]
	adds	r3, r5, #0
	adds	r3, #100
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200cbc0
	adds	r0, r5, #0
	movs	r1, #48
	bl 0x0200cbc8
.L_0200397c:
	ldr	r3, [pc, #8]
	str	r3, [r5, #108]
	b.n	.L_02003a36
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0xb1f9
	.2byte 0x0200
.L_0200398c:
	add	r1, sp, #16
	ldrh	r1, [r1, #0]
	movs	r3, #0
	mov	r2, r8
	strh	r1, [r5, #6]
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	str	r2, [r5, #8]
	str	r7, [r5, #16]
	b.n	.L_02003a82
.L_020039a0:
	mov	r3, fp
	ldrb	r0, [r3, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl 0x0200cc08
	ldr	r3, [r5, #12]
	movs	r1, #128
	subs	r0, r0, r3
	lsls	r1, r1, #12
	cmp	r0, r1
	bgt.n	.L_02003a56
	ldrh	r3, [r5, #32]
	movs	r2, #0
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #20]
	movs	r3, #89
	adds	r3, r3, r6
	mov	r9, r2
	mov	r8, r3
.L_020039ce:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_020039f8
	mov	r1, r8
	ldrb	r2, [r1, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020039f8
	cmp	r6, r5
	beq.n	.L_020039f8
	ldrh	r3, [r6, #32]
	adds	r0, r6, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #20
	bl 0x0200cc78
	cmp	r0, #0
	bge.n	.L_02003a56
.L_020039f8:
	movs	r2, #1
	add	r9, r2
	movs	r3, #128
	mov	r1, r9
	add	r8, r3
	adds	r6, #128
	cmp	r1, #63
	ble.n	.L_020039ce
	ldr	r2, [r7, #0]
	adds	r0, r5, #0
	str	r2, [sp, #8]
	ldr	r3, [r7, #8]
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #52]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	bl 0x0200cbf8
	adds	r0, r5, #0
	bl 0x0200cc00
	ldr	r1, [sp, #12]
	cmp	sl, r1
	bne.n	.L_02003a7c
.L_02003a36:
	movs	r0, #128
	ldr	r1, [sp, #16]
	add	r2, sp, #20
	lsls	r0, r0, #13
	bl 0x0200cb50
	mov	r2, fp
	add	r7, sp, #20
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl 0x0200cc10
	mov	sl, r0
	cmp	r0, #255
	bne.n	.L_020039a0
.L_02003a56:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	ldr	r1, [sp, #8]
	ldr	r3, [sp, #4]
	bl 0x0200cbf8
	adds	r0, r5, #0
	bl 0x0200cc00
	movs	r0, #2
	bl 0x0200cb18
	b.n	.L_020038b6
.L_02003a7c:
	movs	r0, #10
	bl 0x0200cb18
.L_02003a82:
	movs	r3, #0
	str	r3, [r5, #108]
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
.L_02003a90:
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #52]
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200cbc0
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02003af4
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003af4
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
.L_02003aea:
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_02003af4:
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
.L_02003b32:
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
	bl 0x0200ccb8
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02003b78
	cmp	r7, #0
	beq.n	.L_02003b78
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02003b80
.L_02003b78:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02003b80:
	mov	r3, sl
	bl 0x0200cbd8
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02003b8e
	b.n	.L_02003cda
.L_02003b8e:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200cbc0
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200cbd0
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200cc40
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
	bl 0x0200bab0
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
	beq.n	.L_02003cda
	cmp	r7, #0
	beq.n	.L_02003cda
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02003c10
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200ccf0
.L_02003c10:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003c30
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x0200bab0
.L_02003c30:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02003c44
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
.L_02003c40:
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02003c44:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02003c8a
	ldr	r3, [pc, #152]
.L_02003c52:
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_02003c72
.L_02003c5e:
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200cb08
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_02003c84
.L_02003c72:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200cb08
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_02003c84:
	bl 0x0200cb08
	str	r0, [r6, #52]
.L_02003c8a:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02003ca6
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200cbc0
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200cbd0
.L_02003ca6:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003cb8
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02003cb8:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003cca
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02003cca:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003cda
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02003cda:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200e86c
	.4byte 0x0200baf9
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r4, [pc, #268]
	movs	r1, #1
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	negs	r1, r1
	sub	sp, #4
	cmp	r3, r1
	beq.n	.L_02003e04
	lsls	r3, r3, #3
	adds	r3, r3, r4
	adds	r3, #32
	mov	r8, r3
	ldr	r3, [pc, #248]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	str	r4, [sp, #0]
	bl 0x0200ccb8
	mov	r1, r8
	ldr	r3, [r0, #8]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	ldr	r4, [sp, #0]
.L_02003d34:
	cmp	r3, r2
	bne.n	.L_02003d44
.L_02003d38:
	ldr	r3, [r0, #16]
	movs	r5, #2
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_02003d4c
.L_02003d44:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r4, #12]
.L_02003d4c:
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	movs	r2, #1
	negs	r2, r2
	ldr	r1, [pc, #192]
	cmp	r3, r2
	beq.n	.L_02003e04
	movs	r5, #14
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L_02003e04
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	str	r4, [sp, #0]
	adds	r3, r2, #0
	adds	r3, #228
	ldr	r0, [r3, #0]
	ldr	r5, [r3, #4]
	ldr	r3, [r2, #0]
	ands	r0, r1
	ands	r5, r1
	ldr	r6, [r3, #4]
	movs	r1, #16
	ldrsh	r3, [r4, r1]
	ldr	r2, [pc, #156]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	mov	sl, r3
	mov	r3, r8
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	lsls	r1, r1, #20
	subs	r7, r1, r0
	movs	r0, #2
	ldrsh	r2, [r3, r0]
	movs	r0, #0
	lsls	r2, r2, #20
	bl 0x0200cc08
	mov	r2, r8
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	subs	r0, r0, r6
	lsls	r3, r3, #20
	subs	r3, r3, r5
	subs	r3, r3, r6
	subs	r2, r3, r0
	asrs	r7, r7, #16
	adds	r0, r0, r3
	asrs	r0, r0, #16
	adds	r3, r7, #0
	movs	r5, #167
	asrs	r2, r2, #16
	adds	r1, r0, #0
	adds	r3, #15
	lsls	r5, r5, #1
	adds	r2, #14
	adds	r1, #58
	ldr	r4, [sp, #0]
	cmp	r3, r5
	bhi.n	.L_02003e04
	movs	r0, #15
	negs	r0, r0
	cmp	r2, r0
	blt.n	.L_02003e04
	cmp	r2, #239
	bgt.n	.L_02003e04
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r7, r3
	movs	r3, #255
	ands	r2, r3
	movs	r3, #0
	str	r3, [r4, #20]
	lsls	r3, r7, #16
	orrs	r2, r3
	ldr	r3, [pc, #48]
	adds	r0, r4, #0
	orrs	r2, r3
	movs	r3, #128
	str	r2, [r4, #24]
	lsls	r3, r3, #3
	mov	r2, sl
	orrs	r2, r3
	str	r2, [r4, #28]
	adds	r0, #20
	bl 0x0200cb80
.L_02003e04:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200e8b4
	.4byte 0x02000240
	.4byte 0xffff0000
	.4byte 0x020036e0
	.2byte 0x8800
	.2byte 0x8000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	str	r0, [sp, #44]
	ldr	r0, [pc, #540]
	str	r1, [sp, #40]
	mov	r8, r0
	movs	r1, #32
	add	r1, r8
	mov	r9, r1
	mov	ip, r9
	adds	r5, r2, #0
	mov	r2, ip
	adds	r6, r3, #0
	str	r2, [sp, #8]
	ldr	r3, [pc, #520]
	movs	r1, #4
	ldr	r7, [sp, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xa80b
	ldrh	r0, [r0, #0]
	mov	r1, r8
	strh	r0, [r1, #4]
	add	r1, sp, #40
	ldrh	r1, [r1, #0]
	mov	r3, r8
	strh	r1, [r3, #0]
	strh	r5, [r3, #2]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	r5, r8
	mov	r0, r8
	adds	r3, #255
	mov	r1, r8
	strh	r6, [r5, #6]
	movs	r2, #0
	strh	r7, [r0, #8]
	strh	r3, [r1, #12]
	mov	r3, r8
	strh	r2, [r3, #10]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #132
	mov	ip, r3
	lsls	r2, r2, #1
	mov	r1, ip
	add	r2, ip
	adds	r1, #236
	ldr	r0, [r1, #0]
	ldr	r3, [r2, #8]
	ldr	r5, [r2, #48]
	adds	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #32]
	adds	r1, #4
	ldr	r3, [r2, #12]
	ldr	r2, [r1, #0]
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [sp, #28]
	mov	r3, ip
	adds	r3, #244
	ldr	r3, [r3, #0]
	subs	r3, r3, r0
.L_02003eb2:
	asrs	r3, r3, #20
	str	r3, [sp, #24]
	mov	r3, ip
	adds	r3, #248
	ldr	r3, [r3, #0]
	asrs	r0, r0, #20
	subs	r3, r3, r2
	asrs	r2, r2, #20
	lsls	r2, r2, #7
	adds	r2, r2, r0
	lsls	r2, r2, #2
	asrs	r3, r3, #20
	adds	r5, r5, r2
	movs	r0, #0
	str	r3, [sp, #20]
	str	r5, [sp, #36]
	str	r0, [sp, #12]
	cmp	r0, r3
	bge.n	.L_02003fa8
.L_02003ed8:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #36]
	ldr	r5, [sp, #24]
	lsls	r3, r1, #9
	adds	r2, r2, r3
	movs	r3, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, r5
.L_02003eea:
	bge.n	.L_02003f9c
.L_02003eec:
	mov	r0, fp
	ldrb	r5, [r0, #2]
	cmp	r5, #0
	beq.n	.L_02003f8c
	ldr	r1, [sp, #44]
	cmp	r5, r1
	bcc.n	.L_02003f8c
	adds	r1, #1
	mov	sl, r1
	cmp	r5, sl
	bhi.n	.L_02003f8c
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #32]
	mov	r0, r9
	adds	r7, r2, r3
	strh	r7, [r0, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #28]
	add	r0, sp, #40
	ldrh	r0, [r0, #0]
	adds	r6, r1, r2
	mov	r3, r9
	mov	r1, r9
	strh	r6, [r3, #2]
	strh	r0, [r1, #4]
	ldr	r1, [sp, #40]
	movs	r0, #10
	adds	r1, #1
	adds	r0, #255
	str	r1, [sp, #40]
	bl 0x0200cb90
	cmp	r0, #0
	bne.n	.L_02003f40
	cmp	r5, sl
	bne.n	.L_02003f7e
	mov	r3, r9
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	bl 0x0200cb98
	b.n	.L_02003f7e
.L_02003f40:
	mov	r1, r9
	movs	r5, #4
	ldrsh	r0, [r1, r5]
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_02003f7e
	mov	r2, r8
	ldrh	r4, [r2, #6]
	ldrh	r5, [r2, #8]
	movs	r3, #8
	ldrsh	r1, [r2, r3]
	movs	r3, #6
	ldrsh	r0, [r2, r3]
	movs	r2, #64
	adds	r3, r2, #0
	ands	r3, r4
	ands	r2, r5
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	asrs	r3, r3, #16
	asrs	r2, r2, #16
	orrs	r7, r3
	orrs	r6, r2
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200cc20
.L_02003f7e:
	mov	r0, r8
	ldrh	r3, [r0, #10]
	mov	r1, r8
	adds	r3, #1
	strh	r3, [r1, #10]
	movs	r5, #8
	add	r9, r5
.L_02003f8c:
	ldr	r2, [sp, #16]
	ldr	r5, [sp, #24]
	adds	r2, #1
	movs	r3, #4
	str	r2, [sp, #16]
	add	fp, r3
	cmp	r2, r5
	blt.n	.L_02003eec
.L_02003f9c:
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #12]
	cmp	r0, r1
	blt.n	.L_02003ed8
.L_02003fa8:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_02004000
	ldr	r3, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
	ldr	r3, [r0, #8]
	movs	r2, #0
	asrs	r4, r3, #20
	ldr	r3, [r0, #16]
	mov	r0, r8
	asrs	r1, r3, #20
	ldr	r3, [sp, #8]
	mov	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	cmp	r2, r3
	bge.n	.L_02004000
.L_02003fda:
	mov	r0, r9
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	cmp	r3, r4
	bne.n	.L_02003ff0
	movs	r5, #2
	ldrsh	r3, [r0, r5]
	cmp	r3, r1
	bne.n	.L_02003ff0
	mov	r0, r8
	strh	r2, [r0, #12]
.L_02003ff0:
	movs	r3, #8
	mov	r0, r8
	add	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	adds	r2, #1
	cmp	r2, r3
	blt.n	.L_02003fda
.L_02004000:
	movs	r0, #128
	lsls	r0, r0, #1
	bl 0x0200cb58
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r2, #63
.L_0200400e:
	ldr	r3, [pc, #80]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bge.n	.L_0200400e
	bl 0x0200cb78
	mov	r1, r8
	strh	r0, [r1, #16]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #1
	asrs	r0, r0, #16
	bl 0x0200cb70
	adds	r0, r5, #0
	bl 0x0200cb60
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200cb20
	mov	r3, r8
	movs	r2, #10
	ldrsh	r0, [r3, r2]
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200e8b4
	.4byte 0x03000258
	.4byte 0x02000240
	.4byte 0x11111111
	.2byte 0xbcf9
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r1, [pc, #116]
	movs	r2, #133
.L_02004074:
	mov	r8, r1
	lsls	r2, r2, #2
	add	r8, r2
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
	mov	r1, r8
	ldr	r5, [r0, #8]
	ldr	r6, [r0, #16]
	mov	sl, r0
	movs	r2, #128
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200ccc0
	asrs	r5, r5, #20
	mov	r2, r8
	asrs	r6, r6, #20
	ldr	r0, [r2, #0]
	lsls	r1, r5, #4
	lsls	r2, r6, #4
	adds	r1, #8
	adds	r2, #8
	bl 0x0200ccd0
	movs	r0, #1
	bl 0x0200cb18
	movs	r3, #128
	lsls	r3, r3, #12
	lsls	r5, r5, #20
	lsls	r6, r6, #20
	adds	r5, r5, r3
	mov	r1, sl
	adds	r6, r6, r3
	ldr	r2, [r1, #12]
	adds	r3, r6, #0
	adds	r1, r5, #0
	mov	r0, sl
	bl 0x0200cbe8
	movs	r0, #4
	bl 0x0200cca0
	bl 0x0200cd28
	ldr	r2, [pc, #20]
.L_020040d8:
	ldr	r3, [r0, #12]
	adds	r3, r3, r2
	str	r3, [r0, #12]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb560
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #100]
	movs	r1, #133
	lsls	r1, r1, #2
.L_020040fc:
	adds	r3, r3, r1
	mov	r8, r0
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200ccb8
	mov	r2, r8
	ldrh	r1, [r2, #6]
	movs	r2, #64
	ldr	r6, [r0, #8]
	adds	r3, r2, #0
.L_02004112:
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	asrs	r6, r6, #20
	orrs	r6, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r0, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl 0x0200c068
	movs	r0, #161
	bl 0x0200cde0
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200cc20
	movs	r0, #12
	bl 0x0200cca0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r1, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r1, r1, r2
	mov	r8, r0
	ldr	r0, [r1, #0]
	sub	sp, #8
	mov	sl, r1
	bl 0x0200ccb8
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	movs	r2, #64
	asrs	r7, r3, #20
	mov	r3, r8
	ldrh	r1, [r3, #6]
	adds	r3, r2, #0
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	orrs	r7, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r6, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl 0x0200c068
	movs	r0, #229
	bl 0x0200cde0
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #2
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200cc20
	movs	r0, #12
	bl 0x0200cca0
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
	movs	r1, #0
	bl 0x0200cc40
	movs	r2, #226
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #19
	adds	r2, #255
	adds	r3, #74
	strh	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #16]
	movs	r7, #0
	orrs	r3, r2
	strh	r3, [r1, #0]
	mov	r2, sl
	b.n	.L_02004214
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00008000
	.2byte 0x0240
	.2byte 0x0200
.L_02004214:
	movs	r3, #1
	mov	r1, r8
	strh	r3, [r1, #14]
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl 0x0200cce0
	movs	r0, #16
	bl 0x0200cca0
.L_02004228:
	cmp	r7, #5
	bne.n	.L_02004232
	movs	r0, #204
	bl 0x0200cde0
.L_02004232:
	ldr	r3, [r6, #24]
	ldr	r1, [pc, #88]
	ldr	r2, [pc, #92]
	adds	r3, r3, r1
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	adds	r7, #1
	bl 0x0200cb18
	cmp	r7, #39
	ble.n	.L_02004228
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
	movs	r3, #0
	adds	r0, #84
	strb	r3, [r0, #0]
	mov	r1, r8
	strh	r3, [r1, #14]
	movs	r3, #192
	lsls	r3, r3, #18
.L_02004272:
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200cd70
	bl 0x0200cd78
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfffffc00
	.4byte 0xfffffd00
	.4byte 0xffff6667
	.2byte 0x0240
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
	bl 0x0200cb08
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_020042d0
	adds	r3, #15
.L_020042d0:
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
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200ccb8
	adds	r7, r0, #0
	bl 0x0200cca8
	movs	r0, #0
	bl 0x0200cda0
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200cd18
	bl 0x0200cbf0
	movs	r0, #1
	bl 0x0200cb18
	movs	r3, #130
	lsls	r3, r3, #16
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r5, r7, #0
	str	r3, [r7, #72]
	adds	r5, #85
	movs	r3, #0
	str	r3, [r7, #68]
	strb	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r4, #214
	lsls	r4, r4, #1
	movs	r2, #128
	adds	r3, r3, r4
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	bl 0x0200cd68
	bl 0x0200cd78
	movs	r0, #204
	bl 0x0200cde0
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200cca0
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #256]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #163
	lsls	r3, r3, #8
	adds	r3, #215
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_02004392:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200cb48
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200cb40
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200cb30
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #188]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200cb30
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #176]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r5, r5, r3
	adds	r5, r5, r4
	ldr	r4, [r6, #4]
	str	r5, [r6, #8]
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #0]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r4, [sp, #0]
	ldr	r4, [pc, #156]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl 0x0200bb30
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_02004392
	movs	r0, #188
	bl 0x0200cde0
	ldr	r5, [pc, #112]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200cd08
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200cce0
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200cc50
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200cc50
	bl 0x0200cc58
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200cd08
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200cca0
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200cce0
	bl 0x0200ccb0
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c2a1
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #156]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200ccb8
	ldr	r3, [r0, #8]
	ldr	r6, [pc, #144]
	asrs	r3, r3, #20
	mov	r8, r3
	ldr	r3, [r0, #16]
	adds	r5, r6, #0
	asrs	r3, r3, #20
	mov	sl, r3
	movs	r1, #10
	ldrsh	r3, [r6, r1]
	movs	r7, #0
	adds	r5, #32
	ldrh	r2, [r6, #10]
	cmp	r7, r3
	bge.n	.L_0200452c
.L_020044c4:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, r8
	bne.n	.L_02004520
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	cmp	r3, sl
	bne.n	.L_02004520
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	bl 0x0200cb90
	cmp	r0, #0
	bne.n	.L_020044f4
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x0200c0f0
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	bl 0x0200cb98
	strh	r7, [r6, #12]
	b.n	.L_0200452c
.L_020044f4:
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r7, r3
	beq.n	.L_0200452c
	adds	r0, r6, #0
	adds	r1, r5, #0
	strh	r7, [r6, #12]
	bl 0x0200c160
	movs	r2, #2
	ldrsh	r0, [r6, r2]
	mov	r1, r8
	bl 0x0200cbb0
	movs	r3, #2
	ldrsh	r0, [r6, r3]
	mov	r1, sl
	adds	r0, #8
	bl 0x0200cbb0
	movs	r0, #1
	b.n	.L_0200452e
.L_02004520:
	lsls	r3, r2, #16
	adds	r7, #1
	asrs	r3, r3, #16
	adds	r5, #8
	cmp	r7, r3
	blt.n	.L_020044c4
.L_0200452c:
	movs	r0, #0
.L_0200452e:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xe8b4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	ldr	r3, [pc, #156]
	str	r2, [sp, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r9, r0
	ldr	r0, [r3, #0]
	mov	fp, r1
	bl 0x0200ccb8
	movs	r3, #192
	ldr	r5, [pc, #140]
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	mov	sl, r3
	bl 0x0200cba8
	adds	r7, r0, #0
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	adds	r0, #8
	bl 0x0200cba8
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_0200458e
	cmp	r0, #0
	beq.n	.L_020045e2
.L_0200458e:
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #0
	bl 0x0200cbb0
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	movs	r1, #0
	adds	r0, #8
	bl 0x0200cbb0
	mov	r3, r9
	adds	r2, r7, r3
	mov	r3, r8
	movs	r1, #128
	add	r3, fp
	lsls	r1, r1, #12
	lsls	r3, r3, #20
	adds	r3, r3, r1
	str	r3, [r6, #16]
	movs	r3, #230
	lsls	r3, r3, #1
	add	r3, sl
	lsls	r2, r2, #20
	adds	r2, r2, r1
	ldr	r1, [r3, #0]
	str	r2, [r6, #8]
	str	r2, [r1, #8]
	ldr	r3, [r6, #16]
	str	r3, [r1, #16]
	bl 0x0200cbf0
	bl 0x0200c2f8
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	str	r2, [r3, #0]
.L_020045e2:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0xe8b4
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r4, r1, #0
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r1, #128
	ldr	r6, [r1, #0]
	ldr	r1, [pc, #148]
	adds	r5, r0, #0
	str	r5, [r1, #0]
	ldr	r1, [pc, #148]
	str	r4, [r1, #0]
	ldr	r1, [pc, #148]
	str	r2, [r1, #0]
	ldr	r2, [pc, #148]
	str	r3, [r2, #0]
	movs	r2, #255
	ldrh	r3, [r5, #0]
	b.n	.L_02004642
.L_0200461c:
	ldrh	r0, [r4, #0]
	adds	r4, #2
	ldrh	r2, [r4, #0]
	adds	r4, #2
	ldrh	r1, [r5, #0]
	ldrh	r3, [r4, #0]
	adds	r5, #2
	adds	r4, #2
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	movs	r2, #160
	lsls	r2, r2, #19
	lsls	r1, r1, #1
	orrs	r3, r0
	adds	r1, r1, r2
	strh	r3, [r1, #0]
	ldrh	r3, [r5, #0]
	movs	r2, #255
.L_02004642:
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_02004650
	ldrh	r3, [r4, #0]
	cmp	r3, r2
	bne.n	.L_0200461c
.L_02004650:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r0, #160
	lsls	r2, r2, #24
	adds	r3, #212
	lsls	r0, r0, #19
	adds	r1, r6, #0
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r1, r6, r2
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [pc, #56]
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200cd58
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_0200469a
	bl 0x0200c8e8
.L_0200469a:
	pop	{r5, r6, pc}
	.4byte 0x0200e88c
	.4byte 0x0200e890
	.4byte 0x0200e894
	.4byte 0x0200e880
	.4byte 0x05000200
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #44]
	ldr	r5, [pc, #64]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #64]
	ldr	r0, [r3, #0]
	bl 0x0200c794
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #32]
	strh	r0, [r5, #0]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #28]
	movs	r1, #144
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #24]
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #48]
	bl 0x0200cb20
	b.n	.L_02004714
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0x0200e89c
	.4byte 0x0200e898
	.4byte 0x0200e88c
	.4byte 0x0200e888
	.4byte 0x0200e884
	.4byte 0x0200e87c
	.2byte 0xc7b9
	.2byte 0x0200
.L_02004714:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	ldr	r0, [pc, #8]
	bl 0x0200cb28
	pop	{pc}
	.2byte 0x0000
	.2byte 0xc7b9
	.2byte 0x0200
	push	{r5, lr}
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #40]
	ldr	r5, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	ldr	r0, [r3, #0]
	bl 0x0200c794
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #48]
	strh	r0, [r5, #0]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #48]
	movs	r1, #144
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #20]
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #40]
	bl 0x0200cb20
	b.n	.L_02004790
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0200e89c
	.4byte 0x0200e898
	.4byte 0x0200e88c
	.4byte 0x0200e888
	.4byte 0x0200e884
	.4byte 0x0200e87c
	.2byte 0xc7b9
	.2byte 0x0200
.L_02004790:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r1, [pc, #20]
	ldrh	r3, [r0, #0]
	movs	r2, #0
	cmp	r3, r1
	beq.n	.L_020047b0
.L_020047a0:
	adds	r0, #2
	ldrh	r3, [r0, #0]
	adds	r2, #1
	cmp	r3, r1
	bne.n	.L_020047a0
	b.n	.L_020047b0
	.2byte 0xffff
	.2byte 0x0000
.L_020047b0:
	subs	r2, #1
	adds	r0, r2, #0
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r1, [pc, #172]
	movs	r4, #0
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_020047f2
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r2, r0
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_020047f2
	ldr	r0, [pc, #148]
	movs	r4, #1
	ldrh	r2, [r0, #0]
	strh	r2, [r1, #0]
	movs	r1, #128
	lsls	r3, r2, #16
	lsls	r1, r1, #10
	cmp	r3, r1
	bls.n	.L_020047f2
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r2, r1
	strh	r3, [r0, #0]
.L_020047f2:
	cmp	r4, #0
	bne.n	.L_020047f8
	b.n	.L_020048e4
.L_020047f8:
	ldr	r3, [pc, #116]
	ldr	r6, [pc, #120]
	ldr	r1, [r3, #0]
	ldrh	r3, [r6, #0]
	movs	r5, #0
	cmp	r5, r3
	bcs.n	.L_02004846
	ldr	r3, [pc, #112]
	ldr	r2, [pc, #112]
	ldr	r7, [r3, #0]
	mov	lr, r2
	mov	ip, r6
.L_02004810:
	mov	r3, lr
	ldrh	r2, [r3, #0]
	ldrh	r3, [r6, #0]
	movs	r0, #160
	muls	r3, r2
	adds	r3, r3, r5
	lsls	r3, r3, #1
	ldrh	r3, [r3, r7]
	lsls	r0, r0, #19
	lsls	r3, r3, #1
	adds	r4, r3, r0
	ldrh	r0, [r1, #0]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	adds	r1, #2
	ldrh	r3, [r1, #0]
	adds	r1, #2
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r4, #0]
	adds	r5, #1
	mov	r2, ip
	ldrh	r3, [r2, #0]
	cmp	r5, r3
	bcc.n	.L_02004810
.L_02004846:
	ldr	r3, [pc, #44]
	movs	r0, #160
	ldrh	r1, [r3, #0]
	ldr	r3, [pc, #48]
	lsls	r2, r1, #1
	ldr	r3, [r3, #0]
	lsls	r0, r0, #19
	ldrh	r3, [r2, r3]
	adds	r2, r2, r1
	lsls	r3, r3, #1
	adds	r4, r3, r0
	ldr	r3, [pc, #36]
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0200488c
	ldr	r3, [pc, #32]
	b.n	.L_0200488e
	.4byte 0x0200e884
	.4byte 0x0200e888
	.4byte 0x0200e894
	.4byte 0x0200e898
	.4byte 0x0200e880
	.4byte 0x0200e89c
	.4byte 0x0200e88c
	.4byte 0x0200e87c
	.2byte 0xe890
	.2byte 0x0200
.L_0200488c:
	ldr	r3, [pc, #68]
.L_0200488e:
	lsls	r2, r2, #1
	ldr	r3, [r3, #0]
	adds	r1, r3, r2
	ldrh	r0, [r1, #0]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	ldrh	r3, [r1, #2]
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r4, #0]
	ldr	r1, [pc, #48]
	ldr	r2, [pc, #32]
	ldrh	r3, [r1, #0]
	eors	r3, r2
	strh	r3, [r1, #0]
	ldr	r1, [pc, #40]
	ldr	r2, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r3, #1
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	ldrh	r2, [r2, #0]
	lsrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_020048e4
	ldr	r3, [pc, #8]
	strh	r3, [r1, #0]
	b.n	.L_020048e4
	.2byte 0x0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0200e894
	.4byte 0x0200e87c
	.4byte 0x0200e89c
	.2byte 0xe898
	.2byte 0x0200
.L_020048e4:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r5, [r6, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	mov	r8, r1
	add	r5, r8
	ldr	r2, [r5, #0]
	ldr	r0, [pc, #100]
	movs	r1, #1
.L_02004904:
	mov	sl, r2
	bl 0x0200cd58
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x0200cd58
	ldr	r2, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r1, r8
	str	r3, [r2, r1]
	bl 0x0200cd68
.L_02004922:
	bl 0x0200cd78
	bl 0x0200c738
	bl 0x0200cdc8
	movs	r0, #40
	bl 0x0200cb18
	bl 0x0200c718
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl 0x0200cd58
	movs	r0, #16
	bl 0x0200cd60
	movs	r0, #16
	bl 0x0200cb18
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #0
	strb	r2, [r3, #0]
	mov	r3, sl
.L_0200495e:
	str	r3, [r5, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.4byte 0x00202108
	.2byte 0x0240
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
	beq.n	.L_020049d4
	adds	r7, r0, #0
.L_02004986:
	ldrh	r3, [r7, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200ccb8
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
	bl 0x0200cc40
	ldr	r4, [sp, #0]
	lsls	r6, r6, #16
	lsls	r4, r4, #16
	lsrs	r4, r4, #16
	lsrs	r6, r6, #16
.L_020049b8:
	adds	r5, #34
	ldrb	r3, [r5, #0]
	adds	r2, r4, #0
	mov	r0, r8
	adds	r1, r6, #0
	adds	r7, #6
	bl 0x0200ca60
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02004986
.L_020049d4:
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
	b.n	.L_02004a48
.L_020049f8:
	ldrh	r3, [r5, #0]
	movs	r1, #26
	ldrsh	r7, [r2, r1]
	cmp	r7, r3
	bne.n	.L_02004a44
	adds	r0, r7, #0
	bl 0x0200ccb8
	adds	r5, #2
	ldrh	r2, [r5, #0]
	mov	r3, sl
	adds	r6, r0, #0
	mov	r8, r2
	ldrh	r5, [r5, #2]
	cmp	r3, #7
	bgt.n	.L_02004a20
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #64]
	adds	r3, r3, r1
	str	r3, [r6, #28]
.L_02004a20:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_02004a52
	adds	r0, r5, #0
	bl 0x0200cb98
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	adds	r0, r7, #0
	mov	r1, r8
	adds	r2, r5, #0
	bl 0x0200ca60
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #28]
	b.n	.L_02004a52
.L_02004a44:
	adds	r5, #6
	movs	r1, #255
.L_02004a48:
	ldrh	r3, [r5, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_020049f8
.L_02004a52:
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
	bl 0x0200ccb8
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
	bl 0x0200cdb8
	lsls	r0, r0, #2
	adds	r5, r5, r0
	mov	r0, r8
	bl 0x0200cb90
	cmp	r0, #0
	beq.n	.L_02004ad4
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #0
	strb	r3, [r5, #2]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	cmp	r6, #1
	beq.n	.L_02004ac0
	cmp	r6, #1
	bcc.n	.L_02004ab6
	cmp	r6, #2
	beq.n	.L_02004aca
	b.n	.L_02004b02
.L_02004ab6:
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200cbc0
	b.n	.L_02004b02
.L_02004ac0:
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x0200cbc0
	b.n	.L_02004b02
.L_02004aca:
	adds	r0, r7, #0
	movs	r1, #6
	bl 0x0200cbc0
	b.n	.L_02004b02
.L_02004ad4:
	movs	r3, #255
	strb	r3, [r5, #2]
	cmp	r6, #1
	beq.n	.L_02004af0
	cmp	r6, #1
	bcc.n	.L_02004ae6
	cmp	r6, #2
	beq.n	.L_02004afa
	b.n	.L_02004b02
.L_02004ae6:
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200cbc0
	b.n	.L_02004b02
.L_02004af0:
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x0200cbc0
	b.n	.L_02004b02
.L_02004afa:
	adds	r0, r7, #0
	movs	r1, #5
	bl 0x0200cbc0
.L_02004b02:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.section .rodata,"a",%progbits
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x02100000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02100000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x03280000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x03280000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02c80000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02c80000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02280000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02280000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02080000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02080000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00380000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000002e
	.4byte 0x02008071
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x00380000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00800000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x00380000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x00070008
	.4byte 0x00090200
	.4byte 0x02010007
	.4byte 0x0009ffff
	.4byte 0x000a0003
	.4byte 0x000b0003
	.4byte 0x000c0003
	.4byte 0x00000003
	.4byte 0x00000008
	.4byte 0x00100000
	.4byte 0x02020007
	.4byte 0x00070011
	.4byte 0xffff0203
	.4byte 0x00030008
	.4byte 0x00090000
	.4byte 0x00000001
	.4byte 0x0007000c
	.4byte 0x000d0204
	.4byte 0x02050007
	.4byte 0x0007000e
	.4byte 0x000f0206
	.4byte 0x02070007
	.4byte 0x00070010
	.4byte 0xffff0208
	.4byte 0x00030009
	.4byte 0x00080000
	.4byte 0x000a0000
	.4byte 0x00000001
	.4byte 0x0043000b
	.4byte 0x000c0000
	.4byte 0x00000001
	.4byte 0x0007000d
	.4byte 0x000e0210
	.4byte 0x02110007
	.4byte 0x0007000f
	.4byte 0x00100212
	.4byte 0x02130007
	.4byte 0x00070011
	.4byte 0x00120214
	.4byte 0x02150007
	.4byte 0x0000ffff
	.4byte 0x0000002e
	.4byte 0x0200b08d
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00200000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00160000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00800000
	.4byte 0x00160000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x0200b1dd
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200b08d
	.4byte 0x00000004
	.4byte 0x00500000
	.4byte 0x00200000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00500000
	.4byte 0x00160000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00600000
	.4byte 0x00160000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte 0x02008c69
	.4byte 0x0000002e
	.4byte 0x0200b1dd
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200b08d
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00200000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x001e0000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x001e0000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x0200b1dd
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200b139
	.4byte 0x00000004
	.4byte 0x00600000
	.4byte 0x00200000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00600000
	.4byte 0x00160000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00160000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte 0x02008a25
	.4byte 0x0000002e
	.4byte 0x0200b1dd
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200b139
	.4byte 0x00000004
	.4byte 0x03580000
	.4byte 0x00200000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00200000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00160000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00160000
	.4byte 0x00980000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte 0x02008c1d
	.4byte 0x0000002e
	.4byte 0x0200b1dd
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200b139
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00200000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00160000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00800000
	.4byte 0x00160000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte 0x02008809
	.4byte 0x0000002e
	.4byte 0x0200b1dd
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xe000c000
	.4byte 0xc000a000
	.4byte 0x20004000
	.4byte 0x40006000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000c000
	.4byte 0xc0008000
	.4byte 0x00004000
	.4byte 0x40008000
	.4byte 0x0000ffff
	.4byte 0xffff8000
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
	.4byte 0x008a0089
	.4byte 0x008c008b
	.4byte 0x008e008d
	.4byte 0xffff008f
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0xffff0016
	.4byte 0x001b001e
	.4byte 0x0016001f
	.4byte 0x001f0015
	.4byte 0x00120014
	.4byte 0x000d001f
	.4byte 0x001b000b
	.4byte 0x0008000b
	.4byte 0x00080018
	.4byte 0x00150005
	.4byte 0x001f001f
	.4byte 0xffff001f
	.4byte 0x0089008e
	.4byte 0x008b008a
	.4byte 0x008d008c
	.4byte 0x008e008d
	.4byte 0x008a0089
	.4byte 0x008c008b
	.4byte 0x008d008c
	.4byte 0x0089008e
	.4byte 0x008b008a
	.4byte 0x008c008b
	.4byte 0x008e008d
	.4byte 0x008a0089
	.4byte 0x008b008a
	.4byte 0x008d008c
	.4byte 0x0089008e
	.4byte 0x008a0089
	.4byte 0x008c008b
	.4byte 0x008e008d
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000117
	.4byte 0x00154002
	.4byte 0x00202119
	.4byte 0x00000118
	.4byte 0x00101119
	.4byte 0x0020211f
	.4byte 0x00304118
	.4byte 0x00403118
	.4byte 0x0050511a
	.4byte 0x0060611a
	.4byte 0x00708118
	.4byte 0x00807118
	.4byte 0x0090a118
	.4byte 0x00a09118
	.4byte 0x01414119
	.4byte 0x00000119
	.4byte 0x00101118
	.4byte 0x00202117
	.4byte 0x0030311a
	.4byte 0x0040411a
	.4byte 0x0050511b
	.4byte 0x00607119
	.4byte 0x00706119
	.4byte 0x00809119
	.4byte 0x00908119
	.4byte 0x00a0b119
	.4byte 0x00b0a119
	.4byte 0x00c0d119
	.4byte 0x00d0c119
	.4byte 0x00e0f119
	.4byte 0x00f0e119
	.4byte 0x0101011a
	.4byte 0x01112119
	.4byte 0x01211119
	.4byte 0x01403118
	.4byte 0x0000011a
	.4byte 0x0010111b
	.4byte 0x0020211b
	.4byte 0x00303119
	.4byte 0x00404119
	.4byte 0x00505118
	.4byte 0x00606118
	.4byte 0x0070711c
	.4byte 0x0080911a
	.4byte 0x0090811a
	.4byte 0x00a0b11a
	.4byte 0x00b0a11a
	.4byte 0x00e0e11b
	.4byte 0x01010119
	.4byte 0x0000011b
	.4byte 0x0010111a
	.4byte 0x0020211a
	.4byte 0x0030411b
	.4byte 0x0040311b
	.4byte 0x00505119
	.4byte 0x0060711b
	.4byte 0x0070611b
	.4byte 0x0080911b
	.4byte 0x0090811b
	.4byte 0x00a0b11b
	.4byte 0x00b0a11b
	.4byte 0x00c0d11b
	.4byte 0x00d0c11b
	.4byte 0x00e0e11a
	.4byte 0x0000011c
	.4byte 0x0010111d
	.4byte 0x0020311c
	.4byte 0x0030211c
	.4byte 0x0040511c
	.4byte 0x0050411c
	.4byte 0x0060611e
	.4byte 0x0070711a
	.4byte 0x0080811e
	.4byte 0x0090911e
	.4byte 0x00a0b11c
	.4byte 0x00b0a11c
	.4byte 0x00c0d11c
	.4byte 0x00d0c11c
	.4byte 0x00e0f11c
	.4byte 0x00f0e11c
	.4byte 0x0100a11e
	.4byte 0x0000011d
	.4byte 0x0010111c
	.4byte 0x0020311d
	.4byte 0x0030211d
	.4byte 0x0040611d
	.4byte 0x0050511e
	.4byte 0x0060411d
	.4byte 0x0070711e
	.4byte 0x00b0b11e
	.4byte 0x0000011e
	.4byte 0x0010211e
	.4byte 0x0020111e
	.4byte 0x0030411e
	.4byte 0x0040311e
	.4byte 0x0050511d
	.4byte 0x0060611c
	.4byte 0x0070711d
	.4byte 0x0080811c
	.4byte 0x000001ff
	.4byte 0x00000027
	.4byte 0x00000007
	.4byte 0x00000011
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00020000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0x032001e9
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0x006900f5
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0000a000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x0200cde8
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x0200ce50
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d91c
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d91c
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0xffff01a2
	.4byte 0x0200d91c
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d91c
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x02c00000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
	.4byte 0x00000007
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
	.4byte 0xffff019e
	.4byte 0x0200ceb8
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02100000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d91c
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00024000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00020000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00028000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x03a80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x01024000
	.4byte 0x005400f4
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00004000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
	.4byte 0x00000007
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
	.4byte 0xffff01a2
	.4byte 0x0200d91c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d91c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0xffff01a2
	.4byte 0x0200d91c
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d91c
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
	.4byte 0x00000007
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
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x0200cf44
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d91c
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x0200cff0
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d91c
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x0200dae8
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
	.4byte 0x00000007
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
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
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
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000051
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000051
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte 0x02008045
	.4byte 0x0000c400
	.4byte 0xffff000b
	.4byte 0x02008045
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte 0x0200840d
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte 0x0200840d
	.4byte 0x00009815
	.4byte 0x0a3e000a
	.4byte 0x0200851d
	.4byte 0x00000000
	.4byte 0x0a3e000a
	.4byte 0x02008565
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000051
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000051
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000051
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000051
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000031
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000002
	.4byte 0xffff0058
	.4byte 0x0200a63d
	.4byte 0x50008615
	.4byte 0x02020010
	.4byte 0x0200840d
	.4byte 0x50008615
	.4byte 0x02030011
	.4byte 0x0200840d
	.4byte 0x00009c05
	.4byte 0xffff0011
	.4byte 0x02008649
	.4byte 0x00009c05
	.4byte 0xffff0012
	.4byte 0x02008591
	.4byte 0x00008c15
	.4byte 0x0a4f0008
	.4byte 0x020088f5
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x020088f5
	.4byte 0x50009705
	.4byte 0x12300050
	.4byte 0x02008679
	.4byte 0x50009705
	.4byte 0x12310051
	.4byte 0x0200885d
	.4byte 0x10009a15
	.4byte 0xffff000f
	.4byte 0x00000000
	.4byte 0x60009a15
	.4byte 0xffff000d
	.4byte 0x02008711
	.4byte 0x20009a15
	.4byte 0xffff000d
	.4byte 0x02008719
	.4byte 0x00000000
	.4byte 0xffff0040
	.4byte 0x02008931
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
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000051
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000051
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000021
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x0200b2b9
	.4byte 0x00000002
	.4byte 0xffff005a
	.4byte 0x0200a861
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008051
	.4byte 0x50008c15
	.4byte 0xffff000a
	.4byte 0x0200896d
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x50008615
	.4byte 0x0204000c
	.4byte 0x0200840d
	.4byte 0x50008615
	.4byte 0x0205000d
	.4byte 0x0200840d
	.4byte 0x50008615
	.4byte 0x0206000e
	.4byte 0x0200840d
	.4byte 0x50008615
	.4byte 0x0207000f
	.4byte 0x0200840d
	.4byte 0x50008615
	.4byte 0x02080010
	.4byte 0x0200840d
	.4byte 0x00009c05
	.4byte 0xffff000e
	.4byte 0x0200893d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000051
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000051
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000051
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000051
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000051
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x0200b2b9
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x60009a15
	.4byte 0xffff0009
	.4byte 0x020089c1
	.4byte 0x20009a15
	.4byte 0xffff0009
	.4byte 0x020089e9
	.4byte 0x10009a15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x60009a15
	.4byte 0xffff000b
	.4byte 0x02008a75
	.4byte 0x20009a15
	.4byte 0xffff000b
	.4byte 0x02008a7d
	.4byte 0x50009705
	.4byte 0x12320050
	.4byte 0x02008b85
	.4byte 0x00009c05
	.4byte 0xffff000e
	.4byte 0x02008af5
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008b25
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008b55
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000051
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000051
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000051
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x02008e3d
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x02008e3d
	.4byte 0x00000002
	.4byte 0x02340023
	.4byte 0x02008fa5
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x02008fd5
	.4byte 0x00000002
	.4byte 0xffff005a
	.4byte 0x0200a861
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x0200a9a1
	.4byte 0x50008615
	.4byte 0x0210000d
	.4byte 0x0200840d
	.4byte 0x50008615
	.4byte 0x0211000e
	.4byte 0x0200840d
	.4byte 0x50008615
	.4byte 0x0212000f
	.4byte 0x0200840d
	.4byte 0x50008615
	.4byte 0x02130010
	.4byte 0x0200840d
	.4byte 0x50008615
	.4byte 0x02140011
	.4byte 0x0200840d
	.4byte 0x50008615
	.4byte 0x02150012
	.4byte 0x0200840d
	.4byte 0x00009c05
	.4byte 0xffff000c
	.4byte 0x02008c85
	.4byte 0x00009c05
	.4byte 0xffff000d
	.4byte 0x02008cb5
	.4byte 0x00008c15
	.4byte 0x0a4e0008
	.4byte 0x02008ce5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000051
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000051
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x0200b2b9
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x02009139
	.4byte 0x00000202
	.4byte 0xffff0023
	.4byte 0x0200912d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0200d7b8
	.4byte 0x0200d7f4
	.4byte 0x0200d830
