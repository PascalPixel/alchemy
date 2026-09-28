.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020086ed, 0x02008039, 0x02008079, 0x02008081, 0x02008435, 0x02008041, 0x02008a69
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8f2c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000058
	ldr	r0, [pc, #20]
	b.n	.L_02000062
.L_02000058:
	ldr	r3, [pc, #20]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_02000062
	ldr	r0, [pc, #16]
.L_02000062:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000ae
	.4byte 0x02008f5c
	.4byte 0x000000af
	.2byte 0x8f8c
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8fbc
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #44]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02000098
	ldr	r0, [pc, #32]
	b.n	.L_020000ae
.L_02000098:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_020000a2
	ldr	r0, [pc, #32]
	b.n	.L_020000ae
.L_020000a2:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_020000ac
	ldr	r0, [pc, #28]
	b.n	.L_020000ae
.L_020000ac:
	ldr	r0, [pc, #28]
.L_020000ae:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000ae
	.4byte 0x02009044
	.4byte 0x000000af
	.4byte 0x02009194
	.4byte 0x000000b0
	.4byte 0x020092cc
	.2byte 0x902c
	.2byte 0x0200
	push	{lr}
	movs	r1, #192
	lsls	r1, r1, #2
	movs	r0, #17
	bl 0x02008bc0
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r1, #1
	movs	r0, #18
	bl 0x02008bc0
	pop	{pc}
	push	{lr}
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r1, #2
	movs	r0, #19
	bl 0x02008bc0
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	bl 0x02008e84
	movs	r2, #85
	adds	r5, r0, #0
	adds	r2, r2, r5
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r6, #60
	mov	r8, r2
.L_0200011a:
	cmp	r6, #0
	beq.n	.L_0200012c
	movs	r0, #1
	bl 0x02008de4
	ldr	r3, [r5, #40]
	subs	r6, #1
	cmp	r3, #0
	bne.n	.L_0200011a
.L_0200012c:
	cmp	r7, #0
	beq.n	.L_02000136
	adds	r0, r7, #0
	bl 0x02008f24
.L_02000136:
	movs	r0, #10
	bl 0x02008de4
	movs	r3, #0
	mov	r2, r8
	strb	r3, [r2, #0]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #8
	movs	r2, #91
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #7
	movs	r1, #90
	movs	r2, #1
	movs	r3, #1
	bl 0x02008e44
	add	sp, #8
	pop	{pc}
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	sub	sp, #8
	bl 0x02008e84
	movs	r3, #8
	movs	r2, #91
	adds	r6, r0, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #8
	movs	r1, #90
	movs	r2, #1
	movs	r3, #1
	bl 0x02008e44
	cmp	r5, #16
	bne.n	.L_020001c2
	movs	r0, #146
	lsls	r0, r0, #4
	bl 0x02008e14
	cmp	r0, #0
	bne.n	.L_020001c2
	ldr	r3, [r6, #8]
	asrs	r3, r3, #19
	cmp	r3, #11
	bgt.n	.L_020001c2
	movs	r1, #181
	movs	r0, #16
	bl 0x02008100
	movs	r0, #146
	lsls	r0, r0, #4
	bl 0x02008e1c
	movs	r3, #5
	movs	r2, #27
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #6
	movs	r1, #27
	movs	r2, #1
	movs	r3, #1
	bl 0x02008e44
.L_020001c2:
	cmp	r5, #19
	bne.n	.L_02000254
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x02008e14
	cmp	r0, #0
	bne.n	.L_02000254
	ldr	r3, [r6, #8]
	asrs	r3, r3, #19
	cmp	r3, #77
	bne.n	.L_020001f0
	movs	r3, #37
	movs	r2, #102
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #33
	movs	r1, #102
	movs	r2, #1
	movs	r3, #2
	bl 0x02008e44
.L_020001f0:
	ldr	r3, [r6, #8]
	asrs	r3, r3, #19
	cmp	r3, #79
	bne.n	.L_0200021c
	ldr	r3, [r6, #12]
	asrs	r3, r3, #19
	cmp	r3, #12
	bne.n	.L_0200021c
	movs	r0, #19
	movs	r1, #181
	bl 0x02008100
	movs	r3, #39
	movs	r2, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #38
	movs	r1, #46
	movs	r2, #1
	movs	r3, #1
	bl 0x02008e44
.L_0200021c:
	ldr	r3, [r6, #8]
	asrs	r3, r3, #19
	cmp	r3, #81
	bne.n	.L_02000254
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x02008e1c
	movs	r3, #39
	movs	r5, #46
	str	r3, [sp, #0]
	movs	r0, #41
	movs	r1, #46
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x02008e44
	movs	r3, #40
	str	r3, [sp, #0]
	movs	r0, #27
	movs	r1, #44
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x02008e44
.L_02000254:
	add	sp, #8
	pop	{r5, r6, pc}
	push	{lr}
	movs	r0, #8
	sub	sp, #8
	bl 0x02008e84
	ldr	r3, [r0, #16]
	movs	r2, #13
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #13
	movs	r1, #18
	movs	r2, #1
	movs	r3, #1
	bl 0x02008e44
	add	sp, #8
	pop	{pc}
	push	{lr}
	movs	r0, #8
	sub	sp, #8
	bl 0x02008e84
	ldr	r3, [r0, #16]
	movs	r2, #13
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #11
	movs	r1, #14
	movs	r2, #1
	movs	r3, #1
	bl 0x02008e44
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #12
.L_020002a4:
	movs	r3, #110
	movs	r2, #47
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #110
	movs	r1, #64
	movs	r2, #14
	movs	r3, #12
	bl 0x02008f04
	add	sp, #12
	pop	{pc}
	push	{r5, r6, lr}
	movs	r0, #64
	bl 0x02008e84
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020002f6
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #116
	bl 0x02008e14
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020002f6
	movs	r2, #246
	movs	r0, #64
	ldr	r1, [pc, #20]
	lsls	r2, r2, #16
	bl 0x02008e8c
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	str	r3, [r5, #12]
.L_020002f6:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0291
	push	{lr}
	movs	r0, #64
	bl 0x02008e84
	cmp	r0, #0
.L_02000306:
	beq.n	.L_02000312
	movs	r0, #64
	movs	r1, #0
	movs	r2, #0
	bl 0x02008e8c
.L_02000312:
	pop	{pc}
	push	{r5, lr}
	movs	r0, #64
	bl 0x02008e84
	movs	r1, #1
	adds	r5, r0, #0
	ldr	r0, [pc, #72]
	bl 0x02008e64
	cmp	r5, #0
	beq.n	.L_0200033a
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq.n	.L_0200033a
	movs	r0, #130
	lsls	r0, r0, #5
	bl 0x02008edc
	b.n	.L_0200036a
.L_0200033a:
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #116
	bl 0x02008e14
	cmp	r0, #0
	bne.n	.L_02000362
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	ldr	r0, [pc, #32]
	movs	r1, #1
	bl 0x02008e64
	movs	r3, #173
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200036a
.L_02000362:
	ldr	r0, [pc, #16]
	movs	r1, #1
	bl 0x02008e64
.L_0200036a:
	pop	{r5, pc}
	.4byte 0x000022f5
	.4byte 0x000023e9
	.2byte 0x23ea
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x02008e14
	cmp	r0, #0
	beq.n	.L_020003bc
	bl 0x02008e74
	movs	r0, #0
	bl 0x02008ef4
	ldr	r0, [pc, #44]
	bl 0x02008ea4
	movs	r1, #1
	movs	r0, #20
	bl 0x02008e9c
	movs	r0, #15
	bl 0x02008e6c
	movs	r1, #0
	movs	r0, #20
	bl 0x02008eb4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02008e1c
	bl 0x02008e7c
.L_020003bc:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2316
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02008e14
	cmp	r0, #0
	beq.n	.L_020003e4
	ldr	r0, [pc, #28]
	bl 0x02008ea4
	movs	r0, #20
	movs	r1, #0
	bl 0x02008eb4
	b.n	.L_020003f2
.L_020003e4:
	ldr	r0, [pc, #16]
	bl 0x02008ea4
	movs	r0, #20
	movs	r1, #0
	bl 0x02008eac
.L_020003f2:
	pop	{pc}
	.4byte 0x00002316
	.2byte 0x2315
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02008e14
	cmp	r0, #0
	beq.n	.L_0200041c
	ldr	r0, [pc, #28]
	bl 0x02008ea4
	movs	r0, #20
	movs	r1, #0
	bl 0x02008eac
	b.n	.L_0200042a
.L_0200041c:
	ldr	r0, [pc, #16]
	bl 0x02008ea4
	movs	r0, #20
	movs	r1, #0
	bl 0x02008eac
.L_0200042a:
	pop	{pc}
	.4byte 0x0000231a
	.2byte 0x2319
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
	bne.n	.L_0200044c
	ldr	r0, [pc, #24]
	b.n	.L_02000458
.L_0200044c:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000456
	ldr	r0, [pc, #24]
	b.n	.L_02000458
.L_02000456:
	ldr	r0, [pc, #24]
.L_02000458:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000ae
	.4byte 0x020093c8
	.4byte 0x000000af
	.4byte 0x020094b8
	.2byte 0x9554
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	bl 0x02008df4
	ldrh	r6, [r7, #6]
	movs	r1, #128
	lsls	r1, r1, #10
	adds	r5, r0, #0
	adds	r0, r6, #0
	adds	r5, r5, r1
	bl 0x02008e04
	ldr	r2, [pc, #140]
	adds	r1, r0, #0
	mov	r8, r2
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4682
	adds	r0, r6, #0
	bl 0x02008dfc
	adds	r1, r0, #0
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x68bb
	movs	r1, #255
	add	r3, sl
	str	r3, [r7, #8]
	ldr	r3, [r7, #16]
	lsls	r1, r1, #8
	adds	r3, r3, r0
	str	r3, [r7, #16]
	ldrh	r3, [r7, #6]
	adds	r1, #240
	adds	r3, r3, r1
	strh	r3, [r7, #6]
	adds	r5, r7, #0
	adds	r5, #102
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	beq.n	.L_020004e4
	subs	r3, r2, #1
	strh	r3, [r5, #0]
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	strh	r3, [r7, #6]
	b.n	.L_020004fc
.L_020004e4:
	bl 0x02008df4
	lsls	r0, r0, #5
	lsrs	r0, r0, #16
	cmp	r0, #0
	bne.n	.L_020004fc
	bl 0x02008df4
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	adds	r0, #8
	strh	r0, [r5, #0]
.L_020004fc:
	adds	r2, r7, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	movs	r1, #142
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	cmp	r3, r1
	bne.n	.L_02000518
	ldr	r1, [pc, #16]
	adds	r0, r7, #0
	bl 0x02008e2c
.L_02000518:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0x95c0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r1, #0
	movs	r2, #20
	sub	sp, #12
	adds	r7, r0, #0
	mov	r8, r1
	mov	sl, r2
.L_0200053e:
	ldr	r3, [r7, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	mov	r2, r8
	muls	r2, r3
	ldr	r3, [r7, #12]
	movs	r1, #128
	adds	r3, r3, r2
	lsls	r1, r1, #11
	adds	r3, r3, r1
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	bl 0x02008df4
	adds	r6, r0, #0
	bl 0x02008df4
	movs	r2, #192
	adds	r1, r0, #0
	lsls	r2, r2, #10
	lsls	r0, r6, #2
	adds	r0, r0, r6
	mov	r9, r2
	add	r0, r9
	adds	r2, r5, #0
	bl 0x02008e0c
	movs	r0, #154
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	lsls	r0, r0, #1
	bl 0x02008efc
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_020005d0
	ldr	r3, [pc, #56]
	mov	r1, sl
	str	r3, [r6, #108]
	adds	r3, r6, #0
	adds	r3, #100
	strh	r1, [r3, #0]
	movs	r2, #0
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	mov	r2, r9
	str	r3, [r6, #72]
	str	r2, [r6, #40]
	bl 0x02008df4
	ldr	r5, [pc, #20]
	adds	r3, r6, #0
	adds	r3, #35
	strh	r0, [r6, #6]
	movs	r1, #2
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	bl 0x02008e5c
	b.n	.L_020005d0
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x8475
	.2byte 0x0200
.L_020005d0:
	movs	r1, #1
	movs	r3, #2
	add	r8, r1
	negs	r3, r3
	mov	r2, r8
	add	sl, r3
	cmp	r2, #15
	ble.n	.L_0200053e
	movs	r0, #0
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #164
	ldr	r2, [r3, #0]
	movs	r4, #0
	movs	r1, #4
	ldrsh	r3, [r2, r1]
	adds	r0, r4, #0
	cmp	r3, #8
	bne.n	.L_0200060a
	adds	r4, r2, #4
	b.n	.L_02000622
.L_0200060a:
	adds	r0, #1
	cmp	r0, #7
	bgt.n	.L_02000622
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r1, r3, #2
	adds	r3, r1, #4
	ldrsh	r3, [r2, r3]
	cmp	r3, #8
	bne.n	.L_0200060a
	adds	r3, r2, r1
	adds	r4, r3, #4
.L_02000622:
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r4, #16]
	movs	r3, #64
	str	r3, [r4, #4]
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #164
	ldr	r2, [r3, #0]
	movs	r4, #0
	movs	r1, #4
	ldrsh	r3, [r2, r1]
	adds	r0, r4, #0
	cmp	r3, #8
	bne.n	.L_0200064a
	adds	r4, r2, #4
	b.n	.L_02000662
.L_0200064a:
	adds	r0, #1
	cmp	r0, #7
	bgt.n	.L_02000662
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r1, r3, #2
	adds	r3, r1, #4
	ldrsh	r3, [r2, r3]
	cmp	r3, #8
	bne.n	.L_0200064a
	adds	r3, r2, r1
	adds	r4, r3, #4
.L_02000662:
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r4, #16]
	ldr	r3, [r4, #4]
	adds	r3, #128
	str	r3, [r4, #4]
	pop	{r5, pc}
	push	{r5, r6, lr}
	movs	r0, #17
	bl 0x02008e84
	ldr	r3, [pc, #100]
	adds	r5, r0, #0
	ldrb	r6, [r3, #0]
	cmp	r6, #0
	bne.n	.L_020006de
	movs	r1, #202
	bl 0x02008f1c
	movs	r0, #140
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #1
	bl 0x02008efc
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020006de
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #52]
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #24]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #28]
	adds	r3, r5, #0
	adds	r3, #35
	strb	r6, [r3, #0]
	movs	r1, #2
	bl 0x02008e24
	ldr	r1, [r5, #8]
	ldr	r3, [pc, #32]
	ldr	r2, [r5, #12]
	adds	r1, r1, r3
	adds	r0, r5, #0
	ldr	r3, [r5, #16]
	bl 0x02008e3c
	ldr	r1, [pc, #24]
	adds	r0, r5, #0
	bl 0x02008e2c
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x02008e5c
.L_020006de:
	pop	{r5, r6, pc}
	.4byte 0x0300122c
	.4byte 0xffe00000
	.2byte 0x95c4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #192
	lsls	r1, r1, #18
	ldr	r3, [r1, #108]
	ldr	r6, [pc, #128]
	movs	r2, #214
	lsls	r2, r2, #1
	mov	r8, r1
	movs	r1, #152
	adds	r3, r3, r2
	ldr	r5, [pc, #120]
	adds	r2, #88
	lsls	r1, r1, #2
	str	r2, [r3, #0]
	adds	r3, r6, r1
	movs	r1, #5
	mov	sl, r1
	adds	r2, #94
	strh	r5, [r3, #0]
	adds	r3, r6, r2
	mov	r2, sl
	strh	r2, [r3, #0]
	sub	sp, #8
	bl 0x02008f0c
	bl 0x02008e84
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	movs	r1, #240
	orrs	r3, r2
	lsls	r1, r1, #1
	strb	r3, [r0, #0]
	adds	r3, r6, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r7, [pc, #56]
	cmp	r2, r5
	beq.n	.L_02000744
	b.n	.L_0200093c
.L_02000744:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #56]
	bl 0x02008dec
	movs	r0, #8
	movs	r1, #1
	bl 0x02008f14
	movs	r0, #9
	movs	r1, #1
	bl 0x02008f14
	movs	r0, #10
	movs	r1, #1
	bl 0x02008f14
	movs	r0, #11
	movs	r1, #1
	bl 0x02008f14
	movs	r0, #12
	movs	r1, #1
	bl 0x02008f14
	b.n	.L_02000788
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x000000ae
	.2byte 0x8671
	.2byte 0x0200
.L_02000788:
	movs	r0, #13
	movs	r1, #1
	bl 0x02008f14
	movs	r0, #14
	movs	r1, #1
	bl 0x02008f14
	movs	r0, #15
	movs	r1, #1
	bl 0x02008f14
	movs	r0, #146
	lsls	r0, r0, #4
	bl 0x02008e14
	cmp	r0, #0
	beq.n	.L_020007e0
	movs	r0, #16
	bl 0x02008e84
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	movs	r3, #176
	lsls	r3, r3, #15
	str	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	movs	r1, #0
	bl 0x02008e4c
	movs	r3, #27
	mov	r2, sl
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #6
	movs	r1, #27
	movs	r2, #1
	movs	r3, #1
	bl 0x02008e44
.L_020007e0:
	movs	r0, #19
	bl 0x02008e84
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r0, #144
	adds	r3, #35
	lsls	r0, r0, #4
	strb	r7, [r3, #0]
	adds	r0, #33
	bl 0x02008e14
	cmp	r0, #0
	beq.n	.L_02000840
	adds	r3, r5, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	movs	r3, #162
	lsls	r3, r3, #18
	str	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r5, #12]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008e4c
	movs	r3, #37
	movs	r2, #102
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #33
	movs	r1, #102
	movs	r2, #1
	movs	r3, #2
	bl 0x02008e44
	movs	r3, #40
	movs	r2, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #27
	movs	r1, #44
	movs	r2, #1
	movs	r3, #1
	bl 0x02008e44
	b.n	.L_0200088a
.L_02000840:
	movs	r0, #10
	adds	r0, #255
	bl 0x02008e14
	cmp	r0, #0
	beq.n	.L_0200088a
	ldr	r3, [r5, #8]
	asrs	r3, r3, #19
	cmp	r3, #75
	beq.n	.L_02000868
	movs	r3, #37
	movs	r2, #102
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #33
	movs	r1, #102
	movs	r2, #1
	movs	r3, #2
	bl 0x02008e44
.L_02000868:
	ldr	r3, [r5, #8]
	asrs	r3, r3, #19
	cmp	r3, #79
	bne.n	.L_0200088a
	adds	r3, r5, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	movs	r2, #46
	movs	r3, #39
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #38
	movs	r1, #46
	movs	r2, #1
	movs	r3, #1
	bl 0x02008e44
.L_0200088a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x02008e14
	cmp	r0, #0
	bne.n	.L_0200089a
	b.n	.L_02000a36
.L_0200089a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02008e14
	cmp	r0, #0
	beq.n	.L_020008aa
	b.n	.L_02000a36
.L_020008aa:
	movs	r2, #200
	movs	r0, #20
	ldr	r1, [pc, #404]
	lsls	r2, r2, #18
	bl 0x02008e8c
	movs	r5, #49
	movs	r0, #53
	movs	r1, #52
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02008e44
	movs	r3, #50
	str	r3, [sp, #0]
	movs	r0, #53
	movs	r1, #52
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x02008e44
	movs	r0, #20
	movs	r1, #5
	bl 0x02008e94
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #97
	bl 0x02008e14
	cmp	r0, #0
	beq.n	.L_020008f2
	b.n	.L_02000a36
.L_020008f2:
	bl 0x02008ee4
	bl 0x02008eec
	movs	r0, #20
	movs	r1, #3
	bl 0x02008e9c
	movs	r1, #1
	movs	r0, #20
	bl 0x02008ed4
	movs	r0, #30
	bl 0x02008e6c
	ldr	r0, [pc, #308]
	bl 0x02008ea4
	movs	r0, #20
	movs	r1, #0
	bl 0x02008eac
	movs	r1, #2
	movs	r0, #20
	bl 0x02008e9c
	movs	r0, #30
	bl 0x02008e6c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #97
	bl 0x02008e1c
	bl 0x02008e7c
	b.n	.L_02000a36
.L_0200093c:
	ldr	r3, [pc, #268]
	cmp	r2, r3
	bne.n	.L_020009e8
	movs	r0, #8
	movs	r1, #1
	bl 0x02008f14
	movs	r0, #9
	movs	r1, #1
	bl 0x02008f14
	movs	r0, #10
	movs	r1, #1
	bl 0x02008f14
	movs	r0, #11
	movs	r1, #1
	bl 0x02008f14
	movs	r0, #12
	movs	r1, #1
	bl 0x02008f14
	movs	r0, #13
	movs	r1, #1
	bl 0x02008f14
	movs	r1, #1
	movs	r0, #14
	bl 0x02008f14
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #3
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_02000992
	bl 0x02008dd0
.L_02000992:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02008e14
	cmp	r0, #0
	beq.n	.L_020009ae
	movs	r1, #191
	movs	r3, #212
	lsls	r1, r1, #18
	ldr	r2, [pc, #168]
	lsls	r3, r3, #17
	movs	r0, #17
	bl 0x02008b2c
.L_020009ae:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008e14
	cmp	r0, #0
	beq.n	.L_020009ca
	movs	r3, #212
	ldr	r1, [pc, #148]
	ldr	r2, [pc, #148]
	lsls	r3, r3, #17
	movs	r0, #18
	bl 0x02008b2c
.L_020009ca:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02008e14
	cmp	r0, #0
	beq.n	.L_02000a36
	movs	r3, #212
	ldr	r1, [pc, #128]
	ldr	r2, [pc, #128]
	lsls	r3, r3, #17
	movs	r0, #19
	bl 0x02008b2c
	b.n	.L_02000a36
.L_020009e8:
	ldr	r3, [pc, #120]
	cmp	r2, r3
	bne.n	.L_02000a36
	movs	r0, #64
	bl 0x02008e84
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000a04
	movs	r0, #64
	movs	r1, #0
	movs	r2, #0
	bl 0x02008e8c
.L_02000a04:
	mov	r1, r8
	ldr	r3, [r1, #108]
	movs	r2, #138
	ldr	r5, [r3, #84]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	str	r5, [r3, #0]
	movs	r0, #8
	bl 0x02008e84
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	movs	r2, #13
	ldr	r3, [r5, #16]
	movs	r0, #11
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r1, #14
	movs	r2, #1
	movs	r3, #1
	bl 0x02008e44
.L_02000a36:
	movs	r0, #0
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x031e0000
	.4byte 0x00002314
	.4byte 0x000000af
	.4byte 0xfe790000
	.4byte 0x03150000
	.4byte 0xff080000
	.4byte 0x030a0000
	.4byte 0xff890000
	.2byte 0x00b0
	.2byte 0x0000
	push	{lr}
	ldr	r1, [pc, #60]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000aa4
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	movs	r0, #128
	subs	r3, #3
	lsls	r3, r3, #16
	lsls	r0, r0, #9
	cmp	r3, r0
	bhi.n	.L_02000aa4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	subs	r2, #162
	adds	r3, r3, r2
	ldr	r2, [pc, #20]
	str	r2, [r3, #8]
	movs	r2, #137
	lsls	r2, r2, #19
	str	r2, [r3, #12]
.L_02000aa4:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000af
	.2byte 0x0000
	.2byte 0xff80
	.2byte 0xb560
	ldr	r2, [pc, #104]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	bne.n	.L_02000b1e
	movs	r3, #192
	movs	r1, #133
	lsls	r3, r3, #18
	lsls	r1, r1, #2
	ldr	r6, [r3, #108]
	ldr	r5, [r3, #32]
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	bl 0x02008e84
	ldr	r3, [r0, #8]
	cmp	r3, #0
	bge.n	.L_02000ae4
	ldr	r2, [pc, #64]
	adds	r3, r3, r2
.L_02000ae4:
	ldr	r2, [r0, #16]
	asrs	r1, r3, #20
	ldr	r3, [r0, #12]
	subs	r0, r2, r3
	movs	r2, #254
	lsls	r2, r2, #7
	adds	r2, #255
	adds	r3, r0, r2
	cmp	r3, #0
	bge.n	.L_02000afc
	ldr	r2, [pc, #44]
	adds	r3, r0, r2
.L_02000afc:
	movs	r0, #212
	lsls	r0, r0, #1
	asrs	r3, r3, #20
	adds	r2, r5, r0
	ldr	r2, [r2, #0]
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r2, [r2, #2]
	subs	r3, r2, #1
	cmp	r3, #229
	bhi.n	.L_02000b1e
	movs	r1, #170
	lsls	r1, r1, #1
	adds	r3, r6, r1
	strh	r2, [r3, #0]
.L_02000b1e:
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x000fffff
	.2byte 0x7ffe
	.2byte 0x0010
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r1, #0
	mov	r8, r2
	adds	r6, r3, #0
	bl 0x02008e84
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #212
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r4, r0, #0
	ldr	r0, [r3, #0]
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_02000b56
	ldr	r2, [pc, #96]
	adds	r3, r5, r2
.L_02000b56:
	asrs	r7, r3, #20
	mov	r3, r8
	subs	r2, r6, r3
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	adds	r1, r2, r3
	cmp	r1, #0
	bge.n	.L_02000b6c
	ldr	r3, [pc, #76]
	adds	r1, r2, r3
.L_02000b6c:
	asrs	r1, r1, #20
	lsls	r3, r1, #7
	adds	r3, r7, r3
	lsls	r3, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	strb	r3, [r0, #2]
	ldr	r0, [pc, #64]
	movs	r3, #128
	ands	r5, r0
	ands	r6, r0
	lsls	r3, r3, #12
	adds	r2, r5, r3
	lsls	r1, r1, #20
	adds	r3, r6, r3
	str	r2, [r4, #8]
	str	r3, [r4, #16]
	adds	r2, r4, #0
	subs	r3, r3, r1
	str	r3, [r4, #12]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r4, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r4, #0
	movs	r1, #0
	bl 0x02008e4c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x000fffff
	.4byte 0x00107ffe
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	ldr	r3, [pc, #492]
	str	r1, [sp, #12]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008e84
	adds	r5, r0, #0
	ldr	r3, [r5, #72]
	str	r3, [sp, #8]
	bl 0x02008e74
	movs	r0, #0
	bl 0x02008ef4
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x02008ec4
	adds	r0, r6, #0
	bl 0x02008e84
	ldr	r2, [r5, #12]
	adds	r7, r0, #0
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	adds	r0, r6, #0
	bl 0x02008b2c
	ldr	r1, [pc, #424]
	adds	r0, r7, #0
	bl 0x02008e2c
	movs	r2, #15
	mov	fp, r2
.L_02000c24:
	ldr	r2, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	movs	r0, #255
	bl 0x02008e34
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02000c9c
	ldr	r1, [pc, #392]
	bl 0x02008e2c
	bl 0x02008df4
	mov	sl, r0
	bl 0x02008df4
	adds	r6, r0, #0
	bl 0x02008df4
	movs	r3, #128
	lsls	r3, r3, #6
	lsrs	r2, r0, #1
	adds	r2, r2, r3
	str	r0, [sp, #4]
	mov	r0, sl
	mov	r9, r2
	bl 0x02008e04
	ldr	r2, [pc, #356]
	lsls	r6, r6, #3
	adds	r1, r0, #0
	mov	r8, r2
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x62b8
	mov	r0, sl
	bl 0x02008dfc
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [r7, #52]
	mov	r3, r9
	str	r3, [r7, #24]
	str	r3, [r7, #28]
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r0, [r7, #36]
.L_02000c9a:
	str	r3, [r7, #68]
.L_02000c9c:
	movs	r2, #1
	negs	r2, r2
	add	fp, r2
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_02000c24
	movs	r0, #128
	lsls	r0, r0, #2
.L_02000cac:
	adds	r0, #78
	bl 0x02008f24
	movs	r0, #217
	bl 0x02008f24
	movs	r2, #230
	movs	r0, #128
.L_02000cbc:
	movs	r1, #128
	lsls	r2, r2, #8
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	adds	r2, #102
	bl 0x02008e54
	ldr	r3, [pc, #244]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #2
	ldr	r0, [r3, #0]
	adds	r1, #255
	bl 0x02008ebc
	movs	r1, #40
	adds	r0, r5, #0
	bl 0x02008e24
	movs	r0, #40
	bl 0x02008e6c
	movs	r0, #204
	bl 0x02008f24
	adds	r2, r5, #0
	movs	r3, #3
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	movs	r6, #0
	str	r3, [r5, #72]
	b.n	.L_02000d06
.L_02000d04:
	adds	r6, #1
.L_02000d06:
	cmp	r6, #179
	bgt.n	.L_02000d18
	movs	r0, #1
	bl 0x02008de4
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	cmp	r2, r3
	bgt.n	.L_02000d04
.L_02000d18:
	movs	r0, #188
	bl 0x02008f24
	ldr	r3, [sp, #8]
	ldr	r6, [pc, #156]
	str	r3, [r5, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x02008ebc
	adds	r0, r5, #0
	movs	r1, #38
	bl 0x02008e24
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x02008e54
	movs	r0, #10
	bl 0x02008de4
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x02008e54
	movs	r0, #20
	bl 0x02008de4
	ldr	r2, [r5, #16]
	movs	r3, #1
	ldr	r1, [r5, #12]
	ldr	r0, [r5, #8]
	bl 0x02008ec4
	bl 0x02008ecc
	movs	r0, #20
	bl 0x02008e6c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008e4c
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x02008e24
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r0, #20
	bl 0x02008de4
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r2, r6, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	bl 0x02008e7c
	ldr	r0, [sp, #12]
	bl 0x02008e1c
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x020096cc
	.4byte 0x02009688
	.2byte 0x021c
	.2byte 0x0300
	push	{lr}
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl 0x02008dec
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02008ab5
	.irp EntryTarget, 0x080000c1, 0x080000d1, 0x080000f9, 0x08000119, 0x08000121, 0x08000129, 0x080003c9, 0x080003d1, 0x08020091, 0x080200a9, 0x080200c1, 0x08020149, 0x080201e9, 0x08020219, 0x08020229, 0x08020291, 0x08038041, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c80f9, 0x080c8119, 0x080c8141, 0x080c8181, 0x080c81a1, 0x080c81a9, 0x080c8219, 0x080c8239, 0x080c8241, 0x080c8249, 0x080c8369, 0x080c83a9, 0x080c83b9, 0x080c84e1, 0x080c8759, 0x080c8761, 0x080c8779, 0x080c8781, 0x080c88c9, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0xffff0000
	.4byte 0x00000018
	.4byte 0x00000198
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe40303
	.4byte 0x030b01e4
	.4byte 0x01ecffec
	.4byte 0x0002ffff
	.4byte 0x00b40303
	.4byte 0x030b02c3
	.4byte 0x02cb00bc
	.4byte 0x0003ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x012800c3
	.4byte 0x00cb0143
	.4byte 0x014b0130
	.4byte 0x0002ffff
	.4byte 0xfdd602c3
	.4byte 0x02cb01c4
	.4byte 0x01ccfdde
	.4byte 0x0003ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000ae
	.4byte 0x101010af
	.4byte 0xffffffff
	.4byte 0x102030ae
	.4byte 0xffffffff
	.4byte 0x103020ae
	.4byte 0xffffffff
	.4byte 0x104010b2
	.4byte 0xffffffff
	.4byte 0x10527002
	.4byte 0xffffffff
	.4byte 0x000000af
	.4byte 0x101010ae
	.4byte 0xffffffff
	.4byte 0x102030af
	.4byte 0xffffffff
	.4byte 0x103020af
	.4byte 0xffffffff
	.4byte 0x104010b0
	.4byte 0xffffffff
	.4byte 0x000000b0
	.4byte 0x101040af
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x03e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x03a80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x0200901c
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x01270000
	.4byte 0x00000000
	.4byte 0x01ba0000
	.4byte 0x00028000
	.4byte 0xffff0118
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x0200901c
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00024000
	.4byte 0xffff001d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff017a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff017a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff017a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff0181
	.4byte 0x00000001
	.4byte 0x02900000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff018d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0x09ab002a
	.4byte 0x02008379
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020083c5
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x020083fd
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000c
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000e
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000f
	.4byte 0x00000000
	.4byte 0x50008905
	.4byte 0xffff0014
	.4byte 0x020082a1
	.4byte 0x10008c15
	.4byte 0x09200010
	.4byte 0x02008149
	.4byte 0x00008c15
	.4byte 0x09200010
	.4byte 0x02008165
	.4byte 0x00008c15
	.4byte 0x09210013
	.4byte 0x02008165
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008165
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0x03000014
	.4byte 0x020080d1
	.4byte 0x00000002
	.4byte 0x03010015
	.4byte 0x020080e1
	.4byte 0x00000002
	.4byte 0x03020016
	.4byte 0x020080f1
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000c
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000e
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008315
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte 0x02008259
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x0200827d
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008259
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200827d
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x020082c1
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020082fd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000026
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001999
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00001999
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0001b333
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00014ccc
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x0000002e
	.4byte 0x020085f1
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x80010000
	.4byte 0x0000002e
	.4byte 0x02008631
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x00000003
	.4byte 0xc0010000
	.4byte 0x0000002e
	.4byte 0x02008529
	.4byte 0x80020000
	.4byte 0x0000002e
	.4byte 0x02008631
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x0000000e
	.4byte 0xc0020000
	.4byte 0x0000002e
	.4byte 0x02008631
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000011
