.syntax unified
	.thumb
	.global Func_080ae6c8
	.thumb_func
Func_080ae6c8:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	bl PartyInventory_FindOwner
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_080ae6e2
	adds r0, r6, #0
	adds r1, r5, #0
	bl Inventory_AddItem
.L_080ae6e2:
	pop {r5, r6, pc}
