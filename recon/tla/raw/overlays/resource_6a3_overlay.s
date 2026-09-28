.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200ac55, 0x02008045, 0x02008051, 0x02008059, 0x02008969, 0x0200804d, 0x0200ae3d
	overlay_veneer \EntryTarget
	.endr
	push	{lr}
	movs	r1, #0
	bl 0x0200c754
	movs	r0, #0
	pop	{pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd084
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd0b4
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd0e0
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #24]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_02000078
	movs	r0, #24
	bl 0x0200c7ec
	bl 0x0200b474
	movs	r3, #1
	str	r3, [r5, #0]
.L_02000078:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xda3c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r7, [pc, #56]
	ldr	r5, [r7, #0]
	cmp	r5, #0
	bne.n	.L_020000ba
	ldr	r2, [pc, #52]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #18
	adds	r6, r2, r1
	ldrb	r3, [r6, #0]
	cmp	r3, #10
	bne.n	.L_020000ba
	adds	r1, #2
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	bl 0x0200c7ec
	movs	r3, #12
	strb	r3, [r6, #0]
	str	r5, [r0, #48]
	str	r5, [r0, #52]
	movs	r0, #25
	bl 0x0200c7ec
	bl 0x0200b474
	movs	r3, #1
	str	r3, [r7, #0]
.L_020000ba:
	pop	{r5, r6, r7, pc}
	.4byte 0x0200da3c
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r2, [pc, #132]
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #254
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	sub	sp, #12
	strh	r3, [r1, #0]
	movs	r2, #128
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #3
	adds	r3, #8
	strh	r2, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #4
	bl 0x0200c67c
	adds	r5, r0, #0
	ldr	r0, [pc, #88]
	bl 0x0200c6bc
	adds	r1, r5, #0
	bl 0x0200c68c
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #192
	adds	r3, #212
	adds	r0, r5, #0
	lsls	r1, r1, #19
	ldr	r2, [pc, #68]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	bl 0x0200c684
	ldr	r3, [pc, #44]
	mov	r0, sp
	adds	r0, #10
	strh	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #48]
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	bl 0x0200c67c
	adds	r5, r0, #0
	ldr	r0, [pc, #40]
	bl 0x0200c6bc
	adds	r1, r5, #0
	bl 0x0200c68c
	movs	r6, #0
	adds	r4, r5, #0
	b.n	.L_02000164
	.4byte 0x00000000
	.4byte 0x0300123c
	.4byte 0x000001c0
	.4byte 0x84000200
	.4byte 0x06002000
	.4byte 0x81000400
	.2byte 0x01c1
	.2byte 0x0000
.L_02000164:
	ldrh	r2, [r4, #0]
	movs	r3, #252
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	adds	r6, #1
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r6, #64
	bne.n	.L_02000164
	adds	r4, r5, #0
	movs	r6, #0
.L_0200017c:
	ldr	r2, [pc, #104]
	lsls	r1, r6, #6
	adds	r1, r1, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #1
	adds	r4, #8
	cmp	r6, #16
	bne.n	.L_0200017c
	adds	r0, r5, #0
	bl 0x0200c684
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [pc, #56]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #152
	strh	r3, [r2, #0]
	strh	r6, [r2, #2]
	movs	r0, #30
	bl 0x0200c654
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #0
	bl 0x0200c924
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200c91c
	b.n	.L_020001f0
	.2byte 0x0000
	.4byte 0x00001010
	.4byte 0x00003f41
	.4byte 0x06002000
	.2byte 0x1120
	.2byte 0x0300
.L_020001f0:
	movs	r0, #1
	bl 0x0200c92c
	movs	r0, #1
	bl 0x0200c654
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200c924
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200c91c
	movs	r0, #8
	bl 0x0200c92c
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #60]
	movs	r0, #144
	orrs	r3, r2
	strh	r3, [r1, #0]
	bl 0x0200c9c4
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200c75c
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200c75c
	ldr	r3, [pc, #20]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r2, #128
	movs	r1, #128
	ldr	r0, [r3, #0]
	b.n	.L_02000268
	.2byte 0x0000
	.4byte 0x00000100
	.2byte 0x0240
	.2byte 0x0200
.L_02000268:
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r1, #6
	movs	r2, #0
	movs	r0, #4
	bl 0x0200c85c
	movs	r0, #4
	bl 0x0200c7ec
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #16
	strb	r3, [r0, #0]
	negs	r1, r1
	movs	r0, #4
	movs	r2, #0
	bl 0x0200c96c
	movs	r0, #20
	bl 0x0200c7ec
	ldr	r3, [pc, #44]
	movs	r6, #0
	str	r3, [r0, #108]
.L_020002a2:
	ldr	r2, [pc, #32]
	lsrs	r3, r6, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #28]
	movs	r5, #128
	lsls	r5, r5, #19
	orrs	r2, r3
	adds	r5, #82
	strh	r2, [r5, #0]
	movs	r0, #1
	adds	r6, #1
	bl 0x0200c654
	cmp	r6, #32
	bne.n	.L_020002a2
	b.n	.L_020002d0
	.2byte 0x0000
	.4byte 0x00000010
	.2byte 0x1000
	.2byte 0x0000
	push	{r0, r2, r3, r6, lr}
	lsls	r0, r0, #8
.L_020002d0:
	movs	r3, #61
	movs	r2, #42
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #104
	movs	r1, #42
	movs	r2, #5
	movs	r3, #11
	bl 0x0200c74c
	movs	r3, #63
	movs	r2, #84
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #106
	movs	r1, #52
	movs	r2, #1
	movs	r3, #1
	bl 0x0200c74c
	movs	r0, #130
	movs	r1, #160
	movs	r2, #2
	movs	r3, #236
	lsls	r1, r1, #16
	lsls	r0, r0, #19
	bl 0x0200c9ac
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #0
	bl 0x0200c924
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200c91c
	movs	r0, #60
	bl 0x0200c92c
	bl 0x0200c6b4
	bl 0x0200c6a4
	ldr	r3, [pc, #88]
	movs	r1, #147
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r2, r3, r1
	ldrb	r0, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #38
	adds	r3, r3, r2
	ldrb	r1, [r3, #0]
	bl 0x0200c784
	ldr	r3, [pc, #52]
	movs	r2, #128
	strh	r3, [r5, #0]
	ldr	r3, [pc, #48]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	movs	r0, #1
	orrs	r3, r2
	ldr	r2, [pc, #40]
	strh	r3, [r1, #0]
	movs	r3, #0
	strh	r3, [r2, #0]
	strh	r3, [r2, #2]
	bl 0x0200c654
	ldr	r2, [pc, #32]
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	b.n	.L_02000394
	.4byte 0x00001008
	.4byte 0x00003f10
	.4byte 0x00000100
	.4byte 0x02000240
	.4byte 0x03001120
	.2byte 0x123c
	.2byte 0x0300
.L_02000394:
	bl 0x0200c75c
	add	sp, #12
	pop	{r5, r6, pc}
	push	{lr}
	adds	r0, r1, #0
	bl 0x0200c7ec
	ldr	r3, [r0, #8]
	ldr	r1, [r0, #16]
	adds	r0, r3, #0
	movs	r3, #8
	movs	r2, #0
	negs	r3, r3
	bl 0x0200c774
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #580]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	adds	r5, r1, #0
	bl 0x0200c7ec
	mov	r8, r0
	adds	r0, r5, #0
	bl 0x0200c7ec
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	asrs	r3, r3, #20
	cmp	r3, #63
	beq.n	.L_020003e2
	b.n	.L_020005ee
.L_020003e2:
	ldr	r3, [r6, #16]
	asrs	r3, r3, #20
	cmp	r3, #15
	beq.n	.L_020003ec
	b.n	.L_020005ee
.L_020003ec:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #12
	adds	r7, r6, #0
	bl 0x0200c6d4
	adds	r7, #85
	movs	r3, #3
	strb	r3, [r7, #0]
	movs	r0, #2
	bl 0x0200c654
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #20]
	movs	r5, #0
	b.n	.L_0200041c
.L_0200040c:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200c654
	cmp	r5, #29
	bgt.n	.L_02000426
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #20]
.L_0200041c:
	cmp	r2, r3
	bgt.n	.L_0200040c
	ldr	r3, [r6, #40]
	cmp	r3, #0
	bne.n	.L_0200040c
.L_02000426:
	movs	r3, #0
	strb	r3, [r7, #0]
	movs	r0, #134
	bl 0x0200c9c4
	bl 0x0200c7d4
	movs	r0, #0
	bl 0x0200c94c
	movs	r0, #254
	movs	r1, #1
	movs	r2, #232
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200c8ec
	bl 0x0200c8f4
	ldr	r3, [pc, #432]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r2, r2, #8
	lsls	r1, r1, #9
	bl 0x0200c7f4
	mov	r2, r8
	ldr	r3, [r2, #8]
	asrs	r0, r3, #20
	cmp	r0, #64
	bne.n	.L_0200048a
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #3
	movs	r2, #232
	bl 0x0200c82c
	movs	r1, #250
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	movs	r2, #232
	bl 0x0200c82c
	b.n	.L_0200049a
.L_0200048a:
	cmp	r0, #63
	bne.n	.L_0200049a
	movs	r1, #250
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	movs	r2, #232
	bl 0x0200c82c
.L_0200049a:
	ldr	r5, [pc, #360]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #250
	movs	r2, #248
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	bl 0x0200c82c
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c8b4
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200c924
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200c91c
	movs	r0, #60
	bl 0x0200c92c
	movs	r0, #50
	bl 0x0200c7cc
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c7cc
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #6
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c7cc
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c7cc
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #6
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c7cc
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c7cc
	movs	r1, #2
	movs	r2, #50
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200c8cc
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c7cc
	movs	r1, #22
	ldr	r0, [r5, #0]
	bl 0x0200c854
	movs	r0, #40
	bl 0x0200c7cc
	movs	r0, #0
	bl 0x0200aefc
	bl 0x020080c4
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200c854
	ldr	r0, [r5, #0]
	bl 0x0200c7ec
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #3
	movs	r0, #4
	bl 0x0200c864
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x0200c8d4
	movs	r0, #40
	bl 0x0200c7cc
	movs	r0, #60
	bl 0x0200c7cc
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200c8e4
	movs	r0, #131
	movs	r2, #174
	movs	r1, #0
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #19
	bl 0x0200c8ec
	movs	r5, #0
	bl 0x0200c8f4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #80
	str	r5, [r6, #108]
	bl 0x0200c9c4
	bl 0x0200c9bc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #246
	movs	r2, #248
	ldr	r1, [pc, #56]
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200c8ec
	bl 0x0200c8f4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #1
	bl 0x0200aefc
	bl 0x0200c7dc
.L_020005ee:
	movs	r3, #6
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #16]
	movs	r2, #0
	negs	r3, r3
	bl 0x0200c774
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xffc0
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r0, #23
	sub	sp, #8
	bl 0x0200c7ec
	adds	r7, r0, #0
.L_0200061c:
	movs	r0, #146
	lsls	r0, r0, #4
	movs	r2, #0
	adds	r0, #255
	mov	r8, r2
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_02000630
	b.n	.L_020008b4
.L_02000630:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200c6d4
	bl 0x0200c7d4
	movs	r0, #0
	bl 0x0200c94c
	movs	r0, #124
	bl 0x0200c9c4
	movs	r0, #194
	movs	r1, #1
	movs	r2, #208
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200c8ec
	bl 0x0200c8f4
	movs	r0, #6
	bl 0x0200c7cc
	movs	r0, #217
	bl 0x0200c9c4
	movs	r5, #49
	movs	r1, #103
	movs	r2, #4
	movs	r3, #3
	movs	r6, #76
	movs	r0, #49
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c74c
	movs	r0, #6
	bl 0x0200c7cc
	movs	r3, #3
	movs	r0, #49
	movs	r1, #106
	movs	r2, #4
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c74c
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #49
	bne.n	.L_020006a2
	movs	r2, #1
	mov	r8, r2
.L_020006a2:
	cmp	r3, #50
	bne.n	.L_020006aa
	movs	r2, #2
	mov	r8, r2
.L_020006aa:
	cmp	r3, #51
	bne.n	.L_020006b2
	movs	r2, #3
	mov	r8, r2
.L_020006b2:
	cmp	r3, #52
	bne.n	.L_020006ba
	movs	r3, #4
	mov	r8, r3
.L_020006ba:
	ldr	r3, [r7, #12]
	cmp	r3, #0
	beq.n	.L_020006c4
	movs	r2, #0
	mov	r8, r2
.L_020006c4:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #13
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_020006d6
	movs	r3, #0
	mov	r8, r3
.L_020006d6:
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_020006e6
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #13
	bl 0x0200c6d4
.L_020006e6:
	mov	r3, r8
	subs	r3, #2
	cmp	r3, #1
	bls.n	.L_020006f0
	b.n	.L_020007f6
.L_020006f0:
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200c8e4
	movs	r0, #23
	movs	r1, #1
	bl 0x0200c8dc
	bl 0x0200c8f4
	movs	r0, #155
	lsls	r0, r0, #1
	bl 0x0200c9c4
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r2, r7, #0
	str	r3, [r7, #72]
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #408]
	movs	r5, #0
	str	r2, [r7, #20]
	cmp	r3, r2
	ble.n	.L_0200073e
.L_0200072a:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200c654
	cmp	r5, #29
	bgt.n	.L_0200073e
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #20]
	cmp	r2, r3
	bgt.n	.L_0200072a
.L_0200073e:
	movs	r2, #243
	movs	r0, #192
	movs	r1, #192
	lsls	r2, r2, #8
	lsls	r1, r1, #10
	adds	r2, #51
	lsls	r0, r0, #10
	bl 0x0200c75c
	movs	r0, #134
	bl 0x0200c9c4
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #192
	lsls	r3, r3, #11
	movs	r1, #192
	movs	r2, #192
	str	r3, [r7, #40]
	movs	r0, #23
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c7f4
	mov	r3, r8
	cmp	r3, #2
	bne.n	.L_02000784
	movs	r1, #32
	movs	r0, #23
	negs	r1, r1
	movs	r2, #0
	bl 0x0200c834
	b.n	.L_0200078e
.L_02000784:
	movs	r0, #23
	movs	r1, #32
	movs	r2, #0
	bl 0x0200c834
.L_0200078e:
	movs	r0, #7
	bl 0x0200c7cc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	negs	r0, r0
	movs	r3, #0
	bl 0x0200c8ec
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #72]
	ldr	r3, [pc, #272]
	movs	r0, #80
	str	r3, [r7, #20]
	bl 0x0200c7cc
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200c75c
	movs	r0, #144
	bl 0x0200c9c4
	movs	r0, #20
	bl 0x0200c7cc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200c75c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c844
	movs	r0, #10
	bl 0x0200c7cc
	b.n	.L_02000896
.L_020007f6:
	mov	r2, r8
	cmp	r2, #1
	beq.n	.L_02000800
	cmp	r2, #4
	bne.n	.L_02000896
.L_02000800:
	movs	r0, #23
	movs	r1, #1
	bl 0x0200c8c4
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200c8e4
	movs	r1, #1
	movs	r0, #23
	bl 0x0200c8dc
	bl 0x0200c8f4
	movs	r0, #155
	lsls	r0, r0, #1
	bl 0x0200c9c4
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r2, r7, #0
	str	r3, [r7, #72]
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r3, [pc, #136]
	movs	r0, #20
	str	r3, [r7, #20]
	bl 0x0200c7cc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r1, r1
	negs	r2, r2
	negs	r0, r0
	bl 0x0200c8ec
	movs	r0, #60
	bl 0x0200c7cc
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200c75c
	movs	r0, #144
	bl 0x0200c9c4
	movs	r0, #20
	bl 0x0200c7cc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200c75c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c844
	movs	r0, #10
	bl 0x0200c7cc
.L_02000896:
	movs	r0, #20
	bl 0x0200c7cc
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200c8dc
	bl 0x0200c8f4
	bl 0x0200c7dc
.L_020008b4:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0xffb00000
	.4byte 0xff100000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #146
	lsls	r0, r0, #4
	adds	r0, #255
	sub	sp, #8
	bl 0x0200c6cc
	cmp	r0, #0
	bne.n	.L_0200095e
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200c6dc
	bl 0x0200c7d4
	movs	r0, #0
	bl 0x0200c94c
	movs	r0, #125
	bl 0x0200c9c4
	movs	r0, #194
	movs	r1, #1
	movs	r2, #208
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200c8ec
	bl 0x0200c8f4
	movs	r0, #6
	bl 0x0200c7cc
	movs	r5, #49
	movs	r1, #103
	movs	r2, #4
	movs	r3, #3
	movs	r6, #76
	movs	r0, #49
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c74c
	movs	r0, #6
	bl 0x0200c7cc
	movs	r0, #133
	bl 0x0200c9c4
	movs	r1, #100
	movs	r2, #4
	movs	r3, #3
	movs	r0, #49
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c74c
	movs	r0, #20
	bl 0x0200c7cc
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200c8dc
	bl 0x0200c8f4
	bl 0x0200c7dc
.L_0200095e:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd3c8
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200c7d4
	movs	r0, #0
	bl 0x0200c94c
	cmp	r5, #28
	bhi.n	.L_02000a5a
	ldr	r2, [pc, #220]
	lsls	r3, r5, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x02008a00
	.4byte 0x02008a5a
	.4byte 0x02008a12
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a2e
	.4byte 0x02008a3e
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a5a
	.4byte 0x02008a24
	.4byte 0x20002102
	.4byte 0xff32f003
	.4byte 0xf0034816
	.4byte 0x2000ff3f
	.4byte 0x2102e011
	.4byte 0xf0032002
	.4byte 0x4813ff29
	.4byte 0xff36f003
	.4byte 0xe0082002
	.4byte 0xf0034811
	.4byte 0x201cff31
	.4byte 0x4810e003
	.4byte 0xff2cf003
	.4byte 0x21002005
	.4byte 0xff34f003
	.4byte 0x2104e00d
	.4byte 0xf0032006
	.4byte 0x2021ff07
	.4byte 0xfec0f003
	.4byte 0xf0034809
	.4byte 0x2006ff1d
	.4byte 0xf0032100
	.2byte 0xff25
.L_02000a5a:
	bl 0x0200c7dc
	pop	{r5, pc}
	.4byte 0x0200898c
	.4byte 0x00002abd
	.4byte 0x00002abe
	.4byte 0x00002abf
	.4byte 0x00002ac0
	.2byte 0x2ac1
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #1
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02000ac4
	ldr	r6, [pc, #64]
	movs	r2, #127
	ldr	r3, [r6, #0]
	ldr	r5, [r0, #80]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000a9c
	movs	r1, #129
	movs	r0, #29
	lsls	r1, r1, #1
	bl 0x0200c8d4
.L_02000a9c:
	cmp	r5, #0
	beq.n	.L_02000ac4
	movs	r3, #30
	strb	r3, [r5, #23]
	movs	r3, #252
	strb	r3, [r5, #22]
	movs	r3, #182
	ldr	r2, [r6, #0]
	lsls	r3, r3, #2
	adds	r0, r2, #0
	muls	r0, r3
	bl 0x0200c66c
	cmp	r0, #0
	bge.n	.L_02000abc
	adds	r0, #127
.L_02000abc:
	ldr	r2, [pc, #12]
	asrs	r3, r0, #7
	adds	r3, r3, r2
	strh	r3, [r5, #18]
.L_02000ac4:
	movs	r0, #1
	pop	{r5, r6, pc}
	.4byte 0x0300122c
	.2byte 0xf800
	.2byte 0xffff
	push	{lr}
	bl 0x0200c664
	movs	r3, #7
	ands	r0, r3
	cmp	r0, #7
	bhi.n	.L_02000b20
	ldr	r2, [pc, #84]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x02008b08
	.4byte 0x02008b10
	.4byte 0x02008b18
	.4byte 0x02008b20
	.4byte 0x02008b28
	.4byte 0x02008b20
	.4byte 0x02008b20
	.4byte 0x02008b28
	.4byte 0xf003207e
	.4byte 0xe00eff5b
	.4byte 0xf00320d4
	.4byte 0xe00aff57
	.4byte 0xf0032086
	.2byte 0xff53
	.2byte 0xe006
.L_02000b20:
	movs	r0, #133
	bl 0x0200c9c4
	b.n	.L_02000b2e
	.4byte 0xf003209a
	.2byte 0xff4b
.L_02000b2e:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02008ae8
	.4byte 0x7802305a
	.4byte 0x401323fe
	.4byte 0x20007003
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	movs	r1, #192
	movs	r2, #192
	movs	r0, #26
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r1, #192
	movs	r2, #192
	movs	r0, #27
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r1, #192
	movs	r2, #192
	movs	r0, #0
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r1, #192
	movs	r2, #192
	movs	r0, #2
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r3, #128
	movs	r1, #194
	movs	r2, #202
	lsls	r3, r3, #8
	movs	r0, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r3, #192
	movs	r2, #232
	lsls	r3, r3, #8
	movs	r0, #2
	ldr	r1, [pc, #76]
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r3, #128
	movs	r2, #213
	lsls	r3, r3, #7
	movs	r0, #27
	ldr	r1, [pc, #60]
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r2, #202
	lsls	r2, r2, #16
	movs	r3, #0
	movs	r0, #26
	ldr	r1, [pc, #48]
	bl 0x0200c84c
	ldr	r1, [pc, #48]
	movs	r0, #0
	bl 0x0200c7fc
	ldr	r1, [pc, #44]
	movs	r0, #2
	bl 0x0200c7fc
	ldr	r1, [pc, #40]
	movs	r0, #26
	bl 0x0200c7fc
	ldr	r1, [pc, #36]
	movs	r0, #27
	bl 0x0200c7fc
	ldr	r1, [pc, #32]
	movs	r0, #3
	bl 0x0200c7fc
	pop	{pc}
	.4byte 0x02db0000
	.4byte 0x02e10000
	.4byte 0x0200d58c
	.4byte 0x0200d644
	.4byte 0x0200d5ec
	.4byte 0x0200d69c
	.2byte 0xd548
	.2byte 0x0200
	push	{r5, lr}
	movs	r5, #128
	lsls	r5, r5, #6
	movs	r2, #216
	adds	r3, r5, #0
	movs	r0, #2
	ldr	r1, [pc, #124]
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r2, #219
	adds	r3, r5, #0
	movs	r0, #0
	ldr	r1, [pc, #116]
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r1, #204
	adds	r3, r5, #0
	movs	r0, #3
	lsls	r1, r1, #18
	ldr	r2, [pc, #104]
	movs	r5, #192
	bl 0x0200c84c
	lsls	r5, r5, #8
	movs	r1, #206
	movs	r2, #151
	adds	r3, r5, #0
	movs	r0, #1
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200c84c
	movs	r2, #151
	lsls	r2, r2, #17
	adds	r3, r5, #0
	ldr	r1, [pc, #76]
	movs	r0, #29
	bl 0x0200c84c
	movs	r0, #1
	bl 0x0200c7ec
	movs	r1, #0
	bl 0x0200c754
	ldr	r1, [pc, #60]
	movs	r0, #1
	bl 0x0200c7fc
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	bl 0x0200c65c
	movs	r0, #0
	movs	r1, #38
	bl 0x0200c854
	movs	r0, #2
	movs	r1, #31
	bl 0x0200c854
	movs	r0, #3
	movs	r1, #9
	bl 0x0200c854
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02f50000
	.4byte 0x02f90000
	.4byte 0x011d0000
	.4byte 0x03350000
	.4byte 0x0200d524
	.2byte 0x8a79
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	adds	r7, r0, #0
	bl 0x0200c7d4
	movs	r0, #0
	bl 0x0200c94c
	bl 0x0200c934
	bl 0x0200c944
	ldr	r0, [pc, #920]
	bl 0x0200c88c
	movs	r0, #124
	bl 0x0200c9c4
	movs	r0, #26
	bl 0x0200c7cc
	movs	r0, #217
	bl 0x0200c9c4
	ldr	r1, [pc, #900]
	movs	r0, #4
	bl 0x0200c7fc
	cmp	r7, #0
	beq.n	.L_02000d7e
	movs	r3, #15
	movs	r1, #8
	mov	fp, r3
	str	r1, [sp, #0]
	mov	r8, r1
	movs	r2, #20
	movs	r3, #25
	mov	r1, fp
	str	r2, [sp, #8]
	str	r1, [sp, #16]
	str	r3, [sp, #24]
	movs	r5, #1
	movs	r1, #20
	movs	r3, #15
	movs	r6, #2
	mov	r9, r2
	movs	r0, #3
	movs	r2, #9
	str	r5, [sp, #4]
	str	r6, [sp, #12]
	str	r5, [sp, #20]
	bl 0x0200c8bc
	movs	r0, #155
	lsls	r0, r0, #1
	bl 0x0200c9c4
	movs	r0, #30
	bl 0x0200c7cc
	movs	r0, #144
	bl 0x0200c9c4
	movs	r1, #0
	mov	sl, r1
	movs	r2, #16
	movs	r3, #21
	mov	r1, r8
	str	r2, [sp, #16]
	mov	r2, sl
	str	r1, [sp, #0]
	str	r3, [sp, #8]
	str	r2, [sp, #24]
	movs	r0, #3
	movs	r1, #21
	movs	r2, #9
	movs	r3, #16
	str	r5, [sp, #4]
	str	r6, [sp, #12]
	str	r5, [sp, #20]
	bl 0x0200c8bc
	mov	r3, r8
	mov	r2, fp
	str	r3, [sp, #0]
	mov	r3, sl
	mov	r1, r9
	str	r2, [sp, #16]
	str	r3, [sp, #24]
	movs	r0, #0
	movs	r2, #9
	movs	r3, #15
	str	r6, [sp, #4]
	str	r1, [sp, #8]
	str	r6, [sp, #12]
	str	r5, [sp, #20]
	bl 0x0200c8bc
	b.n	.L_02000df4
.L_02000d7e:
	movs	r1, #8
	movs	r2, #5
	movs	r3, #25
	str	r1, [sp, #0]
	str	r2, [sp, #8]
	str	r3, [sp, #24]
	movs	r5, #1
	movs	r3, #0
	movs	r6, #2
	mov	r8, r1
	mov	sl, r2
	movs	r1, #5
	movs	r2, #9
	movs	r0, #3
	str	r5, [sp, #4]
	str	r6, [sp, #12]
	str	r7, [sp, #16]
	str	r5, [sp, #20]
	bl 0x0200c8bc
	movs	r0, #155
	lsls	r0, r0, #1
	bl 0x0200c9c4
	movs	r0, #30
	bl 0x0200c7cc
	movs	r0, #144
	bl 0x0200c9c4
	mov	r3, r8
	mov	r1, sl
	str	r3, [sp, #0]
	movs	r0, #3
	movs	r2, #9
	movs	r3, #0
	str	r1, [sp, #8]
	str	r5, [sp, #4]
	str	r6, [sp, #12]
	str	r7, [sp, #16]
	str	r5, [sp, #20]
	str	r7, [sp, #24]
	bl 0x0200c8bc
	mov	r2, r8
	mov	r3, sl
	str	r2, [sp, #0]
	str	r3, [sp, #8]
	movs	r0, #0
	movs	r1, #5
	movs	r2, #9
	movs	r3, #0
	str	r6, [sp, #4]
	str	r6, [sp, #12]
	str	r7, [sp, #16]
	str	r5, [sp, #20]
	str	r7, [sp, #24]
	bl 0x0200c8bc
.L_02000df4:
	movs	r0, #4
	bl 0x0200c80c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c8ac
	movs	r3, #128
	movs	r0, #7
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200c964
	movs	r1, #16
	movs	r0, #28
	negs	r1, r1
	movs	r2, #0
	movs	r3, #0
	bl 0x0200c964
	movs	r3, #128
	movs	r0, #6
	movs	r1, #16
	movs	r2, #24
	lsls	r3, r3, #8
	bl 0x0200c964
	movs	r1, #16
	movs	r3, #0
	movs	r2, #24
	movs	r0, #5
	negs	r1, r1
	bl 0x0200c964
	movs	r1, #1
	movs	r0, #5
	bl 0x0200c8dc
	movs	r0, #5
	bl 0x0200c83c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #28
	bl 0x0200c8cc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c8ac
	movs	r1, #176
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c8ac
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x0200c8cc
	movs	r1, #208
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200c8ac
	movs	r1, #0
	movs	r0, #7
	bl 0x0200c894
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c8ac
	movs	r0, #4
	movs	r1, #0
	bl 0x0200c7e4
	cmp	r0, #0
	bne.n	.L_02000ee2
	movs	r1, #3
	movs	r0, #28
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #0
	movs	r2, #5
	movs	r0, #28
	bl 0x0200c89c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000f0c
.L_02000ee2:
	movs	r1, #4
	movs	r0, #28
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #28
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
.L_02000f0c:
	movs	r0, #5
	movs	r1, #2
	bl 0x0200c86c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #4
	bl 0x0200c8ac
	movs	r0, #5
	bl 0x0200c7cc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200c8cc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200c8cc
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #4
	ldr	r1, [pc, #280]
	adds	r2, #153
	bl 0x0200c7f4
	cmp	r7, #0
	beq.n	.L_02000f68
	ldr	r1, [pc, #272]
	b.n	.L_02000f6a
.L_02000f68:
	ldr	r1, [pc, #272]
.L_02000f6a:
	movs	r0, #4
	bl 0x0200c7fc
	movs	r0, #30
	bl 0x0200c7cc
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c8ac
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c8ac
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c8ac
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200c8ac
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #7
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c854
	movs	r1, #3
	movs	r0, #28
	bl 0x0200c854
	movs	r0, #30
	bl 0x0200c7cc
	movs	r1, #128
	movs	r2, #128
	movs	r0, #5
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c7f4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #7
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c7f4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #6
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c7f4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #28
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c7f4
	cmp	r7, #0
	beq.n	.L_02001012
	ldr	r1, [pc, #112]
	b.n	.L_02001014
.L_02001012:
	ldr	r1, [pc, #112]
.L_02001014:
	movs	r0, #28
	bl 0x0200c7fc
	cmp	r7, #0
	beq.n	.L_02001022
	ldr	r1, [pc, #104]
	b.n	.L_02001024
.L_02001022:
	ldr	r1, [pc, #104]
.L_02001024:
	movs	r0, #7
	bl 0x0200c7fc
	cmp	r7, #0
	beq.n	.L_02001032
	ldr	r1, [pc, #96]
	b.n	.L_02001034
.L_02001032:
	ldr	r1, [pc, #96]
.L_02001034:
	movs	r0, #5
	bl 0x0200c7fc
	cmp	r7, #0
	beq.n	.L_02001042
	ldr	r1, [pc, #88]
	b.n	.L_02001044
.L_02001042:
	ldr	r1, [pc, #88]
.L_02001044:
	movs	r0, #6
	bl 0x0200c7fc
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	bl 0x0200c8e4
	cmp	r7, #0
	beq.n	.L_020010ac
	ldr	r0, [pc, #68]
	ldr	r1, [pc, #68]
	ldr	r2, [pc, #72]
	movs	r3, #1
	bl 0x0200c8ec
	bl 0x0200c8f4
	b.n	.L_020010bc
	.4byte 0x00002a32
	.4byte 0x0200d6ec
	.4byte 0x00013333
	.4byte 0x0200d748
	.4byte 0x0200d78c
	.4byte 0x0200d7d0
	.4byte 0x0200d800
	.4byte 0x0200d830
	.4byte 0x0200d860
	.4byte 0x0200d890
	.4byte 0x0200d8d8
	.4byte 0x0200d920
	.4byte 0x0200d950
	.4byte 0x02a50000
	.4byte 0xffc00000
	.2byte 0x0000
	.2byte 0x011f
.L_020010ac:
	ldr	r0, [pc, #1012]
	ldr	r1, [pc, #1016]
	ldr	r2, [pc, #1016]
	movs	r3, #1
	bl 0x0200c8ec
	bl 0x0200c8f4
.L_020010bc:
	movs	r0, #4
	bl 0x0200c804
	movs	r0, #7
	bl 0x0200c804
	movs	r0, #5
	bl 0x0200c804
	movs	r0, #6
	bl 0x0200c804
	movs	r0, #28
	bl 0x0200c804
	movs	r6, #128
	movs	r0, #20
	bl 0x0200c7cc
	lsls	r6, r6, #6
	movs	r2, #232
	adds	r3, r6, #0
	lsls	r2, r2, #16
	movs	r0, #2
	ldr	r1, [pc, #960]
	bl 0x0200c84c
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200c8e4
	movs	r0, #200
	movs	r2, #135
	movs	r3, #1
	lsls	r2, r2, #17
	movs	r1, #0
	lsls	r0, r0, #18
	bl 0x0200c8ec
	bl 0x0200c8f4
	movs	r0, #39
	bl 0x0200c9c4
	movs	r0, #50
	bl 0x0200c7cc
	movs	r0, #0
	movs	r1, #2
	bl 0x0200c86c
	movs	r2, #5
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c89c
	movs	r0, #3
	movs	r1, #0
	bl 0x0200c854
	movs	r1, #160
	movs	r0, #3
	lsls	r1, r1, #8
	bl 0x0200c8b4
	movs	r2, #5
	movs	r0, #3
	movs	r1, #0
	bl 0x0200c89c
	adds	r1, r6, #0
	movs	r0, #3
	bl 0x0200c8b4
	movs	r0, #3
	movs	r1, #9
	bl 0x0200c854
	movs	r2, #5
	movs	r0, #3
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #2
	movs	r0, #1
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #1
	movs	r1, #0
	bl 0x0200c89c
	movs	r0, #3
	movs	r1, #3
	bl 0x0200c864
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #3
	bl 0x0200c8d4
	movs	r0, #40
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #3
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #2
	movs	r0, #1
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r1, #0
	movs	r2, #5
	movs	r0, #1
	bl 0x0200c89c
	movs	r0, #3
	bl 0x0200c7ec
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #200
	movs	r2, #192
	lsls	r1, r1, #5
	lsls	r2, r2, #4
	strb	r3, [r0, #0]
	adds	r1, #153
	movs	r0, #3
	adds	r2, #204
	bl 0x0200c7f4
	movs	r2, #2
	movs	r1, #4
	movs	r0, #3
	bl 0x0200c834
	movs	r0, #3
	bl 0x0200c83c
	movs	r0, #3
	movs	r1, #9
	bl 0x0200c854
	movs	r0, #3
	movs	r1, #2
	bl 0x0200c86c
	movs	r1, #2
	movs	r2, #2
	negs	r1, r1
	movs	r0, #3
	bl 0x0200c834
	movs	r0, #3
	bl 0x0200c83c
	movs	r0, #3
	movs	r1, #9
	bl 0x0200c854
	movs	r0, #3
	movs	r1, #2
	bl 0x0200c86c
	movs	r2, #1
	movs	r1, #2
	movs	r0, #3
	bl 0x0200c834
	movs	r0, #3
	bl 0x0200c83c
	movs	r0, #3
	movs	r1, #9
	bl 0x0200c854
	movs	r0, #3
	movs	r1, #2
	bl 0x0200c86c
	movs	r1, #2
	movs	r2, #1
	negs	r2, r2
	negs	r1, r1
	movs	r0, #3
	bl 0x0200c834
	movs	r0, #3
	bl 0x0200c83c
	movs	r0, #3
	movs	r1, #9
	bl 0x0200c854
	movs	r1, #2
	movs	r0, #3
	bl 0x0200c86c
	movs	r0, #40
	bl 0x0200c7cc
	movs	r2, #2
	movs	r1, #0
	negs	r2, r2
	movs	r0, #3
	bl 0x0200c834
	movs	r0, #3
	bl 0x0200c83c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #3
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r0, #3
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #3
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #3
	bl 0x0200c7ec
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	movs	r1, #2
	movs	r2, #8
	strb	r3, [r0, #0]
	negs	r2, r2
	negs	r1, r1
	movs	r0, #3
	bl 0x0200c834
	movs	r0, #3
	bl 0x0200c83c
	movs	r0, #3
	movs	r1, #1
	bl 0x0200c854
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #3
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #3
	movs	r1, #0
	bl 0x0200c89c
	movs	r0, #0
	movs	r1, #1
	bl 0x0200c854
	movs	r0, #2
	movs	r1, #1
	bl 0x0200c854
	movs	r1, #2
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c87c
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #0
	bl 0x0200c8cc
	movs	r0, #0
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #2
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	ldr	r1, [pc, #388]
	movs	r0, #3
	bl 0x0200c7fc
	movs	r2, #128
	adds	r3, r6, #0
	movs	r0, #26
	ldr	r1, [pc, #376]
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r2, #128
	adds	r3, r6, #0
	ldr	r1, [pc, #368]
	lsls	r2, r2, #16
	movs	r0, #27
	bl 0x0200c84c
	movs	r0, #78
	bl 0x0200c9c4
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c8cc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #2
	bl 0x0200c8cc
	movs	r1, #160
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c8ac
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #2
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #180
	movs	r2, #210
	movs	r3, #1
	lsls	r0, r0, #18
	movs	r1, #0
	lsls	r2, r2, #16
	bl 0x0200c8ec
	movs	r1, #128
	adds	r2, r6, #0
	movs	r0, #26
	lsls	r1, r1, #7
	bl 0x0200c7f4
	movs	r1, #128
	adds	r2, r6, #0
	movs	r0, #27
	lsls	r1, r1, #7
	bl 0x0200c7f4
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r0, #26
	adds	r1, #202
	movs	r2, #193
	bl 0x0200c824
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r2, #209
	movs	r0, #27
	adds	r1, #186
	bl 0x0200c824
	ldr	r1, [pc, #228]
	movs	r0, #26
	bl 0x0200c7fc
	ldr	r1, [pc, #224]
	movs	r0, #27
	bl 0x0200c7fc
	movs	r0, #20
	bl 0x0200c7cc
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #2
	bl 0x0200c8cc
	movs	r1, #0
	movs	r2, #5
	movs	r0, #2
	bl 0x0200c89c
	movs	r0, #34
	bl 0x0200c9c4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #0
	bl 0x0200c8cc
	movs	r1, #0
	movs	r2, #5
	movs	r0, #0
	bl 0x0200c89c
	movs	r0, #26
	bl 0x0200c804
	movs	r0, #27
	bl 0x0200c804
	movs	r0, #27
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #2
	bl 0x0200c8cc
	movs	r0, #2
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #2
	adds	r1, #255
	b.n	.L_020014c8
	.4byte 0x03b60000
	.4byte 0xffc00000
	.4byte 0x011f0000
	.4byte 0x02e50000
	.4byte 0x0200d548
	.4byte 0x02990000
	.4byte 0x02810000
	.4byte 0x0200c9cc
	.2byte 0xc9dc
	.2byte 0x0200
.L_020014c8:
	movs	r2, #0
	movs	r0, #2
	bl 0x0200c8cc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #0
	bl 0x0200c8cc
	movs	r1, #2
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c87c
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #0
	bl 0x0200c8cc
	movs	r1, #160
	movs	r2, #0
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200c8ac
	movs	r1, #160
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c8b4
	movs	r0, #0
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #27
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #2
	bl 0x0200c8cc
	movs	r2, #5
	movs	r0, #2
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #26
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #2
	movs	r0, #26
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #0
	bl 0x0200c8cc
	movs	r2, #5
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #2
	movs	r0, #27
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c89c
	adds	r1, r6, #0
	movs	r0, #26
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #2
	bl 0x0200c8cc
	movs	r0, #2
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #0
	bl 0x0200c8cc
	movs	r0, #0
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #27
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #2
	movs	r0, #2
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #2
	movs	r1, #0
	bl 0x0200c89c
	movs	r0, #27
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #26
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #2
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	bl 0x0200c8cc
	movs	r0, #2
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #0
	bl 0x0200c8cc
	adds	r1, r6, #0
	movs	r0, #0
	bl 0x0200c8b4
	movs	r0, #0
	movs	r1, #38
	bl 0x0200c854
	movs	r0, #196
	movs	r2, #137
	movs	r1, #0
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200c8ec
	bl 0x0200c8f4
	movs	r0, #60
	bl 0x0200c7cc
	movs	r0, #180
	movs	r2, #210
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r0, r0, #18
	movs	r1, #0
	bl 0x0200c8ec
	bl 0x0200c8f4
	movs	r0, #0
	movs	r1, #1
	bl 0x0200c854
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #0
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #0
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #27
	movs	r2, #0
	movs	r0, #26
	bl 0x0200c87c
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #26
	bl 0x0200c8cc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #2
	bl 0x0200c8cc
	movs	r0, #2
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	adds	r1, r6, #0
	movs	r2, #0
	movs	r0, #26
	bl 0x0200c8ac
	movs	r0, #27
	adds	r1, r6, #0
	bl 0x0200c8b4
	movs	r0, #26
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #27
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #2
	bl 0x0200c8cc
	movs	r0, #2
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r2, #5
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #2
	movs	r0, #0
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r0, #0
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r2, #5
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #2
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #2
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #27
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #27
	movs	r2, #5
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #27
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c7cc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r1, #128
	movs	r2, #128
	movs	r0, #27
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r1, #24
	movs	r2, #24
	negs	r2, r2
	negs	r1, r1
	movs	r0, #27
	bl 0x0200c974
	movs	r0, #5
	bl 0x0200c7cc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #27
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #27
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #0
	movs	r0, #27
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #27
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r2, #5
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c89c
	adds	r1, r6, #0
	movs	r0, #27
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #26
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #2
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c87c
	movs	r0, #10
	bl 0x0200c7cc
	movs	r3, #12
	movs	r2, #6
	movs	r1, #2
	movs	r0, #17
	movs	r4, #13
	str	r2, [sp, #0]
	str	r1, [sp, #4]
	str	r0, [sp, #8]
	str	r3, [sp, #16]
	str	r3, [sp, #20]
	movs	r5, #0
	movs	r3, #15
	movs	r0, #0
	movs	r1, #20
	movs	r2, #7
	str	r4, [sp, #12]
	str	r5, [sp, #24]
	bl 0x0200c8bc
	movs	r1, #160
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c8ac
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c8ac
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r2, #5
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c89c
	adds	r1, r6, #0
	movs	r0, #26
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r2, #24
	movs	r0, #27
	movs	r1, #24
	bl 0x0200c96c
	ldr	r1, [pc, #672]
	movs	r0, #27
	bl 0x0200c7fc
	movs	r1, #0
	movs	r2, #5
	movs	r0, #27
	bl 0x0200c89c
	movs	r0, #27
	bl 0x0200c804
	movs	r2, #30
	movs	r0, #26
	movs	r1, #27
	bl 0x0200c87c
	movs	r0, #26
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #27
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #30
	bl 0x0200c7cc
	movs	r1, #192
	movs	r2, #192
	movs	r0, #26
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r1, #192
	movs	r2, #192
	movs	r0, #27
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r1, #192
	movs	r2, #192
	movs	r0, #0
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r1, #192
	movs	r2, #192
	movs	r0, #2
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r1, #241
	lsls	r1, r1, #1
	movs	r0, #26
	adds	r1, #255
	movs	r2, #202
	bl 0x0200c824
	movs	r1, #238
	lsls	r1, r1, #1
	movs	r2, #213
	movs	r0, #27
	adds	r1, #255
	bl 0x0200c824
	ldr	r1, [pc, #532]
	movs	r0, #27
	bl 0x0200c7fc
	ldr	r1, [pc, #528]
	movs	r0, #26
	bl 0x0200c7fc
	movs	r0, #5
	bl 0x0200c7cc
	movs	r1, #194
	movs	r0, #0
	lsls	r1, r1, #2
	movs	r2, #202
	bl 0x0200c824
	movs	r1, #238
	lsls	r1, r1, #1
	movs	r2, #232
	movs	r0, #2
	adds	r1, #255
	bl 0x0200c824
	ldr	r1, [pc, #492]
	movs	r0, #0
	bl 0x0200c7fc
	ldr	r1, [pc, #488]
	movs	r0, #2
	bl 0x0200c7fc
	movs	r0, #0
	bl 0x0200c804
	movs	r0, #2
	bl 0x0200c804
	movs	r0, #26
	bl 0x0200c804
	movs	r0, #27
	bl 0x0200c804
	movs	r0, #40
	bl 0x0200c7cc
	bl 0x02008b48
	movs	r0, #200
	bl 0x0200c7cc
	movs	r1, #1
	movs	r0, #4
	bl 0x0200c8dc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #2
	movs	r0, #28
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200c8cc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x0200c8cc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #5
	bl 0x0200c8cc
	movs	r2, #5
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #28
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #5
	movs	r1, #4
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #7
	movs	r1, #4
	movs	r2, #0
	bl 0x0200c874
	movs	r2, #0
	movs	r1, #4
	movs	r0, #6
	bl 0x0200c874
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #7
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #7
	movs	r1, #2
	bl 0x0200c854
	movs	r0, #4
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02001bb8
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200c814
.L_02001bb8:
	movs	r0, #6
	movs	r1, #2
	bl 0x0200c854
	movs	r0, #4
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02001bd8
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200c814
.L_02001bd8:
	movs	r0, #5
	movs	r1, #2
	bl 0x0200c854
	movs	r0, #4
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02001bf8
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200c814
.L_02001bf8:
	movs	r0, #28
	movs	r1, #2
	bl 0x0200c854
	movs	r0, #4
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02001c18
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #28
	bl 0x0200c814
.L_02001c18:
	ldr	r5, [pc, #92]
	movs	r0, #7
	adds	r1, r5, #0
	bl 0x0200c7fc
	adds	r1, r5, #0
	movs	r0, #6
	bl 0x0200c7fc
	adds	r1, r5, #0
	movs	r0, #5
	bl 0x0200c7fc
	adds	r1, r5, #0
	movs	r0, #28
	bl 0x0200c7fc
	movs	r0, #7
	bl 0x0200c804
	movs	r0, #6
	bl 0x0200c804
	movs	r0, #5
	bl 0x0200c804
	movs	r0, #28
	bl 0x0200c804
	bl 0x0200c7dc
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200c9ec
	.4byte 0x0200c9fc
	.4byte 0x0200ca18
	.4byte 0x0200ca34
	.4byte 0x0200ca50
	.2byte 0xd99c
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x0200c7d4
	movs	r0, #0
	bl 0x0200c94c
	movs	r3, #128
	movs	r1, #194
	movs	r2, #202
	lsls	r3, r3, #8
	movs	r0, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r3, #160
	movs	r2, #232
	lsls	r3, r3, #8
	movs	r0, #2
	ldr	r1, [pc, #376]
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r3, #128
	movs	r2, #213
	lsls	r3, r3, #7
	movs	r0, #27
	ldr	r1, [pc, #360]
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r2, #202
	movs	r3, #0
	lsls	r2, r2, #16
	movs	r0, #26
	ldr	r1, [pc, #348]
	bl 0x0200c84c
	movs	r0, #0
	movs	r1, #1
	bl 0x0200c854
	movs	r0, #2
	movs	r1, #1
	bl 0x0200c854
	movs	r0, #26
	movs	r1, #1
	bl 0x0200c854
	movs	r1, #1
	movs	r0, #27
	bl 0x0200c854
	bl 0x0200c934
	bl 0x0200c944
	ldr	r0, [pc, #304]
	bl 0x0200c88c
	movs	r0, #2
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200c8cc
	movs	r2, #177
	movs	r3, #1
	lsls	r2, r2, #16
	movs	r1, #0
	ldr	r0, [pc, #272]
	bl 0x0200c8ec
	bl 0x0200c8f4
	movs	r0, #30
	bl 0x0200c7cc
	movs	r0, #2
	bl 0x0200c7ec
	movs	r1, #0
	bl 0x0200c754
	movs	r1, #32
	movs	r0, #2
	bl 0x0200c854
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200c8b4
	movs	r0, #0
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #24
	movs	r2, #0
	movs	r0, #26
	bl 0x0200c96c
	movs	r0, #20
	bl 0x0200c7cc
	ldr	r6, [pc, #176]
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r6, r3
	movs	r3, #6
	strb	r3, [r2, #0]
	ldr	r5, [pc, #168]
	movs	r1, #77
	adds	r0, r5, #0
	bl 0x0200c90c
	movs	r1, #77
	adds	r0, r5, #0
	bl 0x0200c914
	movs	r0, #4
	bl 0x0200c7c4
	movs	r0, #7
	bl 0x0200c7c4
	movs	r0, #5
	bl 0x0200c7c4
	movs	r0, #6
	bl 0x0200c7c4
	movs	r0, #2
	bl 0x0200c7c4
	movs	r0, #0
	bl 0x0200c7c4
	movs	r0, #2
	bl 0x0200c7bc
	movs	r0, #0
	bl 0x0200c7bc
	movs	r0, #3
	bl 0x0200c7c4
	movs	r0, #1
	bl 0x0200c7c4
	ldr	r1, [pc, #92]
	movs	r0, #2
	bl 0x0200c7ac
	ldr	r1, [pc, #84]
	movs	r0, #0
	bl 0x0200c7ac
	movs	r0, #0
	bl 0x0200c78c
	movs	r0, #2
	bl 0x0200c78c
	movs	r1, #173
	movs	r0, #0
	bl 0x0200c7ac
	ldr	r1, [pc, #56]
	movs	r0, #2
	bl 0x0200c7b4
	ldr	r1, [pc, #48]
	movs	r0, #0
	bl 0x0200c7b4
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	movs	r3, #0
	str	r3, [r6, #0]
	movs	r0, #102
	movs	r1, #4
	bl 0x0200c904
	pop	{r5, r6, pc}
	.4byte 0x02db0000
	.4byte 0x02e10000
	.4byte 0x00002a86
	.4byte 0x02c10000
	.4byte 0x02000240
	.4byte 0x00000107
	.2byte 0xd8f0
	.2byte 0xffff
	.2byte 0xb520
	movs	r5, #160
	lsls	r5, r5, #8
	movs	r2, #219
	adds	r3, r5, #0
	movs	r0, #0
	ldr	r1, [pc, #116]
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r2, #232
	adds	r3, r5, #0
	lsls	r2, r2, #16
	ldr	r1, [pc, #108]
	movs	r0, #2
	bl 0x0200c84c
	movs	r0, #0
	bl 0x0200c7ec
	movs	r1, #0
	bl 0x0200c754
	movs	r1, #39
	movs	r0, #0
	bl 0x0200c854
	movs	r0, #2
	bl 0x0200c7ec
	movs	r1, #0
	bl 0x0200c754
	movs	r0, #2
	movs	r1, #32
	bl 0x0200c854
	movs	r3, #128
	movs	r2, #203
	lsls	r3, r3, #7
	movs	r0, #5
	ldr	r1, [pc, #48]
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r2, #232
	movs	r0, #6
	ldr	r1, [pc, #44]
	lsls	r2, r2, #16
	movs	r3, #0
	bl 0x0200c84c
	movs	r3, #128
	movs	r2, #203
	lsls	r3, r3, #6
	movs	r0, #28
	ldr	r1, [pc, #28]
	lsls	r2, r2, #16
	bl 0x0200c84c
	ldr	r1, [pc, #24]
	movs	r0, #3
	bl 0x0200c7fc
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02f90000
	.4byte 0x02e50000
	.4byte 0x02d50000
	.4byte 0x02d90000
	.2byte 0xd548
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	bl 0x0200c7d4
	movs	r0, #0
	bl 0x0200c94c
	movs	r2, #209
	movs	r1, #0
	ldr	r0, [pc, #1016]
	lsls	r2, r2, #16
	movs	r3, #0
	bl 0x0200c8ec
	ldr	r3, [pc, #1012]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #4
	str	r2, [r3, #0]
	movs	r0, #0
	bl 0x0200c7c4
	movs	r0, #2
	bl 0x0200c7c4
	movs	r0, #5
	bl 0x0200c7c4
	movs	r0, #6
	bl 0x0200c7c4
	movs	r0, #7
	bl 0x0200c7c4
	movs	r0, #3
	bl 0x0200c7c4
	movs	r0, #1
	bl 0x0200c7c4
	movs	r0, #4
	bl 0x0200c7bc
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c7f4
	movs	r3, #128
	movs	r1, #166
	movs	r2, #176
	lsls	r3, r3, #7
	movs	r0, #4
	lsls	r1, r1, #18
	lsls	r2, r2, #15
	movs	r6, #128
	mov	r8, r3
	lsls	r6, r6, #6
	bl 0x0200c84c
	movs	r2, #208
	adds	r3, r6, #0
	movs	r0, #27
	ldr	r1, [pc, #912]
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r2, #195
	adds	r3, r6, #0
	movs	r0, #26
	ldr	r1, [pc, #888]
	lsls	r2, r2, #16
	movs	r5, #160
	bl 0x0200c84c
	lsls	r5, r5, #8
	movs	r2, #219
	adds	r3, r5, #0
	movs	r0, #0
	ldr	r1, [pc, #884]
	lsls	r2, r2, #16
	bl 0x0200c84c
	movs	r2, #232
	adds	r3, r5, #0
	lsls	r2, r2, #16
	movs	r0, #2
	ldr	r1, [pc, #872]
	bl 0x0200c84c
	ldr	r1, [pc, #868]
	movs	r0, #3
	bl 0x0200c7fc
	movs	r0, #0
	bl 0x0200c7ec
	movs	r1, #0
	bl 0x0200c754
	movs	r1, #39
	movs	r0, #0
	bl 0x0200c854
	movs	r0, #2
	bl 0x0200c7ec
	movs	r1, #0
	bl 0x0200c754
	movs	r1, #32
	movs	r0, #2
	bl 0x0200c854
	bl 0x0200c934
	bl 0x0200c944
	ldr	r0, [pc, #816]
	bl 0x0200c88c
	movs	r2, #5
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #8
	movs	r0, #26
	bl 0x0200c854
	movs	r0, #40
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #0
	movs	r0, #27
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c89c
	movs	r0, #0
	movs	r1, #2
	bl 0x0200c864
	movs	r1, #2
	movs	r0, #2
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r2, #5
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #27
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #1
	movs	r0, #26
	bl 0x0200c854
	movs	r0, #30
	bl 0x0200c7cc
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #26
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #26
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	bl 0x0200c8fc
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #1
	movs	r0, #4
	bl 0x0200c8dc
	movs	r0, #20
	bl 0x0200c7cc
	mov	r3, r8
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c964
	movs	r0, #6
	movs	r1, #1
	bl 0x0200c8dc
	mov	r3, r8
	movs	r0, #7
	movs	r1, #16
	movs	r2, #0
	bl 0x0200c964
	movs	r1, #16
	adds	r3, r6, #0
	movs	r0, #28
	negs	r1, r1
	movs	r2, #24
	bl 0x0200c964
	movs	r1, #16
	adds	r3, r6, #0
	movs	r0, #5
	negs	r1, r1
	movs	r2, #0
	bl 0x0200c964
	movs	r0, #4
	movs	r1, #16
	movs	r2, #24
	bl 0x0200c974
	mov	r1, r8
	movs	r0, #4
	movs	r2, #0
	bl 0x0200c8ac
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #28
	bl 0x0200c8cc
	movs	r2, #5
	movs	r0, #28
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #0
	movs	r0, #28
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c8ac
	movs	r2, #35
	movs	r0, #28
	movs	r1, #0
	bl 0x0200c89c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #28
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r0, #4
	adds	r1, #255
	movs	r2, #186
	bl 0x0200c824
	movs	r1, #206
	lsls	r1, r1, #1
	movs	r2, #202
	movs	r0, #28
	adds	r1, #255
	bl 0x0200c824
	ldr	r1, [pc, #420]
	movs	r0, #4
	bl 0x0200c7fc
	ldr	r1, [pc, #416]
	movs	r0, #28
	bl 0x0200c7fc
	movs	r1, #128
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c8ac
	movs	r1, #128
	movs	r0, #27
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c8ac
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200c8cc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #5
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #5
	movs	r1, #7
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #6
	movs	r1, #7
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #7
	movs	r1, #5
	movs	r2, #0
	bl 0x0200c874
	movs	r2, #5
	movs	r0, #7
	movs	r1, #0
	bl 0x0200c89c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #7
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #40
	bl 0x0200c96c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #40
	bl 0x0200c96c
	movs	r2, #40
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c96c
	ldr	r1, [pc, #216]
	movs	r0, #7
	bl 0x0200c7fc
	ldr	r1, [pc, #212]
	movs	r0, #6
	bl 0x0200c7fc
	ldr	r1, [pc, #208]
	movs	r0, #5
	bl 0x0200c7fc
	movs	r1, #1
	movs	r0, #4
	bl 0x0200c8dc
	bl 0x0200c8f4
	movs	r0, #20
	bl 0x0200c7cc
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r2, #5
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #26
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #26
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	mov	r1, r8
	movs	r0, #26
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #26
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #26
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #128
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200c8b4
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	b.n	.L_02002310
	.4byte 0x02e10000
	.4byte 0x02000240
	.4byte 0x02cd0000
	.4byte 0x02f90000
	.4byte 0x02e50000
	.4byte 0x0200d548
	.4byte 0x00002a8a
	.4byte 0x0200ca6c
	.4byte 0x0200ca90
	.4byte 0x0200cab4
	.4byte 0x0200cae8
	.2byte 0xcb1c
	.2byte 0x0200
.L_02002310:
	bl 0x0200c89c
	movs	r0, #27
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #27
	movs	r2, #5
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #2
	movs	r0, #28
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #6
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #27
	bl 0x0200c8cc
	movs	r1, #0
	movs	r0, #27
	bl 0x0200c894
	movs	r0, #28
	movs	r1, #4
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #5
	movs	r1, #4
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #6
	movs	r1, #4
	movs	r2, #0
	bl 0x0200c874
	movs	r1, #4
	movs	r0, #7
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #4
	movs	r1, #0
	bl 0x0200c7e4
	cmp	r0, #0
	bne.n	.L_020023ea
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r2, #5
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c89c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002412
.L_020023ea:
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #27
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
.L_02002412:
	movs	r0, #7
	movs	r1, #26
	movs	r2, #0
	bl 0x0200c874
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x0200c8cc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #26
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #27
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #5
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #27
	movs	r2, #0
	movs	r0, #26
	bl 0x0200c87c
	movs	r0, #26
	bl 0x0200c804
	movs	r0, #27
	bl 0x0200c804
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #26
	bl 0x0200c8cc
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r0, #28
	movs	r1, #27
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #28
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #26
	movs	r1, #28
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #27
	movs	r1, #28
	movs	r2, #0
	bl 0x0200c874
	movs	r1, #0
	movs	r2, #5
	movs	r0, #28
	bl 0x0200c89c
	movs	r0, #60
	bl 0x0200c7cc
	movs	r0, #27
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #2
	movs	r0, #26
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #27
	movs	r1, #4
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200c8cc
	movs	r2, #5
	movs	r1, #0
	movs	r0, #27
	bl 0x0200c89c
	movs	r0, #20
	bl 0x0200c7cc
	movs	r1, #2
	movs	r0, #26
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #26
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #26
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r2, #5
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #27
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #7
	movs	r1, #4
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #6
	movs	r1, #5
	movs	r2, #0
	bl 0x0200c87c
	movs	r1, #28
	movs	r2, #0
	movs	r0, #4
	bl 0x0200c87c
	movs	r0, #4
	bl 0x0200c804
	movs	r0, #28
	bl 0x0200c804
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
.L_020025fa:
	movs	r0, #4
	bl 0x0200c8cc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200c8cc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200c8cc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200c8cc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #28
	bl 0x0200c8cc
	movs	r0, #4
	movs	r1, #26
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #5
	movs	r1, #26
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #6
	movs	r1, #26
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #7
	movs	r1, #26
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #28
	movs	r1, #26
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #26
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #0
	movs	r0, #26
	bl 0x0200c894
	movs	r0, #4
	movs	r1, #0
	bl 0x0200c7e4
	cmp	r0, #0
	bne.n	.L_020026b6
	movs	r1, #4
	movs	r0, #26
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r0, #26
	movs	r2, #5
	movs	r1, #0
	bl 0x0200c89c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020026e0
.L_020026b6:
	movs	r1, #4
	movs	r0, #26
	bl 0x0200c854
	movs	r0, #33
	bl 0x0200c7cc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #26
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
.L_020026e0:
	movs	r1, #4
	movs	r2, #0
	movs	r0, #28
	bl 0x0200c87c
	movs	r0, #28
	bl 0x0200c804
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #28
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #28
	movs	r2, #0
	movs	r0, #4
	bl 0x0200c87c
	movs	r0, #4
	bl 0x0200c804
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200c8cc
	movs	r0, #28
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #40
	bl 0x0200c7cc
	movs	r0, #4
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #30
	bl 0x0200c7cc
	movs	r1, #27
	movs	r2, #0
	movs	r0, #26
	bl 0x0200c87c
	movs	r0, #26
	bl 0x0200c804
	movs	r0, #27
	bl 0x0200c804
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #27
	bl 0x0200c8cc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #4
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #4
	movs	r1, #27
	movs	r2, #0
	bl 0x0200c874
	movs	r1, #222
	lsls	r1, r1, #1
	movs	r0, #27
	adds	r1, #255
	movs	r2, #202
	bl 0x0200c82c
	movs	r0, #27
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c8cc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200c8cc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #2
	bl 0x0200c8cc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200c8cc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #28
	bl 0x0200c8cc
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #28
	bl 0x0200c8cc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c8ac
	movs	r0, #28
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #4
	movs	r1, #7
	movs	r2, #0
	bl 0x0200c87c
	movs	r1, #5
	movs	r2, #0
	movs	r0, #6
	bl 0x0200c87c
	movs	r0, #6
	bl 0x0200c804
	movs	r0, #5
	bl 0x0200c804
	movs	r0, #40
	bl 0x0200c7cc
	movs	r0, #4
	movs	r1, #26
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #7
	movs	r1, #26
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #6
	movs	r1, #26
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #5
	movs	r1, #26
	movs	r2, #0
	bl 0x0200c874
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c8ac
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #7
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200c7f4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #27
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r0, #7
	movs	r1, #8
	movs	r2, #24
	bl 0x0200c96c
	movs	r1, #8
	movs	r2, #4
	movs	r0, #27
	bl 0x0200c974
	movs	r0, #7
	bl 0x0200c83c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #26
	bl 0x0200c8cc
	movs	r0, #26
	movs	r1, #7
	movs	r2, #0
	bl 0x0200c874
	movs	r0, #27
	movs	r1, #7
	movs	r2, #0
	bl 0x0200c874
	movs	r2, #5
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #2
	movs	r0, #0
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r0, #0
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c874
	movs	r2, #0
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c874
	movs	r1, #2
	movs	r0, #0
	bl 0x0200c86c
	movs	r0, #5
	bl 0x0200c7cc
	movs	r2, #5
	movs	r1, #0
	movs	r0, #0
	bl 0x0200c89c
	movs	r0, #40
	bl 0x0200c7cc
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #4
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #4
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #26
	movs	r1, #4
	bl 0x0200c97c
	movs	r0, #27
	movs	r1, #4
	bl 0x0200c97c
	movs	r0, #4
	movs	r1, #5
	movs	r2, #32
	bl 0x0200c974
	movs	r2, #2
	negs	r2, r2
	movs	r1, #56
	movs	r0, #4
	bl 0x0200c974
	movs	r0, #30
	bl 0x0200c7cc
	movs	r1, #2
	movs	r0, #0
	bl 0x0200c864
	movs	r0, #4
	bl 0x0200c7cc
	movs	r1, #2
	movs	r0, #4
	bl 0x0200c86c
	movs	r0, #223
	bl 0x0200c79c
	movs	r0, #245
	bl 0x0200c794
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r0, [r3, #0]
	movs	r1, #5
	adds	r2, r0, #1
	lsls	r0, r0, #16
	strh	r2, [r3, #0]
	asrs	r0, r0, #16
	bl 0x0200c77c
	movs	r5, #0
	movs	r6, #216
	b.n	.L_020029f4
.L_020029f0:
	adds	r6, #2
	adds	r5, #1
.L_020029f4:
	cmp	r5, #14
	bgt.n	.L_02002a18
	movs	r0, #4
	bl 0x0200c6c4
	ldr	r3, [pc, #20]
	ldrh	r2, [r0, r6]
	ands	r3, r2
	cmp	r3, #245
	bne.n	.L_020029f0
	movs	r0, #4
	adds	r1, r5, #0
	bl 0x0200c7a4
	b.n	.L_02002a18
	.2byte 0x0000
	.2byte 0x01ff
	.2byte 0x0000
.L_02002a18:
	movs	r1, #3
	movs	r0, #27
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #27
	movs	r2, #0
	movs	r0, #4
	bl 0x0200c874
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #4
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #20
	bl 0x0200c7cc
	movs	r1, #128
	movs	r2, #128
	movs	r0, #7
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c7f4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #26
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200c7f4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #27
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200c7f4
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #7
	movs	r0, #4
	lsls	r1, r1, #8
	bl 0x0200c7f4
	ldr	r6, [pc, #440]
	movs	r0, #26
	adds	r1, r6, #0
	bl 0x0200c7fc
	movs	r0, #40
	bl 0x0200c7cc
	movs	r1, #16
	movs	r2, #0
	negs	r1, r1
	movs	r0, #7
	bl 0x0200c834
	movs	r0, #10
	bl 0x0200c7cc
	adds	r1, r6, #0
	movs	r0, #27
	bl 0x0200c7fc
	movs	r0, #110
	bl 0x0200c7cc
	adds	r1, r6, #0
	movs	r0, #4
	bl 0x0200c7fc
	movs	r0, #26
	bl 0x0200c804
	ldr	r5, [pc, #380]
	movs	r0, #26
	adds	r1, r5, #0
	bl 0x0200c7fc
	movs	r0, #27
	bl 0x0200c804
	adds	r1, r5, #0
	movs	r0, #27
	bl 0x0200c7fc
	movs	r0, #4
	bl 0x0200c804
	movs	r0, #4
	movs	r1, #1
	bl 0x0200c854
	movs	r0, #7
	movs	r1, #2
	bl 0x0200c864
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c8ac
	movs	r1, #192
	movs	r0, #28
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c8ac
	movs	r2, #5
	movs	r0, #7
	movs	r1, #0
	bl 0x0200c89c
	movs	r0, #6
	movs	r1, #1
	bl 0x0200c8dc
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #4
	bl 0x0200c8cc
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x0200c8cc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200c8cc
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200c8cc
	movs	r2, #5
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c8ac
	movs	r2, #5
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c89c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x0200c8b4
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #28
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #5
	bl 0x0200c89c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200c854
	movs	r0, #20
	bl 0x0200c7cc
	movs	r0, #10
	bl 0x0200c7cc
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #7
	lsls	r1, r1, #9
	bl 0x0200c7f4
	adds	r1, r6, #0
	movs	r0, #7
	bl 0x0200c7fc
	movs	r0, #7
	bl 0x0200c804
	movs	r0, #7
	bl 0x0200c7bc
	movs	r0, #4
	movs	r1, #1
	bl 0x0200c8dc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #7
	bl 0x0200c844
	movs	r0, #20
	bl 0x0200c7cc
	bl 0x0200c95c
	movs	r1, #166
	movs	r0, #4
	lsls	r1, r1, #2
	movs	r2, #70
	bl 0x0200c82c
	bl 0x0200c7dc
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x0200d9a4
	.2byte 0xda00
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	movs	r0, #8
	movs	r1, #1
	sub	sp, #8
	bl 0x0200c994
	movs	r0, #9
	movs	r1, #1
	bl 0x0200c994
	movs	r0, #10
	movs	r1, #1
	bl 0x0200c994
	movs	r0, #11
	movs	r1, #1
	bl 0x0200c994
	movs	r0, #12
	movs	r1, #1
	bl 0x0200c994
	movs	r0, #13
	movs	r1, #1
	bl 0x0200c994
	movs	r0, #14
	movs	r1, #1
	bl 0x0200c994
	movs	r0, #15
	movs	r1, #1
	bl 0x0200c994
	bl 0x0200c984
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r2, #21
	movs	r0, #0
	movs	r3, #22
	bl 0x0200c98c
	ldr	r3, [pc, #376]
	movs	r5, #0
	str	r5, [r3, #0]
	movs	r0, #24
	bl 0x0200c7ec
	adds	r3, r0, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	str	r5, [r0, #12]
	str	r5, [r0, #20]
	movs	r0, #25
	bl 0x0200c7ec
	adds	r3, r0, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r0, #12]
	str	r3, [r0, #20]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_02002d18
	movs	r3, #76
	movs	r5, #49
	str	r3, [sp, #4]
	movs	r0, #49
	movs	r1, #106
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200c74c
	movs	r3, #44
	str	r3, [sp, #4]
	movs	r0, #49
	movs	r1, #45
	movs	r2, #4
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200c744
.L_02002d18:
	movs	r0, #146
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_02002df6
	movs	r3, #76
	str	r3, [sp, #4]
	movs	r5, #49
	movs	r0, #49
	movs	r1, #106
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200c74c
	movs	r3, #44
	str	r3, [sp, #4]
	movs	r0, #49
	movs	r1, #45
	movs	r2, #4
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200c744
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c844
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #35
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_02002d7c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200c6cc
	cmp	r0, #0
	bne.n	.L_02002df6
	bl 0x02008c08
	bl 0x02009e38
	b.n	.L_02002df6
.L_02002d7c:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #36
	bl 0x0200c6cc
	cmp	r0, #0
	bne.n	.L_02002dba
	ldr	r3, [pc, #172]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r5, r3, r2
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #3
	bne.n	.L_02002df6
	bl 0x02008c08
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #36
	bl 0x0200c6d4
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	movs	r0, #0
	cmp	r3, #3
	bne.n	.L_02002db4
	movs	r0, #1
.L_02002db4:
	bl 0x02008cac
	b.n	.L_02002df6
.L_02002dba:
	bl 0x02008c08
	ldr	r3, [pc, #120]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #5
	bne.n	.L_02002dd4
	bl 0x02009c7c
	b.n	.L_02002df6
.L_02002dd4:
	cmp	r3, #77
	bne.n	.L_02002df2
	bl 0x02009ed0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #35
	bl 0x0200c6d4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #36
	bl 0x0200c6dc
	b.n	.L_02002df6
.L_02002df2:
	bl 0x02008b48
.L_02002df6:
	movs	r0, #23
	bl 0x0200c7ec
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #13
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_02002e1a
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c844
.L_02002e1a:
	ldr	r3, [pc, #28]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_02002e2e
	bl 0x02009ed0
.L_02002e2e:
	movs	r0, #0
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x0200da3c
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #0
	sub	sp, #8
	bl 0x0200aed0
	movs	r0, #1
	bl 0x0200afc4
	bl 0x0200ba38
	movs	r0, #20
	movs	r1, #0
	bl 0x0200bac0
	movs	r0, #20
	bl 0x0200c7ec
	adds	r5, r0, #0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #12
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_02002eb4
	movs	r1, #254
	movs	r2, #248
	movs	r0, #20
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200c844
	movs	r3, #61
	movs	r2, #42
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #104
	movs	r1, #42
	movs	r2, #5
	movs	r3, #11
	bl 0x0200c74c
	movs	r3, #63
	movs	r2, #84
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #106
	movs	r1, #52
	movs	r2, #1
	movs	r3, #1
	bl 0x0200c74c
	movs	r0, #130
	movs	r1, #160
	lsls	r0, r0, #19
	lsls	r1, r1, #16
	movs	r2, #2
	movs	r3, #236
	bl 0x0200c9ac
.L_02002eb4:
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r2, #0
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	subs	r3, #6
	bl 0x0200c774
	movs	r0, #0
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #20
	adds	r0, #255
	bl 0x0200c6d4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #37
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_02002ef8
	movs	r0, #98
	adds	r0, #255
	bl 0x0200c6d4
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200c6d4
.L_02002ef8:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x60184b01
	.4byte 0x00004770
	.2byte 0xda18
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #164]
	sub	sp, #32
	ldr	r0, [r3, #0]
	cmp	r0, #0
	bge.n	.L_02002f1a
	adds	r0, #3
.L_02002f1a:
	asrs	r0, r0, #2
	movs	r1, #5
	bl 0x0200c64c
	ldr	r3, [pc, #148]
	mov	r8, r0
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02002f6e
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
	beq.n	.L_02002f4e
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r1
	cmp	r3, #153
	bne.n	.L_02002faa
.L_02002f4e:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_02002faa
	movs	r0, #175
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002faa
.L_02002f6e:
	movs	r5, #0
	movs	r6, #4
.L_02002f72:
	mov	r2, r8
	adds	r0, r2, r5
	movs	r1, #5
	mov	r7, sp
	bl 0x0200c64c
	ldr	r3, [pc, #60]
	lsls	r0, r0, #1
	ldrh	r3, [r3, r6]
	adds	r5, #1
	strh	r3, [r7, r0]
	adds	r6, #2
	cmp	r5, #4
	ble.n	.L_02002f72
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
.L_02002faa:
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200da14
	.4byte 0x0200da18
	.4byte 0x0200da40
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, r6, lr}
	ldr	r2, [pc, #88]
	movs	r3, #1
	adds	r6, r0, #0
	str	r3, [r2, #0]
	cmp	r6, #2
	beq.n	0x0200afe8
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
	bl 0x0200c6cc
	cmp	r0, #0
	bne.n	.L_02002ff8
	cmp	r6, #1
	bne.n	.L_0200300a
.L_02002ff8:
	ldr	r3, [pc, #60]
	movs	r2, #0
	movs	r1, #144
	str	r2, [r3, #0]
	ldr	r0, [pc, #56]
	lsls	r1, r1, #3
	bl 0x0200c65c
	b.n	.L_0200301e
.L_0200300a:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #40]
	ldr	r1, [pc, #44]
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_0200301e:
	pop	{r5, r6, pc}
	.4byte 0x0200da18
	.4byte 0x05000180
	.4byte 0x0200da40
	.4byte 0x03000730
	.4byte 0x0200da60
	.4byte 0x050001a0
	.4byte 0x0200da14
	.4byte 0x0200af09
	.4byte 0x0200da64
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, lr}
	bl 0x0200c7ec
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
	bl 0x0200c734
	adds	r3, r0, #0
	asrs	r3, r3, #19
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	adds	r3, #6
	movs	r2, #0
	bl 0x0200c774
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #128
	bl 0x0200c9b4
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
	bl 0x0200c7ec
	adds	r6, r0, #0
	ldr	r7, [r6, #104]
	bl 0x0200c7d4
	movs	r0, #0
	bl 0x0200c94c
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
	bl 0x0200c754
	movs	r3, #99
	adds	r3, r3, r7
	mov	r8, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0200313c
.L_020030f6:
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #176]
	str	r3, [r6, #8]
	ldr	r3, [r7, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r7, #16]
	str	r3, [r6, #16]
	cmp	r5, r2
	bgt.n	.L_02003112
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r5, r5, r3
.L_02003112:
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
	bl 0x0200c654
	mov	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_020030f6
.L_0200313c:
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
	bl 0x0200c754
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	b.n	.L_020031b4
	.4byte 0x00000001
	.4byte 0x02000240
	.4byte 0x0003ffff
	.2byte 0x122c
	.2byte 0x0300
.L_020031b4:
	bl 0x0200c8ec
	bl 0x0200c8fc
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
	bl 0x0200c734
	ldr	r3, [r6, #16]
	adds	r2, r0, #0
	ldr	r1, [r6, #8]
	adds	r0, r7, #0
	bl 0x0200c724
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	movs	r5, #0
	cmp	r2, r3
	ble.n	.L_0200321a
	b.n	.L_02003200
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02003200:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200c654
	cmp	r5, #59
	bgt.n	.L_0200321a
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	cmp	r2, r3
	bgt.n	.L_02003200
.L_0200321a:
	movs	r0, #127
	bl 0x0200c9c4
	ldr	r3, [r6, #40]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0200323a
.L_02003228:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200c654
	cmp	r5, #59
	bgt.n	.L_0200323a
	ldr	r3, [r6, #40]
	cmp	r3, #0
	bne.n	.L_02003228
.L_0200323a:
	adds	r0, r7, #0
	bl 0x0200c72c
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200c7ec
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200c8dc
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
	bl 0x0200c7dc
	pop	{r3, r5, r6}
	mov	r8, r3
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
	bl 0x0200c7ec
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r7, r0, #0
	cmp	r3, #0
	bne.n	.L_02003312
	movs	r3, #173
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02003312
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02003312
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02003320
.L_02003312:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200c6fc
	b.n	.L_0200345e
	.2byte 0x0240
	.2byte 0x0200
.L_02003320:
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200c6fc
	adds	r3, r6, #0
	adds	r3, #100
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #31
	ands	r3, r2
	cmp	r3, #31
	bne.n	.L_02003340
	movs	r0, #231
	bl 0x0200c9c4
.L_02003340:
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
	bl 0x0200c9a4
	cmp	r0, #255
	beq.n	.L_02003442
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl 0x0200c954
	ldr	r5, [r5, #0]
	movs	r3, #136
	lsls	r3, r3, #17
	cmp	r5, r3
	bgt.n	.L_02003442
	ldr	r2, [pc, #172]
	cmp	r5, r2
	blt.n	.L_02003442
	movs	r3, #98
	adds	r3, r3, r6
	mov	r9, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02003408
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #12]
	subs	r5, r2, r3
	cmp	r5, #0
	bge.n	.L_020033a0
	subs	r5, r3, r2
.L_020033a0:
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	adds	r0, #8
	adds	r1, #8
	mov	r8, r2
	bl 0x0200b288
	cmp	r0, #12
	bgt.n	.L_020033c0
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r5, r3
	bge.n	.L_020033c0
	movs	r2, #1
	mov	r8, r2
.L_020033c0:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_02003408
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200c6cc
	cmp	r0, #0
	bne.n	.L_02003408
	ldrh	r3, [r6, #6]
	str	r6, [r7, #104]
	strh	r3, [r7, #6]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
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
.L_02003408:
	ldrh	r0, [r6, #6]
	bl 0x0200c674
	ldr	r1, [r6, #48]
	ldr	r5, [pc, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200c66c
	ldr	r1, [r6, #48]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6933
	b.n	.L_0200343c
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
.L_0200343c:
	adds	r3, r3, r0
	str	r3, [r6, #16]
	b.n	.L_0200345e
.L_02003442:
	adds	r3, r6, #0
	adds	r3, #99
	movs	r5, #0
	strb	r5, [r3, #0]
	ldr	r1, [pc, #32]
	adds	r0, r6, #0
	str	r5, [r6, #108]
.L_02003450:
	bl 0x0200c704
	movs	r0, #228
	bl 0x0200c9c4
	ldr	r3, [pc, #20]
	str	r5, [r3, #0]
.L_0200345e:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200da1c
	.2byte 0xda3c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #222
	sub	sp, #68
	bl 0x0200c9c4
	ldrh	r0, [r5, #6]
	bl 0x0200c674
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
	bl 0x0200c66c
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
	bl 0x0200c70c
	movs	r1, #2
	adds	r7, r0, #0
	bl 0x0200c6f4
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200c754
	adds	r3, r7, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
.L_020034e4:
	ands	r3, r2
.L_020034e6:
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
	b.n	.L_02003534
	.4byte 0x00000000
	.4byte 0x0300021c
	.4byte 0x0200b2b5
	.2byte 0x0000
	.2byte 0xfffa
.L_02003534:
	.2byte 0x2300
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	str	r4, [sp, #12]
	bl 0x0200bb7c
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
	bne.n	.L_020035da
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
	bl 0x0200c664
	mov	r2, sl
	lsls	r3, r0, #3
	ldr	r2, [r2, #8]
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	subs	r3, #4
	lsls	r3, r3, #16
	mov	r8, r2
	add	r8, r3
	bl 0x0200c664
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
	bl 0x0200c664
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsrs	r0, r0, #16
	movs	r2, #160
	lsls	r2, r2, #11
	lsls	r0, r0, #16
	adds	r0, r0, r2
	movs	r1, #10
	bl 0x0200c644
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
	bl 0x0200bb7c
.L_020035da:
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
	bl 0x0200c7ec
	adds	r6, r0, #0
	movs	r0, #10
	bl 0x0200c7ec
	adds	r7, r0, #0
	movs	r0, #23
	bl 0x0200c7ec
	adds	r5, r0, #0
	ldr	r2, [r5, #80]
	movs	r1, #128
	mov	r8, r2
	movs	r2, #248
	movs	r0, #24
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200c844
	movs	r1, #128
	movs	r2, #248
	movs	r0, #23
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200c844
	movs	r1, #236
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200c844
	movs	r1, #138
	movs	r2, #128
	lsls	r2, r2, #17
	movs	r0, #10
	lsls	r1, r1, #18
	bl 0x0200c844
	movs	r0, #24
	bl 0x0200c7ec
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
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_020036c8
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
	bl 0x0200c7ec
	movs	r1, #4
	bl 0x0200c76c
	movs	r0, #11
	bl 0x0200c7ec
	movs	r1, #4
	bl 0x0200c76c
.L_020036c8:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_02003712
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
	bl 0x0200c7ec
	movs	r1, #4
	bl 0x0200c76c
	movs	r0, #12
	bl 0x0200c7ec
	movs	r1, #4
	bl 0x0200c76c
.L_02003712:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_02003752
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_02003752
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
.L_02003752:
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
	blt.n	.L_020037ce
	b.n	.L_02003902
.L_020037ce:
	ldr	r2, [pc, #340]
	mov	r0, r9
	lsls	r3, r0, #2
	ldr	r5, [r2, r3]
	cmp	r5, #0
	bne.n	.L_020037dc
	b.n	.L_020038f2
.L_020037dc:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bne.n	.L_020037e4
	b.n	.L_020038f2
.L_020037e4:
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
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	adds	r3, r5, #0
	mov	ip, r2
	asrs	r1, r0, #16
	mov	r0, ip
	adds	r3, #100
	asrs	r2, r4, #16
	cmp	r0, #0
	bne.n	.L_0200385a
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
	bhi.n	.L_020038f2
	movs	r2, #16
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_020038f2
	cmp	r4, #239
	bgt.n	.L_020038f2
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
	b.n	.L_02003896
.L_0200385a:
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
	bhi.n	.L_020038f2
	movs	r2, #64
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_020038f2
	cmp	r4, #175
	bgt.n	.L_020038f2
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
.L_02003896:
	movs	r2, #128
	orrs	r4, r3
	stmia	r1!, {r4}
	ldr	r0, [sp, #4]
	lsls	r3, r7, #3
	adds	r3, r0, r3
	lsls	r2, r2, #4
	orrs	r3, r2
	str	r3, [r1, #0]
	ldr	r3, [pc, #136]
	movs	r0, #1
	ldrh	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_020038d4
	adds	r0, r5, #0
	bl 0x0200c99c
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
	b.n	.L_020038e8
.L_020038d4:
	movs	r3, #3
	ands	r3, r2
	movs	r0, #13
	ldrb	r2, [r6, #9]
	negs	r0, r0
	adds	r1, r0, #0
	lsls	r3, r3, #2
	ands	r2, r1
	orrs	r2, r3
	strb	r2, [r6, #9]
.L_020038e8:
	adds	r0, r6, #0
	mov	r1, fp
	bl 0x0200c6ac
	adds	r6, #12
.L_020038f2:
	ldr	r3, [pc, #44]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	add	r9, r1
	cmp	r9, r3
	bge.n	.L_02003902
	b.n	.L_020037ce
.L_02003902:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffff0000
	.4byte 0x0200da80
	.4byte 0x020036e0
	.4byte 0x0200dac4
	.4byte 0x0200da82
	.4byte 0x0200da84
	.4byte 0x0200db84
	.4byte 0x40002000
	.4byte 0xc000a000
	.2byte 0xdb86
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
.L_0200393c:
	lsls	r0, r0, #4
	bl 0x0200c67c
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200c68c
	ldr	r5, [pc, #76]
	bl 0x0200c69c
	movs	r1, #192
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200c694
	adds	r0, r6, #0
	bl 0x0200c684
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200c65c
.L_0200397c:
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
	.4byte 0x0200da84
	.4byte 0x0200cb50
	.4byte 0x0200da80
	.4byte 0x0200b77d
	.4byte 0x0200da82
	.4byte 0x0200db84
	.2byte 0xdb86
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200c67c
	ldr	r3, [pc, #84]
.L_020039c4:
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200c68c
	ldr	r5, [pc, #76]
	bl 0x0200c69c
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200c694
	adds	r0, r6, #0
	bl 0x0200c684
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200c65c
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
	.4byte 0x0200da84
	.4byte 0x0200ccb3
	.4byte 0x0200da80
	.4byte 0x0200b77d
	.4byte 0x0200da82
	.4byte 0x0200db84
	.2byte 0xdb86
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	bl 0x0200c67c
	ldr	r3, [pc, #88]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #84]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #80]
	bl 0x0200c68c
	ldr	r5, [pc, #80]
	bl 0x0200c69c
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
.L_02003a62:
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200c694
	adds	r0, r6, #0
	bl 0x0200c684
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	bl 0x0200c65c
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	b.n	.L_02003abc
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200da84
	.4byte 0x0200cee2
	.4byte 0x0200da80
	.4byte 0x0200b77d
	.4byte 0x0200da82
	.4byte 0x0200db84
	.2byte 0xdb86
	.2byte 0x0200
.L_02003abc:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	bl 0x0200c7ec
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L_02003ae6
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
.L_02003ae6:
	pop	{r5, pc}
	.4byte 0x0200da82
	.4byte 0x0200da84
	.4byte 0x80184b01
	.4byte 0x00004770
	.2byte 0xdb86
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02003b40
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003b40
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
.L_02003b40:
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
	bl 0x0200c7ec
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02003bc4
	cmp	r7, #0
	beq.n	.L_02003bc4
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02003bcc
.L_02003bc4:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02003bcc:
	mov	r3, sl
	bl 0x0200c70c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02003bda
	b.n	.L_02003d26
.L_02003bda:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200c6f4
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200c704
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200c754
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
	bl 0x0200bafc
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
	beq.n	.L_02003d26
	cmp	r7, #0
	beq.n	.L_02003d26
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02003c5c
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200c884
.L_02003c5c:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003c7c
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x0200bafc
.L_02003c7c:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02003c90
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02003c90:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02003cd6
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_02003cbe
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200c644
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_02003cd0
.L_02003cbe:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200c644
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_02003cd0:
	bl 0x0200c644
	str	r0, [r6, #52]
.L_02003cd6:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02003cf2
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200c6f4
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200c704
.L_02003cf2:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003d04
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02003d04:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003d16
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02003d16:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003d26
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02003d26:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200da30
	.4byte 0x0200bb45
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
	beq.n	.L_02003e50
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
	bl 0x0200c7ec
	mov	r1, r8
	ldr	r3, [r0, #8]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	ldr	r4, [sp, #0]
	cmp	r3, r2
	bne.n	.L_02003d90
	ldr	r3, [r0, #16]
	movs	r5, #2
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_02003d98
.L_02003d90:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r4, #12]
.L_02003d98:
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	movs	r2, #1
	negs	r2, r2
	ldr	r1, [pc, #192]
	cmp	r3, r2
	beq.n	.L_02003e50
	movs	r5, #14
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L_02003e50
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
	bl 0x0200c734
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
	bhi.n	.L_02003e50
	movs	r0, #15
	negs	r0, r0
	cmp	r2, r0
	blt.n	.L_02003e50
	cmp	r2, #239
	bgt.n	.L_02003e50
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
	bl 0x0200c6ac
.L_02003e50:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200db88
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
	bge.n	.L_02003ff4
.L_02003f24:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #36]
	ldr	r5, [sp, #24]
	lsls	r3, r1, #9
	adds	r2, r2, r3
	movs	r3, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, r5
	bge.n	.L_02003fe8
.L_02003f38:
	mov	r0, fp
	ldrb	r5, [r0, #2]
	cmp	r5, #0
	beq.n	.L_02003fd8
	ldr	r1, [sp, #44]
	cmp	r5, r1
	bcc.n	.L_02003fd8
	adds	r1, #1
	mov	sl, r1
	cmp	r5, sl
	bhi.n	.L_02003fd8
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
	bl 0x0200c6cc
	cmp	r0, #0
	bne.n	.L_02003f8c
	cmp	r5, sl
	bne.n	.L_02003fca
	mov	r3, r9
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	bl 0x0200c6d4
	b.n	.L_02003fca
.L_02003f8c:
	mov	r1, r9
	movs	r5, #4
	ldrsh	r0, [r1, r5]
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_02003fca
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
	bl 0x0200c73c
.L_02003fca:
	mov	r0, r8
	ldrh	r3, [r0, #10]
	mov	r1, r8
	adds	r3, #1
	strh	r3, [r1, #10]
	movs	r5, #8
	add	r9, r5
.L_02003fd8:
	ldr	r2, [sp, #16]
	ldr	r5, [sp, #24]
	adds	r2, #1
	movs	r3, #4
	str	r2, [sp, #16]
	add	fp, r3
	cmp	r2, r5
	blt.n	.L_02003f38
.L_02003fe8:
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #12]
	cmp	r0, r1
	blt.n	.L_02003f24
.L_02003ff4:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200c6cc
	cmp	r0, #0
	beq.n	.L_0200404c
	ldr	r3, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c7ec
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
	bge.n	.L_0200404c
.L_02004026:
	mov	r0, r9
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	cmp	r3, r4
	bne.n	.L_0200403c
	movs	r5, #2
	ldrsh	r3, [r0, r5]
	cmp	r3, r1
	bne.n	.L_0200403c
	mov	r0, r8
	strh	r2, [r0, #12]
.L_0200403c:
	movs	r3, #8
	mov	r0, r8
	add	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	adds	r2, #1
	cmp	r2, r3
	blt.n	.L_02004026
.L_0200404c:
	movs	r0, #128
	lsls	r0, r0, #1
	bl 0x0200c67c
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r2, #63
.L_0200405a:
	ldr	r3, [pc, #80]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bge.n	.L_0200405a
	bl 0x0200c69c
	mov	r1, r8
	strh	r0, [r1, #16]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #1
	asrs	r0, r0, #16
	bl 0x0200c694
	adds	r0, r5, #0
	bl 0x0200c684
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200c65c
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
	.4byte 0x0200db88
	.4byte 0x03000258
	.4byte 0x02000240
	.4byte 0x11111111
	.2byte 0xbd45
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
	bl 0x0200c7ec
	mov	r1, r8
	ldr	r5, [r0, #8]
	ldr	r6, [r0, #16]
	mov	sl, r0
	movs	r2, #128
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200c7f4
	asrs	r5, r5, #20
	mov	r2, r8
	asrs	r6, r6, #20
	ldr	r0, [r2, #0]
	lsls	r1, r5, #4
	lsls	r2, r6, #4
	adds	r1, #8
	adds	r2, #8
	bl 0x0200c81c
	movs	r0, #1
	bl 0x0200c654
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
	bl 0x0200c714
	movs	r0, #4
	bl 0x0200c7cc
	bl 0x0200c8fc
	ldr	r2, [pc, #20]
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
	adds	r3, r3, r1
	mov	r8, r0
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200c7ec
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
	bl 0x0200c0b4
	movs	r0, #161
	bl 0x0200c9c4
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
	bl 0x0200c73c
	movs	r0, #12
	bl 0x0200c7cc
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
	bl 0x0200c7ec
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
	bl 0x0200c0b4
	movs	r0, #229
	bl 0x0200c9c4
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
	bl 0x0200c73c
	movs	r0, #12
	bl 0x0200c7cc
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	bl 0x0200c7ec
	movs	r1, #0
	bl 0x0200c754
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
	b.n	.L_02004260
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00008000
	.2byte 0x0240
	.2byte 0x0200
.L_02004260:
	movs	r3, #1
	mov	r1, r8
	strh	r3, [r1, #14]
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl 0x0200c854
	movs	r0, #16
	bl 0x0200c7cc
.L_02004274:
	cmp	r7, #5
	bne.n	.L_0200427e
	movs	r0, #204
	bl 0x0200c9c4
.L_0200427e:
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
	bl 0x0200c654
	cmp	r7, #39
	ble.n	.L_02004274
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c7ec
	movs	r3, #0
	adds	r0, #84
	strb	r3, [r0, #0]
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
	bl 0x0200c93c
	bl 0x0200c944
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
	bl 0x0200c644
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_0200431c
	adds	r3, #15
.L_0200431c:
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
	bl 0x0200c7ec
	adds	r7, r0, #0
	bl 0x0200c7d4
	movs	r0, #0
	bl 0x0200c94c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200c8ec
	bl 0x0200c71c
	movs	r0, #1
	bl 0x0200c654
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
	bl 0x0200c934
	bl 0x0200c944
	movs	r0, #204
	bl 0x0200c9c4
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200c7cc
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
.L_020043de:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200c674
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200c66c
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200c664
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #188]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200c664
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
	bl 0x0200bb7c
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_020043de
	movs	r0, #188
	bl 0x0200c9c4
	ldr	r5, [pc, #112]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200c8d4
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200c854
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200c75c
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200c75c
	bl 0x0200c764
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200c8d4
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200c7cc
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200c854
	bl 0x0200c7dc
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c2ed
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
	bl 0x0200c7ec
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
	bge.n	.L_02004578
.L_02004510:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, r8
	bne.n	.L_0200456c
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	cmp	r3, sl
	bne.n	.L_0200456c
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	bl 0x0200c6cc
	cmp	r0, #0
	bne.n	.L_02004540
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x0200c13c
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	bl 0x0200c6d4
	strh	r7, [r6, #12]
	b.n	.L_02004578
.L_02004540:
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r7, r3
	beq.n	.L_02004578
	adds	r0, r6, #0
	adds	r1, r5, #0
	strh	r7, [r6, #12]
	bl 0x0200c1ac
	movs	r2, #2
	ldrsh	r0, [r6, r2]
	mov	r1, r8
	bl 0x0200c6ec
	movs	r3, #2
	ldrsh	r0, [r6, r3]
	mov	r1, sl
	adds	r0, #8
	bl 0x0200c6ec
	movs	r0, #1
	b.n	.L_0200457a
.L_0200456c:
	lsls	r3, r2, #16
	adds	r7, #1
	asrs	r3, r3, #16
	adds	r5, #8
	cmp	r7, r3
	blt.n	.L_02004510
.L_02004578:
	movs	r0, #0
.L_0200457a:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xdb88
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
	bl 0x0200c7ec
	movs	r3, #192
	ldr	r5, [pc, #140]
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	mov	sl, r3
	bl 0x0200c6e4
	adds	r7, r0, #0
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	adds	r0, #8
	bl 0x0200c6e4
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_020045da
	cmp	r0, #0
	beq.n	.L_0200462e
.L_020045da:
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #0
	bl 0x0200c6ec
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	movs	r1, #0
	adds	r0, #8
	bl 0x0200c6ec
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
	bl 0x0200c71c
	bl 0x0200c344
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	str	r2, [r3, #0]
.L_0200462e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200db88
	.irp EntryTarget, 0x03000528, 0x03000508, 0x080000c1, 0x080000d1, 0x080000f9, 0x08000119, 0x08000121, 0x08000169, 0x08000179, 0x080001a9, 0x080001c9, 0x080001d1, 0x080001d9, 0x080001e9, 0x080001f1, 0x08000291, 0x080003c1, 0x080003c9, 0x080003d1, 0x080003d9, 0x080003e9, 0x080003f1, 0x08020091, 0x08020099, 0x080200a9, 0x080200c1, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x080201c1, 0x080201e1, 0x080201e9, 0x080201f1, 0x08020219, 0x08020229, 0x08020231, 0x08020279, 0x08020361, 0x08038041, 0x08038349, 0x080ad009, 0x080ad029, 0x080ad041, 0x080ad049, 0x080ad0c1, 0x080ad0c9, 0x080ad0f9, 0x080ad111, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80b1, 0x080c80c1, 0x080c80c9, 0x080c80d1, 0x080c80d9, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8101, 0x080c8119, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8159, 0x080c8161, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c81f1, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8259, 0x080c8281, 0x080c8291, 0x080c8299, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c85c1, 0x080c85d1, 0x080c85e9, 0x080c85f1, 0x080c85f9, 0x080c8601, 0x080c86a9, 0x080c86e9, 0x080c8781, 0x080c87e9, 0x080c8831, 0x080c8841, 0x080c8849, 0x08108079, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x02bb0000
	.4byte 0x00aa0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x02a70000
	.4byte 0x00aa0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x02930000
	.4byte 0x00aa0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
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
	.4byte 0x00072260
	.4byte 0x6805041a
	.4byte 0x3f1b01ff
	.4byte 0x06150458
	.4byte 0x40182a2c
	.4byte 0x8f000900
	.4byte 0xe0130302
	.4byte 0x0030cd0c
	.4byte 0x00000002
	.4byte 0xac862b05
	.4byte 0xaf643138
	.4byte 0x426905df
	.4byte 0xabd8ac4e
	.4byte 0xbe810f20
	.4byte 0x7015130e
	.4byte 0x58aaf4f3
	.4byte 0x1c57fed1
	.4byte 0xba72f5fa
	.4byte 0x15f3c78f
	.4byte 0x2c4bddfc
	.4byte 0x8c405c2e
	.4byte 0x5a901050
	.4byte 0x669c3d02
	.4byte 0x813f657c
	.4byte 0xc48af4a0
	.4byte 0x70f27d1e
	.4byte 0xdfcf8f1f
	.4byte 0x0be667d7
	.4byte 0x54be3c30
	.4byte 0xa442f8f4
	.4byte 0xeb17cfc7
	.4byte 0x058022f4
	.4byte 0xc3a1d0c0
	.4byte 0xf05c0a2b
	.4byte 0xbcc4f23e
	.4byte 0xe8ae7760
	.4byte 0x38745730
	.4byte 0xe6259c57
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
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000011
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
	.4byte 0x00000107
	.4byte 0x00101100
	.4byte 0x00303100
	.4byte 0x00404100
	.4byte 0x00505102
	.4byte 0x00606102
	.4byte 0x00707105
	.4byte 0x00808104
	.4byte 0x00909101
	.4byte 0x00a0a101
	.4byte 0x000001ff
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x05b80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x06480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff013d
	.4byte 0x0200d078
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff013d
	.4byte 0x0200d078
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0200d078
	.4byte 0x03a80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0200d078
	.4byte 0x03b80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff01ac
	.4byte 0x0200d078
	.4byte 0x03c80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x0200d078
	.4byte 0x04780000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x0200d078
	.4byte 0x04280000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00020000
	.4byte 0xffff01e9
	.4byte 0x0200d078
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e9
	.4byte 0x0200d078
	.4byte 0x03f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00020000
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0004
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0007
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
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0017
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
	.4byte 0xffff01e9
	.4byte 0x0200d078
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
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
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
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
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x02008061
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x02008081
	.4byte 0x00000002
	.4byte 0x02020020
	.4byte 0x0200860d
	.4byte 0x00000002
	.4byte 0x12020021
	.4byte 0x020088c9
	.4byte 0x00000000
	.4byte 0x1a230000
	.4byte 0x02008971
	.4byte 0x00000000
	.4byte 0x1a230002
	.4byte 0x02008971
	.4byte 0x00000000
	.4byte 0x1a230006
	.4byte 0x02008971
	.4byte 0x00000000
	.4byte 0x1a230005
	.4byte 0x02008971
	.4byte 0x00000000
	.4byte 0x1a23001c
	.4byte 0x02008971
	.4byte 0x10008c15
	.4byte 0xffff0014
	.4byte 0x0200839d
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte 0x020083b9
	.4byte 0x00000008
	.4byte 0xffffffff
	.4byte 0x0200839d
	.4byte 0x00000009
	.4byte 0xffffffff
	.4byte 0x020083b9
	.4byte 0x00008c15
	.4byte 0xffff0017
	.4byte 0x00000000
	.4byte 0x00008515
	.4byte 0x02000015
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x0200b09d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x0000000e
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x000c0000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000027
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte 0x02008b39
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000030
	.4byte 0x02e10000
	.4byte 0x00ca0000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000030
	.4byte 0x03080000
	.4byte 0x00ca0000
	.4byte 0x0000002e
	.4byte 0x02008ad1
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte 0x02008b39
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000030
	.4byte 0x03080000
	.4byte 0x00ca0000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000030
	.4byte 0x02e10000
	.4byte 0x00ca0000
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x02008b39
	.4byte 0x00000030
	.4byte 0x02db0000
	.4byte 0x00d50000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000030
	.4byte 0x02db0000
	.4byte 0x00e80000
	.4byte 0x0000002e
	.4byte 0x02008ad1
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000035
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte 0x02008b39
	.4byte 0x00000030
	.4byte 0x02db0000
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000030
	.4byte 0x02db0000
	.4byte 0x00d50000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000035
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x02e80000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000005
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x03800000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x02e40000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x03840000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000030
	.4byte 0x02d40000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000030
	.4byte 0x03940000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000030
	.4byte 0x02a50000
	.4byte 0x011f0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000030
	.4byte 0x02e40000
	.4byte 0x01580000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000030
	.4byte 0x03b60000
	.4byte 0x011f0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000030
	.4byte 0x03840000
	.4byte 0x01580000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000030
	.4byte 0x02c40000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000005
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000030
	.4byte 0x039c0000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x02c20000
	.4byte 0x00ac0000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x02aa0000
	.4byte 0x00970000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x02aa0000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x02980000
	.4byte 0x006a0000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x02980000
	.4byte 0x00640000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000030
	.4byte 0x02980000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0xffffffff
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x0200cfc4
	.4byte 0x0200d000
	.4byte 0x0200d03c
