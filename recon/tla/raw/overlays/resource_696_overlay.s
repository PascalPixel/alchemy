.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200bff9, 0x02008291, 0x020082d5, 0x020082dd, 0x02008c81, 0x020082d1, 0x0200c05d
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
	bl 0x0200c8dc
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
	bl 0x0200c814
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
	bl 0x0200c804
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200c80c
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200c854
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
	bl 0x0200c984
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
	bl 0x0200c734
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
	bl 0x0200c734
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_0200020c:
	bl 0x0200c734
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
	bl 0x0200c804
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200c80c
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
	.4byte 0x0200cc50
	.4byte 0x02008081
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #18
	movs	r1, #3
	movs	r2, #13
	bl 0x0200ca34
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
	bne.n	.L_020002a8
	ldr	r0, [pc, #24]
	b.n	.L_020002b4
.L_020002a8:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020002b2
	ldr	r0, [pc, #24]
	b.n	.L_020002b4
.L_020002b2:
	ldr	r0, [pc, #24]
.L_020002b4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000f5
	.4byte 0x0200d2b4
	.4byte 0x000000f6
	.4byte 0x0200d314
	.2byte 0xd284
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd344
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
	bne.n	.L_020002f4
	ldr	r0, [pc, #84]
	b.n	.L_0200033c
.L_020002f4:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_020002fe
	ldr	r0, [pc, #84]
	b.n	.L_0200033c
.L_020002fe:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_0200031c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #254
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02000318
	ldr	r2, [pc, #68]
	movs	r3, #1
	strb	r3, [r2, #22]
.L_02000318:
	ldr	r0, [pc, #60]
	b.n	.L_0200033c
.L_0200031c:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02000326
	ldr	r0, [pc, #60]
	b.n	.L_0200033c
.L_02000326:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02000330
	ldr	r0, [pc, #56]
	b.n	.L_0200033c
.L_02000330:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_0200033a
	ldr	r0, [pc, #56]
	b.n	.L_0200033c
.L_0200033a:
	ldr	r0, [pc, #56]
.L_0200033c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000f1
	.4byte 0x0200d428
	.4byte 0x000000f3
	.4byte 0x0200d590
	.4byte 0x000000f2
	.4byte 0x0200d6b0
	.4byte 0x000000f4
	.4byte 0x0200d860
	.4byte 0x000000f5
	.4byte 0x0200d998
	.4byte 0x000000f6
	.4byte 0x0200da40
	.2byte 0xd410
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r6, [pc, #148]
	movs	r7, #0
.L_0200037e:
	ldr	r3, [pc, #148]
	movs	r5, #160
	ldrb	r0, [r3, #0]
	subs	r5, r5, r7
	adds	r0, r7, r0
	lsls	r0, r0, #8
	bl 0x0200c764
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
	bl 0x0200c75c
	lsls	r1, r5, #1
	bl 0x0200c73c
	ldrh	r3, [r6, #0]
	subs	r0, r0, r5
	adds	r2, r3, r0
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	strh	r2, [r6, #0]
	cmp	r3, #56
	ble.n	.L_020003c2
	adds	r3, r2, #0
	subs	r3, #56
	b.n	.L_020003ce
.L_020003c2:
	movs	r1, #64
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L_020003d0
	adds	r3, r2, #0
	adds	r3, #64
.L_020003ce:
	strh	r3, [r6, #0]
.L_020003d0:
	adds	r7, #1
	adds	r6, #2
	cmp	r7, #160
	bne.n	.L_0200037e
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
	.4byte 0x0200e350
	.4byte 0x0300122c
	.4byte 0x0300021c
	.2byte 0x0001
	.2byte 0xa260
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r2, r3, r1
	ldr	r3, [r2, #8]
	ldr	r1, [pc, #32]
	adds	r3, r3, r1
	str	r3, [r2, #8]
	ldr	r3, [r2, #12]
	movs	r1, #128
	lsls	r1, r1, #10
	adds	r3, r3, r1
	movs	r1, #130
	lsls	r1, r1, #19
	str	r3, [r2, #12]
	cmp	r3, r1
	ble.n	.L_02000452
	movs	r3, #0
	str	r3, [r2, #8]
	movs	r3, #128
	lsls	r3, r3, #19
	str	r3, [r2, #12]
.L_02000452:
	pop	{pc}
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb500
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
	bl 0x0200c984
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
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
	bl 0x0200c76c
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
	bl 0x0200c764
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
	bl 0x0200c984
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0300021c
	.2byte 0x122c
	.2byte 0x0300
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
	asrs	r1, r1, #16
	str	r3, [r5, #16]
	cmp	r1, #167
	bgt.n	.L_0200053c
	movs	r1, #192
	lsls	r1, r1, #9
	adds	r3, r0, r1
	str	r3, [r5, #68]
	movs	r2, #192
	ldr	r3, [r5, #24]
	lsls	r2, r2, #4
	adds	r2, #204
	b.n	.L_02000546
.L_0200053c:
	ldr	r1, [pc, #52]
	ldr	r2, [pc, #56]
	adds	r3, r0, r1
	str	r3, [r5, #68]
	ldr	r3, [r5, #24]
.L_02000546:
	adds	r3, r3, r2
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldr	r3, [r5, #68]
	cmp	r3, #0
	ble.n	.L_0200055c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008038
	b.n	.L_02000564
.L_0200055c:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x02008038
.L_02000564:
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
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r0, #208
	lsls	r0, r0, #3
	sub	sp, #72
	bl 0x0200c784
.L_02000590:
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #254
	lsls	r3, r3, #8
.L_0200059a:
	adds	r3, #255
	ands	r3, r2
	adds	r5, r0, #0
	strh	r3, [r1, #0]
	ldr	r0, [pc, #168]
	bl 0x0200c7dc
	adds	r1, r5, #0
	bl 0x0200c79c
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #148]
	adds	r2, #208
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #208
	movs	r2, #132
	lsls	r1, r1, #2
	lsls	r2, r2, #24
	adds	r0, r5, r1
	adds	r2, #208
	ldr	r1, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	bl 0x0200c794
	ldr	r3, [pc, #124]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200c9c4
	movs	r0, #14
	movs	r1, #1
	bl 0x0200c9c4
	movs	r3, #128
	movs	r1, #128
	lsls	r3, r3, #3
	lsls	r1, r1, #19
	adds	r3, #13
	adds	r1, #8
	strh	r3, [r1, #0]
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
	bl 0x0200c784
	adds	r5, r0, #0
	ldr	r0, [pc, #44]
	bl 0x0200c7dc
	adds	r1, r5, #0
	bl 0x0200c79c
	b.n	.L_02000668
	.2byte 0x0000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x000001be
	.4byte 0x0600e800
	.4byte 0x0600ec00
	.4byte 0x02000240
	.4byte 0x06002000
	.4byte 0x81000280
	.2byte 0x01bf
	.2byte 0x0000
.L_02000668:
	movs	r7, #0
	adds	r4, r5, #0
.L_0200066c:
	ldrh	r3, [r4, #0]
	movs	r0, #255
	lsls	r0, r0, #8
	movs	r2, #240
	adds	r0, #64
	lsls	r2, r2, #4
	adds	r2, #255
	adds	r3, r3, r0
	ands	r3, r2
	ldr	r2, [pc, #52]
	movs	r1, #144
	orrs	r3, r2
	adds	r7, #1
	lsls	r1, r1, #1
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r7, r1
	bne.n	.L_0200066c
	adds	r4, r5, #0
	movs	r7, #0
.L_02000694:
	ldr	r2, [pc, #32]
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
	b.n	.L_020006bc
	.2byte 0x0000
	.4byte 0x00001000
	.2byte 0x200f
	.2byte 0x0600
.L_020006bc:
	cmp	r7, #18
	bne.n	.L_02000694
	adds	r0, r5, #0
	bl 0x0200c794
	movs	r0, #246
	bl 0x0200ca9c
	movs	r0, #14
	bl 0x0200c8dc
	movs	r3, #9
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r0, #14
	bl 0x0200c8dc
	movs	r3, #1
	adds	r0, #99
	strb	r3, [r0, #0]
	movs	r0, #14
	bl 0x0200c8dc
	ldr	r3, [pc, #328]
	movs	r7, #0
	str	r3, [r0, #108]
.L_020006f0:
	ldr	r3, [pc, #324]
	ldr	r3, [r3, #0]
	mov	r8, r3
	mov	r0, r8
	movs	r3, #3
	ands	r0, r3
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_0200079a
	movs	r0, #14
	bl 0x0200c8dc
	adds	r6, r0, #0
	movs	r0, #14
	bl 0x0200c8dc
	adds	r5, r0, #0
	movs	r0, #14
	bl 0x0200c8dc
	ldr	r3, [r0, #16]
	movs	r0, #168
	ldr	r2, [r5, #12]
	ldr	r1, [r6, #8]
	lsls	r0, r0, #2
	bl 0x0200c814
	adds	r5, r0, #0
	movs	r0, #14
	bl 0x0200c8dc
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
	bl 0x0200c75c
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #48]
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200c804
	adds	r0, r5, #0
	ldr	r1, [pc, #184]
	bl 0x0200c80c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200c854
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200c984
	ldr	r3, [pc, #168]
	str	r3, [r5, #108]
.L_0200079a:
	movs	r0, #1
	adds	r7, #1
	bl 0x0200c8bc
	cmp	r7, #45
	bne.n	.L_020006f0
	ldr	r3, [pc, #156]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200c9dc
	bl 0x0200c9f4
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200c9e4
	movs	r0, #168
	movs	r1, #1
	movs	r2, #212
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200c9ec
	bl 0x0200c9f4
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #100]
	bl 0x0200c74c
	movs	r0, #207
	bl 0x0200ca9c
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
	bl 0x0200c864
	movs	r7, #0
.L_0200081a:
	ldr	r3, [pc, #20]
	lsrs	r2, r7, #2
	subs	r3, r3, r2
	b.n	.L_0200084c
	.2byte 0x0000
	.4byte 0x00000f00
	.4byte 0x00003f41
	.4byte 0x00000100
	.4byte 0x0000000f
	.4byte 0x02008459
	.4byte 0x0300122c
	.4byte 0x0200cae0
	.4byte 0x02008489
	.4byte 0x02000240
	.2byte 0x8379
	.2byte 0x0200
.L_0200084c:
	movs	r6, #128
	lsls	r6, r6, #19
	lsls	r3, r3, #8
	orrs	r3, r2
	adds	r6, #82
	strh	r3, [r6, #0]
	movs	r0, #14
	bl 0x0200c8dc
	adds	r5, r0, #0
	bl 0x0200c75c
	movs	r3, #7
	ands	r0, r3
	adds	r5, #98
	strb	r0, [r5, #0]
	adds	r7, #1
	movs	r0, #1
	bl 0x0200c8bc
	cmp	r7, #64
	bne.n	.L_0200081a
	movs	r0, #60
	bl 0x0200c8bc
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c864
	ldr	r3, [pc, #56]
	movs	r2, #128
	strh	r3, [r6, #0]
	ldr	r3, [pc, #52]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r5, #16
	movs	r0, #2
	movs	r1, #83
	movs	r2, #36
	movs	r3, #64
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200c844
	movs	r0, #64
	movs	r1, #64
	movs	r2, #2
	movs	r3, #83
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200c844
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #10
	ldrh	r2, [r1, #0]
	b.n	.L_020008d0
	.4byte 0x00001000
	.2byte 0x3f42
	.2byte 0x0000
.L_020008d0:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #252
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r1, #144
	ldr	r0, [pc, #68]
	lsls	r1, r1, #3
	bl 0x0200c74c
	movs	r7, #0
.L_020008e6:
	movs	r3, #128
	ldr	r6, [pc, #52]
	lsls	r3, r3, #19
	lsrs	r2, r7, #2
	adds	r3, #82
	mov	r8, r3
	subs	r3, r6, r2
	lsls	r3, r3, #8
	orrs	r3, r2
	mov	r0, r8
	strh	r3, [r0, #0]
	movs	r0, #14
	bl 0x0200c8dc
	adds	r5, r0, #0
	bl 0x0200c75c
	movs	r3, #7
	ands	r0, r3
	adds	r5, #98
	strb	r0, [r5, #0]
	adds	r7, #1
	movs	r0, #1
	bl 0x0200c8bc
	cmp	r7, #68
	bne.n	.L_020008e6
	b.n	.L_02000928
	.2byte 0x0000
	.4byte 0x00000010
	.2byte 0x8421
	.2byte 0x0200
.L_02000928:
	movs	r3, #13
	movs	r2, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #45
	movs	r1, #90
	movs	r2, #4
	movs	r3, #19
	bl 0x0200c844
	movs	r2, #192
	movs	r3, #128
	lsls	r2, r2, #3
	lsls	r3, r3, #19
	adds	r2, #2
	adds	r3, #12
	strh	r2, [r3, #0]
	adds	r3, #200
	ldr	r0, [pc, #72]
	ldr	r1, [pc, #76]
	ldr	r2, [pc, #76]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	ldr	r3, [pc, #56]
	lsls	r2, r2, #19
	mov	r1, r8
	adds	r2, #80
	strh	r6, [r1, #0]
	strh	r3, [r2, #0]
	movs	r7, #0
.L_02000966:
	lsrs	r2, r7, #2
	lsls	r3, r2, #2
	subs	r3, r7, r3
	lsls	r0, r3, #3
	lsls	r1, r2, #1
	subs	r0, r0, r3
	adds	r1, r1, r2
	movs	r3, #13
	movs	r2, #11
	lsls	r0, r0, #1
	lsls	r1, r1, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	adds	r0, #45
	movs	r2, #4
	adds	r1, #90
	movs	r3, #19
	bl 0x0200c844
	movs	r2, #0
	mov	r9, r2
	b.n	.L_020009a4
	.2byte 0x0000
	.4byte 0x00000a44
	.4byte 0x06002800
	.4byte 0x06003000
	.2byte 0x0200
	.2byte 0x8400
.L_020009a4:
	movs	r2, #3
	mov	r3, r9
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_02000a06
	add	r0, sp, #16
	movs	r3, #1
	str	r3, [r0, #0]
	ldr	r3, [pc, #140]
	add	r6, sp, #56
	str	r3, [r0, #36]
	ldr	r3, [pc, #140]
	str	r2, [r6, #0]
	str	r3, [r6, #8]
	str	r2, [r6, #4]
	mov	sl, r0
	bl 0x0200c75c
	movs	r1, #127
	adds	r5, r0, #0
	ands	r5, r1
	movs	r2, #208
	lsls	r2, r2, #15
	lsls	r5, r5, #16
	mov	r8, r1
	adds	r5, r5, r2
	bl 0x0200c75c
	ldr	r1, [r6, #4]
	mov	r3, r8
	ands	r0, r3
	ldr	r3, [r6, #0]
	str	r1, [sp, #0]
	movs	r2, #236
	ldr	r1, [r6, #8]
	lsls	r2, r2, #1
	str	r1, [sp, #4]
	movs	r1, #129
	lsls	r1, r1, #17
	subs	r2, r2, r0
	adds	r1, #1
	mov	r0, sl
	str	r1, [sp, #8]
	str	r0, [sp, #12]
	lsls	r2, r2, #16
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x020080b8
.L_02000a06:
	movs	r1, #128
	ldr	r6, [pc, #52]
	mov	r2, r9
	lsls	r1, r1, #19
	adds	r1, #82
	lsrs	r3, r2, #2
	mov	r8, r1
	lsls	r2, r3, #8
	subs	r3, r6, r3
	orrs	r2, r3
	mov	r3, r8
	strh	r2, [r3, #0]
	movs	r0, #14
	bl 0x0200c8dc
	adds	r5, r0, #0
	bl 0x0200c75c
	movs	r3, #7
	ands	r0, r3
	adds	r5, #98
	strb	r0, [r5, #0]
	movs	r0, #1
	bl 0x0200c744
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	b.n	.L_02000a4c
	.4byte 0x00000010
	.4byte 0x02008509
	.2byte 0x0000
	.2byte 0xfffe
.L_02000a4c:
	.2byte 0x2940
	bne.n	.L_020009a4
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #140]
	ldr	r1, [pc, #140]
	ldr	r2, [pc, #144]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r2, r8
	adds	r7, #1
	strh	r6, [r2, #0]
	cmp	r7, #8
	beq.n	.L_02000a6c
	b.n	.L_02000966
.L_02000a6c:
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #10
	subs	r3, #200
	strh	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #251
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r2, #11
	movs	r3, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #19
	movs	r2, #38
	movs	r0, #47
	movs	r1, #19
	bl 0x0200c844
	mov	r3, r8
	strh	r6, [r3, #0]
	movs	r2, #128
	ldr	r3, [pc, #56]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r7, #0
.L_02000aaa:
	ldr	r3, [pc, #52]
	lsrs	r1, r7, #2
	movs	r6, #128
	subs	r3, r3, r1
	lsls	r2, r1, #8
	lsls	r6, r6, #19
	orrs	r2, r3
	adds	r6, #82
	strh	r2, [r6, #0]
.L_02000abc:
	movs	r0, #14
	bl 0x0200c8dc
	adds	r5, r0, #0
	bl 0x0200c75c
	movs	r3, #7
	ands	r0, r3
	adds	r5, #98
	strb	r0, [r5, #0]
	adds	r7, #1
	movs	r0, #1
	bl 0x0200c8bc
	b.n	.L_02000af0
	.2byte 0x0000
	.4byte 0x00003f42
	.4byte 0x00000010
	.4byte 0x06002800
	.4byte 0x06003000
	.2byte 0x0200
	.2byte 0x8400
.L_02000af0:
	cmp	r7, #68
	bne.n	.L_02000aaa
	ldr	r0, [pc, #88]
	bl 0x0200c754
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
	movs	r0, #36
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r3, #16
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r1, #64
	movs	r2, #2
	movs	r3, #83
	bl 0x0200c844
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	strh	r3, [r6, #0]
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	b.n	.L_02000b54
	.4byte 0x00000001
	.4byte 0x00003f41
	.4byte 0x0000000f
	.2byte 0x8421
	.2byte 0x0200
.L_02000b54:
	bl 0x0200c864
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200ca9c
	movs	r0, #14
	bl 0x0200c8dc
	movs	r3, #6
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r7, #0
.L_02000b6e:
	ldr	r3, [pc, #56]
	lsrs	r1, r7, #2
	movs	r0, #128
	lsls	r2, r1, #8
	subs	r3, r3, r1
	lsls	r0, r0, #19
	adds	r0, #82
	orrs	r2, r3
	strh	r2, [r0, #0]
	adds	r7, #1
	movs	r0, #1
	bl 0x0200c8bc
	cmp	r7, #65
	bne.n	.L_02000b6e
	ldr	r0, [pc, #28]
	bl 0x0200c754
	movs	r0, #14
	bl 0x0200c8dc
	movs	r5, #0
	adds	r0, #99
	strb	r5, [r0, #0]
	movs	r0, #30
	bl 0x0200c744
	movs	r0, #14
	b.n	.L_02000bb0
	.4byte 0x00000010
	.2byte 0x8379
	.2byte 0x0200
.L_02000bb0:
	bl 0x0200c8dc
	str	r5, [r0, #108]
	movs	r0, #14
	bl 0x0200c8dc
	movs	r1, #6
	bl 0x0200c984
	bl 0x0200c86c
	movs	r0, #60
	bl 0x0200c8bc
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r5, [r3, #0]
	bl 0x0200c7d4
	bl 0x0200c7bc
	ldr	r6, [pc, #140]
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
	bl 0x0200c88c
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r6, r2
	ldr	r0, [r6, #0]
	bl 0x0200c8dc
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #223
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r5, #1
	ldr	r0, [r6, #0]
	bl 0x0200c8dc
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #14
	bl 0x0200c8dc
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #128
	orrs	r5, r3
	strb	r5, [r0, #0]
	lsls	r1, r1, #19
	ldr	r2, [pc, #56]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	bl 0x0200c82c
	movs	r0, #1
	bl 0x0200c744
	ldr	r0, [r6, #0]
	movs	r1, #1
	bl 0x0200c9dc
	bl 0x0200c9f4
	movs	r3, #6
	movs	r2, #29
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #5
	movs	r2, #9
	movs	r3, #1
	movs	r0, #1
	bl 0x0200c84c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #76
	b.n	.L_02000c70
	.4byte 0x00000400
	.2byte 0x0240
	.2byte 0x0200
.L_02000c70:
	bl 0x0200c7f4
	add	sp, #72
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{lr}
	ldr	r3, [pc, #76]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02000c98
	ldr	r0, [pc, #64]
	b.n	.L_02000ccc
.L_02000c98:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02000ca2
	ldr	r0, [pc, #64]
	b.n	.L_02000ccc
.L_02000ca2:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02000cac
	ldr	r0, [pc, #60]
	b.n	.L_02000ccc
.L_02000cac:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02000cb6
	ldr	r0, [pc, #60]
	b.n	.L_02000ccc
.L_02000cb6:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02000cc0
	ldr	r0, [pc, #56]
	b.n	.L_02000ccc
.L_02000cc0:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000cca
	ldr	r0, [pc, #56]
	b.n	.L_02000ccc
.L_02000cca:
	ldr	r0, [pc, #56]
.L_02000ccc:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000f1
	.4byte 0x0200daac
	.4byte 0x000000f3
	.4byte 0x0200dbc0
	.4byte 0x000000f2
	.4byte 0x0200dca4
	.4byte 0x000000f4
	.4byte 0x0200df98
	.4byte 0x000000f5
	.4byte 0x0200e1f0
	.4byte 0x000000f6
	.4byte 0x0200e2d4
	.2byte 0xdaa0
	.2byte 0x0200
	push	{lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200c874
	bl 0x0200c8cc
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2936
	.2byte 0x0000
	push	{lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200c874
	bl 0x0200c8cc
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2938
	.2byte 0x0000
	push	{lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200c874
	bl 0x0200c8cc
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2937
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #184]
	bl 0x0200c98c
	movs	r1, #0
	movs	r0, #17
	bl 0x0200c994
	ldr	r3, [pc, #176]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c8d4
	cmp	r0, #0
	bne.n	.L_02000e10
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c954
	movs	r1, #204
	lsls	r1, r1, #6
	ldr	r0, [pc, #148]
	adds	r1, #51
	bl 0x0200c9e4
	movs	r0, #224
	movs	r1, #1
	movs	r2, #140
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c9ec
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #116]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #146
	ldr	r0, [r5, #0]
	movs	r1, #246
	lsls	r2, r2, #1
	bl 0x0200c91c
	movs	r2, #146
	ldr	r0, [r5, #0]
	movs	r1, #204
	lsls	r2, r2, #1
	bl 0x0200c91c
	movs	r2, #140
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	movs	r1, #182
	bl 0x0200c91c
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c9bc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200c7f4
	bl 0x02009380
	b.n	.L_02000e2a
.L_02000e10:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #17
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200c9a4
.L_02000e2a:
	bl 0x0200c8cc
	pop	{r5, pc}
	.4byte 0x000029b0
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #160]
	bl 0x0200c98c
	movs	r1, #0
	movs	r0, #16
	bl 0x0200c994
	ldr	r3, [pc, #152]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c8d4
	cmp	r0, #0
	bne.n	.L_02000ecc
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c954
	movs	r1, #204
	lsls	r1, r1, #6
	ldr	r0, [pc, #124]
	adds	r1, #51
	bl 0x0200c9e4
	movs	r0, #224
	movs	r1, #1
	movs	r2, #140
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c9ec
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #92]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #140
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	movs	r1, #182
	bl 0x0200c91c
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c9bc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200c7f4
	bl 0x02009380
	b.n	.L_02000ee6
.L_02000ecc:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #16
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200c9a4
.L_02000ee6:
	bl 0x0200c8cc
	pop	{r5, pc}
	.4byte 0x000029b3
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #28]
	bl 0x0200c98c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r3, #173
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	bl 0x0200c8cc
	pop	{r5, pc}
	.2byte 0x293e
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #28]
	bl 0x0200c98c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	movs	r3, #173
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	bl 0x0200c8cc
	pop	{r5, pc}
	.2byte 0x2940
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #28]
	bl 0x0200c98c
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c9a4
	movs	r3, #173
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	bl 0x0200c8cc
	pop	{r5, pc}
	.2byte 0x293f
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c8dc
	movs	r2, #190
	ldrh	r3, [r0, #6]
	lsls	r2, r2, #7
	adds	r2, #255
	adds	r3, r3, r2
	ldr	r2, [pc, #16]
	lsls	r3, r3, #16
	movs	r0, #1
	cmp	r3, r2
	bls.n	.L_02000fba
	movs	r0, #0
.L_02000fba:
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0x3ffe
	push	{r5, r6, r7, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #254
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02001004
	bl 0x02008f94
	cmp	r0, #0
	beq.n	.L_02000fe6
	movs	r0, #11
	movs	r1, #13
	bl 0x0200ca84
	b.n	.L_020010b4
.L_02000fe6:
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #196]
	bl 0x0200c98c
	movs	r1, #0
	movs	r0, #13
	bl 0x0200c9ac
	bl 0x0200c8cc
	b.n	.L_020010b4
.L_02001004:
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	bl 0x02008f94
	cmp	r0, #0
	beq.n	.L_020010a2
	movs	r0, #11
	bl 0x0200ca8c
	movs	r1, #5
	adds	r5, r0, #0
	movs	r0, #13
	bl 0x0200c94c
	movs	r0, #30
	bl 0x0200c8bc
	ldr	r7, [pc, #140]
	adds	r0, r7, #0
	bl 0x0200c98c
	adds	r0, r5, #0
	movs	r1, #5
	bl 0x0200c87c
	adds	r0, r5, #0
	movs	r1, #5
	bl 0x0200c87c
	movs	r1, #0
	movs	r0, #12
	bl 0x0200c994
	ldr	r6, [pc, #112]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200c8d4
	cmp	r0, #0
	bne.n	.L_02001098
	ldr	r3, [r6, #16]
	cmp	r3, r5
	bcs.n	.L_02001092
	movs	r1, #6
	movs	r0, #13
	bl 0x0200c94c
	movs	r0, #20
	bl 0x0200c8bc
	movs	r1, #5
	movs	r0, #13
	bl 0x0200c94c
	movs	r0, #30
	bl 0x0200c8bc
	adds	r0, r7, #3
	bl 0x0200c98c
	movs	r0, #12
	movs	r1, #0
	bl 0x0200c9a4
	b.n	.L_02001098
.L_02001092:
	adds	r0, r5, #0
	bl 0x0200ca94
.L_02001098:
	movs	r0, #13
	movs	r1, #6
	bl 0x0200c94c
	b.n	.L_020010b0
.L_020010a2:
	ldr	r0, [pc, #32]
	bl 0x0200c98c
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c9a4
.L_020010b0:
	bl 0x0200c8cc
.L_020010b4:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00002a1e
	.4byte 0x00002962
	.4byte 0x02000240
	.2byte 0x292d
	.2byte 0x0000
	push	{lr}
	bl 0x02008f94
	cmp	r0, #0
	beq.n	.L_020010da
	movs	r0, #8
	bl 0x0200ca7c
	b.n	.L_0200110c
.L_020010da:
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #254
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_020010fa
	ldr	r0, [pc, #28]
	bl 0x0200c98c
	b.n	.L_02001100
.L_020010fa:
	ldr	r0, [pc, #24]
	bl 0x0200c98c
.L_02001100:
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c9a4
	bl 0x0200c8cc
.L_0200110c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00002a2a
	.2byte 0x2966
	.2byte 0x0000
	push	{lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #16]
	bl 0x0200c98c
	movs	r1, #0
	movs	r0, #13
	bl 0x0200c9ac
	bl 0x0200c8cc
	pop	{pc}
	.2byte 0x29dc
	.2byte 0x0000
	push	{lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #237
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_0200115e
	ldr	r0, [pc, #92]
	bl 0x0200c98c
	b.n	.L_02001180
.L_0200115e:
	ldr	r0, [pc, #88]
	bl 0x0200c98c
	movs	r1, #0
	movs	r0, #11
	bl 0x0200c994
	ldr	r3, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200c8d4
	cmp	r0, #1
	bne.n	.L_0200118a
.L_02001180:
	movs	r0, #11
	movs	r1, #0
	bl 0x0200c9a4
	b.n	.L_020011ae
.L_0200118a:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #11
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #237
	bl 0x0200c7f4
.L_020011ae:
	bl 0x0200c8cc
	pop	{pc}
	.4byte 0x00002a30
	.4byte 0x00002a0f
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x02008f94
	cmp	r0, #0
	beq.n	.L_020011d4
	movs	r0, #29
	movs	r1, #12
	bl 0x0200ca74
	b.n	.L_020011f0
.L_020011d4:
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #20]
	bl 0x0200c98c
	movs	r0, #12
	movs	r1, #0
	bl 0x0200c9a4
	bl 0x0200c8cc
.L_020011f0:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2a1b
	.2byte 0x0000
	push	{lr}
	movs	r0, #17
	bl 0x0200c8f4
	movs	r0, #1
	bl 0x0200c744
	movs	r1, #4
	movs	r2, #20
	movs	r0, #17
	bl 0x0200c964
	movs	r0, #229
	bl 0x0200ca9c
	movs	r0, #2
	bl 0x0200c744
	ldr	r1, [pc, #8]
	movs	r0, #16
	bl 0x0200c8fc
	pop	{pc}
	.2byte 0x0000
	.2byte 0xd0f8
	.2byte 0x0200
	push	{lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_0200126c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_0200125c
	ldr	r0, [pc, #276]
	bl 0x0200c98c
	b.n	.L_02001262
.L_0200125c:
	ldr	r0, [pc, #272]
	bl 0x0200c98c
.L_02001262:
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	b.n	.L_02001364
.L_0200126c:
	movs	r0, #0
	bl 0x0200ca9c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02001288
	ldr	r0, [pc, #240]
	bl 0x0200c98c
	b.n	.L_0200128e
.L_02001288:
	ldr	r0, [pc, #236]
	bl 0x0200c98c
.L_0200128e:
	movs	r1, #0
	movs	r0, #17
	bl 0x0200c994
	ldr	r3, [pc, #228]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200c8d4
	cmp	r0, #1
	bne.n	.L_020012b4
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	b.n	.L_02001360
.L_020012b4:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #0
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #17
	bl 0x0200c9a4
	movs	r0, #11
	bl 0x0200ca9c
	movs	r0, #17
	movs	r1, #5
	bl 0x0200c94c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	bl 0x020091f8
	movs	r0, #17
	movs	r1, #6
	bl 0x0200c94c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	bl 0x020091f8
	movs	r0, #17
	movs	r1, #5
	bl 0x0200c94c
	movs	r1, #0
	movs	r0, #17
	bl 0x0200c9a4
	bl 0x020091f8
	movs	r0, #20
	bl 0x0200c8bc
	movs	r0, #17
	movs	r1, #1
	bl 0x0200c94c
	movs	r0, #17
	movs	r1, #3
	bl 0x0200c94c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200c9d4
	movs	r0, #40
	bl 0x0200c8bc
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #17
	movs	r1, #1
	bl 0x0200c974
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200c7f4
.L_02001360:
	bl 0x0200ca2c
.L_02001364:
	bl 0x0200c8cc
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00002a09
	.4byte 0x000029fe
	.4byte 0x00002a00
	.4byte 0x000029f5
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02001396
	ldr	r0, [pc, #664]
	bl 0x0200c98c
	b.n	.L_02001518
.L_02001396:
	movs	r0, #78
	bl 0x0200ca9c
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r1, #204
	lsls	r1, r1, #6
	ldr	r0, [pc, #640]
	adds	r1, #51
	bl 0x0200c9e4
	movs	r0, #224
	movs	r1, #1
	movs	r2, #140
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c9ec
	ldr	r5, [pc, #616]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r2, #153
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #608]
	adds	r2, #153
	bl 0x0200c8e4
	movs	r2, #140
	ldr	r0, [r5, #0]
	movs	r1, #182
	lsls	r2, r2, #1
	bl 0x0200c91c
	movs	r1, #0
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200c9b4
	movs	r0, #76
	bl 0x0200ca9c
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #8
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r1, #3
	movs	r0, #15
	bl 0x0200c954
	ldr	r0, [pc, #532]
	bl 0x0200c98c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #192
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r1, #204
	lsls	r1, r1, #7
	ldr	r0, [pc, #504]
	adds	r1, #102
	bl 0x0200c9e4
	movs	r0, #224
	movs	r1, #1
	movs	r2, #182
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200c9ec
	bl 0x0200c9f4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #129
	movs	r0, #15
	lsls	r1, r1, #1
	bl 0x0200c9d4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #224
	movs	r1, #1
	movs	r2, #140
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200c9ec
	bl 0x0200c9f4
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #178
	movs	r1, #1
	movs	r2, #220
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200c9ec
	bl 0x0200c9f4
	movs	r0, #10
	bl 0x0200c8bc
	movs	r0, #135
	movs	r1, #1
	movs	r2, #220
	lsls	r2, r2, #16
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200c9ec
	bl 0x0200c9f4
	movs	r0, #20
	bl 0x0200c8bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #226
	movs	r1, #1
	movs	r2, #140
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x0200c9ec
	bl 0x0200c9f4
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
.L_02001518:
	movs	r1, #192
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c954
	movs	r1, #128
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #6
	bl 0x0200c9b4
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r0, #16
	movs	r1, #3
	bl 0x0200c94c
	movs	r0, #17
	movs	r1, #3
	bl 0x0200c954
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #16
	ldr	r1, [pc, #200]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #17
	ldr	r1, [pc, #184]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #128
	movs	r0, #16
	movs	r1, #184
	lsls	r2, r2, #1
	bl 0x0200c914
	movs	r1, #132
	movs	r2, #128
	movs	r0, #17
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200c91c
	ldr	r3, [pc, #156]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #192
	ldr	r0, [r3, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #192
	movs	r2, #0
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9b4
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #136]
	adds	r1, #204
	bl 0x0200c9e4
	movs	r0, #224
	movs	r1, #1
	movs	r2, #182
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200c9ec
	movs	r0, #16
	movs	r1, #184
	movs	r2, #128
	bl 0x0200c914
	movs	r1, #132
	movs	r2, #128
	movs	r0, #17
	lsls	r1, r1, #1
	bl 0x0200c91c
	movs	r1, #1
	movs	r0, #16
	bl 0x0200c94c
	movs	r0, #1
	bl 0x0200c744
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	movs	r1, #0
	movs	r2, #0
	movs	r0, #17
	bl 0x0200c934
	bl 0x0200ca14
	bl 0x0200ca1c
	movs	r0, #158
	lsls	r0, r0, #4
	bl 0x0200c7f4
	movs	r0, #28
	adds	r0, #255
	bl 0x0200c7f4
	movs	r0, #10
	bl 0x0200ca04
	pop	{r5, pc}
	.4byte 0x00002999
	.4byte 0x00019999
	.4byte 0x02000240
	.4byte 0x00013333
	.4byte 0x00002993
	.4byte 0x00033333
	.2byte 0x6666
	.2byte 0x0002
	push	{r5, lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r1, #204
	lsls	r1, r1, #7
	ldr	r0, [pc, #244]
	adds	r1, #102
	bl 0x0200c9e4
	movs	r0, #182
	movs	r1, #1
	movs	r2, #152
	lsls	r2, r2, #17
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200c9ec
	bl 0x0200c9f4
	ldr	r0, [pc, #220]
	bl 0x0200c98c
	movs	r1, #0
	movs	r0, #15
	bl 0x0200c994
	ldr	r3, [pc, #208]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200c94c
	movs	r1, #224
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c8d4
	cmp	r0, #0
	bne.n	.L_020016f4
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #160]
	adds	r1, #153
	bl 0x0200c9e4
	movs	r0, #168
	movs	r1, #1
	movs	r2, #184
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c9ec
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #132]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #184
	ldr	r0, [r5, #0]
	movs	r1, #168
	lsls	r2, r2, #1
	bl 0x0200c91c
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c7fc
	b.n	.L_02001746
.L_020016f4:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #15
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #152
	lsls	r1, r1, #6
	ldr	r0, [pc, #76]
	adds	r1, #102
	bl 0x0200c9e4
	movs	r0, #168
	movs	r1, #1
	movs	r2, #140
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x0200c9ec
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #40]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #140
	ldr	r0, [r5, #0]
	movs	r1, #168
	lsls	r2, r2, #1
	bl 0x0200c91c
.L_02001746:
	bl 0x0200c8cc
	pop	{r5, pc}
	.4byte 0x00033333
	.4byte 0x000029b9
	.4byte 0x02000240
	.4byte 0x0004cccc
	.4byte 0x00019999
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, r6, lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #252]
	adds	r1, #204
	bl 0x0200c9e4
	movs	r0, #224
	movs	r1, #1
	movs	r2, #140
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x0200c9ec
	ldr	r3, [pc, #232]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c8e4
	movs	r2, #140
	ldr	r0, [r5, #0]
	movs	r1, #182
	lsls	r2, r2, #1
	bl 0x0200c91c
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200c9b4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #15
	bl 0x0200c9bc
	ldr	r0, [pc, #152]
	bl 0x0200c98c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #15
	bl 0x0200c994
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c8d4
	cmp	r0, #1
	bne.n	.L_02001810
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	bl 0x0200a89c
	b.n	.L_0200186c
.L_02001810:
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r2, [r6, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #6
	movs	r1, #0
	strh	r3, [r2, #0]
	adds	r0, #15
	bl 0x0200c994
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c8d4
	cmp	r0, #0
	bne.n	.L_0200185a
	movs	r0, #76
	bl 0x0200ca9c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200c7f4
	bl 0x02009380
	b.n	.L_0200186c
.L_0200185a:
	ldr	r2, [r6, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	bl 0x0200a90c
.L_0200186c:
	bl 0x0200c8cc
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00026666
	.4byte 0x02000240
	.2byte 0x29be
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #520]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	bl 0x0200c8dc
	adds	r5, r0, #0
	movs	r0, #15
	bl 0x0200c8dc
	mov	r8, r0
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r0, #16
	bl 0x0200c8f4
	movs	r0, #17
	bl 0x0200c8f4
	movs	r0, #15
	bl 0x0200c8f4
	movs	r1, #160
	movs	r2, #0
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #6
	bl 0x0200c9bc
	movs	r0, #16
	movs	r1, #3
	bl 0x0200c954
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #16
	ldr	r1, [pc, #436]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r0, #16
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #15
	bl 0x0200c8dc
	cmp	r0, #0
	beq.n	.L_02001906
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #16
	bl 0x0200c904
.L_02001906:
	movs	r0, #16
	bl 0x0200c92c
	movs	r2, #0
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c934
	movs	r1, #3
	movs	r0, #15
	bl 0x0200c94c
	ldr	r0, [pc, #376]
	bl 0x0200c98c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	ldr	r0, [r7, #0]
	movs	r1, #15
	movs	r2, #0
	bl 0x0200c97c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #7
	bl 0x0200c9b4
	movs	r1, #224
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #17
	movs	r1, #3
	bl 0x0200c954
	movs	r0, #17
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #15
	bl 0x0200c8dc
	cmp	r0, #0
	beq.n	.L_02001974
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #17
	bl 0x0200c904
.L_02001974:
	movs	r0, #17
	bl 0x0200c92c
	movs	r2, #0
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c934
	movs	r1, #160
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	bl 0x0200c9ec
	movs	r1, #204
	lsls	r1, r1, #6
	ldr	r0, [pc, #228]
	adds	r1, #51
	bl 0x0200c9e4
	movs	r0, #224
	movs	r1, #1
	movs	r2, #200
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	adds	r6, r5, #0
	mov	r5, r8
	bl 0x0200c9ec
	adds	r6, #99
	movs	r3, #0
	adds	r5, #99
	strb	r3, [r6, #0]
	strb	r3, [r5, #0]
	ldr	r0, [r7, #0]
	ldr	r1, [pc, #196]
	bl 0x0200c8ec
	ldr	r1, [pc, #192]
	movs	r0, #15
	bl 0x0200c8ec
.L_020019e4:
	movs	r0, #1
	bl 0x0200c744
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_020019e4
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_020019e4
	bl 0x0200b040
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200c9e4
	movs	r0, #224
	movs	r1, #1
	movs	r2, #168
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200c9ec
	ldr	r5, [pc, #112]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #104]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #15
	ldr	r1, [pc, #88]
	adds	r2, #204
	bl 0x0200c8e4
	ldr	r0, [r5, #0]
	movs	r1, #184
	movs	r2, #128
	bl 0x0200c914
	movs	r1, #132
	movs	r2, #128
	movs	r0, #15
	lsls	r1, r1, #1
	bl 0x0200c91c
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200c94c
	movs	r0, #1
	bl 0x0200c744
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	bl 0x0200ca14
	bl 0x0200ca1c
	bl 0x0200c8b4
	movs	r0, #11
	bl 0x0200ca04
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x00019999
	.4byte 0x000029bc
	.4byte 0x0200cc5c
	.2byte 0xccac
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #520]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	bl 0x0200c8dc
	adds	r5, r0, #0
	movs	r0, #15
	bl 0x0200c8dc
	mov	r8, r0
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r0, #16
	bl 0x0200c8f4
	movs	r0, #17
	bl 0x0200c8f4
	movs	r0, #15
	bl 0x0200c8f4
	movs	r1, #224
	movs	r2, #0
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9b4
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r0, #17
	movs	r1, #3
	bl 0x0200c954
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #17
	ldr	r1, [pc, #436]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r0, #17
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #15
	bl 0x0200c8dc
	cmp	r0, #0
	beq.n	.L_02001b2a
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #17
	bl 0x0200c904
.L_02001b2a:
	movs	r0, #17
	bl 0x0200c92c
	movs	r2, #0
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c934
	movs	r1, #3
	movs	r0, #15
	bl 0x0200c94c
	ldr	r0, [pc, #376]
	bl 0x0200c98c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	ldr	r0, [r7, #0]
	movs	r1, #15
	movs	r2, #0
	bl 0x0200c97c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #6
	bl 0x0200c9b4
	movs	r1, #160
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #16
	movs	r1, #3
	bl 0x0200c954
	movs	r0, #16
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #15
	bl 0x0200c8dc
	cmp	r0, #0
	beq.n	.L_02001b98
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #16
	bl 0x0200c904
.L_02001b98:
	movs	r0, #16
	bl 0x0200c92c
	movs	r2, #0
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c934
	movs	r1, #224
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	bl 0x0200c9ec
	movs	r1, #204
	lsls	r1, r1, #6
	ldr	r0, [pc, #228]
	adds	r1, #51
	bl 0x0200c9e4
	movs	r0, #224
	movs	r1, #1
	movs	r2, #200
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	adds	r6, r5, #0
	mov	r5, r8
	bl 0x0200c9ec
	adds	r6, #99
	movs	r3, #0
	adds	r5, #99
	strb	r3, [r6, #0]
	ldr	r1, [pc, #200]
	movs	r0, #15
	strb	r3, [r5, #0]
	bl 0x0200c8ec
	ldr	r0, [r7, #0]
	ldr	r1, [pc, #192]
	bl 0x0200c8ec
.L_02001c08:
	movs	r0, #1
	bl 0x0200c744
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_02001c08
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001c08
	bl 0x0200b040
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200c9e4
	movs	r0, #224
	movs	r1, #1
	movs	r2, #168
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200c9ec
	ldr	r5, [pc, #112]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #104]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #15
	ldr	r1, [pc, #88]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r0, #15
	movs	r1, #184
	movs	r2, #128
	bl 0x0200c914
	movs	r1, #132
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200c91c
	movs	r1, #1
	movs	r0, #15
	bl 0x0200c94c
	movs	r0, #1
	bl 0x0200c744
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	bl 0x0200ca14
	bl 0x0200ca1c
	bl 0x0200c8b4
	movs	r0, #12
	bl 0x0200ca04
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x00019999
	.4byte 0x000029bc
	.4byte 0x0200cc5c
	.2byte 0xccac
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #14
	sub	sp, #8
	bl 0x0200c8dc
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #32
	bne.n	.L_02001d38
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #11
	bne.n	.L_02001d38
	bl 0x0200c8c4
	adds	r7, r5, #0
	movs	r0, #0
	bl 0x0200ca24
	adds	r7, #85
	movs	r3, #3
	strb	r3, [r7, #0]
.L_02001cf6:
	movs	r0, #1
	bl 0x0200c744
	ldr	r6, [r5, #40]
	cmp	r6, #0
	bne.n	.L_02001cf6
	movs	r0, #188
	bl 0x0200ca9c
	movs	r0, #10
	bl 0x0200c744
	movs	r3, #32
	movs	r2, #11
	strb	r6, [r7, #0]
	movs	r1, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #1
	movs	r0, #30
	bl 0x0200c84c
	movs	r0, #1
	bl 0x0200c744
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #78
	bl 0x0200c7f4
	bl 0x0200c8cc
.L_02001d38:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #8
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r0, #158
	bl 0x0200ca9c
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #6
	movs	r1, #40
	movs	r2, #7
	movs	r0, #43
	bl 0x0200c844
	movs	r0, #20
	bl 0x0200c8bc
	movs	r1, #240
	movs	r2, #194
	movs	r0, #15
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x0200c934
	ldr	r5, [pc, #524]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #516]
	ldr	r2, [pc, #520]
	bl 0x0200c8e4
	ldr	r0, [r5, #0]
	movs	r1, #4
	movs	r2, #0
	bl 0x0200c964
	ldr	r0, [r5, #0]
	bl 0x0200c8dc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r6, #254
	adds	r3, r6, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #0
	movs	r1, #120
	mov	sl, r2
	ldr	r0, [r5, #0]
	movs	r2, #220
	bl 0x0200c91c
	movs	r0, #1
	bl 0x0200c8bc
	ldr	r0, [r5, #0]
	bl 0x0200c8dc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #1
	mov	r8, r2
	mov	r2, r8
	orrs	r3, r2
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	strb	r3, [r0, #0]
	adds	r1, #102
	movs	r0, #15
	adds	r2, #51
	bl 0x0200c8e4
	movs	r2, #208
	movs	r0, #15
	movs	r1, #120
	bl 0x0200c91c
	movs	r0, #16
	movs	r1, #15
	bl 0x0200c944
	movs	r1, #15
	movs	r0, #17
	bl 0x0200c944
	movs	r0, #1
	bl 0x0200c744
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #16
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c8e4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #17
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c8e4
	movs	r0, #16
	movs	r1, #104
	movs	r2, #212
	bl 0x0200c914
	movs	r2, #212
	movs	r0, #17
	movs	r1, #136
	bl 0x0200c91c
	movs	r0, #16
	movs	r1, #1
	bl 0x0200c94c
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #7
	bl 0x0200c9b4
	movs	r1, #1
	movs	r0, #15
	bl 0x0200c96c
	ldr	r0, [pc, #304]
	bl 0x0200c98c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r1, #153
	adds	r0, #204
	bl 0x0200c9e4
	bl 0x0200c9fc
	mov	r3, sl
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #200
	movs	r0, #240
	movs	r2, #224
	movs	r3, #1
	lsls	r0, r0, #15
	lsls	r1, r1, #14
	lsls	r2, r2, #16
	bl 0x0200c9ec
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #244]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r2, r2, #1
	movs	r1, #120
	bl 0x0200c91c
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #102
	adds	r2, #51
	bl 0x0200c8e4
	ldr	r0, [r5, #0]
	bl 0x0200c8dc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #132
	ands	r6, r3
	strb	r6, [r0, #0]
	movs	r1, #120
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	bl 0x0200c91c
	movs	r0, #1
	bl 0x0200c8bc
	ldr	r0, [r5, #0]
	bl 0x0200c8dc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r2, r3
	movs	r1, #4
	strb	r2, [r0, #0]
	mov	r8, r2
	adds	r1, #255
	movs	r2, #20
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #17
	movs	r1, #1
	bl 0x0200c96c
	movs	r1, #4
	movs	r2, #0
	adds	r1, #255
	movs	r0, #17
	bl 0x0200c9cc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #16
	movs	r1, #1
	bl 0x0200c96c
	movs	r1, #4
	movs	r2, #0
	adds	r1, #255
	movs	r0, #16
	bl 0x0200c9cc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #16
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c954
	movs	r0, #128
.L_02001f6a:
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #15
	bl 0x0200c9a4
	movs	r0, #133
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200c7f4
	bl 0x0200c8cc
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x00026666
	.4byte 0x00013333
	.4byte 0x00002939
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, r7, lr}
	adds	r4, r0, #0
.L_02001fa4:
	adds	r6, r2, #0
	adds	r5, r1, #0
	lsls	r3, r3, #16
	movs	r0, #244
	asrs	r7, r3, #16
	lsls	r0, r0, #1
	adds	r3, r6, #0
	adds	r1, r4, #0
	adds	r2, r5, #0
	bl 0x0200c814
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02001fe4
	movs	r0, #151
	ldr	r5, [r6, #80]
	bl 0x0200ca9c
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200c804
	ldr	r1, [pc, #20]
	adds	r0, r6, #0
	bl 0x0200c80c
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	strb	r3, [r5, #26]
	strh	r7, [r5, #18]
.L_02001fe4:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xe340
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #234
	movs	r1, #240
	movs	r2, #128
	movs	r3, #132
	adds	r0, #255
	lsls	r1, r1, #15
	lsls	r2, r2, #15
	lsls	r3, r3, #17
	bl 0x0200c814
	adds	r6, r0, #0
	movs	r7, #0
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L_02002064
	ldr	r5, [r6, #80]
	movs	r3, #33
	ldrb	r2, [r5, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r5, #9]
	adds	r3, r6, #0
	adds	r3, #85
	adds	r2, r6, #0
	strb	r7, [r3, #0]
	adds	r2, #92
	movs	r3, #1
	movs	r1, #193
	strb	r3, [r2, #0]
	lsls	r1, r1, #3
	strb	r7, [r5, #26]
	strb	r7, [r5, #27]
	movs	r0, #68
	bl 0x0200c774
	adds	r7, r0, #0
	movs	r0, #65
	bl 0x0200c884
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r2, r7, r3
	movs	r1, #128
	ldrb	r0, [r5, #16]
	bl 0x0200c7ac
	movs	r0, #68
	bl 0x0200c77c
	adds	r0, r6, #0
.L_02002064:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r0, #78
	bl 0x0200ca9c
	ldr	r3, [pc, #80]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	bl 0x0200c8dc
	movs	r6, #192
	ldr	r3, [pc, #60]
	lsls	r6, r6, #8
	strh	r6, [r0, #6]
	movs	r0, #1
	mov	r8, r3
	bl 0x0200c744
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r1, #153
	adds	r0, #204
	bl 0x0200c9e4
	bl 0x0200c9fc
	mov	r2, r8
	adds	r0, #85
	strb	r2, [r0, #0]
	movs	r1, #160
	movs	r0, #240
	movs	r2, #246
	movs	r3, #1
	lsls	r0, r0, #15
	lsls	r1, r1, #14
	lsls	r2, r2, #16
	bl 0x0200c9ec
	movs	r2, #153
	b.n	.L_020020d4
	.4byte 0x00000000
	.2byte 0x0240
	.2byte 0x0200
.L_020020d4:
	lsls	r2, r2, #8
	ldr	r0, [r7, #0]
	ldr	r1, [pc, #1012]
	adds	r2, #153
	bl 0x0200c8e4
	movs	r2, #132
	lsls	r2, r2, #1
	ldr	r0, [r7, #0]
	movs	r1, #120
	bl 0x0200c91c
	movs	r1, #28
	ldr	r0, [r7, #0]
	bl 0x0200c94c
	bl 0x02009fec
	movs	r1, #128
	adds	r5, r0, #0
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #15
	ldr	r1, [pc, #968]
	ldr	r2, [pc, #964]
	bl 0x0200c8e4
	movs	r1, #120
	movs	r2, #242
	movs	r0, #15
	bl 0x0200c91c
	movs	r0, #29
	bl 0x0200ca9c
	ldr	r0, [pc, #948]
	bl 0x0200c98c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #20
	bl 0x0200c8bc
	cmp	r5, #0
	beq.n	.L_0200213e
	adds	r0, r5, #0
	bl 0x0200c81c
.L_0200213e:
	ldr	r0, [r7, #0]
	movs	r1, #1
	bl 0x0200c94c
	adds	r1, r6, #0
	ldr	r0, [r7, #0]
	bl 0x0200c9bc
	ldr	r1, [r7, #0]
	movs	r0, #18
	bl 0x0200c944
	ldr	r1, [r7, #0]
.L_02002158:
	movs	r0, #7
	bl 0x0200c944
	ldr	r1, [r7, #0]
	movs	r0, #5
	bl 0x0200c944
	ldr	r1, [r7, #0]
	movs	r0, #6
	bl 0x0200c944
	movs	r0, #1
	bl 0x0200c744
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #18
	ldr	r1, [pc, #864]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #848]
	adds	r2, #204
.L_0200218c:
	bl 0x0200c8e4
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #836]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #6
	ldr	r1, [pc, #820]
	bl 0x0200c8e4
	ldr	r1, [pc, #816]
	movs	r0, #18
	bl 0x0200c8ec
	ldr	r1, [pc, #812]
	movs	r0, #7
	bl 0x0200c8ec
	ldr	r1, [pc, #808]
	movs	r0, #5
	bl 0x0200c8ec
	ldr	r1, [pc, #804]
	movs	r0, #6
	bl 0x0200c8ec
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #17
	bl 0x0200c9cc
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #17
	ldr	r1, [pc, #752]
	adds	r2, #153
	bl 0x0200c8e4
	movs	r2, #232
	movs	r0, #17
	movs	r1, #136
	bl 0x0200c91c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #16
	ldr	r1, [pc, #720]
	adds	r2, #153
	bl 0x0200c8e4
	movs	r2, #232
	movs	r0, #16
	movs	r1, #104
	bl 0x0200c91c
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c954
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #18
	bl 0x0200c9d4
	movs	r0, #20
.L_02002248:
	bl 0x0200c8bc
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #15
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c8e4
	movs	r1, #120
	movs	r2, #252
	movs	r0, #15
	bl 0x0200c91c
	movs	r0, #40
	bl 0x0200c8bc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #20
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #15
	movs	r1, #136
	movs	r2, #252
	bl 0x0200c91c
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200c9b4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #15
	movs	r1, #104
	movs	r2, #252
	bl 0x0200c91c
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200c9b4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #20
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #15
	movs	r1, #152
	movs	r2, #252
	bl 0x0200c91c
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200c9b4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #15
	movs	r1, #88
	movs	r2, #252
	bl 0x0200c91c
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #40
	bl 0x0200c9b4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #80
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #15
	movs	r1, #120
	movs	r2, #252
	bl 0x0200c91c
	movs	r2, #242
	movs	r0, #15
	movs	r1, #120
	bl 0x0200c91c
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r0, #15
	movs	r1, #4
	bl 0x0200c94c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	ldr	r0, [r7, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #18
	bl 0x0200c9cc
	movs	r2, #0
	movs	r0, #18
	movs	r1, #4
	bl 0x0200c964
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	adds	r1, r6, #0
	movs	r0, #6
	movs	r2, #0
	bl 0x0200c9b4
	adds	r1, r6, #0
	movs	r0, #5
	movs	r2, #0
	bl 0x0200c9b4
	adds	r1, r6, #0
	ldr	r0, [r7, #0]
	movs	r2, #0
	bl 0x0200c9b4
	movs	r2, #0
	adds	r1, r6, #0
	movs	r0, #7
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r0, #15
	movs	r1, #4
	bl 0x0200c954
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #4
	movs	r2, #0
	adds	r1, #255
	movs	r0, #6
	bl 0x0200c9cc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #192
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #5
	bl 0x0200c9cc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c954
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #7
	bl 0x0200c9cc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #15
	lsls	r1, r1, #6
	bl 0x0200c9b4
	movs	r1, #129
	movs	r0, #18
	lsls	r1, r1, #1
	bl 0x0200c9d4
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #131
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200c9cc
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #15
	bl 0x0200c9cc
	movs	r1, #224
	movs	r2, #20
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9b4
	movs	r0, #17
	movs	r1, #3
	bl 0x0200c94c
	movs	r5, #160
	movs	r0, #17
	movs	r1, #0
	movs	r6, #242
	bl 0x0200c9a4
	lsls	r5, r5, #15
	lsls	r6, r6, #16
	movs	r0, #232
	movs	r3, #224
	adds	r1, r5, #0
	lsls	r3, r3, #8
	adds	r2, r6, #0
	lsls	r0, r0, #15
	bl 0x02009fa0
	movs	r0, #10
	bl 0x0200c8bc
	movs	r0, #248
	movs	r3, #128
	b.n	.L_020024f0
	.4byte 0x00013333
	.4byte 0x00026666
	.4byte 0x00002947
	.4byte 0x00019999
	.4byte 0x0200ccfc
	.4byte 0x0200cd40
	.4byte 0x0200cd84
	.2byte 0xcdc8
	.2byte 0x0200
.L_020024f0:
	lsls	r3, r3, #6
.L_020024f2:
	adds	r1, r5, #0
	adds	r2, r6, #0
	lsls	r0, r0, #15
	bl 0x02009fa0
	movs	r0, #30
	bl 0x0200c8bc
	movs	r1, #131
	movs	r2, #20
.L_02002506:
	lsls	r1, r1, #1
	movs	r0, #16
	bl 0x0200c9cc
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c9bc
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #17
	movs	r1, #3
	bl 0x0200c954
	movs	r1, #132
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c954
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #7
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c94c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	adds	r1, #204
	bl 0x0200c9e4
	movs	r0, #180
	movs	r1, #128
	adds	r2, r6, #0
	lsls	r1, r1, #14
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200c9ec
	movs	r0, #16
	bl 0x0200c8dc
	adds	r5, r0, #0
	mov	r3, r8
	adds	r5, #99
	strb	r3, [r5, #0]
	ldr	r0, [r7, #0]
	movs	r1, #15
	bl 0x0200ca3c
	movs	r0, #5
	movs	r1, #15
	bl 0x0200ca3c
	movs	r0, #6
	movs	r1, #15
	bl 0x0200ca3c
	movs	r0, #7
	movs	r1, #15
	bl 0x0200ca3c
	movs	r0, #18
	movs	r1, #15
	bl 0x0200ca3c
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #15
	ldr	r1, [pc, #420]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #16
	ldr	r1, [pc, #404]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #17
	ldr	r1, [pc, #388]
	bl 0x0200c8e4
	ldr	r1, [pc, #388]
	movs	r0, #15
	bl 0x0200c8ec
	movs	r0, #20
	bl 0x0200c8bc
	ldr	r1, [pc, #376]
	movs	r0, #17
	bl 0x0200c8ec
	movs	r0, #16
	ldr	r1, [pc, #372]
	bl 0x0200c8ec
	movs	r0, #78
	bl 0x0200ca9c
.L_02002622:
	movs	r0, #1
	bl 0x0200c744
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02002622
	ldr	r5, [pc, #352]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200c8f4
	movs	r0, #18
	bl 0x0200c8f4
	movs	r0, #7
	bl 0x0200c8f4
	movs	r0, #5
	bl 0x0200c8f4
	movs	r0, #6
	bl 0x0200c8f4
	movs	r0, #1
	bl 0x0200c744
	bl 0x0200ca2c
	movs	r1, #204
	lsls	r1, r1, #7
	ldr	r0, [pc, #304]
	adds	r1, #102
	bl 0x0200c9e4
	movs	r0, #240
	movs	r1, #1
	movs	r2, #130
	movs	r3, #1
	lsls	r0, r0, #15
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c9ec
	bl 0x0200c9f4
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	movs	r2, #0
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c934
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c9bc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #128
	movs	r0, #18
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c9b4
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c9b4
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c9bc
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200c954
	movs	r1, #128
	movs	r2, #128
	movs	r0, #6
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c8e4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #5
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c8e4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #7
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c8e4
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #18
	lsls	r1, r1, #9
	bl 0x0200c8e4
	ldr	r5, [pc, #92]
	movs	r0, #6
	adds	r1, r5, #0
	bl 0x0200c8ec
	adds	r1, r5, #0
	movs	r0, #5
	bl 0x0200c8ec
	adds	r1, r5, #0
	movs	r0, #7
	bl 0x0200c8ec
	adds	r1, r5, #0
	movs	r0, #18
	bl 0x0200c8fc
	movs	r0, #20
	bl 0x0200c8bc
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200c7f4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #225
	bl 0x0200c7f4
	bl 0x0200c8cc
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x00019999
	.4byte 0x0200ce0c
	.4byte 0x0200ce5c
	.4byte 0x0200ceac
	.4byte 0x02000240
	.4byte 0x00033333
	.2byte 0xcf08
	.2byte 0x0200
	push	{lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #16]
	bl 0x0200c98c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	bl 0x0200c8cc
	pop	{pc}
	.2byte 0x2944
	.2byte 0x0000
	push	{lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #16]
	bl 0x0200c98c
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c9a4
	bl 0x0200c8cc
	pop	{pc}
	.2byte 0x2946
	.2byte 0x0000
	push	{lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #16]
	bl 0x0200c98c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	bl 0x0200c8cc
	pop	{pc}
	.2byte 0x2945
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #254
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_0200286c
	movs	r0, #0
	bl 0x0200ca9c
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #68]
	bl 0x0200c98c
	movs	r0, #17
	movs	r1, #5
	bl 0x0200c94c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #192
	ldr	r0, [r3, #0]
	movs	r2, #20
	lsls	r1, r1, #8
	bl 0x0200c9b4
	bl 0x020091f8
	movs	r0, #17
	movs	r1, #1
	bl 0x0200c94c
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c7f4
	bl 0x0200c8cc
.L_0200286c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x000029f4
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r2, #17
	movs	r3, #2
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r0, #0
	str	r3, [sp, #0]
	bl 0x0200c84c
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c7f4
	add	sp, #8
	pop	{pc}
	push	{r5, lr}
	ldr	r5, [pc, #100]
	movs	r3, #133
	lsls	r3, r3, #2
	movs	r2, #204
	adds	r5, r5, r3
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	adds	r2, #204
	ldr	r1, [pc, #88]
	bl 0x0200c8e4
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200c9dc
	movs	r2, #152
	ldr	r0, [r5, #0]
	movs	r1, #168
	lsls	r2, r2, #1
	bl 0x0200c91c
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c9b4
	movs	r2, #184
	ldr	r0, [r5, #0]
	movs	r1, #168
	lsls	r2, r2, #1
	bl 0x0200c91c
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c7fc
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r0, [pc, #256]
	sub	sp, #20
	bl 0x0200c98c
	movs	r0, #65
	bl 0x0200c89c
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_020029aa
	mov	r0, sp
	bl 0x0200c8a4
	movs	r3, #0
	movs	r2, #8
	mov	r9, r0
	mov	r8, r3
	mov	r7, sp
	mov	sl, r2
	cmp	r8, r9
	bge.n	.L_02002992
.L_02002942:
	movs	r1, #0
	ldrsh	r5, [r7, r1]
	adds	r7, #2
	adds	r0, r5, #0
	bl 0x0200c7e4
	adds	r6, r0, #0
	movs	r5, #0
	adds	r6, #216
.L_02002954:
	cmp	r5, #14
	bgt.n	.L_02002984
	ldrh	r0, [r6, #0]
	cmp	r0, #0
	beq.n	.L_02002984
	bl 0x0200c894
	ldrb	r3, [r0, #2]
	movs	r2, #192
	adds	r3, #255
	lsls	r3, r3, #24
	lsls	r2, r2, #18
	cmp	r3, r2
	bhi.n	.L_0200297a
	mov	r3, sl
	adds	r3, #255
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	sl, r3
.L_0200297a:
	mov	r3, sl
	adds	r6, #2
	adds	r5, #1
	cmp	r3, #0
	bne.n	.L_02002954
.L_02002984:
	mov	r1, sl
	cmp	r1, #0
	beq.n	.L_02002992
	movs	r2, #1
	add	r8, r2
	cmp	r8, r9
	blt.n	.L_02002942
.L_02002992:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r3, r1
	ldrh	r3, [r2, #0]
	mov	r1, sl
	adds	r3, #1
	strh	r3, [r2, #0]
	cmp	r1, #0
	beq.n	.L_020029c8
.L_020029aa:
	movs	r1, #2
	adds	r1, #255
	movs	r2, #20
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	bl 0x0200a89c
	b.n	.L_02002a0a
.L_020029c8:
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #6
	strh	r3, [r2, #0]
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	ldr	r5, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r1, [r5, #0]
	movs	r0, #16
	bl 0x0200ca3c
	ldr	r1, [r5, #0]
	movs	r0, #17
	bl 0x0200ca3c
	ldr	r1, [r5, #0]
	movs	r0, #15
	bl 0x0200ca3c
	movs	r0, #158
	lsls	r0, r0, #4
	bl 0x0200c7f4
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c7f4
.L_02002a0a:
	add	sp, #20
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x000029ac
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #348]
	sub	sp, #24
	str	r0, [sp, #8]
	str	r0, [sp, #12]
	ldr	r3, [pc, #344]
	ldr	r2, [pc, #344]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	mov	sl, r3
.L_02002a48:
	ldr	r1, [pc, #332]
	movs	r2, #0
	ldrsh	r5, [r1, r2]
	ldrh	r3, [r1, #0]
	cmp	r5, #0
	bne.n	.L_02002b28
	ldr	r4, [pc, #324]
	movs	r2, #128
	ldr	r0, [r4, #0]
	lsls	r2, r2, #6
	ldrh	r3, [r0, #0]
	adds	r0, #2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	str	r0, [r4, #0]
	cmp	r3, r2
	beq.n	.L_02002aee
	cmp	r3, r2
	bgt.n	.L_02002a80
	movs	r7, #1
	negs	r7, r7
	cmp	r3, r7
	beq.n	.L_02002b16
	movs	r1, #128
	lsls	r1, r1, #5
	cmp	r3, r1
	beq.n	.L_02002ad4
	b.n	.L_02002a48
.L_02002a80:
	movs	r2, #128
	lsls	r2, r2, #7
	cmp	r3, r2
	beq.n	.L_02002aa2
	cmp	r3, r2
	bgt.n	.L_02002a96
	movs	r2, #192
	lsls	r2, r2, #6
	cmp	r3, r2
	beq.n	.L_02002aba
	b.n	.L_02002a48
.L_02002a96:
	movs	r5, #254
	lsls	r5, r5, #7
	adds	r5, #255
	cmp	r3, r5
	beq.n	.L_02002b0c
	b.n	.L_02002a48
.L_02002aa2:
	movs	r7, #0
	ldrsh	r3, [r0, r7]
	ldr	r2, [pc, #248]
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	adds	r3, r0, #2
	ldrh	r2, [r3, #0]
	adds	r3, #2
	ldr	r1, [pc, #240]
	str	r3, [r4, #0]
	ldr	r3, [pc, #240]
	b.n	.L_02002b06
.L_02002aba:
	ldr	r2, [pc, #232]
	ldr	r1, [pc, #236]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldr	r1, [pc, #228]
	ldrh	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r3, r0, #2
	ldrh	r2, [r3, #0]
	adds	r3, #2
	str	r3, [r4, #0]
	ldr	r3, [pc, #220]
	b.n	.L_02002b06
.L_02002ad4:
	ldr	r2, [pc, #220]
	ldr	r1, [pc, #224]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldr	r1, [pc, #220]
	ldrh	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r3, r0, #2
	ldrh	r2, [r3, #0]
	adds	r3, #2
	str	r3, [r4, #0]
	ldr	r3, [pc, #212]
	b.n	.L_02002b06
.L_02002aee:
	ldr	r2, [pc, #212]
	ldr	r1, [pc, #212]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldr	r1, [pc, #212]
	ldrh	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r3, r0, #2
	ldrh	r2, [r3, #0]
	adds	r3, #2
	str	r3, [r4, #0]
	ldr	r3, [pc, #200]
.L_02002b06:
	strh	r2, [r1, #0]
	strh	r5, [r3, #0]
	b.n	.L_02002a48
.L_02002b0c:
	ldrh	r3, [r0, #0]
	strh	r3, [r1, #0]
	adds	r3, r0, #2
	str	r3, [r4, #0]
	b.n	.L_02002a48
.L_02002b16:
	ldr	r0, [pc, #188]
	bl 0x0200c754
	ldr	r3, [pc, #112]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl 0x0200c7a4
	b.n	.L_02002ef8
.L_02002b28:
	subs	r3, #1
	ldr	r2, [pc, #144]
	strh	r3, [r1, #0]
	mov	r8, r2
	movs	r3, #0
	ldrsh	r7, [r2, r3]
	ldr	r2, [pc, #124]
	cmp	r7, #0
	bne.n	.L_02002b42
	movs	r5, #0
	ldrsh	r4, [r2, r5]
	str	r4, [sp, #0]
	b.n	.L_02002b72
.L_02002b42:
	ldr	r3, [pc, #116]
	movs	r0, #0
	ldrsh	r6, [r3, r0]
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	ldr	r2, [pc, #112]
	subs	r3, r3, r6
	ldrh	r5, [r2, #0]
	adds	r1, r7, #0
	adds	r5, #1
	strh	r5, [r2, #0]
	lsls	r5, r5, #16
	asrs	r5, r5, #16
	adds	r0, r5, #0
	muls	r0, r3
	bl 0x0200c734
	adds	r6, r6, r0
	str	r6, [sp, #0]
	cmp	r5, r7
	blt.n	.L_02002b72
	ldr	r3, [pc, #24]
	mov	r2, r8
	strh	r3, [r2, #0]
.L_02002b72:
	ldr	r3, [pc, #88]
	ldr	r2, [pc, #76]
	movs	r4, #0
	ldrsh	r7, [r3, r4]
	mov	r8, r3
	cmp	r7, #0
	bne.n	.L_02002bd8
	movs	r7, #0
	ldrsh	r5, [r2, r7]
	str	r5, [sp, #4]
	b.n	.L_02002c08
	.4byte 0x00000000
	.4byte 0x0200e4d0
	.4byte 0x0200d158
	.4byte 0x020036e0
	.4byte 0x0200e4bc
	.4byte 0x0200e4c0
	.4byte 0x0200e4a0
	.4byte 0x0200e504
	.4byte 0x0200e49c
	.4byte 0x0200e500
	.4byte 0x0200e4a8
	.4byte 0x0200e4a4
	.4byte 0x0200e498
	.4byte 0x0200e490
	.4byte 0x0200e508
	.4byte 0x0200e4b4
	.4byte 0x0200e4b8
	.4byte 0x0200e4c4
	.4byte 0x0200e4ac
	.2byte 0xaa21
	.2byte 0x0200
.L_02002bd8:
	ldr	r3, [pc, #80]
	movs	r0, #0
	ldrsh	r6, [r3, r0]
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	ldr	r2, [pc, #76]
	subs	r3, r3, r6
	ldrh	r5, [r2, #0]
	adds	r1, r7, #0
	adds	r5, #1
	strh	r5, [r2, #0]
	lsls	r5, r5, #16
	asrs	r5, r5, #16
	adds	r0, r5, #0
	muls	r0, r3
	bl 0x0200c734
	adds	r6, r6, r0
	str	r6, [sp, #4]
	cmp	r5, r7
	blt.n	.L_02002c08
	ldr	r3, [pc, #36]
	mov	r2, r8
	strh	r3, [r2, #0]
.L_02002c08:
	ldr	r3, [pc, #40]
	ldr	r0, [sp, #0]
	movs	r4, #0
	ldrsh	r7, [r3, r4]
	add	r5, sp, #16
	lsls	r0, r0, #16
	mov	fp, r3
	ldr	r2, [pc, #32]
	mov	r8, r5
	mov	r9, r0
	cmp	r7, #0
	bne.n	.L_02002c3c
	movs	r1, #0
	ldrsh	r6, [r2, r1]
	b.n	.L_02002c6a
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200e4b8
	.4byte 0x0200e4ac
	.4byte 0x0200e49c
	.2byte 0xe504
	.2byte 0x0200
.L_02002c3c:
	ldr	r3, [pc, #100]
	adds	r1, r7, #0
	movs	r4, #0
	ldrsh	r6, [r3, r4]
	movs	r5, #0
	ldrsh	r3, [r2, r5]
	ldr	r2, [pc, #92]
	subs	r3, r3, r6
	ldrh	r5, [r2, #0]
	adds	r5, #1
	strh	r5, [r2, #0]
	lsls	r5, r5, #16
	asrs	r5, r5, #16
	adds	r0, r5, #0
	muls	r0, r3
	bl 0x0200c734
	adds	r6, r6, r0
	cmp	r5, r7
	blt.n	.L_02002c6a
	ldr	r3, [pc, #56]
	mov	r7, fp
	strh	r3, [r7, #0]
.L_02002c6a:
	mov	r0, r8
	ldr	r3, [r0, #4]
	ldr	r2, [pc, #60]
	ands	r3, r2
	str	r3, [r0, #4]
	mov	r3, r9
	lsrs	r1, r3, #16
	ldr	r3, [sp, #16]
	ands	r3, r2
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	orrs	r3, r1
	ands	r3, r2
	lsls	r1, r1, #16
	orrs	r3, r1
	str	r3, [sp, #16]
	bl 0x0200c7c4
	ldr	r2, [pc, #28]
	lsls	r0, r0, #16
	ldr	r3, [r2, #0]
	asrs	r0, r0, #16
	adds	r3, r3, r6
	mov	r8, r0
	str	r3, [r2, #0]
	b.n	.L_02002cb4
	.4byte 0x00000000
	.4byte 0x0200e500
	.4byte 0x0200e4a8
	.4byte 0xffff0000
	.2byte 0xe4a0
	.2byte 0x0200
.L_02002cb4:
	cmp	r3, #0
	bge.n	.L_02002cba
	adds	r3, #255
.L_02002cba:
	asrs	r6, r3, #8
	ldr	r3, [pc, #584]
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #2
	bne.n	.L_02002cc8
	b.n	.L_02002e34
.L_02002cc8:
	cmp	r3, #2
	bgt.n	.L_02002cd2
	cmp	r3, #1
	beq.n	.L_02002cdc
	b.n	.L_02002e8e
.L_02002cd2:
	cmp	r3, #3
	beq.n	.L_02002d5a
	cmp	r3, #4
	beq.n	.L_02002dda
	b.n	.L_02002e8e
.L_02002cdc:
	movs	r5, #0
.L_02002cde:
	ldr	r7, [sp, #0]
	lsls	r3, r5, #5
	subs	r3, #48
	muls	r3, r7
	movs	r1, #56
	ldr	r0, [pc, #544]
	cmp	r3, #0
	bge.n	.L_02002cf0
	adds	r3, #255
.L_02002cf0:
	asrs	r3, r3, #8
	adds	r3, r6, r3
	movs	r4, #48
	adds	r2, r3, #0
	adds	r4, #255
	adds	r3, #152
	adds	r2, #88
	cmp	r3, r4
	bhi.n	.L_02002d4e
	movs	r7, #0
	mov	ip, r7
	ldr	r7, [sp, #12]
	mov	r4, ip
	stmia	r7!, {r4}
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r2, r3
	adds	r3, r7, #0
	str	r3, [sp, #12]
	lsls	r3, r2, #16
	orrs	r3, r1
	orrs	r3, r0
	mov	r0, r8
	lsls	r2, r0, #25
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	adds	r2, r7, #0
	stmia	r2!, {r3}
	movs	r3, #244
	lsls	r3, r3, #8
	mov	r4, sl
	adds	r1, r2, #0
	orrs	r3, r4
	str	r1, [sp, #12]
	stmia	r2!, {r3}
	ldr	r0, [sp, #8]
	adds	r7, r2, #0
	adds	r1, r0, #0
	adds	r1, #12
	str	r1, [sp, #8]
	movs	r1, #236
	str	r7, [sp, #12]
	bl 0x0200c7cc
.L_02002d4e:
	movs	r2, #8
	adds	r5, #1
	add	sl, r2
	cmp	r5, #3
	ble.n	.L_02002cde
	b.n	.L_02002e8e
.L_02002d5a:
	movs	r5, #0
.L_02002d5c:
	ldr	r4, [sp, #0]
	lsls	r3, r5, #5
	subs	r3, #16
	muls	r3, r4
	movs	r1, #48
	ldr	r0, [pc, #420]
	cmp	r3, #0
	bge.n	.L_02002d6e
	adds	r3, #255
.L_02002d6e:
	asrs	r3, r3, #8
	adds	r3, r6, r3
	movs	r7, #48
	adds	r2, r3, #0
	adds	r7, #255
	adds	r3, #152
	adds	r2, #88
	cmp	r3, r7
	bhi.n	.L_02002dce
	movs	r3, #128
	ldr	r7, [sp, #12]
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r2, r3
	movs	r3, #0
	stmia	r7!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r1
	orrs	r3, r0
	mov	r0, r8
	lsls	r2, r0, #25
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r4, r7, #0
	orrs	r3, r2
	adds	r2, r7, #0
	str	r4, [sp, #12]
	stmia	r2!, {r3}
	ldr	r3, [pc, #356]
	adds	r1, r2, #0
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	movs	r2, #244
	add	r3, sl
	lsls	r2, r2, #8
	adds	r0, r1, #0
	orrs	r3, r2
	stmia	r0!, {r3}
	adds	r7, r0, #0
	ldr	r0, [sp, #8]
	str	r7, [sp, #12]
	adds	r1, r0, #0
	adds	r1, #12
	str	r1, [sp, #8]
	movs	r1, #236
	bl 0x0200c7cc
.L_02002dce:
	movs	r2, #8
	adds	r5, #1
	add	sl, r2
	cmp	r5, #1
	ble.n	.L_02002d5c
	b.n	.L_02002e8e
.L_02002dda:
	adds	r3, r6, #0
	movs	r4, #152
	adds	r2, r6, #0
	adds	r3, #120
	lsls	r4, r4, #1
	movs	r1, #48
	ldr	r0, [pc, #300]
	adds	r2, #56
	cmp	r3, r4
	bcs.n	.L_02002e8e
	ldr	r5, [sp, #8]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r4, r5, #0
	ands	r2, r3
	movs	r3, #0
	stmia	r4!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r1
	mov	r5, r8
	orrs	r3, r0
	lsls	r2, r5, #25
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r7, r4, #0
	orrs	r3, r2
	str	r7, [sp, #12]
	stmia	r4!, {r3}
	ldr	r3, [pc, #248]
	adds	r7, r4, #0
	str	r7, [sp, #12]
	movs	r2, #244
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	lsls	r2, r2, #8
	add	r3, sl
	orrs	r3, r2
	str	r3, [r4, #0]
	ldr	r0, [sp, #8]
	movs	r1, #236
	bl 0x0200c7cc
	b.n	.L_02002e8e
.L_02002e34:
	adds	r3, r6, #0
	movs	r4, #152
	movs	r0, #128
	adds	r2, r6, #0
	adds	r3, #152
	lsls	r4, r4, #1
	movs	r1, #48
	lsls	r0, r0, #24
	adds	r2, #88
	cmp	r3, r4
	bcs.n	.L_02002e8e
	ldr	r5, [sp, #8]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r4, r5, #0
	ands	r2, r3
	movs	r3, #0
	stmia	r4!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r1
	mov	r5, r8
	orrs	r3, r0
	lsls	r2, r5, #25
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r7, r4, #0
	orrs	r3, r2
	str	r7, [sp, #12]
	stmia	r4!, {r3}
	ldr	r3, [pc, #156]
	adds	r7, r4, #0
	str	r7, [sp, #12]
	movs	r2, #244
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	lsls	r2, r2, #8
	add	r3, sl
	orrs	r3, r2
	str	r3, [r4, #0]
	ldr	r0, [sp, #8]
	movs	r1, #236
	bl 0x0200c7cc
.L_02002e8e:
	ldr	r0, [pc, #136]
	ldr	r1, [pc, #136]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02002ec0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	strh	r2, [r0, #0]
	movs	r2, #252
	adds	r3, #4
	lsls	r2, r2, #6
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02002ec0:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r3, [r0, #0]
	cmp	r3, #31
	bgt.n	.L_02002ef6
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r0, #0]
	ldr	r5, [sp, #4]
	movs	r3, #16
	lsls	r2, r2, #2
	subs	r3, r3, r5
	adds	r2, r2, r0
	lsls	r3, r3, #8
	adds	r2, #4
	orrs	r3, r5
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_02002ef6:
	strh	r4, [r1, #0]
.L_02002ef8:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200e4b0
	.4byte 0x80004000
	.4byte 0x0200e494
	.4byte 0xc0004000
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r3, [pc, #52]
	ldr	r2, [pc, #52]
	adds	r5, r0, #0
	adds	r6, r1, #0
	strh	r5, [r3, #0]
	movs	r1, #144
	lsls	r3, r6, #4
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #40]
	bl 0x0200c74c
	ldr	r1, [pc, #40]
	cmp	r5, #2
	bne.n	.L_02002f44
	ldr	r1, [pc, #36]
	b.n	.L_02002f76
.L_02002f44:
	cmp	r5, #4
	bne.n	.L_02002f4c
	ldr	r1, [pc, #32]
	b.n	.L_02002f76
.L_02002f4c:
	cmp	r5, #3
	bne.n	.L_02002f76
	cmp	r6, #0
	beq.n	.L_02002f74
	ldr	r1, [pc, #24]
	b.n	.L_02002f76
	.4byte 0x0200e4b0
	.4byte 0x0200e494
	.4byte 0x0200aa21
	.4byte 0x0200d15a
	.4byte 0x0200cb58
	.4byte 0x0200d186
	.2byte 0xcb80
	.2byte 0x0200
.L_02002f74:
	ldr	r1, [pc, #28]
.L_02002f76:
	ldr	r2, [pc, #24]
	ldr	r3, [pc, #28]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #28]
	str	r1, [r3, #0]
	ldr	r3, [pc, #28]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #28]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #28]
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_02002fac
	.4byte 0x00000000
	.4byte 0x0200d204
	.4byte 0x0200e4bc
	.4byte 0x0200e4c0
	.4byte 0x0200e504
	.4byte 0x0200e49c
	.2byte 0xe4a0
	.2byte 0x0200
.L_02002fac:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #229
	lsls	r0, r0, #5
	bl 0x0200c78c
	ldr	r7, [pc, #108]
	adds	r6, r0, #0
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02002fd6
	bl 0x0200c7b4
	strh	r0, [r7, #0]
.L_02002fd6:
	ldr	r3, [pc, #92]
	ldrb	r3, [r3, r5]
	mov	r8, r3
	cmp	r5, #8
	bne.n	.L_02002fe2
	movs	r5, #4
.L_02002fe2:
	ldr	r0, [pc, #84]
	bl 0x0200c7dc
	adds	r1, r6, #0
	bl 0x0200c79c
	mov	r2, r8
	adds	r0, r6, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r1, [pc, #60]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	lsls	r2, r5, #10
	adds	r2, r2, r6
	movs	r1, #128
	adds	r2, #160
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	lsls	r1, r1, #3
	bl 0x0200c7ac
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #212
.L_0200301c:
	ldr	r3, [r2, #8]
	cmp	r3, #0
	blt.n	.L_0200301c
	adds	r0, r6, #0
	bl 0x0200c794
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200d158
	.4byte 0x0200cbfe
	.4byte 0x000001cc
	.2byte 0x03e0
	.2byte 0x0500
	push	{lr}
	ldr	r2, [pc, #116]
	movs	r3, #180
	strh	r3, [r2, #30]
	movs	r0, #30
	bl 0x0200c8bc
	movs	r0, #78
	bl 0x0200ca9c
	movs	r0, #5
	bl 0x0200afb0
	movs	r1, #0
	movs	r0, #2
	bl 0x0200af20
	movs	r0, #236
	bl 0x0200ca9c
	movs	r0, #60
	bl 0x0200c8bc
	movs	r1, #1
	movs	r0, #2
	bl 0x0200af20
	movs	r0, #236
	bl 0x0200ca9c
	movs	r0, #60
	bl 0x0200c8bc
	movs	r0, #6
	bl 0x0200afb0
	movs	r1, #0
	movs	r0, #2
	bl 0x0200af20
	movs	r0, #236
	bl 0x0200ca9c
	movs	r0, #60
	bl 0x0200c8bc
	movs	r0, #7
	bl 0x0200afb0
	movs	r1, #0
	movs	r0, #4
	bl 0x0200af20
	movs	r0, #237
	bl 0x0200ca9c
	movs	r0, #76
	bl 0x0200ca9c
	pop	{pc}
	.2byte 0xd15a
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r7, #192
	lsls	r7, r7, #18
	adds	r3, r7, #0
	adds	r3, #224
	movs	r0, #16
	ldr	r5, [r3, #0]
	bl 0x0200c8dc
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #34
	adds	r5, #35
	strb	r3, [r2, #0]
	strb	r3, [r5, #0]
	adds	r6, r0, #0
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r0, #0
	bl 0x0200ca9c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	bl 0x0200c9ec
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	adds	r1, #204
	bl 0x0200c9e4
	movs	r0, #168
	movs	r1, #1
	movs	r2, #132
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #18
	bl 0x0200c9ec
	ldr	r5, [pc, #1016]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c8e4
	movs	r2, #134
	lsls	r2, r2, #2
	ldr	r0, [r5, #0]
	movs	r1, #168
	bl 0x0200c91c
	ldr	r1, [r5, #0]
	movs	r0, #18
	bl 0x0200c944
	ldr	r1, [r5, #0]
	movs	r0, #7
	bl 0x0200c944
	ldr	r1, [r5, #0]
	movs	r0, #5
	bl 0x0200c944
	ldr	r1, [r5, #0]
	movs	r0, #7
	bl 0x0200c944
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x0200c944
	movs	r0, #1
	bl 0x0200c744
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #18
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c8e4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #7
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c8e4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #5
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c8e4
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	lsls	r1, r1, #9
	bl 0x0200c8e4
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #868]
	bl 0x0200c8ec
	ldr	r1, [pc, #864]
	movs	r0, #5
	bl 0x0200c8ec
	ldr	r1, [pc, #860]
	movs	r0, #18
	bl 0x0200c8ec
	ldr	r1, [pc, #856]
	movs	r0, #7
	bl 0x0200c8fc
	movs	r1, #0
	movs	r0, #5
	bl 0x0200c9bc
	ldr	r0, [pc, #844]
	bl 0x0200c98c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c94c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #6
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c9bc
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c954
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #6
	adds	r1, #102
	adds	r2, #51
	bl 0x0200c8e4
	movs	r2, #131
	movs	r0, #6
	movs	r1, #168
	lsls	r2, r2, #2
	bl 0x0200c91c
	movs	r1, #192
	movs	r0, #15
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #192
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #8
	bl 0x0200c9b4
	movs	r1, #204
	lsls	r1, r1, #6
	adds	r1, #51
	ldr	r0, [pc, #708]
	bl 0x0200c9e4
	bl 0x0200bb24
	movs	r0, #29
	bl 0x0200ca9c
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #692]
	adds	r1, #153
	bl 0x0200c9e4
	movs	r0, #168
	movs	r1, #1
	movs	r2, #135
	movs	r3, #1
	lsls	r2, r2, #18
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200c9ec
	bl 0x0200c9f4
	movs	r0, #20
	bl 0x0200c8bc
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200c9d4
	movs	r0, #20
	bl 0x0200c8bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #129
	movs	r0, #17
	lsls	r1, r1, #1
	bl 0x0200c9d4
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #129
	movs	r0, #16
	lsls	r1, r1, #1
	bl 0x0200c9d4
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r2, #20
	adds	r0, #16
	movs	r1, #0
	bl 0x0200c99c
	movs	r0, #15
	movs	r1, #1
	bl 0x0200c974
	movs	r1, #160
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200c9cc
	movs	r1, #192
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r2, #0
	movs	r0, #6
	movs	r1, #4
	bl 0x0200c964
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c94c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #7
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #224
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #15
	movs	r1, #4
	bl 0x0200c94c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c9b4
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200c9cc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	bl 0x0200c9bc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #160
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #2
	movs	r2, #40
	adds	r1, #255
	movs	r0, #18
	bl 0x0200c9cc
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #18
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c954
	movs	r1, #192
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #17
	bl 0x0200c9cc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #16
	bl 0x0200c9cc
	movs	r1, #128
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #6
	bl 0x0200c9b4
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r0, #17
	movs	r1, #1
	bl 0x0200c96c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #16
	movs	r1, #1
	bl 0x0200c96c
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200c9cc
	movs	r1, #192
	movs	r2, #20
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #129
	movs	r0, #15
	lsls	r1, r1, #1
	bl 0x0200c9d4
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200c9cc
	movs	r0, #6
	movs	r1, #2
	movs	r2, #10
	bl 0x0200c964
	movs	r2, #10
	movs	r0, #6
	movs	r1, #4
	bl 0x0200c964
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #160
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #17
	movs	r1, #4
	bl 0x0200c94c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #224
	movs	r0, #16
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #16
	movs	r1, #4
	bl 0x0200c94c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #16
	movs	r1, #0
	bl 0x0200c9a4
	movs	r2, #0
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c9b4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #17
	bl 0x0200c9bc
	movs	r0, #10
	bl 0x0200c8bc
	movs	r0, #16
	movs	r1, #3
	bl 0x0200c94c
	movs	r0, #17
	movs	r1, #3
	bl 0x0200c954
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	b.n	.L_0200353c
	.4byte 0x02000240
	.4byte 0x0200cf80
	.4byte 0x0200cfc4
	.4byte 0x0200cf3c
	.4byte 0x0200d008
	.4byte 0x0000296e
	.4byte 0x00019999
	.2byte 0xcccc
	.2byte 0x0004
.L_0200353c:
	adds	r1, #204
	bl 0x0200c9e4
	movs	r0, #168
	movs	r1, #1
	movs	r2, #140
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #18
	bl 0x0200c9ec
	movs	r1, #128
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #7
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r1, #128
	movs	r2, #128
	movs	r0, #17
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c8e4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #16
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200c8e4
	movs	r2, #140
	movs	r0, #16
	movs	r1, #120
	lsls	r2, r2, #2
	bl 0x0200c914
	movs	r2, #140
	lsls	r2, r2, #2
	movs	r0, #17
	movs	r1, #216
	bl 0x0200c91c
	movs	r0, #17
	movs	r1, #1
	bl 0x0200c94c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200c9b4
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c9bc
	movs	r2, #140
	movs	r0, #16
	movs	r1, #152
	lsls	r2, r2, #2
	bl 0x0200c914
	movs	r2, #140
	lsls	r2, r2, #2
	movs	r0, #17
	movs	r1, #184
	bl 0x0200c91c
	movs	r0, #16
	movs	r1, #1
	bl 0x0200c94c
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #7
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #15
	lsls	r1, r1, #9
	bl 0x0200c8e4
	movs	r0, #15
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #17
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #16
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #15
	movs	r1, #0
	movs	r2, #16
	bl 0x0200c924
	movs	r0, #17
	movs	r1, #0
	movs	r2, #16
	bl 0x0200c924
	movs	r2, #16
	movs	r1, #0
	movs	r0, #16
	bl 0x0200c924
	movs	r0, #15
	bl 0x0200c92c
	movs	r0, #15
	movs	r1, #1
	bl 0x0200c94c
	movs	r0, #17
	movs	r1, #1
	bl 0x0200c94c
	movs	r0, #16
	movs	r1, #1
	bl 0x0200c94c
	movs	r1, #192
	movs	r0, #18
	lsls	r1, r1, #6
	bl 0x0200c9bc
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #160
	movs	r0, #15
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #160
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #160
	movs	r0, #16
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #18
	bl 0x0200c9cc
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #17
	bl 0x0200c9cc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #20
	movs	r0, #16
	bl 0x0200c9cc
	movs	r1, #132
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200c9cc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c9a4
	movs	r2, #0
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r0, #7
	movs	r1, #3
	bl 0x0200c954
	movs	r0, #7
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #224
	movs	r0, #15
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #4
	adds	r1, #255
	movs	r2, #20
	movs	r0, #15
	bl 0x0200c9cc
	movs	r1, #4
	movs	r2, #20
	adds	r1, #255
	movs	r0, #6
	bl 0x0200c9cc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #192
	movs	r2, #20
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9b4
	movs	r1, #4
	movs	r0, #15
	bl 0x0200c954
	movs	r0, #20
	bl 0x0200c8bc
	movs	r1, #131
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200c9cc
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #224
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #6
	bl 0x0200c9bc
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #129
	movs	r0, #15
	lsls	r1, r1, #1
	bl 0x0200c9d4
	movs	r1, #160
	movs	r2, #20
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r2, #10
	movs	r0, #17
	movs	r1, #4
	bl 0x0200c964
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c9bc
	movs	r1, #3
	movs	r0, #16
	bl 0x0200c954
	movs	r0, #10
	bl 0x0200c8bc
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #6
	bl 0x0200c9bc
	movs	r0, #16
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200c9b4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200c9bc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c94c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c9a4
	movs	r1, #129
	movs	r0, #15
	lsls	r1, r1, #1
	bl 0x0200c9d4
	movs	r1, #192
	movs	r2, #40
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9b4
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #15
	bl 0x0200c9bc
	movs	r0, #20
	bl 0x0200c8bc
	movs	r1, #3
	movs	r0, #17
	bl 0x0200c954
	movs	r0, #20
	bl 0x0200c8bc
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #15
	bl 0x0200c9bc
	movs	r0, #20
	bl 0x0200c8bc
	movs	r1, #3
	movs	r0, #16
	bl 0x0200c954
	movs	r0, #20
	bl 0x0200c8bc
	movs	r1, #192
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #15
	bl 0x0200c9cc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #15
	movs	r1, #4
	bl 0x0200c94c
	movs	r1, #0
	movs	r0, #15
	bl 0x0200c994
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c9b4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c9b4
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c8d4
	adds	r6, #99
	ldr	r5, [r5, #0]
	cmp	r0, #0
	bne.n	.L_020038ec
	adds	r0, r5, #0
	bl 0x0200c95c
	movs	r0, #15
	movs	r1, #4
	bl 0x0200c954
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	b.n	.L_02003920
.L_020038ec:
	adds	r0, r5, #0
	bl 0x0200c95c
	movs	r0, #15
	movs	r1, #4
	bl 0x0200c954
	ldr	r2, [r7, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #15
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c94c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
.L_02003920:
	movs	r2, #128
	lsls	r2, r2, #2
	movs	r0, #15
	movs	r1, #212
	adds	r2, #70
	bl 0x0200c91c
	movs	r0, #168
	movs	r1, #1
	movs	r2, #250
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x0200c9ec
	ldr	r3, [pc, #464]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #15
	bl 0x0200ca3c
	movs	r0, #18
	movs	r1, #15
	bl 0x0200ca3c
	movs	r0, #5
	movs	r1, #15
	bl 0x0200ca3c
	movs	r0, #7
	movs	r1, #15
	bl 0x0200ca3c
	movs	r0, #6
	movs	r1, #15
	bl 0x0200ca3c
	ldr	r5, [pc, #420]
	movs	r3, #0
	strb	r3, [r6, #0]
	adds	r1, r5, #0
	movs	r0, #15
	bl 0x0200c8ec
	movs	r0, #20
	bl 0x0200c8bc
	movs	r0, #17
	adds	r1, r5, #0
	bl 0x0200c8ec
	movs	r0, #16
	adds	r1, r5, #0
	bl 0x0200c8ec
	movs	r0, #78
	bl 0x0200ca9c
.L_0200399a:
	movs	r0, #1
	bl 0x0200c744
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_0200399a
	ldr	r6, [pc, #364]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	ldr	r0, [r6, #0]
	bl 0x0200c8f4
	movs	r0, #18
	bl 0x0200c8f4
	movs	r0, #5
	bl 0x0200c8f4
	movs	r0, #7
	bl 0x0200c8f4
	movs	r0, #6
	bl 0x0200c8f4
	bl 0x0200ca2c
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #324]
	adds	r1, #153
	bl 0x0200c9e4
	movs	r0, #168
	movs	r1, #1
	movs	r2, #135
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200c9ec
	movs	r5, #128
	movs	r0, #20
	bl 0x0200c8bc
	lsls	r5, r5, #7
	movs	r1, #224
	movs	r2, #140
	adds	r3, r5, #0
	movs	r0, #15
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200c93c
	movs	r1, #208
	movs	r2, #132
	adds	r3, r5, #0
	movs	r0, #16
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200c93c
	movs	r1, #240
	movs	r2, #132
	adds	r3, r5, #0
	movs	r0, #17
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200c93c
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c9b4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c9b4
	movs	r1, #160
	movs	r2, #0
.L_02003a4c:
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200c9b4
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #6
	bl 0x0200c9bc
	movs	r0, #10
	bl 0x0200c8bc
	movs	r0, #18
	movs	r1, #3
	bl 0x0200c94c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c94c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200c94c
	movs	r1, #3
	movs	r0, #6
	bl 0x0200c954
	ldr	r0, [r6, #0]
	bl 0x0200c8f4
	movs	r0, #18
	bl 0x0200c8f4
	movs	r0, #5
	bl 0x0200c8f4
	movs	r0, #7
	bl 0x0200c8f4
	movs	r0, #6
	bl 0x0200c8f4
	ldr	r0, [r6, #0]
	bl 0x0200c8dc
	movs	r5, #0
	str	r5, [r0, #108]
	movs	r0, #18
	bl 0x0200c8dc
	str	r5, [r0, #108]
	movs	r0, #5
	bl 0x0200c8dc
	str	r5, [r0, #108]
	movs	r0, #7
	bl 0x0200c8dc
	str	r5, [r0, #108]
	movs	r0, #6
	bl 0x0200c8dc
	str	r5, [r0, #108]
	movs	r0, #1
	bl 0x0200c744
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #6
	lsls	r1, r1, #9
	bl 0x0200c8e4
	ldr	r5, [pc, #60]
	movs	r0, #7
	adds	r1, r5, #0
	bl 0x0200c8ec
	adds	r1, r5, #0
	movs	r0, #6
	bl 0x0200c8ec
	adds	r1, r5, #0
	movs	r0, #5
	bl 0x0200c8ec
	adds	r1, r5, #0
	movs	r0, #18
	bl 0x0200c8fc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #76
	bl 0x0200c7f4
	bl 0x0200c8cc
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200d04c
	.4byte 0x0004cccc
	.2byte 0xd0bc
	.2byte 0x0200
	push	{lr}
	movs	r1, #1
	movs	r0, #6
	bl 0x0200c9c4
	movs	r0, #131
	bl 0x0200ca9c
	bl 0x0200ca54
	movs	r0, #6
	bl 0x0200c8dc
	movs	r1, #2
	bl 0x0200ca6c
	movs	r0, #40
	bl 0x0200c8bc
	movs	r0, #6
	bl 0x0200c8dc
	movs	r1, #0
	bl 0x0200ca6c
	bl 0x0200ca64
	bl 0x0200ca5c
	bl 0x0200857c
	movs	r0, #6
	movs	r1, #2
	bl 0x0200c9c4
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r3
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #8
	ldr	r3, [r3, #108]
	ldr	r6, [sp, #36]
	adds	r5, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	mov	r9, r3
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	cmp	r6, #2
	bne.n	.L_02003ba0
	movs	r0, #188
	bl 0x0200ca9c
	b.n	.L_02003ba6
.L_02003ba0:
	movs	r0, #158
	bl 0x0200ca9c
.L_02003ba6:
	ldr	r3, [sp, #40]
	adds	r0, r5, #0
	str	r3, [sp, #4]
	adds	r1, r7, #0
	mov	r2, r8
	mov	r3, sl
	str	r6, [sp, #0]
	bl 0x0200c844
	movs	r0, #20
	bl 0x0200c8bc
	ldr	r3, [pc, #144]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200c8e4
	cmp	r6, #1
	bne.n	.L_02003c10
	ldr	r0, [r5, #0]
	bl 0x0200c8dc
	adds	r5, r0, #0
	ldr	r0, [r5, #8]
	asrs	r2, r0, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_02003bec
	adds	r3, #15
.L_02003bec:
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
	bl 0x0200c834
	adds	r0, r5, #0
	bl 0x0200c83c
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r5, #6]
.L_02003c10:
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #56]
	bl 0x0200c8ec
	movs	r0, #12
	bl 0x0200c744
	movs	r0, #123
	bl 0x0200ca9c
	bl 0x0200ca14
	bl 0x0200ca1c
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200ca04
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xcc08
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #43
	movs	r1, #40
	movs	r2, #8
	movs	r3, #22
	bl 0x0200bb6c
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #43
	movs	r1, #42
	movs	r2, #4
	movs	r3, #26
	bl 0x0200bb6c
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #43
	movs	r1, #42
	movs	r2, #27
	movs	r3, #21
	bl 0x0200bb6c
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #43
	movs	r1, #40
	movs	r2, #28
	movs	r3, #17
	bl 0x0200bb6c
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #43
	movs	r1, #42
	movs	r2, #10
	movs	r3, #8
	bl 0x0200bb6c
	add	sp, #8
	pop	{pc}
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	sub	sp, #8
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02003d16
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r0, #123
	bl 0x0200ca9c
	bl 0x0200ca14
	bl 0x0200ca1c
	movs	r0, #13
	bl 0x0200ca04
	b.n	.L_02003d2a
.L_02003d16:
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #43
	movs	r1, #40
	movs	r2, #7
	movs	r3, #6
	bl 0x0200bb6c
.L_02003d2a:
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #28
	adds	r0, #255
	bl 0x0200c7fc
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	movs	r1, #0
	movs	r2, #0
	movs	r0, #17
	bl 0x0200c934
	movs	r0, #15
	bl 0x0200c8dc
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r0, #6]
	movs	r1, #1
	movs	r0, #224
	movs	r2, #182
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #0
	lsls	r0, r0, #16
	bl 0x0200c9ec
	movs	r0, #1
	bl 0x0200c744
	movs	r5, #128
	bl 0x0200c82c
	movs	r0, #1
	bl 0x0200c744
	lsls	r5, r5, #7
	bl 0x0200ca0c
	bl 0x0200ca1c
	movs	r1, #184
	movs	r2, #128
	movs	r0, #16
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	adds	r3, r5, #0
	bl 0x0200c93c
	movs	r1, #132
	movs	r2, #128
	adds	r3, r5, #0
	movs	r0, #17
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200c93c
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #16
	ldr	r1, [pc, #432]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #17
	ldr	r1, [pc, #416]
	bl 0x0200c8e4
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200c9e4
	movs	r0, #224
	movs	r1, #1
	movs	r2, #140
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c9ec
	movs	r2, #128
	movs	r0, #16
	movs	r1, #184
	lsls	r2, r2, #1
	bl 0x0200c914
	movs	r1, #132
	movs	r2, #128
	movs	r0, #17
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200c91c
	ldr	r3, [pc, #356]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	movs	r1, #224
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c9b4
	movs	r2, #132
	movs	r0, #16
	movs	r1, #208
	lsls	r2, r2, #1
	bl 0x0200c914
	movs	r2, #132
	lsls	r2, r2, #1
	movs	r0, #17
	movs	r1, #240
	bl 0x0200c91c
	movs	r1, #1
	movs	r0, #16
	bl 0x0200c94c
	movs	r0, #10
	bl 0x0200c8bc
	movs	r0, #16
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200c9b4
	movs	r2, #0
	movs	r0, #17
	adds	r1, r5, #0
	bl 0x0200c9b4
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c9bc
	movs	r2, #0
	movs	r1, #0
	ldr	r0, [r6, #0]
	bl 0x0200c9b4
	bl 0x0200c9f4
	ldr	r0, [pc, #256]
	bl 0x0200c98c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c94c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #15
	bl 0x0200c994
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200c8d4
	cmp	r0, #1
	bne.n	.L_02003ef4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #15
	bl 0x0200c994
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200c8d4
	cmp	r0, #0
	bne.n	.L_02003ee2
	movs	r1, #3
	movs	r0, #15
	bl 0x0200c954
	movs	r0, #76
	bl 0x0200ca9c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200c7f4
	bl 0x02009380
	b.n	.L_02003f6a
.L_02003ee2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	b.n	.L_02003f04
.L_02003ef4:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #2
.L_02003f04:
	strh	r3, [r2, #0]
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #15
	bl 0x0200c994
	ldr	r3, [pc, #92]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200c8d4
	cmp	r0, #1
	bne.n	.L_02003f38
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	bl 0x0200a89c
	b.n	.L_02003f5a
.L_02003f38:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #6
	strh	r3, [r2, #0]
	adds	r0, #15
	movs	r1, #0
	bl 0x0200c9a4
	bl 0x0200a90c
.L_02003f5a:
	bl 0x0200ca2c
	movs	r0, #48
	adds	r0, #255
	bl 0x0200c7fc
	bl 0x0200c8cc
.L_02003f6a:
	pop	{r5, r6, pc}
	.4byte 0x00019999
	.4byte 0x02000240
	.2byte 0x29a5
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	ldr	r0, [pc, #108]
	bl 0x0200c98c
	movs	r0, #15
	bl 0x0200c8dc
	movs	r5, #128
	lsls	r5, r5, #8
	strh	r5, [r0, #6]
	movs	r0, #16
	bl 0x0200c8dc
	strh	r5, [r0, #6]
	movs	r0, #17
	bl 0x0200c8dc
	movs	r1, #1
	strh	r5, [r0, #6]
	movs	r2, #140
	movs	r0, #224
	lsls	r2, r2, #17
	movs	r3, #0
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200c9ec
	movs	r0, #1
	bl 0x0200c744
	bl 0x0200c82c
	movs	r0, #1
	bl 0x0200c744
	bl 0x0200ca0c
	bl 0x0200ca1c
	movs	r0, #20
	bl 0x0200c8bc
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #15
	bl 0x0200c9a4
	bl 0x0200a89c
	movs	r0, #48
	adds	r0, #255
	bl 0x0200c7fc
	bl 0x0200c8cc
	pop	{r5, pc}
	.2byte 0x29c9
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #72]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02004012
	bl 0x0200c060
	b.n	.L_02004040
.L_02004012:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_0200401e
	bl 0x0200c1b8
	b.n	.L_02004040
.L_0200401e:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_0200402a
	bl 0x0200c3f8
	b.n	.L_02004040
.L_0200402a:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02004036
	bl 0x0200c534
	b.n	.L_02004040
.L_02004036:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_02004040
	bl 0x0200c6ec
.L_02004040:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000f1
	.4byte 0x000000f3
	.4byte 0x000000f2
	.4byte 0x000000f4
	.2byte 0x00f6
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #76
	sub	sp, #8
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_020040a8
	movs	r3, #9
	movs	r2, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #47
	movs	r1, #19
	movs	r2, #38
	movs	r3, #19
	bl 0x0200c844
	movs	r3, #6
	movs	r2, #29
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #5
	movs	r0, #1
	movs	r2, #9
	movs	r3, #1
	bl 0x0200c84c
	movs	r0, #14
	bl 0x0200c8dc
	movs	r1, #6
	bl 0x0200c984
	b.n	.L_020040ee
.L_020040a8:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200c7ec
	cmp	r0, #0
	bne.n	.L_020040ee
	movs	r5, #128
	lsls	r5, r5, #7
	movs	r1, #168
	movs	r2, #142
	movs	r0, #15
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	adds	r3, r5, #0
	bl 0x0200c93c
	movs	r1, #216
	movs	r2, #134
	movs	r0, #17
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	adds	r3, r5, #0
	bl 0x0200c93c
	movs	r1, #240
	movs	r2, #134
.L_020040dc:
	movs	r0, #16
	lsls	r1, r1, #15
	lsls	r2, r2, #18
.L_020040e2:
	adds	r3, r5, #0
	bl 0x0200c93c
	movs	r0, #1
	bl 0x0200c744
.L_020040ee:
	movs	r0, #8
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #9
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #10
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #11
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #12
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #13
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #10
	adds	r0, #255
	bl 0x0200c7ec
	cmp	r0, #0
	bne.n	.L_02004148
	ldr	r3, [pc, #136]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #11
	bne.n	.L_02004140
	bl 0x0200bd30
	b.n	.L_02004148
.L_02004140:
	cmp	r3, #9
	bne.n	.L_02004148
	bl 0x0200bf78
.L_02004148:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #254
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_020041ae
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	movs	r3, #10
.L_02004176:
	str	r3, [sp, #0]
	movs	r5, #13
	movs	r0, #10
	movs	r1, #12
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200c84c
	movs	r3, #15
	str	r3, [sp, #0]
	movs	r0, #15
	movs	r1, #12
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200c84c
	movs	r3, #9
	movs	r2, #21
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #9
	movs	r1, #20
	movs	r2, #3
	movs	r3, #1
	bl 0x0200c84c
.L_020041ae:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #8
	movs	r1, #2
	sub	sp, #8
	bl 0x0200c94c
	movs	r0, #9
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #10
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #11
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #12
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #13
	movs	r1, #2
	bl 0x0200c94c
	movs	r0, #14
	bl 0x0200c8dc
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #78
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_0200423c
	movs	r0, #14
	bl 0x0200c8dc
	movs	r1, #130
	movs	r2, #128
	movs	r3, #184
	lsls	r1, r1, #18
	lsls	r2, r2, #14
	lsls	r3, r3, #16
	bl 0x0200c824
	movs	r0, #1
	bl 0x0200c744
	movs	r3, #32
	movs	r2, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #30
	movs	r1, #11
	movs	r2, #1
	movs	r3, #1
	bl 0x0200c84c
	movs	r0, #10
	bl 0x0200c744
.L_0200423c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #225
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02004270
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #76
	movs	r3, #2
	bl 0x0200c844
	movs	r3, #12
	movs	r2, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #12
	movs	r1, #5
	movs	r2, #3
	movs	r3, #1
	bl 0x0200c84c
.L_02004270:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #77
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02004294
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #48
	movs	r1, #40
	movs	r2, #101
	movs	r3, #11
	bl 0x0200c844
	b.n	.L_0200429e
.L_02004294:
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
.L_0200429e:
	movs	r0, #16
	bl 0x0200c8dc
	adds	r2, r0, #0
	adds	r2, #89
	movs	r3, #8
	strb	r3, [r2, #0]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	movs	r0, #16
	bl 0x0200c8dc
	movs	r1, #0
	bl 0x0200c854
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
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
	bne.n	.L_0200430a
	bl 0x0200c81c
.L_0200430a:
	pop	{pc}
	push	{r5, r6, r7, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	movs	r0, #30
	adds	r3, r2, #0
	adds	r0, #255
	adds	r1, r4, #0
	adds	r2, r5, #0
	bl 0x0200c814
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0200438c
	adds	r2, r6, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r3, r6, #0
	movs	r1, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	adds	r7, r6, #0
	adds	r3, #15
	strh	r1, [r3, #0]
	adds	r7, #35
	ldrb	r2, [r7, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r7, #0]
	movs	r1, #7
	bl 0x0200c984
	ldr	r1, [r6, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldrb	r3, [r7, #0]
	movs	r5, #2
	orrs	r5, r3
	adds	r0, r6, #0
	movs	r1, #0
	strb	r5, [r7, #0]
	bl 0x0200c854
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	movs	r3, #60
	str	r3, [r6, #104]
	ldr	r3, [pc, #20]
	adds	r0, r6, #0
	movs	r1, #5
	str	r3, [r6, #108]
	bl 0x0200c804
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200c85c
.L_0200438c:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xc2c9
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #44]
	movs	r2, #63
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020043c2
	movs	r0, #144
	movs	r1, #132
	movs	r2, #188
	lsls	r0, r0, #15
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x0200c30c
	movs	r0, #208
	movs	r1, #196
	movs	r2, #152
	lsls	r0, r0, #15
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x0200c30c
.L_020043c2:
	pop	{pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, lr}
	ldr	r3, [pc, #40]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c8dc
	ldr	r3, [r0, #80]
	movs	r0, #21
	ldrb	r5, [r3, #9]
	lsls	r5, r5, #28
	lsrs	r5, r5, #30
	adds	r1, r5, #0
	bl 0x0200c9c4
	movs	r0, #14
	adds	r1, r5, #0
	bl 0x0200c9c4
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #10
	adds	r0, #255
	sub	sp, #8
	bl 0x0200c7ec
	cmp	r0, #0
	bne.n	.L_02004480
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #254
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02004434
	movs	r1, #208
	movs	r2, #200
	movs	r0, #16
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x0200c934
	movs	r1, #136
	movs	r2, #200
	movs	r0, #17
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200c934
	b.n	.L_02004480
.L_02004434:
	movs	r0, #133
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02004480
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #225
	bl 0x0200c7ec
	cmp	r0, #0
	bne.n	.L_02004480
	movs	r1, #240
	movs	r2, #208
	movs	r0, #15
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x0200c934
	movs	r1, #208
	movs	r2, #212
.L_02004462:
	movs	r0, #16
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x0200c934
	movs	r1, #136
	movs	r2, #212
	movs	r0, #17
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200c934
	movs	r0, #1
	bl 0x0200c744
.L_02004480:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_020044a2
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #43
	movs	r1, #40
.L_0200449a:
	movs	r2, #7
	movs	r3, #6
	bl 0x0200c844
.L_020044a2:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #77
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_020044d4
	movs	r5, #1
	movs	r6, #2
	movs	r0, #48
	movs	r1, #40
	movs	r2, #77
	movs	r3, #21
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c844
	movs	r0, #48
	movs	r1, #40
	movs	r2, #81
	movs	r3, #25
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c844
.L_020044d4:
	bl 0x0200ca44
	movs	r1, #129
	movs	r0, #0
	lsls	r1, r1, #2
	movs	r2, #19
	movs	r3, #20
	bl 0x0200ca4c
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_020044f8
	bl 0x0200a878
.L_020044f8:
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	subs	r2, #2
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #24]
	bl 0x0200c74c
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl 0x0200c74c
	add	sp, #8
	b.n	.L_02004530
	.4byte 0x00000c08
	.4byte 0x00003f10
	.4byte 0x0200c3c9
	.2byte 0xc395
	.2byte 0x0200
.L_02004530:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	movs	r0, #12
	bl 0x0200c8dc
	movs	r3, #8
	adds	r0, #89
	strb	r3, [r0, #0]
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #254
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02004582
	movs	r3, #192
	movs	r1, #176
	movs	r2, #184
	movs	r0, #8
	lsls	r1, r1, #15
	lsls	r2, r2, #15
	lsls	r3, r3, #6
	bl 0x0200c93c
	movs	r1, #156
	movs	r2, #144
	movs	r0, #11
	lsls	r1, r1, #16
	lsls	r2, r2, #15
	bl 0x0200c934
	b.n	.L_020045a0
.L_02004582:
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
.L_020045a0:
	ldr	r5, [pc, #268]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #30
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_020046ae
	movs	r0, #10
	adds	r0, #255
	bl 0x0200c7ec
	cmp	r0, #0
	bne.n	.L_020046ae
	movs	r0, #1
	bl 0x0200c8ac
	bl 0x0200c8c4
	movs	r0, #0
	bl 0x0200ca24
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #41
	bl 0x0200c94c
	ldr	r0, [r5, #0]
	bl 0x0200c8dc
	movs	r1, #0
	bl 0x0200c854
	movs	r0, #1
	bl 0x0200c744
	bl 0x0200ca0c
	bl 0x0200ca1c
	movs	r0, #20
	bl 0x0200c8bc
	movs	r5, #0
	b.n	.L_02004606
.L_02004604:
	adds	r5, #1
.L_02004606:
	cmp	r5, #59
	bhi.n	.L_02004618
	movs	r0, #1
	bl 0x0200c744
	ldr	r3, [pc, #160]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02004604
.L_02004618:
	ldr	r3, [pc, #148]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #42
	bl 0x0200c94c
	movs	r0, #10
	bl 0x0200c8bc
	movs	r5, #0
	b.n	.L_02004634
.L_02004632:
	adds	r5, #1
.L_02004634:
	cmp	r5, #29
	bhi.n	.L_02004646
	movs	r0, #1
	bl 0x0200c744
	ldr	r3, [pc, #116]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02004632
.L_02004646:
	ldr	r6, [pc, #104]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r6, r3
	movs	r2, #0
	ldr	r0, [r5, #0]
	movs	r1, #4
	bl 0x0200c964
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200c94c
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #80]
	adds	r2, #204
	bl 0x0200c8e4
	movs	r1, #192
	lsls	r1, r1, #2
	movs	r2, #120
	adds	r1, #45
	ldr	r0, [r5, #0]
	bl 0x0200c90c
	ldr	r0, [r5, #0]
	bl 0x0200c8dc
	movs	r1, #1
	bl 0x0200c854
	bl 0x0200c8cc
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #30
	bne.n	.L_020046ae
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #254
	bl 0x0200c7f4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #77
	bl 0x0200c7f4
.L_020046ae:
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x03001150
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, lr}
	ldr	r3, [pc, #40]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c8dc
	ldr	r3, [r0, #80]
	movs	r0, #9
	ldrb	r5, [r3, #9]
	lsls	r5, r5, #28
	lsrs	r5, r5, #30
	adds	r1, r5, #0
	bl 0x0200c9c4
	movs	r0, #10
	adds	r1, r5, #0
	bl 0x0200c9c4
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r0, #144
	adds	r3, r3, r2
	lsls	r0, r0, #4
	adds	r2, #93
	str	r2, [r3, #0]
	adds	r0, #254
	bl 0x0200c7ec
	cmp	r0, #0
	beq.n	.L_02004718
	movs	r1, #144
	ldr	r0, [pc, #32]
	lsls	r1, r1, #3
	bl 0x0200c74c
	b.n	.L_0200472c
.L_02004718:
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c934
.L_0200472c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200c6bd
	.irp EntryTarget, 0x03000528, 0x03000514, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000121, 0x08000141, 0x08000151, 0x08000169, 0x08000171, 0x08000179, 0x080001a9, 0x080001b9, 0x080001c9, 0x080001d1, 0x080001d9, 0x080001e1, 0x080001e9, 0x080001f1, 0x08000291, 0x080003c1, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x080200a9, 0x080200c1, 0x080200c9, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x08020179, 0x080201e9, 0x08020219, 0x08020221, 0x08020229, 0x08020231, 0x08038041, 0x08038121, 0x08038249, 0x08038349, 0x080ad011, 0x080ad039, 0x080ad101, 0x080ad209, 0x080ad2d9, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80b1, 0x080c80b9, 0x080c80c1, 0x080c80c9, 0x080c80d1, 0x080c80d9, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8101, 0x080c8111, 0x080c8119, 0x080c8129, 0x080c8131, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8159, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81a9, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8259, 0x080c8279, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c8571, 0x080c8581, 0x080c8601, 0x080c86a9, 0x080c86e9, 0x080c8809, 0x080c8811, 0x080c8819, 0x080c8821, 0x08108009, 0x08108011, 0x08108019, 0x08108029, 0x08108081, 0x081c0011
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
	.4byte 0x02001000
	.4byte 0x20000001
	.4byte 0x00010000
	.4byte 0x01001000
	.4byte 0x20000006
	.4byte 0x00060010
	.4byte 0x001e7fff
	.4byte 0x00002000
	.4byte 0x7fff001e
	.4byte 0xffff001e
	.4byte 0x00801000
	.4byte 0x20000001
	.4byte 0x00010000
	.4byte 0x01001000
	.4byte 0x20000006
	.4byte 0x00060010
	.4byte 0x003c7fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600f0
	.4byte 0x00067fff
	.4byte 0x01701000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600e0
	.4byte 0x00067fff
	.4byte 0x00002000
	.4byte 0x1000001e
	.4byte 0x00060160
	.4byte 0x00067fff
	.4byte 0x00d01000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060150
	.4byte 0x00067fff
	.4byte 0x00c01000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060140
	.4byte 0x00067fff
	.4byte 0x2000ffff
	.4byte 0x40602020
	.4byte 0x00804040
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
	.4byte 0x0200caa4
	.4byte 0x0200cae0
	.4byte 0x0200cb1c
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01080000
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
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02180000
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
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02180000
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
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02280000
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
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02340000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01d60000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01b20000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffff000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x4000ffff
	.4byte 0x0800ff44
	.4byte 0x01001000
	.4byte 0x20000001
	.4byte 0x00010010
	.4byte 0x000e7fff
	.4byte 0x00003000
	.4byte 0x7fff0014
	.4byte 0x3000003c
	.4byte 0x00140800
	.4byte 0x003c7fff
	.4byte 0x1000ffff
	.4byte 0x00010200
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0xffff0006
	.4byte 0x00801000
	.4byte 0x20000001
	.4byte 0x00010000
	.4byte 0x01001000
	.4byte 0x20000006
	.4byte 0x00060010
	.4byte 0x003c7fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060110
	.4byte 0x00067fff
	.4byte 0x01901000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060120
	.4byte 0x00067fff
	.4byte 0x00002000
	.4byte 0x1000001e
	.4byte 0x000601a0
	.4byte 0x00067fff
	.4byte 0x01301000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601b0
	.4byte 0x00067fff
	.4byte 0x01401000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601c0
	.4byte 0x00067fff
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
	.4byte 0xffff000f
	.4byte 0x00000220
	.4byte 0xc00002a8
	.4byte 0x01a40000
	.4byte 0x02940230
	.4byte 0x000002d0
	.4byte 0xffff0010
	.4byte 0x000002f0
	.4byte 0xc00002a8
	.4byte 0x02760000
	.4byte 0x03660230
	.4byte 0x000002d0
	.4byte 0xffff0011
	.4byte 0x00000220
	.4byte 0xc0000388
	.4byte 0x01a40000
	.4byte 0x0294030c
	.4byte 0x000003ac
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0006
	.4byte 0x000002d0
	.4byte 0xc0000220
	.4byte 0x02580000
	.4byte 0x0348017c
	.4byte 0x00000258
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000f1
	.4byte 0x0011f0f7
	.4byte 0x002200f7
	.4byte 0x003010f3
	.4byte 0x00a320f7
	.4byte 0x00b1f0f7
	.4byte 0x00c200f7
	.4byte 0x000000f3
	.4byte 0x001030f1
	.4byte 0x002080f2
	.4byte 0x0030b0f2
	.4byte 0x004110f5
	.4byte 0x0050e0f2
	.4byte 0x006090f2
	.4byte 0x000000f2
	.4byte 0x0013a002
	.4byte 0x0020f0f5
	.4byte 0x003100f5
	.4byte 0x004060f4
	.4byte 0x005080f4
	.4byte 0x0060b0f4
	.4byte 0x0070d0f4
	.4byte 0x008020f3
	.4byte 0x009060f3
	.4byte 0x00a060f6
	.4byte 0x00b030f3
	.4byte 0x00c030f4
	.4byte 0x00d010f4
	.4byte 0x00e050f3
	.4byte 0x000000f4
	.4byte 0x0010d0f2
	.4byte 0x002040f4
	.4byte 0x0030c0f2
	.4byte 0x004020f4
	.4byte 0x005090f4
	.4byte 0x006040f2
	.4byte 0x0070a0f4
	.4byte 0x008050f2
	.4byte 0x009050f4
	.4byte 0x00a070f4
	.4byte 0x00b060f2
	.4byte 0x00c0e0f4
	.4byte 0x00d070f2
	.4byte 0x00e0c0f4
	.4byte 0x000000f5
	.4byte 0x00f020f2
	.4byte 0x010030f2
	.4byte 0x011040f3
	.4byte 0x000000f6
	.4byte 0x0060a0f2
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff017e
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00034000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00034000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00034000
	.4byte 0xffff0038
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
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff0110
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00020000
	.4byte 0xffff0070
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00010000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00004000
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0x007900f6
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01ac0000
	.4byte 0x00033000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00010000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x016c0000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00018000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00fe0000
	.4byte 0x00000000
	.4byte 0x01b20000
	.4byte 0x00015000
	.4byte 0xffff0071
	.4byte 0x00000003
	.4byte 0x01b20000
	.4byte 0x00000000
	.4byte 0x01be0000
	.4byte 0x0001b000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00015000
	.4byte 0xffff008a
	.4byte 0x00000003
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x00e60000
	.4byte 0x00013000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0xffff0038
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
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x005a0000
	.4byte 0x0001b000
	.4byte 0xffff0088
	.4byte 0x00000003
	.4byte 0x006e0000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00014000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x007e0000
	.4byte 0x00013000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00034000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x017d0000
	.4byte 0x00005000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x017d0000
	.4byte 0x00005000
	.4byte 0xffff0087
	.4byte 0x00000003
	.4byte 0x008c0000
	.4byte 0x00000000
	.4byte 0x01840000
	.4byte 0x00013000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02b60000
	.4byte 0x00008000
	.4byte 0xffff0080
	.4byte 0x00000003
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x02920000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x00015000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01cc0000
	.4byte 0x00010000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x00720000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x0001b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02a60000
	.4byte 0x0000b000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01fc0000
	.4byte 0x00000000
	.4byte 0x02900000
	.4byte 0x00003000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x021e0000
	.4byte 0x00000000
	.4byte 0x02740000
	.4byte 0x00003000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x02260000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x0000b000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x02f00000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00015000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02d40000
	.4byte 0x00000000
	.4byte 0x02740000
	.4byte 0x00013000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x01d20000
	.4byte 0x00013000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02ea0000
	.4byte 0x00000000
	.4byte 0x02040000
	.4byte 0x0001b000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02bc0000
	.4byte 0x00000000
	.4byte 0x020c0000
	.4byte 0x0001d000
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
	.4byte 0x10004e15
	.4byte 0x094c020e
	.4byte 0x0200b0bd
	.4byte 0x00004e15
	.4byte 0x094c040e
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x09e0000a
	.4byte 0x02009381
	.4byte 0x00000002
	.4byte 0x0201000a
	.4byte 0x02009765
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02009645
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02009881
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x02009aa5
	.4byte 0x00000000
	.4byte 0x094c000f
	.4byte 0x00002968
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000029af
	.4byte 0x00008d15
	.4byte 0x094c000f
	.4byte 0x0000296b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000029b6
	.4byte 0x00000000
	.4byte 0x094c0011
	.4byte 0x00002969
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008d69
	.4byte 0x00008d15
	.4byte 0x094c0011
	.4byte 0x0000296c
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000029b7
	.4byte 0x00000000
	.4byte 0x094c0010
	.4byte 0x0000296a
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008e3d
	.4byte 0x00008d15
	.4byte 0x094c0010
	.4byte 0x0000296d
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000029b8
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
	.4byte 0x0000ce01
	.4byte 0x194d0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008281
	.4byte 0x00008c15
	.4byte 0x094e000e
	.4byte 0x02009cc9
	.4byte 0x0000c403
	.4byte 0x094d0016
	.4byte 0x02008d29
	.4byte 0x00000003
	.4byte 0xffff000a
	.4byte 0x02008d49
	.4byte 0x00000002
	.4byte 0x0203000b
	.4byte 0x0200a809
	.4byte 0x0000c400
	.4byte 0xffff0011
	.4byte 0x0200922d
	.4byte 0x00008d15
	.4byte 0x0a210011
	.4byte 0x000029ff
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002a0a
	.4byte 0x00000000
	.4byte 0x09fe000f
	.4byte 0x0000292d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000029da
	.4byte 0x00008d15
	.4byte 0x09fe000f
	.4byte 0x00002932
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000029e4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000ce01
	.4byte 0x194d0002
	.4byte 0x00000002
	.4byte 0x0000ce01
	.4byte 0x194d0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200bc59
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200bc75
	.4byte 0x0000c602
	.4byte 0x194d0006
	.4byte 0x0200bc91
	.4byte 0x0000c602
	.4byte 0x194d0007
	.4byte 0x0200bcad
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000ce01
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x0200bcc9
	.4byte 0x0000c602
	.4byte 0x094f000d
	.4byte 0x02009d3d
	.4byte 0x0000c602
	.4byte 0xffff000d
	.4byte 0x0200bce5
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00008515
	.4byte 0x02040013
	.4byte 0x00000000
	.4byte 0x00000c15
	.4byte 0x02050015
	.4byte 0x0200a879
	.4byte 0x0000c403
	.4byte 0x094d0014
	.4byte 0x02008d09
	.4byte 0x0000c403
	.4byte 0x094d0016
	.4byte 0x02008d29
	.4byte 0x0000c403
	.4byte 0x094f0015
	.4byte 0x02009d3d
	.4byte 0x00000000
	.4byte 0x09fe0008
	.4byte 0x0000292d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000029d6
	.4byte 0x00008d15
	.4byte 0x09fe0008
	.4byte 0x0000292e
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000029e0
	.4byte 0x00000000
	.4byte 0x09fe0009
	.4byte 0x0000292d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000029d7
	.4byte 0x00008d15
	.4byte 0x09fe0009
	.4byte 0x0000292f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000029e1
	.4byte 0x00000000
	.4byte 0x09fe000a
	.4byte 0x0000292d
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000029d8
	.4byte 0x00008d15
	.4byte 0x09fe000a
	.4byte 0x00002930
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000029e2
	.4byte 0x00000000
	.4byte 0x09fe000b
	.4byte 0x0000292d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000029d9
	.4byte 0x00008d15
	.4byte 0x09fe000b
	.4byte 0x00002931
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000029e3
	.4byte 0x00000000
	.4byte 0x09fe000c
	.4byte 0x0000292d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000029db
	.4byte 0x00008d15
	.4byte 0x09fe000c
	.4byte 0x00002933
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000029e5
	.4byte 0x00000000
	.4byte 0x09fe000d
	.4byte 0x0000292d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02009119
	.4byte 0x00008d15
	.4byte 0x09fe000d
	.4byte 0x00002934
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000029e6
	.4byte 0x00000000
	.4byte 0x09fe000e
	.4byte 0x0000292d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000029df
	.4byte 0x00008d15
	.4byte 0x09fe000e
	.4byte 0x00002935
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000029e7
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008ef9
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002941
	.4byte 0x00000000
	.4byte 0x09fe0011
	.4byte 0x02008f2d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002a0b
	.4byte 0x00008d15
	.4byte 0x09fe0011
	.4byte 0x00002943
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002a0d
	.4byte 0x00000000
	.4byte 0x09fe0010
	.4byte 0x02008f61
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002a0c
	.4byte 0x00008d15
	.4byte 0x09fe0010
	.4byte 0x00002942
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002a0e
	.4byte 0x00004114
	.4byte 0x09e1000f
	.4byte 0x0200a069
	.4byte 0x0001ff14
	.4byte 0xffff000f
	.4byte 0x0200a79d
	.4byte 0x0001ff14
	.4byte 0xffff0010
	.4byte 0x0200a7c1
	.4byte 0x0001ff14
	.4byte 0xffff0011
	.4byte 0x0200a7e5
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
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x09fe0008
	.4byte 0x0000292d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002a12
	.4byte 0x00008d15
	.4byte 0x09fe0008
	.4byte 0x0000295f
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002a16
	.4byte 0x00000000
	.4byte 0x09fe0009
	.4byte 0x0000292d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002a13
	.4byte 0x00008d15
	.4byte 0x09fe0009
	.4byte 0x00002960
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002a17
	.4byte 0x00000000
	.4byte 0x09fe000a
	.4byte 0x0000292d
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002a14
	.4byte 0x00008d15
	.4byte 0x09fe000a
	.4byte 0x00002961
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002a18
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0200913d
	.4byte 0x00008d15
	.4byte 0x09ed000b
	.4byte 0x00002a15
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002a31
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008fc5
	.4byte 0x00008d15
	.4byte 0x09fe000d
	.4byte 0x00002963
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002a25
	.4byte 0x00000000
	.4byte 0x09fe000e
	.4byte 0x0000292d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002a21
	.4byte 0x00008d15
	.4byte 0x09fe000e
	.4byte 0x00002964
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002a26
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000029e8
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000029ea
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000029e9
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000029eb
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002a22
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002a27
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002a23
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002a28
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002a24
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002a29
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x0040305b
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x0040305c
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x0040305d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000029ec
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000029f0
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000029ed
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000029f1
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000029ee
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000029f2
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000029ef
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000029f3
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020091c1
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002a1d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002a1a
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002a1c
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x0040305b
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x0040305c
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x0040305d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020090c9
	.4byte 0x00008d15
	.4byte 0x09fe0008
	.4byte 0x00002967
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002a2d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002a2b
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002a2e
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002a2c
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002a2f
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x00000026
