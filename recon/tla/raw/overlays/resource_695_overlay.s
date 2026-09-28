.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008e05, 0x02008039, 0x02008045, 0x02008351, 0x02008395, 0x02008041, 0x02008fe9
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9218
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9248
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r1, #0
	sub	sp, #8
	cmp	r5, #8
	bne.n	.L_02000072
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #169
	bl 0x02009050
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #2
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x02009060
.L_02000072:
	cmp	r5, #10
	bne.n	.L_02000094
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #170
	bl 0x02009050
	movs	r3, #14
	movs	r2, #38
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	bl 0x02009060
.L_02000094:
	add	sp, #8
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	bl 0x020090a0
	adds	r5, r0, #0
	adds	r7, r5, #0
	adds	r7, #85
	movs	r3, #3
	movs	r6, #60
	strb	r3, [r7, #0]
	b.n	.L_020000ae
.L_020000ac:
	subs	r6, #1
.L_020000ae:
	cmp	r6, #0
	beq.n	.L_020000be
	movs	r0, #1
	bl 0x02009040
	ldr	r3, [r5, #40]
	cmp	r3, #0
	bne.n	.L_020000ac
.L_020000be:
	movs	r0, #10
	bl 0x02009040
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009068
	movs	r3, #0
	strb	r3, [r7, #0]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #99
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #99
	movs	r1, #10
	movs	r2, #4
	movs	r3, #3
	bl 0x02009060
	add	sp, #8
	pop	{pc}
	push	{r5, r6, r7, lr}
	adds	r7, r1, #0
	adds	r0, r7, #0
	sub	sp, #8
	bl 0x020090a0
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r6, r3, #20
	cmp	r6, #38
	bne.n	.L_02000130
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #168
	bl 0x02009050
	adds	r3, r5, #0
	adds	r3, #35
	movs	r2, #0
	strb	r2, [r3, #0]
	adds	r0, r7, #0
	bl 0x02008098
	movs	r3, #19
	str	r3, [sp, #4]
	movs	r0, #40
	movs	r1, #19
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x02009060
.L_02000130:
	movs	r3, #99
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #99
	movs	r1, #6
	movs	r2, #4
	movs	r3, #3
	bl 0x02009060
	add	sp, #8
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	movs	r0, #16
	sub	sp, #8
	bl 0x020090a0
	adds	r5, r0, #0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009050
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #35
	strb	r3, [r2, #0]
	strb	r3, [r5, #0]
	movs	r2, #42
	movs	r3, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #8
	movs	r1, #42
	movs	r2, #1
	movs	r3, #1
	bl 0x02009060
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r0, #17
	bl 0x020090a0
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020090a0
	ldr	r3, [r0, #8]
	asrs	r2, r3, #19
	ldr	r3, [r0, #16]
	asrs	r1, r3, #19
	ldr	r3, [r7, #8]
	asrs	r3, r3, #19
	cmp	r2, #36
	bgt.n	.L_020001b2
	cmp	r3, #37
	ble.n	.L_020001b2
	b.n	.L_02000306
.L_020001b2:
	cmp	r2, #37
.L_020001b4:
	ble.n	.L_020001bc
	cmp	r3, #36
	bgt.n	.L_020001bc
	b.n	.L_02000306
.L_020001bc:
	movs	r6, #1
	cmp	r2, #36
	ble.n	.L_020001c4
	negs	r6, r6
.L_020001c4:
	movs	r5, #1
	negs	r5, r5
	cmp	r1, #95
	bgt.n	.L_020001ce
	movs	r5, #1
.L_020001ce:
	cmp	r5, #1
	bne.n	.L_020002a0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009048
	cmp	r0, #0
	beq.n	.L_020002a0
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x02009048
	cmp	r0, #0
	beq.n	.L_020001ee
	b.n	.L_02000306
.L_020001ee:
	bl 0x02009080
	movs	r0, #0
	bl 0x020091e0
	movs	r0, #17
	movs	r1, #4
	movs	r2, #20
	bl 0x02009108
	movs	r2, #32
	movs	r1, #0
	movs	r0, #17
	bl 0x020090d8
	movs	r0, #17
	bl 0x020090e0
	movs	r0, #17
	movs	r1, #1
	bl 0x02009188
	movs	r0, #17
	movs	r1, #2
	bl 0x02009110
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009150
	movs	r1, #8
	negs	r1, r1
	movs	r2, #0
	movs	r0, #17
	bl 0x020090d8
	movs	r0, #17
	bl 0x020090e0
	movs	r2, #20
	movs	r0, #17
	movs	r1, #4
	bl 0x02009108
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x02009168
	movs	r0, #30
	bl 0x02009078
	movs	r0, #17
	bl 0x020090a0
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #8
	movs	r2, #0
	movs	r0, #17
	bl 0x020091f8
	movs	r0, #17
	bl 0x020090e0
	movs	r0, #30
	bl 0x02009078
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #17
	bl 0x02009160
	movs	r0, #30
	bl 0x02009078
	bl 0x02009088
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x02009050
	b.n	.L_02000306
.L_020002a0:
	bl 0x02009080
	movs	r0, #0
	bl 0x020091e0
	lsls	r5, r5, #5
	movs	r0, #17
	movs	r1, #4
	movs	r2, #20
	bl 0x02009108
	adds	r2, r5, #0
	movs	r1, #0
	movs	r0, #17
	bl 0x020090d8
	movs	r0, #17
	bl 0x020090e0
	lsls	r1, r6, #4
	movs	r2, #0
	movs	r0, #17
	bl 0x020090d8
	movs	r0, #17
	bl 0x020090e0
	lsls	r1, r6, #1
	adds	r1, r1, r6
	lsls	r1, r1, #4
	movs	r2, #0
	movs	r0, #17
	bl 0x020090d8
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r7, #40]
	movs	r0, #17
	negs	r5, r5
	bl 0x020090e0
	movs	r0, #17
	movs	r1, #0
	adds	r2, r5, #0
	bl 0x020090d8
	movs	r0, #17
	bl 0x020090e0
	bl 0x02009088
.L_02000306:
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009050
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02009050
	movs	r0, #17
	movs	r1, #35
	bl 0x020091a8
	pop	{pc}
	push	{r5, lr}
	movs	r1, #0
	adds	r5, r0, #0
	bl 0x02009068
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r1, #9]
	adds	r5, #85
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r0, #0
	pop	{r5, pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x92c8
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02009050
	pop	{pc}
	push	{lr}
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009050
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009050
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009050
	pop	{pc}
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9460
	.2byte 0x0200
	push	{lr}
	bl 0x02009080
	movs	r0, #0
	bl 0x020091e0
	movs	r1, #200
	movs	r2, #1
	movs	r0, #22
	lsls	r1, r1, #1
	negs	r2, r2
	bl 0x02009098
	cmp	r0, #0
	bne.n	.L_020003c4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x02009058
.L_020003c4:
	bl 0x02009088
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r5, #160
	lsls	r5, r5, #7
	movs	r1, #231
	movs	r2, #199
	adds	r3, r5, #0
	movs	r0, #19
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x020090f0
	movs	r1, #220
	movs	r2, #191
	adds	r3, r5, #0
	movs	r0, #20
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x020090f0
	movs	r1, #204
	movs	r2, #191
	movs	r0, #21
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	adds	r3, r5, #0
	bl 0x020090f0
	pop	{r5, pc}
	push	{lr}
	movs	r1, #200
	movs	r0, #22
	lsls	r1, r1, #1
	bl 0x02009200
	movs	r3, #192
	movs	r1, #212
	movs	r2, #199
	lsls	r3, r3, #8
	movs	r0, #22
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x020090f0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x02009050
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x02009048
	cmp	r0, #0
	bne.n	.L_02000442
	b.n	.L_0200082c
.L_02000442:
	movs	r0, #0
	bl 0x02009048
	cmp	r0, #0
	bne.n	.L_0200044e
	b.n	.L_0200082c
.L_0200044e:
	bl 0x02009080
	movs	r0, #0
	bl 0x020091e0
	movs	r0, #167
	lsls	r0, r0, #4
	bl 0x02009050
	ldr	r0, [pc, #972]
	bl 0x02009138
	movs	r0, #4
	movs	r1, #1
	bl 0x020090f8
	movs	r0, #19
	movs	r1, #0
	movs	r2, #1
	bl 0x02009148
	movs	r2, #221
	movs	r0, #4
	movs	r1, #200
	bl 0x020090c8
	movs	r1, #2
	movs	r0, #4
	bl 0x02009118
	movs	r0, #5
	bl 0x02009078
	movs	r1, #16
	movs	r0, #4
	negs	r1, r1
	movs	r2, #8
	bl 0x020091f8
	movs	r3, #192
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x020091f0
	movs	r3, #192
	lsls	r3, r3, #8
	movs	r1, #8
	movs	r2, #16
	movs	r0, #18
	bl 0x020091f0
	movs	r0, #0
	bl 0x020090e0
	movs	r0, #18
	bl 0x020090e0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #0
	bl 0x02009150
	movs	r0, #20
	bl 0x02009078
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009150
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009150
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x02009150
	movs	r0, #10
	bl 0x02009078
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #0
	bl 0x02009160
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #18
	bl 0x02009160
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #4
	bl 0x02009160
	movs	r0, #199
	movs	r1, #1
	movs	r2, #177
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	movs	r5, #128
	bl 0x02009178
	lsls	r5, r5, #7
	bl 0x02009180
	movs	r1, #199
	movs	r2, #130
	adds	r3, r5, #0
	movs	r0, #19
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x020090f0
	movs	r1, #231
	movs	r2, #199
	movs	r0, #19
	bl 0x020090c0
	movs	r0, #20
	bl 0x02009078
	movs	r1, #199
	movs	r2, #130
	adds	r3, r5, #0
	movs	r0, #20
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x020090f0
	movs	r1, #220
	movs	r2, #191
	movs	r0, #20
	bl 0x020090c0
	movs	r0, #20
	bl 0x02009078
	movs	r1, #199
	movs	r2, #130
	adds	r3, r5, #0
	movs	r0, #21
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x020090f0
	movs	r0, #21
	movs	r1, #204
	movs	r2, #191
	bl 0x020090c0
	movs	r0, #199
	movs	r1, #1
	movs	r2, #225
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #16
	bl 0x02009178
	bl 0x02009180
	movs	r0, #19
	bl 0x020090e0
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #19
	bl 0x02009150
	movs	r0, #20
	bl 0x020090e0
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #20
	bl 0x02009150
	movs	r0, #21
	bl 0x020090e0
	movs	r1, #160
	movs	r0, #21
	lsls	r1, r1, #7
	bl 0x02009158
	movs	r0, #21
	movs	r1, #2
	bl 0x02009118
	movs	r0, #21
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #18
	bl 0x02009160
	movs	r0, #18
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #19
	bl 0x02009160
	movs	r2, #5
	movs	r0, #19
	movs	r1, #0
	bl 0x02009148
	movs	r1, #2
	movs	r0, #20
	bl 0x02009118
	movs	r0, #5
	bl 0x02009078
	movs	r0, #20
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #21
	bl 0x02009160
	movs	r2, #5
	movs	r0, #21
	movs	r1, #0
	bl 0x02009148
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x02009158
	movs	r1, #3
	movs	r0, #0
	bl 0x02009100
	movs	r0, #10
	bl 0x02009078
	movs	r2, #5
	movs	r0, #0
	movs	r1, #0
	bl 0x02009148
	movs	r1, #2
	movs	r0, #19
	bl 0x02009118
	movs	r0, #5
	bl 0x02009078
	movs	r0, #19
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #4
	movs	r2, #0
	movs	r0, #18
	bl 0x02009128
	movs	r0, #10
	bl 0x02009078
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #0
	bl 0x02009160
	movs	r0, #0
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #20
	bl 0x02009160
	movs	r0, #20
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #224
	movs	r2, #0
	movs	r0, #4
	lsls	r1, r1, #8
	bl 0x02009150
	movs	r1, #224
	movs	r0, #18
	lsls	r1, r1, #8
	bl 0x02009158
	movs	r0, #18
	movs	r1, #3
	bl 0x020090f8
	movs	r1, #3
	movs	r0, #4
	bl 0x02009100
	movs	r0, #10
	bl 0x02009078
	movs	r0, #21
	movs	r1, #4
	bl 0x02009100
	movs	r0, #21
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #18
	bl 0x02009160
	movs	r2, #5
	movs	r0, #18
	movs	r1, #0
	bl 0x02009148
	movs	r0, #21
	movs	r1, #4
	bl 0x020090f8
	movs	r0, #19
	movs	r1, #4
	bl 0x020090f8
	movs	r0, #20
	movs	r1, #4
	bl 0x02009100
	movs	r0, #19
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x02009160
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #18
	bl 0x02009160
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #0
	bl 0x02009160
	movs	r0, #0
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #21
	movs	r2, #0
	movs	r0, #20
	bl 0x02009128
	movs	r0, #10
	bl 0x02009078
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #20
	bl 0x02009160
	movs	r0, #20
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #160
	movs	r0, #20
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009150
	movs	r1, #160
	movs	r2, #0
	movs	r0, #21
	lsls	r1, r1, #7
	bl 0x02009150
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #18
	bl 0x02009158
	movs	r0, #10
	bl 0x02009078
	movs	r0, #18
	movs	r1, #4
	bl 0x02009100
	movs	r2, #5
	movs	r0, #18
	movs	r1, #0
	bl 0x02009148
	movs	r0, #21
	movs	r1, #4
	bl 0x02009100
	movs	r0, #21
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #16
	movs	r0, #20
	negs	r1, r1
	movs	r2, #24
	bl 0x020090d8
	movs	r1, #16
	movs	r0, #21
	negs	r1, r1
	movs	r2, #24
	bl 0x020090d8
	movs	r1, #16
	negs	r1, r1
	movs	r2, #24
	movs	r0, #19
	bl 0x020090d8
	movs	r0, #19
	bl 0x020090e0
	ldr	r3, [pc, #44]
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r5, [pc, #36]
	movs	r1, #20
	adds	r0, r5, #0
	bl 0x020091b0
	adds	r0, r5, #0
	movs	r1, #21
	bl 0x020091b8
	movs	r0, #102
	movs	r1, #2
	bl 0x020091a0
.L_0200082c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00002f5d
	.4byte 0x02000240
	.2byte 0x00f0
	.2byte 0x0000
	push	{r5, lr}
	bl 0x02009080
	movs	r0, #0
	bl 0x020091e0
	ldr	r3, [pc, #40]
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r5, [pc, #32]
	movs	r1, #20
	adds	r0, r5, #0
	bl 0x020091b0
	adds	r0, r5, #0
	movs	r1, #21
	bl 0x020091b8
	movs	r0, #102
	movs	r1, #2
	bl 0x020091a0
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x00f0
	.2byte 0x0000
	push	{r5, lr}
	bl 0x02009080
	movs	r0, #0
	bl 0x020091e0
	movs	r0, #199
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #388]
	movs	r3, #0
	lsls	r0, r0, #16
	bl 0x02009178
	movs	r5, #160
	ldr	r0, [pc, #380]
	bl 0x02009138
	lsls	r5, r5, #8
	movs	r1, #184
	movs	r2, #229
	movs	r0, #4
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	adds	r3, r5, #0
	bl 0x020090f0
	movs	r1, #200
	movs	r2, #221
	movs	r0, #0
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	adds	r3, r5, #0
	bl 0x020090f0
	movs	r3, #192
	movs	r1, #218
	movs	r2, #160
	lsls	r3, r3, #8
	lsls	r2, r2, #17
	movs	r0, #18
	lsls	r1, r1, #16
	bl 0x020090f0
	movs	r0, #4
	movs	r1, #19
	bl 0x020090f8
	movs	r0, #0
	movs	r1, #39
	bl 0x020090f8
	bl 0x020091c8
	bl 0x020091d8
	movs	r1, #2
	movs	r0, #21
	bl 0x02009118
	movs	r0, #5
	bl 0x02009078
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #21
	bl 0x02009160
	movs	r0, #21
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #19
	bl 0x02009160
	movs	r0, #19
	movs	r1, #0
	bl 0x02009140
	movs	r0, #1
	bl 0x020091e8
	cmp	r0, #0
	bne.n	.L_02000950
	bl 0x02009070
	movs	r2, #5
	movs	r0, #20
	movs	r1, #0
	bl 0x02009148
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000970
.L_02000950:
	bl 0x02009070
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #20
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
.L_02000970:
	movs	r0, #21
	movs	r1, #4
	bl 0x02009100
	movs	r2, #5
	movs	r0, #21
	movs	r1, #0
	bl 0x02009148
	movs	r1, #3
	movs	r0, #19
	bl 0x02009100
	movs	r0, #10
	bl 0x02009078
	movs	r2, #5
	movs	r0, #19
	movs	r1, #0
	bl 0x02009148
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	bl 0x02009170
	movs	r0, #199
	movs	r1, #1
	movs	r3, #1
	negs	r1, r1
	ldr	r2, [pc, #108]
	lsls	r0, r0, #16
	bl 0x02009178
	movs	r0, #100
	bl 0x02009078
	movs	r1, #6
	adds	r1, #255
	movs	r2, #1
	movs	r0, #18
	bl 0x02009160
	movs	r0, #20
	bl 0x02009078
	ldr	r5, [pc, #80]
	movs	r2, #242
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	movs	r2, #243
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl 0x02009190
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl 0x02009208
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	str	r2, [r3, #0]
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x01110000
	.4byte 0x00002f6e
	.4byte 0x01750000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #113
	bl 0x02009058
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #114
	bl 0x02009050
	bl 0x02009080
	movs	r0, #0
	bl 0x020091e0
	movs	r0, #199
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #808]
	movs	r3, #0
	lsls	r0, r0, #16
	bl 0x02009178
	movs	r5, #224
	ldr	r0, [pc, #800]
	bl 0x02009138
	lsls	r5, r5, #8
	movs	r1, #184
	movs	r2, #229
	adds	r3, r5, #0
	movs	r0, #4
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x020090f0
	movs	r1, #200
	movs	r2, #221
	adds	r3, r5, #0
	movs	r0, #0
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x020090f0
	movs	r1, #208
	movs	r2, #237
	adds	r3, r5, #0
	lsls	r2, r2, #16
	movs	r0, #18
	lsls	r1, r1, #16
	bl 0x020090f0
	movs	r0, #20
	movs	r1, #8
	bl 0x020090f8
	movs	r0, #19
	movs	r1, #8
	bl 0x020090f8
	movs	r0, #21
	movs	r1, #8
	bl 0x020090f8
	bl 0x020091c8
	bl 0x020091d8
	movs	r1, #2
	movs	r0, #20
	bl 0x02009118
	movs	r0, #5
	bl 0x02009078
	movs	r0, #21
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r0, #19
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r0, #20
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #19
	bl 0x02009160
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #21
	bl 0x02009160
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #0
	bl 0x02009160
	movs	r2, #5
	movs	r0, #0
	movs	r1, #0
	bl 0x02009148
	movs	r1, #3
	movs	r0, #21
	bl 0x02009100
	movs	r0, #10
	bl 0x02009078
	movs	r0, #21
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #19
	bl 0x02009160
	movs	r1, #0
	movs	r0, #19
	bl 0x02009140
	b.n	.L_02000b5c
.L_02000b36:
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x02009160
	movs	r0, #20
	movs	r1, #0
	bl 0x02009140
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	subs	r3, #1
	strh	r3, [r2, #0]
.L_02000b5c:
	movs	r0, #0
	movs	r1, #0
	bl 0x02009090
	cmp	r0, #0
	bne.n	.L_02000b36
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #20
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r0, #20
	movs	r1, #1
	bl 0x020090f8
	movs	r0, #19
	movs	r1, #1
	bl 0x020090f8
	movs	r1, #1
	movs	r0, #21
	bl 0x020090f8
	movs	r0, #30
	bl 0x02009078
	movs	r1, #3
	movs	r0, #21
	bl 0x02009100
	movs	r0, #10
	bl 0x02009078
	movs	r1, #0
	movs	r2, #5
	movs	r0, #21
	bl 0x02009148
	bl 0x02008404
	movs	r0, #60
	bl 0x02009078
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #0
	bl 0x02009160
	movs	r1, #3
	movs	r0, #19
	bl 0x02009100
	movs	r0, #10
	bl 0x02009078
	movs	r2, #5
	movs	r0, #19
	movs	r1, #0
	bl 0x02009148
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #0
	bl 0x02009158
	movs	r0, #10
	bl 0x02009078
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #20
	bl 0x02009160
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009150
	movs	r0, #20
	movs	r1, #0
	movs	r2, #5
	bl 0x02009148
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #21
	bl 0x02009160
	movs	r2, #5
	movs	r0, #21
	movs	r1, #0
	bl 0x02009148
	movs	r1, #3
	movs	r0, #19
	bl 0x02009100
	movs	r0, #10
	bl 0x02009078
	movs	r2, #5
	movs	r0, #19
	movs	r1, #0
	bl 0x02009148
	movs	r1, #3
	movs	r0, #20
	bl 0x02009100
	movs	r0, #10
	bl 0x02009078
	movs	r2, #5
	movs	r0, #20
	movs	r1, #0
	bl 0x02009148
	movs	r0, #0
	movs	r1, #3
	bl 0x020090f8
	movs	r0, #4
	movs	r1, #3
	bl 0x020090f8
	movs	r1, #3
	movs	r0, #18
	bl 0x02009100
	movs	r0, #10
	bl 0x02009078
	movs	r1, #128
	movs	r2, #128
	movs	r0, #19
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x020090a8
	movs	r1, #128
	movs	r2, #128
	movs	r0, #20
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x020090a8
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #21
	lsls	r1, r1, #9
	bl 0x020090a8
	ldr	r1, [pc, #208]
	movs	r0, #19
	bl 0x020090b0
	movs	r0, #20
	bl 0x02009078
	ldr	r5, [pc, #200]
	movs	r0, #20
	adds	r1, r5, #0
	bl 0x020090b0
	movs	r0, #20
	bl 0x02009078
	adds	r1, r5, #0
	movs	r0, #21
	bl 0x020090b0
	movs	r0, #100
	bl 0x02009078
	movs	r0, #0
	movs	r1, #4
	movs	r2, #0
	bl 0x02009128
	movs	r2, #0
	movs	r1, #4
	movs	r0, #18
	bl 0x02009120
	movs	r0, #40
	bl 0x02009078
	movs	r0, #0
	movs	r1, #3
	bl 0x020090f8
	movs	r0, #4
	movs	r1, #3
	bl 0x020090f8
	movs	r1, #3
	movs	r0, #18
	bl 0x02009100
	movs	r0, #10
	bl 0x02009078
	movs	r0, #0
	movs	r1, #2
	bl 0x020090f8
	movs	r0, #4
	bl 0x020090a0
	cmp	r0, #0
	beq.n	.L_02000d2c
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #0
	bl 0x020090b8
.L_02000d2c:
	movs	r0, #0
	bl 0x020090e0
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x020090e8
	movs	r0, #18
	movs	r1, #2
	bl 0x020090f8
	movs	r0, #4
	bl 0x020090a0
	cmp	r0, #0
	beq.n	.L_02000d5c
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #18
	bl 0x020090b8
.L_02000d5c:
	movs	r0, #18
	bl 0x020090e0
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x020090e8
	bl 0x02009088
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x01110000
	.4byte 0x00002f74
	.4byte 0x020095e0
	.2byte 0x961c
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl 0x02009080
	movs	r0, #0
	bl 0x020091e0
	movs	r5, #8
.L_02000d98:
	adds	r0, r5, #0
	bl 0x020090a0
	cmp	r0, #0
	beq.n	.L_02000daa
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_02000daa:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_02000d98
	ldr	r5, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x020090a0
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #2
	ldr	r0, [r5, #0]
	bl 0x020090f8
	movs	r2, #8
	movs	r1, #2
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x020090d0
	movs	r0, #5
	bl 0x02009078
	movs	r0, #123
	bl 0x02009210
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x02009198
	bl 0x020091d0
	bl 0x020091d8
	bl 0x02009088
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r0, #144
	adds	r3, r3, r2
	lsls	r0, r0, #4
	adds	r2, #88
	str	r2, [r3, #0]
	adds	r0, #169
	sub	sp, #8
	bl 0x02009048
	cmp	r0, #0
	beq.n	.L_02000e30
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x020090e8
.L_02000e30:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #170
	bl 0x02009048
	cmp	r0, #0
	beq.n	.L_02000e48
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x020090e8
.L_02000e48:
	movs	r0, #83
	bl 0x02009048
	cmp	r0, #0
	bne.n	.L_02000e5c
	movs	r0, #163
	bl 0x02009048
	cmp	r0, #0
	beq.n	.L_02000e66
.L_02000e5c:
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009050
.L_02000e66:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #168
	bl 0x02009048
	cmp	r0, #0
	beq.n	.L_02000eaa
	movs	r0, #15
	bl 0x020090a0
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r2, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	subs	r3, #50
	strb	r2, [r3, #0]
	movs	r3, #154
	lsls	r3, r3, #18
	str	r3, [r5, #8]
	str	r2, [r5, #12]
	movs	r1, #0
	bl 0x02009068
	movs	r3, #38
	movs	r2, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #40
	movs	r1, #19
	movs	r2, #1
	movs	r3, #1
	bl 0x02009060
.L_02000eaa:
	movs	r0, #16
	bl 0x020090a0
	movs	r1, #0
	bl 0x02009068
	movs	r0, #16
	movs	r1, #1
	bl 0x020090f8
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009048
	cmp	r0, #0
	beq.n	.L_02000f0a
	movs	r0, #16
	bl 0x020090a0
	movs	r1, #5
	adds	r5, r0, #0
	movs	r0, #16
	bl 0x020090f8
	movs	r0, #16
	bl 0x020090a0
	movs	r1, #0
	bl 0x02009130
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	subs	r2, #50
	strb	r3, [r2, #0]
	str	r3, [r5, #12]
	movs	r2, #42
	movs	r3, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #8
	movs	r1, #42
	movs	r2, #1
	movs	r3, #1
	bl 0x02009060
.L_02000f0a:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02009048
	cmp	r0, #0
	beq.n	.L_02000f26
	movs	r1, #164
	movs	r2, #198
	movs	r0, #17
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x020090e8
.L_02000f26:
	movs	r0, #19
	bl 0x020090a0
	movs	r1, #1
	bl 0x02009130
	movs	r0, #20
	bl 0x020090a0
	movs	r1, #0
	bl 0x02009130
	movs	r0, #21
	bl 0x020090a0
	movs	r1, #2
	bl 0x02009130
	movs	r0, #10
	adds	r0, #255
	bl 0x02009048
	cmp	r0, #0
	bne.n	.L_02000f6c
	ldr	r3, [pc, #140]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #20
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bls.n	.L_02000f7a
.L_02000f6c:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #113
	bl 0x02009048
	cmp	r0, #0
	beq.n	.L_02000fca
.L_02000f7a:
	bl 0x020083cc
	movs	r0, #10
	adds	r0, #255
	bl 0x02009048
	cmp	r0, #0
	bne.n	.L_02000fa0
	ldr	r3, [pc, #88]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #20
	bne.n	.L_02000fa0
	bl 0x02008a24
	b.n	.L_02000fca
.L_02000fa0:
	movs	r0, #10
	adds	r0, #255
	bl 0x02009048
	cmp	r0, #0
	bne.n	.L_02000fca
	ldr	r3, [pc, #52]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #21
	bne.n	.L_02000fca
	movs	r0, #160
	lsls	r0, r0, #4
.L_02000fc0:
	adds	r0, #113
	bl 0x02009050
	bl 0x0200887c
.L_02000fca:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x02009048
	cmp	r0, #0
	beq.n	.L_02000fdc
	bl 0x02008404
.L_02000fdc:
	movs	r0, #0
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #169
	sub	sp, #8
	bl 0x02009048
	cmp	r0, #0
	beq.n	.L_0200100e
	movs	r3, #2
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #2
	movs	r2, #1
	movs	r3, #1
	bl 0x02009060
.L_0200100e:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #170
	bl 0x02009048
	cmp	r0, #0
	beq.n	.L_02001030
	movs	r3, #14
	movs	r2, #38
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	bl 0x02009060
.L_02001030:
	movs	r0, #0
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x020091c0
	pop	{pc}
	.irp EntryTarget, 0x080000c1, 0x080003c9, 0x080003d1, 0x080003d9, 0x080201e9, 0x08020219, 0x08038141, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8079, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80c1, 0x080c80d1, 0x080c80d9, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8101, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8159, 0x080c8161, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81d1, 0x080c81d9, 0x080c8211, 0x080c8219, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8249, 0x080c8269, 0x080c8279, 0x080c8281, 0x080c8289, 0x080c8291, 0x080c8299, 0x080c82e1, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c8559, 0x080c85e9, 0x080c85f1, 0x080c8861, 0x080c8941, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
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
	.4byte 0x000000f0
	.4byte 0x10138002
	.4byte 0xffffffff
	.4byte 0x102050f0
	.4byte 0xffffffff
	.4byte 0x103060f0
	.4byte 0xffffffff
	.4byte 0x104090f0
	.4byte 0xffffffff
	.4byte 0x105020f0
	.4byte 0xffffffff
	.4byte 0x106030f0
	.4byte 0xffffffff
	.4byte 0x107080f0
	.4byte 0xffffffff
	.4byte 0x108070f0
	.4byte 0xffffffff
	.4byte 0x109040f0
	.4byte 0xffffffff
	.4byte 0x10a39002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x00000016
	.4byte 0x0000000b
	.4byte 0x00018000
	.4byte 0x0000002e
	.4byte 0x0200832d
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0xffff0133
	.4byte 0x020092a0
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0133
	.4byte 0x020092a0
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x020092b8
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0x005300f4
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00004000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00ef
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00ef
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00ef
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0000
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
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c402
	.4byte 0xffff0002
	.4byte 0x02008d85
	.4byte 0x0000c402
	.4byte 0xffff0003
	.4byte 0x02008d85
	.4byte 0x0000c402
	.4byte 0xffff0004
	.4byte 0x02008d85
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x0000c402
	.4byte 0xffff0008
	.4byte 0x02008d85
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0x02050032
	.4byte 0x02008185
	.4byte 0x00000602
	.4byte 0x02010029
	.4byte 0x02009039
	.4byte 0x00008602
	.4byte 0x0201002a
	.4byte 0x02009039
	.4byte 0x00000602
	.4byte 0x0202002b
	.4byte 0x02009039
	.4byte 0x00008602
	.4byte 0x0202002c
	.4byte 0x02009039
	.4byte 0x00000002
	.4byte 0x0a700064
	.4byte 0x02008431
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0200830d
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0200883d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0200883d
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0200883d
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0200839d
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte 0x0200804d
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x0200804d
	.4byte 0x00008f15
	.4byte 0x0200000b
	.4byte 0x02008359
	.4byte 0x00008f15
	.4byte 0x0201000c
	.4byte 0x02008365
	.4byte 0x00008f15
	.4byte 0x0202000d
	.4byte 0x02008375
	.4byte 0x00008f15
	.4byte 0x0203000e
	.4byte 0x02008385
	.4byte 0x00001815
	.4byte 0x03020010
	.4byte 0x02008149
	.4byte 0x10008c15
	.4byte 0x09a8000f
	.4byte 0x020080d5
	.4byte 0x00008c15
	.4byte 0x09a8000f
	.4byte 0x020080f1
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x020080f1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x00e80000
	.4byte 0x00e30000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x00d90000
	.4byte 0x00f30000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x00d90000
	.4byte 0x014e0000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x00e00000
	.4byte 0x00c30000
	.4byte 0x00000001
	.4byte 0x0000002b
	.4byte 0x020095e0
