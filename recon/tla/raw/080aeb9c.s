.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080aea30, 0x080aea30
	.global Func_080aeb9c
	.thumb_func
Func_080aeb9c:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r0, #0
	bl	sub_08016ca4
	adds	r6, r0, #0
	movs	r1, #56
	ldrsh	r3, [r6, r1]
	movs	r1, #52
	ldrsh	r2, [r6, r1]
	adds	r3, r3, r5
	adds	r1, r2, #0
	cmp	r3, r2
	bgt.n	.L_080aebc0
	movs	r1, #0
	cmp	r3, #0
	blt.n	.L_080aebc0
	adds	r1, r3, #0
.L_080aebc0:
	adds	r0, r7, #0
	strh	r1, [r6, #56]
	bl	sub_080aea30
	movs	r2, #56
	ldrsh	r0, [r6, r2]
	pop	{r5, r6, r7, pc}
	.align 2, 0
