.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xac98
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
	.2byte 0xacc8
	.2byte 0x0200
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_02000060
	ldr	r0, [pc, #4]
	b.n	.L_02000062
.L_02000060:
	ldr	r0, [pc, #4]
.L_02000062:
	pop	{pc}
	.4byte 0x0200aff0
	.2byte 0xad38
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	movs	r5, #8
.L_02000080:
	adds	r0, r5, #0
	bl 0x0200ab48
	cmp	r0, #0
	beq.n	.L_02000092
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_02000092:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_02000080
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r6, r3
	movs	r3, #0
	ldrsh	r5, [r6, r3]
	movs	r0, #158
	bl 0x0200ac90
	subs	r5, #1
	ldr	r0, [pc, #84]
	lsls	r5, r5, #3
	adds	r3, r5, #4
	ldrh	r1, [r0, r3]
	adds	r3, r3, r0
	ldrh	r2, [r3, #2]
	ldr	r0, [r0, r5]
	bl 0x0200aae8
	ldr	r5, [pc, #68]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200ab48
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #2
	ldr	r0, [r5, #0]
	bl 0x0200ab98
	movs	r2, #8
	movs	r1, #2
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x0200ab70
	movs	r0, #10
	bl 0x0200ab20
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl 0x0200ac30
	bl 0x0200ac40
	bl 0x0200ac48
	bl 0x0200ab30
	pop	{r5, r6, pc}
	.4byte 0x0200b308
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200ab48
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #44]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_0200013a
	movs	r0, #4
	adds	r1, r5, #0
	bl 0x0200ac88
	b.n	.L_02000180
.L_0200013a:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_0200015c
	ldr	r0, [pc, #12]
	bl 0x0200abd0
	b.n	.L_02000178
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x2118
	.2byte 0x0000
.L_0200015c:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_02000172
	ldr	r0, [pc, #24]
	bl 0x0200abd0
	b.n	.L_02000178
.L_02000172:
	ldr	r0, [pc, #20]
	bl 0x0200abd0
.L_02000178:
	movs	r0, #29
	movs	r1, #0
	bl 0x0200abe8
.L_02000180:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001d4c
	.2byte 0x18da
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200ab48
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #44]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020001be
	movs	r0, #5
	adds	r1, r5, #0
	bl 0x0200ac88
	b.n	.L_02000204
.L_020001be:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_020001e0
	ldr	r0, [pc, #12]
	bl 0x0200abd0
	b.n	.L_020001fc
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x211a
	.2byte 0x0000
.L_020001e0:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_020001f6
	ldr	r0, [pc, #24]
	bl 0x0200abd0
	b.n	.L_020001fc
.L_020001f6:
	ldr	r0, [pc, #20]
	bl 0x0200abd0
.L_020001fc:
	movs	r0, #30
	movs	r1, #0
	bl 0x0200abe8
.L_02000204:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001d4e
	.2byte 0x18dc
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200ab48
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #44]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000242
	movs	r0, #6
	adds	r1, r5, #0
	bl 0x0200ac88
	b.n	.L_02000288
.L_02000242:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_02000264
	ldr	r0, [pc, #12]
	bl 0x0200abd0
	b.n	.L_02000280
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x211c
	.2byte 0x0000
.L_02000264:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_0200027a
	ldr	r0, [pc, #24]
	bl 0x0200abd0
	b.n	.L_02000280
.L_0200027a:
	ldr	r0, [pc, #20]
	bl 0x0200abd0
.L_02000280:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200abe8
.L_02000288:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001d50
	.2byte 0x18de
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #32]
	bl 0x0200abd0
	movs	r1, #4
	movs	r0, #33
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x208f
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #92]
	bl 0x0200abd0
	movs	r1, #0
	movs	r0, #33
	bl 0x0200abd8
	movs	r0, #4
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_02000316
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #33
	bl 0x0200ac08
	movs	r1, #0
	movs	r0, #33
	bl 0x0200abd8
	movs	r0, #4
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_02000310
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #33
	bl 0x0200ac08
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	b.n	.L_0200031a
.L_02000310:
	bl 0x02008294
	b.n	.L_0200031a
.L_02000316:
	bl 0x02008294
.L_0200031a:
	pop	{pc}
	.2byte 0x208c
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #68]
	bl 0x0200abd0
	movs	r1, #2
	movs	r0, #33
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #33
	bl 0x0200ac08
	movs	r2, #10
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe0
	movs	r1, #4
	movs	r0, #33
	bl 0x0200aba0
	movs	r0, #10
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2090
	.2byte 0x0000
	push	{lr}
	movs	r0, #136
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_020003b0
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #33
	bl 0x0200ac08
	ldr	r0, [pc, #32]
	bl 0x0200abd0
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	bl 0x0200ab30
	movs	r0, #136
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200aae0
.L_020003b0:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2092
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200abd0
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200abd8
	bl 0x0200ac80
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_020003e8
	movs	r0, #10
	bl 0x0200ab20
	adds	r0, r5, #1
	bl 0x0200abd0
	b.n	.L_020003f4
.L_020003e8:
	movs	r0, #20
	bl 0x0200ab20
	adds	r0, r5, #2
	bl 0x0200abd0
.L_020003f4:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200abe8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x205d
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #26
	movs	r1, #4
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #193
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_02000426
	ldr	r0, [pc, #104]
	bl 0x0200abd0
	b.n	.L_0200047c
.L_02000426:
	ldr	r5, [pc, #100]
	adds	r0, r5, #0
	bl 0x0200abd0
	movs	r1, #0
	movs	r0, #26
	bl 0x0200abd8
	bl 0x0200ac80
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_02000468
	movs	r0, #193
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200aae0
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #129
	movs	r0, #26
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200ac08
	adds	r0, r5, #1
	bl 0x0200abd0
	b.n	.L_0200047c
.L_02000468:
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #26
	movs	r1, #3
	bl 0x0200aba0
	adds	r0, r5, #2
	bl 0x0200abd0
.L_0200047c:
	movs	r0, #26
	movs	r1, #0
	bl 0x0200abe8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001870
	.2byte 0x186f
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #26
	movs	r1, #4
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #193
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_020004b2
	ldr	r0, [pc, #104]
	bl 0x0200abd0
	b.n	.L_02000508
.L_020004b2:
	ldr	r5, [pc, #100]
	adds	r0, r5, #0
	bl 0x0200abd0
	movs	r1, #0
	movs	r0, #26
	bl 0x0200abd8
	bl 0x0200ac80
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_020004f4
	movs	r0, #193
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200aae0
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #129
	movs	r0, #26
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200ac08
	adds	r0, r5, #1
	bl 0x0200abd0
	b.n	.L_02000508
.L_020004f4:
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #26
	movs	r1, #3
	bl 0x0200aba0
	adds	r0, r5, #2
	bl 0x0200abd0
.L_02000508:
	movs	r0, #26
	movs	r1, #0
	bl 0x0200abe8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001d09
	.2byte 0x1d08
	.2byte 0x0000
	push	{lr}
	movs	r0, #145
	lsls	r0, r0, #4
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_02000538
	movs	r0, #123
	bl 0x0200ac90
	movs	r0, #16
	bl 0x0200ac30
	b.n	.L_020005b0
.L_02000538:
	movs	r0, #160
	lsls	r0, r0, #2
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_020005b0
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	movs	r3, #128
	movs	r1, #186
	lsls	r3, r3, #7
	lsls	r1, r1, #18
	ldr	r2, [pc, #92]
	movs	r0, #26
	bl 0x0200ab90
	movs	r0, #4
	bl 0x0200ab48
	movs	r3, #160
	lsls	r3, r3, #11
	movs	r1, #128
	movs	r2, #128
	str	r3, [r0, #40]
	lsls	r1, r1, #10
	movs	r0, #4
	lsls	r2, r2, #9
	bl 0x0200ab50
	movs	r1, #180
	movs	r2, #244
	movs	r0, #4
	lsls	r1, r1, #2
	bl 0x0200ab68
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #6
	bl 0x0200abf8
	movs	r3, #128
	movs	r1, #180
	movs	r2, #244
	lsls	r3, r3, #6
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	movs	r0, #4
	bl 0x0200ab90
	movs	r0, #160
	lsls	r0, r0, #2
	bl 0x0200aae0
	bl 0x02008404
.L_020005ac:
	bl 0x0200ab30
.L_020005b0:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0101
	push	{lr}
	movs	r0, #160
	lsls	r0, r0, #2
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_02000634
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	movs	r3, #128
	movs	r1, #186
	movs	r2, #128
	lsls	r3, r3, #7
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r0, #26
	bl 0x0200ab90
	movs	r0, #4
	bl 0x0200ab48
	movs	r3, #160
	lsls	r3, r3, #11
	movs	r1, #128
	movs	r2, #128
	str	r3, [r0, #40]
	lsls	r1, r1, #10
	movs	r0, #4
	lsls	r2, r2, #9
	bl 0x0200ab50
	movs	r1, #180
	movs	r2, #244
	movs	r0, #4
	lsls	r1, r1, #2
	bl 0x0200ab68
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #6
	bl 0x0200abf8
	movs	r3, #128
	movs	r1, #180
	movs	r2, #244
	lsls	r3, r3, #6
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	movs	r0, #4
	bl 0x0200ab90
	movs	r0, #160
	lsls	r0, r0, #2
	bl 0x0200aae0
	bl 0x02008490
	bl 0x0200ab30
.L_02000634:
	pop	{pc}
	.2byte 0x0000
	.global Func_02000638
	.thumb_func
Func_02000638:
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_0200064c
	ldr	r0, [pc, #24]
	b.n	.L_02000660
.L_0200064c:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_0200065e
	ldr	r0, [pc, #12]
	b.n	.L_02000660
.L_0200065e:
	ldr	r0, [pc, #12]
.L_02000660:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200b94c
	.4byte 0x0200b67c
	.2byte 0xb358
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r3, #45
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #109
	movs	r1, #14
	movs	r2, #3
	movs	r3, #3
	bl 0x0200aaf0
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #45
	movs	r2, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #51
	movs	r1, #10
	movs	r2, #3
	movs	r3, #3
	bl 0x0200aaf8
	bl 0x02008670
	add	sp, #8
	pop	{pc}
	.global Func_020006ac
	.thumb_func
Func_020006ac:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r0, #128
	adds	r3, r3, r2
	lsls	r0, r0, #4
	subs	r2, #172
	str	r2, [r3, #0]
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_020006d2
	bl 0x0200868c
	b.n	.L_020007a0
.L_020006d2:
	movs	r0, #26
	bl 0x0200ab48
	movs	r1, #0
	bl 0x0200ab00
	movs	r0, #26
	bl 0x0200ab48
	adds	r6, r0, #0
	movs	r0, #26
	bl 0x0200ab48
	ldrh	r0, [r0, #32]
	movs	r1, #3
	lsls	r0, r0, #2
	bl 0x0200aad0
	strh	r0, [r6, #32]
	movs	r0, #26
	bl 0x0200ab48
	ldr	r1, [r0, #80]
	movs	r3, #8
	ldrb	r2, [r1, #26]
	movs	r0, #26
	orrs	r3, r2
	strb	r3, [r1, #26]
	bl 0x0200ab48
	ldr	r2, [r0, #80]
	movs	r5, #3
	ldrb	r3, [r2, #17]
	movs	r0, #26
	ands	r5, r3
	movs	r3, #32
	orrs	r5, r3
	strb	r5, [r2, #17]
	bl 0x0200ab48
	ldr	r2, [r0, #80]
	movs	r0, #128
	movs	r3, #1
	lsls	r0, r0, #4
	strb	r3, [r2, #25]
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_02000770
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_02000770
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab88
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab88
	movs	r0, #160
	lsls	r0, r0, #2
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_020007a0
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab88
	b.n	.L_020007a0
.L_02000770:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_02000796
	movs	r0, #145
	lsls	r0, r0, #4
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_02000796
	movs	r0, #160
	lsls	r0, r0, #2
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_020007a0
.L_02000796:
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab88
.L_020007a0:
	ldr	r3, [pc, #144]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #9
	bne.n	.L_020007c2
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_020007c2
	bl 0x02008f80
.L_020007c2:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_02000824
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #137
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_02000824
	ldr	r3, [pc, #84]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #7
	bne.n	.L_0200080a
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #142
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_0200080a
	bl 0x020099d8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #142
	bl 0x0200aae0
.L_0200080a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #142
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_0200082e
	movs	r0, #33
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab88
	b.n	.L_0200082e
.L_02000824:
	movs	r0, #66
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab88
.L_0200082e:
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.global Func_02000838
	.thumb_func
Func_02000838:
	movs	r0, #0
	bx	lr
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x0200abd0
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200abd8
	bl 0x0200ac80
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_02000876
	movs	r0, #10
	bl 0x0200ab20
	adds	r0, r5, #1
	bl 0x0200abd0
	b.n	.L_02000882
.L_02000876:
	movs	r0, #20
	bl 0x0200ab20
	adds	r0, r5, #2
	bl 0x0200abd0
.L_02000882:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200abe8
	bl 0x0200ab30
	pop	{r5, r6, pc}
	.2byte 0x18d5
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_020008f0
	ldr	r5, [pc, #176]
	adds	r0, r5, #0
	bl 0x0200abd0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200abd8
	bl 0x0200ac80
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_020008da
	movs	r0, #10
	bl 0x0200ab20
	adds	r0, r5, #1
	bl 0x0200abd0
	b.n	.L_020008e6
.L_020008da:
	movs	r0, #20
	bl 0x0200ab20
	adds	r0, r5, #2
	bl 0x0200abd0
.L_020008e6:
	movs	r0, #8
	movs	r1, #0
	bl 0x0200abe8
	b.n	.L_02000946
.L_020008f0:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_02000906
	movs	r0, #14
	movs	r1, #4
	movs	r2, #0
	bl 0x0200abb8
.L_02000906:
	ldr	r5, [pc, #92]
	adds	r0, r5, #0
	bl 0x0200abd0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200abd8
	bl 0x0200ac80
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_02000932
	movs	r0, #10
	bl 0x0200ab20
	adds	r0, r5, #1
	bl 0x0200abd0
	b.n	.L_0200093e
.L_02000932:
	movs	r0, #20
	bl 0x0200ab20
	adds	r0, r5, #2
	bl 0x0200abd0
.L_0200093e:
	movs	r0, #8
	movs	r1, #0
	bl 0x0200abe8
.L_02000946:
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #14
	bl 0x0200abf0
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200aae0
	bl 0x0200ab30
	pop	{r5, pc}
	.4byte 0x00001d12
	.2byte 0x187b
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_020009c4
	ldr	r5, [pc, #188]
	adds	r0, r5, #0
	bl 0x0200abd0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200abd8
	bl 0x0200ac80
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_020009ae
	movs	r0, #10
	bl 0x0200ab20
	adds	r0, r5, #1
	bl 0x0200abd0
	b.n	.L_020009ba
.L_020009ae:
	movs	r0, #20
	bl 0x0200ab20
	adds	r0, r5, #2
	bl 0x0200abd0
.L_020009ba:
	movs	r0, #8
	movs	r1, #0
	bl 0x0200abe8
	b.n	.L_02000a26
.L_020009c4:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_020009e6
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #14
	bl 0x0200ac08
	movs	r0, #14
	movs	r1, #4
	movs	r2, #0
	bl 0x0200abb8
.L_020009e6:
	ldr	r5, [pc, #92]
	adds	r0, r5, #0
	bl 0x0200abd0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200abd8
	bl 0x0200ac80
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_02000a12
	movs	r0, #10
	bl 0x0200ab20
	adds	r0, r5, #1
	bl 0x0200abd0
	b.n	.L_02000a1e
.L_02000a12:
	movs	r0, #20
	bl 0x0200ab20
	adds	r0, r5, #2
	bl 0x0200abd0
.L_02000a1e:
	movs	r0, #8
	movs	r1, #0
	bl 0x0200abe8
.L_02000a26:
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #14
	bl 0x0200abf0
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200aae0
	bl 0x0200ab30
	pop	{r5, pc}
	.4byte 0x00001d12
	.2byte 0x187b
	.2byte 0x0000
	push	{lr}
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	movs	r1, #4
	movs	r2, #0
	movs	r0, #11
	bl 0x0200abb8
	ldr	r0, [pc, #96]
	bl 0x0200abd0
	movs	r1, #0
	movs	r0, #11
	bl 0x0200abd8
	movs	r0, #4
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_02000a9e
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200aae0
	movs	r0, #11
	movs	r1, #0
	bl 0x0200abe8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000ab8
.L_02000a9e:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #11
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200abe8
.L_02000ab8:
	bl 0x0200ab30
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2043
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	ldr	r0, [pc, #1016]
	bl 0x0200abd0
	movs	r0, #24
	movs	r1, #0
	movs	r2, #2
	bl 0x0200abe0
	movs	r0, #238
	movs	r1, #1
	movs	r2, #210
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200ac20
	movs	r0, #25
	movs	r1, #3
	movs	r2, #8
	bl 0x0200ac70
	movs	r1, #3
	negs	r1, r1
	movs	r2, #8
	movs	r0, #24
	bl 0x0200ac78
	movs	r0, #25
	bl 0x0200ab80
	movs	r1, #236
	movs	r2, #213
	lsls	r2, r2, #1
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x0200ab68
	movs	r0, #10
	bl 0x0200ab20
	ldr	r3, [pc, #940]
	movs	r1, #192
	mov	r8, r3
	movs	r3, #133
	lsls	r3, r3, #2
	add	r8, r3
	mov	r3, r8
	ldr	r0, [r3, #0]
	lsls	r1, r1, #8
	bl 0x0200abf8
	movs	r3, #192
	lsls	r3, r3, #8
	movs	r1, #16
	movs	r2, #1
	movs	r0, #32
	bl 0x0200ac68
	bl 0x0200ac28
	movs	r0, #32
	bl 0x0200ab80
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #32
	bl 0x0200abf0
	movs	r0, #30
	bl 0x0200ab20
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #32
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #2
	movs	r0, #32
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r2, #2
	movs	r1, #0
	movs	r0, #32
	bl 0x0200abe0
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #3
	movs	r0, #24
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r2, #2
	movs	r0, #25
	bl 0x0200abe0
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #25
	movs	r2, #0
	movs	r0, #24
	bl 0x0200abb8
	movs	r0, #10
	bl 0x0200ab20
	movs	r2, #0
	movs	r1, #24
	movs	r0, #25
	bl 0x0200abb8
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #3
	movs	r0, #24
	bl 0x0200aba0
	movs	r0, #30
	bl 0x0200ab20
	movs	r1, #24
	movs	r2, #0
	movs	r0, #25
	bl 0x0200abb8
	movs	r0, #10
	bl 0x0200ab20
	movs	r2, #0
	movs	r0, #25
	movs	r1, #32
	bl 0x0200abb8
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aba0
	movs	r0, #30
	bl 0x0200ab20
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #24
	bl 0x0200abf0
	movs	r0, #25
	bl 0x0200ab48
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #3
	movs	r0, #25
	bl 0x0200ac00
	movs	r1, #202
	movs	r2, #201
	movs	r0, #25
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x0200ab68
	movs	r0, #25
	movs	r1, #16
	movs	r2, #0
	bl 0x0200ac78
	movs	r1, #236
	movs	r2, #210
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #25
	bl 0x0200ab68
	movs	r0, #30
	bl 0x0200ab20
	movs	r1, #242
	movs	r2, #210
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #25
	bl 0x0200ab68
	movs	r0, #25
	bl 0x0200ab48
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r6, #1
	orrs	r3, r6
	movs	r1, #128
	strb	r3, [r0, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #25
	bl 0x0200abf0
	movs	r0, #60
	bl 0x0200ab20
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #25
	bl 0x0200abf0
	movs	r0, #10
	bl 0x0200ab20
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aba0
	movs	r2, #2
	movs	r1, #0
	movs	r0, #25
	bl 0x0200abe0
	movs	r0, #10
	bl 0x0200ab20
	movs	r0, #24
	movs	r1, #3
	bl 0x0200aba0
	movs	r2, #2
	movs	r0, #24
	movs	r1, #0
	bl 0x0200abe0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #25
	bl 0x0200abf8
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aba0
	movs	r0, #5
	bl 0x0200ab20
	movs	r2, #2
	movs	r1, #0
	movs	r0, #25
	bl 0x0200abe0
	movs	r0, #25
	bl 0x0200ab48
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #3
	ands	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #25
	bl 0x0200ac00
	movs	r2, #16
	movs	r0, #25
	movs	r1, #0
	negs	r2, r2
	bl 0x0200ac78
	movs	r1, #220
	movs	r2, #200
	movs	r0, #25
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x0200ab68
	movs	r1, #16
	negs	r1, r1
	movs	r2, #0
	movs	r0, #25
	bl 0x0200ac78
	movs	r0, #25
	bl 0x0200ab48
.L_02000d5a:
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #1
	orrs	r6, r3
	strb	r6, [r0, #0]
	movs	r2, #198
	movs	r0, #238
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200ac20
	movs	r1, #204
	movs	r2, #208
	movs	r0, #25
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x0200ab68
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #25
	bl 0x0200abf0
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #32
	bl 0x0200ac08
	movs	r2, #2
	movs	r1, #0
	movs	r0, #32
	bl 0x0200abe0
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #2
	movs	r0, #24
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r2, #2
	movs	r0, #24
	bl 0x0200abe0
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #32
	bl 0x0200ac08
	movs	r1, #128
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x0200ac08
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aba0
	movs	r1, #176
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #0
	movs	r2, #2
	movs	r0, #25
	bl 0x0200abe0
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #32
	movs	r2, #0
	movs	r0, #4
	bl 0x0200abc0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #10
	adds	r1, #255
	movs	r2, #50
	movs	r0, #25
	bl 0x0200ac08
	movs	r0, #24
	movs	r1, #0
	movs	r2, #2
	bl 0x0200abe0
	movs	r1, #4
	adds	r1, #255
	movs	r2, #20
	movs	r0, #25
	bl 0x0200ac08
	movs	r1, #176
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r2, #2
	movs	r1, #0
	movs	r0, #25
	bl 0x0200abe0
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #3
	movs	r0, #24
	bl 0x0200aba0
	movs	r0, #30
	bl 0x0200ab20
	movs	r1, #208
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r0, #24
	movs	r1, #0
	movs	r2, #2
	bl 0x0200abe0
	movs	r1, #152
	movs	r2, #152
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #24
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ab50
	movs	r1, #152
	movs	r2, #152
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #25
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ab50
	movs	r2, #8
	movs	r0, #24
	movs	r1, #3
	negs	r2, r2
	b.n	.L_02000ed8
	.2byte 0x0000
	.4byte 0x0000185f
	.2byte 0x0240
	.2byte 0x0200
.L_02000ed8:
	bl 0x0200ac70
	movs	r1, #3
	movs	r2, #8
	negs	r1, r1
	negs	r2, r2
	movs	r0, #25
	bl 0x0200ac70
	movs	r0, #24
	bl 0x0200ab80
	movs	r0, #25
	bl 0x0200ab80
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #24
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #10
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #3
	bl 0x0200ab98
	movs	r1, #3
	movs	r0, #4
	bl 0x0200aba0
	movs	r0, #30
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #2
	bl 0x0200ab98
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200ab48
	cmp	r0, #0
	beq.n	.L_02000f52
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #32
	bl 0x0200ab58
.L_02000f52:
	movs	r0, #32
	bl 0x0200ab80
	movs	r1, #0
	movs	r2, #0
	movs	r0, #32
	bl 0x0200ab88
	movs	r0, #145
	lsls	r0, r0, #4
	bl 0x0200aae0
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab88
	bl 0x0200ab30
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_02000f92
	b.n	.L_020014f8
.L_02000f92:
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	movs	r0, #248
	movs	r1, #1
	movs	r2, #210
	movs	r3, #1
	lsls	r0, r0, #15
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200ac20
	bl 0x0200ac28
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #33
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ab50
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #34
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ab50
	bl 0x0200ac38
	movs	r1, #172
	movs	r2, #166
	movs	r0, #33
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200ab88
	movs	r1, #172
	movs	r2, #162
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	movs	r0, #34
	bl 0x0200ab88
	ldr	r0, [pc, #956]
	bl 0x0200abd0
	bl 0x0200ac48
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r2, #186
	movs	r1, #156
	lsls	r2, r2, #1
	movs	r0, #33
	bl 0x0200ab60
	movs	r0, #20
	bl 0x0200ab20
	movs	r2, #186
	movs	r0, #34
	movs	r1, #156
	lsls	r2, r2, #1
	bl 0x0200ab60
	movs	r2, #194
	movs	r1, #76
	lsls	r2, r2, #1
	movs	r0, #4
	bl 0x0200ab60
	movs	r0, #33
	bl 0x0200ab80
	movs	r2, #190
	movs	r1, #108
	lsls	r2, r2, #1
	movs	r0, #33
	bl 0x0200ab60
	movs	r0, #34
	bl 0x0200ab80
	movs	r2, #190
	movs	r1, #124
	lsls	r2, r2, #1
	movs	r0, #34
	bl 0x0200ab60
	movs	r0, #4
	bl 0x0200ab80
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #4
	bl 0x0200abf0
	movs	r0, #33
	bl 0x0200ab80
	movs	r0, #34
	bl 0x0200ab80
	movs	r1, #192
	movs	r0, #33
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #34
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #192
	movs	r0, #33
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	movs	r2, #0
	movs	r0, #34
	lsls	r1, r1, #8
	bl 0x0200abf0
	movs	r1, #129
	movs	r0, #34
	lsls	r1, r1, #1
	bl 0x0200ac10
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #33
	bl 0x0200ac10
	movs	r0, #50
	bl 0x0200ab20
	movs	r0, #34
	movs	r1, #3
	bl 0x0200aba0
	movs	r1, #3
	movs	r0, #33
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r2, #170
	movs	r1, #102
	lsls	r2, r2, #1
	movs	r0, #33
	bl 0x0200ab60
	movs	r0, #20
	bl 0x0200ab20
	movs	r2, #170
	movs	r1, #102
	lsls	r2, r2, #1
	movs	r0, #34
	bl 0x0200ab60
	movs	r0, #33
	bl 0x0200ab80
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #11
	movs	r0, #33
	bl 0x0200ab88
	movs	r0, #34
	bl 0x0200ab80
	movs	r1, #128
	movs	r2, #128
	movs	r0, #34
	lsls	r1, r1, #11
	lsls	r2, r2, #11
	bl 0x0200ab88
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200abf0
	movs	r0, #40
	bl 0x0200ab20
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200ac08
	movs	r0, #40
	bl 0x0200ab20
	movs	r0, #120
	bl 0x0200ab20
	movs	r1, #204
	movs	r2, #170
	movs	r0, #33
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x0200ab88
	movs	r1, #204
	movs	r2, #170
	movs	r0, #34
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x0200ab88
	movs	r2, #182
	movs	r0, #34
	movs	r1, #100
	lsls	r2, r2, #1
	bl 0x0200ab68
	movs	r2, #190
	movs	r0, #34
	movs	r1, #124
	lsls	r2, r2, #1
	bl 0x0200ab60
	movs	r2, #190
	movs	r1, #100
	lsls	r2, r2, #1
	movs	r0, #33
	bl 0x0200ab68
	movs	r0, #34
	bl 0x0200ab80
	movs	r2, #198
	movs	r0, #34
	movs	r1, #132
	lsls	r2, r2, #1
	bl 0x0200ab60
	movs	r2, #198
	movs	r1, #108
	lsls	r2, r2, #1
	movs	r0, #33
	bl 0x0200ab68
	movs	r0, #34
	bl 0x0200ab80
	movs	r0, #4
	movs	r1, #33
	movs	r2, #0
	bl 0x0200abb8
	movs	r1, #192
	movs	r0, #34
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	movs	r0, #33
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #33
	bl 0x0200ac08
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r2, #0
	movs	r0, #34
	movs	r1, #33
	bl 0x0200abb8
	movs	r1, #3
	movs	r0, #34
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #34
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r2, #0
	movs	r1, #34
	movs	r0, #33
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #2
	movs	r0, #33
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r2, #10
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe0
	movs	r1, #3
	movs	r0, #34
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #34
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #33
	bl 0x0200ac08
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r1, #192
	movs	r0, #33
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #34
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #2
	movs	r0, #34
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r2, #10
	movs	r0, #34
	movs	r1, #0
	bl 0x0200abe0
	movs	r1, #3
	movs	r0, #33
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r2, #10
	movs	r0, #33
	bl 0x0200abe0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #33
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #33
	bl 0x0200ac08
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r2, #10
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe0
	movs	r0, #34
	movs	r1, #2
	bl 0x0200abb0
	movs	r1, #33
	movs	r2, #0
	movs	r0, #34
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #34
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r2, #0
	movs	r1, #34
	movs	r0, #33
	bl 0x0200abb8
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #4
	movs	r0, #33
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r1, #4
	movs	r2, #0
	movs	r0, #33
	bl 0x0200abb8
	movs	r0, #30
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r1, #6
	movs	r2, #30
	adds	r1, #255
	movs	r0, #4
	bl 0x0200ac08
	movs	r1, #2
	movs	r0, #34
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r2, #10
	movs	r0, #34
	bl 0x0200abe0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #33
	bl 0x0200ac08
	movs	r1, #0
	movs	r0, #33
	bl 0x0200abd8
	movs	r0, #4
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_020013b8
	movs	r2, #10
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020013d4
	.2byte 0x0000
	.2byte 0x18e8
	.2byte 0x0000
.L_020013b8:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #33
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
.L_020013d4:
	movs	r1, #34
	movs	r2, #0
	movs	r0, #33
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r2, #10
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe0
	movs	r1, #3
	movs	r0, #34
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #2
	movs	r0, #33
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #34
	bl 0x0200ac08
	movs	r1, #3
	movs	r0, #33
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r2, #10
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe0
	movs	r1, #2
	movs	r0, #34
	bl 0x0200abb0
	movs	r0, #60
	bl 0x0200ab20
	movs	r1, #3
	movs	r0, #34
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #34
	movs	r1, #6
	movs	r2, #0
	bl 0x0200ac78
	movs	r2, #158
	lsls	r2, r2, #1
	movs	r1, #164
	movs	r0, #34
	bl 0x0200ab68
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #128
	movs	r0, #33
	lsls	r1, r1, #8
	bl 0x0200abf8
	movs	r1, #3
	movs	r0, #33
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r2, #0
	movs	r0, #34
	movs	r1, #0
	bl 0x0200ab88
	movs	r1, #3
	movs	r0, #4
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #3
	movs	r0, #33
	bl 0x0200aba0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aae0
	movs	r0, #33
	movs	r1, #30
	movs	r2, #0
	bl 0x0200ac78
	movs	r2, #158
	movs	r1, #164
	lsls	r2, r2, #1
	movs	r0, #33
	bl 0x0200ab68
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab88
	movs	r0, #20
	bl 0x0200ab20
	bl 0x0200ab30
.L_020014f8:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	ldr	r0, [pc, #688]
	bl 0x0200abd0
	movs	r0, #24
	movs	r1, #0
	movs	r2, #2
	bl 0x0200abe0
	movs	r0, #238
	movs	r1, #1
	movs	r2, #200
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200ac20
	movs	r1, #3
	movs	r0, #24
	negs	r1, r1
	movs	r2, #8
	bl 0x0200ac78
	movs	r1, #236
	movs	r2, #213
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #4
	bl 0x0200ab68
	movs	r0, #10
	bl 0x0200ab20
	ldr	r3, [pc, #628]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #192
	ldr	r0, [r3, #0]
	lsls	r1, r1, #8
	bl 0x0200abf8
	movs	r3, #192
	lsls	r3, r3, #8
	movs	r1, #16
	movs	r2, #1
	movs	r0, #32
	bl 0x0200ac68
	bl 0x0200ac28
	movs	r0, #32
	bl 0x0200ab80
	movs	r2, #8
	movs	r0, #25
	movs	r1, #3
	bl 0x0200ac78
	movs	r0, #25
	movs	r1, #0
	bl 0x0200abf8
	movs	r1, #4
	movs	r0, #25
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #32
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #32
	bl 0x0200ac08
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #4
	movs	r1, #4
	bl 0x0200aba0
	movs	r1, #160
	movs	r0, #24
	lsls	r1, r1, #7
	bl 0x0200abf8
	movs	r0, #24
	movs	r1, #4
	bl 0x0200aba0
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r1, #208
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #32
	bl 0x0200ac08
	movs	r0, #32
	movs	r1, #6
	movs	r2, #15
	bl 0x0200aba8
	movs	r2, #23
	movs	r0, #32
	movs	r1, #6
	bl 0x0200aba8
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200ac08
	movs	r1, #24
	movs	r2, #0
	movs	r0, #4
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #32
	bl 0x0200ac08
	movs	r1, #208
	movs	r0, #32
	lsls	r1, r1, #8
	bl 0x0200abf8
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #128
	movs	r0, #25
	lsls	r1, r1, #6
	bl 0x0200abf8
	movs	r0, #25
	movs	r1, #3
	bl 0x0200aba0
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #32
	bl 0x0200abf0
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #0
	movs	r0, #25
	bl 0x0200abd8
	movs	r1, #176
	movs	r0, #4
	lsls	r1, r1, #8
	bl 0x0200abf8
	movs	r0, #4
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_020016d2
	movs	r0, #25
	bl 0x0200ab20
	movs	r1, #176
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200abe0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001700
.L_020016d2:
	movs	r0, #25
	bl 0x0200ab20
	movs	r1, #208
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #25
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
.L_02001700:
	movs	r1, #2
	movs	r0, #32
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #176
	movs	r0, #32
	lsls	r1, r1, #8
	bl 0x0200abf8
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #25
	bl 0x0200ac08
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200abe0
	movs	r1, #2
	movs	r0, #24
	bl 0x0200abb0
	movs	r0, #20
.L_02001740:
	bl 0x0200ab20
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r0, #137
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_020017f0
	ldr	r0, [pc, #100]
	bl 0x0200abd0
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #32
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r0, #32
	bl 0x0200abd8
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abf0
	movs	r0, #4
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_020017c8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #3
	bl 0x0200aba0
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020017f0
	.4byte 0x00001cf0
	.4byte 0x02000240
	.2byte 0x1cfc
	.2byte 0x0000
.L_020017c8:
	movs	r0, #20
	bl 0x0200ab20
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #32
	adds	r3, #1
	movs	r1, #4
	strh	r3, [r2, #0]
	bl 0x0200aba0
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
.L_020017f0:
	ldr	r0, [pc, #476]
	bl 0x0200abd0
	movs	r1, #208
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #208
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r0, #32
	bl 0x0200abe8
	movs	r0, #40
	bl 0x0200ab20
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #24
	bl 0x0200ac08
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #24
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #176
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #176
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200ac08
	movs	r1, #4
	movs	r0, #25
	bl 0x0200aba0
	movs	r0, #10
	bl 0x0200ab20
	movs	r1, #128
	movs	r0, #25
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200abf0
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #32
	bl 0x0200ac08
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #4
	bl 0x0200ac08
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #160
	movs	r0, #24
	lsls	r1, r1, #7
	bl 0x0200abf8
	movs	r1, #4
	movs	r0, #24
	bl 0x0200aba0
	movs	r0, #10
	bl 0x0200ab20
	movs	r2, #10
	movs	r0, #24
	movs	r1, #0
	bl 0x0200abe0
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aba0
	movs	r0, #10
	bl 0x0200ab20
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200abe0
	movs	r1, #152
	movs	r2, #152
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #24
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ab50
	movs	r1, #152
	movs	r2, #152
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #25
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ab50
	movs	r2, #8
	movs	r0, #24
	movs	r1, #3
	negs	r2, r2
	bl 0x0200ac70
	movs	r1, #3
	movs	r2, #8
	negs	r1, r1
	negs	r2, r2
	movs	r0, #25
	bl 0x0200ac70
	movs	r0, #24
	bl 0x0200ab80
	movs	r0, #25
	bl 0x0200ab80
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #24
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #32
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #3
	bl 0x0200ab98
	movs	r1, #3
	movs	r0, #4
	bl 0x0200aba0
	movs	r0, #10
	bl 0x0200ab20
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aae0
	movs	r0, #32
	movs	r1, #2
	bl 0x0200ab98
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ab48
	cmp	r0, #0
	beq.n	.L_020019ae
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #32
	bl 0x0200ab58
.L_020019ae:
	movs	r0, #32
	bl 0x0200ab80
	movs	r2, #0
	movs	r0, #32
	movs	r1, #0
	bl 0x0200ab88
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	bl 0x0200abf8
	bl 0x0200ab30
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00001cff
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	bl 0x0200ac38
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #37
	bl 0x0200aad8
	cmp	r0, #0
	bne.n	.L_02001a08
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #38
	bl 0x0200aad8
	cmp	r0, #0
	beq.n	.L_02001a12
.L_02001a08:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #118
	bl 0x0200aae0
.L_02001a12:
	ldr	r0, [pc, #724]
	bl 0x0200abd0
	movs	r0, #34
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abe0
	movs	r2, #140
	movs	r0, #4
	movs	r1, #150
	lsls	r2, r2, #1
	bl 0x0200ab68
	movs	r0, #4
	movs	r1, #16
	movs	r2, #0
	bl 0x0200ac70
	movs	r3, #192
	movs	r0, #5
	movs	r1, #8
	movs	r2, #16
	lsls	r3, r3, #8
	bl 0x0200ac68
	movs	r3, #192
	movs	r0, #32
	movs	r1, #0
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200ac68
	movs	r1, #8
	movs	r3, #192
	movs	r0, #6
	negs	r1, r1
	movs	r2, #16
	lsls	r3, r3, #8
	bl 0x0200ac68
	movs	r1, #16
	movs	r3, #192
	lsls	r3, r3, #8
	negs	r1, r1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200ac68
	movs	r0, #4
	bl 0x0200ab80
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	ldr	r3, [pc, #604]
	movs	r5, #128
	ldrh	r1, [r3, #52]
	ldrh	r2, [r3, #54]
	ldr	r0, [r3, #48]
	bl 0x0200aae8
	lsls	r5, r5, #7
	movs	r1, #153
	adds	r3, r5, #0
	movs	r0, #34
	lsls	r1, r1, #16
	ldr	r2, [pc, #584]
	bl 0x0200ab90
	movs	r0, #34
	movs	r1, #0
	movs	r2, #4
	bl 0x0200ac78
	movs	r0, #34
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #34
	movs	r1, #0
	bl 0x0200abd8
	movs	r0, #32
	movs	r1, #4
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #5
	movs	r1, #4
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #7
	movs	r1, #4
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #6
	movs	r1, #4
	movs	r2, #0
	bl 0x0200abb8
	adds	r1, r5, #0
	movs	r0, #34
	movs	r2, #0
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #4
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_02001b7e
	movs	r1, #192
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	adds	r1, r5, #0
	movs	r2, #0
	movs	r0, #34
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #34
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #33
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r0, #34
	bl 0x0200abe8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001bee
.L_02001b7e:
	movs	r1, #192
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	adds	r1, r5, #0
	movs	r2, #0
	movs	r0, #34
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #34
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #33
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #34
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abe0
.L_02001bee:
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #34
	bl 0x0200ac08
	movs	r0, #34
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abe0
	movs	r0, #34
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #34
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abe0
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200ac08
	movs	r2, #0
	movs	r1, #5
	movs	r0, #6
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #6
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #6
	movs	r2, #0
	movs	r0, #5
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #5
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #33
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #5
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #34
	bl 0x0200ac08
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #34
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abe0
	movs	r0, #34
	movs	r1, #16
	movs	r2, #8
	bl 0x0200ac78
	movs	r1, #128
	movs	r0, #34
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200abf0
	movs	r0, #210
	bl 0x0200ab18
	cmp	r0, #0
	blt.n	.L_02001cf4
	movs	r0, #210
	movs	r1, #3
	bl 0x0200ac50
	movs	r1, #0
	movs	r0, #210
	bl 0x0200ab38
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	movs	r1, #192
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #24
	bl 0x0200aae0
	b.n	.L_02001d88
	.4byte 0x000020cd
	.4byte 0x0200b308
	.2byte 0x0000
	.2byte 0x0103
.L_02001cf4:
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r3, [r6, #108]
	movs	r5, #226
	lsls	r5, r5, #1
	ldrsh	r2, [r3, r5]
	ldr	r0, [pc, #932]
	mov	r8, r2
	movs	r2, #1
	add	r8, r2
	bl 0x0200abd0
	movs	r0, #210
	movs	r1, #2
	bl 0x0200ab10
	ldr	r3, [r6, #108]
	movs	r1, #5
	adds	r3, r3, r5
	ldrh	r0, [r3, #0]
	adds	r2, r0, #1
	lsls	r0, r0, #16
	strh	r2, [r3, #0]
	asrs	r0, r0, #16
	bl 0x0200ab08
	movs	r0, #34
	movs	r1, #2
	bl 0x0200abb0
	movs	r0, #34
	movs	r1, #0
	bl 0x0200abe8
	movs	r2, #0
	movs	r0, #34
	movs	r1, #16
	bl 0x0200ac78
	movs	r0, #210
	movs	r1, #2
	bl 0x0200ab10
	ldr	r3, [r6, #108]
	movs	r1, #5
	adds	r3, r3, r5
	ldrh	r0, [r3, #0]
	adds	r2, r0, #1
	lsls	r0, r0, #16
	strh	r2, [r3, #0]
	asrs	r0, r0, #16
	bl 0x0200ab08
	movs	r1, #16
	movs	r2, #0
	movs	r0, #34
	negs	r1, r1
	bl 0x0200ac78
	movs	r1, #128
	movs	r0, #34
	lsls	r1, r1, #7
	bl 0x0200abf8
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #16
	movs	r0, #66
	lsls	r2, r2, #17
	bl 0x0200ab88
	ldr	r3, [r6, #108]
	mov	r1, r8
	strh	r1, [r3, r5]
.L_02001d88:
	movs	r1, #16
	movs	r2, #8
	negs	r2, r2
	movs	r0, #34
	negs	r1, r1
	bl 0x0200ac78
	movs	r1, #128
	movs	r0, #34
	lsls	r1, r1, #7
	bl 0x0200abf8
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #34
	bl 0x0200ac08
	movs	r0, #34
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abe0
	movs	r0, #34
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r2, #0
	movs	r0, #34
	movs	r1, #0
	bl 0x0200abe0
	movs	r0, #32
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #4
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #5
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #6
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #7
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #192
	movs	r0, #34
	lsls	r1, r1, #8
	bl 0x0200abf8
	movs	r2, #4
	movs	r0, #34
	movs	r1, #153
	adds	r2, #255
	bl 0x0200ab68
	movs	r0, #34
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab88
	movs	r0, #32
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #33
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #0
	movs	r0, #33
	bl 0x0200abe8
	movs	r0, #78
	bl 0x0200ac90
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #4
	bl 0x0200ac08
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200ac08
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #7
	bl 0x0200ac08
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200ac08
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #32
	bl 0x0200ac08
	movs	r0, #4
	movs	r1, #33
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #7
	movs	r1, #33
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #6
	movs	r1, #33
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #5
	movs	r1, #33
	movs	r2, #0
	bl 0x0200abb8
	movs	r2, #0
	movs	r1, #33
	movs	r0, #32
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #1
	movs	r0, #33
	bl 0x0200ac18
	movs	r0, #60
	bl 0x0200ab20
	movs	r0, #36
	bl 0x0200ac90
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #33
	bl 0x0200ac08
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #128
	movs	r0, #33
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	movs	r0, #6
	movs	r1, #5
	movs	r2, #0
	bl 0x0200abc0
	movs	r1, #32
	movs	r2, #0
	movs	r0, #7
	bl 0x0200abc0
	movs	r0, #40
	bl 0x0200ab20
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x0200ac08
	movs	r1, #2
	movs	r0, #33
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #4
	movs	r1, #33
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #7
	movs	r1, #33
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #6
	movs	r1, #33
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #5
	movs	r1, #33
	movs	r2, #0
	bl 0x0200abb8
	movs	r2, #0
	movs	r1, #33
	movs	r0, #32
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #128
	movs	r0, #33
	lsls	r1, r1, #8
	bl 0x0200abf8
	movs	r1, #0
	movs	r0, #33
	bl 0x0200abf8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200ac08
	movs	r2, #0
	movs	r1, #5
	movs	r0, #6
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #6
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200ac08
	movs	r0, #5
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #33
	bl 0x0200ac08
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #4
	movs	r2, #0
	movs	r0, #33
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r0, #33
	bl 0x0200abd8
	movs	r0, #4
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	beq.n	.L_020020ac
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200ac08
	movs	r0, #40
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #5
	movs	r2, #0
	movs	r0, #6
	bl 0x0200abc0
	movs	r0, #40
	bl 0x0200ab20
	movs	r0, #6
	movs	r1, #33
	movs	r2, #0
	bl 0x0200abb8
	movs	r2, #0
	movs	r1, #33
	movs	r0, #5
	bl 0x0200abb8
	movs	r0, #40
	bl 0x0200ab20
	movs	r0, #5
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #6
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #33
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #10
	adds	r1, #255
	movs	r0, #33
	movs	r2, #30
	bl 0x0200ac08
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	b.n	.L_02002114
	.2byte 0x2186
	.2byte 0x0000
.L_020020ac:
	ldr	r0, [pc, #904]
	bl 0x0200abd0
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #5
	movs	r2, #0
	movs	r0, #6
	bl 0x0200abc0
	movs	r0, #40
	bl 0x0200ab20
	movs	r0, #6
	movs	r1, #33
	movs	r2, #0
	bl 0x0200abb8
	movs	r2, #0
	movs	r1, #33
	movs	r0, #5
	bl 0x0200abb8
	movs	r0, #40
	bl 0x0200ab20
	movs	r0, #5
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #6
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #33
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #10
	adds	r1, #255
	movs	r0, #6
	movs	r2, #30
	bl 0x0200ac08
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
.L_02002114:
	ldr	r0, [pc, #804]
	bl 0x0200abd0
	movs	r1, #2
	movs	r0, #33
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #32
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #4
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #7
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #6
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #5
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #33
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #10
	adds	r1, #255
	movs	r2, #0
	movs	r0, #4
	bl 0x0200ac08
	movs	r1, #10
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200ac08
	movs	r1, #10
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200ac08
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #32
	bl 0x0200ac08
	movs	r0, #60
	bl 0x0200ab20
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #33
	bl 0x0200ac08
	movs	r0, #33
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abe0
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200ac08
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200ac08
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200ac08
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #32
	bl 0x0200ac08
	movs	r0, #33
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r2, #0
	movs	r1, #0
	movs	r0, #33
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #32
	bl 0x0200ac08
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #2
	movs	r0, #33
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #33
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #33
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #4
	bl 0x0200ac08
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200ac08
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200ac08
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x0200ac08
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #33
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #33
	bl 0x0200ac08
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #2
	movs	r0, #4
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #33
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #33
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #2
	movs	r0, #33
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200ac08
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200ac08
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200ac08
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200ac08
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200ac08
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #33
	bl 0x0200ac08
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #0
	movs	r0, #33
	bl 0x0200abf8
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #33
	bl 0x0200abf8
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r0, #33
	bl 0x0200abf8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #33
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #2
	movs	r0, #33
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #33
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r0, #33
	bl 0x0200abd8
	movs	r0, #4
	movs	r1, #0
	bl 0x0200ab40
	cmp	r0, #0
	bne.n	.L_02002440
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #33
	bl 0x0200ac08
	movs	r2, #0
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002468
	.2byte 0x0000
	.4byte 0x000020e5
	.2byte 0x20e7
	.2byte 0x0000
.L_02002440:
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #33
	bl 0x0200ac08
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	movs	r0, #33
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abe0
.L_02002468:
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #32
	bl 0x0200ac08
	movs	r0, #32
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #33
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #4
	bl 0x0200ab98
	movs	r0, #33
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #33
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #2
	movs	r0, #33
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #33
	movs	r1, #0
	bl 0x0200abe8
	movs	r2, #0
	movs	r1, #0
	movs	r0, #33
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r0, #33
	bl 0x0200abe8
	movs	r0, #78
	bl 0x0200ac90
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	bl 0x0200ac20
	movs	r0, #33
	movs	r1, #24
	movs	r2, #24
	bl 0x0200ac78
	movs	r0, #33
	movs	r1, #132
	movs	r2, #0
	bl 0x0200ac78
	movs	r2, #0
	movs	r0, #33
	movs	r1, #0
	bl 0x0200ab88
	bl 0x0200ac60
	movs	r0, #4
	movs	r1, #1
	bl 0x0200ac18
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200ac08
	movs	r0, #6
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #5
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #5
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x0200ac08
	movs	r1, #4
	movs	r2, #0
	movs	r0, #7
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abe0
	movs	r0, #32
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #10
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #2
	movs	r0, #6
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #6
	movs	r1, #0
	bl 0x0200abe8
	movs	r2, #0
	movs	r1, #6
	movs	r0, #5
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #5
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #32
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #32
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #20
	movs	r0, #7
	bl 0x0200ac08
	movs	r0, #7
	movs	r1, #32
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #4
	movs	r1, #32
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #6
	movs	r1, #32
	movs	r2, #0
	bl 0x0200abb8
	movs	r1, #32
	movs	r2, #0
	movs	r0, #5
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200abe0
	movs	r2, #0
	movs	r1, #7
	movs	r0, #32
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r2, #0
	movs	r1, #5
	movs	r0, #6
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r0, #6
	bl 0x0200abe8
	movs	r0, #5
	bl 0x0200ab48
	movs	r1, #10
	bl 0x0200abc8
	movs	r0, #5
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #5
	bl 0x0200abf0
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #5
	movs	r1, #0
	bl 0x0200abe8
	movs	r2, #0
	movs	r1, #7
	movs	r0, #4
	bl 0x0200abc0
	movs	r0, #40
	bl 0x0200ab20
	movs	r1, #2
	movs	r0, #32
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #128
	movs	r0, #32
	lsls	r1, r1, #7
	bl 0x0200abf8
	movs	r1, #0
	movs	r0, #32
	bl 0x0200abe8
	movs	r0, #5
	bl 0x0200ab48
	movs	r1, #0
	bl 0x0200abc8
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200ac08
	movs	r2, #0
	movs	r1, #32
	movs	r0, #5
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #5
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200ac08
	movs	r1, #0
	movs	r0, #6
	bl 0x0200abe8
	movs	r0, #5
	bl 0x0200ab48
	movs	r1, #10
	bl 0x0200abc8
	movs	r2, #0
	movs	r1, #6
	movs	r0, #5
	bl 0x0200abb8
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #5
	movs	r1, #0
	bl 0x0200abe8
	movs	r1, #2
	movs	r0, #32
	bl 0x0200abb0
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #0
	movs	r0, #32
	bl 0x0200abe8
	movs	r0, #5
	bl 0x0200ab48
	movs	r1, #0
	bl 0x0200abc8
	movs	r0, #4
	movs	r1, #32
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #5
	movs	r1, #32
	movs	r2, #0
	bl 0x0200abb8
	movs	r0, #6
	movs	r1, #32
	movs	r2, #0
	bl 0x0200abb8
	movs	r1, #32
	movs	r2, #0
	movs	r0, #7
	bl 0x0200abb8
	movs	r0, #40
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #4
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #5
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #6
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #7
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #5
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #32
	bl 0x0200ac08
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #6
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #7
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abf8
	movs	r0, #32
	movs	r1, #0
	bl 0x0200abe8
	movs	r0, #4
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #6
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #5
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #4
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #32
	movs	r1, #3
	bl 0x0200ab98
	movs	r0, #20
	bl 0x0200ab20
	movs	r0, #32
	movs	r2, #153
	lsls	r2, r2, #8
	ldr	r1, [pc, #268]
	adds	r2, #153
	bl 0x0200ab50
	movs	r0, #32
	movs	r1, #2
	bl 0x0200ab98
	movs	r0, #4
	bl 0x0200ab48
	cmp	r0, #0
	beq.n	.L_020028c2
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #32
	bl 0x0200ab58
.L_020028c2:
	movs	r0, #32
	bl 0x0200ab80
	movs	r0, #32
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab88
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #204]
	adds	r2, #153
	bl 0x0200ab50
	movs	r0, #5
	movs	r1, #2
	bl 0x0200ab98
	movs	r0, #4
	bl 0x0200ab48
	cmp	r0, #0
	beq.n	.L_02002900
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200ab58
.L_02002900:
	movs	r0, #5
	bl 0x0200ab80
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab88
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #144]
	adds	r2, #153
	bl 0x0200ab50
	movs	r0, #6
	movs	r1, #2
	bl 0x0200ab98
	movs	r0, #4
	bl 0x0200ab48
	cmp	r0, #0
	beq.n	.L_0200293e
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200ab58
.L_0200293e:
	movs	r0, #6
	bl 0x0200ab80
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab88
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #80]
	adds	r2, #153
	bl 0x0200ab50
	movs	r0, #7
	movs	r1, #2
	bl 0x0200ab98
	movs	r0, #4
	bl 0x0200ab48
	cmp	r0, #0
	beq.n	.L_0200297c
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200ab58
.L_0200297c:
	movs	r0, #7
	bl 0x0200ab80
	movs	r1, #0
	movs	r2, #0
	movs	r0, #7
	bl 0x0200ab88
	movs	r0, #152
	lsls	r0, r0, #4
	bl 0x0200aae0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #142
	bl 0x0200aae0
	bl 0x0200ab30
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x3333
	.2byte 0x0001
	push	{lr}
	movs	r0, #145
	lsls	r0, r0, #4
	bl 0x0200aae0
	pop	{pc}
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aae0
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r5, #16
	cmp	r3, #34
	ble.n	.L_020029e2
	movs	r5, #15
.L_020029e2:
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200abf0
	movs	r0, #10
	bl 0x0200ab20
	movs	r0, #4
	bl 0x0200ab48
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #4
	movs	r1, #0
	movs	r0, #4
	bl 0x0200ab78
	movs	r0, #4
	bl 0x0200ab80
	movs	r0, #4
	bl 0x0200ab48
	movs	r1, #0
	bl 0x0200ab00
	movs	r0, #4
	movs	r1, #13
	bl 0x0200ab98
	movs	r2, #16
	movs	r1, #0
	movs	r0, #4
	bl 0x0200ab78
	movs	r0, #4
	bl 0x0200ab80
	movs	r1, #10
	movs	r0, #4
	bl 0x0200ab98
	movs	r0, #10
	bl 0x0200ab20
	movs	r0, #123
	bl 0x0200ac90
	adds	r0, r5, #0
	bl 0x0200ac30
	bl 0x0200ab30
	pop	{r5, pc}
	push	{lr}
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	movs	r2, #14
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x0200ac78
	movs	r0, #4
	movs	r1, #16
	movs	r2, #0
	bl 0x0200ac78
	bl 0x0200a9c8
	pop	{pc}
	push	{lr}
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	movs	r1, #2
	movs	r2, #6
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x0200ac78
	movs	r1, #14
	movs	r2, #8
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x0200ac78
	bl 0x0200a9c8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x0200ab28
	movs	r0, #0
	bl 0x0200ac58
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200abf0
	bl 0x0200a9c8
	pop	{pc}
	.2byte 0x0000
	.section .rodata,"a",%progbits
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
	.4byte 0x0000002c
	.4byte 0x1010102e
	.4byte 0xffffffff
	.4byte 0x1020102d
	.4byte 0xffffffff
	.4byte 0x1030202d
	.4byte 0xffffffff
	.4byte 0x1040802d
	.4byte 0xffffffff
	.4byte 0x1050a02d
	.4byte 0xffffffff
	.4byte 0x1060f02d
	.4byte 0xffffffff
	.4byte 0x1070502d
	.4byte 0xffffffff
	.4byte 0x1080902d
	.4byte 0xffffffff
	.4byte 0x1091402d
	.4byte 0xffffffff
	.4byte 0x10a0202e
	.4byte 0xffffffff
	.4byte 0x10b09002
	.4byte 0xffffffff
	.4byte 0x10f01030
	.4byte 0xffffffff
	.4byte 0x11003030
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00013000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0001c000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001a000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x0001c000
	.4byte 0xffff004d
	.4byte 0x00000001
	.4byte 0x01120000
	.4byte 0x00000000
	.4byte 0x01fe0000
	.4byte 0x00014000
	.4byte 0xffff004e
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00012000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00004000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0xffff004e
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01030000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0001e000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001c000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00015000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00015000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0000c000
	.4byte 0x191100bb
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001c000
	.4byte 0x191100c0
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001c000
	.4byte 0xffff00cd
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00014000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00014000
	.4byte 0x18ab00c6
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00014000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0058
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0x08ab00ba
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00960000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00015000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0001c000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001a000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0001c000
	.4byte 0xffff004d
	.4byte 0x00000001
	.4byte 0x01120000
	.4byte 0x00000000
	.4byte 0x01fe0000
	.4byte 0x00014000
	.4byte 0xffff004e
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00014000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00004000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00015000
	.4byte 0xffff004e
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01030000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0001e000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001a000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00015000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00015000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x191100bb
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0x191100c0
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff00cd
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00014000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00014000
	.4byte 0xffff00c6
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00014000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0017
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01140000
	.4byte 0x00012000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff00ee
	.4byte 0x00000002
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x01890000
	.4byte 0x00014000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00270001
	.4byte 0x00020001
	.4byte 0xffff0006
	.4byte 0x00270003
	.4byte 0x00020001
	.4byte 0xffff0006
	.4byte 0x0200b2fc
	.4byte 0x00030014
	.4byte 0x0200b2f0
	.4byte 0x0017000d
	.4byte 0x0200b2f0
	.4byte 0x00110011
	.4byte 0x0200b2f0
	.4byte 0x00120017
	.4byte 0x0200b2f0
	.4byte 0x000a001c
	.4byte 0x0200b2f0
	.4byte 0x0006001c
	.4byte 0x0200b2f0
	.4byte 0x00060009
	.4byte 0x0200b2f0
	.4byte 0x00180013
	.4byte 0x0200b2f0
	.4byte 0x00000000
	.4byte 0x0200b2fc
	.4byte 0x00030011
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x0200806d
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x0200806d
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x0200851d
	.4byte 0x00000002
	.4byte 0x09100013
	.4byte 0x0200a9ad
	.4byte 0x00000002
	.4byte 0x09100014
	.4byte 0x02008ac5
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001875
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001876
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001877
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001878
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001879
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000187a
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008895
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000187e
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0000187f
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001880
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001881
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001882
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001883
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001884
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001885
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001886
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x0000186b
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x0000186c
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x02008405
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x0200883d
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x000018d4
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x02008109
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x0200818d
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x02008211
	.4byte 0x00000000
	.4byte 0xffff0023
	.4byte 0x02008211
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001887
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001888
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001889
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000188a
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000188b
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000188c
	.4byte 0x00008d15
	.4byte 0x0300040e
	.4byte 0x02008969
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000188d
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000188e
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000188f
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001890
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001891
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001892
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001893
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001894
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001895
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001896
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000186d
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x0000186e
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001872
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x000018d9
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x000018d8
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x000018db
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x000018dd
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte 0x000018e0
	.4byte 0x00008d15
	.4byte 0xffff0023
	.4byte 0x000018e0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x0200806d
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x0200806d
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x020085b9
	.4byte 0x00000002
	.4byte 0x09140013
	.4byte 0x0200a9b9
	.4byte 0x00000002
	.4byte 0x09140014
	.4byte 0x020094fd
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001d0c
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001d0d
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001d0e
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001d0f
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001d10
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001d11
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008895
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001d15
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001d16
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001d17
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001d18
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001d19
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001d1a
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001d1b
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001d1c
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001d1d
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001d04
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001d05
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x02008491
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x02008109
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x0200818d
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x02008211
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001d1e
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001d1f
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001d20
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001d21
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001d22
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001d23
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001d24
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001d25
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001d26
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001d27
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001d28
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001d29
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001d2a
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001d2b
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001d2c
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001d2d
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001d06
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001d07
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001d0b
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x00001d4d
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x00001d4f
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte 0x00001d52
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x0200806d
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x0200806d
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x0200806d
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000002
	.4byte 0x098e0015
	.4byte 0x0200836d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002040
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002041
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002042
	.4byte 0x00000000
	.4byte 0x0301000b
	.4byte 0x02008a49
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002046
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002047
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002048
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002049
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000204a
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0000204b
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0000204c
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000204d
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0000204e
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0000204f
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002050
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002051
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002052
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x0000203c
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x0000203d
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00001870
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x02008109
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x0200818d
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x02008211
	.4byte 0x00000000
	.4byte 0xffff0021
	.4byte 0x020082bd
	.4byte 0x00000000
	.4byte 0xffff0023
	.4byte 0x020083b9
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002055
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002056
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002057
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002058
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002059
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000205a
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000205b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000205c
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002061
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002062
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002063
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002064
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002065
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002066
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002067
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002068
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000203e
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x0000203f
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x00002119
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x0000211b
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte 0x0000211e
	.4byte 0x00008d15
	.4byte 0xffff0021
	.4byte 0x02008321
	.4byte 0x00008d15
	.4byte 0xffff0023
	.4byte 0x00002060
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
