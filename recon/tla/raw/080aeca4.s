.syntax unified
	.thumb
	.global Inventory_GetQuantity
	.thumb_func
Inventory_GetQuantity:
	push {r5, lr}
	adds r5, r1, #0
	bl Owner_GetState
	lsls r5, r5, #1
	adds r5, #216
	ldrh r0, [r0, r5]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r3, r0
	lsrs r0, r0, #11
	adds r0, #1
	cmp r3, #0
	bne .L_080aecc4
	movs r0, #0
.L_080aecc4:
	pop {r5, pc}
	.2byte 0x0000
