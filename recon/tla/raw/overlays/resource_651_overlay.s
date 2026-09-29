.syntax unified
	.thumb
	.global Func_02000044
	.thumb_func
Func_02000044:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_0200005c
	ldr	r0, [pc, #12]
	b.n	.L_0200005e
.L_0200005c:
	ldr	r0, [pc, #12]
.L_0200005e:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000000d
	.4byte 0x02009068
	.2byte 0x9018
	.2byte 0x0200
	.global Func_02000070
	.thumb_func
Func_02000070:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02000088
	ldr	r0, [pc, #12]
	b.n	.L_0200008a
.L_02000088:
	ldr	r0, [pc, #12]
.L_0200008a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000000d
	.4byte 0x02009238
	.2byte 0x90d0
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl 0x02008c58
	movs	r0, #0
	bl 0x02008d40
	ldr	r5, [pc, #100]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x02008c78
	movs	r1, #2
	ldr	r0, [r5, #0]
	bl 0x02008d00
	ldr	r0, [r5, #0]
	bl 0x02008c70
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #123
	bl 0x02008d78
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x02008cb0
	movs	r2, #6
	movs	r1, #2
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x02008c98
	movs	r0, #10
	bl 0x02008c50
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r6, r3
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl 0x02008d10
	bl 0x02008d20
	bl 0x02008d28
	bl 0x02008c60
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r2, #192
	movs	r1, #66
	lsls	r2, r2, #2
	bl 0x02008d38
	pop	{pc}
	.2byte 0x0000
	.global Func_02000128
	.thumb_func
Func_02000128:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02000140
	ldr	r0, [pc, #12]
	b.n	.L_02000142
.L_02000140:
	ldr	r0, [pc, #12]
.L_02000142:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000000d
	.4byte 0x02009664
	.2byte 0x9490
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #67
	bl 0x02008c38
	cmp	r0, #0
	bne.n	.L_020001be
	bl 0x02008c58
	movs	r0, #0
	bl 0x02008d40
	ldr	r5, [pc, #96]
	adds	r0, r5, #0
	bl 0x02008ce0
	movs	r1, #0
	movs	r0, #8
	bl 0x02008ce8
	bl 0x02008d60
	movs	r1, #0
	bl 0x02008c68
	cmp	r0, #0
	bne.n	.L_0200019a
	movs	r0, #10
	bl 0x02008c50
	adds	r0, r5, #1
	bl 0x02008ce0
	b.n	.L_020001a6
.L_0200019a:
	movs	r0, #20
	bl 0x02008c50
	adds	r0, r5, #2
	bl 0x02008ce0
.L_020001a6:
	movs	r0, #8
	movs	r1, #0
	bl 0x02008cf0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #67
	bl 0x02008c40
	bl 0x02008c60
	b.n	.L_020001cc
.L_020001be:
	ldr	r0, [pc, #20]
	bl 0x02008ce0
	movs	r0, #8
	movs	r1, #0
	bl 0x02008cf0
.L_020001cc:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001712
	.2byte 0x1715
	.2byte 0x0000
	.section .text.x02008288,"ax",%progbits
	.balign 4
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008c70
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020002c4
	movs	r0, #3
	adds	r1, r5, #0
	bl 0x02008d70
	b.n	.L_020002e0
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020002c4:
	bl 0x02008c58
	movs	r0, #0
	bl 0x02008d40
	ldr	r0, [pc, #20]
	bl 0x02008ce0
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008cf0
	bl 0x02008c60
.L_020002e0:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x174b
	.2byte 0x0000
	.section .text.x02008340,"ax",%progbits
	.balign 4
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008c70
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_0200037c
	movs	r0, #3
	adds	r1, r5, #0
	bl 0x02008d70
	b.n	.L_02000398
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200037c:
	bl 0x02008c58
	movs	r0, #0
	bl 0x02008d40
	ldr	r0, [pc, #20]
	bl 0x02008ce0
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008cf0
	bl 0x02008c60
.L_02000398:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1837
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #75
	bl 0x02008c38
	cmp	r0, #0
	beq.n	.L_020003b2
	b.n	.L_0200091e
.L_020003b2:
	movs	r0, #145
	lsls	r0, r0, #4
	bl 0x02008c38
	cmp	r0, #0
	beq.n	.L_020003c0
	b.n	.L_0200091e
.L_020003c0:
	movs	r0, #7
	bl 0x02008c38
	cmp	r0, #0
	beq.n	.L_020003cc
	b.n	.L_0200091e
.L_020003cc:
	bl 0x02008c58
	movs	r0, #0
	bl 0x02008d40
	ldr	r6, [pc, #640]
	adds	r0, r6, #0
	bl 0x02008ce0
	movs	r1, #236
	movs	r2, #132
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #27
	bl 0x02008ca8
	movs	r0, #30
	bl 0x02008c50
	ldr	r5, [pc, #616]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #252
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	subs	r2, #236
	bl 0x02008c90
	movs	r1, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02008cf8
	movs	r1, #236
	movs	r2, #140
	movs	r0, #27
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x02008c88
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #27
	bl 0x02008cf8
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #27
	bl 0x02008d08
	movs	r0, #60
	bl 0x02008c50
	movs	r0, #27
	movs	r1, #0
	bl 0x02008cf0
	movs	r3, #160
	movs	r0, #5
	movs	r1, #8
	movs	r2, #16
	lsls	r3, r3, #8
	bl 0x02008d48
	movs	r3, #160
	movs	r0, #6
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x02008d48
	movs	r2, #8
	movs	r3, #128
	lsls	r3, r3, #8
	negs	r2, r2
	movs	r1, #32
	movs	r0, #28
	bl 0x02008d48
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #27
	bl 0x02008cf0
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #4
	movs	r2, #0
	adds	r1, #255
	movs	r0, #5
	bl 0x02008d08
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #5
	bl 0x02008cf0
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #0
	movs	r2, #0
	movs	r0, #27
	bl 0x02008cf8
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #27
	bl 0x02008cf8
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #27
	bl 0x02008cf8
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #27
	bl 0x02008cf8
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #27
	bl 0x02008cf0
	movs	r0, #30
	bl 0x02008c50
	movs	r2, #0
	movs	r1, #28
	movs	r0, #6
	bl 0x02008cd0
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #3
	movs	r0, #6
	bl 0x02008cb0
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #6
	bl 0x02008cf0
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #28
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x02008cd8
	movs	r0, #30
	bl 0x02008c50
	movs	r0, #28
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r2, #0
	movs	r1, #27
	ldr	r0, [r5, #0]
	bl 0x02008cd0
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #28
	bl 0x02008cf0
	movs	r0, #30
	bl 0x02008c50
	movs	r2, #0
	movs	r0, #6
	movs	r1, #27
	bl 0x02008cd0
	movs	r1, #4
	movs	r0, #27
	bl 0x02008cb0
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #27
	bl 0x02008cf0
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #27
	bl 0x02008cf8
	movs	r0, #60
	bl 0x02008c50
	movs	r0, #27
	movs	r1, #0
	bl 0x02008cf0
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #27
	bl 0x02008cd0
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #1
	movs	r0, #28
	bl 0x02008cc0
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #28
	bl 0x02008cf0
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #3
	movs	r0, #27
	bl 0x02008cb0
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #27
	bl 0x02008cf0
	movs	r0, #30
	bl 0x02008c50
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #5
	bl 0x02008cd0
	movs	r1, #0
	movs	r0, #5
	bl 0x02008ce8
	bl 0x02008d60
	movs	r1, #0
	bl 0x02008c68
	cmp	r0, #0
	bne.n	.L_02000660
	movs	r0, #30
	bl 0x02008c50
	adds	r0, r6, #0
	adds	r0, #11
	bl 0x02008ce0
	movs	r1, #0
	movs	r0, #27
	bl 0x02008cf0
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x02008d08
	movs	r0, #60
	bl 0x02008c50
	movs	r0, #5
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r0, #5
	movs	r1, #0
	bl 0x02008cf0
	adds	r0, r6, #0
	adds	r0, #15
	bl 0x02008ce0
	b.n	.L_02000696
	.2byte 0x0000
	.4byte 0x00001812
	.2byte 0x0240
	.2byte 0x0200
.L_02000660:
	movs	r0, #30
	bl 0x02008c50
	adds	r0, r6, #0
	adds	r0, #13
	bl 0x02008ce0
	movs	r1, #0
	movs	r0, #27
	bl 0x02008cf0
	movs	r0, #60
	bl 0x02008c50
	movs	r0, #5
	movs	r1, #1
	bl 0x02008cc8
	movs	r0, #5
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r0, #5
	movs	r1, #0
	bl 0x02008cf0
.L_02000696:
	movs	r1, #4
	movs	r0, #27
	bl 0x02008cb0
	movs	r0, #60
	bl 0x02008c50
	movs	r0, #27
	movs	r1, #0
	bl 0x02008cf0
	movs	r1, #1
	movs	r0, #6
	bl 0x02008cc8
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #6
	bl 0x02008cf0
	movs	r0, #60
	bl 0x02008c50
	movs	r2, #0
	movs	r1, #6
	movs	r0, #27
	bl 0x02008cd0
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #27
	bl 0x02008cf0
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #129
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #28
	bl 0x02008d08
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #28
	bl 0x02008cf0
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #3
	movs	r0, #27
	bl 0x02008cb0
	movs	r0, #60
	bl 0x02008c50
	movs	r0, #27
	movs	r1, #0
	bl 0x02008cf0
	movs	r1, #252
	movs	r2, #164
	movs	r0, #27
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x02008c90
	ldr	r3, [pc, #500]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r0, #5
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r0, #6
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r0, #28
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r1, #228
	movs	r2, #164
	movs	r0, #27
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x02008c90
	ldr	r0, [r5, #0]
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r0, #5
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r0, #6
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r0, #28
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r1, #212
	movs	r2, #236
	movs	r0, #27
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x02008c90
	ldr	r0, [r5, #0]
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r0, #5
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r0, #6
	movs	r1, #27
	movs	r2, #0
	bl 0x02008cd0
	movs	r1, #27
	movs	r2, #0
	movs	r0, #28
	bl 0x02008cd0
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #0
	movs	r2, #0
	movs	r0, #27
	bl 0x02008ca8
	movs	r0, #120
	bl 0x02008c50
	movs	r0, #5
	movs	r1, #4
	movs	r2, #20
	bl 0x02008cb8
	movs	r1, #4
	movs	r2, #20
	movs	r0, #5
	bl 0x02008cb8
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #4
	movs	r2, #0
	adds	r1, #255
	movs	r0, #5
	bl 0x02008d08
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #5
	bl 0x02008cf0
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #3
	movs	r0, #6
	bl 0x02008cb0
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #6
	bl 0x02008cf0
	movs	r0, #60
	bl 0x02008c50
	ldr	r1, [r5, #0]
	movs	r2, #0
	movs	r0, #28
	bl 0x02008cd0
	movs	r0, #60
	bl 0x02008c50
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #28
	bl 0x02008cf8
	movs	r0, #30
	bl 0x02008c50
	movs	r1, #0
	movs	r0, #28
	bl 0x02008cf0
	movs	r0, #60
	bl 0x02008c50
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x02008cb0
	movs	r0, #5
	movs	r1, #3
	bl 0x02008cb0
	movs	r1, #3
	movs	r0, #6
	bl 0x02008cb0
	movs	r0, #120
	bl 0x02008c50
	movs	r0, #5
	movs	r1, #2
	bl 0x02008cb0
	ldr	r0, [r5, #0]
	bl 0x02008c70
	cmp	r0, #0
	beq.n	.L_020008a0
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x02008c80
.L_020008a0:
	movs	r0, #5
	bl 0x02008ca0
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x02008ca8
	movs	r0, #6
	movs	r1, #2
	bl 0x02008cb0
	ldr	r0, [r5, #0]
	bl 0x02008c70
	cmp	r0, #0
	beq.n	.L_020008d0
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x02008c80
.L_020008d0:
	movs	r0, #6
	bl 0x02008ca0
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x02008ca8
	movs	r0, #28
	movs	r1, #2
	bl 0x02008cb0
	ldr	r0, [r5, #0]
	bl 0x02008c70
	cmp	r0, #0
	beq.n	.L_02000900
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #28
	bl 0x02008c80
.L_02000900:
	movs	r0, #28
	bl 0x02008ca0
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x02008ca8
	bl 0x02008c60
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #75
	bl 0x02008c40
.L_0200091e:
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	bl 0x02008c58
	movs	r0, #0
	bl 0x02008d40
	movs	r3, #11
	movs	r2, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #10
	movs	r1, #19
	movs	r2, #1
	bl 0x02008c48
	movs	r1, #0
	movs	r2, #0
	movs	r0, #22
	bl 0x02008ca8
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x02008c40
	bl 0x02008c60
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	bl 0x02008c58
	movs	r0, #0
	bl 0x02008d40
	movs	r3, #26
	movs	r2, #20
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #25
	movs	r1, #20
	movs	r2, #1
	bl 0x02008c48
	movs	r1, #0
	movs	r2, #0
	movs	r0, #23
	bl 0x02008ca8
	movs	r0, #137
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02008c40
	bl 0x02008c60
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	bl 0x02008c58
	movs	r0, #0
	bl 0x02008d40
	movs	r3, #29
	movs	r2, #23
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #28
	movs	r1, #23
	movs	r2, #1
	bl 0x02008c48
	movs	r1, #0
	movs	r2, #0
	movs	r0, #24
	bl 0x02008ca8
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x02008c40
	bl 0x02008c60
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	bl 0x02008c58
	movs	r0, #0
	bl 0x02008d40
	movs	r3, #18
	movs	r2, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #17
	movs	r1, #11
	movs	r2, #1
	bl 0x02008c48
	movs	r1, #0
	movs	r2, #0
	movs	r0, #25
	bl 0x02008ca8
	movs	r0, #138
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02008c40
	bl 0x02008c60
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	bl 0x02008c58
	movs	r0, #0
	bl 0x02008d40
	movs	r3, #31
	movs	r2, #17
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #30
	movs	r1, #17
	movs	r2, #1
	bl 0x02008c48
	movs	r1, #0
	movs	r2, #0
	movs	r0, #26
	bl 0x02008ca8
	movs	r0, #133
	lsls	r0, r0, #2
	bl 0x02008c40
	bl 0x02008c60
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	bl 0x02008c58
	movs	r0, #0
	bl 0x02008d40
	movs	r3, #17
	movs	r2, #23
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #16
	movs	r1, #23
	movs	r2, #1
	bl 0x02008c48
	movs	r1, #0
	movs	r2, #0
	movs	r0, #31
	bl 0x02008ca8
	movs	r0, #139
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02008c40
	bl 0x02008c60
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [pc, #32]
	ldr	r5, [r3, #108]
	bl 0x02008d58
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #188
	adds	r5, r5, r3
	ldr	r1, [r5, #0]
	movs	r3, #1
	adds	r1, #35
	ldrb	r2, [r1, #0]
	orrs	r3, r2
	movs	r2, #253
	ands	r3, r2
	strb	r3, [r1, #0]
	pop	{r5, pc}
	.2byte 0x8d80
	.2byte 0x0200
	.global Func_02000ac8
	.thumb_func
Func_02000ac8:
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
	ldr	r3, [pc, #84]
	adds	r2, #224
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02000af6
	movs	r0, #12
	movs	r1, #0
	bl 0x02008d68
	b.n	.L_02000b02
.L_02000af6:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02000b02
	ldr	r0, [pc, #64]
	bl 0x02008d50
.L_02000b02:
	movs	r1, #1
	movs	r0, #21
	bl 0x02008d00
	ldr	r3, [pc, #40]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02000b30
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02008c38
	cmp	r0, #0
	bne.n	.L_02000b30
	movs	r0, #66
	movs	r1, #0
	bl 0x02008d30
.L_02000b30:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000000a
	.4byte 0x0000000d
	.2byte 0x8d80
	.2byte 0x0200
	.global Func_02000b44
	.thumb_func
Func_02000b44:
	push	{lr}
	ldr	r3, [pc, #224]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #216]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_02000c22
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x02008c38
	cmp	r0, #0
	beq.n	.L_02000b7a
	movs	r3, #11
	movs	r2, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #10
	movs	r1, #19
	movs	r2, #1
	movs	r3, #1
	bl 0x02008c48
.L_02000b7a:
	movs	r0, #137
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02008c38
	cmp	r0, #0
	beq.n	.L_02000b9c
	movs	r3, #26
	movs	r2, #20
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #25
	movs	r1, #20
	movs	r2, #1
	movs	r3, #1
	bl 0x02008c48
.L_02000b9c:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x02008c38
	cmp	r0, #0
	beq.n	.L_02000bbe
	movs	r3, #29
	movs	r2, #23
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #28
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02008c48
.L_02000bbe:
	movs	r0, #138
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02008c38
	cmp	r0, #0
	beq.n	.L_02000be0
	movs	r3, #18
	movs	r2, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #17
	movs	r1, #11
	movs	r2, #1
	movs	r3, #1
	bl 0x02008c48
.L_02000be0:
	movs	r0, #133
	lsls	r0, r0, #2
	bl 0x02008c38
	cmp	r0, #0
	beq.n	.L_02000c00
	movs	r3, #31
	movs	r2, #17
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #30
	movs	r1, #17
	movs	r2, #1
	movs	r3, #1
	bl 0x02008c48
.L_02000c00:
	movs	r0, #139
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02008c38
	cmp	r0, #0
	beq.n	.L_02000c22
	movs	r3, #17
	movs	r2, #23
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #16
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02008c48
.L_02000c22:
	movs	r0, #0
	add	sp, #8
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x000d
	.2byte 0x0000
	push	{lr}
	bl 0x02008d18
	pop	{pc}
	.section .rodata.x02008d80,"a",%progbits
	.4byte 0x02160016
	.4byte 0x02170017
	.4byte 0x02180018
	.4byte 0x02190019
	.4byte 0x021a001a
	.4byte 0x021b001d
	.4byte 0x0000ffff
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000083
	.4byte 0x00000027
	.4byte 0x00000000
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
	.4byte 0x00000028
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
	.4byte 0x00000005
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
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x00000015
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.global gDeriMuraEntrances
gDeriMuraEntrances:
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000152
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000170
	.4byte 0x400000e0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0014
	.4byte 0x00000050
	.4byte 0x40000160
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x1010100b
	.4byte 0xffffffff
	.4byte 0x1020200b
	.4byte 0xffffffff
	.4byte 0x1030300b
	.4byte 0xffffffff
	.4byte 0x1040400b
	.4byte 0xffffffff
	.4byte 0x1050500b
	.4byte 0xffffffff
	.4byte 0x1060600b
	.4byte 0xffffffff
	.4byte 0x1070100c
	.4byte 0xffffffff
	.4byte 0x10802002
	.4byte 0xffffffff
	.4byte 0x10903002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x0000000d
	.4byte 0x1010100b
	.4byte 0xffffffff
	.4byte 0x1020200b
	.4byte 0xffffffff
	.4byte 0x1030300b
	.4byte 0xffffffff
	.4byte 0x1040400b
	.4byte 0xffffffff
	.4byte 0x1050500b
	.4byte 0xffffffff
	.4byte 0x1060600b
	.4byte 0xffffffff
	.4byte 0x1070100c
	.4byte 0xffffffff
	.4byte 0x10802002
	.4byte 0xffffffff
	.4byte 0x10903002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00010000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00018000
	.4byte 0xffff0070
	.4byte 0x02008d9c
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff008c
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0001c000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0xffff00e0
	.4byte 0x00000003
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00014000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00012000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00015000
	.4byte 0xffff0092
	.4byte 0x02008f44
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00024000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00014000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00010000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00018000
	.4byte 0xffff008b
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00004000
	.4byte 0xffff008c
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0001c000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00010000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00018000
	.4byte 0xffff00e0
	.4byte 0x00000003
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00014000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00012000
	.4byte 0xffff00bb
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00018000
	.4byte 0xffff0092
	.4byte 0x02008f44
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00024000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00018000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0x02100122
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x02110122
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x02120122
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0x02130122
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0x02140122
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x02150122
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200809d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x0200809d
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x0200809d
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200809d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200809d
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x0200809d
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008155
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020081d9
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001719
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000171a
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000171b
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000171c
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000171d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000171e
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0000171f
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008231
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001723
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001724
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001725
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008289
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001726
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001727
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001728
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001729
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000172a
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000172b
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000172c
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000172d
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000172e
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000172f
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001730
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001731
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001732
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x0000174c
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x0040303c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200809d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x0200809d
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x0200809d
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200809d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200809d
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x0200809d
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x020083a1
	.4byte 0x00004602
	.4byte 0xffff001e
	.4byte 0x02008c31
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte 0x02008c31
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte 0x02008c31
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte 0x02008c31
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000017e4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000017e5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000017e6
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000017e7
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020082e9
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000017eb
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000017ec
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000017ed
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000017ee
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000017ef
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000017f0
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000017f1
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000017f2
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008341
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000017f3
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000017f4
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000017f5
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000017f6
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000017f7
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000017f8
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000017f9
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000017fa
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000017fb
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000017fc
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000017fd
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000017fe
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000017ff
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001838
	.4byte 0x50008a05
	.4byte 0x0210003c
	.4byte 0x02008925
	.4byte 0x50008a05
	.4byte 0x0211003d
	.4byte 0x020089e1
	.4byte 0x50008a05
	.4byte 0x0212003e
	.4byte 0x02008961
	.4byte 0x50008a05
	.4byte 0x0213003f
	.4byte 0x020089a1
	.4byte 0x50008a05
	.4byte 0x02140040
	.4byte 0x02008a21
	.4byte 0x50008a05
	.4byte 0x02150041
	.4byte 0x02008a5d
	.4byte 0x00001815
	.4byte 0x02160016
	.4byte 0x02008a9d
	.4byte 0x00001815
	.4byte 0x02170017
	.4byte 0x02008a9d
	.4byte 0x00001815
	.4byte 0x02180018
	.4byte 0x02008a9d
	.4byte 0x00001815
	.4byte 0x02190019
	.4byte 0x02008a9d
	.4byte 0x00001815
	.4byte 0x021a001a
	.4byte 0x02008a9d
	.4byte 0x00001815
	.4byte 0x021b001d
	.4byte 0x02008a9d
	.4byte 0x50008805
	.4byte 0x03000064
	.4byte 0x02008119
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x0040303c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
