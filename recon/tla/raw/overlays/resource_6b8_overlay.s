.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9204
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
	.2byte 0x92f4
	.2byte 0x0200
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr	r0, [pc, #0]
	bx	lr
	.4byte 0x02009300
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	mov	r1, sp
	add	r0, sp, #4
	bl 0x02008f34
	add	sp, #8
	pop	{pc}
	push	{lr}
	movs	r0, #0
	bl 0x02008e04
	pop	{pc}
	.2byte 0x0000
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x02008dfc
	adds	r0, r5, #0
	movs	r1, #5
	movs	r2, #0
	movs	r3, #34
	bl 0x02008dd4
	b.n	.L_02000094
.L_0200008e:
	movs	r0, #1
	bl 0x02008d5c
.L_02000094:
	bl 0x02008de4
	cmp	r0, #0
	beq.n	.L_0200008e
	movs	r0, #1
	bl 0x02008d5c
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r7, r1, #0
	movs	r0, #206
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x02008dec
	movs	r6, #0
	mov	r8, r0
	cmp	r6, r7
	bge.n	.L_02000128
.L_020000c2:
	movs	r0, #1
	movs	r1, #1
	bl 0x02008df4
	movs	r0, #5
	movs	r1, #2
	bl 0x02008df4
	movs	r0, #241
	lsls	r0, r0, #9
	adds	r0, #64
	movs	r1, #5
	bl 0x02008df4
	adds	r0, r5, #0
	bl 0x02008078
	b.n	.L_020000f2
.L_020000e6:
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_02000122
	movs	r0, #1
	bl 0x02008d5c
.L_020000f2:
	ldr	r1, [pc, #72]
	movs	r2, #2
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000128
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000112
	ldr	r3, [r1, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000116
.L_02000112:
	adds	r5, #1
	b.n	.L_02000122
.L_02000116:
	ldr	r3, [r1, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020000e6
	subs	r5, #1
.L_02000122:
	adds	r6, #1
	cmp	r6, r7
	blt.n	.L_020000c2
.L_02000128:
	bl 0x02008dfc
	mov	r0, r8
	movs	r1, #2
	bl 0x02008dcc
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	ldr	r0, [pc, #12]
	ldr	r1, [pc, #12]
	subs	r1, r1, r0
	bl 0x020080a4
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0000124c
	.2byte 0x1277
	.2byte 0x0000
	push	{lr}
.L_0200015a:
	ldr	r0, [pc, #12]
	ldr	r1, [pc, #12]
	subs	r1, r0, r1
	bl 0x020080a4
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00001277
	.2byte 0x124c
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #12]
	ldr	r1, [pc, #12]
	ldr	r0, [pc, #16]
	subs	r1, r1, r3
	bl 0x020080a4
	pop	{pc}
	.4byte 0x0000124c
	.4byte 0x00001277
	.2byte 0x12a2
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #12]
	ldr	r1, [pc, #12]
	subs	r1, r1, r0
	bl 0x020080a4
	pop	{pc}
	.2byte 0x0000
	.4byte 0x000012d2
	.2byte 0x12fd
	.2byte 0x0000
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #1
	bl 0x02008f1c
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #2
	bl 0x02008f1c
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #3
	bl 0x02008f1c
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #24
	bl 0x02008f1c
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #0
	bl 0x02008f2c
	pop	{pc}
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	movs	r1, #1
	ldr	r0, [pc, #52]
	bl 0x02008ddc
.L_020001ee:
	movs	r0, #4
	bl 0x02008d84
	ldrb	r1, [r0, #15]
	movs	r0, #4
	adds	r1, #10
	bl 0x02008e44
	movs	r0, #5
	bl 0x02008d84
	ldrb	r1, [r0, #15]
.L_02000206:
	movs	r0, #5
	adds	r1, #10
	bl 0x02008e44
	movs	r0, #6
	bl 0x02008d84
	ldrb	r1, [r0, #15]
	movs	r0, #6
	adds	r1, #10
	bl 0x02008e44
	pop	{pc}
	.2byte 0x114b
	.2byte 0x0000
	push	{r5, lr}
	ldr	r0, [pc, #56]
	movs	r1, #1
	bl 0x02008ddc
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #52]
	movs	r5, #9
	str	r3, [r2, #16]
.L_02000236:
	movs	r1, #228
	movs	r0, #4
.L_0200023a:
	bl 0x02008e1c
	subs	r5, #1
	movs	r0, #4
	movs	r1, #229
	bl 0x02008e1c
	cmp	r5, #0
	bge.n	.L_02000236
	movs	r0, #4
	bl 0x02008e14
	movs	r0, #5
	bl 0x02008e14
	movs	r0, #6
	bl 0x02008e14
	pop	{r5, pc}
	.4byte 0x0000114d
	.4byte 0x02000240
	.2byte 0xde31
	.2byte 0x000b
	push	{r5, r6, lr}
	ldr	r0, [pc, #128]
	movs	r1, #1
	bl 0x02008ddc
	movs	r1, #100
	negs	r1, r1
	movs	r0, #4
	bl 0x02008e24
	movs	r1, #100
	negs	r1, r1
	movs	r0, #5
	bl 0x02008e24
	movs	r1, #33
	negs	r1, r1
	movs	r0, #6
	bl 0x02008e24
	movs	r1, #50
	negs	r1, r1
	movs	r0, #4
	bl 0x02008e2c
	movs	r1, #40
	negs	r1, r1
	movs	r0, #5
	bl 0x02008e2c
	movs	r1, #35
	negs	r1, r1
	movs	r0, #6
	bl 0x02008e2c
	movs	r0, #4
	bl 0x02008d84
	movs	r6, #50
	movs	r2, #160
	movs	r5, #1
	adds	r6, #255
	lsls	r2, r2, #1
	strb	r5, [r0, r6]
	adds	r0, r0, r2
	strb	r5, [r0, #0]
	movs	r0, #5
	bl 0x02008d84
	movs	r2, #152
	lsls	r2, r2, #1
	adds	r3, r0, r2
	strb	r5, [r3, #0]
	movs	r3, #2
	strb	r3, [r0, r6]
	movs	r0, #4
	bl 0x02008e14
	movs	r0, #5
	bl 0x02008e14
	movs	r0, #6
	bl 0x02008e14
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x114c
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	adds	r5, r0, #0
	lsls	r0, r6, #2
	adds	r0, r0, r6
	adds	r7, r2, #0
	lsls	r0, r0, #2
	adds	r0, r0, r7
	adds	r0, #48
	mov	r8, r3
	bl 0x02008d94
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl 0x02008e34
	mov	r3, r8
	cmp	r3, #1
	bne.n	.L_0200032a
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl 0x02008e3c
.L_0200032a:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{lr}
	ldr	r0, [pc, #268]
	movs	r1, #1
	bl 0x02008ddc
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #4
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #4
	movs	r1, #0
	movs	r2, #2
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #4
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #4
	movs	r1, #0
	movs	r2, #4
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #4
	movs	r1, #0
	movs	r2, #5
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #4
	movs	r1, #0
	movs	r2, #6
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #5
	movs	r1, #2
	movs	r2, #0
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #5
	movs	r1, #2
	movs	r2, #1
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #5
	movs	r1, #2
	movs	r2, #2
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #5
	movs	r1, #2
	movs	r2, #3
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #5
	movs	r1, #2
	movs	r2, #4
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #5
	movs	r1, #2
	movs	r2, #5
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #5
	movs	r1, #2
	movs	r2, #6
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #6
	movs	r1, #1
	movs	r2, #0
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #6
	movs	r1, #1
	movs	r2, #1
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #6
	movs	r1, #1
	movs	r2, #2
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #6
	movs	r1, #1
	movs	r2, #3
	movs	r3, #1
	bl 0x020082f4
	movs	r0, #6
	movs	r1, #1
	movs	r2, #4
	movs	r3, #1
	bl 0x020082f4
	movs	r1, #1
	movs	r2, #5
	movs	r3, #1
	movs	r0, #6
	bl 0x020082f4
	movs	r0, #4
	bl 0x02008e14
	movs	r0, #5
	bl 0x02008e14
	movs	r0, #6
	bl 0x02008e14
	pop	{pc}
	.2byte 0x0000
	.2byte 0x114e
	.2byte 0x0000
	push	{lr}
	movs	r0, #4
	bl 0x02008e64
	movs	r3, #2
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	push	{lr}
	movs	r0, #4
	bl 0x02008e64
	movs	r3, #0
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	push	{r5, lr}
	adds	r5, r1, #0
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #50
	adds	r0, r5, #0
	bl 0x02008eac
	adds	r0, r5, #0
	movs	r1, #6
	movs	r2, #15
	bl 0x02008e84
	adds	r0, r5, #0
	movs	r1, #6
	movs	r2, #15
	bl 0x02008e84
	movs	r0, #10
	bl 0x02008e54
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008d8c
	cmp	r0, #0
	bne.n	.L_020004ca
	ldr	r0, [pc, #100]
	bl 0x02008e94
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008e9c
	movs	r1, #180
	adds	r0, r5, #0
	lsls	r1, r1, #1
	movs	r2, #72
	bl 0x02008e6c
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02008ea4
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008d94
	b.n	.L_020004f8
.L_020004ca:
	ldr	r0, [pc, #56]
	bl 0x02008e94
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008e9c
	movs	r1, #196
	adds	r0, r5, #0
	lsls	r1, r1, #1
	movs	r2, #104
	bl 0x02008e6c
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02008ea4
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008d9c
.L_020004f8:
	movs	r0, #20
	bl 0x02008e54
	pop	{r5, pc}
	.4byte 0x0000156d
	.2byte 0x156e
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	movs	r1, #4
	movs	r2, #0
	bl 0x02008e8c
	movs	r0, #10
	bl 0x02008e54
	movs	r1, #4
	adds	r0, r5, #0
	adds	r1, #255
	movs	r2, #50
	bl 0x02008eac
	adds	r0, r5, #0
	movs	r1, #6
	movs	r2, #15
	bl 0x02008e84
	movs	r2, #15
	movs	r1, #6
	adds	r0, r5, #0
	bl 0x02008e84
	movs	r0, #10
	bl 0x02008e54
	ldr	r0, [pc, #20]
	bl 0x02008e94
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008e9c
	movs	r0, #10
	bl 0x02008e54
	pop	{r5, pc}
	.2byte 0x156f
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	movs	r1, #4
	movs	r2, #0
	bl 0x02008e8c
	movs	r0, #10
	bl 0x02008e54
	movs	r1, #4
	adds	r0, r5, #0
	adds	r1, #255
	movs	r2, #50
	bl 0x02008eac
	adds	r0, r5, #0
	movs	r1, #6
	movs	r2, #15
	bl 0x02008e84
	movs	r2, #15
	movs	r1, #6
	adds	r0, r5, #0
	bl 0x02008e84
	movs	r0, #10
	bl 0x02008e54
	ldr	r0, [pc, #20]
	bl 0x02008e94
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008e9c
	movs	r0, #10
	bl 0x02008e54
	pop	{r5, pc}
	.2byte 0x156f
	.2byte 0x0000
	push	{lr}
	movs	r1, #2
	movs	r0, #21
	sub	sp, #8
	bl 0x02008e7c
	movs	r0, #10
	bl 0x02008e54
	movs	r3, #25
	movs	r2, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #57
	movs	r1, #16
	movs	r2, #4
	movs	r3, #6
	bl 0x02008dac
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	cmp	r0, #1
	bne.n	.L_02000600
	movs	r0, #21
	movs	r1, #2
	bl 0x02008e7c
	movs	r3, #25
	movs	r2, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #57
	movs	r1, #16
	movs	r2, #4
	movs	r3, #6
	bl 0x02008dac
.L_02000600:
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #2
	movs	r2, #10
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #34
	movs	r1, #26
	movs	r2, #5
	movs	r3, #4
	bl 0x02008ecc
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #2
	movs	r2, #10
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r2, #5
	movs	r3, #4
.L_02000638:
	movs	r0, #34
	movs	r1, #26
	bl 0x02008ecc
	ldr	r0, [pc, #8]
	movs	r1, #6
.L_02000644:
	bl 0x02008eb4
	add	sp, #12
	pop	{pc}
	.2byte 0x0142
	.2byte 0x0000
.L_02000650:
	push	{lr}
	ldr	r0, [pc, #8]
	movs	r1, #8
	bl 0x02008eb4
	pop	{pc}
	.2byte 0x0142
	.2byte 0x0000
	push	{lr}
	movs	r0, #249
	movs	r1, #40
	movs	r2, #168
.L_02000668:
	bl 0x02008f14
	ldr	r3, [pc, #20]
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
.L_02000674:
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #14
	movs	r1, #0
	bl 0x02008ebc
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
.L_0200068c:
	movs	r0, #192
	lsls	r0, r0, #2
	sub	sp, #12
	bl 0x02008d94
	movs	r3, #1
.L_02000698:
	str	r3, [sp, #0]
	movs	r3, #10
	str	r3, [sp, #4]
	str	r3, [sp, #8]
	adds	r0, r5, #0
	movs	r1, #36
.L_020006a4:
	movs	r2, #28
	movs	r3, #1
	bl 0x0200892c
	add	sp, #12
	pop	{r5, pc}
.L_020006b0:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008d94
	adds	r0, r5, #0
	movs	r1, #18
	movs	r2, #184
	movs	r3, #184
	bl 0x02008950
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #192
	lsls	r0, r0, #2
.L_020006d4:
	adds	r0, #2
	bl 0x02008d94
	adds	r0, r5, #0
	movs	r1, #19
	movs	r2, #200
.L_020006e0:
	movs	r3, #200
	bl 0x020089c0
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r0, #0
.L_020006ec:
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	sub	sp, #4
	bl 0x02008d94
.L_020006f8:
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #216
	movs	r2, #216
	movs	r3, #14
.L_02000704:
	bl 0x02008a10
	add	sp, #4
	pop	{r5, pc}
	push	{lr}
	movs	r0, #19
.L_02000710:
	bl 0x02008e64
	movs	r2, #196
	adds	r0, #98
	ldrb	r1, [r0, #0]
	lsls	r2, r2, #2
.L_0200071c:
	movs	r0, #19
	bl 0x02008e5c
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #16
	movs	r2, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #48
	movs	r1, #26
	movs	r2, #2
	movs	r3, #4
	bl 0x02008dac
	add	sp, #8
	pop	{pc}
	.4byte 0x00004770
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x02008e64
	ldr	r1, [pc, #52]
	bl 0x02008c4c
	ldr	r5, [pc, #52]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x02008e64
	ldr	r1, [pc, #40]
	bl 0x02008c4c
	movs	r0, #100
	bl 0x02008d5c
	adds	r0, r6, #0
	bl 0x02008e64
	bl 0x02008d4c
	ldr	r0, [r5, #0]
	bl 0x02008e64
	bl 0x02008d4c
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02009860
	.4byte 0x02000240
	.2byte 0x97b0
	.2byte 0x0200
	.global Func_02000794
	.thumb_func
Func_02000794:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x94e0
	.2byte 0x0200
	.global Func_0200079c
	.thumb_func
Func_0200079c:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r1, r2, r3
	subs	r3, #172
	str	r3, [r1, #0]
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #55
	adds	r3, r2, r1
	movs	r1, #0
	strb	r1, [r3, #0]
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #54
	adds	r2, r2, r3
	strb	r1, [r2, #0]
	movs	r0, #5
	movs	r1, #1
	sub	sp, #8
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #5
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #5
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #6
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #6
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #7
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #106
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #108
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #109
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #113
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #123
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #130
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #140
	bl 0x02008e4c
	movs	r1, #1
	movs	r0, #151
	bl 0x02008e4c
	ldr	r3, [pc, #220]
	movs	r1, #139
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #15
	movs	r1, #2
	bl 0x02008e7c
	movs	r0, #16
	movs	r1, #2
	bl 0x02008e7c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x02008edc
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x02008ed4
	movs	r0, #12
	movs	r1, #0
	bl 0x02008ed4
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02008d8c
	cmp	r0, #0
	beq.n	.L_0200088e
	movs	r3, #10
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #36
	movs	r1, #28
	movs	r2, #1
	movs	r3, #1
	bl 0x02008dac
.L_0200088e:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008d8c
	cmp	r0, #0
	beq.n	.L_020008aa
	movs	r1, #184
	movs	r2, #184
	movs	r0, #18
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02008e74
.L_020008aa:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02008d8c
	cmp	r0, #0
	beq.n	.L_020008d2
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x02008d8c
	cmp	r0, #0
	bne.n	.L_020008d2
	movs	r1, #200
	movs	r2, #200
	movs	r0, #19
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02008e74
.L_020008d2:
	movs	r0, #19
	bl 0x02008e64
	movs	r1, #50
	bl 0x02008dc4
	ldr	r5, [pc, #52]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r6, r5, r2
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	cmp	r3, #6
	bne.n	.L_020008f8
	adds	r2, #50
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x02008f04
.L_020008f8:
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	cmp	r3, #8
	bne.n	.L_0200090c
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x02008f0c
.L_0200090c:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.global Func_02000918
	.thumb_func
Func_02000918:
	movs	r0, #0
	bx	lr
	push	{lr}
	bl 0x02008e0c
	pop	{pc}
	push	{lr}
	bl 0x02008f24
	pop	{pc}
	push	{lr}
	sub	sp, #8
	adds	r4, r3, #0
	cmp	r0, #1
	bne.n	.L_0200094a
	ldr	r3, [sp, #16]
	adds	r0, r1, #0
	str	r3, [sp, #0]
	ldr	r3, [sp, #20]
	adds	r1, r2, #0
	str	r3, [sp, #4]
	adds	r2, r4, #0
	ldr	r3, [sp, #12]
	bl 0x02008dac
.L_0200094a:
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r1, #0
	mov	sl, r0
	adds	r0, r6, #0
	adds	r7, r2, #0
	mov	r8, r3
	bl 0x02008e64
	mov	r2, sl
	adds	r5, r0, #0
	cmp	r2, #1
	bne.n	.L_02000980
	mov	r3, r8
	lsls	r2, r3, #16
	lsls	r1, r7, #16
	adds	r0, r6, #0
	bl 0x02008e74
	movs	r3, #0
	str	r3, [r5, #24]
	str	r3, [r5, #28]
.L_02000980:
	mov	r2, sl
	cmp	r2, #2
	bne.n	.L_020009b6
	ldr	r3, [r5, #24]
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r3, r2
	bge.n	.L_020009ae
.L_02000990:
	movs	r0, #1
	bl 0x02008d5c
	ldr	r3, [r5, #24]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #30
	adds	r3, r3, r2
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	cmp	r3, r2
	ble.n	.L_02000990
.L_020009ae:
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
.L_020009b6:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r1, #0
	mov	sl, r0
	adds	r0, r7, #0
	mov	r8, r3
	adds	r5, r2, #0
	bl 0x02008e64
	mov	r3, sl
	adds	r6, r0, #0
	cmp	r3, #2
	bne.n	.L_02000a06
	mov	r3, r8
	lsls	r2, r3, #16
	adds	r0, r7, #0
	lsls	r1, r5, #16
	bl 0x02008e74
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r2, r6, #0
	str	r3, [r6, #12]
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r3, #10
	str	r3, [r6, #72]
	movs	r0, #50
	bl 0x02008d5c
.L_02000a06:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	mov	sl, r3
	cmp	r6, #1
	bne.n	.L_02000a46
	movs	r0, #177
	lsls	r3, r2, #16
	lsls	r1, r7, #16
	lsls	r0, r0, #1
	movs	r2, #0
	bl 0x02008da4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000a46
	movs	r1, #0
	bl 0x02008db4
	adds	r2, r5, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02000a46:
	cmp	r6, #2
	bne.n	.L_02000ac2
	mov	r2, r8
	lsls	r3, r2, #16
	lsls	r1, r7, #16
	movs	r0, #252
	movs	r2, #0
	bl 0x02008da4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000ac2
	movs	r1, #0
	bl 0x02008db4
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r5, #24]
	str	r3, [r5, #28]
.L_02000a74:
	movs	r0, #1
	bl 0x02008d5c
	ldr	r2, [r5, #24]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #30
	adds	r2, r2, r3
	ldrh	r3, [r5, #6]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r3, r1
	strh	r3, [r5, #6]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	str	r2, [r5, #24]
	str	r2, [r5, #28]
	cmp	r2, r3
	ble.n	.L_02000a74
	adds	r3, #1
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldr	r3, [pc, #40]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x02008e64
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	strh	r3, [r5, #6]
	mov	r0, sl
	ldr	r1, [sp, #24]
	bl 0x02008ebc
.L_02000ac2:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
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
	adds	r7, r0, #0
	ldr	r2, [r7, #104]
	adds	r6, r7, #0
	adds	r6, #99
	mov	r8, r2
	ldrb	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	sub	sp, #24
	cmp	r3, #0
	beq.n	.L_02000b0a
	ldrb	r0, [r6, #0]
	movs	r1, #6
	lsrs	r0, r0, #1
	bl 0x02008d54
	adds	r1, r0, #0
	lsls	r1, r1, #24
	lsrs	r1, r1, #24
	adds	r0, r7, #0
	bl 0x02008dbc
.L_02000b0a:
	adds	r3, r7, #0
	adds	r3, #98
	ldrb	r5, [r3, #0]
	cmp	r5, #0
	bne.n	.L_02000b48
	ldrb	r2, [r6, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000b7e
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #86
	bl 0x02008f3c
	mov	r1, r8
	adds	r1, #166
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	r2, r8
	lsls	r3, r3, #1
	adds	r3, #160
	strh	r5, [r2, r3]
	ldr	r2, [pc, #8]
	ldrh	r3, [r1, #0]
	eors	r3, r2
	strh	r3, [r1, #0]
	b.n	.L_02000b7e
	.2byte 0x0000
	.2byte 0x0001
	.2byte 0x0000
.L_02000b48:
	cmp	r5, #1
	bne.n	.L_02000b7e
	mov	r3, r8
	adds	r3, #160
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #31
	ble.n	.L_02000b7e
	mov	r3, r8
	adds	r3, #162
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #31
	ble.n	.L_02000b7e
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x02008dbc
	mov	r3, r8
	adds	r3, #164
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x02008d6c
	movs	r3, #0
	str	r3, [r7, #108]
	b.n	.L_02000c38
.L_02000b7e:
	ldrb	r3, [r6, #0]
	movs	r2, #1
	adds	r3, #1
	strb	r3, [r6, #0]
	movs	r3, #0
	str	r3, [sp, #0]
	mov	r6, r8
	mov	fp, r2
	adds	r6, #160
.L_02000b90:
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	lsls	r0, r0, #10
	bl 0x02008d64
	str	r0, [sp, #4]
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	blt.n	.L_02000c24
	cmp	r3, #31
	bgt.n	.L_02000c24
	ldr	r3, [r7, #8]
	add	r5, sp, #12
	str	r3, [r5, #0]
	adds	r0, r5, #0
	movs	r3, #0
	ldrsh	r2, [r6, r3]
	ldr	r3, [r7, #12]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	bl 0x02008ec4
	ldr	r2, [r5, #0]
	movs	r3, #0
	str	r2, [sp, #8]
	mov	sl, r3
	ldr	r5, [r5, #8]
	mov	r9, r5
	ldr	r5, [sp, #0]
	add	r5, r8
.L_02000bd4:
	ldr	r2, [sp, #8]
	mov	r3, r9
	str	r3, [r5, #16]
	str	r2, [r5, #12]
	ldr	r2, [sp, #4]
	mov	r3, sl
	str	r2, [r5, #20]
	str	r2, [r5, #24]
	cmp	r3, #0
	bne.n	.L_02000bf4
	adds	r0, r7, #0
	bl 0x02008efc
	subs	r0, #1
	strh	r0, [r5, #30]
	b.n	.L_02000c0c
.L_02000bf4:
	adds	r0, r7, #0
	bl 0x02008efc
	ldr	r3, [r5, #16]
	ldr	r2, [pc, #72]
	adds	r0, #1
	adds	r3, r3, r2
	str	r3, [r5, #16]
	ldr	r3, [r5, #24]
	strh	r0, [r5, #30]
	negs	r3, r3
	str	r3, [r5, #24]
.L_02000c0c:
	adds	r0, r5, #0
	bl 0x02008eec
	movs	r3, #1
	add	sl, r3
	mov	r2, sl
	adds	r5, #40
	cmp	r2, #1
	ble.n	.L_02000bd4
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
.L_02000c24:
	ldr	r3, [sp, #0]
	movs	r2, #1
	negs	r2, r2
	adds	r3, #80
	add	fp, r2
	str	r3, [sp, #0]
	mov	r3, fp
	adds	r6, #2
	cmp	r3, #0
	bge.n	.L_02000b90
.L_02000c38:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	adds	r7, r1, #0
	sub	sp, #4
	bl 0x02008d7c
	movs	r1, #164
	adds	r1, r1, r7
	mov	r8, r1
	ldr	r2, [pc, #60]
	mov	r3, r8
	strh	r0, [r3, #0]
	movs	r1, #128
	lsls	r0, r0, #16
	mov	sl, r2
	lsls	r1, r1, #1
	ldr	r2, [pc, #48]
	asrs	r0, r0, #16
	bl 0x02008d74
	adds	r3, r7, #0
	movs	r2, #186
	movs	r5, #0
	adds	r3, #166
	lsls	r2, r2, #2
	strh	r5, [r3, #0]
	adds	r2, #255
	subs	r3, #6
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #6
	str	r6, [r3, #0]
	adds	r3, r6, #0
	mov	r0, sl
	adds	r3, #98
	strb	r0, [r3, #0]
	adds	r3, #1
	b.n	.L_02000cb0
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x8f44
	.2byte 0x0200
.L_02000cb0:
	strb	r0, [r3, #0]
	ldr	r3, [pc, #140]
	mov	r0, r8
	str	r3, [r6, #108]
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	ldr	r2, [pc, #132]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	str	r7, [r6, #104]
	lsrs	r3, r3, #5
	mov	fp, r3
	mov	r9, r5
	mov	sl, r5
.L_02000cce:
	movs	r1, #1
	mov	r2, sl
	mov	r8, r1
	adds	r5, r2, r7
.L_02000cd6:
	mov	r3, fp
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #16
	movs	r2, #16
	ldr	r3, [pc, #100]
	bl 0x02008ee4
	ldrb	r3, [r5, #5]
	movs	r0, #33
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r5, #9]
	adds	r0, r6, #0
	bl 0x02008ef4
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r5, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	movs	r2, #1
	lsls	r0, r0, #2
	negs	r2, r2
	orrs	r3, r0
	add	r8, r2
	strb	r3, [r5, #9]
	mov	r3, r8
	adds	r5, #40
	cmp	r3, #0
	bge.n	.L_02000cd6
	movs	r1, #1
	add	r9, r1
	movs	r0, #80
	mov	r2, r9
	add	sl, r0
	cmp	r2, #1
	ble.n	.L_02000cce
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02008ad1
	.4byte 0x020036e0
	.4byte 0x80004000
	.4byte 0x23013062
	.4byte 0x47707003
	.section .rodata,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x70000000
	.4byte 0xf7700000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x77770000
	.4byte 0xffff7770
	.4byte 0xfffffff7
	.4byte 0x7777ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00007777
	.4byte 0x0777ffff
	.4byte 0x7fffffff
	.4byte 0xffff7777
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000007
	.4byte 0x0000077f
	.4byte 0xfff70000
	.4byte 0xffff7000
	.4byte 0x7ffff700
	.4byte 0x07ffff70
	.4byte 0x007fff70
	.4byte 0x007ffff7
	.4byte 0x0007fff7
	.4byte 0x0007fff7
	.4byte 0x000077ff
	.4byte 0x00000077
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xff770000
	.4byte 0x77000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00007fff
	.4byte 0x0007ffff
	.4byte 0x007ffff7
	.4byte 0x07ffff70
	.4byte 0x07fff700
	.4byte 0x7ffff700
	.4byte 0x7fff7000
	.4byte 0x7fff7000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x40000000
	.4byte 0xf4400000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x44440000
	.4byte 0xffff4440
	.4byte 0xfffffff4
	.4byte 0x4444ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004444
	.4byte 0x0444ffff
	.4byte 0x4fffffff
	.4byte 0xffff4444
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000044f
	.4byte 0xfff40000
	.4byte 0xffff4000
	.4byte 0x4ffff400
	.4byte 0x04ffff40
	.4byte 0x004fff40
	.4byte 0x004ffff4
	.4byte 0x0004fff4
	.4byte 0x0004fff4
	.4byte 0x000044ff
	.4byte 0x00000044
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xff440000
	.4byte 0x44000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004fff
	.4byte 0x0004ffff
	.4byte 0x004ffff4
	.4byte 0x04ffff40
	.4byte 0x04fff400
	.4byte 0x4ffff400
	.4byte 0x4fff4000
	.4byte 0x4fff4000
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0xeb000000
	.4byte 0x000000eb
	.4byte 0xedeb0000
	.4byte 0x0000ebed
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xedeb0000
	.4byte 0x0000ebed
	.4byte 0xeb000000
	.4byte 0x000000eb
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0x0000e900
	.4byte 0x00e90000
	.4byte 0x00ebe9e9
	.4byte 0xe9e9eb00
	.4byte 0xedebeb00
	.4byte 0x00ebebed
	.4byte 0xefed0000
	.4byte 0x0000edef
	.4byte 0xefed0000
	.4byte 0x0000edef
	.4byte 0xedebeb00
	.4byte 0x00ebebed
	.4byte 0x00ebe9e9
	.4byte 0xe9e9eb00
	.4byte 0x0000e900
	.4byte 0x00e90000
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0xebe90000
	.4byte 0x0000e9eb
	.4byte 0xedebe900
	.4byte 0x00e9ebed
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xedebe900
	.4byte 0x00e9ebed
	.4byte 0xebe90000
	.4byte 0x0000e9eb
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0xffff0000
	.4byte 0x00000040
	.4byte 0xc0000038
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000070
	.4byte 0xc0000038
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000040
	.4byte 0x40000078
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000070
	.4byte 0xc0000078
	.4byte 0x00100000
	.4byte 0x01300010
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x00000158
	.4byte 0x00000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000058
	.4byte 0x000000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x000000b8
	.4byte 0x000000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x000001a8
	.4byte 0x00000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x000000b8
	.4byte 0x000000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000142
	.4byte 0x01402142
	.4byte 0x000001ff
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00280000
	.4byte 0x00004000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff00cd
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00280000
	.4byte 0x00004000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00520000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff00f5
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00280000
	.4byte 0x00004000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0xffff012b
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00004000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00024000
	.4byte 0xffff00fc
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00024000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x02008055
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x02008055
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x02008749
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte 0x020081a5
	.4byte 0x00008400
	.4byte 0xffff0008
	.4byte 0x020081b1
	.4byte 0x00000400
	.4byte 0xffff0008
	.4byte 0x020081bd
	.4byte 0x00004400
	.4byte 0xffff0008
	.4byte 0x020081c9
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020081e1
	.4byte 0x00004400
	.4byte 0xffff000a
	.4byte 0x020081d5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008925
	.4byte 0x0000c400
	.4byte 0xffff000b
	.4byte 0x02008141
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte 0x02008159
	.4byte 0x00000400
	.4byte 0xffff000b
	.4byte 0x02008171
	.4byte 0x00004400
	.4byte 0xffff000b
	.4byte 0x0200818d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008055
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200891d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008055
	.4byte 0x0000c400
	.4byte 0xffff000f
	.4byte 0x02008069
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008059
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0200870d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002535
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002536
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002537
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002538
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002539
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000003
	.4byte 0xffff000a
	.4byte 0x0200826d
	.4byte 0x00000003
	.4byte 0xffff000b
	.4byte 0x020081e5
	.4byte 0x00000003
	.4byte 0xffff000c
	.4byte 0x02008225
	.4byte 0x00000003
	.4byte 0xffff000d
	.4byte 0x02008331
	.4byte 0x00000013
	.4byte 0xffff0064
	.4byte 0x0020014d
	.4byte 0x00008e15
	.4byte 0xffff000e
	.4byte 0x02008075
	.4byte 0x00008e15
	.4byte 0xffff000f
	.4byte 0x02008075
	.4byte 0x00004e15
	.4byte 0xffff000f
	.4byte 0x02008075
	.4byte 0x00009415
	.4byte 0xffff000f
	.4byte 0x02008075
	.4byte 0x00001815
	.4byte 0xffff0012
	.4byte 0x02008055
	.4byte 0x50008615
	.4byte 0xffff0015
	.4byte 0x020085dd
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008445
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02008455
	.4byte 0x10009a15
	.4byte 0xffff0010
	.4byte 0x02008055
	.4byte 0x60009a15
	.4byte 0xffff0011
	.4byte 0x02008465
	.4byte 0x20009a15
	.4byte 0xffff0011
	.4byte 0x02008509
	.4byte 0x10009a15
	.4byte 0xffff0011
	.4byte 0x02008055
	.4byte 0x20009d15
	.4byte 0xffff0011
	.4byte 0x0200855d
	.4byte 0x20009d15
	.4byte 0xffff0015
	.4byte 0x020085b1
	.4byte 0x50008805
	.4byte 0x03000032
	.4byte 0x02008689
	.4byte 0x50008805
	.4byte 0x03010033
	.4byte 0x020086b1
	.4byte 0x50008805
	.4byte 0x03020034
	.4byte 0x020086cd
	.4byte 0x50008805
	.4byte 0x03030035
	.4byte 0x020086e9
	.4byte 0x50008a05
	.4byte 0xffff003c
	.4byte 0x02008725
	.4byte 0x50008905
	.4byte 0xffff0037
	.4byte 0x02008605
	.4byte 0x50008905
	.4byte 0xffff0038
	.4byte 0x02008625
	.4byte 0x50008905
	.4byte 0xffff000d
	.4byte 0x02008651
	.4byte 0x50008905
	.4byte 0xffff0039
	.4byte 0x02008661
	.4byte 0x00008715
	.4byte 0xffff000d
	.4byte 0x02008741
	.4byte 0x00008715
	.4byte 0xffff000f
	.4byte 0x02008741
	.4byte 0x00009815
	.4byte 0xffff000b
	.4byte 0x02008745
	.4byte 0x00009815
	.4byte 0xffff000f
	.4byte 0x02008745
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
