.syntax unified
	.thumb
	.global BattlePresentation_DispatchAction
	.thumb_func
BattlePresentation_DispatchAction:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	movs r3, #0
	ldrsh r0, [r0, r3]
	movs r2, #0
	sub sp, #32
	mov r10, r1
	mov r9, r2
	cmp r0, #255
	bne .L_080b9b50
	movs r0, #0
	b .L_080b9d02
.L_080b9b50:
	bl Owner_GetStateFar
	movs r2, #56
	ldrsh r3, [r0, r2]
	cmp r3, #0
	bne .L_080b9b62
	movs r0, #1
	negs r0, r0
	b .L_080b9d02
.L_080b9b62:
	ldr r2, .L_080b9d14
	adds r3, r0, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080b9b74
	mov r0, r8
	movs r1, #1
	bl BattleCommand_SelectAutomatic
.L_080b9b74:
	ldr r2, .L_080b9d18
	ldr r7, [r2]
	movs r3, #60
	str r3, [r7, #4]
	adds r3, r2, #0
	subs r3, #140
	ldr r6, [r3]
	mov r3, r9
	str r3, [r7, #20]
	ldr r3, .L_080b9d1c
	subs r2, #128
	adds r1, r6, r3
	movs r3, #128
	ldr r5, [r2]
	lsls r3, r3, #9
	str r3, [r1]
	bl Render_ResetTransformState
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Graphics_PrepareTransferInIwramWork
	movs r0, #255
	movs r1, #192
	ldr r3, .L_080b9d20
	lsls r1, r1, #8
	lsls r0, r0, #17
	bl _call_via_r3
	adds r1, r0, #0
	movs r0, #255
	ldr r2, .L_080b9d24
	lsls r0, r0, #17
	bl Camera_StoreSceneParameters
	mov r2, r10
	cmp r2, #0
	beq .L_080b9bce
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r7]
	mov r0, r10
	bl WaitFrames
.L_080b9bce:
	mov r2, r8
	ldrh r3, [r2]
	add r0, sp, #28
	strh r3, [r0]
	movs r3, #255
	strh r3, [r0, #2]
	movs r1, #1
	bl BattlePres_SetActorModes
	ldr r3, .L_080b9d28
	mov r0, r8
	adds r1, r6, r3
	bl BattleCommand_BuildPlan
	cmp r0, #0
	bne .L_080b9c9c
	movs r2, #213
	lsls r2, r2, #3
	adds r3, r6, r2
	ldr r3, [r3]
	subs r3, #1
	cmp r3, #8
	bhi .L_080b9cb6
	ldr r2, .L_080b9d2c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080b9c04:
	.4byte .L_080b9c28
	.4byte .L_080b9c34
	.4byte .L_080b9c58
	.4byte .L_080b9c76
	.4byte .L_080b9c40
	.4byte .L_080b9c5e
	.4byte .L_080b9c84
	.4byte .L_080b9c6a
	.4byte .L_080b9c4c
.L_080b9c28:
	ldr r3, .L_080b9d28
	movs r1, #0
	adds r0, r6, r3
	bl BattlePres_RunActorEntries
	b .L_080b9cb6
.L_080b9c34:
	ldr r2, .L_080b9d28
	movs r1, #0
	adds r0, r6, r2
	bl RunBattlePresentation
	b .L_080b9cb6
.L_080b9c40:
	ldr r3, .L_080b9d28
	movs r1, #1
	adds r0, r6, r3
	bl BattlePresentation_RunUnitTransition
	b .L_080b9cb6
.L_080b9c4c:
	ldr r2, .L_080b9d28
	movs r1, #0
	adds r0, r6, r2
	bl BattlePresentation_RunUnitTransition
	b .L_080b9cb6
.L_080b9c58:
	ldr r3, .L_080b9d28
	movs r1, #0
	b .L_080b9c6e
.L_080b9c5e:
	ldr r2, .L_080b9d28
	movs r1, #1
	adds r0, r6, r2
	bl Func_080ba978
	b .L_080b9cb6
.L_080b9c6a:
	ldr r3, .L_080b9d28
	movs r1, #2
.L_080b9c6e:
	adds r0, r6, r3
	bl Func_080ba978
	b .L_080b9cb6
.L_080b9c76:
	ldr r2, .L_080b9d28
	movs r1, #0
	adds r0, r6, r2
	mov r2, r8
	bl Func_080ba6ac
	b .L_080b9cb6
.L_080b9c84:
	ldr r3, .L_080b9d28
	adds r0, r6, r3
	bl BattlePresentation_RunEncounterOrUnitTrigger
	cmp r0, #0
	beq .L_080b9c94
	movs r2, #1
	mov r9, r2
.L_080b9c94:
	mov r3, r9
	cmp r3, #0
	beq .L_080b9cb6
	b .L_080b9cec
.L_080b9c9c:
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_080b9cae
	bl BattlePresentation_WaitForAdvance
	movs r0, #3
	bl WaitFrames
.L_080b9cae:
	movs r0, #0
	movs r1, #0
	bl BattlePres_SetActorModes
.L_080b9cb6:
	bl BattleMotion_DestroyAllSlotObjects
	ldr r3, .L_080b9d28
	adds r0, r6, r3
	bl BattleUnit_ProcessTurnEnd
	bl BattleActor_CommitPlacement
	mov r5, sp
	movs r0, #3
	adds r1, r5, #0
	bl BattleParty_ListActorIds
	cmp r0, #0
	ble .L_080b9ce6
	adds r6, r5, #0
	adds r5, r0, #0
.L_080b9cd8:
	ldrh r0, [r6]
	subs r5, #1
	adds r6, #2
	bl Actor_ResetMotionAtAnchor
	cmp r5, #0
	bne .L_080b9cd8
.L_080b9ce6:
	movs r3, #255
	mov r2, r8
	strh r3, [r2]
.L_080b9cec:
	ldr r3, .L_080b9d30
	movs r2, #201
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r3, r3, r2
	movs r0, #2
	ldrh r1, [r3]
	movs r2, #0
	bl BattlePresentation_ConfigurePaletteFade
	mov r0, r9
.L_080b9d02:
	add sp, #32
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_080b9d14:
	.4byte 0x00000129
.L_080b9d18:
	.4byte gTransitionWork
.L_080b9d1c:
	.4byte 0x00000644
.L_080b9d20:
	.4byte IwramRatioMulQ14
.L_080b9d24:
	.4byte 0x7fff0000
.L_080b9d28:
	.4byte 0x00000654
.L_080b9d2c:
	.4byte .L_080b9c04
.L_080b9d30:
	.4byte gBattleWork
