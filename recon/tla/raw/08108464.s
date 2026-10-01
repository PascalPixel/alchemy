.syntax unified
	.thumb
	.global Func_08108464
	.thumb_func
Func_08108464:
	push {r5, r6, lr}
	ldr r3, .L_081084d8
	ldr r2, .L_081084dc
	movs r1, #128
	str r2, [r3, #16]
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r3, r2
	lsls r1, r1, #3
	movs r2, #28
	strb r2, [r3]
	adds r1, #141
	movs r0, #1
	bl Inventory_AddItemFar
	adds r1, r0, #0
	movs r0, #1
	bl Inventory_EquipFar
	movs r1, #195
	lsls r1, r1, #2
	adds r1, #255
	movs r0, #0
	bl Inventory_AddItemFar
	adds r1, r0, #0
	movs r0, #0
	bl Inventory_EquipFar
	movs r1, #231
	movs r0, #2
	bl Inventory_AddItemFar
	movs r0, #3
	bl Owner_GetState
	movs r6, #50
	movs r5, #1
	adds r6, #255
	strb r5, [r0, r6]
	movs r0, #5
	bl Owner_GetState
	strb r5, [r0, r6]
	movs r0, #2
	bl Owner_GetState
	movs r3, #160
	lsls r3, r3, #1
	adds r0, r0, r3
	strb r5, [r0]
	movs r1, #30
	movs r0, #0
	bl Func_081082c4
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_081084d8:
	.4byte gPartyState
.L_081084dc:
	.4byte 0x00030d40
