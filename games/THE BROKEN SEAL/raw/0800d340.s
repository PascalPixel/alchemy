.syntax unified
	.thumb
	.set sub_080044d0, 0x080044d0
	.set sub_080072ec, 0x080072ec
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.global Overlay_0800d340
Overlay_0800d340:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #548]
	ldr	r5, [r3, #0]
	adds	r2, r5, #0
	sub	sp, #16
	movs	r0, #13
	adds	r2, #85
	str	r0, [sp, #12]
	str	r2, [sp, #0]
.L_0800d35e:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_0800d366
	b.n	.L_0800d62a
.L_0800d366:
	ldr	r4, [r5, #12]
	ldr	r3, [r5, #8]
	ldr	r2, [sp, #0]
	str	r4, [sp, #8]
	mov	fp, r3
	ldr	r0, [r5, #16]
	ldrb	r3, [r2, #12]
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_0800d37c
	b.n	.L_0800d528
.L_0800d37c:
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r4, #128
	ldr	r3, [r5, #56]
	lsls	r4, r4, #24
	cmp	r3, r4
	bne.n	.L_0800d38c
	b.n	.L_0800d472
.L_0800d38c:
	mov	r2, fp
	subs	r0, r3, r2
	cmp	r0, #0
	bge.n	.L_0800d398
	ldr	r3, [pc, #480]
	adds	r0, r0, r3
.L_0800d398:
	ldr	r3, [r5, #64]
	mov	r4, r9
	asrs	r6, r0, #16
	subs	r0, r3, r4
	cmp	r0, #0
	bge.n	.L_0800d3a8
	ldr	r2, [pc, #464]
	adds	r0, r0, r2
.L_0800d3a8:
	asrs	r7, r0, #16
	adds	r3, r7, #0
	muls	r3, r7
	adds	r0, r6, #0
	muls	r0, r6
	adds	r0, r0, r3
	ldr	r3, [pc, #452]
	bl	sub_080072f0
	ldr	r4, [pc, #452]
	lsls	r0, r0, #16
	cmp	r0, r4
	bgt.n	.L_0800d3f0
	ldr	r3, [r5, #56]
	mov	r0, fp
	subs	r6, r3, r0
	ldr	r3, [r5, #64]
	mov	r2, r9
	subs	r7, r3, r2
	adds	r0, r6, #0
	adds	r1, r6, #0
	ldr	r3, [pc, #432]
	mov	ip, pc
	bx	r3
	adds	r3, r0, #0
	adds	r1, r7, #0
	adds	r0, r7, #0
	ldr	r4, [pc, #420]
	mov	ip, pc
	bx	r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	ldr	r2, [pc, #400]
	bl	sub_080072ec
	lsls	r0, r0, #8
.L_0800d3f0:
	cmp	r0, #0
	bne.n	.L_0800d3fe
	ldr	r3, [r5, #56]
	ldr	r4, [r5, #64]
	mov	fp, r3
	mov	r9, r4
	b.n	.L_0800d4dc
.L_0800d3fe:
	ldr	r1, [r5, #52]
	ldr	r2, [pc, #388]
	bl	sub_080072ec
	ldr	r3, [pc, #380]
	adds	r4, r0, #0
	mov	r8, r3
	adds	r0, r6, #0
	adds	r1, r4, #0
	mov	ip, pc
	bx	r8
	ldr	r3, [r5, #36]
	adds	r3, r3, r0
	mov	sl, r3
	str	r3, [r5, #36]
	adds	r0, r7, #0
	adds	r1, r4, #0
	mov	ip, pc
	bx	r8
	ldr	r3, [r5, #44]
	adds	r6, r3, r0
	str	r6, [r5, #44]
	mov	r0, sl
	mov	r1, sl
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r3, r0, #0
	adds	r1, r6, #0
	adds	r0, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r3, r3, r0
	adds	r0, r3, #0
	ldr	r4, [pc, #308]
	bl	sub_080072f4
	ldr	r1, [r5, #48]
	lsls	r0, r0, #8
	cmp	r0, r1
	ble.n	.L_0800d4dc
	ldr	r2, [pc, #308]
	bl	sub_080072ec
	adds	r4, r0, #0
	adds	r1, r4, #0
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r8
	str	r0, [r5, #36]
	adds	r1, r4, #0
	adds	r0, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	b.n	.L_0800d4da
.L_0800d472:
	ldr	r6, [r5, #36]
	ldr	r7, [r5, #44]
	adds	r3, r6, #0
	orrs	r3, r7
	cmp	r3, #0
	beq.n	.L_0800d4dc
	ldr	r3, [pc, #260]
	adds	r0, r6, #0
	mov	r8, r3
	adds	r1, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r3, r0, #0
	adds	r1, r7, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r3, r3, r0
	ldr	r2, [pc, #224]
	adds	r0, r3, #0
	bl	sub_080072ec
	lsls	r0, r0, #8
	cmp	r0, #0
	beq.n	.L_0800d4d6
	ldr	r3, [r5, #52]
	subs	r1, r0, r3
	cmp	r1, #0
	bge.n	.L_0800d4b8
	ldr	r4, [sp, #4]
	str	r4, [r5, #36]
	str	r4, [r5, #44]
	b.n	.L_0800d4dc
.L_0800d4b8:
	ldr	r3, [pc, #204]
	bl	sub_080072f0
	adds	r4, r0, #0
	adds	r1, r4, #0
	adds	r0, r6, #0
	mov	ip, pc
	bx	r8
	str	r0, [r5, #36]
	adds	r1, r4, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	b.n	.L_0800d4da
.L_0800d4d6:
	ldr	r0, [sp, #4]
	str	r0, [r5, #36]
.L_0800d4da:
	str	r0, [r5, #44]
.L_0800d4dc:
	ldr	r3, [sp, #0]
	ldrb	r2, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800d528
	ldr	r3, [r5, #20]
	ldr	r4, [sp, #8]
	cmp	r4, r3
	ble.n	.L_0800d4fc
	ldr	r2, [r5, #40]
	ldr	r3, [r5, #72]
	subs	r2, r2, r3
	str	r2, [r5, #40]
	adds	r0, r2, #0
	b.n	.L_0800d52a
.L_0800d4fc:
	ldr	r0, [r5, #40]
	cmp	r0, #0
	bge.n	.L_0800d52a
	str	r3, [sp, #8]
	ldr	r3, [pc, #124]
	ldr	r1, [r5, #68]
	mov	ip, pc
	bx	r3
	adds	r3, r0, #0
	negs	r0, r3
	adds	r1, r0, #0
	str	r0, [r5, #40]
	cmp	r1, #0
	bge.n	.L_0800d51a
	adds	r1, r3, #0
.L_0800d51a:
	ldr	r3, [r5, #72]
	cmp	r1, r3
	bgt.n	.L_0800d52a
	movs	r3, #0
	str	r3, [r5, #40]
	movs	r0, #0
	b.n	.L_0800d52a
.L_0800d528:
	ldr	r0, [r5, #40]
.L_0800d52a:
	ldr	r4, [sp, #8]
	adds	r4, r4, r0
	ldr	r3, [r5, #36]
	str	r4, [sp, #8]
	add	fp, r3
	ldr	r0, [sp, #0]
	ldr	r3, [r5, #44]
	add	r9, r3
	ldrb	r3, [r0, #1]
	adds	r1, r5, #0
	adds	r1, #86
	cmp	r3, #0
	beq.n	.L_0800d5a4
	cmp	r3, #17
	beq.n	.L_0800d564
	cmp	r3, #17
	bgt.n	.L_0800d552
	cmp	r3, #16
	beq.n	.L_0800d558
	b.n	.L_0800d5a4
.L_0800d552:
	cmp	r3, #18
	beq.n	.L_0800d58c
	b.n	.L_0800d5a4
.L_0800d558:
	ldr	r2, [r5, #56]
	cmp	fp, r2
	beq.n	.L_0800d5a0
	ldr	r3, [r5, #8]
	mov	r4, fp
	b.n	.L_0800d596
.L_0800d564:
	ldr	r2, [r5, #60]
	ldr	r3, [sp, #8]
	cmp	r3, r2
	beq.n	.L_0800d5a0
	ldr	r3, [r5, #12]
	ldr	r4, [sp, #8]
	b.n	.L_0800d596
	movs	r0, r0
	.4byte 0x03001e64
	.4byte 0x0000ffff
	.4byte 0x030001d8
	.4byte 0x00ffffff
	.4byte 0x03000118
	.2byte 0x013c
	.2byte 0x0300
.L_0800d58c:
	ldr	r2, [r5, #64]
	cmp	r9, r2
	beq.n	.L_0800d5a0
	ldr	r3, [r5, #16]
	mov	r4, r9
.L_0800d596:
	subs	r3, r3, r2
	subs	r2, r4, r2
	eors	r3, r2
	cmp	r3, #0
	bge.n	.L_0800d5a4
.L_0800d5a0:
	movs	r0, #1
	str	r0, [sp, #4]
.L_0800d5a4:
	ldr	r2, [sp, #4]
	cmp	r2, #0
	beq.n	.L_0800d5da
	ldr	r4, [sp, #0]
	ldrb	r3, [r4, #3]
	cmp	r3, #0
	beq.n	.L_0800d5cc
	movs	r3, #0
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	ldr	r0, [r5, #56]
	ldr	r2, [r5, #64]
	ldrb	r3, [r4, #0]
	mov	fp, r0
	mov	r9, r2
	cmp	r3, #0
	bne.n	.L_0800d5cc
	ldr	r4, [r5, #60]
	str	r4, [sp, #8]
	str	r3, [r5, #40]
.L_0800d5cc:
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #56]
	str	r3, [r5, #60]
	str	r3, [r5, #64]
	movs	r3, #0
	strb	r3, [r1, #0]
.L_0800d5da:
	mov	r0, fp
	str	r0, [r5, #8]
	ldr	r2, [sp, #8]
	mov	r3, r9
	str	r3, [r5, #16]
	str	r2, [r5, #12]
	ldr	r4, [sp, #0]
	ldrb	r2, [r4, #5]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800d62a
	ldr	r0, [r5, #36]
	ldr	r2, [r5, #44]
	mov	fp, r0
	mov	r9, r2
	cmp	r0, #0
	bne.n	.L_0800d602
	cmp	r2, #0
	beq.n	.L_0800d62a
.L_0800d602:
	mov	r0, r9
	mov	r1, fp
	bl	sub_080044d0
	ldrh	r3, [r5, #6]
	subs	r0, r0, r3
	lsls	r0, r0, #16
	movs	r4, #128
	asrs	r0, r0, #16
	lsls	r4, r4, #5
	cmp	r0, r4
	ble.n	.L_0800d61e
	movs	r0, #128
	lsls	r0, r0, #5
.L_0800d61e:
	ldr	r2, [pc, #48]
	cmp	r0, r2
	bge.n	.L_0800d626
	ldr	r0, [pc, #40]
.L_0800d626:
	adds	r3, r3, r0
	strh	r3, [r5, #6]
.L_0800d62a:
	ldr	r3, [sp, #12]
	ldr	r4, [sp, #0]
	subs	r3, #1
	adds	r4, #112
	str	r3, [sp, #12]
	str	r4, [sp, #0]
	adds	r5, #112
	cmp	r3, #0
	blt.n	.L_0800d63e
	b.n	.L_0800d35e
.L_0800d63e:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0xfffff000
