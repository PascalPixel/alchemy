.syntax unified
	.thumb
	.global BattlePres_SetActorModes
	.thumb_func
BattlePres_SetActorModes:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #28
	mov r8, r0
	mov r10, r1
	ldr r5, [r3, #36]
	cmp r1, #0
	bne .L_08126d40
	ldr r0, .L_08126dfc
	bl Scheduler_RemoveCallback
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #84
	mov r1, r10
	strh r1, [r3]
	bl BattlePres_ClearAllActorRecordModes
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #80
	movs r1, #0
	bl QueueIoWriteDelay2
.L_08126d40:
	cmp r5, #0
	beq .L_08126dee
	mov r2, r10
	cmp r2, #0
	beq .L_08126dee
	movs r1, #207
	lsls r1, r1, #3
	adds r3, r5, r1
	mov r1, r10
	strh r1, [r3]
	movs r1, #192
	lsls r1, r1, #3
	adds r1, #118
	movs r2, #0
	adds r3, r5, r1
	strh r2, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #84
	strh r2, [r3]
	movs r2, #16
	subs r3, #2
	strh r2, [r3]
	mov r5, sp
	adds r1, r5, #0
	movs r0, #3
	bl BattleParty_ListActorIds
	movs r6, #0
	adds r7, r0, #0
	cmp r6, r7
	bcs .L_08126d9e
	movs r2, #1
	mov r11, r5
	mov r9, r2
	movs r5, #0
.L_08126d88:
	mov r3, r11
	ldrsh r0, [r5, r3]
	mov r2, r9
	mov r1, r10
	ands r1, r2
	adds r6, #1
	bl BattlePres_SetActorRecordMode
	adds r5, #2
	cmp r6, r7
	bcc .L_08126d88
.L_08126d9e:
	mov r3, r8
	cmp r3, #0
	beq .L_08126dd2
	ldrh r0, [r3]
	movs r1, #2
	movs r6, #0
	add r8, r1
	cmp r0, #255
	beq .L_08126dd2
	movs r7, #1
	adds r5, r7, #0
	mov r2, r10
	ands r5, r2
.L_08126db8:
	adds r1, r5, #0
	eors r1, r7
	adds r6, #1
	bl BattlePres_SetActorRecordMode
	cmp r6, #13
	bhi .L_08126dd2
	mov r3, r8
	ldrh r0, [r3]
	movs r1, #2
	add r8, r1
	cmp r0, #255
	bne .L_08126db8
.L_08126dd2:
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #80
	movs r1, #0
	bl QueueIoWriteDelay2
	movs r1, #200
	ldr r0, .L_08126dfc
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
.L_08126dee:
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08126dfc:
	.4byte Graphics_AdvancePaletteCycle
