.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200a615, 0x02008479, 0x02008485, 0x0200848d, 0x02008599, 0x02008481, 0x0200a799
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
	bl 0x0200c47c
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02000100
	cmp	r7, #0
	beq.n	.L_02000100
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02000108
.L_02000100:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02000108:
	mov	r3, sl
	bl 0x0200c3dc
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02000116
	b.n	.L_02000262
.L_02000116:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200c3c4
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200c3d4
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200c434
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
	bl 0x02008038
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
	beq.n	.L_02000262
	cmp	r7, #0
	beq.n	.L_02000262
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000198
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200c514
.L_02000198:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
.L_020001a0:
	cmp	r3, #0
	beq.n	.L_020001b8
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x02008038
.L_020001b8:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020001cc
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_020001cc:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000212
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020001fa
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200c33c
.L_020001ee:
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_0200020c
.L_020001fa:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200c33c
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_0200020c:
	bl 0x0200c33c
	str	r0, [r6, #52]
.L_02000212:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200022e
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200c3c4
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200c3d4
.L_0200022e:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000240
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02000240:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000252
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02000252:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000262
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02000262:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c7a0
	.4byte 0x02008081
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #20
	movs	r1, #0
	movs	r2, #16
	bl 0x0200c5f4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #24
	movs	r1, #1
	movs	r2, #19
	bl 0x0200c5f4
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	ldmia	r0!, {r5}
	ldmia	r1!, {r3}
	ldmia	r0!, {r4}
	subs	r5, r5, r3
	ldmia	r1!, {r3}
	asrs	r5, r5, #16
	ldr	r2, [r1, #0]
	subs	r4, r4, r3
	ldr	r3, [r0, #0]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	asrs	r3, r3, #16
	adds	r0, r5, #0
	muls	r0, r5
	adds	r2, r4, #0
	muls	r2, r4
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #4]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd20
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	mov	r9, r3
	adds	r3, r5, #0
	adds	r3, #99
	ldrb	r3, [r3, #0]
	mov	sl, r2
	movs	r2, #0
	mov	r8, r1
	mov	fp, r2
	cmp	r3, #0
	beq.n	.L_02000308
	adds	r2, r5, #0
	adds	r2, #91
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #1
	b.n	.L_020003ac
.L_02000308:
	mov	r6, r8
	adds	r7, r5, #0
	adds	r6, #8
	adds	r7, #8
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl 0x020082a0
	cmp	r0, sl
	blt.n	.L_02000322
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_0200039a
.L_02000322:
	mov	r2, r8
	ldr	r0, [r2, #16]
	ldr	r3, [r5, #16]
	ldr	r1, [r6, #0]
	subs	r0, r0, r3
	ldr	r3, [r7, #0]
	subs	r1, r1, r3
	bl 0x0200c364
	ldr	r3, [pc, #128]
	lsls	r0, r0, #16
	movs	r2, #128
	lsrs	r0, r0, #16
	lsls	r2, r2, #5
	adds	r1, r0, r2
	ldrh	r2, [r5, #6]
	adds	r4, r0, r3
	movs	r3, #240
	lsls	r3, r3, #8
	ands	r4, r3
	ands	r1, r3
	ands	r0, r3
	ands	r3, r2
	cmp	r0, r3
	beq.n	.L_02000362
	cmp	r1, r3
	beq.n	.L_02000362
	cmp	r4, r3
	beq.n	.L_02000362
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_020003aa
.L_02000362:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r2, r5, #0
	adds	r2, #91
	cmp	r3, #120
	ble.n	.L_02000388
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200c3c4
	b.n	.L_020003aa
.L_02000388:
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200c3c4
	movs	r3, #1
	mov	fp, r3
	b.n	.L_020003aa
.L_0200039a:
	adds	r3, r5, #0
	adds	r3, #91
	mov	r2, fp
	strb	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200c3c4
.L_020003aa:
	mov	r0, fp
.L_020003ac:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	ldr	r3, [r3, #108]
	mov	sl, r2
	mov	r8, r3
	ldr	r3, [pc, #160]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200c47c
	adds	r1, r5, #0
	adds	r0, #8
	adds	r1, #8
	movs	r7, #0
	bl 0x020082a0
	cmp	r0, #11
	bgt.n	.L_02000400
	adds	r3, r5, #0
	adds	r3, #91
	adds	r0, r5, #0
	strb	r7, [r3, #0]
	movs	r1, #2
	bl 0x0200c3c4
	b.n	.L_0200046a
.L_02000400:
	adds	r6, r5, #0
	adds	r6, #100
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	bne.n	.L_02000410
	movs	r0, #18
	b.n	.L_0200041a
.L_02000410:
	cmp	r3, #1
	bne.n	.L_02000418
	movs	r0, #16
	b.n	.L_0200041a
.L_02000418:
	movs	r0, #17
.L_0200041a:
	bl 0x0200c47c
	adds	r1, r0, #0
	adds	r0, r5, #0
	movs	r2, #24
	movs	r3, #0
	bl 0x020082d8
	cmp	r0, #0
	bne.n	.L_0200046a
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c47c
	movs	r3, #176
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r1, r0, #0
	cmp	r3, #0
	bne.n	.L_02000454
	mov	r2, sl
	ldrb	r3, [r2, #4]
	cmp	r3, #0
	beq.n	.L_02000460
.L_02000454:
	ldrh	r2, [r6, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000460
	movs	r7, #1
.L_02000460:
	adds	r0, r5, #0
	movs	r2, #32
	adds	r3, r7, #0
	bl 0x020082d8
.L_0200046a:
	movs	r0, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xca44
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xca74
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #180]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #172]
	cmp	r2, r3
	bne.n	.L_020004c0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #197
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_020004c0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #204
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_020004c0
	ldr	r0, [pc, #140]
	b.n	.L_02000542
.L_020004c0:
	ldr	r3, [pc, #128]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #128]
	cmp	r2, r3
	bne.n	.L_020004d6
	ldr	r0, [pc, #128]
	b.n	.L_02000542
.L_020004d6:
	ldr	r3, [pc, #128]
	cmp	r2, r3
	bne.n	.L_020004f2
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_020004ee
	ldr	r0, [pc, #112]
	b.n	.L_02000542
.L_020004ee:
	ldr	r0, [pc, #112]
	b.n	.L_02000542
.L_020004f2:
	ldr	r3, [pc, #112]
	cmp	r2, r3
	bne.n	.L_0200051a
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_02000516
	ldr	r0, [pc, #96]
	movs	r3, #3
	adds	r2, r0, #0
	adds	r2, #46
	strb	r3, [r2, #0]
	adds	r2, #72
	strb	r3, [r2, #0]
	b.n	.L_02000542
.L_02000516:
	ldr	r0, [pc, #84]
	b.n	.L_02000542
.L_0200051a:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02000536
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_02000532
	ldr	r0, [pc, #68]
	b.n	.L_02000542
.L_02000532:
	ldr	r0, [pc, #68]
	b.n	.L_02000542
.L_02000536:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02000540
	ldr	r0, [pc, #64]
	b.n	.L_02000542
.L_02000540:
	ldr	r0, [pc, #64]
.L_02000542:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000010d
	.4byte 0x0200cb2c
	.4byte 0x0000010e
	.4byte 0x0200cc4c
	.4byte 0x0000010f
	.4byte 0x0200cf04
	.4byte 0x0200cd9c
	.4byte 0x00000110
	.4byte 0x0200d21c
	.4byte 0x0200d06c
	.4byte 0x00000111
	.4byte 0x0200d4a4
	.4byte 0x0200d444
	.4byte 0x00000112
	.4byte 0x0200d51c
	.2byte 0xcb14
	.2byte 0x0200
	push	{lr}
	movs	r2, #192
	movs	r1, #64
	lsls	r2, r2, #2
	bl 0x0200c5cc
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #76]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_020005b0
	ldr	r0, [pc, #64]
	b.n	.L_020005e4
.L_020005b0:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_020005ba
.L_020005b6:
	ldr	r0, [pc, #64]
	b.n	.L_020005e4
.L_020005ba:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_020005c4
	ldr	r0, [pc, #60]
	b.n	.L_020005e4
.L_020005c4:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_020005ce
	ldr	r0, [pc, #60]
	b.n	.L_020005e4
.L_020005ce:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_020005d8
	ldr	r0, [pc, #56]
	b.n	.L_020005e4
.L_020005d8:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_020005e2
	ldr	r0, [pc, #56]
	b.n	.L_020005e4
.L_020005e2:
	ldr	r0, [pc, #56]
.L_020005e4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000010d
	.4byte 0x0200d558
	.4byte 0x0000010e
	.4byte 0x0200d588
	.4byte 0x0000010f
	.4byte 0x0200d7f8
	.4byte 0x00000110
	.4byte 0x0200da20
	.4byte 0x00000111
	.4byte 0x0200dda4
	.4byte 0x00000112
	.4byte 0x0200deac
	.2byte 0xd54c
	.2byte 0x0200
	push	{lr}
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	ldr	r0, [pc, #16]
	bl 0x0200c51c
	movs	r1, #0
	movs	r0, #15
	bl 0x0200c53c
	bl 0x0200c46c
	pop	{pc}
	.2byte 0x2cfd
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c47c
	movs	r2, #190
	ldrh	r3, [r0, #6]
	lsls	r2, r2, #7
	adds	r2, #255
	adds	r3, r3, r2
	ldr	r2, [pc, #16]
	lsls	r3, r3, #16
	movs	r0, #1
	cmp	r3, r2
	bls.n	.L_0200066a
	movs	r0, #0
.L_0200066a:
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0x3ffe
	push	{lr}
	bl 0x02008644
	cmp	r0, #0
	beq.n	.L_02000688
	movs	r0, #14
	movs	r1, #22
	bl 0x0200c634
	b.n	.L_020006ba
.L_02000688:
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_020006a8
	ldr	r0, [pc, #24]
	bl 0x0200c51c
	b.n	.L_020006ae
.L_020006a8:
	ldr	r0, [pc, #20]
	bl 0x0200c51c
.L_020006ae:
	movs	r0, #22
	movs	r1, #0
	bl 0x0200c534
	bl 0x0200c46c
.L_020006ba:
	pop	{pc}
	.4byte 0x00002ed6
	.2byte 0x2d1d
	.2byte 0x0000
	push	{lr}
	bl 0x02008644
	cmp	r0, #0
	beq.n	.L_020006d8
	movs	r0, #31
	movs	r1, #10
	bl 0x0200c624
	b.n	.L_0200070a
.L_020006d8:
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_020006f8
	ldr	r0, [pc, #24]
	bl 0x0200c51c
	b.n	.L_020006fe
.L_020006f8:
	ldr	r0, [pc, #20]
	bl 0x0200c51c
.L_020006fe:
	movs	r0, #10
.L_02000700:
	movs	r1, #0
	bl 0x0200c534
	bl 0x0200c46c
.L_0200070a:
	pop	{pc}
	.4byte 0x00002ed3
	.2byte 0x2d1a
	.2byte 0x0000
	push	{lr}
	bl 0x02008644
	cmp	r0, #0
	beq.n	.L_02000726
	movs	r0, #8
	bl 0x0200c62c
	b.n	.L_02000758
.L_02000726:
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_02000746
	ldr	r0, [pc, #28]
.L_02000740:
	bl 0x0200c51c
	b.n	.L_0200074c
.L_02000746:
	ldr	r0, [pc, #24]
	bl 0x0200c51c
.L_0200074c:
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c534
	bl 0x0200c46c
.L_02000758:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00002eda
	.2byte 0x2d21
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #16
	ldr	r7, [r3, #108]
	bl 0x0200c47c
	adds	r5, r0, #0
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	adds	r6, r5, #0
	mov	r8, r2
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	movs	r2, #179
	movs	r3, #1
	lsls	r2, r2, #1
	adds	r6, #99
	strb	r3, [r6, #0]
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020007b8
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #16
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x0200c50c
	ldr	r0, [pc, #56]
	bl 0x0200c51c
	b.n	.L_020007be
.L_020007b8:
	ldr	r0, [pc, #52]
	bl 0x0200c51c
.L_020007be:
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020007d8
	mov	r3, r8
	strh	r3, [r5, #6]
.L_020007d8:
	movs	r3, #0
	strb	r3, [r6, #0]
	bl 0x0200c46c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002ec0
	.2byte 0x2ec4
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #17
	ldr	r7, [r3, #108]
	bl 0x0200c47c
	adds	r5, r0, #0
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	adds	r6, r5, #0
	mov	r8, r2
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	movs	r2, #179
	movs	r3, #1
	lsls	r2, r2, #1
	adds	r6, #99
	strb	r3, [r6, #0]
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02000848
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #17
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x0200c50c
	ldr	r0, [pc, #56]
	bl 0x0200c51c
	b.n	.L_0200084e
.L_02000848:
	ldr	r0, [pc, #52]
	bl 0x0200c51c
.L_0200084e:
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02000868
	mov	r3, r8
	strh	r3, [r5, #6]
.L_02000868:
	movs	r3, #0
	strb	r3, [r6, #0]
	bl 0x0200c46c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002ec1
	.2byte 0x2ec5
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #18
	ldr	r7, [r3, #108]
	bl 0x0200c47c
	adds	r5, r0, #0
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	adds	r6, r5, #0
	mov	r8, r2
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	movs	r2, #179
	movs	r3, #1
	lsls	r2, r2, #1
	adds	r6, #99
	strb	r3, [r6, #0]
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020008d8
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #18
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x0200c50c
	ldr	r0, [pc, #56]
	bl 0x0200c51c
	b.n	.L_020008de
.L_020008d8:
	ldr	r0, [pc, #52]
	bl 0x0200c51c
.L_020008de:
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020008f8
	mov	r3, r8
	strh	r3, [r5, #6]
.L_020008f8:
	movs	r3, #0
	strb	r3, [r6, #0]
	bl 0x0200c46c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002ec2
	.2byte 0x2ec6
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	sub	sp, #8
	bl 0x0200c3ac
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200092a
	b.n	.L_02000ac8
.L_0200092a:
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	movs	r0, #158
	bl 0x0200c63c
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #77
	movs	r3, #16
	movs	r0, #89
	bl 0x0200c404
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200c3b4
	movs	r0, #10
	bl 0x0200c45c
	movs	r3, #192
	movs	r1, #130
	movs	r2, #180
	lsls	r3, r3, #6
	lsls	r2, r2, #17
	lsls	r1, r1, #18
	movs	r0, #8
	bl 0x0200c4d4
	bl 0x0200c584
	movs	r1, #204
	adds	r0, #85
	lsls	r1, r1, #6
	strb	r5, [r0, #0]
	adds	r1, #51
	ldr	r0, [pc, #332]
	bl 0x0200c56c
	movs	r0, #138
	movs	r1, #1
	movs	r2, #194
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	bl 0x0200c574
	movs	r0, #10
	bl 0x0200c45c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #300]
	adds	r2, #153
	bl 0x0200c484
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r2, #192
	movs	r0, #8
	adds	r1, #14
	lsls	r2, r2, #1
	bl 0x0200c4b4
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #135
	movs	r2, #194
	lsls	r2, r2, #1
	movs	r0, #8
	lsls	r1, r1, #2
	bl 0x0200c4b4
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c54c
	movs	r1, #4
	movs	r0, #8
	bl 0x0200c4e4
	ldr	r0, [pc, #240]
	bl 0x0200c51c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #9
	bl 0x0200c55c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #8
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #9
	bl 0x0200c55c
	movs	r1, #176
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #176
	movs	r2, #40
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c54c
	movs	r0, #8
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #8
	bl 0x0200c55c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #176
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r1, #176
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #156
	lsls	r0, r0, #4
	bl 0x0200c3b4
	bl 0x0200c46c
.L_02000ac8:
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x00019999
	.4byte 0x00013333
	.2byte 0x2d0d
	.2byte 0x0000
	push	{lr}
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #696]
	adds	r1, #153
	bl 0x0200c56c
	movs	r0, #160
	movs	r1, #1
	movs	r2, #160
	movs	r3, #1
	lsls	r2, r2, #16
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200c574
	bl 0x0200c57c
	movs	r0, #40
	bl 0x0200c45c
	movs	r0, #10
	movs	r1, #1
	bl 0x0200c504
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #10
	bl 0x0200c54c
	ldr	r0, [pc, #648]
	bl 0x0200c51c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #129
	movs	r0, #12
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r1, #129
	movs	r0, #13
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r1, #129
	movs	r0, #14
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r1, #129
	movs	r0, #15
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r1, #129
	movs	r0, #16
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200c564
	movs	r0, #20
	bl 0x0200c45c
	movs	r1, #160
	movs	r0, #10
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #129
	movs	r0, #12
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r1, #129
	movs	r0, #13
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r1, #129
	movs	r0, #14
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r1, #129
	movs	r0, #15
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r1, #129
	movs	r0, #16
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200c564
	movs	r0, #20
	bl 0x0200c45c
	movs	r0, #12
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #12
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	movs	r0, #10
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200c55c
	movs	r0, #13
	movs	r1, #4
	bl 0x0200c4ec
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #10
	movs	r0, #15
	movs	r1, #4
	bl 0x0200c4fc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #160
	movs	r0, #10
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #6
	movs	r2, #40
	adds	r1, #255
	movs	r0, #10
	bl 0x0200c55c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #8
	movs	r2, #20
	adds	r1, #255
	movs	r0, #16
	bl 0x0200c55c
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #0
	movs	r0, #14
	movs	r1, #2
	bl 0x0200c4fc
	movs	r0, #14
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r1, #132
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200c55c
	movs	r1, #160
	movs	r0, #10
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c54c
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200c55c
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #11
	movs	r1, #4
.L_02000cbc:
	bl 0x0200c4e4
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #129
	movs	r0, #10
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r1, #128
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r0, #17
.L_02000d0c:
	movs	r1, #0
	movs	r2, #40
	bl 0x0200c544
	movs	r1, #176
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #176
	movs	r0, #13
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #208
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #208
	movs	r0, #15
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #208
	movs	r0, #16
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #208
	movs	r2, #20
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200c544
.L_02000d5c:
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #11
	bl 0x0200c55c
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #176
	movs	r2, #80
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r0, #10
	movs	r1, #4
	bl 0x0200c4ec
	movs	r1, #0
	movs	r0, #10
	bl 0x0200c534
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #193
	bl 0x0200c3b4
	bl 0x0200c46c
	pop	{pc}
	.4byte 0x0004cccc
	.2byte 0x2d23
	.2byte 0x0000
	push	{r5, r6, lr}
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200c56c
	movs	r0, #160
	movs	r1, #1
	movs	r2, #160
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200c574
	ldr	r5, [pc, #840]
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
	bl 0x0200c484
	ldr	r0, [r5, #0]
	movs	r2, #156
	movs	r1, #134
	bl 0x0200c4b4
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200c55c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #11
	bl 0x0200c54c
	ldr	r0, [pc, #776]
	bl 0x0200c51c
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #10
	bl 0x0200c55c
	movs	r1, #128
	movs	r2, #40
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	ldr	r0, [r5, #0]
	movs	r1, #144
	movs	r2, #154
	bl 0x0200c4b4
	movs	r1, #192
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200c544
	ldr	r1, [r5, #0]
	movs	r0, #18
	bl 0x0200c4dc
	ldr	r1, [r5, #0]
	movs	r0, #0
	bl 0x0200c4dc
	movs	r0, #1
	bl 0x0200c344
	movs	r1, #176
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #18
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #0
	adds	r1, #204
	bl 0x0200c484
	ldr	r1, [pc, #640]
	movs	r0, #0
	bl 0x0200c48c
	ldr	r1, [pc, #636]
	movs	r0, #18
	bl 0x0200c4a4
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200c55c
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #18
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #18
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #160
	lsls	r0, r0, #8
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r2, #40
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #6
	bl 0x0200c54c
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #160
	movs	r0, #10
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r2, #0
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200c54c
	ldr	r1, [pc, #428]
	ldr	r2, [pc, #432]
	movs	r0, #0
	bl 0x0200c484
	movs	r0, #0
	bl 0x0200c47c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #160
	movs	r2, #140
	movs	r0, #0
	bl 0x0200c4b4
	movs	r0, #1
	bl 0x0200c45c
	movs	r0, #0
	bl 0x0200c47c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #21
	movs	r0, #0
	bl 0x0200c4ec
	movs	r1, #176
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #176
	movs	r0, #13
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #176
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #208
	movs	r0, #15
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #208
	movs	r0, #16
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #208
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #224
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r1, #176
	movs	r0, #18
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #18
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #0
	movs	r0, #0
	bl 0x0200c4e4
	movs	r0, #10
	bl 0x0200c45c
	movs	r1, #1
	movs	r0, #0
	bl 0x0200c4e4
	movs	r0, #10
	bl 0x0200c45c
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #6
	bl 0x0200c54c
	movs	r0, #18
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #18
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #0
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r0, #0
	movs	r1, #160
	movs	r2, #154
	bl 0x0200c4b4
	movs	r1, #160
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r0, #18
	movs	r1, #4
	bl 0x0200c4ec
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #18
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #18
	bl 0x0200c55c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #18
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #11
	bl 0x0200c55c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #60
	movs	r0, #10
	bl 0x0200c55c
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #0
	bl 0x0200c55c
	movs	r0, #160
	lsls	r0, r0, #8
	movs	r1, #0
	bl 0x0200c534
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r2, #40
	movs	r0, #18
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r0, #0
	movs	r1, #4
	bl 0x0200c4ec
	movs	r0, #160
	lsls	r0, r0, #8
	movs	r1, #0
	bl 0x0200c534
	movs	r5, #0
	b.n	.L_0200113e
	.4byte 0x02000240
	.4byte 0x00002d3f
	.4byte 0x0200c9c4
	.4byte 0x0200c980
	.4byte 0x00026666
	.2byte 0x3333
	.2byte 0x0001
.L_0200113c:
	adds	r5, #1
.L_0200113e:
	cmp	r5, #79
	bhi.n	.L_02001150
	movs	r0, #1
	bl 0x0200c344
	ldr	r3, [pc, #536]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0200113c
.L_02001150:
	movs	r0, #12
	movs	r1, #1
	bl 0x0200c504
	movs	r0, #12
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #0
	bl 0x0200c55c
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #6
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #15
	bl 0x0200c55c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #128
	movs	r2, #0
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c54c
	movs	r0, #13
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #6
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #160
	movs	r2, #0
	movs	r0, #10
	lsls	r1, r1, #7
	bl 0x0200c544
	movs	r0, #16
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #0
	bl 0x0200c55c
	movs	r0, #14
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #17
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200c544
	ldr	r5, [pc, #272]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #176
	movs	r0, #18
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200c544
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #11
	bl 0x0200c55c
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	movs	r2, #40
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r0, #10
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #208
	movs	r0, #18
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #18
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #18
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #160
	lsls	r0, r0, #8
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #10
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #18
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #11
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #160
	movs	r1, #0
	lsls	r0, r0, #8
	bl 0x0200c524
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c474
	ldr	r5, [r5, #0]
	cmp	r0, #0
	bne.n	.L_0200136c
	adds	r0, r5, #0
	bl 0x0200c4f4
	movs	r0, #160
	lsls	r0, r0, #8
	movs	r1, #0
	bl 0x0200c534
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200138e
	.4byte 0x03001150
	.2byte 0x0240
	.2byte 0x0200
.L_0200136c:
	adds	r0, r5, #0
	bl 0x0200c4c4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #160
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #8
	movs	r1, #0
	bl 0x0200c534
.L_0200138e:
	movs	r1, #132
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #18
	bl 0x0200c55c
	movs	r1, #128
	movs	r0, #18
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #18
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #0
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #192
	movs	r2, #0
	movs	r0, #11
	lsls	r1, r1, #6
	bl 0x0200c544
	movs	r1, #160
	movs	r0, #10
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200c55c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	ldr	r6, [pc, #440]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #160
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r1, #160
	movs	r0, #18
	lsls	r1, r1, #8
	bl 0x0200c54c
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #18
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #128
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c54c
	movs	r0, #12
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #13
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #14
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #16
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #17
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #128
	movs	r0, #18
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #18
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #0
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200c544
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c54c
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4ec
	ldr	r5, [pc, #208]
	movs	r0, #0
	adds	r1, r5, #0
	bl 0x0200c48c
	adds	r1, r5, #0
	movs	r0, #18
	bl 0x0200c4a4
	movs	r2, #154
	ldr	r0, [r6, #0]
	movs	r1, #160
	bl 0x0200c4b4
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #10
	movs	r1, #1
	bl 0x0200c504
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #2
	ldr	r0, [r6, #0]
	adds	r1, #255
	movs	r2, #0
	bl 0x0200c55c
	movs	r1, #224
	movs	r2, #20
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r0, #10
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200c55c
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c54c
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #10
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #160
	movs	r0, #10
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #6
	bl 0x0200c54c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #0
	movs	r0, #11
	bl 0x0200c534
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #194
	bl 0x0200c3b4
	bl 0x0200c46c
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0xca08
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #8
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	ldr	r6, [pc, #388]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r6, r2
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c484
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #10
	movs	r0, #20
	bl 0x0200c484
	movs	r0, #20
	bl 0x0200c47c
	movs	r3, #1
	mov	r8, r3
	adds	r0, #34
	movs	r3, #1
	strb	r3, [r0, #0]
	movs	r1, #3
	movs	r0, #20
	bl 0x0200c554
	ldr	r0, [r6, #0]
	bl 0x0200c47c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #204
	movs	r2, #163
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	ldr	r0, [r6, #0]
	bl 0x0200c4b4
	movs	r0, #1
	bl 0x0200c45c
	ldr	r0, [r6, #0]
	bl 0x0200c47c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	ldr	r0, [r6, #0]
	bl 0x0200c544
	movs	r0, #161
	bl 0x0200c63c
	bl 0x0200c584
	movs	r2, #128
	ldr	r3, [r0, #12]
	lsls	r2, r2, #10
	mov	sl, r2
	add	r3, sl
	str	r3, [r0, #12]
	movs	r0, #20
	bl 0x0200c47c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #204
	ands	r5, r3
	lsls	r1, r1, #1
	movs	r2, #200
	strb	r5, [r0, #0]
	movs	r0, #20
	bl 0x0200c4b4
	movs	r0, #1
	bl 0x0200c45c
	movs	r0, #20
	bl 0x0200c47c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r2, #0
	movs	r0, #20
	bl 0x0200c4cc
	movs	r0, #176
	bl 0x0200c63c
	movs	r0, #128
	movs	r2, #128
	mov	r1, sl
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200c444
	movs	r0, #30
	bl 0x0200c45c
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200c444
	movs	r0, #127
	bl 0x0200c63c
	movs	r0, #30
	bl 0x0200c45c
	movs	r0, #146
	bl 0x0200c63c
	movs	r0, #20
	bl 0x0200c45c
	movs	r1, #6
	movs	r2, #40
	ldr	r0, [r6, #0]
	adds	r1, #255
	bl 0x0200c55c
	movs	r1, #2
	movs	r0, #20
	bl 0x0200c554
	movs	r0, #20
	bl 0x0200c47c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r2, r3
	strb	r2, [r0, #0]
	movs	r5, #3
	mov	r8, r2
	movs	r0, #72
	movs	r1, #3
	movs	r2, #69
	movs	r3, #3
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200c404
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #48
	movs	r1, #36
	movs	r2, #69
	movs	r3, #8
	str	r5, [sp, #0]
	bl 0x0200c404
	movs	r3, #70
	movs	r2, #20
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #13
	movs	r2, #1
	movs	r3, #3
	movs	r0, #72
	bl 0x0200c42c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #45
	bl 0x0200c3b4
	bl 0x0200c46c
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_020017dc
	movs	r0, #64
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_020017dc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x0200c464
	movs	r0, #1
	bl 0x0200c5ec
	movs	r3, #128
	movs	r1, #204
	movs	r2, #145
	lsls	r3, r3, #7
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #20
	bl 0x0200c4d4
	movs	r0, #1
	bl 0x0200c344
	movs	r0, #20
	movs	r1, #4
	movs	r2, #20
	bl 0x0200c4fc
	ldr	r3, [pc, #32]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #20
	bl 0x0200c50c
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #200
	strh	r3, [r2, #0]
	bl 0x0200c46c
.L_020017dc:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r3, #25
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #25
	movs	r1, #2
	movs	r2, #1
	movs	r3, #1
	bl 0x0200c42c
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #25
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #25
	movs	r1, #3
	movs	r2, #1
	movs	r3, #1
	bl 0x0200c42c
	add	sp, #8
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	adds	r1, #100
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	ldr	r3, [r0, #8]
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r0, #8]
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r0, #12]
	movs	r2, #160
	ldr	r3, [r0, #24]
	lsls	r2, r2, #3
	adds	r2, #30
	adds	r3, r3, r2
	str	r3, [r0, #24]
	ldr	r3, [r0, #28]
	adds	r3, r3, r2
	str	r3, [r0, #28]
	ldrh	r3, [r1, #0]
	adds	r3, #2
	strh	r3, [r1, #0]
	ldr	r3, [r0, #104]
	subs	r3, #1
	str	r3, [r0, #104]
	cmp	r3, #0
	bne.n	.L_0200185e
	bl 0x0200c3e4
.L_0200185e:
	pop	{pc}
	push	{r5, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	movs	r0, #30
	adds	r3, r2, #0
	adds	r0, #255
	adds	r2, r5, #0
	adds	r1, r4, #0
	bl 0x0200c3dc
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020018d8
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r3, r5, #0
	movs	r1, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	adds	r3, #15
	strh	r1, [r3, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #7
	bl 0x0200c514
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200c434
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r3, #60
	str	r3, [r5, #104]
	ldr	r3, [pc, #20]
	adds	r0, r5, #0
	movs	r1, #5
	str	r3, [r5, #108]
	bl 0x0200c3c4
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200c43c
.L_020018d8:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x981d
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #176]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #168]
	cmp	r2, r3
	bne.n	.L_02001970
	ldr	r5, [pc, #164]
	movs	r6, #63
	ldr	r3, [r5, #0]
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_02001910
	movs	r0, #134
	movs	r1, #160
	movs	r2, #232
	lsls	r0, r0, #18
	lsls	r1, r1, #14
	lsls	r2, r2, #16
	bl 0x02009860
.L_02001910:
	ldr	r3, [r5, #0]
	adds	r3, #60
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_0200192a
	movs	r0, #168
	movs	r1, #160
	movs	r2, #148
	lsls	r0, r0, #16
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	bl 0x02009860
.L_0200192a:
	ldr	r3, [r5, #0]
	adds	r3, #20
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_02001954
	movs	r0, #140
	movs	r1, #168
	movs	r2, #184
	lsls	r0, r0, #17
	lsls	r1, r1, #14
	lsls	r2, r2, #16
	bl 0x02009860
	movs	r0, #152
	movs	r1, #192
	movs	r2, #212
	lsls	r0, r0, #16
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	bl 0x02009860
.L_02001954:
	ldr	r3, [r5, #0]
	adds	r3, #40
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_02001992
	movs	r0, #168
	movs	r1, #168
	movs	r2, #181
	lsls	r0, r0, #17
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	bl 0x02009860
	b.n	.L_02001992
.L_02001970:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02001992
	ldr	r3, [pc, #36]
	movs	r2, #63
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02001992
	movs	r0, #142
	movs	r1, #208
	movs	r2, #148
	lsls	r0, r0, #18
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x02009860
.L_02001992:
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x0000010e
	.4byte 0x0300122c
	.2byte 0x010f
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	sub	sp, #8
	adds	r5, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	ldr	r6, [sp, #36]
	mov	r9, r3
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_02001a02
	cmp	r6, #2
	bne.n	.L_020019e4
	movs	r0, #188
	bl 0x0200c63c
	b.n	.L_020019ea
.L_020019e4:
	movs	r0, #158
	bl 0x0200c63c
.L_020019ea:
	ldr	r3, [sp, #40]
	adds	r0, r5, #0
	str	r3, [sp, #4]
	adds	r1, r7, #0
	mov	r2, r8
	mov	r3, sl
	str	r6, [sp, #0]
	bl 0x0200c404
	movs	r0, #20
	bl 0x0200c45c
.L_02001a02:
	ldr	r3, [pc, #144]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200c484
	cmp	r6, #1
	bne.n	.L_02001a54
	ldr	r0, [r5, #0]
	bl 0x0200c47c
	adds	r5, r0, #0
	ldr	r0, [r5, #8]
	asrs	r2, r0, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_02001a30
	adds	r3, #15
.L_02001a30:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r2, r3
	movs	r1, #8
	subs	r1, r1, r3
	lsls	r1, r1, #16
	adds	r1, r1, r0
	ldr	r3, [r5, #16]
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	bl 0x0200c3f4
	adds	r0, r5, #0
	bl 0x0200c3fc
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r5, #6]
.L_02001a54:
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #56]
	bl 0x0200c48c
	movs	r0, #12
	bl 0x0200c344
	movs	r0, #123
	bl 0x0200c63c
	bl 0x0200c5b4
	bl 0x0200c5bc
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200c58c
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xc6f8
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #48
	movs	r1, #39
	movs	r2, #57
	movs	r3, #26
	bl 0x020099a4
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #49
	movs	r1, #39
	movs	r2, #65
	movs	r3, #24
	bl 0x020099a4
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #50
	movs	r1, #39
	movs	r2, #56
	movs	r3, #19
	bl 0x020099a4
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #51
	movs	r1, #39
	movs	r2, #61
	movs	r3, #13
	bl 0x020099a4
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #51
	movs	r1, #39
	movs	r2, #80
	movs	r3, #16
	bl 0x020099a4
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #2
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #52
	movs	r1, #39
	movs	r2, #50
	movs	r3, #9
	bl 0x020099a4
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #87
	movs	r1, #2
	movs	r2, #80
	movs	r3, #7
	bl 0x020099a4
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #89
	movs	r1, #0
	movs	r2, #77
	movs	r3, #16
	bl 0x020099a4
	add	sp, #8
	pop	{pc}
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r2, #0
	ldrsh	r7, [r3, r2]
	ldr	r3, [pc, #140]
	adds	r0, #192
	adds	r6, r3, r0
	ldr	r0, [r6, #0]
	bl 0x0200c47c
	movs	r1, #128
	movs	r2, #128
	adds	r5, r0, #0
	lsls	r2, r2, #7
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200c484
	ldr	r0, [r5, #8]
	asrs	r2, r0, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_02001bb6
	adds	r3, #15
.L_02001bb6:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r2, r3
	movs	r1, #8
	subs	r1, r1, r3
	lsls	r1, r1, #16
	adds	r1, r1, r0
	ldr	r3, [r5, #16]
	ldr	r0, [pc, #88]
	ldr	r2, [r5, #12]
	adds	r3, r3, r0
	adds	r0, r5, #0
	bl 0x0200c3f4
	adds	r0, r5, #0
	bl 0x0200c3fc
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r5, #6]
	movs	r0, #128
	bl 0x0200c63c
	ldr	r0, [r6, #0]
	bl 0x0200c47c
	movs	r1, #0
	bl 0x0200c434
	ldr	r0, [r6, #0]
	cmp	r7, #6
	bne.n	.L_02001bfe
	movs	r1, #25
	bl 0x0200c4e4
	b.n	.L_02001c04
.L_02001bfe:
	movs	r1, #26
	bl 0x0200c4e4
.L_02001c04:
	movs	r0, #4
	bl 0x0200c344
	bl 0x0200c5b4
	bl 0x0200c5bc
	adds	r0, r7, #0
	bl 0x0200c58c
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfffc
	.2byte 0xb5e0
	ldr	r3, [pc, #188]
	adds	r5, r0, #0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r7, r3, r0
	ldr	r0, [r7, #0]
	bl 0x0200c47c
	adds	r6, r0, #0
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	cmp	r5, #0
	beq.n	.L_02001c7a
	ldr	r0, [r7, #0]
	bl 0x0200c47c
	movs	r1, #0
	bl 0x0200c434
	ldr	r1, [r6, #8]
	ldr	r2, [pc, #144]
	ldr	r3, [r6, #16]
	ldr	r0, [pc, #144]
	adds	r1, r1, r2
	adds	r3, r3, r0
	ldr	r2, [r6, #12]
	adds	r0, r6, #0
	bl 0x0200c3ec
	movs	r0, #1
	bl 0x0200c344
	bl 0x0200c5ac
	ldr	r0, [r7, #0]
	movs	r1, #23
	bl 0x0200c4ec
	b.n	.L_02001ca2
.L_02001c7a:
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	ldr	r0, [pc, #108]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r1, r1, r2
	adds	r3, r3, r0
	ldr	r2, [r6, #12]
	adds	r0, r6, #0
	bl 0x0200c3ec
	movs	r0, #1
	bl 0x0200c344
	bl 0x0200c5ac
	ldr	r0, [r7, #0]
	movs	r1, #24
	bl 0x0200c4ec
.L_02001ca2:
	ldr	r5, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200c47c
	movs	r1, #1
	bl 0x0200c434
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200c484
	movs	r2, #8
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c4bc
	ldr	r0, [r5, #0]
	bl 0x0200c4c4
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200c4e4
	bl 0x0200c46c
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xffff0000
	.2byte 0x0000
	.2byte 0xfffa
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r1, r1
	negs	r2, r2
	negs	r0, r0
	bl 0x0200c574
	movs	r0, #20
	bl 0x0200c49c
	movs	r0, #1
	bl 0x0200c344
	ldr	r5, [pc, #844]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #184
	movs	r2, #242
	ldr	r0, [r5, #0]
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200c4cc
	movs	r3, #192
	movs	r1, #168
	movs	r2, #242
	lsls	r3, r3, #8
	movs	r0, #0
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	movs	r6, #176
	mov	sl, r3
	lsls	r6, r6, #8
	bl 0x0200c4d4
	movs	r1, #200
	movs	r2, #242
	movs	r0, #27
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	adds	r3, r6, #0
	bl 0x0200c4d4
	movs	r2, #192
	lsls	r2, r2, #6
	mov	r8, r2
	movs	r1, #138
	movs	r2, #206
	movs	r0, #17
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	mov	r3, r8
	bl 0x0200c4d4
	movs	r1, #220
	movs	r2, #150
	adds	r3, r6, #0
	movs	r0, #20
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200c4d4
	movs	r1, #184
	movs	r2, #171
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	movs	r0, #5
	movs	r7, #192
	bl 0x0200c4cc
	lsls	r7, r7, #18
	movs	r0, #1
	bl 0x0200c344
	ldr	r3, [r7, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	mov	r9, r2
	bl 0x0200c5ac
	bl 0x0200c5bc
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200c56c
	movs	r0, #184
	movs	r1, #1
	movs	r2, #224
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200c574
	bl 0x0200c57c
	movs	r0, #20
	bl 0x0200c45c
	ldr	r0, [pc, #656]
	bl 0x0200c51c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #27
	movs	r1, #3
	bl 0x0200c4e4
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #160
	movs	r0, #26
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #26
	bl 0x0200c55c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r0, #0
	movs	r1, #0
	movs	r2, #20
	bl 0x0200c544
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200c55c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #0
	bl 0x0200c55c
	movs	r2, #0
	movs	r0, #0
	mov	r1, sl
	bl 0x0200c544
	ldr	r0, [r5, #0]
	mov	r1, sl
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200c564
	movs	r0, #20
	bl 0x0200c45c
	movs	r0, #25
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #27
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #19
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #19
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #0
	bl 0x0200c55c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r0, #27
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200c544
	movs	r1, #132
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #27
	bl 0x0200c55c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #27
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #27
	bl 0x0200c55c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #26
	mov	r1, r8
	bl 0x0200c54c
	movs	r0, #26
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200c564
	movs	r0, #20
	bl 0x0200c45c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #19
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #160
	movs	r2, #0
	movs	r0, #26
	lsls	r1, r1, #7
	bl 0x0200c544
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #26
	bl 0x0200c564
	movs	r0, #20
	bl 0x0200c45c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #27
	bl 0x0200c55c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #25
	mov	r1, r8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #4
	movs	r2, #20
	adds	r1, #255
	movs	r0, #25
	bl 0x0200c55c
	movs	r0, #25
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #20
	mov	r1, r9
	movs	r0, #27
	bl 0x0200c55c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #0
	movs	r0, #26
	mov	r1, r8
	bl 0x0200c544
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #0
	movs	r0, #0
	mov	r1, sl
	bl 0x0200c544
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #160
	movs	r0, #25
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #25
	bl 0x0200c55c
	movs	r0, #25
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #26
	movs	r1, #4
	bl 0x0200c4e4
	movs	r2, #20
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c52c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c54c
	movs	r1, #132
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #0
	bl 0x0200c55c
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #7
	bl 0x0200c524
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c474
	cmp	r0, #0
	bne.n	.L_02002078
	movs	r0, #26
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c534
	ldr	r2, [r7, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002096
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x2e70
	.2byte 0x0000
.L_02002078:
	ldr	r2, [r7, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #26
	adds	r3, #1
	movs	r1, #4
	strh	r3, [r2, #0]
	bl 0x0200c4ec
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c534
.L_02002096:
	movs	r1, #128
	movs	r0, #27
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #27
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200c55c
	movs	r0, #25
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #176
	movs	r2, #20
	movs	r0, #27
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r0, #27
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #128
	movs	r0, #27
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #27
	movs	r1, #0
	bl 0x0200c534
	ldr	r5, [pc, #580]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r2, #0
	movs	r1, #0
	bl 0x0200c544
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c54c
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #26
	bl 0x0200c564
	movs	r0, #20
	bl 0x0200c45c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #176
	movs	r2, #0
	movs	r0, #27
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r1, #128
	movs	r0, #25
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #25
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #160
	movs	r0, #25
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #25
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #27
	bl 0x0200c55c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #27
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c534
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200c55c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c55c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #26
	bl 0x0200c55c
	movs	r1, #2
	movs	r2, #40
	adds	r1, #255
	movs	r0, #25
	bl 0x0200c55c
	movs	r0, #27
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4e4
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #26
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #25
	movs	r1, #3
	bl 0x0200c4e4
	movs	r1, #128
	movs	r0, #27
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #27
	bl 0x0200c55c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #10
	adds	r1, #255
	movs	r2, #20
	movs	r0, #26
	bl 0x0200c55c
	movs	r2, #20
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c52c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #129
	movs	r0, #27
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200c544
	movs	r1, #160
	movs	r0, #27
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #27
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c54c
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #7
	bl 0x0200c524
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c474
	cmp	r0, #0
	bne.n	.L_0200234c
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #108]
	adds	r1, #204
	bl 0x0200c56c
	movs	r0, #184
	movs	r1, #1
	movs	r2, #128
	movs	r3, #1
	lsls	r2, r2, #18
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200c574
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #0
	ldr	r1, [pc, #68]
	adds	r2, #204
	bl 0x0200c484
	movs	r2, #130
	lsls	r2, r2, #2
	movs	r0, #0
	movs	r1, #168
	bl 0x0200c4b4
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200c534
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_020023b6
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00026666
	.2byte 0x9999
	.2byte 0x0001
.L_0200234c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #152
	adds	r3, #2
	lsls	r1, r1, #7
	strh	r3, [r2, #0]
	ldr	r0, [pc, #672]
	adds	r1, #204
	bl 0x0200c56c
	movs	r0, #184
	movs	r1, #1
	movs	r2, #128
	movs	r3, #1
	lsls	r2, r2, #18
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200c574
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #0
	ldr	r1, [pc, #632]
	adds	r2, #204
	bl 0x0200c484
	movs	r2, #130
	movs	r0, #0
	movs	r1, #168
	lsls	r2, r2, #2
	bl 0x0200c4b4
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200c534
.L_020023b6:
	bl 0x0200c584
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #1
	movs	r0, #158
	movs	r2, #216
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c574
	bl 0x0200c57c
	movs	r2, #10
	movs	r0, #17
	movs	r1, #4
	bl 0x0200c4fc
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #152
	lsls	r1, r1, #6
	ldr	r0, [pc, #540]
	adds	r1, #102
	bl 0x0200c56c
	movs	r0, #172
	movs	r1, #1
	movs	r2, #130
	lsls	r2, r2, #18
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200c574
	movs	r0, #0
	movs	r1, #17
	bl 0x0200c5fc
	ldr	r3, [pc, #512]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	movs	r1, #17
	bl 0x0200c5fc
	movs	r0, #27
	movs	r1, #17
	bl 0x0200c5fc
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #17
	ldr	r1, [pc, #472]
	adds	r2, #204
	bl 0x0200c484
	movs	r2, #239
	movs	r0, #17
	movs	r1, #140
	lsls	r2, r2, #1
	bl 0x0200c4b4
	movs	r2, #135
	movs	r0, #17
	movs	r1, #150
	lsls	r2, r2, #2
	bl 0x0200c4b4
	movs	r2, #140
	movs	r0, #17
	movs	r1, #160
	lsls	r2, r2, #2
	bl 0x0200c4b4
	movs	r2, #160
	movs	r1, #190
	lsls	r2, r2, #2
	movs	r0, #17
	bl 0x0200c4b4
	movs	r0, #0
	bl 0x0200c49c
	ldr	r0, [r5, #0]
	bl 0x0200c49c
	movs	r0, #27
	bl 0x0200c49c
	movs	r0, #1
	bl 0x0200c344
	movs	r2, #0
	movs	r1, #0
	movs	r0, #17
	bl 0x0200c4cc
	movs	r0, #10
	bl 0x0200c45c
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #368]
	adds	r1, #204
	bl 0x0200c56c
	movs	r0, #184
	movs	r1, #1
	movs	r2, #224
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c574
	movs	r2, #242
	lsls	r2, r2, #1
	movs	r0, #0
	movs	r1, #168
	bl 0x0200c4b4
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c54c
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r1, #128
	movs	r0, #27
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #27
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #160
	movs	r2, #0
	movs	r0, #25
	lsls	r1, r1, #7
	bl 0x0200c544
	movs	r0, #25
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #25
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #176
	movs	r2, #0
	movs	r0, #27
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #26
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #26
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4e4
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #27
	movs	r1, #3
	bl 0x0200c4ec
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #0
	ldr	r1, [pc, #176]
	adds	r2, #204
	bl 0x0200c484
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #27
	ldr	r1, [pc, #160]
	adds	r2, #204
	bl 0x0200c484
	movs	r0, #0
	movs	r1, #2
	bl 0x0200c4e4
	ldr	r0, [r5, #0]
	bl 0x0200c47c
	cmp	r0, #0
	beq.n	.L_0200258c
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #0
	bl 0x0200c4ac
.L_0200258c:
	movs	r0, #0
	bl 0x0200c4c4
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c4cc
	movs	r0, #27
	movs	r1, #2
	bl 0x0200c4e4
	ldr	r0, [r5, #0]
	bl 0x0200c47c
	cmp	r0, #0
	beq.n	.L_020025bc
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #27
	bl 0x0200c4ac
.L_020025bc:
	movs	r0, #27
	bl 0x0200c4c4
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c4cc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #93
	str	r3, [r2, #0]
	subs	r3, #85
	adds	r2, r1, r3
	movs	r0, #48
	movs	r3, #16
	str	r3, [r2, #0]
	adds	r0, #255
	bl 0x0200c3bc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #197
	bl 0x0200c3b4
	bl 0x0200c46c
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x00026666
	.4byte 0x00019999
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_02002634
	movs	r0, #190
	lsls	r0, r0, #1
	bl 0x0200c3b4
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200c3b4
.L_02002634:
	ldr	r3, [pc, #144]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #136]
	cmp	r2, r3
	bne.n	.L_0200265c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200a9c8
	b.n	.L_02002786
.L_0200265c:
	ldr	r3, [pc, #112]
	cmp	r2, r3
	bne.n	.L_0200267c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #133
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	adds	r2, #255
	str	r2, [r3, #0]
	bl 0x0200aa38
	b.n	.L_02002786
.L_0200267c:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_0200269a
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	movs	r0, #0
	str	r2, [r3, #0]
	bl 0x0200c5dc
	b.n	.L_02002786
.L_0200269a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_020026b2
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c3b4
.L_020026b2:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_020026d8
	movs	r0, #1
	bl 0x0200c61c
	b.n	.L_0200272a
	.4byte 0x02000240
	.4byte 0x00000110
	.4byte 0x00000111
	.2byte 0x0112
	.2byte 0x0000
.L_020026d8:
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #44]
	bl 0x0200c34c
	movs	r0, #0
	bl 0x0200c414
	movs	r0, #1
	bl 0x0200c414
	movs	r0, #2
	bl 0x0200c40c
	movs	r0, #3
	bl 0x0200c40c
	movs	r0, #4
	bl 0x0200c40c
	movs	r0, #5
	b.n	.L_02002720
	.4byte 0x00000c08
	.4byte 0x00003f10
	.2byte 0x98e1
	.2byte 0x0200
.L_02002720:
	bl 0x0200c40c
	movs	r0, #6
	bl 0x0200c40c
.L_0200272a:
	ldr	r3, [pc, #96]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_02002756
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_02002750
	movs	r0, #64
	movs	r1, #1
	bl 0x0200c5c4
.L_02002750:
	bl 0x0200a7e4
	b.n	.L_02002760
.L_02002756:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02002760
	bl 0x0200a878
.L_02002760:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_02002786
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #4
	movs	r1, #1
	bl 0x0200c59c
	movs	r0, #16
	bl 0x0200c5a4
	movs	r0, #16
	bl 0x0200c344
.L_02002786:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000010e
	.2byte 0x010f
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	r8, r3
	mov	sl, r0
	adds	r5, r1, #0
	adds	r6, r2, #0
	bl 0x0200c47c
	adds	r1, r0, #0
	adds	r3, r1, #0
	ldr	r2, [pc, #28]
	mov	r0, r8
	adds	r3, #100
	strh	r0, [r3, #0]
	subs	r3, #1
	strb	r2, [r3, #0]
	ldr	r3, [pc, #20]
	lsls	r5, r5, #16
	lsls	r6, r6, #16
	str	r3, [r1, #108]
	mov	r0, sl
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl 0x0200c4cc
	b.n	.L_020027dc
	.4byte 0x00000000
	.2byte 0x83bd
	.2byte 0x0200
.L_020027dc:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	push	{r5, lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_02002854
	movs	r0, #10
	adds	r0, #255
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_02002854
	movs	r5, #134
	lsls	r5, r5, #2
	movs	r2, #180
	lsls	r2, r2, #1
	movs	r0, #16
	adds	r1, r5, #0
	movs	r3, #0
	bl 0x0200a79c
	movs	r2, #212
	lsls	r2, r2, #1
	movs	r0, #17
	adds	r1, r5, #0
	movs	r3, #1
	bl 0x0200a79c
	movs	r1, #145
	movs	r2, #210
	lsls	r2, r2, #1
	movs	r3, #2
	lsls	r1, r1, #2
	movs	r0, #18
	bl 0x0200a79c
	movs	r0, #1
	bl 0x0200c344
	ldr	r1, [pc, #52]
	movs	r0, #16
	bl 0x0200c48c
	ldr	r1, [pc, #48]
	movs	r0, #17
	bl 0x0200c48c
	movs	r0, #18
	ldr	r1, [pc, #40]
	bl 0x0200c48c
	movs	r0, #1
	bl 0x0200c344
.L_02002854:
	movs	r1, #1
	movs	r0, #20
	bl 0x0200c554
	movs	r0, #20
	bl 0x0200c47c
	movs	r1, #15
	bl 0x0200c514
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200c7ac
	.4byte 0x0200c848
	.2byte 0xc8e4
	.2byte 0x0200
	push	{r5, lr}
	movs	r1, #1
	movs	r0, #19
	sub	sp, #8
	bl 0x0200c554
	movs	r0, #19
	bl 0x0200c47c
	movs	r1, #15
	bl 0x0200c514
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_020028f4
	movs	r0, #156
	lsls	r0, r0, #4
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_020028b4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c4cc
.L_020028b4:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_020028d4
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #89
	movs	r1, #0
	movs	r2, #77
	movs	r3, #16
	bl 0x0200c404
.L_020028d4:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #193
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_020028f4
	movs	r0, #10
	bl 0x0200c47c
	movs	r3, #176
	lsls	r3, r3, #8
	strh	r3, [r0, #6]
	movs	r0, #1
	bl 0x0200c344
.L_020028f4:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_02002946
	movs	r5, #3
	movs	r0, #72
	movs	r1, #3
	movs	r2, #69
	movs	r3, #3
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200c404
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #48
	movs	r1, #36
	movs	r2, #69
	movs	r3, #8
	str	r5, [sp, #0]
	bl 0x0200c404
	movs	r3, #70
	movs	r2, #20
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #72
	movs	r1, #13
	movs	r2, #1
	movs	r3, #3
	bl 0x0200c42c
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c4cc
	b.n	.L_020029a0
.L_02002946:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #45
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_020029a0
	movs	r5, #3
	movs	r0, #72
	movs	r1, #3
	movs	r2, #69
	movs	r3, #3
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200c404
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #48
	movs	r1, #36
	movs	r2, #69
	movs	r3, #8
	str	r5, [sp, #0]
	bl 0x0200c404
	movs	r3, #70
	movs	r2, #20
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #72
	movs	r1, #13
	movs	r2, #1
	movs	r3, #3
	bl 0x0200c42c
	movs	r0, #64
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_020029a0
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c4cc
.L_020029a0:
	add	sp, #8
	pop	{r5, pc}
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c47c
	ldr	r3, [r0, #80]
	movs	r0, #16
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x0200c554
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	bl 0x0200c5dc
	movs	r1, #144
	ldr	r0, [pc, #44]
	lsls	r1, r1, #3
	bl 0x0200c34c
	ldr	r3, [pc, #40]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #20
	bne.n	.L_020029fc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #197
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_020029fc
	bl 0x02009cf0
.L_020029fc:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200a9a5
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #40]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c47c
	ldr	r3, [r0, #80]
	movs	r0, #8
	ldrb	r5, [r3, #9]
	lsls	r5, r5, #28
	lsrs	r5, r5, #30
	adds	r1, r5, #0
	bl 0x0200c554
	movs	r0, #10
	adds	r1, r5, #0
	bl 0x0200c554
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #10
	adds	r0, #255
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_02002a72
	ldr	r3, [pc, #60]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #6
	bne.n	.L_02002a60
	movs	r0, #0
	bl 0x02009c24
	b.n	.L_02002a72
.L_02002a60:
	subs	r3, r2, #7
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_02002a72
	movs	r0, #1
	bl 0x02009c24
.L_02002a72:
	movs	r0, #0
	bl 0x0200c5dc
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl 0x0200c34c
	pop	{pc}
	.4byte 0x02000240
	.2byte 0xaa09
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #197
	bl 0x0200c3ac
	cmp	r0, #0
	bne.n	.L_02002aa4
	bl 0x0200b84e
.L_02002aa4:
	bl 0x0200c464
	movs	r0, #0
	bl 0x0200c5ec
	movs	r0, #78
	bl 0x0200c63c
	movs	r0, #5
	movs	r1, #2
	movs	r2, #10
	bl 0x0200c4fc
	movs	r2, #10
	movs	r1, #4
	movs	r0, #5
	bl 0x0200c4fc
	ldr	r0, [pc, #908]
	bl 0x0200c51c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #204
	lsls	r1, r1, #7
	ldr	r0, [pc, #896]
	adds	r1, #102
	bl 0x0200c56c
	movs	r0, #144
	movs	r1, #1
	movs	r2, #228
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200c574
	bl 0x0200c57c
	movs	r1, #132
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200c55c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #10
	bl 0x0200c534
	movs	r0, #37
	bl 0x0200c63c
	ldr	r5, [pc, #840]
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
	bl 0x0200c484
	ldr	r0, [r5, #0]
	movs	r1, #136
	movs	r2, #196
	bl 0x0200c4b4
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	bl 0x0200c544
	ldr	r1, [r5, #0]
	movs	r0, #0
	bl 0x0200c4dc
	ldr	r1, [r5, #0]
	movs	r0, #11
	bl 0x0200c4dc
	movs	r0, #1
	bl 0x0200c344
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #0
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #11
	adds	r1, #204
	bl 0x0200c484
	ldr	r1, [pc, #736]
	movs	r0, #0
	bl 0x0200c48c
	ldr	r1, [pc, #732]
	movs	r0, #11
	bl 0x0200c4a4
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r1, #4
	movs	r2, #20
	adds	r1, #255
	movs	r0, #5
	bl 0x0200c55c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200c55c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #8
	movs	r2, #20
	adds	r1, #255
	movs	r0, #5
	bl 0x0200c55c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #3
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #3
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #132
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200c55c
	movs	r1, #208
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r2, #10
	movs	r0, #5
	movs	r1, #4
	bl 0x0200c4fc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #1
	bl 0x0200c55c
	movs	r1, #160
	movs	r0, #1
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #1
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #6
	movs	r2, #40
	adds	r1, #255
	movs	r0, #2
	bl 0x0200c55c
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #2
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #0
	bl 0x0200c55c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c54c
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200c52c
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200c544
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	movs	r2, #20
	bl 0x0200c52c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #36
	bl 0x0200c3ac
	cmp	r0, #0
	beq.n	.L_02002d24
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #208
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c534
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002d48
.L_02002d24:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #208
	adds	r3, #2
	movs	r0, #8
	lsls	r1, r1, #8
	strh	r3, [r2, #0]
	bl 0x0200c54c
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c534
.L_02002d48:
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200c544
	movs	r1, #128
	movs	r2, #20
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r1, #160
	movs	r0, #11
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c54c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #9
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c54c
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #8
	bl 0x0200c524
	ldr	r5, [pc, #76]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c474
	cmp	r0, #0
	bne.n	.L_02002e6c
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	bl 0x0200c534
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002e90
	.2byte 0x0000
	.4byte 0x00002ee8
	.4byte 0x00033333
	.4byte 0x02000240
	.4byte 0x0200dee8
	.2byte 0xdf2c
	.2byte 0x0200
.L_02002e6c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #0
	adds	r3, #1
	movs	r1, #4
	strh	r3, [r2, #0]
	bl 0x0200c4ec
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	bl 0x0200c534
.L_02002e90:
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #5
	movs	r1, #2
	movs	r2, #10
	bl 0x0200c4fc
	movs	r2, #10
	movs	r0, #5
	movs	r1, #4
	bl 0x0200c4fc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #5
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4e4
	ldr	r3, [pc, #752]
	movs	r1, #3
	mov	r8, r3
	movs	r3, #133
	lsls	r3, r3, #2
	add	r8, r3
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200c4e4
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #7
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #8
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #3
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #2
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #1
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #0
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	mov	r3, r8
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r3, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #11
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #5
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #6
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #7
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #3
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #2
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c484
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #1
	adds	r1, #204
	bl 0x0200c484
	movs	r0, #0
	movs	r1, #2
	bl 0x0200c554
	mov	r3, r8
	ldr	r0, [r3, #0]
	movs	r1, #2
	bl 0x0200c554
	movs	r0, #11
	movs	r1, #2
	bl 0x0200c554
	movs	r0, #5
	movs	r1, #2
	bl 0x0200c554
	movs	r0, #6
	movs	r1, #2
	bl 0x0200c554
	movs	r0, #7
	movs	r1, #2
	bl 0x0200c554
	movs	r0, #8
	movs	r1, #2
	bl 0x0200c554
	movs	r0, #9
	movs	r1, #2
	bl 0x0200c554
	movs	r0, #10
	movs	r1, #2
	bl 0x0200c554
	movs	r0, #3
	movs	r1, #2
	bl 0x0200c554
	movs	r0, #2
	movs	r1, #2
	bl 0x0200c554
	movs	r0, #1
	movs	r1, #2
	bl 0x0200c554
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	adds	r1, #204
	bl 0x0200c56c
	movs	r0, #144
	movs	r1, #1
	movs	r2, #246
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200c574
	ldr	r1, [pc, #304]
	movs	r0, #9
	bl 0x0200c48c
	ldr	r1, [pc, #300]
	movs	r0, #10
	bl 0x0200c48c
	movs	r0, #10
	bl 0x0200c45c
	ldr	r5, [pc, #292]
	movs	r0, #8
	adds	r1, r5, #0
	bl 0x0200c48c
	ldr	r6, [pc, #284]
	movs	r0, #3
	adds	r1, r6, #0
	bl 0x0200c48c
	movs	r0, #10
	bl 0x0200c45c
	movs	r0, #6
	adds	r1, r5, #0
	bl 0x0200c48c
	adds	r1, r5, #0
	movs	r0, #7
	bl 0x0200c48c
	movs	r0, #10
	bl 0x0200c45c
	movs	r0, #2
	adds	r1, r6, #0
	bl 0x0200c48c
	ldr	r1, [pc, #244]
	movs	r0, #1
	bl 0x0200c48c
	movs	r0, #30
	bl 0x0200c45c
	ldr	r1, [pc, #232]
	movs	r0, #5
	bl 0x0200c48c
	movs	r0, #20
	bl 0x0200c45c
	ldr	r1, [pc, #224]
	movs	r0, #0
	bl 0x0200c48c
	mov	r3, r8
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #216]
	bl 0x0200c48c
	movs	r0, #10
	bl 0x0200c45c
	ldr	r1, [pc, #208]
	movs	r0, #11
	bl 0x0200c4a4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #20
	movs	r0, #11
	bl 0x0200c55c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #11
	bl 0x0200c544
	movs	r0, #5
	bl 0x0200c494
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r0, #1
	movs	r1, #0
	movs	r2, #40
	bl 0x0200c544
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #132
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200c55c
	movs	r0, #160
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #11
.L_0200316e:
	bl 0x0200c524
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	mov	r3, r8
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200c474
	cmp	r0, #0
.L_02003188:
	bne.n	.L_020031dc
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r3, #192
	lsls	r3, r3, #18
.L_020031a2:
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02003202
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200df70
	.4byte 0x0200dfac
	.4byte 0x0200dfe8
	.4byte 0x0200e038
	.4byte 0x0200e088
	.4byte 0x0200e0cc
	.4byte 0x0200e0fc
	.4byte 0x0200e17c
	.2byte 0xe1fc
	.2byte 0x0200
.L_020031dc:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #11
	adds	r3, #1
	movs	r1, #4
.L_020031f0:
	strh	r3, [r2, #0]
	bl 0x0200c4ec
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
.L_02003202:
	movs	r1, #192
	movs	r2, #20
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c544
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #11
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #10
	movs	r0, #1
	movs	r1, #2
	bl 0x0200c4fc
	movs	r0, #1
	movs	r1, #0
	bl 0x0200c534
	ldr	r5, [pc, #876]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #11
	bl 0x0200c55c
	movs	r1, #192
	movs	r2, #20
	movs	r0, #11
	lsls	r1, r1, #6
	bl 0x0200c544
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #11
	movs	r1, #4
	bl 0x0200c4ec
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #1
	bl 0x0200c55c
	movs	r2, #20
	movs	r0, #1
	movs	r1, #0
	bl 0x0200c52c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200c544
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	bl 0x0200c544
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #6
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #176
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #11
	bl 0x0200c55c
	movs	r0, #160
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #11
	bl 0x0200c524
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c474
	cmp	r0, #0
	bne.n	.L_0200334c
	movs	r0, #11
	movs	r1, #4
	bl 0x0200c4ec
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02003372
.L_0200334c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #11
	adds	r3, #1
	movs	r1, #3
	strh	r3, [r2, #0]
	bl 0x0200c4ec
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
.L_02003372:
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #1
	bl 0x0200c55c
	movs	r0, #1
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #6
	bl 0x0200c54c
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #5
	bl 0x0200c55c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #176
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	ldr	r5, [pc, #464]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #1
	movs	r1, #3
	bl 0x0200c4e4
	movs	r1, #3
	movs	r0, #5
	bl 0x0200c4ec
	movs	r0, #20
	bl 0x0200c45c
	movs	r0, #1
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #4
	movs	r2, #0
	adds	r1, #255
	movs	r0, #11
	bl 0x0200c55c
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #6
	bl 0x0200c54c
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #129
	movs	r0, #1
	lsls	r1, r1, #1
	bl 0x0200c564
	movs	r0, #1
	movs	r1, #4
	bl 0x0200c4ec
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #10
	movs	r0, #1
	movs	r1, #2
	bl 0x0200c4fc
	movs	r0, #1
	movs	r1, #0
	bl 0x0200c534
.L_02003472:
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
.L_0200348e:
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #1
	movs	r1, #0
.L_020034aa:
	bl 0x0200c54c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c4e4
	movs	r1, #3
	movs	r0, #1
	bl 0x0200c4ec
	movs	r0, #10
	bl 0x0200c45c
	movs	r2, #204
.L_020034c6:
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #228]
	adds	r2, #204
	bl 0x0200c484
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #1
	ldr	r1, [pc, #212]
	bl 0x0200c484
.L_020034e0:
	ldr	r1, [pc, #208]
	movs	r0, #5
	bl 0x0200c48c
	ldr	r1, [pc, #204]
	movs	r0, #1
	bl 0x0200c4a4
	movs	r0, #20
	bl 0x0200c45c
	movs	r1, #176
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #128
	movs	r0, #11
	movs	r1, #156
.L_0200351a:
	lsls	r2, r2, #1
	bl 0x0200c4b4
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200c55c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #176
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #11
	bl 0x0200c55c
	movs	r0, #160
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #11
	bl 0x0200c524
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c474
	cmp	r0, #0
	bne.n	.L_020035bc
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020035e2
	.4byte 0x02000240
	.4byte 0x00019999
	.4byte 0x0200e22c
	.2byte 0xe268
	.2byte 0x0200
.L_020035bc:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #11
	adds	r3, #1
	movs	r1, #4
	strh	r3, [r2, #0]
	bl 0x0200c4ec
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
.L_020035e2:
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #6
	bl 0x0200c54c
	movs	r0, #11
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #160
	lsls	r0, r0, #8
	movs	r2, #20
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c52c
	movs	r1, #0
	movs	r0, #0
	bl 0x0200c534
	movs	r0, #20
	bl 0x0200c45c
	movs	r1, #3
	movs	r0, #11
	bl 0x0200c4ec
	movs	r0, #20
	bl 0x0200c45c
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c544
	ldr	r5, [pc, #540]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200c544
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	bl 0x0200c544
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #6
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #11
	movs	r1, #4
	bl 0x0200c4e4
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c55c
	movs	r1, #2
	movs	r2, #40
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200c55c
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4e4
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c4e4
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4e4
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200c4ec
	movs	r0, #20
	bl 0x0200c45c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #11
	bl 0x0200c55c
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200c544
	movs	r2, #10
	movs	r0, #11
	movs	r1, #4
	bl 0x0200c4fc
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #176
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #6
	bl 0x0200c54c
	movs	r0, #11
	movs	r1, #2
	movs	r2, #10
	bl 0x0200c4fc
	movs	r2, #10
	movs	r0, #11
	movs	r1, #4
	bl 0x0200c4fc
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c544
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r1, #176
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200c54c
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r1, #128
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #6
	bl 0x0200c544
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	bl 0x0200c54c
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c4e4
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200c4ec
	movs	r1, #132
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200c55c
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #6
	bl 0x0200c54c
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #11
	movs	r1, #0
	bl 0x0200c534
	movs	r0, #11
	movs	r1, #9
	bl 0x0200c4e4
	ldr	r0, [r5, #0]
	movs	r1, #33
	bl 0x0200c4e4
	movs	r1, #40
	movs	r0, #0
	bl 0x0200c4e4
	movs	r0, #2
	bl 0x0200c414
	movs	r0, #3
	bl 0x0200c414
	movs	r0, #4
	bl 0x0200c414
	movs	r0, #5
	bl 0x0200c414
	movs	r0, #6
	bl 0x0200c414
	movs	r0, #78
	bl 0x0200c63c
	movs	r0, #40
	bl 0x0200c45c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #9
	bl 0x0200c59c
	movs	r0, #120
	bl 0x0200c5a4
	movs	r0, #120
	bl 0x0200c344
	movs	r5, #0
	b.n	.L_02003818
.L_02003816:
	adds	r5, #1
.L_02003818:
	cmp	r5, #119
	bhi.n	.L_0200382a
	movs	r0, #1
	bl 0x0200c344
	ldr	r3, [pc, #52]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02003816
.L_0200382a:
	movs	r1, #0
	movs	r0, #0
	bl 0x0200c59c
	movs	r0, #16
	bl 0x0200c5a4
	movs	r0, #16
	bl 0x0200c344
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #204
	bl 0x0200c3b4
	movs	r0, #3
	bl 0x0200c58c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #100
	movs	r0, #0
	ldrsh	r1, [r2, r0]
	ldrh	r3, [r2, #0]
	cmp	r1, #0
	beq.n	.L_02003874
	subs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020038da
.L_02003874:
	adds	r3, r5, #0
	adds	r3, #90
	movs	r0, #131
	strb	r1, [r3, #0]
	lsls	r0, r0, #1
	bl 0x0200c3ac
	movs	r3, #1
	negs	r3, r3
	cmp	r0, #0
	bne.n	.L_0200389a
	ldr	r3, [pc, #80]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #76]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
.L_0200389a:
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_020038ac
	adds	r0, r5, #0
	movs	r1, #9
	bl 0x0200c3c4
	b.n	.L_020038da
.L_020038ac:
	ldrh	r1, [r5, #6]
	movs	r2, #128
	subs	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_020038be
	adds	r3, r2, #0
.L_020038be:
	ldr	r2, [pc, #36]
	cmp	r3, r2
	bge.n	.L_020038c6
	adds	r3, r2, #0
.L_020038c6:
	adds	r3, r1, r3
	adds	r0, r5, #0
	movs	r1, #2
	strh	r3, [r5, #6]
	bl 0x0200c3c4
	adds	r0, r5, #0
	movs	r1, #48
	bl 0x0200c3cc
.L_020038da:
	pop	{r5, pc}
	.4byte 0x03001150
	.4byte 0x0200c740
	.2byte 0xf000
	.2byte 0xffff
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #162
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02003918
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200c3b4
	bl 0x0200c594
	bl 0x0200c5e4
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200c3bc
.L_02003918:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #176]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #40
	bl 0x0200c5d4
	adds	r7, r0, #0
.L_0200393c:
	bl 0x0200b8e8
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r1, [pc, #132]
	ldr	r3, [r7, #8]
	add	r2, sp, #28
	mov	sl, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	mov	r0, sl
	str	r3, [sp, #12]
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [sp, #8]
	str	r3, [r0, #8]
	ldr	r2, [sp, #12]
	str	r3, [sp, #16]
	adds	r3, r7, #0
	adds	r3, #34
	str	r2, [sp, #20]
	str	r3, [sp, #4]
	adds	r1, r2, #0
	ldrb	r0, [r3, #0]
	ldr	r2, [sp, #16]
	bl 0x0200c424
	str	r0, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r1, [r7, #8]
	ldr	r0, [sp, #16]
	ldr	r3, [pc, #68]
	ldr	r6, [r7, #16]
	subs	r1, r2, r1
	subs	r6, r0, r6
	mov	r8, r3
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x182d
	movs	r0, #128
	lsls	r0, r0, #11
	cmp	r5, r0
	bge.n	.L_020039f4
	ldr	r3, [pc, #36]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #36]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
	mov	r9, r2
	lsls	r3, r2, #16
	ldr	r2, [pc, #24]
	cmp	r3, r2
	bne.n	.L_02003a20
	b.n	.L_02003bb6
	.4byte 0x02000240
	.4byte 0xfff00000
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200c780
	.2byte 0x0000
	.2byte 0xffff
.L_020039f4:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl 0x0200c364
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #128
	ldr	r2, [pc, #16]
	mov	r9, r0
	lsls	r3, r3, #6
	add	r3, r9
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r9, r3
	b.n	.L_02003a20
	.2byte 0xc000
	.2byte 0xffff
.L_02003a20:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200c36c
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl 0x0200c424
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_02003aa2
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	bl 0x0200c41c
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02003aa2
	ldr	r0, [sp, #12]
	mov	r2, sl
	str	r0, [r2, #0]
	ldr	r3, [sp, #8]
	adds	r0, r7, #0
	str	r3, [r2, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r2, r7, #0
	str	r3, [r7, #52]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #8]
	ldr	r2, [r7, #12]
	bl 0x0200c3f4
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200c3c4
	adds	r0, r7, #0
	movs	r1, #48
	bl 0x0200c3cc
	adds	r0, r7, #0
	bl 0x0200c3fc
	ldr	r3, [pc, #292]
	str	r3, [r7, #108]
	b.n	.L_02003b4c
.L_02003aa2:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_02003b98
.L_02003ab6:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl 0x0200c41c
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02003b6c
	ldrh	r3, [r7, #32]
	movs	r2, #89
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r0, #0
	adds	r2, r2, r5
	mov	sl, r0
	mov	r8, r2
.L_02003ae4:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02003b0e
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003b0e
	cmp	r5, r7
	beq.n	.L_02003b0e
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl 0x0200c454
	cmp	r0, #0
	bge.n	.L_02003b6c
.L_02003b0e:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02003ae4
	ldr	r0, [r6, #0]
	movs	r3, #128
	str	r0, [sp, #20]
	lsls	r3, r3, #10
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	str	r2, [sp, #16]
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #52]
	ldr	r3, [r6, #8]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	bl 0x0200c3f4
	adds	r0, r7, #0
	bl 0x0200c3fc
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_02003b92
.L_02003b4c:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200c36c
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200c424
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_02003ab6
.L_02003b6c:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	ldr	r2, [r7, #12]
	adds	r0, r7, #0
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #16]
	bl 0x0200c3f4
	adds	r0, r7, #0
	bl 0x0200c3fc
	movs	r0, #2
	bl 0x0200c344
	b.n	.L_0200393c
.L_02003b92:
	movs	r0, #10
	bl 0x0200c344
.L_02003b98:
	movs	r3, #0
	str	r3, [r7, #108]
	adds	r1, r7, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200c3c4
.L_02003bb6:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0xb85d
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #80]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #40
	bl 0x0200c5d4
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	adds	r7, r0, #0
	strh	r3, [r2, #0]
.L_02003bee:
	bl 0x0200b8e8
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r1, [pc, #32]
	ldr	r3, [r7, #8]
	add	r2, sp, #28
	mov	sl, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	mov	r0, sl
	str	r3, [sp, #12]
	str	r3, [r0, #0]
	b.n	.L_02003c34
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x0200e2a4
	.2byte 0x0000
	.2byte 0xfff0
.L_02003c34:
	.2byte 0x68fb
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [sp, #8]
	str	r3, [r0, #8]
	ldr	r2, [sp, #12]
	str	r3, [sp, #16]
	adds	r3, r7, #0
	adds	r3, #34
	str	r2, [sp, #20]
	str	r3, [sp, #4]
	adds	r1, r2, #0
	ldrb	r0, [r3, #0]
	ldr	r2, [sp, #16]
	bl 0x0200c424
	str	r0, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r1, [r7, #8]
	ldr	r0, [sp, #16]
	ldr	r3, [pc, #60]
	ldr	r6, [r7, #16]
	subs	r1, r2, r1
	subs	r6, r0, r6
	mov	r8, r3
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x182d
	movs	r0, #128
	lsls	r0, r0, #11
	cmp	r5, r0
	bge.n	.L_02003cb0
	ldr	r3, [pc, #28]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #28]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
	mov	r9, r2
	lsls	r3, r2, #16
	ldr	r2, [pc, #16]
	cmp	r3, r2
	bne.n	.L_02003cdc
	b.n	.L_02003ea6
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200c780
	.2byte 0x0000
	.2byte 0xffff
.L_02003cb0:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl 0x0200c364
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #128
	ldr	r2, [pc, #16]
	mov	r9, r0
	lsls	r3, r3, #6
	add	r3, r9
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r9, r3
	b.n	.L_02003cdc
	.2byte 0xc000
	.2byte 0xffff
.L_02003cdc:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200c36c
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl 0x0200c424
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_02003d56
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	bl 0x0200c41c
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02003d56
	ldr	r0, [sp, #12]
	mov	r2, sl
	str	r0, [r2, #0]
	ldr	r3, [sp, #8]
	adds	r0, r7, #0
	str	r3, [r2, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r2, r7, #0
	str	r3, [r7, #52]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [r7, #12]
	ldr	r3, [sp, #8]
	bl 0x0200c3f4
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200c3c4
	adds	r0, r7, #0
	movs	r1, #48
	bl 0x0200c3cc
	movs	r5, #0
	b.n	.L_02003d7e
.L_02003d56:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_02003ea6
.L_02003d6a:
	ldr	r3, [pc, #360]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_02003d76
	b.n	.L_02003ea6
.L_02003d76:
	movs	r0, #1
	bl 0x0200c344
	adds	r5, #1
.L_02003d7e:
	cmp	r5, #179
	bgt.n	.L_02003d8c
	adds	r0, r7, #0
	bl 0x0200c44c
	cmp	r0, #0
	beq.n	.L_02003d6a
.L_02003d8c:
	ldr	r3, [pc, #328]
	str	r3, [r7, #108]
	b.n	.L_02003e5a
.L_02003d92:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl 0x0200c41c
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02003e7a
	ldr	r3, [pc, #296]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02003ea6
	ldrh	r3, [r7, #32]
	movs	r2, #89
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r0, #0
	adds	r2, r2, r5
	mov	sl, r0
	mov	r8, r2
.L_02003dca:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02003df4
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003df4
	cmp	r5, r7
	beq.n	.L_02003df4
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl 0x0200c454
	cmp	r0, #0
	bge.n	.L_02003e7a
.L_02003df4:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02003dca
	ldr	r0, [r6, #0]
	movs	r3, #128
	str	r0, [sp, #20]
	lsls	r3, r3, #10
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	str	r2, [sp, #16]
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #52]
	movs	r5, #0
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl 0x0200c3f4
	b.n	.L_02003e32
.L_02003e2a:
	movs	r0, #1
	bl 0x0200c344
	adds	r5, #1
.L_02003e32:
	cmp	r5, #179
	bgt.n	.L_02003e4a
	adds	r0, r7, #0
	bl 0x0200c44c
	cmp	r0, #0
	bne.n	.L_02003e4a
	ldr	r3, [pc, #144]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_02003e2a
.L_02003e4a:
	ldr	r3, [pc, #136]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02003ea6
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_02003ea0
.L_02003e5a:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200c36c
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200c424
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_02003d92
.L_02003e7a:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	ldr	r2, [r7, #12]
	adds	r0, r7, #0
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #16]
	bl 0x0200c3f4
	adds	r0, r7, #0
	bl 0x0200c3fc
	movs	r0, #2
	bl 0x0200c344
	b.n	.L_02003bee
.L_02003ea0:
	movs	r0, #10
	bl 0x0200c344
.L_02003ea6:
	movs	r3, #0
	str	r3, [r7, #108]
	adds	r1, r7, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200c3c4
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200e2a4
	.4byte 0x0200b85d
	.4byte 0x80184b01
	.4byte 0x00004770
	.2byte 0xe2a4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #92]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	sub	sp, #32
	bl 0x0200c5d4
	adds	r5, r0, #0
	ldrh	r3, [r5, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #60]
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	str	r3, [sp, #16]
.L_02003f1a:
	bl 0x0200b8e8
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #56]
	str	r3, [r5, #64]
	movs	r3, #0
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	ldr	r2, [pc, #28]
	ldr	r3, [r5, #8]
	movs	r1, #128
	lsls	r1, r1, #12
	ands	r3, r2
	mov	r9, r1
	add	r6, sp, #20
	add	r3, r9
	str	r3, [r6, #0]
	mov	r8, r3
	b.n	.L_02003f5c
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
.L_02003f5c:
	.2byte 0x68eb
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	ands	r3, r2
	adds	r7, r3, r1
	mov	r2, r8
	str	r7, [r6, #8]
	str	r2, [sp, #8]
	str	r7, [sp, #4]
	movs	r3, #34
	adds	r3, r3, r5
	ldrb	r0, [r3, #0]
	adds	r1, r2, #0
	adds	r2, r7, #0
	mov	fp, r3
	bl 0x0200c424
	str	r0, [sp, #12]
	movs	r0, #128
	ldr	r1, [sp, #16]
	lsls	r0, r0, #13
	adds	r2, r6, #0
	bl 0x0200c36c
	mov	r1, fp
	ldrb	r0, [r1, #0]
	ldr	r2, [r6, #8]
	ldr	r1, [r6, #0]
	bl 0x0200c424
	mov	sl, r0
	cmp	r0, #255
	beq.n	.L_02003ff0
	mov	r2, fp
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200c41c
	ldr	r3, [r5, #12]
	subs	r0, r0, r3
	cmp	r0, r9
	bgt.n	.L_02003ff0
	ldr	r3, [sp, #8]
	ldr	r2, [pc, #48]
	str	r3, [r6, #0]
	ldr	r1, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r1, [r6, #8]
	str	r3, [r5, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #52]
	adds	r3, r5, #0
	adds	r3, #100
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200c3c4
	adds	r0, r5, #0
	movs	r1, #48
	bl 0x0200c3cc
	ldr	r3, [pc, #8]
	str	r3, [r5, #108]
	b.n	.L_0200409a
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0xb85d
	.2byte 0x0200
.L_02003ff0:
	add	r1, sp, #16
	ldrh	r1, [r1, #0]
	movs	r3, #0
	mov	r2, r8
	strh	r1, [r5, #6]
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	str	r2, [r5, #8]
	str	r7, [r5, #16]
	b.n	.L_020040e6
.L_02004004:
	mov	r3, fp
	ldrb	r0, [r3, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl 0x0200c41c
	ldr	r3, [r5, #12]
	movs	r1, #128
	subs	r0, r0, r3
	lsls	r1, r1, #12
	cmp	r0, r1
	bgt.n	.L_020040ba
	ldrh	r3, [r5, #32]
	movs	r2, #0
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #20]
	movs	r3, #89
	adds	r3, r3, r6
	mov	r9, r2
	mov	r8, r3
.L_02004032:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_0200405c
	mov	r1, r8
	ldrb	r2, [r1, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200405c
	cmp	r6, r5
	beq.n	.L_0200405c
	ldrh	r3, [r6, #32]
	adds	r0, r6, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #20
	bl 0x0200c454
	cmp	r0, #0
	bge.n	.L_020040ba
.L_0200405c:
	movs	r2, #1
	add	r9, r2
	movs	r3, #128
	mov	r1, r9
	add	r8, r3
	adds	r6, #128
	cmp	r1, #63
	ble.n	.L_02004032
	ldr	r2, [r7, #0]
	adds	r0, r5, #0
	str	r2, [sp, #8]
	ldr	r3, [r7, #8]
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #52]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	bl 0x0200c3f4
	adds	r0, r5, #0
	bl 0x0200c3fc
	ldr	r1, [sp, #12]
	cmp	sl, r1
	bne.n	.L_020040e0
.L_0200409a:
	movs	r0, #128
	ldr	r1, [sp, #16]
	add	r2, sp, #20
	lsls	r0, r0, #13
	bl 0x0200c36c
	mov	r2, fp
	add	r7, sp, #20
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl 0x0200c424
	mov	sl, r0
	cmp	r0, #255
	bne.n	.L_02004004
.L_020040ba:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	ldr	r1, [sp, #8]
	ldr	r3, [sp, #4]
	bl 0x0200c3f4
	adds	r0, r5, #0
	bl 0x0200c3fc
	movs	r0, #2
	bl 0x0200c344
	b.n	.L_02003f1a
.L_020040e0:
	movs	r0, #10
	bl 0x0200c344
.L_020040e6:
	movs	r3, #0
	str	r3, [r5, #108]
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #52]
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200c3c4
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	movs	r3, #192
	adds	r5, r7, r2
	lsls	r3, r3, #4
	movs	r2, #63
	adds	r6, r7, r3
	mov	r8, r2
.L_02004132:
	ldr	r3, [r5, #24]
	cmp	r3, #19
	bhi.n	.L_02004180
	movs	r2, #176
	lsls	r2, r2, #5
	adds	r2, #2
	adds	r1, r7, r2
	ldrh	r1, [r1, #0]
	movs	r2, #7
	asrs	r3, r3, #2
	ands	r3, r2
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	ands	r1, r3
	ldrh	r3, [r6, #8]
	adds	r0, r6, #0
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #8]
	adds	r1, r5, #0
	bl 0x0200c60c
	adds	r0, r5, #0
	movs	r1, #63
	ldr	r2, [pc, #20]
	bl 0x0200c614
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	b.n	.L_02004180
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x8000
	.2byte 0xffff
.L_02004180:
	.2byte 0x2301
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	adds	r6, #40
	adds	r5, #28
	cmp	r2, #0
	bge.n	.L_02004132
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r2, [sp, #0]
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	adds	r2, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	fp, r3
	cmp	r2, #0
	ble.n	.L_02004244
	adds	r7, r2, #0
.L_020041c0:
	bl 0x0200c35c
	movs	r1, #176
	lsls	r1, r1, #5
	add	r1, fp
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	sl, r1
	lsls	r6, r3, #3
	subs	r6, r6, r3
	lsls	r6, r6, #2
	movs	r3, #160
	add	r6, fp
	lsls	r3, r3, #3
	adds	r5, r6, r3
	movs	r1, #0
	str	r1, [r5, #24]
	ldr	r2, [sp, #8]
	mov	r8, r1
	str	r2, [r5, #0]
	ldr	r3, [sp, #4]
	mov	r9, r0
	str	r3, [r5, #4]
	ldr	r1, [sp, #0]
	subs	r7, #1
	str	r1, [r5, #8]
	bl 0x0200c35c
	movs	r2, #128
	lsls	r2, r2, #12
	lsls	r0, r0, #3
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r5, #0
	bl 0x0200c36c
	mov	r3, r8
	str	r3, [r5, #12]
	movs	r3, #160
	lsls	r3, r3, #11
	mov	r1, r8
	str	r3, [r5, #16]
	str	r1, [r5, #20]
	bl 0x0200c35c
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #12
	movs	r2, #128
	adds	r6, r6, r3
	lsls	r2, r2, #10
	lsls	r0, r0, #1
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r6, #0
	bl 0x0200c36c
	mov	r1, sl
	ldrh	r3, [r1, #0]
	movs	r2, #63
	adds	r3, #1
	ands	r3, r2
	mov	r2, sl
	strh	r3, [r2, #0]
	cmp	r7, #0
	bne.n	.L_020041c0
.L_02004244:
	add	sp, #12
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
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r1, #8
	movs	r0, #220
	sub	sp, #4
	bl 0x0200c374
	adds	r6, r0, #0
	ldr	r0, [pc, #152]
	bl 0x0200c3a4
	adds	r1, r6, #0
	bl 0x0200c384
	bl 0x0200c39c
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r2, r6, #0
	adds	r5, r0, #0
	bl 0x0200c394
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r1, #2
	mov	sl, r0
	adds	r3, r6, r1
	mov	r2, sl
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r6, r1
	strh	r5, [r3, #0]
	movs	r2, #160
	movs	r3, #192
	lsls	r2, r2, #3
	lsls	r3, r3, #4
	movs	r1, #63
	adds	r7, r6, r2
	adds	r5, r6, r3
	mov	r8, r1
.L_020042ac:
	mov	r2, sl
	movs	r3, #128
	str	r2, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	bl 0x0200c604
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	movs	r1, #13
	strb	r3, [r5, #5]
	negs	r1, r1
	movs	r3, #15
	ands	r3, r2
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	movs	r3, #240
	strh	r3, [r5, #30]
	subs	r3, #241
	add	r8, r3
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r5, #40
	adds	r7, #28
	cmp	r2, #0
	bge.n	.L_020042ac
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r2, r6, r1
	movs	r3, #0
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200c34c
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x000001f0
	.2byte 0xc115
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [pc, #28]
	ldr	r5, [r3, #0]
	bl 0x0200c354
	movs	r3, #176
	lsls	r3, r3, #5
	adds	r3, #4
	adds	r5, r5, r3
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl 0x0200c38c
	movs	r0, #220
	bl 0x0200c37c
	pop	{r5, pc}
	.4byte 0x0200c115
	.irp EntryTarget, 0x03000528, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000101, 0x08000129, 0x08000141, 0x08000151, 0x080001a9, 0x080001b9, 0x080001c9, 0x080001d1, 0x08000291, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x08020099, 0x080200a9, 0x080200c1, 0x080200c9, 0x080200e9, 0x08020149, 0x08020151, 0x08020179, 0x08020199, 0x080201a1, 0x080201c1, 0x080201c9, 0x080201e9, 0x08020219, 0x08020221, 0x08020229, 0x080202f9, 0x08020349, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80b1, 0x080c80b9, 0x080c80c1, 0x080c80d9, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8101, 0x080c8111, 0x080c8119, 0x080c8129, 0x080c8131, 0x080c8139, 0x080c8149, 0x080c8159, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81a9, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8259, 0x080c8279, 0x080c82f9, 0x080c8379, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c8421, 0x080c8429, 0x080c8459, 0x080c8481, 0x080c84d9, 0x080c84e1, 0x080c8581, 0x080c8601, 0x080c87c1, 0x080c87d1, 0x080c87e1, 0x080c8919, 0x08108009, 0x08108011, 0x08108019, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
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
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xe000c000
	.4byte 0xc000a000
	.4byte 0x20004000
	.4byte 0x40006000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000c000
	.4byte 0xc0008000
	.4byte 0x00004000
	.4byte 0x40008000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0200c644
	.4byte 0x0200c680
	.4byte 0x0200c6bc
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02340000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02340000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02340000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x009a0000
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
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
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
	.4byte 0x0000010d
	.4byte 0x0013e002
	.4byte 0x0020110e
	.4byte 0x00300003
	.4byte 0x0000010e
	.4byte 0x0010210d
	.4byte 0x00208110
	.4byte 0x00301110
	.4byte 0x0040a110
	.4byte 0x00504110
	.4byte 0x00605110
	.4byte 0x00702110
	.4byte 0x00807112
	.4byte 0x0090210f
	.4byte 0x00a09111
	.4byte 0x00b0110f
	.4byte 0x0000010f
	.4byte 0x0010b10e
	.4byte 0x0020910e
	.4byte 0x0033f002
	.4byte 0x00407110
	.4byte 0x00000110
	.4byte 0x0010310e
	.4byte 0x0020710e
	.4byte 0x00306111
	.4byte 0x0040510e
	.4byte 0x0050610e
	.4byte 0x00607111
	.4byte 0x0070410f
	.4byte 0x0080210e
	.4byte 0x00908111
	.4byte 0x00a0410e
	.4byte 0x00000111
	.4byte 0x00603110
	.4byte 0x00706110
	.4byte 0x00809110
	.4byte 0x0090a10e
	.4byte 0x00000112
	.4byte 0x0070810e
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002d000
	.4byte 0xffff003c
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002d000
	.4byte 0xffff003d
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002b000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0002a000
	.4byte 0xffff0038
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
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00af
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00033000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00015000
	.4byte 0xffff00ae
	.4byte 0x00000003
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0xffff00b0
	.4byte 0x00000001
	.4byte 0x01c40000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00015000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x0001b000
	.4byte 0xffff00ad
	.4byte 0x00000001
	.4byte 0x02440000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00038000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x00f40000
	.4byte 0x00033000
	.4byte 0xffff00b3
	.4byte 0x00000001
	.4byte 0x01760000
	.4byte 0x00000000
	.4byte 0x01a40000
	.4byte 0x00015000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff00b3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0xffff011e
	.4byte 0x00000007
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00af
	.4byte 0x00000001
	.4byte 0x021c0000
	.4byte 0x00000000
	.4byte 0x01840000
	.4byte 0x0003b000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01840000
	.4byte 0x0001b000
	.4byte 0xffff002c
	.4byte 0x00000001
	.4byte 0x00b40000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00035000
	.4byte 0xffff002d
	.4byte 0x00000001
	.4byte 0x008c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00033000
	.4byte 0xffff009b
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001b000
	.4byte 0xffff009b
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001b000
	.4byte 0xffff009b
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001d000
	.4byte 0xffff0038
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
	.4byte 0xffff011e
	.4byte 0x00000007
	.4byte 0x01b40000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00024000
	.4byte 0x004000f3
	.4byte 0x00000001
	.4byte 0x01940000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00af
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002b000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002b000
	.4byte 0xffff002c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00025000
	.4byte 0xffff002d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff009b
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0001b000
	.4byte 0xffff009b
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0001b000
	.4byte 0xffff009b
	.4byte 0x00000003
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00a20000
	.4byte 0x0000d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0000d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00920000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0001d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00033000
	.4byte 0xffff0038
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
	.4byte 0xffff011e
	.4byte 0x00000007
	.4byte 0x01b40000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00024000
	.4byte 0x004000f3
	.4byte 0x00000001
	.4byte 0x01940000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00af
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00008000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x0001b000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00005000
	.4byte 0xffff00b0
	.4byte 0x00000003
	.4byte 0x016c0000
	.4byte 0x00000000
	.4byte 0x007b0000
	.4byte 0x00013000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00003000
	.4byte 0xffff00ab
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00018000
	.4byte 0xffff00ad
	.4byte 0x00000001
	.4byte 0x02ae0000
	.4byte 0x00000000
	.4byte 0x006f0000
	.4byte 0x0001d000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x02240000
	.4byte 0x00000000
	.4byte 0x03300000
	.4byte 0x00008000
	.4byte 0xffff00b3
	.4byte 0x00000001
	.4byte 0x01d20000
	.4byte 0x00000000
	.4byte 0x032a0000
	.4byte 0x0001d000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00015000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x02120000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00035000
	.4byte 0xffff00ac
	.4byte 0x00000003
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x020c0000
	.4byte 0x00010000
	.4byte 0xffff00ab
	.4byte 0x00000003
	.4byte 0x00de0000
	.4byte 0x00000000
	.4byte 0x01ea0000
	.4byte 0x00015000
	.4byte 0xffff00ad
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x018a0000
	.4byte 0x0001d000
	.4byte 0xffff00ae
	.4byte 0x00000001
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x031a0000
	.4byte 0x00015000
	.4byte 0xffff00b0
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03300000
	.4byte 0x00013000
	.4byte 0x005700f4
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00af
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00008000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x0001b000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00005000
	.4byte 0xffff00b0
	.4byte 0x00000003
	.4byte 0x016c0000
	.4byte 0x00000000
	.4byte 0x007b0000
	.4byte 0x00013000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00003000
	.4byte 0xffff00ab
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00018000
	.4byte 0xffff00ad
	.4byte 0x00000003
	.4byte 0x029a0000
	.4byte 0x00000000
	.4byte 0x00860000
	.4byte 0x0001d000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00025000
	.4byte 0xffff00b3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002d000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00025000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x02120000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00035000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00005000
	.4byte 0xffff00ab
	.4byte 0x00000003
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x022a0000
	.4byte 0x00005000
	.4byte 0xffff00ad
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002d000
	.4byte 0xffff00ae
	.4byte 0x00000001
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x031a0000
	.4byte 0x00015000
	.4byte 0xffff00b0
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03300000
	.4byte 0x00013000
	.4byte 0x005700f4
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00008000
	.4byte 0xffff002c
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01ac0000
	.4byte 0x00035000
	.4byte 0xffff002d
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01ac0000
	.4byte 0x00013000
	.4byte 0xffff0038
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
	.4byte 0xffff0005
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
	.4byte 0xffff00ae
	.4byte 0x00000001
	.4byte 0x02740000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00015000
	.4byte 0xffff00ae
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0003b000
	.4byte 0xffff00b0
	.4byte 0x00000001
	.4byte 0x01720000
	.4byte 0x00000000
	.4byte 0x02e40000
	.4byte 0x00015000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ae
	.4byte 0x00000003
	.4byte 0x029a0000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00015000
	.4byte 0xffff00ae
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0003b000
	.4byte 0xffff00b0
	.4byte 0x00000001
	.4byte 0x01720000
	.4byte 0x00000000
	.4byte 0x02e40000
	.4byte 0x00015000
	.4byte 0xffff00ad
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x03090000
	.4byte 0x00015000
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
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x09cc000a
	.4byte 0x0200aa8d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02009a9d
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02009ab9
	.4byte 0x0000ce01
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02009ad5
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02009af1
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02009b0d
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02009b29
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02009b45
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x0a3d0008
	.4byte 0x00002cdd
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002ea0
	.4byte 0x00008d15
	.4byte 0x0a3d0008
	.4byte 0x00002ce5
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002ea8
	.4byte 0x00000000
	.4byte 0x0a3d0009
	.4byte 0x00002cde
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002ea1
	.4byte 0x00008d15
	.4byte 0x0a3d0009
	.4byte 0x00002ce6
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002ea9
	.4byte 0x00000000
	.4byte 0x0a3d000a
	.4byte 0x00002cdf
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002ea2
	.4byte 0x00008d15
	.4byte 0x0a3d000a
	.4byte 0x00002ce7
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002eaa
	.4byte 0x00000000
	.4byte 0x0a3d000b
	.4byte 0x00002ce0
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002ea3
	.4byte 0x00008d15
	.4byte 0x0a3d000b
	.4byte 0x00002ce8
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002eab
	.4byte 0x00000000
	.4byte 0x0a3d000c
	.4byte 0x00002ce1
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002ea4
	.4byte 0x00008d15
	.4byte 0x0a3d000c
	.4byte 0x00002ce9
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002eac
	.4byte 0x00000000
	.4byte 0x0a3d000d
	.4byte 0x00002ce2
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002ea5
	.4byte 0x00008d15
	.4byte 0x0a3d000d
	.4byte 0x00002cea
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002ead
	.4byte 0x00000000
	.4byte 0x0a3d000e
	.4byte 0x00002ce3
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002ea6
	.4byte 0x00008d15
	.4byte 0x0a3d000e
	.4byte 0x00002ceb
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002eae
	.4byte 0x00000000
	.4byte 0x0a3d000f
	.4byte 0x00002ce4
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002ea7
	.4byte 0x00008d15
	.4byte 0x0a3d000f
	.4byte 0x00002cec
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002eaf
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008765
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x02008765
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x020087f5
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x020087f5
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008885
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x02008885
	.4byte 0x50008805
	.4byte 0x03000064
	.4byte 0x02008589
	.4byte 0x00008f15
	.4byte 0xffff0013
	.4byte 0x00000000
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
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02009b61
	.4byte 0x00000002
	.4byte 0x0a2d0009
	.4byte 0x020095d1
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x0200b91d
	.4byte 0x10008805
	.4byte 0xffff00ff
	.4byte 0x020097e5
	.4byte 0x00008805
	.4byte 0xffff00ff
	.4byte 0x02009801
	.4byte 0x50008805
	.4byte 0xffff0008
	.4byte 0x02009771
	.4byte 0x00000006
	.4byte 0x0a3d00c8
	.4byte 0x02008281
	.4byte 0x00000002
	.4byte 0x09c0000b
	.4byte 0x02008915
	.4byte 0x00000002
	.4byte 0x09c1000c
	.4byte 0x02008ad9
	.4byte 0x00000002
	.4byte 0x09c2000d
	.4byte 0x02008dad
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002d15
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002d17
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002d16
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002d18
	.4byte 0x00000000
	.4byte 0x0a3d000c
	.4byte 0x00002d33
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002edc
	.4byte 0x00008d15
	.4byte 0x0a3d000c
	.4byte 0x00002d39
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002ee2
	.4byte 0x00000000
	.4byte 0x0a3d000d
	.4byte 0x00002d34
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002edd
	.4byte 0x00008d15
	.4byte 0x0a3d000d
	.4byte 0x00002d3a
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002ee3
	.4byte 0x00000000
	.4byte 0x0a3d000e
	.4byte 0x00002d35
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002ede
	.4byte 0x00008d15
	.4byte 0x0a3d000e
	.4byte 0x00002d3b
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002ee4
	.4byte 0x00000000
	.4byte 0x0a3d000f
	.4byte 0x00002d36
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002edf
	.4byte 0x00008d15
	.4byte 0x0a3d000f
	.4byte 0x00002d3c
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002ee5
	.4byte 0x00000000
	.4byte 0x0a3d0010
	.4byte 0x00002d37
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002ee0
	.4byte 0x00008d15
	.4byte 0x0a3d0010
	.4byte 0x00002d3d
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002ee6
	.4byte 0x00000000
	.4byte 0x0a3d0011
	.4byte 0x00002d38
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002ee1
	.4byte 0x00008d15
	.4byte 0x0a3d0011
	.4byte 0x00002d3e
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002ee7
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002d66
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002d68
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002d67
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002d69
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
	.4byte 0x00000000
	.4byte 0x0a3d0008
	.4byte 0x00002ced
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002eb0
	.4byte 0x00008d15
	.4byte 0x0a3d0008
	.4byte 0x00002cf0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002eb3
	.4byte 0x00000000
	.4byte 0x0a3d0009
	.4byte 0x00002cee
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002eb1
	.4byte 0x00008d15
	.4byte 0x0a3d0009
	.4byte 0x00002cf1
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002eb4
	.4byte 0x00000000
	.4byte 0x0a3d000a
	.4byte 0x00002cef
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002eb2
	.4byte 0x00008d15
	.4byte 0x0a3d000a
	.4byte 0x00002cf2
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002eb5
	.4byte 0x00000000
	.4byte 0x0a3d000b
	.4byte 0x00002cf4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002eb7
	.4byte 0x00008d15
	.4byte 0x0a3d000b
	.4byte 0x00002cf7
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002eba
	.4byte 0x00000000
	.4byte 0x0a3d000c
	.4byte 0x00002cf5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002eb8
	.4byte 0x00008d15
	.4byte 0x0a3d000c
	.4byte 0x00002cf8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002ebb
	.4byte 0x00000000
	.4byte 0x0a3d000d
	.4byte 0x00002cf9
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002ebc
	.4byte 0x00008d15
	.4byte 0x0a3d000d
	.4byte 0x00002cfb
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002ebe
	.4byte 0x00000000
	.4byte 0x0a3d000e
	.4byte 0x00002cfa
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002ebd
	.4byte 0x00008d15
	.4byte 0x0a3d000e
	.4byte 0x00002cfc
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002ebf
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008621
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002d03
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002d00
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002d04
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002d01
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002d05
	.4byte 0x00000000
	.4byte 0x0a3d0012
	.4byte 0x00002d02
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002ec3
	.4byte 0x00008d15
	.4byte 0x0a3d0012
	.4byte 0x00002d06
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002ec7
	.4byte 0x00000000
	.4byte 0x0a3d0013
	.4byte 0x00002d07
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002eca
	.4byte 0x00008d15
	.4byte 0x0a3d0013
	.4byte 0x00002d0a
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002ecf
	.4byte 0x00000000
	.4byte 0x0a3d0014
	.4byte 0x00002d08
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002ecb
	.4byte 0x00008d15
	.4byte 0x0a3d0014
	.4byte 0x00002d0b
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002ed0
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002d09
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002d0c
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00002ec8
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00002ecd
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00002ec9
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00002ece
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008675
	.4byte 0x00008d15
	.4byte 0x0a3d0016
	.4byte 0x00002d1f
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002ed8
	.4byte 0x00000000
	.4byte 0x0a3d0017
	.4byte 0x00002d1e
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002ed7
	.4byte 0x00008d15
	.4byte 0x0a3d0017
	.4byte 0x00002d20
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002ed9
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008291
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x0040305f
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403060
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x00403061
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x00403062
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ca02
	.4byte 0xffff0006
	.4byte 0x02009b7d
	.4byte 0x0000ca02
	.4byte 0xffff0007
	.4byte 0x02009b7d
	.4byte 0x0000ca02
	.4byte 0xffff0008
	.4byte 0x02009b7d
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x0a3d0008
	.4byte 0x00002cf3
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002eb6
	.4byte 0x00008d15
	.4byte 0x0a3d0008
	.4byte 0x00002cf6
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002eb9
	.4byte 0x00000000
	.4byte 0x0a3d0009
	.4byte 0x00002d19
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002ed2
	.4byte 0x00008d15
	.4byte 0x0a3d0009
	.4byte 0x00002d1b
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002ed4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020086c5
	.4byte 0x00008d15
	.4byte 0x0a3d000a
	.4byte 0x00002d1c
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002ed5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002ecc
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002ed1
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x0040305f
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403060
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x00403061
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x00403062
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008715
	.4byte 0x00008d15
	.4byte 0x0a3d0008
	.4byte 0x00002d22
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002edb
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x008c0000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
