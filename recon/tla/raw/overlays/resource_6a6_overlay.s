.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020090c1, 0x02008049, 0x02008079, 0x02008081, 0x02008cc1, 0x02008051, 0x020091ed
	overlay_veneer \EntryTarget
	.endr
	push	{lr}
	movs	r0, #19
	movs	r1, #3
	movs	r2, #16
	bl 0x02009400
	pop	{pc}
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x980c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_02000068
	ldr	r0, [pc, #12]
.L_02000068:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000010b
	.2byte 0x983c
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x985c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02000098
	ldr	r0, [pc, #12]
	b.n	.L_0200009a
.L_02000098:
	ldr	r0, [pc, #12]
.L_0200009a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000010b
	.4byte 0x02009a80
	.2byte 0x9900
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #8
	movs	r3, #24
	str	r3, [sp, #0]
	mov	r8, r3
	movs	r5, #7
	movs	r0, #69
	movs	r1, #70
	movs	r2, #10
	movs	r3, #12
	str	r5, [sp, #4]
	bl 0x020092b0
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r0, #69
	movs	r1, #70
	movs	r2, #10
	movs	r3, #12
	str	r5, [sp, #4]
	bl 0x020092b8
	movs	r6, #88
	movs	r0, #79
	movs	r1, #70
	movs	r2, #10
	movs	r3, #12
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x020092b0
	movs	r0, #79
	movs	r1, #70
	movs	r2, #10
	movs	r3, #12
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x020092b8
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r5, #71
	movs	r0, #69
	movs	r1, #82
	movs	r2, #10
	movs	r3, #12
	str	r5, [sp, #4]
	bl 0x020092b0
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r0, #69
	movs	r1, #82
	movs	r2, #10
	movs	r3, #12
	str	r5, [sp, #4]
	bl 0x020092b8
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x02009380
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02009388
	bl 0x02009420
	movs	r1, #0
	bl 0x020092f0
	cmp	r0, #0
	bne.n	.L_0200015c
	movs	r0, #10
	bl 0x020092d8
	adds	r0, r5, #1
	bl 0x02009380
	b.n	.L_02000168
.L_0200015c:
	movs	r0, #20
	bl 0x020092d8
	adds	r0, r5, #2
	bl 0x02009380
.L_02000168:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009390
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x2ca3
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #24]
	movs	r1, #1
	bl 0x020092d0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
	strh	r3, [r2, #0]
	pop	{pc}
	.2byte 0x2ca9
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #4
	sub	sp, #4
	bl 0x020092f8
	adds	r5, r0, #0
	bl 0x020092e0
	movs	r0, #0
	bl 0x020093f8
	b.n	.L_020001ec
.L_020001b6:
	cmp	r1, #0
	beq.n	.L_020001dc
	cmp	r6, #0
	beq.n	.L_020001ce
	movs	r1, #216
	movs	r2, #173
	movs	r0, #4
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x02009330
	b.n	.L_020001dc
.L_020001ce:
	movs	r1, #253
	movs	r2, #173
	movs	r0, #4
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x02009330
.L_020001dc:
	cmp	r6, #0
	beq.n	.L_020001e2
	b.n	.L_02000982
.L_020001e2:
	movs	r1, #253
	movs	r2, #118
	movs	r0, #4
	lsls	r1, r1, #1
	b.n	.L_0200098a
.L_020001ec:
	ldr	r3, [r5, #8]
	ldr	r0, [pc, #24]
	movs	r6, #0
	movs	r1, #0
	movs	r2, #0
	cmp	r3, r0
	bgt.n	.L_020001fc
	movs	r6, #1
.L_020001fc:
	ldr	r0, [r5, #16]
	ldr	r3, [pc, #12]
	cmp	r0, r3
	bgt.n	.L_02000210
	movs	r1, #1
	b.n	.L_0200021a
	.4byte 0x01e8ffff
	.2byte 0xffff
	.2byte 0x015a
.L_02000210:
	movs	r3, #185
	lsls	r3, r3, #17
	cmp	r0, r3
	ble.n	.L_0200021a
	movs	r2, #1
.L_0200021a:
	cmp	r2, #0
	beq.n	.L_020001b6
.L_0200021e:
	movs	r0, #244
	movs	r1, #1
	movs	r2, #186
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x020093c0
	movs	r0, #150
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009290
	movs	r1, #232
	movs	r2, #122
	adds	r2, #255
	movs	r0, #4
	adds	r1, #255
	bl 0x02009320
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x020093a0
	movs	r0, #30
	bl 0x020092d8
	movs	r0, #140
	bl 0x02009448
	movs	r0, #16
	movs	r1, #2
	bl 0x02009360
	movs	r0, #18
	movs	r1, #2
	bl 0x02009360
	movs	r0, #17
	movs	r1, #2
	bl 0x02009368
	movs	r0, #16
	movs	r1, #4
	movs	r2, #0
	bl 0x02009370
	movs	r0, #17
	movs	r1, #4
	movs	r2, #0
	bl 0x02009370
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #18
	bl 0x02009398
	movs	r0, #25
	bl 0x020092d8
	movs	r0, #4
	movs	r1, #3
	bl 0x02009360
	movs	r1, #129
	movs	r0, #4
	lsls	r1, r1, #1
	bl 0x020093b0
	movs	r2, #12
	movs	r0, #4
	movs	r1, #24
	bl 0x02009410
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x020093a0
	movs	r0, #30
	bl 0x020092d8
	movs	r0, #144
	bl 0x02009448
	movs	r1, #230
	movs	r0, #21
	lsls	r1, r1, #1
	bl 0x02009428
	movs	r1, #244
	movs	r2, #165
	movs	r0, #21
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x02009348
	movs	r0, #20
	movs	r1, #0
	movs	r2, #8
	bl 0x02009338
	movs	r1, #192
	movs	r2, #192
	movs	r0, #21
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x02009300
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x020092c8
	movs	r1, #244
	movs	r2, #150
	movs	r0, #21
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x02009318
	movs	r0, #244
	movs	r1, #1
	movs	r2, #154
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x020093c0
	movs	r0, #21
	bl 0x02009340
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x02009348
	movs	r0, #224
	movs	r1, #224
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #11
	bl 0x020092c8
	movs	r0, #192
	lsls	r0, r0, #3
	bl 0x02009238
	adds	r7, r0, #0
	movs	r0, #128
	lsls	r0, r0, #5
	bl 0x02009230
	adds	r5, r0, #0
	ldr	r0, [pc, #192]
	bl 0x02009270
	adds	r1, r5, #0
	bl 0x02009248
	bl 0x02009260
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #3
	mov	r8, r0
	bl 0x02009258
	mov	sl, r0
	adds	r0, r5, #0
	bl 0x02009240
	adds	r5, r7, #0
	adds	r5, #12
	movs	r6, #95
.L_0200038c:
	bl 0x02009228
	lsls	r0, r0, #5
	lsrs	r0, r0, #16
	negs	r0, r0
	subs	r6, #1
	str	r0, [r5, #0]
	adds	r5, #16
	cmp	r6, #0
	bge.n	.L_0200038c
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #124]
	ldr	r1, [pc, #124]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r6, #0
.L_020003b6:
	movs	r3, #31
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_020003c4
	movs	r0, #145
	bl 0x02009448
.L_020003c4:
	cmp	r6, #30
	bne.n	.L_020003d4
	movs	r0, #90
	bl 0x02009278
	movs	r0, #141
	bl 0x02009448
.L_020003d4:
	adds	r5, r7, #0
.L_020003d6:
	ldr	r2, [r5, #12]
	cmp	r2, #0
	bne.n	.L_02000430
	movs	r3, #128
	lsls	r3, r3, #23
	str	r3, [r5, #4]
	str	r2, [r5, #8]
	bl 0x02009228
	lsls	r3, r0, #1
	adds	r3, r3, r0
	ldr	r2, [pc, #44]
	lsls	r3, r3, #5
	lsrs	r3, r3, #16
	adds	r3, #72
	ands	r3, r2
	ldr	r1, [pc, #40]
	ldrh	r2, [r5, #6]
	ands	r2, r1
	orrs	r2, r3
	strh	r2, [r5, #6]
	bl 0x02009228
	ldrb	r3, [r5, #9]
	lsrs	r0, r0, #10
	strb	r0, [r5, #4]
	movs	r0, #13
	movs	r2, #240
	negs	r0, r0
	orrs	r3, r2
	adds	r2, r0, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	b.n	.L_02000430
	.2byte 0x0000
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x000001e8
	.4byte 0x02009450
	.2byte 0x03e0
	.2byte 0x0500
.L_02000430:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	blt.n	.L_02000452
	ldr	r2, [pc, #52]
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	add	r3, sl
	ands	r3, r2
	ldr	r1, [pc, #44]
	ldrh	r2, [r5, #8]
	adds	r0, r5, #0
	ands	r2, r1
	orrs	r2, r3
	strh	r2, [r5, #8]
	movs	r1, #100
	bl 0x02009268
.L_02000452:
	ldr	r3, [r5, #12]
	adds	r3, #1
	str	r3, [r5, #12]
	cmp	r3, #31
	ble.n	.L_02000474
	bl 0x02009228
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsrs	r3, r3, #16
	negs	r3, r3
	str	r3, [r5, #12]
	b.n	.L_02000474
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_02000474:
	movs	r1, #190
	lsls	r1, r1, #3
	adds	r5, #16
	adds	r3, r7, r1
	cmp	r5, r3
	ble.n	.L_020003d6
	movs	r0, #1
	adds	r6, #1
	bl 0x02009220
	cmp	r6, #119
	ble.n	.L_020003b6
	mov	r0, r8
	bl 0x02009250
	adds	r0, r7, #0
	bl 0x02009240
	movs	r0, #244
	movs	r1, #1
	movs	r2, #195
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x020093c0
	movs	r1, #248
	movs	r2, #240
	movs	r3, #0
	movs	r0, #14
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x02009350
	bl 0x020080ac
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r1, #0
	movs	r0, #0
	bl 0x020092c8
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x02009448
	movs	r0, #180
	bl 0x020092d8
	movs	r0, #120
	bl 0x02009280
	ldr	r0, [pc, #1020]
	bl 0x02009380
	movs	r0, #120
	bl 0x020092d8
	movs	r0, #18
	movs	r1, #2
	bl 0x02009368
	movs	r1, #128
	movs	r0, #18
	lsls	r1, r1, #8
	bl 0x020093a0
	movs	r0, #18
	movs	r1, #0
	bl 0x02009390
	movs	r1, #128
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #6
	bl 0x02009398
	movs	r0, #17
	movs	r1, #3
	bl 0x02009358
	movs	r1, #3
	movs	r0, #16
	bl 0x02009358
	movs	r0, #30
	bl 0x020092d8
	movs	r1, #129
	movs	r2, #35
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x020093a8
	movs	r0, #17
	movs	r1, #0
	bl 0x02009390
	movs	r1, #8
	adds	r1, #255
	movs	r2, #35
	movs	r0, #16
	bl 0x020093a8
	movs	r1, #12
	movs	r2, #24
	movs	r0, #16
	negs	r1, r1
	bl 0x02009410
	movs	r0, #16
	movs	r1, #2
	bl 0x02009368
	movs	r0, #16
	movs	r1, #0
	bl 0x02009390
	movs	r0, #4
	movs	r1, #3
	bl 0x02009358
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #18
	bl 0x020093a0
	movs	r0, #30
	bl 0x020092d8
	movs	r1, #128
	movs	r2, #35
	lsls	r1, r1, #1
	movs	r0, #18
	bl 0x020093a8
	movs	r0, #18
	movs	r1, #0
	bl 0x02009390
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #16
	bl 0x020093a8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #35
	movs	r0, #17
	bl 0x020093a8
	movs	r0, #16
	movs	r1, #18
	movs	r2, #0
	bl 0x02009370
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #17
	bl 0x02009398
	movs	r0, #60
	bl 0x020092d8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x02009398
	movs	r0, #60
	bl 0x020092d8
	movs	r1, #6
	adds	r1, #255
	movs	r2, #50
	movs	r0, #18
	bl 0x020093a8
	movs	r1, #129
	movs	r2, #35
	lsls	r1, r1, #1
	movs	r0, #18
	bl 0x020093a8
	movs	r0, #18
	movs	r1, #0
	bl 0x02009390
	movs	r1, #17
	movs	r2, #0
	movs	r0, #16
	bl 0x02009378
	movs	r0, #40
	bl 0x020092d8
	movs	r1, #192
	movs	r0, #16
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009398
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009398
	movs	r1, #176
	movs	r0, #18
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009398
	movs	r0, #244
	movs	r1, #1
	movs	r2, #163
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x020093c0
	movs	r0, #60
	bl 0x020092d8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #16
	bl 0x020093a8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #17
	bl 0x020093a8
	movs	r1, #129
	movs	r2, #35
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x020093a8
	movs	r0, #17
	movs	r1, #0
	bl 0x02009390
	movs	r0, #16
	movs	r1, #3
	bl 0x02009358
	movs	r0, #16
	movs	r1, #0
	bl 0x02009390
	movs	r0, #18
	movs	r1, #4
	movs	r2, #0
	bl 0x02009370
	movs	r1, #10
	adds	r1, #255
	movs	r2, #35
	movs	r0, #18
	bl 0x020093a8
	movs	r0, #244
	movs	r1, #1
	movs	r2, #183
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x020093c0
	bl 0x020093c8
	movs	r1, #160
	movs	r2, #0
	movs	r0, #18
	lsls	r1, r1, #7
	bl 0x02009398
	movs	r0, #18
	movs	r1, #0
	bl 0x02009390
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #17
	adds	r1, #204
	adds	r2, #102
	bl 0x02009300
	movs	r2, #16
	movs	r1, #0
	movs	r0, #17
	bl 0x02009408
	movs	r0, #17
	bl 0x02009340
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #6
	bl 0x020093a0
	movs	r0, #17
	movs	r1, #4
	bl 0x02009358
	movs	r0, #17
	movs	r1, #0
	bl 0x02009390
	movs	r0, #16
	movs	r1, #3
	bl 0x02009358
	movs	r0, #16
	movs	r1, #0
	bl 0x02009390
	movs	r0, #18
	movs	r1, #3
	bl 0x02009358
	movs	r1, #192
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #6
	bl 0x02009398
	movs	r0, #17
	movs	r1, #0
	bl 0x02009390
	movs	r1, #129
	movs	r2, #35
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x020093a8
	movs	r0, #60
	bl 0x020092d8
	movs	r0, #10
	bl 0x020092d8
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #16
	bl 0x020093a0
	movs	r0, #20
	bl 0x020092d8
	movs	r0, #16
	movs	r1, #2
	bl 0x02009368
	movs	r0, #16
	movs	r1, #0
	bl 0x02009390
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #18
	bl 0x020093a8
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #17
	bl 0x020093a8
	movs	r1, #6
	adds	r1, #255
	movs	r2, #60
	movs	r0, #16
	bl 0x020093a8
	movs	r1, #131
	movs	r2, #35
	lsls	r1, r1, #1
	movs	r0, #18
	bl 0x020093a8
	movs	r0, #18
	movs	r1, #0
	bl 0x02009390
	movs	r1, #3
	movs	r0, #17
	bl 0x02009358
	movs	r0, #40
	bl 0x020092d8
	movs	r1, #131
	movs	r2, #35
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x020093a8
	movs	r0, #17
	movs	r1, #0
	bl 0x02009390
	movs	r1, #3
	movs	r0, #16
	bl 0x02009358
	movs	r0, #30
	bl 0x020092d8
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	bl 0x020093b8
	movs	r1, #1
	movs	r2, #128
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	ldr	r0, [pc, #260]
	bl 0x020093c0
	movs	r0, #16
	bl 0x020092f8
	movs	r5, #0
	adds	r0, #85
	movs	r1, #192
	movs	r2, #192
	lsls	r2, r2, #8
	strb	r5, [r0, #0]
	lsls	r1, r1, #9
	movs	r0, #16
	bl 0x02009300
	ldr	r1, [pc, #232]
	movs	r0, #16
	bl 0x02009308
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #7
	movs	r0, #18
	lsls	r1, r1, #8
	bl 0x02009300
	ldr	r1, [pc, #212]
	movs	r0, #18
	bl 0x02009308
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	adds	r2, #51
	movs	r0, #17
	adds	r1, #102
	bl 0x02009300
	ldr	r1, [pc, #192]
	movs	r0, #17
	bl 0x02009308
	movs	r0, #16
	bl 0x02009310
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r0, #16
	bl 0x020093a0
	movs	r0, #10
	bl 0x020092d8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #16
	bl 0x020093a0
	movs	r0, #10
	bl 0x020092d8
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #16
	bl 0x020093a0
	movs	r0, #10
	bl 0x020092d8
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #16
	bl 0x020093a0
	movs	r0, #10
	bl 0x020092d8
	movs	r1, #0
	movs	r0, #16
	bl 0x020093a0
	movs	r0, #10
	bl 0x020092d8
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #16
	bl 0x020093a0
	movs	r0, #10
	bl 0x020092d8
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #16
	bl 0x020093a0
	movs	r0, #40
	bl 0x020092d8
	movs	r1, #3
	movs	r0, #16
	bl 0x02009358
	movs	r0, #60
	bl 0x020092d8
	movs	r1, #0
	movs	r0, #16
	bl 0x02009390
	movs	r0, #17
	bl 0x02009310
	movs	r0, #18
	bl 0x02009310
	movs	r2, #0
	movs	r1, #18
	movs	r0, #17
	bl 0x02009378
	movs	r0, #20
	bl 0x020092d8
	b.n	.L_020008f0
	.2byte 0x0000
	.4byte 0x00002cab
	.4byte 0x021e0000
	.4byte 0x02009470
	.4byte 0x0200951c
	.2byte 0x954c
	.2byte 0x0200
.L_020008f0:
	movs	r0, #18
	movs	r1, #3
	bl 0x02009358
	movs	r1, #3
	movs	r0, #17
	bl 0x02009358
	movs	r0, #30
	bl 0x020092d8
	movs	r0, #17
	movs	r1, #4
	movs	r2, #0
	bl 0x02009370
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #16
	lsls	r1, r1, #9
	bl 0x02009300
	ldr	r1, [pc, #704]
	movs	r0, #16
	bl 0x02009308
	movs	r0, #230
	movs	r1, #228
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	adds	r0, #204
	adds	r1, #153
	bl 0x020093b8
	movs	r1, #1
	movs	r2, #175
	ldr	r0, [pc, #680]
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x020093c0
	bl 0x020093c8
	movs	r1, #0
	movs	r0, #17
	bl 0x02009388
	movs	r0, #4
	movs	r1, #0
	bl 0x020092f0
	cmp	r0, #0
	bne.n	.L_02000992
	movs	r0, #18
	movs	r1, #3
	bl 0x02009358
	movs	r0, #18
	movs	r1, #0
	bl 0x02009390
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020009c4
.L_02000982:
	movs	r1, #216
	movs	r2, #118
	movs	r0, #4
	adds	r1, #255
.L_0200098a:
	adds	r2, #255
	bl 0x02009330
	b.n	.L_0200021e
.L_02000992:
	movs	r0, #18
	movs	r1, #4
	bl 0x02009358
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r0, #226
	lsls	r0, r0, #1
	adds	r2, r2, r0
	ldrh	r3, [r2, #0]
	movs	r0, #17
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x02009390
	movs	r0, #18
	movs	r1, #3
	bl 0x02009358
	movs	r0, #18
	movs	r1, #0
	bl 0x02009390
.L_020009c4:
	movs	r0, #18
	movs	r1, #0
	movs	r2, #24
	bl 0x02009408
	movs	r0, #17
	movs	r1, #0
	movs	r2, #24
	bl 0x02009410
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x02009398
	movs	r1, #208
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #18
	bl 0x02009398
	movs	r0, #16
	bl 0x02009310
	ldr	r1, [pc, #496]
	movs	r0, #16
	bl 0x02009308
	movs	r0, #16
	bl 0x02009310
	movs	r1, #192
	movs	r0, #18
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009398
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x02009398
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #16
	bl 0x02009398
	movs	r0, #50
	bl 0x020092d8
	movs	r0, #18
	movs	r1, #3
	bl 0x02009358
	movs	r0, #17
	movs	r1, #3
	bl 0x02009358
	movs	r1, #3
	movs	r0, #16
	bl 0x02009358
	movs	r0, #60
	bl 0x020092d8
	movs	r0, #4
	movs	r1, #16
	bl 0x02009418
	movs	r1, #252
	movs	r2, #185
	movs	r0, #18
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x02009328
	movs	r1, #252
	movs	r2, #176
	lsls	r2, r2, #1
	movs	r0, #17
	lsls	r1, r1, #1
	bl 0x02009328
	ldr	r1, [pc, #380]
	movs	r0, #16
	bl 0x02009308
	movs	r0, #17
	bl 0x02009340
	movs	r0, #18
	bl 0x02009340
	movs	r0, #16
	bl 0x02009310
	movs	r0, #16
	movs	r1, #0
	bl 0x020093a0
	movs	r1, #152
	movs	r2, #152
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #16
	adds	r1, #204
	adds	r2, #102
	bl 0x02009300
	movs	r1, #152
	movs	r2, #152
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #17
	adds	r1, #204
	adds	r2, #102
	bl 0x02009300
	movs	r1, #152
	movs	r2, #152
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	adds	r1, #204
	adds	r2, #102
	movs	r0, #18
	bl 0x02009300
	movs	r0, #40
	bl 0x020092d8
	movs	r1, #200
	movs	r2, #192
	lsls	r1, r1, #5
	lsls	r2, r2, #4
	adds	r2, #204
	adds	r1, #153
	movs	r0, #20
	bl 0x02009300
	movs	r0, #20
	bl 0x020092f8
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	ldr	r1, [pc, #260]
	movs	r0, #20
	bl 0x02009308
	movs	r0, #20
	bl 0x02009310
	movs	r1, #152
	movs	r2, #152
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	adds	r2, #102
	movs	r0, #20
	adds	r1, #204
	bl 0x02009300
	movs	r0, #152
	movs	r1, #144
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #204
	adds	r1, #153
	bl 0x020093b8
	movs	r0, #230
	movs	r1, #1
	movs	r2, #175
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #17
	bl 0x020093c0
	movs	r0, #30
	bl 0x020092d8
	movs	r0, #16
	bl 0x020092f8
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #40
	movs	r2, #20
	strb	r3, [r0, #0]
	negs	r1, r1
	movs	r0, #18
	negs	r2, r2
	bl 0x02009408
	movs	r1, #40
	movs	r2, #20
	movs	r0, #17
	negs	r1, r1
	negs	r2, r2
	bl 0x02009408
	movs	r1, #40
	movs	r2, #20
	movs	r0, #16
	negs	r1, r1
	negs	r2, r2
	bl 0x02009408
	movs	r1, #40
	movs	r2, #20
	movs	r0, #20
	negs	r1, r1
	negs	r2, r2
	bl 0x02009410
	movs	r1, #138
	movs	r0, #18
	negs	r1, r1
	movs	r2, #0
	bl 0x02009408
	movs	r1, #138
	movs	r0, #17
	negs	r1, r1
	movs	r2, #0
	bl 0x02009408
	movs	r1, #138
	movs	r0, #16
	negs	r1, r1
	movs	r2, #0
	bl 0x02009408
	movs	r1, #138
	movs	r0, #20
	negs	r1, r1
	movs	r2, #0
	bl 0x02009410
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x02009348
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x02009348
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x02009348
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x02009348
	bl 0x020092e8
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200958c
	.4byte 0x021e0000
	.4byte 0x020095e0
	.4byte 0x02009638
	.2byte 0x966c
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x02009288
	cmp	r0, #0
	beq.n	.L_02000c0c
	ldr	r0, [pc, #20]
	bl 0x02009380
	b.n	.L_02000c12
.L_02000c0c:
	ldr	r0, [pc, #16]
	bl 0x02009380
.L_02000c12:
	movs	r0, #11
	movs	r1, #0
	bl 0x02009390
	pop	{pc}
	.4byte 0x00002cc1
	.2byte 0x2cda
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x02009288
	cmp	r0, #0
	beq.n	.L_02000c3c
	ldr	r0, [pc, #20]
	bl 0x02009380
	b.n	.L_02000c42
.L_02000c3c:
	ldr	r0, [pc, #16]
	bl 0x02009380
.L_02000c42:
	movs	r0, #11
	movs	r1, #0
	bl 0x02009390
	pop	{pc}
	.4byte 0x00002cc5
	.2byte 0x2cdb
	.2byte 0x0000
	push	{lr}
	movs	r2, #192
	movs	r1, #64
	lsls	r2, r2, #2
	bl 0x020093f0
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r2, #192
	lsls	r2, r2, #2
	movs	r1, #65
	adds	r2, #1
	bl 0x020093f0
	pop	{pc}
	push	{lr}
	movs	r2, #192
	lsls	r2, r2, #2
	movs	r1, #66
	adds	r2, #2
	bl 0x020093f0
	pop	{pc}
	push	{lr}
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009290
	movs	r0, #22
	bl 0x020092f8
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #247
	bl 0x02009288
	cmp	r0, #0
	bne.n	.L_02000cbc
	movs	r1, #130
	movs	r2, #136
	movs	r0, #66
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x02009348
.L_02000cbc:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000cd8
	ldr	r0, [pc, #24]
	b.n	.L_02000ce4
.L_02000cd8:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000ce2
	ldr	r0, [pc, #24]
	b.n	.L_02000ce4
.L_02000ce2:
	ldr	r0, [pc, #24]
.L_02000ce4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000010a
	.4byte 0x02009aec
	.4byte 0x0000010b
	.4byte 0x02009dd4
	.2byte 0x9ae0
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x020092f8
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #20]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000d3c
	ldr	r0, [pc, #16]
	movs	r1, #1
	bl 0x020092d0
	b.n	.L_02000d3c
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x2cdc
	.2byte 0x0000
.L_02000d3c:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r1, #8
	movs	r0, #13
	bl 0x02009440
	pop	{pc}
	push	{lr}
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x020092f8
	movs	r0, #200
	lsls	r0, r0, #2
	bl 0x02009288
	cmp	r0, #0
	beq.n	.L_02000d72
	movs	r0, #30
	movs	r1, #8
	bl 0x02009430
	b.n	.L_02000d96
.L_02000d72:
	movs	r0, #150
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009288
	cmp	r0, #0
	beq.n	.L_02000d88
	ldr	r0, [pc, #24]
	bl 0x02009380
	b.n	.L_02000d8e
.L_02000d88:
	ldr	r0, [pc, #20]
	bl 0x02009380
.L_02000d8e:
	movs	r0, #8
	movs	r1, #0
	bl 0x02009390
.L_02000d96:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00002cd4
	.2byte 0x2c9d
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020092f8
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
	bne.n	.L_02000dd4
	adds	r0, r5, #0
	bl 0x02009438
	b.n	.L_02000e06
.L_02000dd4:
	movs	r0, #150
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009288
	cmp	r0, #0
	beq.n	.L_02000df8
	ldr	r0, [pc, #16]
	bl 0x02009380
	b.n	.L_02000dfe
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x2cd6
	.2byte 0x0000
.L_02000df8:
	ldr	r0, [pc, #12]
	bl 0x02009380
.L_02000dfe:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009390
.L_02000e06:
	pop	{r5, pc}
	.2byte 0x2c9f
	.2byte 0x0000
	push	{lr}
	ldr	r1, [pc, #16]
	movs	r0, #8
	bl 0x02009308
	movs	r0, #200
	lsls	r0, r0, #2
	bl 0x02009290
	pop	{pc}
	.2byte 0x97d8
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #16]
	movs	r0, #8
	bl 0x02009308
	movs	r0, #200
	lsls	r0, r0, #2
	bl 0x02009290
	pop	{pc}
	.2byte 0x97a4
	.2byte 0x0200
	push	{lr}
	movs	r0, #200
	lsls	r0, r0, #2
	bl 0x02009298
	ldr	r1, [pc, #8]
	movs	r0, #8
	bl 0x02009308
	pop	{pc}
	.2byte 0x971c
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl 0x020092e0
	movs	r0, #0
	bl 0x020093f8
	movs	r5, #8
.L_02000e68:
	adds	r0, r5, #0
	bl 0x020092f8
	cmp	r0, #0
	beq.n	.L_02000e7a
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_02000e7a:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_02000e68
	movs	r3, #170
	lsls	r3, r3, #1
	movs	r0, #158
	adds	r6, r6, r3
	bl 0x02009448
	ldr	r3, [pc, #84]
	ldrh	r2, [r3, #6]
	ldrh	r1, [r3, #4]
	ldr	r0, [r3, #0]
	bl 0x020092a0
	ldr	r5, [pc, #76]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x02009358
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009300
	movs	r2, #4
	movs	r1, #0
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x02009338
	movs	r0, #123
	bl 0x02009448
	movs	r0, #6
	bl 0x020092d8
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl 0x020093d0
	bl 0x020093d8
	bl 0x020093e0
	bl 0x020092e8
	pop	{r5, r6, pc}
	.4byte 0x02009f14
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [pc, #64]
	ldr	r6, [r3, #108]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x02009358
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009300
	movs	r0, #123
	bl 0x02009448
	movs	r2, #4
	ldr	r0, [r5, #0]
	movs	r1, #0
	negs	r2, r2
	bl 0x02009338
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r6, r3
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl 0x020093d0
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	sub	sp, #8
	cmp	r5, #2
	bne.n	.L_02000f60
	movs	r1, #144
	movs	r2, #224
	movs	r0, #8
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009348
	b.n	.L_02000f6e
.L_02000f60:
	movs	r1, #224
	movs	r2, #224
	movs	r0, #8
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009348
.L_02000f6e:
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x02009398
	movs	r0, #10
	bl 0x020092d8
	movs	r0, #4
	bl 0x020092f8
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #1
	movs	r3, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #14
	movs	r0, #96
	movs	r1, #14
	movs	r2, #72
	bl 0x020092a8
	movs	r2, #4
	movs	r1, #0
	movs	r0, #4
	bl 0x02009338
	movs	r0, #4
	bl 0x02009340
	movs	r0, #4
	bl 0x020092f8
	movs	r1, #0
	bl 0x020092c0
	movs	r0, #4
	movs	r1, #13
	bl 0x02009358
	movs	r2, #16
	movs	r1, #0
	movs	r0, #4
	bl 0x02009338
	movs	r0, #4
	bl 0x02009340
	movs	r1, #10
	movs	r0, #4
	bl 0x02009358
	movs	r0, #14
	bl 0x020092d8
	movs	r0, #123
	bl 0x02009448
	adds	r0, r5, #0
	bl 0x020093d0
	bl 0x020092e8
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	bl 0x020092e0
	movs	r0, #0
	bl 0x020093f8
	movs	r2, #14
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x02009410
	movs	r1, #16
	movs	r2, #0
	movs	r0, #4
	bl 0x02009410
	movs	r0, #10
	bl 0x020092d8
	bl 0x02008f38
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x020092e0
	movs	r0, #0
	bl 0x020093f8
	movs	r2, #14
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x02009410
	movs	r1, #16
	negs	r1, r1
	movs	r2, #0
	movs	r0, #4
	bl 0x02009410
	movs	r0, #10
	bl 0x020092d8
	bl 0x02008f38
	pop	{pc}
	push	{lr}
	bl 0x020092e0
	movs	r0, #0
	bl 0x020093f8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x02009398
	movs	r0, #10
	bl 0x020092d8
	bl 0x02008f38
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	movs	r0, #20
	bl 0x020092f8
	movs	r1, #0
	bl 0x020092c0
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	movs	r0, #9
	bl 0x020092f8
	movs	r1, #0
	bl 0x020092c0
	pop	{pc}
	push	{r5, lr}
	ldr	r3, [pc, #284]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #276]
	cmp	r2, r3
	bne.n	.L_0200111a
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02009288
	cmp	r0, #0
	bne.n	.L_020010e8
	movs	r0, #64
	movs	r1, #0
	bl 0x020093e8
.L_020010e8:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02009288
	cmp	r0, #0
	bne.n	.L_020010fe
	movs	r0, #65
	movs	r1, #1
	bl 0x020093e8
.L_020010fe:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009288
	cmp	r0, #0
	bne.n	.L_02001114
	movs	r0, #66
	movs	r1, #1
	bl 0x020093e8
.L_02001114:
	bl 0x02009080
	b.n	.L_0200111e
.L_0200111a:
	bl 0x020090a0
.L_0200111e:
	movs	r0, #150
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009288
	cmp	r0, #0
	beq.n	.L_020011dc
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x02009348
	movs	r1, #248
	movs	r2, #240
	movs	r0, #14
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x02009350
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x02009288
	cmp	r0, #0
	bne.n	.L_02001174
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x02009348
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x02009348
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x02009348
	b.n	.L_020011dc
.L_02001174:
	movs	r3, #224
	movs	r2, #254
	lsls	r3, r3, #8
	movs	r0, #16
	ldr	r1, [pc, #104]
	lsls	r2, r2, #16
	bl 0x02009350
	movs	r3, #128
	movs	r1, #221
	movs	r2, #203
	lsls	r3, r3, #8
	movs	r0, #18
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02009350
	movs	r3, #128
	movs	r1, #245
	movs	r2, #188
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	lsls	r3, r3, #6
	movs	r0, #17
	bl 0x02009350
	movs	r0, #17
	bl 0x020092f8
	adds	r5, r0, #0
	movs	r0, #17
	bl 0x020092f8
	movs	r2, #10
	ldrsh	r3, [r0, r2]
	adds	r5, #100
	strh	r3, [r5, #0]
	movs	r0, #17
	bl 0x020092f8
	adds	r5, r0, #0
	movs	r0, #17
	bl 0x020092f8
	movs	r1, #18
	ldrsh	r3, [r0, r1]
	adds	r5, #102
	strh	r3, [r5, #0]
	movs	r0, #17
	movs	r1, #2
	bl 0x02009308
.L_020011dc:
	movs	r0, #0
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x0000010a
	.2byte 0x0000
	.2byte 0x01cb
	push	{lr}
	ldr	r3, [pc, #40]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_02001212
	movs	r0, #150
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009288
	cmp	r0, #0
	beq.n	.L_02001212
	bl 0x020080ac
.L_02001212:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000010a
	.irp EntryTarget, 0x080000c1, 0x080000f9, 0x08000169, 0x08000171, 0x08000179, 0x080001a9, 0x080001b9, 0x080001c9, 0x080001d1, 0x080001e9, 0x08000291, 0x080002b9, 0x080002c1, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020171, 0x08020179, 0x080201e9, 0x080201f1, 0x08020219, 0x08020229, 0x08038041, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80c1, 0x080c80c9, 0x080c80d1, 0x080c80d9, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8101, 0x080c8119, 0x080c8141, 0x080c8149, 0x080c8159, 0x080c8161, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c8211, 0x080c8219, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8279, 0x080c83b1, 0x080c83b9, 0x080c8421, 0x080c8429, 0x080c84e1, 0x080c8581, 0x080c85f1, 0x080c85f9, 0x080c8601, 0x080c8779, 0x080c8861, 0x08108009, 0x08108011, 0x08108019, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x575a0260
	.4byte 0x46754ad7
	.4byte 0x31cf3a32
	.4byte 0x28ea294c
	.4byte 0x00750009
	.4byte 0x01bf011f
	.4byte 0x031f027f
	.4byte 0x7fff03ff
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x021b0000
	.4byte 0x01470000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x02470000
	.4byte 0x01470000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x02470000
	.4byte 0x012c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000096
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02460000
	.4byte 0x00000000
	.4byte 0x010a0000
	.4byte 0x00000004
	.4byte 0x02460000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x023a0000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x023a0000
	.4byte 0x01160000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0xffd80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffd80000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x02000000
	.4byte 0x012f0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x023a0000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02470000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02460000
	.4byte 0x00000000
	.4byte 0x010a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000011
	.4byte 0x00000002
	.4byte 0x02470000
	.4byte 0x00000000
	.4byte 0x01470000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x02470000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00000030
	.4byte 0x02470000
	.4byte 0x01470000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x022a0000
	.4byte 0x014c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x01d80000
	.4byte 0x014b0000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x01d80000
	.4byte 0x01690000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x00000011
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x0000e666
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x0000e666
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x009f0000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
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
	.4byte 0x001c01c4
	.4byte 0x01cc02e4
	.4byte 0x02ec0024
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000010a
	.4byte 0x1010110b
	.4byte 0xffffffff
	.4byte 0x1020210b
	.4byte 0xffffffff
	.4byte 0x1030310b
	.4byte 0xffffffff
	.4byte 0x1040410b
	.4byte 0xffffffff
	.4byte 0x1050510b
	.4byte 0xffffffff
	.4byte 0x1060610b
	.4byte 0xffffffff
	.4byte 0x1080610b
	.4byte 0xffffffff
	.4byte 0x1090610b
	.4byte 0xffffffff
	.4byte 0x1073b002
	.4byte 0xffffffff
	.4byte 0x0000010b
	.4byte 0x1010110a
	.4byte 0xffffffff
	.4byte 0x1020210a
	.4byte 0xffffffff
	.4byte 0x1030310a
	.4byte 0xffffffff
	.4byte 0x1040410a
	.4byte 0xffffffff
	.4byte 0x1050510a
	.4byte 0xffffffff
	.4byte 0x1060610a
	.4byte 0xffffffff
	.4byte 0x1070810b
	.4byte 0xffffffff
	.4byte 0x1080710b
	.4byte 0xffffffff
	.4byte 0x1090a10b
	.4byte 0xffffffff
	.4byte 0x10a0910b
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff00dd
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00016000
	.4byte 0xffff00dd
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00012000
	.4byte 0xffff00dd
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00018000
	.4byte 0xffff009a
	.4byte 0x00000001
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x0001c000
	.4byte 0xffff00dd
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x0001a000
	.4byte 0xffff00dd
	.4byte 0x00000002
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x0001c000
	.4byte 0xffff00dd
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00010000
	.4byte 0xffff009a
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x0001c000
	.4byte 0xffff00dd
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x0001a000
	.4byte 0xffff009a
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00010000
	.4byte 0xffff00dd
	.4byte 0x00000001
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00018000
	.4byte 0x007c00f6
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff01a5
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0001c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0082
	.4byte 0x020096b8
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0001a000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
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
	.4byte 0x02008eed
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008eed
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008eed
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008eed
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008e55
	.4byte 0x00004602
	.4byte 0xffff0006
	.4byte 0x0200905d
	.4byte 0x00008602
	.4byte 0xffff0008
	.4byte 0x0200902d
	.4byte 0x00000602
	.4byte 0xffff0009
	.4byte 0x02008ffd
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x1a5f0008
	.4byte 0x00002cbe
	.4byte 0x00000000
	.4byte 0x1a5f0009
	.4byte 0x00002cbf
	.4byte 0x00000000
	.4byte 0x1a5f000a
	.4byte 0x00002cc0
	.4byte 0x00000000
	.4byte 0x1a5f000b
	.4byte 0x02008bf5
	.4byte 0x00008d15
	.4byte 0x1a5f0008
	.4byte 0x00002cc2
	.4byte 0x00008d15
	.4byte 0x1a5f0009
	.4byte 0x00002cc3
	.4byte 0x00008d15
	.4byte 0x1a5f000a
	.4byte 0x00002cc4
	.4byte 0x00008d15
	.4byte 0x1a5f000b
	.4byte 0x02008c25
	.4byte 0x00000000
	.4byte 0x1a5f000c
	.4byte 0x00002cc6
	.4byte 0x00000000
	.4byte 0x1a5f000d
	.4byte 0x00002cc7
	.4byte 0x00000000
	.4byte 0x1a5f000e
	.4byte 0x00002cc8
	.4byte 0x00000000
	.4byte 0x1a5f000f
	.4byte 0x00002cc9
	.4byte 0x00000000
	.4byte 0x1a5f0010
	.4byte 0x00002cca
	.4byte 0x00000000
	.4byte 0x1a5f0012
	.4byte 0x00002ccb
	.4byte 0x00000000
	.4byte 0x1a5f0011
	.4byte 0x00002ccc
	.4byte 0x00008d15
	.4byte 0x1a5f000c
	.4byte 0x00002ccd
	.4byte 0x00008d15
	.4byte 0x1a5f000d
	.4byte 0x00002cce
	.4byte 0x00008d15
	.4byte 0x1a5f000e
	.4byte 0x00002ccf
	.4byte 0x00008d15
	.4byte 0x1a5f000f
	.4byte 0x00002cd0
	.4byte 0x00008d15
	.4byte 0x1a5f0010
	.4byte 0x00002cd1
	.4byte 0x00008d15
	.4byte 0x1a5f0012
	.4byte 0x00002cd2
	.4byte 0x00008d15
	.4byte 0x1a5f0011
	.4byte 0x00002cd3
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002c8d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002c8e
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002c8f
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002c90
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002c95
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002c96
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002c97
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002c98
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002ca1
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002ca2
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0200812d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x02008179
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002c91
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002c92
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002c93
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002c94
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002c99
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002c9a
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002c9b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002c9c
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002ca6
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002ca7
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002ca8
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x00403042
	.4byte 0x0001cc14
	.4byte 0xffff0014
	.4byte 0x02008199
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008039
	.4byte 0x50008805
	.4byte 0x03000065
	.4byte 0x02008c55
	.4byte 0x50008805
	.4byte 0x03010066
	.4byte 0x02008c65
	.4byte 0x50008805
	.4byte 0x03020067
	.4byte 0x02008c75
	.4byte 0x00008f15
	.4byte 0x02010016
	.4byte 0x02008c85
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
	.4byte 0x0000c402
	.4byte 0x03200014
	.4byte 0x02008e25
	.4byte 0x00008402
	.4byte 0x03200016
	.4byte 0x02008e0d
	.4byte 0x00000002
	.4byte 0x13200015
	.4byte 0x02008e3d
	.4byte 0x00000002
	.4byte 0x13200017
	.4byte 0x02008e3d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008d4d
	.4byte 0x00008d15
	.4byte 0x0a5f0008
	.4byte 0x00002c9e
	.4byte 0x00008d15
	.4byte 0x1a5f0008
	.4byte 0x00002cd5
	.4byte 0x00000000
	.4byte 0x13200009
	.4byte 0x02008d41
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008da5
	.4byte 0x00008d15
	.4byte 0x0a5f000a
	.4byte 0x00002ca0
	.4byte 0x00008d15
	.4byte 0x1a5f000a
	.4byte 0x00002cd7
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte 0x02008d41
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte 0x02008d01
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00320040
	.4byte 0x00020002
	.4byte 0x00400002
	.4byte 0x00020034
	.4byte 0x00020002
	.4byte 0x00360040
	.4byte 0x00020002
	.4byte 0xffff0002
	.4byte 0x02009ef4
	.4byte 0x000f004f
