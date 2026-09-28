.syntax unified
	.thumb
	push	{lr}
	movs	r0, #14
	movs	r1, #0
	movs	r2, #15
	bl 0x02009110
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02008fe8
	pop	{pc}
	.2byte 0x0000
	.global Func_02000050
	.thumb_func
Func_02000050:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9130
	.2byte 0x0200
	.global Func_02000058
	.thumb_func
Func_02000058:
	movs	r0, #0
	bx	lr
	.global Func_0200005c
	.thumb_func
Func_0200005c:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9160
	.2byte 0x0200
	.global Func_02000064
	.thumb_func
Func_02000064:
	push	{lr}
	ldr	r3, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_0200009e
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_0200008a
	ldr	r0, [pc, #60]
	b.n	.L_020000ba
.L_0200008a:
	movs	r0, #157
	lsls	r0, r0, #4
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_0200009a
	ldr	r0, [pc, #48]
	b.n	.L_020000ba
.L_0200009a:
	ldr	r0, [pc, #48]
	b.n	.L_020000ba
.L_0200009e:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_020000b8
	movs	r0, #157
	lsls	r0, r0, #4
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_020000b4
	ldr	r0, [pc, #32]
	b.n	.L_020000ba
.L_020000b4:
	ldr	r0, [pc, #32]
	b.n	.L_020000ba
.L_020000b8:
	ldr	r0, [pc, #32]
.L_020000ba:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000ea
	.4byte 0x0200947c
	.4byte 0x020092cc
	.4byte 0x020091ac
	.4byte 0x000000eb
	.4byte 0x0200959c
	.4byte 0x0200965c
	.2byte 0x9194
	.2byte 0x0200
	.global Func_020000e0
	.thumb_func
Func_020000e0:
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_020000f8
	ldr	r0, [pc, #24]
	b.n	.L_02000104
.L_020000f8:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000102
	ldr	r0, [pc, #24]
	b.n	.L_02000104
.L_02000102:
	ldr	r0, [pc, #24]
.L_02000104:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000ea
	.4byte 0x02009728
	.4byte 0x000000eb
	.4byte 0x020098f0
	.2byte 0x971c
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x02009010
	movs	r0, #0
	bl 0x02009108
	ldr	r0, [pc, #204]
	bl 0x02009098
	movs	r1, #0
	movs	r0, #18
	bl 0x020090a0
	ldr	r5, [pc, #196]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r5, r2
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x02009020
	cmp	r0, #0
	bne.n	.L_02000192
	movs	r0, #18
	movs	r1, #3
	bl 0x02009078
	movs	r0, #18
	movs	r1, #0
	bl 0x020090a8
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r5, r2
	movs	r0, #157
	movs	r2, #1
	strh	r2, [r3, #0]
	lsls	r0, r0, #4
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_0200018a
	movs	r0, #252
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02008fe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x02008fe0
.L_0200018a:
	movs	r0, #4
	bl 0x020090f0
	b.n	.L_020001f8
.L_02000192:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #18
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x020090a8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #18
	ldr	r1, [pc, #80]
	adds	r2, #204
	bl 0x02009030
	movs	r0, #18
	movs	r1, #2
	bl 0x02009078
	ldr	r0, [r6, #0]
	bl 0x02009028
	cmp	r0, #0
	beq.n	.L_020001da
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #18
	bl 0x02009038
.L_020001da:
	movs	r0, #18
	bl 0x02009058
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x02009060
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02008fe0
	bl 0x02009018
.L_020001f8:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0000289a
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, lr}
	movs	r0, #157
	lsls	r0, r0, #4
	sub	sp, #8
	bl 0x02008fd8
	cmp	r0, #0
	bne.n	.L_0200021a
	b.n	.L_020003dc
.L_0200021a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_0200022a
	b.n	.L_020003dc
.L_0200022a:
	bl 0x02009010
	movs	r0, #0
	bl 0x02009108
	movs	r0, #192
	movs	r1, #1
	movs	r2, #154
	lsls	r0, r0, #15
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x020090d8
	movs	r3, #208
	movs	r1, #240
	movs	r2, #181
	lsls	r3, r3, #8
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	movs	r0, #9
	bl 0x02009068
	movs	r0, #1
	bl 0x02008fc8
	movs	r1, #128
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x02009030
	movs	r0, #9
	movs	r1, #6
	movs	r2, #0
	bl 0x02009088
	movs	r2, #168
	movs	r0, #9
	movs	r1, #64
	lsls	r2, r2, #1
	bl 0x02009040
	movs	r2, #154
	movs	r0, #9
	movs	r1, #70
	lsls	r2, r2, #1
	bl 0x02009048
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x020090b0
	movs	r3, #160
	movs	r1, #188
	movs	r2, #136
	lsls	r3, r3, #7
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r0, #10
	bl 0x02009068
	movs	r0, #1
	bl 0x02008fc8
	movs	r1, #128
	movs	r2, #128
	movs	r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x02009030
	movs	r0, #10
	movs	r1, #6
	movs	r2, #0
	bl 0x02009088
	movs	r2, #144
	movs	r0, #10
	movs	r1, #90
	lsls	r2, r2, #1
	bl 0x02009040
	movs	r2, #158
	lsls	r2, r2, #1
	movs	r0, #10
	movs	r1, #80
	bl 0x02009048
	movs	r1, #0
	movs	r0, #10
	bl 0x020090b8
	movs	r0, #9
	bl 0x02009028
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r0, #10
	bl 0x02009008
	ldr	r0, [pc, #228]
	bl 0x02009098
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #9
	movs	r1, #0
	bl 0x020090a8
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #10
	bl 0x020090c8
	movs	r0, #10
	movs	r1, #0
	bl 0x020090a8
	movs	r1, #192
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #6
	bl 0x020090b0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #9
	movs	r1, #0
	bl 0x020090a8
	movs	r1, #176
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x020090b8
	movs	r0, #10
	movs	r1, #3
	bl 0x02009078
	movs	r0, #10
	movs	r1, #0
	bl 0x020090a8
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x020090b0
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #9
	bl 0x020090c8
	movs	r2, #20
	movs	r0, #9
	movs	r1, #0
	bl 0x020090b0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #9
	movs	r1, #0
	bl 0x020090a8
	movs	r0, #10
	movs	r1, #0
	bl 0x020090a8
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r6, #19
	movs	r0, #1
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x02008ff8
	movs	r5, #4
	movs	r0, #1
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x02008ff8
	movs	r3, #20
	str	r3, [sp, #4]
	movs	r0, #1
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x02008ff8
	movs	r3, #21
	str	r3, [sp, #4]
	movs	r0, #1
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x02008ff8
	movs	r0, #155
	lsls	r0, r0, #4
	bl 0x02008fe0
	bl 0x02009018
.L_020003dc:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x2bbc
	.2byte 0x0000
	push	{lr}
	movs	r0, #157
	lsls	r0, r0, #4
	bl 0x02008fd8
	cmp	r0, #0
	bne.n	.L_020003f4
	b.n	.L_0200062c
.L_020003f4:
	bl 0x02009010
	movs	r0, #0
	bl 0x02009108
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	bl 0x020090d8
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #536]
	adds	r1, #204
	bl 0x020090d0
	movs	r0, #130
	movs	r1, #1
	movs	r2, #186
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x020090d8
	bl 0x020090e0
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #23
	bl 0x020090c8
	movs	r1, #192
	movs	r2, #20
	lsls	r1, r1, #6
	movs	r0, #23
	bl 0x020090b0
	ldr	r0, [pc, #488]
	bl 0x02009098
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r1, #176
	movs	r0, #23
	lsls	r1, r1, #8
	bl 0x020090b8
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #23
	bl 0x020090a8
	bl 0x020090e8
	movs	r1, #153
	movs	r3, #0
	adds	r0, #85
	lsls	r1, r1, #8
	strb	r3, [r0, #0]
	adds	r1, #153
	ldr	r0, [pc, #440]
	bl 0x020090d0
	movs	r0, #220
	movs	r1, #128
	movs	r2, #184
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #17
	bl 0x020090d8
	bl 0x020090e0
	movs	r0, #40
	bl 0x02009008
	movs	r0, #216
	movs	r1, #128
	movs	r2, #232
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r1, r1, #14
	lsls	r0, r0, #17
	bl 0x020090d8
	bl 0x020090e0
	movs	r0, #40
	bl 0x02009008
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #372]
	adds	r1, #153
	bl 0x020090d0
	movs	r0, #130
	movs	r1, #128
	movs	r2, #186
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r1, r1, #14
	lsls	r0, r0, #18
	bl 0x020090d8
	bl 0x020090e0
	movs	r0, #20
	bl 0x02009008
	movs	r1, #192
	movs	r0, #23
	lsls	r1, r1, #6
	bl 0x020090b8
	movs	r0, #23
	movs	r1, #3
	bl 0x02009080
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #23
	bl 0x020090c8
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r0, #23
	movs	r1, #4
	bl 0x02009078
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r1, #176
	movs	r0, #23
	lsls	r1, r1, #8
	bl 0x020090b8
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r1, #192
	movs	r0, #23
	lsls	r1, r1, #6
	bl 0x020090b8
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r0, #23
	movs	r1, #3
	bl 0x02009080
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r0, #23
	movs	r1, #4
	bl 0x02009078
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	ldr	r3, [pc, #200]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #2
	ldr	r0, [r3, #0]
	adds	r1, #255
	movs	r2, #40
	bl 0x020090c8
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #23
	bl 0x020090c8
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #23
	bl 0x020090c8
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r0, #23
	movs	r1, #3
	bl 0x02009080
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #204
	adds	r2, #102
	movs	r0, #23
	bl 0x02009030
	movs	r0, #23
	bl 0x02009028
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #252
	movs	r2, #180
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #23
	bl 0x02009048
	movs	r0, #1
	bl 0x02009008
	movs	r0, #23
	bl 0x02009028
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	movs	r1, #192
	strb	r3, [r0, #0]
	lsls	r1, r1, #6
	movs	r0, #23
	bl 0x020090b8
	movs	r0, #23
	movs	r1, #3
	bl 0x02009080
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #177
	bl 0x02008fe0
	bl 0x02009018
.L_0200062c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00026666
	.4byte 0x00002bd4
	.4byte 0x0004cccc
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	bl 0x02009010
	movs	r0, #0
	bl 0x02009108
	ldr	r7, [pc, #288]
	movs	r5, #133
	lsls	r5, r5, #2
	adds	r6, r7, r5
	ldr	r1, [r6, #0]
	movs	r0, #18
	bl 0x02009070
	movs	r0, #1
	bl 0x02008fc8
	movs	r1, #160
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020090b0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #18
	adds	r1, #204
	adds	r2, #102
	bl 0x02009030
	movs	r2, #248
	movs	r0, #18
	adds	r1, r5, #0
	bl 0x02009048
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #18
	bl 0x020090b8
	ldr	r0, [pc, #220]
	bl 0x02009098
	movs	r1, #0
	movs	r0, #18
	bl 0x020090a0
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x02009020
	cmp	r0, #0
	bne.n	.L_020006f2
	movs	r0, #18
	movs	r1, #3
	bl 0x02009078
	movs	r0, #18
	movs	r1, #0
	bl 0x020090a8
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r7, r2
	movs	r0, #157
	movs	r2, #1
	strh	r2, [r3, #0]
	lsls	r0, r0, #4
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_020006ea
	movs	r0, #252
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02008fe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x02008fe0
.L_020006ea:
	movs	r0, #4
	bl 0x020090f0
	b.n	.L_0200076e
.L_020006f2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #18
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x020090a8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #18
	ldr	r1, [pc, #100]
	adds	r2, #204
	bl 0x02009030
	movs	r0, #18
	movs	r1, #2
	bl 0x02009078
	ldr	r0, [r6, #0]
	bl 0x02009028
	cmp	r0, #0
	beq.n	.L_0200073a
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #18
	bl 0x02009038
.L_0200073a:
	movs	r0, #18
	bl 0x02009058
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x02009060
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r6, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x02009030
	movs	r1, #132
	movs	r2, #138
	ldr	r0, [r6, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x02009048
	bl 0x02009018
.L_0200076e:
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0000289a
	.2byte 0x9999
	.2byte 0x0001
	push	{lr}
	movs	r0, #130
	lsls	r0, r0, #4
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_020007aa
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #177
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_020007aa
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #173
	bl 0x02008fd8
	cmp	r0, #0
	bne.n	.L_020007aa
	bl 0x0200897c
.L_020007aa:
	pop	{pc}
	push	{r5, r6, r7, lr}
	sub	sp, #12
	movs	r3, #7
	movs	r2, #23
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #9
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02009120
	movs	r0, #63
	bl 0x02008fd8
	cmp	r0, #0
	bne.n	.L_02000854
	ldr	r3, [pc, #132]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	bl 0x02009028
	adds	r5, r0, #0
	bl 0x02009010
	movs	r0, #0
	bl 0x02009108
	ldr	r3, [r5, #8]
	movs	r7, #0
	asrs	r3, r3, #20
	cmp	r3, #7
	bne.n	.L_02000816
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #23
	bne.n	.L_02000816
	ldr	r0, [r6, #0]
	movs	r1, #6
	movs	r2, #0
	bl 0x02009088
	movs	r2, #188
	ldr	r0, [r6, #0]
	movs	r1, #104
	lsls	r2, r2, #1
	bl 0x02009038
	movs	r7, #1
.L_02000816:
	movs	r1, #240
	movs	r2, #188
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r0, #14
	bl 0x02009060
	movs	r0, #1
	bl 0x02008fc8
	movs	r0, #14
	movs	r1, #4
	movs	r2, #10
	bl 0x02009088
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02008fe0
	cmp	r7, #0
	beq.n	.L_02000850
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x020090b8
.L_02000850:
	bl 0x02009018
.L_02000854:
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #4
	movs	r2, #10
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #9
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02009120
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #6
	movs	r2, #9
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #9
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02009120
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #6
	movs	r2, #0
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	str	r2, [sp, #8]
	movs	r0, #9
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02009120
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r3, #12
	movs	r2, #6
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #12
	movs	r1, #5
	movs	r2, #1
	movs	r3, #1
	bl 0x02009120
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #13
	movs	r2, #7
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #9
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02009120
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #14
	movs	r2, #6
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #9
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02009120
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #21
	movs	r2, #6
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #9
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02009120
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #22
	movs	r2, #6
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #9
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02009120
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #23
	movs	r2, #7
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #9
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02009120
	add	sp, #12
	pop	{pc}
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	bl 0x02009010
	movs	r0, #0
	bl 0x02009108
	movs	r2, #208
	lsls	r2, r2, #8
	mov	r8, r2
	movs	r1, #252
	movs	r2, #230
	mov	r3, r8
	lsls	r2, r2, #17
	lsls	r1, r1, #17
	movs	r0, #24
	bl 0x02009068
	movs	r0, #1
	bl 0x02008fc8
	ldr	r0, [pc, #512]
	bl 0x02009098
	movs	r0, #24
	movs	r1, #0
	bl 0x020090a8
	ldr	r3, [pc, #504]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #128
	ldr	r0, [r3, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020090b0
	movs	r1, #192
	movs	r2, #0
	movs	r0, #23
	lsls	r1, r1, #6
	bl 0x020090b0
	movs	r1, #204
	lsls	r1, r1, #6
	adds	r1, #51
	ldr	r0, [pc, #468]
	bl 0x020090d0
	bl 0x020090e8
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #1
	movs	r0, #128
	movs	r2, #190
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x020090d8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #24
	ldr	r1, [pc, #428]
	adds	r2, #204
	bl 0x02009030
	movs	r1, #130
	movs	r2, #216
	movs	r0, #24
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x02009048
	movs	r1, #130
	movs	r2, #194
	movs	r0, #24
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x02009048
	movs	r1, #192
	movs	r2, #0
	movs	r0, #23
	lsls	r1, r1, #6
	bl 0x020090b0
	movs	r0, #24
	movs	r1, #0
	bl 0x020090a8
	movs	r1, #128
	movs	r2, #10
	lsls	r1, r1, #1
	movs	r0, #23
	bl 0x020090c8
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #24
	bl 0x020090c8
	movs	r0, #24
	movs	r1, #0
	bl 0x020090a8
	movs	r0, #23
	movs	r1, #3
	bl 0x02009080
	mov	r1, r8
	movs	r0, #24
	bl 0x020090b8
	movs	r0, #24
	bl 0x02009028
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #130
	movs	r2, #192
	strb	r3, [r0, #0]
	lsls	r2, r2, #1
	lsls	r1, r1, #2
	movs	r0, #24
	bl 0x02009048
	movs	r0, #1
	bl 0x02009008
	movs	r0, #24
	bl 0x02009028
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r6, #1
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #24
	bl 0x02009090
	movs	r1, #130
	movs	r2, #190
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r0, #65
	bl 0x02009060
	movs	r0, #1
	bl 0x02008fc8
	movs	r0, #24
	bl 0x02009028
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #130
	ands	r5, r3
	movs	r2, #194
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	strb	r5, [r0, #0]
	movs	r0, #24
	bl 0x02009048
	movs	r0, #1
	bl 0x02009008
	movs	r0, #24
	bl 0x02009028
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r6, r3
	strb	r6, [r0, #0]
	movs	r0, #10
	bl 0x02009008
	movs	r1, #176
	movs	r2, #10
	movs	r0, #24
	lsls	r1, r1, #8
	bl 0x020090b0
	movs	r0, #24
	movs	r1, #3
	bl 0x02009080
	movs	r1, #130
	movs	r2, #217
	movs	r0, #24
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x02009048
	movs	r0, #128
	movs	r1, #1
	movs	r2, #174
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x020090d8
	movs	r1, #252
	movs	r2, #230
	movs	r0, #24
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x02009048
	movs	r2, #0
	movs	r0, #24
	movs	r1, #0
	bl 0x02009060
	mov	r1, r8
	movs	r0, #23
	bl 0x020090b8
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #23
	bl 0x020090c8
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r0, #23
	movs	r1, #3
	bl 0x02009078
	movs	r0, #23
	movs	r1, #0
	bl 0x020090a8
	movs	r0, #23
	movs	r1, #3
	bl 0x02009080
	movs	r1, #132
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #23
	bl 0x020090c8
	movs	r1, #0
	movs	r0, #23
	bl 0x020090a8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #173
	bl 0x02008fe0
	bl 0x02009018
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x00002c85
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009028
	ldr	r3, [r0, #80]
	movs	r0, #16
	ldrb	r5, [r3, #9]
	lsls	r5, r5, #28
	lsrs	r5, r5, #30
	adds	r1, r5, #0
	bl 0x020090c0
	adds	r1, r5, #0
	movs	r0, #8
	bl 0x020090c0
	movs	r0, #10
	adds	r1, r5, #0
	bl 0x020090c0
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009028
	ldr	r3, [r0, #80]
	movs	r0, #13
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x020090c0
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #92]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000c44
	ldr	r3, [pc, #84]
	movs	r1, #9
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_02000c2c:
	ldr	r4, [pc, #76]
	lsls	r3, r1, #1
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #4
	bne.n	.L_02000c2c
	ldr	r3, [pc, #60]
	strh	r0, [r3, #0]
.L_02000c44:
	ldr	r3, [pc, #44]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000c72
	ldr	r3, [pc, #48]
	movs	r1, #15
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_02000c5a:
	ldr	r4, [pc, #32]
	lsls	r3, r1, #1
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #12
	bne.n	.L_02000c5a
	ldr	r3, [pc, #24]
	strh	r0, [r3, #0]
.L_02000c72:
	pop	{pc}
	.4byte 0x0300122c
	.4byte 0x05000172
	.4byte 0x05000160
	.4byte 0x05000168
	.4byte 0x0500017e
	.2byte 0x0178
	.2byte 0x0500
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #8
	ldr	r6, [r3, #108]
	bl 0x02009010
	movs	r0, #0
	bl 0x02009108
	movs	r0, #158
	bl 0x02009128
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #51
	movs	r2, #70
	movs	r1, #38
	movs	r3, #22
	bl 0x02008ff0
	ldr	r5, [pc, #72]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x02009078
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009030
	movs	r2, #4
	movs	r1, #2
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x02009050
	movs	r0, #8
	bl 0x02009008
	bl 0x020090f8
	bl 0x02009100
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r6, r3
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl 0x020090f0
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	.global Func_02000d08
	.thumb_func
Func_02000d08:
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000d22
	bl 0x02008dc4
	b.n	.L_02000d2c
.L_02000d22:
	ldr	r3, [pc, #20]
	cmp	r2, r3
	bne.n	.L_02000d2c
	bl 0x02008ed0
.L_02000d2c:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000ea
	.2byte 0x00eb
	.2byte 0x0000
	.global Func_02000d3c
	.thumb_func
Func_02000d3c:
	push	{r5, r6, lr}
	ldr	r3, [pc, #124]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #116]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_02000db4
	movs	r0, #157
	lsls	r0, r0, #4
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_02000db4
	movs	r5, #10
	movs	r6, #4
	movs	r0, #54
	movs	r1, #47
	movs	r2, #4
	movs	r3, #15
	str	r6, [sp, #4]
	str	r5, [sp, #0]
	bl 0x02008ff0
	movs	r0, #54
	movs	r1, #38
	movs	r2, #73
	movs	r3, #7
	str	r6, [sp, #4]
	str	r5, [sp, #0]
	bl 0x02008ff0
	movs	r6, #3
	movs	r0, #54
	movs	r1, #43
	movs	r2, #21
	movs	r3, #55
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x02008ff0
	movs	r0, #65
	movs	r1, #43
	movs	r2, #21
	movs	r3, #20
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x02008ff0
	movs	r0, #4
	movs	r1, #47
	movs	r2, #4
	movs	r3, #50
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x02008ff0
.L_02000db4:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x00ea
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #144
	movs	r3, #192
	lsls	r0, r0, #4
	lsls	r3, r3, #18
	adds	r0, #180
	ldr	r5, [r3, #32]
	bl 0x02008fe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #173
	bl 0x02008fd8
	cmp	r0, #0
	bne.n	.L_02000dee
	movs	r0, #65
	movs	r1, #0
	movs	r2, #0
	bl 0x02009060
.L_02000dee:
	ldrb	r2, [r5, #23]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r5, #23]
	ldr	r3, [pc, #196]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	bl 0x02009028
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r1, #144
	strb	r3, [r0, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #172]
	bl 0x02008fd0
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #168]
	bl 0x02008fd0
	movs	r0, #17
	bl 0x02009028
	movs	r1, #0
	bl 0x02009000
	movs	r0, #17
	bl 0x02009028
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_02000e82
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x02009060
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x02009060
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x02009060
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x02009060
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x02009060
	b.n	.L_02000ec2
.L_02000e82:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #177
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_02000ea2
	movs	r3, #192
	movs	r1, #252
	movs	r2, #180
	lsls	r3, r3, #6
	movs	r0, #23
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x02009068
.L_02000ea2:
	ldr	r1, [r6, #0]
	movs	r0, #19
	bl 0x02009118
	ldr	r1, [r6, #0]
	movs	r0, #20
	bl 0x02009118
	ldr	r1, [r6, #0]
	movs	r0, #21
	bl 0x02009118
	ldr	r1, [r6, #0]
	movs	r0, #22
	bl 0x02009118
.L_02000ec2:
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x02008c15
	.2byte 0x8bb9
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #236]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	sub	sp, #8
	cmp	r3, #3
	bne.n	.L_02000eec
	movs	r0, #48
	adds	r0, #255
	bl 0x02008fe8
.L_02000eec:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_02000f1a
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x02009060
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x02009060
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x02009060
	b.n	.L_02000f96
.L_02000f1a:
	movs	r0, #155
	lsls	r0, r0, #4
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_02000f96
	movs	r1, #140
	movs	r2, #154
	movs	r0, #9
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x02009068
	movs	r1, #160
	movs	r2, #158
	movs	r0, #10
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x02009068
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r6, #19
	movs	r0, #1
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x02008ff8
	movs	r5, #4
	movs	r0, #1
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x02008ff8
	movs	r3, #20
	str	r3, [sp, #4]
	movs	r0, #1
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x02008ff8
	movs	r3, #21
	str	r3, [sp, #4]
	movs	r0, #1
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x02008ff8
	movs	r0, #1
	bl 0x02008fc8
.L_02000f96:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02008fd8
	cmp	r0, #0
	beq.n	.L_02000fb0
	movs	r1, #240
	movs	r2, #188
	movs	r0, #14
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x02009060
.L_02000fb0:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl 0x02008fd0
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02008bf1
	.section .rodata,"a",%progbits
	.4byte 0xffff0000
	.4byte 0x00000048
	.4byte 0x00000127
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000ea
	.4byte 0x00101028
	.4byte 0x002010eb
	.4byte 0x003050eb
	.4byte 0x00434002
	.4byte 0x000000eb
	.4byte 0x001020ea
	.4byte 0x00235002
	.4byte 0x003150ed
	.4byte 0x004040eb
	.4byte 0x005030ea
	.4byte 0x00635002
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00012000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00018000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00014000
	.4byte 0x190a01a4
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00012000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00370000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00018000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01040000
	.4byte 0x00000000
	.4byte 0x009e0000
	.4byte 0x00014000
	.4byte 0x190a01a4
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00030000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00038000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00030000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00030000
	.4byte 0xffff003b
	.4byte 0x00000001
	.4byte 0x02040000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00035000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00a20000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00012000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x006f0000
	.4byte 0x00000000
	.4byte 0x01470000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00018000
	.4byte 0xffff006a
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00004000
	.4byte 0x190a01a4
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00008000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x017c0000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00014000
	.4byte 0x190a01a4
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0x003f00f3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0x190a01a4
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0x003f00f3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008c8d
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x09b1000a
	.4byte 0x020083e5
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008121
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008641
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x0200877d
	.4byte 0x00000000
	.4byte 0x09d0000d
	.4byte 0x000028c4
	.4byte 0x00000000
	.4byte 0x09b2000d
	.4byte 0x00002bb8
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002c51
	.4byte 0x00008d15
	.4byte 0x09d0000d
	.4byte 0x000028c8
	.4byte 0x00008d15
	.4byte 0x09b2000d
	.4byte 0x00002bc5
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002c55
	.4byte 0x00000000
	.4byte 0x09d0000e
	.4byte 0x000028c5
	.4byte 0x00000000
	.4byte 0x09b2000e
	.4byte 0x00002bb9
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002c52
	.4byte 0x00008d15
	.4byte 0x09d0000e
	.4byte 0x000028c9
	.4byte 0x00008d15
	.4byte 0x09b2000e
	.4byte 0x00002bc6
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002c56
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000028c6
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000028ca
	.4byte 0x00000000
	.4byte 0x09d00010
	.4byte 0x000028c7
	.4byte 0x00000000
	.4byte 0x09b20010
	.4byte 0x00002bbb
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002c54
	.4byte 0x00008d15
	.4byte 0x09d00010
	.4byte 0x000028cb
	.4byte 0x00008d15
	.4byte 0x09b20010
	.4byte 0x00002bc8
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002c58
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002bcc
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002bd0
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002bcd
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002bd1
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002bce
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002bd2
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002bcf
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002bd3
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002be2
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002be3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c401
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c401
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0x09b0000a
	.4byte 0x02008209
	.4byte 0x00000400
	.4byte 0xffff000e
	.4byte 0x02008039
	.4byte 0x00000000
	.4byte 0x09b20008
	.4byte 0x00002bba
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002c53
	.4byte 0x00008d15
	.4byte 0x09b20008
	.4byte 0x00002bc7
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002c57
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002bc2
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002bc9
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002bc3
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002bca
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002bc4
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002bcb
	.4byte 0x00008f15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x50008905
	.4byte 0xffff0014
	.4byte 0x020087ad
	.4byte 0x50008905
	.4byte 0xffff0015
	.4byte 0x0200885d
	.4byte 0x50008905
	.4byte 0xffff0016
	.4byte 0x0200887d
	.4byte 0x50008905
	.4byte 0xffff0017
	.4byte 0x0200889d
	.4byte 0x50008905
	.4byte 0xffff0018
	.4byte 0x020088bd
	.4byte 0x50008905
	.4byte 0xffff0019
	.4byte 0x020088dd
	.4byte 0x50008905
	.4byte 0xffff001a
	.4byte 0x020088fd
	.4byte 0x50008905
	.4byte 0xffff001b
	.4byte 0x0200891d
	.4byte 0x50008905
	.4byte 0xffff001c
	.4byte 0x0200893d
	.4byte 0x50008905
	.4byte 0xffff001d
	.4byte 0x0200895d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
