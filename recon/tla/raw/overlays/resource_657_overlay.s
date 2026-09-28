.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa278
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
	.2byte 0xa2a8
	.2byte 0x0200
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02000064
	ldr	r0, [pc, #12]
	b.n	.L_02000066
.L_02000064:
	ldr	r0, [pc, #12]
.L_02000066:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000002a
	.4byte 0x0200a35c
	.2byte 0xa2cc
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #133
	sub	sp, #8
	bl 0x02009ee4
	cmp	r0, #0
	beq.n	.L_02000132
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl 0x02009f5c
	movs	r0, #0
	bl 0x0200a0ac
	movs	r5, #8
.L_0200009c:
	adds	r0, r5, #0
	bl 0x02009f74
	cmp	r0, #0
	beq.n	.L_020000b4
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r0, #12]
.L_020000b4:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_0200009c
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009ee4
	cmp	r0, #0
	bne.n	.L_020000e2
	movs	r0, #158
	bl 0x0200a0d4
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #51
	movs	r1, #38
	movs	r2, #70
	movs	r3, #22
	bl 0x02009f2c
.L_020000e2:
	ldr	r5, [pc, #84]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x02009fdc
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009f7c
	movs	r2, #4
	movs	r1, #2
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x02009fb4
	movs	r0, #8
	bl 0x02009f54
	movs	r0, #123
	bl 0x0200a0d4
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200a07c
	bl 0x0200a09c
	bl 0x0200a0a4
	bl 0x02009f64
.L_02000132:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.global Func_0200013c
	.thumb_func
Func_0200013c:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02000154
	ldr	r0, [pc, #12]
	b.n	.L_02000156
.L_02000154:
	ldr	r0, [pc, #12]
.L_02000156:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000002b
	.4byte 0x0200a3f8
	.2byte 0xa3ec
	.2byte 0x0200
	push	{lr}
	bl 0x02009f5c
	movs	r0, #0
	bl 0x0200a0ac
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x02009f44
	bl 0x02009f64
	pop	{pc}
	.2byte 0x0000
	.2byte 0x185d
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #48]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020001b8
	ldr	r3, [pc, #40]
	movs	r1, #15
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_020001a0:
	ldr	r4, [pc, #32]
	lsls	r3, r1, #1
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #12
	bne.n	.L_020001a0
	ldr	r3, [pc, #16]
	strh	r0, [r3, #0]
.L_020001b8:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0x0500017e
	.4byte 0x05000160
	.2byte 0x0178
	.2byte 0x0500
	push	{lr}
	ldr	r3, [pc, #48]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020001fc
	ldr	r3, [pc, #40]
	movs	r1, #9
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_020001e4:
	ldr	r4, [pc, #32]
	lsls	r3, r1, #1
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #4
	bne.n	.L_020001e4
	ldr	r3, [pc, #16]
	strh	r0, [r3, #0]
.L_020001fc:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0x05000172
	.4byte 0x05000160
	.2byte 0x0168
	.2byte 0x0500
	push	{lr}
	bl 0x02009f5c
	movs	r0, #0
	bl 0x0200a0ac
	bl 0x0200a094
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #44]
	bl 0x02009ea4
	bl 0x0200a0a4
	movs	r0, #60
	bl 0x02009f54
	movs	r0, #13
	bl 0x02009f34
	movs	r0, #60
	bl 0x02009f54
	movs	r0, #60
	bl 0x02009f54
	movs	r0, #14
	bl 0x02009f34
	bl 0x02009f64
	pop	{pc}
	.2byte 0x0000
	.2byte 0x81cd
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	ldr	r1, [pc, #32]
	movs	r3, #132
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldr	r3, [r1, #0]
	str	r3, [r2, #8]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #14
	str	r3, [r1, #0]
	cmp	r3, r2
	bne.n	.L_02000280
	movs	r3, #0
	str	r3, [r1, #0]
.L_02000280:
	pop	{pc}
	.2byte 0x0000
	.2byte 0xa4b4
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	movs	r2, #132
	lsls	r2, r2, #1
	adds	r5, r5, r2
	bl 0x0200a074
	ldr	r6, [pc, #40]
	mov	r8, r0
	ldr	r0, [r6, #0]
	bl 0x02009ebc
	mov	r2, r8
	ldr	r3, [r2, #12]
	asrs	r0, r0, #2
	adds	r3, r3, r0
	str	r3, [r2, #12]
	movs	r2, #128
	ldr	r3, [r5, #12]
	lsls	r2, r2, #2
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldr	r3, [r6, #0]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0xa4b0
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #8
	bl 0x02009f5c
	movs	r0, #0
	bl 0x0200a0ac
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200a064
	bl 0x0200a074
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r1, #192
	movs	r0, #171
	movs	r2, #204
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200a064
	movs	r0, #1
	bl 0x02009e9c
	movs	r3, #7
	movs	r2, #3
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #60
	movs	r1, #53
	movs	r2, #67
	movs	r3, #16
	bl 0x02009f2c
	ldr	r6, [pc, #900]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r6, r2
	movs	r1, #138
	movs	r2, #188
	ldr	r0, [r6, #0]
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x02009fcc
	movs	r3, #208
	lsls	r3, r3, #8
	movs	r0, #9
	ldr	r1, [pc, #872]
	ldr	r2, [pc, #876]
	bl 0x02009fcc
	movs	r2, #202
	movs	r0, #7
	ldr	r1, [pc, #868]
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x02009fcc
	movs	r3, #128
	movs	r1, #142
	movs	r2, #204
	lsls	r3, r3, #6
	movs	r0, #6
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	mov	r8, r3
	bl 0x02009fcc
	movs	r3, #128
	movs	r2, #220
	lsls	r2, r2, #17
	lsls	r3, r3, #7
	ldr	r1, [pc, #836]
	movs	r0, #5
	bl 0x02009fcc
	bl 0x02009f1c
	movs	r0, #1
	bl 0x02009e9c
	ldr	r1, [pc, #820]
	movs	r0, #5
	bl 0x02009f84
	ldr	r1, [pc, #816]
	movs	r0, #6
	bl 0x02009f84
	ldr	r1, [pc, #812]
	movs	r0, #9
	bl 0x02009f84
	bl 0x0200a094
	ldr	r3, [pc, #804]
	movs	r1, #144
	str	r5, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #800]
	bl 0x02009ea4
	ldr	r3, [pc, #800]
	movs	r1, #144
	str	r5, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #796]
	bl 0x02009ea4
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200a05c
	movs	r1, #176
	movs	r2, #198
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r1, r1, #14
	ldr	r0, [pc, #768]
	bl 0x0200a064
	bl 0x0200a06c
	movs	r0, #80
	bl 0x02009f54
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200a044
	ldr	r0, [pc, #744]
	bl 0x0200a01c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r5, #192
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r6, #0]
	lsls	r5, r5, #18
	bl 0x0200a04c
	ldr	r2, [r5, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #192
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r1, r1, #7
	movs	r2, #20
	ldr	r0, [r6, #0]
	bl 0x0200a03c
	movs	r1, #0
	movs	r0, #7
	bl 0x0200a034
	movs	r0, #5
	bl 0x02009f8c
	movs	r0, #9
	bl 0x02009f8c
	movs	r0, #6
	bl 0x02009f8c
	movs	r0, #1
	bl 0x02009e9c
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #6
	bl 0x0200a04c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #9
	mov	r1, r8
	bl 0x0200a044
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200a04c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #0
	movs	r0, #7
	bl 0x0200a024
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x02009f6c
	cmp	r0, #0
	bne.n	.L_020004ec
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #9
	bl 0x0200a04c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	ldr	r2, [r5, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200051a
.L_020004ec:
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #20
	movs	r0, #9
	bl 0x0200a04c
	ldr	r2, [r5, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #9
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200a034
.L_0200051a:
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200a04c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #6
	bl 0x0200a04c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	bl 0x0200a044
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200a04c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200a044
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #132
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200a04c
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #129
	movs	r0, #6
	lsls	r1, r1, #1
	bl 0x0200a054
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200a044
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #6
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a03c
	ldr	r5, [pc, #184]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	movs	r1, #192
	ldr	r0, [r3, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #7
	bl 0x0200a04c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200a04c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r2, #10
	movs	r0, #6
	movs	r1, #4
	bl 0x02009ff4
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #5
	movs	r1, #2
	movs	r2, #10
	bl 0x02009ff4
	movs	r2, #10
	movs	r0, #5
	movs	r1, #4
	bl 0x02009ff4
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #9
	movs	r1, #3
	bl 0x02009fdc
	movs	r1, #0
	movs	r0, #9
	bl 0x0200a034
	bl 0x0200a09c
	bl 0x0200a0a4
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r2, r5, r3
	movs	r3, #7
	strb	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	movs	r0, #128
	adds	r5, r5, r2
	movs	r3, #1
	lsls	r0, r0, #4
	strh	r3, [r5, #0]
	adds	r0, #222
	bl 0x02009eec
	movs	r0, #10
	bl 0x0200a07c
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x022e0000
	.4byte 0x01590000
	.4byte 0x02160000
	.4byte 0x02220000
	.4byte 0x0200a18c
	.4byte 0x0200a1e8
	.4byte 0x0200a21c
	.4byte 0x0200a4b4
	.4byte 0x02008259
	.4byte 0x0200a4b0
	.4byte 0x02008289
	.4byte 0x021e0000
	.2byte 0x216b
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	bl 0x02009f5c
	movs	r0, #0
	bl 0x0200a0ac
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200a064
	bl 0x0200a074
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r1, #192
	movs	r0, #229
	movs	r2, #204
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200a064
	ldr	r0, [pc, #1004]
	bl 0x0200a01c
	movs	r1, #235
	movs	r2, #196
	movs	r0, #4
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x02009fcc
	movs	r2, #24
	movs	r3, #128
	movs	r0, #6
	movs	r1, #0
	negs	r2, r2
	lsls	r3, r3, #7
	bl 0x0200a0b4
	movs	r1, #16
	movs	r2, #16
	movs	r3, #128
	movs	r0, #5
	negs	r1, r1
	negs	r2, r2
	lsls	r3, r3, #6
	bl 0x0200a0b4
	movs	r1, #24
	movs	r3, #240
	movs	r0, #9
	negs	r1, r1
	movs	r2, #8
	lsls	r3, r3, #8
	bl 0x0200a0b4
	movs	r1, #4
	movs	r3, #224
	movs	r0, #7
	negs	r1, r1
	movs	r2, #24
	lsls	r3, r3, #8
	bl 0x0200a0b4
	movs	r3, #7
	movs	r2, #3
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #53
	movs	r2, #67
	movs	r3, #16
	movs	r0, #60
	bl 0x02009f2c
	bl 0x02009f1c
	movs	r0, #1
	bl 0x02009e9c
	bl 0x0200a094
	bl 0x0200a0a4
	ldr	r3, [pc, #884]
	movs	r1, #144
	str	r5, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #880]
	bl 0x02009ea4
	ldr	r3, [pc, #876]
	movs	r1, #144
	str	r5, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #872]
	bl 0x02009ea4
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200a04c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #6
	bl 0x0200a044
	movs	r0, #10
	bl 0x02009f54
	movs	r2, #5
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a02c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200a044
	movs	r0, #10
	bl 0x02009f54
	movs	r0, #5
	movs	r1, #4
	bl 0x02009fe4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a02c
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x0200a04c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x0200a044
	movs	r0, #10
	bl 0x02009f54
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200a044
	movs	r0, #10
	bl 0x02009f54
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a02c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200a03c
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #5
	bl 0x0200a044
	movs	r0, #30
	bl 0x02009f54
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200a044
	movs	r0, #10
	bl 0x02009f54
	movs	r2, #5
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a02c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200a044
	movs	r1, #3
	movs	r0, #6
	bl 0x02009fe4
	movs	r0, #10
	bl 0x02009f54
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200a044
	movs	r0, #6
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a02c
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #7
	bl 0x0200a04c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200a044
	movs	r0, #10
	bl 0x02009f54
	movs	r2, #5
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a02c
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #6
	bl 0x0200a044
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x0200a04c
	movs	r2, #5
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a02c
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200a044
	movs	r0, #10
	bl 0x02009f54
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #7
	bl 0x0200a04c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a02c
	movs	r1, #5
	movs	r2, #0
	movs	r0, #6
	bl 0x0200a014
	movs	r0, #30
	bl 0x02009f54
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200a03c
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #5
	bl 0x0200a044
	movs	r0, #10
	bl 0x02009f54
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200a04c
	movs	r2, #5
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a02c
	movs	r1, #2
	movs	r0, #5
	bl 0x0200a004
	movs	r0, #5
	bl 0x02009f54
	movs	r2, #5
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a02c
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r1, #3
	movs	r0, #7
	bl 0x02009fe4
	movs	r0, #10
	bl 0x02009f54
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a02c
	movs	r2, #5
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a02c
	movs	r1, #3
	movs	r0, #7
	bl 0x02009fe4
	movs	r0, #10
	bl 0x02009f54
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a02c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x0200a04c
	movs	r2, #5
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a02c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x0200a044
	movs	r0, #10
	bl 0x02009f54
	movs	r0, #9
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a02c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #4
	lsls	r1, r1, #7
	bl 0x0200a03c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x0200a044
	movs	r0, #10
	bl 0x02009f54
	movs	r2, #5
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a02c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x0200a044
	movs	r0, #10
	bl 0x02009f54
	movs	r0, #9
	movs	r1, #4
	bl 0x02009fe4
	movs	r2, #5
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a02c
	movs	r1, #3
	movs	r0, #4
	bl 0x02009fe4
	movs	r0, #10
	bl 0x02009f54
	movs	r0, #9
	movs	r1, #7
	movs	r2, #0
	bl 0x0200a014
	movs	r2, #0
	movs	r0, #6
	movs	r1, #5
	bl 0x0200a014
	movs	r0, #6
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #5
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #9
	movs	r1, #3
	bl 0x02009fdc
	movs	r1, #3
	movs	r0, #7
	bl 0x02009fdc
	movs	r0, #40
	bl 0x02009f54
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #184]
	adds	r2, #153
	bl 0x02009f7c
	movs	r0, #7
	movs	r1, #2
	bl 0x02009fdc
	movs	r0, #4
	bl 0x02009f74
	cmp	r0, #0
	beq.n	.L_02000a92
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x02009f9c
.L_02000a92:
	movs	r0, #7
	bl 0x02009fbc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x02009fc4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #120]
	adds	r2, #153
	bl 0x02009f7c
	movs	r0, #5
	movs	r1, #2
	bl 0x02009fdc
	movs	r0, #4
	bl 0x02009f74
	cmp	r0, #0
	beq.n	.L_02000ad0
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x02009f9c
.L_02000ad0:
	movs	r0, #5
	bl 0x02009fbc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x02009fc4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #60]
	adds	r2, #153
	bl 0x02009f7c
	movs	r0, #6
	movs	r1, #2
	bl 0x02009fdc
	movs	r0, #4
	bl 0x02009f74
	cmp	r0, #0
	beq.n	.L_02000b28
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x02009f9c
	b.n	.L_02000b28
	.4byte 0x000026b2
	.4byte 0x0200a4b4
	.4byte 0x02008259
	.4byte 0x0200a4b0
	.4byte 0x02008289
	.2byte 0x3333
	.2byte 0x0001
.L_02000b28:
	movs	r0, #6
	bl 0x02009fbc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x02009fc4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #9
	ldr	r1, [pc, #64]
	adds	r2, #153
	bl 0x02009f7c
	movs	r0, #9
	movs	r1, #2
	bl 0x02009fdc
	movs	r0, #4
	bl 0x02009f74
	cmp	r0, #0
	beq.n	.L_02000b66
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #9
	bl 0x02009f9c
.L_02000b66:
	movs	r0, #9
	bl 0x02009fbc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #9
	bl 0x02009fc4
	movs	r0, #1
	bl 0x0200a07c
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, lr}
	ldr	r5, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x02009f74
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r1, #144
	strb	r3, [r0, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	bl 0x02009ea4
	movs	r1, #144
	ldr	r0, [pc, #48]
	lsls	r1, r1, #3
	bl 0x02009ea4
	movs	r3, #241
	lsls	r3, r3, #1
	adds	r5, r5, r3
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #10
	bne.n	.L_02000bc4
	bl 0x020082cc
.L_02000bc4:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #20
	bne.n	.L_02000bd0
	bl 0x020086e4
.L_02000bd0:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02008189
	.2byte 0x81cd
	.2byte 0x0200
	.global Func_02000be0
	.thumb_func
Func_02000be0:
	push	{r5, lr}
	movs	r0, #192
	lsls	r0, r0, #18
	ldr	r1, [r0, #32]
	movs	r3, #13
	ldrb	r2, [r1, #23]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	ldr	r5, [pc, #116]
	strb	r3, [r1, #23]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L_02000c0e
	bl 0x02008b84
	b.n	.L_02000c66
.L_02000c0e:
	ldr	r3, [r0, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	bl 0x02009f74
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	movs	r0, #8
	bl 0x02009fdc
	ldr	r0, [pc, #56]
	bl 0x0200a0c4
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	bl 0x02009ea4
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #99
	bne.n	.L_02000c5c
	bl 0x02008210
	b.n	.L_02000c66
.L_02000c5c:
	movs	r1, #144
	ldr	r0, [pc, #28]
	lsls	r1, r1, #3
	bl 0x02009ea4
.L_02000c66:
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000002a
	.4byte 0x0200a0dc
	.4byte 0x02008189
	.2byte 0x81cd
	.2byte 0x0200
	.global Func_02000c80
	.thumb_func
Func_02000c80:
	push	{lr}
	ldr	r3, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_02000cd0
	movs	r3, #18
	movs	r2, #10
	str	r3, [sp, #0]
	movs	r0, #2
	movs	r1, #2
	movs	r3, #24
	str	r2, [sp, #4]
	bl 0x02009f2c
	movs	r3, #3
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #62
	movs	r2, #26
	movs	r3, #62
	bl 0x02009f2c
	movs	r3, #6
	movs	r2, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #50
	movs	r1, #65
	movs	r2, #25
	movs	r3, #61
	bl 0x02009f2c
.L_02000cd0:
	movs	r0, #0
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x002a
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009ee4
	cmp	r0, #0
	beq.n	.L_02000cfa
	movs	r0, #152
	lsls	r0, r0, #4
	bl 0x02009eec
	b.n	.L_02001048
.L_02000cfa:
	bl 0x02009f5c
	movs	r0, #0
	bl 0x0200a0ac
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #4
	adds	r1, #204
	adds	r2, #102
	bl 0x02009f7c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #136
	movs	r0, #4
	bl 0x02009fac
	ldr	r6, [pc, #808]
	adds	r0, r6, #0
	bl 0x0200a01c
	movs	r2, #16
	movs	r3, #128
	movs	r0, #5
	movs	r1, #16
	negs	r2, r2
	lsls	r3, r3, #7
	bl 0x0200a0b4
	movs	r2, #16
	movs	r3, #128
	movs	r0, #9
	movs	r1, #32
	negs	r2, r2
	lsls	r3, r3, #7
	bl 0x0200a0b4
	movs	r1, #16
	movs	r2, #16
	movs	r3, #128
	lsls	r3, r3, #7
	negs	r1, r1
	negs	r2, r2
	movs	r0, #6
	bl 0x0200a0b4
	movs	r0, #80
	bl 0x02009f54
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #9
	bl 0x0200a04c
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #9
	ldr	r1, [pc, #728]
	adds	r2, #204
	bl 0x02009f7c
	movs	r1, #150
	lsls	r1, r1, #1
	movs	r2, #152
	movs	r0, #9
	bl 0x02009fac
	movs	r0, #10
	bl 0x02009f54
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200a03c
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200a03c
	movs	r1, #160
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200a03c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #660]
	adds	r1, #153
	bl 0x0200a05c
	movs	r0, #200
	movs	r1, #1
	movs	r2, #172
	lsls	r2, r2, #17
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200a064
	bl 0x0200a06c
	movs	r0, #20
	bl 0x02009f54
	movs	r1, #152
	lsls	r1, r1, #6
	ldr	r0, [pc, #624]
	adds	r1, #102
	bl 0x0200a05c
	movs	r0, #150
	movs	r1, #1
	movs	r2, #172
	lsls	r2, r2, #17
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x0200a064
	bl 0x0200a06c
	movs	r0, #40
	bl 0x02009f54
	movs	r1, #204
	lsls	r1, r1, #8
	ldr	r0, [pc, #588]
	adds	r1, #204
	bl 0x0200a05c
	movs	r0, #132
	movs	r1, #1
	movs	r2, #128
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x0200a064
	bl 0x0200a06c
	movs	r0, #5
	movs	r1, #4
	bl 0x02009fe4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a044
	movs	r0, #6
	movs	r1, #4
	bl 0x02009fdc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #176
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
.L_02000e68:
	movs	r0, #9
	bl 0x0200a04c
	movs	r1, #0
	movs	r0, #9
	bl 0x0200a024
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a03c
	ldr	r5, [pc, #476]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	ldr	r0, [r5, #0]
.L_02000e8e:
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a03c
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x02009f6c
	ldr	r5, [r5, #0]
	cmp	r0, #0
	bne.n	.L_02000ece
	adds	r0, r5, #0
	bl 0x02009fec
	movs	r0, #9
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000ee2
.L_02000ece:
	adds	r0, r5, #0
	bl 0x02009fec
	adds	r0, r6, #5
	bl 0x0200a01c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
.L_02000ee2:
	movs	r0, #5
	movs	r1, #4
	bl 0x02009fdc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a044
	movs	r0, #6
	movs	r1, #4
	bl 0x02009fdc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #9
	movs	r1, #4
	bl 0x02009fdc
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #204
	adds	r2, #102
	movs	r0, #9
	bl 0x02009f7c
	movs	r0, #9
	bl 0x02009f74
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #144
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	movs	r2, #144
	movs	r0, #9
	bl 0x02009fac
	movs	r0, #1
	bl 0x02009f54
	movs	r0, #9
	bl 0x02009f74
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #6
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #5
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #4
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #9
	movs	r1, #3
	bl 0x02009fe4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #9
	ldr	r1, [pc, #192]
	adds	r2, #153
	bl 0x02009f7c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #180]
	adds	r2, #153
	bl 0x02009f7c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #164]
	adds	r2, #153
	bl 0x02009f7c
	movs	r0, #9
	movs	r1, #2
	bl 0x02009fdc
	movs	r0, #4
	bl 0x02009f74
	cmp	r0, #0
	beq.n	.L_02000fd8
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #9
	bl 0x02009f9c
.L_02000fd8:
	movs	r0, #5
	movs	r1, #2
	bl 0x02009fdc
	movs	r0, #4
	bl 0x02009f74
	cmp	r0, #0
	beq.n	.L_02000ff8
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x02009f9c
.L_02000ff8:
	movs	r0, #6
	movs	r1, #2
	bl 0x02009fdc
	movs	r0, #4
	bl 0x02009f74
	cmp	r0, #0
	beq.n	.L_02001018
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x02009f9c
.L_02001018:
	movs	r0, #6
	bl 0x02009fbc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x02009fc4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x02009fc4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x02009fc4
	bl 0x02009f64
	movs	r0, #152
	lsls	r0, r0, #4
	bl 0x02009eec
.L_02001048:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00001846
	.4byte 0x00019999
	.4byte 0x0004cccc
	.4byte 0x00013333
	.4byte 0x00066666
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200a0cc
	pop	{pc}
	.2byte 0x0000
	.2byte 0xa0dc
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x02009ee4
	cmp	r0, #0
	bne.n	.L_0200116c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #136
	bl 0x02009ee4
	cmp	r0, #0
	bne.n	.L_0200116c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x02009eec
	movs	r0, #137
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009ee4
	cmp	r0, #0
	beq.n	.L_0200114c
	bl 0x02009f5c
	movs	r0, #0
	bl 0x0200a0ac
	ldr	r5, [pc, #184]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x02009fd4
	movs	r0, #1
	bl 0x02009e9c
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
	bl 0x02009f7c
	movs	r1, #208
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r0, #9
	lsls	r1, r1, #1
	bl 0x02009fac
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x0200a044
	ldr	r0, [pc, #116]
	bl 0x0200a01c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #9
	movs	r1, #2
	bl 0x02009fdc
	ldr	r0, [r5, #0]
	bl 0x02009f74
	cmp	r0, #0
	beq.n	.L_0200112c
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #9
	bl 0x02009f9c
.L_0200112c:
	movs	r0, #9
	bl 0x02009fbc
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x02009fc4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #136
	bl 0x02009eec
	bl 0x02009f64
	b.n	.L_0200116c
.L_0200114c:
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x02009ee4
	cmp	r0, #0
	beq.n	.L_02001164
	movs	r0, #137
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009eec
	b.n	.L_0200116c
.L_02001164:
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x02009eec
.L_0200116c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x185e
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x02009ef4
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	movs	r3, #180
	movs	r7, #128
	movs	r6, #212
	mov	r5, sp
	lsls	r3, r3, #17
	lsls	r7, r7, #15
	lsls	r6, r6, #17
	str	r3, [r5, #0]
	str	r7, [r5, #4]
	str	r6, [r5, #8]
	mov	r8, r3
	bl 0x02009eb4
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, r5, #0
	lsls	r0, r0, #14
	bl 0x02009ec4
	movs	r0, #128
	lsls	r0, r0, #2
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r0, #162
	bl 0x02009f0c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001202
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #52]
	movs	r1, #0
	bl 0x02009f3c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009efc
	adds	r0, r5, #0
	mov	r1, r8
	adds	r2, r7, #0
	adds	r3, r6, #0
	bl 0x02009f24
	ldr	r1, [pc, #16]
	adds	r0, r5, #0
	bl 0x02009f04
.L_02001202:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xa47c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #234
	movs	r1, #180
	movs	r2, #128
	movs	r3, #212
	adds	r0, #255
	lsls	r1, r1, #17
	lsls	r2, r2, #15
	lsls	r3, r3, #17
	bl 0x02009f0c
	adds	r6, r0, #0
	movs	r7, #0
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L_02001288
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
	bl 0x02009ecc
	adds	r7, r0, #0
	movs	r0, #242
	bl 0x02009f4c
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r2, r7, r3
	movs	r1, #128
	ldrb	r0, [r5, #16]
	bl 0x02009edc
	movs	r0, #68
	bl 0x02009ed4
	adds	r0, r6, #0
.L_02001288:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #133
	sub	sp, #8
	bl 0x02009ee4
	cmp	r0, #0
	beq.n	.L_020012a0
	b.n	.L_020014a8
.L_020012a0:
	movs	r0, #7
	bl 0x02009ee4
	cmp	r0, #0
	bne.n	.L_020012ac
	b.n	.L_020014a8
.L_020012ac:
	bl 0x02009f5c
	movs	r0, #0
.L_020012b2:
	bl 0x0200a0ac
	ldr	r3, [pc, #500]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r1, [r6, #0]
	movs	r0, #7
	bl 0x02009fd4
	movs	r0, #1
	bl 0x02009e9c
	ldr	r0, [r6, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a03c
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #464]
	adds	r2, #204
	bl 0x02009f7c
	movs	r1, #206
	movs	r2, #212
	lsls	r2, r2, #1
	movs	r0, #7
	lsls	r1, r1, #1
	bl 0x02009fac
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200a044
	ldr	r0, [pc, #436]
	bl 0x0200a01c
	movs	r2, #10
	movs	r0, #7
	movs	r1, #4
	bl 0x02009ff4
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #193
	movs	r2, #212
	movs	r0, #7
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x02009fac
	movs	r2, #0
	ldr	r1, [r6, #0]
	movs	r0, #7
	bl 0x0200a00c
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	ldr	r0, [r6, #0]
	adds	r1, #204
	bl 0x02009f7c
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #340]
	bl 0x02009f84
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #7
	adds	r1, #204
	adds	r2, #102
	bl 0x02009f7c
	movs	r1, #180
	movs	r2, #212
	lsls	r2, r2, #1
	movs	r0, #7
	lsls	r1, r1, #1
	bl 0x02009fac
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200a044
	movs	r0, #182
	bl 0x0200a0d4
	bl 0x02009210
	adds	r7, r0, #0
	movs	r0, #20
	bl 0x02009f54
	movs	r0, #220
	bl 0x0200a0d4
	ldr	r5, [pc, #272]
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x02009ea4
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200a084
	movs	r0, #20
	bl 0x0200a08c
	movs	r0, #20
	bl 0x02009f54
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #6
	bl 0x0200a084
	movs	r0, #20
	bl 0x0200a08c
	movs	r0, #20
	bl 0x02009f54
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200a084
	movs	r0, #20
	bl 0x0200a08c
	movs	r0, #20
	bl 0x02009f54
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #51
	movs	r1, #38
	movs	r2, #70
	movs	r3, #22
	bl 0x02009f2c
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009eec
	adds	r0, r5, #0
	bl 0x02009eac
	cmp	r7, #0
	beq.n	.L_0200142c
	adds	r0, r7, #0
	bl 0x02009f14
.L_0200142c:
	movs	r0, #20
	bl 0x02009f54
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200a084
	movs	r0, #20
	bl 0x0200a08c
	movs	r0, #20
	bl 0x02009f54
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x0200a044
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #7
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #7
	movs	r1, #2
	bl 0x02009fdc
	ldr	r0, [r6, #0]
	bl 0x02009f74
	cmp	r0, #0
	beq.n	.L_0200148a
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x02009f9c
.L_0200148a:
	movs	r0, #7
	bl 0x02009fbc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x02009fc4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #133
	bl 0x02009eec
	bl 0x02009f64
.L_020014a8:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x00019999
	.4byte 0x00002163
	.4byte 0x0200a120
	.2byte 0x9189
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009ee4
	cmp	r0, #0
	bne.n	.L_020014d4
	bl 0x02009d50
.L_020014d4:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #137
	bl 0x02009ee4
	cmp	r0, #0
	beq.n	.L_020014e6
	bl 0x02009d50
.L_020014e6:
	bl 0x02009f5c
	movs	r0, #0
	bl 0x0200a0ac
	ldr	r0, [pc, #636]
	bl 0x0200a01c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #176
	movs	r0, #4
	bl 0x02009fa4
	movs	r0, #4
	bl 0x02009fbc
	movs	r3, #128
	movs	r2, #8
	lsls	r3, r3, #7
	movs	r0, #7
	movs	r1, #16
	bl 0x0200a0b4
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200a05c
	movs	r0, #144
	movs	r1, #1
	movs	r2, #208
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200a064
	movs	r3, #128
	movs	r0, #9
	movs	r1, #32
	movs	r2, #0
	lsls	r3, r3, #7
	bl 0x0200a0b4
	movs	r2, #8
	movs	r3, #128
	movs	r0, #6
	movs	r1, #48
	negs	r2, r2
	lsls	r3, r3, #7
	bl 0x0200a0b4
	movs	r1, #16
	movs	r2, #8
	movs	r3, #128
	lsls	r3, r3, #7
	negs	r1, r1
	negs	r2, r2
	movs	r0, #5
	bl 0x0200a0b4
	movs	r0, #40
	bl 0x02009f54
	movs	r0, #7
	ldr	r1, [pc, #512]
	ldr	r2, [pc, #516]
	bl 0x02009f7c
	movs	r1, #140
	lsls	r1, r1, #1
	movs	r2, #224
	movs	r0, #7
	bl 0x02009fac
	movs	r0, #20
	bl 0x02009f54
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a04c
	movs	r0, #7
	movs	r1, #4
	movs	r2, #10
	bl 0x02009ff4
	movs	r2, #20
	movs	r0, #7
	movs	r1, #4
	bl 0x02009ff4
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #9
	bl 0x0200a04c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200a05c
	movs	r0, #144
	movs	r1, #1
	movs	r2, #184
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200a064
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #7
	adds	r1, #204
	adds	r2, #102
	bl 0x02009f7c
	movs	r2, #16
	negs	r2, r2
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a0bc
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #7
	movs	r1, #3
	bl 0x02009fe4
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #129
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200a04c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200a044
	movs	r0, #6
	movs	r1, #4
	bl 0x02009fdc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #2
	adds	r1, #255
	movs	r2, #20
	movs	r0, #7
	bl 0x0200a04c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #5
	bl 0x0200a04c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #7
	bl 0x0200a04c
	movs	r0, #6
	movs	r1, #4
	bl 0x02009fdc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #224
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a03c
	movs	r0, #5
	movs	r1, #3
	bl 0x02009fe4
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200a054
	movs	r0, #20
	bl 0x02009f54
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #5
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #6
	movs	r1, #3
	bl 0x02009fe4
	movs	r0, #9
	movs	r1, #4
	bl 0x02009fdc
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #224
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a03c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r1, #0
	movs	r0, #9
	bl 0x0200a024
	ldr	r3, [pc, #88]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a03c
	movs	r0, #4
	movs	r1, #0
	bl 0x02009f6c
	cmp	r0, #0
	bne.n	.L_02001780
	movs	r1, #8
	movs	r2, #20
	adds	r1, #255
	movs	r0, #5
	bl 0x0200a04c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a034
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_020017ca
	.4byte 0x00002132
	.4byte 0x00026666
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
.L_02001780:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
.L_0200178e:
	movs	r1, #160
	adds	r3, #2
	strh	r3, [r2, #0]
	movs	r0, #7
	movs	r2, #0
	lsls	r1, r1, #8
	bl 0x0200a03c
	movs	r0, #5
	movs	r1, #3
	bl 0x02009fe4
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a03c
	movs	r0, #6
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a034
.L_020017ca:
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #9
	bl 0x0200a04c
	movs	r1, #224
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #144
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #7
	movs	r1, #4
	bl 0x02009fdc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	ldr	r5, [pc, #1020]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a04c
	movs	r1, #2
	movs	r2, #40
	adds	r1, #255
	movs	r0, #6
	bl 0x0200a04c
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #9
	bl 0x0200a04c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200a03c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #6
	bl 0x0200a03c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200a044
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #7
	bl 0x0200a04c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200a054
	movs	r0, #20
	bl 0x02009f54
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200a04c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a04c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #6
	bl 0x0200a04c
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #9
	bl 0x0200a04c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a04c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200a04c
	movs	r2, #20
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a03c
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200a044
	movs	r1, #128
	movs	r2, #40
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200a03c
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200a044
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #7
	movs	r1, #3
	bl 0x02009fe4
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200a04c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200a004
	movs	r2, #0
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200a03c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200a054
	movs	r0, #20
	bl 0x02009f54
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #9
	movs	r1, #2
	bl 0x02009ffc
	movs	r0, #9
	movs	r1, #3
	bl 0x02009fe4
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #7
	bl 0x0200a04c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200a044
	movs	r0, #9
	movs	r1, #2
	bl 0x02009ffc
	movs	r1, #6
	movs	r2, #40
	adds	r1, #255
	movs	r0, #9
	bl 0x0200a04c
	movs	r0, #9
	movs	r1, #3
	bl 0x02009fe4
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #7
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #6
	bl 0x0200a04c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #4
	bl 0x0200a04c
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #7
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #5
	bl 0x0200a04c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #7
	movs	r1, #4
	bl 0x02009fdc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #5
	movs	r1, #3
	bl 0x02009fe4
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #9
	movs	r1, #3
	bl 0x02009fe4
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200a04c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a03c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #6
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a034
	movs	r0, #5
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #16
	negs	r1, r1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a0bc
	movs	r0, #40
	bl 0x02009f54
	movs	r1, #16
	movs	r0, #7
	negs	r1, r1
	movs	r2, #0
	bl 0x0200a0bc
	movs	r1, #6
	adds	r1, #255
	movs	r2, #20
	movs	r0, #7
	bl 0x0200a04c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #5
	bl 0x0200a04c
	movs	r2, #16
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a0bc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a03c
	movs	r1, #160
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200a03c
	movs	r0, #7
	movs	r1, #3
	bl 0x02009fe4
	movs	r2, #20
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a02c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r2, #0
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200a03c
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #9
	movs	r1, #3
	bl 0x02009fdc
	movs	r1, #0
	b.n	.L_02001c08
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
.L_02001c08:
	movs	r0, #9
	bl 0x0200a024
	movs	r0, #4
	movs	r1, #0
	bl 0x02009f6c
	cmp	r0, #1
	bne.n	.L_02001c3e
	movs	r0, #9
	movs	r1, #4
	bl 0x02009fe4
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001c60
.L_02001c3e:
	movs	r0, #9
	movs	r1, #3
	bl 0x02009fe4
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
	bl 0x0200a034
.L_02001c60:
	ldr	r5, [pc, #240]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #128
	ldr	r0, [r5, #0]
	movs	r2, #0
	lsls	r1, r1, #7
	bl 0x0200a03c
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200a044
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200a04c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a034
	movs	r1, #132
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200a04c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a034
	movs	r2, #0
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200a03c
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200a044
	movs	r0, #9
	movs	r1, #3
	bl 0x02009fe4
	movs	r0, #5
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #6
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #4
	movs	r1, #3
	bl 0x02009fdc
	movs	r0, #7
	movs	r1, #3
	bl 0x02009fe4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #108]
	adds	r2, #153
	bl 0x02009f7c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #9
	ldr	r1, [pc, #96]
	adds	r2, #153
	bl 0x02009f7c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #80]
	adds	r2, #153
	bl 0x02009f7c
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #7
	ldr	r1, [pc, #64]
	bl 0x02009f7c
	ldr	r5, [pc, #64]
	movs	r0, #5
	adds	r1, r5, #0
	bl 0x02009f84
	movs	r0, #9
	adds	r1, r5, #0
	bl 0x02009f84
	movs	r0, #6
	adds	r1, r5, #0
	bl 0x02009f84
	adds	r1, r5, #0
	movs	r0, #7
	bl 0x02009f94
	movs	r0, #20
	bl 0x02009f54
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #129
	bl 0x02009eec
	bl 0x02009f64
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00013333
	.2byte 0xa0e4
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02001d96
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	movs	r0, #14
	bl 0x02009f0c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001d96
	movs	r1, #0
	bl 0x02009f3c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02009efc
	ldr	r1, [pc, #12]
	adds	r0, r5, #0
	bl 0x02009f04
.L_02001d96:
	pop	{r5, pc}
	.4byte 0x0300122c
	.2byte 0xa498
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #7
	bl 0x02009ee4
	cmp	r0, #0
	beq.n	.L_02001e8e
	bl 0x02009f5c
	movs	r0, #0
	bl 0x0200a0ac
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #204
	adds	r2, #102
	movs	r0, #4
	bl 0x02009f7c
	movs	r0, #7
	bl 0x02009f74
	ldr	r6, [pc, #192]
	movs	r3, #128
	movs	r2, #0
	lsls	r3, r3, #8
	movs	r1, #16
	str	r6, [r0, #108]
	movs	r0, #7
	bl 0x0200a0b4
	movs	r0, #7
	bl 0x02009f74
	movs	r1, #0
	bl 0x02009f3c
	ldr	r5, [pc, #164]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #0
	movs	r1, #0
	ldr	r0, [r5, #0]
	bl 0x0200a03c
	ldr	r0, [pc, #152]
	bl 0x0200a01c
	movs	r0, #7
	bl 0x02009fbc
	movs	r0, #7
	bl 0x02009f74
	movs	r7, #0
	str	r7, [r0, #108]
	movs	r1, #0
	movs	r0, #7
	bl 0x0200a034
	movs	r1, #3
	movs	r0, #4
	bl 0x02009fe4
	ldr	r0, [r5, #0]
	bl 0x02009f74
	movs	r1, #0
	movs	r2, #16
	str	r6, [r0, #108]
	movs	r0, #4
	bl 0x0200a0bc
	ldr	r0, [r5, #0]
	bl 0x02009f74
	str	r7, [r0, #108]
	movs	r0, #7
	bl 0x02009f74
	movs	r1, #2
	str	r6, [r0, #108]
	movs	r0, #7
	bl 0x02009fdc
	movs	r0, #4
	bl 0x02009f74
	cmp	r0, #0
	beq.n	.L_02001e66
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x02009f9c
.L_02001e66:
	movs	r0, #7
	bl 0x02009fbc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #7
	bl 0x02009fc4
	movs	r0, #7
	bl 0x02009f74
	str	r7, [r0, #108]
	movs	r0, #7
	bl 0x02009f74
	movs	r1, #1
	bl 0x02009f3c
	bl 0x02009f64
.L_02001e8e:
	pop	{r5, r6, r7, pc}
	.4byte 0x02009d61
	.4byte 0x02000240
	.4byte 0x00002162
	.section .rodata,"a",%progbits
	.4byte 0x02000008
	.4byte 0x0000ffff
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01070000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
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
	.4byte 0x0000002b
	.4byte 0x00101028
	.4byte 0x00144002
	.4byte 0x00208002
	.4byte 0x0000002a
	.4byte 0x0012a002
	.4byte 0x00208002
	.4byte 0x00a47002
	.4byte 0x000001ff
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
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
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008079
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x0980000a
	.4byte 0x02008ce1
	.4byte 0x00000002
	.4byte 0x0981000b
	.4byte 0x020094c1
	.4byte 0x00000002
	.4byte 0x0989000c
	.4byte 0x02009da1
	.4byte 0x00000002
	.4byte 0x09850001
	.4byte 0x0200928d
	.4byte 0x00000003
	.4byte 0x0985000e
	.4byte 0x02008169
	.4byte 0x00000002
	.4byte 0x0985000f
	.4byte 0x02009075
	.4byte 0x00000002
	.4byte 0x09850010
	.4byte 0x02009179
	.4byte 0x00001815
	.4byte 0x02000008
	.4byte 0x02009065
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
