.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020081d5, 0x02008039, 0x02008045, 0x0200804d, 0x020081cd, 0x02008041, 0x02008251
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xaa04
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xaa34
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xaa68
	.2byte 0x0200
	push	{lr}
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #14
	bne.n	.L_0200006c
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #12
	bne.n	.L_0200006c
	movs	r1, #2
	bl 0x0200a360
.L_0200006c:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r0, #10
	sub	sp, #32
	bl 0x0200a388
	ldr	r3, [pc, #176]
	adds	r7, r0, #0
	add	r5, sp, #8
	str	r3, [r7, #108]
	adds	r0, r5, #0
	bl 0x02008dcc
	cmp	r0, #0
	beq.n	.L_020000a0
	mov	r2, sp
	add	r3, sp, #24
	ldmia	r3!, {r0, r1}
	stmia	r2!, {r0, r1}
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	ldr	r2, [r5, #8]
	ldr	r3, [r5, #12]
	bl 0x02009050
.L_020000a0:
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #16
	bne.n	.L_02000124
	ldr	r3, [r7, #16]
	asrs	r3, r3, #20
	cmp	r3, #12
	bne.n	.L_02000124
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200a2c0
	cmp	r0, #0
	bne.n	.L_02000124
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200a2c8
	adds	r5, r7, #0
	movs	r1, #3
	movs	r0, #10
	bl 0x0200a3c8
	adds	r5, #85
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #5
	bl 0x0200a260
	movs	r0, #134
	bl 0x0200a470
	movs	r3, #2
	strb	r3, [r5, #0]
	movs	r5, #176
	movs	r6, #0
	lsls	r5, r5, #16
.L_020000ea:
	movs	r0, #128
	lsls	r0, r0, #17
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x0200a368
	movs	r0, #128
	lsls	r0, r0, #17
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x0200a458
	cmp	r6, #1
	bgt.n	.L_02000118
	movs	r0, #128
	lsls	r0, r0, #17
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #128
	bl 0x0200a460
.L_02000118:
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r6, #1
	adds	r5, r5, r3
	cmp	r6, #3
	ble.n	.L_020000ea
.L_02000124:
	movs	r3, #0
	str	r3, [r7, #108]
	add	sp, #32
	pop	{r5, r6, r7, pc}
	.4byte 0x02008055
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r0, r1, #0
	bl 0x0200a388
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	movs	r2, #0
	asrs	r3, r3, #20
	cmp	r3, #20
	bne.n	.L_0200015c
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #36
	bne.n	.L_0200015c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #4
	bl 0x0200a2c8
	movs	r2, #1
.L_0200015c:
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #32
	bne.n	.L_02000178
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #36
	bne.n	.L_02000178
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #5
	bl 0x0200a2c8
	movs	r2, #1
.L_02000178:
	cmp	r2, #0
	beq.n	.L_020001bc
	movs	r0, #10
	bl 0x0200a260
	movs	r0, #134
	bl 0x0200a470
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	movs	r6, #0
	b.n	.L_020001a0
.L_02000190:
	movs	r0, #1
	adds	r6, #1
	bl 0x0200a260
	cmp	r6, #29
	bgt.n	.L_020001aa
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
.L_020001a0:
	cmp	r2, r3
	bgt.n	.L_02000190
	ldr	r3, [r5, #40]
	cmp	r3, #0
	bne.n	.L_02000190
.L_020001aa:
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r2, #0
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	bl 0x0200a368
.L_020001bc:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #12
	bl 0x0200a3f8
	pop	{pc}
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xabb8
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #129
	adds	r3, r3, r1
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	movs	r0, #12
	movs	r1, #1
	bl 0x0200a440
	movs	r0, #13
	movs	r1, #1
	bl 0x0200a440
	movs	r0, #10
	adds	r0, #255
	bl 0x0200a2c0
	cmp	r0, #0
	bne.n	.L_02000220
	ldr	r2, [pc, #68]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #12
	bne.n	.L_02000220
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	bl 0x0200a468
.L_02000220:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200a2c0
	cmp	r0, #0
	bne.n	.L_02000248
	ldr	r2, [pc, #28]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #13
	bne.n	.L_02000248
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	bl 0x0200a468
.L_02000248:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #0
	bl 0x020083cc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #4
	bl 0x0200a2c0
	cmp	r0, #0
	beq.n	.L_02000296
	movs	r0, #14
	bl 0x0200a388
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r3, [pc, #328]
	movs	r1, #164
	movs	r2, #146
	str	r3, [r5, #20]
	str	r3, [r5, #12]
	movs	r0, #14
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200a3b0
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #0
	bl 0x0200a368
.L_02000296:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #5
	bl 0x0200a2c0
	cmp	r0, #0
	beq.n	.L_020002d4
	movs	r0, #15
	bl 0x0200a388
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r3, [pc, #264]
	movs	r1, #130
	movs	r2, #146
	str	r3, [r5, #20]
	str	r3, [r5, #12]
	movs	r0, #15
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200a3b0
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #0
	bl 0x0200a368
.L_020002d4:
	movs	r0, #8
	bl 0x0200a388
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #32
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200a388
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200a388
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x0200a388
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #3
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #8
	bl 0x0200a3c8
	movs	r0, #10
	movs	r1, #3
	bl 0x0200a3c8
	movs	r0, #9
	movs	r1, #3
	bl 0x0200a3c8
	movs	r1, #3
	movs	r0, #11
	bl 0x0200a3c8
	movs	r0, #10
	bl 0x0200a388
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200a3d0
	movs	r0, #9
	bl 0x0200a3d0
	movs	r0, #10
	bl 0x0200a3d0
	movs	r0, #11
	bl 0x0200a3d0
	movs	r0, #8
	bl 0x02008c78
	movs	r0, #9
	bl 0x02008c78
	movs	r0, #10
	bl 0x02008c78
	movs	r0, #11
	bl 0x02008c78
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200a2c0
	cmp	r0, #0
	beq.n	.L_020003ba
	movs	r5, #176
	movs	r6, #0
	lsls	r5, r5, #16
.L_02000380:
	movs	r0, #128
	lsls	r0, r0, #17
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x0200a368
	movs	r0, #128
	lsls	r0, r0, #17
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x0200a458
	cmp	r6, #1
	bgt.n	.L_020003ae
	movs	r0, #128
	lsls	r0, r0, #17
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #128
	bl 0x0200a460
.L_020003ae:
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r6, #1
	adds	r5, r5, r3
	cmp	r6, #3
	ble.n	.L_02000380
.L_020003ba:
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb500
	bl 0x0200a400
	pop	{pc}
	push	{lr}
	movs	r0, #20
	adds	r0, #255
	bl 0x0200a2c8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #37
	bl 0x0200a2c0
	cmp	r0, #0
	beq.n	.L_020003f4
	movs	r0, #98
	adds	r0, #255
	bl 0x0200a2c8
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200a2c8
.L_020003f4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x60184b01
	.4byte 0x00004770
	.2byte 0xacc4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #164]
	sub	sp, #32
	ldr	r0, [r3, #0]
	cmp	r0, #0
	bge.n	.L_02000416
	adds	r0, #3
.L_02000416:
	asrs	r0, r0, #2
	movs	r1, #5
	bl 0x0200a258
	ldr	r3, [pc, #148]
	mov	r8, r0
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0200046a
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
	beq.n	.L_0200044a
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r1
	cmp	r3, #153
	bne.n	.L_020004a6
.L_0200044a:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_020004a6
	movs	r0, #175
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_020004a6
.L_0200046a:
	movs	r5, #0
	movs	r6, #4
.L_0200046e:
	mov	r2, r8
	adds	r0, r2, r5
	movs	r1, #5
	mov	r7, sp
	bl 0x0200a258
	ldr	r3, [pc, #60]
	lsls	r0, r0, #1
	ldrh	r3, [r3, r6]
	adds	r5, #1
	strh	r3, [r7, r0]
	adds	r6, #2
	cmp	r5, #4
	ble.n	.L_0200046e
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
.L_020004a6:
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200acc0
	.4byte 0x0200acc4
	.4byte 0x0200acec
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, r6, lr}
	ldr	r2, [pc, #88]
	movs	r3, #1
	adds	r6, r0, #0
	str	r3, [r2, #0]
	cmp	r6, #2
	beq.n	0x020084e4
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
	bl 0x0200a2c0
	cmp	r0, #0
	bne.n	.L_020004f4
	cmp	r6, #1
	bne.n	.L_02000506
.L_020004f4:
	ldr	r3, [pc, #60]
	movs	r2, #0
	movs	r1, #144
	str	r2, [r3, #0]
	ldr	r0, [pc, #56]
	lsls	r1, r1, #3
	bl 0x0200a268
	b.n	.L_0200051a
.L_02000506:
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
.L_0200051a:
	pop	{r5, r6, pc}
	.4byte 0x0200acc4
	.4byte 0x05000180
	.4byte 0x0200acec
	.4byte 0x03000730
	.4byte 0x0200ad0c
	.4byte 0x050001a0
	.4byte 0x0200acc0
	.4byte 0x02008405
	.4byte 0x0200ad10
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, lr}
	bl 0x0200a388
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
	bl 0x0200a320
	adds	r3, r0, #0
	asrs	r3, r3, #19
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	adds	r3, #6
	movs	r2, #0
	bl 0x0200a368
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #128
	bl 0x0200a460
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
	bl 0x0200a388
	adds	r6, r0, #0
	ldr	r7, [r6, #104]
	bl 0x0200a378
	movs	r0, #0
	bl 0x0200a420
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
	bl 0x0200a340
	movs	r3, #99
	adds	r3, r3, r7
	mov	r8, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000638
.L_020005f2:
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #176]
	str	r3, [r6, #8]
	ldr	r3, [r7, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r7, #16]
	str	r3, [r6, #16]
	cmp	r5, r2
	bgt.n	.L_0200060e
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r5, r5, r3
.L_0200060e:
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
.L_02000622:
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	movs	r0, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	bl 0x0200a260
	mov	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_020005f2
.L_02000638:
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
	bl 0x0200a340
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	b.n	.L_020006b0
	.4byte 0x00000001
	.4byte 0x02000240
	.4byte 0x0003ffff
	.2byte 0x122c
	.2byte 0x0300
.L_020006b0:
	bl 0x0200a3e8
	bl 0x0200a3f0
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
	bl 0x0200a320
	ldr	r3, [r6, #16]
	adds	r2, r0, #0
	ldr	r1, [r6, #8]
	adds	r0, r7, #0
	bl 0x0200a310
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	movs	r5, #0
	cmp	r2, r3
	ble.n	.L_02000716
	b.n	.L_020006fc
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020006fc:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200a260
	cmp	r5, #59
	bgt.n	.L_02000716
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	cmp	r2, r3
	bgt.n	.L_020006fc
.L_02000716:
	movs	r0, #127
	bl 0x0200a470
	ldr	r3, [r6, #40]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_02000736
.L_02000724:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200a260
	cmp	r5, #59
	bgt.n	.L_02000736
	ldr	r3, [r6, #40]
	cmp	r3, #0
	bne.n	.L_02000724
.L_02000736:
	adds	r0, r7, #0
	bl 0x0200a318
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200a388
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200a3e0
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
	bl 0x0200a380
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
	bl 0x0200a388
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r7, r0, #0
	cmp	r3, #0
	bne.n	.L_0200080e
	movs	r3, #173
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200080e
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200080e
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0200081c
.L_0200080e:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a2e8
	b.n	.L_0200095a
	.2byte 0x0240
	.2byte 0x0200
.L_0200081c:
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200a2e8
	adds	r3, r6, #0
	adds	r3, #100
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #31
	ands	r3, r2
	cmp	r3, #31
	bne.n	.L_0200083c
	movs	r0, #231
	bl 0x0200a470
.L_0200083c:
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
	bl 0x0200a450
	cmp	r0, #255
	beq.n	.L_0200093e
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl 0x0200a428
	ldr	r5, [r5, #0]
	movs	r3, #136
	lsls	r3, r3, #17
	cmp	r5, r3
	bgt.n	.L_0200093e
	ldr	r2, [pc, #172]
	cmp	r5, r2
	blt.n	.L_0200093e
	movs	r3, #98
	adds	r3, r3, r6
	mov	r9, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02000904
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #12]
	subs	r5, r2, r3
	cmp	r5, #0
	bge.n	.L_0200089c
	subs	r5, r3, r2
.L_0200089c:
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	adds	r0, #8
	adds	r1, #8
	mov	r8, r2
	bl 0x02008784
	cmp	r0, #12
	bgt.n	.L_020008bc
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r5, r3
	bge.n	.L_020008bc
	movs	r2, #1
	mov	r8, r2
.L_020008bc:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_02000904
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200a2c0
	cmp	r0, #0
	bne.n	.L_02000904
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
.L_02000904:
	ldrh	r0, [r6, #6]
	bl 0x0200a280
	ldr	r1, [r6, #48]
	ldr	r5, [pc, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200a278
	ldr	r1, [r6, #48]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6933
	b.n	.L_02000938
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
.L_02000938:
	adds	r3, r3, r0
	str	r3, [r6, #16]
	b.n	.L_0200095a
.L_0200093e:
	adds	r3, r6, #0
	adds	r3, #99
	movs	r5, #0
	strb	r5, [r3, #0]
	ldr	r1, [pc, #32]
	adds	r0, r6, #0
	str	r5, [r6, #108]
	bl 0x0200a2f0
	movs	r0, #228
	bl 0x0200a470
	ldr	r3, [pc, #20]
	str	r5, [r3, #0]
.L_0200095a:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200acc8
	.2byte 0xace8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #222
	sub	sp, #68
	bl 0x0200a470
	ldrh	r0, [r5, #6]
	bl 0x0200a280
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
	bl 0x0200a278
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
	bl 0x0200a2f8
	movs	r1, #2
	adds	r7, r0, #0
	bl 0x0200a2e0
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200a340
	adds	r3, r7, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
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
	b.n	.L_02000a30
	.4byte 0x00000000
	.4byte 0x0300021c
	.4byte 0x020087b1
	.2byte 0x0000
	.2byte 0xfffa
.L_02000a30:
	.2byte 0x2300
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	str	r4, [sp, #12]
	bl 0x02009788
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
	bne.n	.L_02000ad6
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
	bl 0x0200a270
	mov	r2, sl
	lsls	r3, r0, #3
	ldr	r2, [r2, #8]
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	subs	r3, #4
	lsls	r3, r3, #16
	mov	r8, r2
	add	r8, r3
	bl 0x0200a270
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
	bl 0x0200a270
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsrs	r0, r0, #16
	movs	r2, #160
	lsls	r2, r2, #11
	lsls	r0, r0, #16
	adds	r0, r0, r2
	movs	r1, #10
	bl 0x0200a250
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
	bl 0x02009788
.L_02000ad6:
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
	bl 0x0200a388
	adds	r6, r0, #0
	movs	r0, #10
	bl 0x0200a388
	adds	r7, r0, #0
	movs	r0, #23
	bl 0x0200a388
	adds	r5, r0, #0
	ldr	r2, [r5, #80]
	movs	r1, #128
	mov	r8, r2
	movs	r2, #248
	movs	r0, #24
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200a3b0
	movs	r1, #128
	movs	r2, #248
	movs	r0, #23
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200a3b0
	movs	r1, #236
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200a3b0
	movs	r1, #138
	movs	r2, #128
	lsls	r2, r2, #17
	movs	r0, #10
	lsls	r1, r1, #18
	bl 0x0200a3b0
	movs	r0, #24
	bl 0x0200a388
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
	bl 0x0200a2c0
	cmp	r0, #0
	beq.n	.L_02000bc4
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
	bl 0x0200a388
	movs	r1, #4
	bl 0x0200a358
	movs	r0, #11
	bl 0x0200a388
	movs	r1, #4
	bl 0x0200a358
.L_02000bc4:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200a2c0
	cmp	r0, #0
	beq.n	.L_02000c0e
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
	bl 0x0200a388
	movs	r1, #4
	bl 0x0200a358
	movs	r0, #12
	bl 0x0200a388
	movs	r1, #4
	bl 0x0200a358
.L_02000c0e:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x0200a2c0
	cmp	r0, #0
	beq.n	.L_02000c4e
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200a2c0
	cmp	r0, #0
	beq.n	.L_02000c4e
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
.L_02000c4e:
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
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #8
	mov	r8, r3
	bl 0x0200a388
	ldr	r3, [r0, #80]
	ldr	r5, [pc, #232]
	ldr	r3, [r3, #40]
	movs	r1, #0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r4, r3, #16
	ldrh	r3, [r5, r1]
	lsrs	r2, r4, #16
	cmp	r2, r3
	beq.n	.L_02000cbe
.L_02000ca4:
	movs	r2, #128
	lsls	r3, r1, #16
	lsls	r2, r2, #9
	adds	r3, r3, r2
	lsrs	r2, r3, #16
	asrs	r1, r3, #16
	cmp	r2, #5
	bhi.n	.L_02000cbe
	lsls	r3, r2, #1
	ldrh	r3, [r5, r3]
	lsrs	r2, r4, #16
	cmp	r2, r3
	bne.n	.L_02000ca4
.L_02000cbe:
	lsls	r3, r1, #16
	lsrs	r2, r3, #16
	cmp	r2, #6
	bne.n	.L_02000cca
	movs	r0, #0
	b.n	.L_02000d72
.L_02000cca:
	ldr	r6, [pc, #180]
	lsls	r2, r2, #2
	ldrsb	r4, [r6, r2]
	adds	r1, r4, #0
	cmp	r4, #0
	bge.n	.L_02000cd8
	negs	r1, r4
.L_02000cd8:
	adds	r3, r2, #2
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bge.n	.L_02000ce2
	negs	r3, r3
.L_02000ce2:
	adds	r3, r1, r3
	asrs	r7, r3, #4
	adds	r3, r2, #1
	ldrsb	r1, [r6, r3]
	adds	r5, r1, #0
	cmp	r1, #0
	bge.n	.L_02000cf2
	negs	r5, r1
.L_02000cf2:
	adds	r3, r2, #3
	ldrsb	r2, [r6, r3]
	cmp	r2, #0
	bge.n	.L_02000cfc
	negs	r2, r2
.L_02000cfc:
	adds	r5, r5, r2
	mov	sl, r5
	ldr	r6, [r0, #8]
	mov	r3, sl
	ldr	r5, [r0, #16]
	asrs	r3, r3, #4
	mov	sl, r3
	lsls	r3, r4, #16
	adds	r6, r6, r3
	lsls	r3, r1, #16
	adds	r5, r5, r3
	movs	r3, #164
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	asrs	r6, r6, #20
	asrs	r1, r3, #20
	movs	r3, #166
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	lsls	r2, r1, #16
	asrs	r3, r3, #20
	lsls	r3, r3, #16
	asrs	r5, r5, #20
	lsrs	r2, r2, #16
	lsrs	r3, r3, #16
	adds	r2, r6, r2
	adds	r3, r5, r3
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	mov	r3, sl
	bl 0x0200a330
	movs	r3, #255
	mov	r2, sl
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r8, r3
	movs	r0, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	adds	r3, r7, #0
	bl 0x02008d84
	mov	r2, sl
	mov	r3, r8
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #2
	adds	r1, r6, #0
	adds	r2, r5, #0
	adds	r3, r7, #0
	bl 0x02008d84
	movs	r0, #1
.L_02000d72:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0200a478
	.2byte 0xa484
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r3, #0
	ldr	r3, [sp, #12]
	lsls	r2, r2, #7
	mov	ip, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r0, r0, #1
	lsls	r3, r3, #3
	adds	r3, r3, r0
	ldr	r0, [r4, r3]
	adds	r1, r1, r2
	lsls	r1, r1, #2
	adds	r0, r0, r1
	movs	r1, #0
	ldr	r6, [sp, #16]
	cmp	r1, ip
	bcs.n	.L_02000dca
.L_02000db0:
	lsls	r3, r1, #9
	movs	r2, #0
	adds	r3, r0, r3
	cmp	r2, r5
	bcs.n	.L_02000dc4
.L_02000dba:
	adds	r2, #1
	strb	r6, [r3, #2]
	adds	r3, #4
	cmp	r2, r5
	bcc.n	.L_02000dba
.L_02000dc4:
	adds	r1, #1
	cmp	r1, ip
	bcc.n	.L_02000db0
.L_02000dca:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	adds	r5, r6, #0
	sub	sp, #40
	adds	r1, r6, #0
	adds	r5, #12
	add	r0, sp, #24
	adds	r1, #16
	adds	r2, r5, #0
	bl 0x02008f34
	adds	r4, r0, #0
	cmp	r4, #0
	bne.n	.L_02000df6
	b.n	.L_02000f16
.L_02000df6:
	ldr	r5, [r5, #0]
	ldr	r0, [pc, #300]
	str	r5, [sp, #20]
	lsls	r1, r5, #2
	ldrsb	r2, [r0, r1]
	cmp	r2, #0
	bge.n	.L_02000e06
	negs	r2, r2
.L_02000e06:
	adds	r3, r1, #2
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	bge.n	.L_02000e10
	negs	r3, r3
.L_02000e10:
	adds	r3, r2, r3
	asrs	r3, r3, #4
	str	r3, [sp, #16]
	adds	r3, r1, #1
	ldrsb	r2, [r0, r3]
	cmp	r2, #0
	bge.n	.L_02000e20
	negs	r2, r2
.L_02000e20:
	adds	r3, r1, #3
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	bge.n	.L_02000e2a
	negs	r3, r3
.L_02000e2a:
	adds	r3, r2, r3
	asrs	r3, r3, #4
	str	r3, [sp, #12]
	ldr	r3, [sp, #24]
	ldr	r2, [pc, #248]
	ldr	r1, [pc, #248]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	mov	r9, r1
	mov	r2, r9
	ands	r2, r3
	lsls	r3, r3, #16
	mov	sl, r3
	movs	r3, #0
	str	r3, [r6, #20]
	mov	fp, r3
	adds	r3, r4, #0
	adds	r3, #34
	str	r3, [sp, #8]
	ldr	r1, [sp, #8]
.L_02000e52:
	movs	r3, #2
	strb	r3, [r1, #0]
	mov	r9, r2
	ldr	r3, [r4, #8]
	add	r3, r9
	str	r3, [r6, #0]
	ldr	r3, [r4, #16]
	add	r3, sl
	str	r3, [r6, #8]
	ldr	r3, [r4, #12]
	str	r3, [sp, #32]
.L_02000e68:
	ldr	r3, [sp, #20]
	ldr	r2, [pc, #188]
	lsls	r3, r3, #2
	str	r3, [sp, #4]
	adds	r3, #1
	ldrsb	r2, [r2, r3]
	ldr	r3, [r6, #8]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	movs	r1, #0
	mov	r8, r1
	str	r3, [sp, #36]
	cmp	r8, r2
	bge.n	.L_02000ed6
.L_02000e86:
	ldr	r3, [pc, #160]
	ldr	r1, [sp, #4]
	add	r5, sp, #28
	ldrsb	r2, [r3, r1]
	ldr	r3, [r6, #0]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [sp, #16]
	movs	r7, #0
	cmp	r7, r2
	bge.n	.L_02000ec0
.L_02000e9e:
	adds	r0, r4, #0
	add	r1, sp, #28
	str	r4, [sp, #0]
	bl 0x0200a338
	ldr	r4, [sp, #0]
	cmp	r0, #2
	beq.n	.L_02000ee8
	ldr	r3, [r5, #0]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r5, #0]
	ldr	r2, [sp, #16]
	adds	r7, #1
	cmp	r7, r2
	blt.n	.L_02000e9e
.L_02000ec0:
	add	r2, sp, #28
	ldr	r3, [r2, #8]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r2, #8]
	ldr	r3, [sp, #12]
	movs	r2, #1
	add	r8, r2
	cmp	r8, r3
	blt.n	.L_02000e86
.L_02000ed6:
	ldr	r3, [r6, #0]
	movs	r1, #1
	add	r3, r9
	str	r3, [r6, #0]
	ldr	r3, [r6, #8]
	add	fp, r1
	add	r3, sl
	str	r3, [r6, #8]
	b.n	.L_02000e68
.L_02000ee8:
	ldr	r2, [sp, #8]
	movs	r3, #0
	strb	r3, [r2, #0]
	mov	r3, fp
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_02000f18
	mov	r1, r9
	ldr	r3, [r4, #8]
	mov	r2, fp
	muls	r2, r1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	movs	r0, #1
	ldr	r3, [r4, #12]
	str	r3, [r6, #4]
	mov	r3, sl
	mov	r2, fp
	muls	r2, r3
	ldr	r3, [r4, #16]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	b.n	.L_02000f18
.L_02000f16:
	movs	r0, #0
.L_02000f18:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200a484
	.4byte 0x0200a49c
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	ldr	r3, [pc, #236]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl 0x0200a388
	adds	r7, r0, #0
	ldrh	r3, [r7, #6]
	ldr	r1, [sp, #8]
	lsrs	r3, r3, #12
	str	r3, [r1, #0]
	movs	r2, #8
	adds	r5, #52
	mov	fp, r2
	mov	lr, r5
.L_02000f70:
	mov	r3, lr
	ldr	r6, [r3, #0]
	movs	r5, #0
.L_02000f76:
	ldr	r3, [r6, #80]
	ldr	r2, [pc, #200]
	ldr	r3, [r3, #40]
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	cmp	r1, r3
	bne.n	.L_0200101a
	ldr	r0, [sp, #8]
	movs	r2, #10
	ldrsh	r1, [r7, r2]
	ldr	r3, [r0, #0]
	ldr	r2, [pc, #180]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	ldr	r4, [pc, #180]
	asrs	r2, r3, #16
	adds	r1, r1, r2
	asrs	r1, r1, #4
	mov	r9, r1
	movs	r1, #18
	ldrsh	r2, [r7, r1]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	adds	r2, r2, r3
	asrs	r2, r2, #4
	mov	r8, r2
	movs	r2, #10
	ldrsh	r0, [r6, r2]
	lsls	r2, r5, #2
	ldrsb	r3, [r4, r2]
	adds	r3, r0, r3
	asrs	r3, r3, #4
	mov	sl, r3
	movs	r3, #18
	ldrsh	r1, [r6, r3]
	adds	r3, r2, #1
	ldrsb	r3, [r4, r3]
	adds	r3, r1, r3
	asrs	r3, r3, #4
	mov	ip, r3
	adds	r3, r2, #2
	ldrsb	r3, [r4, r3]
	adds	r2, #3
	adds	r0, r0, r3
.L_02000fd2:
	ldrsb	r3, [r4, r2]
	asrs	r0, r0, #4
	adds	r1, r1, r3
.L_02000fd8:
	asrs	r1, r1, #4
	cmp	sl, r9
	bgt.n	.L_0200101a
.L_02000fde:
	cmp	r9, r0
	bge.n	.L_0200101a
	cmp	ip, r8
.L_02000fe4:
	bgt.n	.L_0200101a
	cmp	r8, r1
	bge.n	.L_0200101a
	ldr	r0, [sp, #0]
	movs	r3, #1
	ands	r3, r5
	str	r5, [r0, #0]
	cmp	r3, #0
	beq.n	.L_02001008
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	sl, r3
	beq.n	.L_0200101a
	ldr	r2, [sp, #4]
	mov	r1, fp
	str	r1, [r2, #0]
	adds	r0, r6, #0
	b.n	.L_02001030
.L_02001008:
	ldr	r3, [r7, #16]
	asrs	r3, r3, #20
	cmp	ip, r3
	beq.n	.L_0200101a
	ldr	r0, [sp, #4]
	mov	r3, fp
	str	r3, [r0, #0]
	adds	r0, r6, #0
	b.n	.L_02001030
.L_0200101a:
	adds	r5, #1
	cmp	r5, #5
	bls.n	.L_02000f76
	movs	r2, #1
	add	fp, r2
	movs	r1, #4
	mov	r3, fp
	add	lr, r1
	cmp	r3, #63
	bls.n	.L_02000f70
.L_0200102e:
	movs	r0, #0
.L_02001030:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200a478
	.4byte 0x0200a49c
	.4byte 0x0200a484
	.2byte 0xb084
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r0, [sp, #88]
	str	r1, [sp, #92]
	str	r2, [sp, #96]
	str	r3, [sp, #100]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r0, #133
	str	r3, [sp, #28]
	ldr	r3, [pc, #632]
	lsls	r0, r0, #2
	adds	r0, r0, r3
	mov	sl, r0
	ldr	r0, [r0, #0]
	bl 0x0200a388
	mov	r8, r0
	ldr	r0, [sp, #104]
	bl 0x0200a388
	mov	r3, r8
	ldr	r3, [r3, #48]
	mov	r4, r8
	str	r3, [sp, #20]
	adds	r6, r0, #0
	ldr	r4, [r4, #52]
	mov	r0, sp
	adds	r0, #32
	str	r0, [sp, #12]
	str	r4, [sp, #16]
	ldr	r2, [sp, #100]
	ldr	r3, [r6, #8]
	movs	r1, #0
	str	r3, [r0, #0]
	mov	r9, r1
	ldr	r3, [r6, #16]
	mov	r1, sp
	adds	r1, #44
	str	r3, [r0, #8]
	ldr	r5, [pc, #576]
	str	r1, [sp, #8]
	lsls	r7, r2, #2
	ldrsb	r1, [r5, r7]
	ldr	r3, [r6, #8]
	lsls	r2, r1, #16
	adds	r3, r3, r2
	ldr	r2, [sp, #8]
	asrs	r3, r3, #20
	str	r3, [r2, #0]
	mov	lr, r3
	adds	r3, r7, #1
	ldrsb	r4, [r5, r3]
	ldr	r3, [r6, #16]
	ldr	r0, [sp, #8]
	lsls	r2, r4, #16
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [r0, #8]
	adds	r0, r1, #0
	mov	ip, r3
	cmp	r0, #0
	bge.n	.L_020010e0
	negs	r0, r0
.L_020010e0:
	adds	r3, r7, #2
	ldrsb	r1, [r5, r3]
	cmp	r1, #0
	bge.n	.L_020010ea
	negs	r1, r1
.L_020010ea:
	adds	r3, r0, r1
	asrs	r3, r3, #4
.L_020010ee:
	adds	r1, r4, #0
	str	r3, [sp, #24]
	cmp	r1, #0
	bge.n	.L_020010f8
	negs	r1, r1
.L_020010f8:
	adds	r3, r7, #3
	ldrsb	r2, [r5, r3]
	cmp	r2, #0
	bge.n	.L_02001102
	negs	r2, r2
.L_02001102:
	adds	r3, r1, r2
	asrs	r3, r3, #4
	str	r3, [sp, #0]
	mov	fp, r3
	movs	r3, #0
	str	r3, [sp, #4]
	mov	r1, lr
	mov	r2, ip
	ldr	r3, [sp, #24]
	movs	r0, #0
	bl 0x02008d84
	mov	r1, sl
	movs	r2, #200
	ldr	r0, [r1, #0]
	lsls	r2, r2, #5
	movs	r1, #128
	lsls	r1, r1, #8
	adds	r2, #153
	bl 0x0200a390
	mov	r2, sl
	ldr	r0, [r2, #0]
	movs	r1, #8
	bl 0x0200a3b8
	movs	r0, #15
	bl 0x0200a260
	ldr	r4, [sp, #12]
	ldr	r1, [sp, #88]
	ldr	r3, [r4, #0]
	ldr	r2, [sp, #96]
	subs	r1, r1, r3
	ldr	r3, [r4, #8]
	asrs	r1, r1, #17
	subs	r2, r2, r3
	mov	r3, sl
	asrs	r2, r2, #17
	ldr	r0, [r3, #0]
	bl 0x0200a3a0
	mov	r4, sl
	ldr	r0, [r4, #0]
	bl 0x0200a388
	ldr	r3, [pc, #408]
	str	r3, [r0, #108]
	movs	r0, #4
	bl 0x0200a260
	movs	r1, #2
	adds	r0, r6, #0
	bl 0x0200a2e0
	movs	r0, #239
	bl 0x0200a470
	movs	r2, #200
	movs	r1, #128
	lsls	r2, r2, #5
	ldr	r0, [sp, #104]
	lsls	r1, r1, #8
	adds	r2, #153
	bl 0x0200a390
	adds	r0, r6, #0
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #92]
	ldr	r3, [sp, #96]
	bl 0x0200a310
	ldr	r3, [pc, #348]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r5, r3, r0
	ldr	r0, [r5, #0]
	bl 0x0200a3a8
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200a3b8
	movs	r1, #152
	movs	r2, #200
	lsls	r1, r1, #7
	lsls	r2, r2, #5
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #153
	bl 0x0200a390
	ldr	r2, [pc, #320]
	mov	r1, r9
	lsls	r3, r1, #2
	ldr	r2, [r2, r3]
	ldr	r0, [r5, #0]
	lsls	r2, r2, #16
	asrs	r1, r2, #31
	asrs	r2, r2, #17
	bl 0x0200a3a0
	ldr	r3, [sp, #108]
	cmp	r3, #0
	beq.n	0x020091d8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6828
	bl 0x0200a3a8
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200a3b8
	mov	r3, r8
	movs	r2, #0
	str	r2, [r3, #108]
	ldr	r4, [sp, #20]
	movs	r5, #255
	str	r4, [r3, #48]
	ldr	r0, [sp, #16]
	str	r0, [r3, #52]
	adds	r0, r6, #0
	bl 0x0200a318
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x0200a470
	movs	r0, #213
	bl 0x0200a470
	ldr	r2, [r6, #12]
	ldr	r1, [sp, #88]
	ldr	r3, [sp, #96]
	adds	r0, r6, #0
	bl 0x0200a300
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200a2e0
	ldr	r1, [pc, #212]
	ldr	r0, [sp, #88]
	ldrsb	r3, [r1, r7]
	adds	r2, r7, #1
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldrsb	r3, [r1, r2]
	mov	sl, r1
	ldr	r1, [sp, #96]
	lsls	r3, r3, #16
	adds	r1, r1, r3
	ldr	r4, [sp, #28]
	asrs	r0, r0, #20
	asrs	r1, r1, #20
	str	r0, [sp, #88]
	str	r1, [sp, #96]
	mov	r9, r2
	movs	r2, #164
	lsls	r2, r2, #1
	adds	r3, r4, r2
	ldr	r3, [r3, #0]
	adds	r2, #4
	asrs	r3, r3, #20
	mov	r8, r3
	adds	r3, r4, r2
	ldr	r6, [r3, #0]
	mov	r4, r8
	asrs	r6, r6, #20
	adds	r3, r4, r0
	adds	r2, r6, r1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	mov	r3, fp
	ldr	r2, [sp, #24]
	bl 0x0200a330
	mov	r0, fp
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #96]
	str	r0, [sp, #0]
	ldr	r3, [sp, #24]
	movs	r0, #0
	str	r5, [sp, #4]
	bl 0x02008d84
	mov	r3, fp
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #96]
	str	r3, [sp, #0]
	movs	r0, #2
	ldr	r3, [sp, #24]
	str	r5, [sp, #4]
	bl 0x02008d84
	ldr	r0, [sp, #12]
	mov	r4, sl
	ldrsb	r3, [r4, r7]
	ldr	r1, [r0, #0]
	ldr	r2, [sp, #8]
	lsls	r3, r3, #16
	adds	r1, r1, r3
	asrs	r1, r1, #20
	str	r1, [r2, #0]
	mov	r3, r9
	ldrsb	r2, [r4, r3]
	ldr	r3, [r0, #8]
	ldr	r4, [sp, #8]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [r4, #8]
	add	r8, r1
	adds	r6, r6, r3
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	ldr	r2, [sp, #24]
	mov	r0, r8
	adds	r1, r6, #0
	mov	r3, fp
	bl 0x0200a330
	ldr	r0, [sp, #8]
	mov	r3, fp
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #8]
	movs	r4, #0
	str	r3, [sp, #0]
	movs	r0, #2
	ldr	r3, [sp, #24]
	str	r4, [sp, #4]
	bl 0x02008d84
	bl 0x0200a430
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r3}
	add	sp, #16
	bx	r3
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200a484
	.4byte 0x02009301
	.2byte 0xa49c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldrh	r3, [r5, #6]
	movs	r2, #12
	lsrs	r1, r3, #12
	adds	r3, r1, #2
	ands	r3, r2
	lsls	r1, r3, #12
	ldr	r3, [r5, #8]
	sub	sp, #12
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	movs	r0, #128
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #13
	adds	r2, r6, #0
	str	r3, [r6, #8]
	bl 0x0200a288
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200a438
	cmp	r0, #0
	beq.n	.L_0200135c
	movs	r4, #0
.L_02001338:
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r2, [pc, #64]
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	cmp	r1, r3
	beq.n	.L_02001380
	adds	r4, #1
	cmp	r4, #5
	bls.n	.L_02001338
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200a300
.L_0200135c:
	ldr	r3, [r5, #8]
	adds	r0, r5, #0
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	adds	r1, r6, #0
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl 0x0200a338
	cmp	r0, #0
	ble.n	.L_02001380
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200a300
.L_02001380:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0xa478
	.2byte 0x0200
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
	blt.n	.L_020013da
	b.n	.L_0200150e
.L_020013da:
	ldr	r2, [pc, #340]
	mov	r0, r9
	lsls	r3, r0, #2
	ldr	r5, [r2, r3]
	cmp	r5, #0
	bne.n	.L_020013e8
	b.n	.L_020014fe
.L_020013e8:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bne.n	.L_020013f0
	b.n	.L_020014fe
.L_020013f0:
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
	bne.n	.L_02001466
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
	bhi.n	.L_020014fe
	movs	r2, #16
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_020014fe
	cmp	r4, #239
	bgt.n	.L_020014fe
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
	b.n	.L_020014a2
.L_02001466:
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
	bhi.n	.L_020014fe
	movs	r2, #64
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_020014fe
	cmp	r4, #175
	bgt.n	.L_020014fe
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
.L_020014a2:
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
	bne.n	.L_020014e0
	adds	r0, r5, #0
	bl 0x0200a448
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
	b.n	.L_020014f4
.L_020014e0:
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
.L_020014f4:
	adds	r0, r6, #0
	mov	r1, fp
	bl 0x0200a2b8
	adds	r6, #12
.L_020014fe:
	ldr	r3, [pc, #44]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	add	r9, r1
	cmp	r9, r3
	bge.n	.L_0200150e
	b.n	.L_020013da
.L_0200150e:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffff0000
	.4byte 0x0200ad2c
	.4byte 0x020036e0
	.4byte 0x0200ad70
	.4byte 0x0200ad2e
	.4byte 0x0200ad30
	.4byte 0x0200ae30
	.4byte 0x40002000
	.4byte 0xc000a000
	.2byte 0xae32
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200a290
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200a2a0
	ldr	r5, [pc, #76]
	bl 0x0200a2b0
	movs	r1, #192
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200a2a8
	adds	r0, r6, #0
	bl 0x0200a298
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200a268
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
	.4byte 0x0200ad30
	.4byte 0x0200a4dc
	.4byte 0x0200ad2c
	.4byte 0x02009389
	.4byte 0x0200ad2e
	.4byte 0x0200ae30
	.2byte 0xae32
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200a290
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200a2a0
	ldr	r5, [pc, #76]
	bl 0x0200a2b0
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200a2a8
	adds	r0, r6, #0
	bl 0x0200a298
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200a268
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
	.4byte 0x0200ad30
	.4byte 0x0200a63f
	.4byte 0x0200ad2c
	.4byte 0x02009389
	.4byte 0x0200ad2e
	.4byte 0x0200ae30
	.2byte 0xae32
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	bl 0x0200a290
	ldr	r3, [pc, #88]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #84]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #80]
	bl 0x0200a2a0
	ldr	r5, [pc, #80]
	bl 0x0200a2b0
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200a2a8
	adds	r0, r6, #0
	bl 0x0200a298
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	bl 0x0200a268
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	b.n	.L_020016c8
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200ad30
	.4byte 0x0200a86e
	.4byte 0x0200ad2c
	.4byte 0x02009389
	.4byte 0x0200ad2e
	.4byte 0x0200ae30
	.2byte 0xae32
	.2byte 0x0200
.L_020016c8:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	bl 0x0200a388
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L_020016f2
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
.L_020016f2:
	pop	{r5, pc}
	.4byte 0x0200ad2e
	.4byte 0x0200ad30
	.4byte 0x80184b01
	.4byte 0x00004770
	.2byte 0xae32
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_0200174c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200174c
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
.L_0200174c:
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
	bl 0x0200a388
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_020017d0
	cmp	r7, #0
	beq.n	.L_020017d0
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_020017d8
.L_020017d0:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_020017d8:
	mov	r3, sl
	bl 0x0200a2f8
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020017e6
	b.n	.L_02001932
.L_020017e6:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200a2e0
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200a2f0
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a340
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
	bl 0x02009708
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
	beq.n	.L_02001932
	cmp	r7, #0
	beq.n	.L_02001932
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02001868
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200a3c0
.L_02001868:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001888
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x02009708
.L_02001888:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0200189c
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_0200189c:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020018e2
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020018ca
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200a250
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_020018dc
.L_020018ca:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200a250
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020018dc:
	bl 0x0200a250
	str	r0, [r6, #52]
.L_020018e2:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020018fe
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200a2e0
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200a2f0
.L_020018fe:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001910
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02001910:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001922
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02001922:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001932
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02001932:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200acdc
	.4byte 0x02009751
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
	beq.n	.L_02001a5c
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
	bl 0x0200a388
	mov	r1, r8
	ldr	r3, [r0, #8]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	ldr	r4, [sp, #0]
	cmp	r3, r2
	bne.n	.L_0200199c
	ldr	r3, [r0, #16]
	movs	r5, #2
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_020019a4
.L_0200199c:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r4, #12]
.L_020019a4:
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	movs	r2, #1
	negs	r2, r2
	ldr	r1, [pc, #192]
	cmp	r3, r2
	beq.n	.L_02001a5c
	movs	r5, #14
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L_02001a5c
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
	bl 0x0200a320
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
	bhi.n	.L_02001a5c
	movs	r0, #15
	negs	r0, r0
	cmp	r2, r0
	blt.n	.L_02001a5c
	cmp	r2, #239
	bgt.n	.L_02001a5c
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
	bl 0x0200a2b8
.L_02001a5c:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200ae34
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
.L_02001ade:
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
.L_02001af2:
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
	bge.n	.L_02001c00
.L_02001b30:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #36]
	ldr	r5, [sp, #24]
	lsls	r3, r1, #9
	adds	r2, r2, r3
	movs	r3, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, r5
	bge.n	.L_02001bf4
.L_02001b44:
	mov	r0, fp
	ldrb	r5, [r0, #2]
	cmp	r5, #0
	beq.n	.L_02001be4
	ldr	r1, [sp, #44]
	cmp	r5, r1
	bcc.n	.L_02001be4
	adds	r1, #1
	mov	sl, r1
	cmp	r5, sl
	bhi.n	.L_02001be4
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
	bl 0x0200a2c0
	cmp	r0, #0
	bne.n	.L_02001b98
	cmp	r5, sl
	bne.n	.L_02001bd6
	mov	r3, r9
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	bl 0x0200a2c8
	b.n	.L_02001bd6
.L_02001b98:
	mov	r1, r9
	movs	r5, #4
	ldrsh	r0, [r1, r5]
	bl 0x0200a2c0
	cmp	r0, #0
	beq.n	.L_02001bd6
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
	bl 0x0200a328
.L_02001bd6:
	mov	r0, r8
	ldrh	r3, [r0, #10]
	mov	r1, r8
	adds	r3, #1
	strh	r3, [r1, #10]
	movs	r5, #8
	add	r9, r5
.L_02001be4:
	ldr	r2, [sp, #16]
	ldr	r5, [sp, #24]
	adds	r2, #1
	movs	r3, #4
	str	r2, [sp, #16]
	add	fp, r3
	cmp	r2, r5
	blt.n	.L_02001b44
.L_02001bf4:
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #12]
	cmp	r0, r1
	blt.n	.L_02001b30
.L_02001c00:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200a2c0
	cmp	r0, #0
	beq.n	.L_02001c58
	ldr	r3, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200a388
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
	bge.n	.L_02001c58
.L_02001c32:
	mov	r0, r9
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	cmp	r3, r4
	bne.n	.L_02001c48
	movs	r5, #2
	ldrsh	r3, [r0, r5]
	cmp	r3, r1
	bne.n	.L_02001c48
	mov	r0, r8
	strh	r2, [r0, #12]
.L_02001c48:
	movs	r3, #8
	mov	r0, r8
	add	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	adds	r2, #1
	cmp	r2, r3
	blt.n	.L_02001c32
.L_02001c58:
	movs	r0, #128
	lsls	r0, r0, #1
	bl 0x0200a290
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r2, #63
.L_02001c66:
	ldr	r3, [pc, #80]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bge.n	.L_02001c66
	bl 0x0200a2b0
	mov	r1, r8
	strh	r0, [r1, #16]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #1
	asrs	r0, r0, #16
	bl 0x0200a2a8
	adds	r0, r5, #0
	bl 0x0200a298
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200a268
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
	.4byte 0x0200ae34
	.4byte 0x03000258
	.4byte 0x02000240
	.4byte 0x11111111
	.2byte 0x9951
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
	bl 0x0200a388
	mov	r1, r8
	ldr	r5, [r0, #8]
	ldr	r6, [r0, #16]
	mov	sl, r0
	movs	r2, #128
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200a390
	asrs	r5, r5, #20
	mov	r2, r8
	asrs	r6, r6, #20
	ldr	r0, [r2, #0]
	lsls	r1, r5, #4
	lsls	r2, r6, #4
	adds	r1, #8
	adds	r2, #8
	bl 0x0200a398
	movs	r0, #1
	bl 0x0200a260
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
.L_02001d20:
	bl 0x0200a300
	movs	r0, #4
	bl 0x0200a370
	bl 0x0200a3f0
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
	bl 0x0200a388
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
	bl 0x02009cc0
	movs	r0, #161
	bl 0x0200a470
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
	bl 0x0200a328
	movs	r0, #12
	bl 0x0200a370
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
	bl 0x0200a388
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
	bl 0x02009cc0
	movs	r0, #229
	bl 0x0200a470
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
	bl 0x0200a328
	movs	r0, #12
	bl 0x0200a370
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	bl 0x0200a388
	movs	r1, #0
	bl 0x0200a340
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
	b.n	.L_02001e6c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00008000
	.2byte 0x0240
	.2byte 0x0200
.L_02001e6c:
	movs	r3, #1
	mov	r1, r8
	strh	r3, [r1, #14]
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl 0x0200a3b8
	movs	r0, #16
	bl 0x0200a370
.L_02001e80:
	cmp	r7, #5
	bne.n	.L_02001e8a
	movs	r0, #204
	bl 0x0200a470
.L_02001e8a:
	ldr	r3, [r6, #24]
	ldr	r1, [pc, #88]
	ldr	r2, [pc, #92]
	adds	r3, r3, r1
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	str	r3, [r6, #28]
.L_02001e9c:
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	adds	r7, #1
	bl 0x0200a260
	cmp	r7, #39
	ble.n	.L_02001e80
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200a388
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
	bl 0x0200a410
	bl 0x0200a418
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
	bl 0x0200a250
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02001f28
	adds	r3, #15
.L_02001f28:
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
	bl 0x0200a388
	adds	r7, r0, #0
	bl 0x0200a378
	movs	r0, #0
	bl 0x0200a420
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200a3e8
	bl 0x0200a308
	movs	r0, #1
	bl 0x0200a260
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
	bl 0x0200a408
	bl 0x0200a418
	movs	r0, #204
	bl 0x0200a470
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200a370
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
.L_02001fea:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200a280
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
.L_02001ffa:
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200a278
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
.L_0200200c:
	bl 0x0200a270
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #188]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200a270
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
	bl 0x02009788
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
.L_02002058:
	cmp	r3, #16
	bls.n	.L_02001fea
	movs	r0, #188
	bl 0x0200a470
	ldr	r5, [pc, #112]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200a3d8
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200a3b8
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200a348
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200a348
	bl 0x0200a350
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200a3d8
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200a370
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200a3b8
	bl 0x0200a380
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x02009ef9
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
	bl 0x0200a388
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
	bge.n	.L_02002184
.L_0200211c:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, r8
	bne.n	.L_02002178
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	cmp	r3, sl
	bne.n	.L_02002178
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	bl 0x0200a2c0
	cmp	r0, #0
	bne.n	.L_0200214c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x02009d48
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	bl 0x0200a2c8
	strh	r7, [r6, #12]
	b.n	.L_02002184
.L_0200214c:
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r7, r3
	beq.n	.L_02002184
	adds	r0, r6, #0
	adds	r1, r5, #0
	strh	r7, [r6, #12]
	bl 0x02009db8
	movs	r2, #2
	ldrsh	r0, [r6, r2]
	mov	r1, r8
	bl 0x0200a2d8
	movs	r3, #2
	ldrsh	r0, [r6, r3]
	mov	r1, sl
	adds	r0, #8
	bl 0x0200a2d8
	movs	r0, #1
	b.n	.L_02002186
.L_02002178:
	lsls	r3, r2, #16
	adds	r7, #1
	asrs	r3, r3, #16
	adds	r5, #8
	cmp	r7, r3
	blt.n	.L_0200211c
.L_02002184:
	movs	r0, #0
.L_02002186:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xae34
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
	bl 0x0200a388
	movs	r3, #192
	ldr	r5, [pc, #140]
	lsls	r3, r3, #18
.L_020021c2:
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	mov	sl, r3
	bl 0x0200a2d0
	adds	r7, r0, #0
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	adds	r0, #8
	bl 0x0200a2d0
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_020021e6
	cmp	r0, #0
	beq.n	.L_0200223a
.L_020021e6:
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #0
	bl 0x0200a2d8
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	movs	r1, #0
	adds	r0, #8
	bl 0x0200a2d8
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
	bl 0x0200a308
	bl 0x02009f50
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	str	r2, [r3, #0]
.L_0200223a:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200ae34
	.irp EntryTarget, 0x03000528, 0x03000508, 0x080000c1, 0x080000d1, 0x080000f9, 0x08000119, 0x08000121, 0x08000129, 0x08000169, 0x08000179, 0x080001a9, 0x080001c9, 0x080001d1, 0x080001e9, 0x080003c9, 0x080003d1, 0x080003e9, 0x080003f1, 0x08020091, 0x08020099, 0x080200a9, 0x080200c1, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x080201c1, 0x080201e1, 0x080201e9, 0x08020211, 0x08020219, 0x08020229, 0x08020231, 0x08020279, 0x08020291, 0x08020361, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8099, 0x080c80c9, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8171, 0x080c8201, 0x080c8209, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8259, 0x080c8279, 0x080c82e1, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c85c1, 0x080c8691, 0x080c8739, 0x080c8781, 0x080c87e9, 0x080c8831, 0x080c8841, 0x080c8849, 0x080c8881, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x01030102
	.4byte 0x01260125
	.4byte 0x014b014c
	.4byte 0x0820f8e0
	.4byte 0x2008e0f8
	.4byte 0x0020f0e0
	.4byte 0x2008e0f8
	.4byte 0x0820f8e0
	.4byte 0x2008e0f8
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
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
	.4byte 0x00000100
	.4byte 0x00101107
	.4byte 0x00202101
	.4byte 0x00303107
	.4byte 0x00404107
	.4byte 0x006060ff
	.4byte 0x007070ff
	.4byte 0x008080ff
	.4byte 0x009090ff
	.4byte 0x00a0a0ff
	.4byte 0x00b0b0ff
	.4byte 0x00c0c101
	.4byte 0x000001ff
	.4byte 0xffff014c
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff014c
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff014b
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00024000
	.4byte 0xffff014c
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00024000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00026000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x01022000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x01026000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x01022000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x01022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
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
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
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
	.4byte 0x00000202
	.4byte 0xffff001e
	.4byte 0x02008071
	.4byte 0x00008602
	.4byte 0xffff001f
	.4byte 0x020083c5
	.4byte 0x00004e15
	.4byte 0xffff000c
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x10008c15
	.4byte 0xffff000e
	.4byte 0x02008131
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x02008135
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte 0x02008131
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x02008135
	.4byte 0x50008905
	.4byte 0xffff0028
	.4byte 0x020081c1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x0200a950
	.4byte 0x0200a98c
	.4byte 0x0200a9c8
