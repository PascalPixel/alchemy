.syntax unified
	.thumb
	.set sub_080e19b2, 0x080e19b2
	.set sub_080e19e8, 0x080e19e8
	.set sub_080e1acc, 0x080e1acc
	.set sub_080e1ae2, 0x080e1ae2
	.set sub_080e1b8e, 0x080e1b8e
	.set sub_080e2ab2, 0x080e2ab2
	.set sub_080ed408, 0x080ed408
	.global Fragment_080e1a48
	.global Func_080e1a48
	.thumb_func
Fragment_080e1a48:
Func_080e1a48:
	ldr	r1, [sp, #40]
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #32]
	movs	r4, #1
	add	r9, r4
	adds	r1, #24
	adds	r2, #24
	subs	r3, #48
	mov	r5, r9
	str	r1, [sp, #40]
	str	r2, [sp, #36]
	str	r3, [sp, #32]
	cmp	r5, #3
	bne.n	sub_080e19b2
	ldr	r0, [sp, #116]
	cmp	r0, #179
	ble.n	.L_080e1a6c
	b.n	sub_080e1b8e
.L_080e1a6c:
	movs	r5, #0
	cmp	r0, #155
	ble.n	.L_080e1a76
	adds	r5, r0, #0
	subs	r5, #156
.L_080e1a76:
	cmp	r5, #7
	ble.n	.L_080e1a7c
	movs	r5, #7
.L_080e1a7c:
	ldr	r1, [sp, #116]
	cmp	r1, #139
	bgt.n	sub_080e1acc
	movs	r3, #3
	movs	r2, #7
	movs	r0, #47
	movs	r1, #7
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [pc, #52]
	ldr	r2, [r2, #0]
	str	r2, [sp, #88]
	b.n	sub_080e1ae2
	strb	r5, [r2, r5]
	movs	r0, r0
	add	r2, sp, #684
	movs	r0, r0
	strb	r4, [r0, #30]
	movs	r0, r0
	subs	r2, r3, r0
	subs	r2, r3, r0
	lsls	r4, r7, #4
	lsls	r0, r0, #12
	movs	r0, r0
	ldrb	r7, [r7, #31]
	bl	sub_080e2ab2
	bge.n	sub_080e19e8
	lsrs	r6, r1, #32
	.2byte 0xff00
	.2byte 0xffff
	lsls	r0, r3, #4
	lsls	r0, r0, #12
	lsls	r5, r2, #21
	movs	r0, r0
	udf	#72
	lsrs	r6, r1, #32
	.4byte 0x03001f0c
