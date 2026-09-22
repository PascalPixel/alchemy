.syntax unified
	.thumb
	.set sub_080148e8, 0x080148e8
	.set sub_08020138, 0x08020138
	.set sub_08020148, 0x08020148
	.set sub_0811be3c, 0x0811be3c
	.global Overlay_0811be68
Overlay_0811be68:
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
	movs	r3, #171
	lsls	r3, r3, #8
	adds	r3, #133
	str	r3, [r5, #72]
	adds	r3, r5, #0
	movs	r2, #0
	adds	r3, #90
	str	r2, [r5, #40]
	str	r2, [r5, #68]
	strb	r2, [r3, #0]
	adds	r2, r5, #0
	adds	r2, #88
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	bl	sub_08020138
	adds	r0, r5, #0
	ldr	r1, [r6, #12]
	ldr	r3, [r6, #16]
	movs	r2, #0
	bl	sub_08020148
	ldr	r0, [r6, #16]
	cmp	r0, #0
	bge.n	.L_0811beb4
	adds	r0, #7
.L_0811beb4:
	ldr	r1, [r6, #12]
	asrs	r0, r0, #3
	bl	sub_080148e8
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r0, r0, r3
	strh	r0, [r5, #6]
	pop	{r5, r6, pc}
