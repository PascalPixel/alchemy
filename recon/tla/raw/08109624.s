.syntax unified
	.thumb
	.global Func_08109624
	.thumb_func
Func_08109624:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	adds r6, r1, #0
	mov r10, r3
	adds r5, r0, #0
	bl Owner_GetState
	adds r7, r0, #0
	adds r0, r6, #0
	bl Item_Get
	mov r8, r0
	mov r3, r8
	ldrb r2, [r3, #3]
	movs r3, #16
	ands r3, r2
	movs r0, #1
	cmp r3, #0
	beq .L_081096e6
	ldr r0, .L_081096f0
	bl Func_081084f4
	adds r0, r5, #0
	adds r1, r6, #0
	bl Inventory_FindFar
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_08109678
	lsls r3, r0, #1
	adds r3, #216
	ldrh r3, [r7, r3]
	lsrs r3, r3, #11
	adds r7, r3, #1
	b .L_0810967a
.L_08109678:
	movs r7, #0
.L_0810967a:
	mov r2, r8
	ldrh r3, [r2]
	movs r5, #30
	cmp r3, #0
	beq .L_08109690
	ldr r3, .L_081096f4
	ldrh r1, [r2]
	ldr r0, [r3, #16]
	bl Math_DivU
	adds r5, r0, #0
.L_08109690:
	movs r3, #129
	lsls r3, r3, #3
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_081096bc
	adds r0, r6, #0
	movs r1, #0
	bl Item_AdjustCounterFar
	cmp r5, r0
	ble .L_081096b8
	adds r0, r6, #0
	movs r1, #0
	bl Item_AdjustCounterFar
	b .L_081096ba
.L_081096b8:
	adds r0, r5, #0
.L_081096ba:
	adds r5, r0, #0
.L_081096bc:
	adds r5, r5, r7
	cmp r5, #30
	ble .L_081096c4
	movs r5, #30
.L_081096c4:
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #5
	add r3, r10
	movs r2, #12
	strb r2, [r3]
	movs r0, #0
	movs r1, #128
	movs r2, #48
	bl Func_08108af0
	mov r3, r8
	ldrh r2, [r3]
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_081096f8
.L_081096e6:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081096f0:
	.4byte 0x00001251
.L_081096f4:
	.4byte gPartyState
