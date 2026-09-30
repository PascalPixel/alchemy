.syntax unified
	.thumb
	.global Func_0810a03c
	.thumb_func
Func_0810a03c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	sub sp, #4
	ldr r7, [r3]
	mov r10, r0
	mov r9, r1
	bl Owner_GetState
	mov r1, r9
	lsls r5, r1, #1
	adds r6, r0, #0
	adds r5, #216
	ldrh r0, [r6, r5]
	bl Item_Get
	movs r2, #1
	str r2, [sp, #0]
	mov r8, r0
	ldrh r0, [r6, r5]
	bl Shop_GetSellPrice
	mov r1, r9
	mov r11, r0
	mov r0, r10
	bl Inventory_GetQuantityFar
	mov r3, r8
	ldrb r2, [r3, #3]
	movs r3, #16
	ands r3, r2
	mov r10, r0
	cmp r3, #0
	beq .L_0810a0f4
	cmp r0, #1
	ble .L_0810a0f4
	ldr r0, .L_0810a104
	bl Func_081084f4
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #228
	adds r3, r7, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r0, #0
	mov r8, r2
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #230
	adds r3, r7, r2
	subs r2, #10
	adds r5, r7, r2
	ldr r2, [r5]
	movs r1, #0
	ldrsh r6, [r3, r1]
	movs r3, #4
	strb r3, [r2, #5]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #5
	adds r2, r7, r3
	movs r3, #12
	strb r3, [r2]
	movs r1, #128
	movs r2, #48
	bl Func_08108af0
	mov r1, r10
	mov r2, r11
	movs r0, #0
	bl Func_081096f8
	str r0, [sp, #0]
	movs r0, #1
	bl WaitFrames
	ldr r0, [r5]
	bl UiIcon_PrepareObjectFar
	movs r0, #0
	mov r1, r8
	adds r2, r6, #0
	bl Func_08108af0
.L_0810a0f4:
	ldr r0, [sp, #0]
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0810a104:
	.4byte 0x0000125e
