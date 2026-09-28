.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008315, 0x02008151, 0x0200815d, 0x02008165, 0x020081d1, 0x02008159, 0x02008451
	overlay_veneer \EntryTarget
	.endr
	push	{r5, r6, lr}
	adds	r6, r0, #0
	adds	r5, r6, #0
	adds	r5, #98
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0200004e
	adds	r3, #255
	strb	r3, [r5, #0]
	b.n	.L_02000064
.L_0200004e:
	bl 0x0200d514
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #4
	lsrs	r3, r3, #16
	adds	r3, #60
	strb	r3, [r5, #0]
	bl 0x0200d514
	strh	r0, [r6, #6]
.L_02000064:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #1
	ldr	r3, [r3, #0]
	lsrs	r3, r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000080
	movs	r1, #15
	bl 0x0200d6d4
	b.n	.L_02000086
.L_02000080:
	movs	r1, #0
	bl 0x0200d6d4
.L_02000086:
	pop	{pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldr	r3, [r5, #24]
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r3, r2
	ble.n	.L_020000a8
	ldr	r2, [pc, #64]
	adds	r3, r3, r2
	str	r3, [r5, #24]
	ldr	r2, [pc, #60]
	ldr	r3, [r5, #28]
	adds	r3, r3, r2
	str	r3, [r5, #28]
.L_020000a8:
	adds	r6, r5, #0
	adds	r6, #98
	ldrb	r2, [r6, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_020000ba
	adds	r3, #255
	strb	r3, [r6, #0]
	b.n	.L_020000da
.L_020000ba:
	bl 0x0200d514
	lsls	r3, r0, #2
	ldrb	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #4
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	movs	r3, #144
	lsls	r3, r3, #9
	adds	r2, #40
	strb	r2, [r6, #0]
	str	r3, [r5, #24]
	movs	r3, #136
	lsls	r3, r3, #9
	str	r3, [r5, #28]
.L_020000da:
	pop	{r5, r6, pc}
	.4byte 0xfffff800
	.2byte 0xfc00
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	movs	r0, #8
	bl 0x0200d634
	adds	r5, r7, #0
	adds	r5, #100
	ldrh	r6, [r5, #0]
	mov	r8, r0
	adds	r0, r6, #0
	bl 0x0200d524
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
	bl 0x0200d51c
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
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe224
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe254
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #40]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #2
	bne.n	.L_0200017c
	ldr	r0, [pc, #24]
	b.n	.L_0200018e
.L_0200017c:
	subs	r3, r2, #3
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #10
	cmp	r3, r2
	bhi.n	.L_0200018c
	ldr	r0, [pc, #12]
	b.n	.L_0200018e
.L_0200018c:
	ldr	r0, [pc, #12]
.L_0200018e:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0200e44c
	.4byte 0x0200e6bc
	.2byte 0xe26c
	.2byte 0x0200
	push	{lr}
	bl 0x0200cfac
	bl 0x0200d7cc
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #1
	bl 0x0200d744
	movs	r0, #40
	bl 0x0200d604
	bl 0x0200d010
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe9a4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r2, [pc, #272]
	ldr	r0, [pc, #276]
	ldr	r1, [pc, #276]
	ldr	r3, [pc, #280]
	bl 0x0200cef0
	ldr	r3, [pc, #276]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200d634
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r5, #10
.L_02000206:
	adds	r0, r5, #0
	bl 0x0200d634
	adds	r7, r0, #0
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #128
	orrs	r2, r3
	strb	r2, [r0, #0]
	movs	r1, #5
	adds	r0, r7, #0
	adds	r5, #1
	bl 0x0200d5d4
	cmp	r5, #16
	ble.n	.L_02000206
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #60
	bl 0x0200d554
	cmp	r0, #0
	beq.n	.L_020002ec
	movs	r1, #128
	movs	r2, #132
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r1, #128
	ldr	r2, [pc, #192]
	lsls	r1, r1, #18
	movs	r0, #9
	bl 0x0200d67c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r1, #1
	movs	r0, #8
	bl 0x0200d70c
	movs	r0, #8
	bl 0x0200d634
	adds	r7, r0, #0
	adds	r3, r7, #0
	movs	r5, #224
	adds	r3, #85
	movs	r6, #4
	lsls	r5, r5, #15
	movs	r2, #0
	strb	r6, [r3, #0]
	movs	r1, #5
	str	r5, [r7, #12]
	mov	sl, r2
	bl 0x0200d5d4
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200d5b4
	adds	r2, r7, #0
	adds	r2, #35
	ldrb	r3, [r2, #0]
	movs	r1, #32
	mov	r8, r1
	mov	r1, r8
	orrs	r3, r1
	strb	r3, [r2, #0]
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d70c
	movs	r0, #9
	bl 0x0200d634
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	str	r3, [r7, #24]
	str	r3, [r7, #28]
	movs	r1, #5
	str	r5, [r7, #12]
	bl 0x0200d5d4
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200d5b4
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	mov	r2, r8
	orrs	r3, r2
	movs	r2, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200d5bc
	ldr	r3, [pc, #44]
	mov	r1, sl
	str	r1, [r3, #0]
	movs	r1, #144
	ldr	r0, [pc, #40]
	lsls	r1, r1, #3
	bl 0x0200d504
.L_020002ec:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0200dae0
	.4byte 0x0200daa4
	.4byte 0x0200dab4
	.4byte 0x0200db0c
	.4byte 0x02000240
	.4byte 0x010f0000
	.4byte 0x0200eb60
	.2byte 0x84a5
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
	bl 0x020081d8
	movs	r0, #137
	lsls	r0, r0, #1
	bl 0x0200d55c
	ldr	r5, [pc, #44]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #5
	bne.n	.L_02000364
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #12]
	subs	r2, #2
	strh	r3, [r2, #0]
	bl 0x0200bff4
	b.n	.L_02000446
	.4byte 0x00000c08
	.4byte 0x00003f10
	.2byte 0x0240
	.2byte 0x0200
.L_02000364:
	cmp	r3, #4
	bne.n	.L_0200040e
	bl 0x0200d60c
	movs	r0, #0
	bl 0x0200d784
	movs	r0, #20
	movs	r1, #8
	bl 0x0200d694
	movs	r0, #21
	movs	r1, #8
	bl 0x0200d694
	movs	r0, #22
	movs	r1, #7
	bl 0x0200d694
	movs	r1, #249
	movs	r2, #160
	movs	r0, #19
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r2, #160
	movs	r0, #1
	ldr	r1, [pc, #172]
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	movs	r1, #253
	movs	r2, #140
	ldr	r0, [r3, #0]
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d72c
	bl 0x0200d73c
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #192
	movs	r0, #128
	movs	r2, #156
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200d72c
	bl 0x0200d58c
	movs	r0, #1
	bl 0x0200d4fc
	bl 0x0200d774
	bl 0x0200d77c
	movs	r0, #40
	bl 0x0200d604
	bl 0x0200b658
	bl 0x0200d614
	b.n	.L_02000446
.L_0200040e:
	cmp	r3, #3
	bne.n	.L_0200043e
	movs	r0, #20
	movs	r1, #8
	bl 0x0200d694
	movs	r0, #21
	movs	r1, #8
	bl 0x0200d694
	movs	r0, #22
	movs	r1, #7
	bl 0x0200d694
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200d554
	cmp	r0, #0
	bne.n	.L_02000446
	bl 0x0200a9ac
	b.n	.L_02000446
.L_0200043e:
	cmp	r3, #2
	bne.n	.L_02000446
	bl 0x02009f10
.L_02000446:
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x021a
	push	{lr}
	ldr	r3, [pc, #68]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	bne.n	.L_02000476
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200d554
	cmp	r0, #0
	bne.n	.L_02000476
	movs	r0, #1
	bl 0x0200d7d4
.L_02000476:
	ldr	r3, [pc, #32]
	movs	r2, #253
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r2, [pc, #24]
	ldr	r3, [pc, #24]
	movs	r1, #160
	subs	r3, r3, r2
	adds	r0, r0, r3
	lsls	r1, r1, #19
	bl 0x0200d5dc
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000010e
	.2byte 0x0121
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r0, #8
	bl 0x0200d634
	mov	r8, r0
	bl 0x0200d514
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	lsrs	r3, r3, #16
	mov	r0, r8
	lsls	r6, r3, #16
	cmp	r0, #0
	beq.n	.L_0200058a
	ldr	r3, [pc, #168]
	ldr	r5, [r3, #0]
	movs	r3, #15
	ands	r5, r3
	cmp	r5, #0
	bne.n	.L_0200058a
	ldr	r2, [r0, #12]
	ldr	r1, [r0, #8]
	movs	r3, #128
	lsls	r3, r3, #12
	adds	r2, r2, r6
	adds	r1, r1, r3
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	movs	r0, #168
	lsls	r0, r0, #2
	bl 0x0200d574
	movs	r1, #192
	adds	r7, r0, #0
	lsls	r1, r1, #11
	adds	r0, r6, #0
	bl 0x0200d4ec
	lsls	r6, r0, #16
	cmp	r7, #0
	beq.n	.L_0200058a
.L_02000500:
	ldr	r1, [r7, #80]
	adds	r0, r7, #0
	mov	sl, r1
	ldr	r1, [pc, #112]
	bl 0x0200d56c
	movs	r1, #10
	adds	r0, r7, #0
	bl 0x0200d6d4
	adds	r3, r7, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	bl 0x0200d514
	ldr	r3, [pc, #92]
	adds	r2, r7, #0
	ands	r3, r0
	adds	r2, #100
	strh	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #102
	strh	r5, [r3, #0]
	ldr	r3, [pc, #80]
	ldr	r0, [pc, #60]
	str	r3, [r7, #108]
	ldr	r3, [pc, #76]
	mov	r1, r8
	ands	r6, r3
	mov	r9, r0
	str	r1, [r7, #104]
	asrs	r0, r6, #4
	bl 0x0200d51c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	asrs	r3, r3, #16
	str	r3, [r7, #48]
	mov	r2, r9
	mov	r3, sl
	strb	r2, [r3, #26]
	mov	r0, r8
	ldr	r3, [r0, #80]
	movs	r2, #12
	ldrb	r3, [r3, #9]
	mov	r0, sl
	ands	r2, r3
	mov	r3, sl
	ldrb	r1, [r3, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	b.n	.L_02000588
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200eb60
	.4byte 0x0200ead0
	.4byte 0x0ffff000
	.4byte 0x020085a1
	.2byte 0xffff
	.2byte 0x000f
.L_02000588:
	strb	r3, [r0, #9]
.L_0200058a:
	ldr	r2, [pc, #16]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xeb60
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r7, r5, #0
	adds	r7, #100
	ldrh	r6, [r7, #0]
	ldr	r1, [r5, #104]
	adds	r0, r6, #0
	mov	r8, r1
	bl 0x0200d524
	ldr	r3, [r5, #48]
	mov	r1, r8
	adds	r3, #28
	adds	r2, r3, #0
	muls	r2, r0
	ldr	r3, [r1, #8]
	adds	r0, r6, #0
	adds	r3, r3, r2
	str	r3, [r5, #8]
	bl 0x0200d51c
	movs	r2, #132
	ldr	r3, [r5, #8]
	lsls	r0, r0, #4
	lsls	r2, r2, #17
	adds	r0, r0, r2
	str	r0, [r5, #16]
	str	r3, [r5, #56]
	str	r0, [r5, #64]
	ldr	r1, [r5, #80]
	cmp	r0, r2
	bge.n	.L_020005f0
	ldrb	r3, [r1, #9]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #8
	b.n	.L_020005fa
.L_020005f0:
	ldrb	r3, [r1, #9]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #4
.L_020005fa:
	orrs	r2, r3
	strb	r2, [r1, #9]
	ldrh	r3, [r7, #0]
	ldr	r1, [pc, #8]
	adds	r3, r3, r1
	strh	r3, [r7, #0]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0xfe00
	.2byte 0xffff
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl 0x0200d634
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200d654
	bl 0x0200d514
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #3
	adds	r2, r6, #0
	lsrs	r3, r3, #16
	adds	r3, #20
	adds	r2, #98
	strb	r3, [r2, #0]
	ldr	r3, [pc, #4]
	str	r3, [r6, #108]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x8039
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200d634
	movs	r3, #0
	movs	r1, #192
	str	r3, [r0, #108]
	lsls	r1, r1, #8
	adds	r0, r5, #0
	movs	r2, #0
	bl 0x0200d6fc
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r5, [pc, #1016]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200d634
	ldr	r3, [pc, #1008]
	adds	r7, r0, #0
	mov	r8, r3
	bl 0x0200d60c
	movs	r0, #0
	bl 0x0200d784
	movs	r0, #18
	bl 0x0200d634
	movs	r1, #15
	bl 0x0200d6d4
	movs	r1, #128
	movs	r2, #138
	lsls	r2, r2, #17
	lsls	r1, r1, #18
	movs	r0, #18
	bl 0x0200d67c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #6
	movs	r1, #1
	bl 0x0200d70c
	movs	r0, #5
	movs	r1, #1
	bl 0x0200d70c
	movs	r0, #19
	movs	r1, #1
	bl 0x0200d70c
	movs	r0, #2
	movs	r1, #1
	bl 0x0200d70c
	movs	r1, #1
	movs	r0, #3
	bl 0x0200d70c
	movs	r0, #0
	bl 0x0200d7dc
	ldr	r0, [pc, #916]
	bl 0x0200d6dc
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200d714
	bl 0x0200d73c
	movs	r6, #0
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r1, #200
	movs	r0, #204
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200d724
	movs	r0, #128
	movs	r1, #1
	movs	r2, #152
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200d72c
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #844]
	adds	r2, #204
	bl 0x0200d644
	movs	r1, #252
	movs	r2, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d66c
	movs	r1, #192
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200d6fc
	ldr	r1, [r5, #0]
	movs	r0, #0
	bl 0x0200d68c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #0
	ldr	r1, [pc, #792]
	adds	r2, #204
	bl 0x0200d644
	movs	r1, #130
	movs	r2, #160
	movs	r0, #0
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200d66c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #0
	movs	r0, #1
	bl 0x0200d68c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #1
	ldr	r1, [pc, #736]
	adds	r2, #204
	bl 0x0200d644
	movs	r1, #134
	movs	r2, #160
	movs	r0, #1
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200d66c
	movs	r1, #160
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200d6fc
	movs	r1, #224
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200d6fc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #1
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #1
	movs	r0, #3
	bl 0x0200d68c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #3
	ldr	r1, [pc, #652]
	adds	r2, #204
	bl 0x0200d644
	movs	r1, #136
	movs	r2, #164
	lsls	r2, r2, #1
	movs	r0, #3
	lsls	r1, r1, #2
	bl 0x0200d66c
	movs	r1, #160
	movs	r0, #3
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #3
	movs	r1, #4
	bl 0x0200d694
	movs	r1, #0
	movs	r0, #3
	bl 0x0200d6f4
	ldr	r0, [r5, #0]
	bl 0x0200d654
	bl 0x0200d514
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	adds	r2, r7, #0
	lsrs	r3, r3, #16
	adds	r2, #98
	adds	r3, #20
	strb	r3, [r2, #0]
	ldr	r3, [pc, #580]
	movs	r0, #1
	str	r3, [r7, #108]
	bl 0x02008610
	movs	r0, #3
	bl 0x02008610
	ldr	r1, [r5, #0]
	movs	r0, #5
	bl 0x0200d68c
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x0200d68c
	ldr	r1, [r5, #0]
	movs	r0, #7
	bl 0x0200d68c
	ldr	r1, [r5, #0]
	movs	r0, #2
	bl 0x0200d68c
	ldr	r1, [r5, #0]
	movs	r0, #19
	bl 0x0200d68c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #508]
	adds	r2, #204
	bl 0x0200d644
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #496]
	adds	r2, #204
	bl 0x0200d644
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #480]
	adds	r2, #204
	bl 0x0200d644
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #2
	ldr	r1, [pc, #468]
	adds	r2, #204
	bl 0x0200d644
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #19
	ldr	r1, [pc, #452]
	bl 0x0200d644
	ldr	r1, [pc, #452]
	movs	r0, #5
	bl 0x0200d64c
	ldr	r1, [pc, #448]
	movs	r0, #6
	bl 0x0200d64c
	ldr	r1, [pc, #444]
	movs	r0, #7
	bl 0x0200d64c
	ldr	r1, [pc, #440]
	movs	r0, #19
	bl 0x0200d64c
	ldr	r1, [pc, #436]
	movs	r0, #2
	bl 0x0200d65c
	movs	r0, #10
	bl 0x0200d604
	movs	r0, #16
	bl 0x0200d63c
	movs	r0, #15
	bl 0x0200d63c
	movs	r0, #14
	bl 0x0200d63c
	movs	r0, #13
	bl 0x0200d63c
	movs	r0, #12
	bl 0x0200d63c
	movs	r0, #11
	bl 0x0200d63c
	movs	r0, #10
	bl 0x0200d63c
	movs	r0, #5
	bl 0x02008610
	movs	r0, #6
	bl 0x02008610
	movs	r0, #7
	bl 0x02008610
	movs	r0, #2
	bl 0x02008610
	movs	r0, #19
	bl 0x02008610
	movs	r0, #80
	bl 0x0200d604
	movs	r0, #6
	bl 0x0200d634
	movs	r1, #0
	str	r6, [r0, #108]
	movs	r0, #6
	bl 0x0200d704
	movs	r1, #0
	movs	r0, #6
	bl 0x0200d6f4
	movs	r0, #5
	bl 0x0200d634
	movs	r1, #128
	str	r6, [r0, #108]
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200d704
	movs	r1, #3
	movs	r0, #5
	bl 0x0200d69c
	movs	r0, #7
	bl 0x0200d634
	movs	r1, #0
	str	r6, [r0, #108]
	movs	r0, #7
	bl 0x0200d704
	movs	r2, #10
	movs	r0, #7
	movs	r1, #4
	bl 0x0200d6ac
	movs	r1, #0
	movs	r0, #7
	bl 0x0200d6f4
	ldr	r0, [r5, #0]
	bl 0x0200d634
	movs	r1, #128
	str	r6, [r0, #108]
	lsls	r1, r1, #8
	ldr	r0, [r5, #0]
	bl 0x0200d704
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200d69c
	movs	r0, #0
	bl 0x02008640
	movs	r0, #1
	bl 0x02008640
	movs	r0, #19
	bl 0x02008640
	movs	r0, #2
	bl 0x02008640
	movs	r0, #3
	bl 0x02008640
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200d764
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200d75c
	movs	r0, #40
	bl 0x0200d76c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d644
	movs	r1, #252
	movs	r2, #146
	lsls	r2, r2, #1
	lsls	r1, r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200d66c
	movs	r0, #162
	bl 0x0200d7dc
	movs	r0, #20
	bl 0x0200d604
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200d6bc
	movs	r1, #129
	lsls	r1, r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200d71c
	ldr	r0, [r5, #0]
	bl 0x0200d634
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #204
	movs	r2, #200
	lsls	r1, r1, #6
	lsls	r2, r2, #5
	ldr	r0, [r5, #0]
	adds	r1, #51
	adds	r2, #153
	bl 0x0200d644
	movs	r1, #252
	movs	r2, #154
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200d664
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200d6bc
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200d7dc
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200d71c
	movs	r1, #252
	movs	r2, #160
	lsls	r1, r1, #1
	b.n	.L_02000a88
	.4byte 0x02000240
	.4byte 0x0500021e
	.4byte 0x00002d8c
	.4byte 0x00019999
	.4byte 0x02008039
	.4byte 0x0200dba8
	.4byte 0x0200dbd0
	.4byte 0x0200dbf8
	.4byte 0x0200dc48
	.2byte 0xdc20
	.2byte 0x0200
.L_02000a88:
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	bl 0x0200d664
	ldr	r0, [r5, #0]
	bl 0x0200d634
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	ldr	r3, [pc, #60]
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200d6fc
	movs	r1, #192
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	b.n	.L_02000ae4
	.2byte 0x0000
	.2byte 0x7fff
	.2byte 0x0000
.L_02000ae4:
	bl 0x0200d704
	movs	r1, #130
	movs	r2, #154
	movs	r0, #0
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200d66c
	movs	r1, #160
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200d6fc
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #128
	movs	r2, #20
	lsls	r0, r0, #8
	movs	r1, #0
	bl 0x0200d6ec
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #0
	bl 0x0200d714
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #1
	bl 0x0200d714
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #160
	movs	r2, #40
	movs	r0, #1
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r0, #2
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #7
	bl 0x0200d704
	movs	r0, #0
	movs	r1, #4
	bl 0x0200d694
	movs	r1, #4
	movs	r0, #1
	bl 0x0200d69c
	movs	r0, #36
	bl 0x0200d7dc
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #192
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #1
	bl 0x0200d704
	ldr	r0, [r5, #0]
	bl 0x02008610
	movs	r0, #5
	bl 0x02008610
	movs	r0, #6
	bl 0x02008610
	movs	r0, #7
	bl 0x02008610
	movs	r0, #0
	bl 0x02008610
	movs	r0, #1
	bl 0x02008610
	movs	r0, #3
	bl 0x02008610
	movs	r0, #2
	bl 0x02008610
	movs	r0, #19
	bl 0x02008610
	movs	r0, #40
	bl 0x0200d604
	movs	r0, #19
	bl 0x02008640
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #19
	bl 0x0200d714
	movs	r0, #19
	movs	r1, #4
	movs	r2, #10
	bl 0x0200d6ac
	movs	r2, #10
	movs	r0, #19
	movs	r1, #6
	bl 0x0200d6ac
	movs	r1, #0
	movs	r0, #19
	bl 0x0200d6f4
	ldr	r0, [r5, #0]
	bl 0x02008640
	movs	r0, #5
	bl 0x02008640
	movs	r0, #6
	bl 0x02008640
	movs	r0, #7
	bl 0x02008640
	movs	r0, #0
	bl 0x02008640
	movs	r0, #1
	bl 0x02008640
	movs	r0, #3
	bl 0x02008640
	movs	r0, #2
	bl 0x02008640
	movs	r0, #18
	bl 0x0200d634
	movs	r1, #0
	bl 0x0200d6d4
	movs	r0, #18
	bl 0x0200d634
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r0, #18
	bl 0x0200d634
	movs	r1, #128
	movs	r2, #240
	movs	r3, #208
	lsls	r2, r2, #15
	lsls	r3, r3, #16
	lsls	r1, r1, #18
	bl 0x0200d584
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #1016]
	adds	r1, #153
	bl 0x0200d724
	movs	r0, #128
	movs	r1, #1
	movs	r2, #216
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #18
	bl 0x0200d72c
	bl 0x0200d734
	movs	r0, #40
	bl 0x0200d604
	movs	r0, #1
	movs	r1, #0
	movs	r2, #20
	bl 0x0200d6ec
	movs	r0, #128
	movs	r1, #1
	movs	r2, #152
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x0200d72c
	bl 0x0200d734
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #129
	movs	r0, #0
	lsls	r1, r1, #1
	bl 0x0200d71c
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200d71c
	movs	r1, #129
	movs	r0, #1
	lsls	r1, r1, #1
	bl 0x0200d71c
	movs	r1, #129
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x0200d71c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200d71c
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #0
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #0
	movs	r1, #3
	bl 0x0200d69c
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #0
	bl 0x0200d714
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #2
	bl 0x0200d714
	movs	r0, #2
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #7
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200d704
	movs	r0, #160
	lsls	r0, r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #6
	movs	r2, #0
	adds	r1, #255
	movs	r0, #3
	bl 0x0200d714
	movs	r0, #3
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #4
	movs	r2, #0
	adds	r1, #255
	movs	r0, #1
	bl 0x0200d714
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #6
	bl 0x0200d704
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	adds	r1, #204
	bl 0x0200d724
	movs	r0, #128
	movs	r1, #1
	movs	r2, #148
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200d72c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #18
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d644
	movs	r1, #128
	movs	r2, #192
	movs	r3, #138
	lsls	r3, r3, #17
	lsls	r2, r2, #15
	lsls	r1, r1, #18
	adds	r0, r7, #0
	bl 0x0200d594
	movs	r0, #18
	bl 0x0200d674
	movs	r0, #10
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
.L_02000e40:
	movs	r1, #192
	movs	r2, #0
	movs	r0, #1
.L_02000e46:
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r0, #0
	movs	r1, #1
	bl 0x0200d6bc
	movs	r1, #130
	movs	r2, #160
	lsls	r2, r2, #1
	movs	r0, #0
	lsls	r1, r1, #2
	bl 0x0200d66c
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r1, #9
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #1
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #10
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #129
	movs	r0, #6
	lsls	r1, r1, #1
	bl 0x0200d71c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #9
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #0
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #3
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r2, #10
	movs	r0, #6
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #5
	movs	r1, #4
	bl 0x0200d694
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #11
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d6f4
.L_02000f18:
	movs	r1, #12
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #18
	bl 0x0200d6a4
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
.L_02000f30:
	bl 0x0200d6f4
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200d714
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #10
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #1
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #10
	bl 0x0200d604
	movs	r1, #10
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #5
	bl 0x0200d604
	movs	r1, #1
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #19
.L_02000fda:
	movs	r1, #0
	bl 0x0200d6f4
.L_02000fe0:
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
.L_02000fe6:
	movs	r2, #0
	bl 0x0200d6fc
.L_02000fec:
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #8
.L_02000ff2:
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #6
	movs	r2, #80
	adds	r1, #255
	movs	r0, #18
	bl 0x0200d714
	movs	r0, #5
	movs	r1, #4
	bl 0x0200d694
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #9
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #4
	movs	r2, #20
	adds	r1, #255
	movs	r0, #19
	bl 0x0200d714
	b.n	.L_02001084
	.2byte 0xcccc
	.2byte 0x0004
.L_02001084:
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #10
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #9
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #3
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #10
	bl 0x0200d604
	movs	r1, #6
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #10
	bl 0x0200d604
	ldr	r1, [pc, #912]
	adds	r0, r7, #0
	bl 0x0200d3e4
	movs	r0, #80
	bl 0x0200d604
	movs	r1, #3
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #10
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #19
	bl 0x0200d714
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #3
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #20
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #6
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #142
	bl 0x0200d7dc
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #5
	bl 0x0200d75c
	movs	r0, #40
	bl 0x0200d76c
	movs	r0, #40
	bl 0x0200d604
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #1
	bl 0x0200d75c
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x0200d75c
	movs	r0, #40
	bl 0x0200d76c
	movs	r0, #40
	bl 0x0200d604
	movs	r1, #3
	movs	r0, #18
.L_020011c4:
	bl 0x0200d694
	movs	r0, #20
.L_020011ca:
	bl 0x0200d604
	movs	r0, #18
.L_020011d0:
	movs	r1, #0
	bl 0x0200d6f4
.L_020011d6:
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
.L_020011dc:
	movs	r2, #0
	bl 0x0200d6fc
.L_020011e2:
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #8
.L_020011e8:
	movs	r2, #0
	bl 0x0200d6fc
.L_020011ee:
	movs	r1, #192
	movs	r0, #2
	lsls	r1, r1, #8
.L_020011f4:
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
.L_02001230:
	bl 0x0200d704
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #4
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #19
	bl 0x0200d714
.L_0200125e:
	movs	r1, #0
	movs	r0, #19
	bl 0x0200d6f4
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #13
	movs	r0, #18
.L_02001270:
	bl 0x0200d694
	movs	r0, #20
.L_02001276:
	bl 0x0200d604
	movs	r2, #20
.L_0200127c:
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6ec
	movs	r0, #19
	movs	r1, #0
.L_02001288:
	bl 0x0200d6f4
	movs	r1, #8
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #180
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #8
	movs	r2, #0
	adds	r1, #255
	movs	r0, #2
	bl 0x0200d714
	movs	r0, #2
	movs	r1, #0
	bl 0x0200d6f4
	movs	r2, #0
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6fc
	movs	r1, #1
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #5
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #9
	bl 0x0200d694
	movs	r1, #6
	adds	r1, #255
	movs	r2, #40
	movs	r0, #18
	bl 0x0200d714
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200d714
	movs	r1, #0
	movs	r0, #19
	bl 0x0200d6f4
	adds	r0, r7, #0
	bl 0x0200d4e4
	movs	r1, #1
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #7
	bl 0x0200d714
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r2, #10
	movs	r0, #7
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #10
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #11
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #5
	bl 0x0200d604
	movs	r1, #1
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #8
	movs	r2, #0
	adds	r1, #255
	movs	r0, #1
	bl 0x0200d714
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #6
	bl 0x0200d704
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #3
	bl 0x0200d714
	movs	r0, #3
	movs	r1, #4
	bl 0x0200d694
	movs	r0, #3
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #0
	movs	r1, #4
	bl 0x0200d694
	movs	r0, #0
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #208
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #160
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #20
	movs	r0, #1
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #0
	movs	r1, #4
	bl 0x0200d694
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #7
	bl 0x0200d6e4
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200d62c
	cmp	r0, #0
	bne.n	.L_02001464
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d694
	movs	r2, #10
	movs	r0, #6
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d6f4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001490
	.2byte 0x0000
	.2byte 0xec20
	.2byte 0x0200
.L_02001464:
	movs	r0, #6
	movs	r1, #4
	bl 0x0200d694
	movs	r0, #6
	movs	r1, #4
	movs	r2, #10
	bl 0x0200d6ac
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #6
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200d6f4
.L_02001490:
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	ldr	r3, [pc, #312]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #192
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #11
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d704
	movs	r1, #10
	adds	r1, #255
	movs	r2, #20
	movs	r0, #6
	bl 0x0200d714
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200d714
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #18
	movs	r1, #8
	bl 0x0200d694
	movs	r1, #128
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	ldr	r0, [r5, #0]
	bl 0x0200d6fc
	movs	r0, #18
	bl 0x0200d6a4
	movs	r0, #18
	movs	r1, #1
	bl 0x0200d694
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #2
	bl 0x0200d714
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #2
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #7
	bl 0x0200d6e4
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200d62c
	cmp	r0, #0
	bne.n	.L_02001624
	ldr	r0, [r5, #0]
	bl 0x0200d6a4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001638
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
.L_02001624:
	movs	r0, #1
	movs	r1, #4
	bl 0x0200d694
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
.L_02001638:
	ldr	r2, [pc, #1016]
	movs	r5, #133
	mov	r8, r2
	lsls	r5, r5, #2
	add	r5, r8
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200d704
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200d69c
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #102
	adds	r2, #51
	bl 0x0200d644
	movs	r1, #252
	movs	r2, #154
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200d66c
	movs	r0, #201
	bl 0x0200d7dc
	movs	r1, #14
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #10
	bl 0x0200d604
	movs	r1, #128
	movs	r0, #18
	lsls	r1, r1, #1
	bl 0x0200d6cc
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200d6bc
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #824]
	adds	r2, #204
	bl 0x0200d644
	ldr	r0, [r5, #0]
	movs	r1, #4
	movs	r2, #0
	bl 0x0200d6ac
	ldr	r0, [r5, #0]
	bl 0x0200d634
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #252
	movs	r2, #160
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	bl 0x0200d66c
	movs	r0, #1
	bl 0x0200d604
	ldr	r0, [r5, #0]
	bl 0x0200d634
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	movs	r1, #129
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200d71c
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #14
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #10
	bl 0x0200d604
	movs	r1, #10
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #9
	movs	r0, #18
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #40
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #0
	movs	r0, #7
	bl 0x0200d6f4
	movs	r0, #143
	lsls	r0, r0, #2
	bl 0x0200d7dc
	movs	r1, #0
	movs	r0, #18
	bl 0x0200d6cc
	movs	r0, #18
	bl 0x0200d634
	movs	r1, #7
	bl 0x0200d6d4
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #18
	bl 0x0200d634
	movs	r1, #0
	bl 0x0200d6d4
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r2, #20
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r0, #18
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d694
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #0
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #1
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #2
	movs	r1, #3
	bl 0x0200d694
	movs	r1, #3
	movs	r0, #3
	bl 0x0200d69c
	movs	r0, #78
	bl 0x0200d7dc
	ldr	r3, [pc, #352]
	ldr	r1, [pc, #356]
	str	r3, [r7, #108]
	movs	r0, #18
	bl 0x0200d64c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #1
	bl 0x0200d75c
	movs	r0, #20
	bl 0x0200d76c
	movs	r0, #20
	bl 0x0200d4fc
	movs	r1, #0
	movs	r0, #0
	bl 0x0200d75c
	movs	r0, #40
	bl 0x0200d76c
	movs	r0, #40
	bl 0x0200d4fc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #3
	bl 0x0200d67c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #17
	bl 0x0200d634
	movs	r3, #128
	adds	r7, r0, #0
	lsls	r3, r3, #5
	movs	r1, #128
	movs	r2, #140
	str	r3, [r7, #24]
	str	r3, [r7, #28]
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r0, #17
	bl 0x0200d67c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r3, #85
	adds	r3, r3, r7
	movs	r6, #0
	mov	sl, r3
	strb	r6, [r3, #0]
	movs	r1, #128
	movs	r2, #192
	movs	r3, #140
	lsls	r3, r3, #17
	adds	r0, r7, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #14
	bl 0x0200d584
	movs	r0, #1
	bl 0x0200d4fc
	mov	r2, sl
	movs	r0, #128
	strb	r6, [r2, #0]
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x0200d75c
	movs	r0, #20
	bl 0x0200d76c
	movs	r6, #128
	movs	r0, #39
	bl 0x0200d7dc
	lsls	r6, r6, #19
	ldr	r1, [pc, #108]
	movs	r0, #17
	bl 0x0200d65c
	ldrh	r2, [r6, #0]
	movs	r3, #249
	lsls	r3, r3, #8
	adds	r3, #255
	movs	r0, #128
	ands	r3, r2
	lsls	r0, r0, #9
	strh	r3, [r6, #0]
	movs	r1, #1
	adds	r0, #3
	bl 0x0200d75c
	movs	r0, #40
	bl 0x0200d76c
	movs	r2, #204
	lsls	r2, r2, #6
	adds	r2, #51
	ldr	r1, [pc, #68]
	movs	r0, #17
	bl 0x0200d644
	movs	r0, #138
	bl 0x0200d7dc
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200d75c
	movs	r0, #1
	bl 0x0200d76c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r1, #136
	movs	r2, #176
	movs	r3, #140
	lsls	r2, r2, #15
	lsls	r3, r3, #17
	b.n	.L_02001a4c
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00019999
	.4byte 0x02008069
	.4byte 0x0200dc70
	.4byte 0x0200dcac
	.2byte 0x3333
	.2byte 0x0003
.L_02001a4c:
	adds	r0, r7, #0
	lsls	r1, r1, #18
	bl 0x0200d594
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x0200d75c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200d75c
	movs	r0, #10
	bl 0x0200d76c
	movs	r0, #17
	bl 0x0200d674
	movs	r0, #138
	bl 0x0200d7dc
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200d75c
	movs	r0, #1
	bl 0x0200d76c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r1, #232
	movs	r2, #176
	movs	r3, #140
	lsls	r2, r2, #15
	lsls	r3, r3, #17
	adds	r0, r7, #0
	lsls	r1, r1, #17
	bl 0x0200d594
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x0200d75c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200d75c
	movs	r0, #10
	bl 0x0200d76c
	movs	r0, #17
	bl 0x0200d674
	movs	r0, #138
	bl 0x0200d7dc
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200d75c
	movs	r0, #1
	bl 0x0200d76c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r1, #144
	movs	r2, #176
	movs	r3, #140
	lsls	r3, r3, #17
	lsls	r2, r2, #15
	adds	r0, r7, #0
	lsls	r1, r1, #18
	bl 0x0200d594
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x0200d75c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200d75c
	movs	r0, #10
	bl 0x0200d76c
	movs	r0, #17
	bl 0x0200d674
	ldr	r2, [pc, #188]
	ldr	r1, [pc, #188]
	movs	r0, #17
	bl 0x0200d644
	movs	r0, #138
	bl 0x0200d7dc
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200d75c
	movs	r0, #1
	bl 0x0200d76c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r1, #254
	movs	r2, #208
	movs	r3, #144
	lsls	r2, r2, #16
	lsls	r3, r3, #17
	adds	r0, r7, #0
	lsls	r1, r1, #17
	bl 0x0200d594
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x0200d75c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200d75c
	movs	r0, #10
	bl 0x0200d76c
	movs	r0, #17
	bl 0x0200d674
	movs	r0, #138
	bl 0x0200d7dc
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200d75c
	movs	r0, #1
	bl 0x0200d76c
	movs	r0, #1
	bl 0x0200d4fc
	ldrh	r3, [r6, #0]
	ldr	r2, [pc, #60]
	movs	r1, #244
	orrs	r3, r2
	movs	r2, #160
	strh	r3, [r6, #0]
	movs	r0, #7
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r1, #252
	movs	r2, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r1, #130
	movs	r2, #160
	movs	r0, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r1, #134
	movs	r2, #160
	movs	r0, #1
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	b.n	.L_02001be0
	.2byte 0x0000
	.4byte 0x00000600
	.4byte 0x00033333
	.2byte 0x6666
	.2byte 0x0006
.L_02001be0:
	bl 0x0200d67c
	movs	r1, #240
	movs	r2, #164
	movs	r0, #6
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r1, #248
	movs	r2, #164
	movs	r0, #5
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r1, #128
	movs	r2, #164
	movs	r0, #19
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r1, #132
	movs	r2, #164
	movs	r0, #2
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r1, #136
	movs	r2, #164
	lsls	r2, r2, #17
	lsls	r1, r1, #18
	movs	r0, #3
	bl 0x0200d67c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #1
	bl 0x0200d75c
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x0200d75c
	movs	r0, #10
	bl 0x0200d76c
	movs	r0, #20
	bl 0x0200d4fc
	movs	r0, #138
	bl 0x0200d7dc
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200d75c
	movs	r0, #1
	bl 0x0200d76c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #1
	bl 0x0200d75c
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x0200d75c
	movs	r0, #40
	bl 0x0200d76c
	movs	r0, #40
	bl 0x0200d4fc
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r2, sl
	str	r3, [r7, #72]
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r0, #30
	bl 0x0200d604
	movs	r0, #208
	bl 0x0200d7dc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200d5c4
	movs	r0, #10
	bl 0x0200d604
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r1, r1
	negs	r0, r0
	bl 0x0200d5c4
	bl 0x0200d5cc
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #1
	movs	r0, #17
	bl 0x0200d6b4
	movs	r0, #148
	bl 0x0200d7dc
	movs	r0, #80
	bl 0x0200d604
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #4
	movs	r2, #0
	adds	r1, #255
	movs	r0, #7
	bl 0x0200d714
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #4
	movs	r2, #0
	adds	r1, #255
	movs	r0, #1
	bl 0x0200d714
	movs	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #3
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #3
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #2
	movs	r1, #0
	bl 0x0200d6f4
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #316]
	adds	r2, #204
	bl 0x0200d644
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #304]
	adds	r2, #204
	bl 0x0200d644
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #0
	ldr	r1, [pc, #288]
	adds	r2, #204
	bl 0x0200d644
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #1
	ldr	r1, [pc, #276]
	adds	r2, #204
	bl 0x0200d644
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #260]
	adds	r2, #204
	bl 0x0200d644
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #248]
	adds	r2, #204
	bl 0x0200d644
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #2
	ldr	r1, [pc, #232]
	adds	r2, #204
	bl 0x0200d644
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #3
	ldr	r1, [pc, #216]
	bl 0x0200d644
	ldr	r1, [pc, #216]
	movs	r0, #7
	bl 0x0200d64c
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #208]
	bl 0x0200d64c
	ldr	r1, [pc, #208]
	movs	r0, #0
	bl 0x0200d64c
	ldr	r1, [pc, #204]
	movs	r0, #6
	bl 0x0200d64c
	ldr	r1, [pc, #200]
	movs	r0, #5
	bl 0x0200d64c
	ldr	r1, [pc, #196]
	movs	r0, #2
	bl 0x0200d64c
	ldr	r1, [pc, #192]
	movs	r0, #3
	bl 0x0200d64c
	ldr	r1, [pc, #188]
	movs	r0, #1
	bl 0x0200d65c
	movs	r1, #2
	movs	r2, #40
	adds	r1, #255
	movs	r0, #19
	bl 0x0200d714
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #129
	movs	r0, #19
	lsls	r1, r1, #1
	bl 0x0200d71c
	movs	r1, #160
	movs	r0, #19
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #7
	bl 0x0200d704
	movs	r2, #10
	movs	r0, #1
	movs	r1, #6
	bl 0x0200d6ac
	movs	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #19
	movs	r1, #2
	movs	r2, #10
	bl 0x0200d6ac
	movs	r2, #10
	movs	r0, #19
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	add	r8, r3
	mov	r2, r8
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r0, [pc, #56]
	movs	r1, #2
	bl 0x0200d754
	movs	r0, #102
	movs	r1, #1
	bl 0x0200d74c
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x00019999
	.4byte 0x0200ddf8
	.4byte 0x0200dce8
	.4byte 0x0200dd2c
	.4byte 0x0200ddb4
	.4byte 0x0200dd70
	.4byte 0x0200de3c
	.4byte 0x0200de80
	.4byte 0x0200dec4
	.2byte 0x0129
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	lsrs	r3, r2
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001ecc
	movs	r1, #15
	bl 0x0200d6d4
	b.n	.L_02001ed2
.L_02001ecc:
	movs	r1, #0
	bl 0x0200d6d4
.L_02001ed2:
	ldr	r2, [pc, #8]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	pop	{pc}
	.4byte 0x0200eb64
	.2byte 0xeb6c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #32]
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	lsrs	r3, r2
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001f00
	movs	r1, #0
	bl 0x0200d6d4
	b.n	.L_02001f06
.L_02001f00:
	movs	r1, #15
	bl 0x0200d6d4
.L_02001f06:
	pop	{pc}
	.4byte 0x0200eb64
	.2byte 0xeb6c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #17
	sub	sp, #8
	bl 0x0200d634
	adds	r7, r0, #0
	movs	r0, #30
	bl 0x0200d634
	mov	r9, r0
	movs	r0, #31
	bl 0x0200d634
	mov	fp, r0
	movs	r0, #32
	bl 0x0200d634
	str	r0, [sp, #4]
	bl 0x0200d60c
	movs	r0, #0
	bl 0x0200d784
	movs	r0, #17
	movs	r1, #2
	bl 0x0200d694
	movs	r2, #0
	mov	r8, r2
	adds	r3, r7, #0
	adds	r3, #85
	mov	r2, r8
	strb	r2, [r3, #0]
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #1016]
	movs	r0, #28
	adds	r3, r3, r2
	str	r3, [r7, #8]
	ldr	r3, [r7, #16]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r7, #16]
	movs	r1, #2
	bl 0x0200d694
	movs	r0, #29
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r1, r1
	negs	r0, r0
	bl 0x0200d72c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #30
	movs	r1, #7
	bl 0x0200d694
	movs	r0, #31
	movs	r1, #7
	bl 0x0200d694
	movs	r0, #32
	movs	r1, #6
	bl 0x0200d694
	ldr	r3, [pc, #940]
	movs	r2, #133
	mov	sl, r3
	lsls	r2, r2, #2
	add	sl, r2
	mov	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #252
	movs	r3, #192
	movs	r2, #160
	lsls	r3, r3, #8
	lsls	r2, r2, #17
	lsls	r1, r1, #17
	bl 0x0200d684
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #0
	movs	r1, #0
	bl 0x0200d764
	movs	r1, #0
	movs	r0, #0
	bl 0x0200d75c
	movs	r0, #1
	bl 0x0200d76c
	movs	r0, #30
	bl 0x0200d634
	movs	r1, #15
	bl 0x0200d6d4
	movs	r0, #31
	bl 0x0200d634
	movs	r1, #15
	bl 0x0200d6d4
	movs	r0, #32
	bl 0x0200d634
	movs	r1, #15
	bl 0x0200d6d4
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #21
	movs	r1, #1
	bl 0x0200d70c
	movs	r0, #20
	movs	r1, #1
	bl 0x0200d70c
	movs	r0, #19
	movs	r1, #1
	bl 0x0200d70c
	movs	r0, #26
	movs	r1, #1
	bl 0x0200d70c
	movs	r0, #25
	movs	r1, #1
	bl 0x0200d70c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #88
	str	r3, [r2, #0]
	subs	r3, #80
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	bl 0x0200d774
	bl 0x0200d77c
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200d7dc
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #1
	bl 0x0200d75c
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x0200d75c
	movs	r0, #16
	bl 0x0200d76c
	movs	r0, #16
	bl 0x0200d4fc
	movs	r0, #80
	bl 0x0200d604
	ldr	r3, [pc, #724]
	ldr	r6, [pc, #728]
	mov	r2, r8
	str	r2, [r3, #0]
	movs	r3, #4
	str	r3, [r6, #0]
	movs	r0, #17
	bl 0x0200d634
	ldr	r5, [pc, #716]
	str	r5, [r0, #108]
	movs	r0, #27
	bl 0x0200d634
	str	r5, [r0, #108]
	movs	r0, #28
	bl 0x0200d634
	str	r5, [r0, #108]
	movs	r0, #29
	bl 0x0200d634
	ldr	r3, [pc, #692]
	mov	r2, r9
	str	r5, [r0, #108]
	str	r3, [r2, #108]
	mov	r2, fp
	str	r3, [r2, #108]
	ldr	r2, [sp, #4]
	movs	r0, #170
	lsls	r0, r0, #1
	str	r3, [r2, #108]
	adds	r0, #255
	bl 0x0200d7dc
	movs	r0, #10
	bl 0x0200d4fc
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7dc
	movs	r0, #10
	bl 0x0200d4fc
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7dc
	movs	r0, #10
	bl 0x0200d4fc
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7dc
	movs	r0, #10
	bl 0x0200d4fc
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d7dc
	movs	r0, #40
	bl 0x0200d4fc
	movs	r3, #3
	str	r3, [r6, #0]
	movs	r0, #30
	bl 0x0200d4fc
	movs	r3, #2
	str	r3, [r6, #0]
	movs	r0, #20
	bl 0x0200d4fc
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #29
	bl 0x0200d67c
	movs	r0, #17
	bl 0x0200d634
	mov	r3, r8
	str	r3, [r0, #108]
	movs	r0, #27
	bl 0x0200d634
	mov	r2, r8
	str	r2, [r0, #108]
	movs	r0, #28
	bl 0x0200d634
	mov	r3, r8
	str	r3, [r0, #108]
	movs	r0, #29
	bl 0x0200d634
	mov	r2, r8
	mov	r3, r9
	str	r2, [r0, #108]
	str	r2, [r3, #108]
	mov	r3, fp
	str	r2, [r3, #108]
	ldr	r3, [sp, #4]
	movs	r0, #1
	str	r2, [r3, #108]
	bl 0x0200d4fc
	movs	r0, #103
	bl 0x0200d7dc
	movs	r0, #30
	bl 0x0200d634
	movs	r1, #9
	bl 0x0200d6d4
	movs	r0, #31
	bl 0x0200d634
	movs	r1, #9
	bl 0x0200d6d4
	movs	r0, #32
	bl 0x0200d634
	movs	r1, #9
	bl 0x0200d6d4
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #30
	bl 0x0200d634
	movs	r1, #0
	bl 0x0200d6d4
	movs	r0, #31
	bl 0x0200d634
	movs	r1, #0
	bl 0x0200d6d4
	movs	r0, #32
	bl 0x0200d634
	movs	r1, #0
	bl 0x0200d6d4
	movs	r0, #20
	bl 0x0200d604
	bl 0x0200d514
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #4
	lsrs	r3, r3, #16
	mov	r2, r9
	adds	r3, #40
	adds	r2, #98
	strb	r3, [r2, #0]
	bl 0x0200d514
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #4
	lsrs	r3, r3, #16
	mov	r2, fp
	adds	r3, #40
	adds	r2, #98
	strb	r3, [r2, #0]
	bl 0x0200d514
	lsls	r3, r0, #2
	ldr	r2, [sp, #4]
	adds	r3, r3, r0
	lsls	r3, r3, #4
	lsrs	r3, r3, #16
	adds	r3, #40
	adds	r2, #98
	strb	r3, [r2, #0]
	ldr	r3, [pc, #332]
	mov	r2, r9
	str	r3, [r2, #108]
	mov	r2, fp
	str	r3, [r2, #108]
	ldr	r2, [sp, #4]
	movs	r0, #20
	str	r3, [r2, #108]
	bl 0x0200d604
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl 0x0200d75c
	movs	r0, #40
	bl 0x0200d76c
	movs	r0, #80
	bl 0x0200d4fc
	ldr	r0, [pc, #296]
	bl 0x0200d6dc
	movs	r1, #128
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #26
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #22
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	mov	r3, sl
	movs	r1, #128
	ldr	r0, [r3, #0]
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #23
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #24
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #21
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #20
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #26
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #0
	movs	r0, #25
	lsls	r1, r1, #7
	bl 0x0200d6fc
	movs	r1, #204
	lsls	r1, r1, #6
	adds	r1, #51
	ldr	r0, [pc, #164]
	bl 0x0200d724
	bl 0x0200d73c
	mov	r2, r8
	adds	r0, #85
	strb	r2, [r0, #0]
	movs	r1, #160
	movs	r0, #128
	movs	r2, #170
	movs	r3, #1
	lsls	r0, r0, #18
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x0200d72c
	bl 0x0200d734
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #19
	bl 0x0200d714
	movs	r1, #0
	movs	r0, #19
	bl 0x0200d6f4
	movs	r0, #8
	bl 0x0200d7dc
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #24
	bl 0x0200d71c
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #24
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #25
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #19
	movs	r1, #3
	bl 0x0200d69c
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #129
	movs	r0, #26
	lsls	r1, r1, #1
	bl 0x0200d71c
	movs	r0, #128
	lsls	r0, r0, #5
.L_02002354:
	adds	r0, #26
	movs	r1, #0
	b.n	.L_02002380
	.2byte 0x0000
	.4byte 0xfffc0000
	.4byte 0x02000240
	.4byte 0x0200eb64
	.4byte 0x0200eb6c
	.4byte 0x02009eb1
	.4byte 0x02009ee5
	.4byte 0x0200808d
	.4byte 0x00002ddd
	.2byte 0x9999
	.2byte 0x0001
.L_02002380:
	bl 0x0200d6f4
	movs	r0, #19
	movs	r1, #4
	bl 0x0200d694
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #23
	bl 0x0200d714
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #23
	ldr	r1, [pc, #1016]
	adds	r2, #204
	bl 0x0200d644
	movs	r1, #128
	movs	r2, #164
	lsls	r2, r2, #1
	movs	r0, #23
	lsls	r1, r1, #2
	bl 0x0200d66c
	movs	r1, #128
	movs	r0, #23
	lsls	r1, r1, #6
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #23
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #20
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #20
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200d714
	movs	r1, #2
	movs	r0, #21
	bl 0x0200d70c
	movs	r0, #20
	bl 0x0200d634
	mov	r3, r8
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r0, #21
	bl 0x0200d634
	mov	r2, r8
	adds	r0, #98
	strb	r2, [r0, #0]
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #21
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d644
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #20
	adds	r1, #204
	bl 0x0200d644
	ldr	r1, [pc, #872]
	movs	r0, #21
	bl 0x0200d64c
	ldr	r1, [pc, #868]
	movs	r0, #20
	bl 0x0200d64c
	movs	r0, #19
	ldr	r1, [pc, #864]
	ldr	r2, [pc, #864]
	bl 0x0200d644
	movs	r1, #135
	movs	r2, #170
	movs	r0, #19
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200d66c
	movs	r2, #10
	movs	r0, #19
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #21
	bl 0x0200d634
	adds	r7, r0, #0
.L_0200247a:
	movs	r0, #1
	bl 0x0200d4fc
	adds	r3, r7, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0200247a
	movs	r1, #128
	movs	r2, #0
	movs	r0, #20
	lsls	r1, r1, #6
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #21
	lsls	r1, r1, #6
	bl 0x0200d704
	movs	r0, #21
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #20
	movs	r1, #2
	bl 0x0200d70c
	movs	r0, #22
	movs	r1, #2
	bl 0x0200d70c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #20
	bl 0x0200d714
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #20
	ldr	r1, [pc, #724]
	adds	r2, #204
	bl 0x0200d644
	movs	r1, #245
	movs	r2, #148
	movs	r0, #20
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d66c
	movs	r1, #224
	movs	r0, #21
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200d6fc
	movs	r1, #160
	movs	r0, #20
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200d6fc
	movs	r1, #224
	movs	r0, #20
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200d6fc
	movs	r1, #204
	movs	r2, #200
	lsls	r1, r1, #6
	lsls	r2, r2, #5
	adds	r1, #51
	adds	r2, #153
	movs	r0, #20
	bl 0x0200d644
	movs	r0, #20
	bl 0x0200d634
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	mov	r8, r3
	ands	r3, r2
	movs	r2, #0
	mov	sl, r2
	movs	r1, #244
	movs	r2, #150
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #20
	bl 0x0200d66c
	movs	r0, #1
	bl 0x0200d604
	movs	r0, #20
	bl 0x0200d634
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #1
	mov	fp, r2
	mov	r2, fp
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #2
	movs	r0, #20
	bl 0x0200d6bc
	movs	r0, #20
	bl 0x0200d634
	adds	r0, #90
	ldrb	r2, [r0, #0]
	mov	r3, r8
	ands	r3, r2
	movs	r1, #243
	movs	r2, #154
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #20
	bl 0x0200d66c
	movs	r0, #1
	bl 0x0200d604
	movs	r0, #20
	bl 0x0200d634
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, fp
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #200
	movs	r0, #204
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200d724
	movs	r0, #128
	movs	r1, #192
	movs	r2, #160
	movs	r3, #1
	lsls	r0, r0, #18
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x0200d72c
	bl 0x0200d734
	movs	r0, #21
	movs	r1, #0
	movs	r2, #40
	bl 0x0200d6fc
	movs	r1, #224
	movs	r2, #20
	movs	r0, #21
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r0, #21
	movs	r1, #0
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #21
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #192
	movs	r0, #22
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	ldr	r6, [pc, #448]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	movs	r1, #160
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #160
	movs	r0, #23
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #160
	movs	r0, #24
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #160
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #160
	movs	r2, #0
	movs	r0, #25
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #20
	bl 0x0200d6f4
	movs	r0, #32
	bl 0x0200d634
	movs	r5, #128
	mov	r2, sl
	adds	r7, r0, #0
	lsls	r5, r5, #9
	str	r2, [r7, #108]
	movs	r0, #1
	bl 0x0200d4fc
	str	r5, [r7, #24]
	str	r5, [r7, #28]
	movs	r0, #32
	movs	r1, #2
	bl 0x0200d6bc
	movs	r0, #32
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #23
	bl 0x0200d714
	movs	r1, #224
	movs	r0, #23
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #25
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #25
	bl 0x0200d714
	movs	r0, #25
	movs	r1, #0
	bl 0x0200d6f4
	movs	r2, #40
	movs	r0, #23
	movs	r1, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #23
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #32
	movs	r1, #3
	bl 0x0200d6b4
	movs	r0, #32
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #192
	movs	r2, #40
	movs	r0, #24
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #24
	lsls	r1, r1, #7
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #24
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #21
	lsls	r1, r1, #6
	bl 0x0200d704
	movs	r1, #129
	movs	r0, #21
	lsls	r1, r1, #1
	bl 0x0200d71c
	movs	r0, #21
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #24
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #25
	bl 0x0200d6fc
	movs	r0, #30
	bl 0x0200d634
	mov	r3, sl
	adds	r7, r0, #0
	str	r3, [r7, #108]
	movs	r0, #1
	bl 0x0200d4fc
	str	r5, [r7, #24]
	str	r5, [r7, #28]
	movs	r0, #31
	bl 0x0200d634
	mov	r2, sl
	adds	r7, r0, #0
	str	r2, [r7, #108]
	movs	r0, #1
	bl 0x0200d4fc
	str	r5, [r7, #24]
	str	r5, [r7, #28]
	movs	r0, #10
	bl 0x0200d604
	movs	r0, #31
	movs	r1, #1
	bl 0x0200d6bc
	movs	r0, #249
	lsls	r0, r0, #5
	adds	r0, #255
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #30
	movs	r1, #2
	bl 0x0200d6bc
	movs	r0, #30
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #20
	movs	r1, #2
	movs	r2, #10
	bl 0x0200d6ac
	movs	r2, #10
	movs	r0, #20
	movs	r1, #4
	b.n	.L_020027b8
	.2byte 0x0000
	.4byte 0x00019999
	.4byte 0x0200df08
	.4byte 0x0200df50
	.4byte 0x00026666
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
.L_020027b8:
	bl 0x0200d6ac
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #20
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #20
	ldr	r1, [pc, #408]
	ldr	r2, [pc, #408]
	bl 0x0200d644
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #396]
	ldr	r2, [pc, #400]
	bl 0x0200d644
	movs	r0, #23
	ldr	r1, [pc, #388]
	ldr	r2, [pc, #388]
	bl 0x0200d644
	ldr	r2, [pc, #384]
	movs	r0, #24
	ldr	r1, [pc, #376]
	bl 0x0200d644
	ldr	r1, [pc, #376]
	movs	r0, #20
	bl 0x0200d64c
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #372]
	bl 0x0200d64c
	ldr	r1, [pc, #368]
	movs	r0, #23
	bl 0x0200d64c
	ldr	r1, [pc, #364]
	movs	r0, #24
	bl 0x0200d65c
	movs	r0, #20
	bl 0x0200d604
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #22
	ldr	r1, [pc, #348]
	adds	r2, #204
	bl 0x0200d644
	movs	r1, #132
	movs	r2, #160
	movs	r0, #22
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200d66c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #22
	lsls	r1, r1, #6
	bl 0x0200d6fc
	movs	r1, #160
	movs	r0, #25
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #22
	movs	r1, #0
	bl 0x0200d6f4
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #25
	ldr	r1, [pc, #284]
	bl 0x0200d644
	ldr	r1, [pc, #284]
	movs	r0, #25
	bl 0x0200d64c
	movs	r1, #243
	movs	r2, #154
	lsls	r2, r2, #1
	movs	r0, #22
	lsls	r1, r1, #1
	bl 0x0200d66c
	movs	r1, #160
	movs	r0, #22
	lsls	r1, r1, #8
	bl 0x0200d704
	ldr	r1, [pc, #224]
	ldr	r2, [pc, #224]
	movs	r0, #21
	bl 0x0200d644
	movs	r0, #21
	bl 0x0200d634
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	ands	r2, r3
	strb	r2, [r0, #0]
	mov	r8, r2
	movs	r1, #237
	movs	r2, #158
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #21
	bl 0x0200d66c
	movs	r0, #1
	bl 0x0200d604
	movs	r0, #21
	bl 0x0200d634
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, fp
	orrs	r2, r3
	movs	r1, #224
	strb	r2, [r0, #0]
	lsls	r1, r1, #8
	movs	r0, #21
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #231
	movs	r2, #140
	lsls	r2, r2, #1
	movs	r0, #22
	lsls	r1, r1, #1
	bl 0x0200d66c
	movs	r0, #22
	movs	r1, #0
	bl 0x0200d704
	movs	r1, #1
	movs	r0, #22
	bl 0x0200d70c
	movs	r0, #22
	bl 0x0200d634
	ldr	r1, [pc, #144]
	bl 0x0200d3e4
	movs	r0, #25
	bl 0x0200d634
	ldr	r1, [pc, #136]
	bl 0x0200d3e4
	movs	r0, #40
	bl 0x0200d604
	movs	r0, #19
	movs	r1, #4
	bl 0x0200d69c
	movs	r1, #0
	movs	r0, #19
	bl 0x0200d6f4
	movs	r0, #78
	bl 0x0200d7dc
	movs	r1, #0
	movs	r0, #0
	bl 0x0200d75c
	movs	r0, #120
	bl 0x0200d76c
	movs	r0, #120
	bl 0x0200d4fc
	movs	r0, #22
	bl 0x0200d634
	bl 0x0200d4e4
	movs	r0, #25
	bl 0x0200d634
	bl 0x0200d4e4
	movs	r0, #80
	bl 0x0200d604
	movs	r0, #2
	bl 0x0200d744
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00026666
	.4byte 0x00013333
	.4byte 0x0200df98
	.4byte 0x0200dfdc
	.4byte 0x0200e004
	.4byte 0x0200e02c
	.4byte 0x00019999
	.4byte 0x0200e070
	.4byte 0x0200ec20
	.2byte 0xeb70
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200d634
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #89
	adds	r0, #85
	strb	r3, [r2, #0]
	movs	r1, #1
	strb	r3, [r0, #0]
	adds	r0, r5, #0
	bl 0x0200d70c
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	sub	sp, #8
	bl 0x0200d60c
	movs	r0, #0
	bl 0x0200d784
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d72c
	movs	r0, #1
	bl 0x0200d4fc
	ldr	r6, [pc, #1016]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	movs	r1, #253
	movs	r2, #140
	lsls	r2, r2, #17
	lsls	r1, r1, #17
	ldr	r0, [r6, #0]
	bl 0x0200d67c
	movs	r0, #1
	bl 0x0200d4fc
	bl 0x0200d774
	bl 0x0200d77c
	movs	r1, #1
	movs	r0, #3
	bl 0x0200d70c
	movs	r0, #7
	bl 0x0200d634
	ldr	r1, [pc, #968]
	bl 0x0200d3e4
	movs	r0, #3
	bl 0x0200d634
	ldr	r1, [pc, #960]
	bl 0x0200d3e4
	movs	r0, #120
	bl 0x0200d604
	movs	r0, #7
	bl 0x0200d634
	bl 0x0200d4e4
	movs	r0, #3
	bl 0x0200d634
	bl 0x0200d4e4
	movs	r0, #40
	bl 0x0200d604
	movs	r0, #7
	movs	r1, #4
	bl 0x0200d69c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200d704
	ldr	r0, [pc, #908]
	bl 0x0200d6dc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #2
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200d6fc
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #3
	bl 0x0200d714
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #3
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #7
	movs	r1, #4
	bl 0x0200d69c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r1, #8
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d714
	movs	r0, #5
	movs	r1, #2
	movs	r2, #10
	bl 0x0200d6ac
	movs	r2, #10
	movs	r0, #5
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #0
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #7
	movs	r1, #4
	bl 0x0200d694
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #20
	movs	r0, #5
	bl 0x0200d714
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r2, #40
	adds	r0, #5
	movs	r1, #0
	bl 0x0200d6ec
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #6
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d704
	movs	r0, #5
	movs	r1, #4
	bl 0x0200d69c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #5
	ldr	r1, [pc, #568]
	bl 0x0200d644
	movs	r0, #5
	movs	r1, #5
	bl 0x0200d694
	movs	r1, #230
	movs	r2, #144
	lsls	r2, r2, #1
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x0200d664
	movs	r0, #5
	movs	r1, #22
	bl 0x0200d694
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #19
	bl 0x0200d714
	movs	r0, #78
	bl 0x0200d7dc
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #6
	lsls	r1, r1, #3
	adds	r0, #51
	adds	r1, #102
	bl 0x0200d724
	movs	r0, #128
	movs	r1, #1
	movs	r2, #160
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	negs	r1, r1
	bl 0x0200d72c
	bl 0x0200d734
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #2
	lsls	r1, r1, #7
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #3
	lsls	r1, r1, #7
	bl 0x0200d704
	movs	r0, #19
	movs	r1, #4
	bl 0x0200d69c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #20
	adds	r0, #19
	movs	r1, #0
	bl 0x0200d6ec
	movs	r1, #1
	movs	r0, #5
	bl 0x0200d694
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	bl 0x0200d704
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #20
	adds	r0, #19
	movs	r1, #0
	bl 0x0200d6ec
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #19
	movs	r1, #3
	bl 0x0200d69c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #0
	movs	r1, #0
	movs	r2, #40
	bl 0x0200d6fc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #204
	adds	r2, #102
	movs	r0, #1
	bl 0x0200d644
	movs	r0, #1
	bl 0x0200d634
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #2
	movs	r2, #164
	strb	r3, [r0, #0]
	adds	r1, #30
	lsls	r2, r2, #1
	movs	r0, #1
	bl 0x0200d66c
	movs	r0, #1
	bl 0x0200d604
	movs	r0, #1
	bl 0x0200d634
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #200
	movs	r0, #204
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r1, #153
	adds	r0, #204
	bl 0x0200d724
	bl 0x0200d73c
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r1, #152
	movs	r0, #135
	movs	r2, #170
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	lsls	r1, r1, #15
	bl 0x0200d72c
	bl 0x0200d734
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #7
	bl 0x0200d704
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r2, #170
	movs	r0, #1
	adds	r1, #30
	lsls	r2, r2, #1
	bl 0x0200d66c
	movs	r0, #1
	movs	r1, #2
	movs	r2, #10
	bl 0x0200d6ac
	movs	r2, #10
	movs	r0, #1
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #40
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200d6fc
	movs	r2, #10
	movs	r0, #1
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #24]
	adds	r1, #153
	b.n	.L_02002de4
	.4byte 0x02000240
	.4byte 0x0200ec20
	.4byte 0x0200eb70
	.4byte 0x00002df5
	.4byte 0x00019999
	.2byte 0xcccc
	.2byte 0x0004
.L_02002de4:
	bl 0x0200d724
	movs	r0, #239
	movs	r1, #192
	movs	r2, #156
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x0200d72c
	bl 0x0200d734
	movs	r1, #128
	movs	r2, #40
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r1, #4
	movs	r2, #20
	adds	r1, #255
	movs	r0, #6
	bl 0x0200d714
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200d704
	movs	r2, #10
	movs	r0, #6
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #6
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #240
	movs	r1, #1
	movs	r2, #138
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200d72c
	bl 0x0200d734
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200d6fc
	movs	r1, #160
	movs	r2, #20
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #7
	movs	r1, #4
	bl 0x0200d69c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #160
	lsls	r0, r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #128
	movs	r1, #1
	movs	r2, #156
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200d72c
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #3
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #208
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r2, #40
	movs	r0, #1
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	movs	r2, #20
	bl 0x0200d6ec
	movs	r1, #6
	movs	r2, #40
	adds	r1, #255
	ldr	r0, [r6, #0]
	bl 0x0200d714
	movs	r0, #0
	movs	r1, #0
	bl 0x0200d704
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #0
	bl 0x0200d714
	movs	r0, #144
	lsls	r0, r0, #8
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #160
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #8
	bl 0x0200d6e4
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200d62c
	cmp	r0, #0
	bne.n	.L_02002f84
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d69c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002fa6
.L_02002f84:
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d69c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #5
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200d6f4
.L_02002fa6:
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #1
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d644
	movs	r1, #137
	movs	r2, #160
	lsls	r2, r2, #1
	movs	r0, #1
	lsls	r1, r1, #2
	bl 0x0200d66c
	movs	r1, #160
	movs	r0, #1
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #1
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #6
	bl 0x0200d714
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #6
	movs	r2, #40
	adds	r1, #255
	movs	r0, #2
	bl 0x0200d714
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #2
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #3
	movs	r1, #4
	bl 0x0200d694
	movs	r0, #3
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #19
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d644
	movs	r1, #249
	movs	r2, #160
	lsls	r2, r2, #1
	movs	r0, #19
	lsls	r1, r1, #1
	bl 0x0200d66c
	movs	r0, #19
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	ldr	r5, [pc, #396]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	bl 0x0200d704
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200d69c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200d55c
	movs	r0, #28
	bl 0x0200d7dc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #0
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d644
	movs	r1, #135
	movs	r2, #143
	lsls	r2, r2, #1
	movs	r0, #0
	lsls	r1, r1, #2
	bl 0x0200d66c
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #0
	bl 0x0200d704
	movs	r0, #7
	bl 0x0200a98c
	movs	r0, #5
	bl 0x0200a98c
	movs	r0, #6
	bl 0x0200a98c
	movs	r0, #19
	bl 0x0200a98c
	movs	r0, #0
	bl 0x0200a98c
	movs	r0, #3
	bl 0x0200a98c
	movs	r0, #2
	bl 0x0200a98c
	movs	r0, #1
	bl 0x0200a98c
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200d70c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200d55c
	movs	r3, #23
	movs	r2, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #40
	movs	r2, #18
	movs	r3, #16
	movs	r0, #39
	bl 0x0200d5ac
	movs	r0, #1
	bl 0x0200d5fc
	movs	r0, #7
	bl 0x0200d624
	movs	r0, #5
	bl 0x0200d624
	movs	r0, #6
	bl 0x0200d624
	movs	r0, #0
	bl 0x0200d624
	movs	r0, #3
	bl 0x0200d624
	movs	r0, #2
	bl 0x0200d624
	movs	r0, #1
	bl 0x0200d624
	movs	r0, #20
	bl 0x0200d634
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r5, #8
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #21
	bl 0x0200d634
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r6, #0
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #28
	bl 0x0200d634
	movs	r1, #15
	bl 0x0200d6d4
	movs	r1, #142
	movs	r2, #137
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r0, #28
	bl 0x0200d67c
	movs	r0, #17
	bl 0x0200d634
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r0, #17
	bl 0x0200d634
	movs	r1, #246
	movs	r2, #192
	movs	r3, #142
	lsls	r1, r1, #17
	lsls	r2, r2, #15
	lsls	r3, r3, #17
	bl 0x0200d584
	movs	r0, #29
	bl 0x0200d634
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r0, #29
	bl 0x0200d634
	movs	r1, #255
	movs	r2, #192
	lsls	r1, r1, #17
	lsls	r2, r2, #15
	ldr	r3, [pc, #68]
	bl 0x0200d584
	movs	r0, #30
	bl 0x0200d634
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r0, #30
	bl 0x0200d634
	movs	r1, #135
	movs	r2, #192
	movs	r3, #150
	lsls	r1, r1, #18
	lsls	r2, r2, #15
	lsls	r3, r3, #17
	bl 0x0200d584
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #190
	lsls	r0, r0, #1
	bl 0x0200d55c
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200d55c
	bl 0x0200d614
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0x0127
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	adds	r5, r0, #0
	movs	r0, #23
	bl 0x0200d634
	adds	r6, r5, #0
	adds	r6, #100
	ldrh	r1, [r6, #0]
	mov	sl, r0
	mov	r8, r1
	mov	r0, r8
	bl 0x0200d524
	ldr	r3, [r5, #48]
	mov	r1, sl
	adds	r3, #3
	adds	r2, r3, #0
	muls	r2, r0
	ldr	r3, [r1, #8]
	mov	r0, r8
	adds	r3, r3, r2
	str	r3, [r5, #8]
	bl 0x0200d51c
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
	mov	r7, r8
	push	{r7}
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200d554
	cmp	r0, #0
	bne.n	.L_0200328c
	ldr	r3, [pc, #44]
	movs	r1, #3
	ldr	r0, [r3, #0]
	bl 0x0200d4f4
	cmp	r0, #0
	bne.n	.L_02003350
.L_0200328c:
	movs	r0, #23
	bl 0x0200d634
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200d554
	cmp	r0, #0
	beq.n	.L_020032b0
	bl 0x0200d514
	adds	r2, r0, #0
	ldr	r3, [r5, #12]
	lsls	r2, r2, #8
	b.n	.L_020032ba
	.2byte 0x122c
	.2byte 0x0300
.L_020032b0:
	bl 0x0200d514
	adds	r2, r0, #0
	ldr	r3, [r5, #12]
	lsls	r2, r2, #6
.L_020032ba:
	lsrs	r2, r2, #16
	lsls	r2, r2, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #124]
	movs	r0, #168
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #2
	bl 0x0200d574
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02003350
	ldr	r1, [pc, #108]
	adds	r0, r7, #0
	ldr	r6, [r7, #80]
	bl 0x0200d56c
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200d6d4
	adds	r3, r7, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	bl 0x0200d514
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
	bl 0x0200d514
	adds	r3, r0, #0
	lsls	r0, r3, #16
	subs	r0, r0, r3
	lsrs	r0, r0, #20
	bl 0x0200d51c
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
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r6, #9]
	b.n	.L_02003350
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffe40000
	.4byte 0x0200eafc
	.4byte 0x0ffff000
	.2byte 0xb211
	.2byte 0x0200
.L_02003350:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x061b2380
	.4byte 0x63c36383
	.4byte 0x23006403
	.4byte 0x62836243
	.4byte 0x306462c3
	.2byte 0x8003
	.2byte 0x4770
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #8
	bl 0x0200d634
	ldr	r5, [pc, #328]
	adds	r6, r0, #0
	ldr	r3, [r5, #0]
	movs	r7, #0
	cmp	r3, #26
	bls.n	.L_0200338c
	b.n	.L_02003506
.L_0200338c:
	ldr	r2, [pc, #316]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0xb400
	.2byte 0x0200
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r3, r4}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r2, r3, r5}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, lr}
	lsls	r0, r0, #8
	push	{r1, r2, r3, r4, r5}
	lsls	r0, r0, #8
	push	{r1, r2, r5, r6}
	lsls	r0, r0, #8
	push	{r3, r5, r7}
	lsls	r0, r0, #8
	movs	r0, #220
	bl 0x0200d7dc
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200d5c4
	ldr	r0, [pc, #184]
	b.n	.L_0200341e
	.2byte 0x2080
	.2byte 0x0240
.L_0200341e:
	movs	r1, #1
	bl 0x0200d75c
	movs	r0, #8
	bl 0x0200d76c
	b.n	.L_02003506
	.4byte 0x21802080
	.4byte 0x02402280
	.4byte 0x02520249
	.4byte 0xf8c4f002
	.4byte 0x2380e063
	.4byte 0x60b3049b
	.4byte 0x1c304b23
	.4byte 0x23cc60f3
	.4byte 0x6133041b
	.4byte 0x025b2380
	.4byte 0x61f361b3
	.4byte 0xff7ef7ff
	.4byte 0x2008491e
	.4byte 0xf8f4f002
	.4byte 0x682be04f
	.4byte 0x602b3b01
	.4byte 0x2b0068f3
	.4byte 0x481add0a
	.4byte 0xf0022100
	.4byte 0x2010f971
	.4byte 0xf976f002
	.4byte 0x3301682b
	.4byte 0xe03e602b
	.4byte 0x22074b15
	.4byte 0x4013681b
	.4byte 0xd1022b00
	.4byte 0xf00220f6
	.4byte 0x68f3f9a1
	.4byte 0x02892190
	.4byte 0x2701185b
	.4byte 0xe02e60f3
	.4byte 0x22a0682b
	.4byte 0x602b3b01
	.4byte 0x68f30352
	.4byte 0xdd174293
	.4byte 0x009b23ba
	.4byte 0x602b33ff
	.4byte 0xf0024808
	.4byte 0xe01ef823
	.4byte 0x0200eccc
	.4byte 0x0200b394
	.4byte 0x002063ff
	.4byte 0xfe980000
	.4byte 0x0200db54
	.4byte 0x00203210
	.4byte 0x0300122c
	.4byte 0x0200b371
	.4byte 0x22074b2c
	.4byte 0x4013681b
	.4byte 0xd1022b00
	.4byte 0xf00220f6
	.4byte 0x68f3f971
	.4byte 0x02892190
	.4byte 0x60f3185b
	.2byte 0x2701
.L_02003506:
	cmp	r7, #0
	beq.n	.L_020035c0
	bl 0x0200d514
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
	bl 0x0200d574
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_020035c0
	ldr	r1, [r7, #80]
	movs	r5, #0
	mov	sl, r1
	ldr	r1, [pc, #104]
	bl 0x0200d56c
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200d6d4
	adds	r3, r7, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	bl 0x0200d514
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
	bl 0x0200d514
	adds	r3, r7, #0
	lsrs	r0, r0, #13
	adds	r3, #98
	strb	r0, [r3, #0]
	ldr	r3, [pc, #56]
	str	r3, [r7, #108]
	bl 0x0200d514
	adds	r3, r0, #0
	lsls	r0, r3, #16
	subs	r0, r0, r3
	lsrs	r0, r0, #20
	bl 0x0200d51c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	str	r3, [r7, #48]
	mov	r1, sl
	movs	r2, #50
	ldrsh	r3, [r6, r2]
	str	r3, [r7, #48]
	mov	r3, r8
	b.n	.L_020035b0
	.4byte 0x00000000
	.4byte 0x0300122c
	.4byte 0xfff80000
	.4byte 0x0200e180
	.4byte 0x0ffff000
	.2byte 0x80e5
	.2byte 0x0200
.L_020035b0:
	ldrb	r2, [r1, #9]
	strb	r3, [r1, #26]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
.L_020035c0:
	ldr	r2, [pc, #28]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	ldr	r2, [pc, #24]
	ldr	r2, [r2, #0]
	cmp	r3, r2
	bls.n	.L_020035d6
	ldr	r0, [pc, #20]
	bl 0x0200d50c
.L_020035d6:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200eccc
	.4byte 0x0200eb68
	.2byte 0xb371
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x0200d634
	movs	r1, #1
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200d70c
	ldr	r1, [r5, #8]
	ldr	r0, [pc, #20]
	ldr	r3, [r5, #16]
	adds	r1, r1, r0
	movs	r0, #128
	lsls	r0, r0, #14
	adds	r3, r3, r0
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	bl 0x0200d584
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb560
	adds	r6, r0, #0
	bl 0x0200d634
	movs	r1, #1
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200d70c
	ldr	r1, [r5, #8]
	movs	r0, #128
	lsls	r0, r0, #13
	ldr	r3, [r5, #16]
	adds	r1, r1, r0
	movs	r0, #128
	lsls	r0, r0, #14
	adds	r3, r3, r0
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	bl 0x0200d584
	pop	{r5, r6, pc}
	push	{lr}
	bl 0x0200d634
	movs	r3, #3
	adds	r0, #85
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r1, r1
	negs	r2, r2
	negs	r0, r0
	sub	sp, #8
	bl 0x0200d72c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r2, #0
	movs	r0, #30
	movs	r1, #0
	bl 0x0200d67c
	movs	r1, #204
	lsls	r1, r1, #6
	adds	r1, #51
	ldr	r0, [pc, #112]
	bl 0x0200d724
	bl 0x0200d73c
	movs	r1, #0
	mov	sl, r1
	adds	r0, #85
	mov	r2, sl
	strb	r2, [r0, #0]
	movs	r1, #192
	movs	r0, #128
	movs	r2, #156
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200d72c
	bl 0x0200d734
	movs	r0, #1
	bl 0x0200d4fc
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r1, #10
	movs	r3, #23
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	movs	r2, #18
	movs	r1, #40
	movs	r0, #18
	mov	fp, r3
	movs	r3, #16
	bl 0x0200d5ac
	movs	r0, #7
	bl 0x0200b648
	movs	r0, #5
	bl 0x0200b648
	movs	r0, #6
	bl 0x0200b648
	movs	r0, #19
	b.n	.L_0200371c
	.4byte 0x00000c08
	.4byte 0x00003f10
	.2byte 0x9999
	.2byte 0x0001
.L_0200371c:
	bl 0x0200b648
	movs	r0, #0
	bl 0x0200b648
	movs	r0, #3
	bl 0x0200b648
	movs	r0, #2
	bl 0x0200b648
	movs	r0, #1
	bl 0x0200b648
	movs	r0, #0
	movs	r1, #0
	bl 0x0200d61c
	ldr	r3, [pc, #1012]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r2, r2, r3
	mov	r9, r2
	ldr	r0, [r2, #0]
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d644
	mov	r3, r9
	movs	r1, #128
	movs	r2, #140
	ldr	r0, [r3, #0]
	lsls	r2, r2, #1
	lsls	r1, r1, #2
	bl 0x0200d66c
	mov	r1, r9
	ldr	r0, [r1, #0]
	movs	r1, #192
	lsls	r1, r1, #8
	bl 0x0200d704
	mov	r2, r9
	ldr	r0, [r2, #0]
	movs	r1, #2
	bl 0x0200d6bc
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #222
	bl 0x0200d5f4
	movs	r0, #4
	bl 0x0200d634
	ldr	r2, [r0, #12]
	movs	r3, #128
	lsls	r3, r3, #12
	ldr	r1, [r0, #8]
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	movs	r0, #234
	adds	r0, #255
	bl 0x0200d574
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_020038ac
	ldr	r6, [r7, #80]
	movs	r3, #33
	ldrb	r2, [r6, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	mov	r1, sl
	ands	r3, r2
	movs	r2, #13
	strb	r1, [r6, #27]
	negs	r2, r2
	adds	r1, r7, #0
	adds	r1, #35
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	ldrb	r2, [r1, #0]
	strb	r3, [r6, #9]
	movs	r3, #254
	ands	r3, r2
	movs	r2, #85
	strb	r3, [r1, #0]
	adds	r2, r2, r7
	mov	r3, sl
	strb	r3, [r2, #0]
	mov	r8, r2
	adds	r2, r7, #0
	adds	r2, #92
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r2, #2
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #0
	bl 0x0200d5b4
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r7, #48]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #204
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #8
	movs	r1, #193
	str	r3, [r7, #72]
	lsls	r1, r1, #3
	movs	r0, #68
	bl 0x0200d52c
	adds	r5, r0, #0
	movs	r0, #247
	bl 0x0200d5ec
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r5, r5, r1
	adds	r2, r5, #0
	movs	r1, #128
	ldrb	r0, [r6, #16]
	bl 0x0200d544
	movs	r0, #68
	bl 0x0200d534
	mov	r2, sl
	mov	r3, r8
	strb	r2, [r3, #0]
	movs	r1, #128
	movs	r2, #128
	movs	r3, #140
	lsls	r2, r2, #16
	lsls	r1, r1, #18
	lsls	r3, r3, #17
	adds	r0, r7, #0
	bl 0x0200d594
	adds	r0, r7, #0
	bl 0x0200d59c
	movs	r3, #4
	mov	r1, r8
	strb	r3, [r1, #0]
	movs	r0, #60
	bl 0x0200d604
	mov	r2, sl
	mov	r3, r8
	strb	r2, [r3, #0]
	movs	r1, #128
	movs	r2, #128
	movs	r3, #132
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	lsls	r3, r3, #17
	adds	r0, r7, #0
	bl 0x0200d594
	adds	r0, r7, #0
	bl 0x0200d59c
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #128
	movs	r2, #160
	movs	r3, #252
	lsls	r1, r1, #18
	lsls	r2, r2, #15
	lsls	r3, r3, #16
	adds	r0, r7, #0
	bl 0x0200d594
	adds	r0, r7, #0
	bl 0x0200d59c
	movs	r0, #20
	bl 0x0200d604
	adds	r0, r7, #0
	bl 0x0200d57c
.L_020038ac:
	movs	r0, #40
	bl 0x0200d604
	movs	r0, #23
	movs	r1, #2
	bl 0x0200d70c
	movs	r0, #24
	movs	r1, #2
	bl 0x0200d70c
	movs	r0, #25
	movs	r1, #2
	bl 0x0200d70c
	movs	r0, #26
	movs	r1, #2
	bl 0x0200d70c
	movs	r1, #2
	movs	r0, #27
	bl 0x0200d70c
	movs	r0, #141
	bl 0x0200d7dc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #9
	lsls	r0, r0, #9
	bl 0x0200d5c4
	movs	r0, #23
	bl 0x0200d634
	movs	r1, #7
	bl 0x0200d6d4
	movs	r0, #23
	bl 0x0200d634
	ldr	r6, [pc, #568]
	movs	r5, #200
	adds	r3, r0, #0
	lsls	r5, r5, #5
	adds	r5, #153
	adds	r3, #85
	mov	r1, sl
	str	r5, [r0, #24]
	str	r6, [r0, #28]
	movs	r2, #128
	strb	r1, [r3, #0]
	movs	r1, #128
	movs	r3, #204
	lsls	r2, r2, #15
	lsls	r3, r3, #16
	lsls	r1, r1, #18
	bl 0x0200d584
	movs	r0, #24
	bl 0x0200d634
	movs	r1, #7
	bl 0x0200d6d4
	movs	r0, #24
	bl 0x0200d634
	adds	r3, r0, #0
	adds	r3, #85
	mov	r2, sl
	str	r5, [r0, #24]
	str	r6, [r0, #28]
	movs	r1, #128
	strb	r2, [r3, #0]
	movs	r2, #192
	movs	r3, #204
	lsls	r2, r2, #15
	lsls	r1, r1, #18
	lsls	r3, r3, #16
	bl 0x0200d584
	ldr	r3, [pc, #488]
	movs	r1, #144
	mov	r8, r3
	lsls	r1, r1, #3
	mov	r0, r8
	bl 0x0200d504
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #208
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #160
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #160
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #160
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #160
	movs	r2, #80
	movs	r0, #1
	lsls	r1, r1, #8
	bl 0x0200d6fc
	ldr	r5, [pc, #384]
	movs	r0, #23
	adds	r1, r5, #0
	bl 0x0200d64c
	adds	r1, r5, #0
	movs	r0, #24
	bl 0x0200d64c
	movs	r0, #145
	bl 0x0200d7dc
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	bl 0x0200d5c4
	movs	r1, #0
	ldr	r0, [pc, #344]
	bl 0x0200d75c
	movs	r0, #16
	bl 0x0200d76c
	movs	r0, #20
	bl 0x0200d4fc
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200d75c
	movs	r0, #24
	bl 0x0200d76c
	movs	r0, #60
	bl 0x0200d4fc
	movs	r0, #141
	bl 0x0200d7dc
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200d55c
	movs	r0, #23
	bl 0x0200d634
	ldr	r3, [pc, #288]
	str	r3, [r0, #12]
	movs	r0, #24
	bl 0x0200d634
	ldr	r3, [pc, #280]
	str	r3, [r0, #12]
	movs	r0, #25
	bl 0x0200d634
	movs	r1, #7
	bl 0x0200d6d4
	movs	r0, #25
	bl 0x0200d634
	adds	r5, r0, #0
	str	r6, [r5, #28]
	movs	r0, #23
	bl 0x0200d634
	ldr	r3, [r0, #24]
	mov	r1, sl
	str	r3, [r5, #24]
	adds	r3, r5, #0
	adds	r3, #85
.L_02003a5e:
	strb	r1, [r3, #0]
	movs	r2, #128
	movs	r1, #128
	movs	r3, #204
	lsls	r2, r2, #12
	lsls	r3, r3, #16
	adds	r0, r5, #0
	lsls	r1, r1, #18
	bl 0x0200d584
	movs	r0, #26
	bl 0x0200d634
	movs	r1, #7
	bl 0x0200d6d4
	movs	r0, #26
	bl 0x0200d634
	adds	r5, r0, #0
	str	r6, [r5, #28]
	movs	r0, #23
	bl 0x0200d634
	ldr	r3, [r0, #24]
	mov	r2, sl
	str	r3, [r5, #24]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r1, #128
	movs	r2, #160
	movs	r3, #204
	lsls	r2, r2, #14
	lsls	r3, r3, #16
	adds	r0, r5, #0
	lsls	r1, r1, #18
	bl 0x0200d584
	movs	r0, #27
	bl 0x0200d634
	movs	r1, #7
	bl 0x0200d6d4
	movs	r0, #27
	bl 0x0200d634
	adds	r5, r0, #0
	str	r6, [r5, #28]
	movs	r0, #23
	bl 0x0200d634
	ldr	r3, [r0, #24]
	mov	r1, sl
	str	r3, [r5, #24]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	movs	r2, #144
	movs	r1, #128
	movs	r3, #204
	adds	r0, r5, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #15
	lsls	r3, r3, #16
	bl 0x0200d584
	mov	r2, fp
	str	r2, [sp, #4]
	movs	r5, #17
	movs	r0, #106
	movs	r1, #36
	movs	r2, #87
	movs	r3, #7
	str	r5, [sp, #0]
	bl 0x0200d5a4
	mov	r3, fp
	str	r3, [sp, #4]
	movs	r0, #42
	movs	r1, #98
	movs	r2, #23
	movs	r3, #69
	str	r5, [sp, #0]
	bl 0x0200d5a4
	mov	r1, fp
	movs	r2, #10
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #40
	movs	r2, #18
	movs	r3, #20
	movs	r0, #0
	bl 0x0200d5ac
	movs	r0, #7
	bl 0x0200b5ec
	movs	r0, #5
	bl 0x0200b5ec
	movs	r0, #6
	bl 0x0200b5ec
	movs	r0, #20
	b.n	.L_02003b54
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xffff0000
	.4byte 0x0200b26d
	.4byte 0x0200e0b4
	.4byte 0x004063ff
	.4byte 0xffc80000
	.2byte 0x0000
	.2byte 0xffe8
.L_02003b54:
	.2byte 0xf7ff
	.2byte 0xfd4a
	.2byte 0x2015
	bl 0x0200b5ec
	movs	r0, #19
	bl 0x0200b5ec
	movs	r0, #2
	bl 0x0200b61c
	movs	r0, #1
	bl 0x0200b61c
	movs	r0, #22
	bl 0x0200b61c
	movs	r0, #0
	bl 0x0200b61c
	movs	r0, #3
	bl 0x0200b61c
	mov	r3, r9
	ldr	r0, [r3, #0]
	movs	r1, #242
	movs	r3, #224
	movs	r2, #156
	lsls	r3, r3, #8
	lsls	r2, r2, #17
	lsls	r1, r1, #17
	bl 0x0200d684
	mov	r1, r9
	ldr	r0, [r1, #0]
	movs	r1, #1
	bl 0x0200d70c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	bl 0x0200d5c4
	movs	r1, #0
	ldr	r0, [pc, #1008]
	bl 0x0200d75c
	movs	r0, #120
	bl 0x0200d76c
	ldr	r5, [pc, #1000]
	movs	r0, #23
	adds	r1, r5, #0
	bl 0x0200d64c
	adds	r1, r5, #0
	movs	r0, #24
	bl 0x0200d64c
	adds	r1, r5, #0
	movs	r0, #25
	bl 0x0200d64c
	adds	r1, r5, #0
	movs	r0, #26
	bl 0x0200d64c
	adds	r1, r5, #0
	movs	r0, #27
	bl 0x0200d64c
	movs	r0, #120
	bl 0x0200d4fc
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	bl 0x0200d5c4
	movs	r1, #0
	ldr	r0, [pc, #940]
	bl 0x0200d75c
	movs	r0, #120
	bl 0x0200d76c
	movs	r0, #120
	bl 0x0200d4fc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	bl 0x0200d5c4
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200d75c
	movs	r0, #120
	bl 0x0200d76c
	movs	r0, #120
	bl 0x0200d4fc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #9
	lsls	r0, r0, #9
	bl 0x0200d5c4
	movs	r0, #78
	bl 0x0200d7dc
	movs	r0, #23
	bl 0x0200d634
	ldr	r5, [pc, #860]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #30
	str	r3, [r0, #28]
	adds	r1, r5, #0
	movs	r0, #24
	bl 0x0200d64c
	movs	r0, #25
	adds	r1, r5, #0
	bl 0x0200d64c
	movs	r0, #26
	adds	r1, r5, #0
	bl 0x0200d64c
	adds	r1, r5, #0
	movs	r0, #27
	bl 0x0200d65c
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200d7dc
	movs	r0, #23
	bl 0x0200d634
	movs	r1, #15
	bl 0x0200d6d4
	movs	r0, #20
	bl 0x0200d604
	ldr	r1, [pc, #796]
	movs	r0, #23
	bl 0x0200d65c
	mov	r0, r8
	bl 0x0200d50c
	movs	r0, #8
	bl 0x0200d634
	adds	r3, r0, #0
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	movs	r1, #1
	bl 0x0200d5bc
	movs	r0, #8
	bl 0x0200d634
	movs	r1, #7
	bl 0x0200d6d4
	movs	r0, #8
	movs	r1, #2
	bl 0x0200d70c
	ldr	r3, [pc, #744]
	ldr	r2, [pc, #748]
	mov	r1, sl
	str	r1, [r3, #0]
	movs	r3, #240
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #740]
	lsls	r1, r1, #3
	bl 0x0200d504
.L_02003cea:
	movs	r0, #1
	bl 0x0200d4fc
	ldr	r3, [pc, #716]
	ldr	r3, [r3, #0]
	cmp	r3, #100
	bls.n	.L_02003cea
	movs	r0, #72
	bl 0x0200d7dc
	movs	r0, #120
	bl 0x0200d604
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #3
	bl 0x0200d714
	ldr	r6, [pc, #696]
	adds	r0, r6, #0
	bl 0x0200d6dc
	movs	r0, #3
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #6
	movs	r2, #40
	adds	r1, #255
	movs	r0, #2
	bl 0x0200d714
	movs	r0, #2
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #7
	bl 0x0200d714
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #4
	movs	r2, #40
	adds	r1, #255
	movs	r0, #1
	bl 0x0200d714
	movs	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	ldr	r5, [pc, #608]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #20
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d694
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #19
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #3
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #0
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #1
	movs	r1, #3
	bl 0x0200d694
	movs	r1, #3
	movs	r0, #2
	bl 0x0200d69c
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #176
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #224
	movs	r2, #0
	movs	r0, #1
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r0, #0
	movs	r1, #0
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200d6ec
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200d5c4
	bl 0x0200d5cc
	movs	r0, #23
	movs	r1, #24
	bl 0x0200d78c
	adds	r0, r6, #6
	movs	r1, #1
	bl 0x0200d5e4
	bl 0x0200d794
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #23
	movs	r1, #24
	bl 0x0200d78c
	adds	r0, r6, #7
	movs	r1, #1
	bl 0x0200d5e4
	bl 0x0200d794
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	mov	r8, r1
	add	r2, r8
	mov	sl, r3
	ldrh	r3, [r2, #0]
	movs	r1, #0
	adds	r3, #2
	strh	r3, [r2, #0]
	mov	r9, r1
	ldr	r0, [r5, #0]
	bl 0x02008610
	movs	r0, #7
	bl 0x02008610
	movs	r0, #5
	bl 0x02008610
	movs	r0, #6
	bl 0x02008610
	movs	r0, #19
	bl 0x02008610
	movs	r0, #1
	bl 0x02008610
	movs	r0, #2
	bl 0x02008610
	movs	r0, #3
	bl 0x02008610
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #40
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200d704
	movs	r0, #192
	movs	r2, #10
	lsls	r0, r0, #7
	movs	r1, #0
	bl 0x0200d6ec
	adds	r6, #9
	movs	r0, #23
	movs	r1, #24
	bl 0x0200d78c
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200d5e4
	bl 0x0200d794
	movs	r0, #3
	bl 0x02008640
	movs	r0, #192
	lsls	r0, r0, #7
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r0, #3
	bl 0x0200d704
	movs	r0, #3
	bl 0x0200d634
	mov	r2, r9
	str	r2, [r0, #108]
	mov	r3, sl
	ldr	r2, [r3, #108]
	movs	r1, #128
	add	r2, r8
	ldrh	r3, [r2, #0]
	lsls	r1, r1, #1
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #3
	movs	r2, #20
	bl 0x0200d714
	movs	r2, #20
	movs	r1, #0
	movs	r0, #3
	bl 0x0200d6ec
	movs	r0, #201
	bl 0x0200d7dc
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200d75c
	movs	r0, #20
	b.n	.L_02003fd4
	.4byte 0x004063ff
	.4byte 0x0200e0fc
	.4byte 0x00203210
	.4byte 0x0200e120
	.4byte 0x0200e150
	.4byte 0x0200eccc
	.4byte 0x0200eb68
	.4byte 0x0200b371
	.4byte 0x00002e24
	.2byte 0x0240
	.2byte 0x0200
.L_02003fd4:
	bl 0x0200d76c
	movs	r0, #20
	bl 0x0200d4fc
	movs	r0, #3
	bl 0x0200d744
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	bl 0x0200d60c
	movs	r0, #0
	bl 0x0200d784
	ldr	r6, [pc, #80]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r6, r2
	ldr	r0, [r6, #0]
	bl 0x0200d634
	movs	r5, #128
	ldr	r3, [pc, #60]
	lsls	r5, r5, #7
	strh	r5, [r0, #6]
	movs	r0, #7
	mov	sl, r3
	bl 0x0200d634
	strh	r5, [r0, #6]
	movs	r0, #5
	bl 0x0200d634
	strh	r5, [r0, #6]
	movs	r0, #6
	bl 0x0200d634
	strh	r5, [r0, #6]
	movs	r0, #19
	bl 0x0200d634
	movs	r2, #192
	lsls	r2, r2, #6
	mov	r8, r2
	mov	r3, r8
	strh	r3, [r0, #6]
	movs	r0, #0
	bl 0x0200d634
	strh	r5, [r0, #6]
	movs	r0, #1
	b.n	.L_0200405c
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x0240
	.2byte 0x0200
.L_0200405c:
	bl 0x0200d634
	strh	r5, [r0, #6]
	movs	r0, #2
	bl 0x0200d634
	strh	r5, [r0, #6]
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #20
	movs	r1, #8
	bl 0x0200d694
	movs	r0, #21
	movs	r1, #8
	bl 0x0200d694
	movs	r0, #22
	movs	r1, #7
	bl 0x0200d694
	movs	r1, #249
	movs	r2, #160
	movs	r0, #19
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r2, #160
	movs	r0, #1
	ldr	r1, [pc, #1020]
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r1, #253
	movs	r2, #140
	ldr	r0, [r6, #0]
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d67c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d72c
	bl 0x0200d73c
	mov	r2, sl
	adds	r0, #85
	strb	r2, [r0, #0]
	movs	r1, #192
	movs	r0, #128
	movs	r2, #144
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #0
	lsls	r0, r0, #18
	bl 0x0200d72c
	bl 0x0200d58c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	bl 0x0200d774
	bl 0x0200d77c
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	ldr	r0, [r6, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #40
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200d704
	ldr	r0, [pc, #800]
	bl 0x0200d6dc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #192
	movs	r2, #20
	movs	r0, #3
	lsls	r1, r1, #7
	bl 0x0200d6fc
	movs	r0, #3
	movs	r1, #4
	bl 0x0200d69c
	movs	r2, #10
	movs	r0, #19
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #7
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200d6fc
	ldr	r0, [r6, #0]
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #2
	lsls	r1, r1, #7
	bl 0x0200d6fc
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200d724
	movs	r0, #128
	movs	r1, #1
	movs	r2, #160
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	negs	r1, r1
	bl 0x0200d72c
	bl 0x0200d734
	movs	r1, #208
	movs	r0, #19
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #19
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d694
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #0
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #3
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #2
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #1
	movs	r1, #3
	bl 0x0200d69c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200d71c
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #176
	movs	r0, #19
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #19
	movs	r1, #3
	bl 0x0200d69c
	movs	r1, #4
	movs	r2, #20
	adds	r1, #255
	movs	r0, #1
	bl 0x0200d714
	movs	r1, #0
	movs	r0, #1
	bl 0x0200d6f4
	movs	r0, #28
	bl 0x0200d634
	movs	r1, #15
	bl 0x0200d6d4
	movs	r1, #128
	movs	r2, #180
	lsls	r2, r2, #17
	lsls	r1, r1, #18
	movs	r0, #28
	bl 0x0200d67c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200d764
	movs	r0, #23
	movs	r1, #24
	bl 0x0200d78c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200d6f4
	bl 0x0200d794
	movs	r0, #2
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r2, #10
	movs	r0, #2
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #2
	movs	r1, #0
	bl 0x0200d6f4
	ldr	r0, [r6, #0]
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #7
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #5
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #6
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #19
	mov	r1, r8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #1
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r2, #0
	movs	r0, #3
	adds	r1, r5, #0
	bl 0x0200d6fc
	movs	r0, #0
	adds	r1, r5, #0
	bl 0x0200d704
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #0
	bl 0x0200d714
	movs	r0, #0
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #23
	movs	r1, #24
	bl 0x0200d78c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200d6f4
	bl 0x0200d794
	ldr	r0, [r6, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #20
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200d6fc
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #0
	movs	r1, #3
	bl 0x0200d69c
	movs	r2, #0
	ldr	r0, [r6, #0]
	adds	r1, r5, #0
	bl 0x0200d6fc
	movs	r0, #0
	adds	r1, r5, #0
	bl 0x0200d704
	movs	r0, #23
	movs	r1, #24
	bl 0x0200d78c
	movs	r1, #0
	movs	r0, #28
	bl 0x0200d6f4
	bl 0x0200d794
	movs	r0, #5
	bl 0x0200d634
	movs	r3, #192
	lsls	r3, r3, #7
	adds	r7, r0, #0
	strh	r3, [r7, #6]
	movs	r1, #22
	movs	r0, #5
	bl 0x0200d694
	movs	r0, #10
	bl 0x0200d604
	movs	r0, #5
	movs	r1, #2
	bl 0x0200d6b4
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #23
	movs	r1, #24
	bl 0x0200d78c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200d6f4
	bl 0x0200d794
	strh	r5, [r7, #6]
	movs	r0, #5
	movs	r1, #1
	bl 0x0200d694
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200d714
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #23
	movs	r1, #24
	bl 0x0200d78c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200d6f4
	bl 0x0200d794
	movs	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #23
	movs	r1, #24
	bl 0x0200d78c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200d6f4
	bl 0x0200d794
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #1
	bl 0x0200d714
	movs	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #23
	movs	r1, #24
	bl 0x0200d78c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200d6f4
	bl 0x0200d794
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200d714
	movs	r0, #19
	b.n	.L_020044a0
	.2byte 0x0000
	.4byte 0x021a0000
	.2byte 0x2e44
	.2byte 0x0000
.L_020044a0:
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #23
	movs	r1, #24
	bl 0x0200d78c
	movs	r1, #0
	movs	r0, #28
	bl 0x0200d6f4
	bl 0x0200d794
	movs	r0, #40
	bl 0x0200d604
	movs	r0, #128
	movs	r1, #1
	movs	r2, #148
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200d72c
	movs	r1, #160
	movs	r0, #19
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200d6fc
	movs	r2, #40
	movs	r0, #19
	mov	r1, r8
	bl 0x0200d6fc
	movs	r1, #208
	movs	r0, #19
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #19
	movs	r1, #0
.L_020044f6:
	bl 0x0200d6f4
	movs	r1, #128
.L_020044fc:
	movs	r0, #5
	lsls	r1, r1, #6
	bl 0x0200d704
	movs	r0, #128
	lsls	r0, r0, #7
.L_02004508:
	adds	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #0
.L_02004514:
	lsls	r1, r1, #8
	bl 0x0200d704
.L_0200451a:
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
.L_02004520:
	bl 0x0200d6f4
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d704
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #2
	bl 0x0200d714
	movs	r1, #0
	movs	r0, #2
	bl 0x0200d6e4
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200d704
	movs	r1, #0
	ldr	r0, [r6, #0]
	bl 0x0200d62c
	ldr	r0, [r6, #0]
	bl 0x0200d6a4
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #4
	bl 0x0200d75c
	movs	r0, #40
	bl 0x0200d76c
	movs	r0, #78
	bl 0x0200d7dc
	movs	r0, #141
	bl 0x0200d7dc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #9
	lsls	r0, r0, #9
	bl 0x0200d5c4
	movs	r0, #20
	bl 0x0200d604
	ldr	r0, [r6, #0]
	bl 0x02008610
	movs	r0, #5
	bl 0x02008610
	movs	r0, #6
	bl 0x02008610
	movs	r0, #7
	bl 0x02008610
	movs	r0, #19
	bl 0x02008610
	movs	r0, #0
	bl 0x02008610
	movs	r0, #1
	bl 0x02008610
	movs	r0, #2
	bl 0x02008610
	movs	r0, #3
	bl 0x02008610
	movs	r0, #20
	bl 0x0200d604
	movs	r1, #204
	lsls	r1, r1, #6
	ldr	r0, [pc, #1008]
	adds	r1, #51
	bl 0x0200d724
	movs	r0, #128
	movs	r1, #1
	movs	r2, #144
	lsls	r2, r2, #17
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x0200d72c
	bl 0x0200d734
	ldr	r0, [pc, #984]
	bl 0x0200d50c
	movs	r0, #140
	bl 0x0200d7dc
	movs	r0, #8
	bl 0x0200d634
	movs	r1, #12
	bl 0x0200d6d4
	movs	r0, #9
	bl 0x0200d634
	movs	r1, #12
	bl 0x0200d6d4
	movs	r1, #2
	movs	r0, #23
	bl 0x0200d70c
	movs	r0, #23
	bl 0x0200d634
	movs	r1, #7
	bl 0x0200d6d4
	movs	r0, #23
	bl 0x0200d634
	ldr	r3, [pc, #924]
	adds	r7, r0, #0
	str	r3, [r7, #28]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #24]
	adds	r3, r7, #0
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r2, #128
	movs	r1, #128
	movs	r3, #204
	lsls	r1, r1, #18
	lsls	r2, r2, #15
	lsls	r3, r3, #16
	bl 0x0200d584
	movs	r6, #0
.L_0200466c:
	cmp	r6, #20
	bne.n	.L_0200472a
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200d5c4
	ldr	r5, [pc, #856]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200d714
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d714
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d714
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200d714
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #19
	bl 0x0200d714
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #0
	bl 0x0200d714
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #1
	bl 0x0200d714
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #2
	bl 0x0200d714
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #3
	bl 0x0200d714
	ldr	r0, [r5, #0]
	bl 0x02008640
	movs	r0, #5
	bl 0x02008640
	movs	r0, #6
	bl 0x02008640
	movs	r0, #7
	bl 0x02008640
	movs	r0, #19
	bl 0x02008640
	movs	r0, #0
	bl 0x02008640
	movs	r0, #1
	bl 0x02008640
	movs	r0, #2
	bl 0x02008640
	movs	r0, #3
	bl 0x02008640
.L_0200472a:
	ldr	r3, [r7, #24]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r7, #24]
	movs	r0, #1
	adds	r6, #1
	bl 0x0200d4fc
	cmp	r6, #39
	bls.n	.L_0200466c
	movs	r0, #63
	bl 0x0200d7dc
	ldr	r1, [pc, #664]
	movs	r0, #23
	bl 0x0200d64c
	movs	r0, #80
	bl 0x0200d604
	movs	r0, #1
	movs	r1, #4
	movs	r2, #10
	bl 0x0200d6ac
	movs	r2, #10
	movs	r0, #1
	movs	r1, #6
	bl 0x0200d6ac
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	ldr	r5, [pc, #572]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200d6fc
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r2, #20
	movs	r0, #3
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #3
	movs	r1, #0
	bl 0x0200d6f4
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #20
	movs	r0, #3
	lsls	r1, r1, #8
	bl 0x0200d6fc
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d694
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #0
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #3
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #2
	movs	r1, #3
	bl 0x0200d694
	movs	r0, #1
	movs	r1, #3
	bl 0x0200d69c
	movs	r0, #128
	movs	r1, #1
	movs	r2, #156
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200d72c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200d67c
	movs	r0, #9
	bl 0x0200d634
	movs	r1, #7
	bl 0x0200d6d4
	movs	r1, #1
	movs	r0, #9
	bl 0x0200d70c
	movs	r0, #9
	bl 0x0200d634
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #140
	bl 0x0200d7dc
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	bl 0x0200d5c4
	movs	r1, #0
	ldr	r0, [pc, #100]
	bl 0x0200d75c
	movs	r0, #40
	bl 0x0200d76c
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #176
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #7
	movs	r2, #0
	b.n	.L_020049e8
	.4byte 0x00019999
	.4byte 0x020084a5
	.4byte 0xffff0000
	.4byte 0x02000240
	.4byte 0x0200e1d4
	.2byte 0x3210
	.2byte 0x0020
.L_020049e8:
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #80
	bl 0x0200d6fc
	movs	r0, #1
	movs	r1, #4
	movs	r2, #0
	bl 0x0200d6ac
	ldr	r1, [pc, #416]
	ldr	r2, [pc, #420]
	movs	r0, #1
	bl 0x0200d644
	movs	r0, #1
	bl 0x0200d634
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #2
	movs	r2, #164
	strb	r3, [r0, #0]
	adds	r1, #30
	lsls	r2, r2, #1
	movs	r0, #1
	bl 0x0200d66c
	movs	r0, #1
	bl 0x0200d604
	movs	r0, #1
	bl 0x0200d634
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r0, #1
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #7
	movs	r2, #40
	bl 0x0200d6fc
	movs	r1, #192
	movs	r0, #3
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200d6fc
	movs	r1, #4
	movs	r2, #0
	adds	r1, #255
	movs	r0, #0
	bl 0x0200d714
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200d704
	movs	r2, #10
	movs	r0, #0
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #1
	movs	r1, #4
	bl 0x0200d694
	movs	r0, #1
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d6fc
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #3
	bl 0x0200d714
	movs	r0, #3
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200d71c
	movs	r0, #20
	bl 0x0200d604
	movs	r0, #7
	movs	r1, #0
	bl 0x0200d6f4
	movs	r2, #10
	movs	r0, #5
	movs	r1, #4
	bl 0x0200d6ac
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d6f4
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #2
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #208
	movs	r0, #19
	lsls	r1, r1, #8
	bl 0x0200d704
	movs	r0, #19
	movs	r1, #2
	movs	r2, #10
	bl 0x0200d6ac
	movs	r2, #10
	movs	r0, #19
	movs	r1, #4
	bl 0x0200d6ac
	movs	r1, #0
	movs	r0, #19
	bl 0x0200d6f4
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x0200d7dc
	movs	r0, #78
	bl 0x0200d7dc
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #140
	bl 0x0200d7dc
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200d75c
	movs	r0, #20
	bl 0x0200d76c
	movs	r0, #240
	bl 0x0200d4fc
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r6, #0
	strh	r6, [r3, #0]
	movs	r1, #0
	movs	r0, #0
	bl 0x0200d75c
	movs	r0, #8
	bl 0x0200d76c
	movs	r0, #8
	bl 0x0200d4fc
	movs	r0, #4
	bl 0x0200d744
	pop	{r3, r5}
	mov	r8, r3
.L_02004bb6:
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00033333
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, lr}
	bl 0x0200d60c
	movs	r0, #0
	bl 0x0200d784
	movs	r1, #160
	lsls	r1, r1, #7
.L_02004bd4:
	movs	r0, #19
	bl 0x0200d704
.L_02004bda:
	ldr	r0, [pc, #72]
	bl 0x0200d6dc
.L_02004be0:
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	ldr	r5, [pc, #56]
	movs	r3, #133
	lsls	r3, r3, #2
	movs	r1, #204
	movs	r2, #204
	adds	r5, r5, r3
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d644
	movs	r1, #240
	movs	r2, #174
	ldr	r0, [r5, #0]
	lsls	r2, r2, #1
	lsls	r1, r1, #1
	bl 0x0200d66c
	movs	r1, #176
	movs	r0, #19
	lsls	r1, r1, #8
	bl 0x0200d704
	bl 0x0200d614
	pop	{r5, pc}
	.4byte 0x00002e22
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200d60c
	movs	r0, #0
	bl 0x0200d784
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #19
	bl 0x0200d704
	ldr	r0, [pc, #72]
	bl 0x0200d6dc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #19
	movs	r1, #0
	bl 0x0200d6f4
	ldr	r5, [pc, #56]
	movs	r3, #133
	lsls	r3, r3, #2
	movs	r1, #204
	movs	r2, #204
	adds	r5, r5, r3
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d644
	movs	r1, #136
	movs	r2, #174
	ldr	r0, [r5, #0]
	lsls	r2, r2, #1
	lsls	r1, r1, #2
	bl 0x0200d66c
	movs	r1, #176
	movs	r0, #19
	lsls	r1, r1, #8
	bl 0x0200d704
	bl 0x0200d614
	pop	{r5, pc}
	.4byte 0x00002e22
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200d60c
	movs	r0, #0
	bl 0x0200d784
	movs	r1, #128
	movs	r2, #232
	lsls	r2, r2, #16
	lsls	r1, r1, #18
	movs	r0, #18
	bl 0x0200d67c
	movs	r0, #1
	bl 0x0200d4fc
	ldr	r0, [pc, #84]
	bl 0x0200d6dc
	movs	r1, #0
	movs	r0, #18
	bl 0x0200d6e4
	ldr	r3, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200d62c
	cmp	r0, #0
	bne.n	.L_02004cea
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d67c
	movs	r0, #1
	bl 0x0200d4fc
	bl 0x0200b658
.L_02004cea:
	movs	r1, #0
	movs	r2, #0
	movs	r0, #18
	bl 0x0200d67c
	movs	r0, #1
	bl 0x0200d4fc
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200d55c
	bl 0x0200d614
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00002e23
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200d564
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	movs	r1, #128
	ldr	r3, [pc, #28]
	lsls	r1, r1, #2
	adds	r1, #106
	adds	r3, r3, r1
	movs	r1, #0
.L_02004d38:
	ldrsh	r3, [r3, r1]
	cmp	r3, r0
	bne.n	.L_02004d4a
.L_02004d3e:
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #138
.L_02004d44:
	adds	r2, r2, r3
	movs	r3, #1
	strb	r3, [r2, #0]
.L_02004d4a:
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
.L_02004d50:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
.L_02004d56:
	ldr	r1, [r3, #60]
	movs	r4, #128
	ldr	r3, [pc, #60]
.L_02004d5c:
	lsls	r4, r4, #2
	adds	r4, #106
	adds	r2, r3, r4
.L_02004d62:
	movs	r3, #0
	ldrsh	r6, [r2, r3]
	ldrh	r4, [r2, #0]
.L_02004d68:
	cmp	r6, r0
	bne.n	.L_02004d9c
	movs	r0, #152
	lsls	r0, r0, #5
	adds	r0, #138
	adds	r3, r1, r0
	movs	r5, #0
	strb	r5, [r3, #0]
	ldr	r3, [pc, #24]
	adds	r0, r6, #0
	orrs	r3, r4
	strh	r3, [r2, #0]
	bl 0x0200d634
	str	r5, [r0, #108]
	adds	r0, r6, #0
	bl 0x0200d634
	movs	r1, #0
	bl 0x0200d6d4
	b.n	.L_02004d9c
	.4byte 0x0000ffff
	.2byte 0x0240
	.2byte 0x0200
.L_02004d9c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	bl 0x0200d60c
	movs	r0, #0
	bl 0x0200d784
	movs	r0, #2
	bl 0x0200cd24
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #2
	bl 0x0200d6c4
	ldr	r0, [pc, #36]
	bl 0x0200d6dc
	movs	r0, #2
	movs	r1, #0
	bl 0x0200d6f4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #2
	bl 0x0200d704
	movs	r0, #2
	bl 0x0200cd50
	bl 0x0200d614
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x2e19
	.2byte 0x0000
	push	{lr}
	bl 0x0200d60c
	movs	r0, #0
	bl 0x0200d784
	movs	r0, #20
	bl 0x0200cd24
	ldr	r0, [pc, #24]
	bl 0x0200d6dc
	movs	r1, #0
	movs	r0, #20
	bl 0x0200d6f4
	movs	r0, #20
	bl 0x0200cd50
	bl 0x0200d614
	pop	{pc}
	.2byte 0x2e20
	.2byte 0x0000
	push	{lr}
	bl 0x0200d60c
	movs	r0, #0
	bl 0x0200d784
	movs	r0, #22
	bl 0x0200cd24
	ldr	r0, [pc, #24]
	bl 0x0200d6dc
	movs	r1, #0
	movs	r0, #22
	bl 0x0200d6f4
	movs	r0, #22
	bl 0x0200cd50
	bl 0x0200d614
	pop	{pc}
	.2byte 0x2e21
	.2byte 0x0000
	push	{lr}
	bl 0x0200d60c
	movs	r0, #0
	bl 0x0200d784
	movs	r0, #21
	bl 0x0200cd24
	ldr	r0, [pc, #24]
	bl 0x0200d6dc
	movs	r1, #0
	movs	r0, #21
	bl 0x0200d6f4
	movs	r0, #21
	bl 0x0200cd50
	bl 0x0200d614
	pop	{pc}
	.2byte 0x2e1f
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r6, r1, #0
	bl 0x0200d634
	ldr	r3, [pc, #44]
	movs	r1, #181
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r2, r3, r1
	movs	r5, #0
	strb	r5, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #106
	adds	r3, r3, r2
	mov	r8, r0
	strh	r6, [r3, #0]
	adds	r0, r6, #0
	bl 0x0200d634
	mov	r1, r8
	ldr	r3, [r1, #108]
	str	r3, [r0, #108]
	str	r5, [r1, #108]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #28
	movs	r1, #2
	bl 0x0200ce80
	pop	{pc}
	push	{lr}
	movs	r0, #17
	movs	r1, #20
	bl 0x0200ce80
	pop	{pc}
	push	{lr}
	movs	r0, #30
	movs	r1, #22
	bl 0x0200ce80
	pop	{pc}
	push	{lr}
	movs	r0, #29
	movs	r1, #21
	bl 0x0200ce80
	pop	{pc}
	push	{r5, r6, lr}
	adds	r4, r1, #0
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r1, #128
	ldr	r6, [r1, #0]
	ldr	r1, [pc, #148]
	adds	r5, r0, #0
	str	r5, [r1, #0]
	ldr	r1, [pc, #148]
	str	r4, [r1, #0]
	ldr	r1, [pc, #148]
	str	r2, [r1, #0]
	ldr	r2, [pc, #148]
	str	r3, [r2, #0]
	movs	r2, #255
	ldrh	r3, [r5, #0]
	b.n	.L_02004f3a
.L_02004f14:
	ldrh	r0, [r4, #0]
	adds	r4, #2
	ldrh	r2, [r4, #0]
	adds	r4, #2
	ldrh	r1, [r5, #0]
	ldrh	r3, [r4, #0]
	adds	r5, #2
	adds	r4, #2
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	movs	r2, #160
	lsls	r2, r2, #19
	lsls	r1, r1, #1
	orrs	r3, r0
	adds	r1, r1, r2
	strh	r3, [r1, #0]
	ldrh	r3, [r5, #0]
	movs	r2, #255
.L_02004f3a:
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_02004f48
	ldrh	r3, [r4, #0]
	cmp	r3, r2
	bne.n	.L_02004f14
.L_02004f48:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r0, #160
	lsls	r2, r2, #24
	adds	r3, #212
	lsls	r0, r0, #19
	adds	r1, r6, #0
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r1, r6, r2
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [pc, #56]
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200d75c
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_02004f92
	bl 0x0200d1e0
.L_02004f92:
	pop	{r5, r6, pc}
	.4byte 0x0200ece0
	.4byte 0x0200ece4
	.4byte 0x0200ece8
	.4byte 0x0200ecd4
	.4byte 0x05000200
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #44]
	ldr	r5, [pc, #64]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #64]
	ldr	r0, [r3, #0]
	bl 0x0200d08c
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #32]
	strh	r0, [r5, #0]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #28]
	movs	r1, #144
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #24]
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #48]
	bl 0x0200d504
	b.n	.L_0200500c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0x0200ecf0
	.4byte 0x0200ecec
	.4byte 0x0200ece0
	.4byte 0x0200ecdc
	.4byte 0x0200ecd8
	.4byte 0x0200ecd0
	.2byte 0xd0b1
	.2byte 0x0200
.L_0200500c:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	ldr	r0, [pc, #8]
	bl 0x0200d50c
	pop	{pc}
	.2byte 0x0000
	.2byte 0xd0b1
	.2byte 0x0200
	push	{r5, lr}
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #40]
	ldr	r5, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	ldr	r0, [r3, #0]
	bl 0x0200d08c
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #48]
	strh	r0, [r5, #0]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #48]
	movs	r1, #144
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #20]
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #40]
	bl 0x0200d504
	b.n	.L_02005088
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0200ecf0
	.4byte 0x0200ecec
	.4byte 0x0200ece0
	.4byte 0x0200ecdc
	.4byte 0x0200ecd8
	.4byte 0x0200ecd0
	.2byte 0xd0b1
	.2byte 0x0200
.L_02005088:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r1, [pc, #20]
	ldrh	r3, [r0, #0]
	movs	r2, #0
	cmp	r3, r1
	beq.n	.L_020050a8
.L_02005098:
	adds	r0, #2
	ldrh	r3, [r0, #0]
	adds	r2, #1
	cmp	r3, r1
	bne.n	.L_02005098
	b.n	.L_020050a8
	.2byte 0xffff
	.2byte 0x0000
.L_020050a8:
	subs	r2, #1
	adds	r0, r2, #0
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r1, [pc, #172]
	movs	r4, #0
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_020050ea
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r2, r0
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_020050ea
	ldr	r0, [pc, #148]
	movs	r4, #1
	ldrh	r2, [r0, #0]
	strh	r2, [r1, #0]
	movs	r1, #128
	lsls	r3, r2, #16
.L_020050da:
	lsls	r1, r1, #10
	cmp	r3, r1
	bls.n	.L_020050ea
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r2, r1
	strh	r3, [r0, #0]
.L_020050ea:
	cmp	r4, #0
	bne.n	.L_020050f0
	b.n	.L_020051dc
.L_020050f0:
	ldr	r3, [pc, #116]
	ldr	r6, [pc, #120]
	ldr	r1, [r3, #0]
	ldrh	r3, [r6, #0]
	movs	r5, #0
	cmp	r5, r3
	bcs.n	.L_0200513e
	ldr	r3, [pc, #112]
	ldr	r2, [pc, #112]
	ldr	r7, [r3, #0]
	mov	lr, r2
	mov	ip, r6
.L_02005108:
	mov	r3, lr
	ldrh	r2, [r3, #0]
	ldrh	r3, [r6, #0]
	movs	r0, #160
	muls	r3, r2
	adds	r3, r3, r5
	lsls	r3, r3, #1
	ldrh	r3, [r3, r7]
	lsls	r0, r0, #19
	lsls	r3, r3, #1
	adds	r4, r3, r0
	ldrh	r0, [r1, #0]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	adds	r1, #2
	ldrh	r3, [r1, #0]
	adds	r1, #2
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r4, #0]
	adds	r5, #1
	mov	r2, ip
	ldrh	r3, [r2, #0]
	cmp	r5, r3
	bcc.n	.L_02005108
.L_0200513e:
	ldr	r3, [pc, #44]
	movs	r0, #160
	ldrh	r1, [r3, #0]
	ldr	r3, [pc, #48]
	lsls	r2, r1, #1
	ldr	r3, [r3, #0]
	lsls	r0, r0, #19
	ldrh	r3, [r2, r3]
	adds	r2, r2, r1
.L_02005150:
	lsls	r3, r3, #1
	adds	r4, r3, r0
	ldr	r3, [pc, #36]
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02005184
	ldr	r3, [pc, #32]
	b.n	.L_02005186
	.4byte 0x0200ecd8
	.4byte 0x0200ecdc
	.4byte 0x0200ece8
	.4byte 0x0200ecec
	.4byte 0x0200ecd4
	.4byte 0x0200ecf0
	.4byte 0x0200ece0
	.4byte 0x0200ecd0
	.2byte 0xece4
	.2byte 0x0200
.L_02005184:
	ldr	r3, [pc, #68]
.L_02005186:
	lsls	r2, r2, #1
	ldr	r3, [r3, #0]
	adds	r1, r3, r2
	ldrh	r0, [r1, #0]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	ldrh	r3, [r1, #2]
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r4, #0]
	ldr	r1, [pc, #48]
	ldr	r2, [pc, #32]
	ldrh	r3, [r1, #0]
	eors	r3, r2
	strh	r3, [r1, #0]
	ldr	r1, [pc, #40]
	ldr	r2, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r3, #1
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	ldrh	r2, [r2, #0]
	lsrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_020051dc
	ldr	r3, [pc, #8]
	strh	r3, [r1, #0]
	b.n	.L_020051dc
	.2byte 0x0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0200ece8
	.4byte 0x0200ecd0
	.4byte 0x0200ecf0
	.2byte 0xecec
	.2byte 0x0200
.L_020051dc:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r5, [r6, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	mov	r8, r1
	add	r5, r8
	ldr	r2, [r5, #0]
	ldr	r0, [pc, #100]
	movs	r1, #1
	mov	sl, r2
	bl 0x0200d75c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x0200d75c
	ldr	r2, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r1, r8
	str	r3, [r2, r1]
	bl 0x0200d774
	bl 0x0200d77c
	bl 0x0200d030
	bl 0x0200d7c4
	movs	r0, #40
	bl 0x0200d4fc
	bl 0x0200d010
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl 0x0200d75c
	movs	r0, #16
	bl 0x0200d76c
	movs	r0, #16
	bl 0x0200d4fc
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #0
	strb	r2, [r3, #0]
	mov	r3, sl
	str	r3, [r5, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.4byte 0x00202108
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
	beq.n	.L_020052a2
	ldrb	r0, [r6, #0]
	movs	r1, #6
	lsrs	r0, r0, #1
	bl 0x0200d4f4
	adds	r1, r0, #0
	lsls	r1, r1, #24
	lsrs	r1, r1, #24
	adds	r0, r7, #0
	bl 0x0200d5d4
.L_020052a2:
	adds	r3, r7, #0
	adds	r3, #98
	ldrb	r5, [r3, #0]
	cmp	r5, #0
	bne.n	.L_020052e0
	ldrb	r2, [r6, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02005316
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #86
	bl 0x0200d7dc
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
	b.n	.L_02005316
	.2byte 0x0000
	.2byte 0x0001
	.2byte 0x0000
.L_020052e0:
	cmp	r5, #1
	bne.n	.L_02005316
	mov	r3, r8
	adds	r3, #160
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #31
	ble.n	.L_02005316
	mov	r3, r8
	adds	r3, #162
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #31
	ble.n	.L_02005316
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200d5d4
	mov	r3, r8
	adds	r3, #164
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200d53c
	movs	r3, #0
	str	r3, [r7, #108]
	b.n	.L_020053d0
.L_02005316:
	ldrb	r3, [r6, #0]
	movs	r2, #1
	adds	r3, #1
	strb	r3, [r6, #0]
	movs	r3, #0
	str	r3, [sp, #0]
	mov	r6, r8
	mov	fp, r2
	adds	r6, #160
.L_02005328:
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	lsls	r0, r0, #10
	bl 0x0200d51c
	str	r0, [sp, #4]
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	blt.n	.L_020053bc
	cmp	r3, #31
	bgt.n	.L_020053bc
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
	bl 0x0200d79c
	ldr	r2, [r5, #0]
	movs	r3, #0
	str	r2, [sp, #8]
	mov	sl, r3
	ldr	r5, [r5, #8]
	mov	r9, r5
	ldr	r5, [sp, #0]
	add	r5, r8
.L_0200536c:
	ldr	r2, [sp, #8]
	mov	r3, r9
	str	r3, [r5, #16]
	str	r2, [r5, #12]
	ldr	r2, [sp, #4]
	mov	r3, sl
	str	r2, [r5, #20]
	str	r2, [r5, #24]
	cmp	r3, #0
	bne.n	.L_0200538c
	adds	r0, r7, #0
	bl 0x0200d7bc
	subs	r0, #1
	strh	r0, [r5, #30]
	b.n	.L_020053a4
.L_0200538c:
	adds	r0, r7, #0
	bl 0x0200d7bc
	ldr	r3, [r5, #16]
	ldr	r2, [pc, #72]
	adds	r0, #1
	adds	r3, r3, r2
	str	r3, [r5, #16]
	ldr	r3, [r5, #24]
	strh	r0, [r5, #30]
	negs	r3, r3
	str	r3, [r5, #24]
.L_020053a4:
	adds	r0, r5, #0
	bl 0x0200d7ac
	movs	r3, #1
	add	sl, r3
	mov	r2, sl
	adds	r5, #40
	cmp	r2, #1
	ble.n	.L_0200536c
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
.L_020053bc:
	ldr	r3, [sp, #0]
	movs	r2, #1
	negs	r2, r2
	adds	r3, #80
	add	fp, r2
	str	r3, [sp, #0]
	mov	r3, fp
	adds	r6, #2
	cmp	r3, #0
	bge.n	.L_02005328
.L_020053d0:
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
	bl 0x0200d54c
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
	bl 0x0200d544
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
	b.n	.L_02005448
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0xd7e4
	.2byte 0x0200
.L_02005448:
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
.L_02005466:
	movs	r1, #1
	mov	r2, sl
	mov	r8, r1
	adds	r5, r2, r7
.L_0200546e:
	mov	r3, fp
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #16
	movs	r2, #16
	ldr	r3, [pc, #100]
	bl 0x0200d7a4
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
	bl 0x0200d7b4
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
	bge.n	.L_0200546e
	movs	r1, #1
	add	r9, r1
	movs	r0, #80
	mov	r2, r9
	add	sl, r0
	cmp	r2, #1
	ble.n	.L_02005466
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200d269
	.4byte 0x020036e0
	.4byte 0x80004000
	.4byte 0x23013062
	.4byte 0x47707003
	.irp EntryTarget, 0x03000528, 0x03000514, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000121, 0x08000141, 0x08000151, 0x080001b9, 0x080001c9, 0x080001d1, 0x080003c9, 0x080003d1, 0x080003d9, 0x080200a9, 0x080200c1, 0x080200c9, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x08020179, 0x080201e9, 0x08020219, 0x08020221, 0x08020229, 0x08020231, 0x08020279, 0x08020391, 0x08038041, 0x08038249, 0x080ad041, 0x080ad209, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8049, 0x080c8059, 0x080c8071, 0x080c8089, 0x080c8091, 0x080c8099, 0x080c80a1, 0x080c80b1, 0x080c80b9, 0x080c80c9, 0x080c80d9, 0x080c80f1, 0x080c80f9, 0x080c8101, 0x080c8111, 0x080c8119, 0x080c8129, 0x080c8131, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8159, 0x080c8169, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8259, 0x080c8279, 0x080c8281, 0x080c8291, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83a9, 0x080c83b9, 0x080c84e1, 0x080c84e9, 0x080c84f1, 0x080c85c1, 0x080c87c1, 0x080c87c9, 0x080c87e9, 0x080c87f1, 0x080c88d9, 0x080c88e9, 0x080c8919, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
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
	.4byte 0x008a0089
	.4byte 0x008c008b
	.4byte 0x008e008d
	.4byte 0xffff008f
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0xffff0016
	.4byte 0x001b001e
	.4byte 0x0016001f
	.4byte 0x001f0015
	.4byte 0x00120014
	.4byte 0x000d001f
	.4byte 0x001b000b
	.4byte 0x0008000b
	.4byte 0x00080018
	.4byte 0x00150005
	.4byte 0x001f001f
	.4byte 0xffff001f
	.4byte 0x0089008e
	.4byte 0x008b008a
	.4byte 0x008d008c
	.4byte 0x008e008d
	.4byte 0x008a0089
	.4byte 0x008c008b
	.4byte 0x008d008c
	.4byte 0x0089008e
	.4byte 0x008b008a
	.4byte 0x008c008b
	.4byte 0x008e008d
	.4byte 0x008a0089
	.4byte 0x008b008a
	.4byte 0x008d008c
	.4byte 0x0089008e
	.4byte 0x008a0089
	.4byte 0x008c008b
	.4byte 0x008e008d
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
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000040
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000200
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000200
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000080
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01f40000
	.4byte 0x00000000
	.4byte 0x01320000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x020c0000
	.4byte 0x00000000
	.4byte 0x01320000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x01180000
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
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x021a0000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01dc0000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01e60000
	.4byte 0x00000000
	.4byte 0x011e0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x01240000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02240000
	.4byte 0x00000000
	.4byte 0x01240000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
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
	.4byte 0x00000003
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
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
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
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0xc0010000
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000129
	.4byte 0x0010111f
	.4byte 0x00203129
	.4byte 0x00302135
	.4byte 0x00414110
	.4byte 0x000001ff
	.4byte 0xffff01e9
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
	.4byte 0x00024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02480000
	.4byte 0x00100000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02680000
	.4byte 0x00100000
	.4byte 0x01b80000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01b80000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff00b6
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0137
	.4byte 0x00000007
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
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
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
	.4byte 0xffff0003
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
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00b6
	.4byte 0x00000007
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x00ec0000
	.4byte 0x00024000
	.4byte 0xffff0137
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x01620000
	.4byte 0x0002b000
	.4byte 0xffff0030
	.4byte 0x00000001
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0031
	.4byte 0x00000001
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0032
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x0002c000
	.4byte 0xffff0033
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x0002c000
	.4byte 0xffff0034
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x0002c000
	.4byte 0xffff0036
	.4byte 0x00000001
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0035
	.4byte 0x00000001
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff00b7
	.4byte 0x00000007
	.4byte 0x01de0000
	.4byte 0x00000000
	.4byte 0x010a0000
	.4byte 0x00024000
	.4byte 0xffff00b7
	.4byte 0x00000007
	.4byte 0x01fa0000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00b7
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01140000
	.4byte 0x00024000
	.4byte 0xffff003d
	.4byte 0x00000007
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x011a0000
	.4byte 0x00024000
	.4byte 0xffff003c
	.4byte 0x00000007
	.4byte 0x01fa0000
	.4byte 0x00000000
	.4byte 0x01240000
	.4byte 0x00024000
	.4byte 0xffff003e
	.4byte 0x00000007
	.4byte 0x021c0000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff012f
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff012f
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02480000
	.4byte 0x00100000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02680000
	.4byte 0x00100000
	.4byte 0x01b80000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01b80000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002b000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x01ca0000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00010000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x01dc0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001e000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x01e20000
	.4byte 0x00000000
	.4byte 0x01140000
	.4byte 0x00014000
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x02110000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x021a0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001e000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01120000
	.4byte 0x00014000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x02360000
	.4byte 0x00000000
	.4byte 0x011e0000
	.4byte 0x00018000
	.4byte 0xffff003d
	.4byte 0x00000007
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff003c
	.4byte 0x00000007
	.4byte 0x01f60000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff003e
	.4byte 0x00000007
	.4byte 0x02240000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff003b
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
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
	.4byte 0x00009c05
	.4byte 0xffff0001
	.4byte 0x020081a1
	.4byte 0x00000002
	.4byte 0x0a3d000a
	.4byte 0x0200865d
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00002e17
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00002e18
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00002e1a
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x00002e1b
	.4byte 0x00000000
	.4byte 0xffff0006
	.4byte 0x00002e1c
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte 0x00002e1d
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002e1e
	.4byte 0x00004400
	.4byte 0xffff001c
	.4byte 0x0200cda1
	.4byte 0x00004400
	.4byte 0xffff0011
	.4byte 0x0200cdf1
	.4byte 0x00006400
	.4byte 0xffff0011
	.4byte 0x0200cdf1
	.4byte 0x0000e400
	.4byte 0xffff001e
	.4byte 0x0200ce21
	.4byte 0x00000400
	.4byte 0xffff001e
	.4byte 0x0200ce21
	.4byte 0x00002400
	.4byte 0xffff001e
	.4byte 0x0200ce21
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x0200ce51
	.4byte 0x00009115
	.4byte 0xffff001c
	.4byte 0x0200cec1
	.4byte 0x00009115
	.4byte 0xffff0011
	.4byte 0x0200cecd
	.4byte 0x00009115
	.4byte 0xffff001e
	.4byte 0x0200ced9
	.4byte 0x00009115
	.4byte 0xffff001d
	.4byte 0x0200cee5
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x0200cbc5
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x0200cc2d
	.4byte 0x00000002
	.4byte 0x02020016
	.4byte 0x0200cc95
	.4byte 0x00000002
	.4byte 0x12020017
	.4byte 0x0200cd15
	.4byte 0xffffffff
	.4byte 0x00000000
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
