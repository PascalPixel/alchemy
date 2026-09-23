.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.global ObjectDispatch_WaitForCommandEnd
	.global Func_08023634
	.thumb_func
ObjectDispatch_WaitForCommandEnd:
Func_08023634:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldr	r2, [r5, #0]
	movs	r6, #0
	b.n	.L_08023652
.L_0802363e:
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #172
	lsls	r3, r3, #1
	adds	r6, #1
	adds	r3, #255
	cmp	r6, r3
	bgt.n	.L_08023662
	ldr	r2, [r5, #0]
.L_08023652:
	cmp	r2, #0
	beq.n	.L_08023662
	movs	r1, #4
	ldrsh	r3, [r5, r1]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	cmp	r3, #17
	bne.n	.L_0802363e
.L_08023662:
	pop	{r5, r6, pc}
