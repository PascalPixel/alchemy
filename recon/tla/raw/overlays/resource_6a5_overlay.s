.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200bd9d, 0x0200805d, 0x02008069, 0x02008071, 0x0200ba21, 0x02008065, 0x0200bf1d
	overlay_veneer \EntryTarget
	.endr
	.2byte 0xb520
	.2byte 0x1c05
	.2byte 0x2098
	bl 0x0200d9f4
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200d7a4
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r1, #1
	bl 0x0200d7a4
	movs	r0, #0
	pop	{pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe4d0
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe500
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.4byte 0x0200e520
	.4byte 0x061b2380
	.4byte 0x63c36383
	.4byte 0x23006403
	.4byte 0x62836243
	.4byte 0x306462c3
	.2byte 0x8003
	.2byte 0x4770
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	adds	r5, r0, #0
	movs	r0, #31
	bl 0x0200d82c
	adds	r6, r5, #0
	adds	r6, #100
	ldrh	r1, [r6, #0]
	mov	sl, r0
	mov	r8, r1
	mov	r0, r8
	bl 0x0200d6dc
	ldr	r3, [r5, #48]
	mov	r1, sl
	adds	r3, #3
	adds	r2, r3, #0
	muls	r2, r0
	ldr	r3, [r1, #8]
	mov	r0, r8
	adds	r3, r3, r2
	str	r3, [r5, #8]
	bl 0x0200d6d4
	mov	r2, sl
	ldr	r3, [r2, #16]
	ldr	r2, [r5, #8]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	str	r3, [r5, #16]
	str	r2, [r5, #56]
	str	r3, [r5, #64]
	ldr	r1, [pc, #16]
	ldrh	r3, [r6, #0]
	adds	r3, r3, r1
	strh	r3, [r6, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xf800
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	movs	r0, #36
	bl 0x0200d82c
	adds	r5, r7, #0
	adds	r5, #100
	ldrh	r6, [r5, #0]
	mov	r8, r0
	adds	r0, r6, #0
	bl 0x0200d6dc
	movs	r1, #98
	adds	r1, r1, r7
	ldrb	r2, [r1, #0]
	ldr	r3, [r7, #48]
	mov	sl, r1
	adds	r3, r3, r2
	mov	r1, r8
	adds	r3, #6
	adds	r2, r3, #0
	muls	r2, r0
	ldr	r3, [r1, #8]
	adds	r0, r6, #0
	adds	r3, r3, r2
	str	r3, [r7, #8]
	bl 0x0200d6d4
	mov	r2, sl
	ldrb	r3, [r2, #0]
	mov	r1, r8
	adds	r3, #4
	adds	r2, r3, #0
	muls	r2, r0
	ldr	r3, [r1, #16]
	adds	r3, r3, r2
	ldr	r2, [r7, #8]
	str	r3, [r7, #16]
	str	r2, [r7, #56]
	str	r3, [r7, #64]
	ldr	r2, [pc, #16]
	ldrh	r3, [r5, #0]
	adds	r3, r3, r2
	strh	r3, [r5, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xf800
	.2byte 0xffff
	push	{lr}
	adds	r2, r0, #0
	movs	r0, #0
	cmp	r2, #0
	beq.n	.L_0200018c
	adds	r3, r2, #0
	adds	r3, #100
	ldrh	r3, [r3, #0]
	ldrh	r1, [r2, #6]
	subs	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	cmp	r0, #0
	beq.n	.L_0200018c
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r0, r3
	ble.n	.L_02000180
	movs	r0, #128
	lsls	r0, r0, #4
.L_02000180:
	ldr	r3, [pc, #12]
	cmp	r0, r3
	bge.n	.L_02000188
	ldr	r0, [pc, #12]
.L_02000188:
	adds	r3, r1, r0
	strh	r3, [r2, #6]
.L_0200018c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0xfffff000
	.2byte 0xf800
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #36
	bl 0x0200d82c
	ldr	r5, [pc, #444]
	adds	r6, r0, #0
	ldr	r3, [r5, #0]
	movs	r7, #0
	cmp	r3, #48
	bls.n	.L_020001b4
	b.n	.L_02000390
.L_020001b4:
	ldr	r2, [pc, #432]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x02008280
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x0200829a
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x020082ac
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x020082be
	.4byte 0x020082e6
	.4byte 0x020082fe
	.4byte 0x02008346
	.4byte 0x02008346
	.4byte 0x02008346
	.4byte 0x02008346
	.4byte 0x02008346
	.4byte 0x02008346
	.4byte 0x02008346
	.4byte 0x02008346
	.4byte 0x02008390
	.4byte 0x0200834a
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008390
	.4byte 0x02008380
	.4byte 0xf00520dc
	.4byte 0x20c0fbb7
	.4byte 0x228021c0
	.4byte 0x02c902c0
	.4byte 0xf0050252
	.4byte 0x4835fa8b
	.4byte 0x2080e001
	.4byte 0x21010240
	.4byte 0xfb58f005
	.4byte 0xf0052008
	.4byte 0xe071fb5d
	.4byte 0x21802080
	.4byte 0x02402280
	.4byte 0x02520249
	.4byte 0xfa78f005
	.4byte 0x2398e068
	.4byte 0x60b3045b
	.4byte 0x1c304b2a
	.4byte 0x23a460f3
	.4byte 0x6133041b
	.4byte 0x025b2380
	.4byte 0x61f361b3
	.4byte 0xfecef7ff
	.4byte 0x20244925
	.4byte 0xfaacf005
	.4byte 0x682be054
	.4byte 0x602b3b01
	.4byte 0x2b0068f3
	.4byte 0x4821dd19
	.4byte 0xf0052100
	.4byte 0x2010fb2d
	.4byte 0x682be00d
	.4byte 0x3b0122a0
	.4byte 0x0392602b
	.4byte 0x429368f3
	.4byte 0x2080dd0b
	.4byte 0x21000240
	.4byte 0xfb1ef005
	.4byte 0xf0052028
	.4byte 0x682bfb23
	.4byte 0x602b3301
	.4byte 0x4b15e034
	.4byte 0x681b2207
	.4byte 0x2b004013
	.4byte 0x20f6d102
	.4byte 0xfb5ef005
	.4byte 0x219068f3
	.4byte 0x185b0289
	.4byte 0x60f32701
	.4byte 0x2701e024
	.4byte 0x20bbe022
	.4byte 0xfb52f005
	.4byte 0x01c020fe
	.4byte 0x210030ff
	.4byte 0xfafcf005
	.4byte 0xf005200c
	.4byte 0xe015fb01
	.4byte 0x0200ea24
	.4byte 0x020081bc
	.4byte 0x002063ff
	.4byte 0xfe980000
	.4byte 0x0200e41c
	.4byte 0x00203210
	.4byte 0x0300122c
	.4byte 0xf0052024
	.4byte 0x2080fa63
	.4byte 0x30020080
	.2byte 0xf005
	.2byte 0xf9d2
.L_02000390:
	cmp	r7, #0
	beq.n	.L_02000448
	bl 0x0200d6cc
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r2, [r6, #12]
	lsls	r3, r3, #4
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	subs	r2, r2, r3
	ldr	r3, [pc, #128]
	movs	r0, #168
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	lsls	r0, r0, #2
	bl 0x0200d76c
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02000448
	ldr	r1, [r7, #80]
	movs	r5, #0
	mov	sl, r1
	ldr	r1, [pc, #104]
	bl 0x0200d764
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200d8bc
	adds	r3, r7, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	bl 0x0200d6cc
	ldr	r3, [pc, #84]
	adds	r2, r7, #0
	adds	r2, #100
	ands	r3, r0
	strh	r3, [r2, #0]
	ldr	r1, [pc, #60]
	adds	r3, r7, #0
	adds	r3, #102
	strh	r5, [r3, #0]
	mov	r8, r1
	bl 0x0200d6cc
	adds	r3, r7, #0
	lsrs	r0, r0, #13
	adds	r3, #98
	strb	r0, [r3, #0]
	ldr	r3, [pc, #56]
	str	r3, [r7, #108]
	bl 0x0200d6cc
	adds	r3, r0, #0
	lsls	r0, r3, #16
	subs	r0, r0, r3
	lsrs	r0, r0, #20
	bl 0x0200d6d4
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	str	r3, [r7, #48]
	mov	r1, sl
	movs	r2, #50
	ldrsh	r3, [r6, r2]
	str	r3, [r7, #48]
	mov	r3, r8
	b.n	.L_02000438
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xfff80000
	.4byte 0x0200e470
	.4byte 0x0ffff000
	.2byte 0x80ed
	.2byte 0x0200
.L_02000438:
	ldrb	r2, [r1, #9]
	strb	r3, [r1, #26]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
.L_02000448:
	ldr	r2, [pc, #12]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0xea24
	.2byte 0x0200
	push	{lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #35
	bl 0x0200d72c
	cmp	r0, #0
	beq.n	.L_020004c0
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200d734
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200d72c
	cmp	r0, #0
	bne.n	.L_020004c0
	bl 0x0200d814
	movs	r0, #0
	bl 0x0200d984
	ldr	r0, [pc, #52]
	bl 0x0200d8c4
	movs	r1, #4
	movs	r2, #30
	adds	r1, #255
	movs	r0, #28
	bl 0x0200d8fc
	movs	r0, #28
	movs	r1, #0
	bl 0x0200d8dc
	movs	r0, #27
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200d8dc
	bl 0x0200d81c
.L_020004c0:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2ac7
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d72c
	cmp	r0, #0
	bne.n	.L_020004ea
	ldr	r3, [pc, #44]
	movs	r1, #3
	ldr	r0, [r3, #0]
	bl 0x0200d6ac
	cmp	r0, #0
	bne.n	.L_020005b0
.L_020004ea:
	movs	r0, #31
	bl 0x0200d82c
	adds	r5, r0, #0
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d72c
	cmp	r0, #0
	beq.n	.L_02000510
	bl 0x0200d6cc
	adds	r2, r0, #0
	ldr	r3, [r5, #12]
	lsls	r2, r2, #8
	b.n	.L_0200051a
	.2byte 0x122c
	.2byte 0x0300
.L_02000510:
	bl 0x0200d6cc
	adds	r2, r0, #0
	ldr	r3, [r5, #12]
	lsls	r2, r2, #6
.L_0200051a:
	lsrs	r2, r2, #16
	lsls	r2, r2, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #124]
	movs	r0, #168
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #2
	bl 0x0200d76c
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_020005b0
	ldr	r1, [pc, #108]
	adds	r0, r7, #0
	ldr	r6, [r7, #80]
	bl 0x0200d764
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200d8bc
	adds	r3, r7, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	bl 0x0200d6cc
	ldr	r3, [pc, #80]
	adds	r2, r7, #0
	adds	r2, #100
	ands	r3, r0
	strh	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #102
	strh	r5, [r3, #0]
	ldr	r3, [pc, #68]
	ldr	r1, [pc, #52]
	str	r3, [r7, #108]
	mov	r8, r1
	bl 0x0200d6cc
	adds	r3, r0, #0
	lsls	r0, r3, #16
	subs	r0, r0, r3
	lsrs	r0, r0, #20
	bl 0x0200d6d4
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	asrs	r3, r3, #16
	str	r3, [r7, #48]
	mov	r3, r8
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #26]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #9]
	b.n	.L_020005b0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffe40000
	.4byte 0x0200e8b0
	.4byte 0x0ffff000
	.2byte 0x8091
	.2byte 0x0200
.L_020005b0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #36
	bl 0x0200d82c
	movs	r3, #192
	adds	r5, r0, #0
	lsls	r3, r3, #18
	ldr	r6, [r3, #32]
	bl 0x0200d6cc
	cmp	r5, #0
	beq.n	.L_02000656
	adds	r3, r6, #0
	adds	r3, #232
	movs	r2, #2
	ldrsh	r3, [r3, r2]
	cmp	r3, #177
	bgt.n	.L_0200064c
	movs	r0, #36
	bl 0x0200d82c
	ldr	r3, [r0, #80]
	movs	r2, #1
	ldr	r3, [r3, #40]
	strb	r2, [r3, #5]
	ldr	r3, [pc, #108]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200061a
	movs	r1, #128
	movs	r2, #132
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r0, #36
	bl 0x0200d884
	movs	r0, #36
	bl 0x0200d82c
	movs	r3, #223
	lsls	r3, r3, #15
	str	r3, [r0, #12]
	movs	r0, #36
	bl 0x0200d82c
	movs	r5, #128
	lsls	r5, r5, #9
	b.n	.L_02000640
.L_0200061a:
	movs	r1, #128
	movs	r2, #132
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r0, #36
	bl 0x0200d884
	movs	r0, #36
	bl 0x0200d82c
	movs	r3, #209
	lsls	r3, r3, #15
	str	r3, [r0, #12]
	movs	r0, #36
	bl 0x0200d82c
	movs	r5, #166
	lsls	r5, r5, #9
	adds	r5, #204
.L_02000640:
	str	r5, [r0, #24]
	movs	r0, #36
	bl 0x0200d82c
	str	r5, [r0, #28]
	b.n	.L_02000656
.L_0200064c:
	movs	r0, #36
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d884
.L_02000656:
	pop	{r5, r6, pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #35
	sub	sp, #8
	bl 0x0200d72c
	cmp	r0, #0
	bne.n	.L_0200067c
	b.n	.L_02000cac
.L_0200067c:
	bl 0x0200d814
	movs	r0, #0
	bl 0x0200d984
	ldr	r2, [pc, #56]
	movs	r1, #5
	mov	r8, r2
	mov	r0, r8
	bl 0x0200d7cc
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d824
	mov	fp, r0
	cmp	r0, #0
	beq.n	.L_020006c4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d834
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r2, #54
	movs	r0, #4
	adds	r1, #2
	adds	r2, #255
	bl 0x0200d86c
	b.n	.L_02000ca8
	.2byte 0x2acd
	.2byte 0x0000
.L_020006c4:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200d734
	movs	r0, #252
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200d734
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #37
	bl 0x0200d73c
	movs	r0, #98
	adds	r0, #255
	bl 0x0200d73c
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200d73c
	movs	r1, #208
	movs	r0, #27
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #208
	movs	r0, #28
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d834
	movs	r1, #128
	movs	r2, #147
	lsls	r2, r2, #1
	movs	r0, #4
	lsls	r1, r1, #2
	bl 0x0200d86c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200d8ec
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200d95c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200d954
	movs	r0, #20
	bl 0x0200d964
	movs	r0, #20
	bl 0x0200d6b4
	movs	r0, #245
	bl 0x0200d7e4
	movs	r0, #4
	bl 0x0200d82c
	adds	r5, r0, #0
	ldr	r2, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #12
	movs	r0, #234
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	adds	r0, #255
	bl 0x0200d76c
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_020007f4
	ldr	r6, [r7, #80]
	mov	r2, fp
	strb	r2, [r6, #27]
	ldrb	r2, [r6, #5]
	movs	r3, #33
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	adds	r1, r7, #0
	movs	r2, #13
	adds	r1, #35
	negs	r2, r2
	ands	r3, r2
	ldrb	r2, [r1, #0]
	strb	r3, [r6, #9]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #85
	mov	r2, fp
	strb	r2, [r3, #0]
	adds	r2, r7, #0
	adds	r2, #92
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r7, #48]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #204
	movs	r1, #193
	str	r3, [r7, #52]
	lsls	r1, r1, #3
	movs	r0, #68
	bl 0x0200d6e4
	str	r0, [sp, #4]
	movs	r0, #246
	bl 0x0200d7d4
	ldr	r3, [sp, #4]
	movs	r2, #128
	lsls	r2, r2, #3
	ldrb	r0, [r6, #16]
	adds	r2, r3, r2
	movs	r1, #128
	str	r2, [sp, #0]
	bl 0x0200d70c
	movs	r0, #68
	bl 0x0200d6ec
.L_020007f4:
	movs	r1, #1
	movs	r0, #37
	bl 0x0200d8f4
	movs	r0, #37
	bl 0x0200d82c
	ldr	r3, [r5, #8]
	adds	r6, r0, #0
	str	r3, [r6, #8]
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r6, #12]
	adds	r2, r6, #0
	ldr	r3, [r5, #16]
	adds	r2, #85
	str	r3, [r6, #16]
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #204
	movs	r2, #204
	str	r3, [r6, #52]
	lsls	r2, r2, #7
	movs	r3, #128
	adds	r2, #102
	lsls	r3, r3, #6
	mov	r0, r8
	adds	r0, #1
	str	r2, [r6, #48]
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	mov	r9, r2
	bl 0x0200d8c4
	movs	r0, #36
	bl 0x0200d9f4
	cmp	r7, #0
	bne.n	.L_02000848
	b.n	.L_020009ca
.L_02000848:
	movs	r3, #85
	adds	r3, r3, r7
	mov	r2, fp
	mov	sl, r3
	strb	r2, [r3, #0]
	movs	r3, #153
	lsls	r3, r3, #8
	adds	r3, #153
	str	r3, [r7, #72]
	movs	r3, #204
	lsls	r3, r3, #8
	movs	r2, #128
	adds	r3, #204
	lsls	r2, r2, #12
	mov	r8, r3
	str	r3, [r7, #68]
	str	r2, [r7, #40]
	movs	r1, #128
	movs	r2, #128
	movs	r3, #147
	adds	r0, r7, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	lsls	r3, r3, #17
	bl 0x0200d784
	movs	r1, #128
	movs	r2, #1
	movs	r3, #147
	adds	r0, r6, #0
	lsls	r1, r1, #18
	negs	r2, r2
	lsls	r3, r3, #17
	bl 0x0200d784
	ldr	r3, [pc, #296]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #7
	bl 0x0200d894
	movs	r0, #1
	bl 0x0200d6b4
	mov	r1, r8
	movs	r0, #7
	mov	r2, r9
	bl 0x0200d834
	movs	r0, #7
	movs	r1, #12
	movs	r2, #0
	bl 0x0200d99c
	movs	r1, #12
	movs	r2, #0
	negs	r1, r1
	movs	r0, #4
	bl 0x0200d9a4
	movs	r0, #7
	bl 0x0200d87c
	movs	r0, #7
	bl 0x0200d84c
	movs	r0, #1
	bl 0x0200d6b4
	movs	r1, #1
	movs	r0, #7
	bl 0x0200d89c
	movs	r0, #1
	bl 0x0200d6b4
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x0200d8e4
	adds	r0, r7, #0
	bl 0x0200d78c
	movs	r3, #4
	mov	r8, r3
	mov	r2, r8
	mov	r3, sl
	strb	r2, [r3, #0]
	movs	r0, #90
	bl 0x0200d80c
	mov	r2, fp
	mov	r3, sl
	strb	r2, [r3, #0]
	movs	r1, #128
	movs	r2, #128
	movs	r3, #132
	adds	r0, r7, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	lsls	r3, r3, #17
	bl 0x0200d784
	movs	r1, #128
	movs	r2, #1
	movs	r3, #132
	lsls	r1, r1, #18
	adds	r0, r6, #0
	negs	r2, r2
	lsls	r3, r3, #17
	bl 0x0200d784
	mov	r2, r8
	mov	r3, sl
	strb	r2, [r3, #0]
	movs	r0, #60
	bl 0x0200d80c
	mov	r2, fp
	mov	r3, sl
	strb	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #24
	mov	r2, fp
	str	r3, [r6, #56]
	str	r3, [r6, #60]
	str	r3, [r6, #64]
	adds	r3, r6, #0
	str	r2, [r6, #8]
	str	r2, [r6, #12]
	str	r2, [r6, #16]
	str	r2, [r6, #36]
	str	r2, [r6, #40]
	str	r2, [r6, #44]
	adds	r3, #100
	mov	r2, fp
	strh	r2, [r3, #0]
	adds	r0, r7, #0
	bl 0x0200d78c
	ldr	r6, [pc, #60]
	mov	r3, r8
	mov	r2, sl
	strb	r3, [r2, #0]
	movs	r0, #90
	bl 0x0200d80c
	mov	r3, sl
	strb	r6, [r3, #0]
	movs	r1, #128
	movs	r2, #184
	movs	r3, #132
	lsls	r2, r2, #15
	lsls	r3, r3, #17
	adds	r0, r7, #0
	lsls	r1, r1, #18
	bl 0x0200d784
	adds	r0, r7, #0
	bl 0x0200d78c
	movs	r3, #128
	lsls	r3, r3, #24
	mov	r2, fp
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	adds	r3, r7, #0
	str	r2, [r7, #8]
	str	r2, [r7, #12]
	b.n	.L_020009bc
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x0240
	.2byte 0x0200
.L_020009bc:
	str	r2, [r7, #16]
	str	r2, [r7, #36]
	str	r2, [r7, #40]
	str	r2, [r7, #44]
	adds	r3, #100
	mov	r2, fp
	strh	r2, [r3, #0]
.L_020009ca:
	movs	r0, #40
	bl 0x0200d80c
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200d954
	movs	r0, #20
	bl 0x0200d964
	movs	r0, #20
	bl 0x0200d6b4
	movs	r2, #16
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x0200d99c
	movs	r2, #16
	negs	r2, r2
	movs	r1, #0
	movs	r0, #7
	bl 0x0200d99c
	ldr	r6, [pc, #700]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	ldr	r0, [r6, #0]
	bl 0x0200d87c
	ldr	r0, [r6, #0]
	movs	r1, #1
	bl 0x0200d89c
	movs	r1, #1
	movs	r0, #7
	bl 0x0200d89c
	movs	r0, #50
	bl 0x0200d80c
	movs	r0, #141
	bl 0x0200d9f4
	movs	r0, #30
	bl 0x0200d80c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200d7ac
	movs	r0, #30
	bl 0x0200d80c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200d7ac
	movs	r0, #30
	bl 0x0200d80c
	movs	r0, #0
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #1
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #2
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #3
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #4
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #5
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #7
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #6
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #30
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #31
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #32
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #192
	movs	r0, #128
	movs	r2, #132
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #18
	lsls	r1, r1, #15
	bl 0x0200d91c
	bl 0x0200d924
	movs	r0, #31
	movs	r1, #1
	bl 0x0200d8f4
	movs	r0, #32
	movs	r1, #1
	bl 0x0200d8f4
	ldr	r0, [r6, #0]
	movs	r1, #1
	bl 0x0200d8f4
	movs	r1, #1
	movs	r0, #7
	bl 0x0200d8f4
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #31
	bl 0x0200d82c
	movs	r1, #0
	bl 0x0200d7a4
	movs	r0, #31
	bl 0x0200d82c
	movs	r1, #7
	bl 0x0200d8bc
	movs	r0, #31
	bl 0x0200d82c
	ldr	r3, [pc, #312]
	movs	r2, #200
	lsls	r2, r2, #5
	str	r3, [r0, #28]
	adds	r2, #153
	mov	r9, r3
	adds	r3, r0, #0
	str	r2, [r0, #24]
	mov	sl, r2
	adds	r3, #85
	mov	r2, fp
	strb	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #18
	str	r3, [r0, #8]
	mov	r8, r3
	movs	r6, #132
	movs	r3, #128
	lsls	r6, r6, #17
	lsls	r3, r3, #16
	str	r3, [r0, #12]
	str	r6, [r0, #16]
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r7, #2
	orrs	r3, r7
	strb	r3, [r0, #0]
	movs	r0, #32
	bl 0x0200d82c
	movs	r1, #0
	bl 0x0200d7a4
	movs	r0, #32
	bl 0x0200d82c
	movs	r1, #7
	bl 0x0200d8bc
	movs	r0, #32
	bl 0x0200d82c
	mov	r3, sl
	str	r3, [r0, #24]
	mov	r2, r9
	adds	r3, r0, #0
	str	r2, [r0, #28]
	adds	r3, #85
	mov	r2, fp
	strb	r2, [r3, #0]
	mov	r3, r8
	str	r3, [r0, #8]
	movs	r3, #160
	lsls	r3, r3, #16
	str	r3, [r0, #12]
	str	r6, [r0, #16]
	adds	r0, #35
	ldrb	r3, [r0, #0]
	ldr	r1, [pc, #200]
	orrs	r3, r7
	strb	r3, [r0, #0]
	movs	r0, #31
	bl 0x0200d83c
	movs	r0, #32
	ldr	r1, [pc, #188]
	bl 0x0200d83c
	ldr	r6, [pc, #184]
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r6, #0
	bl 0x0200d6bc
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d884
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d884
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d884
	movs	r1, #0
	movs	r2, #0
	movs	r0, #24
	bl 0x0200d884
	movs	r0, #145
	bl 0x0200d9f4
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	bl 0x0200d7ac
	movs	r1, #0
	ldr	r0, [pc, #112]
	bl 0x0200d954
	movs	r0, #16
	bl 0x0200d964
	movs	r0, #16
	bl 0x0200d6b4
	movs	r0, #63
	bl 0x0200d9f4
	movs	r0, #180
	bl 0x0200d6b4
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200d954
	movs	r0, #60
	bl 0x0200d964
	movs	r0, #141
	bl 0x0200d9f4
	movs	r0, #60
	bl 0x0200d6b4
	adds	r0, r6, #0
	bl 0x0200d6c4
	movs	r0, #141
	lsls	r0, r0, #1
	bl 0x0200d734
	movs	r0, #2
	bl 0x0200d934
.L_02000ca8:
	bl 0x0200d81c
.L_02000cac:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xffff0000
	.4byte 0x0200e338
	.4byte 0x020084c9
	.2byte 0x63ff
	.2byte 0x0040
	push	{r5, r6, r7, lr}
	movs	r7, #192
	bl 0x0200d814
	lsls	r7, r7, #18
	movs	r0, #0
	bl 0x0200d984
	ldr	r2, [r7, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r6, #128
	adds	r3, r2, r1
	lsls	r6, r6, #1
	str	r6, [r3, #0]
	movs	r3, #218
	lsls	r3, r3, #1
	adds	r2, r2, r3
	movs	r3, #120
	str	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d91c
	bl 0x0200d92c
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #192
	movs	r0, #128
	movs	r2, #240
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	lsls	r0, r0, #18
	bl 0x0200d91c
	movs	r0, #1
	bl 0x0200d6b4
	bl 0x0200d77c
	movs	r0, #1
	bl 0x0200d6b4
	movs	r5, #192
	ldr	r0, [pc, #916]
	bl 0x0200d8c4
	lsls	r5, r5, #8
	movs	r1, #254
	movs	r2, #139
	adds	r3, r5, #0
	movs	r0, #4
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #133
	movs	r2, #139
	adds	r3, r5, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r0, #7
	bl 0x0200d88c
	movs	r0, #141
	bl 0x0200d9f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200d7ac
	bl 0x0200d96c
	bl 0x0200d97c
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200d7ac
	movs	r0, #60
	bl 0x0200d80c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200d7ac
	movs	r0, #60
	bl 0x0200d80c
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200d9f4
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200d7ac
	bl 0x0200d7b4
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	adds	r1, #204
	bl 0x0200d914
	movs	r0, #128
	movs	r1, #192
	movs	r2, #148
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200d91c
	bl 0x0200d924
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #7
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	bl 0x0200d8e4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x0200d8ec
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #4
	movs	r1, #0
	movs	r2, #8
	bl 0x0200d99c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #8
	bl 0x0200d99c
	movs	r2, #16
	movs	r0, #27
	movs	r1, #32
	negs	r2, r2
	bl 0x0200d99c
	movs	r2, #16
	movs	r1, #48
	negs	r2, r2
	movs	r0, #28
	bl 0x0200d99c
	movs	r0, #4
	bl 0x0200d87c
	movs	r0, #7
	bl 0x0200d87c
	movs	r0, #27
	bl 0x0200d87c
	movs	r0, #28
	bl 0x0200d87c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #160
	movs	r0, #27
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #176
	movs	r2, #0
	movs	r0, #28
	lsls	r1, r1, #8
	bl 0x0200d8e4
	movs	r1, #2
	movs	r0, #7
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #78
	bl 0x0200d9f4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #28
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #34
	bl 0x0200d9f4
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	adds	r1, r6, #0
	movs	r2, #0
	movs	r0, #7
	bl 0x0200d8fc
	adds	r1, r6, #0
	movs	r2, #30
	movs	r0, #4
	bl 0x0200d8fc
	movs	r0, #27
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r2, #0
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d8e4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #28
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #4
	lsls	r1, r1, #6
	bl 0x0200d8e4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #7
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #27
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200d8fc
	movs	r1, #2
	movs	r0, #28
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r1, #0
	adds	r0, #28
	bl 0x0200d8cc
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d824
	cmp	r0, #0
	bne.n	.L_02000fc4
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r1, #0
	movs	r2, #0
	adds	r0, #28
	bl 0x0200d8d4
	ldr	r2, [r7, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000fe0
.L_02000fc4:
	ldr	r2, [r7, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #5
	strh	r3, [r2, #0]
	adds	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
.L_02000fe0:
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200d8fc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200d8fc
	movs	r1, #128
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200d8e4
	movs	r1, #0
	movs	r0, #4
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #16
	movs	r2, #24
	movs	r0, #28
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d9a4
	movs	r1, #208
	movs	r0, #28
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200d8fc
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200d82c
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r1, #0
	movs	r2, #24
	movs	r0, #28
	bl 0x0200d874
	movs	r0, #28
	bl 0x0200d87c
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	movs	r1, #160
	strb	r3, [r0, #0]
	lsls	r1, r1, #8
	movs	r0, #28
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r0, [r3, #0]
	movs	r1, #5
	adds	r2, r0, #1
	lsls	r0, r0, #16
	strh	r2, [r3, #0]
	asrs	r0, r0, #16
	bl 0x0200d7cc
	movs	r0, #224
	bl 0x0200d7dc
	movs	r5, #0
	movs	r6, #216
	b.n	.L_020010d4
	.2byte 0x0000
	.2byte 0x2ace
	.2byte 0x0000
.L_020010d0:
	adds	r6, #2
	adds	r5, #1
.L_020010d4:
	cmp	r5, #14
	bgt.n	.L_020010f8
	movs	r0, #4
	bl 0x0200d724
	ldr	r3, [pc, #20]
	ldrh	r2, [r0, r6]
	ands	r3, r2
	cmp	r3, #224
	bne.n	.L_020010d0
	movs	r0, #4
	adds	r1, r5, #0
	bl 0x0200d7ec
	b.n	.L_020010f8
	.2byte 0x0000
	.2byte 0x01ff
	.2byte 0x0000
.L_020010f8:
	movs	r1, #2
	movs	r0, #4
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #4
	movs	r1, #0
	movs	r2, #8
	bl 0x0200d99c
	movs	r1, #24
	movs	r2, #8
	negs	r1, r1
	negs	r2, r2
	movs	r0, #27
	bl 0x0200d9a4
	movs	r0, #4
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #8
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r0, #4
	negs	r2, r2
	bl 0x0200d874
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #28
	bl 0x0200d8fc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #28
	bl 0x0200d8d4
	movs	r0, #4
	bl 0x0200d87c
	movs	r0, #4
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #10
	ands	r5, r3
	movs	r2, #30
	strb	r5, [r0, #0]
	adds	r1, #255
	movs	r0, #4
	bl 0x0200d8fc
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200d8fc
	movs	r1, #208
	movs	r0, #27
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #0
	adds	r0, #27
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #7
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #129
	movs	r0, #7
	lsls	r1, r1, #1
	bl 0x0200d904
	ldr	r3, [pc, #440]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #129
	ldr	r0, [r3, #0]
	lsls	r1, r1, #1
	bl 0x0200d904
	movs	r0, #30
	bl 0x0200d80c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #28
	bl 0x0200d8fc
	movs	r1, #176
	movs	r0, #28
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #0
	movs	r0, #28
	bl 0x0200d8cc
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d824
	cmp	r0, #0
	bne.n	.L_02001236
	movs	r1, #3
	movs	r0, #28
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #28
	movs	r2, #0
	movs	r1, #0
	bl 0x0200d8d4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001260
.L_02001236:
	movs	r1, #3
	movs	r0, #28
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	movs	r0, #28
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
.L_02001260:
	movs	r1, #2
	movs	r0, #7
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #27
	bl 0x0200d8fc
	movs	r1, #208
	movs	r0, #27
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200d8fc
	movs	r1, #208
	movs	r0, #28
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #28
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #4
	bl 0x0200d8fc
	movs	r1, #176
	movs	r0, #27
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #27
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #28
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #28
	movs	r2, #0
	movs	r1, #0
	bl 0x0200d8d4
	movs	r0, #4
	movs	r1, #2
	bl 0x0200d8a4
	movs	r1, #2
	movs	r0, #7
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #27
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	ldr	r3, [pc, #48]
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r5, [pc, #40]
	movs	r1, #10
	adds	r0, r5, #0
	bl 0x0200d944
	adds	r0, r5, #0
	movs	r1, #10
	bl 0x0200d94c
	movs	r0, #28
	adds	r0, #255
	bl 0x0200d734
	movs	r0, #13
	movs	r1, #5
	bl 0x0200d93c
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0109
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	bl 0x0200d814
	movs	r6, #160
	movs	r0, #0
	bl 0x0200d984
	lsls	r6, r6, #8
	movs	r1, #128
	movs	r2, #156
	adds	r3, r6, #0
	movs	r0, #4
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #132
	movs	r2, #152
	adds	r3, r6, #0
	movs	r0, #7
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r2, #128
	lsls	r2, r2, #6
	mov	r8, r2
	movs	r1, #252
	movs	r2, #152
	mov	r3, r8
	movs	r0, #27
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #130
	movs	r2, #144
	mov	r3, r8
	movs	r0, #28
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r3, #192
	movs	r1, #138
	movs	r2, #196
	lsls	r3, r3, #8
	lsls	r2, r2, #17
	movs	r0, #5
	lsls	r1, r1, #18
	bl 0x0200d88c
	movs	r0, #27
	movs	r1, #9
	bl 0x0200d89c
	movs	r0, #28
	movs	r1, #8
	bl 0x0200d89c
	movs	r0, #4
	movs	r1, #50
	bl 0x0200d89c
	movs	r0, #7
	movs	r1, #12
	bl 0x0200d89c
	movs	r0, #130
	movs	r1, #208
	movs	r2, #156
	movs	r3, #0
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	bl 0x0200d91c
	bl 0x0200d96c
	bl 0x0200d97c
	ldr	r0, [pc, #684]
	bl 0x0200d8c4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d8fc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200d8fc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #28
	bl 0x0200d8fc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200d8fc
	movs	r0, #4
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #7
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	ands	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #4
	bl 0x0200d82c
	movs	r5, #192
	lsls	r5, r5, #10
	str	r5, [r0, #40]
	movs	r0, #7
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #27
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #28
	bl 0x0200d82c
	movs	r1, #8
	str	r5, [r0, #40]
	movs	r2, #4
	movs	r0, #4
	bl 0x0200d874
	movs	r0, #7
	movs	r1, #8
	movs	r2, #4
	bl 0x0200d874
	movs	r1, #16
	movs	r2, #8
	movs	r0, #27
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d874
	movs	r1, #16
	movs	r2, #8
	negs	r2, r2
	negs	r1, r1
	movs	r0, #28
	bl 0x0200d874
	movs	r0, #4
	bl 0x0200d87c
	movs	r0, #4
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r5, #1
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #7
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #1
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #4
	bl 0x0200d90c
	movs	r1, #138
	movs	r2, #172
	movs	r0, #5
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200d8fc
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #7
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r2, #0
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #192
	movs	r0, #28
	lsls	r1, r1, #7
	bl 0x0200d8ec
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #28
	bl 0x0200d8fc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r2, #0
	mov	r1, r8
	movs	r0, #28
	bl 0x0200d8e4
	mov	r1, r8
	movs	r0, #27
	bl 0x0200d8ec
	adds	r1, r6, #0
	movs	r0, #5
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d8dc
	movs	r1, #8
	movs	r2, #24
	negs	r2, r2
	movs	r0, #5
	negs	r1, r1
	bl 0x0200d99c
	movs	r0, #4
	mov	r1, r8
	bl 0x0200d8ec
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #7
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #4
	adds	r1, r6, #0
	bl 0x0200d8ec
	adds	r1, r6, #0
	movs	r0, #5
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #16
	movs	r2, #8
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d874
	movs	r1, #16
	movs	r2, #8
	movs	r0, #7
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d874
	movs	r1, #16
	movs	r2, #8
	movs	r0, #5
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d874
	movs	r0, #27
	movs	r1, #16
	movs	r2, #8
	bl 0x0200d874
	movs	r2, #8
	movs	r1, #16
	movs	r0, #28
	bl 0x0200d874
	movs	r0, #4
	bl 0x0200d82c
	movs	r5, #128
	lsls	r5, r5, #11
	str	r5, [r0, #40]
	movs	r0, #7
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #5
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #27
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #28
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #4
	bl 0x0200d80c
	ldr	r5, [pc, #80]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #86
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_020016ae
	movs	r1, #10
	movs	r0, #4
	negs	r1, r1
	bl 0x0200d7f4
.L_020016ae:
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r5, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r5, [pc, #40]
	movs	r1, #11
	adds	r0, r5, #0
	bl 0x0200d944
	adds	r0, r5, #0
	movs	r1, #11
	bl 0x0200d94c
	movs	r0, #13
	movs	r1, #6
	bl 0x0200d93c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00002ae4
	.4byte 0x02000240
	.2byte 0x0109
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	bl 0x0200d814
	movs	r6, #160
	movs	r0, #0
	bl 0x0200d984
	lsls	r6, r6, #8
	movs	r1, #128
	movs	r2, #156
	adds	r3, r6, #0
	movs	r0, #4
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #132
	movs	r2, #152
	adds	r3, r6, #0
	movs	r0, #7
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r2, #128
	lsls	r2, r2, #6
	mov	r8, r2
	movs	r1, #252
	movs	r2, #152
	mov	r3, r8
	movs	r0, #27
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #130
	movs	r2, #144
	mov	r3, r8
	movs	r0, #28
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #132
	movs	r2, #160
	adds	r3, r6, #0
	movs	r0, #5
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r3, #192
	movs	r1, #136
	movs	r2, #192
	lsls	r3, r3, #8
	movs	r0, #6
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	mov	sl, r3
	bl 0x0200d88c
	movs	r0, #130
	movs	r1, #208
	movs	r2, #156
	movs	r3, #0
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	bl 0x0200d91c
	bl 0x0200d96c
	bl 0x0200d97c
	ldr	r0, [pc, #820]
	bl 0x0200d8c4
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d8d4
	movs	r0, #27
	movs	r1, #9
	bl 0x0200d89c
	movs	r0, #28
	movs	r1, #8
	bl 0x0200d89c
	movs	r0, #4
	movs	r1, #50
	bl 0x0200d89c
	movs	r0, #7
	movs	r1, #12
	bl 0x0200d89c
	movs	r0, #5
	movs	r1, #27
	bl 0x0200d89c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d8fc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200d8fc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d8fc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #28
	bl 0x0200d8fc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200d8fc
	movs	r0, #4
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #7
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #5
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	ands	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #4
	bl 0x0200d82c
	movs	r5, #192
	lsls	r5, r5, #10
	str	r5, [r0, #40]
	movs	r0, #7
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #5
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #27
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #28
	bl 0x0200d82c
	movs	r1, #8
	str	r5, [r0, #40]
	movs	r2, #4
	movs	r0, #4
	bl 0x0200d874
	movs	r0, #7
	movs	r1, #8
	movs	r2, #4
	bl 0x0200d874
	movs	r0, #5
	movs	r1, #8
	movs	r2, #4
	bl 0x0200d874
	movs	r1, #16
	movs	r2, #8
	movs	r0, #27
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d874
	movs	r1, #16
	movs	r2, #8
	negs	r2, r2
	negs	r1, r1
	movs	r0, #28
	bl 0x0200d874
	movs	r0, #4
	bl 0x0200d87c
	movs	r0, #4
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r5, #1
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #7
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #5
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #1
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #4
	bl 0x0200d90c
	movs	r1, #138
	movs	r2, #172
	movs	r0, #6
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200d8fc
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #2
	movs	r0, #5
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #28
	bl 0x0200d8fc
	movs	r2, #0
	movs	r0, #28
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #2
	movs	r0, #27
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #27
	bl 0x0200d8d4
	movs	r0, #10
	bl 0x0200d80c
	mov	r1, sl
	movs	r0, #6
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r2, #24
	negs	r2, r2
	movs	r1, #0
	movs	r0, #6
	bl 0x0200d99c
	movs	r0, #6
	bl 0x0200d87c
	mov	r1, r8
	movs	r0, #4
	bl 0x0200d8ec
	movs	r0, #6
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #7
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #4
	adds	r1, r6, #0
	bl 0x0200d8ec
	adds	r1, r6, #0
	movs	r0, #5
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #16
	movs	r2, #8
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d874
	movs	r1, #16
	movs	r2, #8
	movs	r0, #7
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d874
	movs	r1, #16
	movs	r2, #8
	movs	r0, #5
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d874
	movs	r1, #16
	movs	r2, #8
	movs	r0, #6
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d874
	movs	r0, #27
	movs	r1, #16
	movs	r2, #8
	bl 0x0200d874
	movs	r2, #8
	movs	r1, #16
	movs	r0, #28
	bl 0x0200d874
	movs	r0, #4
	bl 0x0200d82c
	movs	r5, #128
	lsls	r5, r5, #11
	str	r5, [r0, #40]
	movs	r0, #7
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #5
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #6
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #27
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #28
	bl 0x0200d82c
	str	r5, [r0, #40]
	movs	r0, #4
	bl 0x0200d80c
	ldr	r5, [pc, #88]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #86
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02001a82
	movs	r1, #10
	movs	r0, #4
	negs	r1, r1
	bl 0x0200d7f4
.L_02001a82:
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r5, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r5, [pc, #48]
	movs	r1, #12
	adds	r0, r5, #0
	bl 0x0200d944
	adds	r0, r5, #0
	movs	r1, #13
	bl 0x0200d94c
	movs	r0, #28
	adds	r0, #255
	bl 0x0200d73c
	movs	r0, #13
	movs	r1, #7
	bl 0x0200d93c
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.4byte 0x00002aea
	.4byte 0x02000240
	.2byte 0x0109
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl 0x0200d9c4
	adds	r0, r5, #0
	bl 0x0200d82c
	movs	r1, #2
	bl 0x0200d9dc
	movs	r0, #201
	bl 0x0200d9f4
	movs	r0, #60
	bl 0x0200d80c
	adds	r0, r5, #0
	bl 0x0200d82c
	movs	r1, #0
	bl 0x0200d9dc
	bl 0x0200d9d4
	bl 0x0200d9cc
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200d89c
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r9
	push	{r7}
	sub	sp, #4
	mov	r1, r9
	str	r1, [sp, #0]
	adds	r7, r0, #0
	bl 0x0200d7fc
	movs	r5, #0
	adds	r6, r0, #0
	cmp	r5, r6
	bge.n	.L_02001bb6
.L_02001b1e:
	ldr	r2, [pc, #160]
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldrb	r0, [r2, r3]
	bl 0x0200d724
	ldrh	r1, [r0, #52]
	mov	r9, r0
	mov	r2, r9
	strh	r1, [r2, #56]
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	lsls	r0, r1, #14
	bl 0x0200d69c
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_02001b4e
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_02001b4e
	adds	r3, r0, #0
.L_02001b4e:
	mov	r1, r9
	strh	r3, [r1, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02001b64
	movs	r2, #56
	ldrsh	r3, [r1, r2]
	cmp	r3, #0
	beq.n	.L_02001b64
	movs	r3, #1
	strh	r3, [r1, #20]
.L_02001b64:
	mov	r3, r9
	movs	r2, #58
	ldrsh	r0, [r3, r2]
	movs	r2, #54
	ldrsh	r1, [r3, r2]
	lsls	r0, r0, #14
	bl 0x0200d69c
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_02001b84
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_02001b84
	adds	r3, r0, #0
.L_02001b84:
	mov	r1, r9
	strh	r3, [r1, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02001b9a
	movs	r2, #58
	ldrsh	r3, [r1, r2]
	cmp	r3, #0
	beq.n	.L_02001b9a
	movs	r3, #1
	strh	r3, [r1, #22]
.L_02001b9a:
	cmp	r7, #1
	bne.n	.L_02001bb0
	movs	r3, #50
	adds	r3, #255
	add	r3, r9
	movs	r2, #0
	strb	r2, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #1
	add	r3, r9
	strb	r2, [r3, #0]
.L_02001bb0:
	adds	r5, #1
	cmp	r5, r6
	blt.n	.L_02001b1e
.L_02001bb6:
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	bl 0x0200d814
	movs	r0, #0
	bl 0x0200d984
	movs	r0, #0
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #1
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #1
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r6, #192
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #2
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	lsls	r6, r6, #8
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #3
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #4
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #5
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #7
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #6
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #30
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #252
	orrs	r5, r3
	movs	r2, #152
	movs	r3, #160
	strb	r5, [r0, #0]
	lsls	r3, r3, #8
	movs	r0, #4
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	mov	sl, r3
	bl 0x0200d88c
	movs	r1, #132
	movs	r2, #144
	mov	r3, sl
	movs	r0, #7
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r3, #128
	movs	r1, #244
	movs	r2, #148
	lsls	r3, r3, #6
	movs	r0, #27
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	mov	r8, r3
	bl 0x0200d88c
	movs	r1, #252
	movs	r2, #140
	mov	r3, r8
	movs	r0, #28
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #132
	movs	r2, #152
	mov	r3, sl
	movs	r0, #5
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #138
	movs	r2, #144
	adds	r3, r6, #0
	movs	r0, #6
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #240
	movs	r2, #180
	adds	r3, r6, #0
	movs	r0, #29
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #148
	movs	r2, #196
	lsls	r2, r2, #17
	movs	r3, #0
	movs	r0, #30
	lsls	r1, r1, #18
	bl 0x0200d88c
	movs	r0, #5
	movs	r1, #19
	bl 0x0200d89c
	movs	r0, #6
	movs	r1, #19
	bl 0x0200d89c
	movs	r0, #4
	movs	r1, #19
	bl 0x0200d89c
	movs	r1, #13
	movs	r0, #7
	bl 0x0200d89c
	movs	r0, #4
	bl 0x0200d82c
	ldr	r3, [r0, #24]
	negs	r3, r3
	str	r3, [r0, #24]
	movs	r0, #6
	bl 0x0200d82c
	ldr	r3, [r0, #24]
	movs	r1, #208
	negs	r3, r3
	str	r3, [r0, #24]
	movs	r2, #156
	movs	r0, #130
	movs	r3, #0
	lsls	r2, r2, #17
	lsls	r1, r1, #15
	lsls	r0, r0, #18
	bl 0x0200d91c
	bl 0x0200d96c
	bl 0x0200d97c
	ldr	r0, [pc, #1016]
	bl 0x0200d8c4
	movs	r0, #27
	movs	r1, #9
	bl 0x0200d89c
	movs	r0, #28
	movs	r1, #8
	bl 0x0200d89c
	movs	r1, #2
	movs	r0, #28
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r2, #0
	movs	r0, #28
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #224
	movs	r0, #27
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200d8fc
	movs	r2, #0
	movs	r0, #27
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #2
	movs	r0, #4
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	mov	r1, r8
	movs	r0, #28
	bl 0x0200d8ec
	mov	r1, r8
	movs	r0, #27
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #27
	bl 0x0200d8fc
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #28
	bl 0x0200d8fc
	movs	r0, #28
	movs	r1, #2
	bl 0x0200d8ac
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #27
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #28
	bl 0x0200d8fc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #29
	bl 0x0200d8d4
	movs	r0, #78
	bl 0x0200d9f4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #27
	lsls	r1, r1, #7
	bl 0x0200d8e4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #28
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #130
	movs	r1, #208
	movs	r2, #172
	movs	r3, #1
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	bl 0x0200d91c
	bl 0x0200d924
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #39
	bl 0x0200d9f4
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #27
	bl 0x0200d8fc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #29
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #28
	bl 0x0200d8fc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #29
	bl 0x0200d8fc
	movs	r2, #0
	movs	r0, #29
	movs	r1, #0
	bl 0x0200d8d4
	movs	r0, #28
	movs	r1, #1
	bl 0x0200d89c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #28
	lsls	r1, r1, #7
	bl 0x0200d8e4
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #27
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #29
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #29
	movs	r2, #0
	movs	r1, #0
	bl 0x0200d8d4
	movs	r0, #29
	movs	r1, #0
	bl 0x0200d8ec
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r2, #0
	movs	r0, #30
	movs	r1, #0
	bl 0x0200d8d4
	movs	r0, #28
	movs	r1, #2
	bl 0x0200d8a4
	movs	r0, #27
	movs	r1, #2
	bl 0x0200d8ac
	movs	r2, #0
	mov	r1, r8
	movs	r0, #28
	bl 0x0200d8e4
	mov	r1, r8
	movs	r0, #27
	bl 0x0200d8ec
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #29
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #29
	adds	r1, r6, #0
	bl 0x0200d8ec
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #28
	bl 0x0200d8fc
	movs	r1, #128
	movs	r0, #28
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #29
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #29
	movs	r2, #0
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r0, #28
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #224
	movs	r0, #27
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r0, #27
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #29
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r2, #0
	movs	r0, #29
	movs	r1, #0
	bl 0x0200d8d4
	movs	r0, #27
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #28
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #27
	movs	r1, #1
	bl 0x0200d90c
	ldr	r5, [pc, #316]
	movs	r0, #27
	adds	r1, r5, #0
	bl 0x0200d83c
	movs	r0, #30
	bl 0x0200d80c
	adds	r1, r5, #0
	movs	r0, #29
	bl 0x0200d83c
	adds	r1, r5, #0
	movs	r0, #28
	bl 0x0200d83c
	movs	r0, #29
	bl 0x0200d844
	movs	r0, #29
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #64
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #29
	bl 0x0200d82c
	ldr	r5, [pc, #232]
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	adds	r1, r5, #0
	movs	r0, #29
	bl 0x0200d83c
	movs	r0, #27
	bl 0x0200d844
	adds	r1, r5, #0
	movs	r0, #27
	bl 0x0200d83c
	movs	r0, #28
	bl 0x0200d844
	adds	r1, r5, #0
	movs	r0, #28
	bl 0x0200d83c
	movs	r0, #29
	bl 0x0200d844
	movs	r1, #56
	movs	r2, #128
	movs	r0, #29
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #29
	bl 0x0200d8e4
	movs	r0, #27
	bl 0x0200d844
	movs	r1, #166
	movs	r2, #133
	movs	r0, #27
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #27
	bl 0x0200d8e4
	movs	r0, #28
	bl 0x0200d844
	movs	r1, #145
	movs	r2, #133
	movs	r0, #28
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #28
	bl 0x0200d8e4
	movs	r0, #29
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #32
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #1
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r2, #1
	movs	r0, #1
	movs	r3, #0
	negs	r1, r1
	negs	r2, r2
	negs	r0, r0
	bl 0x0200d91c
	movs	r0, #78
	bl 0x0200d9f4
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #128
	movs	r2, #164
	adds	r3, r6, #0
	lsls	r1, r1, #18
	b.n	.L_0200215c
	.4byte 0x00002b1c
	.4byte 0x0200dff8
	.2byte 0xe088
	.2byte 0x0200
.L_0200215c:
	lsls	r2, r2, #17
	movs	r0, #30
	bl 0x0200d88c
	movs	r0, #0
	bl 0x0200ba6c
	bl 0x0200d98c
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #134
	movs	r2, #184
	adds	r3, r6, #0
	movs	r0, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #134
	movs	r2, #192
	adds	r3, r6, #0
	movs	r0, #1
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #138
	movs	r2, #184
	adds	r3, r6, #0
	movs	r0, #2
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #138
	movs	r2, #176
	adds	r3, r6, #0
	movs	r0, #3
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
	bl 0x0200d8d4
	movs	r0, #0
	movs	r1, #1
	bl 0x0200d90c
	movs	r1, #134
	movs	r2, #168
	movs	r0, #0
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200d864
	movs	r1, #134
	movs	r2, #176
	movs	r0, #1
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200d864
	movs	r1, #138
	movs	r2, #168
	movs	r0, #2
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200d864
	movs	r1, #138
	movs	r2, #160
	lsls	r2, r2, #1
	lsls	r1, r1, #2
	movs	r0, #3
	bl 0x0200d864
	movs	r0, #0
	bl 0x0200d87c
	movs	r0, #1
	bl 0x0200d87c
	movs	r0, #2
	bl 0x0200d87c
	movs	r0, #3
	bl 0x0200d87c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #30
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #30
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #2
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #2
	movs	r2, #0
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #3
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r2, #0
	movs	r0, #3
	movs	r1, #0
	bl 0x0200d8d4
	mov	r1, sl
	movs	r0, #0
	bl 0x0200d8ec
	movs	r0, #160
	movs	r1, #192
	movs	r2, #180
	movs	r3, #1
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200d91c
	bl 0x0200d924
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #128
	movs	r1, #192
	movs	r2, #176
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200d91c
	bl 0x0200d924
	movs	r0, #0
	bl 0x0200bc58
	movs	r0, #1
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #1
	movs	r2, #0
	movs	r1, #0
	bl 0x0200d8d4
	mov	r1, sl
	movs	r0, #2
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #2
	movs	r0, #2
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #3
	bl 0x0200d8fc
	movs	r2, #0
	movs	r0, #3
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #2
	movs	r0, #4
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	adds	r1, r6, #0
	movs	r0, #30
	bl 0x0200d8ec
	movs	r1, #2
	movs	r0, #30
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #30
	bl 0x0200d8d4
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #2
	movs	r0, #5
	bl 0x0200d8a4
	movs	r0, #5
	bl 0x0200d80c
	movs	r1, #2
	movs	r0, #7
	bl 0x0200d8a4
	movs	r0, #5
	bl 0x0200d80c
	movs	r1, #2
	movs	r0, #6
	bl 0x0200d8ac
	movs	r0, #30
	bl 0x0200d80c
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #3
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	mov	r1, sl
	movs	r0, #1
	bl 0x0200d8ec
	movs	r2, #0
	movs	r0, #1
	movs	r1, #0
	bl 0x0200d8d4
	mov	r1, r8
	movs	r0, #30
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r2, #0
	movs	r0, #30
	movs	r1, #0
	bl 0x0200d8d4
	adds	r1, r6, #0
	movs	r0, #30
	bl 0x0200d8ec
	movs	r1, #48
	movs	r0, #3
	negs	r1, r1
	movs	r2, #0
	bl 0x0200d9a4
	adds	r1, r6, #0
	movs	r2, #0
	movs	r0, #3
	bl 0x0200d8e4
	movs	r0, #3
	movs	r1, #4
	bl 0x02009ac4
	mov	r1, r8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d8e4
	movs	r0, #4
	bl 0x0200d82c
	ldr	r3, [r0, #24]
	movs	r1, #40
	negs	r3, r3
	str	r3, [r0, #24]
	movs	r2, #0
	movs	r0, #3
	bl 0x0200d9a4
	movs	r2, #16
	movs	r0, #3
	movs	r1, #8
	negs	r2, r2
	bl 0x0200d9a4
	adds	r1, r6, #0
	movs	r2, #0
	movs	r0, #3
	bl 0x0200d8e4
	movs	r0, #3
	movs	r1, #6
	bl 0x02009ac4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d8e4
	movs	r0, #6
	bl 0x0200d82c
	ldr	r3, [r0, #24]
	mov	r1, sl
	negs	r3, r3
	str	r3, [r0, #24]
	movs	r2, #0
	movs	r0, #3
	bl 0x0200d8e4
	movs	r0, #3
	movs	r1, #7
	bl 0x02009ac4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #3
	lsls	r1, r1, #8
	bl 0x0200d8e4
	movs	r0, #3
	movs	r1, #5
	bl 0x02009ac4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200d8e4
	movs	r0, #0
	movs	r1, #2
	bl 0x0200d8ac
	mov	r1, sl
	movs	r0, #0
	bl 0x0200d8ec
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	mov	r1, r8
	movs	r0, #30
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #30
	bl 0x0200d8fc
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #1
	bl 0x0200d8fc
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #2
	bl 0x0200d8fc
	movs	r1, #138
	movs	r2, #160
	lsls	r2, r2, #1
	movs	r0, #2
	lsls	r1, r1, #2
	bl 0x0200d864
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r0, #1
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #2
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #2
	movs	r1, #2
	bl 0x0200d8ac
	movs	r2, #0
	movs	r0, #2
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r1, #2
	movs	r0, #0
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #192
	movs	r0, #2
	lsls	r1, r1, #7
	bl 0x0200d8ec
	movs	r1, #2
	movs	r0, #2
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #5
	movs	r1, #6
	movs	r2, #0
	bl 0x0200d8b4
	movs	r2, #0
	movs	r1, #4
	movs	r0, #7
	bl 0x0200d8b4
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #30
	movs	r1, #0
	bl 0x0200d8ec
	movs	r0, #30
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #30
	movs	r1, #0
	bl 0x0200d8ec
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #192
	movs	r2, #0
	movs	r0, #3
	lsls	r1, r1, #7
	bl 0x0200d8e4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #0
	bl 0x0200d8ec
	movs	r0, #50
	bl 0x0200d80c
	movs	r0, #1
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	adds	r1, r6, #0
	movs	r0, #0
	bl 0x0200d8ec
	movs	r0, #50
	bl 0x0200d80c
	movs	r0, #3
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #0
	mov	r1, sl
	bl 0x0200d8ec
	movs	r0, #4
	mov	r1, r8
	bl 0x0200d8ec
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d824
	cmp	r0, #0
	bne.n	.L_02002678
	movs	r1, #3
	movs	r0, #0
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	b.n	.L_020026fe
.L_02002678:
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #0
	bl 0x0200d8fc
	movs	r0, #5
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #128
	adds	r3, #2
	strh	r3, [r2, #0]
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #5
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #2
	movs	r0, #5
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #0
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
.L_020026fe:
	movs	r0, #2
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #0
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_0200271e
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #2
	bl 0x0200d854
.L_0200271e:
	movs	r0, #3
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #0
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_0200273e
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #3
	bl 0x0200d854
.L_0200273e:
	movs	r0, #1
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #0
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_0200275e
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #1
	bl 0x0200d854
.L_0200275e:
	ldr	r5, [pc, #884]
	movs	r0, #2
	adds	r1, r5, #0
	bl 0x0200d83c
	adds	r1, r5, #0
	movs	r0, #3
	bl 0x0200d83c
	adds	r1, r5, #0
	movs	r0, #1
	bl 0x0200d83c
	movs	r0, #2
	bl 0x0200d844
	movs	r0, #3
	bl 0x0200d844
	movs	r0, #1
	bl 0x0200d844
	movs	r0, #0
	movs	r1, #1
	bl 0x0200d90c
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d9ac
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d9ac
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d9ac
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d9ac
	movs	r1, #0
	movs	r0, #30
	bl 0x0200d9ac
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #192
	movs	r2, #192
	movs	r0, #0
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d834
	movs	r1, #144
	lsls	r1, r1, #1
	movs	r2, #108
	adds	r2, #255
	adds	r1, #255
	movs	r0, #0
	bl 0x0200d86c
	movs	r0, #0
	bl 0x0200d87c
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r1, #2
	movs	r0, #0
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #159
	lsls	r1, r1, #1
	movs	r2, #179
	movs	r0, #0
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r1, #146
	movs	r2, #150
	movs	r0, #0
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r1, #146
	movs	r0, #0
	lsls	r1, r1, #2
	movs	r2, #252
	bl 0x0200d86c
	movs	r1, #154
	movs	r0, #0
	lsls	r1, r1, #2
	movs	r2, #247
	bl 0x0200d86c
	movs	r1, #181
	lsls	r1, r1, #1
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r1, #255
	movs	r0, #0
	bl 0x0200d86c
	movs	r0, #0
	bl 0x0200d82c
	movs	r6, #2
	adds	r0, #85
	strb	r6, [r0, #0]
	ldr	r1, [pc, #636]
	movs	r0, #0
	bl 0x0200d83c
	movs	r0, #0
	bl 0x0200d844
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #0
	bl 0x0200d8ec
	movs	r0, #30
	bl 0x0200d80c
	movs	r0, #0
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r0, #4
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #30
	bl 0x0200d8e4
	movs	r0, #1
	bl 0x0200ba6c
	movs	r0, #100
	bl 0x0200d80c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #30
	bl 0x0200d8e4
	movs	r0, #1
	bl 0x0200bc58
	movs	r0, #60
	bl 0x0200d80c
	movs	r1, #1
	movs	r0, #4
	bl 0x0200d90c
	bl 0x0200d924
	movs	r0, #60
	bl 0x0200d80c
	movs	r1, #0
	movs	r0, #4
	bl 0x0200d8ec
	movs	r0, #60
	bl 0x0200d80c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #7
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #4
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_02002980
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200d854
.L_02002980:
	movs	r0, #6
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #4
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_020029a0
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200d854
.L_020029a0:
	movs	r0, #5
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #4
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_020029c0
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200d854
.L_020029c0:
	movs	r0, #30
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #4
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_020029e0
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #30
	bl 0x0200d854
.L_020029e0:
	adds	r1, r5, #0
	movs	r0, #7
	bl 0x0200d83c
	adds	r1, r5, #0
	movs	r0, #6
	bl 0x0200d83c
	adds	r1, r5, #0
	movs	r0, #5
	bl 0x0200d83c
	adds	r1, r5, #0
	movs	r0, #30
	bl 0x0200d83c
	movs	r0, #7
	bl 0x0200d844
	movs	r0, #6
	bl 0x0200d844
	movs	r0, #5
	bl 0x0200d844
	movs	r0, #30
	bl 0x0200d844
	movs	r0, #4
	movs	r1, #1
	bl 0x0200d90c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #4
	negs	r1, r1
	bl 0x0200d9a4
	ldr	r1, [pc, #172]
	movs	r0, #4
	bl 0x0200d83c
	movs	r0, #4
	bl 0x0200d844
	movs	r0, #4
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #29
	bl 0x0200d82c
	adds	r0, #85
	strb	r6, [r0, #0]
	ldr	r1, [pc, #136]
	movs	r0, #4
	bl 0x0200d83c
	movs	r0, #4
	bl 0x0200d844
	movs	r1, #56
	movs	r2, #128
	lsls	r2, r2, #1
	movs	r0, #4
	adds	r1, #255
	bl 0x0200d86c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d91c
	movs	r0, #2
	bl 0x0200ba6c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #84
	str	r3, [r2, #0]
	subs	r3, #76
	adds	r2, r1, r3
	movs	r3, #24
	str	r3, [r2, #0]
	bl 0x0200d974
	bl 0x0200d97c
	movs	r0, #0
	mov	r9, sp
	bl 0x02009b04
	movs	r0, #1
	bl 0x0200d934
	bl 0x0200d81c
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0x0200dff0
	.4byte 0x0200e1e0
	.4byte 0x0200dff8
	.2byte 0xe088
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	bl 0x0200d814
	movs	r0, #0
	bl 0x0200d984
	movs	r0, #0
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #1
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #1
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r6, #128
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #2
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	lsls	r6, r6, #6
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #3
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #4
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #5
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #7
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #6
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #30
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #29
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #130
	orrs	r5, r3
	movs	r2, #152
	movs	r3, #160
	strb	r5, [r0, #0]
	lsls	r3, r3, #8
	movs	r0, #4
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	mov	r8, r3
	bl 0x0200d88c
	movs	r1, #134
	movs	r2, #144
	mov	r3, r8
	movs	r0, #7
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #240
	movs	r2, #148
	adds	r3, r6, #0
	movs	r0, #27
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #248
	movs	r2, #140
	adds	r3, r6, #0
	movs	r0, #28
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #136
	movs	r2, #152
	mov	r3, r8
	movs	r0, #5
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r5, #192
	bl 0x0200d88c
	lsls	r5, r5, #8
	movs	r1, #140
	movs	r2, #144
	adds	r3, r5, #0
	movs	r0, #6
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #240
	movs	r2, #180
	adds	r3, r5, #0
	movs	r0, #29
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #148
	movs	r2, #196
	lsls	r2, r2, #17
	movs	r3, #0
	movs	r0, #30
	lsls	r1, r1, #18
	bl 0x0200d88c
	movs	r0, #27
	movs	r1, #10
	bl 0x0200d89c
	movs	r0, #28
	movs	r1, #9
	bl 0x0200d89c
	movs	r0, #128
	adds	r1, r6, #0
	lsls	r0, r0, #9
	bl 0x0200d914
	movs	r0, #130
	movs	r1, #208
	movs	r2, #156
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #0
	lsls	r0, r0, #18
	bl 0x0200d91c
	bl 0x0200d96c
	bl 0x0200d97c
	ldr	r0, [pc, #376]
	bl 0x0200d8c4
	movs	r0, #7
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r0, #6
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #2
	movs	r0, #27
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r2, #0
	movs	r0, #27
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #2
	movs	r0, #28
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r2, #0
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d8e4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200d8ec
	movs	r0, #40
	bl 0x0200d80c
	movs	r2, #0
	mov	r1, r8
	movs	r0, #4
	bl 0x0200d8e4
	mov	r1, r8
	movs	r0, #5
	bl 0x0200d8ec
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #5
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #5
	movs	r2, #0
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #2
	movs	r0, #27
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #224
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200d8e4
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r0, #6
	bl 0x0200d8ec
	movs	r0, #30
	bl 0x0200d80c
	movs	r0, #28
	movs	r1, #2
	bl 0x0200d8ac
	movs	r1, #0
	movs	r0, #28
	bl 0x0200d8cc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d824
	cmp	r0, #0
	bne.n	.L_02002ddc
	movs	r2, #0
	movs	r0, #29
	movs	r1, #0
	bl 0x0200d8d4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002df8
	.2byte 0x0000
	.2byte 0x2af0
	.2byte 0x0000
.L_02002ddc:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #29
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
.L_02002df8:
	movs	r0, #78
	bl 0x0200d9f4
	movs	r0, #130
	movs	r1, #208
	movs	r2, #176
	movs	r3, #1
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	bl 0x0200d91c
	bl 0x0200d924
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #39
	bl 0x0200d9f4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200d8fc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #130
	movs	r1, #208
	movs	r2, #156
	movs	r3, #1
	lsls	r0, r0, #18
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x0200d91c
	movs	r1, #240
	movs	r2, #160
	movs	r0, #29
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r0, #29
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #29
	movs	r2, #0
	movs	r1, #0
	bl 0x0200d8d4
	movs	r0, #29
	movs	r1, #0
	bl 0x0200d8ec
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #0
	movs	r0, #7
	bl 0x0200d8fc
	movs	r1, #10
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d8fc
	movs	r1, #10
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d8fc
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #4
	bl 0x0200d8fc
	movs	r1, #192
	movs	r0, #29
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r2, #20
	negs	r2, r2
	movs	r0, #29
	movs	r1, #20
	bl 0x0200d9a4
	movs	r1, #128
	movs	r0, #29
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r0, #29
	movs	r1, #27
	bl 0x02009ac4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200d8fc
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #192
	movs	r0, #29
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r0, #29
	movs	r1, #28
	bl 0x02009ac4
	movs	r1, #20
	movs	r2, #20
	movs	r0, #29
	negs	r1, r1
	bl 0x0200d9a4
	movs	r0, #29
	movs	r1, #0
	bl 0x0200d8ec
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #29
	bl 0x0200d8fc
	movs	r2, #0
	movs	r0, #29
	movs	r1, #0
	bl 0x0200d8d4
	movs	r0, #27
	movs	r1, #2
	bl 0x0200d8a4
	movs	r1, #2
	movs	r0, #28
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200d8fc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #29
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #7
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #29
	bl 0x0200d8fc
	movs	r2, #0
	movs	r0, #29
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #128
	movs	r0, #27
	lsls	r1, r1, #7
	bl 0x0200d8ec
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #27
	bl 0x0200d8fc
	movs	r2, #0
	movs	r0, #27
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #128
	movs	r0, #28
	lsls	r1, r1, #7
	bl 0x0200d8ec
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #28
	bl 0x0200d8fc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #29
	bl 0x0200d8fc
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #224
	movs	r2, #0
	movs	r0, #27
	lsls	r1, r1, #8
	bl 0x0200d8e4
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r0, #28
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #28
	bl 0x0200d8fc
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #27
	bl 0x0200d8fc
	movs	r0, #29
	movs	r1, #0
	bl 0x0200d8ec
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #28
	bl 0x0200d8e4
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #29
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r2, #0
	movs	r0, #29
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #2
	movs	r0, #6
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #29
	bl 0x0200d8fc
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #224
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200d8e4
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r0, #6
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #29
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200d8e4
	movs	r1, #192
	movs	r0, #29
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r0, #29
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #28
	lsls	r1, r1, #7
	bl 0x0200d8e4
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #27
	bl 0x0200d8ec
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #28
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #27
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #27
	movs	r1, #1
	bl 0x0200d90c
	ldr	r5, [pc, #1020]
	movs	r0, #29
	adds	r1, r5, #0
	bl 0x0200d83c
	movs	r0, #30
	bl 0x0200d80c
	adds	r1, r5, #0
	movs	r0, #27
	bl 0x0200d83c
	movs	r0, #30
	bl 0x0200d80c
	adds	r1, r5, #0
	movs	r0, #28
	bl 0x0200d83c
	movs	r0, #29
	bl 0x0200d844
	movs	r0, #29
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #64
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #29
	bl 0x0200d82c
	ldr	r5, [pc, #928]
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	adds	r1, r5, #0
	movs	r0, #29
	bl 0x0200d83c
	movs	r0, #27
	bl 0x0200d844
	adds	r1, r5, #0
	movs	r0, #27
	bl 0x0200d83c
	movs	r0, #28
	bl 0x0200d844
	adds	r1, r5, #0
	movs	r0, #28
	bl 0x0200d83c
	movs	r0, #29
	bl 0x0200d844
	movs	r1, #56
	movs	r2, #128
	movs	r0, #29
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #29
	bl 0x0200d8e4
	movs	r0, #27
	bl 0x0200d844
	movs	r1, #166
	movs	r2, #133
	movs	r0, #27
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #27
	bl 0x0200d8e4
	movs	r0, #28
	bl 0x0200d844
	movs	r1, #145
	movs	r2, #133
	movs	r0, #28
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #28
	bl 0x0200d8e4
	movs	r0, #29
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #32
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #1
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r2, #1
	movs	r0, #1
	movs	r3, #0
	negs	r1, r1
	negs	r2, r2
	negs	r0, r0
	bl 0x0200d91c
	movs	r0, #78
	bl 0x0200d9f4
	movs	r5, #192
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	lsls	r5, r5, #8
	movs	r1, #128
	movs	r2, #164
	adds	r3, r5, #0
	movs	r0, #30
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #134
	movs	r2, #168
	adds	r3, r5, #0
	movs	r0, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #134
	movs	r2, #176
	adds	r3, r5, #0
	movs	r0, #1
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #138
	movs	r2, #176
	adds	r3, r5, #0
	movs	r0, #2
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #138
	movs	r2, #184
	adds	r3, r5, #0
	movs	r0, #3
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d88c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x0200d8e4
	movs	r0, #3
	bl 0x0200ba6c
	bl 0x0200d98c
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200d8fc
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #5
	bl 0x0200d8fc
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
	bl 0x0200d8ec
	adds	r1, r5, #0
	movs	r0, #1
	bl 0x0200d8ec
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #0
	bl 0x0200bc58
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #1
	bl 0x0200d8fc
	movs	r2, #0
	movs	r0, #1
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #160
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r1, #2
	movs	r0, #0
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #30
	bl 0x0200d8fc
	movs	r1, #128
	movs	r0, #30
	lsls	r1, r1, #6
	bl 0x0200d8ec
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #1
	bl 0x0200d8fc
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #2
	bl 0x0200d8fc
	movs	r1, #138
	movs	r2, #160
	lsls	r2, r2, #1
	movs	r0, #2
	lsls	r1, r1, #2
	bl 0x0200d864
	adds	r1, r5, #0
	movs	r0, #2
	bl 0x0200d8ec
	movs	r1, #192
	movs	r0, #2
	lsls	r1, r1, #7
	bl 0x0200d8ec
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r0, #2
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r0, #2
	movs	r1, #2
	bl 0x0200d8ac
	movs	r2, #0
	movs	r0, #2
	movs	r1, #0
	bl 0x0200d8d4
	movs	r0, #0
	movs	r1, #2
	bl 0x0200d8ac
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #0
	bl 0x0200d8ec
	movs	r0, #5
	bl 0x0200d80c
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #192
	movs	r0, #2
	lsls	r1, r1, #7
	bl 0x0200d8ec
	movs	r1, #2
	movs	r0, #2
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200d8e4
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #30
	movs	r1, #0
	bl 0x0200d8ec
	movs	r0, #30
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #30
	movs	r1, #0
	bl 0x0200d8ec
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200d8e4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #0
	bl 0x0200d8ec
	movs	r0, #60
	bl 0x0200d80c
	adds	r1, r5, #0
	movs	r0, #1
	bl 0x0200d8ec
	movs	r0, #3
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #1
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #0
	bl 0x0200d8ec
	movs	r0, #60
	bl 0x0200d80c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #7
	b.n	.L_0200354c
	.2byte 0x0000
	.4byte 0x0200dff8
	.2byte 0xe088
	.2byte 0x0200
.L_0200354c:
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	adds	r1, r5, #0
	movs	r0, #0
	bl 0x0200d8ec
	movs	r0, #30
	bl 0x0200d80c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	bl 0x0200d8ec
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d824
	cmp	r0, #0
	bne.n	.L_020035be
	movs	r1, #3
	movs	r0, #0
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #0
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	b.n	.L_02003644
.L_020035be:
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #0
	bl 0x0200d8fc
	movs	r0, #5
	movs	r1, #4
	bl 0x0200d89c
	movs	r0, #33
	bl 0x0200d80c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #128
	adds	r3, #2
	strh	r3, [r2, #0]
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d8d4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #5
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #2
	movs	r0, #5
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r0, #0
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
.L_02003644:
	movs	r0, #2
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #0
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_02003664
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #2
	bl 0x0200d854
.L_02003664:
	movs	r0, #3
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #0
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_02003684
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #3
	bl 0x0200d854
.L_02003684:
	movs	r0, #1
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #0
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_020036a4
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #1
	bl 0x0200d854
.L_020036a4:
	ldr	r5, [pc, #872]
	movs	r0, #2
	adds	r1, r5, #0
	bl 0x0200d83c
	adds	r1, r5, #0
	movs	r0, #3
	bl 0x0200d83c
	adds	r1, r5, #0
	movs	r0, #1
	bl 0x0200d83c
	movs	r0, #2
	bl 0x0200d844
	movs	r0, #3
	bl 0x0200d844
	movs	r0, #1
	bl 0x0200d844
	movs	r0, #0
	movs	r1, #1
	bl 0x0200d90c
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d9ac
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d9ac
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d9ac
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d9ac
	movs	r1, #0
	movs	r0, #30
	bl 0x0200d9ac
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #192
	movs	r2, #192
	movs	r0, #0
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d834
	movs	r1, #144
	lsls	r1, r1, #1
	movs	r2, #108
	adds	r2, #255
	movs	r0, #0
	adds	r1, #255
	bl 0x0200d86c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #0
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #2
	movs	r0, #0
	bl 0x0200d8ac
	movs	r0, #5
	bl 0x0200d80c
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8d4
	movs	r1, #159
	lsls	r1, r1, #1
	movs	r2, #179
	movs	r0, #0
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r1, #146
	movs	r2, #150
	movs	r0, #0
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200d86c
	movs	r1, #146
	movs	r0, #0
	lsls	r1, r1, #2
	movs	r2, #252
	bl 0x0200d86c
	movs	r1, #154
	movs	r0, #0
	lsls	r1, r1, #2
	movs	r2, #247
	bl 0x0200d86c
	movs	r1, #181
	lsls	r1, r1, #1
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r1, #255
	movs	r0, #0
	bl 0x0200d86c
	movs	r0, #0
	bl 0x0200d82c
	movs	r6, #2
	adds	r0, #85
	strb	r6, [r0, #0]
	ldr	r1, [pc, #628]
	movs	r0, #0
	bl 0x0200d83c
	movs	r0, #0
	bl 0x0200d844
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #0
	bl 0x0200d8ec
	movs	r0, #30
	bl 0x0200d80c
	movs	r0, #0
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r0, #4
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e4
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #30
	bl 0x0200d8e4
	movs	r0, #1
	bl 0x0200ba6c
	movs	r0, #100
	bl 0x0200d80c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d8e4
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #30
	bl 0x0200d8e4
	movs	r0, #1
	bl 0x0200bc58
	movs	r0, #60
	bl 0x0200d80c
	movs	r1, #1
	movs	r0, #4
	bl 0x0200d90c
	bl 0x0200d924
	movs	r0, #60
	bl 0x0200d80c
	movs	r1, #0
	movs	r0, #4
	bl 0x0200d8ec
	movs	r0, #60
	bl 0x0200d80c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #10
	bl 0x0200d80c
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	bl 0x0200d8ec
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d89c
	movs	r0, #20
	bl 0x0200d80c
	movs	r0, #7
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #4
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_020038c6
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200d854
.L_020038c6:
	movs	r0, #6
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #4
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_020038e6
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200d854
.L_020038e6:
	movs	r0, #5
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #4
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_02003906
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200d854
.L_02003906:
	movs	r0, #30
	movs	r1, #2
	bl 0x0200d89c
	movs	r0, #4
	bl 0x0200d82c
	cmp	r0, #0
	beq.n	.L_02003926
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #30
	bl 0x0200d854
.L_02003926:
	adds	r1, r5, #0
	movs	r0, #7
	bl 0x0200d83c
	adds	r1, r5, #0
	movs	r0, #6
	bl 0x0200d83c
	adds	r1, r5, #0
	movs	r0, #5
	bl 0x0200d83c
	adds	r1, r5, #0
	movs	r0, #30
	bl 0x0200d83c
	movs	r0, #7
	bl 0x0200d844
	movs	r0, #6
	bl 0x0200d844
	movs	r0, #5
	bl 0x0200d844
	movs	r0, #30
	bl 0x0200d844
	movs	r0, #4
	movs	r1, #1
	bl 0x0200d90c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #4
	negs	r1, r1
	bl 0x0200d9a4
	ldr	r1, [pc, #164]
	movs	r0, #4
	bl 0x0200d83c
	movs	r0, #4
	bl 0x0200d844
	movs	r0, #4
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #29
	bl 0x0200d82c
	adds	r0, #85
	strb	r6, [r0, #0]
	ldr	r1, [pc, #128]
	movs	r0, #4
	bl 0x0200d83c
	movs	r0, #4
	bl 0x0200d844
	movs	r1, #56
	movs	r2, #128
	lsls	r2, r2, #1
	movs	r0, #4
	adds	r1, #255
	bl 0x0200d86c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x0200d8ec
	movs	r0, #10
	bl 0x0200d80c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d91c
	movs	r0, #2
	bl 0x0200ba6c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #84
	str	r3, [r2, #0]
	subs	r3, #76
	adds	r2, r1, r3
	movs	r3, #24
	str	r3, [r2, #0]
	bl 0x0200d974
	bl 0x0200d97c
	movs	r0, #1
	bl 0x0200d934
	bl 0x0200d81c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200dff0
	.4byte 0x0200e1e0
	.4byte 0x0200dff8
	.2byte 0xe088
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe970
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r4, r1, #0
	ldr	r1, [r3, #108]
	asrs	r2, r4, #20
	movs	r3, #64
	adds	r5, r0, #0
	subs	r4, r3, r2
	movs	r0, #0
	adds	r1, #20
.L_02003a3e:
	ldmia	r1!, {r2}
	cmp	r2, #0
	beq.n	.L_02003a62
	ldr	r3, [r2, #8]
	ldr	r2, [r2, #16]
	asrs	r3, r3, #20
	subs	r3, #4
	asrs	r2, r2, #20
	cmp	r3, #4
	bhi.n	.L_02003a62
	adds	r3, r4, #0
	adds	r3, #8
	cmp	r3, r2
	bgt.n	.L_02003a62
	adds	r3, #3
	cmp	r2, r3
	bge.n	.L_02003a62
	stmia	r5!, {r0}
.L_02003a62:
	adds	r0, #1
	cmp	r0, #63
	bls.n	.L_02003a3e
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r1, #188
	movs	r2, #200
	lsls	r1, r1, #1
	lsls	r2, r2, #5
	adds	r1, r1, r3
	sub	sp, #8
	adds	r2, #153
	movs	r3, #0
	str	r2, [sp, #4]
	str	r3, [sp, #0]
	ldr	r2, [pc, #420]
	ldr	r3, [r1, #12]
	mov	r8, r0
	mov	fp, r1
	str	r3, [r2, #0]
	cmp	r0, #1
	beq.n	.L_02003aba
	cmp	r0, #1
	bgt.n	.L_02003aac
	cmp	r0, #0
	beq.n	.L_02003ab6
	b.n	.L_02003ac6
.L_02003aac:
	mov	r1, r8
	cmp	r1, #2
	beq.n	.L_02003ac0
	cmp	r1, #3
	bne.n	.L_02003ac6
.L_02003ab6:
	ldr	r2, [pc, #392]
	b.n	.L_02003ac8
.L_02003aba:
	ldr	r3, [pc, #392]
	mov	r9, r3
	b.n	.L_02003aca
.L_02003ac0:
	ldr	r1, [pc, #388]
	mov	r9, r1
	b.n	.L_02003aca
.L_02003ac6:
	ldr	r2, [pc, #388]
.L_02003ac8:
	mov	r9, r2
.L_02003aca:
	mov	r1, r9
	ldr	r3, [r1, #0]
	movs	r7, #0
	cmp	r3, #64
	beq.n	.L_02003afc
	mov	r5, r9
.L_02003ad6:
	ldr	r0, [r5, #0]
	bl 0x0200d82c
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #1
	ldmia	r5!, {r0}
	bl 0x0200d8f4
	ldr	r2, [sp, #0]
	adds	r7, #1
	adds	r2, #1
	str	r2, [sp, #0]
	cmp	r7, #4
	bhi.n	.L_02003afc
	ldr	r3, [r5, #0]
	cmp	r3, #64
	bne.n	.L_02003ad6
.L_02003afc:
	movs	r7, #0
.L_02003afe:
	ldr	r6, [pc, #336]
	lsls	r5, r7, #2
	ldr	r0, [r6, r5]
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	ldr	r0, [r6, r5]
	adds	r7, #1
	bl 0x0200d8f4
	cmp	r7, #5
	bls.n	.L_02003afe
	movs	r0, #223
	bl 0x0200d9f4
	movs	r7, #0
.L_02003b28:
	mov	r1, fp
	ldr	r3, [r1, #12]
	ldr	r2, [sp, #4]
	subs	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r1, [sp, #0]
	movs	r3, #0
	mov	sl, r3
	cmp	sl, r1
	bcs.n	.L_02003b68
	mov	r6, r9
.L_02003b3e:
	ldr	r0, [r6, #0]
	bl 0x0200d82c
	ldr	r3, [r0, #16]
	ldr	r2, [sp, #4]
	adds	r3, r3, r2
	str	r3, [r0, #16]
	ldr	r0, [r6, #0]
	bl 0x0200d82c
	adds	r5, r0, #0
	ldmia	r6!, {r0}
	bl 0x0200d82c
	ldr	r3, [r0, #16]
	str	r3, [r5, #64]
	ldr	r1, [sp, #0]
	movs	r3, #1
	add	sl, r3
	cmp	sl, r1
	bcc.n	.L_02003b3e
.L_02003b68:
	movs	r3, #3
	ands	r3, r7
	cmp	r3, #3
	bne.n	.L_02003b7c
	ldr	r2, [sp, #4]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r2, r2, r3
	str	r2, [sp, #4]
.L_02003b7c:
	ldr	r1, [sp, #4]
	ldr	r2, [pc, #212]
	cmp	r1, r2
	ble.n	.L_02003b8a
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [sp, #4]
.L_02003b8a:
	movs	r0, #1
	bl 0x0200d6b4
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_02003ba2
	cmp	r7, #40
	bne.n	.L_02003bbe
	movs	r0, #128
	movs	r1, #208
	movs	r2, #152
	b.n	.L_02003bb2
.L_02003ba2:
	mov	r2, r8
	cmp	r2, #1
	bne.n	.L_02003bc4
	cmp	r7, #40
	bne.n	.L_02003bbe
	movs	r0, #128
	movs	r1, #208
	movs	r2, #176
.L_02003bb2:
	lsls	r0, r0, #18
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x0200d91c
.L_02003bbe:
	cmp	r7, #120
	bne.n	.L_02003bda
	b.n	.L_02003be0
.L_02003bc4:
	mov	r3, r8
	cmp	r3, #3
	bne.n	.L_02003bda
	cmp	r7, #40
	bne.n	.L_02003bd6
	movs	r0, #0
	movs	r1, #1
	bl 0x0200d90c
.L_02003bd6:
	cmp	r7, #120
	beq.n	.L_02003be0
.L_02003bda:
	adds	r7, #1
	cmp	r7, #227
	bls.n	.L_02003b28
.L_02003be0:
	movs	r7, #0
.L_02003be2:
	ldr	r6, [pc, #108]
	lsls	r5, r7, #2
	ldr	r0, [r6, r5]
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #223
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r6, r5]
	adds	r7, #1
	bl 0x0200d8f4
	cmp	r7, #5
	bls.n	.L_02003be2
	ldr	r1, [sp, #0]
	movs	r7, #0
	cmp	r7, r1
	bcs.n	.L_02003c2c
	mov	r5, r9
.L_02003c0e:
	movs	r2, #0
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200d884
	ldmia	r5!, {r0}
	bl 0x0200d82c
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	ldr	r2, [sp, #0]
	adds	r7, #1
	cmp	r7, r2
	bcc.n	.L_02003c0e
.L_02003c2c:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200ea1c
	.4byte 0x0200d9fc
	.4byte 0x0200da0c
	.4byte 0x0200da14
	.4byte 0x0200da1c
	.4byte 0x0200e9dc
	.2byte 0x7fff
	.2byte 0x0001
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	str	r0, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r2, r2, r3
	mov	sl, r2
	ldr	r2, [pc, #280]
	movs	r3, #192
	lsls	r3, r3, #9
	mov	r9, r3
	ldr	r3, [r2, #0]
	movs	r4, #0
	mov	fp, r4
	mov	r8, r4
	cmp	r3, #64
	beq.n	.L_02003cac
	adds	r5, r2, #0
.L_02003c8e:
	ldmia	r5!, {r0}
	bl 0x0200d82c
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	add	fp, r3
	cmp	r4, #4
	bhi.n	.L_02003cac
	ldr	r3, [r5, #0]
	cmp	r3, #64
	bne.n	.L_02003c8e
.L_02003cac:
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L_02003d74
	movs	r0, #223
	bl 0x0200d9f4
	movs	r3, #0
	mov	r4, sl
	mov	r8, r3
	ldr	r3, [r4, #12]
	movs	r2, #192
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r4, #12]
	b.n	.L_02003d6c
.L_02003cca:
	movs	r7, #0
	cmp	r7, fp
	bcs.n	.L_02003cf8
	ldr	r6, [pc, #192]
.L_02003cd2:
	ldr	r0, [r6, #0]
	bl 0x0200d82c
	ldr	r3, [r0, #16]
	mov	r4, r9
	subs	r3, r3, r4
	str	r3, [r0, #16]
	adds	r7, #1
	ldr	r0, [r6, #0]
	bl 0x0200d82c
	adds	r5, r0, #0
	ldmia	r6!, {r0}
	bl 0x0200d82c
	ldr	r3, [r0, #16]
	str	r3, [r5, #64]
	cmp	r7, fp
	bcc.n	.L_02003cd2
.L_02003cf8:
	movs	r3, #3
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #3
	bne.n	.L_02003d24
	mov	r4, r9
	lsls	r3, r4, #4
	subs	r0, r3, r4
	mov	r4, sl
	ldr	r2, [r4, #12]
	ldr	r1, [pc, #136]
	lsls	r3, r0, #1
	adds	r3, r3, r2
	ldr	r2, [r1, #0]
	cmp	r2, r3
	bge.n	.L_02003d24
	adds	r3, r0, #0
	cmp	r3, #0
	bge.n	.L_02003d20
	adds	r3, #15
.L_02003d20:
	asrs	r3, r3, #4
	mov	r9, r3
.L_02003d24:
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #203
	cmp	r9, r2
	bgt.n	.L_02003d36
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #204
	mov	r9, r3
.L_02003d36:
	ldr	r4, [sp, #0]
	cmp	r4, #1
	bne.n	.L_02003d54
	mov	r2, r8
	cmp	r2, #40
	bne.n	.L_02003d54
	movs	r0, #144
	movs	r1, #208
	movs	r2, #184
	lsls	r0, r0, #17
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x0200d91c
.L_02003d54:
	movs	r0, #1
	bl 0x0200d6b4
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #119
	bhi.n	.L_02003d74
	mov	r2, sl
	ldr	r3, [r2, #12]
	add	r3, r9
	str	r3, [r2, #12]
.L_02003d6c:
	ldr	r2, [pc, #40]
	ldr	r2, [r2, #0]
	cmp	r3, r2
	blt.n	.L_02003cca
.L_02003d74:
	ldr	r3, [pc, #32]
	mov	r4, sl
	ldr	r3, [r3, #0]
	str	r3, [r4, #12]
	bl 0x0200d77c
	movs	r0, #2
	bl 0x0200d6b4
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200da20
	.2byte 0xea1c
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #129
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	adds	r2, #255
	str	r2, [r3, #0]
	movs	r0, #15
	movs	r1, #1
	bl 0x0200d9b4
	movs	r1, #1
	movs	r0, #16
	bl 0x0200d9b4
	ldr	r3, [pc, #332]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #17
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #128
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #18
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #19
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #21
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #22
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200d72c
	cmp	r0, #0
	bne.n	.L_02003e44
	bl 0x0200c644
	b.n	.L_02003e4e
.L_02003e44:
	movs	r0, #24
	bl 0x0200d82c
	movs	r3, #0
	str	r3, [r0, #24]
.L_02003e4e:
	ldr	r5, [pc, #192]
	movs	r3, #241
	lsls	r3, r3, #1
	adds	r6, r5, r3
	ldrh	r3, [r6, #0]
	movs	r1, #128
	subs	r3, #1
	lsls	r3, r3, #16
	lsls	r1, r1, #9
	cmp	r3, r1
	bls.n	.L_02003e6e
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d884
.L_02003e6e:
	movs	r0, #36
	bl 0x0200d82c
	movs	r1, #0
	bl 0x0200d7a4
	movs	r0, #36
	bl 0x0200d82c
	movs	r1, #0
	bl 0x0200d7a4
	movs	r1, #144
	ldr	r0, [pc, #136]
	lsls	r1, r1, #3
	bl 0x0200d6bc
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #9
	bne.n	.L_02003eb4
	bl 0x02008cd0
	ldr	r2, [pc, #120]
	movs	r1, #152
	lsls	r1, r1, #2
	adds	r3, r5, r1
	strh	r2, [r3, #0]
.L_02003ea6:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #98
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02003f0a
.L_02003eb4:
	cmp	r3, #10
	bne.n	.L_02003ec4
	movs	r0, #5
	bl 0x0200d804
	bl 0x02009388
	b.n	.L_02003f0a
.L_02003ec4:
	cmp	r3, #11
	bne.n	.L_02003ed4
	movs	r0, #6
	bl 0x0200d804
	bl 0x020096e8
	b.n	.L_02003f0a
.L_02003ed4:
	cmp	r3, #12
	bne.n	.L_02003ede
	bl 0x0200aae4
	b.n	.L_02003f0a
.L_02003ede:
	cmp	r3, #13
	bne.n	.L_02003ee8
	bl 0x02009bc4
	b.n	.L_02003f0a
.L_02003ee8:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200d72c
	cmp	r0, #0
	beq.n	.L_02003f0a
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d884
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d884
.L_02003f0a:
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x020085b9
	.2byte 0x00fb
	.2byte 0x0000
	push	{lr}
	movs	r0, #0
	bl 0x0200bf28
	movs	r0, #0
	pop	{pc}
	push	{lr}
	movs	r0, #20
	adds	r0, #255
.L_02003f2e:
	bl 0x0200d734
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #37
	bl 0x0200d72c
	cmp	r0, #0
	beq.n	.L_02003f50
	movs	r0, #98
	adds	r0, #255
	bl 0x0200d734
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200d734
.L_02003f50:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x60184b01
	.4byte 0x00004770
	.2byte 0xe9f8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #164]
	sub	sp, #32
	ldr	r0, [r3, #0]
	cmp	r0, #0
	bge.n	.L_02003f72
	adds	r0, #3
.L_02003f72:
	asrs	r0, r0, #2
	movs	r1, #5
	bl 0x0200d6a4
	ldr	r3, [pc, #148]
	mov	r8, r0
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02003fc6
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
	beq.n	.L_02003fa6
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r1
	cmp	r3, #153
	bne.n	.L_02004002
.L_02003fa6:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_02004002
	movs	r0, #175
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02004002
.L_02003fc6:
	movs	r5, #0
	movs	r6, #4
.L_02003fca:
	mov	r2, r8
	adds	r0, r2, r5
	movs	r1, #5
	mov	r7, sp
	bl 0x0200d6a4
	ldr	r3, [pc, #60]
	lsls	r0, r0, #1
	ldrh	r3, [r3, r6]
	adds	r5, #1
	strh	r3, [r7, r0]
	adds	r6, #2
	cmp	r5, #4
	ble.n	.L_02003fca
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
.L_02004002:
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200e9f4
	.4byte 0x0200e9f8
	.4byte 0x0200ea2c
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, r6, lr}
	ldr	r2, [pc, #88]
	movs	r3, #1
	adds	r6, r0, #0
	str	r3, [r2, #0]
	cmp	r6, #2
	beq.n	0x0200c040
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
	bl 0x0200d72c
	cmp	r0, #0
	bne.n	.L_02004050
	cmp	r6, #1
	bne.n	.L_02004062
.L_02004050:
	ldr	r3, [pc, #60]
	movs	r2, #0
	movs	r1, #144
	str	r2, [r3, #0]
	ldr	r0, [pc, #56]
	lsls	r1, r1, #3
	bl 0x0200d6bc
	b.n	.L_02004076
.L_02004062:
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
.L_02004076:
	pop	{r5, r6, pc}
	.4byte 0x0200e9f8
	.4byte 0x05000180
	.4byte 0x0200ea2c
	.4byte 0x03000730
	.4byte 0x0200ea4c
	.4byte 0x050001a0
	.4byte 0x0200e9f4
	.4byte 0x0200bf61
	.4byte 0x0200ea50
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, lr}
	bl 0x0200d82c
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
	bl 0x0200d794
	adds	r3, r0, #0
	asrs	r3, r3, #19
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	adds	r3, #6
	movs	r2, #0
	bl 0x0200d7c4
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #128
	bl 0x0200d9ec
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
	bl 0x0200d82c
	adds	r6, r0, #0
	ldr	r7, [r6, #104]
	bl 0x0200d814
	movs	r0, #0
	bl 0x0200d984
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
	bl 0x0200d7a4
	movs	r3, #99
	adds	r3, r3, r7
	mov	r8, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02004194
.L_0200414e:
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #176]
	str	r3, [r6, #8]
	ldr	r3, [r7, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r7, #16]
	str	r3, [r6, #16]
	cmp	r5, r2
	bgt.n	.L_0200416a
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r5, r5, r3
.L_0200416a:
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
	bl 0x0200d6b4
	mov	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_0200414e
.L_02004194:
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
	bl 0x0200d7a4
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	b.n	.L_0200420c
	.4byte 0x00000001
	.4byte 0x02000240
	.4byte 0x0003ffff
	.2byte 0x122c
	.2byte 0x0300
.L_0200420c:
	bl 0x0200d91c
	bl 0x0200d92c
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
	bl 0x0200d794
	ldr	r3, [r6, #16]
	adds	r2, r0, #0
	ldr	r1, [r6, #8]
	adds	r0, r7, #0
	bl 0x0200d784
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	movs	r5, #0
	cmp	r2, r3
	ble.n	.L_02004272
	b.n	.L_02004258
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02004258:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200d6b4
	cmp	r5, #59
	bgt.n	.L_02004272
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	cmp	r2, r3
	bgt.n	.L_02004258
.L_02004272:
	movs	r0, #127
	bl 0x0200d9f4
	ldr	r3, [r6, #40]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_02004292
.L_02004280:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200d6b4
	cmp	r5, #59
	bgt.n	.L_02004292
	ldr	r3, [r6, #40]
	cmp	r3, #0
	bne.n	.L_02004280
.L_02004292:
	adds	r0, r7, #0
	bl 0x0200d78c
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200d82c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200d90c
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
	bl 0x0200d81c
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
	bl 0x0200d82c
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r7, r0, #0
	cmp	r3, #0
	bne.n	.L_0200436a
	movs	r3, #173
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200436a
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200436a
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02004378
.L_0200436a:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d75c
	b.n	.L_020044b6
	.2byte 0x0240
	.2byte 0x0200
.L_02004378:
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200d75c
	adds	r3, r6, #0
	adds	r3, #100
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #31
	ands	r3, r2
	cmp	r3, #31
	bne.n	.L_02004398
	movs	r0, #231
	bl 0x0200d9f4
.L_02004398:
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
	bl 0x0200d9e4
	cmp	r0, #255
	beq.n	.L_0200449a
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl 0x0200d994
	ldr	r5, [r5, #0]
	movs	r3, #136
	lsls	r3, r3, #17
	cmp	r5, r3
	bgt.n	.L_0200449a
	ldr	r2, [pc, #172]
	cmp	r5, r2
	blt.n	.L_0200449a
	movs	r3, #98
	adds	r3, r3, r6
	mov	r9, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02004460
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #12]
	subs	r5, r2, r3
	cmp	r5, #0
	bge.n	.L_020043f8
	subs	r5, r3, r2
.L_020043f8:
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	adds	r0, #8
	adds	r1, #8
	mov	r8, r2
	bl 0x0200c2e0
	cmp	r0, #12
	bgt.n	.L_02004418
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r5, r3
	bge.n	.L_02004418
	movs	r2, #1
	mov	r8, r2
.L_02004418:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_02004460
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200d72c
	cmp	r0, #0
	bne.n	.L_02004460
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
.L_02004460:
	ldrh	r0, [r6, #6]
	bl 0x0200d6dc
	ldr	r1, [r6, #48]
	ldr	r5, [pc, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200d6d4
	ldr	r1, [r6, #48]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6933
	b.n	.L_02004494
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
.L_02004494:
	adds	r3, r3, r0
	str	r3, [r6, #16]
	b.n	.L_020044b6
.L_0200449a:
	adds	r3, r6, #0
	adds	r3, #99
	movs	r5, #0
	strb	r5, [r3, #0]
	ldr	r1, [pc, #32]
	adds	r0, r6, #0
	str	r5, [r6, #108]
	bl 0x0200d764
	movs	r0, #228
	bl 0x0200d9f4
	ldr	r3, [pc, #20]
	str	r5, [r3, #0]
.L_020044b6:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200e9fc
	.2byte 0xea28
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #222
	sub	sp, #68
	bl 0x0200d9f4
	ldrh	r0, [r5, #6]
	bl 0x0200d6dc
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
	bl 0x0200d6d4
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
	bl 0x0200d76c
	movs	r1, #2
	adds	r7, r0, #0
	bl 0x0200d754
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200d7a4
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
	b.n	.L_0200458c
	.4byte 0x00000000
	.4byte 0x0300021c
	.4byte 0x0200c30d
	.2byte 0x0000
	.2byte 0xfffa
.L_0200458c:
	.2byte 0x2300
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	str	r4, [sp, #12]
	bl 0x0200cbd4
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
	bne.n	.L_02004632
	add	r6, sp, #16
	movs	r3, #3
	str	r3, [r6, #0]
.L_020045c2:
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	movs	r3, #14
	str	r3, [r6, #4]
	bl 0x0200d6cc
	mov	r2, sl
	lsls	r3, r0, #3
	ldr	r2, [r2, #8]
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	subs	r3, #4
	lsls	r3, r3, #16
	mov	r8, r2
	add	r8, r3
	bl 0x0200d6cc
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
	bl 0x0200d6cc
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsrs	r0, r0, #16
	movs	r2, #160
	lsls	r2, r2, #11
	lsls	r0, r0, #16
	adds	r0, r0, r2
	movs	r1, #10
	bl 0x0200d69c
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
	bl 0x0200cbd4
.L_02004632:
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
	bl 0x0200d82c
	adds	r6, r0, #0
	movs	r0, #10
	bl 0x0200d82c
	adds	r7, r0, #0
	movs	r0, #23
	bl 0x0200d82c
	adds	r5, r0, #0
	ldr	r2, [r5, #80]
	movs	r1, #128
	mov	r8, r2
	movs	r2, #248
	movs	r0, #24
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200d884
	movs	r1, #128
	movs	r2, #248
	movs	r0, #23
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200d884
	movs	r1, #236
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d884
	movs	r1, #138
	movs	r2, #128
	lsls	r2, r2, #17
	movs	r0, #10
	lsls	r1, r1, #18
	bl 0x0200d884
	movs	r0, #24
	bl 0x0200d82c
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
	bl 0x0200d72c
	cmp	r0, #0
	beq.n	.L_02004720
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
	bl 0x0200d82c
	movs	r1, #4
	bl 0x0200d7bc
	movs	r0, #11
	bl 0x0200d82c
	movs	r1, #4
	bl 0x0200d7bc
.L_02004720:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200d72c
	cmp	r0, #0
	beq.n	.L_0200476a
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
	bl 0x0200d82c
	movs	r1, #4
	bl 0x0200d7bc
	movs	r0, #12
	bl 0x0200d82c
	movs	r1, #4
	bl 0x0200d7bc
.L_0200476a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x0200d72c
	cmp	r0, #0
	beq.n	.L_020047aa
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200d72c
	cmp	r0, #0
	beq.n	.L_020047aa
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
.L_020047aa:
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
	blt.n	.L_02004826
	b.n	.L_0200495a
.L_02004826:
	ldr	r2, [pc, #340]
	mov	r0, r9
	lsls	r3, r0, #2
	ldr	r5, [r2, r3]
	cmp	r5, #0
	bne.n	.L_02004834
	b.n	.L_0200494a
.L_02004834:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bne.n	.L_0200483c
	b.n	.L_0200494a
.L_0200483c:
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
	bne.n	.L_020048b2
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
	bhi.n	.L_0200494a
	movs	r2, #16
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_0200494a
	cmp	r4, #239
	bgt.n	.L_0200494a
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
	b.n	.L_020048ee
.L_020048b2:
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
	bhi.n	.L_0200494a
	movs	r2, #64
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_0200494a
	cmp	r4, #175
	bgt.n	.L_0200494a
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
.L_020048ee:
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
	bne.n	.L_0200492c
	adds	r0, r5, #0
	bl 0x0200d9bc
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
	b.n	.L_02004940
.L_0200492c:
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
.L_02004940:
	adds	r0, r6, #0
	mov	r1, fp
	bl 0x0200d71c
	adds	r6, #12
.L_0200494a:
	ldr	r3, [pc, #44]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	add	r9, r1
	cmp	r9, r3
	bge.n	.L_0200495a
	b.n	.L_02004826
.L_0200495a:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffff0000
	.4byte 0x0200ea6c
	.4byte 0x020036e0
	.4byte 0x0200eab0
	.4byte 0x0200ea6e
	.4byte 0x0200ea70
	.4byte 0x0200eb70
	.4byte 0x40002000
	.4byte 0xc000a000
	.2byte 0xeb72
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200d6f4
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200d704
	ldr	r5, [pc, #76]
	bl 0x0200d714
	movs	r1, #192
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200d70c
	adds	r0, r6, #0
	bl 0x0200d6fc
	movs	r1, #144
	lsls	r1, r1, #3
.L_020049ce:
	ldr	r0, [pc, #48]
	bl 0x0200d6bc
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
	.4byte 0x0200ea70
	.4byte 0x0200da24
	.4byte 0x0200ea6c
	.4byte 0x0200c7d5
	.4byte 0x0200ea6e
	.4byte 0x0200eb70
	.2byte 0xeb72
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200d6f4
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200d704
	ldr	r5, [pc, #76]
	bl 0x0200d714
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200d70c
	adds	r0, r6, #0
	bl 0x0200d6fc
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200d6bc
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
	.4byte 0x0200ea70
	.4byte 0x0200db87
	.4byte 0x0200ea6c
	.4byte 0x0200c7d5
	.4byte 0x0200ea6e
	.4byte 0x0200eb70
	.2byte 0xeb72
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	bl 0x0200d6f4
	ldr	r3, [pc, #88]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #84]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #80]
	bl 0x0200d704
	ldr	r5, [pc, #80]
	bl 0x0200d714
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200d70c
	adds	r0, r6, #0
	bl 0x0200d6fc
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	bl 0x0200d6bc
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	b.n	.L_02004b14
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200ea70
	.4byte 0x0200ddb6
	.4byte 0x0200ea6c
	.4byte 0x0200c7d5
	.4byte 0x0200ea6e
	.4byte 0x0200eb70
	.2byte 0xeb72
	.2byte 0x0200
.L_02004b14:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	bl 0x0200d82c
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L_02004b3e
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
.L_02004b3e:
	pop	{r5, pc}
	.4byte 0x0200ea6e
	.4byte 0x0200ea70
	.4byte 0x80184b01
	.4byte 0x00004770
	.2byte 0xeb72
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02004b98
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
.L_02004b6a:
	beq.n	.L_02004b98
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
.L_02004b98:
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
	bl 0x0200d82c
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02004c1c
	cmp	r7, #0
	beq.n	.L_02004c1c
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02004c24
.L_02004c1c:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02004c24:
	mov	r3, sl
	bl 0x0200d76c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02004c32
	b.n	.L_02004d7e
.L_02004c32:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200d754
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200d764
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d7a4
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
	bl 0x0200cb54
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
	beq.n	.L_02004d7e
	cmp	r7, #0
	beq.n	.L_02004d7e
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02004cb4
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200d8bc
.L_02004cb4:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02004cd4
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x0200cb54
.L_02004cd4:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02004ce8
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02004ce8:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02004d2e
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_02004d16
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200d69c
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_02004d28
.L_02004d16:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200d69c
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_02004d28:
	bl 0x0200d69c
	str	r0, [r6, #52]
.L_02004d2e:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02004d4a
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200d754
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200d764
.L_02004d4a:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02004d5c
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02004d5c:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02004d6e
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02004d6e:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02004d7e
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02004d7e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200ea10
	.4byte 0x0200cb9d
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
	beq.n	.L_02004ea8
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
	bl 0x0200d82c
	mov	r1, r8
	ldr	r3, [r0, #8]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	ldr	r4, [sp, #0]
	cmp	r3, r2
	bne.n	.L_02004de8
	ldr	r3, [r0, #16]
	movs	r5, #2
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_02004df0
.L_02004de8:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r4, #12]
.L_02004df0:
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	movs	r2, #1
	negs	r2, r2
	ldr	r1, [pc, #192]
	cmp	r3, r2
	beq.n	.L_02004ea8
	movs	r5, #14
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L_02004ea8
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
	bl 0x0200d794
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
	bhi.n	.L_02004ea8
	movs	r0, #15
	negs	r0, r0
	cmp	r2, r0
	blt.n	.L_02004ea8
	cmp	r2, #239
	bgt.n	.L_02004ea8
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
	bl 0x0200d71c
.L_02004ea8:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200eb74
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
	bge.n	.L_0200504c
.L_02004f7c:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #36]
	ldr	r5, [sp, #24]
	lsls	r3, r1, #9
	adds	r2, r2, r3
	movs	r3, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, r5
	bge.n	.L_02005040
.L_02004f90:
	mov	r0, fp
	ldrb	r5, [r0, #2]
	cmp	r5, #0
	beq.n	.L_02005030
	ldr	r1, [sp, #44]
	cmp	r5, r1
	bcc.n	.L_02005030
	adds	r1, #1
	mov	sl, r1
	cmp	r5, sl
	bhi.n	.L_02005030
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
	bl 0x0200d72c
	cmp	r0, #0
	bne.n	.L_02004fe4
	cmp	r5, sl
	bne.n	.L_02005022
	mov	r3, r9
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	bl 0x0200d734
	b.n	.L_02005022
.L_02004fe4:
	mov	r1, r9
	movs	r5, #4
	ldrsh	r0, [r1, r5]
	bl 0x0200d72c
	cmp	r0, #0
	beq.n	.L_02005022
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
	bl 0x0200d79c
.L_02005022:
	mov	r0, r8
	ldrh	r3, [r0, #10]
	mov	r1, r8
	adds	r3, #1
	strh	r3, [r1, #10]
	movs	r5, #8
	add	r9, r5
.L_02005030:
	ldr	r2, [sp, #16]
	ldr	r5, [sp, #24]
	adds	r2, #1
	movs	r3, #4
	str	r2, [sp, #16]
	add	fp, r3
	cmp	r2, r5
	blt.n	.L_02004f90
.L_02005040:
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #12]
	cmp	r0, r1
	blt.n	.L_02004f7c
.L_0200504c:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200d72c
	cmp	r0, #0
	beq.n	.L_020050a4
	ldr	r3, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d82c
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
	bge.n	.L_020050a4
.L_0200507e:
	mov	r0, r9
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	cmp	r3, r4
	bne.n	.L_02005094
	movs	r5, #2
	ldrsh	r3, [r0, r5]
	cmp	r3, r1
	bne.n	.L_02005094
	mov	r0, r8
	strh	r2, [r0, #12]
.L_02005094:
	movs	r3, #8
	mov	r0, r8
	add	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	adds	r2, #1
	cmp	r2, r3
	blt.n	.L_0200507e
.L_020050a4:
	movs	r0, #128
	lsls	r0, r0, #1
	bl 0x0200d6f4
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r2, #63
.L_020050b2:
	ldr	r3, [pc, #80]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bge.n	.L_020050b2
	bl 0x0200d714
	mov	r1, r8
	strh	r0, [r1, #16]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #1
	asrs	r0, r0, #16
	bl 0x0200d70c
	adds	r0, r5, #0
	bl 0x0200d6fc
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200d6bc
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
	.4byte 0x0200eb74
	.4byte 0x03000258
	.4byte 0x02000240
	.4byte 0x11111111
	.2byte 0xcd9d
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
	bl 0x0200d82c
	mov	r1, r8
	ldr	r5, [r0, #8]
	ldr	r6, [r0, #16]
	mov	sl, r0
	movs	r2, #128
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200d834
	asrs	r5, r5, #20
	mov	r2, r8
	asrs	r6, r6, #20
	ldr	r0, [r2, #0]
	lsls	r1, r5, #4
	lsls	r2, r6, #4
	adds	r1, #8
	adds	r2, #8
	bl 0x0200d85c
	movs	r0, #1
	bl 0x0200d6b4
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
	bl 0x0200d774
	movs	r0, #4
	bl 0x0200d80c
	bl 0x0200d92c
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
	bl 0x0200d82c
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
	bl 0x0200d10c
	movs	r0, #161
	bl 0x0200d9f4
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
	bl 0x0200d79c
	movs	r0, #12
	bl 0x0200d80c
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
	bl 0x0200d82c
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
	bl 0x0200d10c
	movs	r0, #229
	bl 0x0200d9f4
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
	bl 0x0200d79c
	movs	r0, #12
	bl 0x0200d80c
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	bl 0x0200d82c
	movs	r1, #0
	bl 0x0200d7a4
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
	b.n	.L_020052b8
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00008000
	.2byte 0x0240
	.2byte 0x0200
.L_020052b8:
	movs	r3, #1
	mov	r1, r8
	strh	r3, [r1, #14]
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl 0x0200d89c
	movs	r0, #16
	bl 0x0200d80c
.L_020052cc:
	cmp	r7, #5
	bne.n	.L_020052d6
	movs	r0, #204
	bl 0x0200d9f4
.L_020052d6:
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
	bl 0x0200d6b4
	cmp	r7, #39
	ble.n	.L_020052cc
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d82c
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
	bl 0x0200d974
	bl 0x0200d97c
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
	bl 0x0200d69c
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02005374
	adds	r3, #15
.L_02005374:
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
	bl 0x0200d82c
	adds	r7, r0, #0
	bl 0x0200d814
	movs	r0, #0
	bl 0x0200d984
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d91c
	bl 0x0200d77c
	movs	r0, #1
	bl 0x0200d6b4
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
	bl 0x0200d96c
	bl 0x0200d97c
	movs	r0, #204
	bl 0x0200d9f4
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200d80c
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
.L_02005436:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200d6dc
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200d6d4
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200d6cc
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #188]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200d6cc
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
	bl 0x0200cbd4
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_02005436
	movs	r0, #188
	bl 0x0200d9f4
	ldr	r5, [pc, #112]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200d904
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200d89c
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200d7ac
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200d7ac
	bl 0x0200d7b4
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200d904
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200d80c
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200d89c
	bl 0x0200d81c
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200d345
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
	bl 0x0200d82c
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
	bge.n	.L_020055d0
.L_02005568:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, r8
	bne.n	.L_020055c4
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	cmp	r3, sl
	bne.n	.L_020055c4
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	bl 0x0200d72c
	cmp	r0, #0
	bne.n	.L_02005598
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x0200d194
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	bl 0x0200d734
	strh	r7, [r6, #12]
	b.n	.L_020055d0
.L_02005598:
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r7, r3
	beq.n	.L_020055d0
	adds	r0, r6, #0
	adds	r1, r5, #0
	strh	r7, [r6, #12]
	bl 0x0200d204
	movs	r2, #2
	ldrsh	r0, [r6, r2]
	mov	r1, r8
	bl 0x0200d74c
	movs	r3, #2
	ldrsh	r0, [r6, r3]
.L_020055b8:
	mov	r1, sl
	adds	r0, #8
	bl 0x0200d74c
	movs	r0, #1
	b.n	.L_020055d2
.L_020055c4:
	lsls	r3, r2, #16
	adds	r7, #1
	asrs	r3, r3, #16
	adds	r5, #8
	cmp	r7, r3
	blt.n	.L_02005568
.L_020055d0:
	movs	r0, #0
.L_020055d2:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xeb74
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
	bl 0x0200d82c
	movs	r3, #192
	ldr	r5, [pc, #140]
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	mov	sl, r3
	bl 0x0200d744
	adds	r7, r0, #0
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	adds	r0, #8
	bl 0x0200d744
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_02005632
	cmp	r0, #0
	beq.n	.L_02005686
.L_02005632:
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #0
	bl 0x0200d74c
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	movs	r1, #0
	adds	r0, #8
	bl 0x0200d74c
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
	bl 0x0200d77c
	bl 0x0200d39c
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	str	r2, [r3, #0]
.L_02005686:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200eb74
	.irp EntryTarget, 0x03000528, 0x03000508, 0x03000514, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000121, 0x08000141, 0x08000151, 0x08000169, 0x08000179, 0x080001a9, 0x080001c9, 0x080001d1, 0x080001e9, 0x080003c1, 0x080003c9, 0x080003d1, 0x080003d9, 0x080003e9, 0x080003f1, 0x08020091, 0x08020099, 0x080200a9, 0x080200c1, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x080201c1, 0x080201e1, 0x08020219, 0x08020229, 0x08020231, 0x08020279, 0x08020361, 0x08038041, 0x08038249, 0x080ad029, 0x080ad041, 0x080ad049, 0x080ad0c1, 0x080ad0f1, 0x080ad0f9, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80b1, 0x080c80c1, 0x080c80c9, 0x080c80d1, 0x080c80d9, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8101, 0x080c8111, 0x080c8119, 0x080c8141, 0x080c8149, 0x080c8161, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8259, 0x080c8279, 0x080c8281, 0x080c8291, 0x080c8299, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c8571, 0x080c85c1, 0x080c85f1, 0x080c85f9, 0x080c8601, 0x080c8781, 0x080c87e9, 0x080c8809, 0x080c8811, 0x080c8819, 0x080c8821, 0x080c8831, 0x080c8849, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x0000001d
	.4byte 0x0000001b
	.4byte 0x0000001c
	.4byte 0x00000040
	.4byte 0x00000000
	.4byte 0x00000040
	.4byte 0x00000004
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
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
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001200
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001200
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffffee00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffffee00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffffee00
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffffee00
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001200
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001200
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01e00000
	.4byte 0x00480000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01c80000
	.4byte 0x00480000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00480000
	.4byte 0x01000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00480000
	.4byte 0x01000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01990000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00038000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000004
	.4byte 0x01990000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x02008051
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x02008051
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x02008051
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00200000
	.4byte 0x01160000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x02008051
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00038000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000004
	.4byte 0x02880000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x02008051
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02a90000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x02008051
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02c80000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x02008051
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02c80000
	.4byte 0x00200000
	.4byte 0x01080000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x02008051
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000008c
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffa00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000600
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0020000
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
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
	.4byte 0x00000109
	.4byte 0x001050fb
	.4byte 0x00248002
	.4byte 0x00b01103
	.4byte 0x00c02103
	.4byte 0x00d03103
	.4byte 0x00e04106
	.4byte 0x000001ff
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0003
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
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01020000
	.4byte 0x0a210153
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00280000
	.4byte 0x01000000
	.4byte 0x00028000
	.4byte 0x0a210153
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00280000
	.4byte 0x01000000
	.4byte 0x01020000
	.4byte 0xffff0154
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00020000
	.4byte 0xffff0154
	.4byte 0x00000001
	.4byte 0x03700000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01028000
	.4byte 0xffff01ac
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff01ac
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x01580000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02c80000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0x0a2101a9
	.4byte 0x0000000b
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00020000
	.4byte 0xffff01a9
	.4byte 0x0000000b
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01020000
	.4byte 0xffff01aa
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01aa
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0x1a230021
	.4byte 0x00000006
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x1a230017
	.4byte 0x00000006
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x01500000
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
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x1a21012f
	.4byte 0x0200e41c
	.4byte 0x02000000
	.4byte 0x00080000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00016666
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00016666
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000026
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000008
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0x02000064
	.4byte 0x0200845d
	.4byte 0x00000002
	.4byte 0x0a210065
	.4byte 0x0200865d
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00002ac9
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00002aca
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00002acb
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00002acc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000012
	.4byte 0x00000013
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000016
	.4byte 0xffffffff
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x0200de98
	.4byte 0x0200ded4
	.4byte 0x0200df10
