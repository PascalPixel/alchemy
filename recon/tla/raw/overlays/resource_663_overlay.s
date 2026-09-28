.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200aca1, 0x020084b9, 0x02008501, 0x02008509, 0x020087e5, 0x020084c1, 0x0200ba7d
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
	bl 0x0200bb40
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
	bl 0x0200bb88
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x0200bc00
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bb90
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
	bl 0x0200bb40
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
	bl 0x0200bb88
	adds	r0, r5, #0
	movs	r1, #15
	bl 0x0200bc00
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
	bl 0x0200bbd8
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
	bl 0x0200bb40
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
	bl 0x0200bb28
	ldr	r2, [pc, #352]
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl 0x0200bb38
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200bb88
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
	bl 0x0200bc00
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
	bl 0x0200baa0
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
	bl 0x0200baa0
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020002c8:
	bl 0x0200baa0
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
	bl 0x0200bb28
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200bb38
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
	.4byte 0x0200bda0
	.4byte 0x02008125
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r1, #0
	bl 0x0200bb88
	movs	r0, #0
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_02000358
	movs	r0, #0
	b.n	.L_0200037e
.L_02000358:
	cmp	r0, #2
	bhi.n	.L_0200036c
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_0200036e
.L_0200036c:
	ldr	r4, [pc, #16]
.L_0200036e:
	lsls	r3, r2, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
.L_0200037e:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r1, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_02000398
	movs	r0, #0
	b.n	.L_020003c4
.L_02000398:
	cmp	r0, #2
	bhi.n	.L_020003ac
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_020003ae
.L_020003ac:
	ldr	r4, [pc, #24]
.L_020003ae:
	lsls	r3, r2, #7
	adds	r3, r5, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
	asrs	r3, r1, #8
	strb	r3, [r4, #2]
	strb	r1, [r4, #3]
.L_020003c4:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r5, #60
.L_020003d2:
	cmp	r5, #0
	beq.n	.L_020003e4
	movs	r0, #1
	bl 0x0200bab0
	ldr	r3, [r6, #40]
	subs	r5, #1
	cmp	r3, #0
	bne.n	.L_020003d2
.L_020003e4:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	b.n	.L_02000458
.L_020003ee:
	ldrh	r0, [r6, #0]
	bl 0x0200bbd8
	adds	r5, r0, #0
	adds	r7, r5, #0
	movs	r3, #0
	adds	r7, #99
	strb	r3, [r7, #0]
	movs	r0, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	bl 0x02008348
	movs	r3, #64
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_02000456
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	cmp	r0, #0
	beq.n	.L_02000464
	movs	r3, #212
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #12]
	ldr	r1, [r5, #8]
	subs	r2, r2, r3
	asrs	r2, r2, #20
	adds	r3, r2, #1
	asrs	r1, r1, #20
	ldr	r4, [r0, #0]
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	subs	r2, #1
	movs	r3, #255
	strb	r3, [r4, #2]
	lsls	r2, r2, #7
	ldr	r4, [r0, #0]
	adds	r1, r1, r2
	lsls	r1, r1, #2
	movs	r3, #1
	negs	r3, r3
	adds	r4, r4, r1
	strb	r3, [r4, #2]
	movs	r3, #1
	strb	r3, [r7, #0]
.L_02000456:
	adds	r6, #2
.L_02000458:
	movs	r2, #255
	ldrh	r3, [r6, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_020003ee
.L_02000464:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r2, #99
	adds	r2, r2, r5
	ldrb	r3, [r2, #0]
	mov	ip, r2
	cmp	r3, #0
	beq.n	.L_020004b6
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	cmp	r0, #0
	beq.n	.L_020004b6
	movs	r3, #212
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #12]
	ldr	r1, [r5, #8]
	subs	r2, r2, r3
	asrs	r2, r2, #20
	adds	r3, r2, #1
	asrs	r1, r1, #20
	ldr	r4, [r0, #0]
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	subs	r2, #1
	movs	r3, #0
	strb	r3, [r4, #2]
	lsls	r2, r2, #7
	ldr	r4, [r0, #0]
	adds	r1, r1, r2
	lsls	r1, r1, #2
	adds	r4, r4, r1
	mov	r2, ip
	strb	r3, [r4, #2]
	strb	r3, [r2, #0]
.L_020004b6:
	pop	{r5, pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xbe54
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
	bne.n	.L_020004d8
	ldr	r0, [pc, #24]
	b.n	.L_020004e4
.L_020004d8:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020004e2
	ldr	r0, [pc, #24]
	b.n	.L_020004e4
.L_020004e2:
	ldr	r0, [pc, #24]
.L_020004e4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000052
	.4byte 0x0200be84
	.4byte 0x00000053
	.4byte 0x0200bec4
	.2byte 0xbf44
	.2byte 0x0200
	.2byte 0x4800
	bx	lr
	.2byte 0xbff4
	.2byte 0x0200
	pushal	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000520
	ldr	r0, [pc, #24]
	b.n	.L_0200052c
.L_02000520:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_0200052a
	ldr	r0, [pc, #24]
	b.n	.L_0200052c
.L_0200052a:
	ldr	r0, [pc, #24]
.L_0200052c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000052
	.4byte 0x0200c0ec
	.4byte 0x00000053
	.4byte 0x0200c194
	.2byte 0xc314
	.2byte 0x0200
	push	{lr}
	bl 0x0200bbc8
	movs	r0, #0
	bl 0x0200bc88
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200bbb0
	bl 0x0200bbd0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x1a94
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	bl 0x0200bbc8
	movs	r0, #0
	bl 0x0200bc88
	movs	r0, #158
	bl 0x0200bcd8
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #64
	movs	r2, #83
	movs	r3, #35
	movs	r0, #64
	bl 0x0200bb68
	movs	r0, #0
	bl 0x0200bb70
	movs	r0, #45
	bl 0x0200bbc0
	ldr	r5, [pc, #72]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200bbe0
	movs	r1, #156
	movs	r2, #170
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	ldr	r0, [r5, #0]
	bl 0x0200bbe8
	movs	r0, #5
	bl 0x0200bbc0
	bl 0x0200bc70
	bl 0x0200bc78
	ldr	r0, [r5, #0]
	bl 0x0200bbf0
	movs	r0, #123
	bl 0x0200bcd8
	movs	r0, #4
	bl 0x0200bc48
	bl 0x0200bbd0
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #10
	bl 0x0200bbd8
	adds	r5, r0, #0
	bl 0x0200bbc8
	movs	r0, #0
	bl 0x0200bc88
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #31
	bne.n	.L_02000612
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200bb18
	b.n	.L_0200061a
.L_02000612:
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200bb20
.L_0200061a:
	bl 0x0200bbd0
	pop	{r5, pc}
	push	{r5, r6, lr}
	movs	r0, #11
	sub	sp, #8
	bl 0x0200bbd8
	movs	r1, #70
	movs	r2, #67
	adds	r5, r0, #0
	movs	r0, #3
	bl 0x02008348
	adds	r6, r0, #0
	bl 0x0200bbc8
	movs	r0, #0
	bl 0x0200bc88
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #32
	bne.n	.L_02000672
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #17
	bl 0x0200bb18
	movs	r3, #255
	ands	r6, r3
	movs	r1, #70
	movs	r2, #67
	adds	r3, r6, #0
	movs	r0, #4
	bl 0x02008384
	movs	r0, #2
	movs	r1, #33
	movs	r2, #54
	adds	r3, r6, #0
	bl 0x02008384
	b.n	.L_0200069c
.L_02000672:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #17
	bl 0x0200bb20
	movs	r3, #255
	ands	r6, r3
	lsls	r3, r3, #8
	orrs	r6, r3
	movs	r1, #70
	movs	r2, #67
	adds	r3, r6, #0
	movs	r0, #4
	bl 0x02008384
	movs	r0, #2
	movs	r1, #33
	movs	r2, #54
	adds	r3, r6, #0
	bl 0x02008384
.L_0200069c:
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bbd8
	ldr	r3, [r0, #12]
	asrs	r3, r3, #19
	cmp	r3, #26
	bne.n	.L_020006c6
	movs	r3, #33
	movs	r2, #54
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #70
	movs	r1, #67
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bb80
.L_020006c6:
	bl 0x0200bbd0
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #13
	bl 0x0200bbd8
	adds	r5, r0, #0
	bl 0x0200bbc8
	movs	r0, #0
	bl 0x0200bc88
	ldr	r3, [r5, #8]
	asrs	r3, r3, #19
	cmp	r3, #23
	bne.n	.L_020006fa
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #121
	bl 0x0200bb18
.L_020006fa:
	bl 0x0200bbd0
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r5, [pc, #208]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r5, r5, r0
	ldr	r0, [r5, #0]
	bl 0x0200bbd8
	adds	r7, r0, #0
	movs	r0, #195
	lsls	r0, r0, #1
	ldr	r6, [r7, #16]
	bl 0x0200bcd8
	movs	r1, #6
	adds	r0, r7, #0
	bl 0x0200bb28
	movs	r0, #10
	bl 0x0200bab0
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200bb28
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200bb88
	movs	r1, #85
	adds	r1, r1, r7
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	movs	r0, #0
	strb	r3, [r1, #0]
	mov	sl, r0
	movs	r3, #128
.L_02000752:
	movs	r0, #192
	lsls	r3, r3, #11
	lsls	r0, r0, #12
	ldr	r2, [r7, #12]
	mov	r8, r1
	str	r3, [r7, #40]
	ldr	r1, [r7, #8]
	adds	r3, r6, r0
	adds	r0, r7, #0
	bl 0x0200bb58
	movs	r0, #6
	bl 0x0200bab0
	movs	r0, #217
	bl 0x0200bcd8
	movs	r1, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200bc08
	mov	r1, sl
	mov	r2, r8
	strb	r1, [r2, #0]
	movs	r5, #0
.L_02000788:
	ldr	r3, [r7, #12]
	ldr	r0, [pc, #84]
	adds	r5, #1
	adds	r3, r3, r0
	str	r3, [r7, #12]
	str	r3, [r7, #60]
	movs	r0, #1
	bl 0x0200bab0
	cmp	r5, #13
	bls.n	.L_02000788
	mov	r1, r8
	movs	r3, #3
	strb	r3, [r1, #0]
	movs	r0, #128
	movs	r3, #192
	lsls	r3, r3, #10
	lsls	r0, r0, #13
	ldr	r2, [r7, #12]
	ldr	r1, [r7, #8]
	str	r3, [r7, #40]
	adds	r3, r6, r0
	adds	r0, r7, #0
	bl 0x0200bb58
	adds	r0, r7, #0
	bl 0x0200bb60
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200bb88
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200bcd8
	bl 0x0200bc98
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb500
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_020007fc
	ldr	r0, [pc, #24]
	b.n	.L_02000808
.L_020007fc:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000806
	ldr	r0, [pc, #24]
	b.n	.L_02000808
.L_02000806:
	ldr	r0, [pc, #24]
.L_02000808:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000052
	.4byte 0x0200c3d4
	.4byte 0x00000053
	.4byte 0x0200c4d0
	.2byte 0xc59c
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	bl 0x0200bc88
	pop	{pc}
	.2byte 0x0000
	.4byte 0x049b23c0
	.4byte 0x681b33e0
	.4byte 0x33342201
	.2byte 0x701a
	.2byte 0x4770
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	adds	r5, r0, #0
	ldr	r6, [r5, #12]
	ldr	r0, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #13
	asrs	r6, r6, #1
	adds	r6, r6, r3
	movs	r3, #255
	ands	r0, r3
	lsls	r0, r0, #11
	mov	r8, r3
	bl 0x0200bad8
	ldr	r3, [pc, #68]
	adds	r1, r6, #0
	mov	sl, r3
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x6c6b
	adds	r3, r3, r0
	ldr	r0, [r5, #48]
	str	r3, [r5, #8]
	mov	r3, r8
	ands	r0, r3
	lsls	r0, r0, #11
	bl 0x0200bad0
	adds	r1, r6, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x2103
	bl 0x0200baa0
	ldr	r3, [r5, #76]
	ldr	r2, [r5, #72]
	adds	r3, r3, r0
	str	r3, [r5, #16]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r3, [r5, #48]
	adds	r3, #1
	str	r3, [r5, #48]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	adds	r5, r0, #0
	ldr	r6, [r5, #12]
	ldr	r0, [r5, #48]
	movs	r3, #192
	lsls	r3, r3, #12
	asrs	r6, r6, #2
	adds	r6, r6, r3
	movs	r3, #255
	ands	r0, r3
	lsls	r0, r0, #11
	mov	r8, r3
	bl 0x0200bad8
	ldr	r3, [pc, #84]
	adds	r1, r6, #0
	mov	sl, r3
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x6c6b
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r5, #48]
	str	r3, [r5, #8]
	mov	r3, r8
	ands	r0, r3
	lsls	r0, r0, #11
	bl 0x0200bad0
	adds	r1, r6, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x6ceb
	ldr	r2, [r5, #72]
	adds	r3, r3, r0
	str	r3, [r5, #16]
	ldr	r3, [r5, #12]
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r3, [r5, #48]
	movs	r2, #2
	adds	r3, #1
	str	r3, [r5, #48]
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	ands	r3, r2
	lsrs	r3, r3, #1
	lsls	r1, r3, #3
	adds	r1, r1, r3
	bl 0x0200bc00
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0300021c
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	ldr	r3, [pc, #40]
	movs	r2, #3
	ldr	r3, [r3, #0]
	ands	r3, r2
	adds	r2, r0, #0
	adds	r2, #99
	ldrb	r2, [r2, #0]
	lsrs	r3, r2
	movs	r2, #1
	ands	r3, r2
	adds	r2, r0, #0
	adds	r2, #98
	ldrb	r2, [r2, #0]
	adds	r1, r2, #0
	muls	r1, r3
	lsls	r1, r1, #24
	lsrs	r1, r1, #24
	bl 0x0200bc00
	pop	{pc}
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
	ldr	r3, [pc, #864]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #128
	bl 0x0200bbd8
	movs	r3, #192
.L_0200097c:
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r1, #0
	str	r1, [sp, #16]
	mov	r8, r3
	mov	sl, r0
	mov	r9, r1
	bl 0x0200bbc8
	movs	r0, #0
	bl 0x0200bc88
	movs	r0, #207
	bl 0x0200bcd8
	mov	r5, sl
	mov	r2, sl
	movs	r0, #140
	ldr	r3, [r5, #16]
	ldr	r1, [r2, #8]
	lsls	r0, r0, #1
	ldr	r2, [r2, #12]
	bl 0x0200bb40
	movs	r1, #2
	adds	r7, r0, #0
	bl 0x0200bb28
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200bb88
	ldr	r3, [pc, #784]
	add	r0, sp, #16
	str	r3, [r7, #24]
	movs	r3, #204
	lsls	r3, r3, #6
	ldrb	r0, [r0, #0]
	adds	r3, #51
	str	r3, [r7, #28]
	adds	r3, r7, #0
	adds	r3, #85
	mov	r1, r8
	strb	r0, [r3, #0]
	ldr	r0, [r1, #20]
	ldr	r4, [r7, #80]
	ldr	r3, [r0, #80]
	ldrb	r1, [r4, #9]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
.L_020009e6:
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r4, #9]
	ldr	r5, [pc, #740]
	ldr	r3, [r0, #16]
.L_020009f2:
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	adds	r3, r3, r5
	adds	r0, r7, #0
	bl 0x0200bb58
	movs	r0, #166
	ldr	r3, [r7, #28]
	lsls	r0, r0, #9
	adds	r0, #203
	cmp	r3, r0
	bgt.n	.L_02000a26
.L_02000a0a:
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #153
	adds	r3, r3, r1
.L_02000a12:
	str	r3, [r7, #28]
	movs	r0, #1
	bl 0x0200bbc0
	movs	r2, #166
	ldr	r3, [r7, #28]
.L_02000a1e:
	lsls	r2, r2, #9
	adds	r2, #203
	cmp	r3, r2
	ble.n	.L_02000a0a
.L_02000a26:
	adds	r0, r7, #0
	mov	r5, r8
	bl 0x0200bb60
	ldr	r3, [r5, #20]
	ldr	r0, [pc, #672]
	ldr	r3, [r3, #16]
	movs	r1, #230
	adds	r3, r3, r0
	str	r3, [r7, #16]
	ldr	r3, [r7, #28]
	lsls	r1, r1, #9
	adds	r1, #203
	cmp	r3, r1
	bgt.n	.L_02000b00
.L_02000a44:
	ldr	r2, [pc, #656]
	movs	r3, #3
	ldr	r6, [r2, #0]
	mov	fp, r2
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_02000aba
	mov	r5, r8
	ldr	r3, [r5, #20]
	movs	r0, #168
	ldr	r1, [r3, #8]
	movs	r2, #0
	ldr	r3, [r3, #16]
	lsls	r0, r0, #2
	bl 0x0200bb40
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	ldr	r3, [r5, #8]
	str	r3, [r5, #68]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r5, #72]
	ldr	r3, [r5, #16]
	str	r3, [r5, #76]
	bl 0x0200bac8
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #48]
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200bb28
	adds	r0, r5, #0
	ldr	r1, [pc, #568]
	bl 0x0200bb38
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bb88
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200bc00
	ldr	r3, [pc, #552]
	str	r3, [r5, #108]
.L_02000aba:
	mov	r0, fp
	ldr	r3, [r0, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000ae4
	ldr	r3, [r7, #24]
	movs	r2, #200
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	str	r3, [r7, #24]
	ldr	r3, [r7, #28]
	mov	r1, r8
	adds	r3, r3, r2
	ldr	r0, [r1, #20]
	str	r3, [r7, #28]
	movs	r1, #15
	bl 0x0200bc00
	b.n	.L_02000aee
.L_02000ae4:
	mov	r2, r8
	ldr	r0, [r2, #20]
	movs	r1, #4
	bl 0x0200bc00
.L_02000aee:
	movs	r0, #1
	bl 0x0200bbc0
	movs	r5, #230
	ldr	r3, [r7, #28]
	lsls	r5, r5, #9
	adds	r5, #203
	cmp	r3, r5
	ble.n	.L_02000a44
.L_02000b00:
	mov	r1, r8
	ldr	r0, [r1, #20]
	movs	r1, #0
	bl 0x0200bc00
	movs	r0, #136
	bl 0x0200bcd8
	mov	r3, r8
	ldr	r2, [r3, #20]
	mov	r0, sl
	ldr	r3, [r2, #8]
	ldrh	r5, [r0, #6]
	asrs	r3, r3, #20
	str	r3, [sp, #20]
	ldr	r3, [r2, #16]
	movs	r1, #128
	asrs	r3, r3, #20
	str	r3, [sp, #24]
	lsls	r1, r1, #6
	movs	r3, #192
	adds	r5, r5, r1
	lsls	r3, r3, #8
	ands	r5, r3
	adds	r0, r5, #0
	bl 0x0200bad8
	asrs	r0, r0, #16
	str	r0, [sp, #28]
	adds	r0, r5, #0
	bl 0x0200bad0
	asrs	r0, r0, #16
	str	r0, [sp, #32]
.L_02000b44:
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #24]
	movs	r0, #2
	bl 0x02008348
	ldr	r3, [sp, #20]
	ldr	r5, [sp, #28]
	mov	fp, r0
	adds	r1, r3, r5
	ldr	r0, [sp, #24]
	mov	r2, fp
	ldr	r3, [sp, #32]
	asrs	r2, r2, #8
	mov	fp, r2
	movs	r5, #2
	adds	r2, r0, r3
	mov	r0, fp
	str	r1, [sp, #20]
	str	r2, [sp, #24]
	add	r9, r5
	cmp	r0, #0
	beq.n	.L_02000b44
	movs	r0, #2
	bl 0x02008348
	adds	r1, r0, #0
	asrs	r1, r1, #8
	subs	r1, #1
	str	r1, [sp, #16]
	movs	r2, #2
	ldr	r0, [sp, #28]
	mov	r3, r8
	negs	r2, r2
	add	r9, r2
	ldr	r2, [r3, #20]
	mov	r3, r9
	muls	r3, r0
	movs	r5, #10
	ldrsh	r6, [r2, r5]
	movs	r1, #18
	ldrsh	r5, [r2, r1]
	ldr	r2, [sp, #32]
	lsls	r3, r3, #3
	ldr	r1, [r7, #80]
	adds	r6, r6, r3
	mov	r3, r9
	muls	r3, r2
	ldrb	r2, [r1, #9]
	lsls	r3, r3, #3
	adds	r5, r5, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r7, #48]
	str	r3, [r7, #52]
	adds	r0, r5, #0
	adds	r3, r6, #0
	ldr	r2, [r7, #12]
	lsls	r1, r3, #16
	lsls	r3, r0, #16
	adds	r0, r7, #0
	str	r6, [sp, #20]
	str	r5, [sp, #24]
	bl 0x0200bb58
	movs	r0, #160
	movs	r1, #160
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	bl 0x0200bc20
	lsls	r6, r6, #16
	lsls	r5, r5, #16
	movs	r1, #1
	adds	r0, r6, #0
	negs	r1, r1
	adds	r2, r5, #0
	movs	r3, #1
	bl 0x0200bc28
	b.n	.L_02000c6c
.L_02000bf0:
	ldr	r1, [pc, #228]
	movs	r3, #1
	ldr	r0, [r1, #0]
	mov	r9, r1
	mov	sl, r0
	mov	r2, sl
	ands	r2, r3
	mov	sl, r2
	cmp	r2, #0
	bne.n	.L_02000c66
	add	r6, sp, #76
	str	r3, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #162
	strh	r3, [r6, #24]
	movs	r3, #2
	str	r3, [r6, #4]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	lsls	r0, r0, #12
	mov	r8, r3
	bl 0x0200bad8
	add	r5, sp, #116
	lsls	r0, r0, #1
	str	r0, [r5, #0]
	bl 0x0200bac8
	ldr	r3, [r7, #12]
	movs	r2, #31
	ands	r2, r0
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r5, #4]
	mov	r1, r9
	ldr	r0, [r1, #0]
	mov	r2, r8
	ands	r0, r2
	lsls	r0, r0, #12
	bl 0x0200bad0
	str	r0, [r5, #8]
	ldr	r4, [r7, #8]
	ldr	r1, [r5, #4]
	ldr	r3, [r5, #0]
	ldr	r2, [r7, #16]
	str	r0, [sp, #4]
	movs	r0, #152
	lsls	r0, r0, #13
	mov	r5, sl
	str	r0, [sp, #8]
	adds	r0, r4, #0
	str	r5, [sp, #0]
	str	r6, [sp, #12]
	bl 0x0200815c
.L_02000c66:
	movs	r0, #1
	bl 0x0200bbc0
.L_02000c6c:
	adds	r0, r7, #0
	bl 0x0200bba8
	cmp	r0, #0
	beq.n	.L_02000bf0
	mov	r0, fp
	cmp	r0, #255
	bne.n	.L_02000c84
	movs	r0, #136
	bl 0x0200bcd8
	b.n	.L_02000e78
.L_02000c84:
	ldr	r5, [sp, #28]
	movs	r1, #10
	ldrsh	r2, [r7, r1]
	lsls	r3, r5, #1
	ldr	r1, [sp, #32]
	adds	r3, r3, r5
	lsls	r3, r3, #4
	adds	r3, r2, r3
	str	r3, [sp, #20]
	movs	r0, #18
	ldrsh	r2, [r7, r0]
	lsls	r3, r1, #1
	adds	r3, r3, r1
	lsls	r3, r3, #4
	adds	r3, r2, r3
	str	r3, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r5, [sp, #24]
	movs	r3, #128
	lsls	r3, r3, #8
	lsls	r1, r2, #16
	str	r3, [r7, #48]
	str	r3, [r7, #52]
	ldr	r2, [r7, #12]
	lsls	r3, r5, #16
	adds	r0, r7, #0
	bl 0x0200bb58
	movs	r0, #235
	bl 0x0200bcd8
	movs	r0, #30
	negs	r0, r0
	add	fp, r0
	b.n	.L_02000e36
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0001b333
	.4byte 0xffff0000
	.4byte 0x0300122c
	.4byte 0x0200bd1c
	.2byte 0x8841
	.2byte 0x0200
.L_02000ce4:
	ldr	r1, [pc, #504]
	movs	r2, #1
	ldr	r3, [r1, #0]
	mov	r9, r1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000d5a
	add	r3, sp, #36
	str	r2, [r3, #0]
	mov	r8, r3
	bl 0x0200bac8
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r5, #128
	lsls	r5, r5, #9
	adds	r3, #255
	mov	sl, r5
	ands	r3, r0
	add	r3, sl
	mov	r0, r8
	str	r3, [r0, #12]
	str	r3, [r0, #8]
	mov	r1, r9
	ldr	r0, [r1, #0]
	movs	r6, #128
	lsls	r6, r6, #1
	adds	r6, #255
	ands	r0, r6
	lsls	r0, r0, #12
	bl 0x0200bad8
	mov	r2, sl
	add	r5, sp, #116
	lsls	r0, r0, #1
	str	r2, [r5, #4]
	str	r0, [r5, #0]
	mov	r3, r9
	ldr	r0, [r3, #0]
	ands	r0, r6
	lsls	r0, r0, #12
	bl 0x0200bad0
	str	r0, [r5, #8]
	ldr	r6, [r7, #8]
	ldr	r3, [r5, #0]
	ldr	r4, [r5, #4]
	ldr	r1, [r7, #12]
	ldr	r2, [r7, #16]
	str	r0, [sp, #4]
	movs	r0, #160
	lsls	r0, r0, #12
	str	r0, [sp, #8]
	mov	r5, r8
	adds	r0, r6, #0
	str	r4, [sp, #0]
	str	r5, [sp, #12]
	bl 0x0200815c
.L_02000d5a:
	mov	r0, r9
	ldr	r3, [r0, #0]
	movs	r4, #3
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_02000e30
	ldr	r6, [pc, #380]
	mov	r1, fp
	lsls	r5, r1, #3
	adds	r3, r5, #4
	ldrsh	r2, [r6, r3]
	ldr	r3, [r7, #8]
	ldrh	r0, [r6, r5]
	asrs	r3, r3, #20
	adds	r2, r2, r3
	adds	r3, r5, #6
	mov	sl, r2
	str	r2, [sp, #20]
	ldrsh	r2, [r6, r3]
	ldr	r3, [r7, #16]
	adds	r1, r5, #2
	asrs	r3, r3, #20
	adds	r2, r2, r3
	mov	r9, r2
	str	r2, [sp, #24]
	mov	r3, fp
	movs	r2, #1
	ands	r3, r2
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_02000de0
	ldrsh	r1, [r6, r1]
	lsls	r0, r0, #16
	movs	r2, #1
	mov	r3, r9
	subs	r3, #1
	str	r2, [sp, #0]
	asrs	r0, r0, #16
	mov	r2, sl
	str	r4, [sp, #4]
	bl 0x0200bb68
	mov	r2, r8
	ldrsh	r1, [r6, r2]
	ldrsh	r0, [r6, r5]
	mov	r2, sl
	mov	r3, r9
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #1
	movs	r3, #2
.L_02000dc0:
	bl 0x0200bb80
	mov	r2, r8
	ldrsh	r0, [r6, r5]
	ldrsh	r1, [r6, r2]
	ldr	r2, [r7, #8]
	ldr	r3, [r7, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #1
	movs	r3, #2
	bl 0x0200bb80
	b.n	.L_02000e2c
.L_02000de0:
	mov	r2, r8
	ldrsh	r1, [r6, r2]
	mov	r2, fp
	asrs	r3, r2, #1
	ldr	r2, [sp, #24]
	lsls	r0, r0, #16
	subs	r3, r2, r3
	movs	r2, #1
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	asrs	r0, r0, #16
	mov	r2, sl
	bl 0x0200bb68
	mov	r2, r8
	ldrsh	r1, [r6, r2]
	ldrsh	r0, [r6, r5]
	mov	r2, sl
	mov	r3, r9
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bb80
	mov	r2, r8
	ldrsh	r0, [r6, r5]
	ldrsh	r1, [r6, r2]
	ldr	r2, [r7, #8]
	ldr	r3, [r7, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bb80
.L_02000e2c:
	bl 0x0200bb50
.L_02000e30:
	movs	r0, #1
	bl 0x0200bbc0
.L_02000e36:
	adds	r0, r7, #0
	bl 0x0200bba8
	cmp	r0, #0
	bne.n	.L_02000e42
	b.n	.L_02000ce4
.L_02000e42:
	mov	r5, fp
	cmp	r5, #2
	bne.n	.L_02000e64
	ldr	r3, [pc, #152]
	movs	r1, #16
	ldrsh	r0, [r3, r1]
	movs	r2, #18
	ldrsh	r1, [r3, r2]
	movs	r2, #1
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [sp, #24]
	ldr	r2, [sp, #20]
	bl 0x0200bb68
	bl 0x0200bb50
.L_02000e64:
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x0200bcd8
	ldr	r3, [sp, #16]
	movs	r5, #133
	lsls	r5, r5, #4
	adds	r0, r3, r5
	bl 0x0200bb18
.L_02000e78:
	movs	r3, #0
	str	r3, [r7, #52]
	str	r3, [r7, #48]
	str	r3, [r7, #64]
	str	r3, [r7, #60]
	str	r3, [r7, #56]
	movs	r5, #0
.L_02000e86:
	ldr	r3, [r7, #24]
	ldr	r0, [pc, #92]
	ldr	r1, [pc, #96]
	adds	r3, r3, r0
	str	r3, [r7, #24]
	ldr	r3, [r7, #28]
	movs	r2, #224
.L_02000e94:
	adds	r3, r3, r1
	str	r3, [r7, #28]
	ldr	r3, [r7, #12]
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r7, #12]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200bbc0
	cmp	r5, #8
	bne.n	.L_02000e86
	adds	r0, r7, #0
	bl 0x0200bb48
	movs	r0, #30
.L_02000eb4:
	bl 0x0200bbc0
	ldr	r3, [pc, #52]
	movs	r5, #133
.L_02000ebc:
	lsls	r5, r5, #2
	adds	r3, r3, r5
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200bc38
	bl 0x0200bc30
	bl 0x0200bbd0
	add	sp, #128
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0x0200c674
	.4byte 0xffffd99a
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r6, [pc, #148]
	movs	r7, #0
.L_02000efa:
	ldr	r3, [pc, #148]
	movs	r5, #160
	ldrb	r0, [r3, #0]
	subs	r5, r5, r7
	adds	r0, r7, r0
	lsls	r0, r0, #8
	bl 0x0200bad0
	movs	r1, #144
	subs	r1, r1, r7
	ldr	r3, [pc, #132]
	lsls	r1, r1, #10
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x12c0
	subs	r0, #4
	asrs	r5, r5, #2
	strh	r0, [r6, #0]
	bl 0x0200bac8
	lsls	r1, r5, #1
	bl 0x0200baa8
	ldrh	r3, [r6, #0]
	subs	r0, r0, r5
	adds	r2, r3, r0
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	strh	r2, [r6, #0]
	cmp	r3, #56
	ble.n	.L_02000f3e
	adds	r3, r2, #0
	subs	r3, #56
	b.n	.L_02000f4a
.L_02000f3e:
	movs	r1, #64
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L_02000f4c
	adds	r3, r2, #0
	adds	r3, #64
.L_02000f4a:
	strh	r3, [r6, #0]
.L_02000f4c:
	adds	r7, #1
	adds	r6, #2
	cmp	r7, #160
	bne.n	.L_02000efa
	ldr	r6, [pc, #52]
	movs	r1, #128
	ldrh	r3, [r6, #0]
	lsls	r1, r1, #19
	adds	r1, #16
	strh	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r0, [r3, #10]
	movs	r2, #197
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r0
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r0, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r0
	strh	r2, [r3, #10]
	adds	r0, r6, #2
	ldrh	r2, [r3, #10]
	ldr	r2, [pc, #20]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200c6a0
	.4byte 0x0300122c
	.4byte 0x0300021c
	.2byte 0x0001
	.2byte 0xa260
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r3, [r5, #12]
	ldr	r2, [r5, #72]
	ldr	r1, [r5, #8]
	adds	r3, r3, r2
	ldr	r0, [r5, #68]
	str	r3, [r5, #12]
	ldr	r2, [r5, #76]
	ldr	r3, [r5, #16]
	adds	r1, r1, r0
	str	r1, [r5, #8]
	adds	r3, r3, r2
	asrs	r1, r1, #19
	str	r3, [r5, #16]
	cmp	r1, #39
	bgt.n	.L_02000fd0
	movs	r1, #192
	lsls	r1, r1, #9
	adds	r3, r0, r1
	str	r3, [r5, #68]
	movs	r2, #192
	ldr	r3, [r5, #24]
	lsls	r2, r2, #4
	adds	r2, #204
	b.n	.L_02000fda
.L_02000fd0:
	ldr	r1, [pc, #52]
	ldr	r2, [pc, #56]
	adds	r3, r0, r1
	str	r3, [r5, #68]
	ldr	r3, [r5, #24]
.L_02000fda:
	adds	r3, r3, r2
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldr	r3, [r5, #68]
	cmp	r3, #0
	ble.n	.L_02000ff0
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008038
	b.n	.L_02000ff8
.L_02000ff0:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x02008038
.L_02000ff8:
	ldr	r2, [r5, #80]
	movs	r1, #192
	ldrh	r3, [r2, #18]
	lsls	r1, r1, #5
	adds	r3, r3, r1
	strh	r3, [r2, #18]
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0xfffe8000
	.2byte 0xfae2
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r0, #208
	lsls	r0, r0, #3
	sub	sp, #72
	bl 0x0200bae0
	adds	r5, r0, #0
	bl 0x0200bbc8
	movs	r0, #0
	bl 0x0200bc88
	movs	r1, #128
.L_0200102e:
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #254
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r0, [pc, #172]
	bl 0x0200bb08
	adds	r1, r5, #0
	bl 0x0200baf0
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #152]
	adds	r2, #208
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #208
	movs	r2, #132
	lsls	r1, r1, #2
	lsls	r2, r2, #24
	adds	r0, r5, r1
	adds	r2, #208
	ldr	r1, [pc, #136]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	bl 0x0200bae8
	ldr	r3, [pc, #128]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200bc10
	movs	r0, #8
	movs	r1, #1
	bl 0x0200bc10
	movs	r2, #128
	movs	r3, #128
	lsls	r2, r2, #3
	lsls	r3, r3, #19
	adds	r2, #13
	adds	r3, #8
	strh	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #10
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #252
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #52]
	ldrh	r3, [r1, #0]
	mov	r0, sp
	orrs	r3, r2
	strh	r3, [r1, #0]
	ldr	r3, [pc, #48]
	adds	r0, #70
	strh	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #56]
	ldr	r2, [pc, #56]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200bae0
	adds	r5, r0, #0
	ldr	r0, [pc, #44]
	bl 0x0200bb08
	adds	r1, r5, #0
	bl 0x0200baf0
	b.n	.L_02001108
	.2byte 0x0000
	.4byte 0x00000002
	.4byte 0x00000010
	.4byte 0x000001be
	.4byte 0x0600e800
	.4byte 0x0600ec00
	.4byte 0x02000240
	.4byte 0x06002000
	.4byte 0x81000280
	.2byte 0x01bf
	.2byte 0x0000
.L_02001108:
	movs	r7, #0
	adds	r4, r5, #0
.L_0200110c:
	ldrh	r3, [r4, #0]
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #64
	movs	r1, #144
	adds	r3, r3, r0
	adds	r7, #1
	lsls	r1, r1, #1
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r7, r1
	bne.n	.L_0200110c
	adds	r4, r5, #0
	movs	r7, #0
.L_02001128:
	ldr	r2, [pc, #400]
	lsls	r1, r7, #6
	adds	r1, r1, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r7, #1
	adds	r4, #32
	cmp	r7, #18
	bne.n	.L_02001128
	adds	r0, r5, #0
	bl 0x0200bae8
	movs	r0, #246
	bl 0x0200bcd8
	movs	r0, #8
	bl 0x0200bbd8
	movs	r3, #9
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200bbd8
	movs	r3, #1
	adds	r0, #99
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200bbd8
	ldr	r3, [pc, #332]
	movs	r7, #0
	str	r3, [r0, #108]
.L_02001178:
	ldr	r3, [pc, #328]
	ldr	r3, [r3, #0]
	mov	r8, r3
	mov	r0, r8
	movs	r3, #3
	ands	r0, r3
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_02001222
	movs	r0, #8
	bl 0x0200bbd8
	adds	r6, r0, #0
	movs	r0, #8
	bl 0x0200bbd8
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200bbd8
	ldr	r3, [r0, #16]
	movs	r0, #168
	ldr	r2, [r5, #12]
	ldr	r1, [r6, #8]
	lsls	r0, r0, #2
	bl 0x0200bb40
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200bbd8
	ldr	r3, [r0, #80]
	ldr	r4, [r5, #80]
	ldrb	r3, [r3, #9]
	movs	r1, #12
	ands	r1, r3
	movs	r0, #13
	ldrb	r3, [r4, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	orrs	r3, r1
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	strb	r3, [r4, #9]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	mov	r1, r8
	adds	r3, #85
	strb	r1, [r3, #0]
	ldr	r3, [r5, #8]
	str	r3, [r5, #68]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r5, #72]
	ldr	r3, [r5, #16]
	str	r3, [r5, #76]
	bl 0x0200bac8
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #48]
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200bb28
	adds	r0, r5, #0
	ldr	r1, [pc, #188]
	bl 0x0200bb38
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bb88
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200bc00
	ldr	r3, [pc, #172]
	str	r3, [r5, #108]
.L_02001222:
	movs	r0, #1
	adds	r7, #1
	bl 0x0200bbc0
	cmp	r7, #45
	bne.n	.L_02001178
	ldr	r3, [pc, #160]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200bc18
	bl 0x0200bc30
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200bc20
	movs	r0, #165
	movs	r1, #1
	movs	r2, #146
	lsls	r2, r2, #15
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x0200bc28
	bl 0x0200bc30
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #104]
	bl 0x0200bab8
	movs	r0, #163
	bl 0x0200bcd8
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	movs	r0, #128
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r2, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	bl 0x0200bb98
	movs	r7, #0
.L_020012a2:
	movs	r3, #128
	ldr	r6, [pc, #16]
	lsls	r3, r3, #19
	b.n	.L_020012d8
	.2byte 0x0000
	.4byte 0x00000f00
	.4byte 0x00003f41
	.4byte 0x00000100
	.4byte 0x0000000f
	.4byte 0x0600200f
	.4byte 0x0200892d
	.4byte 0x0300122c
	.4byte 0x0200bd1c
	.4byte 0x020088ad
	.4byte 0x02000240
	.2byte 0x8ef5
	.2byte 0x0200
.L_020012d8:
	lsrs	r2, r7, #2
	adds	r3, #82
	mov	r8, r3
	subs	r3, r6, r2
	lsls	r3, r3, #8
	orrs	r3, r2
	mov	r0, r8
	strh	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200bbd8
	adds	r5, r0, #0
	bl 0x0200bac8
	movs	r3, #7
	ands	r0, r3
	adds	r5, #98
	strb	r0, [r5, #0]
	adds	r7, #1
	movs	r0, #1
	bl 0x0200bbc0
	cmp	r7, #64
	bne.n	.L_020012a2
	movs	r0, #60
	bl 0x0200bbc0
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	mov	r1, r8
	adds	r2, #80
	strh	r6, [r1, #0]
	movs	r0, #192
	strh	r3, [r2, #0]
	movs	r1, #192
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200bb98
	movs	r7, #0
	b.n	.L_02001334
	.2byte 0x0c42
	.2byte 0x0000
.L_02001334:
	adds	r7, #1
	movs	r2, #3
	ands	r2, r7
	cmp	r2, #0
	bne.n	.L_02001396
	add	r3, sp, #16
	mov	r8, r3
	mov	r0, r8
	movs	r3, #1
	str	r3, [r0, #0]
	ldr	r3, [pc, #136]
	add	r6, sp, #56
	str	r3, [r0, #36]
	ldr	r3, [pc, #136]
	str	r2, [r6, #0]
	str	r2, [r6, #4]
	str	r3, [r6, #8]
	bl 0x0200bac8
	movs	r1, #144
	bl 0x0200baa8
	movs	r1, #128
	adds	r5, r0, #0
	lsls	r1, r1, #17
	lsls	r5, r5, #16
	adds	r5, r5, r1
	bl 0x0200bac8
	movs	r1, #144
	bl 0x0200baa8
	ldr	r1, [r6, #4]
	ldr	r3, [r6, #0]
	str	r1, [sp, #0]
	adds	r2, r0, #0
	ldr	r1, [r6, #8]
	mov	r0, r8
	str	r1, [sp, #4]
	movs	r1, #129
	lsls	r1, r1, #17
	adds	r1, #1
	str	r1, [sp, #8]
	str	r0, [sp, #12]
	lsls	r2, r2, #16
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200815c
.L_02001396:
	ldr	r6, [pc, #52]
	movs	r1, #128
	ldr	r3, [pc, #52]
	lsls	r1, r1, #19
	lsrs	r2, r7, #4
	ands	r2, r6
	adds	r1, #82
	subs	r3, r3, r2
	mov	r8, r1
	lsls	r1, r2, #8
	orrs	r1, r3
	mov	r2, r8
	strh	r1, [r2, #0]
	movs	r0, #8
	bl 0x0200bbd8
	adds	r5, r0, #0
	bl 0x0200bac8
	movs	r3, #7
	ands	r0, r3
	adds	r5, #98
	strb	r0, [r5, #0]
	movs	r0, #1
	bl 0x0200bbc0
	b.n	.L_020013dc
	.4byte 0x0000000f
	.4byte 0x00000010
	.4byte 0x02008f9d
	.2byte 0x0000
	.2byte 0xfffe
.L_020013dc:
	.2byte 0x2fff
	bne.n	.L_02001334
	movs	r3, #11
	movs	r2, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #68
	movs	r1, #0
	movs	r2, #79
	movs	r3, #0
	bl 0x0200bb68
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r5, #9
	movs	r0, #38
	movs	r1, #9
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200bb80
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r1, #9
	movs	r2, #1
	movs	r3, #1
	movs	r0, #38
	str	r5, [sp, #4]
	bl 0x0200bb80
	bl 0x0200bb50
	movs	r0, #1
	bl 0x0200bbc0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r2, #230
	mov	r3, r8
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	strh	r6, [r3, #0]
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200bb98
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200bcd8
	movs	r0, #8
	bl 0x0200bbd8
	movs	r3, #6
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r7, #0
.L_0200145a:
	ldr	r3, [pc, #8]
	lsrs	r1, r7, #2
	b.n	.L_02001468
	.4byte 0x00003f41
	.2byte 0x0010
	.2byte 0x0000
.L_02001468:
	movs	r0, #128
	lsls	r2, r1, #8
	subs	r3, r3, r1
	lsls	r0, r0, #19
	adds	r0, #82
	orrs	r2, r3
	strh	r2, [r0, #0]
	adds	r7, #1
	movs	r0, #1
	bl 0x0200bbc0
	cmp	r7, #65
	bne.n	.L_0200145a
	ldr	r0, [pc, #176]
	bl 0x0200bac0
	movs	r0, #8
	bl 0x0200bbd8
	movs	r5, #0
	adds	r0, #99
	strb	r5, [r0, #0]
	movs	r0, #30
	bl 0x0200bab0
	movs	r0, #8
	bl 0x0200bbd8
	str	r5, [r0, #108]
	movs	r0, #8
	bl 0x0200bbd8
	movs	r1, #6
	bl 0x0200bc00
	bl 0x0200bba0
	movs	r0, #60
	bl 0x0200bbc0
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r5, [r3, #0]
	bl 0x0200bb00
	bl 0x0200baf8
	ldr	r6, [pc, #108]
	movs	r0, #147
	lsls	r0, r0, #1
	movs	r1, #128
	adds	r0, #255
	lsls	r1, r1, #2
	adds	r3, r6, r0
	adds	r1, #38
	ldrb	r0, [r3, #0]
	adds	r3, r6, r1
	ldrb	r1, [r3, #0]
	bl 0x0200bbb8
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #10
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #252
.L_020014f0:
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #56]
	ldrh	r3, [r1, #0]
	movs	r5, #1
	orrs	r3, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r6, r2
	strh	r3, [r1, #0]
	ldr	r0, [r6, #0]
	bl 0x0200bbd8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #223
	ands	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [r6, #0]
	bl 0x0200bbd8
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200bbd8
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #1
	b.n	.L_0200153c
	.4byte 0x00000001
	.4byte 0x02008ef5
	.2byte 0x0240
	.2byte 0x0200
.L_0200153c:
	orrs	r5, r3
	strb	r5, [r0, #0]
	ldr	r0, [r6, #0]
	bl 0x0200bc18
	bl 0x0200bc30
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #88
	bl 0x0200bb18
	bl 0x0200bbd0
	add	sp, #72
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #808]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #4
	bl 0x0200bbd8
	adds	r7, r0, #0
	ldr	r6, [r7, #104]
	bl 0x0200bbc8
	movs	r0, #0
	bl 0x0200bc88
	movs	r3, #85
	adds	r3, r3, r7
	mov	fp, r3
	mov	r2, fp
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #0
	adds	r0, r7, #0
	bl 0x0200bb88
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x0200bb18
	movs	r0, #137
	bl 0x0200bcd8
	movs	r3, #99
	adds	r3, r3, r6
	mov	r8, r3
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_02001602
.L_020015bc:
	ldr	r3, [r6, #8]
	ldr	r2, [pc, #732]
	str	r3, [r7, #8]
	ldr	r3, [r6, #12]
	adds	r3, r3, r5
	str	r3, [r7, #12]
	ldr	r3, [r6, #16]
	str	r3, [r7, #16]
	cmp	r5, r2
	bgt.n	.L_020015d8
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r5, r5, r3
.L_020015d8:
	ldr	r3, [pc, #708]
	adds	r1, r7, #0
	ldr	r2, [r3, #0]
	ldrb	r3, [r3, #0]
	adds	r1, #35
	lsls	r3, r3, #12
	strh	r3, [r7, #6]
	movs	r3, #1
	ands	r2, r3
	movs	r3, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	movs	r0, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	bl 0x0200bab0
	mov	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_020015bc
.L_02001602:
	movs	r0, #144
	lsls	r0, r0, #1
	bl 0x0200bcd8
	movs	r2, #35
	adds	r2, r2, r7
	ldrh	r3, [r6, #6]
	mov	r9, r2
	movs	r2, #192
	lsls	r2, r2, #8
	cmp	r3, r2
	beq.n	.L_0200161c
	b.n	.L_020017b4
.L_0200161c:
	ldr	r3, [r6, #104]
	movs	r2, #0
	str	r3, [sp, #0]
	mov	sl, r2
	movs	r3, #4
	mov	r8, r2
	mov	r2, r9
	strb	r3, [r2, #0]
	ldr	r3, [r7, #8]
	movs	r2, #2
	negs	r2, r2
	asrs	r3, r3, #19
	ands	r3, r2
	adds	r3, #1
	lsls	r3, r3, #19
	str	r3, [r7, #8]
	movs	r5, #0
	ldr	r3, [r6, #12]
	asrs	r3, r3, #19
	ands	r3, r2
	adds	r3, #3
	lsls	r3, r3, #19
	str	r3, [r7, #12]
	ldr	r3, [r7, #16]
	asrs	r3, r3, #19
	ands	r3, r2
	adds	r3, #1
	lsls	r3, r3, #19
	str	r3, [r7, #16]
.L_02001656:
	ldr	r0, [pc, #588]
	movs	r2, #64
	ldr	r3, [r0, #0]
	movs	r1, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001672
	ldr	r3, [pc, #576]
	mov	sl, r1
	mov	r8, r3
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r7, #6]
	movs	r1, #1
.L_02001672:
	ldr	r3, [r0, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200168c
	movs	r2, #128
	movs	r3, #0
	lsls	r2, r2, #13
	mov	r8, r3
	mov	sl, r2
	mov	r2, r8
	strh	r2, [r7, #6]
	movs	r1, #1
.L_0200168c:
	ldr	r3, [r0, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020016a6
	ldr	r3, [pc, #528]
	movs	r2, #0
	mov	sl, r3
	movs	r3, #128
	lsls	r3, r3, #8
	strh	r3, [r7, #6]
	mov	r8, r2
	movs	r1, #1
.L_020016a6:
	cmp	r1, #0
	beq.n	.L_020016c6
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #12]
	ldr	r1, [r6, #8]
	subs	r2, r2, r3
	add	r1, sl
	add	r2, r8
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #2
	bl 0x02008348
	asrs	r0, r0, #8
	cmp	r0, #255
	bne.n	.L_02001730
.L_020016c6:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200bab0
	cmp	r5, #60
	bne.n	.L_02001656
	movs	r3, #0
	mov	sl, r3
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r7, #6]
	ldr	r2, [pc, #456]
	ldr	r3, [r6, #12]
	mov	r8, r2
	ldr	r2, [r6, #16]
	ldr	r1, [r6, #8]
	subs	r2, r2, r3
	add	r2, r8
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #2
	bl 0x02008348
	lsls	r0, r0, #8
	lsrs	r0, r0, #16
	cmp	r0, #255
	bne.n	.L_02001730
	movs	r2, #0
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r8, r2
	mov	sl, r3
	mov	r3, r8
	strh	r3, [r7, #6]
	movs	r0, #2
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #12]
	add	r1, sl
	subs	r2, r2, r3
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	bl 0x02008348
	lsls	r0, r0, #8
	lsrs	r0, r0, #16
	cmp	r0, #255
	bne.n	.L_02001730
	movs	r3, #128
	ldr	r2, [pc, #380]
	lsls	r3, r3, #8
	strh	r3, [r7, #6]
	mov	sl, r2
.L_02001730:
	ldr	r5, [sp, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	adds	r5, #98
	add	r2, r8
	add	r1, sl
	ldrb	r0, [r5, #0]
	bl 0x0200bb78
	movs	r1, #6
	str	r0, [r7, #12]
	str	r0, [r7, #20]
	adds	r0, r7, #0
	bl 0x0200bb28
	movs	r0, #6
	bl 0x0200bab0
	movs	r0, #152
	bl 0x0200bcd8
	adds	r0, r7, #0
	movs	r1, #7
	bl 0x0200bb28
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	mov	r2, fp
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #1
	mov	r2, r9
	strb	r3, [r2, #0]
	adds	r2, r7, #0
	ldrb	r3, [r5, #0]
	adds	r2, #34
	strb	r3, [r2, #0]
	ldr	r1, [r7, #8]
	ldr	r3, [r7, #16]
	ldr	r2, [r7, #12]
	add	r3, r8
	add	r1, sl
	adds	r0, r7, #0
	bl 0x0200bb58
	adds	r0, r7, #0
	bl 0x020083cc
	adds	r0, r7, #0
	bl 0x0200bb60
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200bb88
	movs	r3, #3
	mov	r2, fp
	strb	r3, [r2, #0]
	b.n	.L_02001844
.L_020017b4:
	ldr	r3, [r6, #8]
	ldrh	r1, [r6, #6]
	movs	r2, #2
	negs	r2, r2
	asrs	r3, r3, #19
	ands	r3, r2
	asrs	r1, r1, #13
	adds	r3, r3, r1
	subs	r3, #1
	lsls	r3, r3, #19
	str	r3, [r7, #8]
	adds	r0, r7, #0
	ldr	r3, [r6, #16]
	movs	r6, #0
	asrs	r3, r3, #19
	ands	r3, r2
	movs	r2, #2
	ands	r1, r2
	subs	r3, r3, r1
	adds	r3, #1
	lsls	r3, r3, #19
	str	r3, [r7, #16]
	movs	r1, #1
	bl 0x0200bb88
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	mov	r2, r9
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r3, #3
	mov	r2, fp
	strb	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200bc28
	bl 0x0200bc40
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #12
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	movs	r0, #0
	bl 0x0200bb78
	ldr	r1, [r7, #8]
	adds	r2, r0, #0
	ldr	r3, [r7, #16]
	adds	r0, r5, #0
	bl 0x0200bb58
	adds	r0, r7, #0
	bl 0x020083cc
	adds	r0, r5, #0
	bl 0x0200bb60
.L_02001844:
	ldr	r5, [pc, #80]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200bbd8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [r5, #0]
	bl 0x0200bbd8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x0200bb20
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200bc18
	bl 0x0200bc30
	movs	r0, #10
	bl 0x0200bab0
	bl 0x0200bbd0
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x000bffff
	.4byte 0x0300122c
	.4byte 0x03001150
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb500
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
	push	{lr}
	ldmia	r1!, {r3}
	ldmia	r0!, {r4}
	ldr	r2, [r1, #0]
	subs	r4, r4, r3
	ldr	r3, [r0, #0]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	movs	r2, #192
	lsls	r2, r2, #11
	adds	r3, r3, r2
	asrs	r3, r3, #16
	adds	r2, r3, #0
	muls	r2, r3
	adds	r0, r4, #0
	muls	r0, r4
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #4]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r7, [pc, #232]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r7, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #4
	bl 0x0200bbd8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r5, r0, #0
	mov	r8, r3
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02001976
	movs	r3, #173
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02001976
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02001976
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02001976
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r3, r3, r7
	mov	sl, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #5
	bne.n	.L_02001980
.L_02001976:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200bb30
	b.n	.L_02001b42
.L_02001980:
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200bb30
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #12]
	ldr	r1, [r6, #8]
	subs	r2, r2, r3
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #2
	bl 0x02008348
	asrs	r0, r0, #8
	cmp	r0, #212
	bne.n	.L_020019ce
.L_020019a0:
	movs	r1, #142
	lsls	r1, r1, #1
	adds	r0, r6, #0
	adds	r1, #255
	bl 0x0200bcd0
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #99
	strb	r3, [r2, #0]
	str	r3, [r6, #108]
	ldrh	r3, [r6, #6]
	movs	r2, #192
	lsls	r2, r2, #8
	cmp	r3, r2
	bne.n	.L_020019c4
	ldr	r1, [pc, #60]
	b.n	.L_020019c6
.L_020019c4:
	ldr	r1, [pc, #60]
.L_020019c6:
	adds	r0, r6, #0
	bl 0x0200bb38
	b.n	.L_02001b42
.L_020019ce:
	movs	r3, #98
	adds	r3, r3, r6
	mov	r9, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02001a7a
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r7, #0
	adds	r0, #8
	adds	r1, #8
	cmp	r3, #2
	bne.n	.L_02001a08
	bl 0x020098d8
	cmp	r0, #16
	bgt.n	.L_02001a1e
	ldr	r2, [r5, #16]
	ldr	r3, [r6, #16]
	b.n	.L_02001a14
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200bde0
	.2byte 0xbdcc
	.2byte 0x0200
.L_02001a08:
	bl 0x020098ac
	cmp	r0, #8
	bgt.n	.L_02001a1e
	ldr	r2, [r5, #12]
	ldr	r3, [r6, #12]
.L_02001a14:
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02001a1e
	movs	r7, #1
.L_02001a1e:
	cmp	r7, #0
	beq.n	.L_02001a7a
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200bb10
	cmp	r0, #0
	bne.n	.L_02001a7a
	ldrh	r3, [r6, #6]
	str	r6, [r5, #104]
	strh	r3, [r5, #6]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
.L_02001a3a:
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r2, #12
	ldr	r3, [r6, #80]
	ldr	r0, [r5, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r0, #9]
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	movs	r2, #181
	lsls	r2, r2, #1
	add	r2, r8
	strb	r3, [r0, #9]
	movs	r3, #200
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	movs	r2, #128
.L_02001a64:
	ldr	r4, [pc, #28]
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	strb	r4, [r3, #0]
	mov	r2, r9
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r2, r6, #0
	adds	r2, #99
	strb	r3, [r2, #0]
.L_02001a7a:
	mov	r3, r8
	adds	r3, #52
	str	r3, [sp, #0]
	movs	r7, #8
	b.n	.L_02001a8c
	.4byte 0x00000000
	.2byte 0x0240
	.2byte 0x0200
.L_02001a8c:
	ldr	r3, [sp, #0]
	ldmia	r3!, {r5}
	adds	r2, r3, #0
	str	r2, [sp, #0]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001ade
	ldr	r1, [r6, #104]
	ldr	r3, [r5, #8]
	ldr	r2, [r1, #8]
	asrs	r3, r3, #20
	asrs	r2, r2, #20
	cmp	r2, r3
	bne.n	.L_02001ac0
	ldr	r2, [r1, #12]
	ldr	r3, [r5, #12]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02001ac0
	ldr	r2, [r1, #16]
	ldr	r3, [r5, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	beq.n	.L_02001ade
.L_02001ac0:
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r0, #8
	adds	r1, #8
	bl 0x020098ac
	cmp	r0, #8
	bgt.n	.L_02001ade
	ldr	r2, [r5, #12]
	ldr	r3, [r6, #12]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02001ade
	b.n	.L_020019a0
.L_02001ade:
	adds	r7, #1
	cmp	r7, #64
	bne.n	.L_02001a8c
	ldrh	r3, [r6, #6]
	movs	r2, #192
	lsls	r2, r2, #8
	adds	r0, r3, #0
	cmp	r3, r2
	bne.n	.L_02001b1a
	bl 0x0200bad8
	movs	r1, #192
	lsls	r1, r1, #9
	ldr	r5, [pc, #84]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200bad0
	movs	r1, #192
	lsls	r1, r1, #9
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	subs	r3, r3, r0
	str	r3, [r6, #12]
	b.n	.L_02001b42
.L_02001b1a:
	bl 0x0200bad8
	movs	r1, #192
	lsls	r1, r1, #9
	ldr	r5, [pc, #44]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200bad0
	movs	r1, #192
	lsls	r1, r1, #9
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6933
	adds	r3, r3, r0
	str	r3, [r6, #16]
.L_02001b42:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
.L_02001b68:
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	sub	sp, #68
	adds	r7, r0, #0
	cmp	r3, #0
	beq.n	.L_02001b76
	b.n	.L_02001c8c
.L_02001b76:
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_02001b86
	b.n	.L_02001c8c
.L_02001b86:
	movs	r1, #175
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_02001b96
	b.n	.L_02001c8c
.L_02001b96:
	movs	r1, #180
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02001c8c
	movs	r3, #100
	adds	r3, r3, r7
	mov	sl, r3
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #240
	bne.n	.L_02001c82
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x0200bb10
	add	r2, sp, #16
	add	r6, sp, #56
	mov	r8, r2
	cmp	r0, #0
	bne.n	.L_02001bcc
	adds	r0, r7, #0
	movs	r1, #202
	bl 0x0200bcd0
.L_02001bcc:
	ldrh	r3, [r7, #6]
	movs	r1, #192
	lsls	r1, r1, #8
	cmp	r3, r1
	bne.n	.L_02001bde
	ldr	r3, [r7, #8]
	str	r3, [r6, #0]
	ldr	r3, [r7, #16]
	b.n	.L_02001c0a
.L_02001bde:
	ldrh	r0, [r7, #6]
	bl 0x0200bad8
	adds	r1, r0, #0
	movs	r0, #128
	ldr	r5, [pc, #172]
	lsls	r0, r0, #12
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68bb
	adds	r3, r3, r0
	str	r3, [r6, #0]
	ldrh	r0, [r7, #6]
	bl 0x0200bad0
	adds	r1, r0, #0
	movs	r0, #128
	lsls	r0, r0, #12
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x693b
.L_02001c08:
	adds	r3, r3, r0
.L_02001c0a:
	str	r3, [r6, #8]
	add	r6, sp, #56
	movs	r0, #140
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #8]
	ldr	r1, [r6, #0]
	lsls	r0, r0, #1
	bl 0x0200bb40
	movs	r1, #2
	adds	r5, r0, #0
	bl 0x0200bb28
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bb88
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	adds	r3, r5, #0
	movs	r4, #0
	adds	r3, #85
	strb	r4, [r3, #0]
	adds	r3, #13
	strb	r4, [r3, #0]
	adds	r3, #1
	strb	r4, [r3, #0]
	ldrh	r3, [r7, #6]
	mov	r1, r8
	strh	r3, [r5, #6]
	ldr	r3, [pc, #72]
	mov	r2, sl
	str	r3, [r5, #108]
	movs	r3, #1
	str	r7, [r5, #104]
	strh	r4, [r2, #0]
	str	r3, [r1, #0]
	movs	r3, #7
	str	r3, [r1, #4]
	ldr	r3, [pc, #56]
	ldr	r2, [r6, #8]
	ldr	r0, [r6, #0]
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #10
	ldr	r1, [r7, #12]
	str	r3, [sp, #8]
	mov	r3, r8
	str	r3, [sp, #12]
	movs	r3, #0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	bl 0x0200815c
.L_02001c82:
	adds	r2, r7, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_02001c8c:
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0300021c
	.4byte 0x02009909
	.2byte 0x0000
	.2byte 0xfffa
	.2byte 0xb520
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [pc, #24]
	ldr	r5, [r3, #108]
	bl 0x0200bcb8
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #188
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x02008468
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xbd94
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #188
	adds	r3, r3, r2
	ldr	r6, [r3, #0]
	bl 0x0200bbc8
	movs	r0, #0
	bl 0x0200bc88
	bl 0x0200bcc0
	ldr	r0, [pc, #180]
	bl 0x020083e8
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	movs	r0, #2
	bl 0x0200bb78
	ldr	r3, [r6, #12]
	cmp	r0, r3
	beq.n	.L_02001d30
	movs	r3, #34
	adds	r3, r3, r6
	mov	r8, r3
	mov	r2, r8
	movs	r3, #2
	adds	r7, r6, #0
	strb	r3, [r2, #0]
	adds	r7, #85
	movs	r3, #3
	strb	r3, [r7, #0]
	adds	r0, r6, #0
	bl 0x020083cc
	adds	r0, r6, #0
	bl 0x020083cc
	movs	r0, #188
	bl 0x0200bcd8
	movs	r5, #0
	mov	r3, r8
	strb	r5, [r7, #0]
	strb	r5, [r3, #0]
.L_02001d30:
	ldr	r3, [r6, #8]
	asrs	r2, r3, #20
	ldr	r3, [r6, #16]
	asrs	r0, r3, #20
	cmp	r2, #16
	bne.n	.L_02001d4c
	cmp	r0, #43
	bne.n	.L_02001d4c
	movs	r0, #236
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200bb18
	b.n	.L_02001d98
.L_02001d4c:
	cmp	r2, #45
	bne.n	.L_02001d5e
	cmp	r0, #45
	bne.n	.L_02001d5e
	movs	r0, #134
	lsls	r0, r0, #4
	bl 0x0200bb18
	b.n	.L_02001d98
.L_02001d5e:
	cmp	r2, #21
	bne.n	.L_02001d72
	cmp	r0, #48
	bne.n	.L_02001d72
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #97
	bl 0x0200bb18
	b.n	.L_02001d98
.L_02001d72:
	cmp	r2, #39
	bne.n	.L_02001d86
	cmp	r0, #49
	bne.n	.L_02001d86
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #98
	bl 0x0200bb18
	b.n	.L_02001d98
.L_02001d86:
	cmp	r2, #49
	bne.n	.L_02001d98
	cmp	r0, #18
	bne.n	.L_02001d98
	movs	r0, #241
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200bb18
.L_02001d98:
	bl 0x0200bbd0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xbd94
	.2byte 0x0200
	push	{lr}
	bl 0x0200bbc8
	movs	r0, #0
	bl 0x0200bc88
	ldr	r0, [pc, #8]
	bl 0x0200bca8
	bl 0x0200bbd0
	pop	{pc}
	.2byte 0xbe4c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r0
	mov	sl, r1
	movs	r2, #0
	mov	r3, sl
	movs	r0, #255
.L_02001ddc:
	mov	r1, r9
	sub	sp, #68
	bl 0x0200bb40
	movs	r2, #1
	add	r3, sp, #28
	str	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #8]
	str	r2, [r3, #12]
	mov	fp, r0
	movs	r0, #0
	mov	r8, r0
.L_02001df8:
	bl 0x0200bac8
	movs	r5, #63
	ldr	r2, [pc, #364]
	ands	r0, r5
	lsls	r0, r0, #16
	add	r0, r9
	add	r7, sp, #16
	adds	r0, r0, r2
	str	r0, [r7, #0]
	bl 0x0200bac8
	movs	r3, #15
	ands	r3, r0
	lsls	r3, r3, #12
	str	r3, [r7, #4]
	bl 0x0200bac8
	ldr	r3, [pc, #336]
	ands	r0, r5
	lsls	r0, r0, #16
	add	r0, sl
	adds	r2, r0, r3
	movs	r1, #3
	mov	r0, r8
	ands	r1, r0
	mov	r5, r8
	str	r2, [r7, #8]
	add	r4, sp, #28
	ldr	r0, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r5, #1
	cmp	r1, #0
	beq.n	.L_02001e56
	str	r3, [sp, #0]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #160
	lsls	r3, r3, #12
	adds	r3, #1
	str	r3, [sp, #8]
	movs	r1, #0
	movs	r3, #0
	str	r4, [sp, #12]
	bl 0x0200815c
	b.n	.L_02001e6c
.L_02001e56:
	str	r3, [sp, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	adds	r3, #1
	str	r1, [sp, #4]
	str	r3, [sp, #8]
	movs	r1, #0
	movs	r3, #0
	str	r4, [sp, #12]
	bl 0x0200815c
.L_02001e6c:
	mov	r8, r5
	cmp	r5, #12
	bne.n	.L_02001df8
	movs	r2, #0
	mov	r8, r2
.L_02001e76:
	mov	r3, r8
	mov	r6, fp
	cmp	r3, #0
	beq.n	.L_02001e96
	mov	r1, r9
	movs	r2, #0
	movs	r0, #255
	mov	r3, sl
	bl 0x0200bb40
	mov	r2, fp
	adds	r6, r0, #0
	ldr	r0, [r6, #80]
	ldr	r1, [r2, #80]
	bl 0x0200bc90
.L_02001e96:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200bb88
	adds	r0, r6, #0
	ldr	r1, [pc, #208]
	bl 0x0200bb38
	movs	r1, #2
	adds	r0, r6, #0
	bl 0x0200bb28
	bl 0x0200bac8
	movs	r3, #3
	ands	r3, r0
	movs	r0, #128
	lsls	r0, r0, #11
	lsls	r3, r3, #16
	adds	r3, r3, r0
	str	r3, [r6, #40]
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #51
	str	r3, [r6, #72]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r6, #68]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	str	r3, [r6, #48]
	bl 0x0200bac8
	movs	r3, #7
	ands	r0, r3
	str	r0, [r7, #4]
	lsls	r0, r0, #12
	bl 0x0200bad8
	movs	r1, #128
	lsls	r1, r1, #15
	ldr	r5, [pc, #132]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6038
	ldr	r0, [r7, #4]
	lsls	r0, r0, #12
	bl 0x0200bad0
	movs	r1, #128
	lsls	r1, r1, #15
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6d31
	str	r0, [r7, #8]
	movs	r0, #13
	ldrb	r3, [r1, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldr	r3, [r7, #0]
	ldr	r1, [r6, #8]
	ldr	r0, [r7, #8]
	adds	r1, r1, r3
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #12]
	adds	r3, r3, r0
	adds	r0, r6, #0
	bl 0x0200bb58
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #6
	bne.n	.L_02001e76
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200bb98
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200bb98
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xffe00000
	.4byte 0xffd00000
	.4byte 0x0200bdfc
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	sub	sp, #16
	bl 0x0200bae0
	adds	r5, r0, #0
	bl 0x0200bbc8
	movs	r0, #0
	bl 0x0200bc88
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #254
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r2, #128
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #3
	adds	r3, #8
.L_02001fae:
	strh	r2, [r3, #0]
	ldr	r0, [pc, #84]
	bl 0x0200bb08
	adds	r1, r5, #0
	bl 0x0200baf0
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #192
	adds	r3, #212
	adds	r0, r5, #0
.L_02001fc6:
	lsls	r1, r1, #19
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	bl 0x0200bae8
	ldr	r3, [pc, #44]
	mov	r0, sp
	adds	r0, #14
	strh	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #44]
	ldr	r2, [pc, #44]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	bl 0x0200bae0
	adds	r5, r0, #0
	ldr	r0, [pc, #36]
	bl 0x0200bb08
	adds	r1, r5, #0
	bl 0x0200baf0
	movs	r7, #0
	adds	r4, r5, #0
	b.n	.L_0200201c
	.4byte 0x00000000
	.4byte 0x000001c0
	.4byte 0x84000200
	.4byte 0x06002000
	.4byte 0x81000400
	.2byte 0x01c1
	.2byte 0x0000
.L_0200201c:
	ldrh	r2, [r4, #0]
	movs	r3, #252
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	adds	r7, #1
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r7, #64
	bne.n	.L_0200201c
	adds	r4, r5, #0
	movs	r7, #0
.L_02002034:
	ldr	r2, [pc, #424]
	lsls	r1, r7, #6
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
	adds	r7, #1
	adds	r4, #8
	cmp	r7, #16
	bne.n	.L_02002034
	adds	r0, r5, #0
	bl 0x0200bae8
	movs	r0, #246
	bl 0x0200bcd8
	movs	r0, #14
	bl 0x0200bbd8
	movs	r3, #9
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r0, #14
	bl 0x0200bbd8
	movs	r3, #1
	adds	r0, #99
	strb	r3, [r0, #0]
	movs	r0, #14
	bl 0x0200bbd8
	ldr	r3, [pc, #356]
	movs	r7, #0
	str	r3, [r0, #108]
.L_02002084:
	ldr	r3, [pc, #352]
	ldr	r6, [r3, #0]
	movs	r3, #3
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_02002128
	movs	r0, #14
	bl 0x0200bbd8
	str	r0, [sp, #8]
	movs	r0, #14
	bl 0x0200bbd8
	adds	r5, r0, #0
	movs	r0, #14
	bl 0x0200bbd8
	ldr	r3, [sp, #8]
	ldr	r2, [r5, #12]
	ldr	r1, [r3, #8]
	ldr	r3, [r0, #16]
	movs	r0, #168
	lsls	r0, r0, #2
	bl 0x0200bb40
	adds	r5, r0, #0
	movs	r0, #14
	bl 0x0200bbd8
	ldr	r3, [r0, #80]
	ldr	r4, [r5, #80]
	ldrb	r3, [r3, #9]
	movs	r1, #12
	ands	r1, r3
	movs	r0, #13
	ldrb	r3, [r4, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	orrs	r3, r1
	adds	r1, r5, #0
	adds	r1, #35
.L_020020d8:
	ldrb	r2, [r1, #0]
	strb	r3, [r4, #9]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	adds	r3, #85
.L_020020e6:
	strb	r6, [r3, #0]
	ldr	r3, [r5, #8]
	str	r3, [r5, #68]
.L_020020ec:
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r5, #72]
	ldr	r3, [r5, #16]
	str	r3, [r5, #76]
	bl 0x0200bac8
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #48]
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200bb28
	adds	r0, r5, #0
	ldr	r1, [pc, #220]
	bl 0x0200bb38
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bb88
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200bc00
	ldr	r3, [pc, #200]
.L_02002126:
	str	r3, [r5, #108]
.L_02002128:
	movs	r0, #1
	adds	r7, #1
	bl 0x0200bbc0
	cmp	r7, #45
	bne.n	.L_02002084
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200bc58
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200bc50
	movs	r0, #60
	bl 0x0200bc60
	ldr	r5, [pc, #160]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r5, r1
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200bc18
	bl 0x0200bc30
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200bc20
	movs	r0, #134
	movs	r1, #1
	movs	r2, #216
	lsls	r2, r2, #16
	negs	r1, r1
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200bc28
	bl 0x0200bc30
	movs	r0, #14
	bl 0x0200bbd8
	movs	r3, #0
	str	r3, [r0, #108]
	movs	r0, #14
	bl 0x0200bbd8
	movs	r1, #6
	bl 0x0200bc00
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [pc, #72]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #136
	strh	r3, [r2, #0]
	movs	r3, #16
	strh	r3, [r2, #2]
	movs	r0, #30
	bl 0x0200bab0
	movs	r0, #138
	bl 0x0200bcd8
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #0
	bl 0x0200bc58
	movs	r0, #254
	lsls	r0, r0, #7
	b.n	.L_020021fc
	.4byte 0x00001010
	.4byte 0x00003f41
	.4byte 0x06002000
	.4byte 0x0200892d
	.4byte 0x0300122c
	.4byte 0x0200bd1c
	.4byte 0x020088ad
	.4byte 0x02000240
	.2byte 0x1120
	.2byte 0x0300
.L_020021fc:
	movs	r1, #0
	adds	r0, #255
	bl 0x0200bc50
	movs	r0, #1
	bl 0x0200bc60
	ldr	r0, [r5, #0]
	bl 0x0200bbd8
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r0, #6]
	movs	r0, #1
	bl 0x0200bab0
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200bc58
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200bc50
	movs	r0, #8
	bl 0x0200bc60
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #56]
	movs	r5, #9
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r0, #52
	movs	r1, #12
	movs	r2, #29
	movs	r3, #9
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bb68
	movs	r0, #52
	movs	r1, #76
	movs	r2, #29
	movs	r3, #73
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bb68
	movs	r0, #138
	movs	r1, #228
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	bl 0x02009dc4
	movs	r7, #0
.L_02002276:
	ldr	r3, [pc, #8]
	lsrs	r2, r7, #1
	b.n	.L_02002284
	.4byte 0x00000100
	.2byte 0x0010
	.2byte 0x0000
.L_02002284:
	subs	r3, r3, r2
	ldr	r2, [pc, #56]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r0, #1
	adds	r7, #1
	bl 0x0200bab0
	cmp	r7, #32
	bne.n	.L_02002276
	ldr	r2, [pc, #36]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #172
	movs	r1, #0
	strh	r3, [r2, #0]
	strh	r1, [r2, #2]
	movs	r0, #1
	bl 0x0200bab0
	movs	r0, #138
	bl 0x0200bcd8
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	b.n	.L_020022c8
	.4byte 0x00001000
	.2byte 0x1120
	.2byte 0x0300
.L_020022c8:
	movs	r1, #0
	bl 0x0200bc58
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200bc50
	movs	r0, #1
	bl 0x0200bc60
	movs	r0, #1
	bl 0x0200bab0
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200bc58
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200bc50
	movs	r0, #8
	bl 0x0200bc60
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #60]
	movs	r5, #9
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r0, #52
	movs	r1, #22
	movs	r2, #29
	movs	r3, #9
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bb68
	movs	r0, #52
	movs	r1, #86
	movs	r2, #29
	movs	r3, #73
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bb68
	movs	r0, #130
	movs	r1, #242
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	bl 0x02009dc4
	movs	r7, #0
.L_02002340:
	ldr	r3, [pc, #8]
	lsrs	r2, r7, #1
	b.n	.L_02002350
	.2byte 0x0000
	.4byte 0x00000100
	.2byte 0x0010
	.2byte 0x0000
.L_02002350:
	subs	r3, r3, r2
	ldr	r2, [pc, #24]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r0, #1
	adds	r7, #1
	bl 0x0200bab0
	cmp	r7, #32
	bne.n	.L_02002340
	b.n	.L_02002370
	.2byte 0x1000
	.2byte 0x0000
.L_02002370:
	movs	r0, #145
	bl 0x0200bcd8
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #0
	bl 0x0200bc58
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	movs	r7, #128
	bl 0x0200bc50
	lsls	r7, r7, #19
	movs	r0, #1
	bl 0x0200bc60
	ldrh	r2, [r7, #0]
	movs	r3, #254
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r7, #0]
	movs	r0, #20
	bl 0x0200bbc0
	movs	r0, #128
	movs	r1, #198
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	bl 0x02009dc4
	movs	r1, #242
	ldr	r0, [pc, #224]
	lsls	r1, r1, #16
	bl 0x02009dc4
	movs	r1, #223
	ldr	r0, [pc, #220]
	lsls	r1, r1, #16
	bl 0x02009dc4
	movs	r5, #9
	movs	r0, #62
	movs	r1, #2
	movs	r2, #29
	movs	r3, #9
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bb68
	movs	r3, #29
	str	r3, [sp, #0]
	movs	r0, #62
	movs	r1, #2
	movs	r2, #9
	movs	r3, #9
	str	r5, [sp, #4]
	bl 0x0200bb80
	movs	r2, #29
	movs	r3, #73
	movs	r0, #62
	movs	r1, #66
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bb68
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200bc58
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200bc50
	movs	r0, #60
	bl 0x0200bc60
	movs	r0, #60
	bl 0x0200bab0
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #0
	bl 0x0200bc58
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200bc50
	movs	r0, #60
	bl 0x0200bc60
	movs	r0, #70
	bl 0x0200bab0
	bl 0x0200bb00
	bl 0x0200baf8
	ldr	r3, [pc, #84]
	movs	r0, #147
	movs	r1, #128
	lsls	r0, r0, #1
	lsls	r1, r1, #2
	adds	r0, #255
	adds	r1, #38
	adds	r2, r3, r0
	adds	r3, r3, r1
	ldrb	r1, [r3, #0]
	ldrb	r0, [r2, #0]
	bl 0x0200bbb8
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldrh	r3, [r7, #0]
	ldr	r2, [pc, #36]
	movs	r0, #128
	orrs	r3, r2
	ldr	r2, [pc, #44]
	strh	r3, [r7, #0]
	lsls	r0, r0, #4
	movs	r3, #0
	strh	r3, [r2, #0]
	strh	r3, [r2, #2]
	adds	r0, #123
	bl 0x0200bb18
	bl 0x0200bbd0
	add	sp, #16
	b.n	.L_020024ac
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00000100
	.4byte 0x02320000
	.4byte 0x02090000
	.4byte 0x02000240
	.2byte 0x1120
	.2byte 0x0300
.L_020024ac:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	ldr	r0, [pc, #60]
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r7, r3, r2
	ldr	r2, [pc, #48]
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	movs	r1, #128
	lsls	r1, r1, #19
	orrs	r3, r2
	adds	r1, #82
	strh	r3, [r1, #0]
	ldr	r3, [pc, #40]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #40]
	ands	r3, r2
	ldrb	r4, [r0, #0]
	cmp	r3, #0
	bne.n	.L_02002514
	ldr	r1, [pc, #32]
	ldrb	r2, [r1, #0]
	adds	r3, r4, r2
	strb	r3, [r0, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #8
	beq.n	.L_0200250c
	b.n	.L_02002508
	.2byte 0x0000
	.4byte 0x00001000
	.4byte 0x0200c694
	.4byte 0x0300122c
	.4byte 0x0200c6a0
	.2byte 0xc695
	.2byte 0x0200
.L_02002508:
	cmp	r3, #4
	bne.n	.L_02002514
.L_0200250c:
	lsls	r3, r2, #24
	asrs	r3, r3, #24
	negs	r3, r3
	strb	r3, [r1, #0]
.L_02002514:
	movs	r5, #0
.L_02002516:
	ldr	r3, [pc, #88]
	ldrb	r0, [r3, #0]
	movs	r2, #6
	ldrsh	r3, [r7, r2]
	adds	r0, r5, r0
	adds	r0, r0, r3
	lsls	r0, r0, #9
	bl 0x0200bad0
	movs	r2, #2
	ldrsh	r3, [r7, r2]
	asrs	r0, r0, #13
	adds	r3, r3, r0
	adds	r5, #1
	strh	r3, [r6, #0]
	adds	r6, #2
	cmp	r5, #160
	bne.n	.L_02002516
	ldr	r6, [pc, #56]
	movs	r1, #128
	ldrh	r3, [r6, #0]
	lsls	r1, r1, #19
	adds	r1, #20
	strh	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r0, [r3, #10]
	movs	r2, #197
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r0
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r0, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r0
	strh	r2, [r3, #10]
	adds	r0, r6, #2
	ldrh	r2, [r3, #10]
	ldr	r2, [pc, #12]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{r5, r6, r7, pc}
	.4byte 0x0300122c
	.4byte 0x0200c6a0
	.2byte 0x0001
	.2byte 0xa260
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r1, #188
	lsls	r1, r1, #1
	ldr	r6, [pc, #76]
	adds	r1, r1, r3
	mov	sl, r1
	movs	r7, #0
.L_02002596:
	ldr	r3, [pc, #72]
	movs	r5, #160
	ldrb	r0, [r3, #0]
	subs	r5, r5, r7
	adds	r0, r7, r0
	lsls	r0, r0, #8
	bl 0x0200bad0
	movs	r1, #144
	subs	r1, r1, r7
	ldr	r3, [pc, #56]
	lsls	r1, r1, #10
	mov	r8, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x12c0
	subs	r0, #4
	asrs	r5, r5, #2
	strh	r0, [r6, #0]
	bl 0x0200bac8
	lsls	r1, r5, #1
	bl 0x0200baa8
	ldrh	r3, [r6, #0]
	subs	r0, r0, r5
	adds	r2, r3, r0
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	strh	r2, [r6, #0]
	cmp	r3, #56
	ble.n	.L_020025e8
	adds	r3, r2, #0
	subs	r3, #56
	b.n	.L_020025f4
	.4byte 0x0200c6a0
	.4byte 0x0300122c
	.2byte 0x021c
	.2byte 0x0300
.L_020025e8:
	movs	r1, #64
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L_020025f6
	adds	r3, r2, #0
	adds	r3, #64
.L_020025f4:
	strh	r3, [r6, #0]
.L_020025f6:
	ldr	r3, [pc, #48]
	mov	r1, sl
	strh	r3, [r6, #2]
	movs	r2, #6
	ldrsh	r3, [r1, r2]
	mov	r2, r8
	adds	r0, r2, r3
	movs	r2, #2
	ldrsh	r3, [r1, r2]
	asrs	r2, r0, #13
	adds	r3, r3, r2
	adds	r7, #1
	strh	r3, [r6, #4]
	adds	r6, #6
	cmp	r7, #160
	bne.n	.L_02002596
	ldr	r0, [pc, #24]
	ldr	r2, [pc, #16]
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	movs	r1, #128
	lsls	r1, r1, #19
	orrs	r3, r2
	adds	r1, #82
	b.n	.L_02002634
	.4byte 0x00000000
	.4byte 0x00001000
	.2byte 0xc696
	.2byte 0x0200
.L_02002634:
	strh	r3, [r1, #0]
	ldr	r3, [pc, #112]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldrb	r4, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02002660
	ldr	r1, [pc, #100]
	ldrb	r2, [r1, #0]
	adds	r3, r4, r2
	strb	r3, [r0, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #8
	beq.n	.L_02002658
	cmp	r3, #4
	bne.n	.L_02002660
.L_02002658:
	lsls	r3, r2, #24
	asrs	r3, r3, #24
	negs	r3, r3
	strb	r3, [r1, #0]
.L_02002660:
	ldr	r0, [pc, #76]
	movs	r1, #128
	ldrh	r3, [r0, #0]
	lsls	r1, r1, #19
	adds	r1, #16
	strh	r3, [r1, #0]
	adds	r0, #4
	ldrh	r3, [r0, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #20
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r4, [r3, #10]
	movs	r2, #197
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r4
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r4, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r4
	strh	r2, [r3, #10]
	adds	r0, #2
	ldrh	r2, [r3, #10]
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0300122c
	.4byte 0x0200c697
	.4byte 0x0200c6a0
	.2byte 0x0003
	.2byte 0xa260
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #208
	lsls	r0, r0, #3
	sub	sp, #8
	bl 0x0200bae0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	adds	r5, r0, #0
	movs	r0, #188
	lsls	r0, r0, #1
	adds	r0, r0, r3
	mov	r8, r0
	bl 0x0200bbc8
	movs	r0, #0
	bl 0x0200bc88
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #254
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r0, [pc, #124]
	bl 0x0200bb08
	adds	r1, r5, #0
	bl 0x0200baf0
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #104]
	adds	r2, #208
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #208
	lsls	r2, r2, #2
	adds	r0, r5, r2
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r1, [pc, #88]
	adds	r2, #208
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	bl 0x0200bae8
	movs	r2, #128
	movs	r3, #128
	lsls	r2, r2, #3
	lsls	r3, r3, #19
	adds	r2, #14
	adds	r3, #8
	strh	r2, [r3, #0]
	mov	r0, sp
	ldr	r3, [pc, #48]
	adds	r0, #6
	strh	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #52]
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200bae0
	adds	r5, r0, #0
	ldr	r0, [pc, #40]
	bl 0x0200bb08
	adds	r1, r5, #0
	bl 0x0200baf0
	movs	r7, #0
	adds	r4, r5, #0
	b.n	.L_02002788
	.2byte 0x0000
	.4byte 0x00000010
	.4byte 0x000001be
	.4byte 0x0600e800
	.4byte 0x0600ec00
	.4byte 0x06002000
	.4byte 0x81000280
	.2byte 0x01bf
	.2byte 0x0000
.L_02002788:
	ldrh	r3, [r4, #0]
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #64
	movs	r2, #144
	adds	r3, r3, r0
	adds	r7, #1
	lsls	r2, r2, #1
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r7, r2
	bne.n	.L_02002788
	adds	r4, r5, #0
	movs	r7, #0
.L_020027a4:
	ldr	r2, [pc, #452]
	lsls	r1, r7, #6
	adds	r1, r1, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r7, #1
	adds	r4, #32
	cmp	r7, #18
	bne.n	.L_020027a4
	adds	r0, r5, #0
	bl 0x0200bae8
	movs	r0, #246
	bl 0x0200bcd8
	movs	r0, #12
	bl 0x0200bbd8
	movs	r3, #9
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r0, #12
	bl 0x0200bbd8
	movs	r3, #1
	adds	r0, #99
	strb	r3, [r0, #0]
	movs	r0, #12
	bl 0x0200bbd8
	ldr	r3, [pc, #384]
	movs	r7, #0
	str	r3, [r0, #108]
.L_020027f4:
	ldr	r3, [pc, #380]
	ldr	r6, [r3, #0]
	movs	r3, #3
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_02002898
	movs	r0, #12
	bl 0x0200bbd8
	str	r0, [sp, #0]
	movs	r0, #12
	bl 0x0200bbd8
	adds	r5, r0, #0
	movs	r0, #12
	bl 0x0200bbd8
	ldr	r3, [sp, #0]
	ldr	r2, [r5, #12]
	ldr	r1, [r3, #8]
	ldr	r3, [r0, #16]
	movs	r0, #168
	lsls	r0, r0, #2
	bl 0x0200bb40
	adds	r5, r0, #0
	movs	r0, #12
	bl 0x0200bbd8
	ldr	r3, [r0, #80]
	ldr	r4, [r5, #80]
	ldrb	r3, [r3, #9]
	movs	r1, #12
	ands	r1, r3
	movs	r0, #13
	ldrb	r3, [r4, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	orrs	r3, r1
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	strb	r3, [r4, #9]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	ldr	r3, [r5, #8]
	str	r3, [r5, #68]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r5, #72]
	ldr	r3, [r5, #16]
	str	r3, [r5, #76]
	bl 0x0200bac8
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #48]
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200bb28
	adds	r0, r5, #0
	ldr	r1, [pc, #248]
	bl 0x0200bb38
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bb88
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200bc00
	ldr	r3, [pc, #228]
	str	r3, [r5, #108]
.L_02002898:
	movs	r0, #1
	adds	r7, #1
	bl 0x0200bbc0
	cmp	r7, #45
	bne.n	.L_020027f4
	ldr	r3, [pc, #216]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200bc18
	bl 0x0200bc30
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200bc20
	movs	r0, #152
	movs	r1, #1
	movs	r2, #144
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200bc28
	bl 0x0200bc30
	ldr	r0, [pc, #168]
	bl 0x0200bac0
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	ldrh	r1, [r2, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #252
	ands	r3, r1
	strh	r3, [r2, #0]
	movs	r0, #128
	ldrh	r3, [r2, #0]
	lsls	r0, r0, #9
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200bc58
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200bc50
	movs	r0, #1
	bl 0x0200bc60
	movs	r0, #1
	bl 0x0200bab0
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200bc58
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200bc50
	movs	r0, #8
	bl 0x0200bc60
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #76]
	bl 0x0200bab8
	movs	r0, #163
	bl 0x0200bcd8
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #28]
	movs	r0, #192
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r2, #128
	movs	r1, #192
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200bb98
	ldr	r6, [pc, #40]
	movs	r7, #128
	lsls	r7, r7, #7
	b.n	.L_02002990
	.4byte 0x00000100
	.4byte 0x0600200f
	.4byte 0x0200892d
	.4byte 0x0300122c
	.4byte 0x0200bd1c
	.4byte 0x020088ad
	.4byte 0x02000240
	.4byte 0x0200a4b1
	.4byte 0x0200a57d
	.2byte 0x8000
	.2byte 0xffff
.L_02002990:
	.2byte 0x4643
	str	r7, [r3, #24]
	str	r6, [r3, #28]
	movs	r0, #12
	bl 0x0200bbd8
	adds	r5, r0, #0
	bl 0x0200bac8
	movs	r3, #7
	ands	r0, r3
	adds	r5, #98
	strb	r0, [r5, #0]
	movs	r0, #1
	bl 0x0200bab0
	movs	r0, #128
	ldr	r2, [pc, #100]
	lsls	r0, r0, #3
	movs	r3, #144
	adds	r7, r7, r0
	lsls	r3, r3, #11
	adds	r6, r6, r2
	cmp	r7, r3
	bne.n	.L_02002990
	ldr	r0, [pc, #88]
	bl 0x0200bac0
	movs	r1, #200
	ldr	r0, [pc, #84]
	lsls	r1, r1, #4
	bl 0x0200bab8
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r7, #0
.L_020029e4:
	ldr	r3, [pc, #36]
	asrs	r2, r7, #5
	subs	r3, r3, r2
	ldr	r2, [pc, #40]
	movs	r6, #128
	lsls	r6, r6, #19
	orrs	r3, r2
	adds	r6, #82
	strh	r3, [r6, #0]
	movs	r0, #12
	bl 0x0200bbd8
	adds	r5, r0, #0
	bl 0x0200bac8
	movs	r3, #7
	ands	r0, r3
	adds	r5, #98
	b.n	.L_02002a24
	.2byte 0x0000
	.4byte 0x00000008
	.4byte 0x00003f42
	.4byte 0x00001000
	.4byte 0xfffffc00
	.4byte 0x0200a57d
	.2byte 0x8ef5
	.2byte 0x0200
.L_02002a24:
	strb	r0, [r5, #0]
	movs	r0, #1
	bl 0x0200bbc0
	movs	r0, #2
	adds	r7, #1
	adds	r0, #255
	cmp	r7, r0
	bne.n	.L_020029e4
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #253
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r0, #30
	bl 0x0200bab0
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200bcd8
	ldr	r3, [pc, #56]
	movs	r2, #128
	strh	r3, [r6, #0]
	ldr	r3, [pc, #56]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200bb98
	movs	r0, #12
	bl 0x0200bbd8
	movs	r3, #6
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r7, #0
.L_02002a82:
	ldr	r3, [pc, #12]
	asrs	r1, r7, #3
	movs	r0, #128
	lsls	r2, r1, #8
	subs	r3, r3, r1
	b.n	.L_02002a98
	.2byte 0x0000
	.4byte 0x00000010
	.2byte 0x3f41
	.2byte 0x0000
.L_02002a98:
	lsls	r0, r0, #19
	adds	r0, #82
	orrs	r2, r3
	strh	r2, [r0, #0]
	adds	r7, #1
	movs	r0, #1
	bl 0x0200bbc0
	cmp	r7, #129
	bne.n	.L_02002a82
	ldr	r0, [pc, #172]
	bl 0x0200bac0
	movs	r0, #12
	bl 0x0200bbd8
	movs	r5, #0
	adds	r0, #99
	strb	r5, [r0, #0]
	movs	r0, #30
	bl 0x0200bab0
	movs	r0, #12
	bl 0x0200bbd8
	str	r5, [r0, #108]
	movs	r0, #12
	bl 0x0200bbd8
	movs	r1, #6
	bl 0x0200bc00
	bl 0x0200bba0
	movs	r0, #60
	bl 0x0200bbc0
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r5, [r3, #0]
	bl 0x0200bb00
	bl 0x0200baf8
	ldr	r5, [pc, #108]
	movs	r2, #147
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r5, r2
	adds	r2, #1
	ldrb	r0, [r3, #0]
	adds	r3, r5, r2
	ldrb	r1, [r3, #0]
	bl 0x0200bbb8
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #10
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #252
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #60]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r7, r5, r3
	ldr	r0, [r7, #0]
	bl 0x0200bbd8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #223
	ands	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [r7, #0]
	bl 0x0200bbd8
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #1
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #12
	bl 0x0200bbd8
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #1
	b.n	.L_02002b64
	.2byte 0x0000
	.4byte 0x00000001
	.4byte 0x02008ef5
	.2byte 0x0240
	.2byte 0x0200
.L_02002b64:
	orrs	r5, r3
	strb	r5, [r0, #0]
	ldr	r0, [r7, #0]
	bl 0x0200bc18
	bl 0x0200bc30
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #122
	bl 0x0200bb18
	bl 0x0200bbd0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	sub	sp, #8
	bl 0x0200bcc8
	bl 0x0200bbd8
	adds	r1, r0, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	ldr	r0, [r0, #12]
	lsls	r3, r3, #14
	adds	r0, r0, r3
	asrs	r0, r0, #16
	movs	r1, #112
	bl 0x0200baa0
	ldr	r3, [pc, #108]
	adds	r5, r0, #0
	ldr	r3, [r3, #0]
	cmp	r3, r5
	beq.n	.L_02002c1a
	cmp	r5, #1
	beq.n	.L_02002bec
	cmp	r5, #1
	bgt.n	.L_02002bc8
	cmp	r5, #0
	beq.n	.L_02002c02
	b.n	.L_02002c16
.L_02002bc8:
	cmp	r5, #2
	beq.n	.L_02002bde
	cmp	r5, #3
	bne.n	.L_02002c16
	movs	r3, #27
	movs	r2, #54
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	b.n	.L_02002bf8
.L_02002bde:
	movs	r3, #27
	movs	r2, #54
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #67
	b.n	.L_02002bf8
.L_02002bec:
	movs	r3, #27
	movs	r2, #54
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #70
.L_02002bf8:
	movs	r2, #21
	movs	r3, #3
	bl 0x0200bb80
	b.n	.L_02002c16
.L_02002c02:
	movs	r3, #27
	movs	r2, #54
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #73
	movs	r2, #21
	movs	r3, #3
	bl 0x0200bb80
.L_02002c16:
	ldr	r3, [pc, #8]
	str	r5, [r3, #0]
.L_02002c1a:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xc698
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #108]
	adds	r7, r0, #0
	mov	r8, r3
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r0, [r3, #0]
	mov	sl, r3
	bl 0x0200bbd8
	adds	r5, r0, #0
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bb10
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02002c94
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bb88
	ldr	r3, [r5, #12]
	movs	r2, #128
	adds	r3, r3, r7
	str	r3, [r5, #12]
	lsls	r2, r2, #2
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r2, #18
	str	r3, [r5, #48]
	add	r2, r8
	movs	r3, #2
	str	r6, [r5, #40]
	strb	r3, [r2, #0]
	adds	r2, r5, #0
	adds	r2, #90
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200bc18
	bl 0x0200bb50
	movs	r0, #1
	bl 0x0200bab0
.L_02002c94:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	ldr	r6, [pc, #932]
	movs	r0, #214
	lsls	r0, r0, #1
	ldr	r5, [pc, #932]
	movs	r2, #129
	movs	r1, #152
	adds	r3, r3, r0
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	adds	r3, r6, r1
	strh	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #98
	adds	r2, r6, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r0, #104
	adds	r3, r6, r0
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200bbd8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	movs	r1, #240
	orrs	r3, r2
	lsls	r1, r1, #1
	strb	r3, [r0, #0]
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, r5
	beq.n	.L_02002cf6
	b.n	.L_02002f9c
.L_02002cf6:
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r6, r0
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	cmp	r2, #4
	bgt.n	.L_02002d10
	cmp	r2, #2
	blt.n	.L_02002d10
	movs	r0, #176
	lsls	r0, r0, #15
	bl 0x0200ac24
.L_02002d10:
	movs	r0, #133
	lsls	r0, r0, #4
	bl 0x0200bb10
	cmp	r0, #0
	beq.n	.L_02002d54
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #113
	movs	r3, #11
	bl 0x0200bb68
	movs	r3, #113
	movs	r5, #12
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x0200bb80
	movs	r3, #49
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x0200bb80
.L_02002d54:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #81
	bl 0x0200bb10
	cmp	r0, #0
	beq.n	.L_02002d9c
	movs	r3, #1
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #70
	movs	r3, #39
	bl 0x0200bb68
	movs	r3, #70
	movs	r5, #40
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200bb80
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200bb80
.L_02002d9c:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #82
	bl 0x0200bb10
	cmp	r0, #0
	beq.n	.L_02002de2
	movs	r3, #1
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #103
	movs	r3, #38
	bl 0x0200bb68
	movs	r3, #103
	movs	r5, #39
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200bb80
	movs	r0, #64
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bb80
.L_02002de2:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #83
	bl 0x0200bb10
	cmp	r0, #0
	beq.n	.L_02002e2a
	movs	r3, #1
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #87
	movs	r3, #43
	bl 0x0200bb68
	movs	r3, #87
	movs	r5, #44
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200bb80
	movs	r3, #23
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200bb80
.L_02002e2a:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #84
	bl 0x0200bb10
	cmp	r0, #0
	beq.n	.L_02002e74
	movs	r3, #1
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #70
	movs	r3, #52
	bl 0x0200bb68
	movs	r3, #70
	movs	r2, #52
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	bl 0x0200bb80
	movs	r3, #6
	movs	r2, #53
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	bl 0x0200bb80
.L_02002e74:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #85
	bl 0x0200bb10
	cmp	r0, #0
	beq.n	.L_02002eba
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #79
	movs	r3, #49
	bl 0x0200bb68
	movs	r3, #79
	movs	r5, #50
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x0200bb80
	movs	r3, #15
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x0200bb80
.L_02002eba:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #86
	bl 0x0200bb10
	cmp	r0, #0
	beq.n	.L_02002f02
	movs	r3, #1
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #103
	movs	r3, #48
	bl 0x0200bb68
	movs	r3, #103
	movs	r5, #49
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200bb80
	movs	r3, #39
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200bb80
.L_02002f02:
	movs	r0, #235
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200bb10
	cmp	r0, #0
	beq.n	.L_02002f48
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #111
	movs	r3, #52
	bl 0x0200bb68
	movs	r3, #111
	movs	r5, #53
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x0200bb80
	movs	r3, #47
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x0200bb80
.L_02002f48:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #88
	bl 0x0200bb10
	cmp	r0, #0
	beq.n	.L_02002f9c
	movs	r3, #11
	movs	r2, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #68
	movs	r1, #0
	movs	r2, #79
	movs	r3, #0
	bl 0x0200bb68
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r5, #9
	movs	r0, #38
	movs	r1, #9
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200bb80
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r1, #9
	movs	r0, #38
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200bb80
	movs	r0, #8
	bl 0x0200bbd8
	movs	r1, #6
	bl 0x0200bc00
.L_02002f9c:
	ldr	r1, [pc, #180]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #176]
	cmp	r2, r3
	beq.n	.L_02002fb0
	b.n	.L_02003398
.L_02002fb0:
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	subs	r3, #1
	cmp	r3, #11
	bls.n	.L_02002fc2
	b.n	.L_02003398
.L_02002fc2:
	ldr	r2, [pc, #156]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200b028
	.4byte 0x0200b028
	.4byte 0x0200affc
	.4byte 0x0200b0a8
	.4byte 0x0200b06c
	.4byte 0x0200b0a0
	.4byte 0x0200b398
	.4byte 0x0200b398
	.4byte 0x0200b398
	.4byte 0x0200b1f2
	.4byte 0x0200b1c0
	.4byte 0x0200b21c
	.4byte 0x30ff200a
	.4byte 0xfd86f000
	.4byte 0xd10b2800
	.4byte 0x21854b12
	.4byte 0x185b0089
	.4byte 0xf0006818
	.4byte 0x6903fde1
	.4byte 0x03522280
	.4byte 0x6103189b
	.4byte 0xf7ff4810
	.4byte 0xe03ffdff
	.4byte 0x30ff200a
	.4byte 0xfd70f000
	.4byte 0xd10b2800
	.4byte 0x20854b07
	.4byte 0x181b0080
	.4byte 0xf0006818
	.4byte 0x6903fdcb
	.4byte 0x03492180
	.4byte 0x6103185b
	.4byte 0xf7ff4806
	.4byte 0xe029fde9
	.4byte 0x02000240
	.4byte 0x00000052
	.4byte 0x00000053
	.4byte 0x0200afcc
	.4byte 0xffb80000
	.4byte 0xffd80000
	.4byte 0x30ff200a
	.4byte 0xfd4ef000
	.4byte 0xd1132800
	.4byte 0x22854d4d
	.4byte 0x18ad0092
	.4byte 0xf0006828
	.4byte 0x6883fda9
	.4byte 0x038921c0
	.4byte 0x6083185b
	.4byte 0xf0006828
	.4byte 0x68c3fda1
	.4byte 0x03522280
	.4byte 0x60c3189b
	.4byte 0x038020e0
	.4byte 0xfdbef7ff
	.4byte 0xf0002008
	.4byte 0x263cfd95
	.4byte 0x23003064
	.4byte 0x20088006
	.4byte 0xf0004698
	.4byte 0x4d3dfd8d
	.4byte 0x200866c5
	.4byte 0xfd88f000
	.4byte 0xf0002100
	.4byte 0x2009fd5d
	.4byte 0xfd82f000
	.4byte 0x306423b4
	.4byte 0x20098003
	.4byte 0xfd7cf000
	.4byte 0x200966c5
	.4byte 0xfd78f000
	.4byte 0xf0002100
	.4byte 0x200afd4d
	.4byte 0xfd72f000
	.4byte 0x30644641
	.4byte 0x200a8001
	.4byte 0xfd6cf000
	.4byte 0x200a66c5
	.4byte 0xfd68f000
	.4byte 0xf0002100
	.4byte 0x200bfd3d
	.4byte 0xfd62f000
	.4byte 0x80063064
	.4byte 0xf000200b
	.4byte 0x66c5fd5d
	.4byte 0xf000200b
	.4byte 0x2100fd59
	.4byte 0xfd2ef000
	.4byte 0x00c020ec
	.4byte 0xf00030ff
	.4byte 0x2800fced
	.4byte 0x2184d006
	.4byte 0x200c22ae
	.4byte 0x04920449
	.4byte 0xfd58f000
	.4byte 0x01002086
	.4byte 0xfce0f000
	.4byte 0xd0062800
	.4byte 0x22b621b6
	.4byte 0x0489200d
	.4byte 0xf0000492
	.4byte 0x2080fd4b
	.4byte 0x30610100
	.4byte 0xfcd2f000
	.4byte 0xd0062800
	.4byte 0x22c221ac
	.4byte 0x0449200f
	.4byte 0xf0000492
	.4byte 0x2080fd3d
	.4byte 0x30620100
	.4byte 0xfcc4f000
	.4byte 0xd0062800
	.4byte 0x22c6219e
	.4byte 0x04892010
	.4byte 0xf0000492
	.4byte 0x4d07fd2f
	.4byte 0xf0001c28
	.4byte 0x1c28fd87
	.4byte 0xf920f7fd
	.4byte 0xf0004804
	.4byte 0xe0f3fd79
	.4byte 0x02000240
	.4byte 0x02009b55
	.4byte 0x0200bd94
	.4byte 0x0200be4c
	.4byte 0x30ff200a
	.4byte 0xfca4f000
	.4byte 0xd1122800
	.4byte 0x22854d39
	.4byte 0x18ad0092
	.4byte 0xf0006828
	.4byte 0x6883fcff
	.4byte 0x03492180
	.4byte 0x6083185b
	.4byte 0xf0006828
	.4byte 0x4a33fcf7
	.4byte 0x189b68c3
	.4byte 0x200a60c3
	.4byte 0xf00030ff
	.4byte 0x2800fc8b
	.4byte 0x4b2dd10b
	.4byte 0x00802085
	.4byte 0x6818181b
	.4byte 0xfce6f000
	.4byte 0x21806903
	.4byte 0x185b0349
	.4byte 0x48296103
	.4byte 0xfd04f7ff
	.4byte 0x00c020f1
	.4byte 0xf00030ff
	.4byte 0x2800fc75
	.4byte 0x21c6d006
	.4byte 0x20162294
	.4byte 0x04520489
	.4byte 0xfce0f000
	.4byte 0x1c284d21
	.4byte 0xfd38f000
	.4byte 0xf7fd1c28
	.4byte 0x2011f8d1
	.4byte 0xfcc6f000
	.4byte 0x30552300
	.4byte 0x20117003
	.4byte 0xfcc0f000
	.4byte 0x035b2380
	.4byte 0x201160c3
	.4byte 0xfcbaf000
	.4byte 0x3064233c
	.4byte 0x20118003
	.4byte 0xfcb4f000
	.4byte 0x4d0f4e14
	.4byte 0x201166c6
	.4byte 0xfcaef000
	.4byte 0xf0002100
	.4byte 0x2013fc83
	.4byte 0xfca8f000
	.4byte 0x70053055
	.4byte 0xf0002013
	.4byte 0x23a0fca3
	.4byte 0x60c303db
	.4byte 0xf0002013
	.4byte 0x235afc9d
	.4byte 0x80033064
	.4byte 0xf0002013
	.4byte 0x66c6fc97
	.4byte 0xe00b2013
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0xfff00000
	.4byte 0xffc80000
	.4byte 0x0200bd94
	.4byte 0x02009b55
	.4byte 0xfc86f000
	.4byte 0xf0002100
	.4byte 0x2012fc5b
	.4byte 0xfc80f000
	.4byte 0x70053055
	.4byte 0xf0002012
	.4byte 0x4b9cfc7b
	.4byte 0x201260c3
	.4byte 0xfc76f000
	.4byte 0x30642364
	.4byte 0x20128003
	.4byte 0xfc70f000
	.4byte 0x201266c6
	.4byte 0xfc6cf000
	.4byte 0xf0002100
	.4byte 0x2014fc41
	.4byte 0xfc66f000
	.4byte 0x70053055
	.4byte 0xf0002014
	.4byte 0x2390fc61
	.4byte 0x60c3041b
	.4byte 0xf0002014
	.4byte 0x22b4fc5b
	.4byte 0x46434690
	.4byte 0x80033064
	.4byte 0xf0002014
	.4byte 0x66c6fc53
	.4byte 0xf0002014
	.4byte 0x2100fc4f
	.4byte 0xfc24f000
	.4byte 0xf0002015
	.4byte 0x3055fc49
	.4byte 0x20157005
	.4byte 0xfc44f000
	.4byte 0x21024b81
	.4byte 0x201560c3
	.4byte 0xfc5af000
	.4byte 0xf0002015
	.4byte 0x3023fc3b
	.4byte 0x23027802
	.4byte 0x70034313
	.4byte 0xf0002015
	.4byte 0x4641fc33
	.4byte 0x80013064
	.4byte 0xf0002015
	.4byte 0x2301fc2d
	.4byte 0x70033062
	.4byte 0xf0002015
	.4byte 0x66c6fc27
	.4byte 0xf0002015
	.4byte 0x2100fc23
	.2byte 0xf000
	.2byte 0xfbf8
.L_02003398:
	ldr	r1, [pc, #448]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #440]
	cmp	r2, r3
	beq.n	.L_020033ac
	.2byte 0xe35c
.L_020033ac:
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	subs	r3, #1
	cmp	r3, #98
	bls.n	.L_020033be
	.2byte 0xe353
.L_020033be:
	ldr	r2, [pc, #420]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	push	{r3, r5, r6, lr}
	lsls	r0, r0, #8
	push	{r3, r5, r6, lr}
	lsls	r0, r0, #8
	push	{r3, r5, r6, lr}
	lsls	r0, r0, #8
	.2byte 0xb70e
	lsls	r0, r0, #8
	.2byte 0xb70e
	lsls	r0, r0, #8
	.2byte 0xb6e2
	lsls	r0, r0, #8
	.2byte 0xb706
	lsls	r0, r0, #8
	.2byte 0xb720
	lsls	r0, r0, #8
	.2byte 0xb720
	lsls	r0, r0, #8
	.2byte 0xb754
	lsls	r0, r0, #8
	.2byte 0xb778
	lsls	r0, r0, #8
	.2byte 0xb8b0
	lsls	r0, r0, #8
	.2byte 0xb8d2
	lsls	r0, r0, #8
	.2byte 0xb8fc
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xba66
	lsls	r0, r0, #8
	.2byte 0xb9f2
	lsls	r0, r0, #8
	movs	r0, r0
	.2byte 0xffd0
	.2byte 0x0000
	.2byte 0xff70
	.2byte 0x0240
	lsls	r0, r0, #8
	lsls	r4, r2, #1
	movs	r0, r0
	.2byte 0xb3c8
	lsls	r0, r0, #8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #88]
	bl 0x0200bab8
	movs	r0, #8
	bl 0x0200bbd8
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200bbd8
	ldr	r1, [pc, #68]
	ldr	r5, [pc, #60]
	mov	r8, r1
	str	r1, [r0, #12]
	movs	r1, #2
	movs	r0, #8
	bl 0x0200bc10
	movs	r0, #8
	bl 0x0200bbd8
	movs	r3, #60
	adds	r0, #100
	strh	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200bbd8
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #8
	bl 0x0200bbd8
	ldr	r6, [pc, #28]
	str	r6, [r0, #108]
	movs	r0, #8
	bl 0x0200bbd8
	movs	r1, #0
	bl 0x0200bb88
	movs	r0, #9
	b.n	.L_020035d4
	.4byte 0x00000000
	.4byte 0x0200ab89
	.4byte 0xfff80000
	.2byte 0x9b55
	.2byte 0x0200
.L_020035d4:
	bl 0x0200bbd8
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #9
	bl 0x0200bbd8
	mov	r2, r8
	str	r2, [r0, #12]
	movs	r1, #2
	movs	r0, #9
	bl 0x0200bc10
	movs	r0, #9
	bl 0x0200bbd8
	movs	r3, #180
	adds	r0, #100
	strh	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200bbd8
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #9
	bl 0x0200bbd8
	str	r6, [r0, #108]
	movs	r0, #9
	bl 0x0200bbd8
	movs	r1, #0
	bl 0x0200bb88
	movs	r0, #10
	bl 0x0200bbd8
	movs	r3, #2
	mov	r8, r3
	adds	r0, #34
	mov	r1, r8
	strb	r1, [r0, #0]
	movs	r1, #1
	movs	r0, #10
	bl 0x0200bc10
	movs	r0, #10
	bl 0x0200bbd8
	movs	r6, #208
	lsls	r6, r6, #16
	str	r6, [r0, #12]
	movs	r0, #10
	bl 0x0200bbd8
	adds	r5, r0, #0
	movs	r0, #10
	bl 0x0200bbd8
	ldr	r3, [r0, #12]
	movs	r0, #11
	str	r3, [r5, #20]
	bl 0x0200bbd8
	mov	r2, r8
	adds	r0, #34
	strb	r2, [r0, #0]
	movs	r1, #1
	movs	r0, #11
	bl 0x0200bc10
	movs	r0, #11
	bl 0x0200bbd8
	str	r6, [r0, #12]
	movs	r0, #11
	bl 0x0200bbd8
	adds	r5, r0, #0
	movs	r0, #11
	bl 0x0200bbd8
	ldr	r3, [r0, #12]
	movs	r0, #196
	str	r3, [r5, #20]
	lsls	r0, r0, #2
	bl 0x0200bb10
	cmp	r0, #0
	beq.n	.L_02003696
	movs	r1, #252
	movs	r2, #218
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200bbf8
.L_02003696:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #17
	bl 0x0200bb10
	cmp	r0, #0
	beq.n	.L_020036b2
	movs	r1, #130
	movs	r2, #218
	movs	r0, #11
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200bbf8
.L_020036b2:
	bl 0x02008620
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	movs	r3, #188
	lsls	r3, r3, #1
	adds	r2, r2, r3
	movs	r3, #176
	lsls	r3, r3, #19
	str	r3, [r2, #8]
	movs	r3, #144
	lsls	r3, r3, #18
	str	r3, [r2, #12]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r2, #16]
	str	r3, [r2, #20]
	bl 0x0200bb50
	movs	r0, #1
	bl 0x0200bab0
	b.n	.L_02003a66
	.2byte 0x200a
	.4byte 0xf00030ff
	.4byte 0x2800fa13
	.4byte 0x4b17d10b
	.4byte 0x00802085
	.4byte 0x6818181b
	.4byte 0xfa6ef000
	.4byte 0x21c068c3
	.4byte 0x185b0349
	.4byte 0x209060c3
	.4byte 0xf7ff03c0
	.4byte 0x4b0ffa8b
	.4byte 0x00922285
	.4byte 0x6818189b
	.4byte 0xf0002101
	.4byte 0xe1a2fa79
	.4byte 0x30ff200a
	.4byte 0xf9f4f000
	.4byte 0xd10b2800
	.4byte 0x20854b07
	.4byte 0x181b0080
	.4byte 0xf0006818
	.4byte 0x6903fa4f
	.4byte 0x03492180
	.4byte 0x6103185b
	.4byte 0xf7ff4802
	.4byte 0xe019fa6d
	.4byte 0x02000240
	.4byte 0xffe80000
	.4byte 0x30ff200a
	.4byte 0xf9daf000
	.4byte 0xd10b2800
	.4byte 0x22854b1b
	.4byte 0x189b0092
	.4byte 0xf0006818
	.4byte 0x6883fa35
	.4byte 0x038921c0
	.4byte 0x6083185b
	.4byte 0x04002090
	.4byte 0xfa52f7ff
	.4byte 0x04c92180
	.4byte 0x20ff310a
	.4byte 0x0200880a
	.4byte 0x1c0330fc
	.4byte 0x800b4013
	.4byte 0x880b4a0b
	.4byte 0x800b4313
	.4byte 0x880a3102
	.4byte 0x40131c03
	.4byte 0x4a08800b
	.4byte 0x4313880b
	.4byte 0x3102800b
	.4byte 0x4a06880b
	.4byte 0x80084018
	.4byte 0x880b200c
	.4byte 0x800b4313
	.4byte 0x0000e008
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x02000240
	.4byte 0xfa00f000
	.4byte 0x78023023
	.4byte 0x43132302
	.4byte 0x20007003
	.4byte 0xfa4cf000
	.4byte 0x01002080
	.4byte 0xf0003079
	.4byte 0x2800f98f
	.4byte 0x21b8d006
	.4byte 0x200d228c
	.4byte 0x04520409
	.4byte 0xf9faf000
	.4byte 0x01002080
	.4byte 0xf000307a
	.4byte 0x2800f981
	.4byte 0x23c0d131
	.4byte 0x6edb049b
	.4byte 0x005222d6
	.4byte 0x3aac189b
	.4byte 0x4b0d601a
	.4byte 0x04d22280
	.4byte 0x80133252
	.4byte 0x3a024b0b
	.4byte 0x32608013
	.4byte 0x2b006893
	.4byte 0x21c8dbfc
	.4byte 0x01094808
	.4byte 0xf93af000
	.4byte 0x049b23c0
	.4byte 0x23bc6a1a
	.4byte 0x18d2005b
	.4byte 0x01db2380
	.4byte 0x0000e006
	.4byte 0x00001008
	.4byte 0x00003f42
	.4byte 0x0200a4b1
	.4byte 0x4b116193
	.4byte 0x230361d3
	.4byte 0x23c08513
	.4byte 0x8553005b
	.4byte 0x4b0be0f7
	.4byte 0x04d22280
	.4byte 0x80133252
	.4byte 0x3a024b09
	.4byte 0x21808013
	.4byte 0x880a04c9
	.4byte 0x021b23fd
	.4byte 0x401333ff
	.4byte 0x200c800b
	.4byte 0xf99ef000
	.4byte 0xf0002106
	.4byte 0xe0e0f9af
	.4byte 0x00001000
	.4byte 0x00003f42
	.4byte 0xffff8000
	.4byte 0x30ff200a
	.4byte 0xf92cf000
	.4byte 0xd10a2800
	.4byte 0x20854b6c
	.4byte 0x181b0080
	.4byte 0xf0006818
	.4byte 0x496af987
	.4byte 0x185b68c3
	.4byte 0x200a60c3
	.4byte 0xf00030ff
	.4byte 0x2800f91b
	.4byte 0x4b64d10b
	.4byte 0x00922285
	.4byte 0x6818189b
	.4byte 0xf976f000
	.4byte 0x21806903
	.4byte 0x185b0349
	.4byte 0x48606103
	.4byte 0xf994f7ff
	.4byte 0x049b23c0
	.4byte 0x23bc6a1a
	.4byte 0x18d2005b
	.4byte 0x04db2390
	.4byte 0x23c86093
	.4byte 0x60d3049b
	.4byte 0x23d02080
	.4byte 0x0100021b
	.4byte 0x61536113
	.4byte 0xf000307b
	.4byte 0x2800f8f5
	.4byte 0x2509d134
	.4byte 0x2109201d
	.4byte 0x2302223e
	.4byte 0x95019500
	.4byte 0xf916f000
	.4byte 0x2202233e
	.4byte 0x92019300
	.4byte 0x2109201d
	.4byte 0x23092209
	.4byte 0xf918f000
	.4byte 0x2149201d
	.4byte 0x2342223e
	.4byte 0x95019500
	.4byte 0xf904f000
	.4byte 0x21022034
	.4byte 0x2309221d
	.4byte 0x95019500
	.4byte 0xf8fcf000
	.4byte 0x9300231d
	.4byte 0x21022034
	.4byte 0x23092209
	.4byte 0xf0009501
	.4byte 0x2034f8ff
	.4byte 0x221d2142
	.4byte 0x95002349
	.4byte 0xf0009501
	.4byte 0xe005f8eb
	.4byte 0xf000200e
	.4byte 0x2106f91f
	.4byte 0xf930f000
	.4byte 0x2101200e
	.4byte 0xf934f000
	.4byte 0xf8d2f000
	.4byte 0xf0002001
	.4byte 0x200af87f
	.4byte 0xf00030ff
	.4byte 0x2800f8ab
	.4byte 0x4e2cd053
	.4byte 0x00802085
	.4byte 0x68301836
	.4byte 0xf906f000
	.4byte 0x1c2a1c05
	.4byte 0x23003222
	.4byte 0x20007013
	.4byte 0x68eb692a
	.4byte 0x1ad268a9
	.4byte 0xf8caf000
	.4byte 0x60e86168
	.4byte 0x68302100
	.4byte 0xf914f000
	.4byte 0x2509e039
	.4byte 0x2109201d
	.4byte 0x2302223e
	.4byte 0x95019500
	.4byte 0xf8b2f000
	.4byte 0x2202233e
	.4byte 0x92019300
	.4byte 0x2109201d
	.4byte 0x23092209
	.4byte 0xf8b4f000
	.4byte 0x2149201d
	.4byte 0x2342223e
	.4byte 0x95019500
	.4byte 0xf8a0f000
	.4byte 0x21022034
	.4byte 0x2309221d
	.4byte 0x95019500
	.4byte 0xf898f000
	.4byte 0x9300231d
	.4byte 0x21022034
	.4byte 0x23092209
	.4byte 0xf0009501
	.4byte 0x2034f89b
	.4byte 0x221d2142
	.4byte 0x95002349
	.4byte 0xf0009501
	.4byte 0xf000f887
	.4byte 0xf000f905
	.4byte 0xf7fef90b
	.2byte 0xfa8b
.L_02003a66:
	movs	r0, #0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0xffe00000
	.2byte 0x0000
	.2byte 0xffd0
	.2byte 0xb500
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02003a94
	bl 0x0200ab88
.L_02003a94:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000054
	.irp EntryTarget, 0x03000528, 0x03000514, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000121, 0x08000169, 0x08000179, 0x080001a9, 0x080001d9, 0x080001f1, 0x08000291, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x08020099, 0x080200a9, 0x080200c1, 0x080200c9, 0x08020121, 0x08020149, 0x08020151, 0x08020179, 0x08020199, 0x080201c1, 0x080201e9, 0x08020219, 0x08020221, 0x08020229, 0x08020231, 0x080202f9, 0x08038041, 0x08038349, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8099, 0x080c80d1, 0x080c80f1, 0x080c80f9, 0x080c8171, 0x080c81d1, 0x080c8201, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8249, 0x080c8259, 0x080c8279, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c8481, 0x080c84e1, 0x080c8519, 0x080c8691, 0x080c86f9, 0x080c8709, 0x080c8719, 0x080c8721, 0x080c8729, 0x080c8779, 0x080c88c9, 0x081c0011
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
	.4byte 0x000d000c
	.4byte 0x0010000f
	.4byte 0xffff0016
	.4byte 0x0200bce0
	.4byte 0x0200bd1c
	.4byte 0x0200bd58
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte 0x0200833d
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffae2
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffae2
	.4byte 0x0000000d
	.4byte 0x00000020
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
	.4byte 0x0200000e
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
	.4byte 0x00600100
	.4byte 0x01100090
	.4byte 0x00a00070
	.4byte 0x0002ffff
	.4byte 0x00600260
	.4byte 0x02700090
	.4byte 0x00a00070
	.4byte 0x0003ffff
	.4byte 0x00600360
	.4byte 0x03700090
	.4byte 0x00a00070
	.4byte 0x0004ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xff400100
	.4byte 0x01100330
	.4byte 0x0340ff50
	.4byte 0x0001ffff
	.4byte 0xff400260
	.4byte 0x02700330
	.4byte 0x0340ff50
	.4byte 0x0002ffff
	.4byte 0xff400360
	.4byte 0x03700330
	.4byte 0x0340ff50
	.4byte 0x0003ffff
	.4byte 0x00b00270
	.4byte 0x028002c0
	.4byte 0x02d000c0
	.4byte 0x0005ffff
	.4byte 0x00a00310
	.4byte 0x032002b0
	.4byte 0x02c000b0
	.4byte 0x0006ffff
	.4byte 0xff380270
	.4byte 0x02800140
	.4byte 0x0150ff48
	.4byte 0x0007ffff
	.4byte 0xff280310
	.4byte 0x03200130
	.4byte 0x0140ff38
	.4byte 0x0008ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffa00090
	.4byte 0x00a00130
	.4byte 0x0140ffb0
	.4byte 0x0006ffff
	.4byte 0xffa000d0
	.4byte 0x00e00130
	.4byte 0x0140ffb0
	.4byte 0x0007ffff
	.4byte 0xffb00090
	.4byte 0x00a00130
	.4byte 0x0140ffc0
	.4byte 0x0008ffff
	.4byte 0xffb000d0
	.4byte 0x00e00130
	.4byte 0x0140ffc0
	.4byte 0x0009ffff
	.4byte 0x00a00090
	.4byte 0x00a000c0
	.4byte 0x00d000b0
	.4byte 0x000affff
	.4byte 0x00a000d0
	.4byte 0x00e000c0
	.4byte 0x00d000b0
	.4byte 0x000bffff
	.4byte 0x00a00090
	.4byte 0x00a000c0
	.4byte 0x00d000b0
	.4byte 0x000affff
	.4byte 0x00a000d0
	.4byte 0x00e000c0
	.4byte 0x00d000b0
	.4byte 0x000bffff
	.4byte 0xffa00210
	.4byte 0x02200130
	.4byte 0x0140ffb0
	.4byte 0x000cffff
	.4byte 0xffa00250
	.4byte 0x02600130
	.4byte 0x0140ffb0
	.4byte 0x000dffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000052
	.4byte 0x10110002
	.4byte 0xffffffff
	.4byte 0x10201053
	.4byte 0xffffffff
	.4byte 0x10302053
	.4byte 0xffffffff
	.4byte 0x10403053
	.4byte 0xffffffff
	.4byte 0x00000053
	.4byte 0x10102052
	.4byte 0xffffffff
	.4byte 0x10203052
	.4byte 0xffffffff
	.4byte 0x10304052
	.4byte 0xffffffff
	.4byte 0x1040a055
	.4byte 0xffffffff
	.4byte 0x1050a053
	.4byte 0xffffffff
	.4byte 0x1060b053
	.4byte 0xffffffff
	.4byte 0x10705053
	.4byte 0xffffffff
	.4byte 0x10806053
	.4byte 0xffffffff
	.4byte 0x10901054
	.4byte 0xffffffff
	.4byte 0x00000054
	.4byte 0x1010c053
	.4byte 0xffffffff
	.4byte 0x10204054
	.4byte 0xffffffff
	.4byte 0x10305054
	.4byte 0xffffffff
	.4byte 0x10402054
	.4byte 0xffffffff
	.4byte 0x10503054
	.4byte 0xffffffff
	.4byte 0x10608054
	.4byte 0xffffffff
	.4byte 0x10709054
	.4byte 0xffffffff
	.4byte 0x10806054
	.4byte 0xffffffff
	.4byte 0x10907054
	.4byte 0xffffffff
	.4byte 0x10a08054
	.4byte 0x0000087a
	.4byte 0x10b09054
	.4byte 0x0000087a
	.4byte 0x10a0c054
	.4byte 0xffffffff
	.4byte 0x10b0d054
	.4byte 0xffffffff
	.4byte 0x10c0a054
	.4byte 0xffffffff
	.4byte 0x10d0b054
	.4byte 0xffffffff
	.4byte 0x10e01056
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff017e
	.4byte 0x00000007
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0145
	.4byte 0x0200bdac
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0145
	.4byte 0x0200bdac
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00024000
	.4byte 0xffff0145
	.4byte 0x0200bdac
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00024000
	.4byte 0xffff0145
	.4byte 0x0200bdac
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00024000
	.4byte 0xffff0145
	.4byte 0x0200bdac
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00020000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00028000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00020000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00028000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x0002c000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x0102c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x0102c000
	.4byte 0xffff017e
	.4byte 0x00000007
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0xffff017e
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00780000
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
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte 0x02008549
	.4byte 0x80004e15
	.4byte 0x08580008
	.4byte 0x02008825
	.4byte 0x10004e15
	.4byte 0x08580008
	.4byte 0x02008831
	.4byte 0x00004e15
	.4byte 0x08580008
	.4byte 0x02009011
	.4byte 0x80004e15
	.4byte 0xffff0009
	.4byte 0x02008825
	.4byte 0x10004e15
	.4byte 0xffff0009
	.4byte 0x02008831
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte 0x0200895d
	.4byte 0x80004e15
	.4byte 0xffff000a
	.4byte 0x02008825
	.4byte 0x10004e15
	.4byte 0xffff000a
	.4byte 0x02008831
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x0200895d
	.4byte 0x80004e15
	.4byte 0xffff000b
	.4byte 0x02008825
	.4byte 0x10004e15
	.4byte 0xffff000b
	.4byte 0x02008831
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte 0x0200895d
	.4byte 0x80004e15
	.4byte 0xffff000c
	.4byte 0x02008825
	.4byte 0x10004e15
	.4byte 0xffff000c
	.4byte 0x02008831
	.4byte 0x00004e15
	.4byte 0xffff000c
	.4byte 0x0200895d
	.4byte 0x80004e15
	.4byte 0xffff000d
	.4byte 0x02008825
	.4byte 0x10004e15
	.4byte 0xffff000d
	.4byte 0x02008831
	.4byte 0x00004e15
	.4byte 0xffff000d
	.4byte 0x0200895d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008569
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000009
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02009ca5
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02009cc9
	.4byte 0x10008c15
	.4byte 0x085f000c
	.4byte 0x02009ca5
	.4byte 0x00008c15
	.4byte 0x085f000c
	.4byte 0x02009cc9
	.4byte 0x10008c15
	.4byte 0x0860000d
	.4byte 0x02009ca5
	.4byte 0x00008c15
	.4byte 0x0860000d
	.4byte 0x02009cc9
	.4byte 0x10008c15
	.4byte 0x0861000f
	.4byte 0x02009ca5
	.4byte 0x00008c15
	.4byte 0x0861000f
	.4byte 0x02009cc9
	.4byte 0x10008c15
	.4byte 0x08620010
	.4byte 0x02009ca5
	.4byte 0x00008c15
	.4byte 0x08620010
	.4byte 0x02009cc9
	.4byte 0x00001815
	.4byte 0x0200000e
	.4byte 0x02009da9
	.4byte 0x10008c15
	.4byte 0x08870016
	.4byte 0x02009ca5
	.4byte 0x00008c15
	.4byte 0x08870016
	.4byte 0x02009cc9
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x02009561
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
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x0000000e
	.4byte 0x00004602
	.4byte 0xffff0014
	.4byte 0x02008701
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x020085ed
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x02008621
	.4byte 0x00008c15
	.4byte 0x0879000d
	.4byte 0x020086d5
	.4byte 0x80004e15
	.4byte 0x087a000c
	.4byte 0x02008825
	.4byte 0x10004e15
	.4byte 0x087a000c
	.4byte 0x02008831
	.4byte 0x00004e15
	.4byte 0x087a000c
	.4byte 0x0200a6b9
	.4byte 0x80004e15
	.4byte 0x087b000e
	.4byte 0x02008825
	.4byte 0x10004e15
	.4byte 0x087b000e
	.4byte 0x02008831
	.4byte 0x00004e15
	.4byte 0x087b000e
	.4byte 0x02009f7d
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x02009561
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x01060106
	.4byte 0xffffffff
