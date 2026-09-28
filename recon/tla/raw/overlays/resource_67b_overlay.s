.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020097f5, 0x0200833d, 0x0200837d, 0x020084a5, 0x020093b9, 0x02008345, 0x02009d2d
	overlay_veneer \EntryTarget
	.endr
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_0200007c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200007c
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
.L_0200007c:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl 0x0200a920
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020000ca
	movs	r1, #0
	bl 0x02008038
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a978
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x0200a9e8
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200a980
	adds	r0, r5, #0
	b.n	.L_020000cc
.L_020000ca:
	movs	r0, #0
.L_020000cc:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
.L_020000d6:
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl 0x0200a920
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200011e
	movs	r1, #1
	bl 0x02008038
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a978
	adds	r0, r5, #0
	movs	r1, #15
	bl 0x0200a9e8
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #34
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	b.n	.L_02000120
.L_0200011e:
	movs	r0, #0
.L_02000120:
	pop	{r5, r6, pc}
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
	sub	sp, #4
	str	r3, [sp, #0]
	ldr	r3, [pc, #444]
	mov	r8, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	sl, r1
	ldr	r7, [sp, #48]
	bl 0x0200a9a8
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, sl
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_020001a4
	cmp	r7, #0
	beq.n	.L_020001a4
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_020001ac
.L_020001a4:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_020001ac:
	mov	r3, r8
	bl 0x0200a920
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020001ba
	b.n	.L_0200031e
.L_020001ba:
	ldr	r3, [r6, #80]
	mov	r1, sl
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	mov	r8, r3
	bl 0x0200a910
	ldr	r2, [pc, #352]
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl 0x0200a918
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a978
	ldr	r3, [pc, #324]
	mov	r1, r9
	str	r3, [r6, #108]
	ldr	r3, [sp, #0]
	adds	r0, r6, #0
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x02008038
	movs	r2, #100
	adds	r2, r2, r6
	mov	r9, r2
	mov	r3, r9
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #280]
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200031e
	cmp	r7, #0
	beq.n	.L_0200031e
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200023c
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200a9e8
.L_0200023c:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, sl
	ands	r3, r2
.L_02000244:
	cmp	r3, #0
	beq.n	.L_02000274
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #3
	ldrb	r2, [r7, #0]
	adds	r0, r6, #0
	ands	r2, r3
	mov	r3, r8
	ldrb	r1, [r3, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	lsls	r2, r2, #2
	mov	r1, r8
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldr	r1, [r7, #0]
	bl 0x02008038
.L_02000274:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, sl
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02000288
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02000288:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002ce
	ldr	r3, [pc, #152]
	mov	r1, fp
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020002b6
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200a860
.L_020002aa:
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_020002c8
.L_020002b6:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200a860
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020002c8:
	bl 0x0200a860
	str	r0, [r6, #52]
.L_020002ce:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002ea
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200a910
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200a918
.L_020002ea:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002fc
	ldrh	r3, [r7, #32]
	mov	r1, r8
	strh	r3, [r1, #18]
.L_020002fc:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200030e
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_0200030e:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200031e
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_0200031e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200adf4
	.4byte 0x02008125
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x4800
	bx	lr
	.2byte 0xae00
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
	bne.n	.L_0200035c
	ldr	r0, [pc, #20]
	b.n	.L_02000366
.L_0200035c:
	ldr	r3, [pc, #20]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_02000366
	ldr	r0, [pc, #16]
.L_02000366:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000009e
	.4byte 0x0200ae30
	.4byte 0x0000009f
	.2byte 0xae60
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xaeb0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #128
	adds	r7, r0, #0
	lsls	r2, r2, #1
	movs	r0, #0
	sub	sp, #24
	mov	sl, r0
	mov	r9, r2
.L_0200039c:
	movs	r3, #1
	add	sl, r3
	mov	r5, sl
	cmp	r5, #3
	bgt.n	.L_02000436
	ldr	r3, [r7, #8]
	add	r0, sp, #12
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	mov	r8, r0
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	str	r3, [r0, #8]
	bl 0x0200a878
	movs	r1, #128
	ldr	r3, [pc, #164]
	lsls	r1, r1, #12
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2280
	lsls	r2, r2, #11
	adds	r6, r0, #0
	adds	r6, r6, r2
	bl 0x0200a878
	adds	r5, r0, #0
	bl 0x0200a878
	ldrh	r1, [r7, #6]
	lsrs	r5, r5, #2
	adds	r1, r1, r5
	lsrs	r0, r0, #2
	subs	r1, r1, r0
	mov	r2, r8
	adds	r0, r6, #0
	mov	r5, r8
	bl 0x0200a888
	ldr	r3, [r5, #0]
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_020003fa
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r3, r0
.L_020003fa:
	adds	r2, r7, #0
	adds	r2, #100
	movs	r5, #0
	ldrsh	r2, [r2, r5]
	asrs	r3, r3, #16
	subs	r0, r3, r2
	mov	r3, r8
	ldr	r2, [r3, #8]
	adds	r4, r2, #0
	cmp	r2, #0
	bge.n	.L_02000418
	movs	r5, #255
	lsls	r5, r5, #8
	adds	r5, #255
	adds	r2, r2, r5
.L_02000418:
	adds	r3, r7, #0
	adds	r3, #102
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	asrs	r2, r2, #16
	subs	r2, r2, r3
	adds	r3, r0, #0
	muls	r3, r0
	adds	r0, r2, #0
	muls	r0, r2
	adds	r2, r0, #0
	adds	r3, r3, r2
	cmp	r3, r9
	ble.n	.L_0200044a
	b.n	.L_0200039c
.L_02000436:
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	adds	r2, r7, #0
	strh	r3, [r7, #6]
	adds	r2, #94
	movs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000456
.L_0200044a:
	mov	r3, r8
	ldr	r2, [r3, #4]
	adds	r0, r7, #0
	adds	r3, r4, #0
	bl 0x0200a930
.L_02000456:
	movs	r0, #0
	add	sp, #24
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.4byte 0x6d0288c3
	.4byte 0x01c92180
	.4byte 0x8253185b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, lr}
	movs	r1, #0
	adds	r5, r0, #0
	bl 0x0200a978
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #48]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #204
	adds	r2, r5, #0
	str	r3, [r5, #52]
	adds	r2, #89
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r3, [pc, #4]
	movs	r0, #0
	str	r3, [r5, #108]
	pop	{r5, pc}
	.2byte 0x8469
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_020004bc
	ldr	r0, [pc, #24]
	b.n	.L_020004c8
.L_020004bc:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020004c6
	ldr	r0, [pc, #24]
	b.n	.L_020004c8
.L_020004c6:
	ldr	r0, [pc, #24]
.L_020004c8:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000009e
	.4byte 0x0200af04
	.4byte 0x0000009f
	.4byte 0x0200b12c
	.2byte 0xaeec
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200a950
	ldr	r3, [r5, #12]
	cmp	r0, r3
	ble.n	.L_02000502
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r5, #12]
	b.n	.L_0200050a
.L_02000502:
	ldr	r3, [r5, #16]
	ldr	r2, [pc, #32]
	adds	r3, r3, r2
	str	r3, [r5, #16]
.L_0200050a:
	ldr	r3, [r5, #12]
	asrs	r3, r3, #20
	cmp	r3, #23
	bne.n	.L_02000518
	adds	r0, r5, #0
	bl 0x0200a928
.L_02000518:
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #48
	bne.n	.L_02000526
	adds	r0, r5, #0
	bl 0x0200a928
.L_02000526:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb520
	adds	r5, r0, #0
	bl 0x0200a878
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r0, r3
	bcs.n	.L_02000574
	bl 0x0200a878
	ldr	r1, [r5, #8]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	adds	r1, r1, r3
	ldr	r3, [pc, #44]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r1, r1, r3
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, #162
	bl 0x0200a920
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000574
	movs	r1, #0
	bl 0x0200a978
	ldr	r3, [pc, #16]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_02000574:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0xffe80000
	.2byte 0x84e5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #80]
	sub	sp, #20
	movs	r1, #0
	mov	sl, r0
	movs	r2, #2
	str	r1, [sp, #16]
	add	r2, sl
	mov	r8, r2
.L_0200059e:
	mov	r4, r8
	ldrh	r3, [r4, #16]
	ldr	r0, [pc, #48]
	mov	r6, r8
	ands	r0, r3
	bl 0x0200a8d8
	ldrh	r3, [r6, #16]
	ldr	r2, [pc, #40]
	adds	r4, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020005bc
	movs	r3, #1
	eors	r4, r3
.L_020005bc:
	mov	r1, r8
	movs	r0, #18
	ldrsh	r3, [r1, r0]
	cmp	r3, r4
	beq.n	.L_0200060a
	movs	r2, #28
	ldrsh	r3, [r1, r2]
	ldr	r2, [pc, #16]
	lsls	r3, r3, #1
	mov	r6, r8
	b.n	.L_020005e4
	.2byte 0x0000
	.4byte 0x00000fff
	.4byte 0x00001000
	.4byte 0x00000000
	.2byte 0xb450
	.2byte 0x0200
.L_020005e4:
	adds	r3, #20
	strh	r2, [r6, r3]
	movs	r2, #0
	movs	r0, #28
	ldrsh	r3, [r6, r0]
	movs	r0, #128
	lsls	r3, r3, #1
	adds	r3, #24
	strh	r4, [r1, r3]
	lsls	r0, r0, #9
	ldrh	r3, [r1, #28]
	adds	r3, #1
	strh	r3, [r6, #28]
	lsls	r3, r3, #16
	cmp	r3, r0
	ble.n	.L_02000606
	strh	r2, [r1, #28]
.L_02000606:
	mov	r2, r8
	strh	r4, [r2, #18]
.L_0200060a:
	movs	r4, #24
	str	r4, [sp, #8]
	movs	r3, #20
	movs	r6, #1
	mov	r9, r3
	mov	fp, r6
.L_02000616:
	mov	r0, r8
	str	r0, [sp, #12]
	mov	r3, r9
	mov	r1, r9
	ldrsh	r7, [r0, r3]
	movs	r4, #14
	ldrsh	r3, [r0, r4]
	ldrh	r1, [r0, r1]
	lsls	r3, r3, #2
	mov	ip, r1
	cmp	r7, r3
	bge.n	.L_02000706
	mov	r6, sl
	ldrh	r4, [r6, #8]
	mov	r2, r8
	adds	r3, r7, #0
	ldrh	r0, [r6, #0]
	ldrh	r1, [r2, #0]
	mov	lr, r4
	ldrh	r5, [r6, #10]
	cmp	r7, #0
	bge.n	.L_02000644
	adds	r3, r7, #3
.L_02000644:
	asrs	r7, r3, #2
	ldr	r3, [sp, #8]
	mov	r6, r8
	ldrsh	r4, [r6, r3]
	movs	r3, #1
	adds	r6, r4, #0
	mov	r2, ip
	eors	r6, r3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020006aa
	mov	r3, lr
	lsls	r2, r3, #16
	lsls	r3, r5, #16
	lsls	r0, r0, #16
	lsls	r5, r4, #1
	lsls	r1, r1, #16
	asrs	r3, r3, #16
	adds	r5, r5, r4
	asrs	r2, r2, #16
	adds	r3, r3, r7
	asrs	r0, r0, #16
	asrs	r1, r1, #16
	adds	r0, r5, r0
	adds	r1, r1, r7
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #3
	movs	r3, #1
	bl 0x0200a968
	mov	r6, sl
	movs	r4, #4
	ldrsh	r2, [r6, r4]
	movs	r0, #6
	ldrsh	r1, [r6, r0]
	movs	r3, #12
	ldrsh	r0, [r6, r3]
	movs	r4, #14
	ldrsh	r3, [r6, r4]
	adds	r5, r5, r2
	adds	r3, r3, r7
	str	r0, [sp, #0]
	str	r3, [sp, #4]
	adds	r1, r1, r7
	adds	r0, r5, #0
	movs	r2, #3
	movs	r3, #1
	bl 0x0200a958
	b.n	.L_020006f8
.L_020006aa:
	mov	r3, lr
	lsls	r2, r3, #16
	lsls	r3, r5, #16
	lsls	r0, r0, #16
	lsls	r5, r6, #1
	lsls	r1, r1, #16
	asrs	r3, r3, #16
	adds	r5, r5, r6
	asrs	r2, r2, #16
	adds	r3, r3, r7
	asrs	r0, r0, #16
	asrs	r1, r1, #16
	adds	r0, r5, r0
	adds	r1, r1, r7
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #3
	movs	r3, #1
	bl 0x0200a968
	mov	r6, sl
	movs	r4, #4
	ldrsh	r2, [r6, r4]
	movs	r0, #6
	ldrsh	r1, [r6, r0]
	movs	r3, #12
	ldrsh	r0, [r6, r3]
	movs	r4, #14
	ldrsh	r3, [r6, r4]
	adds	r5, r5, r2
	adds	r3, r3, r7
	str	r0, [sp, #0]
	str	r3, [sp, #4]
	adds	r1, r1, r7
	adds	r0, r5, #0
	movs	r2, #3
	movs	r3, #1
	bl 0x0200a958
.L_020006f8:
	ldr	r6, [sp, #12]
	mov	r0, r9
	ldrh	r3, [r6, r0]
	adds	r1, r6, #0
	adds	r3, #1
	mov	r2, r9
	strh	r3, [r1, r2]
.L_02000706:
	ldr	r4, [sp, #8]
	movs	r6, #1
	negs	r6, r6
	add	fp, r6
	movs	r3, #2
	adds	r4, #2
	mov	r0, fp
	add	r9, r3
	str	r4, [sp, #8]
	cmp	r0, #0
	blt.n	.L_0200071e
	b.n	.L_02000616
.L_0200071e:
	ldr	r1, [sp, #16]
	movs	r2, #32
	adds	r1, #1
	str	r1, [sp, #16]
	add	r8, r2
	add	sl, r2
	cmp	r1, #4
	bgt.n	.L_02000730
	b.n	.L_0200059e
.L_02000730:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200a8d8
	cmp	r0, #0
	bne.n	.L_02000832
	ldr	r3, [pc, #228]
	ldr	r3, [r3, #0]
	cmp	r3, #149
	bgt.n	.L_02000776
	movs	r0, #200
	lsls	r0, r0, #2
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_0200076c
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200a8e0
	b.n	.L_0200077e
.L_0200076c:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200a8e8
	b.n	.L_0200077e
.L_02000776:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200a8e0
.L_0200077e:
	ldr	r2, [pc, #180]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	cmp	r3, #239
	ble.n	.L_0200078e
	movs	r3, #0
	str	r3, [r2, #0]
.L_0200078e:
	ldr	r3, [pc, #168]
	ldr	r3, [r3, #0]
	cmp	r3, #149
	bgt.n	.L_020007bc
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #33
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_020007b0
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a8e0
	b.n	.L_020007c6
.L_020007b0:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a8e8
	b.n	.L_020007c6
.L_020007bc:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a8e0
.L_020007c6:
	ldr	r2, [pc, #112]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	cmp	r3, #239
	ble.n	.L_020007d6
	movs	r3, #0
	str	r3, [r2, #0]
.L_020007d6:
	ldr	r3, [pc, #100]
	ldr	r3, [r3, #0]
	cmp	r3, #149
	bgt.n	.L_020007ea
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200a8e8
	b.n	.L_020007f4
.L_020007ea:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200a8e0
.L_020007f4:
	ldr	r2, [pc, #68]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	cmp	r3, #239
	ble.n	.L_02000804
	movs	r3, #0
	str	r3, [r2, #0]
.L_02000804:
	ldr	r3, [pc, #56]
	ldr	r3, [r3, #0]
	cmp	r3, #149
	bgt.n	.L_02000818
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a8e8
	b.n	.L_02000822
.L_02000818:
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a8e0
.L_02000822:
	ldr	r2, [pc, #28]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	cmp	r3, #239
	ble.n	.L_02000832
	movs	r3, #0
	str	r3, [r2, #0]
.L_02000832:
	pop	{pc}
	.4byte 0x0200b204
	.4byte 0x0200b208
	.4byte 0x0200b20c
	.2byte 0xb210
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #92]
	movs	r3, #64
	movs	r2, #70
	mov	sl, r2
	strh	r3, [r5, #0]
	movs	r3, #76
	strh	r3, [r5, #4]
	mov	r3, sl
	strh	r3, [r5, #8]
	movs	r3, #6
	strh	r3, [r5, #12]
	movs	r4, #10
	ldr	r3, [pc, #56]
	mov	fp, r4
	movs	r4, #128
	movs	r6, #74
	mov	r2, fp
	lsls	r4, r4, #2
	movs	r0, #128
	strh	r6, [r5, #2]
	strh	r6, [r5, #6]
	strh	r2, [r5, #10]
	strh	r6, [r5, #14]
	strh	r3, [r5, #16]
	strh	r4, [r5, #18]
	lsls	r0, r0, #2
	sub	sp, #8
	bl 0x0200a8d8
	movs	r3, #156
	ldr	r7, [pc, #24]
	ldr	r2, [pc, #24]
	lsls	r3, r3, #6
	adds	r3, #15
	mov	r8, r3
	strh	r2, [r5, #30]
	eors	r0, r7
	mov	r2, r8
	mov	r4, r8
	strh	r0, [r5, #20]
	b.n	.L_020008b4
	.4byte 0x00000024
	.4byte 0x00000001
	.4byte 0x00000000
	.2byte 0xb450
	.2byte 0x0200
.L_020008b4:
	strh	r4, [r5, #24]
	strh	r2, [r5, #22]
	mov	r3, sl
	adds	r5, #32
	strh	r3, [r5, #0]
	strh	r6, [r5, #2]
	strh	r6, [r5, #6]
	strh	r6, [r5, #14]
	movs	r3, #82
	ldr	r6, [pc, #56]
	movs	r0, #129
	strh	r3, [r5, #4]
	lsls	r0, r0, #1
	movs	r3, #78
	mov	r4, fp
	strh	r3, [r5, #8]
	adds	r0, #255
	movs	r3, #14
	strh	r4, [r5, #10]
	strh	r3, [r5, #12]
	strh	r6, [r5, #16]
	strh	r0, [r5, #18]
	bl 0x0200a8d8
	ldr	r2, [pc, #28]
	eors	r0, r7
	mov	r3, r8
	mov	r4, r8
	strh	r0, [r5, #20]
	strh	r2, [r5, #30]
	strh	r3, [r5, #24]
	strh	r4, [r5, #22]
	movs	r3, #102
	adds	r5, #32
	movs	r2, #26
	strh	r3, [r5, #4]
	b.n	.L_02000908
	.2byte 0x0000
	.4byte 0x00000024
	.2byte 0x0000
	.2byte 0x0000
.L_02000908:
	mov	r9, r2
	ldr	r6, [pc, #56]
	movs	r3, #103
	strh	r3, [r5, #8]
	ldr	r4, [pc, #48]
	movs	r2, #15
	mov	r3, r9
	movs	r0, #128
	mov	fp, r2
	strh	r3, [r5, #10]
	lsls	r0, r0, #2
	movs	r3, #39
	strh	r6, [r5, #0]
	strh	r3, [r5, #12]
	movs	r6, #93
	mov	r3, fp
	adds	r0, #2
	strh	r6, [r5, #2]
	strh	r6, [r5, #6]
	strh	r4, [r5, #14]
	strh	r3, [r5, #16]
	strh	r0, [r5, #18]
	bl 0x0200a8d8
	ldr	r4, [pc, #12]
	eors	r0, r7
	mov	r2, r8
	mov	r3, r8
	strh	r0, [r5, #20]
	b.n	.L_0200094c
	.4byte 0x0000005a
	.2byte 0x0000
	.2byte 0x0000
.L_0200094c:
	strh	r4, [r5, #30]
	strh	r2, [r5, #24]
	strh	r3, [r5, #22]
	movs	r4, #96
	adds	r5, #32
	movs	r3, #108
	mov	sl, r4
	strh	r3, [r5, #4]
	ldr	r4, [pc, #56]
	movs	r3, #107
	movs	r0, #130
	strh	r3, [r5, #8]
	lsls	r0, r0, #1
	mov	r3, r9
	mov	r2, sl
	strh	r6, [r5, #2]
	strh	r6, [r5, #6]
	strh	r3, [r5, #10]
	mov	r6, fp
	movs	r3, #43
	adds	r0, #255
	strh	r2, [r5, #0]
	strh	r3, [r5, #12]
	strh	r4, [r5, #14]
	strh	r6, [r5, #16]
	strh	r0, [r5, #18]
	bl 0x0200a8d8
	ldr	r2, [pc, #20]
	mov	r4, r8
	eors	r0, r7
	mov	r3, r8
	strh	r0, [r5, #20]
	strh	r2, [r5, #30]
	strh	r3, [r5, #24]
	strh	r4, [r5, #22]
	movs	r3, #116
	b.n	.L_020009a0
	.4byte 0x0000005a
	.2byte 0x0000
	.2byte 0x0000
.L_020009a0:
	adds	r5, #32
	strh	r3, [r5, #0]
	movs	r0, #208
	movs	r3, #122
	movs	r2, #95
	strh	r3, [r5, #4]
	lsls	r0, r0, #5
	movs	r3, #32
	strh	r2, [r5, #2]
	strh	r2, [r5, #6]
	mov	r6, sl
	strh	r3, [r5, #10]
	strh	r3, [r5, #12]
	mov	r2, sl
	movs	r3, #13
	adds	r0, #107
	strh	r6, [r5, #8]
	strh	r3, [r5, #16]
	strh	r2, [r5, #14]
	strh	r0, [r5, #18]
	bl 0x0200a8d8
	ldr	r3, [pc, #36]
	eors	r0, r7
	strh	r0, [r5, #20]
	mov	r4, r8
	mov	r6, r8
	movs	r0, #200
	strh	r3, [r5, #30]
	strh	r4, [r5, #24]
	strh	r6, [r5, #22]
	lsls	r0, r0, #2
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_020009f8
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200a8e0
	b.n	.L_02000a00
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020009f8:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200a8e8
.L_02000a00:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #33
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02000a1a
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a8e0
	b.n	.L_02000a24
.L_02000a1a:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a8e8
.L_02000a24:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02000a80
	ldr	r5, [pc, #204]
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	movs	r6, #8
	ldrsh	r2, [r5, r6]
	movs	r3, #2
	ldrsh	r1, [r5, r3]
	movs	r4, #16
	ldrsh	r3, [r5, r4]
	movs	r6, #10
	ldrsh	r4, [r5, r6]
	adds	r0, #3
	str	r2, [sp, #0]
	movs	r2, #3
	str	r4, [sp, #4]
	bl 0x0200a968
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	movs	r6, #12
	ldrsh	r2, [r5, r6]
	movs	r3, #6
	ldrsh	r1, [r5, r3]
	movs	r4, #16
	ldrsh	r3, [r5, r4]
	movs	r6, #14
	ldrsh	r4, [r5, r6]
	adds	r0, #3
	str	r2, [sp, #0]
	movs	r2, #3
	str	r4, [sp, #4]
	bl 0x0200a958
	movs	r3, #156
	lsls	r3, r3, #6
	adds	r3, #15
	strh	r3, [r5, #24]
	strh	r3, [r5, #22]
	movs	r3, #1
	strh	r3, [r5, #20]
.L_02000a80:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02000ade
	ldr	r5, [pc, #116]
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	movs	r6, #8
	ldrsh	r2, [r5, r6]
	movs	r3, #2
	ldrsh	r1, [r5, r3]
	movs	r4, #16
	ldrsh	r3, [r5, r4]
	movs	r6, #10
	ldrsh	r4, [r5, r6]
	adds	r0, #3
	str	r2, [sp, #0]
	movs	r2, #3
	str	r4, [sp, #4]
	bl 0x0200a968
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	movs	r6, #12
	ldrsh	r2, [r5, r6]
	movs	r3, #6
	ldrsh	r1, [r5, r3]
	movs	r4, #16
	ldrsh	r3, [r5, r4]
	movs	r6, #14
	ldrsh	r4, [r5, r6]
	adds	r0, #3
	str	r2, [sp, #0]
	movs	r2, #3
	str	r4, [sp, #4]
	bl 0x0200a958
	movs	r3, #156
	lsls	r3, r3, #6
	adds	r3, #15
	strh	r3, [r5, #24]
	strh	r3, [r5, #22]
	movs	r3, #1
	strh	r3, [r5, #20]
.L_02000ade:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #36]
	bl 0x0200a870
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #28]
	bl 0x0200a870
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200b450
	.4byte 0x0200b470
	.4byte 0x02008741
	.2byte 0x8581
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #2
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000b26
	movs	r1, #7
	bl 0x0200a988
	b.n	.L_02000b2c
.L_02000b26:
	movs	r1, #0
	bl 0x0200a988
.L_02000b2c:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #2
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000b4a
	movs	r1, #6
	bl 0x0200a988
	b.n	.L_02000b50
.L_02000b4a:
	movs	r1, #0
	bl 0x0200a988
.L_02000b50:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, lr}
	movs	r1, #0
	adds	r5, r0, #0
	bl 0x0200a988
	movs	r3, #0
	str	r3, [r5, #108]
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	adds	r0, r1, #0
	sub	sp, #8
	bl 0x0200a9a8
	adds	r7, r0, #0
	bl 0x0200a998
	movs	r0, #0
	bl 0x0200aa48
	ldr	r3, [pc, #336]
	movs	r0, #30
	str	r3, [r7, #108]
	bl 0x0200a868
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #104
	bl 0x0200a8d8
	cmp	r0, #0
	bne.n	.L_02000b9e
	movs	r0, #26
	bl 0x0200a834
	b.n	.L_02000ba4
.L_02000b9e:
	movs	r0, #0
	bl 0x0200a834
.L_02000ba4:
	movs	r5, #3
.L_02000ba6:
	bl 0x0200a878
	movs	r6, #254
	lsls	r6, r6, #7
	adds	r6, #255
	movs	r1, #0
	ands	r0, r6
	bl 0x0200aa20
	movs	r0, #15
	bl 0x0200aa28
	movs	r0, #10
	bl 0x0200a868
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200aa20
	movs	r0, #10
	bl 0x0200aa28
	subs	r5, #1
	movs	r0, #10
	bl 0x0200a868
	cmp	r5, #0
	bge.n	.L_02000ba6
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200aa20
	movs	r0, #20
	bl 0x0200aa28
	movs	r0, #20
	bl 0x0200a990
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #104
	bl 0x0200a8d8
	cmp	r0, #0
	bne.n	.L_02000c58
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #104
	bl 0x0200a8e0
	movs	r3, #51
	movs	r2, #82
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #1
	movs	r3, #2
	bl 0x0200a958
	movs	r3, #115
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #2
	movs	r1, #64
	movs	r2, #1
	movs	r0, #66
	bl 0x0200a958
	movs	r0, #5
	bl 0x0200a948
	movs	r0, #6
	bl 0x0200a948
	movs	r0, #7
	bl 0x0200a940
	movs	r0, #8
	bl 0x0200a940
	ldr	r3, [pc, #132]
	movs	r0, #26
	str	r3, [r7, #108]
	bl 0x0200a5a8
	b.n	.L_02000cac
.L_02000c58:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #104
	bl 0x0200a8e8
	movs	r3, #51
	movs	r2, #82
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #65
	movs	r1, #64
	movs	r2, #1
	movs	r3, #2
	bl 0x0200a958
	movs	r3, #115
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #2
	movs	r1, #64
	movs	r2, #1
	movs	r0, #67
	bl 0x0200a958
	movs	r0, #7
	bl 0x0200a948
	movs	r0, #8
	bl 0x0200a948
	movs	r0, #5
	bl 0x0200a940
	movs	r0, #6
	bl 0x0200a940
	ldr	r3, [pc, #48]
	movs	r0, #0
	str	r3, [r7, #108]
	bl 0x0200a5a8
.L_02000cac:
	movs	r0, #30
	bl 0x0200a990
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200aa20
	movs	r0, #30
	bl 0x0200aa28
	movs	r0, #15
	bl 0x0200a990
	bl 0x0200a9a0
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x02008b11
	.2byte 0x8b59
	.2byte 0x0200
	push	{r5, r6, lr}
	cmp	r1, #24
	bne.n	.L_02000d36
	movs	r0, #24
	bl 0x0200a9a8
	ldr	r3, [r0, #8]
	asrs	r6, r3, #20
	ldr	r3, [r0, #16]
	movs	r0, #196
	lsls	r0, r0, #2
	adds	r1, r6, #0
	asrs	r5, r3, #20
	bl 0x0200a8f8
	movs	r0, #198
	lsls	r0, r0, #2
	adds	r1, r5, #0
	bl 0x0200a8f8
	movs	r0, #200
	lsls	r0, r0, #2
	bl 0x0200a8e8
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #33
	bl 0x0200a8e8
	cmp	r6, #7
	bne.n	.L_02000d22
	cmp	r5, #35
	bne.n	.L_02000d22
	movs	r0, #200
	lsls	r0, r0, #2
	bl 0x0200a8e0
.L_02000d22:
	cmp	r6, #15
	bne.n	.L_02000d62
	cmp	r5, #35
	bne.n	.L_02000d62
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #33
	bl 0x0200a8e0
	b.n	.L_02000d62
.L_02000d36:
	cmp	r1, #25
	bne.n	.L_02000d62
	movs	r0, #25
	bl 0x0200a9a8
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #32
	bne.n	.L_02000d62
	movs	r0, #30
	bl 0x0200a868
	ldr	r3, [r5, #16]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	movs	r0, #149
	str	r3, [r5, #16]
	lsls	r0, r0, #4
	bl 0x0200a8e0
.L_02000d62:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	adds	r6, r1, #0
	movs	r5, #60
.L_02000d6c:
	cmp	r5, #0
	beq.n	.L_02000d7e
	movs	r0, #1
	bl 0x0200a868
	ldr	r3, [r7, #12]
	subs	r5, #1
	cmp	r3, r6
	bgt.n	.L_02000d6c
.L_02000d7e:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #104
	sub	sp, #12
	bl 0x0200a8d8
	cmp	r0, #0
	bne.n	.L_02000da0
	b.n	.L_02000f5a
.L_02000da0:
	ldr	r0, [pc, #456]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #133
	mov	sl, r0
	lsls	r3, r3, #2
	add	r3, sl
	movs	r1, #230
	ldr	r3, [r3, #0]
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldr	r2, [r2, #0]
	mov	r8, r3
	mov	r0, r8
	mov	r9, r2
	bl 0x0200a9a8
	adds	r6, r0, #0
	ldr	r0, [pc, #424]
	ldr	r1, [pc, #424]
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	mov	fp, r1
	cmp	r3, fp
	bne.n	.L_02000ddc
	ldr	r3, [r6, #20]
	cmp	r3, #0
	bne.n	.L_02000ddc
	b.n	.L_02000f5e
.L_02000ddc:
	ldr	r3, [r6, #8]
	mov	r7, sp
	str	r3, [r7, #0]
	movs	r2, #128
	ldr	r3, [r6, #12]
	lsls	r2, r2, #10
	str	r3, [r7, #4]
	adds	r0, r6, #0
	ldr	r3, [r6, #16]
	adds	r1, r7, #0
	adds	r3, r3, r2
	str	r3, [r7, #8]
	bl 0x0200a970
	ldr	r3, [pc, #380]
	movs	r2, #4
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000e0c
	adds	r0, r6, #0
	bl 0x02009d30
.L_02000e0c:
	cmp	r5, #0
	bge.n	.L_02000e5c
	movs	r1, #129
	mov	r0, r8
	lsls	r1, r1, #1
	bl 0x0200a9f8
	ldr	r3, [r6, #16]
	ldr	r0, [pc, #348]
	ldr	r1, [r6, #8]
	adds	r3, r3, r0
	ldr	r2, [r6, #12]
	adds	r0, r6, #0
	bl 0x0200a930
	adds	r0, r6, #0
	movs	r1, #7
	bl 0x0200a910
	adds	r0, r6, #0
	bl 0x0200a938
.L_02000e38:
	movs	r0, #1
	bl 0x0200a868
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #20]
	cmp	r2, r3
	bne.n	.L_02000e38
	adds	r0, r6, #0
	bl 0x02009d30
	adds	r0, r6, #0
	movs	r1, #6
	bl 0x0200a910
	movs	r0, #3
	bl 0x0200a868
	b.n	.L_02000f5e
.L_02000e5c:
	ldr	r3, [r6, #8]
	ldr	r1, [pc, #284]
	str	r3, [r7, #0]
	adds	r0, r6, #0
	ldr	r3, [r6, #12]
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	adds	r3, r3, r1
	str	r3, [r7, #8]
	adds	r1, r7, #0
	bl 0x0200a970
	adds	r5, r0, #0
	cmp	r5, #0
	ble.n	.L_02000f06
	ldr	r0, [pc, #244]
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	cmp	r3, fp
	bne.n	.L_02000f5e
	bl 0x0200a998
	movs	r0, #0
	bl 0x0200aa48
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200a978
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	add	r2, sl
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	bl 0x02009d30
	movs	r0, #5
	bl 0x0200a990
	movs	r5, #59
.L_02000eb8:
	ldr	r3, [r6, #12]
	movs	r1, #128
	lsls	r1, r1, #10
	adds	r3, r3, r1
	str	r3, [r6, #12]
	ldrh	r3, [r6, #6]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	strh	r3, [r6, #6]
	bl 0x0200a878
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	cmp	r0, r3
	bhi.n	.L_02000ee0
	adds	r0, r6, #0
	bl 0x02009d30
.L_02000ee0:
	movs	r0, #1
	subs	r5, #1
	bl 0x0200a868
	cmp	r5, #0
	bge.n	.L_02000eb8
	ldr	r3, [pc, #124]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	adds	r3, r3, r0
	movs	r2, #0
	strb	r2, [r3, #0]
	bl 0x0200a9a0
	movs	r0, #6
	bl 0x0200aa18
	b.n	.L_02000f5e
.L_02000f06:
	ldr	r3, [r6, #8]
	ldr	r1, [pc, #116]
	ldr	r2, [pc, #120]
	adds	r3, r3, r1
	str	r3, [r7, #0]
	adds	r0, r6, #0
	ldr	r3, [r6, #12]
	adds	r1, r7, #0
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	adds	r3, r3, r2
	str	r3, [r7, #8]
	bl 0x0200a970
	adds	r5, r0, #0
	cmp	r5, #0
	bgt.n	.L_02000f5e
	ldr	r3, [r6, #8]
	ldr	r0, [pc, #88]
	adds	r1, r7, #0
	adds	r3, r3, r0
	str	r3, [r7, #0]
	ldr	r3, [r6, #12]
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	adds	r3, r3, r0
	str	r3, [r7, #8]
	adds	r0, r6, #0
	bl 0x0200a970
	adds	r5, r0, #0
	cmp	r5, #0
	bgt.n	.L_02000f5e
	mov	r1, r9
	ldr	r3, [r1, #16]
	ldr	r2, [pc, #56]
	adds	r3, r3, r2
	str	r3, [r1, #16]
	ldr	r3, [r6, #16]
	adds	r3, r3, r2
	str	r3, [r6, #16]
	b.n	.L_02000f5e
.L_02000f5a:
	bl 0x02009e00
.L_02000f5e:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x02000420
	.4byte 0x0000009e
	.4byte 0x0300122c
	.4byte 0xfff80000
	.4byte 0x0005b333
	.4byte 0xfffa4ccd
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb5e0
	ldr	r5, [pc, #120]
	movs	r3, #192
	movs	r2, #133
	lsls	r3, r3, #18
	lsls	r2, r2, #2
	ldr	r6, [r3, #32]
	ldr	r7, [r3, #108]
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200a9a8
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r5, r5, r3
	ldrb	r3, [r5, #0]
	ldr	r2, [r0, #8]
	ldr	r4, [r0, #16]
	ldr	r1, [r0, #12]
	cmp	r3, #2
	bne.n	.L_02000fe4
	asrs	r0, r2, #20
	ldr	r2, [pc, #80]
	subs	r3, r4, r1
	adds	r3, r3, r2
	movs	r2, #184
	lsls	r2, r2, #1
	asrs	r1, r3, #20
	adds	r3, r6, r2
	ldr	r2, [r3, #0]
	lsls	r3, r1, #7
	adds	r3, r3, r0
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r3, [r2, #3]
	cmp	r3, #0
	beq.n	.L_02001004
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #44
	strh	r3, [r2, #0]
	b.n	.L_02001004
.L_02000fe4:
	asrs	r0, r2, #20
	movs	r2, #184
	subs	r3, r4, r1
	lsls	r2, r2, #1
	asrs	r1, r3, #20
	adds	r3, r6, r2
	ldr	r2, [r3, #0]
	lsls	r3, r1, #7
	adds	r3, r3, r0
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r3, [r2, #3]
	cmp	r3, #0
	beq.n	.L_02001004
	bl 0x02009e00
.L_02001004:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff4
	.2byte 0xb560
	sub	sp, #8
	movs	r3, #33
	movs	r2, #41
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #62
	movs	r1, #41
	movs	r2, #1
	movs	r3, #2
	bl 0x0200a960
	movs	r6, #96
	movs	r5, #32
	movs	r0, #125
	movs	r1, #32
	movs	r2, #3
	movs	r3, #13
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a968
	movs	r2, #3
	movs	r3, #13
	movs	r1, #96
	movs	r0, #61
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200a968
	movs	r0, #12
	bl 0x0200a9a8
	movs	r1, #5
	bl 0x0200a9e8
	movs	r0, #12
	bl 0x02009f60
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #107
	bl 0x0200a8e0
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #128]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200a9a8
	adds	r6, r0, #0
	ldr	r7, [r6, #72]
	bl 0x0200a998
	movs	r0, #0
	bl 0x0200aa48
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200a9f8
	adds	r0, r6, #0
	movs	r1, #40
	bl 0x0200a910
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #3
	movs	r5, #0
	strb	r3, [r2, #0]
	b.n	.L_020010aa
.L_020010a8:
	adds	r5, #1
.L_020010aa:
	cmp	r5, #179
	bgt.n	.L_020010bc
	movs	r0, #1
	bl 0x0200a868
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #20]
	cmp	r2, r3
	bgt.n	.L_020010a8
.L_020010bc:
	ldr	r5, [pc, #48]
	str	r7, [r6, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200a9f8
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200a978
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200a910
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r5, r5, r3
	movs	r3, #0
	strb	r3, [r5, #0]
	bl 0x0200a9a0
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	adds	r0, r1, #0
	sub	sp, #8
	bl 0x0200a9a8
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #35
	adds	r0, #85
	strb	r3, [r2, #0]
	strb	r3, [r0, #0]
	movs	r2, #32
	movs	r3, #44
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	movs	r0, #42
	bl 0x0200a960
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a8e0
	add	sp, #8
	pop	{pc}
	push	{lr}
	adds	r0, r1, #0
	sub	sp, #8
	bl 0x0200a9a8
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #35
	adds	r0, #85
	strb	r3, [r2, #0]
	strb	r3, [r0, #0]
	movs	r2, #32
	movs	r3, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	movs	r0, #48
	bl 0x0200a960
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200a8e0
	add	sp, #8
	pop	{pc}
	push	{lr}
	adds	r0, r1, #0
	sub	sp, #8
	bl 0x0200a9a8
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #35
	adds	r0, #85
	strb	r3, [r2, #0]
	strb	r3, [r0, #0]
	movs	r2, #44
	movs	r3, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #44
	movs	r2, #1
	movs	r3, #1
	movs	r0, #44
	bl 0x0200a960
	movs	r0, #146
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a8e0
	add	sp, #8
	pop	{pc}
	push	{r5, r6, lr}
	adds	r0, r1, #0
	sub	sp, #8
	bl 0x0200a9a8
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r6, r3, #20
	cmp	r6, #53
	bne.n	.L_020011dc
	movs	r0, #30
	bl 0x0200a868
	ldr	r3, [r5, #16]
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r0, #144
	adds	r3, r3, r2
	lsls	r0, r0, #4
	str	r3, [r5, #16]
	adds	r0, #81
	bl 0x0200a8e0
	movs	r3, #52
	str	r3, [sp, #4]
	movs	r0, #54
	movs	r1, #58
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200a960
.L_020011dc:
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r0, r1, #0
	sub	sp, #8
	bl 0x0200a9a8
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r6, r3, #20
	cmp	r6, #53
	bne.n	.L_02001220
	movs	r0, #30
	bl 0x0200a868
	ldr	r3, [r5, #16]
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r0, #144
	adds	r3, r3, r2
	lsls	r0, r0, #4
	str	r3, [r5, #16]
	adds	r0, #82
	bl 0x0200a8e0
	movs	r3, #58
	str	r3, [sp, #4]
	movs	r0, #54
	movs	r1, #58
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200a960
.L_02001220:
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	adds	r0, r1, #0
	sub	sp, #8
	bl 0x0200a9a8
	adds	r7, r0, #0
	bl 0x0200a998
	movs	r0, #0
	bl 0x0200aa48
	ldr	r3, [pc, #364]
	movs	r0, #30
	str	r3, [r7, #108]
	bl 0x0200a868
	movs	r0, #8
	bl 0x0200a834
	movs	r0, #220
	bl 0x0200aa70
	movs	r5, #3
.L_02001252:
	bl 0x0200a878
	movs	r6, #254
	lsls	r6, r6, #7
	adds	r6, #255
	movs	r1, #0
	ands	r0, r6
	bl 0x0200aa20
	movs	r0, #15
	bl 0x0200aa28
	movs	r0, #10
	bl 0x0200a868
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200aa20
	movs	r0, #10
	bl 0x0200aa28
	subs	r5, #1
	movs	r0, #10
	bl 0x0200a868
	cmp	r5, #0
	bge.n	.L_02001252
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200aa20
	movs	r0, #20
	bl 0x0200aa28
	movs	r0, #20
	bl 0x0200a990
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #105
	bl 0x0200a8e0
	movs	r3, #48
	str	r3, [sp, #4]
	movs	r5, #16
	movs	r0, #15
	movs	r1, #48
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200a960
	movs	r3, #50
	str	r3, [sp, #4]
	movs	r1, #50
	movs	r2, #1
	movs	r0, #17
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200a960
	ldr	r3, [pc, #216]
	movs	r0, #8
	str	r3, [r7, #108]
	bl 0x0200a5a8
	movs	r3, #94
	str	r3, [sp, #4]
	movs	r5, #14
	movs	r0, #64
	movs	r1, #68
	movs	r2, #5
	movs	r3, #13
	str	r5, [sp, #0]
	bl 0x0200a968
	movs	r3, #34
	str	r3, [sp, #4]
	movs	r2, #5
	movs	r3, #8
	movs	r1, #68
	movs	r0, #64
	str	r5, [sp, #0]
	bl 0x0200a960
	movs	r0, #30
	bl 0x0200a990
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200aa20
	movs	r0, #30
	bl 0x0200aa28
	movs	r0, #15
	bl 0x0200a990
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200aa00
	movs	r0, #248
	movs	r2, #190
	movs	r3, #1
	lsls	r0, r0, #16
	ldr	r1, [pc, #124]
	lsls	r2, r2, #18
	bl 0x0200aa08
	bl 0x0200aa10
	movs	r1, #132
	movs	r2, #198
	lsls	r2, r2, #18
	lsls	r1, r1, #17
	movs	r0, #15
	bl 0x0200a9d8
	movs	r0, #15
	bl 0x0200a9a8
	movs	r1, #0
	bl 0x0200a978
	movs	r0, #15
	bl 0x0200a9a8
	movs	r3, #0
	adds	r0, #89
	strb	r3, [r0, #0]
	movs	r0, #15
	bl 0x0200a9a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #15
	bl 0x0200a9a8
	ldr	r3, [pc, #56]
	movs	r5, #5
	str	r3, [r0, #108]
	movs	r0, #30
	bl 0x0200a990
.L_02001384:
	movs	r0, #184
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200aa70
	subs	r5, #1
	movs	r0, #10
	bl 0x0200a868
	cmp	r5, #0
	bge.n	.L_02001384
	movs	r0, #30
	bl 0x0200a990
	bl 0x0200a9a0
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x02008b11
	.4byte 0x02008b59
	.4byte 0xffe00000
	.2byte 0x9d99
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_020013d0
	ldr	r0, [pc, #24]
	b.n	.L_020013dc
.L_020013d0:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020013da
	ldr	r0, [pc, #24]
	b.n	.L_020013dc
.L_020013da:
	ldr	r0, [pc, #24]
.L_020013dc:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000009e
	.4byte 0x0200b220
	.4byte 0x0000009f
	.4byte 0x0200b2d4
	.2byte 0xb214
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200aa68
	bl 0x0200a9a8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200145a
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L_0200145a
	ldr	r3, [pc, #68]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	ldrb	r2, [r3, #0]
	movs	r1, #2
	ldr	r4, [r5, #80]
	eors	r2, r1
	negs	r3, r2
	orrs	r3, r2
	ldrb	r0, [r4, #9]
	lsrs	r3, r3, #31
	movs	r2, #13
	subs	r1, r1, r3
	negs	r2, r2
	movs	r3, #3
	ands	r1, r3
	adds	r3, r2, #0
	lsls	r1, r1, #2
	ands	r3, r0
	orrs	r3, r1
	strb	r3, [r4, #9]
	adds	r4, #37
	ldrb	r3, [r4, #0]
	ands	r2, r3
	orrs	r2, r1
	adds	r1, r5, #0
	adds	r1, #35
	strb	r2, [r4, #0]
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_0200145a:
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #200]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r5, r1
	ldr	r0, [r5, #0]
	bl 0x0200a9a8
	adds	r6, r0, #0
	adds	r7, r6, #0
	adds	r7, #85
	ldrb	r2, [r7, #0]
	movs	r0, #153
	lsls	r0, r0, #2
	mov	r8, r2
	bl 0x0200aa70
	bl 0x0200a998
	movs	r0, #0
	bl 0x0200aa48
	bl 0x0200aa30
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a978
	movs	r3, #0
	strb	r3, [r7, #0]
	ldr	r3, [pc, #144]
	movs	r1, #128
	str	r3, [r6, #12]
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200a9b0
	movs	r5, #15
.L_020014b4:
	ldr	r3, [r6, #12]
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r6, #12]
	ldrh	r3, [r6, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	strh	r3, [r6, #6]
	movs	r0, #1
	subs	r5, #1
	bl 0x0200a868
	cmp	r5, #0
	bge.n	.L_020014b4
	adds	r0, r6, #0
	bl 0x02009d30
	mov	r3, r8
	strb	r3, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r6, #40]
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200a978
	ldr	r3, [pc, #72]
	movs	r2, #64
	ldr	r3, [r3, #0]
	ands	r3, r2
	ldr	r2, [pc, #56]
	cmp	r3, #0
	beq.n	.L_02001510
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r2, r1
	movs	r1, #132
	movs	r2, #190
	ldr	r0, [r3, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200a9c0
	b.n	.L_02001524
.L_02001510:
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r2, r1
	movs	r1, #132
	movs	r2, #206
	ldr	r0, [r3, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200a9c0
.L_02001524:
	bl 0x0200a9a0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff00000
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #156]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200a9a8
	adds	r6, r0, #0
	adds	r7, r6, #0
	adds	r7, #85
	ldrb	r3, [r7, #0]
	mov	r8, r3
	bl 0x0200a998
	movs	r0, #0
	bl 0x0200aa48
	bl 0x0200aa30
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a978
	movs	r3, #0
	strb	r3, [r7, #0]
	ldr	r3, [pc, #108]
	movs	r1, #128
	str	r3, [r6, #12]
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200a9b0
	movs	r5, #23
.L_02001588:
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldrh	r3, [r6, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	strh	r3, [r6, #6]
	movs	r0, #1
	subs	r5, #1
	bl 0x0200a868
	cmp	r5, #0
	bge.n	.L_02001588
	adds	r0, r6, #0
	bl 0x02009d30
	mov	r3, r8
	strb	r3, [r7, #0]
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r6, #40]
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200a978
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #170
	movs	r2, #228
	ldr	r0, [r3, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x0200a9c0
	bl 0x0200a9a0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xffc0
	.2byte 0xb560
	adds	r1, r0, #0
	adds	r1, #100
	ldrh	r3, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r6, [r0, #80]
	strh	r3, [r1, #0]
	adds	r5, r0, #0
	adds	r5, #102
	ldrh	r3, [r5, #0]
	movs	r0, #128
	adds	r3, #32
	strh	r3, [r5, #0]
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r0, r0, #18
	lsls	r2, r2, #2
	cmp	r3, r0
	ble.n	.L_02001614
	strh	r2, [r5, #0]
.L_02001614:
	movs	r2, #0
	ldrsh	r0, [r1, r2]
	bl 0x0200a880
	movs	r3, #0
	ldrsh	r1, [r5, r3]
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x8270
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, lr}
	adds	r2, r0, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #6
	adds	r3, r3, r1
	ldr	r6, [r0, #80]
	strh	r3, [r2, #0]
	adds	r5, r0, #0
	adds	r5, #102
	ldrh	r3, [r5, #0]
	subs	r3, #16
	strh	r3, [r5, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_02001656
	movs	r3, #0
	strh	r3, [r5, #0]
.L_02001656:
	movs	r3, #0
	ldrsh	r0, [r2, r3]
	bl 0x0200a880
	movs	r3, #0
	ldrsh	r1, [r5, r3]
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x8270
	pop	{r5, r6, pc}
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, lr}
	adds	r3, r0, #0
	ldr	r5, [r3, #80]
	adds	r3, #100
	ldrh	r0, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r0, r0, r2
	strh	r0, [r3, #0]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	bl 0x0200a880
	cmp	r0, #0
	bge.n	.L_02001690
	adds	r0, #63
.L_02001690:
	movs	r2, #192
	asrs	r3, r0, #6
	lsls	r2, r2, #3
	adds	r3, r3, r2
	strh	r3, [r5, #18]
	pop	{r5, pc}
	push	{r5, r6, lr}
	sub	sp, #8
	bl 0x0200a998
	movs	r0, #0
	bl 0x0200aa48
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	bl 0x0200aa08
	ldr	r3, [pc, #292]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a9d8
	bl 0x0200aa30
	bl 0x0200aa40
	movs	r0, #184
	bl 0x0200aa70
	movs	r6, #17
.L_020016de:
	adds	r0, r6, #0
	bl 0x0200a9a8
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #7
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #3
	str	r3, [r5, #52]
	ldr	r3, [pc, #244]
	adds	r6, #1
	str	r3, [r5, #108]
	bl 0x0200a878
	adds	r3, r5, #0
	adds	r3, #100
	movs	r2, #0
	adds	r5, #102
	strh	r0, [r3, #0]
	strh	r2, [r5, #0]
	cmp	r6, #21
	ble.n	.L_020016de
	movs	r0, #60
	bl 0x0200a990
	movs	r3, #53
	str	r3, [sp, #4]
	movs	r5, #87
	movs	r3, #4
	movs	r0, #64
	movs	r1, #69
	movs	r2, #11
	str	r5, [sp, #0]
	bl 0x0200a958
	movs	r1, #196
	movs	r2, #206
	movs	r0, #17
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200a9b8
	movs	r1, #212
	movs	r2, #206
	movs	r0, #18
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200a9b8
	movs	r1, #228
	movs	r2, #206
	movs	r0, #19
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200a9b8
	movs	r1, #244
	movs	r2, #206
	movs	r0, #20
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200a9b8
	movs	r1, #130
	movs	r2, #206
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	movs	r0, #21
	bl 0x0200a9b8
	movs	r0, #240
	bl 0x0200a990
	movs	r3, #50
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #66
	movs	r2, #11
	movs	r3, #7
	str	r5, [sp, #0]
	bl 0x0200a958
	movs	r6, #17
.L_02001786:
	adds	r0, r6, #0
	bl 0x0200a9a8
	ldr	r3, [pc, #92]
	adds	r6, #1
	str	r3, [r0, #108]
	cmp	r6, #21
	ble.n	.L_02001786
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x0200aa70
	movs	r0, #60
	bl 0x0200a990
	movs	r0, #21
	bl 0x0200a9a8
	ldr	r3, [pc, #68]
	str	r3, [r0, #108]
	movs	r3, #0
	adds	r0, #100
	strh	r3, [r0, #0]
	bl 0x0200aa38
	bl 0x0200aa40
	movs	r0, #10
	adds	r0, #255
	bl 0x0200a8e0
	movs	r0, #48
	adds	r0, #255
	bl 0x0200a8e8
	movs	r0, #77
	bl 0x0200aa18
	bl 0x0200a9a0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #85
	bl 0x0200a8e0
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x020095e9
	.4byte 0x02009631
	.2byte 0x9671
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r7, [pc, #724]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [pc, #700]
	adds	r2, #88
	str	r2, [r3, #0]
	adds	r2, #92
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #98
	adds	r2, r7, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	ldrb	r2, [r1, #23]
	subs	r3, #14
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #23]
	movs	r0, #170
	sub	sp, #8
	bl 0x0200aa58
	cmp	r6, r5
	beq.n	.L_02001842
	b.n	.L_02001ae0
.L_02001842:
	movs	r0, #8
	bl 0x0200a9a8
	movs	r1, #4
	bl 0x0200a9e8
	movs	r0, #9
	bl 0x0200a9a8
	movs	r1, #4
	bl 0x0200a9e8
	movs	r0, #10
	bl 0x0200a9a8
	movs	r1, #4
	bl 0x0200a9e8
	movs	r0, #11
	bl 0x0200a9a8
	movs	r1, #4
	bl 0x0200a9e8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #107
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_0200188e
	movs	r0, #12
	bl 0x0200a9a8
	movs	r1, #5
	bl 0x0200a9e8
	b.n	.L_0200189a
.L_0200188e:
	movs	r0, #12
	bl 0x0200a9a8
	movs	r1, #6
	bl 0x0200a9e8
.L_0200189a:
	movs	r0, #8
	bl 0x02009f60
	movs	r0, #9
	bl 0x02009f60
	movs	r0, #10
	bl 0x02009f60
	movs	r0, #11
	bl 0x02009f60
	movs	r0, #13
	bl 0x02009f60
	movs	r0, #14
	bl 0x02009f60
	movs	r5, #15
.L_020018c0:
	adds	r0, r5, #0
	adds	r5, #1
	bl 0x02009f60
	cmp	r5, #23
	ble.n	.L_020018c0
	movs	r5, #8
.L_020018ce:
	adds	r0, r5, #0
	movs	r1, #3
	adds	r5, #1
	bl 0x0200a9f0
	cmp	r5, #14
	ble.n	.L_020018ce
	movs	r5, #15
.L_020018de:
	adds	r0, r5, #0
	movs	r1, #1
	adds	r5, #1
	bl 0x0200a9f0
	cmp	r5, #23
	ble.n	.L_020018de
	movs	r0, #15
	bl 0x0200a9a8
	movs	r3, #2
	adds	r0, #92
	strb	r3, [r0, #0]
	movs	r1, #3
	movs	r0, #15
	bl 0x0200a9e0
	movs	r5, #16
.L_02001902:
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200a9e0
	adds	r0, r5, #0
	movs	r1, #0
	adds	r5, #1
	bl 0x0200a9e0
	cmp	r5, #23
	ble.n	.L_02001902
	movs	r0, #149
	lsls	r0, r0, #4
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02001932
	movs	r1, #130
	movs	r2, #146
	movs	r0, #25
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200a9d8
.L_02001932:
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200a8f0
	adds	r5, r0, #0
	movs	r0, #198
	lsls	r0, r0, #2
	bl 0x0200a8f0
	cmp	r0, #0
	beq.n	.L_0200195a
	movs	r3, #128
	lsls	r3, r3, #12
	lsls	r2, r0, #20
	lsls	r1, r5, #20
	adds	r1, r1, r3
	adds	r2, r2, r3
	movs	r0, #24
	bl 0x0200a9d8
.L_0200195a:
	bl 0x0200aa68
	bl 0x0200a9a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r1, #144
	strb	r3, [r0, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #352]
	bl 0x0200a870
	movs	r0, #28
	movs	r1, #1
	bl 0x0200a9f0
	movs	r1, #1
	movs	r0, #24
	bl 0x0200a9f0
	bl 0x0200a550
	bl 0x0200a76c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #104
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_020019ca
	movs	r0, #26
	bl 0x0200a5a8
	movs	r0, #26
	bl 0x0200a834
	movs	r0, #5
	bl 0x0200a948
	movs	r0, #6
	bl 0x0200a948
	movs	r0, #7
	bl 0x0200a940
	movs	r0, #8
	bl 0x0200a940
	movs	r0, #26
	bl 0x0200a9a8
	ldr	r3, [pc, #272]
	str	r3, [r0, #108]
.L_020019ca:
	movs	r1, #1
	movs	r0, #26
	bl 0x0200a9f0
	ldr	r3, [pc, #248]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r7, r3, r1
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #5
	bne.n	.L_020019e6
	bl 0x0200969c
.L_020019e6:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #85
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02001a6c
	movs	r1, #196
	movs	r2, #206
	movs	r0, #17
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200a9d8
	movs	r1, #212
	movs	r2, #206
	movs	r0, #18
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200a9d8
	movs	r1, #228
	movs	r2, #206
	movs	r0, #19
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200a9d8
	movs	r1, #244
	movs	r2, #206
	movs	r0, #20
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200a9d8
	movs	r1, #130
	movs	r2, #206
	movs	r0, #21
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200a9d8
	movs	r3, #24
	movs	r2, #53
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #66
	movs	r2, #9
	movs	r3, #3
	bl 0x0200a960
	movs	r3, #87
	movs	r2, #50
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #7
	movs	r0, #64
	movs	r1, #66
	movs	r2, #11
	bl 0x0200a958
	movs	r0, #21
	bl 0x0200a9a8
	ldr	r3, [pc, #112]
	str	r3, [r0, #108]
.L_02001a6c:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #107
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02001ab8
	movs	r3, #33
	movs	r2, #41
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #62
	movs	r1, #41
	movs	r2, #1
	movs	r3, #2
	bl 0x0200a960
	movs	r6, #96
	movs	r5, #32
	movs	r0, #116
	movs	r1, #95
	movs	r2, #3
	movs	r3, #13
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a968
	movs	r0, #122
	movs	r1, #95
	movs	r2, #3
	movs	r3, #13
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200a968
	movs	r0, #12
	bl 0x02009f60
.L_02001ab8:
	bl 0x02008844
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	cmp	r3, #4
	beq.n	.L_02001ac6
	b.n	.L_02001d20
.L_02001ac6:
	bl 0x0200a158
	b.n	.L_02001d20
	.4byte 0x02000240
	.4byte 0x0000009e
	.4byte 0x020093f9
	.4byte 0x02008b59
	.2byte 0x9671
	.2byte 0x0200
.L_02001ae0:
	ldr	r3, [pc, #324]
	cmp	r6, r3
	beq.n	.L_02001ae8
	b.n	.L_02001d20
.L_02001ae8:
	movs	r0, #14
	bl 0x0200a9a8
	movs	r1, #0
	bl 0x0200a978
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02001b16
	movs	r3, #44
	movs	r2, #32
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #42
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a960
.L_02001b16:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02001b38
	movs	r3, #46
	movs	r2, #32
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #48
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a960
.L_02001b38:
	movs	r0, #146
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02001b5a
	movs	r3, #46
	movs	r2, #44
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #44
	movs	r1, #44
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a960
.L_02001b5a:
	movs	r0, #12
	movs	r1, #2
	bl 0x0200a9e0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #81
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02001b92
	movs	r1, #214
	movs	r2, #210
	movs	r0, #12
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200a9d8
	movs	r3, #53
	movs	r2, #52
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #54
	movs	r1, #58
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a960
.L_02001b92:
	movs	r0, #13
	movs	r1, #2
	bl 0x0200a9e0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #82
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02001bca
	movs	r1, #214
	movs	r2, #234
	movs	r0, #13
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200a9d8
	movs	r3, #53
	movs	r2, #58
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #54
	movs	r1, #58
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a960
.L_02001bca:
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	cmp	r2, #5
	beq.n	.L_02001bdc
	cmp	r2, #7
	bne.n	.L_02001c2c
.L_02001bdc:
	bl 0x0200a550
	bl 0x0200a76c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #104
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02001c00
	movs	r0, #14
	bl 0x0200a5a8
	movs	r0, #14
	bl 0x0200a834
	b.n	.L_02001c18
.L_02001c00:
	movs	r0, #7
	bl 0x0200a948
	movs	r0, #8
	bl 0x0200a948
	movs	r0, #5
	bl 0x0200a940
	movs	r0, #6
	bl 0x0200a940
.L_02001c18:
	movs	r0, #14
	bl 0x0200a9a8
	movs	r1, #0
	bl 0x0200a978
	b.n	.L_02001cf4
	.2byte 0x0000
	.2byte 0x009f
	.2byte 0x0000
.L_02001c2c:
	bl 0x0200a550
	bl 0x0200a76c
	movs	r0, #8
	bl 0x0200a9a8
	ldr	r3, [pc, #128]
	str	r3, [r0, #108]
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #105
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02001cf4
	movs	r0, #8
	bl 0x0200a5a8
	movs	r0, #8
	bl 0x0200a834
	movs	r1, #132
	movs	r2, #198
	lsls	r2, r2, #18
	lsls	r1, r1, #17
	movs	r0, #15
	bl 0x0200a9d8
	movs	r0, #15
	bl 0x0200a9a8
	movs	r1, #0
	bl 0x0200a978
	movs	r0, #15
	bl 0x0200a9a8
	ldr	r3, [pc, #60]
	adds	r0, #89
	strb	r3, [r0, #0]
	movs	r0, #15
	bl 0x0200a9a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #15
	bl 0x0200a9a8
	ldr	r3, [pc, #40]
	movs	r5, #16
	str	r3, [r0, #108]
	movs	r3, #48
	str	r3, [sp, #4]
	movs	r0, #15
	movs	r1, #48
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200a960
	movs	r3, #50
	str	r3, [sp, #4]
	movs	r0, #17
	movs	r1, #50
	b.n	.L_02001cc4
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02008b59
	.2byte 0x9d99
	.2byte 0x0200
.L_02001cc4:
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200a960
	movs	r3, #94
	movs	r5, #14
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #68
	movs	r2, #5
	movs	r3, #13
	str	r5, [sp, #0]
	bl 0x0200a968
	movs	r3, #34
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #68
	movs	r2, #5
	movs	r3, #8
	str	r5, [sp, #0]
	bl 0x0200a960
.L_02001cf4:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200a8d8
	cmp	r0, #0
	bne.n	.L_02001d20
	ldr	r3, [pc, #36]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r5, r3, r2
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #1
	bne.n	.L_02001d14
	bl 0x02009460
.L_02001d14:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #6
	bne.n	.L_02001d20
	bl 0x0200953c
.L_02001d20:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	movs	r0, #128
	lsls	r0, r0, #11
	adds	r2, r2, r0
	adds	r3, r3, r0
	ldr	r1, [r6, #8]
	movs	r0, #14
	bl 0x0200a920
	ldr	r2, [r6, #80]
	adds	r5, r0, #0
	mov	r8, r2
	cmp	r5, #0
	beq.n	.L_02001d8c
	ldr	r3, [r6, #20]
	ldr	r7, [r5, #80]
	str	r3, [r5, #20]
	ldr	r1, [pc, #52]
	bl 0x0200a918
	adds	r3, r5, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	cmp	r7, #0
	beq.n	.L_02001d8c
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200a900
	strb	r5, [r7, #26]
	mov	r2, r8
	ldrb	r3, [r2, #9]
	ldrb	r1, [r7, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r7, #9]
.L_02001d8c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xac44
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #88]
	ldr	r7, [r3, #0]
	movs	r3, #15
	ands	r7, r3
	cmp	r7, #0
	bne.n	.L_02001df0
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	movs	r0, #14
	adds	r0, #255
	bl 0x0200a920
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001df0
	ldr	r1, [pc, #60]
	ldr	r6, [r5, #80]
	bl 0x0200a918
	adds	r3, r5, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	ldr	r3, [pc, #48]
	adds	r2, r5, #0
	str	r3, [r5, #12]
	adds	r2, #34
	movs	r3, #1
	strb	r3, [r2, #0]
	cmp	r6, #0
	beq.n	.L_02001df0
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200a900
	ldrb	r3, [r6, #9]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #8
	orrs	r2, r3
	strb	r7, [r6, #26]
	strb	r2, [r6, #9]
.L_02001df0:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0x0200ac50
	.2byte 0x8000
	.2byte 0xfff8
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r6, [pc, #320]
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #133
	ldr	r2, [r3, #108]
	lsls	r0, r0, #2
	adds	r3, r6, r0
	movs	r1, #230
	ldr	r3, [r3, #0]
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldr	r2, [r2, #0]
	mov	r8, r3
	mov	r0, r8
	mov	sl, r2
	sub	sp, #12
	bl 0x0200a9a8
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r6, r2
	adds	r5, r0, #0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #276]
	cmp	r2, r3
	bne.n	.L_02001e44
	ldr	r3, [r5, #20]
	cmp	r3, #0
	beq.n	.L_02001f40
.L_02001e44:
	ldr	r3, [r5, #8]
	mov	r7, sp
	str	r3, [r7, #0]
	movs	r1, #128
	ldr	r3, [r5, #12]
	lsls	r1, r1, #10
	str	r3, [r7, #4]
	adds	r0, r5, #0
	ldr	r3, [r5, #16]
	adds	r3, r3, r1
	str	r3, [r7, #8]
	adds	r1, r7, #0
	bl 0x0200a970
	ldr	r3, [pc, #240]
	movs	r2, #4
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02001e74
	adds	r0, r5, #0
	bl 0x02009d30
.L_02001e74:
	cmp	r6, #0
	bge.n	.L_02001ec6
	movs	r1, #129
	mov	r0, r8
	lsls	r1, r1, #1
	bl 0x0200a9f8
	ldr	r3, [r5, #16]
	movs	r0, #128
	lsls	r0, r0, #12
	adds	r3, r3, r0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	bl 0x0200a930
	adds	r0, r5, #0
	movs	r1, #49
	bl 0x0200a910
	adds	r0, r5, #0
	bl 0x0200a938
.L_02001ea2:
	movs	r0, #1
	bl 0x0200a868
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	cmp	r2, r3
	bne.n	.L_02001ea2
	adds	r0, r5, #0
	bl 0x02009d30
	adds	r0, r5, #0
	movs	r1, #49
	bl 0x0200a910
	movs	r0, #3
	bl 0x0200a868
	b.n	.L_02001f40
.L_02001ec6:
	ldr	r3, [r5, #8]
	movs	r1, #128
	str	r3, [r7, #0]
	lsls	r1, r1, #12
	ldr	r3, [r5, #12]
	adds	r0, r5, #0
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	adds	r3, r3, r1
	str	r3, [r7, #8]
	adds	r1, r7, #0
	bl 0x0200a970
	adds	r6, r0, #0
	cmp	r6, #0
	bgt.n	.L_02001f40
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #108]
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r1, r7, #0
	ldr	r3, [r5, #12]
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r7, #8]
	bl 0x0200a970
	adds	r6, r0, #0
	cmp	r6, #0
	bgt.n	.L_02001f40
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #80]
	ldr	r0, [pc, #76]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r1, r7, #0
	ldr	r3, [r5, #12]
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	adds	r3, r3, r0
	str	r3, [r7, #8]
	adds	r0, r5, #0
	bl 0x0200a970
	adds	r6, r0, #0
	cmp	r6, #0
	bgt.n	.L_02001f40
	mov	r1, sl
	ldr	r3, [r1, #16]
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r1, #16]
	ldr	r3, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #16]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r5, #6]
.L_02001f40:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000009e
	.4byte 0x0300122c
	.4byte 0x0005b333
	.2byte 0x4ccd
	.2byte 0xfffa
	.2byte 0xb520
	bl 0x0200a9a8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001f8c
	movs	r1, #126
	adds	r1, #255
	ldr	r0, [r5, #80]
	bl 0x0200a908
	movs	r3, #0
	strb	r3, [r0, #5]
	strb	r3, [r0, #6]
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200a910
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200a910
.L_02001f8c:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #148]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	adds	r7, r0, #0
	ldr	r0, [r5, #0]
	bl 0x0200a9a8
	adds	r6, r0, #0
	bl 0x0200a998
	movs	r0, #0
	bl 0x0200aa48
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200a9f0
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r0, #215
	bl 0x0200aa70
	adds	r0, r6, #0
	movs	r1, #18
	bl 0x0200a910
	movs	r0, #153
	lsls	r0, r0, #2
	bl 0x0200aa70
	movs	r5, #0
.L_02001fe2:
	cmp	r5, #30
	bne.n	.L_02001fea
	bl 0x0200aa38
.L_02001fea:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #60]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldrh	r3, [r6, #6]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	strh	r3, [r6, #6]
	movs	r3, #7
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_0200200e
	movs	r0, #15
	bl 0x0200a9a8
	bl 0x02009d30
.L_0200200e:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200a868
	cmp	r5, #59
	ble.n	.L_02001fe2
	bl 0x0200a9a0
	adds	r0, r7, #0
	bl 0x0200aa18
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xc000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #128]
	sub	sp, #56
	ldr	r2, [r3, #0]
	mov	r8, r3
	movs	r3, #1
	ands	r3, r2
	adds	r7, r0, #0
	cmp	r3, #0
	beq.n	.L_020020ac
	movs	r3, #7
	add	r6, sp, #16
	str	r3, [r6, #4]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0200205a
	movs	r3, #5
	str	r3, [r6, #4]
.L_0200205a:
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	movs	r5, #0
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	str	r5, [r6, #0]
	bl 0x0200a878
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r4, r0, #1
	adds	r4, r4, r0
	lsls	r3, r4, #4
	adds	r4, r4, r3
	lsls	r3, r4, #8
	adds	r4, r4, r3
	mov	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #15
	ldr	r0, [r7, #8]
	ands	r2, r3
	movs	r3, #8
	subs	r3, r3, r2
	ldr	r1, [r7, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	movs	r3, #208
	lsls	r3, r3, #13
	adds	r1, r1, r3
	movs	r3, #176
	lsls	r3, r3, #12
	ldr	r2, [r7, #16]
	negs	r4, r4
	str	r3, [sp, #8]
	movs	r3, #0
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	str	r6, [sp, #12]
	bl 0x0200815c
.L_020020ac:
	movs	r0, #0
	add	sp, #56
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r5, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	mov	sl, r0
	ldr	r0, [r5, #0]
	bl 0x0200a9a8
	adds	r6, r0, #0
	bl 0x0200a998
	movs	r0, #0
	bl 0x0200aa48
	movs	r0, #228
	bl 0x0200aa70
	ldr	r3, [pc, #108]
	movs	r2, #0
	str	r3, [r6, #108]
	mov	r8, r2
	adds	r3, r6, #0
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #51
	str	r3, [r6, #48]
	movs	r1, #2
	ldr	r0, [r5, #0]
	bl 0x0200a9e0
	movs	r2, #8
	negs	r2, r2
	movs	r1, #0
	ldr	r0, [r5, #0]
	bl 0x0200a9c8
	ldr	r0, [r5, #0]
	bl 0x0200a9d0
	ldr	r0, [r5, #0]
	bl 0x0200a9a8
	movs	r1, #9
	bl 0x0200a9e8
	ldr	r0, [r5, #0]
	bl 0x0200a9a8
	movs	r1, #0
	bl 0x0200a978
	mov	r3, r8
	str	r3, [r6, #108]
	bl 0x0200aa38
	bl 0x0200aa40
	mov	r0, sl
	bl 0x0200aa18
	bl 0x0200a9a0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xa031
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #228]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200a9a8
	adds	r6, r0, #0
	movs	r0, #10
	adds	r0, #255
	bl 0x0200a8d8
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_0200223e
	bl 0x0200a998
	movs	r0, #0
	bl 0x0200aa48
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	negs	r0, r0
	negs	r1, r1
	movs	r3, #0
	bl 0x0200aa08
	movs	r3, #85
	adds	r3, r3, r6
	strb	r7, [r3, #0]
	mov	r8, r3
	movs	r2, #10
	ldrsh	r1, [r6, r2]
	movs	r3, #18
	ldrsh	r2, [r6, r3]
	ldr	r3, [pc, #156]
	lsls	r2, r2, #16
	adds	r2, r2, r3
	lsls	r1, r1, #16
	ldr	r0, [r5, #0]
	bl 0x0200a9d8
	ldr	r0, [r5, #0]
	bl 0x0200a9a8
	movs	r1, #9
	bl 0x0200a9e8
	ldr	r0, [r5, #0]
	bl 0x0200a9a8
	movs	r1, #0
	bl 0x0200a978
	bl 0x0200aa30
	movs	r0, #228
	bl 0x0200aa70
	ldr	r3, [pc, #112]
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	str	r3, [r6, #108]
	ldr	r0, [r5, #0]
	adds	r1, #102
	adds	r2, #51
	bl 0x0200a9b0
	movs	r2, #8
	movs	r1, #0
	ldr	r0, [r5, #0]
	bl 0x0200aa50
	ldr	r0, [r5, #0]
	bl 0x0200a9a8
	movs	r1, #0
	bl 0x0200a9e8
	ldr	r0, [r5, #0]
.L_02002206:
	bl 0x0200a9a8
	movs	r1, #1
	bl 0x0200a978
	ldr	r1, [r6, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r2, #10
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200aa50
	movs	r3, #3
	mov	r2, r8
	strb	r3, [r2, #0]
	str	r7, [r6, #108]
	bl 0x0200aa60
	bl 0x0200aa40
	bl 0x0200a9a0
.L_0200223e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfff00000
	.2byte 0xa031
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
	ldr	r3, [pc, #324]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r0, [r2, #0]
	ldr	r2, [r2, #4]
	ands	r0, r3
	ands	r2, r3
	ldr	r3, [r1, #0]
	ldr	r7, [pc, #312]
	ldr	r3, [r3, #4]
	mov	r8, r2
	mov	r9, r3
	ldrh	r3, [r7, #4]
	ldr	r2, [pc, #308]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	mov	sl, r0
	lsrs	r3, r3, #5
	mov	fp, r3
	bl 0x0200a878
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r0, r3
	bcs.n	.L_020022de
	ldr	r6, [r7, #0]
	cmp	r6, #0
	beq.n	.L_020022de
	ldrh	r2, [r7, #6]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r7
	adds	r5, r3, #0
	bl 0x0200a878
	ldr	r3, [r6, #8]
	lsls	r2, r0, #1
	adds	r2, r2, r0
	ldr	r0, [pc, #256]
	lsls	r2, r2, #4
	adds	r3, r3, r2
	adds	r5, #12
	adds	r3, r3, r0
	str	r3, [r5, #0]
	movs	r2, #0
	ldr	r3, [r6, #12]
	movs	r1, #252
	str	r3, [r5, #4]
	lsls	r1, r1, #14
	ldr	r3, [r6, #16]
	str	r2, [r5, #12]
	str	r3, [r5, #8]
	ldrh	r3, [r7, #6]
	adds	r3, #1
	strh	r3, [r7, #6]
	lsls	r3, r3, #16
	cmp	r3, r1
	bls.n	.L_020022de
	strh	r2, [r7, #6]
.L_020022de:
	adds	r5, r7, #0
	adds	r5, #12
	movs	r6, #63
.L_020022e4:
	ldr	r1, [r5, #0]
	cmp	r1, #0
	beq.n	.L_02002396
	ldr	r2, [r5, #8]
	movs	r0, #0
	bl 0x0200a950
	ldr	r3, [r5, #4]
	cmp	r0, r3
	ble.n	.L_02002302
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r3, r3, r2
.L_020022fe:
	str	r3, [r5, #4]
	b.n	.L_0200230a
.L_02002302:
	ldr	r3, [r5, #8]
	ldr	r0, [pc, #180]
	adds	r3, r3, r0
	str	r3, [r5, #8]
.L_0200230a:
	ldr	r3, [r5, #4]
	asrs	r3, r3, #20
	cmp	r3, #23
	bne.n	.L_02002316
	movs	r3, #0
	str	r3, [r5, #0]
.L_02002316:
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #48
	bne.n	.L_02002322
	movs	r3, #0
	str	r3, [r5, #0]
.L_02002322:
	ldr	r3, [r5, #12]
	mov	r1, sl
	adds	r3, #1
	str	r3, [r5, #12]
	ldr	r3, [r5, #0]
	mov	r0, r8
	subs	r4, r3, r1
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #4]
	mov	r2, r9
	subs	r1, r1, r0
	subs	r1, r1, r2
	subs	r3, r3, r2
	subs	r2, r1, r3
	adds	r3, r3, r1
	asrs	r3, r3, #16
	asrs	r0, r4, #16
	adds	r1, r3, #0
	movs	r3, #167
	subs	r4, r0, #4
	asrs	r2, r2, #16
	adds	r0, #11
	lsls	r3, r3, #1
	subs	r2, #4
	adds	r1, #58
	cmp	r0, r3
	bhi.n	.L_02002396
	movs	r0, #16
	negs	r0, r0
	cmp	r2, r0
	ble.n	.L_02002396
	cmp	r2, #239
	bgt.n	.L_02002396
	adds	r3, #177
	ands	r4, r3
	movs	r3, #255
	ands	r2, r3
	movs	r3, #0
	str	r3, [r5, #16]
	lsls	r3, r4, #16
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #6
	orrs	r2, r3
	ldr	r3, [r5, #12]
	str	r2, [r5, #20]
	movs	r2, #3
	ands	r3, r2
	lsls	r3, r3, #1
	movs	r2, #128
	add	r3, fp
	lsls	r2, r2, #3
	orrs	r3, r2
	adds	r0, r5, #0
	str	r3, [r5, #24]
	adds	r0, #16
	bl 0x0200a8d0
.L_02002396:
	subs	r6, #1
	adds	r5, #28
	cmp	r6, #0
	bge.n	.L_020022e4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xffff0000
	.4byte 0x0200b4f0
	.4byte 0x020036e0
	.4byte 0xffe80000
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r3, [pc, #328]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r0, [r2, #0]
	ldr	r2, [r2, #4]
	ands	r0, r3
	ands	r2, r3
	ldr	r3, [r1, #0]
	ldr	r7, [pc, #316]
	ldr	r3, [r3, #4]
	mov	r8, r2
	mov	r9, r3
	ldrh	r3, [r7, #4]
	ldr	r2, [pc, #312]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	mov	sl, r0
	lsrs	r3, r3, #5
	mov	fp, r3
	bl 0x0200a878
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r0, r3
	bcs.n	.L_0200244e
	ldr	r6, [r7, #0]
	cmp	r6, #0
	beq.n	.L_0200244e
	ldrh	r2, [r7, #6]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r7
	adds	r5, r3, #0
	bl 0x0200a878
	ldr	r3, [r6, #8]
	lsls	r2, r0, #1
	adds	r2, r2, r0
	ldr	r0, [pc, #260]
	lsls	r2, r2, #3
	adds	r3, r3, r2
	adds	r5, #12
	adds	r3, r3, r0
	str	r3, [r5, #0]
	movs	r2, #0
	ldr	r3, [r6, #12]
	movs	r1, #252
	str	r3, [r5, #4]
	lsls	r1, r1, #14
	ldr	r3, [r6, #16]
	str	r2, [r5, #12]
	str	r3, [r5, #8]
	ldrh	r3, [r7, #6]
	adds	r3, #1
	strh	r3, [r7, #6]
	lsls	r3, r3, #16
	cmp	r3, r1
	bls.n	.L_0200244e
	strh	r2, [r7, #6]
.L_0200244e:
	adds	r5, r7, #0
	adds	r5, #12
	movs	r6, #63
.L_02002454:
	ldr	r1, [r5, #0]
	cmp	r1, #0
	beq.n	.L_0200250a
	ldr	r2, [r5, #8]
	movs	r0, #0
	bl 0x0200a950
	ldr	r3, [r5, #4]
	cmp	r0, r3
	bge.n	.L_02002470
	ldr	r2, [pc, #196]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	b.n	.L_0200247a
.L_02002470:
	ldr	r3, [r5, #8]
	movs	r0, #128
	lsls	r0, r0, #10
	adds	r3, r3, r0
	str	r3, [r5, #8]
.L_0200247a:
	ldr	r3, [r5, #4]
	movs	r1, #4
	asrs	r3, r3, #20
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_0200248a
	movs	r3, #0
	str	r3, [r5, #0]
.L_0200248a:
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #50
	bne.n	.L_02002496
	movs	r3, #0
	str	r3, [r5, #0]
.L_02002496:
	ldr	r3, [r5, #12]
	mov	r2, sl
	adds	r3, #1
	str	r3, [r5, #12]
	ldr	r3, [r5, #0]
	ldr	r1, [r5, #8]
	subs	r4, r3, r2
	ldr	r3, [r5, #4]
	mov	r2, r8
	mov	r0, r9
	subs	r1, r1, r2
	subs	r1, r1, r0
	subs	r3, r3, r0
	subs	r2, r1, r3
	adds	r3, r3, r1
	asrs	r3, r3, #16
	asrs	r0, r4, #16
	adds	r1, r3, #0
	movs	r3, #167
	subs	r4, r0, #4
	asrs	r2, r2, #16
	adds	r0, #11
	lsls	r3, r3, #1
	subs	r2, #4
	adds	r1, #58
	cmp	r0, r3
	bhi.n	.L_0200250a
	movs	r0, #16
	negs	r0, r0
	cmp	r2, r0
	ble.n	.L_0200250a
	cmp	r2, #239
	bgt.n	.L_0200250a
	adds	r3, #177
	ands	r4, r3
	movs	r3, #255
	ands	r2, r3
	movs	r3, #0
	str	r3, [r5, #16]
	lsls	r3, r4, #16
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #6
	orrs	r2, r3
	ldr	r3, [r5, #12]
	str	r2, [r5, #20]
	movs	r2, #3
	ands	r3, r2
	lsls	r3, r3, #1
	movs	r2, #128
	add	r3, fp
	lsls	r2, r2, #3
	orrs	r3, r2
	adds	r0, r5, #0
	str	r3, [r5, #24]
	adds	r0, #16
	bl 0x0200a8d0
.L_0200250a:
	subs	r6, #1
	adds	r5, #28
	cmp	r6, #0
	bge.n	.L_02002454
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xffff0000
	.4byte 0x0200b4f0
	.4byte 0x020036e0
	.4byte 0xfff40000
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb500
	ldr	r3, [pc, #20]
	movs	r2, #8
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	ble.n	.L_02002546
	bl 0x0200a250
	b.n	.L_0200254a
.L_02002546:
	bl 0x0200a3c0
.L_0200254a:
	pop	{pc}
	.2byte 0xb4f0
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r6, [pc, #68]
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r3, [pc, #64]
	adds	r1, #12
	adds	r0, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2080
	lsls	r0, r0, #1
	bl 0x0200a8a0
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #48]
	bl 0x0200a8b8
	bl 0x0200a8c8
	strh	r0, [r6, #4]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r2, r5, #0
	ldrh	r0, [r6, #4]
	bl 0x0200a8c0
	adds	r0, r5, #0
	bl 0x0200a8a8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200a870
	pop	{r5, r6, pc}
	.4byte 0x0200b4f0
	.4byte 0x03000258
	.4byte 0x0200ac74
	.2byte 0xa535
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldr	r6, [pc, #32]
	cmp	r5, #0
	bne.n	.L_020025b6
	str	r5, [r6, #0]
	b.n	.L_020025ce
.L_020025b6:
	adds	r0, r5, #0
	bl 0x0200a9a8
	str	r0, [r6, #0]
	cmp	r5, #8
	bne.n	.L_020025ca
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	b.n	.L_020025cc
.L_020025ca:
	movs	r3, #1
.L_020025cc:
	strh	r3, [r6, #8]
.L_020025ce:
	pop	{r5, r6, pc}
	.2byte 0xb4f0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #32]
	ldr	r2, [r2, #116]
	adds	r3, #228
	ldr	r1, [r3, #0]
	ldr	r3, [r3, #4]
	mov	r8, r2
	mov	r5, r8
	movs	r2, #0
	adds	r5, #8
	mov	fp, r1
	mov	r9, r3
	mov	sl, r2
.L_020025fe:
	ldrh	r3, [r5, #28]
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
	adds	r2, r1, #0
	ands	r2, r3
	strh	r3, [r5, #28]
	cmp	r2, r1
	bne.n	.L_02002614
	b.n	.L_0200274a
.L_02002614:
	movs	r0, #179
	lsls	r0, r0, #1
	bl 0x0200a8d8
	cmp	r0, #0
	beq.n	.L_02002626
	ldrh	r3, [r5, #28]
	adds	r3, #1
	strh	r3, [r5, #28]
.L_02002626:
	ldrh	r2, [r5, #28]
	mov	r1, fp
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #164]
	lsls	r3, r3, #1
	adds	r4, r3, r2
	ldr	r3, [r5, #12]
	subs	r2, r3, r1
	cmp	r2, #0
	bge.n	.L_02002644
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
.L_02002644:
	movs	r1, #0
	ldrsh	r3, [r4, r1]
	asrs	r2, r2, #16
	adds	r7, r2, r3
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #20]
	adds	r4, #2
	subs	r3, r3, r2
	mov	r2, r9
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_02002664
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
.L_02002664:
	movs	r1, #0
	ldrsh	r2, [r4, r1]
	asrs	r3, r3, #16
	adds	r6, r3, r2
	adds	r3, r7, #0
	adds	r3, #16
	adds	r4, #2
	cmp	r3, #255
	bhi.n	.L_020026f4
	movs	r2, #32
	negs	r2, r2
	cmp	r6, r2
	blt.n	.L_020026f4
	cmp	r6, #159
	bgt.n	.L_020026f4
	ldrb	r3, [r5, #9]
	movs	r1, #13
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r5, #9]
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #48]
	ands	r7, r3
	ldrh	r3, [r5, #6]
	strb	r6, [r5, #4]
	ands	r3, r2
	orrs	r3, r7
	strh	r3, [r5, #6]
	mov	r2, r8
	ldrh	r3, [r4, #0]
	ldr	r1, [r2, #4]
	ldr	r2, [pc, #32]
	adds	r1, r1, r3
	ldr	r3, [pc, #32]
	adds	r4, #2
	ands	r1, r3
	ldrh	r3, [r5, #8]
	ldrb	r0, [r5, #5]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #8]
	movs	r2, #63
	ldrb	r1, [r4, #0]
	adds	r3, r2, #0
	b.n	.L_020026d8
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0xfffffc00
	.4byte 0x000003ff
	.2byte 0xacd4
	.2byte 0x0200
.L_020026d8:
	lsls	r1, r1, #6
	ands	r3, r0
	orrs	r3, r1
	strb	r3, [r5, #5]
	ldrb	r1, [r5, #7]
	ldrb	r3, [r4, #2]
	ands	r2, r1
	lsls	r3, r3, #6
	orrs	r2, r3
	strb	r2, [r5, #7]
	adds	r0, r5, #0
	movs	r1, #240
	bl 0x0200a8d0
.L_020026f4:
	ldrh	r3, [r5, #28]
	cmp	r3, #0
	bne.n	.L_0200274a
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #8
	add	r3, r8
	ldr	r6, [r3, #0]
	cmp	r6, #0
	beq.n	.L_02002740
	bl 0x0200a878
	ldr	r3, [r6, #0]
	lsls	r2, r0, #4
	ldr	r1, [pc, #80]
	subs	r2, r2, r0
	lsls	r2, r2, #4
	adds	r3, r3, r2
	adds	r7, r3, r1
	bl 0x0200a878
	ldr	r3, [r6, #8]
	lsls	r2, r0, #2
	adds	r2, r2, r0
	lsls	r2, r2, #5
	adds	r3, r3, r2
	ldr	r2, [pc, #60]
	str	r7, [r5, #12]
	adds	r6, r3, r2
	str	r6, [r5, #20]
	movs	r0, #0
	adds	r1, r7, #0
	adds	r2, r6, #0
	bl 0x0200a950
	movs	r3, #16
	str	r0, [r5, #16]
	b.n	.L_02002748
.L_02002740:
	movs	r3, #16
	str	r6, [r5, #12]
	str	r6, [r5, #20]
	str	r6, [r5, #16]
.L_02002748:
	strh	r3, [r5, #28]
.L_0200274a:
	movs	r3, #1
	add	sl, r3
	mov	r1, sl
	adds	r5, #32
	cmp	r1, #63
	bhi.n	.L_02002758
	b.n	.L_020025fe
.L_02002758:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xff880000
	.2byte 0x0000
	.2byte 0xffb0
	.2byte 0xb5e0
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #20
	movs	r0, #116
	sub	sp, #8
	bl 0x0200a890
	movs	r3, #128
	adds	r5, r0, #0
	movs	r0, #0
	str	r0, [sp, #0]
	adds	r7, r5, #0
	add	r0, sp, #4
	movs	r1, #0
	lsls	r3, r3, #19
	str	r1, [r0, #0]
	adds	r7, #8
	adds	r3, #212
	adds	r1, r5, #0
	ldr	r2, [pc, #136]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #56
	bl 0x0200a890
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #120]
	bl 0x0200a8b0
	bl 0x0200a8c8
	movs	r1, #192
	str	r0, [r5, #0]
	lsls	r1, r1, #2
	adds	r2, r6, #0
	bl 0x0200a8c0
	str	r0, [r5, #4]
	movs	r0, #56
	bl 0x0200a898
	movs	r3, #128
	lsls	r3, r3, #4
	ldr	r0, [sp, #0]
	adds	r3, #8
	adds	r5, r5, r3
	str	r0, [r5, #0]
	movs	r5, #0
.L_020027d4:
	movs	r2, #0
	adds	r3, r7, #0
	str	r7, [sp, #0]
	stmia	r3!, {r2}
	adds	r1, r3, #0
	ldr	r3, [pc, #72]
	stmia	r1!, {r3}
	movs	r3, #180
	adds	r0, r1, #0
	lsls	r3, r3, #8
	str	r0, [sp, #0]
	str	r3, [r1, #0]
	movs	r0, #0
	str	r2, [r7, #12]
	str	r2, [r7, #20]
	movs	r1, #0
	bl 0x0200a950
	ldr	r2, [pc, #32]
	adds	r3, r5, #0
	ands	r3, r2
	lsls	r0, r0, #16
	adds	r3, #1
	adds	r5, #1
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	adds	r7, #32
	cmp	r5, #63
	bls.n	.L_020027d4
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #24]
	bl 0x0200a870
	add	sp, #8
	b.n	.L_02002830
	.4byte 0x0000000f
	.4byte 0x85000205
	.4byte 0x0200b388
	.4byte 0x40000400
	.2byte 0xa5d5
	.2byte 0x0200
.L_02002830:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #116]
	adds	r6, r0, #0
	bl 0x0200a9a8
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #8
	adds	r5, r5, r3
	movs	r3, #0
	str	r3, [r5, #0]
	cmp	r6, #0
	beq.n	.L_0200285c
	cmp	r0, #0
	beq.n	.L_0200285c
	adds	r3, r0, #0
	adds	r3, #8
	str	r3, [r5, #0]
.L_0200285c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.irp EntryTarget, 0x03000528, 0x080000c1, 0x080000d1, 0x080000f9, 0x08000119, 0x08000129, 0x08000149, 0x08000151, 0x08000169, 0x08000179, 0x080001a1, 0x080001a9, 0x080001c9, 0x080001d1, 0x080001e9, 0x080003c9, 0x080003d1, 0x080003d9, 0x080003e9, 0x080003f1, 0x08020031, 0x08020059, 0x08020091, 0x080200a9, 0x080200c1, 0x080200c9, 0x08020149, 0x08020151, 0x08020199, 0x080201a1, 0x080201c1, 0x080201e1, 0x080201e9, 0x080201f1, 0x08020211, 0x08020219, 0x08020221, 0x08020279, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8099, 0x080c80c1, 0x080c80c9, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8171, 0x080c8201, 0x080c8219, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8279, 0x080c8379, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c85f9, 0x080c8689, 0x080c8691, 0x080c8779, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
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
	.4byte 0x02008479
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte 0x02008385
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte 0x02008385
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte 0x02008385
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x000000b4
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x02008479
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte 0x02008385
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte 0x02008385
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte 0x02008385
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x000000b4
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000026
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000026
	.4byte 0x7e080100
	.4byte 0x82e00afd
	.4byte 0xd35d1389
	.4byte 0x855a6d2b
	.4byte 0xfda62705
	.4byte 0xc4ed21f0
	.4byte 0xa9eea465
	.4byte 0xa82af7e3
	.4byte 0xa5e3d158
	.4byte 0xfc79e5e9
	.4byte 0xfe9e5e82
	.4byte 0x3ccb0e0c
	.4byte 0xdcbf3bae
	.4byte 0x5eaee6e3
	.4byte 0x80a3af17
	.4byte 0x5e3e0a7c
	.4byte 0x100fcf2f
	.4byte 0x4401362e
	.4byte 0x5cfd9b8f
	.4byte 0x71f039f8
	.4byte 0xe6075e04
	.4byte 0xf3783dc2
	.4byte 0x3ec75e86
	.4byte 0x0007c083
	.4byte 0xfff4fff8
	.4byte 0x00000014
	.4byte 0xfff80001
	.4byte 0x0014fff4
	.4byte 0x00010000
	.4byte 0xfff4fff8
	.4byte 0x00000014
	.4byte 0xfff80001
	.4byte 0x0010fff4
	.4byte 0x00010000
	.4byte 0xfff4fff8
	.4byte 0x00000010
	.4byte 0xfff80001
	.4byte 0x000cfff4
	.4byte 0x00010000
	.4byte 0xfff4fff8
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0x0000ffe0
	.4byte 0x00020002
	.4byte 0xffd00008
	.4byte 0x00020000
	.4byte 0x00100002
	.4byte 0x0000ffc0
	.4byte 0x00020002
	.4byte 0xffb00018
	.4byte 0x00020000
	.4byte 0x00200002
	.4byte 0x0000ffa0
	.4byte 0x00020002
	.4byte 0xff900028
	.4byte 0x00020000
	.4byte 0x00300002
	.4byte 0x0000ff80
	.4byte 0x00020002
	.4byte 0xff700038
	.4byte 0x00020000
	.4byte 0x00400002
	.4byte 0x0000ff60
	.4byte 0x00020002
	.4byte 0x000cfffe
	.4byte 0x000cfffc
	.4byte 0x000cfffa
	.4byte 0x0008fff8
	.4byte 0x0008fff6
	.4byte 0x0008fff4
	.4byte 0x0008fff2
	.4byte 0x0008fff0
	.4byte 0x0004ffed
	.4byte 0x0004ffeb
	.4byte 0x0004ffe8
	.4byte 0x0004ffe5
	.4byte 0x0004ffe2
	.4byte 0x0004ffdf
	.4byte 0x0004ffdc
	.4byte 0x0004ffd8
	.4byte 0x0004ffd4
	.4byte 0x0000ffd0
	.4byte 0x0000ffcc
	.4byte 0x0000ffc8
	.4byte 0x0000ffc4
	.4byte 0x0000ffc0
	.4byte 0x0000ffbc
	.4byte 0x0000ffb8
	.4byte 0x0000ffb4
	.4byte 0x0000ffb0
	.4byte 0x0000ffab
	.4byte 0x0000ffa6
	.4byte 0x0000ffa1
	.4byte 0x0000ff9c
	.4byte 0x0000ff92
	.4byte 0x0000ff88
	.4byte 0x0200aa78
	.4byte 0x0200aab4
	.4byte 0x0200aaf0
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
	.4byte 0x01a800b0
	.4byte 0x00c00220
	.4byte 0x023001b8
	.4byte 0x0002ffff
	.4byte 0x01a80110
	.4byte 0x01200220
	.4byte 0x023001b8
	.4byte 0x0003ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffc80030
	.4byte 0x004003c0
	.4byte 0x03d0ffd8
	.4byte 0x0002ffff
	.4byte 0xffc80090
	.4byte 0x00a003c0
	.4byte 0x03d0ffd8
	.4byte 0x0003ffff
	.4byte 0xff9400ec
	.4byte 0x010403bc
	.4byte 0x03d4ffac
	.4byte 0x0007ffff
	.4byte 0xff94010c
	.4byte 0x012403bc
	.4byte 0x03d4ffac
	.4byte 0x0007ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000009e
	.4byte 0x00125002
	.4byte 0x0020209f
	.4byte 0x0030309f
	.4byte 0x004010a3
	.4byte 0x0060609f
	.4byte 0x04d0109a
	.4byte 0x0000009f
	.4byte 0x0010d0a5
	.4byte 0x0020209e
	.4byte 0x0030309e
	.4byte 0x0040509f
	.4byte 0x0050409f
	.4byte 0x0070709e
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02270000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02270000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02670000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02670000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02870000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02870000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x02a70000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x03370000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x039f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x036f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x035f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x034f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x035f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x036f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x034f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x03bf0000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00024000
	.4byte 0xffff017f
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00024000
	.4byte 0xffff0191
	.4byte 0x0200ab2c
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03c00000
	.4byte 0x00024000
	.4byte 0xffff0191
	.4byte 0x0200ab2c
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00024000
	.4byte 0xffff0191
	.4byte 0x0200abb4
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x03900000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff017f
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00024000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x03a80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x03a80000
	.4byte 0x00024000
	.4byte 0xffff01e9
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
	.4byte 0x0000c402
	.4byte 0xffff0004
	.4byte 0x0200a0bd
	.4byte 0x00000002
	.4byte 0x0320001e
	.4byte 0x02008f8d
	.4byte 0x00000002
	.4byte 0x0321001f
	.4byte 0x02008f8d
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte 0x02008f8d
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte 0x02008f8d
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte 0x02009e01
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte 0x02008d81
	.4byte 0x00000006
	.4byte 0xffff002c
	.4byte 0x0200906d
	.4byte 0x00002115
	.4byte 0x0a6b000c
	.4byte 0x02009011
	.4byte 0x00002115
	.4byte 0x0a68001a
	.4byte 0x02008b69
	.4byte 0x00008c15
	.4byte 0xffff0018
	.4byte 0x02008cd9
	.4byte 0x00008c15
	.4byte 0x09500019
	.4byte 0x02008cd9
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008cd9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02009f91
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
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte 0x02009e01
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte 0x02008d81
	.4byte 0x00001815
	.4byte 0x02210009
	.4byte 0x020090f5
	.4byte 0x00001815
	.4byte 0x0222000a
	.4byte 0x0200912d
	.4byte 0x00001815
	.4byte 0x0223000b
	.4byte 0x02009165
	.4byte 0x00008c15
	.4byte 0x0951000c
	.4byte 0x0200919d
	.4byte 0x00008c15
	.4byte 0x0952000d
	.4byte 0x020091e1
	.4byte 0x00000009
	.4byte 0x09520000
	.4byte 0x020091e1
	.4byte 0x00002115
	.4byte 0x0a690008
	.4byte 0x02009225
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01000057
	.4byte 0x04061011
	.4byte 0x0e040501
	.4byte 0x0f01000f
	.4byte 0x0406205d
	.4byte 0x0e040502
	.4byte 0x3001030f
	.4byte 0x03750406
	.4byte 0x0f0e0405
	.4byte 0x400f0100
	.4byte 0x05040406
	.4byte 0x0f0ef504
	.4byte 0x031e0100
	.4byte 0x0397049a
	.4byte 0x06300702
	.4byte 0x0109f54b
	.4byte 0x1d03f30a
	.4byte 0x0a400a06
	.4byte 0x100240ad
	.4byte 0x0802445d
	.4byte 0x021f0843
	.4byte 0x2049044e
	.4byte 0x03b92205
	.4byte 0xd700341d
	.4byte 0x106b1612
	.4byte 0x02021261
	.4byte 0x30555e02
	.4byte 0x02245a02
	.4byte 0x561644bf
	.4byte 0x531f0833
	.4byte 0x0f46160f
	.4byte 0x00237919
	.4byte 0x0016f610
	.4byte 0x22621527
	.4byte 0x63010302
	.4byte 0x04302010
	.4byte 0x03ea11f8
	.4byte 0x0b1302ef
	.4byte 0x7b031205
	.4byte 0x30901411
	.4byte 0x12d303b5
	.4byte 0x01001806
	.4byte 0x8f261134
	.4byte 0xdbc31820
	.4byte 0x3c14bb04
	.4byte 0x161f0401
	.4byte 0x712701b5
	.4byte 0x01701d02
	.4byte 0x07012c05
	.2byte 0x0000
