.syntax unified
	.thumb
	.set sub_08020090, 0x08020090
	.set sub_08020138, 0x08020138
	.set sub_08020148, 0x08020148
	.set sub_0811be3c, 0x0811be3c
	.global Func_0811bec8
	.thumb_func
Func_0811bec8:
	push	{r5, r6, lr}
	bl	sub_0811be3c
	adds	r6, r0, #0
	ldr	r5, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #48]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #81
	adds	r2, r5, #0
	str	r3, [r5, #72]
	adds	r2, #90
	movs	r3, #0
	str	r3, [r5, #68]
	adds	r0, r5, #0
	strb	r3, [r2, #0]
	bl	sub_08020138
	ldr	r3, [r6, #12]
	adds	r0, r5, #0
	lsls	r1, r3, #1
	adds	r1, r1, r3
	movs	r2, #0
	ldr	r3, [r6, #16]
	bl	sub_08020148
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08020090
	pop	{r5, r6, pc}
	.align 2, 0
