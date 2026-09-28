.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200aec5, 0x020082ed, 0x02008341, 0x02008349, 0x020083f5, 0x020082f5, 0x0200b895
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
	bl 0x0200b9e8
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
	bl 0x0200b978
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
	bl 0x0200b968
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200b970
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200b9b0
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
	bl 0x0200ba68
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
	bl 0x0200b918
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
	bl 0x0200b918
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_0200020c:
	bl 0x0200b918
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
	bl 0x0200b968
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200b970
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
	.4byte 0x0200bc04
	.4byte 0x02008081
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb560
	ldr	r3, [pc, #100]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	adds	r6, r0, #0
	movs	r0, #14
	bl 0x0200b9e8
	adds	r5, r0, #0
	adds	r5, #98
	ldrb	r3, [r5, #0]
	movs	r2, #128
	adds	r3, #252
	lsls	r3, r3, #24
	lsls	r2, r2, #17
	cmp	r3, r2
	bhi.n	.L_020002e4
	ldr	r3, [r6, #12]
	movs	r2, #192
	lsls	r2, r2, #12
	cmp	r3, r2
	ble.n	.L_020002e4
	movs	r0, #208
	lsls	r0, r0, #2
	bl 0x0200b958
	ldrb	r0, [r5, #0]
	cmp	r0, #4
	bne.n	.L_020002ce
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #65
	bl 0x0200b958
	b.n	.L_020002dc
.L_020002ce:
	cmp	r0, #5
	bne.n	.L_020002dc
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #66
	bl 0x0200b958
.L_020002dc:
	movs	r0, #14
	movs	r1, #67
	bl 0x0200bae0
.L_020002e4:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xbdc4
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
	bne.n	.L_0200030c
	ldr	r0, [pc, #32]
	b.n	.L_02000320
.L_0200030c:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_02000316
	ldr	r0, [pc, #32]
	b.n	.L_02000320
.L_02000316:
	ldr	r3, [pc, #32]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_02000320
	ldr	r0, [pc, #28]
.L_02000320:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001f
	.4byte 0x0200bdf4
	.4byte 0x00000020
	.4byte 0x0200be14
	.4byte 0x00000021
	.2byte 0xbe44
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xbe64
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #96]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_02000360
	ldr	r0, [pc, #84]
	b.n	.L_020003a8
.L_02000360:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_0200036a
	ldr	r0, [pc, #84]
	b.n	.L_020003a8
.L_0200036a:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02000374
	ldr	r0, [pc, #80]
	b.n	.L_020003a8
.L_02000374:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_0200037e
	ldr	r0, [pc, #80]
	b.n	.L_020003a8
.L_0200037e:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02000388
	ldr	r0, [pc, #76]
	b.n	.L_020003a8
.L_02000388:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02000392
	ldr	r0, [pc, #76]
	b.n	.L_020003a8
.L_02000392:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_0200039c
	ldr	r0, [pc, #72]
	b.n	.L_020003a8
.L_0200039c:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020003a6
	ldr	r0, [pc, #72]
	b.n	.L_020003a8
.L_020003a6:
	ldr	r0, [pc, #72]
.L_020003a8:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000017
	.4byte 0x0200bf20
	.4byte 0x00000018
	.4byte 0x0200bfe0
	.4byte 0x00000019
	.4byte 0x0200c058
	.4byte 0x0000001a
	.4byte 0x0200c130
	.4byte 0x0000001b
	.4byte 0x0200c2c8
	.4byte 0x0000001c
	.4byte 0x0200c388
	.4byte 0x0000001d
	.4byte 0x0200c400
	.4byte 0x0000001e
	.4byte 0x0200c5f8
	.2byte 0xbf08
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #104]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #96]
	cmp	r2, r3
	bne.n	.L_0200040c
	ldr	r0, [pc, #92]
	b.n	.L_0200045e
.L_0200040c:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02000416
	ldr	r0, [pc, #92]
	b.n	.L_0200045e
.L_02000416:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02000420
	ldr	r0, [pc, #88]
	b.n	.L_0200045e
.L_02000420:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_0200042a
	ldr	r0, [pc, #88]
	b.n	.L_0200045e
.L_0200042a:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_02000434
	ldr	r0, [pc, #84]
	b.n	.L_0200045e
.L_02000434:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_0200043e
	ldr	r0, [pc, #84]
	b.n	.L_0200045e
.L_0200043e:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02000448
	ldr	r0, [pc, #80]
	b.n	.L_0200045e
.L_02000448:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02000452
	ldr	r0, [pc, #80]
	b.n	.L_0200045e
.L_02000452:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_0200045c
	ldr	r0, [pc, #76]
	b.n	.L_0200045e
.L_0200045c:
	ldr	r0, [pc, #76]
.L_0200045e:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000017
	.4byte 0x0200c664
	.4byte 0x00000018
	.4byte 0x0200c6c4
	.4byte 0x00000019
	.4byte 0x0200c6f4
	.4byte 0x0000001a
	.4byte 0x0200c790
	.4byte 0x0000001b
	.4byte 0x0200c7cc
	.4byte 0x0000001c
	.4byte 0x0200c8ec
	.4byte 0x0000001d
	.4byte 0x0200c928
	.4byte 0x0000001e
	.4byte 0x0200c9a0
	.4byte 0x0000001f
	.4byte 0x0200c9f4
	.2byte 0xc658
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	ldr	r0, [pc, #24]
	movs	r1, #1
	bl 0x0200b9c8
	movs	r3, #173
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	bl 0x0200b9e0
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x17e2
	.2byte 0x0000
	push	{lr}
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	movs	r0, #11
	bl 0x0200ba00
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #11
	bl 0x0200baa8
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #11
	bl 0x0200ba60
	ldr	r0, [pc, #56]
	bl 0x0200ba70
	movs	r0, #11
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #11
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #11
	movs	r1, #4
	bl 0x0200ba40
	movs	r2, #20
	movs	r0, #11
	movs	r1, #0
	bl 0x0200ba78
	ldr	r1, [pc, #20]
	movs	r0, #11
	bl 0x0200b9f8
	bl 0x0200b9e0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000017c3
	.4byte 0x0200bc10
	.4byte 0x049b23c0
	.4byte 0x21ad6edb
	.4byte 0x185a0049
	.4byte 0x80132301
	.2byte 0x4770
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
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #160
	adds	r5, r3, r1
	lsls	r2, r2, #1
	adds	r1, #112
	adds	r7, r3, r1
	adds	r2, r2, r3
	ldr	r3, [pc, #128]
	mov	r8, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	adds	r6, r0, #0
	bl 0x0200bad0
	mov	sl, r0
	movs	r0, #8
	bl 0x0200b9e8
	mov	r9, r0
	movs	r0, #9
	bl 0x0200b9e8
	mov	fp, r0
	movs	r0, #10
	bl 0x0200b9e8
	ldr	r3, [r5, #12]
	ldr	r1, [pc, #84]
	mov	r2, r8
	adds	r3, r3, r1
	str	r3, [r5, #12]
	mov	r1, r8
	ldr	r3, [r2, #12]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r3, [r7, #12]
	mov	r1, sl
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r2, [pc, #56]
	ldr	r3, [r6, #16]
	adds	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r1, #12]
	adds	r3, r3, r2
	str	r3, [r1, #12]
	mov	r2, r9
	ldr	r3, [r2, #16]
	ldr	r1, [pc, #40]
	adds	r3, r3, r1
	str	r3, [r2, #16]
	mov	r2, fp
	ldr	r3, [r2, #16]
	adds	r3, r3, r1
	str	r3, [r2, #16]
	ldr	r3, [r0, #16]
	adds	r3, r3, r1
	str	r3, [r0, #16]
	movs	r0, #4
	bl 0x0200b920
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x8000
	.2byte 0xffff
	.2byte 0xb500
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	movs	r1, #132
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldr	r3, [r2, #12]
	ldr	r1, [pc, #12]
	movs	r0, #4
	adds	r3, r3, r1
	str	r3, [r2, #12]
	bl 0x0200b920
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #132
	lsls	r2, r2, #1
	adds	r7, r3, r2
	adds	r2, #56
	sub	sp, #8
	adds	r6, r3, r2
	movs	r3, #18
	movs	r2, #6
	str	r3, [sp, #0]
	movs	r3, #56
	str	r2, [sp, #4]
	bl 0x0200b998
	movs	r5, #0
.L_02000660:
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #32]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	bl 0x0200b920
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	cmp	r5, #15
	bls.n	.L_02000660
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xe000
	.2byte 0xffff
	.2byte 0xb560
	sub	sp, #8
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	bl 0x0200b9a0
	ldr	r6, [pc, #476]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r6, r2
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200ba88
	movs	r0, #248
	movs	r1, #1
	movs	r2, #168
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200bac0
	bl 0x0200bac8
	movs	r0, #244
	bl 0x0200bb48
	movs	r3, #128
	movs	r5, #128
	lsls	r3, r3, #5
	lsls	r5, r5, #19
	adds	r3, #1
	adds	r5, #82
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200b920
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #2
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200b920
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #3
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200b920
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #4
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200b920
	movs	r1, #128
	movs	r2, #128
	movs	r0, #0
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	bl 0x0200b9b8
	movs	r2, #3
	str	r2, [sp, #4]
	movs	r3, #12
	movs	r2, #9
	movs	r1, #124
	movs	r0, #59
	str	r3, [sp, #0]
	bl 0x0200b998
	ldr	r0, [r6, #0]
	bl 0x0200b9e8
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r6, #0]
	bl 0x0200ba98
	movs	r0, #8
	movs	r1, #1
	bl 0x0200ba98
	movs	r0, #9
	movs	r1, #1
	bl 0x0200ba98
	movs	r0, #10
	movs	r1, #1
	bl 0x0200ba98
	movs	r3, #18
	movs	r2, #25
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #56
	movs	r1, #51
	movs	r2, #6
	movs	r0, #46
	bl 0x0200b998
	movs	r0, #10
	bl 0x0200b920
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	bl 0x0200b9b8
	movs	r0, #65
	movs	r1, #51
	bl 0x0200863c
	movs	r0, #65
	movs	r1, #58
	bl 0x0200863c
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #5
	strh	r3, [r5, #0]
	movs	r0, #65
	movs	r1, #65
	bl 0x0200863c
	movs	r0, #65
	movs	r1, #72
	bl 0x0200863c
	movs	r3, #224
	lsls	r3, r3, #4
	adds	r3, #6
	strh	r3, [r5, #0]
	movs	r0, #65
	movs	r1, #79
	bl 0x0200863c
	movs	r0, #65
	movs	r1, #86
	bl 0x0200863c
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #7
	strh	r3, [r5, #0]
	movs	r0, #65
	movs	r1, #93
	bl 0x0200863c
	movs	r0, #65
	movs	r1, #100
	bl 0x0200863c
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #8
	strh	r3, [r5, #0]
	movs	r0, #84
	movs	r1, #51
	bl 0x0200863c
	movs	r0, #84
	movs	r1, #58
	bl 0x0200863c
	movs	r3, #176
	lsls	r3, r3, #4
	adds	r3, #9
	strh	r3, [r5, #0]
	movs	r0, #84
	movs	r1, #65
	bl 0x0200863c
	movs	r0, #84
	movs	r1, #72
	bl 0x0200863c
	movs	r3, #160
	lsls	r3, r3, #4
	adds	r3, #10
	strh	r3, [r5, #0]
	movs	r0, #84
	movs	r1, #79
	bl 0x0200863c
	movs	r0, #84
	movs	r1, #86
	bl 0x0200863c
	movs	r3, #144
	lsls	r3, r3, #4
	adds	r3, #11
	strh	r3, [r5, #0]
	movs	r0, #84
	movs	r1, #93
	bl 0x0200863c
	movs	r0, #84
	movs	r1, #100
	bl 0x0200863c
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #12
	strh	r3, [r5, #0]
	movs	r5, #0
.L_02000840:
	adds	r5, #1
	bl 0x02008618
	cmp	r5, #23
	bls.n	.L_02000840
	movs	r5, #0
.L_0200084c:
	adds	r5, #1
	bl 0x02008568
	cmp	r5, #23
	bls.n	.L_0200084c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200baf0
	bl 0x0200baf8
	movs	r0, #3
	bl 0x0200bad8
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0240
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
	ldr	r3, [r3, #32]
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #160
	adds	r5, r3, r1
	lsls	r2, r2, #1
	adds	r1, #112
	adds	r7, r3, r1
	adds	r2, r2, r3
	ldr	r3, [pc, #128]
	mov	r8, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	adds	r6, r0, #0
	bl 0x0200bad0
	mov	sl, r0
	movs	r0, #8
	bl 0x0200b9e8
	mov	r9, r0
	movs	r0, #9
	bl 0x0200b9e8
	mov	fp, r0
	movs	r0, #10
	bl 0x0200b9e8
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r5, #12]
	mov	r1, r8
	ldr	r3, [r1, #12]
	ldr	r1, [pc, #72]
	adds	r3, r3, r1
	mov	r1, r8
	str	r3, [r1, #12]
	ldr	r3, [r7, #12]
	ldr	r1, [pc, #64]
	adds	r3, r3, r1
	str	r3, [r7, #12]
	mov	r1, sl
	ldr	r3, [r6, #16]
	adds	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r1, #12]
	adds	r3, r3, r2
	str	r3, [r1, #12]
	mov	r1, r9
	ldr	r3, [r1, #16]
	adds	r3, r3, r2
	str	r3, [r1, #16]
	mov	r1, fp
	ldr	r3, [r1, #16]
	adds	r3, r3, r2
	str	r3, [r1, #16]
	ldr	r3, [r0, #16]
	adds	r3, r3, r2
	str	r3, [r0, #16]
	movs	r0, #4
	bl 0x0200b920
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x8000
	.2byte 0xffff
	.2byte 0xb500
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	movs	r1, #132
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldr	r3, [r2, #12]
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r2, #12]
	movs	r0, #4
	bl 0x0200b920
	pop	{pc}
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #132
	lsls	r2, r2, #1
	adds	r7, r3, r2
	adds	r2, #56
	sub	sp, #8
	adds	r6, r3, r2
	movs	r3, #18
	movs	r2, #6
	str	r3, [sp, #0]
	movs	r3, #56
	str	r2, [sp, #4]
	bl 0x0200b998
	movs	r5, #0
.L_02000970:
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	str	r3, [r7, #12]
	movs	r0, #1
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	bl 0x0200b920
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	cmp	r5, #15
	bls.n	.L_02000970
	add	sp, #8
	pop	{r5, r6, r7, pc}
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
	movs	r2, #132
	lsls	r2, r2, #1
	adds	r6, r3, r2
	adds	r2, #56
	adds	r2, r2, r3
	mov	r8, r2
	movs	r2, #188
	lsls	r2, r2, #1
	sub	sp, #16
	adds	r2, r3, r2
	ldr	r5, [pc, #792]
	str	r2, [sp, #12]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200b9e8
	adds	r7, r0, #0
	bl 0x0200bad0
	mov	sl, r0
	movs	r0, #8
	bl 0x0200b9e8
	mov	r9, r0
	movs	r0, #9
	bl 0x0200b9e8
	mov	fp, r0
	movs	r0, #10
	bl 0x0200b9e8
	str	r0, [sp, #8]
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	negs	r1, r1
	movs	r3, #0
	negs	r0, r0
	bl 0x0200bac0
	ldr	r0, [r5, #0]
	bl 0x0200b9e8
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200ba98
	movs	r0, #8
	movs	r1, #1
	bl 0x0200ba98
	movs	r0, #9
	movs	r1, #1
	bl 0x0200ba98
	movs	r1, #1
	movs	r0, #10
	bl 0x0200ba98
	bl 0x0200b9a0
	movs	r0, #1
	bl 0x0200b920
	movs	r3, #25
	str	r3, [sp, #4]
	movs	r5, #18
	movs	r0, #46
	movs	r1, #51
	movs	r2, #6
	movs	r3, #56
	str	r5, [sp, #0]
	bl 0x0200b998
	movs	r3, #6
	str	r3, [sp, #4]
	movs	r0, #84
	movs	r1, #100
	movs	r2, #6
	movs	r3, #56
	str	r5, [sp, #0]
	bl 0x0200b998
	movs	r2, #3
	movs	r3, #12
	str	r2, [sp, #4]
	movs	r0, #59
	movs	r1, #124
	movs	r2, #9
	str	r3, [sp, #0]
	bl 0x0200b998
	movs	r2, #128
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #19
	adds	r2, #12
	adds	r3, #82
	strh	r2, [r3, #0]
	movs	r0, #1
	bl 0x0200b920
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #588]
	movs	r5, #0
	adds	r3, r3, r2
	str	r3, [r6, #12]
	mov	r2, r8
	ldr	r3, [r2, #12]
	ldr	r2, [pc, #580]
	adds	r3, r3, r2
	mov	r2, r8
	str	r3, [r2, #12]
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #12]
	movs	r2, #192
	lsls	r2, r2, #12
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	str	r3, [r2, #12]
	ldr	r3, [r7, #16]
	ldr	r2, [pc, #560]
	adds	r3, r3, r2
	str	r3, [r7, #16]
	mov	r2, sl
	ldr	r3, [r2, #12]
	ldr	r2, [pc, #552]
	adds	r3, r3, r2
	mov	r2, sl
	str	r3, [r2, #12]
	mov	r2, r9
	ldr	r3, [r2, #16]
	ldr	r2, [pc, #540]
	adds	r3, r3, r2
	mov	r2, r9
	str	r3, [r2, #16]
	mov	r2, fp
	ldr	r3, [r2, #16]
	ldr	r2, [pc, #528]
	adds	r3, r3, r2
	mov	r2, fp
	str	r3, [r2, #16]
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #16]
	ldr	r2, [pc, #516]
	adds	r3, r3, r2
	ldr	r2, [sp, #8]
	str	r3, [r2, #16]
	bl 0x0200b988
	movs	r0, #4
	bl 0x0200b920
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200bae8
	bl 0x0200baf8
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	bl 0x0200b9b8
	movs	r0, #245
	bl 0x0200bb48
.L_02000b1e:
	adds	r5, #1
	bl 0x0200887c
	cmp	r5, #24
	bls.n	.L_02000b1e
	movs	r5, #0
.L_02000b2a:
	adds	r5, #1
	bl 0x0200892c
	cmp	r5, #24
	bls.n	.L_02000b2a
	movs	r0, #84
	movs	r1, #93
	bl 0x0200894c
	movs	r0, #84
	movs	r1, #86
	bl 0x0200894c
	movs	r3, #144
	movs	r5, #128
	lsls	r3, r3, #4
	lsls	r5, r5, #19
	adds	r3, #11
	adds	r5, #82
	strh	r3, [r5, #0]
	movs	r0, #84
	movs	r1, #79
	bl 0x0200894c
	movs	r0, #84
	movs	r1, #72
	bl 0x0200894c
	movs	r3, #160
	lsls	r3, r3, #4
	adds	r3, #10
	strh	r3, [r5, #0]
	movs	r0, #84
	movs	r1, #65
	bl 0x0200894c
	movs	r0, #84
	movs	r1, #58
	bl 0x0200894c
	movs	r3, #176
	lsls	r3, r3, #4
	adds	r3, #9
	strh	r3, [r5, #0]
	movs	r0, #84
	movs	r1, #51
	bl 0x0200894c
	movs	r0, #65
	movs	r1, #100
	bl 0x0200894c
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #8
	strh	r3, [r5, #0]
	movs	r0, #65
	movs	r1, #93
	bl 0x0200894c
	movs	r0, #65
	movs	r1, #86
	bl 0x0200894c
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #7
	strh	r3, [r5, #0]
	movs	r0, #65
	movs	r1, #79
	bl 0x0200894c
	movs	r0, #65
	movs	r1, #72
	bl 0x0200894c
	movs	r3, #224
	lsls	r3, r3, #4
	adds	r3, #6
	strh	r3, [r5, #0]
	movs	r0, #65
	movs	r1, #65
	bl 0x0200894c
	movs	r0, #65
	movs	r1, #58
	bl 0x0200894c
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #5
	strh	r3, [r5, #0]
	movs	r0, #65
	movs	r1, #51
	bl 0x0200894c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #0
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	bl 0x0200b9b8
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #4
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200b920
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200bb48
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200b9b8
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #3
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200b920
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #2
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200b920
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #1
	strh	r3, [r5, #0]
	movs	r0, #10
	bl 0x0200b920
	movs	r3, #128
	lsls	r3, r3, #5
	strh	r3, [r5, #0]
	movs	r3, #18
	movs	r2, #25
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #103
	movs	r1, #51
	movs	r2, #6
	movs	r3, #56
	bl 0x0200b998
	movs	r5, #3
	movs	r1, #108
	movs	r3, #12
	movs	r2, #9
	movs	r0, #46
	str	r3, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200b998
	movs	r0, #10
	bl 0x0200b920
	bl 0x0200b9c0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	movs	r0, #8
	bl 0x0200b854
	movs	r0, #9
	bl 0x0200b854
	movs	r0, #10
	bl 0x0200b854
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	adds	r3, r7, #0
	movs	r1, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	str	r1, [r7, #12]
	str	r1, [r7, #20]
	bl 0x0200b9e0
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xffbc0000
	.4byte 0xffec0000
	.2byte 0x0000
	.2byte 0xfff4
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #32]
	mov	r8, r2
	movs	r2, #160
	lsls	r2, r2, #1
	ldr	r5, [pc, #308]
	adds	r6, r3, r2
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	sub	sp, #8
	bl 0x0200b9e8
	adds	r7, r0, #0
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	movs	r0, #244
	bl 0x0200bb48
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	bl 0x0200bac0
	movs	r0, #172
	movs	r1, #1
	movs	r2, #248
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #0
	bl 0x0200bac0
	movs	r3, #19
	movs	r2, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #18
	movs	r2, #62
	movs	r1, #77
	movs	r0, #62
	bl 0x0200b998
	movs	r0, #1
	bl 0x0200b920
	movs	r1, #2
	ldr	r0, [r5, #0]
	bl 0x0200ba98
	adds	r2, r7, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r2, [pc, #196]
	movs	r5, #0
	str	r2, [r7, #12]
	str	r2, [r7, #20]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	bl 0x0200b988
	movs	r0, #1
	bl 0x0200b920
	mov	r2, r8
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #87
	str	r2, [r3, #0]
	bl 0x0200bae8
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	bl 0x0200b9b8
.L_02000da6:
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r3, [r7, #20]
	movs	r0, #2
	adds	r3, r3, r2
	str	r3, [r7, #20]
.L_02000db8:
	adds	r5, #1
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
.L_02000dc0:
	bl 0x0200b920
	cmp	r5, #99
	bls.n	.L_02000da6
.L_02000dc8:
	movs	r0, #144
	lsls	r0, r0, #1
	bl 0x0200bb48
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200bb48
.L_02000dd8:
	movs	r1, #0
	movs	r0, #0
	movs	r2, #0
	bl 0x0200b9b8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
.L_02000df0:
	str	r2, [r3, #0]
	ldr	r3, [pc, #60]
	adds	r2, #11
	adds	r3, r3, r2
.L_02000df8:
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_02000e08:
	movs	r2, #6
	movs	r3, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
.L_02000e10:
	movs	r1, #69
	movs	r2, #62
	movs	r3, #18
	movs	r0, #62
	bl 0x0200b998
	movs	r0, #1
	bl 0x0200b920
	bl 0x0200b9e0
	add	sp, #8
.L_02000e28:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xffce
.L_02000e38:
	.2byte 0xb5e0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
.L_02000e40:
	movs	r2, #160
	lsls	r2, r2, #1
	ldr	r5, [pc, #200]
	adds	r7, r3, r2
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
.L_02000e50:
	sub	sp, #8
	bl 0x0200b9e8
	adds	r6, r0, #0
.L_02000e58:
	bl 0x0200b9d8
.L_02000e5c:
	movs	r0, #0
	bl 0x0200bb08
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
.L_02000e68:
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
.L_02000e70:
	bl 0x0200bac0
	movs	r0, #172
	movs	r1, #1
	movs	r2, #248
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200bac0
	movs	r3, #19
	movs	r2, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #18
	movs	r0, #62
	movs	r1, #77
	movs	r2, #62
	bl 0x0200b998
	movs	r1, #128
	ldr	r0, [r5, #0]
	movs	r2, #20
	lsls	r1, r1, #7
	bl 0x0200ba88
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200ba98
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #245
	bl 0x0200bb48
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	bl 0x0200b9b8
	movs	r5, #0
.L_02000ece:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #64]
	movs	r0, #2
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #20]
	adds	r3, r3, r2
	str	r3, [r6, #20]
	ldr	r3, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #12]
	bl 0x0200b920
	cmp	r5, #90
	bne.n	.L_02000f00
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #87
	str	r2, [r3, #0]
	bl 0x0200baf0
.L_02000f00:
	adds	r5, #1
	cmp	r5, #99
	bls.n	.L_02000ece
	movs	r0, #1
	bl 0x0200bad8
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x8000
	.2byte 0xffff
	.2byte 0xb520
	adds	r5, r0, #0
	bl 0x0200baa0
	adds	r0, r5, #0
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #8
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #0
	bl 0x0200b9a8
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200b958
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #10
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #0
	bl 0x0200b9a8
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b958
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #10
	movs	r2, #17
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #0
	bl 0x0200b9a8
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200b958
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200bb38
	pop	{pc}
	.2byte 0x0000
	.2byte 0xbcbc
	.2byte 0x0200
	push	{lr}
	bl 0x0200bb40
	movs	r0, #8
	bl 0x0200b9e8
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #21
	bne.n	.L_02000fe0
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #13
	bne.n	.L_02000fe0
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b958
	b.n	.L_02000fea
.L_02000fe0:
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b960
.L_02000fea:
	pop	{pc}
	push	{r5, lr}
	movs	r0, #8
	bl 0x0200b9e8
	adds	r5, r0, #0
	bl 0x0200bb40
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #12
	bne.n	.L_0200100c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #76
	bl 0x0200b958
.L_0200100c:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	adds	r7, r6, #0
	adds	r7, #85
	movs	r3, #3
	strb	r3, [r7, #0]
.L_0200101c:
	movs	r0, #1
	bl 0x0200b920
	ldr	r5, [r6, #40]
	cmp	r5, #0
	bne.n	.L_0200101c
	movs	r0, #188
	bl 0x0200bb48
	movs	r0, #10
	bl 0x0200b920
	strb	r5, [r7, #0]
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r0, #8
	sub	sp, #8
	bl 0x0200b9e8
	adds	r5, r0, #0
	ldr	r3, [r5, #16]
	asrs	r6, r3, #20
	cmp	r6, #16
	bne.n	.L_02001084
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	adds	r0, r5, #0
	bl 0x02009010
	movs	r3, #15
	str	r3, [sp, #0]
	movs	r2, #1
	movs	r3, #1
	movs	r0, #0
	movs	r1, #0
	str	r6, [sp, #4]
	bl 0x0200b9a8
	movs	r0, #8
	movs	r1, #3
	bl 0x0200ba98
	bl 0x0200b9e0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200b958
.L_02001084:
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #84]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	movs	r6, #9
	mov	r8, r0
.L_020010a0:
	adds	r0, r6, #0
	bl 0x0200b9e8
	mov	r2, r8
	ldr	r3, [r2, #12]
	movs	r2, #128
	adds	r5, r0, #0
	lsls	r2, r2, #13
	adds	r5, #35
	adds	r7, r6, #1
	cmp	r3, r2
	ble.n	.L_020010c8
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200ba98
	ldrb	r2, [r5, #0]
	movs	r3, #2
	orrs	r3, r2
	b.n	.L_020010d6
.L_020010c8:
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200ba98
	ldrb	r2, [r5, #0]
	movs	r3, #253
	ands	r3, r2
.L_020010d6:
	strb	r3, [r5, #0]
	adds	r6, r7, #0
	cmp	r6, #13
	bls.n	.L_020010a0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r6, [pc, #56]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	adds	r5, r0, #0
	bl 0x0200bb28
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #13
	cmp	r3, r2
	bge.n	.L_02001114
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r6, r3
	movs	r3, #0
	b.n	.L_0200111e
.L_02001114:
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r6, r3
	movs	r3, #1
.L_0200111e:
	strb	r3, [r2, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #112]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #13
	cmp	r3, r2
	bge.n	.L_02001180
	ldr	r3, [r0, #16]
	movs	r2, #148
	lsls	r2, r2, #17
	cmp	r3, r2
	bge.n	.L_02001166
	movs	r0, #10
	movs	r1, #1
	bl 0x0200ba98
	movs	r0, #11
	movs	r1, #1
	bl 0x0200ba98
	movs	r0, #12
	movs	r1, #2
	bl 0x0200ba98
	b.n	.L_02001198
.L_02001166:
	movs	r0, #10
	movs	r1, #2
	bl 0x0200ba98
	movs	r0, #11
	movs	r1, #2
	bl 0x0200ba98
	movs	r0, #12
	movs	r1, #3
	bl 0x0200ba98
	b.n	.L_02001198
.L_02001180:
	movs	r0, #10
	movs	r1, #2
	bl 0x0200ba98
	movs	r0, #11
	movs	r1, #2
	bl 0x0200ba98
	movs	r0, #12
	movs	r1, #3
	bl 0x0200ba98
.L_02001198:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #8
	bl 0x0200b9e8
	adds	r5, r0, #0
	movs	r0, #9
	bl 0x0200b9e8
	ldr	r3, [r5, #8]
	str	r3, [r0, #8]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r0, #12]
	ldr	r3, [r5, #16]
	str	r3, [r0, #16]
	pop	{r5, pc}
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #223
	ands	r3, r2
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #20]
	cmp	r2, r3
	bne.n	.L_0200121c
	movs	r1, #9
	movs	r2, #3
	bl 0x02009250
.L_0200121c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x001c
	.2byte 0x0000
	push	{lr}
	movs	r0, #1
	bl 0x02009200
	movs	r0, #130
	lsls	r0, r0, #2
	bl 0x0200b958
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #0
	bl 0x02009200
	movs	r0, #130
	lsls	r0, r0, #2
	bl 0x0200b960
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r2, #0
	movs	r5, #0
	mov	r8, r0
	adds	r7, r1, #0
	cmp	r5, r6
	bcs.n	.L_02001288
.L_02001262:
	adds	r0, r7, r5
	bl 0x0200b9e8
	mov	r3, r8
	adds	r0, #35
	adds	r1, r5, #1
	cmp	r3, #0
	beq.n	.L_0200127a
	ldrb	r2, [r0, #0]
	movs	r3, #239
	ands	r3, r2
	b.n	.L_02001280
.L_0200127a:
	ldrb	r2, [r0, #0]
	movs	r3, #16
	orrs	r3, r2
.L_02001280:
	strb	r3, [r0, #0]
	adds	r5, r1, #0
	cmp	r5, r6
	bcc.n	.L_02001262
.L_02001288:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200ba98
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #10
	bl 0x0200b958
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #44]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200ba98
	ldr	r0, [r5, #0]
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #10
	bl 0x0200b960
	pop	{r5, pc}
	.2byte 0x0000
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
	bl 0x0200b918
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02001318
	adds	r3, #15
.L_02001318:
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
	ldr	r3, [pc, #212]
	mov	r8, r0
	ldr	r7, [r0, #80]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	adds	r6, r1, #0
	movs	r1, #129
	ldr	r0, [r3, #0]
	lsls	r1, r1, #1
	bl 0x0200bab0
	movs	r0, #210
	bl 0x0200bb48
	mov	r2, r8
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	mov	sl, r3
.L_02001370:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #176]
	movs	r5, #0
	adds	r3, r3, r2
	str	r3, [r6, #12]
.L_0200137a:
	ldrh	r3, [r7, #18]
	movs	r0, #1
	adds	r3, #128
	strh	r3, [r7, #18]
	adds	r5, #1
	bl 0x0200b920
	cmp	r5, #3
	bls.n	.L_0200137a
	ldr	r3, [r6, #12]
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r3, r3, r0
	str	r3, [r6, #12]
	movs	r5, #0
.L_02001398:
	ldrh	r3, [r7, #18]
	movs	r2, #255
	lsls	r2, r2, #8
.L_0200139e:
	adds	r2, #128
	adds	r3, r3, r2
	strh	r3, [r7, #18]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200b920
	cmp	r5, #7
	bls.n	.L_02001398
	ldr	r3, [r6, #12]
	ldr	r0, [pc, #112]
	movs	r5, #0
	adds	r3, r3, r0
	str	r3, [r6, #12]
.L_020013ba:
	ldrh	r3, [r7, #18]
	movs	r0, #1
	adds	r3, #128
	strh	r3, [r7, #18]
	adds	r5, #1
	bl 0x0200b920
	cmp	r5, #3
	bls.n	.L_020013ba
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #1
	bls.n	.L_02001370
	ldr	r3, [r6, #12]
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r3, r3, r0
	str	r3, [r6, #12]
	movs	r0, #20
	bl 0x0200b920
	mov	r0, r8
	ldr	r2, [r0, #8]
	ldr	r3, [r6, #8]
	ldr	r1, [pc, #56]
	subs	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r2, [r0, #12]
	ldr	r3, [r6, #12]
	subs	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r2, [r0, #16]
	ldr	r3, [r6, #16]
	subs	r3, r3, r2
	str	r3, [r1, #8]
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	movs	r1, #0
	bl 0x0200b9b0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xffff0000
	.2byte 0xca18
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #44]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	ldr	r1, [pc, #32]
	ldr	r3, [r0, #8]
	ldr	r2, [r1, #0]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r2, [r1, #4]
	ldr	r3, [r0, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r3, [r0, #16]
	ldr	r2, [r1, #8]
	adds	r3, r3, r2
	str	r3, [r5, #16]
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xca18
	.2byte 0x0200
	push	{lr}
	movs	r0, #14
	bl 0x0200b9e8
	bl 0x0200942c
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #440]
	movs	r2, #133
.L_02001486:
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	sub	sp, #68
.L_0200148e:
	bl 0x0200b9e8
	mov	sl, r0
	movs	r0, #14
	bl 0x0200b9e8
	ldr	r3, [r0, #80]
	mov	r9, r0
	mov	fp, r3
	bl 0x0200b9d8
.L_020014a4:
	movs	r0, #0
	bl 0x0200bb08
	movs	r1, #204
.L_020014ac:
	movs	r2, #164
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200ba20
	mov	r0, sl
	mov	r1, r9
.L_020014bc:
	bl 0x02009340
	movs	r3, #24
	movs	r2, #70
.L_020014c4:
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #50
	movs	r2, #3
	movs	r3, #1
	movs	r0, #0
	bl 0x0200b9a8
.L_020014d4:
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x0200b958
.L_020014dc:
	movs	r0, #1
	bl 0x0200b920
	mov	r2, sl
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r0, #14
.L_020014ec:
	movs	r1, #3
	bl 0x0200ba98
	mov	r1, r9
.L_020014f4:
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #144
	ldr	r0, [pc, #316]
	lsls	r1, r1, #3
.L_02001504:
	bl 0x0200b928
.L_02001508:
	movs	r0, #1
	bl 0x0200b920
	mov	r4, sl
	ldr	r3, [r4, #40]
	cmp	r3, #0
	bne.n	.L_02001508
	movs	r0, #227
	bl 0x0200bb48
.L_0200151c:
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
.L_02001524:
	lsls	r0, r0, #9
	lsls	r1, r1, #10
	bl 0x0200b9b8
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #272]
.L_02001534:
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #163
	lsls	r3, r3, #8
	adds	r3, #215
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r7, #0
.L_02001544:
	lsls	r5, r7, #12
	adds	r0, r5, #0
	bl 0x0200b948
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200b940
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200b938
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #208]
	adds	r7, #1
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200b938
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #196]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r5, r5, r3
	adds	r5, r5, r4
	str	r5, [r6, #8]
	mov	r2, sl
	ldr	r1, [r2, #12]
	ldr	r4, [r6, #4]
	ldr	r3, [pc, #180]
	ldr	r0, [r2, #8]
	adds	r1, r1, r3
	ldr	r2, [r2, #16]
	ldr	r3, [r6, #0]
	str	r4, [sp, #0]
	ldr	r4, [pc, #172]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl 0x020080b8
	cmp	r7, #16
	bls.n	.L_02001544
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r2, sl
	str	r3, [r2, #40]
	movs	r0, #2
	bl 0x0200b920
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200b9b8
	ldr	r0, [pc, #104]
	bl 0x0200b930
	movs	r7, #0
.L_020015de:
	mov	r4, fp
	ldrh	r3, [r4, #18]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r3, r3, r2
	strh	r3, [r4, #18]
	mov	r2, r9
	ldr	r3, [r2, #16]
	movs	r4, #128
	lsls	r4, r4, #9
	adds	r3, r3, r4
	str	r3, [r2, #16]
	movs	r0, #1
	adds	r7, #1
	bl 0x0200b920
	cmp	r7, #7
	bls.n	.L_020015de
	bl 0x0200b9c0
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	movs	r1, #1
	bl 0x0200b9b0
	mov	r2, sl
	adds	r2, #34
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #10
	bl 0x0200b920
	bl 0x0200b9e0
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02009465
	.4byte 0x020092e9
	.4byte 0xffffa000
	.4byte 0xffffd000
	.4byte 0xfffc0000
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	adds	r5, r1, #0
.L_0200165e:
	movs	r0, #1
	bl 0x0200b920
	ldr	r3, [r7, #40]
	cmp	r3, #0
.L_02001668:
	bne.n	.L_0200165e
	adds	r6, r5, #0
	adds	r6, #21
	movs	r0, #240
	bl 0x0200bb48
	adds	r5, #22
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200ba98
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x0200ba98
	adds	r0, r6, #0
	bl 0x0200b9e8
	ldr	r1, [r7, #8]
	ldr	r3, [r7, #16]
	movs	r2, #0
	bl 0x0200b980
	adds	r0, r5, #0
	bl 0x0200b9e8
	ldr	r3, [r7, #16]
	ldr	r2, [pc, #36]
	ldr	r1, [r7, #8]
	adds	r3, r3, r2
	movs	r2, #0
	bl 0x0200b980
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200ba40
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ba40
	movs	r0, #20
	bl 0x0200b920
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb500
	movs	r0, #15
	bl 0x0200b9e8
	bl 0x0200942c
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #444]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r6, r2
	ldr	r0, [r6, #0]
	sub	sp, #8
	bl 0x0200b9e8
	mov	sl, r0
	movs	r0, #15
	bl 0x0200b9e8
	mov	r8, r0
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	movs	r1, #164
	ldr	r0, [r6, #0]
	movs	r2, #216
	lsls	r1, r1, #1
	bl 0x0200ba20
	mov	r0, sl
	mov	r1, r8
	bl 0x02009340
	movs	r3, #13
	str	r3, [sp, #4]
	movs	r5, #19
	movs	r0, #0
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200b9a8
	movs	r3, #33
	str	r3, [sp, #4]
	movs	r1, #0
	movs	r2, #2
	movs	r3, #1
	movs	r0, #40
	str	r5, [sp, #0]
	mov	r7, sl
	bl 0x0200b9a8
	movs	r0, #1
	bl 0x0200b920
	adds	r7, #85
	movs	r2, #3
	movs	r3, #0
	strb	r2, [r7, #0]
	movs	r0, #15
	movs	r1, #3
	mov	fp, r3
	bl 0x0200ba98
	movs	r3, #35
	add	r8, r3
	mov	r2, r8
	ldrb	r3, [r2, #0]
	movs	r2, #2
	mov	r9, r2
	mov	r2, r9
	orrs	r3, r2
	mov	r2, r8
	strb	r3, [r2, #0]
	ldr	r3, [pc, #308]
	movs	r1, #144
	mov	r8, r3
	mov	r0, r8
	lsls	r1, r1, #3
	bl 0x0200b928
	mov	r0, sl
	movs	r1, #0
	bl 0x02009658
	movs	r1, #1
	movs	r0, #9
	bl 0x0200ba98
	movs	r0, #9
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r5, #253
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	movs	r0, #11
	bl 0x0200ba98
	movs	r0, #11
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r3, [r0, #0]
	mov	r2, fp
	ands	r5, r3
	strb	r5, [r0, #0]
	strb	r2, [r7, #0]
	ldr	r0, [r6, #0]
	bl 0x0200b9e8
	movs	r1, #1
	bl 0x0200b9b0
	movs	r1, #153
	movs	r2, #200
	lsls	r1, r1, #8
	lsls	r2, r2, #5
	ldr	r0, [r6, #0]
	adds	r1, #153
	adds	r2, #153
	bl 0x0200b9f0
	movs	r1, #164
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	movs	r2, #224
	bl 0x0200ba18
	movs	r1, #66
	movs	r2, #128
	ldr	r0, [r6, #0]
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x0200ba18
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ba30
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ba30
	movs	r1, #66
	movs	r2, #160
	ldr	r0, [r6, #0]
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x0200ba18
	movs	r1, #66
	movs	r2, #200
	ldr	r0, [r6, #0]
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x0200ba18
	movs	r1, #66
	movs	r2, #240
	ldr	r0, [r6, #0]
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x0200ba18
	movs	r1, #66
	movs	r2, #134
	lsls	r2, r2, #2
	ldr	r0, [r6, #0]
	adds	r1, #255
	bl 0x0200ba18
	movs	r0, #161
	bl 0x0200bb48
	movs	r1, #2
	movs	r0, #9
	bl 0x0200ba98
	movs	r0, #9
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r3, [r0, #0]
	mov	r2, r9
	orrs	r3, r2
	movs	r1, #2
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x0200ba98
	movs	r0, #11
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r3, [r0, #0]
	mov	r2, r9
	orrs	r2, r3
	movs	r3, #3
	strb	r2, [r0, #0]
	strb	r3, [r7, #0]
	mov	r0, r8
	mov	r9, r2
	bl 0x0200b930
	movs	r0, #1
	bl 0x0200b920
	bl 0x0200b9e0
	movs	r0, #153
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b958
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x96c9
	.2byte 0x0200
	push	{lr}
	movs	r0, #16
	bl 0x0200b9e8
	bl 0x0200942c
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r6, [pc, #268]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r6, r2
	ldr	r0, [r6, #0]
	sub	sp, #8
	bl 0x0200b9e8
	mov	sl, r0
	movs	r0, #16
	bl 0x0200b9e8
	mov	r8, r0
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	movs	r1, #204
	movs	r2, #180
	ldr	r0, [r6, #0]
	lsls	r2, r2, #1
	lsls	r1, r1, #1
	bl 0x0200ba20
	mov	r0, sl
	mov	r1, r8
	bl 0x02009340
	movs	r3, #22
	str	r3, [sp, #4]
	movs	r5, #24
	movs	r0, #0
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200b9a8
	movs	r1, #0
	movs	r2, #2
	movs	r3, #1
	movs	r0, #40
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200b9a8
	movs	r0, #1
	bl 0x0200b920
	mov	r2, sl
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r0, #16
	movs	r1, #3
	bl 0x0200ba98
	movs	r3, #35
	add	r8, r3
	mov	r3, r8
	ldrb	r2, [r3, #0]
	ldr	r5, [pc, #148]
	movs	r3, #2
	orrs	r3, r2
	movs	r1, #144
	mov	r2, r8
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	lsls	r1, r1, #3
	bl 0x0200b928
	mov	r0, sl
	movs	r1, #2
	bl 0x02009658
	ldr	r0, [r6, #0]
	bl 0x0200b9e8
	movs	r1, #1
	bl 0x0200b9b0
	movs	r1, #153
	movs	r2, #200
	lsls	r1, r1, #8
	lsls	r2, r2, #5
	ldr	r0, [r6, #0]
	adds	r1, #153
	adds	r2, #153
	bl 0x0200b9f0
	movs	r1, #203
	movs	r2, #180
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200ba18
	movs	r1, #200
	movs	r2, #196
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200ba18
	movs	r0, #161
	bl 0x0200bb48
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ba30
	movs	r1, #0
	movs	r2, #0
	movs	r0, #24
	bl 0x0200ba30
	adds	r0, r5, #0
	bl 0x0200b930
	movs	r0, #1
	bl 0x0200b920
	bl 0x0200b9e0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #50
	bl 0x0200b958
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x98ad
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #42
	movs	r1, #1
	movs	r2, #15
	movs	r3, #14
	bl 0x0200b998
	movs	r3, #15
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200b9a8
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #15
	movs	r1, #9
	movs	r2, #15
	movs	r3, #14
	bl 0x0200b998
	movs	r3, #15
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #14
	movs	r1, #14
	movs	r2, #1
	movs	r3, #1
	bl 0x0200b9a8
	add	sp, #8
	pop	{pc}
	push	{r5, lr}
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	ldr	r0, [pc, #708]
	bl 0x0200ba70
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	ldr	r5, [pc, #696]
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
	bl 0x0200b9f0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #14
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b9f0
	movs	r1, #136
	movs	r2, #168
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200ba20
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200ba88
	movs	r1, #136
	movs	r2, #168
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #14
	bl 0x0200ba30
	movs	r0, #1
	bl 0x0200b920
	movs	r1, #128
	movs	r2, #168
	movs	r0, #14
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200ba20
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ba88
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200ba88
	movs	r1, #208
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200ba88
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200ba88
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	bl 0x0200bac0
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #528]
	adds	r1, #153
	bl 0x0200bab8
	movs	r0, #144
	movs	r1, #1
	movs	r2, #252
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #15
	bl 0x0200bac0
	bl 0x0200bac8
	movs	r2, #20
	movs	r0, #13
	movs	r1, #4
	bl 0x0200ba50
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #3
	movs	r0, #12
	bl 0x0200ba48
	movs	r0, #20
	bl 0x0200b9d0
	movs	r0, #12
	movs	r1, #2
	bl 0x0200ba58
	movs	r2, #80
	movs	r0, #12
	movs	r1, #6
	bl 0x0200ba50
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #12
	movs	r1, #4
	bl 0x0200ba40
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200ba88
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #13
	bl 0x0200baa8
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	movs	r2, #0
	movs	r0, #13
	movs	r1, #0
	bl 0x0200ba88
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #13
	bl 0x0200bab0
	movs	r0, #20
	bl 0x0200b9d0
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #12
	movs	r1, #3
	bl 0x0200ba48
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #204
	lsls	r1, r1, #7
	ldr	r0, [pc, #304]
	adds	r1, #102
	bl 0x0200bab8
	movs	r0, #210
	movs	r1, #1
	movs	r2, #200
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200bac0
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #276]
	adds	r2, #204
	bl 0x0200b9f0
	movs	r1, #173
	movs	r0, #12
	lsls	r1, r1, #1
	movs	r2, #186
	bl 0x0200ba20
	movs	r1, #187
	movs	r0, #12
	lsls	r1, r1, #1
	movs	r2, #192
	bl 0x0200ba20
	movs	r1, #196
	movs	r0, #12
	lsls	r1, r1, #1
	movs	r2, #224
	bl 0x0200ba20
	movs	r1, #197
	movs	r0, #12
	lsls	r1, r1, #1
	movs	r2, #246
	bl 0x0200ba20
	movs	r1, #210
	movs	r2, #134
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200ba20
	movs	r1, #228
	movs	r2, #134
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200ba20
	movs	r1, #244
	movs	r0, #12
	lsls	r1, r1, #1
	movs	r2, #240
	bl 0x0200ba20
	movs	r1, #244
	movs	r0, #12
	lsls	r1, r1, #1
	movs	r2, #226
	bl 0x0200ba20
	movs	r2, #0
	movs	r0, #12
	movs	r1, #0
	bl 0x0200ba30
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #148]
	adds	r1, #153
	bl 0x0200bab8
	movs	r0, #132
	movs	r1, #1
	movs	r2, #160
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200bac0
	bl 0x0200bac8
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200ba88
	movs	r2, #20
	movs	r0, #14
	movs	r1, #0
	bl 0x0200ba88
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200ba40
	movs	r0, #14
	movs	r1, #3
	bl 0x0200ba48
	movs	r0, #14
	movs	r1, #2
	bl 0x0200ba40
	ldr	r0, [r5, #0]
	bl 0x0200b9e8
	cmp	r0, #0
	beq.n	.L_02001ce6
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #14
	bl 0x0200ba10
.L_02001ce6:
	movs	r0, #14
	bl 0x0200ba28
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ba30
	movs	r1, #0
	movs	r2, #0
	movs	r0, #13
	bl 0x0200ba30
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #114
	bl 0x0200b958
	bl 0x0200b9e0
	pop	{r5, pc}
	.4byte 0x000017bb
	.4byte 0x02000240
	.4byte 0x0004cccc
	.4byte 0x00033333
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x0200b950
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02001d40
	b.n	.L_02002372
.L_02001d40:
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	bl 0x0200bad0
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r1, #1
	movs	r0, #170
	movs	r2, #176
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200bac0
	ldr	r1, [pc, #1016]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r1, r1, r2
	mov	r9, r1
	ldr	r0, [r1, #0]
	movs	r2, #204
	movs	r1, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b9f0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #14
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b9f0
	mov	r3, r9
	movs	r1, #172
	ldr	r0, [r3, #0]
	lsls	r1, r1, #1
	movs	r2, #184
	bl 0x0200ba20
	mov	r1, r9
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200ba88
	movs	r1, #172
	movs	r2, #184
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #14
	bl 0x0200ba30
	movs	r0, #1
	bl 0x0200b920
	movs	r1, #178
	movs	r2, #196
	movs	r0, #14
	lsls	r1, r1, #1
	bl 0x0200ba20
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #14
	bl 0x0200ba90
	ldr	r0, [pc, #904]
	bl 0x0200ba70
	movs	r0, #14
	movs	r1, #0
	bl 0x0200ba80
	mov	r2, r9
	ldr	r0, [r2, #0]
	movs	r1, #3
	bl 0x0200ba48
	movs	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #184
	movs	r2, #168
	movs	r0, #13
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200ba30
	mov	r3, r9
	movs	r1, #128
	ldr	r0, [r3, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200ba88
	movs	r1, #128
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #8
	bl 0x0200ba88
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #832]
	adds	r1, #153
	bl 0x0200bab8
	movs	r0, #232
	movs	r1, #128
	movs	r2, #176
	movs	r3, #1
	lsls	r0, r0, #16
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x0200bac0
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #13
	ldr	r1, [pc, #804]
	adds	r2, #204
	bl 0x0200b9f0
	movs	r2, #168
	movs	r1, #232
	movs	r0, #13
	bl 0x0200ba20
	movs	r0, #20
	bl 0x0200b9d0
	movs	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #152
	lsls	r1, r1, #6
	ldr	r0, [pc, #772]
	adds	r1, #102
	bl 0x0200bab8
	movs	r0, #174
	movs	r1, #1
	movs	r2, #216
	movs	r3, #1
	lsls	r2, r2, #16
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200bac0
	movs	r0, #13
	bl 0x0200b9e8
	adds	r5, r0, #0
	ldr	r1, [r5, #80]
	movs	r0, #13
	mov	r8, r1
	movs	r1, #1
	bl 0x0200ba98
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #13
	ldr	r1, [pc, #716]
	adds	r2, #204
	bl 0x0200b9f0
	movs	r0, #13
	movs	r1, #4
	movs	r2, #0
	bl 0x0200ba50
	movs	r2, #168
	movs	r0, #13
	movs	r1, #248
	bl 0x0200ba18
	movs	r0, #13
	movs	r1, #6
	bl 0x0200ba40
	movs	r2, #208
	ldr	r3, [pc, #688]
	lsls	r2, r2, #8
	mov	sl, r2
	adds	r7, r5, #0
	mov	r2, r8
	mov	r1, sl
	adds	r7, #85
	str	r3, [r5, #24]
	strh	r1, [r2, #18]
	strb	r3, [r7, #0]
	movs	r1, #192
	ldr	r3, [r5, #12]
	lsls	r1, r1, #11
	adds	r3, r3, r1
	str	r3, [r5, #12]
	movs	r0, #13
	bl 0x0200b9e8
	movs	r1, #0
	bl 0x0200b9b0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #13
	adds	r1, #102
	adds	r2, #51
	bl 0x0200b9f0
	movs	r1, #164
	movs	r2, #152
	movs	r3, #168
	adds	r0, r5, #0
	lsls	r1, r1, #17
	lsls	r2, r2, #14
	lsls	r3, r3, #16
	bl 0x0200b990
	movs	r0, #13
	bl 0x0200ba28
	movs	r3, #3
	strb	r3, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r5, #20]
	movs	r3, #128
	lsls	r3, r3, #9
	mov	r2, r8
	str	r3, [r5, #24]
	movs	r1, #1
	strh	r6, [r2, #18]
	movs	r0, #13
	bl 0x0200ba40
	movs	r0, #13
	bl 0x0200b9e8
	movs	r1, #1
	bl 0x0200b9b0
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #13
	ldr	r1, [pc, #548]
	adds	r2, #204
	bl 0x0200b9f0
	movs	r0, #13
	movs	r1, #4
	movs	r2, #0
	bl 0x0200ba50
	movs	r1, #164
	movs	r0, #13
	lsls	r1, r1, #1
	movs	r2, #184
	bl 0x0200ba18
	movs	r0, #13
	movs	r1, #0
	movs	r2, #20
	bl 0x0200ba88
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #13
	bl 0x0200baa8
	movs	r0, #13
	movs	r1, #2
	bl 0x0200ba98
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #14
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #13
	bl 0x0200baa8
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #176
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200ba88
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200ba88
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #60
	adds	r0, #14
	movs	r1, #0
	bl 0x0200ba78
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #13
	movs	r1, #3
	bl 0x0200ba40
	movs	r1, #197
	movs	r2, #143
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #12
	bl 0x0200ba30
	movs	r0, #1
	bl 0x0200b920
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #356]
	adds	r2, #204
	bl 0x0200b9f0
	movs	r1, #197
	movs	r0, #12
	lsls	r1, r1, #1
	movs	r2, #248
	bl 0x0200ba20
	movs	r1, #176
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200ba88
	movs	r2, #20
	movs	r0, #12
	movs	r1, #4
	bl 0x0200ba50
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #13
	bl 0x0200baa8
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200ba88
	mov	r3, r9
	movs	r1, #128
	ldr	r0, [r3, #0]
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200ba88
	movs	r1, #192
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #6
	bl 0x0200ba88
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #244]
	adds	r1, #153
	bl 0x0200bab8
	movs	r0, #188
	movs	r1, #1
	movs	r2, #132
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x0200bac0
	bl 0x0200bac8
	movs	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #220]
	adds	r1, #204
	bl 0x0200bab8
	movs	r0, #174
	movs	r1, #1
	movs	r2, #216
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x0200bac0
	mov	r1, r9
	ldr	r0, [r1, #0]
	movs	r1, #12
	bl 0x0200bb10
	movs	r0, #14
	movs	r1, #12
	bl 0x0200bb10
	movs	r0, #13
	movs	r1, #12
	bl 0x0200bb10
	movs	r1, #197
	movs	r0, #12
	lsls	r1, r1, #1
	movs	r2, #220
	bl 0x0200ba20
	movs	r1, #185
	movs	r0, #12
	lsls	r1, r1, #1
	movs	r2, #198
	bl 0x0200ba20
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #14
	ldr	r1, [pc, #124]
	adds	r2, #204
	bl 0x0200b9f0
	movs	r1, #184
	movs	r2, #180
	movs	r0, #14
	lsls	r1, r1, #1
	bl 0x0200ba18
	movs	r0, #14
	movs	r1, #12
	bl 0x0200bb10
	movs	r1, #168
	movs	r0, #12
	lsls	r1, r1, #1
	movs	r2, #198
	bl 0x0200ba20
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #12
	bl 0x0200ba88
	movs	r0, #13
	bl 0x0200ba00
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r2, #0
	movs	r0, #12
	mov	r1, sl
	bl 0x0200ba88
	movs	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #176
	movs	r0, #12
	lsls	r1, r1, #8
	bl 0x0200ba90
	movs	r0, #12
	movs	r1, #4
	movs	r2, #20
	bl 0x0200ba50
	movs	r0, #12
	movs	r1, #0
	movs	r2, #20
	b.n	.L_0200217c
	.4byte 0x02000240
	.4byte 0x000017c7
	.4byte 0x0004cccc
	.4byte 0x00019999
	.4byte 0x00013333
	.4byte 0xffff0000
	.2byte 0x6666
	.2byte 0x0002
.L_0200217c:
	bl 0x0200ba78
	movs	r1, #6
	adds	r1, #255
	movs	r2, #40
	movs	r0, #13
	bl 0x0200baa8
	movs	r1, #2
	movs	r2, #60
	adds	r1, #255
	movs	r0, #12
	bl 0x0200baa8
	movs	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #13
	movs	r1, #4
	bl 0x0200ba40
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #12
	bl 0x0200baa8
	movs	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #128
	movs	r2, #20
	movs	r0, #13
	lsls	r1, r1, #8
	bl 0x0200ba88
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #128
	movs	r2, #0
	movs	r0, #12
	lsls	r1, r1, #8
	bl 0x0200ba88
	movs	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #13
	movs	r1, #4
	bl 0x0200ba48
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #12
	bl 0x0200baa8
	movs	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #12
	bl 0x0200baa8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #13
	bl 0x0200baa8
	movs	r1, #176
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200ba88
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200ba88
	movs	r2, #40
	movs	r0, #13
	movs	r1, #4
	bl 0x0200ba50
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #12
	movs	r1, #3
	bl 0x0200ba40
	movs	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #13
	movs	r1, #3
	bl 0x0200ba48
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #13
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #12
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #12
	movs	r1, #3
	bl 0x0200ba40
	movs	r0, #13
	movs	r1, #3
	bl 0x0200ba48
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #212]
	adds	r2, #153
	bl 0x0200b9f0
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #13
	ldr	r1, [pc, #196]
	bl 0x0200b9f0
	ldr	r5, [pc, #196]
	movs	r0, #12
	adds	r1, r5, #0
	bl 0x0200b9f8
	movs	r0, #10
	bl 0x0200b9d0
	adds	r1, r5, #0
	movs	r0, #13
	bl 0x0200ba08
	movs	r0, #40
	bl 0x0200b9d0
	mov	r2, r9
	ldr	r0, [r2, #0]
	bl 0x0200ba00
	movs	r0, #14
	bl 0x0200ba00
	movs	r0, #1
	bl 0x0200b920
	movs	r0, #14
	movs	r1, #0
	movs	r2, #40
	bl 0x0200ba78
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ba30
	movs	r2, #0
	movs	r0, #12
	movs	r1, #0
	bl 0x0200ba30
	movs	r0, #14
	movs	r1, #0
	bl 0x0200ba80
	mov	r3, r9
	ldr	r0, [r3, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ba88
	movs	r1, #128
	movs	r2, #20
	movs	r0, #14
	lsls	r1, r1, #8
	bl 0x0200ba88
	movs	r0, #14
	movs	r1, #0
	bl 0x0200ba80
	movs	r0, #14
	movs	r1, #2
	bl 0x0200ba40
	mov	r1, r9
	ldr	r0, [r1, #0]
	bl 0x0200b9e8
	cmp	r0, #0
	beq.n	.L_02002354
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #14
	bl 0x0200ba10
.L_02002354:
	movs	r0, #14
	bl 0x0200ba28
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ba30
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x0200b958
	bl 0x0200b9e0
.L_02002372:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x00013333
	.2byte 0xbc58
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	bl 0x0200bad0
	movs	r6, #0
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r1, #128
	movs	r0, #240
	movs	r2, #170
	lsls	r0, r0, #17
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x0200bac0
	bl 0x0200bac8
	ldr	r5, [pc, #116]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r1, [r5, #0]
	movs	r0, #15
	movs	r2, #0
	bl 0x0200ba60
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200baa8
	movs	r1, #1
	movs	r0, #15
	bl 0x0200ba98
	movs	r0, #15
	bl 0x0200b9e8
	movs	r2, #204
	adds	r0, #85
	lsls	r2, r2, #8
	strb	r6, [r0, #0]
	ldr	r1, [pc, #68]
	movs	r0, #15
	adds	r2, #204
	bl 0x0200b9f0
	movs	r1, #250
	movs	r2, #172
	movs	r0, #15
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200ba20
	movs	r1, #145
	movs	r2, #172
	lsls	r2, r2, #1
	movs	r0, #15
	lsls	r1, r1, #2
	bl 0x0200ba20
	ldr	r1, [r5, #0]
	movs	r0, #15
	bl 0x0200bb10
	movs	r0, #20
	bl 0x0200b9d0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #116
	bl 0x0200b958
	bl 0x0200b9e0
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, lr}
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	movs	r0, #15
	bl 0x0200ba00
.L_02002442:
	bl 0x0200bad0
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r1, #128
	movs	r0, #141
	movs	r2, #172
	lsls	r0, r0, #18
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x0200bac0
	bl 0x0200bac8
	ldr	r3, [pc, #120]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
.L_0200246a:
	ldr	r1, [r3, #0]
	movs	r0, #15
	movs	r2, #0
	bl 0x0200ba60
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200baa8
	movs	r1, #1
	movs	r0, #15
	bl 0x0200ba98
	movs	r0, #15
	bl 0x0200b9e8
	movs	r2, #204
	adds	r0, #85
	lsls	r2, r2, #8
	strb	r5, [r0, #0]
	ldr	r1, [pc, #72]
	movs	r0, #15
	adds	r2, #204
	bl 0x0200b9f0
	movs	r1, #154
	movs	r2, #188
	movs	r0, #15
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200ba20
	movs	r1, #154
	movs	r2, #230
	movs	r0, #15
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200ba20
	movs	r1, #0
	movs	r2, #0
	movs	r0, #15
	bl 0x0200ba30
	movs	r0, #20
	bl 0x0200b9d0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #117
	bl 0x0200b958
	bl 0x0200b9e0
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, lr}
	movs	r0, #13
	bl 0x0200b9e8
	adds	r6, r0, #0
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	ldr	r5, [pc, #248]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200baa8
	bl 0x0200bad0
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r0, #164
	movs	r2, #138
	lsls	r0, r0, #17
	lsls	r1, r1, #14
	lsls	r2, r2, #18
	movs	r3, #1
	bl 0x0200bac0
	bl 0x0200bac8
	movs	r3, #192
	movs	r1, #164
	movs	r2, #156
	lsls	r3, r3, #8
	lsls	r2, r2, #18
	lsls	r1, r1, #17
	movs	r0, #14
	bl 0x0200ba38
	movs	r0, #1
	bl 0x0200b920
	adds	r6, #35
	ldr	r0, [r5, #0]
	movs	r1, #14
	bl 0x0200bb10
	ldrb	r2, [r6, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r6, #0]
	movs	r0, #14
	movs	r1, #3
	bl 0x0200ba98
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #14
	ldr	r1, [pc, #144]
	adds	r2, #204
	bl 0x0200b9f0
	movs	r1, #164
	movs	r2, #142
	movs	r0, #14
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200ba20
	movs	r1, #164
	movs	r2, #248
	movs	r0, #14
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200ba20
	movs	r1, #140
	movs	r2, #216
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x0200ba20
	ldr	r0, [r5, #0]
	bl 0x0200ba00
	movs	r0, #1
	bl 0x0200b920
	movs	r3, #128
	movs	r1, #140
	movs	r2, #164
	lsls	r3, r3, #7
	lsls	r2, r2, #17
	movs	r0, #14
	lsls	r1, r1, #17
	bl 0x0200ba38
	movs	r1, #2
	movs	r0, #14
	bl 0x0200ba98
	movs	r0, #14
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r3, #2
	ldrb	r2, [r6, #0]
	movs	r0, #1
	orrs	r3, r2
	strb	r3, [r6, #0]
	bl 0x0200b920
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #118
	bl 0x0200b958
	movs	r0, #239
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200b958
	bl 0x0200b9e0
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #118
	bl 0x0200b950
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020026ec
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	ldr	r5, [pc, #212]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200baa8
	bl 0x0200bad0
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r1, #192
	movs	r0, #164
	movs	r2, #138
	lsls	r0, r0, #17
	lsls	r1, r1, #14
	lsls	r2, r2, #18
	movs	r3, #1
	bl 0x0200bac0
	bl 0x0200bac8
	movs	r3, #128
	movs	r1, #212
	movs	r2, #130
	lsls	r3, r3, #7
	lsls	r2, r2, #18
	lsls	r1, r1, #17
	movs	r0, #14
	bl 0x0200ba38
	movs	r0, #1
	bl 0x0200b920
	ldr	r0, [r5, #0]
	movs	r1, #14
	bl 0x0200bb10
	movs	r0, #14
	movs	r1, #1
	bl 0x0200ba98
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #14
	ldr	r1, [pc, #120]
	adds	r2, #204
	bl 0x0200b9f0
	movs	r1, #190
	movs	r2, #138
	movs	r0, #14
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200ba20
	movs	r1, #132
	movs	r2, #138
	movs	r0, #14
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200ba20
	movs	r1, #132
	movs	r2, #135
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	movs	r0, #14
	bl 0x0200ba20
	ldr	r0, [r5, #0]
	bl 0x0200ba00
	movs	r0, #1
	bl 0x0200b920
	movs	r2, #0
	movs	r0, #14
	movs	r1, #0
	bl 0x0200ba30
	movs	r1, #2
	movs	r0, #14
	bl 0x0200ba98
	movs	r0, #14
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #1
	bl 0x0200b920
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #118
	bl 0x0200b958
	bl 0x0200b9e0
.L_020026ec:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, lr}
	movs	r0, #239
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_02002792
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	bl 0x0200bad0
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r1, #1
	movs	r0, #160
	movs	r2, #150
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200bac0
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #14
	bl 0x0200baa8
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #14
	ldr	r1, [pc, #80]
	adds	r2, #153
	bl 0x0200b9f0
	movs	r1, #90
	movs	r2, #148
	movs	r0, #14
	adds	r1, #255
	lsls	r2, r2, #1
	bl 0x0200ba20
	movs	r1, #196
	movs	r2, #148
	movs	r0, #14
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200ba20
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #14
	bl 0x0200bb10
	movs	r0, #14
	bl 0x0200b9e8
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #10
	bl 0x0200b920
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #124
	bl 0x0200b958
	bl 0x0200b9e0
.L_02002792:
	pop	{r5, pc}
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200b9d8
	movs	r0, #0
	bl 0x0200bb08
	bl 0x0200bad0
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #14
	bl 0x0200ba00
	movs	r0, #1
	bl 0x0200b920
	movs	r0, #14
	ldr	r1, [pc, #8]
	ldr	r2, [pc, #12]
	bl 0x0200b9f0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00026666
	.2byte 0x3333
	.2byte 0x0001
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #14
	bl 0x0200bb10
	movs	r0, #5
	bl 0x0200b9d0
	bl 0x0200b9e0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200a79c
.L_020027fe:
	movs	r0, #196
	movs	r1, #128
	movs	r2, #216
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #16
	bl 0x0200bac0
	movs	r1, #196
	lsls	r1, r1, #1
	movs	r2, #216
	movs	r0, #14
	bl 0x0200ba20
	movs	r0, #14
	bl 0x0200b9e8
	movs	r3, #1
	adds	r0, #98
	strb	r3, [r0, #0]
	bl 0x0200a7d4
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #14
	bl 0x0200b9e8
	adds	r5, r0, #0
	bl 0x0200a79c
	movs	r0, #180
	movs	r1, #128
	movs	r2, #216
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #16
	bl 0x0200bac0
	movs	r1, #180
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #216
	bl 0x0200ba20
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b950
	adds	r6, r5, #0
	adds	r6, #98
	cmp	r0, #0
	beq.n	.L_02002894
	movs	r3, #128
	lsls	r3, r3, #8
	movs	r1, #129
	strh	r3, [r5, #6]
	movs	r0, #14
	lsls	r1, r1, #1
	bl 0x0200bab0
	movs	r0, #20
	bl 0x0200b9d0
	movs	r0, #128
	movs	r3, #4
	lsls	r0, r0, #2
	strb	r3, [r6, #0]
	adds	r0, #6
	bl 0x0200b958
	b.n	.L_0200290a
.L_02002894:
	movs	r0, #5
	bl 0x0200b9d0
.L_0200289a:
	movs	r0, #148
	movs	r1, #128
	movs	r2, #216
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r1, r1, #13
	lsls	r0, r0, #17
	bl 0x0200bac0
	movs	r0, #153
	bl 0x0200bb48
	movs	r0, #14
	bl 0x0200b9e8
	movs	r1, #0
	bl 0x0200b9b0
	movs	r0, #14
	ldr	r1, [pc, #76]
	ldr	r2, [pc, #80]
	bl 0x0200b9f0
	movs	r3, #128
	lsls	r3, r3, #12
	movs	r1, #164
	str	r3, [r5, #40]
	movs	r2, #216
	lsls	r1, r1, #1
	movs	r0, #14
	bl 0x0200ba18
	movs	r0, #14
	bl 0x0200b9e8
	movs	r1, #1
	bl 0x0200b9b0
	movs	r0, #10
	bl 0x0200b9d0
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #14
	ldr	r1, [pc, #36]
	adds	r2, #204
	bl 0x0200b9f0
	movs	r1, #148
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #216
	bl 0x0200ba20
	movs	r3, #2
	strb	r3, [r6, #0]
.L_0200290a:
	bl 0x0200a7d4
	pop	{r5, r6, pc}
	.4byte 0x0004cccc
	.4byte 0x00026666
	.2byte 0x9999
	.2byte 0x0001
	push	{lr}
	bl 0x0200a79c
	movs	r0, #140
	movs	r1, #128
	movs	r2, #148
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	bl 0x0200bac0
	movs	r1, #148
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #248
	bl 0x0200ba20
	movs	r1, #140
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #248
	bl 0x0200ba20
	movs	r1, #140
	movs	r2, #148
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x0200ba20
	movs	r0, #14
	bl 0x0200b9e8
	movs	r3, #3
	adds	r0, #98
	strb	r3, [r0, #0]
	bl 0x0200a7d4
	pop	{pc}
	push	{lr}
	bl 0x0200a79c
	movs	r0, #196
	movs	r1, #128
	movs	r2, #148
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	bl 0x0200bac0
	movs	r1, #196
	movs	r2, #148
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x0200ba20
	movs	r0, #14
	bl 0x0200b9e8
	movs	r3, #0
	adds	r0, #98
	strb	r3, [r0, #0]
	bl 0x0200a7d4
	pop	{pc}
	push	{lr}
	bl 0x0200a79c
	movs	r0, #140
	movs	r1, #128
	movs	r2, #148
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	bl 0x0200bac0
	movs	r1, #140
	movs	r2, #148
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x0200ba20
	movs	r0, #14
	bl 0x0200b9e8
	movs	r3, #3
	adds	r0, #98
	strb	r3, [r0, #0]
	bl 0x0200a7d4
	pop	{pc}
	push	{lr}
	bl 0x0200a79c
	movs	r0, #148
	movs	r1, #128
	movs	r2, #216
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #16
	bl 0x0200bac0
	movs	r1, #140
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #248
	bl 0x0200ba20
	movs	r1, #148
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #248
	bl 0x0200ba20
	movs	r1, #148
	lsls	r1, r1, #1
	movs	r2, #216
	movs	r0, #14
	bl 0x0200ba20
	movs	r0, #14
	bl 0x0200b9e8
	movs	r3, #2
	adds	r0, #98
	strb	r3, [r0, #0]
	bl 0x0200a7d4
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #14
	bl 0x0200b9e8
	adds	r5, r0, #0
	bl 0x0200a79c
	movs	r0, #164
	movs	r1, #128
	movs	r2, #216
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #16
	bl 0x0200bac0
	movs	r1, #164
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #216
	bl 0x0200ba20
	movs	r0, #132
	lsls	r0, r0, #1
.L_02002a5c:
	adds	r0, #255
	bl 0x0200b950
	adds	r6, r5, #0
	adds	r6, #98
	cmp	r0, #0
	beq.n	.L_02002a8e
	movs	r3, #0
	movs	r1, #129
	strh	r3, [r5, #6]
	movs	r0, #14
	lsls	r1, r1, #1
	bl 0x0200bab0
	movs	r0, #20
	bl 0x0200b9d0
	movs	r0, #128
	movs	r3, #5
	lsls	r0, r0, #2
	strb	r3, [r6, #0]
	adds	r0, #6
	bl 0x0200b958
	b.n	.L_02002b04
.L_02002a8e:
	movs	r0, #5
	bl 0x0200b9d0
	movs	r0, #196
	movs	r1, #128
	movs	r2, #216
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r1, r1, #13
	lsls	r0, r0, #17
	bl 0x0200bac0
	movs	r0, #153
	bl 0x0200bb48
	movs	r0, #14
	bl 0x0200b9e8
	movs	r1, #0
	bl 0x0200b9b0
	movs	r0, #14
	ldr	r1, [pc, #80]
	ldr	r2, [pc, #80]
	bl 0x0200b9f0
	movs	r3, #128
	lsls	r3, r3, #12
	movs	r1, #180
	str	r3, [r5, #40]
	movs	r2, #216
	lsls	r1, r1, #1
	movs	r0, #14
	bl 0x0200ba18
	movs	r0, #14
	bl 0x0200b9e8
	movs	r1, #1
	bl 0x0200b9b0
	movs	r0, #10
	bl 0x0200b9d0
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #14
	ldr	r1, [pc, #36]
	adds	r2, #204
	bl 0x0200b9f0
	movs	r1, #196
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #216
	bl 0x0200ba20
	movs	r3, #1
	strb	r3, [r6, #0]
.L_02002b04:
	bl 0x0200a7d4
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0004cccc
	.4byte 0x00026666
	.2byte 0x9999
	.2byte 0x0001
	push	{lr}
	bl 0x0200a79c
	movs	r0, #196
	movs	r1, #128
	movs	r2, #148
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	bl 0x0200bac0
	movs	r1, #196
	movs	r2, #148
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x0200ba20
	movs	r0, #14
	bl 0x0200b9e8
	movs	r3, #0
	adds	r0, #98
	strb	r3, [r0, #0]
	bl 0x0200a7d4
	pop	{pc}
	push	{r5, r6, lr}
	movs	r0, #14
	bl 0x0200b9e8
	adds	r5, r0, #0
	bl 0x0200a79c
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b950
	adds	r6, r5, #0
	adds	r6, #98
	cmp	r0, #0
	beq.n	.L_02002b96
	movs	r3, #128
	lsls	r3, r3, #8
	movs	r1, #129
	strh	r3, [r5, #6]
	movs	r0, #14
	lsls	r1, r1, #1
	bl 0x0200bab0
	movs	r0, #20
	bl 0x0200b9d0
	movs	r0, #128
	movs	r3, #4
	lsls	r0, r0, #2
	strb	r3, [r6, #0]
	adds	r0, #6
	bl 0x0200b958
	b.n	.L_02002c06
.L_02002b96:
	movs	r0, #148
	movs	r1, #128
	movs	r2, #216
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r1, r1, #13
	lsls	r0, r0, #17
	bl 0x0200bac0
	movs	r0, #153
	bl 0x0200bb48
	movs	r0, #14
	bl 0x0200b9e8
	movs	r1, #0
	bl 0x0200b9b0
	movs	r0, #14
	ldr	r1, [pc, #76]
	ldr	r2, [pc, #80]
	bl 0x0200b9f0
	movs	r3, #128
	lsls	r3, r3, #12
	movs	r1, #164
	str	r3, [r5, #40]
	movs	r2, #216
	lsls	r1, r1, #1
	movs	r0, #14
	bl 0x0200ba18
	movs	r0, #14
	bl 0x0200b9e8
	movs	r1, #1
	bl 0x0200b9b0
	movs	r0, #10
	bl 0x0200b9d0
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #14
	ldr	r1, [pc, #36]
	adds	r2, #204
	bl 0x0200b9f0
	movs	r1, #148
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #216
	bl 0x0200ba20
	movs	r3, #2
	strb	r3, [r6, #0]
.L_02002c06:
	bl 0x0200a7d4
	pop	{r5, r6, pc}
	.4byte 0x0004cccc
	.4byte 0x00026666
	.2byte 0x9999
	.2byte 0x0001
	push	{lr}
	bl 0x0200a79c
	movs	r0, #196
	movs	r1, #128
	movs	r2, #216
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #16
	bl 0x0200bac0
	movs	r1, #196
	lsls	r1, r1, #1
	movs	r2, #216
	movs	r0, #14
	bl 0x0200ba20
.L_02002c3c:
	movs	r0, #14
	bl 0x0200b9e8
	movs	r3, #1
	adds	r0, #98
	strb	r3, [r0, #0]
	bl 0x0200a7d4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x0200a79c
	movs	r0, #148
	movs	r1, #128
	movs	r2, #216
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #16
	bl 0x0200bac0
	movs	r1, #148
	lsls	r1, r1, #1
	movs	r2, #216
	movs	r0, #14
	bl 0x0200ba20
	movs	r0, #14
	bl 0x0200b9e8
	movs	r3, #2
	adds	r0, #98
	strb	r3, [r0, #0]
	bl 0x0200a7d4
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #14
	bl 0x0200b9e8
	adds	r5, r0, #0
	bl 0x0200a79c
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b950
	adds	r6, r5, #0
	adds	r6, #98
	cmp	r0, #0
	beq.n	.L_02002ccc
	movs	r3, #0
	movs	r1, #129
	strh	r3, [r5, #6]
	movs	r0, #14
	lsls	r1, r1, #1
	bl 0x0200bab0
	movs	r0, #20
	bl 0x0200b9d0
	movs	r0, #128
	movs	r3, #5
	lsls	r0, r0, #2
	strb	r3, [r6, #0]
	adds	r0, #6
	bl 0x0200b958
	b.n	.L_02002d3c
.L_02002ccc:
	movs	r0, #196
	movs	r1, #128
	movs	r2, #216
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r1, r1, #13
	lsls	r0, r0, #17
	bl 0x0200bac0
	movs	r0, #153
	bl 0x0200bb48
	movs	r0, #14
	bl 0x0200b9e8
	movs	r1, #0
	bl 0x0200b9b0
	movs	r0, #14
	ldr	r1, [pc, #80]
	ldr	r2, [pc, #80]
	bl 0x0200b9f0
	movs	r3, #128
	lsls	r3, r3, #12
	movs	r1, #180
	str	r3, [r5, #40]
	movs	r2, #216
	lsls	r1, r1, #1
	movs	r0, #14
	bl 0x0200ba18
	movs	r0, #14
	bl 0x0200b9e8
	movs	r1, #1
	bl 0x0200b9b0
	movs	r0, #10
	bl 0x0200b9d0
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #14
	ldr	r1, [pc, #36]
	adds	r2, #204
	bl 0x0200b9f0
	movs	r1, #196
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #216
	bl 0x0200ba20
	movs	r3, #1
	strb	r3, [r6, #0]
.L_02002d3c:
	bl 0x0200a7d4
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0004cccc
	.4byte 0x00026666
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #124
	bl 0x0200b950
	cmp	r0, #0
	bne.n	.L_02002d62
	b.n	.L_02002e6a
.L_02002d62:
	movs	r0, #115
	bl 0x0200b950
	cmp	r0, #0
	bne.n	.L_02002e6a
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r0, #14
	bl 0x0200b9e8
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r5, r5, r2
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	subs	r3, #13
	cmp	r3, #7
	bhi.n	.L_02002e6a
	ldr	r2, [pc, #224]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	adds	r0, #98
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200adb4
	.4byte 0x0200adca
	.4byte 0x0200add6
	.4byte 0x0200adfa
	.4byte 0x0200ae10
	.4byte 0x0200ae26
	.4byte 0x0200ae4a
	.4byte 0x0200ae56
	.4byte 0x2b007803
	.4byte 0xf7ffd101
	.4byte 0x2080fd1d
	.4byte 0x30060080
	.4byte 0xfdccf000
	.4byte 0x7803e04f
	.4byte 0xd14c2b00
	.4byte 0xfde8f7ff
	.4byte 0x7800e049
	.4byte 0xd1022801
	.4byte 0xfd28f7ff
	.4byte 0x2804e043
	.4byte 0x2080d141
	.4byte 0x30060080
	.4byte 0xfdb0f000
	.4byte 0xd13a2800
	.4byte 0xfeacf7ff
	.4byte 0x7800e037
	.4byte 0xd1022801
	.4byte 0xfe8af7ff
	.4byte 0x2805e031
	.4byte 0xf7ffd12f
	.4byte 0xe02cff21
	.4byte 0x28027800
	.4byte 0xf7ffd102
	.4byte 0xe026fd81
	.4byte 0xd1242804
	.4byte 0xfefaf7ff
	.4byte 0x7800e021
	.4byte 0xd1022802
	.4byte 0xfdfef7ff
	.4byte 0x2805e01b
	.4byte 0x2080d119
	.4byte 0x30060080
	.4byte 0xfd88f000
	.4byte 0xd1122800
	.4byte 0xff20f7ff
	.4byte 0x7803e00f
	.4byte 0xd10c2b03
	.4byte 0xfd8cf7ff
	.4byte 0x7803e009
	.4byte 0xd1012b03
	.4byte 0xfdbef7ff
	.4byte 0x00802080
	.4byte 0xf0003006
	.2byte 0xfd7b
.L_02002e6a:
	pop	{r5, pc}
	.2byte 0xad94
	.2byte 0x0200
	push	{lr}
	movs	r0, #8
	movs	r1, #2
	bl 0x0200ba40
	movs	r0, #9
	movs	r1, #0
	bl 0x0200ba40
	movs	r0, #9
	bl 0x0200baa0
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	movs	r3, #2
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	movs	r3, #0
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
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
	ldr	r3, [pc, #128]
	subs	r2, #36
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #124]
	cmp	r2, r3
	bne.n	.L_02002eee
	bl 0x0200b094
	b.n	.L_02002f58
.L_02002eee:
	ldr	r3, [pc, #116]
	cmp	r2, r3
	bne.n	.L_02002efa
	bl 0x0200b134
	b.n	.L_02002f58
.L_02002efa:
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L_02002f06
	bl 0x0200b20c
	b.n	.L_02002f58
.L_02002f06:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_02002f12
	bl 0x0200b354
	b.n	.L_02002f58
.L_02002f12:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02002f1e
	bl 0x0200b38c
	b.n	.L_02002f58
.L_02002f1e:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02002f2a
	bl 0x0200b504
	b.n	.L_02002f58
.L_02002f2a:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02002f36
	bl 0x0200b598
	b.n	.L_02002f58
.L_02002f36:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02002f42
	bl 0x0200b740
	b.n	.L_02002f58
.L_02002f42:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02002f4e
	bl 0x0200b77c
	b.n	.L_02002f58
.L_02002f4e:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02002f58
	bl 0x0200b80c
.L_02002f58:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000017
	.4byte 0x00000018
	.4byte 0x00000019
	.4byte 0x0000001a
	.4byte 0x0000001b
	.4byte 0x0000001c
	.4byte 0x0000001d
	.4byte 0x0000001e
	.4byte 0x0000001f
	.2byte 0x0021
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x0200b9e8
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #192
	lsls	r3, r3, #12
	adds	r0, r6, #0
	str	r3, [r5, #12]
	bl 0x0200baa0
	adds	r0, r5, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
.L_02002fb2:
	adds	r5, r0, #0
	bl 0x0200baa0
	adds	r0, r5, #0
	bl 0x0200b9e8
	movs	r1, #15
	bl 0x0200ba68
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ba98
	adds	r0, r5, #0
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #10
	adds	r0, #255
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_02003000
	movs	r0, #130
	lsls	r0, r0, #2
	bl 0x0200b950
	cmp	r0, #0
	bne.n	.L_02003008
	bl 0x0200923c
	b.n	.L_02003008
.L_02003000:
	movs	r0, #130
	lsls	r0, r0, #2
	bl 0x0200b958
.L_02003008:
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r5, #0
.L_02003010:
	adds	r0, r5, #0
	adds	r0, #9
	adds	r5, #1
	bl 0x0200afb0
	cmp	r5, #2
	bls.n	.L_02003010
	bl 0x0200afe0
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #100]
	movs	r3, #0
	mov	r8, r3
.L_02003030:
	mov	r5, r8
	adds	r5, #8
	adds	r0, r5, #0
	bl 0x0200b9e8
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200baa0
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ba40
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x0200ba98
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	bl 0x0200b9e8
	movs	r1, #12
	bl 0x0200ba68
	ldr	r3, [r7, #0]
	ldr	r2, [r6, #80]
	str	r3, [r6, #8]
	ldr	r3, [r7, #4]
	str	r3, [r6, #16]
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	adds	r7, #16
	strh	r3, [r2, #18]
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	cmp	r3, #15
	bls.n	.L_02003030
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xbcc0
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	movs	r0, #8
	bl 0x0200b854
	movs	r0, #9
	bl 0x0200b854
	ldrb	r2, [r5, #23]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	movs	r1, #144
	strb	r3, [r5, #23]
	lsls	r1, r1, #3
	ldr	r0, [pc, #60]
	bl 0x0200b928
	bl 0x0200bb18
	movs	r1, #130
	lsls	r1, r1, #1
	movs	r0, #0
	adds	r1, #255
	movs	r2, #10
	movs	r3, #11
	bl 0x0200bb20
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #114
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_020030f6
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ba30
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ba30
.L_020030f6:
	pop	{r5, pc}
	.2byte 0x90e9
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b9e8
	adds	r5, r0, #0
	movs	r0, #11
	bl 0x0200b9e8
	ldr	r3, [r5, #8]
	ldr	r2, [r0, #8]
	adds	r0, #89
	cmp	r2, r3
	ble.n	.L_02003126
	ldrb	r2, [r0, #0]
	movs	r3, #8
	orrs	r3, r2
	b.n	.L_0200312c
.L_02003126:
	ldrb	r2, [r0, #0]
	movs	r3, #247
	ands	r3, r2
.L_0200312c:
	strb	r3, [r0, #0]
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #10
	sub	sp, #8
	bl 0x0200af88
	ldr	r3, [pc, #184]
	movs	r1, #3
	str	r3, [r0, #12]
	movs	r0, #10
	bl 0x0200ba98
	ldr	r0, [pc, #176]
	bl 0x0200bb30
	bl 0x0200ae70
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #164]
	bl 0x0200b928
	movs	r0, #11
	bl 0x0200baa0
	movs	r0, #12
	bl 0x0200baa0
	movs	r0, #12
	bl 0x0200b9e8
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r2, #15
	movs	r3, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #3
	movs	r1, #15
	movs	r2, #1
	movs	r3, #1
	bl 0x0200b9a8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_020031a4
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ba30
	b.n	.L_020031ea
.L_020031a4:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #114
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_020031ea
	movs	r3, #128
	movs	r1, #164
	movs	r2, #200
	lsls	r3, r3, #8
	lsls	r2, r2, #16
	lsls	r1, r1, #17
	movs	r0, #11
	bl 0x0200ba38
	movs	r0, #1
	bl 0x0200b920
	ldr	r1, [pc, #56]
	movs	r0, #11
	bl 0x0200b9f8
	movs	r0, #11
	bl 0x0200b9e8
	movs	r1, #1
	bl 0x0200b9b0
	movs	r0, #11
	bl 0x0200b9e8
	movs	r3, #1
	adds	r0, #89
	strb	r3, [r0, #0]
.L_020031ea:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #24]
	bl 0x0200b928
	add	sp, #8
	pop	{pc}
	.4byte 0xfff00000
	.4byte 0x0200bcbc
	.4byte 0x020091a1
	.4byte 0x0200bc10
	.2byte 0xb0fd
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #9
	sub	sp, #8
	bl 0x0200af88
	movs	r0, #10
	bl 0x0200af88
	movs	r0, #11
	bl 0x0200af88
	movs	r0, #12
	bl 0x0200af88
	movs	r0, #13
	bl 0x0200af88
	movs	r0, #14
	bl 0x0200af88
	movs	r1, #0
	adds	r5, r0, #0
	movs	r0, #9
	bl 0x0200ba40
	movs	r0, #13
	movs	r1, #0
	bl 0x0200ba40
	movs	r0, #14
	movs	r1, #2
	bl 0x0200ba40
	movs	r0, #9
	movs	r1, #1
	bl 0x0200ba98
	movs	r0, #10
	movs	r1, #1
	bl 0x0200ba98
	movs	r0, #11
	movs	r1, #1
	bl 0x0200ba98
	movs	r0, #12
	movs	r1, #1
	bl 0x0200ba98
	movs	r0, #13
	movs	r1, #1
	bl 0x0200ba98
	movs	r0, #14
	movs	r1, #1
	bl 0x0200ba98
	movs	r1, #144
	ldr	r0, [pc, #196]
	lsls	r1, r1, #3
	bl 0x0200b928
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_020032d4
	movs	r1, #204
	movs	r3, #168
	adds	r0, r5, #0
	lsls	r1, r1, #17
	ldr	r2, [pc, #172]
	lsls	r3, r3, #17
	bl 0x0200b980
	ldr	r2, [r5, #80]
	movs	r3, #128
	lsls	r3, r3, #5
	strh	r3, [r2, #18]
	movs	r0, #14
	movs	r1, #3
	bl 0x0200ba98
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r2, #70
	movs	r3, #24
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #50
	movs	r2, #3
	movs	r3, #1
	bl 0x0200b9a8
.L_020032d4:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #76
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_020032f6
	movs	r0, #8
	bl 0x0200b9e8
	movs	r1, #200
	movs	r3, #148
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r3, r3, #17
	bl 0x0200b980
.L_020032f6:
	ldr	r0, [pc, #88]
	bl 0x0200bb30
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #117
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_02003316
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ba30
	b.n	.L_02003336
.L_02003316:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #116
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_02003336
	movs	r3, #128
	movs	r1, #145
	movs	r2, #172
	lsls	r3, r3, #7
	movs	r0, #15
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200ba38
.L_02003336:
	movs	r0, #0
	bl 0x0200bb00
	movs	r0, #1
	bl 0x0200b920
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02009089
	.4byte 0xfff30000
	.2byte 0xbcbc
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	movs	r3, #192
	lsls	r0, r0, #4
	lsls	r3, r3, #18
	adds	r0, #118
	ldr	r5, [r3, #32]
	bl 0x0200b950
	cmp	r0, #0
	bne.n	.L_0200336e
	bl 0x0200b024
.L_0200336e:
	ldrb	r2, [r5, #23]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	movs	r1, #144
	strb	r3, [r5, #23]
	lsls	r1, r1, #3
	ldr	r0, [pc, #4]
	bl 0x0200b928
	pop	{r5, pc}
	.2byte 0x90e9
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #13
	sub	sp, #8
	bl 0x0200af88
	ldr	r3, [pc, #348]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #2
	adds	r5, r0, #0
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	adds	r5, #35
	cmp	r3, r2
	bhi.n	.L_020033c0
	movs	r0, #13
	movs	r1, #3
	bl 0x0200ba98
	ldrb	r2, [r5, #0]
	movs	r3, #2
	orrs	r3, r2
	b.n	.L_020033ce
.L_020033c0:
	movs	r0, #13
	movs	r1, #1
	bl 0x0200ba98
	ldrb	r2, [r5, #0]
	movs	r3, #253
	ands	r3, r2
.L_020033ce:
	strb	r3, [r5, #0]
	movs	r0, #10
	bl 0x02008f18
	movs	r0, #11
	bl 0x02008f18
	movs	r0, #12
	bl 0x02008f18
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_02003402
	movs	r3, #8
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200b9a8
.L_02003402:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_02003424
	movs	r3, #10
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200b9a8
.L_02003424:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_02003446
	movs	r3, #10
	movs	r2, #17
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200b9a8
.L_02003446:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #172]
	bl 0x0200b928
	ldr	r0, [pc, #168]
	bl 0x0200bb30
	bl 0x0200ae70
	movs	r1, #144
	ldr	r0, [pc, #160]
	lsls	r1, r1, #3
	bl 0x0200b928
	movs	r0, #115
	bl 0x0200b950
	cmp	r0, #0
	bne.n	.L_0200347a
	movs	r0, #208
	lsls	r0, r0, #2
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_02003486
.L_0200347a:
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b958
	b.n	.L_020034ee
.L_02003486:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #124
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_020034ce
	movs	r0, #14
	bl 0x0200b9e8
	adds	r5, r0, #0
	movs	r0, #10
	adds	r0, #255
	bl 0x0200b950
	cmp	r0, #0
	bne.n	.L_020034bc
	adds	r3, r5, #0
	adds	r3, #98
	movs	r1, #196
	movs	r2, #148
	strb	r0, [r3, #0]
	lsls	r1, r1, #17
	movs	r0, #14
	lsls	r2, r2, #17
	bl 0x0200ba30
.L_020034bc:
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #14
	bl 0x0200bb10
	b.n	.L_020034ee
.L_020034ce:
	movs	r0, #239
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_020034ee
	movs	r3, #128
	movs	r1, #140
	movs	r2, #164
	lsls	r3, r3, #7
	movs	r0, #14
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200ba38
.L_020034ee:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02009129
	.4byte 0x0200bcbc
	.2byte 0x91a1
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #140]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200b9e8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200b9e8
	movs	r1, #0
	bl 0x0200b9b0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200b950
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200357a
	movs	r1, #248
	movs	r2, #132
	lsls	r2, r2, #17
	movs	r0, #8
	lsls	r1, r1, #16
	bl 0x0200ba30
	movs	r1, #3
	movs	r0, #8
	bl 0x0200ba98
	movs	r0, #8
	bl 0x0200b9e8
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r2, #16
	movs	r3, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200b9a8
	movs	r0, #8
	bl 0x0200baa0
	b.n	.L_0200358c
.L_0200357a:
	movs	r0, #8
	movs	r1, #1
	bl 0x0200ba98
	movs	r0, #8
	bl 0x0200b9e8
	adds	r0, #85
	strb	r5, [r0, #0]
.L_0200358c:
	bl 0x0200b00c
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	movs	r3, #13
	ldrb	r2, [r1, #23]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #23]
	movs	r0, #8
	sub	sp, #8
	bl 0x0200af88
	movs	r0, #9
	bl 0x0200af88
	movs	r0, #10
	bl 0x0200af88
	movs	r0, #11
	bl 0x0200af88
	movs	r0, #12
	bl 0x0200af88
	movs	r0, #13
	bl 0x0200af88
	movs	r0, #14
	bl 0x0200af88
	movs	r0, #15
	bl 0x0200af88
	movs	r0, #16
	bl 0x0200af88
	movs	r0, #13
	movs	r1, #0
	bl 0x0200ba40
	movs	r0, #14
	movs	r1, #0
	bl 0x0200ba40
	movs	r0, #15
	movs	r1, #2
	bl 0x0200ba40
	movs	r0, #16
	movs	r1, #2
	bl 0x0200ba40
	movs	r0, #153
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_02003658
	movs	r3, #13
	str	r3, [sp, #4]
	movs	r5, #19
	movs	r0, #0
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200b9a8
	movs	r3, #33
	str	r3, [sp, #4]
	movs	r1, #0
	movs	r2, #2
	movs	r3, #1
	movs	r0, #40
	str	r5, [sp, #0]
	bl 0x0200b9a8
	movs	r0, #15
	bl 0x0200b9e8
	movs	r1, #160
	lsls	r1, r1, #17
	ldr	r2, [pc, #236]
	ldr	r3, [pc, #240]
	bl 0x0200b980
	movs	r0, #15
	movs	r1, #3
	bl 0x0200ba98
.L_02003658:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #50
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_020036a6
	movs	r3, #22
	str	r3, [sp, #4]
	movs	r5, #24
	movs	r0, #0
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200b9a8
	movs	r1, #0
	movs	r2, #2
	movs	r3, #1
	movs	r0, #40
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200b9a8
	movs	r0, #16
	bl 0x0200b9e8
	movs	r1, #200
	movs	r3, #195
	lsls	r1, r1, #17
	ldr	r2, [pc, #160]
	lsls	r3, r3, #17
	bl 0x0200b980
	movs	r0, #16
	movs	r1, #3
	bl 0x0200ba98
.L_020036a6:
	bl 0x0200bb18
	movs	r1, #130
	lsls	r1, r1, #1
	movs	r0, #0
	adds	r1, #255
	movs	r2, #17
	movs	r3, #18
	bl 0x0200bb20
	movs	r1, #129
	lsls	r1, r1, #2
	movs	r2, #19
	movs	r3, #20
	movs	r0, #1
	bl 0x0200bb20
	movs	r0, #21
	bl 0x0200baa0
	movs	r0, #21
	bl 0x0200b9e8
	movs	r3, #0
	mov	r8, r3
	mov	r3, r8
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #22
	bl 0x0200b9e8
	adds	r5, r0, #0
	movs	r0, #22
	bl 0x0200baa0
	adds	r2, r5, #0
	adds	r2, #35
	ldrb	r3, [r2, #0]
	movs	r6, #2
	orrs	r3, r6
	strb	r3, [r2, #0]
	adds	r5, #85
	mov	r3, r8
	strb	r3, [r5, #0]
	movs	r0, #23
	bl 0x0200baa0
	movs	r0, #23
	bl 0x0200b9e8
	mov	r3, r8
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #24
	bl 0x0200b9e8
	adds	r5, r0, #0
	movs	r0, #24
	bl 0x0200baa0
	adds	r2, r5, #0
	adds	r2, #35
	ldrb	r3, [r2, #0]
	adds	r5, #85
	orrs	r6, r3
	mov	r3, r8
	strb	r6, [r2, #0]
	add	sp, #8
	strb	r3, [r5, #0]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0xffeb0000
	.2byte 0x0000
	.2byte 0x0216
	push	{lr}
	movs	r0, #8
	bl 0x0200b854
	movs	r0, #9
	bl 0x0200b854
	movs	r0, #10
	bl 0x0200b854
	ldr	r3, [pc, #32]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_02003774
	movs	r0, #10
	adds	r0, #255
	bl 0x0200b950
	cmp	r0, #0
	bne.n	.L_02003774
	bl 0x02008994
.L_02003774:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	bne.n	.L_0200379e
	movs	r0, #10
	adds	r0, #255
	bl 0x0200b950
	cmp	r0, #0
	bne.n	.L_0200379e
	bl 0x02008ce8
.L_0200379e:
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #92]
	movs	r6, #7
	ldr	r7, [r3, #0]
	sub	sp, #56
	ands	r7, r6
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_020037fe
	bl 0x0200b938
	movs	r5, #15
	ands	r5, r0
	bl 0x0200b938
	movs	r3, #209
	lsls	r3, r3, #1
	ands	r0, r6
	adds	r3, #255
	add	r6, sp, #16
	strh	r3, [r6, #24]
	mov	r3, r8
	ldr	r4, [r3, #8]
	ldr	r1, [r3, #12]
	ldr	r2, [r3, #16]
	movs	r3, #128
	lsls	r3, r3, #8
	subs	r5, #8
	lsls	r5, r5, #16
	subs	r0, #8
	str	r3, [sp, #0]
	movs	r3, #128
	lsls	r0, r0, #16
	lsls	r3, r3, #13
	adds	r4, r4, r5
	adds	r1, r1, r0
	str	r3, [sp, #8]
	adds	r0, r4, #0
	movs	r3, #0
	str	r7, [sp, #4]
	str	r6, [sp, #12]
	bl 0x020080b8
.L_020037fe:
	add	sp, #56
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, lr}
	movs	r0, #64
	bl 0x0200b9e8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldr	r3, [r2, #8]
	movs	r1, #128
	lsls	r1, r1, #16
	adds	r3, r3, r1
	str	r3, [r2, #8]
	adds	r5, r0, #0
	bl 0x0200b988
	movs	r0, #1
	bl 0x0200b920
	cmp	r5, #0
	beq.n	.L_0200384c
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r3, #132
	lsls	r3, r3, #15
	str	r3, [r5, #12]
	ldr	r3, [pc, #4]
	str	r3, [r5, #108]
.L_0200384c:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xb7a5
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl 0x0200b9e8
	movs	r1, #3
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200ba98
	adds	r0, r5, #0
	bl 0x0200baa0
	adds	r0, r6, #0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	adds	r3, r6, #0
	movs	r1, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r6, #24]
	movs	r3, #230
	lsls	r3, r3, #8
	adds	r3, #102
	str	r3, [r6, #28]
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #208
	lsls	r0, r0, #2
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_02003910
	ldr	r3, [pc, #112]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #86
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	bne.n	.L_020038f4
.L_020038b4:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #65
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_020038d4
	movs	r1, #180
	movs	r2, #216
	movs	r0, #14
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r3, #0
	bl 0x0200ba38
	b.n	.L_020038f4
.L_020038d4:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #66
	bl 0x0200b950
	cmp	r0, #0
	beq.n	.L_020038f4
	movs	r3, #128
	movs	r1, #164
	movs	r2, #216
	lsls	r3, r3, #8
	movs	r0, #14
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200ba38
.L_020038f4:
	movs	r0, #208
	lsls	r0, r0, #2
	bl 0x0200b960
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #65
	bl 0x0200b960
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #66
	bl 0x0200b960
.L_02003910:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.irp EntryTarget, 0x03000528, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000121, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x080200a9, 0x080200c1, 0x080200e9, 0x08020121, 0x08020149, 0x08020179, 0x080201b1, 0x080201e9, 0x08020219, 0x08020229, 0x08020231, 0x08038041, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80b1, 0x080c80b9, 0x080c80c1, 0x080c80c9, 0x080c80d9, 0x080c80f1, 0x080c80f9, 0x080c8101, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8149, 0x080c8159, 0x080c8171, 0x080c8181, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8209, 0x080c8211, 0x080c8219, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8259, 0x080c8279, 0x080c8289, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c8481, 0x080c84e1, 0x080c8601, 0x080c86a9, 0x080c86e9, 0x080c8711, 0x080c8719, 0x080c8721, 0x080c8729, 0x081c0011
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
	.4byte 0x0200bb50
	.4byte 0x0200bb8c
	.4byte 0x0200bbc8
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x017a0000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0xffff0008
	.4byte 0x013c0000
	.4byte 0x00880000
	.4byte 0xffff0000
	.4byte 0x00008000
	.4byte 0x013e0000
	.4byte 0x008e0000
	.4byte 0x00010000
	.4byte 0x00007000
	.4byte 0x01440000
	.4byte 0x009a0000
	.4byte 0xffff0000
	.4byte 0x00006000
	.4byte 0x014a0000
	.4byte 0x009e0000
	.4byte 0x00010000
	.4byte 0x00005000
	.4byte 0x01520000
	.4byte 0x00a60000
	.4byte 0xffff0000
	.4byte 0x00004800
	.4byte 0x01580000
	.4byte 0x00a60000
	.4byte 0x00010000
	.4byte 0x00004800
	.4byte 0x01200000
	.4byte 0x00f00000
	.4byte 0x00010000
	.4byte 0x0000d800
	.4byte 0x011c0000
	.4byte 0x00ec0000
	.4byte 0xffff0000
	.4byte 0x0000d800
	.4byte 0x01100000
	.4byte 0x00e80000
	.4byte 0x00010000
	.4byte 0x0000c800
	.4byte 0x010a0000
	.4byte 0x00e60000
	.4byte 0xffff0000
	.4byte 0x0000c800
	.4byte 0x01000000
	.4byte 0x00ee0000
	.4byte 0x00010000
	.4byte 0x0000b000
	.4byte 0x00f80000
	.4byte 0x00ee0000
	.4byte 0xffff0000
	.4byte 0x0000b800
	.4byte 0x00f00000
	.4byte 0x00fa0000
	.4byte 0x00010000
	.4byte 0x0000a800
	.4byte 0x00ea0000
	.4byte 0x00fc0000
	.4byte 0xffff0000
	.4byte 0x0000b000
	.4byte 0x00e00000
	.4byte 0x01060000
	.4byte 0x00010000
	.4byte 0x0000a800
	.4byte 0x00dc0000
	.4byte 0x01060000
	.4byte 0xffff0000
	.4byte 0x0000a800
	.4byte 0x0000ffff
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
	.4byte 0x00200100
	.4byte 0x011000c0
	.4byte 0x00d00030
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffda02d0
	.4byte 0x02e000ee
	.4byte 0x00feffea
	.4byte 0x0001ffff
	.4byte 0x00200360
	.4byte 0x037000d0
	.4byte 0x00e00030
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffda0310
	.4byte 0x032000a0
	.4byte 0x00b0ffea
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00105019
	.4byte 0x00201018
	.4byte 0x00305002
	.4byte 0x00000018
	.4byte 0x00102017
	.4byte 0x00000019
	.4byte 0x0010201d
	.4byte 0x0020101e
	.4byte 0x0030201e
	.4byte 0x0040101a
	.4byte 0x00501017
	.4byte 0x0000001a
	.4byte 0x00104019
	.4byte 0x0020201b
	.4byte 0x0030401b
	.4byte 0x0040301b
	.4byte 0x0000001b
	.4byte 0x0010101c
	.4byte 0x0020201a
	.4byte 0x0030401a
	.4byte 0x0040301a
	.4byte 0x0000001c
	.4byte 0x0010101b
	.4byte 0x0020101d
	.4byte 0x0000001d
	.4byte 0x0010201c
	.4byte 0x00201019
	.4byte 0x0000001e
	.4byte 0x00102019
	.4byte 0x00203019
	.4byte 0x0030101f
	.4byte 0x0000001f
	.4byte 0x0010301e
	.4byte 0x00228020
	.4byte 0x00000020
	.4byte 0x0010201f
	.4byte 0x00228021
	.4byte 0x00000021
	.4byte 0x00129020
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0129
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0129
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0xffff0012
	.4byte 0x00000001
	.4byte 0x01540000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00028000
	.4byte 0xffff0011
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00020000
	.4byte 0xffff0038
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
	.4byte 0xffff014f
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00024000
	.4byte 0xffff0012
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
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0172
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff014f
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0x007300f6
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0172
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x01024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0114
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0114
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0114
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0114
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0129
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00ca0000
	.4byte 0x00024000
	.4byte 0xffff0129
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x012a0000
	.4byte 0x01024000
	.4byte 0xffff0129
	.4byte 0x00000001
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0x00da0000
	.4byte 0x01024000
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
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00008515
	.4byte 0x0203000a
	.4byte 0x02009d25
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x0200ae8d
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x0200aea9
	.4byte 0x00000002
	.4byte 0x0872000a
	.4byte 0x02009a3d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte 0x020084e1
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000017c6
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x10008c15
	.4byte 0x084c0008
	.4byte 0x02008fa9
	.4byte 0x00008c15
	.4byte 0x084c0008
	.4byte 0x02008fed
	.4byte 0x00000002
	.4byte 0x020a000a
	.4byte 0x02009291
	.4byte 0x00000002
	.4byte 0x120a000b
	.4byte 0x020092b5
	.4byte 0x00000002
	.4byte 0x0230000f
	.4byte 0x02009475
	.4byte 0x00000002
	.4byte 0x0874000b
	.4byte 0x0200a385
	.4byte 0x00000002
	.4byte 0x0875000c
	.4byte 0x0200a431
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00001815
	.4byte 0x0200000a
	.4byte 0x02008f35
	.4byte 0x00001815
	.4byte 0x0201000b
	.4byte 0x02008f59
	.4byte 0x00001815
	.4byte 0x0202000c
	.4byte 0x02008f81
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008fa9
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008fb9
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte 0x02008fa9
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02008fb9
	.4byte 0x00000002
	.4byte 0x0877000a
	.4byte 0x0200a4e5
	.4byte 0x00000002
	.4byte 0x0877000b
	.4byte 0x0200a5fd
	.4byte 0x00000002
	.4byte 0x087c000c
	.4byte 0x0200a6f9
	.4byte 0x00000002
	.4byte 0x0205000d
	.4byte 0x0200ad51
	.4byte 0x00000002
	.4byte 0x0205000e
	.4byte 0x0200ad51
	.4byte 0x00000002
	.4byte 0x0205000f
	.4byte 0x0200ad51
	.4byte 0x00000002
	.4byte 0x02050010
	.4byte 0x0200ad51
	.4byte 0x00000002
	.4byte 0x02050011
	.4byte 0x0200ad51
	.4byte 0x00000002
	.4byte 0x02050012
	.4byte 0x0200ad51
	.4byte 0x00000002
	.4byte 0x02050013
	.4byte 0x0200ad51
	.4byte 0x00000002
	.4byte 0x02050014
	.4byte 0x0200ad51
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008281
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000009
	.4byte 0x03010000
	.4byte 0x02009039
	.4byte 0x00008c15
	.4byte 0x03010008
	.4byte 0x02009039
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00008515
	.4byte 0x02030011
	.4byte 0x00000000
	.4byte 0x00008515
	.4byte 0x02040013
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x02310005
	.4byte 0x020096d9
	.4byte 0x00000002
	.4byte 0x02320006
	.4byte 0x020098bd
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x02008555
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x02008555
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x02008555
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0xffff000a
	.4byte 0x020084b1
	.4byte 0x0001ca04
	.4byte 0xffff000a
	.4byte 0x02008689
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x020099dd
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02009a0d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0xffff0001
	.4byte 0x020084b1
	.4byte 0x0001ca04
	.4byte 0xffff0001
	.4byte 0x02008e39
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
