.syntax unified
	.thumb
	.global Func_0803f968
	.thumb_func
Func_0803f968:
	push {lr}
	adds r0, r1, #0
	bl PartyInventory_RemoveFar
	movs r0, #0
	pop {pc}
