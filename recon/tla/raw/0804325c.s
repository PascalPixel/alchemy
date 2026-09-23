.syntax unified
	.thumb
	.set sub_0801596c, 0x0801596c
	.set sub_08016054, 0x08016054
	.set sub_0801613c, 0x0801613c
	.global Func_0804325c
	.thumb_func
Func_0804325c:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_0801596c
	movs	r5, #9
	negs	r5, r5
	cmp	r0, #0
	bne.n	.L_0804329a
	bl	sub_08016054
	adds	r5, r0, #0
	cmp	r6, #0
	beq.n	.L_0804329a
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r3, [r3, #0]
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #109
	adds	r2, r3, r1
	movs	r1, #2
.L_08043288:
	ldrb	r3, [r2, #0]
	adds	r2, #64
	lsls	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08043294
	subs	r5, #1
.L_08043294:
	subs	r1, #1
	cmp	r1, #0
	bge.n	.L_08043288
.L_0804329a:
	bl	sub_0801613c
	adds	r0, r5, #0
	pop	{r5, r6, pc}
	.align 2, 0
