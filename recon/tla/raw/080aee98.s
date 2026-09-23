.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.global Inventory_Find
	.global Func_080aee98
	.thumb_func
Inventory_Find:
Func_080aee98:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_08016ca4
	movs	r4, #128
	lsls	r4, r4, #1
	movs	r1, #0
	adds	r4, #255
	adds	r0, #216
.L_080aeeaa:
	ldrh	r2, [r0, #0]
	adds	r3, r4, #0
	ands	r3, r2
	adds	r0, #2
	cmp	r3, r5
	bne.n	.L_080aeeba
	adds	r0, r1, #0
	b.n	.L_080aeec4
.L_080aeeba:
	adds	r1, #1
	cmp	r1, #14
	ble.n	.L_080aeeaa
	movs	r0, #1
	negs	r0, r0
.L_080aeec4:
	pop	{r5, pc}
	.2byte 0x0000
