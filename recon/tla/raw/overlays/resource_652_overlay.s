.syntax unified
	.thumb
	.section .text.x02008704,"ax",%progbits
	.balign 4
	.global Func_02000704
	.thumb_func
Func_02000704:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, .L_0200872c
	ldr r5, [r3, #108]
	bl Func_02000dcc
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #188
	adds r5, r5, r3
	ldr r1, [r5]
	movs r3, #1
	adds r1, #35
	ldrb r2, [r1]
	orrs r3, r2
	movs r2, #253
	ands r3, r2
	strb r3, [r1]
	pop {r5, pc}
.L_0200872c:
	.4byte Data_02000e04
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
	bl UiText_ShowPositionedMessageAndWait
	ldr r0, .L_02008a40
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
.L_020088c4:
	ldr r2, .L_02008a44
	movs r1, #1
	mov r8, r2
	mov r0, r8
	bl UiText_ShowPositionedMessageAndWait
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
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	bl Func_02000dbc
	adds r5, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r5, #0
	bne .L_020088c4
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	mov r0, r8
	adds r0, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r3, #226
	lsls r3, r3, #1
	add r3, r10
	mov r2, r9
	strh r2, [r3]
	b .L_02008a2c
.L_02008940:
	ldr r0, [sp, #4]
	bl UnitGet
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
	bl UiText_ShowPositionedMessageAndWait
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
	bl Owner_RefreshClassActions
	ldr r0, [sp, #4]
	bl UnitCalc
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
	bl UiText_ShowPositionedMessageAndWait
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
	bl UiText_ShowPositionedMessageAndWait
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
	bl UiText_ShowPositionedMessageAndWait
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
	bl UiText_ShowPositionedMessageAndWait
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
	.4byte gPartyState
.L_02008a4c:
	.4byte MsgItemGiven
	.section .rodata.x02008e04,"a",%progbits
	.global Data_02000e04
Data_02000e04:
	.4byte 0x0220000a
	.4byte 0x0221000b
	.4byte 0x0222000c
	.4byte 0x0000ffff
.L_02008e14:
	.4byte 0x00000016
	.4byte 0x0000001e
	.ifdef TLA_EDITION_JA
	.4byte 0x00000083
	.else
	.4byte 0x00000081
	.endif
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
.L_02008ecc:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.ifdef TLA_EDITION_JA
	.4byte 0x00000083
	.else
	.4byte 0x00000081
	.endif
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000000a
	.4byte 0x00020000
	.4byte 0x00020000
	.4byte 0x00060000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200116c
Data_0200116c:
	.4byte 0x0000000b
	.4byte 0x1010100a
	.4byte 0xffffffff
	.4byte 0x1020200a
	.4byte 0xffffffff
	.4byte 0x1030300a
	.4byte 0xffffffff
	.4byte 0x1040400a
	.4byte 0xffffffff
	.4byte 0x1050500a
	.4byte 0xffffffff
	.4byte 0x1060600a
	.4byte 0xffffffff
	.4byte 0x10703085
	.4byte 0xffffffff
	.4byte 0x0000000c
	.4byte 0x1010700a
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_020011b8
Data_020011b8:
	.4byte 0x0000000b
	.4byte 0x1010100d
	.4byte 0xffffffff
	.4byte 0x1020200d
	.4byte 0xffffffff
	.4byte 0x1030300d
	.4byte 0xffffffff
	.4byte 0x1040400d
	.4byte 0xffffffff
	.4byte 0x1050500d
	.4byte 0xffffffff
	.4byte 0x1060600d
	.4byte 0xffffffff
	.4byte 0x10703085
	.4byte 0xffffffff
	.4byte 0x0000000c
	.4byte 0x1010700d
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02001204
Data_02001204:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte .L_02008e14
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00018000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00012000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0xffff0071
	.4byte 0x00000002
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x0001e000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00016000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d40000
	.4byte 0x00014000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0xffff0087
	.4byte 0x00000002
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00012000
	.4byte 0xffff004a
	.4byte 0x00000003
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x0001c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00018000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020013fc
Data_020013fc:
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x014d0000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00024000
	.4byte 0x02230122
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x02240122
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x02250122
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020014a4
Data_020014a4:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte .L_02008e14
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00018000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00012000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0xffff0071
	.4byte 0x00000002
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001e000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00016000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d40000
	.4byte 0x00014000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0xffff0087
	.4byte .L_02008ecc
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00012000
	.4byte 0xffff0012
	.4byte 0x00000001
	.4byte 0x00a10000
	.4byte 0x00000000
	.4byte 0x006f0000
	.4byte 0x00014000
	.4byte 0xffff0011
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte .L_02008ecc
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000003
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x0001c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00018000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020016e4
Data_020016e4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgFieldRikisReallyInForItWhen
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgFieldThatNoGoodKidOfMine
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFieldIWonderWhereOurLittleTavi
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgFieldTaviNeverMissesThreeOclockSnacksies
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgFieldIfYoureLookingForABoat
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldIfYouHeadSouthYoullFind
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgFieldInAllMyYearsAsMayor
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldTheWaveWasAwfulButThe
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldALotOfFolkSaidThey
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldBetweenThePiratesAndTheTidal
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000270
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_020002d0
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte Func_02000330
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgFieldWeRunAVerySmallInn
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgFieldIDontKnowWhenIllBe
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgFieldIHearTheSeafoodInMadra
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgSeaOfTimeCurrents
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgSeaOfTimeRedRocks
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte MsgSeaOfTimeThreeGenerations
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte Func_020002d0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldRikiTaviWhereAreYouWhat
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgFieldThatTavisABadInfluenceOn
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgFieldIllBetThatWhereverTaviIs
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFieldIfTaviIsntHomeSoonWere
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgFieldIWonderHowTheRoadTo
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldIWonderIfTheHolyMan
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldAtLeastNobodySeemsToHave
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldIStillDontKnowWhatThat
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldWeShouldFindADifferentWay
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldNowThatBriggsIsLockedAway
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldImGladImNotGoingTo
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgDeriHeyaScamps
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFieldWeCantSailAndWeCant
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFieldLookAtAllTheseGuestsWe
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFieldAfterAllThatShakingIWouldnt
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgFieldImSoTornDoILike
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgSeaOfTimeHusbandDizzy
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgSeaOfTimeYeppToldMe
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgSeaOfTimeGiveUp
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte DeriHeya_TalkScamps
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001924
Data_02001924:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000390
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldTravelersWithNowhereToGoThis
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001990
Data_02001990:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgFieldLetMeTellYouKidsAre
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgFieldWhoStaysOutsideUntilHesAbsolutely
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFieldOurLittleOneIsFinallyHome
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgFieldTaviWolfedDownHisSnackAnd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_02000614
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldIThinkMadraIsEastOf
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_0200066c
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldItsTooDangerousToGoTo
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldWhoWasThatGuyIWonder
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldWhyDidThePirateBriggsPlague
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_020004f4
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_02000554
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgDeriHeyaDinnerShowPractice
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgDeriHeyaSeafoodWhen
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgDeriHeyaVegetablesAgain
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgFieldMmmMmnphDadWasSoMad
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgFieldMmmphMmmmDontTellAnyoneWe
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_020006c4
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte MsgSeaOfTimeCurrents
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgSeaOfTimeRedRocks
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgSeaOfTimeThreeGenerations
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte Func_020002d0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldDarnThatRikiAhImGlad
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgFieldHeeHeeMyRikiIsMommys
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgFieldIGuessRikiNextDoorMade
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFieldOnceTaviFinishesEatingImPutting
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgFieldDailaGotHitPrettyHardI
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldSomeoneShouldSeeHowTheRoad
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldHeSaidHedPayMeA
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldTheRoadToMadraGoesThrough
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldMaybeThatGuyWashedUpOn
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldBriggsProbablyHasLotsOfMen
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldThisTownMayBeDirtPoor
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgDeriHeyaMazeTreasure
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgDeriHeyaTiredPerformer
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgDeriHeyaDragonKingLine
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgDeriHeyaBadShows
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgDeriHeyaGuestDance
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgFieldIStillWantToCatchThat
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgFieldImSureGladIMadeIt
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgDeriHeyaSeafoodStory
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgSeaOfTimeHusbandDizzy
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgSeaOfTimeYeppToldMe
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgSeaOfTimeGiveUp
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte DeriHeya_TalkMazeTreasure
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001c18
Data_02001c18:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00004602
	.4byte 0xffff001e
	.4byte Func_02000c74
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte Func_02000c74
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte Func_02000c74
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte Func_02000c74
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000498
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgDeriHeyaBoringVillage
	.4byte 0x00001815
	.4byte 0x0220000a
	.4byte Func_02000704
	.4byte 0x00001815
	.4byte 0x0221000b
	.4byte Func_02000704
	.4byte 0x00001815
	.4byte 0x0222000c
	.4byte Func_02000704
	.4byte 0x00000003
	.4byte 0x0a200028
	.4byte Func_02000730
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
