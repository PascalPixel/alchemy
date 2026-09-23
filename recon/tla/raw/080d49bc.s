.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_080148e8, 0x080148e8
	.set sub_08020090, 0x08020090
	.set sub_08020148, 0x08020148
	.global Func_080d49bc
	.thumb_func
Func_080d49bc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldr	r1, [r6, #104]
	cmp	r1, #0
	beq.n	.L_080d4a48
	ldr	r2, [r1, #8]
	ldr	r3, [r6, #8]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_080d49dc
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r0, r0, r2
.L_080d49dc:
	ldr	r2, [r1, #16]
	ldr	r3, [r6, #16]
	asrs	r5, r0, #16
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_080d49f0
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_080d49f0:
	asrs	r0, r0, #16
	mov	r8, r0
	mov	r2, r8
	mov	r3, r8
	muls	r3, r2
	adds	r0, r5, #0
	muls	r0, r5
	adds	r0, r0, r3
	ldr	r3, [pc, #76]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c33
	adds	r3, #100
	movs	r2, #0
	ldrsh	r7, [r3, r2]
	cmp	r0, r7
	blt.n	.L_080d4a40
	lsls	r0, r5, #20
	adds	r1, r7, #0
	bl	sub_08002054
	ldr	r5, [r6, #8]
	mov	r3, r8
	adds	r5, r5, r0
	adds	r1, r7, #0
	lsls	r0, r3, #20
	bl	sub_08002054
	ldr	r3, [r6, #16]
	adds	r1, r5, #0
	adds	r3, r3, r0
	ldr	r2, [r6, #12]
	adds	r0, r6, #0
	bl	sub_08020148
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08020090
	b.n	.L_080d4a48
.L_080d4a40:
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08020090
.L_080d4a48:
	movs	r0, #1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r1, [r5, #104]
	cmp	r1, #0
	beq.n	.L_080d4a9e
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [r1, #16]
	ldr	r3, [r5, #16]
	ldr	r1, [r1, #8]
	subs	r0, r0, r3
	ldr	r3, [r5, #8]
	subs	r1, r1, r3
	bl	sub_080148e8
	ldrh	r3, [r5, #6]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	subs	r0, r0, r3
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	cmp	r0, #0
	beq.n	.L_080d4a9e
	movs	r2, #128
	lsls	r2, r2, #5
	cmp	r0, r2
	ble.n	.L_080d4a92
	adds	r0, r2, #0
.L_080d4a92:
	ldr	r2, [pc, #16]
	cmp	r0, r2
	bge.n	.L_080d4a9a
	adds	r0, r2, #0
.L_080d4a9a:
	adds	r3, r3, r0
	strh	r3, [r5, #6]
.L_080d4a9e:
	movs	r0, #1
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0xf000
	.2byte 0xffff
	ldr	r3, [pc, #4]
	str	r1, [r0, #104]
	str	r3, [r0, #108]
	bx	lr
	.4byte 0x080d4a55
