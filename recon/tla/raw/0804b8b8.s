.syntax unified
	.thumb
	.global Func_0804b8b8
	.thumb_func
Func_0804b8b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #28]
	mov r0, sp
	adds r0, #36
	str r0, [sp, #24]
	bl Func_08118138
	movs r1, #0
	movs r2, #1
	str r1, [sp, #20]
	str r2, [sp, #16]
	movs r3, #192
	lsls r3, r3, #18
	mov r11, r0
	ldr r0, [r3, #36]
	str r1, [sp, #12]
	adds r3, #228
	ldr r3, [r3]
	mov r1, r11
	mov r9, r0
	str r3, [sp, #8]
	cmp r1, #0
	bgt .L_0804b8fa
	movs r0, #1
	negs r0, r0
	b .L_0804baa6
.L_0804b8fa:
	ldr r2, [sp, #28]
	movs r3, #255
	ands r2, r3
	lsls r3, r2, #1
	str r3, [sp, #4]
	str r2, [sp, #28]
	adds r3, #88
	mov r0, r9
	ldrsh r0, [r0, r3]
	mov r10, r0
	movs r0, #112
	bl Audio_PlayCue
	movs r2, #0
	add r3, sp, #32
	str r2, [sp, #0]
	mov r8, r3
	b .L_0804ba46
.L_0804b91e:
	ldr r3, [r1, #4]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0804b984
	ldr r0, [sp, #16]
	cmp r0, #0
	bne .L_0804b984
	movs r0, #1
	movs r1, #0
	bl Func_08118140
	cmp r0, #0
	bne .L_0804b942
	movs r0, #114
	bl Audio_PlayCue
	b .L_0804ba46
.L_0804b942:
	movs r0, #110
	bl Audio_PlayCue
	ldr r1, [sp, #8]
	movs r2, #130
	lsls r2, r2, #1
	adds r3, r1, r2
	mov r0, r10
	str r0, [r3]
	adds r2, #4
	ldr r0, [sp, #28]
	ldr r4, .L_0804bab4
	adds r3, r1, r2
	movs r2, #134
	lsls r2, r2, #2
	str r0, [r3]
	adds r1, r4, r2
	movs r0, #0
.L_0804b966:
	ldrb r3, [r1]
	cmp r3, r10
	bne .L_0804b970
	strb r7, [r4, r2]
	b .L_0804b978
.L_0804b970:
	cmp r3, r7
	bne .L_0804b978
	mov r3, r10
	strb r3, [r4, r2]
.L_0804b978:
	adds r0, #1
	adds r1, #1
	adds r2, #1
	cmp r0, #7
	ble .L_0804b966
	b .L_0804ba78
.L_0804b984:
	ldr r3, [r1, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0804b9ae
	ldr r0, [sp, #20]
	mov r1, r11
	add r0, r11
	subs r0, #1
	bl Math_Mod
	str r0, [sp, #20]
	movs r0, #112
	bl Audio_PlayCue
	ldr r1, [sp, #20]
	movs r0, #1
	lsls r1, r1, #1
	str r0, [sp, #16]
	str r1, [sp, #0]
	b .L_0804b9d4
.L_0804b9ae:
	ldr r3, [r1, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_0804b9d4
	ldr r0, [sp, #20]
	mov r1, r11
	adds r0, #1
	bl Math_Mod
	str r0, [sp, #20]
	movs r0, #112
	bl Audio_PlayCue
	ldr r3, [sp, #20]
	movs r2, #1
	lsls r3, r3, #1
	str r2, [sp, #16]
	str r3, [sp, #0]
.L_0804b9d4:
	ldr r0, [sp, #16]
	cmp r0, #0
	beq .L_0804ba40
	movs r1, #0
	str r1, [sp, #16]
	ldr r0, [sp, #24]
	ldr r1, [sp, #0]
	ldr r2, [sp, #4]
	ldrh r3, [r0, r1]
	adds r2, #88
	mov r0, r9
	strh r3, [r0, r2]
	lsls r3, r3, #16
	mov r0, r10
	asrs r7, r3, #16
	bl GetBattleObjectSlotFar
	adds r5, r0, #0
	adds r0, r7, #0
	bl GetBattleObjectSlotFar
	ldr r3, [r5, #12]
	ldr r4, [r5]
	str r3, [r0, #12]
	ldr r6, [r0]
	ldr r3, [r5, #16]
	str r3, [r0, #16]
	adds r0, r6, #0
	ldr r1, [r4, #8]
	ldr r2, [r4, #12]
	ldr r3, [r4, #16]
	bl Object_SetPositionAndResetMotionFar
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #65
	ldrb r0, [r3]
	bl Func_0804297c
	bl BattleActor_CommitPlacementFar
	mov r1, r8
	movs r3, #255
	mov r2, r8
	strh r7, [r1]
	mov r0, r8
	strh r3, [r2, #2]
	movs r1, #2
	bl BattlePres_SetActorModesFar
	mov r0, r8
	bl Func_080461c8
.L_0804ba40:
	movs r0, #1
	bl WaitFrames
.L_0804ba46:
	ldr r1, [sp, #24]
	ldr r2, [sp, #0]
	ldr r0, [sp, #4]
	ldrh r3, [r1, r2]
	adds r0, #88
	mov r1, r9
	strh r3, [r1, r0]
	ldr r1, .L_0804bab8
	lsls r3, r3, #16
	asrs r7, r3, #16
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_0804ba66
	b .L_0804b91e
.L_0804ba66:
	mov r3, r10
	mov r2, r9
	strh r3, [r2, r0]
	movs r0, #1
	negs r0, r0
	str r0, [sp, #12]
	movs r0, #113
	bl Audio_PlayCue
.L_0804ba78:
	movs r3, #255
	mov r1, r8
	strh r3, [r1]
	mov r0, r8
	movs r1, #0
	bl BattlePres_SetActorModesFar
	mov r0, r8
	bl Func_080461c8
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #65
	ldrb r0, [r3]
	bl Func_0804297c
	bl BattleActor_CommitPlacementFar
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #12]
.L_0804baa6:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0804bab4:
	.4byte gPartyState
.L_0804bab8:
	.4byte gInput
