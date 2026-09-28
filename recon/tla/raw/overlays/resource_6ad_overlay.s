.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe430
	.2byte 0x0200
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs	r0, #0
	bx	lr
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe460
	.2byte 0x0200
	push	{lr}
	movs	r1, #0
	bl 0x0200d500
	movs	r0, #0
	pop	{pc}
	.global Func_02000058
	.thumb_func
Func_02000058:
	push	{lr}
	ldr	r3, [pc, #56]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000070
	ldr	r0, [pc, #44]
	b.n	.L_02000090
.L_02000070:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_0200007a
	ldr	r0, [pc, #44]
	b.n	.L_02000090
.L_0200007a:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02000084
	ldr	r0, [pc, #40]
	b.n	.L_02000090
.L_02000084:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_0200008e
	ldr	r0, [pc, #40]
	b.n	.L_02000090
.L_0200008e:
	ldr	r0, [pc, #40]
.L_02000090:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000127
	.4byte 0x0200e530
	.4byte 0x00000125
	.4byte 0x0200ea0c
	.4byte 0x00000124
	.4byte 0x0200e710
	.4byte 0x00000126
	.4byte 0x0200e890
	.4byte 0x0200e518
	.4byte 0x049b23c0
	.4byte 0x22d06edb
	.4byte 0x32380112
	.4byte 0x2201189b
	.2byte 0x701a
	.2byte 0x4770
	push	{r5, r6, lr}
	ldr	r3, [pc, #144]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d570
	ldrh	r2, [r0, #6]
	cmp	r2, #0
	beq.n	.L_020000ee
	movs	r3, #128
	lsls	r3, r3, #8
	cmp	r2, r3
	bne.n	.L_02000148
.L_020000ee:
	ldr	r3, [r0, #8]
	asrs	r5, r3, #20
	ldr	r3, [r0, #16]
	asrs	r6, r3, #20
	cmp	r2, #0
	beq.n	.L_020000fe
	subs	r5, #1
	b.n	.L_02000100
.L_020000fe:
	adds	r5, #1
.L_02000100:
	movs	r0, #10
	bl 0x0200d570
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, r5
	bne.n	.L_02000116
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, r6
	beq.n	.L_02000148
.L_02000116:
	movs	r0, #11
	bl 0x0200d570
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, r5
	bne.n	.L_0200012c
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, r6
	beq.n	.L_02000148
.L_0200012c:
	movs	r0, #12
	bl 0x0200d570
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, r5
	bne.n	.L_02000142
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, r6
	beq.n	.L_02000148
.L_02000142:
	bl 0x0200d600
	b.n	.L_02000162
.L_02000148:
	ldr	r3, [pc, #24]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d5f8
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_02000162
	bl 0x0200d688
.L_02000162:
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200d600
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #120]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200d570
	ldrh	r3, [r0, #6]
	movs	r2, #12
	lsrs	r6, r3, #12
	adds	r3, r6, #2
	ands	r3, r2
	lsls	r6, r3, #12
	ldr	r3, [r0, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r2, #128
	ldr	r3, [r0, #12]
	lsls	r2, r2, #11
	adds	r3, r3, r2
	ldr	r2, [pc, #80]
	adds	r1, r6, #0
	ands	r3, r2
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r0, #16]
	movs	r0, #128
	lsls	r0, r0, #13
	str	r3, [r5, #8]
	bl 0x0200d408
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200d680
	cmp	r0, #0
	beq.n	.L_020001e2
	ldr	r3, [r0, #8]
	adds	r1, r6, #0
	str	r3, [r5, #0]
	adds	r2, r5, #0
	ldr	r3, [r0, #12]
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	movs	r0, #128
	lsls	r0, r0, #13
	str	r3, [r5, #8]
	bl 0x0200d408
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200d680
	cmp	r0, #0
	bne.n	.L_020001e6
.L_020001e2:
	bl 0x0200d688
.L_020001e6:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb520
	adds	r0, r1, #0
	bl 0x0200d570
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #50
	bne.n	.L_02000218
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #56
	bl 0x0200d458
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
.L_02000218:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #16
	bl 0x0200d570
	adds	r5, r0, #0
	bl 0x0200d688
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #50
	bne.n	.L_02000244
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #56
	bl 0x0200d458
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
.L_02000244:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	cmp	r1, #10
	bne.n	.L_0200025a
	movs	r0, #10
	bl 0x0200d570
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
.L_0200025a:
	pop	{pc}
	push	{lr}
	cmp	r1, #16
	bne.n	.L_0200026a
	movs	r1, #16
	bl 0x020081f4
	b.n	.L_0200027e
.L_0200026a:
	cmp	r1, #10
	bne.n	.L_0200027e
	movs	r0, #10
	bl 0x0200d570
	movs	r3, #3
	adds	r0, #85
	strb	r3, [r0, #0]
	bl 0x020082ec
.L_0200027e:
	pop	{pc}
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	bl 0x0200d570
	ldr	r3, [pc, #36]
	lsls	r5, r5, #1
	subs	r5, #36
	ldrsh	r4, [r3, r5]
	adds	r3, r0, #0
	adds	r3, #100
	adds	r0, #102
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [pc, #16]
	lsls	r2, r2, #16
	lsls	r1, r1, #16
	adds	r2, r2, r3
	adds	r0, r4, #0
	bl 0x0200d598
	pop	{r5, pc}
	.4byte 0x0200db5c
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb520
	adds	r5, r1, #0
	adds	r0, r5, #0
	bl 0x0200d570
	ldr	r3, [pc, #36]
	lsls	r5, r5, #1
	ldr	r2, [r0, #8]
	subs	r5, #36
	adds	r0, #100
	ldrsh	r1, [r3, r5]
	ldrh	r3, [r0, #0]
	asrs	r2, r2, #20
	lsls	r3, r3, #16
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_020002e4
	adds	r0, r1, #0
	movs	r2, #0
	movs	r1, #0
	bl 0x0200d598
.L_020002e4:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xdb5c
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #38
	str	r3, [sp, #0]
	movs	r5, #72
	movs	r0, #53
	movs	r1, #72
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200d4e8
	movs	r3, #43
	str	r3, [sp, #0]
	movs	r0, #53
	movs	r1, #72
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200d4e8
	movs	r3, #47
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #53
	movs	r1, #8
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d4e8
	movs	r5, #10
.L_0200032c:
	adds	r0, r5, #0
	bl 0x0200d570
	ldr	r3, [r0, #8]
	asrs	r2, r3, #20
	ldr	r3, [r0, #16]
	asrs	r0, r3, #20
	cmp	r2, #38
	bne.n	.L_02000354
	cmp	r0, #8
	bne.n	.L_02000354
	movs	r3, #72
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #54
	movs	r1, #72
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d4e8
.L_02000354:
	adds	r5, #1
	cmp	r5, #12
	ble.n	.L_0200032c
	movs	r5, #10
.L_0200035c:
	adds	r0, r5, #0
	bl 0x0200d570
	ldr	r3, [r0, #8]
	asrs	r2, r3, #20
	ldr	r3, [r0, #16]
	asrs	r0, r3, #20
	cmp	r2, #43
	bne.n	.L_02000384
	cmp	r0, #8
	bne.n	.L_02000384
	movs	r3, #72
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #54
	movs	r1, #72
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d4e8
.L_02000384:
	adds	r5, #1
	cmp	r5, #12
	ble.n	.L_0200035c
	movs	r0, #10
	bl 0x0200d570
	ldr	r3, [r0, #8]
	asrs	r2, r3, #20
	ldr	r3, [r0, #16]
	asrs	r0, r3, #20
	cmp	r2, #47
	bne.n	.L_020003b0
	cmp	r0, #8
	bne.n	.L_020003b0
	str	r2, [sp, #0]
	str	r0, [sp, #4]
	movs	r1, #8
	movs	r0, #54
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d4e8
.L_020003b0:
	add	sp, #8
	pop	{r5, pc}
	push	{lr}
	movs	r0, #15
	bl 0x0200d570
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #14
	bne.n	.L_020003d6
	movs	r1, #65
	movs	r0, #0
	bl 0x0200a7ac
	movs	r0, #2
	movs	r1, #19
	bl 0x0200a7ac
	b.n	.L_020003e6
.L_020003d6:
	movs	r1, #17
	movs	r0, #0
	bl 0x0200a7ac
	movs	r0, #2
	movs	r1, #67
	bl 0x0200a7ac
.L_020003e6:
	movs	r0, #16
	bl 0x0200d570
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #6
	bne.n	.L_02000406
	movs	r1, #65
	movs	r0, #1
	bl 0x0200a7ac
	movs	r0, #3
	movs	r1, #3
	bl 0x0200a7ac
	b.n	.L_02000416
.L_02000406:
	movs	r1, #49
	movs	r0, #1
	bl 0x0200a7ac
	movs	r0, #3
	movs	r1, #67
	bl 0x0200a7ac
.L_02000416:
	pop	{pc}
	push	{r5, lr}
	sub	sp, #32
	add	r5, sp, #8
	adds	r0, r5, #0
	.2byte 0xf004
	.2byte 0xfb34
	cmp	r0, #0
	beq.n	.L_02000440
	mov	r2, sp
	add	r3, sp, #24
	ldmia	r3!, {r0, r1}
	stmia	r2!, {r0, r1}
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	ldr	r2, [r5, #8]
	ldr	r3, [r5, #12]
	.2byte 0xf004
	.2byte 0xfc6a
	bl 0x020083b4
.L_02000440:
	add	sp, #32
	pop	{r5, pc}
	push	{r5, lr}
	sub	sp, #8
	cmp	r0, #1
	bne.n	.L_0200047c
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r5, #50
	movs	r0, #29
	movs	r1, #56
	movs	r2, #3
	movs	r3, #4
.L_0200045a:
	str	r5, [sp, #4]
	bl 0x0200d4e0
	movs	r3, #74
	str	r3, [sp, #0]
	movs	r0, #93
	movs	r1, #56
	movs	r2, #3
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200d4f0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #68
	bl 0x0200d458
.L_0200047c:
	add	sp, #8
	pop	{r5, pc}
	push	{r5, lr}
	sub	sp, #8
	cmp	r0, #1
	bne.n	.L_020004b8
	movs	r3, #18
	str	r3, [sp, #0]
	movs	r5, #50
	movs	r0, #29
	movs	r1, #56
	movs	r2, #3
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200d4e0
	movs	r3, #82
	str	r3, [sp, #0]
	movs	r0, #93
	movs	r1, #56
	movs	r2, #3
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200d4f0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #69
	bl 0x0200d458
.L_020004b8:
	add	sp, #8
	pop	{r5, pc}
	push	{r5, lr}
	sub	sp, #8
	cmp	r0, #1
	bne.n	.L_020004f4
	movs	r3, #14
	str	r3, [sp, #0]
	movs	r5, #50
	movs	r0, #29
	movs	r1, #56
	movs	r2, #3
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200d4e0
	movs	r3, #78
	str	r3, [sp, #0]
	movs	r0, #93
	movs	r1, #56
	movs	r2, #3
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200d4f0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #70
	bl 0x0200d458
.L_020004f4:
	add	sp, #8
	pop	{r5, pc}
	push	{lr}
	movs	r0, #146
	lsls	r0, r0, #2
	bl 0x0200d6c0
	movs	r1, #3
	movs	r0, #0
	bl 0x0200a7ac
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200d458
	pop	{pc}
	push	{r5, lr}
	movs	r0, #9
	bl 0x0200d570
	movs	r2, #0
	adds	r5, r0, #0
	movs	r1, #0
	movs	r0, #10
	bl 0x0200d598
	adds	r3, r5, #0
	adds	r3, #100
	adds	r5, #102
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	movs	r0, #9
	bl 0x0200d598
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #66
	bl 0x0200d458
	movs	r0, #136
	bl 0x0200d6c0
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	bl 0x02009448
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
.L_02000568:
	push	{r7}
	bl 0x0200d570
	movs	r1, #11
	mov	r8, r0
	mov	fp, r1
.L_02000574:
	bl 0x0200d3e8
	adds	r5, r0, #0
	bl 0x0200d3e8
	mov	r2, r8
	ldr	r6, [r2, #8]
	lsls	r5, r5, #4
	adds	r6, r6, r5
	lsls	r0, r0, #4
	subs	r6, r6, r0
	bl 0x0200d3e8
	adds	r5, r0, #0
	bl 0x0200d3e8
	mov	r1, r8
	ldr	r2, [r1, #12]
	lsls	r5, r5, #3
	adds	r5, r5, r2
	ldr	r2, [r1, #16]
	adds	r3, r0, #0
	lsls	r3, r3, #4
	movs	r0, #74
	adds	r3, r3, r2
	adds	r1, r6, #0
	adds	r0, #255
	adds	r2, r5, #0
	bl 0x0200d498
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0200064c
	bl 0x0200d3e8
	movs	r2, #224
	lsls	r2, r2, #8
	lsrs	r0, r0, #2
	adds	r7, r0, r2
	bl 0x0200d3e8
	lsls	r0, r0, #3
	mov	sl, r0
	bl 0x0200d3e8
	movs	r3, #128
	lsrs	r0, r0, #1
	lsls	r3, r3, #7
	adds	r3, r3, r0
	mov	r9, r3
	bl 0x0200d3e8
	movs	r1, #254
	lsls	r1, r1, #7
	adds	r1, #255
	cmp	r0, r1
	bhi.n	.L_020005f0
	adds	r0, r6, #0
	ldr	r1, [pc, #120]
	bl 0x0200d490
	b.n	.L_020005f8
.L_020005f0:
	adds	r0, r6, #0
	ldr	r1, [pc, #116]
	bl 0x0200d490
.L_020005f8:
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200d500
	movs	r2, #128
	lsls	r2, r2, #10
	lsls	r3, r7, #2
	adds	r3, r3, r2
	str	r3, [r6, #40]
	adds	r0, r7, #0
	bl 0x0200d400
	ldr	r5, [pc, #88]
	adds	r1, r0, #0
	mov	r0, sl
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x62f0
	adds	r0, r7, #0
	bl 0x0200d3f8
	adds	r1, r0, #0
	mov	r0, sl
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [r6, #52]
	mov	r3, r9
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	str	r3, [r6, #72]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r0, [r6, #36]
	str	r3, [r6, #68]
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200d520
.L_0200064c:
	movs	r1, #1
	negs	r1, r1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #0
	bge.n	.L_02000574
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200db68
	.4byte 0x0200dbac
	.2byte 0x021c
	.2byte 0x0300
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
	bl 0x0200d460
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200d460
	bl 0x0200d560
	movs	r0, #0
	.2byte 0xf004
	.2byte 0xffd6
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200d5d0
	movs	r0, #248
	movs	r1, #1
	movs	r2, #178
	lsls	r2, r2, #18
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200d5d8
	bl 0x0200d5e0
	movs	r0, #20
	bl 0x0200d3d8
	movs	r0, #160
	movs	r1, #160
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200d5d0
	movs	r0, #248
	movs	r1, #1
	movs	r2, #218
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200d5d8
	bl 0x0200d5e0
	movs	r0, #120
	bl 0x0200d558
	bl 0x0200d568
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #70
	sub	sp, #8
	bl 0x0200d450
	cmp	r0, #0
	bne.n	.L_02000750
	movs	r3, #14
	movs	r5, #50
	str	r3, [sp, #0]
	movs	r0, #29
	movs	r1, #60
	movs	r2, #3
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200d4e0
	movs	r3, #78
	str	r3, [sp, #0]
	movs	r0, #93
	movs	r1, #60
	movs	r2, #3
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200d4f0
	cmp	r6, #0
	beq.n	.L_02000746
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #67
	bl 0x0200d450
	cmp	r0, #0
	bne.n	.L_02000746
	movs	r0, #14
	bl 0x0200855c
.L_02000746:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #67
	bl 0x0200d458
.L_02000750:
	add	sp, #8
	pop	{r5, r6, pc}
	push	{lr}
	movs	r0, #2
	.2byte 0xf004
	.2byte 0xff4a
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #3
	.2byte 0xf004
	.2byte 0xff44
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #4
	.2byte 0xf004
	.2byte 0xff3e
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #5
	.2byte 0xf004
	.2byte 0xff38
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #6
	.2byte 0xf004
	.2byte 0xff32
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #7
	.2byte 0xf004
	.2byte 0xff2c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #8
	.2byte 0xf004
	.2byte 0xff26
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #9
	.2byte 0xf004
	.2byte 0xff20
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #10
	.2byte 0xf004
	.2byte 0xff1a
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #11
	.2byte 0xf004
	.2byte 0xff14
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #12
	.2byte 0xf004
	.2byte 0xff0e
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #13
	.2byte 0xf004
	.2byte 0xff08
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #14
	.2byte 0xf004
	.2byte 0xff02
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
.L_020007f2:
	adds	r7, r1, #0
	adds	r0, r7, #0
	sub	sp, #8
	.2byte 0xf004
	.2byte 0xfeba
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #13
	bne.n	.L_0200089a
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	.2byte 0xf004
	.2byte 0xfe56
	ldr	r3, [r5, #20]
	cmp	r3, r0
	beq.n	.L_0200084c
	movs	r0, #2
	bl 0x0200d3d8
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	movs	r6, #0
	cmp	r2, r3
	ble.n	.L_02000846
.L_02000832:
	movs	r0, #1
	adds	r6, #1
	bl 0x0200d3d8
	cmp	r6, #29
	bgt.n	.L_02000846
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	cmp	r2, r3
	bgt.n	.L_02000832
.L_02000846:
	movs	r0, #133
	.2byte 0xf004
	.2byte 0xff3a
.L_0200084c:
	movs	r3, #5
	str	r3, [sp, #4]
	movs	r5, #71
	movs	r0, #71
	movs	r1, #24
	movs	r2, #7
	movs	r3, #1
	str	r5, [sp, #0]
	.2byte 0xf004
	.2byte 0xfe48
	movs	r6, #7
	movs	r0, #7
	movs	r1, #24
	movs	r2, #7
	movs	r3, #1
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	.2byte 0xf004
	.2byte 0xfe3b
	movs	r1, #88
	movs	r2, #7
	movs	r3, #1
	movs	r0, #7
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	.2byte 0xf004
	.2byte 0xfe33
	movs	r0, #167
	.2byte 0xf004
	.2byte 0xff1c
	adds	r0, r7, #0
	movs	r1, #0
	movs	r2, #0
	.2byte 0xf004
	.2byte 0xfe83
	movs	r0, #214
	lsls	r0, r0, #2
	.2byte 0xf004
	.2byte 0xfddf
.L_0200089a:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	adds	r0, r1, #0
	.2byte 0xf004
	.2byte 0xfe64
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #7
	bne.n	.L_020008ba
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #55
	.2byte 0xf004
	.2byte 0xfdcf
.L_020008ba:
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #17
	movs	r2, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #17
	movs	r1, #24
	movs	r2, #3
	movs	r3, #1
	.2byte 0xf004
	.2byte 0xfe06
	movs	r3, #18
	movs	r2, #77
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #20
	movs	r1, #24
	movs	r2, #1
	movs	r3, #1
	.2byte 0xf004
	.2byte 0xfe00
	movs	r3, #54
	movs	r2, #20
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #62
	movs	r1, #20
	movs	r2, #1
	movs	r3, #2
	.2byte 0xf004
	.2byte 0xfdf2
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #17
	movs	r2, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #17
	movs	r1, #25
	movs	r2, #3
	movs	r3, #1
	.2byte 0xf004
	.2byte 0xfde4
	movs	r3, #18
	movs	r2, #77
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #20
	movs	r1, #25
	movs	r2, #1
	movs	r3, #1
	.2byte 0xf004
	.2byte 0xfdde
	movs	r3, #54
	movs	r2, #20
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #62
	movs	r1, #22
	movs	r2, #1
	movs	r3, #2
	.2byte 0xf004
	.2byte 0xfdd0
	add	sp, #8
	pop	{pc}
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #148]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	.2byte 0xf004
	.2byte 0xfe0e
	adds	r6, r0, #0
	.2byte 0xf004
	.2byte 0xfe57
	adds	r0, r6, #0
	movs	r1, #0
	.2byte 0xf004
	.2byte 0xfdcf
	ldr	r0, [r5, #0]
	movs	r1, #28
	.2byte 0xf004
	.2byte 0xfe1b
	movs	r0, #16
	.2byte 0xf004
	.2byte 0xfdf4
	movs	r5, #0
.L_02000972:
	cmp	r5, #5
	bne.n	.L_0200097c
	movs	r0, #204
	bl 0x0200d6c0
.L_0200097c:
	adds	r3, r6, #0
	adds	r3, #85
	movs	r7, #0
	strb	r7, [r3, #0]
	ldr	r2, [pc, #88]
	ldr	r3, [r6, #24]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [pc, #84]
	ldr	r3, [r6, #28]
	adds	r5, #1
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r2, [pc, #76]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	.2byte 0xf004
	.2byte 0xfd1a
	cmp	r5, #39
	ble.n	.L_02000972
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d570
	adds	r0, #84
	strb	r7, [r0, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200d620
	bl 0x0200d628
	movs	r0, #16
	bl 0x0200d5f0
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfffffc00
	.4byte 0xfffffd00
	.2byte 0x6667
	.2byte 0xffff
	.2byte 0xb500
	movs	r1, #65
	movs	r0, #2
	bl 0x0200a7ac
	movs	r1, #65
	movs	r0, #1
	bl 0x0200a7ac
	movs	r1, #65
	movs	r0, #0
	bl 0x0200a7ac
	movs	r0, #212
	lsls	r0, r0, #2
	.2byte 0xf004
	.2byte 0xfd29
	movs	r0, #10
	bl 0x0200d570
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #15
	beq.n	.L_02000a34
	movs	r0, #9
	bl 0x0200d570
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #15
	beq.n	.L_02000a34
	movs	r0, #1
	movs	r1, #33
	bl 0x0200a7ac
	b.n	.L_02000aa8
.L_02000a34:
	movs	r0, #9
	bl 0x0200d570
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #13
	beq.n	.L_02000a4c
	movs	r0, #0
	movs	r1, #17
	bl 0x0200a7ac
	b.n	.L_02000aa8
.L_02000a4c:
	movs	r0, #8
	bl 0x0200d570
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #37
	beq.n	.L_02000a64
	movs	r0, #2
	movs	r1, #49
	bl 0x0200a7ac
	b.n	.L_02000aa8
.L_02000a64:
	movs	r0, #212
	lsls	r0, r0, #2
	.2byte 0xf004
	.2byte 0xfcf6
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #81
	.2byte 0xf004
	.2byte 0xfced
	cmp	r0, #0
	bne.n	.L_02000aa8
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200d5d0
	movs	r0, #154
	movs	r1, #1
	movs	r2, #192
	negs	r1, r1
	lsls	r2, r2, #13
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200d5d8
	movs	r0, #60
	bl 0x0200d558
	.2byte 0xf004
	.2byte 0xfddf
	movs	r0, #9
	bl 0x0200d5f0
.L_02000aa8:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r1, #3
	movs	r0, #3
	bl 0x0200a7ac
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200d5d0
	movs	r0, #184
	movs	r1, #1
	movs	r2, #140
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #1
	bl 0x0200d5d8
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	movs	r0, #131
	lsls	r3, r3, #18
	lsls	r0, r0, #1
	ldr	r5, [r3, #108]
	.2byte 0xf004
	.2byte 0xfcb8
	bl 0x0200d560
	movs	r0, #0
	bl 0x0200d648
	movs	r3, #179
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r0, #131
	movs	r3, #0
	strh	r3, [r2, #0]
	lsls	r0, r0, #1
	.2byte 0xf004
	.2byte 0xfcae
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200d460
	movs	r0, #60
	bl 0x0200d558
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200d5d0
	movs	r0, #184
	movs	r1, #1
	movs	r2, #186
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #1
	lsls	r0, r0, #16
	.2byte 0xf004
	.2byte 0xfd54
	.2byte 0xf004
	.2byte 0xfd56
	movs	r0, #60
	bl 0x0200d558
	bl 0x0200d568
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	sub	sp, #44
	add	r5, sp, #20
	adds	r0, r5, #0
	bl 0x0200ca8c
	cmp	r0, #0
	beq.n	.L_02000be6
	mov	r3, sp
	add	r2, sp, #36
	ldmia	r2!, {r0, r1}
	stmia	r3!, {r0, r1}
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	ldr	r2, [r5, #8]
	ldr	r3, [r5, #12]
	bl 0x0200cd10
	bl 0x020089ec
	movs	r0, #0
	bl 0x0200a7dc
	cmp	r0, #64
	beq.n	.L_02000ba8
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02000ba8
	movs	r0, #12
	bl 0x0200d570
	movs	r3, #0
	str	r3, [r0, #108]
	movs	r1, #0
	bl 0x0200d520
	movs	r0, #12
	movs	r1, #6
	bl 0x0200d5a0
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d460
	movs	r0, #12
	bl 0x02008cb8
.L_02000ba8:
	movs	r0, #2
	bl 0x0200a7dc
	cmp	r0, #64
	beq.n	.L_02000c68
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02000c68
	movs	r0, #11
	bl 0x0200d570
	movs	r3, #0
	str	r3, [r0, #108]
	movs	r1, #0
	bl 0x0200d520
	movs	r0, #11
	movs	r1, #6
	.2byte 0xf004
	.2byte 0xfce5
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200d460
	movs	r0, #11
	bl 0x02008cb8
	b.n	.L_02000c68
.L_02000be6:
	ldr	r3, [pc, #132]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	.2byte 0xf004
	.2byte 0xfcbe
	ldr	r3, [pc, #120]
	ldr	r1, [pc, #124]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r6, [r1, r3]
	movs	r1, #1
	negs	r1, r1
	cmp	r6, r1
	beq.n	.L_02000c68
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r5, #0]
	ldr	r7, [r3, #32]
	.2byte 0xf004
	.2byte 0xfcac
	ldr	r1, [pc, #92]
	ldr	r3, [r0, #8]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	add	r5, sp, #8
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r0, #12]
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	movs	r0, #128
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [r5, #8]
	lsls	r0, r0, #13
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl 0x0200d408
	ldr	r3, [r5, #0]
	ldr	r2, [pc, #56]
	asrs	r0, r3, #20
	ldr	r3, [r5, #8]
	asrs	r1, r3, #20
	cmp	r7, #0
	beq.n	.L_02000c56
	movs	r2, #212
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r2, [r3, #0]
.L_02000c56:
	lsls	r3, r1, #7
	adds	r3, r0, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r3, [r2, #2]
	cmp	r3, #255
	beq.n	.L_02000c68
	bl 0x0200d600
.L_02000c68:
	add	sp, #44
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x03001150
	.4byte 0x0200df18
	.4byte 0xfff00000
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200d570
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r2, [r0, #16]
	ldr	r3, [r0, #8]
	movs	r0, #212
	lsls	r0, r0, #1
	adds	r1, r1, r0
	asrs	r2, r2, #20
	lsls	r2, r2, #7
	ldr	r1, [r1, #0]
	asrs	r3, r3, #20
.L_02000ca0:
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r1, r1, r3
	movs	r3, #255
	strb	r3, [r1, #2]
	adds	r0, r5, #0
	bl 0x0200d570
	movs	r3, #0
	adds	r0, #35
	strb	r3, [r0, #0]
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200d570
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r2, [r0, #16]
	ldr	r3, [r0, #8]
	movs	r0, #212
	lsls	r0, r0, #1
	adds	r1, r1, r0
	asrs	r2, r2, #20
	lsls	r2, r2, #7
	ldr	r1, [r1, #0]
	asrs	r3, r3, #20
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r1, r1, r3
	movs	r3, #0
	strb	r3, [r1, #2]
	adds	r0, r5, #0
	bl 0x0200d570
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	bl 0x02008c80
	movs	r0, #136
	lsls	r0, r0, #2
	.2byte 0xf004
	.2byte 0xfba9
	movs	r0, #2
	bl 0x0200a7dc
	cmp	r0, #64
	beq.n	.L_02000d36
	adds	r0, r5, #0
	bl 0x0200d570
	movs	r3, #0
	str	r3, [r0, #108]
	movs	r1, #0
	bl 0x0200d520
	adds	r0, r5, #0
	movs	r1, #6
	bl 0x0200d5a0
	movs	r0, #136
	lsls	r0, r0, #2
	.2byte 0xf004
	.2byte 0xfb98
	adds	r0, r5, #0
	bl 0x02008cb8
.L_02000d36:
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	bl 0x02008c80
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d458
	movs	r0, #0
	bl 0x0200a7dc
	cmp	r0, #64
	beq.n	.L_02000d7e
	adds	r0, r5, #0
	bl 0x0200d570
	movs	r3, #0
	str	r3, [r0, #108]
	movs	r1, #0
	bl 0x0200d520
	adds	r0, r5, #0
	movs	r1, #6
	bl 0x0200d5a0
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d460
	adds	r0, r5, #0
	bl 0x02008cb8
.L_02000d7e:
	pop	{r5, pc}
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #164
	lsls	r0, r0, #4
	sub	sp, #8
	bl 0x0200d458
	movs	r3, #38
	str	r3, [sp, #4]
	movs	r5, #10
	mov	r8, r3
	movs	r0, #24
	movs	r1, #38
	movs	r2, #3
	movs	r3, #9
	str	r5, [sp, #0]
	bl 0x0200d4e8
	movs	r3, #36
	str	r3, [sp, #4]
	movs	r6, #74
	movs	r0, #64
	movs	r1, #64
	movs	r2, #3
	movs	r3, #9
	str	r6, [sp, #0]
	bl 0x0200d4f0
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #3
	movs	r3, #9
	str	r5, [sp, #0]
	bl 0x0200d4f0
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #3
	movs	r3, #9
	str	r6, [sp, #0]
	bl 0x0200d4e8
	movs	r3, #102
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #3
	movs	r3, #9
	str	r5, [sp, #0]
	bl 0x0200d4e8
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	push	{lr}
	bl 0x0200c3ec
	bl 0x0200ace0
	pop	{pc}
	.global Func_02000e04
	.thumb_func
Func_02000e04:
	push	{lr}
	ldr	r3, [pc, #56]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000e1c
	ldr	r0, [pc, #44]
	b.n	.L_02000e3c
.L_02000e1c:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02000e26
	ldr	r0, [pc, #44]
	b.n	.L_02000e3c
.L_02000e26:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02000e30
	ldr	r0, [pc, #40]
	b.n	.L_02000e3c
.L_02000e30:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02000e3a
	ldr	r0, [pc, #40]
	b.n	.L_02000e3c
.L_02000e3a:
	ldr	r0, [pc, #40]
.L_02000e3c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000127
	.4byte 0x0200eb98
	.4byte 0x00000125
	.4byte 0x0200ef64
	.4byte 0x00000124
	.4byte 0x0200ed78
	.4byte 0x00000126
	.4byte 0x0200ee38
	.2byte 0xeb8c
	.2byte 0x0200
	push	{lr}
	.2byte 0xf001
	.2byte 0xff47
	cmp	r0, #0
	beq.n	.L_02000e78
	movs	r0, #1
	bl 0x0200c700
.L_02000e78:
	pop	{pc}
	.2byte 0x0000
	.global Func_02000e7c
	.thumb_func
Func_02000e7c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r2, [pc, #876]
	movs	r3, #240
	mov	sl, r2
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	movs	r3, #241
	lsls	r3, r3, #1
	movs	r6, #192
	add	r3, sl
	lsls	r6, r6, #18
	movs	r2, #0
	ldrsh	r7, [r3, r2]
	ldr	r3, [r6, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #137
	adds	r2, #88
	str	r2, [r3, #0]
	lsls	r0, r0, #1
	sub	sp, #8
	mov	r8, r2
	bl 0x0200d458
	bl 0x0200a5b0
	ldr	r3, [pc, #824]
	cmp	r5, r3
	bne.n	.L_02000f8e
	movs	r0, #0
	bl 0x0200d638
	ldr	r3, [r6, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r2, r8
	str	r2, [r3, #0]
	subs	r3, r7, #1
	cmp	r3, #1
	bhi.n	.L_02000f64
	ldr	r0, [pc, #800]
	ldr	r1, [pc, #800]
	ldr	r2, [pc, #804]
	movs	r3, #88
	bl 0x0200a624
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #66
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02000f00
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d598
	b.n	.L_02000f0a
.L_02000f00:
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d598
.L_02000f0a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #67
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02000f1e
	movs	r0, #0
	bl 0x020086f4
.L_02000f1e:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #68
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02000f34
	movs	r0, #1
	movs	r1, #0
	bl 0x02008444
.L_02000f34:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #69
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02000f4a
	movs	r0, #1
	movs	r1, #0
	bl 0x02008480
.L_02000f4a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #70
	bl 0x0200d450
	cmp	r0, #0
	bne.n	.L_02000f5a
	b.n	.L_020013e0
.L_02000f5a:
	movs	r0, #1
	movs	r1, #0
	bl 0x020084bc
	b.n	.L_020013e0
.L_02000f64:
	ldr	r1, [pc, #672]
	movs	r2, #0
	movs	r3, #88
	ldr	r0, [pc, #672]
	bl 0x0200a624
	movs	r0, #15
	bl 0x0200d5b8
	movs	r0, #16
	bl 0x0200d5b8
	movs	r0, #15
	bl 0x0200c938
	movs	r0, #16
	bl 0x0200c938
	bl 0x020083b4
	b.n	.L_020013e0
.L_02000f8e:
	ldr	r3, [pc, #640]
	cmp	r5, r3
	beq.n	.L_02000f96
	b.n	.L_0200113c
.L_02000f96:
	movs	r0, #0
	bl 0x0200d638
	ldr	r3, [r6, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r2, r8
	str	r2, [r3, #0]
	ldr	r1, [pc, #616]
	ldr	r0, [pc, #620]
	ldr	r2, [pc, #620]
	movs	r3, #88
	bl 0x0200a624
	movs	r0, #3
	movs	r1, #1
	.2byte 0xf001
	.2byte 0xfc08
	cmp	r7, #2
	bne.n	.L_02000fc4
	bl 0x0200a604
.L_02000fc4:
	cmp	r7, #9
	bne.n	.L_0200104a
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #81
	bl 0x0200d458
	bl 0x0200d560
	movs	r0, #0
	bl 0x0200d648
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	bl 0x0200d5d8
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d598
	movs	r1, #0
	movs	r2, #0
	movs	r0, #17
	bl 0x0200d598
	bl 0x0200d618
	bl 0x0200d628
	movs	r0, #30
	bl 0x0200d558
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d598
	movs	r1, #132
	movs	r2, #130
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	movs	r0, #17
	bl 0x0200d598
	movs	r0, #136
	bl 0x0200d6c0
	movs	r0, #60
	bl 0x0200d558
	movs	r0, #10
	adds	r0, #255
	bl 0x0200d458
	movs	r0, #3
	bl 0x0200d5f0
	b.n	.L_020013e2
.L_0200104a:
	movs	r0, #212
	lsls	r0, r0, #2
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02001092
	movs	r1, #150
	movs	r2, #144
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200d598
	movs	r1, #136
	movs	r2, #208
	movs	r0, #9
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200d598
	movs	r1, #136
	movs	r2, #240
	movs	r0, #10
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200d598
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200d458
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d458
.L_02001092:
	movs	r0, #8
	bl 0x0200d5b8
	movs	r0, #9
	bl 0x0200d5b8
	movs	r0, #10
	bl 0x0200d5b8
	movs	r0, #8
	bl 0x0200c938
	movs	r0, #9
	bl 0x0200c938
	movs	r0, #10
	bl 0x0200c938
	movs	r0, #11
	bl 0x0200d570
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #2
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #12
	bl 0x0200d570
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_020010e6
	movs	r0, #11
	bl 0x02008c80
.L_020010e6:
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_020010fa
	movs	r0, #12
	bl 0x02008c80
.L_020010fa:
	bl 0x020089ec
	movs	r0, #212
	lsls	r0, r0, #2
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02001116
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d598
	b.n	.L_02001120
.L_02001116:
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d598
.L_02001120:
	movs	r0, #164
	lsls	r0, r0, #4
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02001130
	bl 0x02008d80
.L_02001130:
	movs	r1, #144
	ldr	r0, [pc, #236]
	lsls	r1, r1, #3
	bl 0x0200d3e0
	b.n	.L_020013e0
.L_0200113c:
	ldr	r3, [pc, #228]
	cmp	r5, r3
	beq.n	.L_02001144
	b.n	.L_020012b8
.L_02001144:
	movs	r0, #0
	bl 0x0200d638
	ldr	r3, [r6, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r2, r8
	str	r2, [r3, #0]
	movs	r0, #1
	bl 0x020095a4
	bl 0x0200d284
	movs	r0, #2
	bl 0x0200d3bc
	movs	r0, #11
	movs	r1, #6
	bl 0x0200d38c
	movs	r0, #214
	lsls	r0, r0, #2
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_020011ba
	movs	r3, #5
	str	r3, [sp, #4]
	movs	r5, #71
	movs	r0, #71
	movs	r1, #24
	movs	r2, #7
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200d4f0
	movs	r6, #7
	movs	r0, #7
	movs	r1, #24
	movs	r2, #7
	movs	r3, #1
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200d4e8
	movs	r0, #7
	movs	r1, #88
	movs	r2, #7
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d4e8
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d598
.L_020011ba:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #55
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_020011d6
	movs	r1, #240
	movs	r2, #148
	movs	r0, #12
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x0200d598
.L_020011d6:
	adds	r3, r7, #0
	subs	r3, #14
	cmp	r3, #1
	bhi.n	.L_02001234
	ldr	r1, [pc, #72]
	ldr	r0, [pc, #72]
	ldr	r2, [pc, #76]
	movs	r3, #88
	.2byte 0xf001
	.2byte 0xfa1d
	movs	r0, #0
	movs	r1, #1
	.2byte 0xf001
	.2byte 0xfaed
	b.n	.L_02001240
	.4byte 0x02000240
	.4byte 0x00000124
	.4byte 0x0200dbf0
	.4byte 0x0200dbf6
	.4byte 0x02008515
	.4byte 0x0200dc0e
	.4byte 0x0200dbfc
	.4byte 0x00000125
	.4byte 0x0200dc26
	.4byte 0x0200dc14
	.4byte 0x02009469
	.4byte 0x02008e69
	.4byte 0x00000126
	.4byte 0x0200dc32
	.4byte 0x0200dc2c
	.2byte 0x9491
	.2byte 0x0200
.L_02001234:
	ldr	r0, [pc, #436]
	ldr	r1, [pc, #440]
	movs	r2, #0
	movs	r3, #88
	.2byte 0xf001
	.2byte 0xf9f2
.L_02001240:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_0200124e
	b.n	.L_020013e0
.L_0200124e:
	cmp	r7, #2
	beq.n	.L_0200126e
	cmp	r7, #4
	beq.n	.L_0200126e
	cmp	r7, #6
	beq.n	.L_0200126e
	cmp	r7, #7
	beq.n	.L_0200126e
	cmp	r7, #8
	beq.n	.L_0200126e
	cmp	r7, #10
	beq.n	.L_0200126e
	cmp	r7, #11
	beq.n	.L_0200126e
	cmp	r7, #13
	bne.n	.L_0200127c
.L_0200126e:
	ldr	r3, [pc, #388]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d6b0
.L_0200127c:
	cmp	r7, #3
	beq.n	.L_02001290
	cmp	r7, #5
	beq.n	.L_02001290
	cmp	r7, #9
	beq.n	.L_02001290
	cmp	r7, #12
	beq.n	.L_02001290
	cmp	r7, #14
	bne.n	.L_0200129e
.L_02001290:
	ldr	r3, [pc, #352]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d6b8
.L_0200129e:
	cmp	r7, #16
	beq.n	.L_020012a4
	b.n	.L_020013e0
.L_020012a4:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_020012b2
	b.n	.L_020013e0
.L_020012b2:
	.2byte 0xf002
	.2byte 0xfd65
	b.n	.L_020013e0
.L_020012b8:
	ldr	r3, [pc, #316]
	cmp	r5, r3
	beq.n	.L_020012c0
	b.n	.L_020013e0
.L_020012c0:
	movs	r0, #0
	bl 0x0200d638
	ldr	r3, [r6, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r2, r8
	str	r2, [r3, #0]
	bl 0x0200d670
	movs	r1, #8
	movs	r2, #9
	movs	r0, #0
	bl 0x0200d678
	movs	r0, #8
	bl 0x0200d570
	movs	r5, #1
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #9
	bl 0x0200d570
	subs	r3, r7, #5
	adds	r0, #98
	strb	r5, [r0, #0]
	cmp	r3, #1
	bls.n	.L_02001300
	cmp	r7, #8
	bne.n	.L_02001312
.L_02001300:
	ldr	r1, [pc, #248]
	movs	r2, #0
	movs	r3, #88
	ldr	r0, [pc, #248]
	.2byte 0xf001
	.2byte 0xf98c
	bl 0x020082ec
	b.n	.L_02001332
.L_02001312:
	cmp	r7, #11
	beq.n	.L_02001326
	cmp	r7, #13
	beq.n	.L_02001326
	cmp	r7, #14
	beq.n	.L_02001326
	cmp	r7, #1
	beq.n	.L_02001326
	cmp	r7, #15
	bne.n	.L_02001332
.L_02001326:
	ldr	r0, [pc, #220]
	ldr	r1, [pc, #220]
	movs	r2, #0
	movs	r3, #88
	.2byte 0xf001
	.2byte 0xf979
.L_02001332:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200d450
	cmp	r0, #0
	bne.n	.L_02001362
.L_0200133e:
	cmp	r7, #15
	bne.n	.L_02001350
	movs	r1, #166
	movs	r2, #178
	movs	r0, #23
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200d598
.L_02001350:
	cmp	r7, #11
	bne.n	.L_02001362
	movs	r1, #134
	movs	r2, #138
	movs	r0, #17
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200d598
.L_02001362:
	movs	r0, #16
	bl 0x0200d570
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #56
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_0200139a
	movs	r1, #202
	movs	r2, #156
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r0, #16
	bl 0x0200d598
	movs	r0, #16
	bl 0x0200d570
	str	r5, [r0, #12]
	movs	r0, #16
	bl 0x0200d570
	str	r5, [r0, #20]
.L_0200139a:
	ldr	r3, [pc, #88]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d570
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200d570
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #192
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x0200d570
.L_020013c8:
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #128
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #12
	bl 0x0200d570
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
.L_020013e0:
	movs	r0, #0
.L_020013e2:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0200dc38
	.4byte 0x0200dc46
	.4byte 0x02000240
	.4byte 0x00000127
	.4byte 0x0200dc5a
	.4byte 0x0200dc4c
	.4byte 0x0200dc60
	.2byte 0xdc6e
	.2byte 0x0200
	.global Func_0200140c
	.thumb_func
Func_0200140c:
	push	{r5, lr}
	movs	r0, #163
	lsls	r0, r0, #4
	ldr	r5, [pc, #40]
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02001436
.L_0200141c:
	movs	r2, #253
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #24]
	movs	r1, #160
	subs	r3, r3, r2
	adds	r0, r0, r3
	lsls	r1, r1, #19
	bl 0x0200d548
.L_02001436:
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000121
	.2byte 0x010e
	.2byte 0x0000
	push	{lr}
	ldr	r2, [pc, #24]
	sub	sp, #4
	movs	r3, #128
	str	r2, [sp, #0]
	lsls	r3, r3, #7
	movs	r1, #12
	movs	r2, #13
	movs	r0, #11
	.2byte 0xf001
	.2byte 0xff17
	add	sp, #4
	pop	{pc}
	.2byte 0x0000
	.2byte 0xdc84
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #32]
	sub	sp, #4
	movs	r3, #128
	str	r2, [sp, #0]
	lsls	r3, r3, #7
	movs	r2, #22
	movs	r1, #21
	movs	r0, #19
	bl 0x0200b28c
	movs	r1, #131
	movs	r0, #3
	.2byte 0xf001
	.2byte 0xf993
	add	sp, #4
	pop	{pc}
	.2byte 0x0000
	.2byte 0xdce0
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #24]
	sub	sp, #4
	movs	r3, #128
	str	r2, [sp, #0]
	lsls	r3, r3, #7
	movs	r1, #15
	movs	r2, #16
	movs	r0, #14
	.2byte 0xf001
	.2byte 0xfef3
	add	sp, #4
	pop	{pc}
	.2byte 0x0000
	.2byte 0xdd3c
	.2byte 0x0200
	push	{lr}
	movs	r0, #20
	adds	r0, #255
	bl 0x0200d458
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #37
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_020014d8
	movs	r0, #98
	adds	r0, #255
	bl 0x0200d458
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200d458
.L_020014d8:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x60184b01
	.4byte 0x00004770
	.2byte 0xf040
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #164]
	sub	sp, #32
	ldr	r0, [r3, #0]
	cmp	r0, #0
	bge.n	.L_020014fa
	adds	r0, #3
.L_020014fa:
	asrs	r0, r0, #2
	movs	r1, #5
	bl 0x0200d3d0
	ldr	r3, [pc, #148]
	mov	r8, r0
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0200154e
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r0, #179
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrh	r1, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_0200152e
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r1
	cmp	r3, #153
	bne.n	.L_0200158a
.L_0200152e:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0200158a
	movs	r0, #175
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0200158a
.L_0200154e:
	movs	r5, #0
	movs	r6, #4
.L_02001552:
	mov	r2, r8
	adds	r0, r2, r5
	movs	r1, #5
	mov	r7, sp
	bl 0x0200d3d0
	ldr	r3, [pc, #60]
	lsls	r0, r0, #1
	ldrh	r3, [r3, r6]
	adds	r5, #1
	strh	r3, [r7, r0]
	adds	r6, #2
	cmp	r5, #4
	ble.n	.L_02001552
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r7, #0
	ldr	r1, [pc, #36]
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #16]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_0200158a:
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200f03c
	.4byte 0x0200f040
	.4byte 0x0200f068
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, r6, lr}
	ldr	r2, [pc, #88]
	movs	r3, #1
	adds	r6, r0, #0
	str	r3, [r2, #0]
	cmp	r6, #2
	beq.n	0x020095c8
	ldr	r1, [pc, #80]
	movs	r2, #32
	ldr	r0, [pc, #80]
	ldr	r5, [pc, #80]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4814
	ldr	r1, [pc, #80]
	movs	r2, #32
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x20a0
	lsls	r0, r0, #4
	bl 0x0200d450
	cmp	r0, #0
	bne.n	.L_020015d8
	cmp	r6, #1
	bne.n	.L_020015ea
.L_020015d8:
	ldr	r3, [pc, #60]
	movs	r2, #0
	movs	r1, #144
	str	r2, [r3, #0]
	ldr	r0, [pc, #56]
	lsls	r1, r1, #3
	bl 0x0200d3e0
	b.n	.L_020015fe
.L_020015ea:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #40]
	ldr	r1, [pc, #44]
	adds	r2, #2
.L_020015fa:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_020015fe:
	pop	{r5, r6, pc}
	.4byte 0x0200f040
	.4byte 0x05000180
	.4byte 0x0200f068
	.4byte 0x03000730
	.4byte 0x0200f088
	.4byte 0x050001a0
	.4byte 0x0200f03c
	.4byte 0x020094e9
	.4byte 0x0200f08c
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, lr}
	bl 0x0200d570
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #85
	movs	r3, #4
	strb	r3, [r1, #0]
	movs	r2, #0
	ldr	r3, [r5, #20]
	str	r2, [r5, #68]
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r3, r3, r2
	str	r3, [r5, #12]
	subs	r1, #50
	ldrb	r2, [r1, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	adds	r3, #34
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldrb	r0, [r3, #0]
	bl 0x0200d4c8
	adds	r3, r0, #0
	asrs	r3, r3, #19
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	adds	r3, #6
	movs	r2, #0
	bl 0x0200d540
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #128
	bl 0x0200d6a8
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #133
	mov	sl, r3
	ldr	r3, [pc, #244]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d570
	adds	r6, r0, #0
	ldr	r7, [r6, #104]
	bl 0x0200d560
	movs	r0, #0
	bl 0x0200d648
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #70
	add	r3, sl
	movs	r5, #0
	strh	r5, [r3, #0]
	movs	r3, #85
	adds	r3, r3, r6
	mov	r9, r3
	mov	r2, r9
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d500
	movs	r3, #99
	adds	r3, r3, r7
	mov	r8, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0200171c
.L_020016d6:
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #176]
	str	r3, [r6, #8]
	ldr	r3, [r7, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r7, #16]
	str	r3, [r6, #16]
	cmp	r5, r2
	bgt.n	.L_020016f2
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r5, r5, r3
.L_020016f2:
	ldr	r3, [pc, #156]
	adds	r1, r6, #0
	ldr	r2, [r3, #0]
	ldrb	r3, [r3, #0]
	adds	r1, #35
	lsls	r3, r3, #12
	strh	r3, [r6, #6]
	movs	r3, #1
	ands	r2, r3
	movs	r3, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	movs	r0, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	bl 0x0200d3d8
	mov	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_020016d6
.L_0200171c:
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #68
	movs	r2, #1
	add	r3, sl
	strh	r2, [r3, #0]
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #70
	add	r3, sl
	strh	r2, [r3, #0]
	ldr	r3, [r7, #8]
	ldrh	r1, [r7, #6]
	subs	r2, #3
	asrs	r3, r3, #19
	ands	r3, r2
	asrs	r1, r1, #13
	adds	r3, r3, r1
	subs	r3, #1
	lsls	r3, r3, #19
	str	r3, [r6, #8]
	ldr	r3, [r7, #16]
	ldr	r0, [pc, #56]
	asrs	r3, r3, #19
	ands	r3, r2
	movs	r2, #2
	ands	r1, r2
	subs	r3, r3, r1
	adds	r3, #1
	lsls	r3, r3, #19
	str	r3, [r6, #16]
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	adds	r3, r6, #0
	adds	r3, #35
	strb	r0, [r3, #0]
	mov	r2, r9
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200d500
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	b.n	.L_02001794
	.4byte 0x00000001
	.4byte 0x02000240
	.4byte 0x0003ffff
	.2byte 0x122c
	.2byte 0x0300
.L_02001794:
	bl 0x0200d5d8
	bl 0x0200d5e8
	movs	r3, #128
	adds	r7, r0, #0
	lsls	r3, r3, #12
	str	r3, [r7, #48]
	movs	r3, #128
	ldr	r5, [pc, #52]
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	adds	r3, r7, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r0, #0
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl 0x0200d4c8
	ldr	r3, [r6, #16]
	adds	r2, r0, #0
	ldr	r1, [r6, #8]
	adds	r0, r7, #0
	bl 0x0200d4b8
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	movs	r5, #0
	cmp	r2, r3
	ble.n	.L_020017fa
	b.n	.L_020017e0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020017e0:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200d3d8
	cmp	r5, #59
	bgt.n	.L_020017fa
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	cmp	r2, r3
	bgt.n	.L_020017e0
.L_020017fa:
	movs	r0, #127
	bl 0x0200d6c0
	ldr	r3, [r6, #40]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0200181a
.L_02001808:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200d3d8
	cmp	r5, #59
	bgt.n	.L_0200181a
	ldr	r3, [r6, #40]
	cmp	r3, #0
	bne.n	.L_02001808
.L_0200181a:
	adds	r0, r7, #0
	bl 0x0200d4c0
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200d570
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200d5c8
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #70
	add	r2, sl
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #170
	lsls	r3, r3, #1
	movs	r6, #0
	add	r3, sl
	strh	r6, [r3, #0]
	bl 0x0200d568
	pop	{r3, r5, r6}
	mov	r8, r3
.L_0200185e:
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [r1, #0]
	ldr	r4, [r0, #0]
	ldr	r2, [r1, #8]
	subs	r4, r4, r3
	ldr	r3, [r0, #8]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	asrs	r3, r3, #16
	adds	r2, r3, #0
	muls	r2, r3
	adds	r0, r4, #0
	muls	r0, r4
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	.2byte 0x0000
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
.L_0200189a:
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #133
	mov	sl, r3
	ldr	r3, [pc, #80]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200d570
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r7, r0, #0
	cmp	r3, #0
	bne.n	.L_020018f2
	movs	r3, #173
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020018f2
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020018f2
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02001900
.L_020018f2:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d488
	b.n	.L_02001a3e
	.2byte 0x0240
	.2byte 0x0200
.L_02001900:
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200d488
	adds	r3, r6, #0
	adds	r3, #100
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #31
	ands	r3, r2
	cmp	r3, #31
	bne.n	.L_02001920
	movs	r0, #231
	bl 0x0200d6c0
.L_02001920:
	ldr	r3, [r7, #80]
	ldr	r0, [r6, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r0, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #9]
	movs	r2, #2
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #16]
	bl 0x0200d6a0
	cmp	r0, #255
	beq.n	.L_02001a22
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl 0x0200d658
	ldr	r5, [r5, #0]
	movs	r3, #136
	lsls	r3, r3, #17
	cmp	r5, r3
	bgt.n	.L_02001a22
	ldr	r2, [pc, #172]
	cmp	r5, r2
	blt.n	.L_02001a22
	movs	r3, #98
	adds	r3, r3, r6
	mov	r9, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_020019e8
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #12]
	subs	r5, r2, r3
	cmp	r5, #0
.L_0200197c:
	bge.n	.L_02001980
	subs	r5, r3, r2
.L_02001980:
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	adds	r0, #8
	adds	r1, #8
	mov	r8, r2
	bl 0x02009868
	cmp	r0, #12
	bgt.n	.L_020019a0
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r5, r3
	bge.n	.L_020019a0
	movs	r2, #1
	mov	r8, r2
.L_020019a0:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_020019e8
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200d450
	cmp	r0, #0
	bne.n	.L_020019e8
	ldrh	r3, [r6, #6]
	str	r6, [r7, #104]
	strh	r3, [r7, #6]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
.L_020019c0:
	ands	r3, r2
	movs	r2, #181
	lsls	r2, r2, #1
	strb	r3, [r1, #0]
	add	r2, sl
	movs	r3, #200
	strh	r3, [r2, #0]
	ldr	r3, [pc, #68]
	movs	r2, #128
	ldr	r0, [pc, #56]
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	strb	r0, [r3, #0]
	mov	r2, r9
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r2, r6, #0
	adds	r2, #99
	strb	r3, [r2, #0]
.L_020019e8:
	ldrh	r0, [r6, #6]
	bl 0x0200d400
	ldr	r1, [r6, #48]
	ldr	r5, [pc, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200d3f8
	ldr	r1, [r6, #48]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6933
	b.n	.L_02001a1c
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
.L_02001a1c:
	adds	r3, r3, r0
	str	r3, [r6, #16]
	b.n	.L_02001a3e
.L_02001a22:
	adds	r3, r6, #0
	adds	r3, #99
	movs	r5, #0
	strb	r5, [r3, #0]
	ldr	r1, [pc, #32]
	adds	r0, r6, #0
	str	r5, [r6, #108]
	bl 0x0200d490
.L_02001a34:
	movs	r0, #228
	bl 0x0200d6c0
	ldr	r3, [pc, #20]
	str	r5, [r3, #0]
.L_02001a3e:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200f044
	.2byte 0xf064
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #222
	sub	sp, #68
	bl 0x0200d6c0
	ldrh	r0, [r5, #6]
	bl 0x0200d400
	adds	r1, r0, #0
	movs	r0, #128
	ldr	r6, [pc, #152]
	lsls	r0, r0, #12
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x68ab
	add	r2, sp, #56
	adds	r3, r3, r0
	str	r3, [r2, #0]
	mov	r8, r2
	ldrh	r0, [r5, #6]
	bl 0x0200d3f8
	adds	r1, r0, #0
	movs	r0, #128
	lsls	r0, r0, #12
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x692b
	mov	r2, r8
	adds	r3, r3, r0
	str	r3, [r2, #8]
	movs	r0, #140
	ldr	r1, [r2, #0]
	lsls	r0, r0, #1
	ldr	r2, [r5, #12]
	bl 0x0200d498
	movs	r1, #2
	adds	r7, r0, #0
	bl 0x0200d480
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200d500
	adds	r3, r7, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	ldr	r2, [pc, #56]
	ldrh	r3, [r5, #6]
	add	r4, sp, #16
	strh	r3, [r7, #6]
	adds	r3, r7, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	subs	r3, #2
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	ldr	r3, [pc, #44]
	str	r3, [r7, #108]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r7, #48]
.L_02001ae8:
	movs	r3, #1
	str	r3, [r4, #0]
	movs	r3, #7
	str	r3, [r4, #4]
	mov	r3, r8
	ldr	r0, [r3, #0]
	ldr	r2, [r3, #8]
	ldr	r3, [pc, #24]
	ldr	r1, [r5, #12]
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [sp, #8]
	b.n	.L_02001b14
	.4byte 0x00000000
	.4byte 0x0300021c
	.4byte 0x02009895
	.2byte 0x0000
	.2byte 0xfffa
.L_02001b14:
	.2byte 0x2300
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	str	r4, [sp, #12]
	.2byte 0xf001
	.2byte 0xfd4c
	adds	r0, r7, #0
	add	sp, #68
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #144]
	sub	sp, #56
	ldr	r7, [r3, #0]
	movs	r3, #7
	ands	r7, r3
	mov	sl, r0
	cmp	r7, #0
	bne.n	.L_02001bba
	add	r6, sp, #16
	movs	r3, #3
	str	r3, [r6, #0]
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	movs	r3, #14
	str	r3, [r6, #4]
	bl 0x0200d3e8
	mov	r2, sl
	lsls	r3, r0, #3
	ldr	r2, [r2, #8]
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	subs	r3, #4
	lsls	r3, r3, #16
	mov	r8, r2
	add	r8, r3
	bl 0x0200d3e8
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	lsrs	r3, r3, #16
	movs	r2, #32
	subs	r2, r2, r3
	mov	r3, sl
	ldr	r5, [r3, #12]
	lsls	r2, r2, #16
	adds	r5, r5, r2
	bl 0x0200d3e8
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsrs	r0, r0, #16
	movs	r2, #160
	lsls	r2, r2, #11
	lsls	r0, r0, #16
	adds	r0, r0, r2
	movs	r1, #10
	bl 0x0200d3c8
	mov	r3, sl
	ldr	r2, [r3, #16]
	movs	r3, #176
	lsls	r3, r3, #12
	str	r0, [sp, #0]
	str	r3, [sp, #8]
	mov	r0, r8
	adds	r1, r5, #0
	movs	r3, #0
	str	r7, [sp, #4]
	str	r6, [sp, #12]
	.2byte 0xf001
	.2byte 0xfcff
.L_02001bba:
	movs	r0, #0
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #9
	bl 0x0200d570
	adds	r6, r0, #0
	movs	r0, #10
	bl 0x0200d570
	adds	r7, r0, #0
	movs	r0, #23
	bl 0x0200d570
	adds	r5, r0, #0
	ldr	r2, [r5, #80]
	movs	r1, #128
	mov	r8, r2
	movs	r2, #248
	movs	r0, #24
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200d598
	movs	r1, #128
	movs	r2, #248
	movs	r0, #23
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200d598
	movs	r1, #236
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d598
	movs	r1, #138
	movs	r2, #128
	lsls	r2, r2, #17
	movs	r0, #10
	lsls	r1, r1, #18
	bl 0x0200d598
	movs	r0, #24
	bl 0x0200d570
	movs	r3, #85
	movs	r2, #0
	adds	r3, r3, r5
	str	r2, [r0, #24]
	strb	r2, [r3, #0]
	mov	fp, r3
	ldr	r3, [r5, #20]
	movs	r0, #160
	str	r3, [r5, #12]
	movs	r3, #85
	adds	r3, r3, r6
	strb	r2, [r3, #0]
	mov	r9, r3
	ldr	r3, [r6, #20]
	lsls	r0, r0, #4
	str	r3, [r6, #12]
	movs	r3, #85
	adds	r3, r3, r7
	strb	r2, [r3, #0]
	mov	sl, r3
	ldr	r3, [r7, #20]
	adds	r0, #10
	str	r3, [r7, #12]
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02001ca8
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #208]
	movs	r0, #9
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r2, [pc, #204]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r2, [pc, #200]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	mov	r2, r8
	ldrh	r3, [r2, #18]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	mov	r2, r8
	strh	r3, [r2, #18]
	bl 0x0200d570
	movs	r1, #4
	bl 0x0200d520
	movs	r0, #11
	bl 0x0200d570
	movs	r1, #4
	bl 0x0200d520
.L_02001ca8:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02001cf2
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #132]
	movs	r0, #10
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r2, [pc, #128]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r2, [pc, #128]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	mov	r2, r8
	ldrh	r3, [r2, #18]
	ldr	r2, [pc, #120]
	adds	r3, r3, r2
	mov	r2, r8
	strh	r3, [r2, #18]
	bl 0x0200d570
	movs	r1, #4
	bl 0x0200d520
	movs	r0, #12
	bl 0x0200d570
	movs	r1, #4
	bl 0x0200d520
.L_02001cf2:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02001d32
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02001d32
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #64]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r2, [pc, #56]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	mov	r2, r9
	movs	r3, #4
	strb	r3, [r2, #0]
	mov	r2, sl
	strb	r3, [r2, #0]
	mov	r2, fp
	strb	r3, [r2, #0]
.L_02001d32:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00066640
	.4byte 0x0001eb80
	.4byte 0xfffd70c0
	.4byte 0x00028f40
	.4byte 0xfffff800
	.4byte 0x00199900
	.2byte 0x8480
	.2byte 0x001b
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
.L_02001d64:
	cmp	r2, #0
	blt.n	.L_02001d72
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001d7c
	b.n	.L_02001dbc
.L_02001d72:
	movs	r2, #128
	subs	r3, r3, r4
	lsls	r2, r2, #12
.L_02001d78:
	cmp	r3, r2
	bge.n	.L_02001dbc
.L_02001d7c:
	ldr	r4, [r0, #12]
	ldr	r3, [r1, #12]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_02001d90
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001d9a
	b.n	.L_02001dbc
.L_02001d90:
	movs	r2, #128
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001dbc
.L_02001d9a:
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_02001dae
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001db8
	b.n	.L_02001dbc
.L_02001dae:
	movs	r2, #128
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001dbc
.L_02001db8:
	movs	r0, #1
	b.n	.L_02001dbe
.L_02001dbc:
	movs	r0, #0
.L_02001dbe:
	pop	{pc}
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_02001dd6
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001de0
	b.n	.L_02001e12
.L_02001dd6:
	movs	r2, #192
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001e12
.L_02001de0:
	ldr	r2, [r1, #12]
	ldr	r3, [r0, #12]
	subs	r3, r3, r2
	ldr	r2, [pc, #48]
	adds	r3, r3, r2
	ldr	r2, [pc, #48]
	cmp	r3, r2
	bhi.n	.L_02001e12
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_02001e04
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001e0e
	b.n	.L_02001e12
.L_02001e04:
	movs	r2, #192
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001e12
.L_02001e0e:
	movs	r0, #1
	b.n	.L_02001e14
.L_02001e12:
	movs	r0, #0
.L_02001e14:
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
	bl 0x0200d570
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
	bl 0x0200d3c8
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bge.n	.L_02001e7c
	adds	r3, #15
.L_02001e7c:
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
	bne.n	.L_02001f0e
	adds	r0, r6, #0
	adds	r1, r4, #0
	.2byte 0xf7ff
	.2byte 0xff56
	cmp	r0, #0
	beq.n	.L_02001f0e
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	r3, sl
	ldrb	r2, [r3, #0]
	cmp	r2, #0
	bne.n	.L_02001f0e
	ldr	r1, [r6, #76]
	cmp	r1, #0
	beq.n	.L_02001ee2
	mov	r3, r8
	adds	r3, #104
	strh	r2, [r3, #0]
	mov	r2, r8
	adds	r2, #106
	cmp	r1, #0
	ble.n	.L_02001eda
	movs	r3, #1
	b.n	.L_02001ee0
.L_02001eda:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
.L_02001ee0:
	strh	r3, [r2, #0]
.L_02001ee2:
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
.L_02001f0e:
	movs	r3, #84
	mov	r2, r8
	ldrh	r0, [r2, r3]
	movs	r7, #0
	cmp	r0, #0
	beq.n	.L_02001f6e
	mov	r5, r8
	adds	r5, #84
.L_02001f1e:
	bl 0x0200d570
	adds	r4, r0, #0
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_02001f60
	adds	r0, r6, #0
	adds	r1, r4, #0
	.2byte 0xf7ff
	.2byte 0xff46
	cmp	r0, #0
	beq.n	.L_02001f60
	ldrh	r1, [r5, #2]
	cmp	r1, #0
	bne.n	.L_02001f5a
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
	b.n	.L_02001f60
.L_02001f5a:
	movs	r3, #1
	mov	r2, r8
	strh	r3, [r2, #6]
.L_02001f60:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #3
	bgt.n	.L_02001f6e
	ldrh	r0, [r5, #0]
	cmp	r0, #0
	bne.n	.L_02001f1e
.L_02001f6e:
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
	bl 0x0200d500
	ldr	r1, [pc, #20]
	adds	r0, r5, #0
	bl 0x0200d490
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xdd8c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200d498
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001ff4
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	str	r6, [r5, #76]
	movs	r1, #2
	.2byte 0xf7ff
	.2byte 0xffd4
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_02001fec
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02001fec:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200d480
.L_02001ff4:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x9e21
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
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
	bl 0x0200d570
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
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
	ldr	r3, [r6, #8]
	adds	r4, r0, #0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #8]
	str	r4, [sp, #0]
	bl 0x0200d3c8
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bge.n	.L_02002058
	adds	r3, #15
.L_02002058:
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
	bne.n	.L_020020e8
	adds	r0, r6, #0
	adds	r1, r4, #0
	.2byte 0xf7ff
	.2byte 0xfe68
	cmp	r0, #0
	beq.n	.L_020020e8
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	r3, sl
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_020020e8
	ldr	r2, [r6, #76]
	cmp	r2, #0
	beq.n	.L_020020bc
	mov	r1, r8
	adds	r1, #106
	strh	r3, [r1, #0]
	subs	r1, #2
	cmp	r2, #0
	ble.n	.L_020020b4
	movs	r3, #1
	b.n	.L_020020ba
.L_020020b4:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
.L_020020ba:
	strh	r3, [r1, #0]
.L_020020bc:
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
.L_020020e8:
	movs	r3, #84
	mov	r2, r8
	ldrh	r0, [r2, r3]
	movs	r7, #0
	cmp	r0, #0
	beq.n	.L_02002148
	mov	r5, r8
	adds	r5, #84
.L_020020f8:
	bl 0x0200d570
	adds	r4, r0, #0
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_0200213a
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x02009dc0
	cmp	r0, #0
	beq.n	.L_0200213a
	ldrh	r1, [r5, #2]
	cmp	r1, #0
	bne.n	.L_02002134
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
	b.n	.L_0200213a
.L_02002134:
	movs	r3, #1
	mov	r2, r8
	strh	r3, [r2, #6]
.L_0200213a:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #3
	bgt.n	.L_02002148
	ldrh	r0, [r5, #0]
	cmp	r0, #0
	bne.n	.L_020020f8
.L_02002148:
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
	bl 0x0200d500
	ldr	r1, [pc, #16]
	adds	r0, r5, #0
	bl 0x0200d490
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0xdd8c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200d498
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020021e2
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	negs	r3, r6
	str	r3, [r5, #76]
	movs	r1, #3
	.2byte 0xf7ff
	.2byte 0xffc9
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_020021da
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_020021da:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200d480
.L_020021e2:
	pop	{r5, r6, r7, pc}
	.2byte 0x9e21
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200d498
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02002228
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	str	r6, [r5, #76]
	movs	r1, #3
	.2byte 0xf7ff
	.2byte 0xffa6
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_02002220
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02002220:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200d480
.L_02002228:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x9ffd
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200d498
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02002272
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	negs	r3, r6
	str	r3, [r5, #76]
	movs	r1, #3
	.2byte 0xf7ff
	.2byte 0xff81
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_0200226a
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_0200226a:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200d480
.L_02002272:
	pop	{r5, r6, r7, pc}
	.2byte 0x9ffd
	.2byte 0x0200
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_0200228e
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02002298
	b.n	.L_020022c8
.L_0200228e:
	movs	r2, #192
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_020022c8
.L_02002298:
	ldr	r2, [r1, #12]
	ldr	r3, [r0, #12]
	subs	r3, r3, r2
	ldr	r2, [pc, #44]
	subs	r3, #1
	cmp	r3, r2
	bhi.n	.L_020022c8
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_020022ba
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_020022c4
	b.n	.L_020022c8
.L_020022ba:
	movs	r2, #128
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_020022c8
.L_020022c4:
	movs	r0, #1
	b.n	.L_020022ca
.L_020022c8:
	movs	r0, #0
.L_020022ca:
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
	bl 0x0200d570
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
	bl 0x0200d3c8
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02002314
	adds	r3, #15
.L_02002314:
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
	bne.n	.L_02002362
	adds	r0, r6, #0
	mov	r1, r8
	bl 0x0200a278
	cmp	r0, #0
	beq.n	.L_02002362
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
.L_02002362:
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
	bl 0x0200d500
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200d480
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	bl 0x0200d490
	adds	r0, r5, #0
	movs	r1, #10
	bl 0x0200d5b0
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xdd8c
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
	bl 0x0200d498
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200241e
	bl 0x0200d3e8
	adds	r3, r0, #0
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #12
	movs	r3, #192
	lsls	r3, r3, #6
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	bl 0x0200d400
	str	r0, [r5, #68]
	bl 0x0200d3e8
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r0, [r5, #72]
	bl 0x0200d3e8
	lsls	r0, r0, #17
	lsrs	r0, r0, #16
	adds	r0, r0, r6
	str	r0, [r5, #76]
	bl 0x0200d3e8
	ldr	r3, [pc, #36]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r5, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	movs	r1, #2
	adds	r0, r5, #0
	bl 0x0200a36c
	ldr	r3, [pc, #20]
	adds	r0, r5, #0
	str	r3, [r5, #108]
	movs	r1, #1
	bl 0x0200d508
.L_0200241e:
	pop	{r5, r6, pc}
	.4byte 0xfffe0000
	.4byte 0xffff8000
	.2byte 0xa2d1
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
	beq.n	0x0200a486
	movs	r1, #8
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	bne.n	0x0200a486
	ldr	r3, [r0, #16]
	cmp	r3, #0
	beq.n	0x0200a486
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
.L_02002498:
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_02002588
	ldr	r5, [r7, #8]
	cmp	r5, #0
	beq.n	.L_02002588
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200d450
	ldr	r4, [sp, #0]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r4, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020024c2
	movs	r3, #1
	orrs	r0, r3
.L_020024c2:
	adds	r6, r5, #0
	adds	r6, #91
	strb	r0, [r6, #0]
	mov	r0, r8
	movs	r4, #14
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	beq.n	.L_020024dc
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200d450
	strb	r0, [r6, #0]
.L_020024dc:
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
	bne.n	.L_02002512
	adds	r3, r2, #7
	movs	r2, #167
	lsls	r2, r2, #1
	cmp	r3, r2
	bhi.n	.L_02002588
	movs	r3, #48
	negs	r3, r3
	cmp	r1, r3
	ble.n	.L_02002588
	cmp	r1, #239
	bgt.n	.L_02002588
.L_02002512:
	movs	r0, #2
	ldrsh	r3, [r7, r0]
	ldrh	r1, [r7, #2]
	cmp	r3, #0
	bgt.n	.L_02002584
	ldrh	r3, [r7, #4]
	movs	r1, #240
	ands	r1, r3
	cmp	r1, #32
	beq.n	.L_02002558
	cmp	r1, #32
	bgt.n	.L_02002534
	cmp	r1, #0
	beq.n	.L_02002574
	cmp	r1, #16
	beq.n	.L_02002566
	b.n	.L_02002580
.L_02002534:
	cmp	r1, #48
	beq.n	.L_0200254a
	cmp	r1, #128
	bne.n	.L_02002580
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	.2byte 0xf7ff
	.2byte 0xff32
	b.n	.L_02002580
.L_0200254a:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	.2byte 0xf7ff
	.2byte 0xfe25
	b.n	.L_02002580
.L_02002558:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	.2byte 0xf7ff
	.2byte 0xfe66
	b.n	.L_02002580
.L_02002566:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	.2byte 0xf7ff
	.2byte 0xfe3b
	b.n	.L_02002580
.L_02002574:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	.2byte 0xf7ff
	.2byte 0xfd1a
.L_02002580:
	movs	r3, #8
	b.n	.L_02002586
.L_02002584:
	subs	r3, r1, #1
.L_02002586:
	strh	r3, [r7, #2]
.L_02002588:
	movs	r1, #1
	negs	r1, r1
	add	fp, r1
	mov	r2, fp
	adds	r7, #16
	cmp	r2, #0
	blt.n	.L_02002598
	b.n	.L_02002498
.L_02002598:
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
	bl 0x0200d450
	cmp	r0, #0
	bne.n	0x0200a5ca
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
	bl 0x0200d498
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #1
	bl 0x0200d480
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
	bl 0x0200d4a0
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
	bl 0x0200d450
	cmp	r0, #0
	bne.n	.L_02002708
	ldrh	r3, [r6, #0]
	movs	r2, #0
	mov	fp, r2
	mov	sl, r3
	adds	r6, #2
	cmp	r3, #0
	ble.n	.L_020026c6
.L_0200265e:
	ldrh	r7, [r6, #0]
	movs	r1, #15
	ands	r1, r7
	movs	r3, #240
	mov	r0, sl
	str	r1, [sp, #0]
	ands	r7, r3
	bl 0x0200d570
	adds	r5, r0, #0
	adds	r6, #2
	cmp	r5, #0
	beq.n	.L_020026b2
	movs	r1, #0
	bl 0x0200d500
	adds	r3, r5, #0
	movs	r2, #128
	adds	r3, #98
	movs	r1, #1
	ands	r2, r7
	strb	r1, [r3, #0]
	cmp	r2, #0
	bne.n	.L_02002692
	subs	r3, #9
	strb	r2, [r3, #0]
.L_02002692:
	mov	r2, r9
	mov	r3, r9
	strh	r1, [r2, #0]
	mov	r0, sl
	strh	r7, [r3, #4]
	bl 0x0200d570
	mov	r1, r9
	str	r0, [r1, #8]
	ldr	r2, [sp, #0]
	lsls	r3, r2, #16
	str	r3, [r1, #12]
	mov	r3, fp
	strh	r3, [r1, #2]
	movs	r2, #16
	add	r9, r2
.L_020026b2:
	movs	r3, #1
	add	fp, r3
	mov	r1, fp
	cmp	r1, #3
	bgt.n	.L_020026c6
	ldrh	r2, [r6, #0]
	adds	r6, #2
	mov	sl, r2
	cmp	r2, #0
	bgt.n	.L_0200265e
.L_020026c6:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_02002708
	movs	r1, #0
	ldrh	r2, [r3, #0]
	mov	fp, r1
	ldr	r1, [sp, #4]
	movs	r3, #2
	add	r8, r3
	movs	r3, #84
	strh	r2, [r1, r3]
	cmp	r2, #0
	ble.n	.L_02002708
	adds	r2, r1, #0
	adds	r2, #84
.L_020026e4:
	mov	r1, r8
	ldrh	r3, [r1, #0]
	movs	r1, #1
	strh	r3, [r2, #2]
	add	fp, r1
	movs	r3, #2
	add	r8, r3
	mov	r3, fp
	adds	r2, #4
	cmp	r3, #3
	bgt.n	.L_02002708
	mov	r1, r8
	ldrh	r3, [r1, #0]
.L_020026fe:
	movs	r1, #2
	add	r8, r1
	strh	r3, [r2, #0]
	cmp	r3, #0
	bgt.n	.L_020026e4
.L_02002708:
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
	bne.n	.L_02002738
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
.L_02002738:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl 0x0200d3e0
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200254c
	.4byte 0x0200a42d
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
	bl 0x0200d450
	cmp	r0, #0
	bne.n	.L_020027a2
	ldr	r3, [r5, #0]
	adds	r3, #1
	str	r3, [r5, #0]
	cmp	r3, r6
	blt.n	.L_020027a2
	str	r0, [r5, #0]
.L_020027a2:
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
	bhi.n	.L_020027fa
	lsls	r3, r0, #2
	adds	r3, #84
	strh	r1, [r2, r3]
.L_020027fa:
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
	bl 0x0200d690
	mov	r8, r0
	bl 0x0200d570
	bl 0x0200d550
	movs	r5, #0
	adds	r7, r0, #0
	cmp	r5, r7
	bge.n	.L_02002842
.L_02002826:
	ldr	r2, [pc, #192]
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldrb	r0, [r2, r3]
	bl 0x0200d448
	ldrh	r3, [r0, #56]
	lsls	r2, r5, #1
	mov	r1, sp
	adds	r5, #1
	strh	r3, [r1, r2]
	cmp	r5, r7
	blt.n	.L_02002826
.L_02002842:
	movs	r0, #10
	negs	r0, r0
	movs	r1, #0
	bl 0x0200d650
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
	bl 0x0200d5c0
	cmp	r5, r7
	bge.n	.L_020028dc
.L_0200286e:
	ldr	r1, [pc, #120]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r2, r2, r5
	ldrb	r0, [r1, r2]
	mov	sl, r1
	mov	r8, r2
	bl 0x0200d448
	movs	r4, #56
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	ble.n	.L_02002896
	movs	r1, #183
	lsls	r1, r1, #1
	adds	r2, r6, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020028d6
.L_02002896:
	mov	r3, sp
	lsls	r2, r5, #1
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020028d6
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
.L_020028d6:
	adds	r5, #1
	cmp	r5, r7
	blt.n	.L_0200286e
.L_020028dc:
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
	bl 0x0200d570
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
	bl 0x0200d3c8
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_0200292a
	adds	r3, #15
.L_0200292a:
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
	beq.n	.L_02002968
	cmp	r2, #4
	beq.n	.L_02002970
	b.n	.L_02002976
.L_02002968:
	movs	r1, #10
	bl 0x0200d520
	b.n	.L_02002976
.L_02002970:
	movs	r1, #0
	bl 0x0200d520
.L_02002976:
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
	beq.n	.L_0200299e
	b.n	.L_02002b5c
.L_0200299e:
	movs	r0, #10
	movs	r1, #0
	negs	r0, r0
	.2byte 0xf7ff
	.2byte 0xff2c
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	bl 0x0200d570
	ldr	r2, [pc, #444]
	adds	r6, r0, #0
	str	r2, [sp, #0]
	bl 0x0200d560
	movs	r0, #0
	bl 0x0200d648
	ldr	r3, [pc, #432]
	adds	r0, r6, #0
	str	r3, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	movs	r1, #49
	bl 0x0200d480
.L_020029d6:
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl 0x0200d4d8
	ldr	r3, [sp, #0]
	ldr	r1, [sp, #0]
	adds	r3, #104
	adds	r1, #106
	mov	r9, r1
	mov	sl, r3
	add	r1, sp, #4
	cmp	r0, #7
	bne.n	.L_02002a52
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
.L_02002a10:
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
	bl 0x0200d4f8
	cmp	r0, #0
	beq.n	.L_02002a44
	movs	r3, #0
	str	r3, [r6, #36]
	str	r3, [r6, #44]
	b.n	.L_02002b50
.L_02002a44:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200d3d8
	cmp	r5, #9
	ble.n	.L_02002a10
	b.n	.L_02002b50
.L_02002a52:
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
	bl 0x0200d4f8
	cmp	r0, #0
	bgt.n	.L_02002b50
	cmp	r0, #0
	bge.n	.L_02002a9c
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
	bl 0x0200d5a8
	b.n	.L_02002b50
.L_02002a9c:
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
.L_02002ab0:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02002ae0
	ldrb	r2, [r7, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002ae0
	cmp	r5, r6
	beq.n	.L_02002ae0
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	mov	r1, fp
	add	r2, sp, #4
	bl 0x0200d538
	cmp	r0, #0
	blt.n	.L_02002ae0
	movs	r0, #1
	bl 0x0200d3d8
	b.n	.L_02002b50
.L_02002ae0:
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	adds	r7, #128
	adds	r5, #128
	cmp	r0, #63
	ble.n	.L_02002ab0
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
	ble.n	.L_02002b1e
	adds	r2, r4, #0
.L_02002b1e:
	ldr	r0, [pc, #100]
	cmp	r2, r0
	bge.n	.L_02002b26
	adds	r2, r0, #0
.L_02002b26:
	subs	r3, r1, r2
	ldr	r1, [pc, #84]
	str	r3, [r6, #8]
	adds	r3, r5, #0
	ands	r3, r7
	adds	r2, r3, r1
	cmp	r2, r4
	ble.n	.L_02002b38
	adds	r2, r4, #0
.L_02002b38:
	cmp	r2, r0
	bge.n	.L_02002b3e
	adds	r2, r0, #0
.L_02002b3e:
	subs	r3, r5, r2
	str	r3, [r6, #16]
	ldr	r2, [sp, #0]
	movs	r3, #0
	strh	r3, [r2, #4]
	movs	r0, #1
	bl 0x0200d3d8
	b.n	.L_020029d6
.L_02002b50:
	movs	r3, #0
	str	r3, [r6, #108]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d520
.L_02002b5c:
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
	.4byte 0x0200a955
	.4byte 0x000fffff
	.4byte 0xfff80000
	.2byte 0x0000
	.2byte 0xffff
.L_02002b88:
	.2byte 0xb500
	ldr	r3, [pc, #16]
	movs	r2, #4
	ldrsh	r3, [r3, r2]
.L_02002b90:
	cmp	r3, #0
	beq.n	.L_02002b98
	bl 0x0200a97c
.L_02002b98:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
.L_02002ba6:
	ldr	r5, [r3, #108]
	movs	r1, #217
	lsls	r1, r1, #1
	adds	r6, r5, r1
.L_02002bae:
	ldrh	r3, [r6, #0]
	sub	sp, #12
	cmp	r3, #0
	bne.n	.L_02002bcc
	movs	r2, #214
.L_02002bb8:
	lsls	r2, r2, #1
	adds	r3, r5, r2
	adds	r1, #2
	ldr	r0, [r3, #0]
.L_02002bc0:
	adds	r3, r5, r1
	ldr	r1, [r3, #0]
	bl 0x0200d610
	movs	r3, #1
	strh	r3, [r6, #0]
.L_02002bcc:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200d460
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
	bl 0x0200a800
	movs	r0, #224
	movs	r1, #224
	lsls	r1, r1, #8
	lsls	r0, r0, #11
	bl 0x0200d5d0
	ldr	r3, [pc, #192]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d570
	adds	r6, r0, #0
	movs	r0, #131
	lsls	r0, r0, #1
	ldr	r7, [pc, #176]
	bl 0x0200d458
	bl 0x0200d560
	movs	r0, #0
	bl 0x0200d648
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200d460
	ldr	r3, [pc, #156]
	movs	r1, #49
	str	r3, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r0, r6, #0
	bl 0x0200d480
	ldr	r3, [r7, #108]
	adds	r3, #100
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002cb8
.L_02002c58:
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	movs	r0, #0
	bl 0x0200d4d8
	ldr	r1, [r7, #108]
	cmp	r0, #7
	beq.n	.L_02002c78
	ldr	r2, [r1, #112]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldr	r3, [r6, #16]
	ldr	r2, [r1, #120]
	adds	r3, r3, r2
	b.n	.L_02002ca0
.L_02002c78:
	ldr	r3, [r1, #112]
	cmp	r3, #0
	beq.n	.L_02002c8c
	ldr	r3, [r6, #8]
	ldr	r2, [pc, #88]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r6, #8]
.L_02002c8c:
	ldr	r3, [r7, #108]
	ldr	r3, [r3, #120]
	cmp	r3, #0
	beq.n	.L_02002ca2
	ldr	r3, [r6, #16]
	ldr	r2, [pc, #68]
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #12
	adds	r3, r3, r1
.L_02002ca0:
	str	r3, [r6, #16]
.L_02002ca2:
	movs	r3, #0
	strh	r3, [r7, #4]
	movs	r0, #1
	bl 0x0200d3d8
	ldr	r3, [r7, #108]
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
.L_02002cb4:
	cmp	r3, #0
	beq.n	.L_02002c58
.L_02002cb8:
	movs	r5, #0
	movs	r0, #30
	bl 0x0200d558
	adds	r0, r6, #0
	str	r5, [r6, #108]
	movs	r1, #0
	bl 0x0200d520
	strh	r5, [r7, #4]
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200254c
	.4byte 0x0200a955
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb500
	ldr	r3, [pc, #20]
	movs	r2, #4
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02002cf4
	bl 0x0200aba0
	movs	r0, #1
	b.n	.L_02002cf6
.L_02002cf4:
	movs	r0, #0
.L_02002cf6:
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
	bne.n	.L_02002d24
	b.n	.L_02002e7c
.L_02002d24:
	movs	r2, #0
	str	r2, [sp, #0]
.L_02002d28:
	bl 0x0200d3e8
	adds	r5, r0, #0
	bl 0x0200d3e8
	mov	r3, fp
	ldr	r3, [r3, #8]
	lsls	r5, r5, #4
	mov	r8, r3
	add	r8, r5
	lsls	r0, r0, #4
	mov	r1, r8
	subs	r1, r1, r0
	mov	r8, r1
	bl 0x0200d3e8
	adds	r6, r0, #0
	bl 0x0200d3e8
	adds	r5, r0, #0
	bl 0x0200d3e8
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
	bl 0x0200d498
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02002e5c
	bl 0x0200d3e8
	mov	sl, r0
	bl 0x0200d3e8
	adds	r6, r0, #0
	bl 0x0200d3e8
	adds	r5, r0, #0
	bl 0x0200d3e8
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r5, r5, r0
	ldr	r1, [pc, #216]
	adds	r0, r7, #0
	mov	r9, r2
	bl 0x0200d490
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200d500
	mov	r1, sl
	movs	r2, #128
	lsls	r2, r2, #10
	lsls	r3, r1, #2
	adds	r3, r3, r2
	str	r3, [r7, #40]
	mov	r0, sl
	bl 0x0200d400
	ldr	r3, [pc, #184]
	lsls	r6, r6, #3
	mov	r8, r3
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x62f8
	mov	r0, sl
	bl 0x0200d3f8
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
	bl 0x0200d428
	ldrb	r3, [r5, #17]
	ldr	r1, [pc, #100]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r5, #17]
	ldrh	r3, [r1, #12]
	ldr	r0, [r5, #40]
	strb	r3, [r5, #16]
	bl 0x0200d478
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
.L_02002e5c:
	ldr	r1, [sp, #0]
	subs	r1, #1
	str	r1, [sp, #0]
	cmp	r1, #0
	blt.n	.L_02002e68
	b.n	.L_02002d28
.L_02002e68:
	b.n	.L_02002e7c
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0x0200254c
	.4byte 0x0200ddbc
	.2byte 0x021c
	.2byte 0x0300
.L_02002e7c:
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
.L_02002ea8:
	movs	r0, #70
	adds	r0, #255
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #4]
	bl 0x0200d498
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02002f4a
	bl 0x0200d3e8
	adds	r5, r0, #0
	bl 0x0200d3e8
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
	bl 0x0200d3e8
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #3
	mov	r8, r3
	bl 0x0200d3e8
	ldr	r1, [pc, #116]
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl 0x0200d490
	movs	r1, #0
	adds	r0, r7, #0
	bl 0x0200d500
	movs	r3, #160
	lsls	r3, r3, #9
	adds	r5, r5, r3
	str	r5, [r7, #40]
	mov	r0, r9
	bl 0x0200d400
	ldr	r5, [pc, #88]
	adds	r1, r0, #0
	mov	r0, r8
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x62f8
	mov	r0, r9
	bl 0x0200d3f8
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
	bl 0x0200d520
.L_02002f4a:
	movs	r3, #1
	negs	r3, r3
	add	fp, r3
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_02002ea8
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200de00
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200d450
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
	beq.n	.L_02002fa8
	b.n	.L_02003234
.L_02002fa8:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200d450
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L_02002fc2
	movs	r3, #1
	orrs	r0, r3
.L_02002fc2:
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
	bl 0x0200d408
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	movs	r0, #0
	bl 0x0200d4d8
	cmp	r0, #7
	bne.n	0x0200b038
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
	bl 0x0200d6c0
	movs	r0, #160
	lsls	r0, r0, #11
	movs	r2, #128
	adds	r1, r0, #0
	lsls	r2, r2, #9
	bl 0x0200d510
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200d510
	ldr	r3, [r5, #104]
	cmp	r3, #0
	beq.n	0x0200b038
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
.L_020030a4:
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
.L_020030c0:
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
	bne.n	.L_02003120
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
	bl 0x0200d4f0
	mov	r4, r8
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	movs	r0, #64
	str	r4, [sp, #4]
	str	r6, [sp, #0]
	bl 0x0200d4e8
	movs	r0, #159
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d6c0
	mov	r3, r8
	movs	r4, #128
	lsls	r4, r4, #12
	lsls	r2, r3, #20
	lsls	r0, r6, #20
	adds	r0, r0, r4
	ldr	r1, [r5, #12]
	ldrh	r3, [r5, #6]
	adds	r2, r2, r4
	bl 0x0200ae8c
.L_02003120:
	ldr	r4, [sp, #12]
	ldr	r0, [sp, #24]
	adds	r2, r4, r0
	ldrb	r3, [r2, #2]
	cmp	r3, #77
	bne.n	.L_02003174
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
	bl 0x0200d4f0
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	movs	r0, #64
	str	r6, [sp, #0]
	bl 0x0200d4e8
	movs	r0, #143
	lsls	r0, r0, #2
	bl 0x0200d6c0
	mov	r3, r8
	movs	r4, #128
	lsls	r4, r4, #12
	lsls	r2, r3, #20
	lsls	r0, r6, #20
	adds	r0, r0, r4
	ldr	r1, [r5, #12]
	ldrh	r3, [r5, #6]
	adds	r2, r2, r4
	bl 0x0200ae8c
.L_02003174:
	ldr	r0, [sp, #36]
	movs	r4, #1
	adds	r0, #1
	add	r9, r4
	adds	r6, #1
	add	sl, r4
	str	r0, [sp, #36]
	cmp	r0, #1
	ble.n	.L_020030c0
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #40]
	adds	r1, #1
	adds	r2, #1
	str	r1, [sp, #8]
	add	r8, r4
	add	fp, r4
	str	r2, [sp, #40]
	cmp	r2, #1
	ble.n	.L_020030a4
	ldr	r3, [r5, #24]
	movs	r4, #128
	lsls	r4, r4, #9
.L_020031a0:
	cmp	r3, r4
	bge.n	.L_020031b2
	movs	r2, #128
	lsls	r2, r2, #5
.L_020031a8:
	adds	r3, r3, r2
	str	r3, [r5, #24]
	ldr	r3, [r5, #28]
	adds	r3, r3, r2
.L_020031b0:
	str	r3, [r5, #28]
.L_020031b2:
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	adds	r0, r5, #0
	bl 0x0200d4b8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	ldr	r3, [pc, #176]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	ldr	r6, [pc, #172]
	bl 0x0200d570
	ldr	r1, [r5, #8]
	ldr	r3, [r0, #8]
	subs	r2, r1, r3
	cmp	r2, #0
.L_020031dc:
	blt.n	.L_020031e8
	movs	r1, #160
	lsls	r1, r1, #13
	cmp	r2, r1
	blt.n	.L_020031f2
	b.n	.L_02003268
.L_020031e8:
	movs	r2, #160
	subs	r3, r3, r1
	lsls	r2, r2, #13
	cmp	r3, r2
	bge.n	.L_02003268
.L_020031f2:
	ldr	r3, [r5, #12]
.L_020031f4:
	ldr	r2, [r0, #12]
.L_020031f6:
	ldr	r4, [pc, #136]
	ldr	r1, [pc, #136]
	subs	r3, r3, r2
	adds	r3, r3, r4
	cmp	r3, r1
	bhi.n	.L_02003268
	ldr	r3, [r5, #16]
	ldr	r0, [r0, #16]
	subs	r2, r3, r0
	cmp	r2, #0
	blt.n	.L_02003216
.L_0200320c:
	movs	r3, #160
	lsls	r3, r3, #13
	cmp	r2, r3
	blt.n	.L_02003220
	b.n	.L_02003268
.L_02003216:
	movs	r4, #160
	subs	r3, r0, r3
	lsls	r4, r4, #13
	cmp	r3, r4
	bge.n	.L_02003268
.L_02003220:
	movs	r3, #2
	strh	r3, [r6, #4]
	ldrh	r3, [r6, #10]
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r3, #2
	adds	r2, r7, r0
	str	r5, [r6, #108]
	strh	r3, [r2, #0]
	b.n	.L_02003268
.L_02003234:
	cmp	r3, #1
	bne.n	.L_02003268
	adds	r3, r5, #0
	adds	r3, #91
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L_0200325a
	ldr	r2, [pc, #64]
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r5, #24]
	ldr	r3, [r5, #28]
	adds	r3, r3, r2
	str	r3, [r5, #28]
	.2byte 0xf7ff
	.2byte 0xfd58
	b.n	.L_02003268
.L_0200325a:
	str	r2, [r5, #16]
	str	r2, [r5, #12]
	str	r2, [r5, #8]
.L_02003260:
	str	r2, [r5, #44]
	str	r2, [r5, #40]
	str	r2, [r5, #36]
	str	r2, [r5, #108]
.L_02003268:
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
	bl 0x0200d570
	mov	r8, r0
	adds	r0, r5, #0
	bl 0x0200d570
	adds	r5, r0, #0
	mov	r0, r8
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	ldr	r1, [r0, #8]
	adds	r0, r5, #0
	bl 0x0200d4a8
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl 0x0200d490
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200d500
	adds	r3, r5, #0
	adds	r3, #85
	movs	r6, #0
	strb	r6, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200d480
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
	.2byte 0xaf6d
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
	bl 0x0200d570
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
.L_02003348:
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl 0x0200d408
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
	bl 0x0200d4a8
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
.L_02003388:
	mov	r2, r9
	adds	r6, r6, r3
	b.n	.L_020033a0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xfff40000
	.4byte 0x0200af81
	.2byte 0xc000
	.2byte 0xffff
.L_020033a0:
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
	bl 0x0200d6c0
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
	bne.n	.L_02003404
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200d4c8
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
	bl 0x0200d528
	b.n	.L_0200346c
.L_02003404:
	cmp	r6, #30
	bgt.n	.L_0200341c
	cmp	r6, #30
	bne.n	.L_0200346c
	movs	r0, #136
	bl 0x0200d6c0
	ldr	r0, [r5, #104]
	movs	r1, #2
	bl 0x0200d480
	b.n	.L_0200346c
.L_0200341c:
	cmp	r6, #60
	bgt.n	.L_02003444
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
	bl 0x0200d520
	b.n	.L_0200346c
.L_02003444:
	movs	r1, #1
	ldr	r0, [r5, #104]
	bl 0x0200d480
	movs	r0, #184
	bl 0x0200d6c0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200d4c8
	movs	r3, #0
	str	r0, [r5, #12]
	str	r3, [r5, #104]
	movs	r0, #0
	b.n	.L_02003474
	.2byte 0x0000
	.2byte 0x0001
	.2byte 0x0000
.L_0200346c:
	ldrh	r3, [r7, #0]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #0]
.L_02003474:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r5, #0
	adds	r7, #100
	movs	r3, #0
	ldrsh	r6, [r7, r3]
	cmp	r6, #0
	bne.n	.L_020034c2
	movs	r0, #136
	bl 0x0200d6c0
	ldr	r0, [r5, #104]
	movs	r1, #2
	bl 0x0200d480
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200d4c8
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
	bl 0x0200d528
	b.n	.L_02003510
.L_020034c2:
	cmp	r6, #32
	bgt.n	.L_020034ea
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
	bl 0x0200d520
	b.n	.L_02003510
.L_020034ea:
	movs	r1, #1
	ldr	r0, [r5, #104]
	bl 0x0200d480
	movs	r0, #184
	bl 0x0200d6c0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200d4c8
	movs	r3, #0
	str	r0, [r5, #12]
	str	r3, [r5, #104]
.L_02003508:
	movs	r0, #0
	b.n	.L_02003518
	.2byte 0x0001
	.2byte 0x0000
.L_02003510:
	ldrh	r3, [r7, #0]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #0]
.L_02003518:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200b304
	movs	r3, #0
	str	r3, [r5, #8]
	str	r3, [r5, #12]
	str	r3, [r5, #16]
.L_0200352c:
	str	r3, [r5, #36]
	str	r3, [r5, #40]
	str	r3, [r5, #44]
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_0200357c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200357c
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
.L_0200357c:
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
.L_020035c6:
	mov	fp, r3
	ldr	r3, [pc, #420]
	sub	sp, #4
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
.L_020035d6:
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl 0x0200d570
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02003600
	cmp	r7, #0
	beq.n	.L_02003600
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02003608
.L_02003600:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02003608:
	mov	r3, sl
	bl 0x0200d498
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02003616
	b.n	.L_02003762
.L_02003616:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200d480
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200d490
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d500
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
	bl 0x0200b538
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
	beq.n	.L_02003762
	cmp	r7, #0
	beq.n	.L_02003762
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02003698
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200d5b0
.L_02003698:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020036b8
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x0200b538
.L_020036b8:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020036cc
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_020036cc:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02003712
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020036fa
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200d3c8
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_0200370c
.L_020036fa:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200d3c8
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_0200370c:
	bl 0x0200d3c8
	str	r0, [r6, #52]
.L_02003712:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200372e
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200d480
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200d490
.L_0200372e:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003740
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02003740:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003752
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02003752:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003762
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02003762:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0xf058
	.2byte 0x0200
	push	{r0, r7, lr}
	lsls	r0, r0, #8
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
.L_02003786:
	push	{r6, r7}
	ldr	r4, [pc, #268]
	movs	r1, #1
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	negs	r1, r1
	sub	sp, #4
	cmp	r3, r1
	beq.n	.L_0200388c
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
	bl 0x0200d570
	mov	r1, r8
	ldr	r3, [r0, #8]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	ldr	r4, [sp, #0]
	cmp	r3, r2
	bne.n	.L_020037cc
	ldr	r3, [r0, #16]
	movs	r5, #2
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_020037d4
.L_020037cc:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r4, #12]
.L_020037d4:
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	movs	r2, #1
	negs	r2, r2
	ldr	r1, [pc, #192]
	cmp	r3, r2
	beq.n	.L_0200388c
	movs	r5, #14
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L_0200388c
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
.L_0200380c:
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
	bl 0x0200d4c8
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
	bhi.n	.L_0200388c
	movs	r0, #15
	negs	r0, r0
	cmp	r2, r0
	blt.n	.L_0200388c
	cmp	r2, #239
	bgt.n	.L_0200388c
	movs	r3, #128
.L_02003862:
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
	bl 0x0200d440
.L_0200388c:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200f0a8
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
.L_0200392e:
	asrs	r3, r3, #20
	str	r3, [sp, #28]
	mov	r3, ip
	adds	r3, #244
	ldr	r3, [r3, #0]
	subs	r3, r3, r0
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
.L_02003954:
	movs	r0, #0
	str	r3, [sp, #20]
	str	r5, [sp, #36]
	str	r0, [sp, #12]
	cmp	r0, r3
	bge.n	.L_02003a30
.L_02003960:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #36]
	ldr	r5, [sp, #24]
	lsls	r3, r1, #9
	adds	r2, r2, r3
	movs	r3, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, r5
	bge.n	.L_02003a24
.L_02003974:
	mov	r0, fp
	ldrb	r5, [r0, #2]
	cmp	r5, #0
	beq.n	.L_02003a14
	ldr	r1, [sp, #44]
	cmp	r5, r1
	bcc.n	.L_02003a14
	adds	r1, #1
	mov	sl, r1
	cmp	r5, sl
	bhi.n	.L_02003a14
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
	bl 0x0200d450
	cmp	r0, #0
	bne.n	.L_020039c8
	cmp	r5, sl
	bne.n	.L_02003a06
.L_020039bc:
	mov	r3, r9
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	bl 0x0200d458
	b.n	.L_02003a06
.L_020039c8:
	mov	r1, r9
	movs	r5, #4
	ldrsh	r0, [r1, r5]
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02003a06
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
	bl 0x0200d4e0
.L_02003a06:
	mov	r0, r8
	ldrh	r3, [r0, #10]
	mov	r1, r8
	adds	r3, #1
	strh	r3, [r1, #10]
	movs	r5, #8
	add	r9, r5
.L_02003a14:
	ldr	r2, [sp, #16]
	ldr	r5, [sp, #24]
	adds	r2, #1
	movs	r3, #4
	str	r2, [sp, #16]
	add	fp, r3
	cmp	r2, r5
	blt.n	.L_02003974
.L_02003a24:
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #12]
	cmp	r0, r1
	blt.n	.L_02003960
.L_02003a30:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200d450
	cmp	r0, #0
	beq.n	.L_02003a88
	ldr	r3, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d570
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
	bge.n	.L_02003a88
.L_02003a62:
	mov	r0, r9
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	cmp	r3, r4
	bne.n	.L_02003a78
	movs	r5, #2
	ldrsh	r3, [r0, r5]
	cmp	r3, r1
	bne.n	.L_02003a78
	mov	r0, r8
	strh	r2, [r0, #12]
.L_02003a78:
	movs	r3, #8
	mov	r0, r8
	add	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	adds	r2, #1
	cmp	r2, r3
	blt.n	.L_02003a62
.L_02003a88:
	movs	r0, #128
	lsls	r0, r0, #1
	bl 0x0200d410
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r2, #63
.L_02003a96:
	ldr	r3, [pc, #80]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bge.n	.L_02003a96
	bl 0x0200d438
	mov	r1, r8
	strh	r0, [r1, #16]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #1
	asrs	r0, r0, #16
	bl 0x0200d430
	adds	r0, r5, #0
	bl 0x0200d418
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200d3e0
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
	.4byte 0x0200f0a8
	.4byte 0x03000258
	.4byte 0x02000240
	.4byte 0x11111111
	.2byte 0xb781
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r1, [pc, #116]
	movs	r2, #133
	mov	r8, r1
	lsls	r2, r2, #2
	add	r8, r2
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200d570
	mov	r1, r8
	ldr	r5, [r0, #8]
	ldr	r6, [r0, #16]
	mov	sl, r0
	movs	r2, #128
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200d578
	asrs	r5, r5, #20
	mov	r2, r8
	asrs	r6, r6, #20
	ldr	r0, [r2, #0]
	lsls	r1, r5, #4
	lsls	r2, r6, #4
	adds	r1, #8
	adds	r2, #8
	bl 0x0200d580
	movs	r0, #1
	bl 0x0200d3d8
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
	bl 0x0200d4a8
	movs	r0, #4
	bl 0x0200d558
	bl 0x0200d5e8
	ldr	r2, [pc, #20]
	ldr	r3, [r0, #12]
	adds	r3, r3, r2
	str	r3, [r0, #12]
	pop	{r3, r5}
.L_02003b68:
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
	adds	r3, r3, r1
	mov	r8, r0
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200d570
	mov	r2, r8
	ldrh	r1, [r2, #6]
	movs	r2, #64
	ldr	r6, [r0, #8]
	adds	r3, r2, #0
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
	bl 0x0200baf0
	movs	r0, #161
	bl 0x0200d6c0
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
	bl 0x0200d4e0
	movs	r0, #12
	bl 0x0200d558
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
	bl 0x0200d570
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	movs	r2, #64
.L_02003c0a:
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
	bl 0x0200baf0
	movs	r0, #229
.L_02003c30:
	bl 0x0200d6c0
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
	bl 0x0200d4e0
	movs	r0, #12
	bl 0x0200d558
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	bl 0x0200d570
	movs	r1, #0
	bl 0x0200d500
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
	b.n	.L_02003c9c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00008000
	.2byte 0x0240
	.2byte 0x0200
.L_02003c9c:
	movs	r3, #1
	mov	r1, r8
	strh	r3, [r1, #14]
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl 0x0200d5a0
	movs	r0, #16
	bl 0x0200d558
.L_02003cb0:
	cmp	r7, #5
	bne.n	.L_02003cba
	movs	r0, #204
	bl 0x0200d6c0
.L_02003cba:
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
	bl 0x0200d3d8
	cmp	r7, #39
	ble.n	.L_02003cb0
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d570
	movs	r3, #0
	adds	r0, #84
	strb	r3, [r0, #0]
.L_02003cf2:
	mov	r1, r8
	strh	r3, [r1, #14]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200d620
	bl 0x0200d628
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
	bl 0x0200d3c8
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02003d58
	adds	r3, #15
.L_02003d58:
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
	bl 0x0200d570
	adds	r7, r0, #0
	bl 0x0200d560
	movs	r0, #0
	bl 0x0200d648
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d5d8
	bl 0x0200d4b0
	movs	r0, #1
	bl 0x0200d3d8
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
.L_02003de6:
	str	r2, [r3, #0]
	bl 0x0200d618
	bl 0x0200d628
	movs	r0, #204
	bl 0x0200d6c0
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200d558
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
.L_02003e1a:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200d400
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200d3f8
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200d3e8
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #188]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200d3e8
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
	bl 0x0200b5b8
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_02003e1a
	movs	r0, #188
	bl 0x0200d6c0
	ldr	r5, [pc, #112]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200d5c0
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200d5a0
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200d510
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200d510
	bl 0x0200d518
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200d5c0
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200d558
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200d5a0
	bl 0x0200d568
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200bd29
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
	bl 0x0200d570
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
	bge.n	.L_02003fb4
.L_02003f4c:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, r8
	bne.n	.L_02003fa8
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	cmp	r3, sl
	bne.n	.L_02003fa8
	movs	r2, #4
.L_02003f5e:
	ldrsh	r0, [r5, r2]
	bl 0x0200d450
	cmp	r0, #0
	bne.n	.L_02003f7c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x0200bb78
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	bl 0x0200d458
	strh	r7, [r6, #12]
	b.n	.L_02003fb4
.L_02003f7c:
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r7, r3
	beq.n	.L_02003fb4
	adds	r0, r6, #0
	adds	r1, r5, #0
	strh	r7, [r6, #12]
	bl 0x0200bbe8
	movs	r2, #2
	ldrsh	r0, [r6, r2]
	mov	r1, r8
	bl 0x0200d470
	movs	r3, #2
	ldrsh	r0, [r6, r3]
	mov	r1, sl
	adds	r0, #8
	bl 0x0200d470
	movs	r0, #1
	b.n	.L_02003fb6
.L_02003fa8:
	lsls	r3, r2, #16
	adds	r7, #1
	asrs	r3, r3, #16
	adds	r5, #8
	cmp	r7, r3
	blt.n	.L_02003f4c
.L_02003fb4:
	movs	r0, #0
.L_02003fb6:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xf0a8
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
	bl 0x0200d570
	movs	r3, #192
	ldr	r5, [pc, #140]
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	mov	sl, r3
	bl 0x0200d468
	adds	r7, r0, #0
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	adds	r0, #8
	bl 0x0200d468
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_02004016
	cmp	r0, #0
	beq.n	.L_0200406a
.L_02004016:
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #0
	bl 0x0200d470
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	movs	r1, #0
	adds	r0, #8
	bl 0x0200d470
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
	bl 0x0200d4b0
	bl 0x0200bd80
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	str	r2, [r3, #0]
.L_0200406a:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0xf0a8
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #100
	movs	r0, #0
	ldrsh	r1, [r2, r0]
	ldrh	r3, [r2, #0]
	cmp	r1, #0
	beq.n	.L_02004098
	subs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020040fe
.L_02004098:
	adds	r3, r5, #0
	adds	r3, #90
	movs	r0, #131
	strb	r1, [r3, #0]
	lsls	r0, r0, #1
	bl 0x0200d450
	movs	r3, #1
	negs	r3, r3
	cmp	r0, #0
	bne.n	.L_020040be
	ldr	r3, [pc, #80]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #76]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
.L_020040be:
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_020040d0
	adds	r0, r5, #0
	movs	r1, #9
	bl 0x0200d480
	b.n	.L_020040fe
.L_020040d0:
	ldrh	r1, [r5, #6]
	movs	r2, #128
	subs	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_020040e2
	adds	r3, r2, #0
.L_020040e2:
	ldr	r2, [pc, #36]
	cmp	r3, r2
	bge.n	.L_020040ea
	adds	r3, r2, #0
.L_020040ea:
	adds	r3, r1, r3
	adds	r0, r5, #0
	movs	r1, #2
	strh	r3, [r5, #6]
	bl 0x0200d480
	adds	r0, r5, #0
	movs	r1, #48
	bl 0x0200d488
.L_020040fe:
	pop	{r5, pc}
	.4byte 0x03001150
	.4byte 0x0200def8
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
	beq.n	.L_0200413c
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200d458
	bl 0x0200d608
	bl 0x0200d640
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200d460
.L_0200413c:
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
	bl 0x0200d630
	adds	r7, r0, #0
.L_02004160:
	bl 0x0200c10c
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
	bl 0x0200d4d0
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
	bge.n	.L_02004218
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
	bne.n	.L_02004244
	b.n	.L_020043da
	.4byte 0x02000240
	.4byte 0xfff00000
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200df38
	.2byte 0x0000
	.2byte 0xffff
.L_02004218:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl 0x0200d3f0
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
	b.n	.L_02004244
	.2byte 0xc000
	.2byte 0xffff
.L_02004244:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200d408
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl 0x0200d4d0
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_020042c6
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	bl 0x0200d4c8
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_020042c6
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
	bl 0x0200d4b8
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200d480
	adds	r0, r7, #0
	movs	r1, #48
	bl 0x0200d488
	adds	r0, r7, #0
	bl 0x0200d4c0
	ldr	r3, [pc, #292]
	str	r3, [r7, #108]
	b.n	.L_02004370
.L_020042c6:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_020043bc
.L_020042da:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl 0x0200d4c8
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02004390
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
.L_02004308:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02004332
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02004332
	cmp	r5, r7
	beq.n	.L_02004332
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
.L_02004324:
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl 0x0200d538
	cmp	r0, #0
	bge.n	.L_02004390
.L_02004332:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02004308
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
	bl 0x0200d4b8
	adds	r0, r7, #0
	bl 0x0200d4c0
.L_0200436a:
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_020043b6
.L_02004370:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200d408
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200d4d0
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_020042da
.L_02004390:
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
	bl 0x0200d4b8
	adds	r0, r7, #0
	bl 0x0200d4c0
	movs	r0, #2
	bl 0x0200d3d8
.L_020043b4:
	b.n	.L_02004160
.L_020043b6:
	movs	r0, #10
	bl 0x0200d3d8
.L_020043bc:
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
	bl 0x0200d480
.L_020043da:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0xc081
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
	bl 0x0200d630
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	adds	r7, r0, #0
	strh	r3, [r2, #0]
.L_02004412:
	bl 0x0200c10c
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
	b.n	.L_02004458
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x0200f4c8
	.2byte 0x0000
	.2byte 0xfff0
.L_02004458:
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
	bl 0x0200d4d0
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
	bge.n	.L_020044d4
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
	bne.n	.L_02004500
	b.n	.L_020046ca
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200df38
	.2byte 0x0000
	.2byte 0xffff
.L_020044d4:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl 0x0200d3f0
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
	b.n	.L_02004500
	.2byte 0xc000
	.2byte 0xffff
.L_02004500:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200d408
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl 0x0200d4d0
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_0200457a
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	bl 0x0200d4c8
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
.L_02004532:
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_0200457a
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
	bl 0x0200d4b8
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200d480
	adds	r0, r7, #0
	movs	r1, #48
	bl 0x0200d488
	movs	r5, #0
	b.n	.L_020045a2
.L_0200457a:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_020046ca
.L_0200458e:
	ldr	r3, [pc, #360]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_0200459a
	b.n	.L_020046ca
.L_0200459a:
	movs	r0, #1
	bl 0x0200d3d8
	adds	r5, #1
.L_020045a2:
	cmp	r5, #179
	bgt.n	.L_020045b0
	adds	r0, r7, #0
	bl 0x0200d530
	cmp	r0, #0
	beq.n	.L_0200458e
.L_020045b0:
	ldr	r3, [pc, #328]
	str	r3, [r7, #108]
	b.n	.L_0200467e
.L_020045b6:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl 0x0200d4c8
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_0200469e
	ldr	r3, [pc, #296]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_020046ca
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
.L_020045ee:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02004618
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02004618
	cmp	r5, r7
	beq.n	.L_02004618
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl 0x0200d538
	cmp	r0, #0
	bge.n	.L_0200469e
.L_02004618:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_020045ee
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
	bl 0x0200d4b8
	b.n	.L_02004656
.L_0200464e:
	movs	r0, #1
	bl 0x0200d3d8
	adds	r5, #1
.L_02004656:
	cmp	r5, #179
	bgt.n	.L_0200466e
	adds	r0, r7, #0
	bl 0x0200d530
	cmp	r0, #0
	bne.n	.L_0200466e
	ldr	r3, [pc, #144]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_0200464e
.L_0200466e:
	ldr	r3, [pc, #136]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_020046ca
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_020046c4
.L_0200467e:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200d408
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200d4d0
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_020045b6
.L_0200469e:
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
	bl 0x0200d4b8
	adds	r0, r7, #0
	bl 0x0200d4c0
	movs	r0, #2
	bl 0x0200d3d8
	b.n	.L_02004412
.L_020046c4:
	movs	r0, #10
	bl 0x0200d3d8
.L_020046ca:
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
	bl 0x0200d480
.L_020046e8:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
.L_020046f4:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200f4c8
	.4byte 0x0200c081
	.4byte 0x80184b01
	.4byte 0x00004770
	.2byte 0xf4c8
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
	bl 0x0200d630
	adds	r5, r0, #0
	ldrh	r3, [r5, #6]
	movs	r2, #128
.L_02004730:
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #60]
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	str	r3, [sp, #16]
.L_0200473e:
	bl 0x0200c10c
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
.L_02004758:
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	ldr	r2, [pc, #28]
	ldr	r3, [r5, #8]
	movs	r1, #128
	lsls	r1, r1, #12
	ands	r3, r2
	mov	r9, r1
	add	r6, sp, #20
	add	r3, r9
	str	r3, [r6, #0]
	mov	r8, r3
	b.n	.L_02004780
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
.L_02004780:
	.2byte 0x68eb
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	ands	r3, r2
	adds	r7, r3, r1
	mov	r2, r8
	str	r7, [r6, #8]
	str	r2, [sp, #8]
	str	r7, [sp, #4]
.L_02004792:
	movs	r3, #34
	adds	r3, r3, r5
	ldrb	r0, [r3, #0]
	adds	r1, r2, #0
	adds	r2, r7, #0
	mov	fp, r3
	bl 0x0200d4d0
	str	r0, [sp, #12]
	movs	r0, #128
	ldr	r1, [sp, #16]
	lsls	r0, r0, #13
	adds	r2, r6, #0
	bl 0x0200d408
	mov	r1, fp
	ldrb	r0, [r1, #0]
	ldr	r2, [r6, #8]
	ldr	r1, [r6, #0]
	bl 0x0200d4d0
	mov	sl, r0
	cmp	r0, #255
	beq.n	.L_02004814
	mov	r2, fp
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200d4c8
	ldr	r3, [r5, #12]
	subs	r0, r0, r3
	cmp	r0, r9
	bgt.n	.L_02004814
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
	bl 0x0200d480
	adds	r0, r5, #0
	movs	r1, #48
	bl 0x0200d488
	ldr	r3, [pc, #8]
	str	r3, [r5, #108]
	b.n	.L_020048be
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0xc081
	.2byte 0x0200
.L_02004814:
	add	r1, sp, #16
	ldrh	r1, [r1, #0]
	movs	r3, #0
	mov	r2, r8
	strh	r1, [r5, #6]
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	str	r2, [r5, #8]
	str	r7, [r5, #16]
	b.n	.L_0200490a
.L_02004828:
	mov	r3, fp
	ldrb	r0, [r3, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl 0x0200d4c8
	ldr	r3, [r5, #12]
	movs	r1, #128
	subs	r0, r0, r3
	lsls	r1, r1, #12
	cmp	r0, r1
	bgt.n	.L_020048de
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
.L_02004856:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_02004880
	mov	r1, r8
	ldrb	r2, [r1, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02004880
	cmp	r6, r5
	beq.n	.L_02004880
	ldrh	r3, [r6, #32]
	adds	r0, r6, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #20
	bl 0x0200d538
	cmp	r0, #0
	bge.n	.L_020048de
.L_02004880:
	movs	r2, #1
	add	r9, r2
	movs	r3, #128
	mov	r1, r9
	add	r8, r3
	adds	r6, #128
	cmp	r1, #63
	ble.n	.L_02004856
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
	bl 0x0200d4b8
	adds	r0, r5, #0
	bl 0x0200d4c0
	ldr	r1, [sp, #12]
	cmp	sl, r1
	bne.n	.L_02004904
.L_020048be:
	movs	r0, #128
	ldr	r1, [sp, #16]
	add	r2, sp, #20
	lsls	r0, r0, #13
	bl 0x0200d408
	mov	r2, fp
	add	r7, sp, #20
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl 0x0200d4d0
	mov	sl, r0
	cmp	r0, #255
	bne.n	.L_02004828
.L_020048de:
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
.L_020048f2:
	bl 0x0200d4b8
	adds	r0, r5, #0
	bl 0x0200d4c0
	movs	r0, #2
	bl 0x0200d3d8
	b.n	.L_0200473e
.L_02004904:
	movs	r0, #10
	bl 0x0200d3d8
.L_0200490a:
	movs	r3, #0
	str	r3, [r5, #108]
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #52]
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200d480
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #8
	mov	r8, r3
	bl 0x0200d570
	ldr	r3, [r0, #80]
	ldr	r5, [pc, #232]
	ldr	r3, [r3, #40]
	movs	r1, #0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r4, r3, #16
	ldrh	r3, [r5, r1]
	lsrs	r2, r4, #16
	cmp	r2, r3
	beq.n	.L_0200497e
.L_02004964:
	movs	r2, #128
	lsls	r3, r1, #16
	lsls	r2, r2, #9
	adds	r3, r3, r2
	lsrs	r2, r3, #16
	asrs	r1, r3, #16
	cmp	r2, #5
	bhi.n	.L_0200497e
	lsls	r3, r2, #1
	ldrh	r3, [r5, r3]
	lsrs	r2, r4, #16
	cmp	r2, r3
	bne.n	.L_02004964
.L_0200497e:
	lsls	r3, r1, #16
	lsrs	r2, r3, #16
	cmp	r2, #6
	bne.n	.L_0200498a
	movs	r0, #0
	b.n	.L_02004a32
.L_0200498a:
	ldr	r6, [pc, #180]
	lsls	r2, r2, #2
	ldrsb	r4, [r6, r2]
	adds	r1, r4, #0
	cmp	r4, #0
	bge.n	.L_02004998
	negs	r1, r4
.L_02004998:
	adds	r3, r2, #2
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bge.n	.L_020049a2
	negs	r3, r3
.L_020049a2:
	adds	r3, r1, r3
	asrs	r7, r3, #4
	adds	r3, r2, #1
	ldrsb	r1, [r6, r3]
	adds	r5, r1, #0
	cmp	r1, #0
	bge.n	.L_020049b2
	negs	r5, r1
.L_020049b2:
	adds	r3, r2, #3
	ldrsb	r2, [r6, r3]
	cmp	r2, #0
	bge.n	.L_020049bc
	negs	r2, r2
.L_020049bc:
	adds	r5, r5, r2
	mov	sl, r5
	ldr	r6, [r0, #8]
	mov	r3, sl
	ldr	r5, [r0, #16]
	asrs	r3, r3, #4
	mov	sl, r3
	lsls	r3, r4, #16
	adds	r6, r6, r3
	lsls	r3, r1, #16
	adds	r5, r5, r3
	movs	r3, #164
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	asrs	r6, r6, #20
	asrs	r1, r3, #20
	movs	r3, #166
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	lsls	r2, r1, #16
	asrs	r3, r3, #20
	lsls	r3, r3, #16
	asrs	r5, r5, #20
	lsrs	r2, r2, #16
	lsrs	r3, r3, #16
	adds	r2, r6, r2
	adds	r3, r5, r3
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	mov	r3, sl
	bl 0x0200d4e8
	movs	r3, #255
	mov	r2, sl
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r8, r3
	movs	r0, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	adds	r3, r7, #0
	bl 0x0200ca44
	mov	r2, sl
	mov	r3, r8
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #2
	adds	r1, r6, #0
	adds	r2, r5, #0
	adds	r3, r7, #0
	bl 0x0200ca44
	movs	r0, #1
.L_02004a32:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0200df58
	.2byte 0xdf64
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r3, #0
	ldr	r3, [sp, #12]
	lsls	r2, r2, #7
	mov	ip, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r0, r0, #1
	lsls	r3, r3, #3
	adds	r3, r3, r0
	ldr	r0, [r4, r3]
	adds	r1, r1, r2
	lsls	r1, r1, #2
	adds	r0, r0, r1
	movs	r1, #0
	ldr	r6, [sp, #16]
	cmp	r1, ip
	bcs.n	.L_02004a8a
.L_02004a70:
	lsls	r3, r1, #9
	movs	r2, #0
	adds	r3, r0, r3
	cmp	r2, r5
	bcs.n	.L_02004a84
.L_02004a7a:
	adds	r2, #1
	strb	r6, [r3, #2]
	adds	r3, #4
	cmp	r2, r5
	bcc.n	.L_02004a7a
.L_02004a84:
	adds	r1, #1
	cmp	r1, ip
	bcc.n	.L_02004a70
.L_02004a8a:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	adds	r5, r6, #0
	sub	sp, #40
	adds	r1, r6, #0
	adds	r5, #12
	add	r0, sp, #24
	adds	r1, #16
	adds	r2, r5, #0
	bl 0x0200cbf4
	adds	r4, r0, #0
	cmp	r4, #0
	bne.n	.L_02004ab6
	b.n	.L_02004bd6
.L_02004ab6:
	ldr	r5, [r5, #0]
	ldr	r0, [pc, #300]
	str	r5, [sp, #20]
	lsls	r1, r5, #2
	ldrsb	r2, [r0, r1]
	cmp	r2, #0
	bge.n	.L_02004ac6
	negs	r2, r2
.L_02004ac6:
	adds	r3, r1, #2
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	bge.n	.L_02004ad0
	negs	r3, r3
.L_02004ad0:
	adds	r3, r2, r3
	asrs	r3, r3, #4
	str	r3, [sp, #16]
	adds	r3, r1, #1
	ldrsb	r2, [r0, r3]
	cmp	r2, #0
	bge.n	.L_02004ae0
	negs	r2, r2
.L_02004ae0:
	adds	r3, r1, #3
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	bge.n	.L_02004aea
	negs	r3, r3
.L_02004aea:
	adds	r3, r2, r3
	asrs	r3, r3, #4
	str	r3, [sp, #12]
	ldr	r3, [sp, #24]
	ldr	r2, [pc, #248]
	ldr	r1, [pc, #248]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	mov	r9, r1
	mov	r2, r9
	ands	r2, r3
	lsls	r3, r3, #16
	mov	sl, r3
	movs	r3, #0
	str	r3, [r6, #20]
	mov	fp, r3
	adds	r3, r4, #0
	adds	r3, #34
	str	r3, [sp, #8]
	ldr	r1, [sp, #8]
	movs	r3, #2
	strb	r3, [r1, #0]
	mov	r9, r2
	ldr	r3, [r4, #8]
	add	r3, r9
	str	r3, [r6, #0]
	ldr	r3, [r4, #16]
	add	r3, sl
	str	r3, [r6, #8]
	ldr	r3, [r4, #12]
	str	r3, [sp, #32]
.L_02004b28:
	ldr	r3, [sp, #20]
	ldr	r2, [pc, #188]
	lsls	r3, r3, #2
	str	r3, [sp, #4]
	adds	r3, #1
	ldrsb	r2, [r2, r3]
	ldr	r3, [r6, #8]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	movs	r1, #0
	mov	r8, r1
	str	r3, [sp, #36]
	cmp	r8, r2
	bge.n	.L_02004b96
.L_02004b46:
	ldr	r3, [pc, #160]
	ldr	r1, [sp, #4]
	add	r5, sp, #28
	ldrsb	r2, [r3, r1]
	ldr	r3, [r6, #0]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [sp, #16]
	movs	r7, #0
	cmp	r7, r2
	bge.n	.L_02004b80
.L_02004b5e:
	adds	r0, r4, #0
	add	r1, sp, #28
	str	r4, [sp, #0]
	bl 0x0200d4f8
	ldr	r4, [sp, #0]
	cmp	r0, #2
	beq.n	.L_02004ba8
	ldr	r3, [r5, #0]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r5, #0]
	ldr	r2, [sp, #16]
	adds	r7, #1
	cmp	r7, r2
	blt.n	.L_02004b5e
.L_02004b80:
	add	r2, sp, #28
	ldr	r3, [r2, #8]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r2, #8]
	ldr	r3, [sp, #12]
	movs	r2, #1
	add	r8, r2
	cmp	r8, r3
	blt.n	.L_02004b46
.L_02004b96:
	ldr	r3, [r6, #0]
	movs	r1, #1
	add	r3, r9
	str	r3, [r6, #0]
	ldr	r3, [r6, #8]
	add	fp, r1
	add	r3, sl
	str	r3, [r6, #8]
	b.n	.L_02004b28
.L_02004ba8:
	ldr	r2, [sp, #8]
	movs	r3, #0
	strb	r3, [r2, #0]
	mov	r3, fp
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_02004bd8
	mov	r1, r9
	ldr	r3, [r4, #8]
	mov	r2, fp
	muls	r2, r1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	movs	r0, #1
	ldr	r3, [r4, #12]
	str	r3, [r6, #4]
	mov	r3, sl
	mov	r2, fp
	muls	r2, r3
	ldr	r3, [r4, #16]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	b.n	.L_02004bd8
.L_02004bd6:
	movs	r0, #0
.L_02004bd8:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200df64
	.4byte 0x0200df7c
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	ldr	r3, [pc, #236]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl 0x0200d570
	adds	r7, r0, #0
	ldrh	r3, [r7, #6]
	ldr	r1, [sp, #8]
	lsrs	r3, r3, #12
	str	r3, [r1, #0]
	movs	r2, #8
	adds	r5, #52
	mov	fp, r2
	mov	lr, r5
.L_02004c30:
	mov	r3, lr
	ldr	r6, [r3, #0]
	movs	r5, #0
.L_02004c36:
	ldr	r3, [r6, #80]
	ldr	r2, [pc, #200]
	ldr	r3, [r3, #40]
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	cmp	r1, r3
	bne.n	.L_02004cda
	ldr	r0, [sp, #8]
	movs	r2, #10
	ldrsh	r1, [r7, r2]
	ldr	r3, [r0, #0]
	ldr	r2, [pc, #180]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	ldr	r4, [pc, #180]
	asrs	r2, r3, #16
	adds	r1, r1, r2
	asrs	r1, r1, #4
	mov	r9, r1
	movs	r1, #18
	ldrsh	r2, [r7, r1]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	adds	r2, r2, r3
	asrs	r2, r2, #4
	mov	r8, r2
	movs	r2, #10
	ldrsh	r0, [r6, r2]
	lsls	r2, r5, #2
	ldrsb	r3, [r4, r2]
	adds	r3, r0, r3
	asrs	r3, r3, #4
	mov	sl, r3
	movs	r3, #18
	ldrsh	r1, [r6, r3]
	adds	r3, r2, #1
	ldrsb	r3, [r4, r3]
	adds	r3, r1, r3
	asrs	r3, r3, #4
	mov	ip, r3
	adds	r3, r2, #2
	ldrsb	r3, [r4, r3]
	adds	r2, #3
	adds	r0, r0, r3
	ldrsb	r3, [r4, r2]
	asrs	r0, r0, #4
	adds	r1, r1, r3
	asrs	r1, r1, #4
	cmp	sl, r9
	bgt.n	.L_02004cda
	cmp	r9, r0
	bge.n	.L_02004cda
	cmp	ip, r8
	bgt.n	.L_02004cda
	cmp	r8, r1
	bge.n	.L_02004cda
	ldr	r0, [sp, #0]
	movs	r3, #1
	ands	r3, r5
	str	r5, [r0, #0]
	cmp	r3, #0
	beq.n	.L_02004cc8
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	sl, r3
	beq.n	.L_02004cda
	ldr	r2, [sp, #4]
	mov	r1, fp
	str	r1, [r2, #0]
	adds	r0, r6, #0
	b.n	.L_02004cf0
.L_02004cc8:
	ldr	r3, [r7, #16]
	asrs	r3, r3, #20
	cmp	ip, r3
	beq.n	.L_02004cda
	ldr	r0, [sp, #4]
	mov	r3, fp
	str	r3, [r0, #0]
	adds	r0, r6, #0
	b.n	.L_02004cf0
.L_02004cda:
	adds	r5, #1
	cmp	r5, #5
	bls.n	.L_02004c36
	movs	r2, #1
	add	fp, r2
	movs	r1, #4
	mov	r3, fp
	add	lr, r1
	cmp	r3, #63
	bls.n	.L_02004c30
	movs	r0, #0
.L_02004cf0:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200df58
	.4byte 0x0200df7c
	.4byte 0x0200df64
	.2byte 0xb084
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r0, [sp, #88]
	str	r1, [sp, #92]
	str	r2, [sp, #96]
	str	r3, [sp, #100]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r0, #133
	str	r3, [sp, #28]
	ldr	r3, [pc, #632]
	lsls	r0, r0, #2
	adds	r0, r0, r3
	mov	sl, r0
	ldr	r0, [r0, #0]
	bl 0x0200d570
	mov	r8, r0
	ldr	r0, [sp, #104]
	bl 0x0200d570
	mov	r3, r8
	ldr	r3, [r3, #48]
	mov	r4, r8
	str	r3, [sp, #20]
	adds	r6, r0, #0
	ldr	r4, [r4, #52]
	mov	r0, sp
	adds	r0, #32
	str	r0, [sp, #12]
	str	r4, [sp, #16]
	ldr	r2, [sp, #100]
	ldr	r3, [r6, #8]
	movs	r1, #0
	str	r3, [r0, #0]
	mov	r9, r1
	ldr	r3, [r6, #16]
	mov	r1, sp
	adds	r1, #44
	str	r3, [r0, #8]
	ldr	r5, [pc, #576]
	str	r1, [sp, #8]
	lsls	r7, r2, #2
	ldrsb	r1, [r5, r7]
	ldr	r3, [r6, #8]
	lsls	r2, r1, #16
	adds	r3, r3, r2
	ldr	r2, [sp, #8]
	asrs	r3, r3, #20
	str	r3, [r2, #0]
	mov	lr, r3
	adds	r3, r7, #1
	ldrsb	r4, [r5, r3]
	ldr	r3, [r6, #16]
	ldr	r0, [sp, #8]
	lsls	r2, r4, #16
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [r0, #8]
	adds	r0, r1, #0
	mov	ip, r3
	cmp	r0, #0
	bge.n	.L_02004da0
	negs	r0, r0
.L_02004da0:
	adds	r3, r7, #2
	ldrsb	r1, [r5, r3]
	cmp	r1, #0
	bge.n	.L_02004daa
	negs	r1, r1
.L_02004daa:
	adds	r3, r0, r1
	asrs	r3, r3, #4
	adds	r1, r4, #0
	str	r3, [sp, #24]
	cmp	r1, #0
	bge.n	.L_02004db8
	negs	r1, r1
.L_02004db8:
	adds	r3, r7, #3
	ldrsb	r2, [r5, r3]
	cmp	r2, #0
	bge.n	.L_02004dc2
	negs	r2, r2
.L_02004dc2:
	adds	r3, r1, r2
	asrs	r3, r3, #4
	str	r3, [sp, #0]
	mov	fp, r3
	movs	r3, #0
	str	r3, [sp, #4]
	mov	r1, lr
	mov	r2, ip
	ldr	r3, [sp, #24]
	movs	r0, #0
	bl 0x0200ca44
	mov	r1, sl
	movs	r2, #200
	ldr	r0, [r1, #0]
	lsls	r2, r2, #5
	movs	r1, #128
	lsls	r1, r1, #8
	adds	r2, #153
	bl 0x0200d578
	mov	r2, sl
	ldr	r0, [r2, #0]
	movs	r1, #8
	bl 0x0200d5a0
	movs	r0, #15
	bl 0x0200d3d8
	ldr	r4, [sp, #12]
	ldr	r1, [sp, #88]
	ldr	r3, [r4, #0]
	ldr	r2, [sp, #96]
	subs	r1, r1, r3
	ldr	r3, [r4, #8]
	asrs	r1, r1, #17
	subs	r2, r2, r3
	mov	r3, sl
	asrs	r2, r2, #17
	ldr	r0, [r3, #0]
	bl 0x0200d588
	mov	r4, sl
	ldr	r0, [r4, #0]
	bl 0x0200d570
	ldr	r3, [pc, #408]
	str	r3, [r0, #108]
	movs	r0, #4
	bl 0x0200d3d8
	movs	r1, #2
	adds	r0, r6, #0
	bl 0x0200d480
	movs	r0, #239
	bl 0x0200d6c0
	movs	r2, #200
	movs	r1, #128
	lsls	r2, r2, #5
	ldr	r0, [sp, #104]
	lsls	r1, r1, #8
	adds	r2, #153
	bl 0x0200d578
	adds	r0, r6, #0
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #92]
	ldr	r3, [sp, #96]
	bl 0x0200d4b8
	ldr	r3, [pc, #348]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r5, r3, r0
	ldr	r0, [r5, #0]
	bl 0x0200d590
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200d5a0
	movs	r1, #152
	movs	r2, #200
	lsls	r1, r1, #7
	lsls	r2, r2, #5
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #153
	bl 0x0200d578
	ldr	r2, [pc, #320]
	mov	r1, r9
	lsls	r3, r1, #2
	ldr	r2, [r2, r3]
	ldr	r0, [r5, #0]
	lsls	r2, r2, #16
	asrs	r1, r2, #31
	asrs	r2, r2, #17
	bl 0x0200d588
	ldr	r3, [sp, #108]
	cmp	r3, #0
	beq.n	0x0200ce98
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6828
	bl 0x0200d590
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200d5a0
	mov	r3, r8
	movs	r2, #0
	str	r2, [r3, #108]
	ldr	r4, [sp, #20]
.L_02004eae:
	movs	r5, #255
	str	r4, [r3, #48]
	ldr	r0, [sp, #16]
	str	r0, [r3, #52]
	adds	r0, r6, #0
	bl 0x0200d4c0
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x0200d6c0
	movs	r0, #213
	bl 0x0200d6c0
	ldr	r2, [r6, #12]
	ldr	r1, [sp, #88]
	ldr	r3, [sp, #96]
	adds	r0, r6, #0
	bl 0x0200d4a8
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200d480
	ldr	r1, [pc, #212]
	ldr	r0, [sp, #88]
	ldrsb	r3, [r1, r7]
	adds	r2, r7, #1
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldrsb	r3, [r1, r2]
	mov	sl, r1
	ldr	r1, [sp, #96]
	lsls	r3, r3, #16
	adds	r1, r1, r3
	ldr	r4, [sp, #28]
	asrs	r0, r0, #20
	asrs	r1, r1, #20
	str	r0, [sp, #88]
	str	r1, [sp, #96]
	mov	r9, r2
	movs	r2, #164
	lsls	r2, r2, #1
	adds	r3, r4, r2
	ldr	r3, [r3, #0]
	adds	r2, #4
	asrs	r3, r3, #20
	mov	r8, r3
	adds	r3, r4, r2
	ldr	r6, [r3, #0]
	mov	r4, r8
	asrs	r6, r6, #20
	adds	r3, r4, r0
	adds	r2, r6, r1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	mov	r3, fp
	ldr	r2, [sp, #24]
	bl 0x0200d4e8
	mov	r0, fp
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #96]
	str	r0, [sp, #0]
	ldr	r3, [sp, #24]
	movs	r0, #0
	str	r5, [sp, #4]
	bl 0x0200ca44
	mov	r3, fp
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #96]
	str	r3, [sp, #0]
	movs	r0, #2
	ldr	r3, [sp, #24]
	str	r5, [sp, #4]
	bl 0x0200ca44
	ldr	r0, [sp, #12]
	mov	r4, sl
	ldrsb	r3, [r4, r7]
	ldr	r1, [r0, #0]
	ldr	r2, [sp, #8]
	lsls	r3, r3, #16
	adds	r1, r1, r3
	asrs	r1, r1, #20
	str	r1, [r2, #0]
	mov	r3, r9
	ldrsb	r2, [r4, r3]
	ldr	r3, [r0, #8]
	ldr	r4, [sp, #8]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [r4, #8]
	add	r8, r1
	adds	r6, r6, r3
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	ldr	r2, [sp, #24]
	mov	r0, r8
	adds	r1, r6, #0
	mov	r3, fp
	bl 0x0200d4e8
	ldr	r0, [sp, #8]
	mov	r3, fp
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #8]
	movs	r4, #0
	str	r3, [sp, #0]
	movs	r0, #2
	ldr	r3, [sp, #24]
	str	r4, [sp, #4]
	bl 0x0200ca44
	bl 0x0200d668
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r3}
	add	sp, #16
	bx	r3
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200df64
	.4byte 0x0200cfc1
	.2byte 0xdf7c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldrh	r3, [r5, #6]
	movs	r2, #12
	lsrs	r1, r3, #12
	adds	r3, r1, #2
	ands	r3, r2
	lsls	r1, r3, #12
	ldr	r3, [r5, #8]
	sub	sp, #12
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	movs	r0, #128
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #13
	adds	r2, r6, #0
	str	r3, [r6, #8]
	bl 0x0200d408
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200d680
	cmp	r0, #0
	beq.n	.L_0200501c
	movs	r4, #0
.L_02004ff8:
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r2, [pc, #64]
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	cmp	r1, r3
	beq.n	.L_02005040
	adds	r4, #1
	cmp	r4, #5
	bls.n	.L_02004ff8
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200d4a8
.L_0200501c:
	ldr	r3, [r5, #8]
	adds	r0, r5, #0
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	adds	r1, r6, #0
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl 0x0200d4f8
	cmp	r0, #0
	ble.n	.L_02005040
.L_02005034:
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200d4a8
.L_02005040:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0xdf58
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
	ldr	r1, [r3, #32]
	ldr	r3, [pc, #380]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r0, [r2, #0]
	ldr	r2, [r2, #4]
	ands	r0, r3
	ands	r2, r3
	ldr	r3, [pc, #372]
	mov	sl, r0
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	mov	r8, r2
	ldr	r2, [pc, #364]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	sub	sp, #8
	lsrs	r3, r3, #5
	str	r3, [sp, #4]
	ldr	r6, [pc, #356]
	ldr	r3, [r1, #0]
	movs	r1, #0
	ldr	r3, [r3, #4]
	mov	r9, r1
	str	r3, [sp, #0]
	ldr	r3, [pc, #348]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r9, r3
	blt.n	.L_0200509a
	b.n	.L_020051ce
.L_0200509a:
	ldr	r2, [pc, #340]
	mov	r0, r9
	lsls	r3, r0, #2
	ldr	r5, [r2, r3]
	cmp	r5, #0
	bne.n	.L_020050a8
	b.n	.L_020051be
.L_020050a8:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bne.n	.L_020050b0
	b.n	.L_020051be
.L_020050b0:
	mov	r1, sl
	subs	r0, r3, r1
	ldr	r2, [sp, #0]
	ldr	r3, [r5, #12]
	movs	r1, #128
	subs	r3, r3, r2
	ldr	r2, [r5, #16]
	lsls	r1, r1, #12
	adds	r3, r3, r1
	mov	r1, r8
	subs	r2, r2, r1
	ldr	r1, [sp, #0]
	subs	r2, r2, r1
	subs	r4, r2, r3
	adds	r3, r3, r2
	asrs	r3, r3, #16
	adds	r3, #58
	mov	fp, r3
	ldr	r3, [pc, #284]
.L_020050d6:
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	adds	r3, r5, #0
	mov	ip, r2
	asrs	r1, r0, #16
	mov	r0, ip
	adds	r3, #100
	asrs	r2, r4, #16
	cmp	r0, #0
	bne.n	.L_02005126
	movs	r0, #0
	ldrsh	r7, [r3, r0]
	adds	r0, r1, #0
	adds	r3, r1, #7
	movs	r1, #167
	adds	r4, r2, #0
	lsls	r1, r1, #1
	subs	r0, #8
	subs	r4, #16
	cmp	r3, r1
	bhi.n	.L_020051be
	movs	r2, #16
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_020051be
	cmp	r4, #239
	bgt.n	.L_020051be
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	movs	r3, #255
	adds	r1, r6, #0
	ands	r4, r3
	mov	r3, ip
	stmia	r1!, {r3}
	lsls	r3, r0, #16
	orrs	r4, r3
	ldr	r3, [pc, #212]
	b.n	.L_02005162
.L_02005126:
	movs	r0, #0
	ldrsh	r7, [r3, r0]
	adds	r0, r1, #0
	adds	r3, r1, #0
	movs	r1, #175
	adds	r4, r2, #0
	adds	r3, #23
	lsls	r1, r1, #1
	subs	r0, #8
	subs	r4, #64
	cmp	r3, r1
	bhi.n	.L_020051be
	movs	r2, #64
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_020051be
	cmp	r4, #175
	bgt.n	.L_020051be
.L_0200514a:
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	movs	r3, #255
	adds	r1, r6, #0
	ands	r4, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r0, #16
	orrs	r4, r3
	ldr	r3, [pc, #152]
.L_02005162:
	movs	r2, #128
	orrs	r4, r3
	stmia	r1!, {r4}
	ldr	r0, [sp, #4]
	lsls	r3, r7, #3
	adds	r3, r0, r3
	lsls	r2, r2, #4
.L_02005170:
	orrs	r3, r2
	str	r3, [r1, #0]
	ldr	r3, [pc, #136]
	movs	r0, #1
	ldrh	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_020051a0
	adds	r0, r5, #0
	bl 0x0200d698
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r6, #9]
	negs	r1, r1
	adds	r2, r1, #0
	lsls	r0, r0, #2
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r6, #9]
	b.n	.L_020051b4
.L_020051a0:
	movs	r3, #3
	ands	r3, r2
	movs	r0, #13
.L_020051a6:
	ldrb	r2, [r6, #9]
	negs	r0, r0
	adds	r1, r0, #0
	lsls	r3, r3, #2
	ands	r2, r1
	orrs	r2, r3
	strb	r2, [r6, #9]
.L_020051b4:
	adds	r0, r6, #0
	mov	r1, fp
	bl 0x0200d440
	adds	r6, #12
.L_020051be:
	ldr	r3, [pc, #44]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	add	r9, r1
	cmp	r9, r3
	bge.n	.L_020051ce
	b.n	.L_0200509a
.L_020051ce:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffff0000
	.4byte 0x0200f4cc
	.4byte 0x020036e0
	.4byte 0x0200f510
	.4byte 0x0200f4ce
	.4byte 0x0200f4d0
	.4byte 0x0200f5d0
	.4byte 0x40002000
	.4byte 0xc000a000
	.2byte 0xf5d2
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200d410
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200d420
	ldr	r5, [pc, #76]
	bl 0x0200d438
	movs	r1, #192
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200d430
	adds	r0, r6, #0
	bl 0x0200d418
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200d3e0
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #16]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #8]
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200f4d0
	.4byte 0x0200dfbc
	.4byte 0x0200f4cc
	.4byte 0x0200d049
	.4byte 0x0200f4ce
	.4byte 0x0200f5d0
	.2byte 0xf5d2
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200d410
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200d420
	ldr	r5, [pc, #76]
	bl 0x0200d438
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200d430
	adds	r0, r6, #0
	bl 0x0200d418
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200d3e0
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #16]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #8]
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200f4d0
	.4byte 0x0200e11f
	.4byte 0x0200f4cc
	.4byte 0x0200d049
	.4byte 0x0200f4ce
	.4byte 0x0200f5d0
	.2byte 0xf5d2
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	bl 0x0200d410
	ldr	r3, [pc, #88]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #84]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #80]
	bl 0x0200d420
	ldr	r5, [pc, #80]
	bl 0x0200d438
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200d430
	adds	r0, r6, #0
	bl 0x0200d418
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	bl 0x0200d3e0
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	b.n	.L_02005388
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200f4d0
	.4byte 0x0200e34e
	.4byte 0x0200f4cc
	.4byte 0x0200d049
	.4byte 0x0200f4ce
	.4byte 0x0200f5d0
	.2byte 0xf5d2
	.2byte 0x0200
.L_02005388:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	bl 0x0200d570
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L_020053b2
	adds	r3, r4, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	ldr	r1, [pc, #16]
	ldr	r0, [pc, #20]
	ldrh	r2, [r1, #0]
	movs	r5, #0
	ldrsh	r3, [r1, r5]
	adds	r2, #1
	lsls	r3, r3, #2
	str	r4, [r0, r3]
	strh	r2, [r1, #0]
.L_020053b2:
	pop	{r5, pc}
	.4byte 0x0200f4ce
	.4byte 0x0200f4d0
	.4byte 0x80184b01
	.4byte 0x00004770
	.4byte 0x0200f5d2
	.section .text.x0200d6c8,"ax",%progbits
	.4byte 0x0000002e
	.4byte 0x0200804d
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01300000
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
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01300000
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
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000012
	.4byte 0x00000220
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0020000
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
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
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
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
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00200000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00500000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00500000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00500000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00200000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00200000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
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
	.4byte 0x03c80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03c80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00190018
	.4byte 0x0018001a
	.4byte 0x00000019
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
	.4byte 0x00000000
	.4byte 0x0000002a
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
	.4byte 0x00430008
	.4byte 0x000a0000
	.4byte 0x00000001
	.4byte 0x00110012
	.4byte 0x00310013
	.4byte 0x00130014
	.4byte 0x00030015
	.4byte 0x00160000
	.4byte 0x00000000
	.4byte 0x0011000d
	.4byte 0x0021000e
	.4byte 0x0031000f
	.4byte 0x00830010
	.4byte 0x00130000
	.4byte 0x00000001
	.4byte 0x0003000d
	.4byte 0x000e0000
	.4byte 0x00000001
	.4byte 0x00030008
	.4byte 0x00030009
	.4byte 0x0003000a
	.4byte 0x000c0000
	.4byte 0x00000000
	.4byte 0x0003000d
	.4byte 0x0003000e
	.4byte 0x0003000f
	.4byte 0x000a0000
	.4byte 0x00000000
	.4byte 0x00030018
	.4byte 0x00030019
	.4byte 0x0003001a
	.4byte 0x00120000
	.4byte 0x00130000
	.4byte 0x00140000
	.4byte 0x00150000
	.4byte 0x00160000
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x0200b479
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00200000
	.4byte 0x02a00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00160000
	.4byte 0x02a00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00160000
	.4byte 0x02b00000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte 0x020086f5
	.2byte 0x002e
	.2byte 0x0000
	push	{r0, r2, r3, r4, lr}
	lsls	r0, r0, #8
	movs	r1, r2
	movs	r0, r0
	movs	r6, r5
.L_02005ce2:
	movs	r0, r0
	push	{r0, r3, r4, r5, r6}
	lsls	r0, r0, #8
	movs	r4, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #2
.L_02005cf0:
	movs	r0, r0
	lsls	r0, r0, #1
	movs	r0, r0
	lsls	r0, r6, #8
	movs	r1, r0
	movs	r0, r0
	movs	r4, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #2
	movs	r0, r0
	movs	r6, r6
	movs	r0, r0
	lsls	r0, r6, #8
	movs	r1, r0
	movs	r0, r0
.L_02005d10:
	movs	r4, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #2
	movs	r0, r0
	movs	r6, r6
	movs	r0, r0
	lsls	r0, r0, #9
	movs	r1, r0
	movs	r0, r0
	movs	r6, r2
	movs	r0, r0
	movs	r5, r4
	movs	r0, r0
	ldrh	r1, [r0, #44]
	lsls	r0, r0, #8
	movs	r6, r5
	movs	r0, r0
	push	{r0, r2, r3, r4, lr}
	lsls	r0, r0, #8
	movs	r1, r2
	movs	r0, r0
	movs	r6, r5
	movs	r0, r0
	push	{r0, r3, r4, r5, r6}
	lsls	r0, r0, #8
	movs	r4, r0
	movs	r0, r0
	movs	r0, r0
.L_02005d4a:
	lsls	r0, r5, #13
	movs	r0, r0
.L_02005d4e:
	movs	r0, r4
	movs	r0, r0
	lsls	r0, r0, #2
	movs	r1, r0
.L_02005d56:
	movs	r0, r0
.L_02005d58:
	movs	r4, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #13
	movs	r0, r0
.L_02005d62:
	movs	r6, r2
	movs	r0, r0
	lsls	r0, r0, #2
.L_02005d68:
	movs	r1, r0
.L_02005d6a:
	movs	r0, r0
	movs	r4, r0
.L_02005d6e:
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #13
	movs	r0, r0
	movs	r6, r2
	movs	r0, r0
.L_02005d7a:
	lsls	r0, r2, #2
	movs	r1, r0
	movs	r0, r0
	movs	r6, r5
	movs	r0, r0
	push	{r0, r2, r3, r4, lr}
.L_02005d86:
	lsls	r0, r0, #8
	movs	r1, r2
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r6, r3
.L_02005d92:
	movs	r0, r0
	movs	r6, r2
	movs	r0, r0
	movs	r6, r4
	movs	r0, r0
	movs	r0, r0
.L_02005d9e:
	movs	r0, r0
	movs	r2, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02005daa:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02005db2:
	movs	r0, r0
	movs	r1, r0
.L_02005db6:
	movs	r0, r0
	movs	r6, r4
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r4, r2
.L_02005dc2:
	movs	r0, r0
	movs	r0, r0
	strh	r1, [r0, #0]
	movs	r6, r2
	movs	r0, r0
	movs	r1, r3
.L_02005dce:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	movs	r1, r0
.L_02005dda:
	movs	r0, r0
	movs	r6, r2
	movs	r0, r0
.L_02005de0:
	movs	r1, r3
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r1
	movs	r0, r0
	movs	r2, r1
.L_02005df6:
	movs	r0, r0
	movs	r0, r0
	stmia	r0!, {r0}
	movs	r6, r4
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r4, r2
	movs	r0, r0
	movs	r0, r0
	strh	r1, [r0, #0]
	movs	r6, r2
	movs	r0, r0
	movs	r1, r3
.L_02005e12:
	.2byte 0x0000
	.2byte 0x0000
.L_02005e16:
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	movs	r1, r0
.L_02005e1e:
	movs	r0, r0
	movs	r6, r2
	movs	r0, r0
	movs	r1, r3
	movs	r0, r0
.L_02005e28:
	movs	r1, r0
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r1
	movs	r0, r0
	movs	r2, r1
	movs	r0, r0
	movs	r0, r0
	stmia	r0!, {r0}
	movs	r6, r4
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r1
	movs	r0, r0
	movs	r6, r2
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
.L_02005e60:
	movs	r2, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02005e6c:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r4
	movs	r0, r0
	movs	r1, r2
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r1
	movs	r0, r0
	movs	r4, r5
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r2, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r4
	movs	r0, r0
	movs	r1, r2
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r1, r0
.L_02005ec2:
	movs	r0, r0
.L_02005ec4:
	movs	r5, r1
.L_02005ec6:
	movs	r0, r0
	lsls	r6, r7, #1
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r2, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r4
	movs	r0, r0
	movs	r1, r2
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0000
	strh	r0, [r0, #0]
	.2byte 0xffff
	.2byte 0xc000
	b.n	.L_02005f06
	.2byte 0xa000
.L_02005f06:
	.2byte 0xc000
.L_02005f08:
	ands	r0, r0
.L_02005f0a:
	movs	r0, #0
	str	r0, [r0, #0]
	ands	r0, r0
	.2byte 0xffff
	.2byte 0x0000
	strh	r0, [r0, #0]
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	strh	r0, [r0, #0]
	.2byte 0xffff
	.2byte 0xc000
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xc000
	ands	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	ands	r0, r0
	.2byte 0xffff
	.2byte 0x0000
	strh	r0, [r0, #0]
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	strh	r0, [r0, #0]
.L_02005f3e:
	.2byte 0xffff
	.2byte 0xc000
	movs	r0, r0
	strh	r0, [r0, #0]
.L_02005f46:
	.2byte 0xc000
	ands	r0, r0
	movs	r0, r0
	strh	r0, [r0, #0]
	ands	r0, r0
	.2byte 0xffff
	.2byte 0x0000
	strh	r0, [r0, #0]
	.2byte 0xffff
	.2byte 0x0102
	lsls	r3, r0, #4
	lsls	r5, r4, #4
.L_02005f5e:
	lsls	r6, r4, #4
.L_02005f60:
	lsls	r4, r1, #5
.L_02005f62:
	lsls	r3, r1, #5
	.2byte 0xf8e0
	.2byte 0x0820
	b.n	.L_0200615c
	.2byte 0x2008
	.4byte 0x0020f0e0
	.4byte 0x2008e0f8
	.4byte 0x0820f8e0
	.4byte 0x2008e0f8
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.4byte 0x06345d01
	.4byte 0x08003b01
	.4byte 0x2f010026
	.4byte 0x5f1f7000
	.4byte 0x667b0906
	.4byte 0x01040901
	.4byte 0x0e287800
	.4byte 0x56053b3f
	.4byte 0x5f6f080e
	.4byte 0x08100800
	.4byte 0xa1076601
	.4byte 0x001d0800
	.4byte 0x08071768
	.4byte 0x660015bb
	.4byte 0x0e020016
	.4byte 0x66177000
	.4byte 0x3b02080d
	.4byte 0x020010df
	.4byte 0x66128010
	.4byte 0x01037a01
	.4byte 0x04277910
	.4byte 0xfb44013d
	.4byte 0x02007800
	.4byte 0x20590414
	.4byte 0x57052700
	.4byte 0x50002066
	.4byte 0x20ff0016
	.4byte 0x00162a00
	.4byte 0x0016002e
	.4byte 0x162a0020
	.4byte 0x4e002000
	.4byte 0x7f320040
	.4byte 0x04aa023b
	.4byte 0x02500006
	.4byte 0xe7169207
	.4byte 0x0010e103
	.4byte 0x0301ff0d
	.4byte 0x0c01e709
	.4byte 0x5906dd03
	.4byte 0x210a6118
	.4byte 0xfd340030
	.4byte 0x0403a425
	.4byte 0x5f290630
	.4byte 0x02490100
	.4byte 0xf8063b58
	.4byte 0x3b5704af
	.4byte 0x163bb406
	.4byte 0x0800200e
	.4byte 0x283ea00f
	.4byte 0x070820ff
	.4byte 0x2e050010
	.4byte 0x057807f8
	.4byte 0x0a980488
	.4byte 0xdd660ba8
	.4byte 0x07080030
	.4byte 0x38205f22
	.4byte 0x33481706
	.4byte 0x00603b58
	.4byte 0x783ffe78
	.4byte 0x08076918
	.4byte 0x1f017900
	.4byte 0x0a782700
	.4byte 0x2aff5f11
	.4byte 0x701f07a8
	.4byte 0xef273000
	.4byte 0x00505704
	.4byte 0x4c002037
	.4byte 0xf7000040
	.4byte 0x0040002a
	.4byte 0x1100206e
	.4byte 0x403b0046
	.4byte 0x00200000
	.4byte 0x2a00402c
	.4byte 0x244819f7
	.4byte 0x2d004000
	.4byte 0x043b0803
	.4byte 0x05830722
	.4byte 0x1817b705
	.4byte 0x0028003b
	.4byte 0x66015920
	.2byte 0x2260
	.2byte 0x0007
.L_02006100:
	lsls	r2, r3, #16
	ldr	r5, [r0, #0]
	lsls	r7, r7, #7
.L_02006106:
	subs	r7, #27
	lsls	r0, r3, #17
	lsls	r5, r2, #24
	cmp	r2, #44
	ands	r0, r3
	lsrs	r0, r0, #4
.L_02006112:
	ldrh	r0, [r0, #56]
	lsls	r2, r0, #12
	b.n	.L_02006140
	.4byte 0x0030cd0c
	.4byte 0x00000002
	.4byte 0xac862b05
	.4byte 0xaf643138
	.4byte 0x426905df
	.4byte 0xabd8ac4e
	.4byte 0xbe810f20
	.4byte 0x7015130e
	.4byte 0x58aaf4f3
	.2byte 0xfed1
	.2byte 0x1c57
.L_02006140:
	.2byte 0xf5fa
	.2byte 0xba72
.L_02006144:
	stmia	r7!, {r0, r1, r2, r3, r7}
	asrs	r3, r6, #23
	ble.n	.L_02006144
	cmp	r4, #75
	ldrb	r6, [r5, r0]
	ldrh	r0, [r0, #34]
	asrs	r0, r2, #1
	ldrh	r0, [r2, r2]
	subs	r5, #2
	str	r4, [r3, #104]
	str	r4, [r7, #84]
	strh	r7, [r7, #8]
.L_0200615c:
	.2byte 0xf4a0
	.2byte 0xc48a
	ldrb	r6, [r3, #20]
.L_02006162:
	strb	r2, [r6, #3]
	ldrh	r7, [r3, #56]
	svc	207
	str	r7, [r2, #124]
	lsrs	r6, r4, #15
	subs	r4, #48
	strb	r6, [r7, r2]
	.2byte 0xf8f4
	.2byte 0xa442
.L_02006174:
	ldmia	r7, {r0, r1, r2, r6, r7}
	.2byte 0xeb17
	.2byte 0x22f4
	lsls	r0, r0, #22
	beq.n	.L_02006100
	stmia	r3!, {r0, r5, r7}
	lsrs	r3, r5, #8
	.2byte 0xf05c
	.2byte 0xf23e
.L_02006186:
	pop	{r2, r6, r7}
	strb	r0, [r4, #29]
	.2byte 0xe8ae
	.2byte 0x5730
.L_0200618e:
	subs	r0, #116
	ldr	r4, [sp, #348]
	b.n	.L_02005de0
	.4byte 0x0e73008e
	.4byte 0x075f2408
	.4byte 0x1297a050
	.4byte 0x8f028687
	.4byte 0xebc03222
	.4byte 0x3e706883
	.4byte 0x8034e810
	.4byte 0x161c479c
	.4byte 0x9c090038
	.4byte 0x0a75ee0f
	.4byte 0xbf6039f8
	.4byte 0xf613af4e
	.4byte 0x08e7ee5a
	.4byte 0x3df8973f
	.4byte 0xf81f363e
	.4byte 0x63e27cd8
	.4byte 0x9df381f3
	.4byte 0xf7e793f3
	.4byte 0xd0e1c9fa
	.4byte 0xf258ce1d
	.4byte 0x3873c780
	.4byte 0xb93be3cc
	.4byte 0x2f3be3cc
	.4byte 0x6118fcc1
	.4byte 0x980e30ca
	.4byte 0x7cef9c33
	.4byte 0x29f1be0c
	.4byte 0xc7c786f0
	.4byte 0x5edecbe6
	.4byte 0x9239f837
	.4byte 0xebc2b9d8
	.4byte 0xd2f88ad8
	.4byte 0xa9bcc731
	.4byte 0x71e99cc7
	.4byte 0x5f547988
	.4byte 0x27049ac1
	.4byte 0xd7ae264f
	.4byte 0xc532a694
	.4byte 0x12fa7ae5
	.4byte 0xc43ef833
	.4byte 0x7d1e54f6
	.4byte 0x97fdb2f9
	.4byte 0xc6a7c3dd
	.4byte 0x3415957d
	.4byte 0x1f1897ad
	.4byte 0xac141210
	.4byte 0x047b4ae7
	.4byte 0x9eb43c03
	.4byte 0xcd180907
	.4byte 0x000079ea
	.4byte 0xf6450423
	.4byte 0xa7d80d99
	.4byte 0x78059b49
	.4byte 0x467a0580
	.4byte 0xb85c205b
	.4byte 0xd050008c
	.4byte 0x500b31ab
	.4byte 0x7833d893
	.4byte 0x8307f302
	.4byte 0x3a9c0502
	.4byte 0x3831102c
	.4byte 0x306a301c
	.4byte 0x3831101d
	.4byte 0x3026301c
	.4byte 0x38f2201a
	.4byte 0x0f2f2e80
	.4byte 0xc0861624
	.4byte 0x1f1ec23c
	.4byte 0x04f95397
	.4byte 0xc067c07c
	.4byte 0xc701f244
	.4byte 0x644c7e42
	.4byte 0x28ae605c
	.4byte 0xa2b8f289
	.4byte 0x087d19d7
	.4byte 0xb7fbd5ce
	.4byte 0x8db92233
	.4byte 0xa5310573
	.4byte 0x8e792a73
	.4byte 0x80804174
	.4byte 0x783318f3
	.4byte 0x3905033e
	.4byte 0x7c0bd78f
	.4byte 0x71e9ce5c
	.4byte 0xebc2b80d
	.4byte 0xfe12e605
	.4byte 0x385d04be
	.4byte 0xc06a9047
	.4byte 0x83a09e03
	.4byte 0x1efc23d7
	.4byte 0xf02b869f
	.4byte 0x3002f9f9
	.4byte 0x3f0df1e0
	.4byte 0x793f904f
	.4byte 0xc78f423c
	.4byte 0x3e067c3b
	.4byte 0x3e29ebe7
	.4byte 0xf7f39f16
	.4byte 0x33967f4d
	.4byte 0x780c9067
	.4byte 0x6ce3c57c
	.4byte 0x37ce61c6
	.4byte 0x37cc0e8c
	.4byte 0xcfc6d8fc
	.4byte 0xfcf83ef7
	.4byte 0xe07f9f03
	.4byte 0xfe7c0ff3
	.4byte 0xf03fcf81
	.4byte 0xff3e07f9
	.4byte 0xf816c7c0
	.4byte 0x01000000
	.4byte 0xcf81fe7c
	.4byte 0x07f9f03f
	.4byte 0x6080ff3e
	.4byte 0xead9f7b4
	.4byte 0x1e7cf8d3
	.4byte 0xca9f3dc1
	.4byte 0x8bf7f3e3
	.4byte 0x5b2fc50f
	.4byte 0xf00e1c7d
	.4byte 0xe7c5f8b1
	.4byte 0x4a1f1bef
	.4byte 0x8bf0f3e0
	.4byte 0x3e3dc58f
	.4byte 0xa1f0df7f
	.4byte 0x0a7dbf05
	.4byte 0xf8d3e16e
	.4byte 0x829f7b46
	.4byte 0xe7b8e75e
	.4byte 0x828df1f3
	.4byte 0x5ad8295e
	.4byte 0x3e060bd0
	.4byte 0x49a8e07d
	.4byte 0x6264f270
	.4byte 0x32a6829c
	.4byte 0x127b65c5
	.4byte 0xf6fcf033
	.4byte 0x54f6c43c
	.4byte 0xb2f97d1e
	.4byte 0xc3dd97fd
	.4byte 0x957dc6a7
	.4byte 0x8914e015
	.4byte 0x012101f1
	.4byte 0x166b2b8f
	.4byte 0xf00c11e4
	.4byte 0x241e7ad0
	.4byte 0xe7ab3460
	.4byte 0x108c0001
	.4byte 0x3667d914
	.4byte 0xde0f9f60
	.4byte 0x1b4b2a37
	.4byte 0x2c23c02c
	.4byte 0x02da33d0
	.4byte 0x0465c6e1
	.4byte 0x8d5e8282
	.4byte 0xc49a8459
	.4byte 0x9813c19e
	.4byte 0x2854187f
	.4byte 0x8161d4e0
	.4byte 0x80e1c388
	.4byte 0x80e98751
	.4byte 0x80e1c388
	.4byte 0x00d18131
	.4byte 0x7401c791
	.4byte 0xb1207979
	.4byte 0x7c37dd8f
	.4byte 0x00000000
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
	.4byte 0x00000124
	.4byte 0x00102122
	.4byte 0x00203124
	.4byte 0x00302124
	.4byte 0x00403128
	.4byte 0x00000125
	.4byte 0x00102120
	.4byte 0x00203125
	.4byte 0x00302125
	.4byte 0x00405125
	.4byte 0x00504125
	.4byte 0x00607125
	.4byte 0x00706125
	.4byte 0x00802128
	.4byte 0x00909125
	.4byte 0x00000126
	.4byte 0x00102121
	.4byte 0x00203126
	.4byte 0x00302126
	.4byte 0x00405126
	.4byte 0x00504126
	.4byte 0x00606126
	.4byte 0x00707126
	.4byte 0x00809126
	.4byte 0x00908126
	.4byte 0x00a0a126
	.4byte 0x00b0c126
	.4byte 0x00c0b126
	.4byte 0x00d0e126
	.4byte 0x00e0d126
	.4byte 0x00f04128
	.4byte 0x01010126
	.4byte 0x00000127
	.4byte 0x00102123
	.4byte 0x00607127
	.4byte 0x00706127
	.4byte 0x00809127
	.4byte 0x00908127
	.4byte 0x00a0b127
	.4byte 0x00b0a127
	.4byte 0x00c0d127
	.4byte 0x00d0c127
	.4byte 0x00e0f127
	.4byte 0x00f0e127
	.4byte 0x00501128
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x0200d748
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x0200d6d4
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019e
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
	.4byte 0xffff019e
	.4byte 0x0200d7bc
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00024000
	.4byte 0xffff01a2
	.4byte 0x0200d6c8
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d6c8
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d6c8
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
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
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03500000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x00000007
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00024000
	.4byte 0xffff0127
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff019f
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff019e
	.4byte 0x0200d890
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x0200d93c
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x0200da3c
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
	.4byte 0xffff01ac
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00020000
	.4byte 0xffff019e
	.4byte 0x0200da88
	.4byte 0x03c80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d6c8
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00780000
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
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00500000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00500000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x00000007
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x00000007
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02400000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00028000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00028000
	.4byte 0xffff019e
	.4byte 0x0200e980
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00024000
	.4byte 0xffff01a2
	.4byte 0x0200d6c8
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d6c8
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200d6c8
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff0127
	.4byte 0x00000007
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00cc0000
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
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000202
	.4byte 0xffff001e
	.4byte 0x020080d1
	.4byte 0x00008602
	.4byte 0xffff001f
	.4byte 0x02008169
	.4byte 0x00000202
	.4byte 0xffff001f
	.4byte 0x02008171
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
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
	.4byte 0x00000021
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
	.4byte 0x00000051
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000051
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008249
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200825d
	.4byte 0x10008c15
	.4byte 0xffff000a
	.4byte 0x02008249
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x0200825d
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x020082ed
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x020082ed
	.4byte 0x00009315
	.4byte 0xffff000a
	.4byte 0x020082ed
	.4byte 0x00009315
	.4byte 0xffff000b
	.4byte 0x020082ed
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte 0x020082ed
	.4byte 0x00000602
	.4byte 0x0a380021
	.4byte 0x0200821d
	.4byte 0x00008c15
	.4byte 0x0a380010
	.4byte 0x020081f5
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte 0x00000000
	.4byte 0x10008c15
	.4byte 0xffff0012
	.4byte 0x02008281
	.4byte 0x10008c15
	.4byte 0xffff0013
	.4byte 0x02008281
	.4byte 0x10008c15
	.4byte 0xffff0014
	.4byte 0x02008281
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte 0x020082b9
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte 0x020082b9
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte 0x020082b9
	.4byte 0x10008c15
	.4byte 0xffff0015
	.4byte 0x02008281
	.4byte 0x10008c15
	.4byte 0xffff0016
	.4byte 0x02008281
	.4byte 0x00008c15
	.4byte 0xffff0015
	.4byte 0x020082b9
	.4byte 0x00008c15
	.4byte 0xffff0016
	.4byte 0x020082b9
	.4byte 0x00008c15
	.4byte 0xffff0017
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0058
	.4byte 0x0200a97d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0x02200021
	.4byte 0x020084f9
	.4byte 0x00000202
	.4byte 0xffff001e
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x0200ab89
	.4byte 0x00000002
	.4byte 0xffff0058
	.4byte 0x0200a97d
	.4byte 0x00008c15
	.4byte 0xffff0016
	.4byte 0x00000000
	.4byte 0x50009705
	.4byte 0x0a440014
	.4byte 0x02008445
	.4byte 0x50009705
	.4byte 0x0a450015
	.4byte 0x02008481
	.4byte 0x50009705
	.4byte 0x0a460016
	.4byte 0x020084bd
	.4byte 0x10009a15
	.4byte 0xffff0009
	.4byte 0x020080bd
	.4byte 0x60009a15
	.4byte 0xffff000b
	.4byte 0x02008555
	.4byte 0x20009a15
	.4byte 0xffff000b
	.4byte 0x02008671
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x02008945
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x50008905
	.4byte 0xffff0002
	.4byte 0x02008755
	.4byte 0x50008905
	.4byte 0xffff0003
	.4byte 0x02008761
	.4byte 0x50008905
	.4byte 0xffff0004
	.4byte 0x0200876d
	.4byte 0x50008905
	.4byte 0xffff0005
	.4byte 0x02008779
	.4byte 0x50008905
	.4byte 0xffff0006
	.4byte 0x02008785
	.4byte 0x50008905
	.4byte 0xffff0007
	.4byte 0x02008791
	.4byte 0x50008905
	.4byte 0xffff0008
	.4byte 0x0200879d
	.4byte 0x50008905
	.4byte 0xffff0009
	.4byte 0x020087a9
	.4byte 0x50008905
	.4byte 0xffff000a
	.4byte 0x020087b5
	.4byte 0x50008905
	.4byte 0xffff000b
	.4byte 0x020087c1
	.4byte 0x50008905
	.4byte 0xffff000c
	.4byte 0x020087cd
	.4byte 0x50008905
	.4byte 0xffff000d
	.4byte 0x020087d9
	.4byte 0x50008905
	.4byte 0xffff000e
	.4byte 0x020087e5
	.4byte 0x00008c15
	.4byte 0x0358000b
	.4byte 0x020087f1
	.4byte 0x00008c15
	.4byte 0x0a37000c
	.4byte 0x020088a1
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x020088bd
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008901
	.4byte 0x00000002
	.4byte 0xffff0058
	.4byte 0x0200a97d
	.4byte 0x00000002
	.4byte 0xffff005a
	.4byte 0x0200aba1
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x0200ace1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
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
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x0200c141
	.4byte 0x00000202
	.4byte 0xffff0021
	.4byte 0x02008b41
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte 0x0200ab89
	.4byte 0x00000002
	.4byte 0xffff0058
	.4byte 0x0200a97d
	.4byte 0x10009a15
	.4byte 0xffff0011
	.4byte 0x020080bd
	.4byte 0x60009a15
	.4byte 0xffff0010
	.4byte 0x02008aad
	.4byte 0x20009a15
	.4byte 0xffff0010
	.4byte 0x02008ad9
	.4byte 0x00001815
	.4byte 0x0220000b
	.4byte 0x02008cf5
	.4byte 0x00001815
	.4byte 0x0221000c
	.4byte 0x02008d39
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x0200de44
	.4byte 0x0200de80
	.4byte 0x0200debc
