.syntax unified
	.thumb
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_08015128, 0x08015128
	.set sub_08015768, 0x08015768
	.global Func_081ac028
	.thumb_func
Func_081ac028:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	movs	r3, #152
	lsls	r3, r3, #8
	strh	r3, [r5, #52]
	movs	r3, #255
	lsls	r3, r3, #17
	str	r3, [r5, #32]
	ldr	r3, [pc, #96]
	movs	r6, #0
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #20]
	strh	r6, [r5, #54]
	str	r6, [r5, #28]
	str	r6, [r3, #12]
	str	r6, [r3, #16]
	str	r6, [r5, #24]
	sub	sp, #12
	bl	sub_08014de4
	adds	r0, r5, #0
	adds	r0, #12
	bl	sub_08015128
	movs	r3, #54
	ldrsh	r0, [r5, r3]
	bl	sub_08015068
	movs	r3, #52
	ldrsh	r0, [r5, r3]
	bl	sub_08015024
	mov	r0, sp
	str	r6, [r0, #0]
	str	r6, [r0, #4]
	adds	r1, r5, #0
	ldr	r3, [r5, #32]
	str	r3, [r0, #8]
	ldr	r3, [pc, #40]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20fa
	movs	r1, #192
	ldr	r3, [pc, #32]
	lsls	r1, r1, #8
	lsls	r0, r0, #16
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c01
	movs	r0, #250
	lsls	r0, r0, #16
	ldr	r2, [pc, #20]
	bl	sub_08015768
	add	sp, #12
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x030011e0
	.4byte 0x03000354
	.4byte 0x03000230
	.2byte 0x0000
	.2byte 0x7fff
	push	{lr}
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	asrs	r0, r0, #16
	muls	r0, r1
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_081ac0c4
	adds	r3, #255
.L_081ac0c4:
	lsls	r0, r3, #8
	asrs	r0, r0, #16
	pop	{pc}
	.2byte 0x0000
