.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020086bd, 0x02008095, 0x020080a1, 0x020080a9, 0x02008629, 0x0200809d, 0x020087a5
	overlay_veneer \EntryTarget
	.endr
	push	{lr}
	movs	r0, #12
	movs	r1, #1
	movs	r2, #13
	bl 0x02009a04
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r6, [r0, #80]
	adds	r0, #100
	ldrh	r5, [r0, #0]
	adds	r3, r5, #1
	lsls	r5, r5, #16
	asrs	r5, r5, #16
	strh	r3, [r0, #0]
	lsls	r0, r5, #12
	bl 0x020098cc
	movs	r1, #128
	ldr	r3, [pc, #24]
	lsls	r1, r1, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x230f
	ands	r3, r5
	strh	r0, [r6, #18]
	cmp	r3, #0
	bne.n	.L_02000078
	movs	r0, #155
	bl 0x02009a2c
.L_02000078:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x021c
	.2byte 0x0300
	push	{lr}
	bl 0x0200992c
	ldr	r2, [r0, #80]
	movs	r3, #0
	str	r3, [r0, #108]
	adds	r0, #100
	strh	r3, [r0, #0]
	strh	r3, [r2, #18]
	pop	{pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9a34
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9a64
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.4byte 0x02009a8c
	.4byte 0x049b23c0
	.4byte 0x4a026a1b
	.4byte 0x601a33f0
	.4byte 0x00004770
	.4byte 0x02ee0000
	.4byte 0x049b23c0
	.4byte 0x22b06a1b
	.4byte 0x045233f0
	.4byte 0x4770601a
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	adds	r0, r1, #0
	bl 0x0200992c
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #16]
	asrs	r3, r3, #20
	asrs	r2, r2, #20
	cmp	r3, #8
	bne.n	.L_020000fa
	cmp	r2, #36
	bne.n	.L_020000fa
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #97
	bl 0x020098dc
.L_020000fa:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #97
	bl 0x020098d4
	cmp	r0, #0
	beq.n	.L_0200010c
	bl 0x02008838
.L_0200010c:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	sub	sp, #12
	movs	r3, #5
	movs	r2, #13
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #7
	movs	r1, #13
	movs	r2, #1
	movs	r3, #1
	bl 0x02009a24
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #108
	bl 0x020098d4
	cmp	r0, #0
	beq.n	.L_0200013c
	b.n	.L_020003ae
.L_0200013c:
	movs	r0, #12
	bl 0x0200992c
	adds	r5, r0, #0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #108
	bl 0x020098dc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #110
	bl 0x020098dc
	movs	r0, #12
	bl 0x0200992c
	movs	r1, #0
	bl 0x020098fc
	movs	r1, #176
	movs	r2, #216
	movs	r0, #12
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x0200995c
	movs	r0, #12
	movs	r1, #9
	movs	r2, #0
	bl 0x0200997c
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r6, #29
.L_02000186:
	ldrh	r3, [r5, #6]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	strh	r3, [r5, #6]
	movs	r2, #255
	ldr	r3, [r5, #24]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bgt.n	.L_020001a4
	movs	r2, #160
	lsls	r2, r2, #4
	adds	r3, r3, r2
	str	r3, [r5, #24]
.L_020001a4:
	ldr	r3, [r5, #24]
	movs	r0, #1
	str	r3, [r5, #28]
	subs	r6, #1
	bl 0x020098bc
	cmp	r6, #0
	bge.n	.L_02000186
	movs	r3, #192
	lsls	r3, r3, #6
	strh	r3, [r5, #6]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r0, #12
	bl 0x0200992c
	movs	r1, #1
	bl 0x020098fc
	movs	r1, #1
	movs	r0, #12
	bl 0x020099bc
	movs	r0, #8
	bl 0x0200990c
	movs	r0, #12
	movs	r1, #9
	movs	r2, #0
	bl 0x0200997c
	movs	r1, #120
	movs	r2, #200
	movs	r0, #12
	bl 0x0200994c
	movs	r0, #12
	bl 0x0200992c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #200
	movs	r1, #136
	movs	r0, #12
	bl 0x0200994c
	movs	r0, #3
	bl 0x0200990c
	ldr	r5, [pc, #416]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200996c
	movs	r1, #224
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ac
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r2, #16
	movs	r1, #0
	negs	r2, r2
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r2, #16
	movs	r1, #0
	negs	r2, r2
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r2, #16
	movs	r1, #0
	negs	r2, r2
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x020099ac
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r1, #0
	movs	r2, #16
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	movs	r0, #12
	movs	r1, #5
	movs	r2, #0
	bl 0x0200997c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #12
	bl 0x02009a1c
	movs	r0, #3
	bl 0x0200990c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #12
	bl 0x0200995c
	movs	r0, #13
	bl 0x0200992c
	ldr	r3, [pc, #24]
	movs	r6, #0
	str	r3, [r0, #108]
	movs	r0, #13
	bl 0x0200992c
	adds	r0, #100
	strh	r6, [r0, #0]
.L_020003ae:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x8049
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #8
	movs	r2, #9
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #9
	movs	r1, #9
	movs	r2, #2
	movs	r3, #3
	bl 0x02009a24
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #12
	movs	r2, #7
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #38
	movs	r2, #5
	movs	r3, #2
	str	r1, [sp, #8]
	bl 0x02009a24
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r3, #14
	movs	r2, #11
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #39
	movs	r1, #2
	movs	r2, #4
	movs	r3, #3
	bl 0x02009a24
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #21
	movs	r2, #9
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #39
	movs	r1, #2
	movs	r2, #3
	movs	r3, #3
	bl 0x02009a24
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #25
	movs	r2, #8
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #39
	movs	r2, #4
	movs	r3, #3
	str	r1, [sp, #8]
	bl 0x02009a24
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #13
	bl 0x02008080
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #109
	bl 0x020098d4
	cmp	r0, #0
	bne.n	.L_02000528
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #108
	bl 0x020098d4
	cmp	r0, #0
	beq.n	.L_02000528
	movs	r0, #12
	bl 0x0200992c
	adds	r5, r0, #0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #109
	bl 0x020098dc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #110
	bl 0x020098e4
	movs	r0, #13
	bl 0x02008080
	movs	r1, #135
	movs	r2, #184
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200995c
	movs	r3, #176
	lsls	r3, r3, #13
	str	r3, [r5, #12]
	movs	r0, #12
	movs	r1, #11
	movs	r2, #0
	bl 0x0200997c
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r0, #12
	ldr	r1, [pc, #88]
	ldr	r2, [pc, #92]
	bl 0x02009934
	movs	r1, #156
	movs	r0, #12
	lsls	r1, r1, #1
	movs	r2, #184
	bl 0x02009944
	movs	r6, #23
.L_020004e6:
	ldrh	r3, [r5, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	strh	r3, [r5, #6]
	movs	r2, #255
	ldr	r3, [r5, #24]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bgt.n	.L_02000504
	movs	r2, #160
	lsls	r2, r2, #4
	adds	r3, r3, r2
	str	r3, [r5, #24]
.L_02000504:
	ldr	r3, [r5, #24]
	movs	r0, #1
	str	r3, [r5, #28]
	subs	r6, #1
	bl 0x020098bc
	cmp	r6, #0
	bge.n	.L_020004e6
	movs	r3, #192
	lsls	r3, r3, #6
	strh	r3, [r5, #6]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r0, #12
	bl 0x02009954
.L_02000528:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00023333
	.2byte 0x1999
	.2byte 0x0001
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #97
	bl 0x020098d4
	cmp	r0, #0
	beq.n	.L_02000552
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #106
	bl 0x020098d4
	cmp	r0, #0
	beq.n	.L_02000560
.L_02000552:
	movs	r0, #123
	bl 0x02009a2c
	movs	r0, #2
	bl 0x020099ec
	b.n	.L_02000614
.L_02000560:
	bl 0x02009914
	movs	r0, #0
	bl 0x020099fc
	movs	r2, #16
	movs	r3, #160
	lsls	r3, r3, #7
	movs	r0, #11
	movs	r1, #0
	negs	r2, r2
	bl 0x02009a0c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x020099ac
	movs	r0, #11
	bl 0x02009954
	movs	r0, #20
	bl 0x0200990c
	movs	r0, #7
	bl 0x020098d4
	cmp	r0, #0
	beq.n	.L_020005ae
	ldr	r0, [pc, #120]
	bl 0x02009994
	movs	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	b.n	.L_020005be
.L_020005ae:
	ldr	r0, [pc, #108]
	bl 0x02009994
	movs	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
.L_020005be:
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #11
	ldr	r1, [pc, #88]
	bl 0x02009934
	movs	r0, #11
	movs	r1, #2
	bl 0x0200996c
	ldr	r3, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200992c
	cmp	r0, #0
	beq.n	.L_020005f4
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #11
	bl 0x0200993c
.L_020005f4:
	movs	r0, #11
	bl 0x02009954
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200995c
	movs	r2, #208
	movs	r0, #4
	movs	r1, #168
	lsls	r2, r2, #2
	bl 0x0200994c
	bl 0x0200991c
.L_02000614:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00002706
	.4byte 0x00002707
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9b4c
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #120]
	movs	r2, #3
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0200068e
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #252
	ldrh	r3, [r3, #0]
	movs	r1, #14
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_0200064c:
	movs	r4, #160
	lsls	r4, r4, #19
	lsls	r3, r1, #1
	adds	r4, #224
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #1
	bne.n	.L_0200064c
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #226
	strh	r0, [r3, #0]
	adds	r3, #58
	ldrh	r3, [r3, #0]
	movs	r1, #14
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_02000676:
	ldr	r4, [pc, #56]
	lsls	r3, r1, #1
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #1
	bne.n	.L_02000676
	ldr	r3, [pc, #40]
	strh	r0, [r3, #0]
.L_0200068e:
	ldr	r3, [pc, #28]
	movs	r1, #12
	ldr	r0, [r3, #0]
	ldr	r5, [pc, #32]
	lsrs	r0, r0, #2
	bl 0x020098b4
	lsls	r0, r0, #1
	ldrsh	r0, [r5, r0]
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #254
	strh	r0, [r3, #0]
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0x05000100
	.4byte 0x05000102
	.2byte 0x9c30
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #200
	adds	r2, #85
	str	r2, [r3, #0]
	ldr	r0, [pc, #196]
	lsls	r1, r1, #4
	bl 0x020098c4
	ldr	r3, [pc, #192]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	beq.n	.L_020006f2
	movs	r0, #48
	adds	r0, #255
	bl 0x020098e4
	b.n	.L_020006fa
.L_020006f2:
	movs	r0, #48
	adds	r0, #255
	bl 0x020098dc
.L_020006fa:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #97
	bl 0x020098d4
	cmp	r0, #0
	beq.n	.L_0200072a
	movs	r1, #136
	movs	r2, #146
	movs	r0, #8
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200995c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200995c
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200995c
.L_0200072a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #109
	bl 0x020098d4
	cmp	r0, #0
	beq.n	.L_02000746
	movs	r1, #156
	movs	r2, #184
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200995c
.L_02000746:
	ldr	r3, [pc, #84]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	bne.n	.L_02000788
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #109
	bl 0x020098d4
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_02000788
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #108
	bl 0x020098d4
	cmp	r0, #0
	beq.n	.L_02000788
	movs	r0, #13
	bl 0x0200992c
	ldr	r3, [pc, #36]
	str	r3, [r0, #108]
	movs	r0, #13
	bl 0x0200992c
	adds	r0, #100
	strh	r5, [r0, #0]
.L_02000788:
	movs	r0, #13
	bl 0x0200992c
	movs	r1, #0
	bl 0x020098fc
	movs	r0, #0
	pop	{r5, pc}
	.4byte 0x02008631
	.4byte 0x02000240
	.2byte 0x8049
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #132]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	sub	sp, #8
	cmp	r3, #1
	beq.n	.L_020007ce
	cmp	r3, #2
	beq.n	.L_020007c0
	cmp	r3, #3
	bne.n	.L_020007ca
.L_020007c0:
	movs	r0, #150
	lsls	r0, r0, #4
	bl 0x020098e4
	b.n	.L_020007e4
.L_020007ca:
	cmp	r3, #4
	bne.n	.L_020007d8
.L_020007ce:
	movs	r0, #150
	lsls	r0, r0, #4
	bl 0x020098dc
	b.n	.L_020007e4
.L_020007d8:
	cmp	r3, #5
	bne.n	.L_020007e4
	movs	r0, #150
	lsls	r0, r0, #4
	bl 0x020098dc
.L_020007e4:
	movs	r0, #150
	lsls	r0, r0, #4
	bl 0x020098d4
	cmp	r0, #0
	beq.n	.L_02000826
	movs	r5, #47
	movs	r6, #8
	movs	r0, #38
	movs	r1, #47
	movs	r2, #26
	movs	r3, #12
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x020098f4
	movs	r3, #72
	str	r3, [sp, #0]
	movs	r0, #102
	movs	r1, #47
	movs	r2, #26
	movs	r3, #12
	str	r5, [sp, #4]
	bl 0x020098f4
	movs	r0, #38
	movs	r1, #47
	movs	r2, #26
	movs	r3, #12
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x020098ec
.L_02000826:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x020099f4
	pop	{pc}
	push	{r5, lr}
	bl 0x02009914
	movs	r0, #0
	bl 0x020099fc
	ldr	r0, [pc, #432]
	bl 0x02009994
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #4
	bl 0x020099c4
	movs	r2, #10
	movs	r1, #0
	movs	r0, #10
	bl 0x020099a4
	bl 0x020099f4
	bl 0x02009914
	movs	r0, #0
	bl 0x020099fc
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #1
	movs	r0, #4
	bl 0x0200996c
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009934
	movs	r2, #16
	movs	r1, #0
	negs	r2, r2
	movs	r0, #4
	bl 0x02009a1c
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #168
	movs	r1, #1
	movs	r2, #134
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #16
	bl 0x020099dc
	bl 0x020099e4
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
	bl 0x02009934
	movs	r0, #9
	movs	r1, #24
	movs	r2, #0
	bl 0x02009a1c
	movs	r1, #160
	movs	r2, #0
.L_02000942:
	lsls	r1, r1, #7
	movs	r0, #9
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200998c
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #204
	adds	r2, #102
	bl 0x02009934
	movs	r1, #24
	movs	r0, #10
	negs	r1, r1
	movs	r2, #0
	bl 0x02009a1c
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #10
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200998c
	movs	r0, #10
	bl 0x0200990c
	movs	r1, #0
	movs	r0, #10
	bl 0x0200999c
	movs	r0, #4
	movs	r1, #0
	bl 0x02009924
	cmp	r0, #0
	bne.n	.L_020009fc
	movs	r0, #30
	bl 0x0200990c
	movs	r0, #10
	movs	r1, #3
	bl 0x02009984
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x020099cc
	movs	r0, #40
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x020099a4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000a50
	.2byte 0x0000
	.2byte 0x26c3
	.2byte 0x0000
.L_020009fc:
	movs	r0, #40
	bl 0x0200990c
	movs	r0, #10
	movs	r1, #3
	bl 0x02009984
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x020099cc
	movs	r0, #40
	bl 0x0200990c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #10
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x020099a4
	movs	r0, #10
	movs	r1, #6
	movs	r2, #15
	bl 0x0200997c
	movs	r0, #10
	movs	r1, #6
	movs	r2, #23
	bl 0x0200997c
	movs	r0, #10
	bl 0x0200990c
.L_02000a50:
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x020099c4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x020099c4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r2, #16
	movs	r3, #176
	lsls	r3, r3, #8
	movs	r1, #16
	negs	r2, r2
	movs	r0, #11
	bl 0x02009a0c
	movs	r0, #11
	bl 0x02009954
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #11
	bl 0x020099c4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #9
	bl 0x020099ac
	movs	r0, #10
	bl 0x0200990c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #9
	bl 0x020099c4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #11
	bl 0x020099ac
	movs	r0, #50
	bl 0x0200990c
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r0, #4
	movs	r1, #4
	bl 0x0200996c
	movs	r1, #4
	movs	r0, #11
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200998c
	movs	r0, #10
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x020099a4
	movs	r1, #3
	movs	r0, #9
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x020099a4
	movs	r1, #4
	movs	r0, #7
	bl 0x02009964
	movs	r0, #7
	bl 0x0200992c
	movs	r3, #0
	str	r3, [r0, #24]
	movs	r1, #0
	movs	r0, #7
	movs	r2, #10
	bl 0x020099a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x020099ac
	movs	r0, #45
	bl 0x0200990c
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x020099ac
	movs	r0, #25
	bl 0x0200990c
	movs	r0, #7
	bl 0x0200992c
	movs	r5, #128
	lsls	r5, r5, #9
	movs	r2, #16
	movs	r3, #192
	lsls	r3, r3, #8
	movs	r1, #0
	negs	r2, r2
	str	r5, [r0, #24]
	movs	r0, #7
	bl 0x02009a0c
	movs	r0, #7
	bl 0x02009954
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #9
	bl 0x020099c4
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x020099c4
	movs	r1, #0
	movs	r0, #9
	bl 0x0200999c
	movs	r0, #20
	bl 0x0200990c
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	movs	r2, #0
	adds	r0, #10
	bl 0x020099a4
	bl 0x02009904
	movs	r0, #10
	bl 0x0200990c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #7
	bl 0x020099c4
	movs	r1, #8
	movs	r0, #9
	negs	r1, r1
	movs	r2, #8
	bl 0x02009a1c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #9
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #8
	movs	r0, #10
	negs	r1, r1
	movs	r2, #8
	bl 0x02009a1c
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #10
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #9
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x020099a4
	movs	r1, #3
	movs	r0, #7
	bl 0x02009974
	movs	r0, #20
	bl 0x0200990c
	movs	r0, #10
	movs	r1, #4
	bl 0x02009974
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x020099c4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #7
	bl 0x020099ac
	movs	r0, #35
	bl 0x0200990c
	movs	r1, #160
	movs	r0, #11
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x020099ac
	movs	r0, #50
	bl 0x0200990c
	movs	r1, #176
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200996c
	movs	r1, #3
	movs	r0, #4
	bl 0x02009974
	movs	r0, #25
	bl 0x0200990c
	movs	r0, #9
	movs	r1, #3
	bl 0x02009984
	movs	r1, #129
	movs	r0, #9
	lsls	r1, r1, #1
	bl 0x020099cc
	movs	r0, #10
	movs	r1, #3
	bl 0x02009984
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x020099cc
	movs	r0, #40
	bl 0x0200990c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x020099ac
	movs	r0, #40
	bl 0x0200990c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #7
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r2, #128
	movs	r0, #7
	adds	r1, r5, #0
	lsls	r2, r2, #8
	bl 0x02009934
	movs	r2, #16
	movs	r1, #0
	negs	r2, r2
	movs	r0, #7
	bl 0x02009a1c
	movs	r0, #10
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x020099a4
	movs	r0, #9
	movs	r1, #2
	bl 0x02009984
	movs	r1, #2
	movs	r0, #10
	bl 0x0200998c
	movs	r0, #10
	bl 0x0200990c
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x020099c4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x020099c4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x020099c4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #11
	bl 0x020099c4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r0, #9
	movs	r1, #6
	movs	r2, #15
	bl 0x0200997c
	movs	r0, #9
	movs	r1, #6
	movs	r2, #23
	bl 0x0200997c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x020099c4
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x020099a4
	movs	r0, #11
	movs	r1, #2
	bl 0x020099bc
	movs	r0, #11
	movs	r1, #6
	movs	r2, #15
	bl 0x0200997c
	movs	r0, #11
	movs	r1, #6
	movs	r2, #23
	bl 0x0200997c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #11
	bl 0x020099c4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x020099c4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #11
	bl 0x020099c4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #7
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r0, #7
	movs	r1, #4
	bl 0x02009974
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x020099a4
	movs	r0, #11
	movs	r1, #3
	bl 0x02009984
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x020099cc
	movs	r0, #50
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #7
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #11
	bl 0x020099c4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r2, #16
	movs	r0, #11
	movs	r1, #0
	negs	r2, r2
	bl 0x02009a1c
	movs	r1, #176
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ac
	movs	r2, #0
	movs	r1, #0
	movs	r0, #7
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #11
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #11
	movs	r1, #0
	bl 0x020099a4
	movs	r1, #3
	movs	r0, #9
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r2, #0
	movs	r1, #0
	movs	r0, #9
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #9
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x020099a4
	movs	r1, #3
	movs	r0, #10
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x020099a4
	movs	r1, #3
	movs	r0, #9
	bl 0x02009974
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #10
	bl 0x02009974
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x020099c4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x020099c4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #11
	bl 0x020099c4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x020099c4
	movs	r1, #132
	movs	r2, #55
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x020099c4
	movs	r1, #3
	movs	r0, #9
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #208
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r2, #16
	movs	r0, #9
	movs	r1, #0
	negs	r2, r2
	bl 0x02009a14
	movs	r2, #16
	movs	r0, #10
	movs	r1, #0
	negs	r2, r2
	bl 0x02009a1c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #2
	movs	r0, #11
	bl 0x0200998c
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #7
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x020099a4
	movs	r0, #9
	movs	r1, #3
	bl 0x02009984
	movs	r1, #129
	movs	r0, #9
	lsls	r1, r1, #1
	bl 0x020099cc
	movs	r0, #10
	movs	r1, #3
	bl 0x02009984
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x020099cc
	movs	r0, #40
	bl 0x0200990c
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r0, #168
	movs	r1, #1
	movs	r2, #128
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #16
	bl 0x020099dc
	bl 0x020099e4
	movs	r0, #10
	bl 0x0200990c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x020099c4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #176
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #55
	movs	r0, #11
	bl 0x020099c4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x020099c4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #9
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r0, #4
	movs	r1, #2
	bl 0x020099bc
	movs	r2, #20
	movs	r0, #4
	movs	r1, #8
	negs	r2, r2
	bl 0x02009a1c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #7
	bl 0x020099c4
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x020099a4
	movs	r1, #3
	movs	r0, #11
	bl 0x02009974
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #11
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x020099a4
	movs	r1, #3
	movs	r0, #9
	bl 0x02009974
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #10
	bl 0x02009974
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #10
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #9
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x020099a4
	movs	r1, #3
	movs	r0, #10
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x020099a4
	movs	r1, #2
	movs	r0, #9
	bl 0x0200998c
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200999c
	ldr	r3, [pc, #332]
	movs	r2, #157
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #4
	bgt.n	.L_0200140c
	movs	r0, #4
	movs	r1, #0
	bl 0x02009924
	cmp	r0, #0
	bne.n	.L_0200139c
	movs	r0, #25
	bl 0x0200990c
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #11
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #11
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #11
	movs	r1, #0
	bl 0x020099a4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020013f8
.L_0200139c:
	movs	r0, #35
	bl 0x0200990c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #9
	bl 0x020099c4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #10
	bl 0x020099c4
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #11
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #11
	bl 0x020099c4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #6
	strh	r3, [r2, #0]
	adds	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
.L_020013f8:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_020014f6
.L_0200140c:
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #108]
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r2, [r3, #0]
	movs	r0, #4
	adds	r2, #2
	strh	r2, [r3, #0]
	movs	r1, #0
	bl 0x02009924
	cmp	r0, #0
	bne.n	.L_02001488
	movs	r0, #25
	bl 0x0200990c
	movs	r1, #160
	movs	r2, #0
	movs	r0, #11
	lsls	r1, r1, #7
	bl 0x020099ac
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #7
	bl 0x020099b4
	movs	r0, #10
	bl 0x0200990c
	movs	r1, #129
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x020099c4
	movs	r1, #2
	movs	r0, #11
	bl 0x0200998c
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #11
	movs	r1, #0
	bl 0x020099a4
	ldr	r2, [r5, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020014e6
	.2byte 0x0240
	.2byte 0x0200
.L_02001488:
	movs	r0, #35
	bl 0x0200990c
	movs	r1, #160
	movs	r2, #0
	movs	r0, #11
	lsls	r1, r1, #7
	bl 0x020099ac
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #7
	bl 0x020099b4
	movs	r0, #10
	bl 0x0200990c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #40
	movs	r0, #11
	bl 0x020099c4
	movs	r0, #11
	movs	r1, #6
	movs	r2, #15
	bl 0x0200997c
	movs	r0, #11
	movs	r1, #6
	movs	r2, #23
	bl 0x0200997c
	ldr	r2, [r5, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #6
	strh	r3, [r2, #0]
	adds	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
.L_020014e6:
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x020099b4
	movs	r0, #10
	bl 0x0200990c
.L_020014f6:
	movs	r2, #10
	negs	r2, r2
	movs	r1, #0
	movs	r0, #7
	bl 0x02009a1c
	movs	r0, #15
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #7
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #10
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x020099a4
	movs	r1, #2
	movs	r0, #9
	bl 0x0200998c
	movs	r0, #10
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x020099a4
	movs	r0, #10
	movs	r1, #4
	bl 0x02009974
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #7
	bl 0x020099c4
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x020099a4
	movs	r0, #9
	movs	r1, #3
	bl 0x0200996c
	movs	r1, #3
	movs	r0, #10
	bl 0x02009974
	movs	r0, #25
	bl 0x0200990c
	movs	r0, #10
	movs	r1, #1
	bl 0x020099d4
	movs	r2, #32
	movs	r1, #0
	negs	r2, r2
	movs	r0, #10
	bl 0x02009a14
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #9
	movs	r1, #16
	movs	r2, #0
	bl 0x02009a1c
	movs	r2, #32
	movs	r1, #0
	negs	r2, r2
	movs	r0, #9
	bl 0x02009a14
	movs	r0, #10
	bl 0x02009954
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	bl 0x020099dc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200995c
	movs	r0, #9
	bl 0x02009954
	movs	r1, #0
	movs	r2, #0
	movs	r0, #9
	bl 0x0200995c
	movs	r0, #30
	bl 0x0200990c
	movs	r0, #168
	movs	r1, #1
	movs	r2, #134
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #16
	bl 0x020099dc
	bl 0x020099e4
	movs	r0, #20
	bl 0x0200990c
	movs	r2, #10
	movs	r0, #11
	movs	r1, #0
	negs	r2, r2
	bl 0x02009a1c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #11
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #7
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x020099c4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #7
	bl 0x020099c4
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x020099a4
	movs	r1, #2
	movs	r0, #7
	bl 0x0200998c
	movs	r0, #10
	bl 0x0200990c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x020099ac
	movs	r0, #40
	bl 0x0200990c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #11
	bl 0x020099c4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #6
	adds	r1, #255
	movs	r2, #50
	movs	r0, #7
	bl 0x020099c4
	movs	r2, #0
	movs	r1, #0
	movs	r0, #7
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r0, #7
	movs	r1, #4
	bl 0x02009974
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r2, #32
	movs	r0, #7
	movs	r1, #0
	negs	r2, r2
	bl 0x02009a1c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #11
	bl 0x020099ac
	movs	r0, #10
	bl 0x0200990c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #7
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #7
	bl 0x020099c4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x020099a4
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ac
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #11
	bl 0x020099ac
	movs	r0, #30
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #11
	bl 0x02009974
	movs	r0, #10
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #4
	bl 0x02009974
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #4
	adds	r1, #204
	adds	r2, #102
	bl 0x02009934
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #11
	adds	r1, #204
	adds	r2, #102
	bl 0x02009934
	movs	r1, #8
	movs	r2, #16
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x02009a1c
	movs	r2, #10
	movs	r0, #11
	movs	r1, #0
	negs	r2, r2
	bl 0x02009a14
	movs	r2, #16
	movs	r1, #0
	negs	r2, r2
	movs	r0, #4
	bl 0x02009a1c
	movs	r0, #11
	bl 0x02009954
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x020099ac
	movs	r0, #20
	bl 0x0200990c
	movs	r1, #3
	movs	r0, #7
	bl 0x02009974
	movs	r0, #20
	bl 0x0200990c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200996c
	movs	r1, #3
	movs	r0, #11
	bl 0x02009974
	movs	r0, #20
	bl 0x0200990c
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #7
	ldr	r1, [pc, #128]
	bl 0x02009934
	movs	r0, #7
	movs	r1, #2
	bl 0x0200996c
	ldr	r3, [pc, #120]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200992c
	cmp	r0, #0
	beq.n	.L_02001856
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200993c
.L_02001856:
	movs	r0, #7
	bl 0x02009954
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200995c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #11
	ldr	r1, [pc, #60]
	adds	r2, #153
	bl 0x02009934
	movs	r0, #11
	movs	r1, #2
	bl 0x0200996c
	ldr	r0, [r5, #0]
	bl 0x0200992c
	cmp	r0, #0
	beq.n	.L_02001894
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #11
	bl 0x0200993c
.L_02001894:
	movs	r0, #11
	bl 0x02009954
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200995c
	bl 0x0200991c
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00013333
	.4byte 0x02000240
	.irp EntryTarget, 0x03000514, 0x080000c1, 0x080000d1, 0x08000119, 0x080003c9, 0x080003d1, 0x080003d9, 0x080201e9, 0x080201f1, 0x08020219, 0x08038141, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80c1, 0x080c80d1, 0x080c80d9, 0x080c80f1, 0x080c80f9, 0x080c8111, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8241, 0x080c8279, 0x080c82e1, 0x080c84e1, 0x080c8581, 0x080c85e9, 0x080c85f1, 0x080c85f9, 0x080c8761, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0xffff0000
	.4byte 0x00000064
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000c9
	.4byte 0x101040c9
	.4byte 0xffffffff
	.4byte 0x102020c8
	.4byte 0xffffffff
	.4byte 0x103040ca
	.4byte 0xffffffff
	.4byte 0x104010c9
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0121
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00024000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00003000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00005000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0x005100f4
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00004000
	.4byte 0xffff0121
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008535
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x020080b1
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x020080c5
	.4byte 0x00000602
	.4byte 0xffff0016
	.4byte 0x02008831
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008039
	.4byte 0x10008c15
	.4byte 0x09610008
	.4byte 0x020080d5
	.4byte 0x00008c15
	.4byte 0x09610008
	.4byte 0x020080d9
	.4byte 0x50008905
	.4byte 0xffff001e
	.4byte 0x02008111
	.4byte 0x50008905
	.4byte 0xffff001f
	.4byte 0x020083bd
	.4byte 0x50008905
	.4byte 0xffff0020
	.4byte 0x020083dd
	.4byte 0x50008905
	.4byte 0xffff0021
	.4byte 0x020083fd
	.4byte 0x50008905
	.4byte 0xffff0022
	.4byte 0x0200841d
	.4byte 0x50008905
	.4byte 0xffff0023
	.4byte 0x0200843d
	.4byte 0x10008715
	.4byte 0x196e000d
	.4byte 0x0200845d
	.4byte 0x00008715
	.4byte 0x196e000d
	.4byte 0x02008469
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x62c85f0c
	.4byte 0x6a406684
	.4byte 0x69c06a00
	.4byte 0x69c06980
	.4byte 0x6a406a00
	.4byte 0x62c86684
