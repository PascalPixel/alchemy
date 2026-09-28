.syntax unified
	.thumb
	push	{lr}
	movs	r0, #13
	movs	r1, #26
	bl 0x02008a48
	pop	{pc}
	.global Func_02000044
	.thumb_func
Func_02000044:
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_0200005c
	ldr	r0, [pc, #24]
	b.n	.L_02000068
.L_0200005c:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000066
	ldr	r0, [pc, #24]
	b.n	.L_02000068
.L_02000066:
	ldr	r0, [pc, #24]
.L_02000068:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000007
	.4byte 0x02008d98
	.4byte 0x00000008
	.4byte 0x02008e40
	.2byte 0x8cd8
	.2byte 0x0200
	.global Func_02000084
	.thumb_func
Func_02000084:
	movs	r0, #0
	bx	lr
	.global Func_02000088
	.thumb_func
Func_02000088:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8ed0
	.2byte 0x0200
	.global Func_02000090
	.thumb_func
Func_02000090:
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_020000a8
	ldr	r0, [pc, #24]
	b.n	.L_020000b4
.L_020000a8:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020000b2
	ldr	r0, [pc, #24]
	b.n	.L_020000b4
.L_020000b2:
	ldr	r0, [pc, #24]
.L_020000b4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000007
	.4byte 0x02008f58
	.4byte 0x00000006
	.4byte 0x02009048
	.2byte 0x8f10
	.2byte 0x0200
	.global Func_020000d0
	.thumb_func
Func_020000d0:
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_020000e8
	ldr	r0, [pc, #24]
	b.n	.L_020000f4
.L_020000e8:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020000f2
	ldr	r0, [pc, #24]
	b.n	.L_020000f4
.L_020000f2:
	ldr	r0, [pc, #24]
.L_020000f4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000007
	.4byte 0x020090e4
	.4byte 0x00000008
	.4byte 0x02009180
	.2byte 0x9078
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #80]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_0200012c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #148
	bl 0x02008968
.L_0200012c:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #148
	bl 0x02008960
	cmp	r0, #0
	beq.n	.L_02000160
	movs	r1, #228
	movs	r2, #156
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x020089d0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #8
	movs	r2, #0
	bl 0x02008a98
	movs	r0, #8
	bl 0x02008998
	movs	r1, #8
	bl 0x02008a88
.L_02000160:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #102
	movs	r2, #56
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #1
	movs	r0, #108
	movs	r1, #38
	bl 0x02008978
	ldr	r3, [pc, #212]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_0200019a
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #149
	bl 0x02008968
.L_0200019a:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #149
	bl 0x02008960
	cmp	r0, #0
	beq.n	.L_0200022a
	movs	r2, #230
	movs	r0, #13
	ldr	r1, [pc, #172]
	lsls	r2, r2, #18
	bl 0x020089d0
	movs	r1, #156
	movs	r0, #14
	lsls	r1, r1, #18
	ldr	r2, [pc, #164]
	bl 0x020089d0
	movs	r1, #158
	movs	r2, #230
	lsls	r2, r2, #18
	lsls	r1, r1, #18
	movs	r0, #15
	bl 0x020089d0
	movs	r0, #13
	bl 0x02008998
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r1, #192
	strh	r3, [r0, #6]
	lsls	r1, r1, #7
	movs	r0, #13
	bl 0x02008a90
	movs	r0, #14
	bl 0x02008998
	movs	r5, #0
	strh	r5, [r0, #6]
	movs	r1, #0
	movs	r0, #14
	bl 0x02008a90
	movs	r0, #15
	bl 0x02008998
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r0, #6]
	movs	r1, #0
	movs	r0, #15
	bl 0x02008a90
	movs	r0, #13
	bl 0x02008998
	movs	r5, #1
	adds	r0, #89
	strb	r5, [r0, #0]
	movs	r0, #14
	bl 0x02008998
	adds	r0, #89
	strb	r5, [r0, #0]
	movs	r0, #15
	bl 0x02008998
	adds	r0, #89
	strb	r5, [r0, #0]
.L_0200022a:
	movs	r0, #8
	movs	r1, #4
	bl 0x020089d8
	movs	r0, #9
	movs	r1, #4
	bl 0x020089d8
	movs	r0, #10
	movs	r1, #3
	bl 0x020089d8
	movs	r0, #11
	movs	r1, #4
	bl 0x020089d8
	movs	r0, #12
	movs	r1, #3
	bl 0x020089d8
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x024e0000
	.2byte 0x0000
	.2byte 0x03ab
	push	{lr}
	movs	r0, #10
	adds	r0, #255
	bl 0x02008960
	cmp	r0, #0
	bne.n	.L_02000286
	ldr	r3, [pc, #20]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_02000286
	bl 0x02008800
.L_02000286:
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	.global Func_0200028c
	.thumb_func
Func_0200028c:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #128
	adds	r3, r3, r1
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	ldr	r3, [pc, #44]
	adds	r2, #224
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_020002b6
	bl 0x02008110
	b.n	.L_020002cc
.L_020002b6:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_020002c2
	bl 0x02008168
	b.n	.L_020002cc
.L_020002c2:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020002cc
	bl 0x02008264
.L_020002cc:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000006
	.4byte 0x00000007
	.2byte 0x0008
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #102
	movs	r2, #56
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #38
	movs	r2, #1
	movs	r3, #1
	movs	r0, #108
	bl 0x02008978
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #222
	bl 0x02008968
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #0
	sub	sp, #8
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	bl 0x02008998
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #158
	bl 0x02008aa0
	movs	r5, #2
	movs	r1, #36
	movs	r2, #71
	movs	r3, #8
	movs	r0, #66
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02008970
	movs	r0, #4
	bl 0x02008958
	movs	r3, #8
	movs	r1, #36
	movs	r2, #71
	movs	r0, #68
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02008970
	movs	r0, #4
	bl 0x02008958
	movs	r2, #16
	movs	r1, #3
	negs	r2, r2
	movs	r0, #0
	bl 0x020089c0
	movs	r0, #123
	bl 0x02008aa0
	adds	r0, r6, #0
	bl 0x02008a38
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	ldr	r3, [pc, #24]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #4
	str	r2, [r3, #0]
	movs	r0, #123
	bl 0x02008aa0
	adds	r0, r5, #0
	bl 0x02008a38
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	.global Func_020003a8
	.thumb_func
Func_020003a8:
	movs	r0, #0
	bx	lr
	push	{r5, r6, lr}
	bl 0x02008988
	movs	r0, #0
	bl 0x02008a70
	ldr	r0, [pc, #200]
	bl 0x02008a00
	ldr	r6, [pc, #200]
	movs	r3, #133
.L_020003c2:
	lsls	r3, r3, #2
	adds	r5, r6, r3
	movs	r1, #236
	movs	r2, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x020089b0
	movs	r0, #228
	movs	r1, #1
	movs	r2, #140
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x02008a28
	bl 0x02008a30
	ldr	r0, [r5, #0]
	bl 0x020089c8
	movs	r1, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02008a10
	movs	r0, #10
	bl 0x02008980
	movs	r1, #6
	movs	r2, #20
	movs	r0, #8
	bl 0x020089e8
	movs	r0, #20
	bl 0x02008980
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x02008a08
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #8
	bl 0x02008a18
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x02008a08
	movs	r1, #3
	movs	r0, #8
	bl 0x020089e0
	movs	r0, #30
	bl 0x02008980
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x02008a08
	movs	r1, #228
	movs	r2, #156
	lsls	r2, r2, #1
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x020089b8
	ldr	r0, [pc, #48]
	movs	r1, #99
	bl 0x02008a50
	ldr	r0, [pc, #44]
	movs	r1, #98
	bl 0x02008a58
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r6, r6, r3
	movs	r3, #2
	strb	r3, [r6, #0]
	movs	r0, #9
	movs	r1, #1
	bl 0x02008a40
	bl 0x02008990
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00001609
	.4byte 0x02000240
	.4byte 0x00000006
	.2byte 0x0004
	.2byte 0x0000
	push	{r5, r6, lr}
	bl 0x02008988
	movs	r0, #0
	bl 0x02008a70
	ldr	r0, [pc, #308]
	bl 0x02008a00
	ldr	r6, [pc, #308]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r6, r3
	movs	r2, #4
	movs	r1, #0
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x02008a80
	movs	r0, #30
	bl 0x02008980
	ldr	r0, [r5, #0]
	bl 0x02008998
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	ldr	r0, [r5, #0]
	movs	r2, #12
	bl 0x02008a80
	movs	r0, #10
	bl 0x02008980
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #16
	bl 0x02008a80
	movs	r0, #30
	bl 0x02008980
	movs	r1, #154
	movs	r2, #228
	movs	r0, #13
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x020089d0
	movs	r1, #150
	movs	r2, #230
	movs	r0, #13
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x020089b8
	movs	r1, #154
	movs	r2, #228
	movs	r0, #15
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x020089d0
	movs	r1, #128
	movs	r0, #13
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02008a10
	movs	r1, #158
	movs	r2, #230
	movs	r0, #15
	lsls	r1, r1, #2
.L_0200052e:
	lsls	r2, r2, #2
	bl 0x020089b8
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02008a10
	movs	r1, #154
	movs	r2, #228
	movs	r0, #14
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x020089d0
	movs	r1, #154
	movs	r2, #230
	lsls	r2, r2, #2
	movs	r0, #14
	lsls	r1, r1, #2
	bl 0x020089b8
	movs	r1, #2
	movs	r0, #13
	bl 0x020089f0
	movs	r0, #20
	bl 0x02008980
	movs	r0, #13
	movs	r1, #0
	movs	r2, #10
	bl 0x02008a08
	movs	r2, #10
	movs	r0, #14
	movs	r1, #0
	bl 0x02008a08
	movs	r1, #3
	movs	r0, #15
	bl 0x020089e0
	movs	r0, #30
	bl 0x02008980
	movs	r2, #10
	movs	r0, #15
	movs	r1, #0
	bl 0x02008a08
	movs	r0, #13
	movs	r1, #3
	bl 0x020089e0
	movs	r0, #14
	movs	r1, #3
	bl 0x020089d8
	movs	r0, #15
	movs	r1, #3
	bl 0x020089e0
	ldr	r0, [pc, #48]
	movs	r1, #99
	bl 0x02008a50
	ldr	r0, [pc, #44]
	movs	r1, #98
	bl 0x02008a58
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r6, r6, r3
	movs	r3, #2
	strb	r3, [r6, #0]
	movs	r0, #9
	movs	r1, #2
	bl 0x02008a40
	bl 0x02008990
	pop	{r5, r6, pc}
	.4byte 0x0000160d
	.4byte 0x02000240
	.4byte 0x00000007
	.2byte 0x0004
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #8
	movs	r2, #0
	movs	r1, #4
	bl 0x020089e8
	ldr	r5, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x02008998
	cmp	r0, #0
	beq.n	.L_02000614
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #8
	bl 0x020089a0
.L_02000614:
	movs	r0, #20
	bl 0x02008980
	ldr	r0, [pc, #44]
	movs	r1, #99
	bl 0x02008a50
	ldr	r0, [pc, #40]
	movs	r1, #98
	bl 0x02008a58
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r5, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r0, #9
	movs	r1, #3
	bl 0x02008a40
	bl 0x02008990
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x00000008
	.2byte 0x0004
	.2byte 0x0000
	push	{r5, lr}
	bl 0x02008988
	movs	r0, #0
	bl 0x02008a70
	movs	r1, #236
	movs	r2, #240
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #8
	bl 0x020089d0
	movs	r0, #16
	bl 0x02008980
	ldr	r5, [pc, #164]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #8
	movs	r2, #0
	bl 0x020089f8
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x02008a18
	movs	r0, #8
	movs	r1, #1
	bl 0x02008a20
	bl 0x02008a30
	movs	r1, #4
	movs	r2, #0
	movs	r0, #8
	bl 0x020089e8
	movs	r0, #16
	bl 0x02008980
	movs	r0, #8
	movs	r1, #4
	movs	r2, #0
	bl 0x020089e8
	movs	r1, #240
	movs	r2, #236
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #8
	bl 0x020089a8
	movs	r0, #30
	bl 0x02008980
	movs	r0, #8
	movs	r1, #2
	movs	r2, #0
	bl 0x020089e8
	movs	r1, #248
	movs	r2, #232
	movs	r0, #8
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x020089a8
	movs	r0, #8
	movs	r1, #4
	movs	r2, #0
	bl 0x020089e8
	movs	r1, #244
	movs	r2, #228
	movs	r0, #8
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x020089a8
	movs	r0, #8
	movs	r1, #4
	movs	r2, #0
	bl 0x020089e8
	movs	r1, #248
	movs	r2, #224
	movs	r0, #8
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x020089a8
	bl 0x020085e8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	bl 0x02008988
	movs	r0, #0
	bl 0x02008a70
	movs	r1, #236
	movs	r2, #240
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #8
	bl 0x020089d0
	movs	r0, #16
	bl 0x02008980
	ldr	r5, [pc, #188]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #8
	movs	r2, #0
	bl 0x020089f8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x02008a18
	movs	r0, #16
	bl 0x02008980
	movs	r1, #244
	movs	r2, #220
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x020089b0
	movs	r0, #16
	bl 0x02008980
	movs	r0, #8
	movs	r1, #1
	bl 0x02008a20
	bl 0x02008a30
	movs	r1, #4
	movs	r2, #0
	movs	r0, #8
	bl 0x020089e8
	movs	r0, #16
	bl 0x02008980
	movs	r0, #8
	movs	r1, #4
	movs	r2, #0
	bl 0x020089e8
	movs	r1, #244
	movs	r2, #240
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #8
	bl 0x020089a8
	movs	r0, #30
	bl 0x02008980
	movs	r0, #8
	movs	r1, #2
	movs	r2, #0
	bl 0x020089e8
	movs	r1, #252
	movs	r2, #236
	movs	r0, #8
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x020089a8
	movs	r0, #8
	movs	r1, #4
	movs	r2, #0
	bl 0x020089e8
	movs	r1, #130
	movs	r2, #232
	movs	r0, #8
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x020089a8
	movs	r0, #8
	movs	r1, #4
	movs	r2, #0
	bl 0x020089e8
	movs	r1, #132
	movs	r2, #228
	movs	r0, #8
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x020089a8
	bl 0x020085e8
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	bl 0x02008988
	movs	r0, #0
	bl 0x02008a70
	ldr	r0, [pc, #180]
	bl 0x02008a00
	movs	r1, #128
	movs	r2, #216
	movs	r0, #9
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x020089d0
	ldr	r5, [pc, #164]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #132
	movs	r2, #216
	ldr	r0, [r5, #0]
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x020089d0
	bl 0x02008a60
	bl 0x02008a68
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x02008a08
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x020089e0
	movs	r0, #30
	bl 0x02008980
	movs	r1, #2
	movs	r0, #9
	bl 0x020089f0
	movs	r0, #20
	bl 0x02008980
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x02008a08
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x020089e0
	movs	r0, #30
	bl 0x02008980
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #150
	bl 0x02008968
	movs	r0, #9
	movs	r1, #2
	bl 0x020089d8
	ldr	r0, [r5, #0]
	bl 0x02008998
	cmp	r0, #0
	beq.n	.L_020008a6
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #9
	bl 0x020089a0
.L_020008a6:
	movs	r0, #9
	bl 0x020089c8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #9
	bl 0x020089d0
	movs	r0, #10
	bl 0x02008980
	bl 0x02008990
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001613
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	bl 0x02008988
	movs	r0, #0
	bl 0x02008a70
	ldr	r0, [pc, #116]
	bl 0x02008a00
	movs	r1, #16
	movs	r3, #0
	negs	r1, r1
	movs	r2, #0
	movs	r0, #16
	bl 0x02008a78
	movs	r0, #16
	bl 0x020089c8
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #16
	movs	r1, #0
	bl 0x02008a08
	movs	r0, #16
	movs	r1, #2
	bl 0x020089d8
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x02008998
	cmp	r0, #0
	beq.n	.L_02000928
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #16
	bl 0x020089a0
.L_02000928:
	movs	r0, #16
	bl 0x020089c8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #16
	bl 0x020089d0
	movs	r0, #10
	bl 0x02008980
	ldr	r0, [r5, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x02008a80
	bl 0x02008990
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001616
	.4byte 0x02000240
	.section .text.x02008aa8,"ax",%progbits
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000008
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000008
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0xffff0000
	.4byte 0x00000098
	.4byte 0x400001f8
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0001
	.4byte 0x00000098
	.4byte 0xc0000208
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x80000118
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0003
	.4byte 0x00000088
	.4byte 0x40000130
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0004
	.4byte 0x00000168
	.4byte 0x00000128
	.4byte 0x01500000
	.4byte 0x02e00020
	.4byte 0x000001f0
	.4byte 0xffff0005
	.4byte 0x00000298
	.4byte 0x800001a8
	.4byte 0x01500000
	.4byte 0x02e00020
	.4byte 0x000001f0
	.4byte 0xffff0063
	.4byte 0x000001d8
	.4byte 0x80000138
	.4byte 0x01500000
	.4byte 0x02e00020
	.4byte 0x000001f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000028
	.4byte 0x00000130
	.4byte 0x00100000
	.4byte 0x01a00020
	.4byte 0x000001e0
	.4byte 0xffff0002
	.4byte 0x00000188
	.4byte 0x80000170
	.4byte 0x00100000
	.4byte 0x01a00020
	.4byte 0x000001e0
	.4byte 0xffff0003
	.4byte 0x00000028
	.4byte 0x00000390
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0xffff0004
	.4byte 0x000002a8
	.4byte 0x80000388
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0xffff0005
	.4byte 0x00000268
	.4byte 0x40000398
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0xffff0063
	.4byte 0x00000268
	.4byte 0x40000398
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0xc0000108
	.4byte 0x00180000
	.4byte 0x01180018
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000000b8
	.4byte 0x400000a8
	.4byte 0x00180000
	.4byte 0x01180018
	.4byte 0x00000100
	.4byte 0xffff0003
	.4byte 0x00000118
	.4byte 0x40000158
	.4byte 0x00d00000
	.4byte 0x02c00120
	.4byte 0x00000280
	.4byte 0xffff0004
	.4byte 0x00000298
	.4byte 0xc0000268
	.4byte 0x00d00000
	.4byte 0x02c00120
	.4byte 0x00000280
	.4byte 0xffff0063
	.4byte 0x00000210
	.4byte 0x400001b0
	.4byte 0x00d00000
	.4byte 0x02c00120
	.4byte 0x00000280
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00201007
	.4byte 0x00404007
	.4byte 0x00502004
	.4byte 0x00000007
	.4byte 0x00102006
	.4byte 0x00203007
	.4byte 0x00302007
	.4byte 0x00404006
	.4byte 0x00501008
	.4byte 0x00000008
	.4byte 0x00105007
	.4byte 0x00203008
	.4byte 0x00302008
	.4byte 0x0040b009
	.4byte 0x000001ff
	.4byte 0xffff00f7
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0000c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00025000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00025000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x03900000
	.4byte 0x00025000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x03900000
	.4byte 0x00025000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x03700000
	.4byte 0x00025000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00022000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00022000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00022000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008309
	.4byte 0x00000002
	.4byte 0x08940014
	.4byte 0x020083ad
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x020088cd
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000160c
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
	.4byte 0x08950005
	.4byte 0x02008495
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x020088cd
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001610
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001611
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001612
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008039
	.4byte 0x00000013
	.4byte 0x0fb40064
	.4byte 0x001000c3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008379
	.4byte 0x00000002
	.4byte 0x08960016
	.4byte 0x02008651
	.4byte 0x00000002
	.4byte 0x08960017
	.4byte 0x0200871d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
