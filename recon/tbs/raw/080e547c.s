.syntax unified
	.thumb
	.set sub_080e46f0, 0x080e46f0
	.set sub_080e551a, 0x080e551a
	.global Fragment_080e547c
	.global Func_080e547c
	.thumb_func
Fragment_080e547c:
Func_080e547c:
	ldr	r0, [pc, #92]
	bl	sub_080e46f0
	b.n	sub_080e551a
	ldr	r0, [pc, #88]
	bl	sub_080e46f0
	b.n	sub_080e551a
	ldr	r0, [pc, #84]
	bl	sub_080e46f0
	b.n	sub_080e551a
	ldr	r0, [pc, #80]
	bl	sub_080e46f0
	b.n	sub_080e551a
	ldr	r0, [pc, #76]
	bl	sub_080e46f0
	b.n	sub_080e551a
	ldr	r0, [pc, #72]
	bl	sub_080e46f0
	b.n	sub_080e551a
	ldr	r0, [pc, #68]
	bl	sub_080e46f0
	b.n	sub_080e551a
	ldr	r0, [pc, #64]
	bl	sub_080e46f0
	b.n	sub_080e551a
	ldr	r0, [pc, #60]
	bl	sub_080e46f0
	b.n	sub_080e551a
	ldr	r0, [pc, #56]
	bl	sub_080e46f0
	b.n	sub_080e551a
	ands	r0, r0
	lsls	r1, r0, #8
	.2byte 0xbb9d
	lsrs	r5, r1, #32
	ldrb	r0, [r5, #0]
	movs	r0, r0
	strh	r4, [r6, r7]
	lsrs	r6, r1, #32
	.4byte 0x00000053
	.4byte 0x0000006f
	.4byte 0x00000079
	.4byte 0x000000c3
	.4byte 0x0000008d
	.4byte 0x000000bb
	.4byte 0x000000b9
	.4byte 0x000000c0
	.4byte 0x000000a4
	.4byte 0x000000a3
