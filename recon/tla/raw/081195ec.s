.syntax unified
	.thumb
	.global Func_081195ec
	.thumb_func
Func_081195ec:
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
	sub sp, #40
	movs r0, #1
	movs r1, #0
	str r3, [sp, #8]
	bl Func_0811a24c
	cmp r0, #0
	beq .L_081196ea
	mov r1, sp
	adds r1, #28
	movs r0, #1
	str r1, [sp, #4]
	bl BattleParty_ListActorIds
	mov r2, sp
	adds r2, #16
	mov r8, r0
	str r0, [sp, #12]
	adds r0, r2, #0
	str r2, [sp, #0]
	bl BattleParty_PrepareReserveOwners
	cmp r8, r0
	ble .L_08119632
	mov r8, r0
.L_08119632:
	ldr r0, [sp, #4]
	movs r1, #4
	bl BattlePres_SetActorModes
	movs r0, #32
	bl WaitFrames
	mov r3, r8
	cmp r3, #0
	ble .L_081196a6
	ldr r3, .L_081196f8
	movs r1, #134
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r2, #88
	movs r3, #0
	mov r11, r1
	mov r10, r2
	mov r9, r3
	mov r7, r8
.L_0811965a:
	ldr r3, [sp, #8]
	mov r1, r10
	ldrsh r0, [r1, r3]
	ldr r2, [sp, #0]
	mov r1, r9
	ldrh r6, [r1, r2]
	bl GetBattleObjectSlot
	adds r5, r0, #0
	adds r0, r6, #0
	bl GetBattleObjectSlot
	ldr r3, [r0]
	ldr r4, [r5]
	mov r12, r3
	ldr r3, [r5, #12]
	subs r7, #1
	str r3, [r0, #12]
	ldr r3, [r5, #16]
	str r3, [r0, #16]
	mov r0, r12
	ldr r1, [r4, #8]
	ldr r2, [r4, #12]
	ldr r3, [r4, #16]
	bl Object_SetPositionAndResetMotionFar
	mov r1, r11
	strb r6, [r1]
	ldr r1, [sp, #8]
	movs r2, #1
	mov r3, r10
	add r11, r2
	movs r2, #2
	strh r6, [r3, r1]
	add r10, r2
	add r9, r2
	cmp r7, #0
	bne .L_0811965a
.L_081196a6:
	mov r3, r8
	cmp r3, #0
	ble .L_081196ca
	ldr r1, [sp, #12]
	ldr r3, .L_081196f8
	ldr r0, [sp, #4]
	adds r3, r1, r3
	movs r1, #134
	lsls r1, r1, #2
	adds r2, r3, r1
	mov r7, r8
.L_081196bc:
	ldrh r3, [r0]
	subs r7, #1
	strb r3, [r2]
	adds r0, #2
	adds r2, #1
	cmp r7, #0
	bne .L_081196bc
.L_081196ca:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #65
	ldrb r0, [r3]
	bl UiWindow_DrawPartyStatusContentsFar
	bl BattleActor_CommitPlacement
	movs r1, #6
	ldr r0, [sp, #0]
	bl BattlePres_SetActorModes
	movs r0, #32
	bl WaitFrames
.L_081196ea:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081196f8:
	.4byte gPartyState
