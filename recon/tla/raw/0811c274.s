.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08020058, 0x08020058
	.set sub_08020080, 0x08020080
	.set sub_0811be3c, 0x0811be3c
	.global Func_0811c274
	.thumb_func
Func_0811c274:
	push	{r5, lr}
	bl	sub_0811be3c
	ldr	r0, [r0, #0]
	cmp	r0, #0
	beq.n	.L_0811c2b0
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L_0811c2b0
	movs	r1, #208
	lsls	r1, r1, #1
	ldr	r0, [r0, #80]
	adds	r1, #255
	bl	sub_08020058
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0811c2aa
	movs	r1, #1
	bl	sub_08020080
	movs	r3, #3
	strb	r3, [r5, #6]
.L_0811c2aa:
	movs	r0, #10
	bl	sub_08013560
.L_0811c2b0:
	pop	{r5, pc}
	.align 2, 0
