.syntax unified
	.thumb
	push	{lr}
	movs	r0, #8
	movs	r1, #75
	bl 0x0200d9a0
	pop	{pc}
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe3c0
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
	.2byte 0xe408
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r2, [r5, #16]
	ldr	r1, [r5, #8]
	movs	r0, #0
	bl 0x0200d808
	movs	r3, #128
	lsls	r3, r3, #10
	adds	r2, r5, #0
	adds	r0, r0, r3
	adds	r2, #85
	movs	r3, #0
	str	r0, [r5, #20]
	str	r0, [r5, #12]
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r5, #28]
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.global Func_02000088
	.thumb_func
Func_02000088:
	push	{lr}
	ldr	r3, [pc, #68]
	movs	r2, #240
	movs	r0, #16
	ldrsh	r1, [r3, r0]
	ldr	r3, [pc, #64]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_020000ae
	movs	r3, #7
	ldr	r2, [pc, #52]
	ands	r1, r3
	lsls	r3, r1, #2
	ldr	r0, [r2, r3]
	b.n	.L_020000ce
.L_020000ae:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_020000b8
	ldr	r0, [pc, #44]
	b.n	.L_020000ce
.L_020000b8:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_020000c2
	ldr	r0, [pc, #44]
	b.n	.L_020000ce
.L_020000c2:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_020000cc
	ldr	r0, [pc, #40]
	b.n	.L_020000ce
.L_020000cc:
	ldr	r0, [pc, #40]
.L_020000ce:
	pop	{pc}
	.4byte 0x0200244c
	.4byte 0x02000240
	.4byte 0x000000f7
	.4byte 0x0200f024
	.4byte 0x000000f9
	.4byte 0x0200eea4
	.4byte 0x000000fa
	.4byte 0x0200eed4
	.4byte 0x000000f8
	.4byte 0x0200ef04
	.2byte 0xe46c
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #52]
	adds	r6, r0, #0
	movs	r1, #26
	ldrsh	r2, [r5, r1]
	movs	r1, #28
	ldrsh	r3, [r5, r1]
	movs	r0, #1
	cmp	r2, r3
	blt.n	.L_02000132
	movs	r0, #0
	cmp	r2, r3
	bgt.n	.L_02000132
	movs	r0, #161
	lsls	r0, r0, #2
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02000126
	movs	r0, #0
	b.n	.L_02000132
.L_02000126:
	movs	r2, #20
	ldrsh	r3, [r5, r2]
	movs	r0, #1
	cmp	r3, #0
	bne.n	.L_02000132
	adds	r0, r6, #0
.L_02000132:
	pop	{r5, r6, pc}
	.2byte 0x244c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	subs	r3, r6, #1
	ldr	r5, [pc, #208]
	cmp	r3, #1
	bhi.n	.L_02000194
	movs	r0, #22
	ldrsh	r3, [r5, r0]
	ldrh	r2, [r5, #22]
	cmp	r3, #0
	bgt.n	.L_02000184
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_0200017c
	movs	r0, #0
	bl 0x02008330
	cmp	r0, #0
	bne.n	.L_0200020c
	ldr	r3, [pc, #172]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r2, #16
	ldr	r0, [r3, #0]
	movs	r1, #0
	negs	r2, r2
	bl 0x0200da00
	b.n	.L_0200020c
.L_0200017c:
	adds	r0, r6, #4
	bl 0x0200d990
	b.n	.L_0200018e
.L_02000184:
	subs	r3, r2, #1
	strh	r3, [r5, #22]
	adds	r0, r6, #0
	bl 0x0200d990
.L_0200018e:
	movs	r0, #123
	bl 0x0200da68
.L_02000194:
	subs	r3, r6, #3
	cmp	r3, #1
	bhi.n	.L_020001fa
	ldrh	r3, [r5, #22]
	movs	r2, #192
	adds	r3, #1
	strh	r3, [r5, #22]
	lsls	r2, r2, #10
	lsls	r3, r3, #16
	cmp	r3, r2
	ble.n	.L_020001de
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_020001cc
	movs	r0, #0
	bl 0x020080fc
	cmp	r0, #0
	bne.n	.L_020001cc
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #133
	bl 0x0200d7b0
.L_020001cc:
	movs	r0, #128
	bl 0x0200da68
	bl 0x0200da58
	adds	r0, r6, #4
	bl 0x0200d990
	b.n	.L_020001ea
.L_020001de:
	adds	r0, r6, #0
	bl 0x0200d990
	movs	r0, #123
	bl 0x0200da68
.L_020001ea:
	movs	r3, #22
	ldrsh	r2, [r5, r3]
	movs	r0, #26
	ldrsh	r3, [r5, r0]
	ldrh	r1, [r5, #22]
	cmp	r2, r3
	ble.n	.L_020001fa
	strh	r1, [r5, #26]
.L_020001fa:
	movs	r1, #22
	ldrsh	r3, [r5, r1]
	movs	r0, #18
	ldrsh	r2, [r5, r0]
	lsls	r3, r3, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	ldrh	r3, [r5, r3]
	strh	r3, [r5, #16]
.L_0200020c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200244c
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	cmp	r0, #0
	bne.n	.L_02000260
	cmp	r1, #3
	bne.n	.L_02000236
	movs	r3, #23
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	movs	r0, #0
	movs	r1, #120
	movs	r2, #1
	movs	r3, #2
	bl 0x0200d820
.L_02000236:
	movs	r3, #57
	movs	r2, #3
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #127
	movs	r1, #3
	movs	r2, #1
	movs	r3, #2
	bl 0x0200d820
	movs	r3, #23
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #127
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d818
	b.n	.L_0200029e
.L_02000260:
	cmp	r1, #3
	bne.n	.L_02000276
	movs	r3, #9
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	movs	r0, #0
	movs	r1, #120
	movs	r2, #1
	movs	r3, #2
	bl 0x0200d820
.L_02000276:
	movs	r3, #43
	movs	r2, #3
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #127
	movs	r1, #3
	movs	r2, #1
	movs	r3, #2
	bl 0x0200d820
	movs	r3, #9
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #1
	movs	r1, #127
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d818
.L_0200029e:
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	cmp	r0, #0
	bne.n	.L_020002ec
	cmp	r1, #3
	bne.n	.L_020002c2
	movs	r3, #28
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	movs	r0, #0
.L_020002b8:
	movs	r1, #120
	movs	r2, #1
	movs	r3, #2
	bl 0x0200d820
.L_020002c2:
	movs	r3, #62
	movs	r2, #3
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #127
	movs	r1, #3
	movs	r2, #1
	movs	r3, #2
	bl 0x0200d820
	movs	r3, #28
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #1
	movs	r1, #127
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d818
	b.n	.L_0200032a
.L_020002ec:
	cmp	r1, #3
	bne.n	.L_02000302
	movs	r3, #4
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	movs	r0, #0
	movs	r1, #120
	movs	r2, #1
	movs	r3, #2
	bl 0x0200d820
.L_02000302:
	movs	r3, #38
	movs	r2, #3
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #127
	movs	r1, #3
	movs	r2, #1
	movs	r3, #2
	bl 0x0200d820
	movs	r3, #4
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #127
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d818
.L_0200032a:
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #220]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	bl 0x0200d8a0
	adds	r5, r0, #0
	movs	r0, #144
	movs	r3, #192
	lsls	r0, r0, #4
	lsls	r3, r3, #18
	adds	r0, #255
	ldr	r6, [r3, #108]
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02000400
	bl 0x0200d880
	movs	r0, #0
	bl 0x0200d9e8
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	lsls	r1, r1, #4
	lsls	r2, r2, #4
	adds	r1, #8
	adds	r2, #8
	ldr	r0, [r7, #0]
	bl 0x0200d8c8
	movs	r0, #229
	bl 0x0200da68
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r7, #0]
	lsls	r1, r1, #7
	bl 0x0200d948
	ldr	r5, [pc, #136]
	movs	r1, #13
	adds	r0, r5, #0
	bl 0x0200d850
	movs	r2, #203
	lsls	r2, r2, #4
	adds	r3, r6, r2
	adds	r5, #1
	strh	r5, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #178
	adds	r2, r6, r3
	movs	r3, #4
	strh	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #180
	adds	r2, r6, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	ldr	r0, [r7, #0]
	movs	r1, #1
	bl 0x0200d898
	cmp	r0, #0
	bne.n	.L_02000400
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	ldr	r0, [r7, #0]
	bl 0x0200d948
	movs	r0, #10
	bl 0x0200d878
	movs	r1, #22
	ldr	r0, [r7, #0]
	bl 0x0200d8f8
	movs	r0, #10
	bl 0x0200d878
	movs	r1, #10
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r7, #0]
	bl 0x0200d960
	movs	r0, #30
	bl 0x0200d878
	bl 0x0200d868
	movs	r0, #9
	bl 0x0200d990
	movs	r0, #1
	b.n	.L_0200040c
.L_02000400:
	movs	r0, #193
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7b0
	movs	r0, #0
.L_0200040c:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x29c6
	.2byte 0x0000
	push	{lr}
	movs	r0, #193
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7b8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #192]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200d8a0
	ldr	r7, [pc, #180]
	adds	r6, r0, #0
	bl 0x0200d880
	movs	r0, #0
	bl 0x0200d9e8
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	lsls	r1, r1, #4
	lsls	r2, r2, #4
	ldr	r0, [r5, #0]
	adds	r1, #8
	adds	r2, #8
	bl 0x0200d8c8
	bl 0x0200cdac
	movs	r0, #0
	bl 0x020080fc
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200047c
	movs	r3, #22
	ldrsh	r0, [r7, r3]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #129
	adds	r0, r0, r2
	bl 0x0200d7b0
.L_0200047c:
	movs	r0, #124
	bl 0x0200da68
	movs	r2, #18
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	bne.n	.L_020004b4
	movs	r0, #9
	movs	r1, #6
	bl 0x0200d8f8
	movs	r0, #195
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7b0
	cmp	r5, #0
	beq.n	.L_020004dc
	movs	r0, #10
	movs	r1, #6
	bl 0x0200d8f8
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #134
	bl 0x0200d7b0
	b.n	.L_020004dc
.L_020004b4:
	movs	r0, #11
	movs	r1, #6
	bl 0x0200d8f8
	movs	r0, #195
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7b0
	cmp	r5, #0
	beq.n	.L_020004dc
	movs	r0, #12
	movs	r1, #6
	bl 0x0200d8f8
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #134
	bl 0x0200d7b0
.L_020004dc:
	movs	r0, #161
	lsls	r0, r0, #2
	bl 0x0200d7b0
	bl 0x0200d888
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x244c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r1, #0
	mov	sl, r2
	adds	r5, r0, #0
	bl 0x0200da40
	bl 0x0200d9b8
	ldr	r2, [pc, #160]
	adds	r6, r0, #0
	mov	r8, r2
	bl 0x0200d880
	movs	r0, #0
	bl 0x0200d9e8
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200d858
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200d858
	movs	r1, #1
	ldr	r0, [pc, #128]
	bl 0x0200d850
	movs	r0, #125
	bl 0x0200da68
	adds	r0, r6, #0
	movs	r1, #5
	bl 0x0200d900
	cmp	r6, #9
	beq.n	.L_02000548
	cmp	r6, #11
	bne.n	.L_02000554
.L_02000548:
	movs	r0, #195
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7b8
	b.n	.L_0200055e
.L_02000554:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #134
	bl 0x0200d7b8
.L_0200055e:
	movs	r0, #195
.L_02000560:
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02000598
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #134
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02000598
	mov	r2, r8
	movs	r3, #18
	ldrsh	r0, [r2, r3]
	movs	r3, #22
	ldrsh	r1, [r2, r3]
	bl 0x02008218
	movs	r0, #138
	bl 0x0200da68
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #130
	bl 0x0200d7b0
.L_02000598:
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x0200d870
	bl 0x0200d888
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0200244c
	.2byte 0x29c5
	.2byte 0x0000
	push	{lr}
	cmp	r0, #9
	beq.n	.L_020005c4
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #134
	cmp	r0, #11
	bne.n	.L_020005ca
.L_020005c4:
	movs	r3, #195
	lsls	r3, r3, #1
	adds	r3, #255
.L_020005ca:
	adds	r0, r3, #0
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_020005ec
	bl 0x0200d880
	movs	r0, #0
	bl 0x0200d9e8
	ldr	r0, [pc, #60]
	movs	r1, #1
	bl 0x0200d850
	bl 0x0200d888
	b.n	.L_02000618
.L_020005ec:
	ldr	r0, [pc, #48]
	movs	r1, #13
	bl 0x0200d850
	ldr	r3, [pc, #44]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200d898
	cmp	r0, #0
	bne.n	.L_02000618
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
	strh	r3, [r2, #0]
.L_02000618:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x000029c4
	.4byte 0x000029c3
	.4byte 0x02000240
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	bl 0x0200d8a0
	cmp	r0, #0
	beq.n	.L_0200065c
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	ldr	r3, [r0, #16]
	ldr	r2, [r0, #8]
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	asrs	r2, r2, #20
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r1, r1, r2
	ldrb	r2, [r1, #3]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r1, #3]
.L_0200065c:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	bl 0x0200d8a0
	cmp	r0, #0
	beq.n	.L_02000690
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	ldr	r3, [r0, #16]
	ldr	r2, [r0, #8]
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	asrs	r2, r2, #20
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r1, r1, r2
	ldrb	r2, [r1, #3]
	movs	r3, #127
	ands	r3, r2
	strb	r3, [r1, #3]
.L_02000690:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x688b6e81
	.4byte 0x1c036083
	.4byte 0x781a3363
	.4byte 0x041268cb
	.4byte 0x60c3189b
	.4byte 0x6103690b
	.4byte 0x6143694b
	.2byte 0x2001
	.2byte 0x4770
	push	{r5, r6, lr}
	bl 0x0200d8a0
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02000706
	movs	r0, #234
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, #255
	bl 0x0200d7d8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000706
	movs	r1, #0
	bl 0x0200d830
	ldr	r1, [pc, #44]
	adds	r0, r5, #0
	bl 0x0200d7d0
	str	r6, [r5, #104]
	adds	r0, r5, #0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r0, #0]
	adds	r3, r5, #0
	movs	r1, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	adds	r2, r5, #0
	adds	r3, #4
	strb	r1, [r3, #0]
	adds	r2, #99
	movs	r3, #16
	strb	r3, [r2, #0]
	str	r5, [r6, #104]
.L_02000706:
	pop	{r5, r6, pc}
	.2byte 0xda7c
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r1, #0
	bl 0x0200d8a0
	cmp	r0, #0
	beq.n	.L_02000724
	ldr	r0, [r0, #104]
	cmp	r0, #0
	beq.n	.L_02000724
	adds	r3, r0, #0
	adds	r3, #99
	strb	r5, [r3, #0]
.L_02000724:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	adds	r0, r1, #0
	cmp	r3, #0
	bne.n	.L_02000740
	bl 0x02008660
.L_02000740:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	adds	r0, r1, #0
	cmp	r3, #0
	bne.n	.L_02000760
	bl 0x0200862c
.L_02000760:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #17
	bl 0x0200d8a0
	movs	r3, #0
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r0, #130
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200d7b0
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x0200d9c0
	pop	{pc}
	push	{lr}
	sub	sp, #8
	bl 0x0200d8a0
	cmp	r0, #0
	beq.n	.L_020007b0
	ldr	r2, [r0, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
.L_0200079e:
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #14
	movs	r1, #27
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d820
.L_020007b0:
	add	sp, #8
	pop	{pc}
	push	{lr}
	ldr	r3, [pc, #44]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #14
	cmp	r3, r2
	bne.n	.L_020007d8
	movs	r0, #130
	lsls	r0, r0, #2
	bl 0x0200d7b0
	b.n	.L_020007e0
.L_020007d8:
	movs	r0, #130
	lsls	r0, r0, #2
	bl 0x0200d7b8
.L_020007e0:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #16
	bl 0x0200d8a0
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #23
	bne.n	.L_020007fe
	bl 0x0200d9c0
	b.n	.L_02000810
.L_020007fe:
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #6
	movs	r2, #0
	bl 0x0200d918
.L_02000810:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	adds	r0, r1, #0
	adds	r3, r0, #0
	subs	r3, #15
	cmp	r3, #1
	bhi.n	.L_02000828
	bl 0x02008660
.L_02000828:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r3, r5, #0
	subs	r3, #15
	cmp	r3, #1
	bhi.n	.L_02000840
	adds	r0, r5, #0
	bl 0x0200862c
	b.n	.L_020008a2
.L_02000840:
	adds	r0, r5, #0
	bl 0x0200d8a0
	ldr	r3, [r0, #8]
	movs	r5, #0
	asrs	r0, r3, #20
	cmp	r0, #13
	bne.n	.L_02000852
	movs	r5, #17
.L_02000852:
	cmp	r0, #15
	bne.n	.L_02000858
	movs	r5, #18
.L_02000858:
	cmp	r0, #17
	bne.n	.L_0200085e
	movs	r5, #19
.L_0200085e:
	cmp	r0, #19
	bne.n	.L_02000864
	movs	r5, #20
.L_02000864:
	cmp	r5, #0
	beq.n	.L_020008a2
	movs	r3, #130
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r7, r5, r3
	adds	r0, r7, #0
	bl 0x0200d7a8
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020008a2
	adds	r0, r5, #0
	bl 0x0200d8a0
	movs	r1, #0
	str	r6, [r0, #108]
	bl 0x0200d840
	adds	r0, r5, #0
	movs	r1, #6
	bl 0x0200d8f8
	movs	r1, #16
	adds	r0, r5, #0
	negs	r1, r1
	bl 0x0200870c
	adds	r0, r7, #0
	bl 0x0200d7b0
.L_020008a2:
	pop	{r5, r6, r7, pc}
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d9b8
	movs	r3, #130
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r0, r0, r3
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_020008c8
	bl 0x0200d9c0
.L_020008c8:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	bl 0x0200d8a0
	movs	r1, #16
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200870c
	movs	r3, #130
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r5, r5, r3
	adds	r0, r5, #0
	bl 0x0200d7b8
	cmp	r6, #0
	beq.n	.L_02000902
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r1, #0]
.L_02000902:
	pop	{r5, r6, pc}
	push	{r5, lr}
	adds	r5, r1, #0
	subs	r0, r5, #5
	bl 0x0200d8a0
	movs	r3, #0
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r3, #255
	adds	r5, r5, r3
	adds	r0, r5, #0
	bl 0x0200d7b0
	pop	{r5, pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x0200d8a0
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000958
	movs	r1, #16
	negs	r1, r1
	adds	r0, r6, #0
	bl 0x0200870c
	adds	r2, r5, #0
	adds	r2, #89
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	movs	r1, #6
	bl 0x0200d7c8
.L_02000958:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x0200d8a0
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200098e
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200870c
	adds	r2, r5, #0
	adds	r2, #89
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	movs	r1, #5
	bl 0x0200d7c8
.L_0200098e:
	pop	{r5, r6, pc}
	push	{lr}
	movs	r0, #16
	bl 0x0200d8a0
	movs	r3, #0
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #57
	bl 0x0200d7b0
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r7, #18
.L_020009b6:
	adds	r0, r7, #0
	bl 0x0200d8a0
	ldr	r3, [r0, #8]
	ldr	r1, [pc, #84]
	asrs	r3, r3, #20
	mov	sl, r3
	ldr	r3, [r0, #16]
	movs	r2, #3
	asrs	r3, r3, #20
	mov	r8, r3
	adds	r3, r7, #0
	subs	r3, #18
	ands	r3, r2
	ldrsb	r0, [r1, r3]
	bl 0x0200d8a0
	adds	r6, r0, #0
	adds	r6, #99
	movs	r3, #0
	movs	r5, #15
	strb	r3, [r6, #0]
	b.n	.L_020009e6
.L_020009e4:
	adds	r5, #1
.L_020009e6:
	cmp	r5, #17
	bgt.n	.L_02000a04
	adds	r0, r5, #0
	bl 0x0200d8a0
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	sl, r3
	bne.n	.L_020009e4
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r8, r3
	bne.n	.L_020009e4
	movs	r3, #1
	strb	r3, [r6, #0]
.L_02000a04:
	adds	r7, #1
	cmp	r7, #21
	ble.n	.L_020009b6
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xda88
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	mov	fp, r1
	adds	r3, r5, #0
	ldr	r1, [pc, #248]
	mov	sl, r2
	subs	r3, #18
	movs	r2, #3
	ands	r3, r2
	ldrsb	r3, [r1, r3]
	mov	r9, r3
	bl 0x0200d8a0
	mov	r8, r0
	mov	r0, r9
	bl 0x0200d8a0
	ldr	r3, [pc, #228]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	mov	r3, sl
	adds	r6, r0, #0
	cmp	r3, #10
	bgt.n	.L_02000a68
	lsls	r2, r3, #12
	movs	r3, #128
	lsls	r3, r3, #9
	subs	r3, r3, r2
	b.n	.L_02000a6c
.L_02000a68:
	movs	r3, #128
	lsls	r3, r3, #9
.L_02000a6c:
	mov	r2, r8
	str	r3, [r2, #28]
	mov	r3, fp
	cmp	r3, #1
	bne.n	.L_02000af4
	adds	r0, r5, #0
	bl 0x02008924
	mov	r0, r9
	bl 0x0200895c
	cmp	r5, #19
	beq.n	.L_02000aa6
	cmp	r5, #19
	bgt.n	.L_02000a90
	cmp	r5, #18
	beq.n	.L_02000a9a
	b.n	.L_02000ac8
.L_02000a90:
	cmp	r5, #20
	beq.n	.L_02000ab2
	cmp	r5, #21
	beq.n	.L_02000abe
	b.n	.L_02000ac8
.L_02000a9a:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #53
	bl 0x0200d7b0
	b.n	.L_02000ac8
.L_02000aa6:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #53
	bl 0x0200d7b8
	b.n	.L_02000ac8
.L_02000ab2:
	movs	r0, #142
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200d7b0
	b.n	.L_02000ac8
.L_02000abe:
	movs	r0, #142
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200d7b8
.L_02000ac8:
	ldr	r3, [r7, #8]
	ldr	r4, [r6, #8]
	asrs	r3, r3, #20
	asrs	r2, r4, #20
	cmp	r3, r2
	bne.n	.L_02000af4
	ldr	r3, [r7, #16]
	ldr	r0, [r6, #16]
	asrs	r3, r3, #20
	asrs	r2, r0, #20
	cmp	r3, r2
	bne.n	.L_02000af4
	ldr	r1, [pc, #76]
	movs	r3, #128
	lsls	r3, r3, #12
	ands	r4, r1
	ands	r0, r1
	adds	r2, r4, r3
	str	r3, [r6, #40]
	adds	r3, r0, r3
	str	r2, [r6, #8]
	str	r3, [r6, #16]
.L_02000af4:
	mov	r2, sl
	cmp	r2, #9
	bne.n	.L_02000afe
	ldr	r3, [pc, #56]
	b.n	.L_02000b1a
.L_02000afe:
	mov	r3, sl
	cmp	r3, #11
	bne.n	.L_02000b08
	ldr	r3, [pc, #48]
	b.n	.L_02000b1a
.L_02000b08:
	mov	r2, sl
	cmp	r2, #13
	bne.n	.L_02000b16
	movs	r3, #134
	lsls	r3, r3, #9
	adds	r3, #204
	b.n	.L_02000b1a
.L_02000b16:
	movs	r3, #128
	lsls	r3, r3, #9
.L_02000b1a:
	str	r3, [r7, #28]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200da88
	.4byte 0x02000240
	.4byte 0xfff00000
	.4byte 0x00013333
	.2byte 0x1999
	.2byte 0x0001
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	movs	r0, #18
	adds	r1, r3, #0
	bl 0x02008a18
	pop	{pc}
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	movs	r0, #19
	adds	r1, r3, #0
	bl 0x02008a18
	pop	{pc}
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	movs	r0, #20
	adds	r1, r3, #0
	bl 0x02008a18
	pop	{pc}
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	movs	r0, #21
	adds	r1, r3, #0
	bl 0x02008a18
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	mov	fp, r3
	mov	r9, r2
	mov	r8, r0
	bl 0x0200d8a0
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl 0x0200d8a0
	adds	r3, r7, #0
	adds	r3, #99
	ldrb	r3, [r3, #0]
	mov	sl, r0
	cmp	r3, #0
	bne.n	.L_02000c26
	bl 0x0200d880
	movs	r5, #128
	movs	r0, #0
	bl 0x0200d9e8
	lsls	r5, r5, #6
	mov	r3, sl
	str	r5, [r3, #28]
	movs	r0, #1
	bl 0x0200d728
	adds	r0, r6, #0
	bl 0x0200895c
	adds	r6, r5, #0
.L_02000bca:
	movs	r3, #128
	lsls	r3, r3, #8
	subs	r1, r3, r6
	cmp	r1, #0
	bge.n	.L_02000bdc
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, #255
	adds	r1, r1, r3
.L_02000bdc:
	movs	r5, #128
	asrs	r1, r1, #11
	mov	r0, r8
	lsls	r5, r5, #9
	bl 0x0200870c
	subs	r3, r5, r6
	str	r3, [r7, #28]
	mov	r3, sl
	str	r6, [r3, #28]
	movs	r0, #1
	bl 0x0200d728
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r6, r6, r3
	cmp	r6, r5
	ble.n	.L_02000bca
	mov	r0, r8
	str	r5, [r7, #28]
	bl 0x02008924
	movs	r0, #138
	bl 0x0200da68
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_02000c1c
	mov	r0, r9
	bl 0x0200d7b0
	b.n	.L_02000c22
.L_02000c1c:
	mov	r0, r9
	bl 0x0200d7b8
.L_02000c22:
	bl 0x0200d888
.L_02000c26:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #53
	movs	r0, #18
	movs	r1, #19
	movs	r3, #1
	bl 0x02008b7c
	pop	{pc}
	push	{lr}
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #53
	movs	r0, #19
	movs	r1, #18
	movs	r3, #0
	bl 0x02008b7c
	pop	{pc}
	push	{lr}
	movs	r2, #142
	lsls	r2, r2, #2
	adds	r2, #255
	movs	r0, #20
	movs	r1, #21
	movs	r3, #1
	bl 0x02008b7c
	pop	{pc}
	push	{lr}
	movs	r2, #142
	lsls	r2, r2, #2
	adds	r2, #255
	movs	r0, #21
	movs	r1, #20
	movs	r3, #0
	bl 0x02008b7c
	pop	{pc}
	push	{lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #13
	cmp	r3, r2
	blt.n	.L_02000ca4
	bl 0x0200d9c0
	b.n	.L_02000cb8
.L_02000ca4:
	bl 0x0200da30
	cmp	r0, #0
	beq.n	.L_02000cb8
	bl 0x0200da38
	movs	r0, #0
	movs	r1, #0
	bl 0x020089ac
.L_02000cb8:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200c418
	pop	{pc}
	.2byte 0x0000
	.2byte 0xf734
	.2byte 0x0200
	push	{lr}
	cmp	r0, #3
	bne.n	.L_02000cde
	ldr	r0, [pc, #8]
	movs	r1, #15
	bl 0x0200c5ac
.L_02000cde:
	pop	{pc}
	.2byte 0xf734
	.2byte 0x0200
	push	{lr}
	cmp	r0, #3
	bne.n	.L_02000cf2
	ldr	r0, [pc, #8]
	movs	r1, #16
	bl 0x0200c5ac
.L_02000cf2:
	pop	{pc}
	.2byte 0xf734
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r6, r1, #0
	ldr	r5, [r3, #32]
	bl 0x0200d8a0
	cmp	r0, #0
	beq.n	.L_02000d6a
	ldr	r3, [r0, #16]
	ldr	r2, [r0, #8]
	asrs	r3, r3, #20
	movs	r1, #156
	lsls	r3, r3, #7
	lsls	r1, r1, #1
	asrs	r2, r2, #20
	adds	r2, r2, r3
	adds	r3, r5, r1
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	adds	r1, #112
	adds	r4, r3, r2
	adds	r3, r5, r1
	ldr	r3, [r3, #0]
	adds	r0, r3, r2
	cmp	r6, #0
	bne.n	.L_02000d54
	ldr	r3, [pc, #60]
	adds	r1, #88
	adds	r2, r4, r3
	movs	r3, #255
	strb	r3, [r2, #2]
	movs	r3, #1
	negs	r3, r3
	adds	r2, r4, r1
	strb	r3, [r4, #2]
	strb	r3, [r2, #2]
	ldr	r3, [pc, #40]
	adds	r1, r0, r1
	adds	r2, r0, r3
	movs	r3, #1
	negs	r3, r3
	strb	r3, [r2, #2]
	strb	r3, [r0, #2]
	strb	r3, [r1, #2]
	b.n	.L_02000d6a
.L_02000d54:
	subs	r2, r4, #4
	movs	r3, #255
	strb	r3, [r2, #2]
	movs	r3, #1
	negs	r3, r3
	subs	r2, r0, #4
	strb	r3, [r4, #2]
	strb	r3, [r4, #6]
	strb	r3, [r2, #2]
	strb	r3, [r0, #2]
	strb	r3, [r0, #6]
.L_02000d6a:
	pop	{r5, r6, pc}
	.2byte 0xfe00
	.2byte 0xffff
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #11
	str	r3, [sp, #0]
	movs	r5, #22
	movs	r0, #11
	movs	r1, #64
	movs	r2, #11
	movs	r3, #8
	str	r5, [sp, #4]
	bl 0x0200d818
	movs	r3, #79
.L_02000d8a:
	str	r3, [sp, #0]
	movs	r2, #11
	movs	r3, #8
	movs	r0, #79
	movs	r1, #64
	str	r5, [sp, #4]
	bl 0x0200d818
	movs	r0, #16
	movs	r1, #0
.L_02000d9e:
	bl 0x02008cf8
	movs	r0, #17
	movs	r1, #0
	bl 0x02008cf8
	movs	r0, #18
	movs	r1, #1
	bl 0x02008cf8
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02000dd4
	movs	r3, #19
	movs	r2, #23
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #48
	movs	r2, #1
	movs	r3, #3
	bl 0x0200d818
.L_02000dd4:
	movs	r0, #149
	lsls	r0, r0, #2
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02000df4
	movs	r3, #19
	movs	r2, #26
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #48
	movs	r2, #1
	movs	r3, #3
	bl 0x0200d818
.L_02000df4:
	add	sp, #8
	pop	{r5, pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #15
	adds	r5, r1, #0
	bl 0x0200d8a0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, r6
	bne.n	.L_02000e1c
	ldr	r3, [r0, #16]
	movs	r0, #1
	asrs	r3, r3, #20
	cmp	r3, r5
	beq.n	.L_02000e3c
.L_02000e1c:
	movs	r1, #156
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldr	r2, [r3, #0]
	lsls	r3, r5, #7
	adds	r3, r6, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r2, [r2, #2]
	movs	r3, #255
	eors	r2, r3
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	movs	r0, #1
	subs	r0, r0, r3
.L_02000e3c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	mov	r9, r1
	mov	fp, r2
	str	r3, [sp, #16]
	str	r0, [sp, #20]
	bl 0x0200d8a0
	ldr	r3, [pc, #520]
	adds	r7, r0, #0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
.L_02000e68:
	bl 0x0200d8a0
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	str	r3, [sp, #12]
	ldr	r3, [r7, #16]
	ldr	r2, [sp, #12]
	asrs	r3, r3, #20
	str	r3, [sp, #8]
	mov	r8, r3
	movs	r3, #2
	str	r3, [sp, #0]
	mov	sl, r2
	ldr	r3, [r0, #8]
	asrs	r5, r3, #20
.L_02000e86:
	ldr	r3, [r0, #16]
	mov	r0, r9
	asrs	r6, r3, #20
	cmp	r0, #0
	beq.n	.L_02000e96
	adds	r3, r5, r0
	cmp	sl, r3
	bne.n	.L_02000ea2
.L_02000e96:
	mov	r2, fp
	cmp	r2, #0
	beq.n	.L_02000eb4
	adds	r3, r6, r2
	cmp	r8, r3
	beq.n	.L_02000eb4
.L_02000ea2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r2, #13
	strh	r2, [r3, #0]
	b.n	.L_0200105a
.L_02000eb4:
	movs	r2, #0
	str	r2, [sp, #4]
	b.n	.L_02000edc
.L_02000eba:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl 0x02008df8
	cmp	r0, #0
	bne.n	.L_02000f22
	adds	r0, r5, #1
	adds	r1, r6, #0
	bl 0x02008df8
	cmp	r0, #0
	bne.n	.L_02000f22
.L_02000ed2:
	ldr	r3, [sp, #4]
	mov	sl, r5
	adds	r3, #1
	str	r3, [sp, #4]
	mov	r8, r6
.L_02000edc:
	ldr	r0, [sp, #4]
	cmp	r0, #5
	bgt.n	.L_02000f22
	ldr	r2, [sp, #16]
	mov	r5, sl
	mov	r6, r8
	add	r5, r9
	add	r6, fp
	cmp	r2, #0
	bne.n	.L_02000f16
	subs	r1, r6, #1
	adds	r0, r5, #0
	bl 0x02008df8
	cmp	r0, #0
	bne.n	.L_02000f22
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl 0x02008df8
	cmp	r0, #0
	bne.n	.L_02000f22
	adds	r1, r6, #1
	adds	r0, r5, #0
	bl 0x02008df8
	cmp	r0, #0
	beq.n	.L_02000ed2
	b.n	.L_02000f22
.L_02000f16:
	subs	r0, r5, #1
	adds	r1, r6, #0
	bl 0x02008df8
	cmp	r0, #0
	beq.n	.L_02000eba
.L_02000f22:
	ldr	r3, [sp, #12]
	cmp	r3, sl
	bne.n	.L_02000f30
	ldr	r0, [sp, #8]
	cmp	r0, r8
	bne.n	.L_02000f30
	b.n	.L_0200105a
.L_02000f30:
	bl 0x0200d880
	movs	r0, #0
	bl 0x0200d9e8
	ldr	r3, [pc, #300]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #8
	bl 0x0200d8f8
	movs	r0, #6
	bl 0x0200d878
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #48]
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #51
	str	r3, [r7, #52]
	movs	r0, #239
	bl 0x0200da68
	mov	r3, r9
	cmp	r3, #0
	blt.n	.L_02000f70
	mov	r0, fp
	cmp	r0, #0
	bge.n	.L_02000f74
.L_02000f70:
	movs	r2, #3
	str	r2, [sp, #0]
.L_02000f74:
	ldr	r1, [sp, #0]
	adds	r0, r7, #0
	bl 0x0200d7c8
	mov	r3, sl
	mov	r0, r8
	movs	r2, #128
	lsls	r2, r2, #12
	lsls	r1, r3, #20
	lsls	r3, r0, #20
	adds	r6, r3, r2
	adds	r3, r6, #0
	adds	r1, r1, r2
	adds	r0, r7, #0
	movs	r2, #0
	bl 0x0200d7f8
	movs	r0, #6
	bl 0x0200d878
	ldr	r5, [pc, #200]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
.L_02000fa6:
	movs	r1, #2
	bl 0x0200d8f8
	movs	r1, #152
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #51
	bl 0x0200d8a8
	mov	r3, r9
	lsls	r1, r3, #3
	mov	r3, fp
	lsls	r2, r3, #3
	ldr	r0, [r5, #0]
	bl 0x0200d8d0
	movs	r0, #24
	bl 0x0200d878
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200d8f8
	adds	r0, r7, #0
	bl 0x0200d800
	mov	r0, sl
	cmp	r0, #17
	bgt.n	.L_02000ffe
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200d7c8
.L_02000fee:
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x0200da68
	movs	r0, #213
	bl 0x0200da68
	b.n	.L_0200104c
.L_02000ffe:
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x0200d7c8
	movs	r1, #156
	movs	r2, #0
	adds	r3, r6, #0
	lsls	r1, r1, #17
	adds	r0, r7, #0
	bl 0x0200d7f8
	movs	r0, #30
	bl 0x0200d878
	movs	r1, #8
	adds	r0, r7, #0
	bl 0x0200d7c8
	adds	r0, r7, #0
	bl 0x0200d800
	adds	r3, r7, #0
	adds	r3, #35
	movs	r2, #2
	movs	r0, #149
	strb	r2, [r3, #0]
	lsls	r0, r0, #1
	bl 0x0200da68
	movs	r0, #240
	bl 0x0200da68
	movs	r3, #162
	ldr	r2, [sp, #20]
	lsls	r3, r3, #1
	adds	r3, #255
.L_02001046:
	adds	r0, r2, r3
	bl 0x0200d7b0
.L_0200104c:
	movs	r0, #15
	bl 0x0200d878
	bl 0x0200d888
	bl 0x02008d70
.L_0200105a:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r1, #1
	negs	r1, r1
	movs	r0, #16
	movs	r2, #0
	movs	r3, #0
	bl 0x02008e40
.L_0200107c:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #16
	movs	r1, #1
	movs	r2, #0
	movs	r3, #0
	bl 0x02008e40
	pop	{pc}
	push	{lr}
	movs	r1, #1
	negs	r1, r1
	movs	r0, #17
	movs	r2, #0
	movs	r3, #0
	bl 0x02008e40
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #17
	movs	r1, #1
	movs	r2, #0
	movs	r3, #0
	bl 0x02008e40
	pop	{pc}
	push	{lr}
	movs	r2, #1
	negs	r2, r2
	movs	r0, #18
	movs	r1, #0
	movs	r3, #1
	bl 0x02008e40
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #18
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x02008e40
	pop	{pc}
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	bne.n	.L_020010f0
	bl 0x0200d9c8
	b.n	.L_020010fc
.L_020010f0:
	movs	r0, #18
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x02008e40
.L_020010fc:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	cmp	r2, #30
	bne.n	.L_02001144
	bl 0x0200d8a0
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #100
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	adds	r3, #2
	lsls	r1, r1, #16
	str	r1, [r5, #8]
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	movs	r0, #0
	lsls	r2, r2, #16
	str	r2, [r5, #16]
	bl 0x0200d808
	str	r0, [r5, #20]
	str	r0, [r5, #12]
	adds	r0, r6, #0
	bl 0x0200878c
	movs	r2, #149
	lsls	r2, r2, #2
	adds	r2, #255
	adds	r0, r6, r2
	bl 0x0200d7b0
.L_02001144:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	movs	r0, #16
	adds	r1, r3, #0
	bl 0x02009104
	pop	{pc}
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	movs	r0, #17
	adds	r1, r3, #0
	bl 0x02009104
	pop	{pc}
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	movs	r0, #18
	adds	r1, r3, #0
	bl 0x02009104
	pop	{pc}
	push	{r5, lr}
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	adds	r5, r0, #0
	movs	r0, #15
	bl 0x0200d8a0
	ldr	r3, [r5, #8]
	ldr	r2, [r0, #8]
	asrs	r3, r3, #20
	asrs	r2, r2, #20
	cmp	r2, r3
	bne.n	.L_020011b8
	ldr	r2, [r0, #16]
	ldr	r3, [r5, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_020011b8
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #14
	cmp	r3, r2
	bge.n	.L_020011b8
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r5, #12]
.L_020011b8:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	movs	r3, #2
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	movs	r3, #0
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r3, #19
	movs	r2, #21
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #21
	movs	r2, #1
	movs	r3, #1
	movs	r0, #12
	bl 0x0200d818
	movs	r0, #221
	lsls	r0, r0, #2
	bl 0x0200d7b0
	add	sp, #8
	pop	{pc}
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02001260
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001260
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
.L_02001260:
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
	bl 0x0200d8a0
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_020012e4
	cmp	r7, #0
	beq.n	.L_020012e4
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_020012ec
.L_020012e4:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_020012ec:
	mov	r3, sl
	bl 0x0200d7d8
.L_020012f2:
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020012fa
	b.n	.L_02001446
.L_020012fa:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200d7c8
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200d7d0
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d830
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
	bl 0x0200921c
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
	beq.n	.L_02001446
	cmp	r7, #0
	beq.n	.L_02001446
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200137c
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200d930
.L_0200137c:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200139c
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x0200921c
.L_0200139c:
	movs	r2, #128
.L_0200139e:
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020013b0
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_020013b0:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020013f6
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020013de
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200d708
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
.L_020013dc:
	b.n	.L_020013f0
.L_020013de:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200d708
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020013f0:
	bl 0x0200d708
	str	r0, [r6, #52]
.L_020013f6:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02001412
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200d7c8
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200d7d0
.L_02001412:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001424
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02001424:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001436
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02001436:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001446
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02001446:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200fbc0
	.4byte 0x02009265
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
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
	bl 0x0200d708
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02001494
	adds	r3, #15
.L_02001494:
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
	bl 0x0200d8a0
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
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #168]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r5, r0
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200d8a0
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r5, r5, r2
	ldrb	r3, [r5, #0]
	adds	r7, r0, #0
	cmp	r3, #4
	beq.n	.L_0200158a
	ldr	r3, [r7, #16]
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r7, #16]
	ldr	r3, [pc, #128]
	ldr	r2, [r3, #0]
	movs	r3, #7
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0200158a
	ldr	r3, [r7, #44]
.L_02001522:
	cmp	r3, #0
	bne.n	.L_0200152c
	ldr	r3, [r7, #36]
	cmp	r3, #0
.L_0200152a:
	beq.n	.L_0200158a
.L_0200152c:
	add	r3, sp, #28
	mov	r8, r3
	ldr	r3, [pc, #104]
	mov	r4, r8
	str	r3, [r4, #36]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	add	r6, sp, #16
	str	r3, [r4, #8]
	str	r3, [r4, #12]
	str	r2, [r6, #4]
	str	r1, [r6, #8]
	str	r2, [r6, #0]
	bl 0x0200d738
	ldr	r3, [r6, #0]
	lsls	r0, r0, #17
	lsrs	r0, r0, #16
	adds	r3, r3, r0
	ldr	r0, [pc, #72]
	adds	r3, r3, r0
	str	r3, [r6, #0]
	bl 0x0200d738
	ldr	r5, [r6, #8]
.L_02001560:
	ldr	r2, [pc, #64]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	ldr	r4, [r6, #4]
	adds	r5, r5, r0
	adds	r5, r5, r2
	str	r5, [r6, #8]
	ldr	r0, [r7, #8]
.L_02001570:
	ldr	r1, [r7, #12]
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #0]
	str	r4, [sp, #0]
	movs	r4, #128
	lsls	r4, r4, #17
	adds	r4, #1
	str	r4, [sp, #8]
.L_02001580:
	mov	r4, r8
	str	r5, [sp, #4]
	str	r4, [sp, #12]
	bl 0x0200929c
.L_0200158a:
	add	sp, #68
	pop	{r3}
	mov	r8, r3
.L_02001590:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0300122c
	.4byte 0x02009465
	.4byte 0xffff0000
	.2byte 0x8000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #197
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7b0
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r2, #64
	movs	r3, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r2, [sp, #8]
	movs	r0, #68
	movs	r1, #6
	movs	r2, #1
	movs	r3, #21
	bl 0x0200da50
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r2, #64
	movs	r3, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r2, [sp, #8]
	movs	r0, #68
	movs	r1, #6
	movs	r2, #1
	movs	r3, #21
	bl 0x0200da50
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #185
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_0200162e
	movs	r0, #249
	movs	r1, #72
	movs	r2, #152
	bl 0x0200da60
	ldr	r2, [pc, #40]
	movs	r3, #149
	lsls	r3, r3, #2
	adds	r1, r2, r3
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #185
	strh	r3, [r1, #0]
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r2, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r0, #106
	movs	r1, #4
	bl 0x0200d998
.L_0200162e:
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x2001
	.2byte 0x4770
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #8
	movs	r2, #11
	movs	r3, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	mov	sl, r2
	movs	r0, #33
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d810
	movs	r3, #7
	str	r3, [sp, #4]
	movs	r5, #18
.L_02001662:
	mov	r8, r3
	movs	r0, #33
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200d810
	movs	r6, #15
	movs	r0, #33
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200d810
	mov	r2, sl
	movs	r3, #74
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #35
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d810
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r5, #82
	movs	r0, #35
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200d810
	movs	r0, #35
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200d810
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #8
	movs	r2, #11
	movs	r3, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	mov	sl, r2
	movs	r0, #34
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d810
	movs	r3, #7
	str	r3, [sp, #4]
	movs	r5, #18
	mov	r8, r3
	movs	r0, #34
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200d810
	movs	r6, #15
	movs	r0, #34
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200d810
	mov	r2, sl
	movs	r3, #74
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #36
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d810
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r5, #82
	movs	r0, #36
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200d810
	movs	r0, #36
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200d810
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #255
	sub	sp, #8
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_020017cc
	ldr	r3, [pc, #112]
	movs	r2, #18
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200179c
	movs	r3, #13
	movs	r2, #79
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #26
	movs	r1, #79
	movs	r2, #1
	movs	r3, #2
	bl 0x0200d820
	movs	r3, #77
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #78
	movs	r1, #18
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d818
	b.n	.L_020017c2
.L_0200179c:
	movs	r3, #79
	movs	r5, #18
	str	r3, [sp, #4]
	movs	r0, #26
	movs	r1, #79
	movs	r2, #1
	movs	r3, #2
	str	r5, [sp, #0]
	bl 0x0200d820
	movs	r3, #82
	str	r3, [sp, #0]
	movs	r0, #81
	movs	r1, #18
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200d818
.L_020017c2:
	cmp	r6, #0
	beq.n	.L_020017cc
	movs	r0, #138
	bl 0x0200da68
.L_020017cc:
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7b0
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x244c
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #129
.L_020017e8:
	bl 0x0200d7a8
	adds	r6, r0, #0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #130
	bl 0x0200d7a8
	adds	r6, r6, r0
	movs	r0, #161
.L_020017fc:
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200d7a8
	adds	r6, r6, r0
	movs	r0, #225
	lsls	r0, r0, #2
	bl 0x0200d7a8
	adds	r6, r6, r0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #133
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02001824
	bl 0x0200adec
	b.n	.L_02001828
.L_02001824:
	bl 0x0200b22c
.L_02001828:
	ldr	r3, [pc, #40]
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r5, [pc, #32]
	movs	r1, #4
	adds	r0, r5, #0
	bl 0x0200d9a8
	adds	r0, r5, #0
	movs	r1, #5
	bl 0x0200d9b0
	movs	r1, #4
	subs	r1, r1, r6
	movs	r0, #13
	bl 0x0200d998
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x00f8
	.2byte 0x0000
	push	{r5, lr}
	adds	r0, r1, #0
	bl 0x0200d8a0
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #8
	bne.n	.L_02001888
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #252
	bl 0x0200d7b0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r2, [pc, #8]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
.L_02001888:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffff
	.global Func_02001890
	.thumb_func
Func_02001890:
	.2byte 0xb500
	ldr	r3, [pc, #68]
	movs	r2, #240
	movs	r0, #16
	ldrsh	r1, [r3, r0]
	ldr	r3, [pc, #64]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_020018b6
	movs	r3, #7
	ldr	r2, [pc, #52]
	ands	r1, r3
	lsls	r3, r1, #2
	ldr	r0, [r2, r3]
	b.n	.L_020018d6
.L_020018b6:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_020018c0
	ldr	r0, [pc, #44]
	b.n	.L_020018d6
.L_020018c0:
	ldr	r3, [pc, #44]
.L_020018c2:
	cmp	r2, r3
	bne.n	.L_020018ca
	ldr	r0, [pc, #44]
	b.n	.L_020018d6
.L_020018ca:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_020018d4
	ldr	r0, [pc, #40]
	b.n	.L_020018d6
.L_020018d4:
	ldr	r0, [pc, #40]
.L_020018d6:
	pop	{pc}
	.4byte 0x0200244c
	.4byte 0x02000240
	.4byte 0x000000f7
	.4byte 0x0200fe00
	.4byte 0x000000f9
	.4byte 0x0200fcf8
	.4byte 0x000000fa
	.4byte 0x0200fd40
	.4byte 0x000000f8
	.4byte 0x0200fd94
	.2byte 0xf044
	.2byte 0x0200
	push	{lr}
	movs	r0, #130
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001922
	movs	r0, #17
	bl 0x0200d8a0
	movs	r3, #1
	adds	r0, #98
	strb	r3, [r0, #0]
	b.n	.L_0200192c
.L_02001922:
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
.L_0200192c:
	movs	r0, #15
	bl 0x020086b4
	movs	r0, #16
	bl 0x020086b4
	movs	r0, #17
	bl 0x020086b4
	movs	r0, #18
	bl 0x020086b4
	movs	r0, #19
	bl 0x020086b4
	movs	r0, #16
	bl 0x0200862c
	movs	r0, #17
	bl 0x0200862c
	movs	r0, #18
	bl 0x0200862c
	movs	r0, #19
	bl 0x0200862c
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl 0x0200d730
	pop	{pc}
	.2byte 0x0000
	.2byte 0x87b5
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #15
	bl 0x020086b4
	movs	r0, #16
	bl 0x020086b4
	movs	r0, #17
	bl 0x020086b4
	movs	r0, #18
	bl 0x020086b4
	movs	r0, #19
	bl 0x020086b4
	movs	r0, #20
	bl 0x020086b4
	movs	r0, #15
	bl 0x0200862c
	movs	r0, #16
	bl 0x0200862c
	movs	r0, #17
	bl 0x0200862c
	movs	r0, #18
	bl 0x0200862c
	movs	r0, #19
	bl 0x0200862c
	movs	r0, #20
	bl 0x0200862c
	movs	r0, #17
	bl 0x0200d8a0
	movs	r5, #1
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #18
	bl 0x0200d8a0
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #19
	bl 0x0200d8a0
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #20
	bl 0x0200d8a0
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #133
	lsls	r0, r0, #2
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_020019fe
	movs	r0, #17
	movs	r1, #5
	bl 0x0200d8f8
	b.n	.L_02001a08
.L_020019fe:
	movs	r1, #16
	negs	r1, r1
	movs	r0, #17
	bl 0x0200870c
.L_02001a08:
	movs	r0, #139
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001a20
	movs	r0, #18
	movs	r1, #5
	bl 0x0200d8f8
	b.n	.L_02001a2a
.L_02001a20:
	movs	r1, #16
	negs	r1, r1
	movs	r0, #18
	bl 0x0200870c
.L_02001a2a:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #22
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001a42
	movs	r0, #19
	movs	r1, #5
	bl 0x0200d8f8
	b.n	.L_02001a4c
.L_02001a42:
	movs	r1, #16
	negs	r1, r1
	movs	r0, #19
	bl 0x0200870c
.L_02001a4c:
	movs	r0, #140
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001a64
	movs	r0, #20
	movs	r1, #5
	bl 0x0200d8f8
	b.n	.L_02001a6e
.L_02001a64:
	movs	r1, #16
	negs	r1, r1
	movs	r0, #20
	bl 0x0200870c
.L_02001a6e:
	pop	{r5, pc}
	push	{lr}
	movs	r0, #138
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001a8e
	movs	r0, #15
	bl 0x0200d8a0
	movs	r3, #1
	adds	r0, #98
	strb	r3, [r0, #0]
	b.n	.L_02001a98
.L_02001a8e:
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
.L_02001a98:
	movs	r0, #202
	lsls	r0, r0, #2
	bl 0x0200d7a8
.L_02001aa0:
	cmp	r0, #0
	bne.n	.L_02001ab2
	movs	r0, #16
	bl 0x0200d8a0
	movs	r3, #1
	adds	r0, #98
	strb	r3, [r0, #0]
	b.n	.L_02001abc
.L_02001ab2:
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
.L_02001abc:
	movs	r0, #15
	bl 0x020086b4
	movs	r0, #16
	bl 0x020086b4
	movs	r0, #17
	bl 0x020086b4
	movs	r0, #18
	bl 0x020086b4
	movs	r0, #19
	bl 0x020086b4
	movs	r0, #15
	bl 0x0200862c
	movs	r0, #16
	bl 0x0200862c
	movs	r0, #17
	bl 0x0200862c
	movs	r0, #18
	bl 0x0200862c
	movs	r0, #19
	bl 0x0200862c
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #18
	bl 0x0200d8a0
	movs	r5, #1
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #19
	bl 0x0200d8a0
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #20
	bl 0x0200d8a0
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #21
	bl 0x0200d8a0
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #57
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001b42
	movs	r0, #16
	bl 0x0200d8a0
	adds	r0, #98
	strb	r5, [r0, #0]
	b.n	.L_02001b4c
.L_02001b42:
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
.L_02001b4c:
	movs	r0, #15
	bl 0x020086b4
	movs	r0, #16
	bl 0x020086b4
	movs	r0, #17
	bl 0x020086b4
	movs	r0, #18
	bl 0x020086b4
	movs	r0, #19
	bl 0x020086b4
	movs	r0, #20
	bl 0x020086b4
	movs	r0, #21
	bl 0x020086b4
	movs	r0, #15
	bl 0x0200862c
	movs	r0, #16
	bl 0x0200862c
	movs	r0, #17
	bl 0x0200862c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #53
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001ba4
	movs	r0, #18
	bl 0x0200895c
	movs	r0, #19
	bl 0x02008924
	b.n	.L_02001bb0
.L_02001ba4:
	movs	r0, #19
	bl 0x0200895c
	movs	r0, #18
	bl 0x02008924
.L_02001bb0:
	movs	r0, #142
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001bcc
	movs	r0, #20
	bl 0x0200895c
	movs	r0, #21
	bl 0x02008924
	b.n	.L_02001bd8
.L_02001bcc:
	movs	r0, #21
	bl 0x0200895c
	movs	r0, #20
	bl 0x02008924
.L_02001bd8:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200c2f0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xf734
	.2byte 0x0200
	push	{r5, r6, lr}
	sub	sp, #8
	movs	r3, #11
	str	r3, [sp, #0]
	movs	r5, #64
	movs	r0, #11
	movs	r1, #22
	movs	r2, #11
	movs	r3, #8
	str	r5, [sp, #4]
	bl 0x0200d818
	movs	r3, #79
	str	r3, [sp, #0]
	movs	r1, #22
	movs	r2, #11
	movs	r3, #8
	movs	r0, #79
	str	r5, [sp, #4]
	bl 0x0200d818
	movs	r0, #16
	bl 0x0200d8a0
	movs	r5, #1
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #17
	bl 0x0200d8a0
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #18
	bl 0x0200d8a0
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #16
	bl 0x0200d8a0
	movs	r6, #0
	adds	r0, #89
	strb	r6, [r0, #0]
	movs	r0, #17
	bl 0x0200d8a0
	adds	r0, #89
	strb	r6, [r0, #0]
	movs	r0, #18
	bl 0x0200d8a0
	adds	r0, #89
	strb	r6, [r0, #0]
	bl 0x02008d70
	movs	r0, #15
	bl 0x020086b4
	movs	r0, #15
	bl 0x0200862c
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r0, #0
	movs	r1, #240
	sub	sp, #4
	bl 0x0200c188
	movs	r5, #64
	movs	r1, #16
	movs	r2, #0
	movs	r3, #0
	movs	r0, #0
	str	r5, [sp, #0]
	bl 0x0200c224
	movs	r3, #240
	mov	sl, r3
	mov	r3, sl
	strh	r3, [r0, #6]
	movs	r3, #120
	mov	r8, r3
	movs	r6, #150
	mov	r3, r8
	strh	r3, [r0, #8]
	strh	r6, [r0, #10]
	movs	r1, #17
	movs	r2, #0
	movs	r3, #0
	movs	r0, #1
	str	r5, [sp, #0]
	bl 0x0200c224
	mov	r3, sl
	strh	r3, [r0, #6]
	mov	r3, r8
	strh	r3, [r0, #8]
.L_02001cb8:
	strh	r6, [r0, #10]
	movs	r1, #18
	movs	r2, #0
	movs	r3, #0
	movs	r0, #2
	str	r5, [sp, #0]
.L_02001cc4:
	bl 0x0200c224
	mov	r3, sl
	strh	r3, [r0, #6]
	mov	r3, r8
	strh	r3, [r0, #8]
	strh	r6, [r0, #10]
	movs	r5, #32
	movs	r1, #19
	movs	r2, #0
	movs	r3, #0
	movs	r0, #3
	str	r5, [sp, #0]
	bl 0x0200c224
	mov	r3, sl
	strh	r3, [r0, #6]
	mov	r3, r8
	strh	r3, [r0, #8]
	strh	r6, [r0, #10]
	movs	r3, #0
.L_02001cee:
	movs	r0, #4
	movs	r1, #20
	movs	r2, #0
	str	r5, [sp, #0]
	bl 0x0200c224
.L_02001cfa:
	mov	r3, sl
	strh	r3, [r0, #6]
	mov	r3, r8
	strh	r3, [r0, #8]
	strh	r6, [r0, #10]
	movs	r0, #153
.L_02001d06:
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001d1e
	movs	r0, #16
.L_02001d14:
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
	b.n	.L_02001d24
.L_02001d1e:
	movs	r0, #16
	bl 0x0200878c
.L_02001d24:
	movs	r0, #217
	lsls	r0, r0, #2
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001d3c
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
	b.n	.L_02001d42
.L_02001d3c:
	movs	r0, #17
	bl 0x0200878c
.L_02001d42:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #101
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001d5c
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
	b.n	.L_02001d62
.L_02001d5c:
	movs	r0, #18
	bl 0x0200878c
.L_02001d62:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	movs	r3, #13
	ldrb	r2, [r1, #23]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #23]
	movs	r0, #15
	bl 0x020086b4
	movs	r0, #15
	bl 0x0200862c
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	push	{lr}
	sub	sp, #8
	bl 0x0200da08
	movs	r0, #0
	movs	r1, #15
	movs	r2, #16
	bl 0x0200da10
	movs	r0, #221
	lsls	r0, r0, #2
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001dc0
	movs	r3, #19
	movs	r2, #21
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #18
	movs	r1, #21
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d818
	b.n	.L_02001dca
.L_02001dc0:
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
.L_02001dca:
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r0, #8
	bl 0x0200d8a0
	ldr	r3, [pc, #196]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	adds	r7, r0, #0
	movs	r0, #162
	lsls	r0, r0, #2
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02001ea2
	ldr	r0, [r7, #8]
	ldr	r3, [r6, #8]
	ldr	r2, [r7, #16]
	subs	r3, r0, r3
	asrs	r5, r3, #16
	ldr	r3, [r6, #16]
	adds	r1, r5, #0
	muls	r1, r5
	subs	r3, r2, r3
	asrs	r4, r3, #16
	adds	r3, r4, #0
	muls	r3, r4
	adds	r1, r1, r3
	ldr	r3, [pc, #144]
	asrs	r2, r2, #16
	adds	r4, r2, #0
	asrs	r0, r0, #16
	adds	r5, r0, r3
	subs	r4, #248
	adds	r2, r5, #0
	muls	r2, r5
	adds	r3, r4, #0
	muls	r3, r4
	adds	r7, r2, r3
	movs	r2, #200
	lsls	r2, r2, #5
	cmp	r7, r2
	ble.n	.L_02001e3a
	movs	r3, #144
	lsls	r3, r3, #4
	cmp	r1, r3
	bgt.n	.L_02001e50
.L_02001e3a:
	movs	r1, #148
	lsls	r1, r1, #1
	movs	r3, #248
	subs	r1, r1, r5
	subs	r3, r3, r4
	lsls	r1, r1, #16
	ldr	r2, [r6, #12]
	lsls	r3, r3, #16
	adds	r0, r6, #0
	bl 0x0200d7f8
.L_02001e50:
	cmp	r7, #15
	bgt.n	.L_02001ea2
	movs	r0, #146
	bl 0x0200da68
	movs	r0, #122
	bl 0x0200da68
	movs	r3, #128
	lsls	r3, r3, #10
	adds	r1, r6, #0
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	adds	r1, #85
	movs	r2, #0
	movs	r3, #2
	strb	r3, [r1, #0]
	str	r2, [r6, #20]
	adds	r2, r6, #0
	adds	r2, #89
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #12
	str	r3, [r6, #40]
	ldr	r1, [pc, #40]
	movs	r0, #8
	bl 0x0200d8b0
	movs	r1, #172
	movs	r3, #132
	adds	r0, r6, #0
	ldr	r2, [r6, #12]
	lsls	r1, r1, #17
	lsls	r3, r3, #17
	bl 0x0200d7f8
	movs	r0, #162
	lsls	r0, r0, #2
	bl 0x0200d7b0
.L_02001ea2:
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfffffed8
	.2byte 0xdb44
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #123
	bl 0x0200d7a8
	adds	r6, r0, #0
.L_02001eba:
	cmp	r6, #0
	bne.n	.L_02001ef6
	movs	r0, #8
	bl 0x0200d8a0
	adds	r5, r0, #0
	movs	r0, #162
	lsls	r0, r0, #2
	bl 0x0200d7a8
	adds	r2, r5, #0
	adds	r2, #85
	cmp	r0, #0
	bne.n	.L_02001ee6
	movs	r3, #5
	strb	r3, [r2, #0]
	movs	r2, #136
	ldr	r3, [r5, #12]
.L_02001ede:
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r5, #12]
	b.n	.L_02001eec
.L_02001ee6:
	movs	r3, #3
	strb	r3, [r2, #0]
	str	r6, [r5, #12]
.L_02001eec:
	movs	r1, #144
	ldr	r0, [pc, #8]
.L_02001ef0:
	lsls	r1, r1, #3
	bl 0x0200d730
.L_02001ef6:
	pop	{r5, r6, pc}
	.4byte 0x02009dd5
	.4byte 0x60184b01
	.4byte 0x00004770
	.2byte 0xfe24
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #168]
	sub	sp, #32
	ldr	r0, [r3, #0]
.L_02001f14:
	cmp	r0, #0
	bge.n	.L_02001f1a
	adds	r0, #3
.L_02001f1a:
	asrs	r0, r0, #2
	movs	r1, #5
	bl 0x0200d710
	ldr	r3, [pc, #152]
	mov	r8, r0
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02001f6e
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r0, #179
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrh	r1, [r3, #0]
.L_02001f3a:
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_02001f4e
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r1
	cmp	r3, #153
	bne.n	.L_02001fae
.L_02001f4e:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_02001fae
	movs	r0, #175
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02001fae
.L_02001f6e:
	movs	r5, #0
	movs	r6, #4
.L_02001f72:
	mov	r2, r8
	adds	r0, r2, r5
	movs	r1, #5
	mov	r7, sp
.L_02001f7a:
	bl 0x0200d710
	ldr	r3, [pc, #64]
.L_02001f80:
	lsls	r0, r0, #1
	ldrh	r3, [r3, r6]
	adds	r5, #1
.L_02001f86:
	strh	r3, [r7, r0]
	adds	r6, #2
	cmp	r5, #4
.L_02001f8c:
	ble.n	.L_02001f72
	movs	r3, #128
	movs	r1, #160
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r7, #0
	adds	r1, #164
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #16]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_02001fae:
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200fe20
	.4byte 0x0200fe24
	.2byte 0xfe28
	.2byte 0x0200
	push	{r5, lr}
	ldr	r2, [pc, #48]
	movs	r3, #1
.L_02001fca:
	adds	r5, r0, #0
	str	r3, [r2, #0]
	cmp	r5, #2
.L_02001fd0:
	beq.n	0x02009fe2
	movs	r1, #160
	lsls	r1, r1, #19
.L_02001fd6:
	ldr	r0, [pc, #36]
	ldr	r3, [pc, #36]
	adds	r1, #160
.L_02001fdc:
	movs	r2, #32
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2d01
.L_02001fe4:
	bne.n	.L_02001ff6
	ldr	r3, [pc, #28]
.L_02001fe8:
	movs	r2, #0
.L_02001fea:
	movs	r1, #144
	str	r2, [r3, #0]
	ldr	r0, [pc, #24]
.L_02001ff0:
	lsls	r1, r1, #3
	bl 0x0200d730
.L_02001ff6:
	pop	{r5, pc}
	.4byte 0x0200fe24
	.4byte 0x0200fe28
	.4byte 0x03000730
	.4byte 0x0200fe20
	.2byte 0x9f09
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #176]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #4
	bne.n	.L_0200202e
	bl 0x0200d868
	bl 0x0200b4b4
	movs	r0, #4
	bl 0x0200d990
	b.n	.L_0200207c
.L_0200202e:
	cmp	r3, #5
	bne.n	.L_02002042
	bl 0x0200d868
	bl 0x0200b7c8
.L_0200203a:
	movs	r0, #5
	bl 0x0200d990
.L_02002040:
	b.n	.L_0200207c
.L_02002042:
	movs	r0, #144
	lsls	r0, r0, #4
.L_02002046:
	adds	r0, #255
	bl 0x0200d7a8
.L_0200204c:
	cmp	r0, #0
	beq.n	.L_0200207c
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02002064
	movs	r0, #0
	bl 0x02009754
.L_02002064:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #133
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02002078
	bl 0x0200adb0
	b.n	.L_0200207c
.L_02002078:
	bl 0x0200adb4
.L_0200207c:
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #1
	bl 0x02009fc4
	movs	r0, #10
	bl 0x020086b4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #252
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_020020bc
	movs	r1, #136
	movs	r2, #164
	movs	r0, #10
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200d8e0
.L_020020bc:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.global Func_020020c4
	.thumb_func
Func_020020c4:
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #152]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r5, r1
	adds	r1, #2
	movs	r2, #0
	ldrsh	r7, [r3, r2]
	adds	r3, r5, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #136]
	movs	r0, #0
	movs	r1, #16
	ldrsh	r6, [r3, r1]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #172
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0200215e
	ldr	r3, [pc, #112]
	cmp	r7, r3
	bne.n	.L_02002136
	cmp	r2, #7
	bgt.n	.L_0200212e
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200d7b0
	movs	r2, #133
.L_0200210a:
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
.L_02002110:
	bl 0x0200d8a0
	adds	r0, #35
.L_02002116:
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
.L_0200211c:
	strb	r3, [r0, #0]
	ldr	r2, [pc, #76]
	movs	r3, #7
.L_02002122:
	ands	r6, r3
	lsls	r3, r6, #2
	ldr	r0, [r2, r3]
	mov	lr, r0
	.2byte 0xf800
	.2byte 0xe003
.L_0200212e:
	cmp	r2, #55
	bne.n	.L_02002136
	bl 0x0200b900
.L_02002136:
	ldr	r3, [pc, #56]
	cmp	r7, r3
	bne.n	.L_02002140
	bl 0x02009dd0
.L_02002140:
	ldr	r3, [pc, #48]
	cmp	r7, r3
	bne.n	.L_0200214a
	bl 0x02009eb0
.L_0200214a:
	ldr	r3, [pc, #44]
	cmp	r7, r3
	bne.n	.L_0200215c
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200d7b0
	bl 0x0200a00c
.L_0200215c:
	movs	r0, #0
.L_0200215e:
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200244c
	.4byte 0x000000f7
	.4byte 0x0200db64
	.4byte 0x000000f9
	.4byte 0x000000fa
	.2byte 0x00f8
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
.L_02002180:
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
.L_02002186:
	mov	sl, r2
	ldr	r2, [sp, #32]
	ldr	r4, [sp, #28]
.L_0200218c:
	lsls	r2, r2, #7
	adds	r2, r2, r4
	ldr	r4, [pc, #152]
	ldr	r5, [pc, #156]
	lsls	r1, r1, #7
	adds	r1, r1, r0
	subs	r3, #1
	adds	r6, r2, r4
	adds	r0, r1, r4
	lsls	r2, r2, #2
	lsls	r1, r1, #2
	lsls	r3, r3, #16
	adds	r4, r2, r5
.L_020021a6:
	adds	r5, r1, r5
	asrs	r1, r3, #16
	cmp	r1, #0
.L_020021ac:
	blt.n	.L_02002220
	ldr	r2, [pc, #132]
	lsls	r3, r1, #16
.L_020021b2:
	adds	r2, r2, r3
	lsls	r3, r1, #7
	mov	r1, sl
	mov	r8, r2
	adds	r2, r0, r1
	adds	r2, r2, r3
	mov	lr, r2
	adds	r2, r6, r1
	adds	r2, r2, r3
	add	r3, sl
	lsls	r3, r3, #2
	mov	ip, r2
	adds	r7, r3, r5
	adds	r6, r3, r4
.L_020021ce:
	mov	r3, sl
	subs	r3, #1
	lsls	r3, r3, #16
	mov	r4, ip
	mov	r0, lr
	asrs	r3, r3, #16
	subs	r5, r6, #4
	subs	r1, r7, #4
	subs	r4, #1
	subs	r0, #1
	cmp	r3, #0
	blt.n	.L_02002208
	lsls	r2, r3, #16
	ldr	r3, [pc, #72]
	adds	r2, r2, r3
.L_020021ec:
	ldr	r3, [r1, #0]
	mov	r9, r2
	str	r3, [r5, #0]
	subs	r1, #4
	ldrb	r3, [r0, #0]
	subs	r5, #4
	strb	r3, [r4, #0]
	ldr	r3, [pc, #56]
	subs	r4, #1
	adds	r2, r2, r3
	mov	r3, r9
	subs	r0, #1
.L_02002204:
	cmp	r3, #0
	bge.n	.L_020021ec
.L_02002208:
	movs	r3, #128
.L_0200220a:
	negs	r3, r3
	ldr	r1, [pc, #36]
	add	lr, r3
.L_02002210:
	add	ip, r3
	ldr	r3, [pc, #36]
	mov	r2, r8
.L_02002216:
	adds	r7, r7, r3
	add	r8, r1
	adds	r6, r6, r3
.L_0200221c:
	cmp	r2, #0
	bge.n	.L_020021ce
.L_02002220:
	pop	{r3, r5, r6}
.L_02002222:
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
.L_02002228:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02024000
	.4byte 0x02010000
	.4byte 0xffff0000
	.2byte 0xfe00
	.2byte 0xffff
	.2byte 0xb5e0
	ldr	r7, [pc, #152]
	movs	r0, #24
	ldrsh	r3, [r7, r0]
	cmp	r3, #3
	ble.n	.L_0200224a
	b.n	.L_0200242a
.L_0200224a:
	bl 0x0200d6e8
	cmp	r0, #0
	beq.n	.L_020022ee
	movs	r1, #20
	ldrsh	r3, [r7, r1]
	cmp	r3, #0
.L_02002258:
	bne.n	.L_02002280
	movs	r0, #1
	bl 0x020080fc
	movs	r3, #1
	adds	r6, r0, #0
	eors	r6, r3
	ldr	r2, [pc, #116]
.L_02002268:
	lsls	r5, r6, #2
	adds	r3, r5, #0
	adds	r3, #32
	ldr	r0, [r2, r3]
	adds	r5, r5, r6
	bl 0x0200d6c4
	lsls	r3, r5, #4
.L_02002278:
	subs	r3, r3, r5
	lsls	r3, r3, #3
	adds	r3, #1
	b.n	.L_020022ec
.L_02002280:
	movs	r3, #24
	ldrsh	r2, [r7, r3]
	movs	r0, #28
	ldrsh	r3, [r7, r0]
	ldrh	r1, [r7, #24]
	cmp	r2, r3
	blt.n	.L_02002292
	adds	r3, r1, #1
.L_02002290:
	strh	r3, [r7, #28]
.L_02002292:
	ldrh	r3, [r7, #24]
	movs	r5, #0
	adds	r3, #1
	strh	r3, [r7, #24]
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	cmp	r1, #3
	bgt.n	.L_020022e0
	ldr	r2, [pc, #48]
	ldrh	r3, [r7, #18]
	lsls	r1, r1, #1
	eors	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	adds	r1, r1, r3
	lsls	r1, r1, #1
	ldrsh	r3, [r7, r1]
	ldr	r1, [pc, #36]
	movs	r2, #7
	ands	r3, r2
	lsls	r3, r3, #2
	ldr	r0, [r1, r3]
	bl 0x0200d6c4
	movs	r0, #8
	bl 0x0200d8a0
	str	r5, [r0, #20]
	movs	r0, #8
	bl 0x0200d8a0
	str	r5, [r0, #12]
	b.n	.L_020022ea
	.4byte 0x00000001
	.4byte 0x0200244c
	.2byte 0xdb84
	.2byte 0x0200
.L_020022e0:
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
.L_020022ea:
	movs	r3, #0
.L_020022ec:
	strh	r3, [r7, #20]
.L_020022ee:
	movs	r0, #8
	bl 0x0200d8a0
	movs	r3, #22
	ldrsh	r2, [r7, r3]
	movs	r1, #24
	ldrsh	r3, [r7, r1]
	adds	r0, #84
	cmp	r2, r3
	bne.n	.L_02002306
	movs	r3, #1
	b.n	.L_02002308
.L_02002306:
	movs	r3, #0
.L_02002308:
	strb	r3, [r0, #0]
	movs	r2, #20
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	bne.n	.L_02002314
.L_02002312:
	b.n	.L_0200242a
.L_02002314:
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02002322
	b.n	.L_0200242a
.L_02002322:
	movs	r3, #22
	ldrsh	r2, [r7, r3]
	movs	r0, #24
	ldrsh	r3, [r7, r0]
	ldrh	r1, [r7, #24]
	cmp	r2, r3
	bne.n	.L_02002424
	movs	r2, #20
	ldrsh	r3, [r7, r2]
	movs	r2, #199
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_020023e8
	cmp	r3, r2
	bgt.n	.L_02002360
	cmp	r3, #50
	beq.n	.L_020023f6
	cmp	r3, #50
	bgt.n	.L_02002350
	cmp	r3, #1
	beq.n	.L_0200238e
	b.n	.L_02002424
.L_02002350:
	cmp	r3, #70
	beq.n	.L_02002412
	movs	r0, #173
	lsls	r0, r0, #1
	adds	r0, #255
.L_0200235a:
	cmp	r3, r0
	beq.n	.L_0200238e
	b.n	.L_02002424
.L_02002360:
	movs	r2, #177
	lsls	r2, r2, #2
	cmp	r3, r2
	beq.n	.L_020023e8
	cmp	r3, r2
	bgt.n	.L_02002378
	movs	r1, #200
	lsls	r1, r1, #1
	adds	r1, #255
	cmp	r3, r1
	beq.n	.L_020023d2
	b.n	.L_02002424
.L_02002378:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #198
	cmp	r3, r2
	beq.n	.L_020023f6
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #218
	cmp	r3, r0
	beq.n	.L_02002412
	b.n	.L_02002424
.L_0200238e:
	movs	r0, #124
	bl 0x0200da68
	movs	r1, #18
	ldrsh	r3, [r7, r1]
	cmp	r3, #0
	beq.n	.L_020023b4
	movs	r0, #9
	movs	r1, #6
	bl 0x0200d8f8
	movs	r2, #20
	ldrsh	r3, [r7, r2]
	movs	r0, #150
	lsls	r0, r0, #2
.L_020023ac:
	cmp	r3, r0
	ble.n	.L_02002424
	movs	r0, #10
	b.n	.L_020023ca
.L_020023b4:
	movs	r1, #6
	movs	r0, #11
	bl 0x0200d8f8
	movs	r1, #20
	ldrsh	r3, [r7, r1]
	movs	r2, #150
	lsls	r2, r2, #2
	cmp	r3, r2
	ble.n	.L_02002424
	movs	r0, #12
.L_020023ca:
	movs	r1, #6
	bl 0x0200d8f8
	b.n	.L_02002424
.L_020023d2:
	movs	r0, #125
	bl 0x0200da68
	movs	r0, #18
	ldrsh	r3, [r7, r0]
	cmp	r3, #0
	beq.n	.L_020023e4
	movs	r0, #10
	b.n	.L_0200240a
.L_020023e4:
	movs	r0, #12
	b.n	.L_0200240a
.L_020023e8:
	movs	r0, #8
	bl 0x0200d8a0
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r0, #6]
	b.n	.L_02002424
.L_020023f6:
	movs	r0, #125
	bl 0x0200da68
	movs	r1, #18
	ldrsh	r3, [r7, r1]
	cmp	r3, #0
	beq.n	.L_02002408
	movs	r0, #9
	b.n	.L_0200240a
.L_02002408:
	movs	r0, #11
.L_0200240a:
	movs	r1, #5
	bl 0x0200d8f8
	b.n	.L_02002424
.L_02002412:
	lsls	r1, r1, #16
	movs	r2, #18
	ldrsh	r0, [r7, r2]
	asrs	r1, r1, #16
	bl 0x020082a4
	movs	r0, #138
	bl 0x0200da68
.L_02002424:
	ldrh	r3, [r7, #20]
	adds	r3, #1
	strh	r3, [r7, #20]
.L_0200242a:
	pop	{r5, r6, r7, pc}
	.global Func_0200242c
	.thumb_func
Func_0200242c:
	push	{r5, r6, r7, lr}
.L_0200242e:
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #184]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r5, r0
	adds	r0, #2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	adds	r3, r5, r0
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	ldr	r3, [pc, #168]
	sub	sp, #16
	mov	sl, r1
	cmp	r2, r3
	beq.n	.L_0200245a
	b.n	.L_02002bac
.L_0200245a:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	ldr	r1, [pc, #152]
	mov	r8, r3
	mov	r3, sl
	subs	r3, #31
	mov	fp, r1
	cmp	r3, #1
	bhi.n	.L_0200251a
	movs	r0, #224
	lsls	r0, r0, #2
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_020024b2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200d7a0
	movs	r1, #32
	adds	r5, r0, #0
	ldr	r3, [pc, #116]
	mov	r0, fp
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c2a
	adds	r1, r2, #0
	movs	r6, #0
	adds	r1, #13
.L_0200249a:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	adds	r6, r6, r3
	cmp	r2, r1
	ble.n	.L_0200249a
	adds	r0, r6, #0
	bl 0x0200ac30
	movs	r0, #224
	lsls	r0, r0, #2
.L_020024ae:
	bl 0x0200d7b0
.L_020024b2:
	ldr	r1, [pc, #60]
	mov	r3, sl
	subs	r3, #1
	mov	r0, fp
	ands	r3, r1
	movs	r2, #0
	strh	r3, [r0, #18]
	strh	r2, [r0, #22]
	strh	r2, [r0, #24]
.L_020024c4:
	strh	r2, [r0, #26]
	strh	r2, [r0, #28]
	eors	r3, r1
	lsls	r3, r3, #1
.L_020024cc:
	ldrsh	r6, [r0, r3]
	ldr	r2, [pc, #52]
	movs	r3, #7
.L_020024d2:
	ands	r6, r3
	lsls	r3, r6, #2
	ldr	r0, [r2, r3]
	bl 0x0200d6c4
	mov	r0, fp
	movs	r2, #22
	ldrsh	r3, [r0, r2]
	movs	r1, #18
	ldrsh	r2, [r0, r1]
	lsls	r3, r3, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
.L_020024ec:
	b.n	.L_02002508
	.2byte 0x0000
	.4byte 0x00000001
	.4byte 0x02000240
	.4byte 0x000000f7
	.4byte 0x0200244c
	.4byte 0x03000258
	.2byte 0xdb84
	.2byte 0x0200
.L_02002508:
	ldrh	r3, [r0, r3]
	mov	r2, fp
	movs	r1, #10
	strh	r3, [r2, #16]
	mov	r0, sl
	bl 0x0200d710
	adds	r1, r0, #0
	b.n	.L_02002616
.L_0200251a:
	mov	r3, sl
	subs	r3, #33
.L_0200251e:
	cmp	r3, #1
	bhi.n	.L_020025c8
	movs	r0, #224
	lsls	r0, r0, #2
	bl 0x0200d7a8
	cmp	r0, #0
.L_0200252c:
	bne.n	.L_02002566
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r5, r0
	ldr	r0, [r3, #0]
	bl 0x0200d7a0
	movs	r1, #32
	adds	r5, r0, #0
	ldr	r3, [pc, #104]
	mov	r0, fp
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c2a
	adds	r1, r2, #0
	movs	r6, #0
	adds	r1, #13
.L_0200254e:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	adds	r6, r6, r3
	cmp	r2, r1
	ble.n	.L_0200254e
	adds	r0, r6, #0
	bl 0x0200ac30
	movs	r0, #224
	lsls	r0, r0, #2
	bl 0x0200d7b0
.L_02002566:
	ldr	r1, [pc, #60]
.L_02002568:
	mov	r3, sl
	subs	r3, #1
	ands	r3, r1
	mov	r2, fp
	strh	r3, [r2, #18]
	mov	r0, fp
	movs	r2, #3
	strh	r2, [r0, #22]
	strh	r2, [r0, #24]
	movs	r2, #4
	eors	r3, r1
	strh	r2, [r0, #26]
	strh	r2, [r0, #28]
	adds	r3, #6
	lsls	r3, r3, #1
	ldrsh	r6, [r0, r3]
	ldr	r2, [pc, #32]
	movs	r3, #7
	ands	r6, r3
	lsls	r3, r6, #2
	ldr	r0, [r2, r3]
	bl 0x0200d6c4
	mov	r0, fp
	movs	r2, #22
	ldrsh	r3, [r0, r2]
	movs	r1, #18
	ldrsh	r2, [r0, r1]
	b.n	.L_020025b0
	.2byte 0x0000
	.4byte 0x00000001
	.4byte 0x03000258
	.2byte 0xdb84
	.2byte 0x0200
.L_020025b0:
	lsls	r3, r3, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
.L_020025b6:
	ldrh	r3, [r0, r3]
	mov	r2, fp
	movs	r1, #10
	strh	r3, [r2, #16]
	mov	r0, sl
	bl 0x0200d710
	adds	r1, r0, #0
	b.n	.L_02002616
.L_020025c8:
	mov	r3, sl
	cmp	r3, #50
	bne.n	.L_0200261e
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r5, r0
	ldr	r0, [r3, #0]
	bl 0x0200d7a0
	movs	r1, #32
	adds	r5, r0, #0
	ldr	r3, [pc, #812]
	mov	r0, fp
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c2a
	adds	r1, r2, #0
	movs	r6, #0
	adds	r1, #13
.L_020025ee:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	adds	r6, r6, r3
	cmp	r2, r1
	ble.n	.L_020025ee
	adds	r0, r6, #0
	bl 0x0200ac30
	movs	r3, #0
	mov	r1, fp
	mov	r2, fp
	mov	r0, fp
	strh	r3, [r1, #18]
	strh	r3, [r2, #22]
	strh	r3, [r0, #24]
	strh	r3, [r1, #26]
	strh	r3, [r2, #28]
	ldrh	r3, [r2, #0]
	movs	r1, #55
	strh	r3, [r0, #16]
.L_02002616:
	ldr	r0, [pc, #760]
	bl 0x0200d988
	b.n	.L_02002c0e
.L_0200261e:
	mov	r1, r8
	mov	r2, r8
	adds	r1, #228
	adds	r2, #232
	mov	r3, sl
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	cmp	r3, #55
	beq.n	.L_02002632
	b.n	.L_020027ce
.L_02002632:
	movs	r7, #160
	lsls	r7, r7, #1
	movs	r3, #136
	add	r7, r8
	lsls	r3, r3, #18
	str	r3, [r7, #8]
	ldr	r3, [pc, #724]
	ldr	r5, [pc, #724]
	adds	r3, #136
	str	r3, [r7, #48]
	adds	r3, r5, #0
	movs	r0, #0
	adds	r3, #34
	str	r0, [r7, #12]
	str	r3, [r7, #52]
	ldr	r2, [sp, #12]
	movs	r1, #34
	mov	r9, r0
	mov	sl, r1
	ldr	r0, [r2, #0]
	ldr	r1, [r7, #16]
	ldr	r3, [pc, #700]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x68bb
	ldr	r2, [pc, #692]
	adds	r0, r0, r3
	str	r0, [r7, #0]
	ldr	r1, [sp, #8]
	adds	r5, #68
	ldr	r0, [r1, #0]
	ldr	r1, [r7, #20]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x68fb
	ldr	r1, [pc, #664]
	adds	r0, r0, r3
	str	r0, [r7, #4]
	movs	r7, #188
	lsls	r7, r7, #1
	movs	r3, #136
	add	r7, r8
	lsls	r3, r3, #19
	movs	r2, #136
	str	r3, [r7, #8]
	lsls	r2, r2, #1
	mov	r3, r9
	str	r3, [r7, #12]
	adds	r3, r1, r2
	str	r5, [r7, #52]
	str	r3, [r7, #48]
	ldr	r3, [sp, #12]
	movs	r0, #68
	mov	r8, r0
	ldr	r1, [r7, #16]
	ldr	r0, [r3, #0]
	ldr	r2, [pc, #632]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x68bb
	ldr	r2, [pc, #624]
	adds	r0, r0, r3
	str	r0, [r7, #0]
	ldr	r3, [sp, #8]
	ldr	r1, [r7, #20]
	ldr	r0, [r3, #0]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x68fb
	movs	r5, #13
	adds	r0, r0, r3
.L_020026c0:
	str	r0, [r7, #4]
	movs	r0, #9
	bl 0x0200d8a0
	movs	r1, #3
	bl 0x0200d930
	movs	r0, #10
	bl 0x0200d8a0
	movs	r1, #3
	bl 0x0200d930
	movs	r0, #11
	bl 0x0200d8a0
	movs	r1, #3
	bl 0x0200d930
	movs	r0, #12
	bl 0x0200d8a0
	movs	r1, #3
	bl 0x0200d930
	mov	r3, r9
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #12
	movs	r2, #1
	movs	r3, #21
	str	r5, [sp, #4]
	bl 0x0200d828
	mov	r0, sl
	movs	r1, #12
	movs	r2, #1
	movs	r3, #21
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d828
	mov	r1, r8
	str	r1, [sp, #0]
	movs	r0, #68
	movs	r1, #12
	movs	r2, #1
	movs	r3, #21
	str	r5, [sp, #4]
	bl 0x0200d828
	movs	r2, #1
	str	r2, [sp, #0]
	movs	r0, #0
	movs	r1, #13
	movs	r2, #33
	movs	r3, #21
	str	r5, [sp, #4]
	bl 0x0200d828
	movs	r3, #35
	str	r3, [sp, #0]
	mov	r8, r3
	movs	r0, #34
	movs	r1, #13
	movs	r2, #33
	movs	r3, #21
	str	r5, [sp, #4]
	bl 0x0200d828
	movs	r0, #69
	str	r0, [sp, #0]
	movs	r1, #13
.L_02002752:
	mov	sl, r0
	movs	r2, #33
	movs	r0, #68
	movs	r3, #21
	str	r5, [sp, #4]
	bl 0x0200d828
	mov	r2, fp
	movs	r1, #16
	ldrsh	r6, [r2, r1]
	ldr	r2, [pc, #440]
	lsls	r3, r6, #2
	ldrsh	r7, [r2, r3]
	adds	r3, #2
	ldrsh	r1, [r2, r3]
	adds	r0, r7, #0
	mov	r9, r1
	movs	r1, #1
	str	r1, [sp, #0]
	movs	r2, #13
	mov	r1, r9
	movs	r3, #21
	str	r5, [sp, #4]
	bl 0x0200d828
	mov	r2, r8
	adds	r0, r7, #0
	str	r2, [sp, #0]
	adds	r0, #42
	mov	r1, r9
	movs	r2, #13
	movs	r3, #21
	str	r5, [sp, #4]
	bl 0x0200d828
	mov	r3, sl
	adds	r0, r7, #0
	str	r3, [sp, #0]
	adds	r0, #84
	mov	r1, r9
	movs	r2, #13
	movs	r3, #21
	str	r5, [sp, #4]
	bl 0x0200d828
	movs	r5, #0
.L_020027ae:
	ldr	r3, [pc, #372]
	ldrsb	r3, [r3, r5]
	cmp	r3, r6
	beq.n	.L_020027c2
	adds	r0, r5, #0
	adds	r0, #64
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
.L_020027c2:
	adds	r5, #1
	cmp	r5, #9
	bls.n	.L_020027ae
	bl 0x0200d7e8
	b.n	.L_02002c0e
.L_020027ce:
	movs	r7, #160
	lsls	r7, r7, #1
	movs	r3, #136
	add	r7, r8
	lsls	r3, r3, #18
	str	r3, [r7, #8]
	ldr	r3, [pc, #312]
	ldr	r5, [pc, #312]
	adds	r3, #136
	str	r3, [r7, #48]
	adds	r3, r5, #0
	movs	r0, #0
	adds	r3, #34
	str	r0, [r7, #12]
	str	r3, [r7, #52]
	ldr	r1, [sp, #12]
	mov	r9, r0
	ldr	r2, [pc, #296]
	ldr	r0, [r1, #0]
	ldr	r1, [r7, #16]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x68bb
	ldr	r1, [r7, #20]
	adds	r0, r0, r3
	str	r0, [r7, #0]
	ldr	r3, [sp, #8]
	ldr	r2, [pc, #276]
	ldr	r0, [r3, #0]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x68fb
	ldr	r1, [pc, #260]
	adds	r0, r0, r3
	str	r0, [r7, #4]
	movs	r7, #188
	lsls	r7, r7, #1
	movs	r3, #136
	add	r7, r8
	lsls	r3, r3, #19
	movs	r2, #136
	str	r3, [r7, #8]
	lsls	r2, r2, #1
.L_02002824:
	mov	r3, r9
	str	r3, [r7, #12]
	adds	r5, #68
	adds	r3, r1, r2
	str	r3, [r7, #48]
	str	r5, [r7, #52]
	ldr	r3, [sp, #12]
	movs	r0, #68
	ldr	r1, [r7, #16]
	ldr	r2, [pc, #228]
	mov	r8, r0
	ldr	r0, [r3, #0]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x68bb
	ldr	r1, [r7, #20]
	adds	r0, r0, r3
	str	r0, [r7, #0]
	ldr	r3, [sp, #8]
	ldr	r2, [pc, #208]
	ldr	r0, [r3, #0]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x68fb
	movs	r6, #1
	adds	r0, r0, r3
	str	r0, [r7, #4]
	adds	r3, r6, #0
	mov	r0, sl
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_02002928
	mov	r1, r9
	movs	r5, #9
	movs	r0, #0
	movs	r2, #102
	movs	r3, #13
	str	r1, [sp, #4]
	str	r5, [sp, #0]
	bl 0x0200a17c
	mov	r2, r9
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #10
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200d828
	mov	r3, r9
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #10
	movs	r3, #12
	str	r6, [sp, #4]
	bl 0x0200d828
	movs	r0, #34
	movs	r1, #0
	movs	r2, #10
	movs	r3, #12
	str	r0, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200d828
	mov	r1, r8
	str	r1, [sp, #0]
	movs	r0, #68
	movs	r1, #0
	movs	r2, #10
	movs	r3, #12
	str	r6, [sp, #4]
	bl 0x0200d828
	movs	r2, #34
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #34
	movs	r2, #102
	movs	r3, #10
	str	r5, [sp, #0]
	bl 0x0200a17c
	mov	r3, r9
	movs	r0, #34
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	movs	r1, #33
	movs	r0, #0
	movs	r2, #10
	movs	r3, #10
	bl 0x0200d828
	movs	r1, #34
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	movs	r0, #34
	movs	r1, #33
	movs	r2, #10
	movs	r3, #10
	bl 0x0200d828
	mov	r2, r8
	movs	r3, #34
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #68
	movs	r1, #33
	movs	r2, #10
	movs	r3, #10
	bl 0x0200d828
	b.n	.L_02002a0a
	.2byte 0x0000
	.4byte 0x03000258
	.4byte 0x000000f7
	.4byte 0x02010000
	.4byte 0x02024000
	.4byte 0x0300021c
	.4byte 0x0200dbac
	.2byte 0xdbcc
	.2byte 0x0200
.L_02002928:
	mov	r0, r9
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	movs	r1, #0
	movs	r0, #10
	movs	r2, #92
	movs	r3, #13
	bl 0x0200d828
	mov	r1, r9
	str	r1, [sp, #4]
	movs	r5, #24
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200d828
	movs	r3, #23
	str	r3, [sp, #0]
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	movs	r3, #12
	str	r6, [sp, #4]
	bl 0x0200d828
	movs	r3, #57
	str	r3, [sp, #0]
	movs	r0, #57
	movs	r1, #0
	movs	r2, #10
	movs	r3, #12
	str	r6, [sp, #4]
	bl 0x0200d828
	movs	r3, #91
	str	r3, [sp, #0]
	movs	r0, #91
	movs	r1, #0
	movs	r2, #10
	movs	r3, #12
	str	r6, [sp, #4]
	bl 0x0200d828
	mov	r2, r9
	movs	r3, #34
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #10
	movs	r1, #34
	movs	r2, #92
	movs	r3, #10
	bl 0x0200d828
	movs	r0, #34
	str	r0, [sp, #4]
	movs	r1, #33
	movs	r0, #24
	movs	r2, #10
	movs	r3, #10
	str	r5, [sp, #0]
	bl 0x0200d828
	movs	r3, #58
	movs	r1, #34
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	movs	r0, #58
	movs	r1, #33
	movs	r2, #10
	movs	r3, #10
	bl 0x0200d828
	movs	r3, #92
	movs	r2, #34
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #10
.L_020029c8:
	movs	r0, #92
	movs	r1, #33
	movs	r2, #10
	bl 0x0200d828
	movs	r1, #160
	movs	r2, #152
	movs	r0, #9
	lsls	r1, r1, #14
	lsls	r2, r2, #16
	bl 0x0200d8e0
	movs	r1, #192
	movs	r2, #152
	movs	r0, #10
	lsls	r1, r1, #13
	lsls	r2, r2, #16
	bl 0x0200d8e0
	movs	r1, #184
	movs	r2, #152
	movs	r0, #11
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200d8e0
	movs	r1, #200
	movs	r2, #152
	movs	r0, #12
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200d8e0
.L_02002a0a:
	movs	r0, #9
	bl 0x0200d8a0
	movs	r1, #3
	bl 0x0200d930
	movs	r0, #10
	bl 0x0200d8a0
	movs	r1, #3
	bl 0x0200d930
	movs	r0, #11
	bl 0x0200d8a0
	movs	r1, #3
	bl 0x0200d930
	movs	r0, #12
	bl 0x0200d8a0
	movs	r1, #3
	bl 0x0200d930
	mov	r0, fp
	movs	r3, #16
	ldrsh	r6, [r0, r3]
	ldr	r2, [pc, #292]
	lsls	r3, r6, #2
	ldrsh	r7, [r2, r3]
	adds	r3, #2
	ldrsh	r0, [r2, r3]
	movs	r3, #10
	mov	r9, r0
	movs	r5, #13
	str	r3, [sp, #0]
	adds	r0, r7, #0
	mov	r1, r9
	movs	r2, #13
	movs	r3, #21
	str	r5, [sp, #4]
	bl 0x0200d828
	movs	r3, #44
	adds	r0, r7, #0
	str	r3, [sp, #0]
	adds	r0, #42
	mov	r1, r9
	movs	r2, #13
	movs	r3, #21
	str	r5, [sp, #4]
	bl 0x0200d828
	movs	r3, #78
	adds	r0, r7, #0
	str	r3, [sp, #0]
	adds	r0, #84
	mov	r1, r9
	movs	r2, #13
	movs	r3, #21
	str	r5, [sp, #4]
	bl 0x0200d828
	movs	r5, #0
.L_02002a8a:
	ldr	r3, [pc, #224]
	ldrsb	r3, [r3, r5]
	cmp	r3, r6
	beq.n	.L_02002a9e
	adds	r0, r5, #0
	adds	r0, #64
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
.L_02002a9e:
	adds	r5, #1
	cmp	r5, #9
	bls.n	.L_02002a8a
	bl 0x0200d7e8
	ldr	r3, [pc, #196]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	adds	r5, r0, #0
	movs	r0, #10
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02002ad2
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200d808
	str	r0, [r5, #20]
	str	r0, [r5, #12]
.L_02002ad2:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02002b7c
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200d7b0
	ldr	r5, [pc, #136]
	movs	r3, #22
	ldrsh	r2, [r5, r3]
	movs	r0, #26
	ldrsh	r3, [r5, r0]
	cmp	r2, r3
	blt.n	.L_02002b04
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #130
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02002b22
.L_02002b04:
	movs	r1, #18
	ldrsh	r0, [r5, r1]
	movs	r2, #22
	ldrsh	r1, [r5, r2]
	bl 0x02008218
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #130
	bl 0x0200d7b0
	movs	r0, #161
	lsls	r0, r0, #2
	bl 0x0200d7b0
.L_02002b22:
	movs	r3, #22
	ldrsh	r1, [r5, r3]
	movs	r0, #28
	ldrsh	r3, [r5, r0]
	cmp	r1, r3
	bge.n	.L_02002b36
	movs	r2, #18
	ldrsh	r0, [r5, r2]
	bl 0x020082a4
.L_02002b36:
	ldr	r3, [pc, #56]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	movs	r1, #8
	bl 0x0200d5c4
	mov	r1, fp
	ldrh	r0, [r1, #18]
	ldr	r3, [pc, #24]
	eors	r0, r3
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	bl 0x0200d688
	movs	r1, #144
	ldr	r0, [pc, #28]
	lsls	r1, r1, #3
	bl 0x0200d730
	b.n	.L_02002c0e
	.2byte 0x0000
	.4byte 0x00000001
	.4byte 0x0200dbac
	.4byte 0x0200dbd6
	.4byte 0x02000240
	.4byte 0x0200244c
	.2byte 0xa23d
	.2byte 0x0200
.L_02002b7c:
	mov	r3, fp
	movs	r2, #18
	ldrsh	r0, [r3, r2]
	movs	r2, #22
	ldrsh	r1, [r3, r2]
	bl 0x02008218
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #130
	bl 0x0200d7b0
	movs	r0, #161
	lsls	r0, r0, #2
	bl 0x0200d7b0
	mov	r1, fp
	movs	r3, #18
	ldrsh	r0, [r1, r3]
	movs	r2, #22
	ldrsh	r1, [r1, r2]
	bl 0x020082a4
	b.n	.L_02002c0e
.L_02002bac:
	ldr	r3, [pc, #112]
	cmp	r2, r3
	beq.n	.L_02002c0e
	ldr	r3, [pc, #112]
	cmp	r2, r3
	beq.n	.L_02002c0e
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L_02002c0e
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02002c0e
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #133
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02002c0e
	ldr	r3, [pc, #80]
	movs	r0, #18
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_02002bfa
	movs	r3, #13
	movs	r2, #79
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #26
	movs	r1, #79
	movs	r2, #1
	movs	r3, #2
	bl 0x0200d820
	b.n	.L_02002c0e
.L_02002bfa:
	movs	r3, #18
	movs	r2, #79
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #26
	movs	r1, #79
	movs	r2, #1
	movs	r3, #2
	bl 0x0200d820
.L_02002c0e:
	movs	r0, #0
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x000000f9
	.4byte 0x000000fa
	.4byte 0x000000f8
	.2byte 0x244c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #36]
	ldr	r7, [pc, #36]
	ldr	r2, [r3, #0]
	str	r0, [r3, #0]
	sub	sp, #8
	mov	r8, r2
.L_02002c42:
	adds	r2, r7, #0
	movs	r5, #7
.L_02002c46:
	ldr	r3, [pc, #16]
	subs	r5, #1
	strh	r3, [r2, #0]
	adds	r2, #2
	cmp	r5, #0
	bge.n	.L_02002c46
	movs	r5, #0
	b.n	.L_02002c64
	.2byte 0x0000
	.4byte 0xffffffff
	.4byte 0x030011bc
	.2byte 0x244c
	.2byte 0x0200
.L_02002c64:
	bl 0x0200d738
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r0, r0, #1
	ldrsh	r3, [r7, r0]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02002c64
	strh	r5, [r7, r0]
	adds	r5, #1
	cmp	r5, #7
	ble.n	.L_02002c64
	movs	r3, #0
	mov	r6, sp
	str	r3, [r6, #4]
	str	r3, [sp, #0]
	ldr	r3, [pc, #84]
	movs	r5, #0
	mov	ip, r3
	adds	r0, r7, #0
.L_02002c90:
	movs	r4, #0
	mov	r1, ip
.L_02002c94:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	lsls	r3, r3, #24
	mov	lr, r3
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	mov	r3, lr
	asrs	r3, r3, #24
	mov	lr, r3
	cmp	r2, lr
	bne.n	.L_02002cb8
	movs	r3, #1
	ands	r3, r5
	lsls	r3, r3, #2
	adds	r3, r3, r6
	ldr	r2, [r3, #0]
	adds	r2, #1
	str	r2, [r3, #0]
.L_02002cb8:
	adds	r4, #1
	cmp	r4, #3
	ble.n	.L_02002c94
	adds	r5, #1
	adds	r0, #2
	cmp	r5, #7
	ble.n	.L_02002c90
	ldr	r3, [sp, #0]
	cmp	r3, #0
	beq.n	.L_02002c42
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq.n	.L_02002c42
	ldr	r3, [pc, #16]
	mov	r2, r8
	str	r2, [r3, #0]
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200dbe0
	.4byte 0x030011bc
	.4byte 0x23014a03
	.4byte 0x005b4003
	.4byte 0x231618d2
	.4byte 0x47705ed0
	.4byte 0x0200244c
	.4byte 0x22124b01
	.4byte 0x47705e98
	.2byte 0x244c
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200acfc
	cmp	r0, #0
	beq.n	.L_02002d1c
	movs	r0, #128
	lsls	r0, r0, #2
	subs	r0, r0, r5
	b.n	.L_02002d1e
.L_02002d1c:
	adds	r0, r5, #0
.L_02002d1e:
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200acfc
	cmp	r0, #0
	beq.n	.L_02002d34
	movs	r0, #64
	subs	r0, r0, r5
	lsls	r0, r0, #3
	b.n	.L_02002d36
.L_02002d34:
	lsls	r0, r5, #3
.L_02002d36:
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200acfc
	cmp	r0, #0
	beq.n	.L_02002d4a
	negs	r0, r5
	lsls	r0, r0, #3
	b.n	.L_02002d4c
.L_02002d4a:
	lsls	r0, r5, #3
.L_02002d4c:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200acfc
	cmp	r0, #0
	beq.n	.L_02002d6c
	movs	r0, #128
	movs	r3, #255
	lsls	r0, r0, #8
	lsls	r3, r3, #8
	subs	r0, r0, r5
	adds	r3, #255
	ands	r0, r3
	b.n	.L_02002d74
.L_02002d6c:
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	ands	r0, r5
.L_02002d74:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	sl, r0
	adds	r0, r1, #0
	mov	r8, r3
	adds	r6, r2, #0
	bl 0x0200ad08
	adds	r5, r0, #0
	mov	r0, r8
	bl 0x0200ad50
	adds	r3, r0, #0
	lsls	r5, r5, #16
	lsls	r6, r6, #16
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	mov	r0, sl
.L_02002da0:
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl 0x0200d8e8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r5, #128
	lsls	r5, r5, #8
	movs	r1, #144
	movs	r2, #132
	movs	r6, #148
	adds	r3, r5, #0
	lsls	r6, r6, #1
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #8
	bl 0x0200ad78
	adds	r1, r6, #0
	adds	r3, r5, #0
	movs	r0, #13
	movs	r2, #248
	bl 0x0200ad78
	movs	r2, #140
	lsls	r2, r2, #1
	movs	r0, #14
	adds	r1, r6, #0
	adds	r3, r5, #0
	bl 0x0200ad78
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	bl 0x0200d880
	movs	r0, #0
	bl 0x0200d9e8
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200d970
	movs	r0, #128
	movs	r1, #1
	movs	r2, #132
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x0200d978
	movs	r0, #5
	movs	r1, #4
	bl 0x0200d8f0
	movs	r0, #6
	movs	r1, #4
	bl 0x0200d8f0
	movs	r0, #7
	movs	r1, #4
	bl 0x0200d8f0
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d8a8
	movs	r1, #128
	movs	r2, #128
	movs	r0, #5
.L_02002e4c:
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d8a8
	movs	r1, #128
	movs	r2, #128
	movs	r0, #6
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d8a8
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #7
	bl 0x0200d8a8
	movs	r0, #28
	bl 0x0200ad20
	movs	r2, #136
	adds	r1, r0, #0
	lsls	r2, r2, #1
	movs	r0, #4
	bl 0x0200d8c0
	movs	r0, #28
	bl 0x0200ad20
	movs	r2, #128
	adds	r1, r0, #0
	lsls	r2, r2, #1
	movs	r0, #7
	bl 0x0200d8c0
	movs	r0, #27
	bl 0x0200ad20
	movs	r2, #248
	adds	r1, r0, #0
	movs	r0, #5
	bl 0x0200d8c0
	movs	r0, #27
	bl 0x0200ad20
	movs	r2, #140
	adds	r1, r0, #0
	lsls	r2, r2, #1
	movs	r0, #6
	bl 0x0200d8c0
	movs	r0, #4
	bl 0x0200d8d8
	movs	r0, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #4
	bl 0x0200d948
	movs	r0, #7
	bl 0x0200d8d8
	movs	r0, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #7
	bl 0x0200d948
	movs	r0, #5
	bl 0x0200d8d8
	movs	r0, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #5
	bl 0x0200d948
	movs	r0, #6
	bl 0x0200d8d8
	movs	r0, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #6
	bl 0x0200d948
	bl 0x0200d980
	movs	r0, #20
	bl 0x0200d728
	movs	r0, #138
	bl 0x0200da68
	movs	r0, #20
	bl 0x0200d728
	movs	r1, #172
	movs	r2, #148
	movs	r3, #192
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	lsls	r3, r3, #8
	movs	r0, #8
	bl 0x0200ad78
	movs	r0, #8
	bl 0x0200d8a0
	movs	r6, #128
	adds	r5, r0, #0
	movs	r2, #85
	movs	r0, #128
	lsls	r6, r6, #8
	adds	r2, r2, r5
	lsls	r0, r0, #9
	movs	r3, #0
	str	r0, [r5, #48]
	str	r6, [r5, #52]
	strb	r3, [r2, #0]
	mov	sl, r2
	ldr	r2, [r5, #12]
	movs	r3, #176
	lsls	r3, r3, #15
	mov	r8, r0
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200d7f8
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200d7c8
	adds	r0, r5, #0
	bl 0x0200d800
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200d7c8
	mov	r0, sl
	movs	r3, #2
	strb	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #16
	str	r3, [r5, #20]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r9, r3
	str	r3, [r5, #52]
	ldr	r0, [pc, #644]
	ldr	r3, [r5, #16]
	movs	r2, #192
	lsls	r2, r2, #10
	ldr	r1, [r5, #8]
	adds	r3, r3, r0
	mov	sl, r2
	str	r2, [r5, #48]
	adds	r0, r5, #0
	ldr	r2, [r5, #12]
	bl 0x0200d7f8
	adds	r0, r5, #0
	bl 0x0200d800
	movs	r0, #12
	bl 0x0200d728
	mov	r2, r8
	str	r2, [r5, #48]
	str	r6, [r5, #52]
	movs	r0, #41
	bl 0x0200ad20
	movs	r3, #132
	adds	r1, r0, #0
	ldr	r2, [r5, #12]
	lsls	r1, r1, #16
	lsls	r3, r3, #17
	adds	r0, r5, #0
	bl 0x0200d7f8
	adds	r0, r5, #0
	bl 0x0200d800
	movs	r0, #6
	bl 0x0200d728
	movs	r0, #152
	bl 0x0200da68
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	mov	r0, r9
	mov	r3, sl
	str	r3, [r5, #48]
	str	r0, [r5, #52]
	movs	r0, #37
	bl 0x0200ad20
	movs	r3, #132
	adds	r1, r0, #0
	lsls	r3, r3, #17
	ldr	r2, [r5, #12]
	lsls	r1, r1, #16
	adds	r0, r5, #0
	bl 0x0200d7f8
	adds	r0, r5, #0
	bl 0x0200d800
	movs	r0, #6
	bl 0x0200d728
	movs	r0, #13
	movs	r1, #8
	bl 0x0200d8f0
	movs	r0, #14
	movs	r1, #8
	bl 0x0200d8f0
	mov	r1, r8
	adds	r2, r6, #0
	movs	r0, #8
	bl 0x0200d8a8
	mov	r1, r8
	adds	r2, r6, #0
	movs	r0, #13
	bl 0x0200d8a8
	mov	r1, r8
	adds	r2, r6, #0
	movs	r0, #14
	bl 0x0200d8a8
	movs	r0, #36
	bl 0x0200ad20
	movs	r2, #132
	adds	r1, r0, #0
	lsls	r2, r2, #1
	movs	r0, #8
	bl 0x0200d8c0
	movs	r0, #37
	bl 0x0200ad20
	movs	r2, #248
	adds	r1, r0, #0
	movs	r0, #13
	bl 0x0200d8c0
	movs	r0, #37
	bl 0x0200ad20
	movs	r2, #140
	adds	r1, r0, #0
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x0200d8c0
	movs	r0, #8
	bl 0x0200d8d8
	adds	r0, r6, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #8
	bl 0x0200d948
	movs	r0, #13
	bl 0x0200d8d8
	adds	r0, r6, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #13
	bl 0x0200d948
	movs	r0, #14
	bl 0x0200d8d8
	adds	r0, r6, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #14
	bl 0x0200d948
	ldr	r0, [pc, #356]
	bl 0x0200d938
	movs	r1, #128
	movs	r2, #10
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200d960
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r0, #8
	movs	r1, #3
	bl 0x0200d900
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r0, #8
	movs	r1, #2
	bl 0x0200d910
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d8f8
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d8f8
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d8f8
	movs	r1, #3
	movs	r0, #7
	bl 0x0200d8f8
	movs	r0, #15
	bl 0x0200d878
	mov	r1, r9
	mov	r2, r8
	movs	r0, #4
	bl 0x0200d8a8
	movs	r1, #243
	movs	r2, #243
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #5
	adds	r1, #102
	adds	r2, #51
	bl 0x0200d8a8
	movs	r2, #217
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #228]
	adds	r2, #153
	bl 0x0200d8a8
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #7
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d8a8
	mov	r1, r9
	mov	r2, r8
	movs	r0, #8
	bl 0x0200d8a8
	movs	r1, #243
	movs	r2, #243
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #13
	adds	r1, #102
	adds	r2, #51
	bl 0x0200d8a8
	movs	r2, #217
	lsls	r2, r2, #8
	ldr	r1, [pc, #172]
	adds	r2, #153
	movs	r0, #14
	bl 0x0200d8a8
	movs	r0, #3
	bl 0x0200ad38
	movs	r5, #3
	adds	r1, r0, #0
	negs	r5, r5
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d9f8
	adds	r0, r5, #0
	bl 0x0200ad38
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #8
	bl 0x0200d9f8
	movs	r0, #1
	bl 0x0200d728
	movs	r0, #3
	bl 0x0200ad38
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #5
	bl 0x0200d9f8
	adds	r0, r5, #0
	bl 0x0200ad38
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #13
	bl 0x0200d9f8
	movs	r0, #1
	bl 0x0200d728
	movs	r0, #3
	bl 0x0200ad38
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #7
	bl 0x0200d9f8
	adds	r0, r5, #0
	bl 0x0200ad38
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #14
	bl 0x0200d9f8
	movs	r0, #1
	bl 0x0200d728
	movs	r0, #3
	bl 0x0200ad38
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #6
	bl 0x0200d9f8
	movs	r0, #1
	bl 0x0200d728
	movs	r0, #6
	bl 0x0200d878
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0xfff00000
	.4byte 0x000029cd
	.2byte 0xb333
	.2byte 0x0001
	push	{r5, lr}
	bl 0x0200d880
	movs	r0, #0
	bl 0x0200d9e8
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200d970
	movs	r0, #128
	movs	r1, #1
	movs	r2, #132
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x0200d978
	movs	r0, #5
	movs	r1, #4
	bl 0x0200d8f0
	movs	r0, #6
	movs	r1, #4
	bl 0x0200d8f0
	movs	r0, #7
	movs	r1, #4
	bl 0x0200d8f0
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d8a8
	movs	r1, #128
	movs	r2, #128
	movs	r0, #5
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d8a8
	movs	r1, #128
	movs	r2, #128
	movs	r0, #6
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d8a8
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #7
	bl 0x0200d8a8
	movs	r0, #28
	bl 0x0200ad20
	movs	r2, #136
	adds	r1, r0, #0
	lsls	r2, r2, #1
	movs	r0, #4
	bl 0x0200d8c0
	movs	r0, #28
	bl 0x0200ad20
	movs	r2, #128
	adds	r1, r0, #0
	lsls	r2, r2, #1
	movs	r0, #7
	bl 0x0200d8c0
	movs	r0, #27
	bl 0x0200ad20
	movs	r2, #248
	adds	r1, r0, #0
	movs	r0, #5
	bl 0x0200d8c0
	movs	r0, #27
	bl 0x0200ad20
	movs	r2, #140
	adds	r1, r0, #0
	lsls	r2, r2, #1
	movs	r0, #6
	bl 0x0200d8c0
	movs	r0, #4
	bl 0x0200d8d8
	movs	r0, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #4
	bl 0x0200d948
	movs	r0, #7
	bl 0x0200d8d8
	movs	r0, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #7
	bl 0x0200d948
	movs	r0, #5
	bl 0x0200d8d8
	movs	r0, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #5
	bl 0x0200d948
	movs	r0, #6
	bl 0x0200d8d8
	movs	r0, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #6
	bl 0x0200d948
	bl 0x0200d980
	ldr	r0, [pc, #352]
	bl 0x0200d938
	movs	r1, #128
	movs	r2, #10
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200d960
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r0, #8
	movs	r1, #3
	bl 0x0200d900
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r0, #8
	movs	r1, #2
	bl 0x0200d910
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d8f8
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d8f8
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d8f8
	movs	r1, #3
	movs	r0, #7
	bl 0x0200d8f8
	movs	r0, #15
	bl 0x0200d878
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200d8a8
	movs	r1, #243
	movs	r2, #243
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #5
	adds	r1, #102
	adds	r2, #51
	bl 0x0200d8a8
	movs	r2, #217
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #224]
	adds	r2, #153
	bl 0x0200d8a8
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #7
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d8a8
	movs	r1, #128
	movs	r2, #128
	movs	r0, #8
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200d8a8
	movs	r1, #243
	movs	r2, #243
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #13
	adds	r1, #102
	adds	r2, #51
	bl 0x0200d8a8
	movs	r2, #217
	lsls	r2, r2, #8
	ldr	r1, [pc, #160]
	adds	r2, #153
	movs	r0, #14
	bl 0x0200d8a8
	movs	r0, #3
	bl 0x0200ad38
	movs	r5, #3
	adds	r1, r0, #0
	negs	r5, r5
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d9f8
	adds	r0, r5, #0
	bl 0x0200ad38
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #8
	bl 0x0200d9f8
	movs	r0, #1
	bl 0x0200d728
	movs	r0, #3
	bl 0x0200ad38
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #5
	bl 0x0200d9f8
	adds	r0, r5, #0
	bl 0x0200ad38
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #13
	bl 0x0200d9f8
	movs	r0, #1
	bl 0x0200d728
	movs	r0, #3
	bl 0x0200ad38
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #7
	bl 0x0200d9f8
	adds	r0, r5, #0
	bl 0x0200ad38
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #14
	bl 0x0200d9f8
	movs	r0, #1
	bl 0x0200d728
	movs	r0, #3
	bl 0x0200ad38
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #6
	bl 0x0200d9f8
	movs	r0, #1
	bl 0x0200d728
	movs	r0, #6
	bl 0x0200d878
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000029ca
	.2byte 0xb333
	.2byte 0x0001
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	bl 0x0200d880
	movs	r0, #0
	bl 0x0200d9e8
	movs	r6, #136
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	lsls	r6, r6, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	bl 0x0200d978
	adds	r2, r6, #0
	movs	r0, #4
	movs	r1, #248
	movs	r3, #0
	bl 0x0200ad78
	movs	r0, #7
	movs	r1, #224
	movs	r2, #248
	movs	r3, #0
	bl 0x0200ad78
	movs	r3, #132
	lsls	r3, r3, #1
	mov	sl, r3
	mov	r2, sl
	movs	r0, #5
	movs	r1, #232
	movs	r3, #0
	bl 0x0200ad78
	movs	r3, #140
	lsls	r3, r3, #1
	mov	r8, r3
	mov	r2, r8
	movs	r0, #6
	movs	r1, #224
	movs	r3, #0
	bl 0x0200ad78
	movs	r5, #128
	movs	r2, #128
	lsls	r5, r5, #7
	lsls	r2, r2, #1
	movs	r0, #9
	movs	r1, #248
	movs	r3, #0
	bl 0x0200ad78
	adds	r1, r6, #0
	mov	r2, sl
	adds	r3, r5, #0
	adds	r6, #16
	movs	r0, #8
	bl 0x0200ad78
	adds	r1, r6, #0
	adds	r3, r5, #0
	movs	r0, #13
	movs	r2, #248
	bl 0x0200ad78
	adds	r3, r5, #0
	mov	r2, r8
	adds	r1, r6, #0
	movs	r0, #14
	bl 0x0200ad78
	movs	r0, #8
	movs	r1, #6
	bl 0x0200d8f8
	movs	r0, #13
	movs	r1, #6
	bl 0x0200d8f8
	movs	r1, #6
	movs	r0, #14
	bl 0x0200d8f8
	bl 0x0200d9d0
	bl 0x0200d9d8
	movs	r0, #30
	bl 0x0200d878
	ldr	r0, [pc, #588]
	bl 0x0200d938
	movs	r1, #2
	movs	r0, #13
	bl 0x0200d908
	movs	r0, #4
	bl 0x0200d728
	movs	r1, #2
	movs	r0, #14
	bl 0x0200d908
	movs	r0, #10
	bl 0x0200d728
	movs	r0, #8
	movs	r1, #2
	bl 0x0200d910
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r1, #7
	movs	r0, #8
	bl 0x0200d8f8
	movs	r0, #15
	bl 0x0200d878
	movs	r1, #7
	movs	r0, #13
	bl 0x0200d8f8
	movs	r0, #10
	bl 0x0200d878
	movs	r1, #7
	movs	r0, #14
	bl 0x0200d8f8
	movs	r0, #5
	bl 0x0200d878
	movs	r5, #128
	movs	r1, #1
	movs	r0, #8
	bl 0x0200d8f8
	lsls	r5, r5, #8
	movs	r0, #15
	bl 0x0200d878
	adds	r0, r5, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #8
	bl 0x0200d948
	movs	r1, #1
	movs	r0, #13
	bl 0x0200d8f8
	movs	r0, #10
	bl 0x0200d878
	adds	r0, r5, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #13
	bl 0x0200d948
	movs	r1, #1
	movs	r0, #14
	bl 0x0200d8f8
	movs	r0, #5
	bl 0x0200d878
	adds	r0, r5, #0
	bl 0x0200ad50
	movs	r2, #0
	adds	r1, r0, #0
	movs	r0, #14
	bl 0x0200d948
	movs	r0, #20
	bl 0x0200d878
	movs	r0, #8
	movs	r1, #3
	bl 0x0200d900
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #9
	movs	r0, #8
	bl 0x0200d8a8
	movs	r0, #32
	bl 0x0200ad20
	mov	r2, sl
	adds	r1, r0, #0
	movs	r0, #8
	bl 0x0200d8c0
	movs	r0, #9
	movs	r1, #7
	movs	r2, #0
	bl 0x0200d928
	movs	r1, #6
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d928
	movs	r0, #8
	bl 0x0200d8d8
	movs	r0, #9
	movs	r1, #4
	movs	r2, #0
	bl 0x0200d920
	movs	r0, #7
	movs	r1, #4
	movs	r2, #0
	bl 0x0200d920
	movs	r0, #6
	movs	r1, #4
	movs	r2, #0
	bl 0x0200d920
	movs	r2, #0
	movs	r1, #4
	movs	r0, #5
	bl 0x0200d920
	movs	r0, #20
	bl 0x0200d878
	movs	r0, #65
	bl 0x0200d860
	movs	r1, #0
	movs	r0, #215
	bl 0x0200d890
	movs	r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d920
	movs	r0, #8
	bl 0x0200d8a0
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #35
	bl 0x0200ad20
	mov	r2, sl
	adds	r1, r0, #0
	movs	r0, #8
	bl 0x0200d8c8
	movs	r0, #1
	bl 0x0200d878
	movs	r0, #8
	bl 0x0200d8a0
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200d878
	movs	r1, #6
	movs	r2, #10
	adds	r1, #255
	movs	r0, #8
	bl 0x0200d960
	movs	r1, #0
	movs	r0, #8
	bl 0x0200d940
	movs	r0, #0
	bl 0x0200ad50
	adds	r1, r0, #0
	movs	r0, #8
	bl 0x0200d950
	movs	r0, #13
	movs	r1, #8
	movs	r2, #0
	bl 0x0200d920
	movs	r2, #0
	movs	r0, #14
	movs	r1, #8
	bl 0x0200d920
	movs	r0, #8
	movs	r1, #3
	bl 0x0200d900
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r1, #3
	movs	r0, #13
	bl 0x0200d8f8
	movs	r0, #3
	bl 0x0200d728
	movs	r0, #14
	movs	r1, #3
	bl 0x0200d900
	movs	r0, #4
	movs	r1, #9
	movs	r2, #0
	bl 0x0200d920
	movs	r0, #7
	movs	r1, #9
	movs	r2, #0
	bl 0x0200d920
	movs	r0, #5
	movs	r1, #9
	movs	r2, #0
	bl 0x0200d920
	movs	r2, #0
	movs	r0, #6
	movs	r1, #9
	bl 0x0200d920
	movs	r0, #9
	movs	r1, #0
	bl 0x0200d940
	movs	r1, #3
	movs	r0, #4
	bl 0x0200d8f8
	movs	r0, #2
	bl 0x0200d728
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d8f8
	movs	r1, #3
	movs	r0, #7
	bl 0x0200d8f8
	movs	r0, #1
	bl 0x0200d728
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d8f8
.L_020037b6:
	movs	r0, #30
	bl 0x0200d728
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x29d1
	.2byte 0x0000
	push	{r5, r6, lr}
	bl 0x0200d880
	movs	r0, #0
	bl 0x0200d9e8
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	bl 0x0200d978
	movs	r6, #128
	movs	r2, #132
	lsls	r6, r6, #8
	lsls	r2, r2, #1
	movs	r5, #140
	lsls	r5, r5, #1
	adds	r1, r2, #0
	adds	r3, r6, #0
	movs	r0, #8
	bl 0x0200ad78
	adds	r1, r5, #0
	adds	r3, r6, #0
	movs	r0, #13
	movs	r2, #248
	bl 0x0200ad78
	adds	r1, r5, #0
	adds	r2, r5, #0
	adds	r3, r6, #0
	movs	r0, #14
	bl 0x0200ad78
	movs	r2, #138
	lsls	r2, r2, #1
	movs	r0, #4
	movs	r1, #240
	movs	r3, #0
	bl 0x0200ad78
	movs	r0, #7
	movs	r1, #216
	movs	r2, #254
	movs	r3, #0
	bl 0x0200ad78
	movs	r0, #5
	movs	r1, #232
	movs	r2, #244
	movs	r3, #0
	bl 0x0200ad78
	movs	r2, #148
	lsls	r2, r2, #1
	movs	r3, #0
	movs	r0, #6
	movs	r1, #224
	bl 0x0200ad78
	movs	r0, #4
	movs	r1, #38
	bl 0x0200d8f8
	movs	r0, #5
	movs	r1, #19
	bl 0x0200d8f8
	movs	r0, #6
	movs	r1, #19
	bl 0x0200d8f8
	movs	r0, #7
	movs	r1, #19
	bl 0x0200d8f8
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #7
	bl 0x0200da48
	movs	r0, #4
	bl 0x0200d8a0
	movs	r1, #0
	bl 0x0200d830
	movs	r0, #5
	bl 0x0200d8a0
	movs	r1, #0
	bl 0x0200d830
	movs	r0, #6
	bl 0x0200d8a0
	movs	r1, #0
	bl 0x0200d830
	movs	r0, #7
	bl 0x0200d8a0
	movs	r1, #0
	bl 0x0200d830
	bl 0x0200d9d0
	bl 0x0200d9d8
	movs	r0, #30
	bl 0x0200d878
	ldr	r0, [pc, #72]
	bl 0x0200d938
	movs	r0, #8
	movs	r1, #3
	bl 0x0200d900
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r1, #2
	movs	r0, #4
	bl 0x0200d908
	movs	r0, #1
	bl 0x0200d728
	movs	r1, #2
	movs	r0, #6
	bl 0x0200d908
	movs	r0, #2
	bl 0x0200d728
	movs	r0, #5
	movs	r1, #2
	bl 0x0200d908
	movs	r0, #7
	movs	r1, #2
	bl 0x0200d908
	movs	r0, #15
	bl 0x0200d878
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x29d0
	.2byte 0x0000
	push	{r5, r6, lr}
	sub	sp, #8
	bl 0x0200d880
	movs	r0, #0
	bl 0x0200d9e8
	ldr	r3, [pc, #1008]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #254
	ldr	r0, [r3, #0]
	movs	r1, #0
	lsls	r2, r2, #18
	bl 0x0200d8e0
	movs	r1, #254
	movs	r2, #254
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200d8e0
	movs	r5, #9
.L_02003932:
	adds	r0, r5, #0
	bl 0x0200d8a0
	ldr	r3, [r0, #8]
	cmp	r3, #0
	beq.n	.L_02003944
	ldr	r2, [pc, #964]
	adds	r3, r3, r2
	str	r3, [r0, #8]
.L_02003944:
	adds	r5, #1
	cmp	r5, #79
	ble.n	.L_02003932
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	bl 0x0200d978
	ldr	r1, [pc, #936]
	movs	r0, #13
	bl 0x0200d8b0
	ldr	r1, [pc, #932]
	movs	r0, #14
	bl 0x0200d8b0
	bl 0x0200d9d0
	movs	r0, #20
	bl 0x0200d878
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200d970
	movs	r0, #184
	movs	r1, #1
	movs	r2, #150
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #1
	bl 0x0200d978
	bl 0x0200d980
	movs	r0, #240
	movs	r1, #1
	movs	r2, #138
	lsls	r0, r0, #15
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #1
	bl 0x0200d978
	bl 0x0200d980
	movs	r0, #240
	movs	r1, #1
	movs	r2, #204
	lsls	r0, r0, #15
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x0200d978
	bl 0x0200d980
	movs	r0, #240
	movs	r1, #1
	movs	r2, #240
	lsls	r2, r2, #15
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #15
	bl 0x0200d978
	ldr	r0, [pc, #824]
	bl 0x0200d938
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	bl 0x0200d980
	movs	r0, #216
	movs	r1, #1
	movs	r2, #240
	movs	r3, #1
	lsls	r2, r2, #15
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200d978
	bl 0x0200d980
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d940
	ldr	r1, [pc, #772]
	movs	r0, #13
	bl 0x0200d8b0
	movs	r0, #13
	bl 0x0200d8b8
	movs	r1, #1
	movs	r0, #13
	bl 0x0200d8f8
	movs	r0, #10
	bl 0x0200d878
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #13
	bl 0x0200d8a8
	movs	r0, #13
	bl 0x0200d8a0
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r2, #8
	movs	r0, #13
	bl 0x0200da00
	movs	r0, #1
	bl 0x0200d878
	movs	r0, #13
	bl 0x0200d8a0
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r6, #1
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200d878
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r0, #13
	lsls	r1, r1, #10
	bl 0x0200d8a8
	movs	r0, #13
	movs	r1, #5
	bl 0x0200d8f8
	movs	r2, #40
	movs	r1, #0
	negs	r2, r2
	movs	r0, #13
	bl 0x0200d8d0
	movs	r0, #13
	bl 0x0200d8d8
	movs	r0, #103
	bl 0x0200da68
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200d838
	movs	r0, #13
	bl 0x0200d8a0
	movs	r3, #128
	lsls	r3, r3, #11
	movs	r2, #230
	str	r3, [r0, #40]
	movs	r1, #1
	movs	r0, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200d838
	movs	r0, #13
	bl 0x0200d8a0
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #0
	ands	r5, r3
	movs	r2, #16
	strb	r5, [r0, #0]
	movs	r0, #13
	bl 0x0200da00
	movs	r0, #1
	bl 0x0200d878
	movs	r0, #13
	bl 0x0200d8a0
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r5, #8
	orrs	r6, r3
	strb	r6, [r0, #0]
	movs	r0, #60
	bl 0x0200d878
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #13
	bl 0x0200d948
	movs	r0, #20
	bl 0x0200d878
	movs	r0, #13
	movs	r1, #4
	bl 0x0200d900
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r1, #160
	movs	r2, #0
	movs	r0, #13
	lsls	r1, r1, #8
	bl 0x0200d948
	movs	r1, #3
	movs	r0, #13
	bl 0x0200d900
	movs	r0, #10
	bl 0x0200d878
	movs	r1, #128
	movs	r2, #128
	movs	r0, #13
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d8a8
	movs	r1, #16
	movs	r2, #16
	movs	r0, #13
	negs	r1, r1
	negs	r2, r2
	bl 0x0200da00
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #13
	bl 0x0200d948
	movs	r0, #124
	bl 0x0200da68
	movs	r1, #6
	movs	r0, #9
	bl 0x0200d8f8
	movs	r0, #30
	bl 0x0200d878
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r0, #13
	movs	r1, #200
	movs	r2, #168
	bl 0x0200d8c8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #13
	bl 0x0200d948
	movs	r0, #40
	bl 0x0200d878
	movs	r0, #125
	bl 0x0200da68
	movs	r1, #5
	movs	r0, #9
	bl 0x0200d8f8
	movs	r0, #20
	bl 0x0200d878
	movs	r0, #138
	bl 0x0200da68
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r6, #3
	movs	r0, #127
	movs	r1, #3
	movs	r2, #1
	movs	r3, #2
	str	r6, [sp, #4]
	bl 0x0200d820
	movs	r3, #14
	str	r3, [sp, #0]
	movs	r2, #1
	movs	r3, #1
	movs	r1, #127
	movs	r0, #0
	str	r5, [sp, #4]
	bl 0x0200d818
	movs	r0, #20
	bl 0x0200d878
	ldr	r1, [pc, #308]
	movs	r0, #13
	bl 0x0200d8b0
	movs	r0, #13
	bl 0x0200d8b8
	movs	r0, #123
	bl 0x0200da68
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	ldr	r1, [pc, #284]
	movs	r0, #14
	bl 0x0200d8b0
	movs	r0, #164
	movs	r1, #1
	movs	r2, #240
	movs	r3, #1
	lsls	r2, r2, #15
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x0200d978
	bl 0x0200d980
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r1, #224
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #8
	bl 0x0200d948
	movs	r1, #3
	movs	r0, #14
	bl 0x0200d900
	movs	r0, #10
	bl 0x0200d878
	movs	r1, #128
	movs	r2, #128
	movs	r0, #14
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d8a8
	movs	r2, #16
	movs	r0, #14
	movs	r1, #16
	negs	r2, r2
	bl 0x0200da00
	movs	r2, #0
	movs	r1, #0
	movs	r0, #14
	bl 0x0200d948
	movs	r0, #124
	bl 0x0200da68
	movs	r0, #11
	movs	r1, #6
	bl 0x0200d8f8
	movs	r1, #6
	movs	r0, #12
	bl 0x0200d8f8
	movs	r0, #30
	bl 0x0200d878
	movs	r1, #175
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #168
	bl 0x0200d8c8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #14
	bl 0x0200d948
	movs	r0, #40
	bl 0x0200d878
	movs	r0, #125
	bl 0x0200da68
	movs	r1, #5
	movs	r0, #11
	bl 0x0200d8f8
	movs	r0, #40
	bl 0x0200d878
	movs	r1, #182
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #168
	bl 0x0200d8c8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #14
	bl 0x0200d948
	movs	r0, #40
	bl 0x0200d878
	movs	r0, #125
	bl 0x0200da68
	movs	r1, #5
	movs	r0, #12
	bl 0x0200d8f8
	movs	r0, #20
	bl 0x0200d878
	movs	r0, #138
	bl 0x0200da68
	movs	r3, #53
	str	r3, [sp, #0]
	movs	r0, #127
	movs	r1, #3
	movs	r2, #1
	movs	r3, #2
	str	r6, [sp, #4]
	b.n	.L_02003d20
	.4byte 0x02000240
	.4byte 0xff700000
	.4byte 0x0200dc88
	.4byte 0x0200dbe8
	.4byte 0x0000299a
	.4byte 0x0200dd28
	.4byte 0x0200ddc8
	.2byte 0xde40
	.2byte 0x0200
.L_02003d20:
	bl 0x0200d820
	movs	r3, #19
	str	r3, [sp, #0]
	movs	r1, #127
	movs	r3, #1
	movs	r2, #1
	movs	r0, #0
	str	r5, [sp, #4]
	bl 0x0200d818
	movs	r0, #30
	bl 0x0200d878
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #14
	bl 0x0200d948
	movs	r0, #10
	bl 0x0200d878
	movs	r0, #14
	movs	r1, #3
	bl 0x0200d900
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	movs	r0, #204
	movs	r1, #1
	movs	r2, #240
	lsls	r2, r2, #15
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x0200d978
	bl 0x0200d980
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d940
	ldr	r1, [pc, #28]
	movs	r0, #14
	bl 0x0200d8b0
	movs	r0, #14
	bl 0x0200d8b8
	movs	r1, #0
	movs	r0, #8
	bl 0x0200d940
	movs	r0, #10
	bl 0x0200d990
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0xded4
	.2byte 0x0200
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
.L_02003de4:
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	cmp	r0, #0
	bne.n	.L_02003dee
	b.n	.L_02003f34
.L_02003dee:
	bl 0x0200d8a0
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	cmp	r3, #0
	bne.n	.L_02003e02
	ldr	r3, [r7, #16]
	cmp	r3, #0
	bne.n	.L_02003e02
	b.n	.L_02003f34
.L_02003e02:
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #8
	ldrsh	r3, [r5, r4]
	cmp	r2, r3
	bne.n	.L_02003e18
	movs	r0, #206
	bl 0x0200da68
	movs	r3, #4
	strh	r3, [r5, #18]
.L_02003e18:
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	movs	r1, #10
	ldrsh	r3, [r5, r1]
	cmp	r2, r3
	bne.n	.L_02003e34
	movs	r0, #140
	adds	r0, #255
	bl 0x0200da68
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #248
	strh	r3, [r5, #18]
.L_02003e34:
	movs	r4, #18
	ldrsh	r3, [r5, r4]
	ldrh	r2, [r5, #18]
	cmp	r3, #0
	beq.n	.L_02003e6e
	ldrh	r3, [r5, #4]
	movs	r0, #0
	adds	r3, r3, r2
	movs	r4, #12
	ldrsh	r2, [r5, r4]
	strh	r3, [r5, #4]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	ldrh	r1, [r5, #12]
	cmp	r3, r2
	blt.n	.L_02003e5a
	strh	r1, [r5, #4]
	strh	r0, [r5, #18]
	strh	r0, [r5, #22]
.L_02003e5a:
	movs	r1, #4
	ldrsh	r2, [r5, r1]
	movs	r4, #14
	ldrsh	r3, [r5, r4]
	ldrh	r1, [r5, #14]
	cmp	r2, r3
	bgt.n	.L_02003e6e
	strh	r1, [r5, #4]
	strh	r0, [r5, #18]
	strh	r0, [r5, #22]
.L_02003e6e:
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02003e8e
	ldrh	r3, [r5, #2]
	movs	r1, #6
	ldrsh	r2, [r5, r1]
	adds	r3, #1
	strh	r3, [r5, #2]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	blt.n	.L_02003e8e
	strh	r0, [r5, #2]
.L_02003e8e:
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
	bge.n	.L_02003eb4
	adds	r2, #7
.L_02003eb4:
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
	beq.n	.L_02003ece
	movs	r3, #0
	b.n	.L_02003ed4
.L_02003ece:
	ldrb	r2, [r1, #3]
	movs	r3, #128
	orrs	r3, r2
.L_02003ed4:
	strb	r3, [r1, #3]
	movs	r0, #4
	ldrsh	r3, [r5, r0]
	cmp	r3, #0
	beq.n	.L_02003ee4
	ldrb	r2, [r1, #3]
	movs	r3, #16
	orrs	r3, r2
.L_02003ee4:
	strb	r3, [r1, #3]
	ldr	r6, [r5, #24]
	cmp	r6, #0
	beq.n	.L_02003f34
	movs	r1, #4
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	bne.n	.L_02003efc
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #100]
	adds	r3, r3, r2
	b.n	.L_02003f2a
.L_02003efc:
	ldrh	r0, [r5, #22]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r0, r0, r3
	strh	r0, [r5, #22]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	bl 0x0200d748
.L_02003f0e:
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
	adds	r3, r3, r0
.L_02003f2a:
	str	r3, [r6, #12]
	ldr	r3, [r7, #8]
	str	r3, [r6, #8]
	ldr	r3, [r7, #16]
	str	r3, [r6, #16]
.L_02003f34:
	movs	r3, #1
	add	sl, r3
	movs	r2, #4
	mov	r4, sl
	add	r8, r2
	adds	r5, #28
	cmp	r4, #15
	bgt.n	.L_02003f46
	b.n	.L_02003de4
.L_02003f46:
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
.L_02003fc2:
	ldr	r3, [sp, #32]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	bne.n	.L_02003fce
	b.n	.L_0200414c
.L_02003fce:
	movs	r1, #4
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02003fd8
	b.n	.L_0200414c
.L_02003fd8:
	bl 0x0200d8a0
	ldr	r3, [pc, #408]
	movs	r1, #12
	mov	sl, r0
	ldr	r0, [r3, #0]
	bl 0x0200d720
	movs	r1, #3
	bl 0x0200d718
	lsls	r0, r0, #3
	adds	r0, #32
	str	r0, [sp, #20]
	ldr	r1, [sp, #32]
	movs	r2, #0
.L_02003ff8:
	movs	r0, #4
	ldrsh	r3, [r1, r0]
	mov	fp, r2
	cmp	fp, r3
	bge.n	.L_0200409c
.L_02004002:
	ldr	r2, [sp, #16]
	cmp	r2, #79
	bgt.n	.L_0200408e
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
	bhi.n	.L_0200408e
	movs	r0, #16
	negs	r0, r0
	cmp	r6, r0
	ble.n	.L_0200408e
	cmp	r6, #239
	bgt.n	.L_0200408e
	adds	r3, #177
	ands	r7, r3
.L_0200405c:
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
	orrs	r3, r2
	mov	r0, r9
	str	r3, [r4, #0]
	bl 0x0200d790
	ldr	r2, [sp, #16]
	movs	r1, #12
	adds	r2, #1
	str	r2, [sp, #16]
	add	r9, r1
.L_0200408e:
	ldr	r1, [sp, #32]
	movs	r3, #16
	add	fp, r3
	movs	r0, #4
	ldrsh	r3, [r1, r0]
	cmp	fp, r3
	blt.n	.L_02004002
.L_0200409c:
	ldr	r2, [sp, #16]
	cmp	r2, #79
	bgt.n	.L_0200414c
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
	bl 0x0200d748
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
	bhi.n	.L_0200414c
	movs	r2, #16
	negs	r2, r2
	cmp	r6, r2
	ble.n	.L_0200414c
	cmp	r6, #239
	bgt.n	.L_0200414c
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
	bl 0x0200d790
	ldr	r0, [sp, #16]
	movs	r3, #12
	adds	r0, #1
	str	r0, [sp, #16]
	add	r9, r3
.L_0200414c:
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #32]
	subs	r1, #1
	adds	r2, #28
	str	r1, [sp, #28]
	str	r2, [sp, #32]
	cmp	r1, #0
	blt.n	.L_0200415e
	b.n	.L_02003fc2
.L_0200415e:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
.L_02004166:
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
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	0x0200c1a6
	movs	r1, #228
	ldr	r3, [pc, #116]
	adds	r0, r6, #0
	lsls	r1, r1, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2080
	lsls	r0, r0, #4
	bl 0x0200d760
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #100]
	bl 0x0200d770
	bl 0x0200d788
	movs	r1, #128
	strh	r0, [r6, #0]
	lsls	r0, r0, #16
	adds	r2, r5, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200d780
	adds	r0, r5, #0
	bl 0x0200d768
	movs	r0, #240
	lsls	r0, r0, #2
	bl 0x0200d760
	movs	r2, #226
	lsls	r2, r2, #1
	movs	r1, #128
	adds	r3, r6, r2
	lsls	r1, r1, #3
	str	r0, [r3, #0]
	adds	r1, #141
	strh	r7, [r6, #2]
	ldr	r0, [pc, #48]
	bl 0x0200d730
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200d730
	bl 0x0200da40
	bl 0x0200d8a0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.4byte 0x03000258
	.4byte 0x0200dfa0
	.4byte 0x0200bda1
	.2byte 0xbf69
	.2byte 0x0200
	.2byte 0xb5e0
	.2byte 0x4657
	.2byte 0x464e
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
	bl 0x0200d8a0
	adds	r6, r0, #0
	mov	r0, r8
	bl 0x0200d8a0
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d830
	movs	r0, #10
	adds	r0, #255
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_020042bc
	cmp	r7, sl
	bge.n	.L_0200426e
	mov	ip, sl
	mov	sl, r7
	mov	r7, ip
.L_0200426e:
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
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strh	r0, [r5, #4]
	strh	r7, [r5, #12]
	strh	r0, [r5, #18]
	strh	r0, [r5, #22]
	strh	r0, [r5, #20]
	strb	r3, [r1, #0]
	b.n	.L_020042bc
	.4byte 0x00000000
	.2byte 0x254c
	.2byte 0x0200
.L_020042bc:
	movs	r0, #128
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	lsls	r0, r0, #8
	bl 0x0200d7d8
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
	sub	sp, #20
	str	r3, [sp, #16]
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	mov	fp, r0
	cmp	r3, r2
	beq.n	.L_020043d6
.L_02004316:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	adds	r0, r3, #0
	str	r3, [sp, #12]
	bl 0x0200d8a0
	mov	r2, fp
	ldrh	r2, [r2, #2]
	adds	r7, r0, #0
	str	r2, [sp, #8]
	movs	r3, #34
	adds	r3, r3, r7
	adds	r0, r2, #0
	ldrb	r2, [r3, #0]
	mov	r9, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [sp, #16]
	adds	r0, #1
	ldr	r5, [r2, r3]
	ldr	r2, [pc, #196]
	adds	r3, r5, r2
	ldr	r2, [pc, #196]
	asrs	r3, r3, #2
	adds	r6, r3, r2
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02004364
	ldr	r0, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
	b.n	.L_020043c4
.L_02004364:
	adds	r0, r7, #0
	bl 0x0200da18
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r5, r5, r3
	str	r5, [sp, #4]
	mov	r2, r9
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	bl 0x0200d848
	mov	r3, r9
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	mov	sl, r0
	ldrb	r0, [r3, #0]
	bl 0x0200d808
	adds	r5, r0, #0
	ldr	r0, [sp, #12]
	bl 0x0200d958
	ldr	r2, [sp, #4]
	movs	r3, #128
	asrs	r5, r5, #19
	strb	r3, [r2, #3]
	adds	r5, #4
	mov	r3, r9
	adds	r2, r5, #0
	ldrb	r0, [r3, #0]
	mov	r1, sl
	bl 0x0200da20
	add	r8, r6
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_020043c4
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200d7c8
.L_020043c4:
	movs	r3, #4
	add	fp, r3
	mov	r2, fp
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02004316
.L_020043d6:
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x0200d808
	ldr	r3, [r5, #12]
	cmp	r3, r0
	bge.n	.L_020043fe
	str	r0, [r5, #20]
	str	r0, [r5, #12]
.L_020043fe:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #12
	adds	r5, r0, #0
	bl 0x0200d9c0
	cmp	r0, #0
	beq.n	.L_0200442e
	b.n	.L_0200459e
.L_0200442e:
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r6, #12]
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	str	r3, [r0, #8]
	bl 0x0200da28
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_0200445a
	b.n	.L_0200459e
.L_0200445a:
	b.n	.L_02004590
.L_0200445c:
	ldrh	r7, [r5, #0]
	adds	r0, r7, #0
	bl 0x0200d8a0
	cmp	r0, r8
	beq.n	.L_0200446c
	adds	r5, #4
	b.n	.L_02004590
.L_0200446c:
	ldrh	r5, [r5, #2]
	bl 0x0200d880
	adds	r0, r5, #0
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_020044da
	movs	r0, #125
	bl 0x0200da68
	adds	r0, r7, #0
	bl 0x0200d8a0
	movs	r1, #7
	bl 0x0200d930
	movs	r0, #2
	bl 0x0200d728
	movs	r1, #0
	mov	r0, r8
	bl 0x0200d7c8
	adds	r0, r7, #0
	bl 0x0200d8a0
	movs	r1, #0
	bl 0x0200d930
	movs	r0, #2
	bl 0x0200d728
	adds	r0, r7, #0
	bl 0x0200d8a0
	movs	r1, #7
	bl 0x0200d930
	movs	r0, #4
	bl 0x0200d728
	adds	r0, r7, #0
	bl 0x0200d8a0
	movs	r1, #0
	bl 0x0200d930
	movs	r0, #0
	bl 0x0200ca94
	adds	r0, r5, #0
	bl 0x0200d7b0
	b.n	.L_0200458a
.L_020044da:
	adds	r5, #1
	mov	sl, r5
	mov	r0, sl
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_0200458a
	adds	r6, #85
	strb	r0, [r6, #0]
	movs	r0, #185
	bl 0x0200da68
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200d838
	movs	r0, #0
	bl 0x0200ca94
	movs	r5, #2
	movs	r0, #8
	mov	r7, r8
	bl 0x0200d728
	negs	r5, r5
	mov	r0, r8
	movs	r1, #2
	adds	r7, #34
	bl 0x0200d7c8
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl 0x0200cd6c
	movs	r0, #1
	bl 0x0200ca94
	movs	r0, #16
	bl 0x0200d728
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl 0x0200cd6c
	movs	r0, #4
	bl 0x0200d728
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200d838
	movs	r0, #8
	bl 0x0200d728
	movs	r3, #3
	strb	r3, [r6, #0]
	movs	r0, #5
	bl 0x0200d728
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	mov	r0, r8
	bl 0x0200d7e0
	movs	r0, #2
	bl 0x0200d728
	movs	r0, #188
	bl 0x0200da68
	bl 0x0200cbc8
	movs	r0, #20
	bl 0x0200d728
	mov	r0, sl
	bl 0x0200d7b0
.L_0200458a:
	bl 0x0200d888
	b.n	.L_0200459e
.L_02004590:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_0200459e
	b.n	.L_0200445c
.L_0200459e:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200d8a0
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r7, r0, #0
	cmp	r3, r2
	beq.n	.L_02004686
.L_020045ca:
	ldrh	r3, [r5, #0]
	cmp	r3, r6
	beq.n	.L_020045d4
	adds	r5, #4
	b.n	.L_0200467a
.L_020045d4:
	ldrh	r5, [r5, #2]
	bl 0x0200d880
	adds	r3, r5, #1
	mov	r8, r3
	mov	r0, r8
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02004674
	movs	r0, #185
	bl 0x0200da68
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200d838
	movs	r0, #0
	bl 0x0200ca94
	movs	r0, #8
	bl 0x0200d728
	adds	r0, r7, #0
.L_0200460c:
	movs	r1, #2
	bl 0x0200d7c8
	adds	r3, r7, #0
	adds	r3, #34
	movs	r0, #4
	ldrb	r1, [r3, #0]
	adds	r2, r6, #0
	negs	r0, r0
	bl 0x0200cce0
	movs	r0, #1
	bl 0x0200ca94
	movs	r0, #16
	bl 0x0200d728
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200d838
	movs	r0, #8
	bl 0x0200d728
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl 0x0200d7e0
	movs	r0, #2
	bl 0x0200d728
	movs	r0, #188
	bl 0x0200da68
	bl 0x0200cbc8
	movs	r0, #20
	bl 0x0200d728
	adds	r0, r5, #0
	bl 0x0200d7b0
	mov	r0, r8
	bl 0x0200d7b0
.L_02004674:
	bl 0x0200d888
	b.n	.L_02004686
.L_0200467a:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_020045ca
.L_02004686:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200d8a0
	movs	r3, #3
	adds	r0, #92
	strb	r3, [r0, #0]
	adds	r0, r5, #0
	bl 0x0200d958
	pop	{r5, pc}
	.2byte 0x0000
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
	sub	sp, #16
	ldr	r5, [pc, #324]
	str	r3, [sp, #12]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	mov	fp, r0
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl 0x0200d8f0
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x0200d8f0
	ldr	r1, [r5, #0]
	movs	r0, #10
	bl 0x0200d8f0
	movs	r0, #1
	bl 0x0200d728
	movs	r0, #8
	bl 0x0200c68c
	movs	r0, #9
	bl 0x0200c68c
	movs	r0, #10
	bl 0x0200c68c
	movs	r1, #0
	movs	r0, #9
	bl 0x0200d8f8
	movs	r0, #1
	bl 0x0200d728
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200d8e0
	movs	r0, #1
	bl 0x0200d728
	b.n	.L_020047de
.L_0200472a:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	mov	r9, r3
	mov	r0, r9
	bl 0x0200d8a0
	mov	r2, fp
	ldrh	r2, [r2, #2]
	adds	r5, r0, #0
	str	r2, [sp, #8]
	adds	r7, r5, #0
	adds	r7, #34
	adds	r0, r2, #0
	ldrb	r2, [r7, #0]
	adds	r0, #1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	ldr	r6, [r2, r3]
	ldr	r2, [pc, #168]
	adds	r3, r6, r2
	ldr	r2, [pc, #168]
	asrs	r3, r3, #2
	adds	r2, r2, r3
	mov	sl, r2
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #3
	strb	r3, [r2, #0]
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_02004780
	mov	r0, r9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e0
	b.n	.L_020047da
.L_02004780:
	adds	r0, r5, #0
	bl 0x0200da18
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r6, r6, r3
	str	r6, [sp, #4]
	add	r8, sl
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldrb	r0, [r7, #0]
	bl 0x0200d848
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	mov	sl, r0
	ldrb	r0, [r7, #0]
	bl 0x0200d808
	adds	r5, r0, #0
	mov	r0, r9
	bl 0x0200d958
	ldr	r6, [sp, #4]
	asrs	r5, r5, #19
	movs	r3, #128
	adds	r5, #4
	adds	r2, r5, #0
	strb	r3, [r6, #3]
	ldrb	r0, [r7, #0]
	mov	r1, sl
	bl 0x0200da20
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl 0x0200d7a8
	cmp	r0, #0
	beq.n	.L_020047da
	mov	r0, r9
	movs	r1, #9
	bl 0x0200d968
.L_020047da:
	movs	r3, #4
	add	fp, r3
.L_020047de:
	mov	r2, fp
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_0200472a
	movs	r0, #10
	bl 0x0200d728
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfdff0000
	.2byte 0x4000
	.2byte 0x0202
	push	{r5, lr}
	adds	r5, r1, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	bl 0x0200d7e0
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x0200d7e0
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #12
	adds	r6, r0, #0
	bl 0x0200d9c0
	cmp	r0, #0
	beq.n	.L_02004840
	b.n	.L_02004a02
.L_02004840:
	ldr	r3, [pc, #460]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200d8a0
	ldr	r3, [r5, #8]
	adds	r7, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r5, #12]
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	str	r3, [r0, #8]
	bl 0x0200da28
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_02004874
	b.n	.L_02004a02
.L_02004874:
	b.n	.L_020049f4
.L_02004876:
	ldrh	r3, [r6, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200d8a0
	cmp	r0, sl
	beq.n	.L_02004888
	adds	r6, #4
	b.n	.L_020049f4
.L_02004888:
	ldrh	r6, [r6, #2]
	bl 0x0200d880
	adds	r0, r6, #0
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02004922
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200d7c8
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x0200c80c
	movs	r0, #1
	bl 0x0200d728
	movs	r0, #125
	bl 0x0200da68
	movs	r0, #8
	bl 0x0200d8a0
	movs	r1, #7
	bl 0x0200d930
	movs	r0, #2
	bl 0x0200d728
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200d7c8
	movs	r1, #9
	mov	r0, r8
	bl 0x0200d968
	movs	r0, #8
	bl 0x0200d8a0
	movs	r1, #0
	bl 0x0200d930
	movs	r0, #2
	bl 0x0200d728
	movs	r0, #8
	bl 0x0200d8a0
	movs	r1, #7
	bl 0x0200d930
	movs	r0, #4
	bl 0x0200d728
	movs	r0, #8
	bl 0x0200d8a0
	movs	r1, #0
	bl 0x0200d930
	movs	r0, #0
	bl 0x0200ca94
	mov	r0, sl
	adds	r1, r7, #0
	bl 0x0200c80c
	movs	r0, #1
	bl 0x0200d728
	adds	r0, r6, #0
	bl 0x0200d7b0
	b.n	.L_020049ee
.L_02004922:
	adds	r6, #1
	mov	r9, r6
	mov	r0, r9
	bl 0x0200d7a8
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020049ee
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200d7c8
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x0200c80c
	adds	r5, #85
	movs	r0, #1
	bl 0x0200d728
	strb	r6, [r5, #0]
	movs	r0, #185
	bl 0x0200da68
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200d838
	movs	r0, #0
	bl 0x0200ca94
	mov	r8, r5
	movs	r0, #8
	movs	r6, #2
	mov	r5, sl
	bl 0x0200d728
	negs	r6, r6
	adds	r0, r7, #0
	movs	r1, #2
	adds	r5, #34
	bl 0x0200d7c8
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x0200cd6c
	movs	r0, #1
	bl 0x0200ca94
	movs	r0, #16
	bl 0x0200d728
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x0200cd6c
	movs	r0, #4
	bl 0x0200d728
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200d838
	movs	r0, #8
	bl 0x0200d728
	movs	r3, #3
	mov	r2, r8
	strb	r3, [r2, #0]
	movs	r0, #5
	bl 0x0200d728
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl 0x0200d7e0
	movs	r0, #2
	bl 0x0200d728
	movs	r0, #188
	bl 0x0200da68
	bl 0x0200cbc8
	movs	r0, #20
	bl 0x0200d728
	mov	r0, r9
	bl 0x0200d7b0
.L_020049ee:
	bl 0x0200d888
	b.n	.L_02004a02
.L_020049f4:
	movs	r2, #255
	ldrh	r3, [r6, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_02004a02
	b.n	.L_02004876
.L_02004a02:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
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
	bl 0x0200d708
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02004a44
	adds	r3, #15
.L_02004a44:
	asrs	r3, r3, #4
	subs	r3, r7, r3
.L_02004a48:
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
.L_02004a52:
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
	bl 0x0200d8a0
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
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #256]
	mov	r8, r0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	movs	r2, #0
	adds	r7, r0, #0
	mov	r9, r2
	mov	sl, r2
.L_02004ab6:
	bl 0x0200d738
	lsls	r3, r0, #3
	subs	r3, r3, r0
	ldr	r2, [r7, #12]
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	subs	r2, r2, r3
	mov	r3, sl
	lsls	r1, r3, #17
	ldr	r3, [r7, #8]
	ldr	r0, [pc, #212]
	adds	r1, r1, r3
	ldr	r3, [pc, #212]
	adds	r1, r1, r0
	movs	r0, #30
	adds	r2, r2, r3
	adds	r0, #255
	ldr	r3, [r7, #16]
	bl 0x0200d7d8
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02004b8a
	mov	r1, r9
	ldr	r0, [r6, #80]
	bl 0x0200d9f0
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	movs	r1, #0
	mov	r9, r0
	adds	r0, r6, #0
	bl 0x0200d830
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200d7c8
	adds	r0, r6, #0
	ldr	r1, [pc, #152]
	bl 0x0200d7d0
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	ldr	r1, [r6, #80]
	movs	r0, #13
	ldrb	r3, [r1, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	mov	r2, r8
	strb	r3, [r1, #9]
	cmp	r2, #0
	beq.n	.L_02004b54
	mov	r3, sl
	lsls	r5, r3, #13
	adds	r0, r5, #0
	bl 0x0200d750
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #108]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6470
	adds	r0, r5, #0
	bl 0x0200d748
	b.n	.L_02004b58
.L_02004b54:
	mov	r0, r8
	str	r0, [r6, #68]
.L_02004b58:
	str	r0, [r6, #76]
	bl 0x0200d738
	movs	r2, #192
	lsls	r0, r0, #14
	lsls	r2, r2, #7
	lsrs	r0, r0, #16
	adds	r0, r0, r2
	negs	r0, r0
	str	r0, [r6, #72]
	bl 0x0200d738
	ldr	r3, [pc, #68]
	lsls	r0, r0, #9
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r6, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	ldr	r3, [pc, #60]
	str	r3, [r6, #48]
	ldr	r3, [pc, #60]
	str	r3, [r6, #52]
	ldr	r3, [pc, #60]
	str	r3, [r6, #108]
.L_02004b8a:
	movs	r0, #1
	add	sl, r0
	mov	r2, sl
	cmp	r2, #7
	bls.n	.L_02004ab6
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff80000
	.4byte 0xfffe0000
	.4byte 0x0200e254
	.4byte 0x0300021c
	.4byte 0x00013333
	.4byte 0xffffff00
	.4byte 0xfffff800
	.4byte 0xfffffa00
	.2byte 0xca15
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #244]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02004cbc
	movs	r3, #0
	mov	r9, r3
	mov	sl, r3
.L_02004bec:
	movs	r0, #30
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, #255
	bl 0x0200d7d8
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02004cb2
	mov	r1, r9
	ldr	r0, [r7, #80]
	bl 0x0200d9f0
	movs	r4, #0
	mov	r8, r4
	adds	r3, r7, #0
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r3, #4
	strb	r2, [r3, #0]
	movs	r1, #0
	mov	r9, r0
	adds	r0, r7, #0
	bl 0x0200d830
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200d7c8
	ldr	r1, [pc, #160]
	adds	r0, r7, #0
	bl 0x0200d7d0
	mov	r3, sl
	lsls	r5, r3, #12
	adds	r0, r5, #0
	bl 0x0200d750
	mov	r4, r8
	str	r4, [r7, #72]
	str	r0, [r7, #68]
	adds	r0, r5, #0
	bl 0x0200d748
	ldr	r3, [r7, #68]
	str	r0, [r7, #76]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r7, #68]
	bl 0x0200d738
	lsls	r3, r0, #1
	ldr	r2, [r7, #68]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #108]
	adds	r2, r2, r3
	str	r2, [r7, #68]
	bl 0x0200d738
	lsls	r3, r0, #1
	ldr	r2, [r7, #76]
	adds	r3, r3, r0
	ldr	r4, [pc, #96]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	adds	r2, r2, r4
	str	r2, [r7, #76]
	bl 0x0200d738
	ldr	r2, [pc, #84]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	adds	r3, r7, #0
	adds	r0, r0, r2
	adds	r3, #100
	strh	r0, [r3, #0]
	mov	r3, r8
	str	r3, [r7, #48]
	str	r3, [r7, #52]
	ldr	r3, [pc, #68]
	ldr	r0, [r7, #80]
	str	r3, [r7, #108]
	ldr	r3, [r6, #80]
	movs	r1, #12
	ldrb	r3, [r3, #9]
	movs	r4, #13
	ands	r1, r3
	ldrb	r3, [r0, #9]
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #9]
.L_02004cb2:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_02004bec
.L_02004cbc:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200e284
	.4byte 0xffffa000
	.4byte 0xffffd000
	.4byte 0xfffff800
	.2byte 0xca15
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	adds	r0, r2, #0
	adds	r5, r1, #0
	bl 0x0200d8a0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r5, #3
	subs	r3, r3, r5
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r3, [r2, r3]
	ldr	r2, [pc, #88]
	adds	r7, r0, #0
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	adds	r5, r7, #0
	asrs	r3, r3, #2
	adds	r5, #34
	adds	r6, r3, r1
	ldr	r2, [r7, #16]
	ldr	r1, [r7, #8]
	ldrb	r0, [r5, #0]
	bl 0x0200d848
	ldr	r2, [r7, #16]
	mov	r8, r0
	ldr	r1, [r7, #8]
	ldrb	r0, [r5, #0]
	bl 0x0200d808
	ldr	r3, [r7, #8]
	asrs	r2, r0, #19
	add	r2, sl
	cmp	r3, #0
	bge.n	.L_02004d3a
	ldr	r1, [pc, #48]
	adds	r3, r3, r1
.L_02004d3a:
	ldr	r0, [r7, #16]
	asrs	r1, r3, #20
	cmp	r0, #0
	bge.n	.L_02004d46
	ldr	r3, [pc, #36]
	adds	r0, r0, r3
.L_02004d46:
	asrs	r3, r0, #20
	lsls	r3, r3, #7
	adds	r3, r1, r3
	ldrb	r0, [r5, #0]
	mov	r1, r8
	adds	r6, r6, r3
	bl 0x0200da20
	strb	r0, [r6, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0xffff
	.2byte 0x000f
	push	{lr}
	ldr	r3, [pc, #16]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r3, #0]
	bl 0x0200cce0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r4, [pc, #32]
	adds	r0, #8
	movs	r3, #0
	strb	r3, [r0, #0]
	movs	r2, #7
	subs	r0, #1
.L_02004d92:
	movs	r3, #15
	ands	r3, r1
	ldrb	r3, [r4, r3]
	subs	r2, #1
	strb	r3, [r0, #0]
	lsrs	r1, r1, #4
	subs	r0, #1
	cmp	r2, #0
	bge.n	.L_02004d92
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200e3ac
	.section .text.x0200cddc,"ax",%progbits
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [pc, #768]
	adds	r3, #236
	ldr	r3, [r3, #0]
	mov	fp, r0
	movs	r1, #4
	ldrsh	r0, [r0, r1]
	mov	r8, r3
	sub	sp, #8
	bl 0x0200d8a0
	ldr	r5, [pc, #752]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200d8a0
	cmp	r7, #0
	bne.n	.L_02004e18
	b.n	.L_02005190
.L_02004e18:
	mov	r1, fp
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	cmp	r2, #4
	bne.n	.L_02004e9e
	ldr	r4, [r0, #16]
.L_02004e24:
	movs	r2, #255
	ldr	r6, [r0, #8]
	lsls	r2, r2, #24
	movs	r0, #152
	adds	r3, r4, r2
.L_02004e2e:
	lsls	r0, r0, #17
	cmp	r3, r0
	bls.n	.L_02004e3e
	ldr	r3, [pc, #704]
	movs	r2, #132
	ands	r3, r6
	lsls	r2, r2, #18
	subs	r6, r2, r3
.L_02004e3e:
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrh	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02004e56
	movs	r2, #152
	lsls	r2, r2, #17
	adds	r6, r6, r2
	b.n	.L_02004e5a
.L_02004e56:
	ldr	r3, [pc, #676]
	adds	r6, r6, r3
.L_02004e5a:
	ldr	r1, [r7, #8]
	cmp	r1, r6
	bne.n	.L_02004e68
	ldr	r3, [r7, #16]
	cmp	r3, r4
	bne.n	.L_02004e68
	b.n	.L_020050d2
.L_02004e68:
	ldr	r3, [r7, #16]
	subs	r1, r6, r1
	subs	r0, r4, r3
	str	r4, [sp, #0]
	bl 0x0200d740
	ldrh	r3, [r7, #6]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	subs	r0, r0, r3
	lsls	r0, r0, #16
	movs	r2, #128
	asrs	r0, r0, #16
	lsls	r2, r2, #5
	ldr	r4, [sp, #0]
	cmp	r0, r2
	ble.n	.L_02004e8c
	adds	r0, r2, #0
.L_02004e8c:
	ldr	r2, [pc, #624]
	cmp	r0, r2
	bge.n	.L_02004e94
	adds	r0, r2, #0
.L_02004e94:
	adds	r3, r3, r0
	strh	r3, [r7, #6]
	str	r6, [r7, #8]
	str	r4, [r7, #16]
	b.n	.L_020050d2
.L_02004e9e:
	cmp	r2, #3
	beq.n	.L_02004ea4
	b.n	.L_02004fdc
.L_02004ea4:
	movs	r0, #190
	lsls	r0, r0, #2
	bl 0x0200d7c0
	movs	r1, #1
	eors	r0, r1
	lsls	r3, r0, #1
	ldr	r2, [pc, #592]
	adds	r3, r3, r0
	lsls	r3, r3, #3
	adds	r5, r3, r2
	ldr	r3, [pc, #588]
	ldrh	r3, [r3, #0]
	asrs	r3, r0
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_02004ec8
	b.n	.L_02005190
.L_02004ec8:
	movs	r2, #0
	ldrsh	r6, [r5, r2]
	adds	r5, #2
	movs	r3, #0
	ldrsh	r4, [r5, r3]
	adds	r5, #2
	movs	r1, #0
	ldrsh	r0, [r5, r1]
	adds	r5, #2
	ldrh	r3, [r5, #0]
	mov	r9, r0
	lsls	r3, r3, #16
	asrs	r2, r3, #24
	str	r2, [sp, #4]
	movs	r2, #255
	lsls	r2, r2, #16
	movs	r0, #137
	ands	r2, r3
	lsls	r0, r0, #1
	asrs	r2, r2, #16
	adds	r0, #255
	str	r4, [sp, #0]
	mov	sl, r2
	bl 0x0200d7a8
	adds	r5, #2
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_02004f7e
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq.n	.L_02004f7e
	movs	r3, #9
	mov	r0, fp
	strh	r3, [r0, #0]
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200d7c8
	mov	r3, r8
	adds	r3, #236
	ldr	r2, [r3, #0]
	ldr	r3, [r7, #8]
	movs	r1, #192
	lsls	r1, r1, #12
	adds	r6, r2, r1
	cmp	r2, r3
	blt.n	.L_02004f2c
	ldr	r3, [pc, #480]
	adds	r6, r2, r3
.L_02004f2c:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200d7a8
	mov	r3, r8
	adds	r2, r7, #0
	adds	r3, #240
	adds	r2, #100
	cmp	r0, #0
	bne.n	.L_02004f50
	ldr	r3, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r4, r3, r0
	mov	r3, r8
	adds	r3, #232
	b.n	.L_02004f5a
.L_02004f50:
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #444]
	adds	r4, r3, r1
	mov	r3, r8
	adds	r3, #230
.L_02004f5a:
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	adds	r3, r4, #0
	bl 0x0200d7f8
	adds	r0, r7, #0
	bl 0x0200d7f0
	b.n	.L_02004f92
.L_02004f7e:
	lsls	r6, r6, #16
	lsls	r4, r4, #16
	mov	r2, r9
	strh	r2, [r7, #6]
	str	r6, [r7, #8]
	str	r4, [r7, #16]
	adds	r0, r7, #0
	mov	r1, sl
	bl 0x0200d7c8
.L_02004f92:
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	adds	r5, #2
	movs	r1, #0
	ldrsh	r6, [r5, r1]
	adds	r5, #2
	movs	r2, #0
	ldrsh	r4, [r5, r2]
	adds	r5, #2
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	mov	r9, r3
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	mov	sl, r2
	cmp	r0, #0
	bne.n	.L_02004fb6
	b.n	.L_02005190
.L_02004fb6:
	str	r4, [sp, #0]
	bl 0x0200d8a0
	adds	r7, r0, #0
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bne.n	.L_02004fc6
	b.n	.L_02005190
.L_02004fc6:
	lsls	r3, r6, #16
	mov	r0, r9
	str	r3, [r7, #8]
	lsls	r3, r4, #16
	strh	r0, [r7, #6]
	str	r3, [r7, #16]
	adds	r0, r7, #0
	mov	r1, sl
	bl 0x0200d7c8
	b.n	.L_02005190
.L_02004fdc:
	cmp	r2, #1
	beq.n	.L_02004fe2
	b.n	.L_02005114
.L_02004fe2:
	mov	r5, r8
	adds	r5, #234
	mov	r2, fp
	movs	r1, #10
	ldrsh	r0, [r2, r1]
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r0, r3
	beq.n	.L_02005006
	bl 0x0200d798
	mov	r1, r8
	adds	r1, #244
	bl 0x0200d770
	mov	r2, fp
	ldrh	r3, [r2, #10]
	strh	r3, [r5, #0]
.L_02005006:
	mov	r1, fp
	movs	r0, #6
	ldrsh	r3, [r1, r0]
	mov	r0, r8
	lsls	r3, r3, #1
	adds	r2, r3, #0
	adds	r2, #244
	adds	r3, #246
	ldrsh	r6, [r0, r2]
	ldrsh	r4, [r0, r3]
	movs	r0, #130
	lsls	r0, r0, #1
	str	r4, [sp, #0]
	bl 0x0200d7a8
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_02005034
	mov	r0, fp
	ldrh	r3, [r0, #6]
	mov	r1, fp
	adds	r3, #2
	strh	r3, [r1, #6]
.L_02005034:
	cmp	r6, #0
	bne.n	.L_02005040
	cmp	r4, #0
	bne.n	.L_02005040
	movs	r3, #9
	b.n	.L_02005186
.L_02005040:
	mov	r1, fp
	movs	r0, #2
	ldrsh	r3, [r1, r0]
	lsls	r6, r6, #16
	lsls	r4, r4, #16
	cmp	r3, #0
	beq.n	.L_02005062
	movs	r2, #255
	lsls	r2, r2, #24
	movs	r0, #152
	adds	r3, r4, r2
	lsls	r0, r0, #17
	cmp	r3, r0
	bls.n	.L_02005062
	movs	r3, #132
	lsls	r3, r3, #18
	subs	r6, r3, r6
.L_02005062:
	ldr	r3, [pc, #144]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200507c
	movs	r2, #152
	lsls	r2, r2, #17
	adds	r6, r6, r2
	b.n	.L_02005080
.L_0200507c:
	ldr	r3, [pc, #124]
	adds	r6, r6, r3
.L_02005080:
	ldr	r1, [r7, #8]
	cmp	r1, r6
	bne.n	.L_0200508c
	ldr	r3, [r7, #16]
	cmp	r3, r4
	beq.n	.L_020050c8
.L_0200508c:
	ldr	r3, [r7, #16]
	subs	r1, r6, r1
	subs	r0, r4, r3
	str	r4, [sp, #0]
	bl 0x0200d740
	ldrh	r3, [r7, #6]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	subs	r0, r0, r3
	lsls	r0, r0, #16
	movs	r2, #128
	asrs	r0, r0, #16
	lsls	r2, r2, #5
	ldr	r4, [sp, #0]
	cmp	r0, r2
	ble.n	.L_020050b0
	adds	r0, r2, #0
.L_020050b0:
	ldr	r2, [pc, #76]
	cmp	r0, r2
	bge.n	.L_020050b8
	adds	r0, r2, #0
.L_020050b8:
	adds	r3, r3, r0
	movs	r2, #0
	mov	r0, fp
	strh	r3, [r7, #6]
	str	r6, [r7, #8]
	str	r4, [r7, #16]
	strh	r2, [r0, #8]
	b.n	.L_020050d2
.L_020050c8:
	mov	r1, fp
	ldrh	r3, [r1, #8]
	mov	r2, fp
	adds	r3, #1
	strh	r3, [r2, #8]
.L_020050d2:
	mov	r1, fp
	movs	r0, #8
	ldrsh	r3, [r1, r0]
.L_020050d8:
	cmp	r3, #2
	ble.n	.L_020050e6
	adds	r0, r7, #0
.L_020050de:
	movs	r1, #1
	bl 0x0200d7c8
	b.n	.L_02005190
.L_020050e6:
	adds	r0, r7, #0
	movs	r1, #5
	bl 0x0200d7c8
	b.n	.L_02005190
	.4byte 0x0200234c
	.4byte 0x02000240
	.4byte 0xffff0000
	.4byte 0xfed00000
	.4byte 0xfffff000
	.4byte 0x02003874
	.4byte 0x0300124c
	.4byte 0xfff40000
	.2byte 0x0000
	.2byte 0xfff0
.L_02005114:
	.2byte 0x2a02
	bne.n	.L_02005190
	mov	r1, fp
	movs	r3, #18
	ldrsh	r4, [r7, r3]
	movs	r0, #6
	ldrsh	r3, [r1, r0]
	movs	r2, #10
	ldrsh	r6, [r7, r2]
	lsls	r3, r3, #1
	adds	r2, r3, #0
	mov	r0, r8
	adds	r2, #244
	strh	r6, [r0, r2]
	adds	r3, #246
	mov	r1, r8
	movs	r0, #130
	strh	r4, [r1, r3]
	lsls	r0, r0, #1
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_0200514c
	mov	r2, fp
	ldrh	r3, [r2, #6]
	mov	r0, fp
	adds	r3, #2
	strh	r3, [r0, #6]
.L_0200514c:
	mov	r1, fp
	ldrh	r3, [r1, #6]
	movs	r0, #6
	ldrsh	r2, [r1, r0]
	movs	r1, #224
	lsls	r1, r1, #5
	adds	r1, #30
	cmp	r2, r1
	bne.n	.L_02005190
	ldr	r1, [pc, #44]
	adds	r3, #1
	lsls	r2, r2, #1
	lsls	r3, r3, #16
	adds	r2, #244
	mov	r0, r8
	asrs	r3, r3, #15
	strh	r1, [r0, r2]
	adds	r3, #244
	mov	r2, r8
	strh	r1, [r2, r3]
	mov	r3, r8
	adds	r3, #228
	ldrh	r3, [r3, #0]
	movs	r2, #0
	mov	r0, fp
	mov	r1, fp
	strh	r3, [r0, #4]
	strh	r2, [r1, #6]
	movs	r3, #1
.L_02005186:
	mov	r2, fp
	strh	r3, [r2, #0]
	b.n	.L_02005190
	.2byte 0x0000
	.2byte 0x0000
.L_02005190:
	add	sp, #8
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
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r3, r1, #0
	adds	r3, #236
	ldr	r3, [r3, #0]
	sub	sp, #40
	str	r3, [sp, #36]
	str	r3, [sp, #32]
	adds	r7, r3, #0
	adds	r7, #216
	adds	r0, r3, #0
	movs	r4, #0
	ldrsh	r3, [r7, r4]
	ldr	r2, [pc, #828]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	mov	fp, r0
	lsrs	r3, r3, #5
	str	r3, [sp, #20]
	adds	r3, r0, #0
	adds	r3, #224
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	mov	r5, fp
	ldr	r6, [r1, #108]
	adds	r5, #218
	cmp	r3, #0
	beq.n	.L_020051ee
	movs	r3, #2
	strh	r3, [r5, #0]
	b.n	.L_0200526a
.L_020051ee:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_02005208
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02005218
.L_02005208:
	movs	r4, #0
.L_0200520a:
	ldrsh	r3, [r5, r4]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	ble.n	.L_0200526a
	subs	r3, r2, #1
	strh	r3, [r5, #0]
	b.n	.L_0200526a
.L_02005218:
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	ldrh	r2, [r5, #0]
	cmp	r3, #1
	bgt.n	.L_0200526a
	adds	r3, r2, #1
	movs	r1, #128
	strh	r3, [r5, #0]
	lsls	r1, r1, #9
	lsls	r3, r3, #16
.L_0200522c:
	cmp	r3, r1
	bne.n	.L_0200526a
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #720]
	ldr	r1, [pc, #720]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200d760
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #704]
	bl 0x0200d770
	movs	r1, #144
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	lsls	r1, r1, #2
	adds	r2, r5, #0
	bl 0x0200d780
	adds	r0, r5, #0
	bl 0x0200d768
.L_0200526a:
	ldr	r3, [sp, #36]
	adds	r3, #218
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	bne.n	.L_02005284
	ldr	r3, [sp, #36]
	adds	r3, #216
	movs	r7, #0
	ldrsh	r0, [r3, r7]
	bl 0x0200d778
	b.n	.L_020055b2
.L_02005284:
	movs	r0, #0
	str	r0, [sp, #28]
.L_02005288:
	mov	r1, fp
	adds	r1, #222
	str	r1, [sp, #12]
	mov	r4, fp
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	adds	r4, #220
	adds	r7, r3, #0
	adds	r7, #8
	movs	r3, #255
.L_0200529c:
	ands	r7, r3
	ldr	r3, [sp, #28]
	subs	r1, #4
	cmp	r3, #0
	bne.n	.L_020052cc
	movs	r0, #0
	ldrsh	r2, [r1, r0]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r4, r1]
	lsls	r3, r3, #1
	adds	r3, r3, r2
	subs	r3, #12
	mov	r9, r3
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	mov	r2, r9
	ands	r2, r3
	movs	r3, #0
	mov	r9, r2
	str	r3, [sp, #16]
	b.n	.L_020052f4
.L_020052cc:
	movs	r0, #0
	ldrsh	r2, [r1, r0]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r4, r1]
	lsls	r3, r3, #1
	negs	r3, r3
	subs	r3, r3, r2
	adds	r3, #236
	mov	r9, r3
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	mov	r2, r9
	ands	r2, r3
	movs	r3, #128
	lsls	r3, r3, #21
	str	r3, [sp, #16]
	mov	r9, r2
.L_020052f4:
	movs	r4, #0
	str	r4, [sp, #24]
	cmp	r7, #159
	bgt.n	.L_0200537a
	ldr	r4, [sp, #36]
.L_020052fe:
	movs	r0, #0
	str	r0, [r4, #0]
	ldr	r2, [sp, #16]
	mov	r1, r9
	lsls	r6, r1, #16
	adds	r3, r7, #0
	orrs	r3, r6
	orrs	r3, r2
	ldr	r2, [pc, #520]
	movs	r5, #228
	orrs	r3, r2
	str	r3, [r4, #4]
	ldr	r3, [sp, #20]
	lsls	r5, r5, #8
	orrs	r3, r5
	str	r3, [r4, #8]
	ldr	r3, [sp, #32]
	mov	r8, r0
	adds	r3, #12
	ldr	r0, [sp, #32]
	movs	r1, #255
	mov	sl, r3
	str	r4, [sp, #4]
	bl 0x0200d790
	ldr	r4, [sp, #4]
	mov	r0, r8
	str	r0, [r4, #12]
	ldr	r1, [sp, #16]
	adds	r3, r7, #0
	adds	r3, #32
	orrs	r3, r6
	movs	r2, #128
	orrs	r3, r1
	lsls	r2, r2, #7
	orrs	r3, r2
	str	r3, [r4, #16]
	ldr	r3, [sp, #20]
	mov	r0, sl
	adds	r3, #8
	orrs	r3, r5
	str	r3, [r4, #20]
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #32]
	adds	r4, #24
	adds	r2, #24
	adds	r3, #24
	movs	r1, #255
	str	r4, [sp, #4]
	str	r2, [sp, #36]
	str	r3, [sp, #32]
	bl 0x0200d790
	ldr	r0, [sp, #24]
	adds	r7, #36
	adds	r0, #1
	str	r0, [sp, #24]
	ldr	r4, [sp, #4]
	cmp	r0, #3
	bhi.n	.L_0200537a
	cmp	r7, #159
	ble.n	.L_020052fe
.L_0200537a:
	ldr	r1, [sp, #28]
	adds	r1, #1
	str	r1, [sp, #28]
	cmp	r1, #1
	bls.n	.L_02005288
	ldr	r3, [pc, #404]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #4
	bhi.n	.L_02005392
	b.n	.L_020055b2
.L_02005392:
	mov	r3, fp
	adds	r3, #226
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200d9e0
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02005492
	ldr	r3, [pc, #376]
	ldr	r7, [sp, #36]
	movs	r4, #241
	ldr	r0, [sp, #32]
	lsls	r4, r4, #1
	adds	r3, r3, r4
	adds	r7, #4
	ldrh	r3, [r3, #0]
	adds	r0, #12
	mov	sl, r7
	ldr	r7, [sp, #20]
	str	r0, [sp, #8]
	movs	r2, #1
	ands	r2, r3
	mov	r1, fp
	mov	r4, fp
	mov	r3, fp
	mov	r6, fp
	adds	r7, #10
	adds	r1, #218
	adds	r4, #220
	adds	r3, #236
	ldr	r0, [r5, #8]
	adds	r6, #240
	mov	r8, r7
	cmp	r2, #0
	beq.n	.L_02005400
	ldr	r3, [r3, #0]
	subs	r2, r0, r3
	cmp	r2, #0
	bge.n	.L_020053e6
	ldr	r0, [pc, #320]
	adds	r2, r2, r0
.L_020053e6:
	movs	r3, #0
	ldrsh	r1, [r1, r3]
	asrs	r2, r2, #20
	lsls	r3, r1, #1
	adds	r3, r3, r1
	lsls	r3, r3, #1
	adds	r2, r2, r3
	movs	r7, #0
	ldrsh	r3, [r4, r7]
	movs	r0, #0
	adds	r2, r2, r3
	subs	r2, #19
	b.n	.L_02005424
.L_02005400:
	ldr	r3, [r3, #0]
	subs	r2, r0, r3
	cmp	r2, #0
	bge.n	.L_0200540c
	ldr	r0, [pc, #280]
	adds	r2, r2, r0
.L_0200540c:
	movs	r3, #0
	ldrsh	r1, [r1, r3]
	asrs	r2, r2, #20
	lsls	r3, r1, #1
	adds	r3, r3, r1
	lsls	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #0
	ldrsh	r3, [r4, r7]
	movs	r0, #0
	subs	r2, r2, r3
	adds	r2, #232
.L_02005424:
	mov	r9, r2
	bl 0x0200ace8
	ldr	r2, [r5, #16]
	ldr	r3, [r6, #0]
	adds	r1, r0, #0
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_0200543a
	ldr	r2, [pc, #236]
	adds	r0, r0, r2
.L_0200543a:
	ldr	r7, [sp, #12]
	asrs	r2, r0, #20
	movs	r4, #0
	ldrsh	r3, [r7, r4]
	adds	r2, r2, r3
	lsls	r3, r1, #3
	adds	r3, r3, r1
	lsls	r3, r3, #2
	subs	r2, r2, r3
	adds	r7, r2, #4
	movs	r3, #128
	lsls	r3, r3, #1
	ldr	r1, [sp, #36]
	adds	r3, #255
	mov	r0, r9
	ands	r0, r3
	movs	r3, #255
	ands	r7, r3
	movs	r3, #0
	str	r3, [r1, #0]
	lsls	r3, r0, #16
	orrs	r7, r3
	movs	r3, #128
	mov	r2, sl
	lsls	r3, r3, #23
	adds	r0, r2, #0
	orrs	r7, r3
	stmia	r0!, {r7}
	movs	r3, #228
	lsls	r3, r3, #8
	mov	r1, r8
	adds	r4, r0, #0
	orrs	r1, r3
	str	r4, [sp, #36]
	stmia	r0!, {r1}
	ldr	r3, [sp, #32]
	ldr	r4, [sp, #8]
	adds	r2, r0, #0
	movs	r1, #255
	adds	r0, r3, #0
	str	r2, [sp, #36]
	str	r4, [sp, #32]
	bl 0x0200d790
.L_02005492:
	mov	r3, fp
	adds	r3, #228
	movs	r7, #0
	ldrsh	r0, [r3, r7]
	bl 0x0200d9e0
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_020054a6
	b.n	.L_020055b2
.L_020054a6:
	ldr	r0, [r5, #8]
	cmp	r0, #0
	bne.n	.L_020054ae
	b.n	.L_020055b2
.L_020054ae:
	ldr	r3, [pc, #112]
	movs	r1, #241
	ldr	r7, [sp, #36]
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r3, [r3, #0]
	adds	r7, #4
	mov	sl, r7
	ldr	r7, [sp, #20]
	movs	r2, #1
	ands	r2, r3
	mov	r3, fp
	adds	r3, #222
	mov	r1, fp
	mov	r4, fp
	mov	r6, fp
	adds	r7, #14
	str	r3, [sp, #0]
	adds	r1, #218
	adds	r4, #220
	adds	r3, #14
.L_020054d8:
	adds	r6, #240
	mov	r8, r7
	cmp	r2, #0
	beq.n	.L_02005528
	ldr	r3, [r3, #0]
	subs	r2, r0, r3
	cmp	r2, #0
	bge.n	.L_020054ec
	ldr	r0, [pc, #56]
	adds	r2, r2, r0
.L_020054ec:
	movs	r3, #0
	ldrsh	r1, [r1, r3]
	asrs	r2, r2, #20
	lsls	r3, r1, #1
	adds	r3, r3, r1
	lsls	r3, r3, #1
	adds	r2, r2, r3
	movs	r7, #0
	ldrsh	r3, [r4, r7]
	movs	r0, #1
	adds	r2, r2, r3
	adds	r2, #189
	b.n	.L_0200554c
	.2byte 0x0000
	.4byte 0x020036e0
	.4byte 0x0200e2b4
	.4byte 0x050003c0
	.4byte 0x0200e2d4
	.4byte 0x80008000
	.4byte 0x0300122c
	.4byte 0x02000240
	.2byte 0xffff
	.2byte 0x000f
.L_02005528:
	ldr	r3, [r3, #0]
	subs	r2, r0, r3
	cmp	r2, #0
	bge.n	.L_02005534
	ldr	r0, [pc, #140]
	adds	r2, r2, r0
.L_02005534:
	movs	r3, #0
	ldrsh	r1, [r1, r3]
	asrs	r2, r2, #20
	lsls	r3, r1, #1
	adds	r3, r3, r1
	lsls	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #0
	ldrsh	r3, [r4, r7]
	movs	r0, #1
	adds	r2, r2, r3
	adds	r2, #23
.L_0200554c:
	mov	r9, r2
	bl 0x0200ace8
	ldr	r2, [r5, #16]
	ldr	r3, [r6, #0]
	adds	r1, r0, #0
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_02005562
	ldr	r2, [pc, #96]
	adds	r0, r0, r2
.L_02005562:
	ldr	r7, [sp, #0]
	asrs	r2, r0, #20
	movs	r4, #0
	ldrsh	r3, [r7, r4]
	adds	r2, r2, r3
	lsls	r3, r1, #3
	adds	r3, r3, r1
	lsls	r3, r3, #2
	subs	r2, r2, r3
	adds	r7, r2, #4
	movs	r3, #128
	lsls	r3, r3, #1
	ldr	r1, [sp, #36]
	adds	r3, #255
	mov	r0, r9
	ands	r0, r3
	movs	r3, #255
	ands	r7, r3
	movs	r3, #0
	str	r3, [r1, #0]
	lsls	r3, r0, #16
	orrs	r7, r3
	movs	r3, #128
	lsls	r3, r3, #23
	mov	r2, sl
	adds	r0, r2, #0
	orrs	r7, r3
	stmia	r0!, {r7}
	movs	r3, #228
	lsls	r3, r3, #8
	mov	r1, r8
	orrs	r1, r3
	adds	r4, r0, #0
	str	r4, [sp, #36]
	str	r1, [r0, #0]
	ldr	r3, [sp, #32]
	movs	r1, #255
	adds	r0, r3, #0
	bl 0x0200d790
.L_020055b2:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0xffff
	.2byte 0x000f
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r9, r1
	movs	r1, #228
	lsls	r1, r1, #6
	adds	r1, #52
	adds	r6, r0, #0
	movs	r0, #236
	bl 0x0200d758
	adds	r5, r0, #0
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200d760
	adds	r3, r5, #0
	adds	r3, #226
	strh	r6, [r3, #0]
	adds	r7, r0, #0
	adds	r3, #2
	mov	r0, r9
	strh	r0, [r3, #0]
	ldr	r3, [pc, #128]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #218
	adds	r0, r0, r5
	ldr	r3, [r3, #0]
	mov	r8, r0
	movs	r0, #224
	adds	r2, r5, #0
	adds	r1, r5, #0
	adds	r0, r0, r5
	adds	r2, #236
	adds	r1, #240
	mov	sl, r0
	adds	r5, #216
	cmp	r6, r3
	bne.n	.L_02005620
	movs	r3, #160
	lsls	r3, r3, #16
	b.n	.L_02005624
.L_02005620:
	movs	r3, #224
	lsls	r3, r3, #15
.L_02005624:
	str	r3, [r2, #0]
	ldr	r3, [pc, #84]
	str	r3, [r1, #0]
	adds	r0, r6, #0
	bl 0x0200d8a0
	mov	r0, r9
	bl 0x0200d8a0
	movs	r3, #0
	mov	r2, r8
	mov	r0, sl
	strh	r3, [r2, #0]
	adds	r1, r7, #0
	strh	r3, [r0, #0]
	ldr	r0, [pc, #60]
	bl 0x0200d770
	bl 0x0200d788
	movs	r1, #144
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r7, #0
	lsls	r1, r1, #2
	asrs	r0, r0, #16
	bl 0x0200d780
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #118
	ldr	r0, [pc, #32]
	bl 0x0200d730
	adds	r0, r7, #0
	bl 0x0200d768
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xf9a00000
	.4byte 0x0200e2d4
	.2byte 0xd1a1
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	adds	r7, r0, #0
	lsls	r3, r3, #18
	movs	r0, #10
	adds	r3, #236
	adds	r0, #255
	ldr	r6, [r3, #0]
	ldr	r5, [pc, #32]
	bl 0x0200d7a8
	cmp	r0, #0
	bne.n	.L_020056ae
	adds	r3, r6, #0
	adds	r3, #228
	ldrh	r3, [r3, #0]
	strh	r7, [r5, #2]
	strh	r3, [r5, #4]
	strh	r0, [r5, #8]
.L_020056ae:
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #133
	ldr	r0, [pc, #8]
	bl 0x0200d730
	pop	{r5, r6, r7, pc}
	.4byte 0x0200234c
	.2byte 0xcddd
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #236
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #20]
	movs	r1, #0
	adds	r3, #234
	strh	r1, [r3, #0]
	movs	r3, #1
	strh	r0, [r2, #10]
	strh	r1, [r2, #6]
	strh	r3, [r2, #0]
	bl 0x0200cddc
	pop	{pc}
	.4byte 0x0200234c
	.4byte 0x20014b05
	.4byte 0x5e5a2100
	.4byte 0x405a2309
	.4byte 0x43134253
	.4byte 0x1ac00fdb
	.4byte 0x00004770
	.4byte 0x0200234c
	.4byte 0x00004770
	.section .rodata.x0200da70,"a",%progbits
	.4byte 0x0000002e
	.4byte 0x02008059
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x02008695
	.4byte 0x00000011
	.4byte 0x14151213
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
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x02009905
	.4byte 0x02009975
	.4byte 0x02009a71
	.4byte 0x02009afd
	.4byte 0x02009bdd
	.4byte 0x02009bed
	.4byte 0x02009c6d
	.4byte 0x02009d8d
	.4byte 0x000001c2
	.4byte 0x000001c3
	.4byte 0x000001c4
	.4byte 0x000001c5
	.4byte 0x000001c6
	.4byte 0x000001c7
	.4byte 0x000001c8
	.4byte 0x000001c9
	.4byte 0x000001ca
	.4byte 0x000001cb
	.4byte 0x00300001
	.4byte 0x0030000f
	.4byte 0x0030001d
	.4byte 0x00460001
	.4byte 0x0046000f
	.4byte 0x0046001d
	.4byte 0x005c0001
	.4byte 0x005c000f
	.4byte 0x03020100
	.4byte 0x05040404
	.4byte 0x01000706
	.4byte 0x04040302
	.4byte 0x07060504
	.4byte 0x07030200
	.4byte 0x06050401
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x00e80000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x00780000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x01580000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00b80000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000011
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
	.4byte 0x00000000
	.4byte 0x00000021
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
	.4byte 0x0000002c
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
	.4byte 0x377f10a0
	.4byte 0x121722bb
	.4byte 0x0ccc1172
	.4byte 0x7df07ef7
	.4byte 0x610b7ce7
	.4byte 0x50007c00
	.4byte 0x00147fff
	.4byte 0x2f1f0000
	.4byte 0x04060100
	.4byte 0x06e21008
	.4byte 0x44bad8c7
	.4byte 0x1dfaab43
	.4byte 0x8b8c4623
	.4byte 0x0b2ef778
	.4byte 0x01d0f313
	.4byte 0x111ae0ee
	.4byte 0x1bd5eee2
	.4byte 0xad56ab63
	.4byte 0x46239571
	.4byte 0xfc318244
	.4byte 0x5aac0e23
	.4byte 0x8486a2b5
	.4byte 0x0463843e
	.4byte 0xcaaa595a
	.4byte 0x157f9552
	.4byte 0x18214fa5
	.4byte 0x32bbb121
	.4byte 0x094ceaad
	.2byte 0x1c7c
	.2byte 0xc607
	push	{r0, r4, r6, lr}
	stmia	r0!, {r5, r6}
	strb	r7, [r0, #7]
	ldrh	r4, [r0, #34]
	strb	r7, [r4, #24]
	lsls	r3, r4, #24
	.2byte 0xf456
	.2byte 0x1859
.L_02006338:
	ldr	r3, [r2, r2]
	strh	r6, [r1, #12]
	ldrh	r2, [r2, #20]
	.2byte 0xfe01
	.2byte 0x3e05
	str	r0, [r1, #48]
	adds	r4, r0, r0
	str	r3, [r2, #16]
	asrs	r5, r1, #5
	stmia	r2!, {r0, r3, r4, r5}
	strh	r0, [r5, #14]
	ldmia	r7, {r0, r7}
	strh	r1, [r0, #8]
	adds	r2, r0, r0
	pop	{r0, r1, r2, r4, r6, pc}
	.2byte 0x0501
	.4byte 0xc4606351
	.4byte 0xeb1e31d0
	.4byte 0x7041167c
	.4byte 0xe4d59d1d
	.4byte 0x75761cf6
	.4byte 0x99d54eef
	.4byte 0xd0741bc3
	.4byte 0xd87bdd91
	.4byte 0x66403da5
	.4byte 0xf66782cb
	.4byte 0xd877e03a
	.4byte 0x97641fec
	.4byte 0x7640e601
	.4byte 0xcf81c6ad
	.4byte 0xf891f720
	.4byte 0x717cdc12
	.4byte 0x7c54f882
	.4byte 0x0e62cee9
	.4byte 0xee209704
	.4byte 0xe2f921f1
	.4byte 0x00000003
	.4byte 0x33323130
	.4byte 0x37363534
	.4byte 0x42413938
	.4byte 0x46454443
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000001b8
	.4byte 0x800000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000158
	.4byte 0x80000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000f7
	.4byte 0x001030f7
	.4byte 0x002040f7
	.4byte 0x003010f7
	.4byte 0x004020f7
	.4byte 0x005010f1
	.4byte 0x006020f1
	.4byte 0x007010f8
	.4byte 0x008020f8
	.4byte 0x009090f1
	.4byte 0x00a0b0f1
	.4byte 0x000000f9
	.4byte 0x001020f9
	.4byte 0x002010f9
	.4byte 0x003030f8
	.4byte 0x004010fa
	.4byte 0x000000fa
	.4byte 0x001040f9
	.4byte 0x000000f8
	.4byte 0x001210f7
	.4byte 0x002220f7
	.4byte 0x003030f9
	.4byte 0x0041e0f4
	.4byte 0x0051f0f4
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01020000
	.4byte 0xffff0133
	.4byte 0x0200da70
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01020000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00020000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00020000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00020000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff0133
	.4byte 0x0200da70
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x00020000
	.4byte 0xffff0133
	.4byte 0x0200da70
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x01020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00020000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00020000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00020000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00020000
	.4byte 0xffff0133
	.4byte 0x0200da70
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00020000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00020000
	.4byte 0xffff0105
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00020000
	.4byte 0xffff0105
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00020000
	.4byte 0xffff0104
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x01020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x01020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00020000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01020000
	.4byte 0xffff0133
	.4byte 0x0200da70
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x007b00f6
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0004
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0200e484
	.4byte 0x0200e5d4
	.4byte 0x0200e754
	.4byte 0x0200e8bc
	.4byte 0x0200ea3c
	.4byte 0x0200eb2c
	.4byte 0x0200ec4c
	.4byte 0x0200ed9c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte 0x02008429
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte 0x02008429
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020085b5
	.4byte 0x00004602
	.4byte 0x12080033
	.4byte 0x020087e9
	.4byte 0x00004602
	.4byte 0x12080034
	.4byte 0x02008785
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008729
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte 0x02008729
	.4byte 0x10008c15
	.4byte 0xffff0010
	.4byte 0x02008729
	.4byte 0x10008c15
	.4byte 0x13070011
	.4byte 0x02008729
	.4byte 0x10008c15
	.4byte 0xffff0012
	.4byte 0x02008729
	.4byte 0x10008c15
	.4byte 0xffff0013
	.4byte 0x02008729
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008749
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x02008749
	.4byte 0x00008c15
	.4byte 0xffff0010
	.4byte 0x02008749
	.4byte 0x00008c15
	.4byte 0x13070011
	.4byte 0x02008749
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte 0x02008749
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte 0x02008749
	.4byte 0x00004e15
	.4byte 0x03070014
	.4byte 0x02008769
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte 0x02008429
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte 0x02008429
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020085b5
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte 0x020088a5
	.4byte 0x00004602
	.4byte 0xffff0021
	.4byte 0x020088a5
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008819
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte 0x02008819
	.4byte 0x10008c15
	.4byte 0xffff0010
	.4byte 0x02008819
	.4byte 0x10008c15
	.4byte 0xffff0015
	.4byte 0x02008819
	.4byte 0x10008c15
	.4byte 0xffff0016
	.4byte 0x02008819
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200882d
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x0200882d
	.4byte 0x00008c15
	.4byte 0xffff0010
	.4byte 0x0200882d
	.4byte 0x00008c15
	.4byte 0xffff0015
	.4byte 0x0200882d
	.4byte 0x00008c15
	.4byte 0xffff0016
	.4byte 0x0200882d
	.4byte 0x00001815
	.4byte 0x12140011
	.4byte 0x020088d1
	.4byte 0x00001815
	.4byte 0x12150012
	.4byte 0x020088d1
	.4byte 0x00001815
	.4byte 0x12160013
	.4byte 0x020088d1
	.4byte 0x00001815
	.4byte 0x12170014
	.4byte 0x020088d1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte 0x02008429
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte 0x02008429
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020085b5
	.4byte 0x00004602
	.4byte 0xffff0021
	.4byte 0x02008785
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008729
	.4byte 0x10008c15
	.4byte 0x1327000f
	.4byte 0x02008729
	.4byte 0x10008c15
	.4byte 0x13280010
	.4byte 0x02008729
	.4byte 0x10008c15
	.4byte 0xffff0011
	.4byte 0x02008729
	.4byte 0x10008c15
	.4byte 0xffff0012
	.4byte 0x02008729
	.4byte 0x10008c15
	.4byte 0xffff0013
	.4byte 0x02008729
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008749
	.4byte 0x00008c15
	.4byte 0x1327000f
	.4byte 0x02008749
	.4byte 0x00008c15
	.4byte 0x13280010
	.4byte 0x02008749
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte 0x02008749
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte 0x02008749
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte 0x02008749
	.4byte 0x00004e15
	.4byte 0x03270014
	.4byte 0x02008905
	.4byte 0x00004e15
	.4byte 0x03280015
	.4byte 0x02008905
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte 0x02008429
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte 0x02008429
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020085b5
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte 0x02008c85
	.4byte 0x00000202
	.4byte 0xffff0033
	.4byte 0x02008c85
	.4byte 0x00000202
	.4byte 0xffff0034
	.4byte 0x02008c85
	.4byte 0x00000202
	.4byte 0xffff0035
	.4byte 0x02008c85
	.4byte 0x00000002
	.4byte 0x03350032
	.4byte 0x02008c35
	.4byte 0x00000002
	.4byte 0x13350033
	.4byte 0x02008c49
	.4byte 0x00000002
	.4byte 0x03370034
	.4byte 0x02008c5d
	.4byte 0x00000002
	.4byte 0x13370035
	.4byte 0x02008c71
	.4byte 0x00000202
	.4byte 0xffff003c
	.4byte 0x02008c85
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x020089ad
	.4byte 0x00008c15
	.4byte 0x13390010
	.4byte 0x020089ad
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte 0x020089ad
	.4byte 0x00004e15
	.4byte 0x03390016
	.4byte 0x02008991
	.4byte 0x50008615
	.4byte 0x03350012
	.4byte 0x02008b3d
	.4byte 0x50008615
	.4byte 0x13350013
	.4byte 0x02008b4d
	.4byte 0x50008615
	.4byte 0x03370014
	.4byte 0x02008b5d
	.4byte 0x50008615
	.4byte 0x13370015
	.4byte 0x02008b6d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0342000f
	.4byte 0x03440010
	.4byte 0x0000ffff
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte 0x02008429
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte 0x02008429
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020085b5
	.4byte 0x00004602
	.4byte 0xffff0021
	.4byte 0x02008785
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte 0x02008785
	.4byte 0x0000c602
	.4byte 0xffff0032
	.4byte 0x02008785
	.4byte 0x0000c602
	.4byte 0xffff0033
	.4byte 0x02008785
	.4byte 0x00000602
	.4byte 0xffff0022
	.4byte 0x02008cc1
	.4byte 0x00008602
	.4byte 0xffff0022
	.4byte 0x02008cc1
	.4byte 0x50009705
	.4byte 0x03430032
	.4byte 0x02008cd1
	.4byte 0x50009705
	.4byte 0x03450033
	.4byte 0x02008ce5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte 0x02008429
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte 0x02008429
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020085b5
	.4byte 0x00000602
	.4byte 0xffff0021
	.4byte 0x02008785
	.4byte 0x00008602
	.4byte 0xffff0032
	.4byte 0x0200906d
	.4byte 0x00000602
	.4byte 0xffff0032
	.4byte 0x02009081
	.4byte 0x00008602
	.4byte 0xffff0033
	.4byte 0x02009091
	.4byte 0x00000602
	.4byte 0xffff0033
	.4byte 0x020090a5
	.4byte 0x0000c602
	.4byte 0xffff0032
	.4byte 0x020090b5
	.4byte 0x00004602
	.4byte 0xffff0032
	.4byte 0x020090c9
	.4byte 0x0000c602
	.4byte 0xffff0033
	.4byte 0x020090b5
	.4byte 0x00004602
	.4byte 0xffff0033
	.4byte 0x020090c9
	.4byte 0x0000c602
	.4byte 0xffff0034
	.4byte 0x020090b5
	.4byte 0x00004602
	.4byte 0xffff0034
	.4byte 0x020090d9
	.4byte 0x0000c602
	.4byte 0xffff0035
	.4byte 0x020090b5
	.4byte 0x00004602
	.4byte 0xffff0035
	.4byte 0x020090c9
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008729
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte 0x02008729
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008749
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x02008749
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte 0x02008429
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte 0x02008429
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020085b5
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte 0x02009179
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008729
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte 0x02008729
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008749
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x02008749
	.4byte 0x50008805
	.4byte 0x03630032
	.4byte 0x02009149
	.4byte 0x50008805
	.4byte 0x03640033
	.4byte 0x02009159
	.4byte 0x50008805
	.4byte 0x03650034
	.4byte 0x02009169
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0200da8c
	.4byte 0x0200dac8
	.4byte 0x0200db04
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte 0x02008419
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte 0x02008429
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte 0x02008629
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte 0x02008429
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte 0x020084f5
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020085b5
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte 0x020094e5
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x020091c1
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x020091dd
	.4byte 0x00004e15
	.4byte 0x03740011
	.4byte 0x020091f9
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
	.4byte 0x00008f15
	.4byte 0x02890008
	.4byte 0x020095a9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x50008905
	.4byte 0xffff0021
	.4byte 0x020095b9
	.4byte 0x50008905
	.4byte 0xffff0022
	.4byte 0x020095d9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008039
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200963d
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020096c9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x02830021
	.4byte 0x02009755
	.4byte 0x00000002
	.4byte 0x19ff0028
	.4byte 0x020097e1
	.4byte 0x00000002
	.4byte 0x19ff0029
	.4byte 0x020097e1
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00008c15
	.4byte 0x09fc000a
	.4byte 0x0200985d
	.4byte 0x00000009
	.4byte 0x09fc0000
	.4byte 0x0200985d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0200f050
	.4byte 0x0200f200
	.4byte 0x0200f3bc
	.4byte 0x0200f56c
	.4byte 0x0200f740
	.4byte 0x0200f89c
	.4byte 0x0200fa64
	.4byte 0x0200fbcc
	.4byte 0xffffffff
	.4byte 0x00000001
