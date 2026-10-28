.syntax unified
	.thumb
	.section .text.x02008878,"ax",%progbits
	.balign 4
	.global Func_02000878
	.thumb_func
Func_02000878:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #226
	mov r10, r3
	lsls r2, r2, #1
	add r2, r10
	mov r8, r2
	movs r3, #0
	ldrsh r2, [r2, r3]
	sub sp, #8
	adds r6, r0, #0
	mov r9, r2
	bl PartyInventory_Add
	movs r3, #1
	adds r7, r0, #0
	negs r3, r3
	cmp r7, r3
	beq .L_020088ac
	b .L_020089e8
.L_020088ac:
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	ldr r0, .L_02008a3c
	movs r1, #1
	bl FieldMessage_Show
	ldr r0, .L_02008a40
	movs r1, #1
	bl FieldMessage_Show
.L_020088c4:
	ldr r2, .L_02008a44
	movs r1, #1
	mov r8, r2
	mov r0, r8
	bl FieldMessage_Show
	add r0, sp, #4
	mov r1, sp
	bl Func_02000df4
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_02008940
	adds r0, r6, #0
	bl Func_02000ce4
	ldrb r2, [r0, #3]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_02008900
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	mov r0, r8
	adds r0, #4
	b .L_02008970
.L_02008900:
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	mov r0, r8
	adds r0, #1
	movs r1, #5
	bl FieldMessage_Show
	movs r0, #1
	bl Func_02000dbc
	adds r5, r0, #0
	bl Message_CloseFinishedWindows
	cmp r5, #0
	bne .L_020088c4
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	mov r0, r8
	adds r0, #2
	movs r1, #1
	bl FieldMessage_Show
	movs r3, #226
	lsls r3, r3, #1
	add r3, r10
	mov r2, r9
	strh r2, [r3]
	b .L_02008a2c
.L_02008940:
	ldr r0, [sp, #4]
	bl Unit_Get
	ldr r1, [sp, #0]
	ldr r0, [sp, #4]
	bl Inventory_GetQuantity
	adds r1, r6, #0
	adds r5, r0, #0
	ldr r0, [sp, #4]
	bl Inventory_CountItem
	cmp r0, #29
	ble .L_02008978
	ldr r0, [sp, #4]
	movs r1, #1
	bl Func_02000ccc
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	mov r0, r8
	adds r0, #7
.L_02008970:
	movs r1, #1
	bl FieldMessage_Show
	b .L_020088c4
.L_02008978:
	cmp r5, #0
	ble .L_0200898a
.L_0200897c:
	ldr r0, [sp, #4]
	ldr r1, [sp, #0]
	subs r5, #1
	bl Inventory_Discard
	cmp r5, #0
	bne .L_0200897c
.L_0200898a:
	ldr r0, [sp, #4]
	bl Unit_RefreshPsynergy
	ldr r0, [sp, #4]
	bl Unit_Calculate
	adds r0, r6, #0
	bl PartyInventory_Add
	adds r7, r0, #0
	movs r0, #83
	bl Func_02000dfc
	ldr r3, .L_02008a48
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r3, [r3]
	cmp r7, r3
	bne .L_020089c4
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	ldr r0, .L_02008a3c
	movs r1, #3
	bl FieldMessage_Show
	b .L_020089dc
.L_020089c4:
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	adds r0, r7, #0
	movs r1, #1
	bl Func_02000ccc
	ldr r0, .L_02008a4c
	movs r1, #3
	bl FieldMessage_Show
.L_020089dc:
	movs r3, #226
	lsls r3, r3, #1
	add r3, r10
	mov r2, r9
	strh r2, [r3]
	b .L_02008a2c
.L_020089e8:
	movs r0, #83
	bl Func_02000dfc
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	ldr r5, .L_02008a3c
	movs r1, #3
	adds r0, r5, #0
	bl FieldMessage_Show
	ldr r3, .L_02008a48
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r3, [r3]
	cmp r7, r3
	beq .L_02008a26
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	adds r0, r7, #0
	movs r1, #1
	bl Func_02000ccc
	adds r0, r5, #1
	movs r1, #1
	bl FieldMessage_Show
.L_02008a26:
	mov r3, r9
	mov r2, r8
	strh r3, [r2]
.L_02008a2c:
	adds r0, r7, #0
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a3c:
	.4byte MsgItemReceived
.L_02008a40:
	.4byte MsgItemCannotCarry
.L_02008a44:
	.4byte MsgItemDropPrompt
.L_02008a48:
	.4byte gMainRoutineWork
.L_02008a4c:
	.4byte MsgItemGiven
