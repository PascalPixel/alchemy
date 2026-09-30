.syntax unified
	.thumb
	.global Func_0811df70
	.thumb_func
Func_0811df70:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #136
	mov r10, r0
	add r0, sp, #20
	mov r8, r0
	str r1, [sp, #16]
	mov r0, r10
	mov r1, r8
	bl Func_0811ddd8
	mov r1, r10
	ldrb r1, [r1]
	mov r2, r10
	str r1, [sp, #12]
	mov r0, r10
	ldrb r2, [r2, #3]
	str r2, [sp, #8]
	movs r2, #128
	ldr r3, [r0, #88]
	lsls r2, r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L_0811dfc8
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r2, [r3]
	cmp r1, #7
	bhi .L_0811dfbc
	movs r3, #128
	lsls r3, r3, #6
	b .L_0811dfc0
.L_0811dfbc:
	movs r3, #160
	lsls r3, r3, #7
.L_0811dfc0:
	str r3, [r2]
	movs r3, #60
	str r3, [r2, #4]
	b .L_0811dfe4
.L_0811dfc8:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r1, [r3]
	ldr r3, [sp, #12]
	ldr r2, .L_0811e03c
	cmp r3, #7
	bhi .L_0811dfdc
	movs r2, #128
	lsls r2, r2, #6
.L_0811dfdc:
	ldr r3, [r1]
	cmp r3, r2
	beq .L_0811dfe4
	str r2, [r1]
.L_0811dfe4:
	movs r1, #0
	movs r0, #0
	bl BattlePres_SetActorModes
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r6, #0
	adds r3, #65
	ldrb r0, [r3]
	movs r3, #2
	negs r3, r3
	ands r0, r3
	bl UiWindow_DrawPartyStatusContentsFar
	ldr r0, [sp, #12]
	bl GetBattleObjectSlot
	ldr r0, [r0]
	ldr r3, .L_0811e038
	movs r2, #128
	mov r11, r0
	lsls r2, r2, #19
	mov r0, sp
	adds r2, #80
	adds r0, #108
	strh r3, [r2]
	str r0, [sp, #4]
	ldr r1, [sp, #4]
	movs r0, #3
	bl BattleParty_ListActorIds
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0811e094
	ldr r2, [sp, #16]
	movs r1, #2
	ldr r5, [sp, #4]
	ands r2, r1
	mov r9, r2
	b .L_0811e040
	.2byte 0x0000
.L_0811e038:
	.4byte 0x00003f40
.L_0811e03c:
	.4byte 0xffffe000
.L_0811e040:
	ldrh r3, [r5]
	cmp r3, #254
	beq .L_0811e08c
	mov r3, r9
	cmp r3, #0
	bne .L_0811e056
	ldrh r0, [r5]
	ldr r1, [sp, #12]
	cmp r0, r1
	beq .L_0811e076
	b .L_0811e058
.L_0811e056:
	ldrh r0, [r5]
.L_0811e058:
	ldr r3, [sp, #8]
	movs r2, #0
	cmp r3, #7
	bhi .L_0811e062
	movs r2, #1
.L_0811e062:
	movs r3, #0
	cmp r0, #7
	bhi .L_0811e06a
	movs r3, #1
.L_0811e06a:
	cmp r2, r3
	beq .L_0811e08c
	movs r1, #1
	bl BattlePres_SetActorRecordMode
	b .L_0811e08c
.L_0811e076:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #105
	adds r3, r3, r0
	ldrb r1, [r3]
	mov r0, r11
	bl Object_SetMode
.L_0811e08c:
	adds r6, #1
	adds r5, #2
	cmp r6, r7
	bne .L_0811e040
.L_0811e094:
	ldr r1, [sp, #16]
	movs r3, #2
	ands r3, r1
	cmp r3, #0
	bne .L_0811e0b4
	movs r0, #154
	bl Audio_PlayCue
	mov r2, r8
	mov r3, r10
	ldr r0, [r2, #8]
	ldr r1, [r3, #80]
	movs r2, #0
	movs r3, #0
	bl Func_08127308
.L_0811e0b4:
	ldr r0, [sp, #16]
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_0811e0c6
	ldr r0, [sp, #12]
	movs r1, #1
	bl BattlePres_SetActorRecordMode
.L_0811e0c6:
	movs r1, #128
	ldr r2, .L_0811e0d8
	lsls r1, r1, #19
	ldr r5, .L_0811e0dc
	adds r1, #82
	movs r6, #0
	mov r11, r1
	mov r9, r2
	b .L_0811e0e0
.L_0811e0d8:
	.4byte 0x00000010
.L_0811e0dc:
	.4byte 0x00001000
.L_0811e0e0:
	mov r0, r9
	subs r3, r0, r6
	orrs r3, r5
	mov r1, r11
	strh r3, [r1]
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #16
	bne .L_0811e0e0
	mov r2, r10
	ldr r3, [r2, #92]
	cmp r3, #0
	beq .L_0811e126
	cmp r3, #1
	bne .L_0811e114
	ldrb r1, [r2]
	movs r0, #0
	bl BattleEv_Push
	ldr r1, .L_0811e154
	movs r0, #4
	bl BattleEv_Push
	b .L_0811e11c
.L_0811e114:
	ldr r1, .L_0811e158
	movs r0, #4
	bl BattleEv_Push
.L_0811e11c:
	bl BattleEv_DispatchQueued
	bl Func_0812756c
	b .L_0811e29a
.L_0811e126:
	movs r6, #0
	movs r2, #0
	cmp r6, r7
	bcs .L_0811e17e
	ldr r0, [sp, #16]
	ldr r5, [sp, #4]
	movs r3, #1
	ands r0, r3
	mov r12, r0
	adds r1, r5, #0
.L_0811e13a:
	ldrh r3, [r5]
	ldr r0, [sp, #12]
	adds r5, #2
	cmp r3, r0
	bne .L_0811e15c
	mov r3, r12
	cmp r3, #0
	bne .L_0811e178
	add r0, sp, #12
	ldrh r0, [r0]
	adds r2, #1
	strh r0, [r1]
	b .L_0811e176
.L_0811e154:
	.4byte 0x00000cad
.L_0811e158:
	.4byte 0x00000cac
.L_0811e15c:
	ldr r0, [sp, #8]
	movs r4, #0
	cmp r0, #7
	bls .L_0811e166
	movs r4, #1
.L_0811e166:
	movs r0, #0
	cmp r3, #7
	bhi .L_0811e16e
	movs r0, #1
.L_0811e16e:
	cmp r4, r0
	beq .L_0811e178
	strh r3, [r1]
	adds r2, #1
.L_0811e176:
	adds r1, #2
.L_0811e178:
	adds r6, #1
	cmp r6, r7
	bcc .L_0811e13a
.L_0811e17e:
	ldr r3, .L_0811e1a4
	ldr r1, [sp, #4]
	lsls r2, r2, #1
	strh r3, [r1, r2]
	ldr r0, [sp, #4]
	movs r1, #0
	bl BattleActor_SpawnObjectsForList
	mov r2, r10
	movs r3, #1
	ldrsb r3, [r2, r3]
	movs r6, #0
	cmp r6, r3
	bcs .L_0811e1b6
	ldr r1, [sp, #4]
	mov r12, r3
	adds r2, #3
	b .L_0811e1a8
	.2byte 0x0000
.L_0811e1a4:
	.4byte 0x000000ff
.L_0811e1a8:
	ldrb r3, [r2]
	adds r6, #1
	strh r3, [r1]
	adds r2, #1
	adds r1, #2
	cmp r6, r12
	bcc .L_0811e1a8
.L_0811e1b6:
	ldr r2, .L_0811e1cc
	ldr r0, [sp, #4]
	lsls r3, r6, #1
	strh r2, [r0, r3]
	mov r2, r8
	ldr r3, [r2, #20]
	movs r6, #0
	cmp r3, #0
	beq .L_0811e212
	movs r5, #0
	b .L_0811e1d0
.L_0811e1cc:
	.4byte 0x000000ff
.L_0811e1d0:
	lsls r3, r6, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlot
	movs r1, #0
	ldr r0, [r0]
	bl GetMotionRecord
	ldrb r3, [r0, #27]
	movs r1, #0
	subs r3, #1
	cmp r3, #0
	beq .L_0811e206
	add r2, sp, #136
	mov r12, r3
	adds r3, r2, r5
	adds r2, r3, #0
	subs r2, #62
	adds r0, #40
.L_0811e1f8:
	ldmia r0!, {r3}
	adds r1, #1
	ldrb r3, [r3, #5]
	strb r3, [r2]
	adds r2, #1
	cmp r1, r12
	bne .L_0811e1f8
.L_0811e206:
	mov r2, r8
	ldr r3, [r2, #20]
	adds r6, #1
	adds r5, #4
	cmp r6, r3
	bne .L_0811e1d0
.L_0811e212:
	mov r3, r10
	ldr r2, [r3, #88]
	movs r3, #128
	lsls r3, r3, #8
	ands r2, r3
	cmp r2, #0
	beq .L_0811e22e
	ldr r0, [sp, #8]
	cmp r0, #7
	bls .L_0811e236
	movs r3, #0
	mov r2, r8
	str r3, [r2, #4]
	b .L_0811e242
.L_0811e22e:
	mov r0, r10
	ldrb r3, [r0, #3]
	cmp r3, #7
	bhi .L_0811e23e
.L_0811e236:
	movs r3, #1
	mov r1, r8
	str r3, [r1, #4]
	b .L_0811e242
.L_0811e23e:
	mov r3, r8
	str r2, [r3, #4]
.L_0811e242:
	mov r0, r10
	ldr r3, [r0, #88]
	movs r2, #128
	lsls r2, r2, #10
	ands r3, r2
	cmp r3, #0
	beq .L_0811e25a
	mov r1, r8
	ldr r3, [r1, #4]
	movs r2, #1
	eors r3, r2
	str r3, [r1, #4]
.L_0811e25a:
	movs r1, #144
	ldr r0, .L_0811e28c
	lsls r1, r1, #3
	bl Func_080145a8
	mov r2, r10
	ldr r0, [r2, #88]
	movs r3, #128
	lsls r3, r3, #8
	ands r3, r0
	cmp r3, #0
	beq .L_0811e27a
	mov r0, r8
	bl Resource_FarCall00C + 0x10
	b .L_0811e296
.L_0811e27a:
	movs r3, #128
	lsls r3, r3, #7
	ands r3, r0
	cmp r3, #0
	beq .L_0811e290
	mov r0, r8
	bl Resource_FarCall00C + 0x8
	b .L_0811e296
.L_0811e28c:
	.4byte Func_08122d10
.L_0811e290:
	mov r0, r8
	bl Resource_FarCall00C + 0x18
.L_0811e296:
	bl Func_081234f0
.L_0811e29a:
	bl BattleActor_CommitPlacement
	movs r0, #3
	ldr r1, [sp, #4]
	bl BattleParty_ListActorIds
	movs r2, #128
	ldr r3, .L_0811e2c4
	lsls r2, r2, #19
	adds r2, #80
	adds r7, r0, #0
	strh r3, [r2]
	movs r6, #0
	cmp r7, #0
	beq .L_0811e30a
	ldr r3, [sp, #16]
	ldr r5, [sp, #4]
	movs r4, #2
	ands r4, r3
	b .L_0811e2c8
	.2byte 0x0000
.L_0811e2c4:
	.4byte 0x00003f40
.L_0811e2c8:
	ldrh r3, [r5]
	cmp r3, #254
	beq .L_0811e302
	cmp r4, #0
	bne .L_0811e2e0
	ldr r0, [sp, #12]
	cmp r3, r0
	bne .L_0811e2e0
	mov r1, r8
	ldr r3, [r1]
	cmp r3, #3
	beq .L_0811e302
.L_0811e2e0:
	ldr r3, [sp, #8]
	ldrh r0, [r5]
	movs r2, #0
	cmp r3, #7
	bhi .L_0811e2ec
	movs r2, #1
.L_0811e2ec:
	movs r3, #0
	cmp r0, #7
	bhi .L_0811e2f4
	movs r3, #1
.L_0811e2f4:
	cmp r2, r3
	beq .L_0811e302
	movs r1, #1
	str r4, [sp, #0]
	bl BattlePres_SetActorRecordMode
	ldr r4, [sp, #0]
.L_0811e302:
	adds r6, #1
	adds r5, #2
	cmp r6, r7
	bne .L_0811e2c8
.L_0811e30a:
	movs r0, #128
	lsls r0, r0, #19
	ldr r5, .L_0811e334
	adds r0, #82
	movs r6, #0
	mov r8, r0
.L_0811e316:
	adds r3, r6, #0
	orrs r3, r5
	mov r1, r8
	strh r3, [r1]
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #16
	bne .L_0811e316
	movs r6, #0
	cmp r7, #0
	beq .L_0811e348
	ldr r5, [sp, #4]
	b .L_0811e338
.L_0811e334:
	.4byte 0x00001000
.L_0811e338:
	ldrh r0, [r5]
	movs r1, #0
	adds r6, #1
	adds r5, #2
	bl BattlePres_SetActorRecordMode
	cmp r6, r7
	bne .L_0811e338
.L_0811e348:
	movs r1, #0
	movs r2, #0
	movs r3, #100
	movs r0, #0
	bl BattlePres_SetupTransitionScene
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	add sp, #136
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
