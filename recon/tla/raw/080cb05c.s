.syntax unified
	.thumb
	.set sub_08016ce4, 0x08016ce4
	.set sub_080ad2e8, 0x080ad2e8
	.global Func_080cb05c
	.thumb_func
Func_080cb05c:
	push	{r5, lr}
	movs	r0, #252
	movs	r3, #192
	lsls	r0, r0, #3
	lsls	r3, r3, #18
	adds	r0, #255
	ldr	r5, [r3, #108]
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080cb08c
	movs	r0, #153
	bl	sub_080ad2e8
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_080cb08c
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #57
	adds	r2, r5, r3
	movs	r3, #1
	b.n	.L_080cb096
.L_080cb08c:
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #57
	adds	r2, r5, r3
	movs	r3, #0
.L_080cb096:
	strb	r3, [r2, #0]
	pop	{r5, pc}
	.2byte 0x0000
