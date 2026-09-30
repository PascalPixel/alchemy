.syntax unified
	.thumb
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r7, #226
	mov r11, r3
	lsls r7, r7, #1
	add r7, r11
	movs r3, #0
	ldrsh r2, [r7, r3]
	sub sp, #12
	str r2, [sp, #0]
	adds r6, r0, #0
	bl PartyInventory_AddFar
	movs r2, #1
	mov r8, r0
	negs r2, r2
	cmp r8, r2
	beq .L_080d2642
	b .L_080d2782
.L_080d2642:
	adds r0, r6, #0
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r0, .L_080d27d8
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	ldr r0, .L_080d27dc
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r3, #8
	movs r2, #4
	add r3, sp
	add r2, sp
	mov r9, r3
	mov r10, r2
.L_080d2666:
	ldr r7, .L_080d27e0
	movs r1, #1
	adds r0, r7, #0
	bl UiText_ShowPositionedMessageAndWaitFar
	mov r0, r9
	mov r1, r10
	bl Func_08108068
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_080d26d8
	adds r0, r6, #0
	bl Item_Get
	ldrb r2, [r0, #3]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_080d269e
	adds r0, r6, #0
	movs r1, #2
	bl UiText_DrawQuantity
	adds r0, r7, #4
	b .L_080d2706
.L_080d269e:
	adds r0, r6, #0
	movs r1, #2
	bl UiText_DrawQuantity
	adds r0, r7, #1
	movs r1, #5
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r0, #1
	bl Func_080d295c
	adds r5, r0, #0
	bl UiWork_FinalizePendingCoreFar
	cmp r5, #0
	bne .L_080d2666
	movs r1, #1
	adds r0, r6, #0
	bl Item_AdjustCounterFar
	adds r0, r6, #0
	movs r1, #2
	bl UiText_DrawQuantity
	adds r0, r7, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080d2772
.L_080d26d8:
	ldr r0, [sp, #8]
	bl Owner_GetState
	ldr r1, [sp, #4]
	ldr r0, [sp, #8]
	bl Inventory_GetQuantityFar
	adds r1, r6, #0
	adds r5, r0, #0
	ldr r0, [sp, #8]
	bl Inventory_CountItemFar
	cmp r0, #29
	ble .L_080d270e
	ldr r0, [sp, #8]
	movs r1, #1
	bl UiText_DrawQuantity
	adds r0, r6, #0
	movs r1, #2
	bl UiText_DrawQuantity
	adds r0, r7, #7
.L_080d2706:
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080d2666
.L_080d270e:
	cmp r5, #0
	ble .L_080d2720
.L_080d2712:
	ldr r0, [sp, #8]
	ldr r1, [sp, #4]
	subs r5, #1
	bl Inventory_DiscardFar
	cmp r5, #0
	bne .L_080d2712
.L_080d2720:
	ldr r0, [sp, #8]
	bl Func_080ad288
	ldr r0, [sp, #8]
	bl Owner_RecalculateStatsFar
	adds r0, r6, #0
	bl PartyInventory_AddFar
	mov r8, r0
	movs r0, #83
	bl Audio_PlayCue
	ldr r3, .L_080d27e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r3, [r3]
	cmp r8, r3
	bne .L_080d275a
	adds r0, r6, #0
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r0, .L_080d27d8
	movs r1, #3
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080d2772
.L_080d275a:
	adds r0, r6, #0
	movs r1, #2
	bl UiText_DrawQuantity
	mov r0, r8
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_080d27e8
	movs r1, #3
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080d2772:
	mov r2, sp
	movs r3, #226
	ldrh r2, [r2]
	lsls r3, r3, #1
	add r3, r11
	strh r2, [r3]
	mov r0, r8
	b .L_080d27c8
.L_080d2782:
	movs r0, #83
	bl Audio_PlayCue
	adds r0, r6, #0
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r5, .L_080d27d8
	movs r1, #3
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWaitFar
	ldr r3, .L_080d27e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r3, [r3]
	cmp r8, r3
	beq .L_080d27c0
	adds r0, r6, #0
	movs r1, #2
	bl UiText_DrawQuantity
	mov r0, r8
	movs r1, #1
	bl UiText_DrawQuantity
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080d27c0:
	mov r3, sp
	ldrh r3, [r3]
	mov r0, r8
	strh r3, [r7]
.L_080d27c8:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d27d8:
	.4byte 0x00000e11
.L_080d27dc:
	.4byte 0x00000e20
.L_080d27e0:
	.4byte 0x00000e21
.L_080d27e4:
	.4byte gPartyState
.L_080d27e8:
	.4byte 0x00000e12
