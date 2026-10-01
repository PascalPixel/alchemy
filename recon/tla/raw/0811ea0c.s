.syntax unified
	.thumb
	.global Func_0811ea0c
	.thumb_func
Func_0811ea0c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #116
	str r0, [sp, #16]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r3, [r3]
	adds r7, r1, #0
	str r3, [sp, #8]
	ldrb r5, [r0]
	adds r0, r5, #0
	bl GetBattleObjectSlot
	ldr r2, [r0, #12]
	ldr r3, [r0]
	str r2, [r3, #8]
	ldr r1, [r0, #16]
	adds r0, r2, #0
	str r1, [r3, #16]
	bl ArcTan2
	ldr r1, .L_0811ebd8
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r2, r0, r1
	cmp r5, #7
	bls .L_0811ea54
	movs r3, #192
	lsls r3, r3, #7
	adds r2, r0, r3
.L_0811ea54:
	movs r3, #254
	ldr r4, .L_0811ebd8
	lsls r3, r3, #7
	adds r3, #255
	ands r2, r3
	adds r3, r2, r4
	lsrs r2, r3, #31
	ldr r4, [sp, #8]
	adds r3, r3, r2
	movs r1, #128
	asrs r3, r3, #1
	lsls r1, r1, #6
	adds r2, r3, r1
	ldr r3, [r4]
	cmp r3, r2
	bne .L_0811ea7e
	str r2, [r4]
	movs r0, #1
	bl WaitFrames
	b .L_0811ea88
.L_0811ea7e:
	ldr r1, [sp, #8]
	movs r0, #1
	str r2, [r1]
	bl WaitFrames
.L_0811ea88:
	movs r0, #0
	movs r1, #0
	bl BattlePres_SetActorModes
	add r2, sp, #28
	mov r8, r2
	ldr r0, [sp, #16]
	mov r1, r8
	bl Func_0811ddd8
	mov r4, r8
	ldr r3, [r4]
	cmp r3, #135
	bne .L_0811eab8
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #65
	ldrb r0, [r3]
	movs r3, #2
	negs r3, r3
	ands r0, r3
	bl UiWindow_DrawPartyStatusContentsFar
.L_0811eab8:
	ldr r0, [sp, #16]
	mov r1, r8
	bl Func_0811e3ac
	str r0, [sp, #4]
	mov r1, r8
	ldr r0, [r1, #8]
	bl Owner_GetState
	mov r3, r8
	adds r6, r0, #0
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Owner_GetState
	ldr r3, [sp, #16]
	ldr r1, [sp, #16]
	adds r3, #45
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r4, #0
	str r3, [sp, #12]
	mov r10, r4
	movs r3, #31
	ldrsb r3, [r1, r3]
	cmp r3, #0
	bne .L_0811eaf4
	movs r2, #1
	mov r10, r2
.L_0811eaf4:
	ldr r3, [sp, #16]
	ldrb r0, [r3]
	bl GetBattleObjectSlot
	ldr r5, [r0]
	movs r1, #0
	adds r0, r5, #0
	bl GetMotionRecord
	ldr r3, [r0, #40]
	movs r1, #2
	movs r4, #0
	ldrsh r0, [r3, r4]
	movs r2, #1
	bl ResourceMetadata_SumCommandLengthsFar
	mov r1, r8
	ldr r3, [r1]
	mov r11, r0
	cmp r3, #10
	bne .L_0811eb28
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetMode
	b .L_0811eb46
.L_0811eb28:
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrh r0, [r3]
	bl Func_0812814c
	mov r4, r8
	adds r3, r0, #0
	movs r2, #36
	ldrsh r1, [r4, r2]
	lsls r3, r3, #16
	ldr r0, [r4, #8]
	mov r2, r11
	bl Func_0811c120
.L_0811eb46:
	movs r3, #32
	ands r3, r7
	cmp r3, #0
	beq .L_0811ebb8
	ldr r3, [sp, #16]
	ldrb r2, [r3]
	ldrb r3, [r3, #2]
	cmp r2, r3
	beq .L_0811ebb8
	mov r1, r8
	ldrh r2, [r1, #36]
	adds r0, r3, #0
	movs r3, #128
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r0, r3
	beq .L_0811ebb8
	bl GetBattleObjectSlot
	adds r5, r0, #0
	movs r1, #0
	ldr r0, [r5]
	bl GetMotionRecord
	ldr r3, [r0, #40]
	movs r1, #2
	movs r2, #0
	ldrsh r0, [r3, r2]
	movs r2, #1
	bl ResourceMetadata_SumCommandLengthsFar
	ldr r3, [sp, #16]
	mov r11, r0
	ldrb r0, [r3, #2]
	bl Owner_GetState
	movs r4, #165
	lsls r4, r4, #1
	adds r3, r0, r4
	ldrh r0, [r3]
	bl Func_0812814c
	ldr r1, [sp, #16]
	adds r3, r0, #0
	mov r4, r8
	ldrb r0, [r1, #2]
	lsls r3, r3, #16
	movs r2, #36
	ldrsh r1, [r4, r2]
	mov r2, r11
	bl Func_0811c120
	ldr r0, [r5]
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
.L_0811ebb8:
	mov r1, r8
	ldr r0, [r1, #8]
	bl GetBattleObjectSlot
	movs r1, #16
	ldr r0, [r0]
	bl ObjectDispatch_ApplyValueToChildrenFar
	mov r2, r8
	ldrh r3, [r2, #36]
	cmp r3, #7
	bhi .L_0811ebdc
	movs r3, #1
	str r3, [r2, #4]
	b .L_0811ebe2
	.2byte 0x0000
.L_0811ebd8:
	.4byte 0xffffe000
.L_0811ebdc:
	movs r3, #0
	mov r4, r8
	str r3, [r4, #4]
.L_0811ebe2:
	ldr r1, .L_0811ec18
	movs r3, #128
	ldr r2, .L_0811ec1c
	lsls r3, r3, #19
	adds r3, #64
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r3, .L_0811ec20
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	strh r3, [r2]
	ldr r3, .L_0811ec24
	adds r2, #2
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0811ec28
	orrs r3, r2
	b .L_0811ec2c
	.2byte 0x0000
.L_0811ec18:
	.4byte 0x000000f0
.L_0811ec1c:
	.4byte 0x00001088
.L_0811ec20:
	.4byte 0x00003537
.L_0811ec24:
	.4byte 0x00003f21
.L_0811ec28:
	.4byte 0x00006000
.L_0811ec2c:
	strh r3, [r1]
	mov r1, r10
	cmp r1, #0
	beq .L_0811ec78
	movs r0, #10
	bl WaitFrames
	mov r3, r8
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_0811bfd0
	movs r0, #2
	bl WaitFrames
	movs r0, #4
	bl WaitFrames
	movs r0, #10
	bl WaitFrames
	ldr r4, [sp, #16]
	movs r0, #0
	ldrb r1, [r4, #3]
	bl BattleEv_Push
	ldr r1, .L_0811eda8
	movs r0, #4
	bl BattleEv_Push
	bl BattleEv_DispatchQueued
	mov r2, r8
	movs r1, #36
	ldrsh r0, [r2, r1]
	bl Actor_ResetMotionAtAnchor
	b .L_0811ed88
.L_0811ec78:
	movs r3, #0
	mov r4, r8
	str r3, [sp, #0]
	str r3, [r4, #28]
	ldr r1, [sp, #16]
	ldr r3, [r1, #88]
	cmp r3, #0
	beq .L_0811ec8c
	movs r3, #1
	str r3, [r4, #28]
.L_0811ec8c:
	ldr r2, [sp, #12]
	cmp r2, #0
	beq .L_0811ec9a
	mov r4, r8
	ldr r3, [r4]
	adds r3, #200
	str r3, [r4]
.L_0811ec9a:
	mov r1, r8
	ldr r3, [r1]
	cmp r3, #210
	beq .L_0811ecc4
	ldr r2, [sp, #12]
	cmp r2, #0
	beq .L_0811ecc4
	ldr r4, [sp, #8]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [r4, #20]
	add r0, sp, #20
	ldr r3, [r1, #8]
	strh r3, [r0]
	ldr r3, [r1, #12]
	movs r1, #0
	strh r3, [r0, #2]
	movs r3, #255
	strh r3, [r0, #4]
	bl BattleActor_SpawnObjectsForList
.L_0811ecc4:
	movs r1, #8
	negs r1, r1
	add r11, r1
	mov r2, r11
	cmp r2, #0
	bgt .L_0811ecd4
	movs r3, #1
	mov r11, r3
.L_0811ecd4:
	movs r4, #0
	mov r1, r11
	mov r9, r4
	cmp r1, #0
	beq .L_0811ed12
	mov r7, r8
	mov r10, r4
.L_0811ece2:
	ldr r2, [sp, #0]
	cmp r2, #0
	beq .L_0811ed00
	mov r1, r11
	mov r0, r10
	bl __divsi3
	ldr r5, [r7, #8]
	ldr r6, [r7, #12]
	adds r2, r0, #0
	adds r2, #100
	adds r0, r5, #0
	adds r1, r6, #0
	bl BattlePres_SetupTransitionAtPairMidpoint
.L_0811ed00:
	movs r0, #1
	bl WaitFrames
	movs r4, #1
	movs r3, #30
	add r9, r4
	add r10, r3
	cmp r9, r11
	bne .L_0811ece2
.L_0811ed12:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0811edac
	bl Scheduler_AddOrUpdateCallback
	mov r1, r8
	ldr r3, [r1]
	cmp r3, #0
	beq .L_0811ed4c
	ldr r2, [sp, #4]
	cmp r2, #0
	beq .L_0811ed30
	adds r0, r2, #0
	bl Func_0811e7dc
.L_0811ed30:
	ldr r4, [sp, #16]
	movs r2, #128
	ldr r3, [r4, #88]
	lsls r2, r2, #7
	ands r3, r2
	cmp r3, #0
	beq .L_0811ed46
	mov r0, r8
	bl Func_08138008
	b .L_0811ed4c
.L_0811ed46:
	mov r0, r8
	bl Func_08138018
.L_0811ed4c:
	ldr r1, [sp, #4]
	cmp r1, #0
	beq .L_0811ed5e
	adds r0, r1, #0
	bl Func_0811e830
	ldr r0, [sp, #4]
	bl Sys_Free
.L_0811ed5e:
	bl Func_081234f0
	ldr r2, [sp, #12]
	cmp r2, #0
	beq .L_0811ed7e
	ldr r4, [sp, #8]
	movs r3, #0
	str r3, [r4, #20]
	bl BattleParty_ListAllUnitsAndSubmit
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #100
	bl BattlePres_SetupTransitionScene
.L_0811ed7e:
	mov r2, r8
	movs r1, #36
	ldrsh r0, [r2, r1]
	bl Actor_ResetMotionAtAnchor
.L_0811ed88:
	ldr r3, [sp, #4]
	cmp r3, #0
	bne .L_0811ed96
	mov r4, r8
	ldr r0, [r4, #8]
	bl Actor_ResetMotionAtAnchor
.L_0811ed96:
	movs r0, #0
	add sp, #116
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811eda8:
	.4byte 0x00000caa
.L_0811edac:
	.4byte Func_08122d10
