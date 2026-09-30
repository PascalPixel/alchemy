.syntax unified
	.thumb
	.global Func_081284c0
	.thumb_func
Func_081284c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r0, #170
	lsls r0, r0, #3
	adds r0, r3, r0
	sub sp, #64
	str r0, [sp, #24]
	ldr r0, [r0, #4]
	cmp r0, #0
	beq .L_081284f4
	movs r1, #5
	bl UiText_DrawQuantity
	ldr r0, .L_08128790
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_081284f4:
	mov r2, sp
	adds r2, #28
	adds r1, r2, #0
	movs r0, #1
	str r2, [sp, #16]
	bl BattleParty_ListLivingUnits
	str r0, [sp, #20]
	ldr r1, [sp, #16]
	movs r0, #1
	bl Func_0811a24c
	str r0, [sp, #12]
	movs r0, #166
	lsls r0, r0, #1
	bl Runtime_BumpAllocateAlternatePool
	mov r11, r0
	ldr r0, [sp, #12]
	movs r3, #0
	str r3, [sp, #8]
	cmp r3, r0
	blt .L_08128524
	b .L_081286ec
.L_08128524:
	movs r2, #48
	str r3, [sp, #0]
	add r2, sp
	mov r8, r2
.L_0812852c:
	ldr r0, [sp, #16]
	ldr r3, [sp, #0]
	ldrh r3, [r3, r0]
	adds r0, r3, #0
	str r3, [sp, #4]
	bl Owner_GetState
	ldr r2, [sp, #8]
	ldr r3, [sp, #20]
	mov r10, r0
	cmp r2, r3
	blt .L_08128558
	ldr r0, [sp, #24]
	movs r1, #146
	lsls r1, r1, #1
	add r1, r10
	ldr r2, [r0, #4]
	ldr r3, [r1]
	lsrs r2, r2, #1
	adds r3, r3, r2
	str r3, [r1]
	b .L_081286bc
.L_08128558:
	ldr r0, [sp, #24]
	movs r3, #146
	lsls r3, r3, #1
	add r3, r10
	ldr r2, [r3]
	ldr r1, [r0, #4]
	adds r2, r2, r1
	str r2, [r3]
	b .L_081286bc
.L_0812856a:
	movs r0, #89
	bl Audio_PlayCue
	bl UiWork_ClearValueNameTablesFar
	movs r3, #42
	adds r3, #255
	add r3, r10
	ldrb r0, [r3]
	movs r1, #3
	bl UiText_DrawQuantity
	ldr r2, [sp, #0]
	ldr r3, [sp, #16]
	movs r1, #1
	ldrh r0, [r2, r3]
	bl UiText_DrawQuantity
	mov r2, r10
	ldrb r0, [r2, #15]
	movs r1, #5
	bl UiText_DrawQuantity
	ldr r0, .L_08128794
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	mov r6, r10
	mov r9, r3
	adds r6, #88
	movs r7, #31
.L_081285b0:
	ldrh r5, [r6]
	mov r0, r9
	adds r3, r5, #0
	ands r3, r0
	adds r6, #4
	cmp r3, #0
	beq .L_0812861a
	lsrs r3, r5, #15
	cmp r3, #0
	beq .L_0812861a
	movs r3, #88
	mov r2, r11
	ldrh r3, [r2, r3]
	movs r1, #0
	cmp r5, r3
	beq .L_081285e0
	adds r2, #88
.L_081285d2:
	adds r1, #1
	cmp r1, #31
	bgt .L_081285e0
	adds r2, #4
	ldrh r3, [r2]
	cmp r5, r3
	bne .L_081285d2
.L_081285e0:
	cmp r1, #32
	bne .L_0812861a
	bl UiWork_ClearValueNameTablesFar
	movs r3, #42
	adds r3, #255
	add r3, r10
	ldrb r0, [r3]
	movs r1, #3
	bl UiText_DrawQuantity
	movs r1, #1
	ldr r0, [sp, #4]
	bl UiText_DrawQuantity
	mov r3, r9
	ands r5, r3
	movs r1, #4
	adds r0, r5, #0
	bl UiText_DrawQuantity
	movs r0, #154
	bl Audio_PlayCue
	ldr r0, .L_08128798
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_0812861a:
	subs r7, #1
	cmp r7, #0
	bge .L_081285b0
	mov r3, r8
	movs r2, #4
	ldrsh r0, [r3, r2]
	cmp r0, #0
	beq .L_0812863a
	movs r1, #5
	bl UiText_DrawQuantity
	ldr r0, .L_0812879c
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_0812863a:
	mov r3, r8
	movs r2, #6
	ldrsh r0, [r3, r2]
	cmp r0, #0
	beq .L_08128654
	movs r1, #5
	bl UiText_DrawQuantity
	ldr r0, .L_081287a0
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_08128654:
	mov r3, r8
	movs r2, #8
	ldrsh r0, [r3, r2]
	cmp r0, #0
	beq .L_0812866e
	movs r1, #5
	bl UiText_DrawQuantity
	ldr r0, .L_081287a4
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_0812866e:
	mov r3, r8
	movs r2, #10
	ldrsh r0, [r3, r2]
	cmp r0, #0
	beq .L_08128688
	movs r1, #5
	bl UiText_DrawQuantity
	ldr r0, .L_081287a8
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_08128688:
	mov r3, r8
	movs r2, #12
	ldrsh r0, [r3, r2]
	cmp r0, #0
	beq .L_081286a2
	movs r1, #5
	bl UiText_DrawQuantity
	ldr r0, .L_081287ac
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_081286a2:
	mov r3, r8
	movs r2, #14
	ldrsh r0, [r3, r2]
	cmp r0, #0
	beq .L_081286bc
	movs r1, #5
	bl UiText_DrawQuantity
	ldr r0, .L_081287b0
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_081286bc:
	movs r2, #166
	mov r1, r10
	ldr r3, .L_081287b4
	mov r0, r11
	lsls r2, r2, #1
	mov lr, r3
	.2byte 0xf800
	mov r1, r8
	ldr r0, [sp, #4]
	bl Func_080ad0b0
	cmp r0, #0
	beq .L_081286d8
	b .L_0812856a
.L_081286d8:
	ldr r0, [sp, #0]
	ldr r2, [sp, #8]
	ldr r3, [sp, #12]
	adds r0, #2
	adds r2, #1
	str r0, [sp, #0]
	str r2, [sp, #8]
	cmp r2, r3
	bge .L_081286ec
	b .L_0812852c
.L_081286ec:
	mov r0, r11
	bl Sys_Free
	ldr r2, [sp, #24]
	ldr r0, [r2]
	cmp r0, #0
	beq .L_08128712
	movs r1, #5
	bl UiText_DrawQuantity
	ldr r0, .L_081287b8
	bl UiText_ShowMessageAndWaitCoreFar
	ldr r3, [sp, #24]
	ldr r0, [r3]
	bl Party_AdjustSixDigitCounterAFar
	bl BattlePresentation_WaitForAdvance
.L_08128712:
	ldr r0, .L_081287bc
	mov r10, r0
.L_08128716:
	movs r7, #1
	negs r7, r7
	mov r8, r7
	movs r6, #0
	movs r5, #12
.L_08128720:
	ldr r2, [sp, #24]
	ldrh r3, [r5, r2]
	cmp r3, #0
	beq .L_08128736
	adds r0, r3, #0
	bl Item_EncodeBankedId
	cmp r0, r8
	blt .L_08128736
	mov r8, r0
	adds r7, r6, #0
.L_08128736:
	adds r6, #1
	adds r5, #2
	cmp r6, #3
	ble .L_08128720
	movs r6, #1
	negs r6, r6
	cmp r7, r6
	beq .L_0812877e
	lsls r3, r7, #1
	adds r5, r3, #0
	ldr r3, [sp, #24]
	adds r5, #12
	ldrh r0, [r3, r5]
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r0, .L_081287c0
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
	ldr r2, [sp, #24]
	ldrh r0, [r2, r5]
	bl PartyInventory_AddFar
	cmp r0, r6
	bne .L_08128776
	ldr r0, [sp, #24]
	mov r2, r10
	ldrh r3, [r0, r5]
	strh r3, [r2]
	b .L_0812877e
.L_08128776:
	ldr r3, .L_0812878c
	ldr r0, [sp, #24]
	strh r3, [r0, r5]
	b .L_08128716
.L_0812877e:
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0812878c:
	.4byte 0x00000000
.L_08128790:
	.4byte 0x00000c86
.L_08128794:
	.4byte 0x00000cfa
.L_08128798:
	.4byte 0x00000cfb
.L_0812879c:
	.4byte 0x00000cfc
.L_081287a0:
	.4byte 0x00000cfd
.L_081287a4:
	.4byte 0x00000cfe
.L_081287a8:
	.4byte 0x00000cff
.L_081287ac:
	.4byte 0x00000d00
.L_081287b0:
	.4byte 0x00000d01
.L_081287b4:
	.4byte IwramCopyWords
.L_081287b8:
	.4byte 0x00000c87
.L_081287bc:
	.4byte Data_0200049c
.L_081287c0:
	.4byte 0x00000c88
