.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08023f64, 0x08023f64
	.global Func_08023f3c
	.thumb_func
Func_08023f3c:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r5, #0
	b.n	.L_08023f4c
.L_08023f44:
	movs	r0, #1
	bl	sub_08013560
	adds	r5, #1
.L_08023f4c:
	movs	r3, #172
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	r5, r3
	bgt.n	.L_08023f60
	adds	r0, r6, #0
	bl	sub_08023f64
	cmp	r0, #0
	beq.n	.L_08023f44
.L_08023f60:
	pop	{r5, r6, pc}
	.2byte 0x0000
