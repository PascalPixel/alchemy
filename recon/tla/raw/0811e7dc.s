.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_08020068, 0x08020068
	.set sub_08020370, 0x08020370
	.set sub_0811bdb0, 0x0811bdb0
	.set sub_0811be3c, 0x0811be3c
	.global Func_0811e7dc
	.thumb_func
Func_0811e7dc:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #48
	ldrb	r0, [r3, #0]
	bl	sub_0811be3c
	ldr	r3, [r5, #44]
	ldr	r6, [r0, #0]
	cmp	r3, #0
	beq.n	.L_0811e82c
	movs	r1, #0
	adds	r0, r6, #0
	bl	sub_0811bdb0
	bl	sub_08020370
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [r5, #44]
	bl	sub_08013164
	movs	r3, #0
	movs	r1, #0
	adds	r0, r6, #0
	str	r3, [r5, #44]
	bl	sub_0811bdb0
	ldrb	r2, [r0, #26]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #26]
	movs	r1, #0
	adds	r0, r6, #0
	bl	sub_0811bdb0
	movs	r1, #0
	bl	sub_08020068
.L_0811e82c:
	pop	{r5, r6, pc}
	.align 2, 0
