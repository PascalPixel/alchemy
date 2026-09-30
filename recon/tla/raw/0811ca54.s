.syntax unified
	.thumb
	.global Func_0811ca54
	.thumb_func
Func_0811ca54:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	adds r5, r0, #0
	ldr r2, [r3]
	movs r1, #0
	ldrsh r3, [r5, r1]
	sub sp, #96
	ldr r1, .L_0811cbe4
	cmp r3, #4
	bgt .L_0811ca76
	movs r1, #128
	lsls r1, r1, #6
.L_0811ca76:
	ldr r3, [r2]
	cmp r3, r1
	bne .L_0811ca88
	movs r3, #40
	str r3, [r2, #4]
	movs r0, #40
	bl WaitFrames
	b .L_0811ca94
.L_0811ca88:
	movs r3, #40
	str r1, [r2]
	str r3, [r2, #4]
	movs r0, #40
	bl WaitFrames
.L_0811ca94:
	movs r2, #8
	ldrsh r3, [r5, r2]
	add r6, sp, #8
	str r3, [r6]
	movs r1, #12
	ldrsh r3, [r5, r1]
	movs r2, #0
	ldrsh r0, [r5, r2]
	str r3, [r6, #16]
	movs r1, #10
	ldrsh r3, [r5, r1]
	str r0, [r6, #8]
	str r3, [r6, #12]
	bl BattleObject_IsValidId
	cmp r0, #0
	bge .L_0811cabc
	movs r0, #1
	negs r0, r0
	b .L_0811cbd8
.L_0811cabc:
	ldr r3, [r6, #12]
	cmp r3, #127
	ble .L_0811cac8
	add r7, sp, #44
	movs r0, #2
	b .L_0811cacc
.L_0811cac8:
	add r7, sp, #44
	movs r0, #1
.L_0811cacc:
	adds r1, r7, #0
	bl BattleParty_ListLivingUnits
	str r0, [r6, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #65
	ldrb r0, [r3]
	movs r3, #2
	negs r3, r3
	ands r0, r3
	bl UiWindow_DrawPartyStatusContentsFar
	ldr r0, [r6, #8]
	bl GetBattleObjectSlot
	ldr r0, [r0]
	movs r1, #3
	mov r10, r0
	bl Object_SetMode
	mov r0, r10
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	ldrh r3, [r5, #10]
	cmp r3, #7
	bhi .L_0811cb18
	movs r2, #1
	mov r8, r2
	str r2, [r6, #4]
	movs r0, #1
	adds r1, r7, #0
	bl BattleParty_ListLivingUnits
	mov r3, r8
	b .L_0811cb26
.L_0811cb18:
	movs r3, #0
	str r3, [r6, #4]
	movs r0, #2
	adds r1, r7, #0
	bl BattleParty_ListLivingUnits
	movs r3, #1
.L_0811cb26:
	str r3, [r6, #20]
	ldr r3, [r6, #20]
	movs r7, #0
	adds r2, r6, #0
	cmp r3, #0
	beq .L_0811cb72
	movs r5, #0
.L_0811cb34:
	lsls r3, r7, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlot
	ldr r3, [r0]
	movs r0, #0
	ldr r1, [r3, #80]
	ldrb r3, [r1, #27]
	subs r3, #1
	cmp r3, #0
	beq .L_0811cb66
	add r2, sp, #96
	mov r12, r3
	adds r3, r2, r5
	adds r2, r3, #0
	subs r2, #34
	adds r1, #40
.L_0811cb58:
	ldmia r1!, {r3}
	adds r0, #1
	ldrb r3, [r3, #5]
	strb r3, [r2]
	adds r2, #1
	cmp r0, r12
	bne .L_0811cb58
.L_0811cb66:
	ldr r3, [r6, #20]
	adds r7, #1
	adds r5, #4
	adds r2, r6, #0
	cmp r7, r3
	bne .L_0811cb34
.L_0811cb72:
	movs r7, #0
	adds r0, r6, #0
	str r7, [r6]
	str r7, [r6, #24]
	bl Func_08138020
	movs r3, #1
	str r3, [r6]
	adds r0, r6, #0
	bl Func_08138020
	movs r3, #2
	str r3, [r6]
	adds r0, r6, #0
	bl Func_08138020
	movs r3, #3
	str r3, [r6]
	adds r0, r6, #0
	bl Func_08138020
	adds r0, r6, #0
	str r7, [r6]
	bl Func_08138018
	mov r0, r10
	movs r1, #1
	bl Object_SetMode
	add r5, sp, #8
	ldr r3, [r5, #20]
	adds r2, r5, #0
	cmp r3, #0
	beq .L_0811cbd0
	movs r6, #36
.L_0811cbb8:
	str r2, [sp, #4]
	str r2, [sp, #0]
	ldrsh r0, [r2, r6]
	bl Actor_ResetMotionAtAnchor
	ldr r1, [sp, #4]
	adds r7, #1
	ldr r3, [r1, #20]
	adds r6, #2
	ldr r2, [sp, #0]
	cmp r7, r3
	bne .L_0811cbb8
.L_0811cbd0:
	ldr r0, [r5, #8]
	bl Actor_ResetMotionAtAnchor
	movs r0, #0
.L_0811cbd8:
	add sp, #96
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811cbe4:
	.4byte 0xffffe000
