.syntax unified
	.thumb
	.global BattleUnit_ProcessTurnEnd
	.thumb_func
BattleUnit_ProcessTurnEnd:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #32
	str r0, [sp, #12]
	ldrb r0, [r0]
	movs r1, #0
	mov r8, r0
	str r1, [sp, #4]
	bl Owner_GetStateFar
	mov r2, r8
	str r0, [sp, #8]
	movs r0, #0
	cmp r2, #7
	bls .L_080bfbce
	movs r0, #1
.L_080bfbce:
	bl Trade_GetOfferStateFar
	adds r3, r0, #0
	movs r0, #132
	lsls r0, r0, #1
	adds r6, r3, #0
	adds r3, r3, r0
	ldr r3, [r3]
	ldr r1, [sp, #4]
	adds r6, #8
	movs r7, #0
	cmp r1, r3
	bge .L_080bfc18
	movs r2, #1
	negs r2, r2
	mov r10, r2
	adds r5, r6, #0
.L_080bfbf0:
	ldrb r3, [r5, #2]
	cmp r3, r8
	bne .L_080bfc08
	movs r3, #3
	ldrsb r3, [r5, r3]
	cmp r3, r10
	bne .L_080bfc08
	ldrb r1, [r5]
	ldrb r2, [r5, #1]
	mov r0, r8
	bl Djinn_DeactivateFar
.L_080bfc08:
	movs r0, #128
	lsls r0, r0, #1
	adds r3, r6, r0
	ldr r3, [r3]
	adds r7, #1
	adds r5, #4
	cmp r7, r3
	blt .L_080bfbf0
.L_080bfc18:
	movs r0, #1
	movs r1, #0
	bl BattleParty_ListLivingUnits
	cmp r0, #0
	beq .L_080bfc34
	movs r0, #2
	movs r1, #0
	bl BattleParty_ListLivingUnits
	cmp r0, #0
	beq .L_080bfc34
	movs r1, #1
	str r1, [sp, #4]
.L_080bfc34:
	mov r2, r8
	movs r0, #0
	cmp r2, #7
	bls .L_080bfc3e
	movs r0, #1
.L_080bfc3e:
	bl Trade_GetOfferStateFar
	mov r3, sp
	adds r3, #16
	str r3, [sp, #0]
	adds r6, r0, #0
	ldr r0, [sp, #0]
	adds r6, #8
	movs r2, #0
	add r3, sp, #28
	mov r12, r0
.L_080bfc54:
	str r2, [r3]
	subs r3, #4
	cmp r3, r12
	bge .L_080bfc54
	movs r3, #128
	lsls r3, r3, #1
	movs r1, #2
	adds r7, r6, r3
	negs r1, r1
	mov r9, r1
	mov r11, r7
.L_080bfc6a:
	movs r2, #1
	ldr r3, [r7]
	negs r2, r2
	movs r4, #0
	mov r12, r2
	cmp r4, r3
	bge .L_080bfc9e
	movs r3, #3
	ldrsb r3, [r6, r3]
	cmp r3, r9
	bne .L_080bfc86
	ldrb r3, [r6, #2]
	mov r12, r3
	b .L_080bfc9e
.L_080bfc86:
	ldr r3, [r7]
	adds r4, #1
	cmp r4, r3
	bge .L_080bfc9e
	lsls r3, r4, #2
	adds r2, r6, r3
	movs r3, #3
	ldrsb r3, [r2, r3]
	cmp r3, r9
	bne .L_080bfc86
	ldrb r2, [r2, #2]
	mov r12, r2
.L_080bfc9e:
	movs r3, #1
	negs r3, r3
	cmp r12, r3
	beq .L_080bfd18
	adds r5, r3, #0
	ldr r3, [r7]
	cmp r3, #0
	ble .L_080bfccc
	mov r0, r11
	ldr r4, [r0]
	adds r2, r6, #0
.L_080bfcb4:
	ldrb r3, [r2, #2]
	cmp r3, r12
	bne .L_080bfcc4
	movs r3, #3
	ldrsb r3, [r2, r3]
	cmp r3, r5
	ble .L_080bfcc4
	adds r5, r3, #0
.L_080bfcc4:
	subs r4, #1
	adds r2, #4
	cmp r4, #0
	bne .L_080bfcb4
.L_080bfccc:
	adds r5, #1
	cmp r5, #1
	bgt .L_080bfcd4
	movs r5, #2
.L_080bfcd4:
	ldr r3, [r7]
	movs r4, #0
	cmp r4, r3
	bge .L_080bfc6a
	movs r2, #128
	movs r1, #2
	lsls r2, r2, #1
	negs r1, r1
	adds r2, r2, r6
	ldr r0, [sp, #0]
	mov r10, r1
	mov lr, r2
	adds r1, r6, #0
.L_080bfcee:
	ldrb r3, [r1, #2]
	cmp r3, r12
	bne .L_080bfd0a
	movs r3, #3
	ldrsb r3, [r1, r3]
	cmp r3, r10
	bne .L_080bfd0a
	ldrb r2, [r1]
	strb r5, [r1, #3]
	lsls r2, r2, #2
	ldr r3, [r0, r2]
	adds r3, #1
	str r3, [r0, r2]
	adds r5, #1
.L_080bfd0a:
	mov r2, lr
	ldr r3, [r2]
	adds r4, #1
	adds r1, #4
	cmp r4, r3
	blt .L_080bfcee
	b .L_080bfc6a
.L_080bfd18:
	ldr r3, [sp, #4]
	cmp r3, #0
	bne .L_080bfd20
	b .L_080bff78
.L_080bfd20:
	movs r5, #166
	movs r0, #0
	lsls r5, r5, #1
	mov r10, r0
	adds r0, r5, #0
	bl Runtime_BumpAllocateAlternatePool
	adds r2, r5, #0
	ldr r3, .L_080bff90
	ldr r1, [sp, #8]
	mov r9, r0
	bl _call_via_r3
	movs r7, #1
	ldr r2, [sp, #0]
	negs r7, r7
	movs r6, #0
.L_080bfd42:
	ldmia r2!, {r3}
	cmp r3, r10
	ble .L_080bfd4c
	mov r10, r3
	adds r7, r6, #0
.L_080bfd4c:
	adds r6, #1
	cmp r6, #3
	ble .L_080bfd42
	cmp r7, #0
	blt .L_080bfd68
	movs r1, #150
	lsls r1, r1, #1
	ldr r0, [sp, #8]
	adds r2, r7, r1
	ldrsb r3, [r0, r2]
	cmp r3, r10
	bge .L_080bfd68
	mov r1, r10
	strb r1, [r0, r2]
.L_080bfd68:
	mov r0, r8
	bl Owner_RecalculateStatsFar
	movs r6, #0
	movs r7, #72
.L_080bfd72:
	ldr r3, [sp, #8]
	mov r1, r9
	ldrsh r2, [r7, r3]
	ldrsh r3, [r7, r1]
	subs r5, r2, r3
	cmp r5, #0
	ble .L_080bfde8
	bl BattleEventRuntime_Reset
	movs r0, #25
	bl BattleEventRuntime_SchedulePhase
	mov r1, r8
	movs r0, #0
	bl BattleEv_Push
	adds r1, r5, #0
	movs r0, #1
	bl BattleEv_Push
	movs r1, #175
	movs r0, #14
	bl BattleEv_Push
	ldr r1, .L_080bff94
	movs r0, #4
	adds r1, r6, r1
	bl BattleEv_Push
	mov r1, r8
	movs r0, #11
	bl BattleEv_Push
	movs r0, #212
	bl AudioCommand_PlayFar
	mov r0, r8
	bl GetBattleObjectSlot
	movs r1, #3
	ldr r0, [r0]
	bl Object_SetMode
	mov r0, r8
	bl GetBattleObjectSlot
	movs r1, #32
	ldr r0, [r0]
	bl ObjectDispatch_ApplyValueToChildrenFar
	mov r3, r10
	adds r1, r6, #0
	movs r2, #2
	subs r3, #1
	mov r0, r8
	bl BattleFx_PlayUnitElementEffect
	bl BattleEventRuntime_WaitForReady
.L_080bfde8:
	adds r6, #1
	adds r7, #4
	cmp r6, #3
	ble .L_080bfd72
	mov r0, r9
	bl Runtime_BumpFree
	ldr r1, [sp, #4]
	cmp r1, #0
	bne .L_080bfdfe
	b .L_080bff78
.L_080bfdfe:
	bl BattleEventRuntime_Reset
	ldr r2, [sp, #12]
	ldr r3, [r2, #96]
	cmp r3, #0
	beq .L_080bfe68
	mov r1, r8
	movs r0, #8
	bl BattleEv_Push
	mov r1, r8
	movs r0, #0
	bl BattleEv_Push
	ldr r3, [sp, #12]
	movs r0, #1
	ldr r1, [r3, #96]
	bl BattleEv_Push
	ldr r1, .L_080bff98
	movs r0, #4
	bl BattleEv_Push
	ldr r0, [sp, #12]
	ldr r1, [r0, #96]
	mov r0, r8
	negs r1, r1
	bl Owner_AdjustFirstValueFar
	cmp r0, #0
	bne .L_080bfe60
	mov r1, r8
	movs r0, #9
	bl BattleEv_Push
	mov r1, r8
	movs r0, #0
	bl BattleEv_Push
	mov r1, r8
	cmp r1, #7
	bhi .L_080bfe56
	ldr r1, .L_080bff9c
	b .L_080bfe58
.L_080bfe56:
	ldr r1, .L_080bffa0
.L_080bfe58:
	movs r0, #4
	bl BattleEv_Push
	b .L_080bfe68
.L_080bfe60:
	movs r0, #11
	mov r1, r8
	bl BattleEv_Push
.L_080bfe68:
	bl BattleEv_DispatchQueued
	bl BattleEventRuntime_Reset
	ldr r3, .L_080bffa4
	ldr r2, [sp, #8]
	adds r6, r2, r3
	movs r0, #0
	ldrsb r0, [r6, r0]
	cmp r0, #0
	beq .L_080bff04
	movs r1, #52
	ldrsh r3, [r2, r1]
	movs r1, #10
	muls r0, r3
	bl __divsi3
	ldr r3, .L_080bffa8
	adds r7, r0, #0
	mov r1, r8
	movs r0, #8
	ldr r5, [r3]
	bl BattleEv_Push
	mov r1, r8
	movs r0, #0
	bl BattleEv_Push
	adds r1, r7, #0
	movs r0, #1
	bl BattleEv_Push
	ldr r1, .L_080bffac
	movs r0, #4
	bl BattleEv_Push
	movs r3, #0
	ldrsb r3, [r6, r3]
	cmp r3, #0
	beq .L_080bfec2
	movs r3, #130
	lsls r3, r3, #4
	adds r2, r5, r3
	movs r3, #134
	b .L_080bfeca
.L_080bfec2:
	movs r0, #130
	lsls r0, r0, #4
	adds r2, r5, r0
	movs r3, #133
.L_080bfeca:
	str r3, [r2]
	negs r1, r7
	mov r0, r8
	bl Owner_AdjustFirstValueFar
	cmp r0, #0
	bne .L_080bfefc
	mov r1, r8
	movs r0, #9
	bl BattleEv_Push
	mov r1, r8
	movs r0, #0
	bl BattleEv_Push
	mov r1, r8
	cmp r1, #7
	bhi .L_080bfef2
	ldr r1, .L_080bff9c
	b .L_080bfef4
.L_080bfef2:
	ldr r1, .L_080bffa0
.L_080bfef4:
	movs r0, #4
	bl BattleEv_Push
	b .L_080bff04
.L_080bfefc:
	movs r0, #11
	mov r1, r8
	bl BattleEv_Push
.L_080bff04:
	bl BattleEv_DispatchQueued
	bl BattleEventRuntime_Reset
	ldr r3, .L_080bffb0
	ldr r2, [sp, #8]
	adds r1, r2, r3
	ldrb r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080bff74
	adds r3, #255
	strb r3, [r1]
	lsls r3, r3, #24
	cmp r3, #0
	bne .L_080bff74
	movs r1, #192
	lsls r1, r1, #24
	mov r0, r8
	bl Owner_AdjustFirstValueFar
	cmp r0, #0
	bne .L_080bff74
	mov r1, r8
	movs r0, #0
	bl BattleEv_Push
	ldr r5, .L_080bffb4
	movs r0, #4
	adds r1, r5, #0
	bl BattleEv_Push
	mov r1, r8
	movs r0, #8
	bl BattleEv_Push
	mov r1, r8
	movs r0, #9
	bl BattleEv_Push
	movs r0, #0
	mov r1, r8
	bl BattleEv_Push
	mov r0, r8
	cmp r0, #7
	bhi .L_080bff6c
	subs r1, r5, #3
	movs r0, #4
	bl BattleEv_Push
	b .L_080bff74
.L_080bff6c:
	adds r1, r5, #3
	movs r0, #4
	bl BattleEv_Push
.L_080bff74:
	bl BattleEv_DispatchQueued
.L_080bff78:
	mov r0, r8
	bl Owner_RecalculateStatsFar
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080bff90:
	.4byte IwramCopyWords
.L_080bff94:
	.4byte 0x00000879
.L_080bff98:
	.4byte 0x0000084b
.L_080bff9c:
	.4byte 0x00000825
.L_080bffa0:
	.4byte 0x0000082b
.L_080bffa4:
	.4byte 0x00000131
.L_080bffa8:
	.4byte gBattleWork
.L_080bffac:
	.4byte 0x00000851
.L_080bffb0:
	.4byte 0x00000141
.L_080bffb4:
	.4byte 0x00000828
