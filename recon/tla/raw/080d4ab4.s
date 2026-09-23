.syntax unified
	.thumb
	.set sub_080200a8, 0x080200a8
	.set sub_080cad84, 0x080cad84
	.set sub_080d2d84, 0x080d2d84
	.global Func_080d4ab4
	.thumb_func
Func_080d4ab4:
	push	{r5, lr}
	ldr	r3, [pc, #80]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_080cad84
	ldr	r3, [r0, #80]
	ldrb	r3, [r3, #9]
	lsls	r3, r3, #28
	lsrs	r4, r3, #30
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L_080d4b04
	ldr	r1, [r5, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	negs	r2, r2
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
.L_080d4b04:
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	subs	r3, r1, #1
	adds	r5, r0, #0
	cmp	r3, #10
	bhi.n	.L_080d4b86
	ldr	r2, [pc, #120]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080d4b4c
	.4byte 0x080d4b50
	.4byte 0x080d4b54
	.4byte 0x080d4b58
	.4byte 0x080d4b5c
	.4byte 0x080d4b60
	.4byte 0x080d4b74
	.4byte 0x080d4b78
	.4byte 0x080d4b7c
	.4byte 0x080d4b80
	.2byte 0x4b84
	.2byte 0x080d
	ldr	r1, [pc, #68]
	b.n	.L_080d4b86
	ldr	r1, [pc, #68]
	b.n	.L_080d4b86
	ldr	r1, [pc, #68]
	b.n	.L_080d4b86
	ldr	r1, [pc, #68]
	b.n	.L_080d4b86
	ldr	r1, [pc, #68]
	b.n	.L_080d4b86
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_080d2d84
	ldr	r1, [pc, #60]
	str	r0, [r5, #104]
	b.n	.L_080d4b86
	ldr	r1, [pc, #56]
	b.n	.L_080d4b86
	ldr	r1, [pc, #56]
	b.n	.L_080d4b86
	ldr	r1, [pc, #56]
	b.n	.L_080d4b86
	ldr	r1, [pc, #56]
	b.n	.L_080d4b86
	ldr	r1, [pc, #56]
.L_080d4b86:
	adds	r0, r5, #0
	bl	sub_080200a8
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x080d4b20
	.4byte 0x080f34e4
	.4byte 0x080f3428
	.4byte 0x080f350c
	.4byte 0x080f35c8
	.4byte 0x080f373c
	.4byte 0x02000240
	.4byte 0x080f3750
	.4byte 0x080f34e8
	.4byte 0x080f3614
	.4byte 0x080f3764
	.4byte 0x080f3500
	.4byte 0x080f34f4
