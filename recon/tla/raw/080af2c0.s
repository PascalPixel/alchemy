.syntax unified
	.thumb
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	PartyInventory_FindOwner
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	movs	r0, #0
	cmp	r5, r3
	beq.n	.L_080af2e6
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	Inventory_Find
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	0x080af1c8
	movs	r0, #0
.L_080af2e6:
	pop	{r5, r6, pc}
