.syntax unified
	.thumb
	.set sub_08077394, 0x08077394
	.set sub_08077428, 0x08077428
	.set sub_0807808c, 0x0807808c
	.set sub_08078414, 0x08078414
	.set sub_08079358, 0x08079358
	.set sub_08079ae8, 0x08079ae8
	.set sub_0807a628, 0x0807a628
	.global Overlay_0807a664
Overlay_0807a664:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #132]
	ldr	r2, [pc, #132]
	ldrh	r3, [r0, #0]
	sub	sp, #4
	mov	r8, r0
	cmp	r3, r2
	bne.n	.L_0807a682
	b.n	.L_0807a780
.L_0807a682:
	mov	r1, r8
	ldr	r3, [pc, #120]
	movs	r0, #136
	strh	r2, [r1, #0]
	lsls	r0, r0, #2
	movs	r2, #2
	add	r8, r2
	adds	r2, r3, r0
	movs	r0, #0
	ldrsh	r1, [r2, r0]
	mov	r9, r1
	ldr	r1, [pc, #104]
	adds	r3, r3, r1
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	movs	r1, #0
	mov	fp, r2
	mov	sl, r1
.L_0807a6a6:
	mov	r0, sl
	bl	sub_08077394
	adds	r7, r0, #0
	adds	r2, r7, #0
	adds	r2, #216
	movs	r5, #14
.L_0807a6b4:
	ldrh	r3, [r2, #0]
	mov	r0, r8
	movs	r1, #2
	subs	r5, #1
	adds	r2, #2
	strh	r3, [r0, #0]
	add	r8, r1
	cmp	r5, #0
	bge.n	.L_0807a6b4
	ldr	r2, [pc, #44]
	movs	r6, #216
	movs	r5, #14
.L_0807a6cc:
	ldrh	r0, [r6, r7]
	str	r2, [sp, #0]
	bl	sub_08078414
	ldrb	r3, [r0, #2]
	ldr	r2, [sp, #0]
	cmp	r3, #6
	beq.n	.L_0807a6de
	strh	r2, [r6, r7]
.L_0807a6de:
	subs	r5, #1
	adds	r6, #2
	cmp	r5, #0
	bge.n	.L_0807a6cc
	adds	r0, r7, #0
	adds	r0, #216
	movs	r5, #0
	adds	r4, r0, #0
	adds	r1, r0, #0
	movs	r6, #14
	b.n	.L_0807a708
	.4byte 0x00000000
	.4byte 0x02001078
	.4byte 0x00006774
	.4byte 0x02000240
	.2byte 0x0222
	.2byte 0x0000
.L_0807a708:
	ldrh	r2, [r4, #0]
	lsls	r3, r2, #16
	adds	r4, #2
	cmp	r3, #0
	beq.n	.L_0807a718
	strh	r2, [r1, #0]
	adds	r5, #1
	adds	r1, #2
.L_0807a718:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L_0807a708
	cmp	r5, #14
	bgt.n	.L_0807a73c
	lsls	r3, r5, #1
	adds	r0, r3, r0
	ldr	r2, [pc, #16]
	movs	r3, #15
	subs	r5, r3, r5
.L_0807a72c:
	subs	r5, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r5, #0
	bne.n	.L_0807a72c
	b.n	.L_0807a73c
	.2byte 0x0000
	.2byte 0x0000
.L_0807a73c:
	mov	r0, sl
	bl	sub_08079ae8
	mov	r0, sl
	bl	sub_08077428
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #3
	ble.n	.L_0807a6a6
	movs	r2, #2
	mov	r1, r8
	mov	r0, r9
	add	r8, r2
	strh	r0, [r1, #0]
	mov	r3, fp
	mov	r0, r8
	strh	r3, [r0, #0]
	ldr	r0, [pc, #52]
	add	r8, r2
	ldrh	r3, [r0, #0]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #2]
	mov	r2, r8
	movs	r0, #0
	strh	r3, [r2, #2]
	movs	r1, #16
	bl	sub_0807a628
	ldr	r0, [pc, #32]
	bl	sub_08079358
.L_0807a780:
	movs	r0, #1
	bl	sub_0807808c
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000438
	.4byte 0x00000952
