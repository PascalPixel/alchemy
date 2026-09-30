.syntax unified
	.thumb
	.global Owner_RefreshDerivedData
	.thumb_func
Owner_RefreshDerivedData:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Owner_GetState
	movs r1, #10
	adds r6, r0, #0
	bl Inventory_GetEquippedItem
	movs r1, #165
	lsls r1, r1, #1
	adds r3, r6, r1
	adds r1, r6, #0
	adds r2, r0, #0
	adds r1, #248
	ldrh r0, [r3]
	bl Func_080b0144
	movs r2, #42
	adds r2, #255
	adds r3, r6, r2
	strb r0, [r3]
	adds r0, r5, #0
	bl Func_080af4e4
	adds r1, r6, #0
	adds r1, #36
	adds r0, r5, #0
	bl Func_080b0084
	pop {r5, r6, pc}
