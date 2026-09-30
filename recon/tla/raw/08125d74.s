.syntax unified
	.thumb
	.global Func_08125d74
	.thumb_func
Func_08125d74:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #156
	movs r5, #192
	str r0, [sp, #8]
	lsls r5, r5, #18
	adds r3, r5, #0
	adds r3, #176
	adds r7, r1, #0
	movs r0, #168
	movs r1, #4
	ldr r6, [r3]
	bl Runtime_AllocateHeapBlock
	adds r3, r7, #0
	subs r3, #78
	str r0, [sp, #4]
	cmp r3, #2
	bhi .L_08125da6
	b .L_08125fc0
.L_08125da6:
	ldr r0, .L_08125e80
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	mov r12, r0
	adds r3, #212
	ldr r1, .L_08125e84
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	mov r0, r12
	lsls r2, r2, #24
	adds r0, #32
	adds r1, #32
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	mov r0, r12
	lsls r2, r2, #24
	adds r0, #64
	adds r1, #32
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	mov r0, r12
	lsls r2, r2, #24
	adds r0, #96
	adds r1, #32
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	mov r0, r12
	lsls r2, r2, #24
	adds r0, #128
	adds r1, #32
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	mov r0, r12
	lsls r2, r2, #24
	adds r0, #160
	adds r1, #32
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	mov r0, r12
	lsls r2, r2, #24
	adds r0, #192
	adds r1, #32
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #0
	str r3, [r6, #16]
	movs r3, #128
	movs r2, #1
	lsls r3, r3, #19
	str r2, [r6, #12]
	str r2, [r6, #8]
	strh r2, [r3]
	ldr r5, .L_08125e88
	movs r1, #32
	ldr r2, .L_08125e8c
	ldr r0, .L_08125e90
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_08125e94
	movs r1, #32
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	movs r2, #128
	ldr r3, .L_08125e78
	lsls r2, r2, #19
	adds r2, #10
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #8
	ldrh r3, [r1]
	ldr r2, .L_08125e7c
	orrs r3, r2
	ldr r2, .L_08125e98
	strh r3, [r1]
	movs r3, #2
	str r3, [r6, #8]
	movs r6, #0
.L_08125e62:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #128
	cmp r6, #20
	bls .L_08125e72
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #136
.L_08125e72:
	movs r3, #0
	b .L_08125e9c
	.2byte 0x0000
.L_08125e78:
	.4byte 0x00000c04
.L_08125e7c:
	.4byte 0x00000002
.L_08125e80:
	.4byte Data_0812cd74
.L_08125e84:
	.4byte 0x06005020
.L_08125e88:
	.4byte IwramFillWords
.L_08125e8c:
	.4byte 0x33333333
.L_08125e90:
	.4byte 0x06005000
.L_08125e94:
	.4byte 0x06005100
.L_08125e98:
	.4byte 0x06006000
.L_08125e9c:
	adds r3, #1
	strh r1, [r2]
	adds r2, #2
	cmp r3, #31
	bls .L_08125e9c
	adds r6, #1
	cmp r6, #31
	bls .L_08125e62
	ldr r5, .L_08125f0c
	movs r6, #32
	movs r3, #8
	movs r1, #0
	strh r3, [r5, #4]
	strh r6, [r5, #2]
	strh r6, [r5, #6]
	movs r0, #1
	mov r8, r1
	bl WaitFrames
	ldr r1, .L_08125efc
	movs r3, #128
	ldr r2, .L_08125f00
	lsls r3, r3, #19
	adds r3, #64
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r3, .L_08125f04
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	strh r3, [r2]
	ldr r3, .L_08125f08
	movs r1, #238
	adds r2, #2
	movs r0, #128
	lsls r1, r1, #7
	strh r3, [r2]
	lsls r0, r0, #19
	adds r1, #65
	bl QueueIoWriteDelay2
	b .L_08125f10
	.2byte 0x0000
.L_08125efc:
	.4byte 0x000000f0
.L_08125f00:
	.4byte 0x00000088
.L_08125f04:
	.4byte 0x00003537
.L_08125f08:
	.4byte 0x00003f21
.L_08125f0c:
	.4byte Data_03001120
.L_08125f10:
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #180
	bl BattlePres_SetupTransitionScene
	ldr r3, [sp, #4]
	mov r2, r8
	movs r1, #144
	str r2, [r3]
	lsls r1, r1, #3
	ldr r0, .L_08125fb0
	bl Scheduler_AddOrUpdateCallback
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_08125fb4
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_08125fb8
	movs r1, #32
	movs r0, #2
	bl Func_08013438
	strh r6, [r5, #2]
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r5, #128
	adds r3, #65
	ldrb r0, [r3]
	lsls r5, r5, #19
	bl Func_08038128
	adds r5, #8
	movs r0, #20
	bl WaitFrames
	adds r0, r5, #0
	movs r1, #2
	bl Func_08013d0c
	adds r0, r5, #0
	movs r1, #0
	bl Func_08013c58
	adds r3, r7, #0
	subs r3, #74
	cmp r3, #2
	bls .L_08125f84
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #126
	cmp r7, r0
	bne .L_08125f8c
.L_08125f84:
	movs r0, #60
	bl WaitFrames
	b .L_08125f92
.L_08125f8c:
	ldr r0, [sp, #8]
	bl Func_08118bcc
.L_08125f92:
	ldr r0, .L_08125fb0
	bl Scheduler_RemoveCallback
	ldr r0, .L_08125fb4
	bl Scheduler_RemoveCallback
	ldr r2, .L_08125fbc
	movs r3, #0
	strh r3, [r2, #2]
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Func_08013438
	b .L_081261b8
.L_08125fb0:
	.4byte Func_08125c94
.L_08125fb4:
	.4byte Func_08125cfc
.L_08125fb8:
	.4byte Func_08125d68
.L_08125fbc:
	.4byte Data_03001120
.L_08125fc0:
	ldr r5, [r5, #36]
	movs r3, #1
	str r5, [sp, #0]
	add r5, sp, #40
	str r3, [r6, #12]
	movs r3, #0
	str r3, [r6, #16]
	adds r0, r5, #0
	bl BattleParty_PrepareActiveOwners
	mov r8, r0
	lsls r0, r0, #1
	adds r0, r5, r0
	bl Func_0811a0b0
	add r8, r0
	mov r1, r8
	movs r6, #0
	cmp r1, #0
	beq .L_0812603a
	movs r2, #102
	movs r3, #114
	adds r2, #255
	adds r3, #255
	mov r11, r2
	mov r10, r5
	mov r9, r3
.L_08125ff6:
	mov r0, r10
	ldrh r5, [r0]
	movs r1, #2
	adds r0, r5, #0
	add r10, r1
	bl GetBattleObjectSlot
	adds r7, r0, #0
	adds r0, r5, #0
	bl Owner_GetState
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrh r0, [r3]
	cmp r0, r11
	beq .L_08126034
	cmp r0, r9
	beq .L_08126034
	movs r3, #118
	adds r3, #255
	cmp r0, r3
	beq .L_08126034
	movs r1, #188
	lsls r1, r1, #1
	cmp r0, r1
	beq .L_08126034
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r7, #24]
.L_08126034:
	adds r6, #1
	cmp r6, r8
	bne .L_08125ff6
.L_0812603a:
	ldr r1, .L_081260d0
	ldr r0, .L_081260d4
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0812606c
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #192
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0812606c:
	strh r4, [r0]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #0]
	ldr r2, .L_081260d8
	movs r1, #206
	lsls r1, r1, #3
	add r6, sp, #104
	adds r3, r0, r1
	movs r0, #0
	strh r2, [r3]
	mov r8, r0
	adds r1, r6, #0
	movs r0, #2
	add r5, sp, #68
	bl BattleParty_ListActorIds
	ldr r1, .L_081260cc
	str r0, [r5, #20]
	mov r10, r1
	lsls r0, r0, #1
	mov r2, r10
	adds r0, #36
	strh r2, [r5, r0]
	movs r1, #0
	adds r0, r6, #0
	bl BattleActor_SpawnObjectsForList
	adds r0, r5, #0
	bl Func_08138028
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #100
	bl BattlePres_SetupTransitionScene
	ldr r0, [sp, #4]
	mov r3, r8
	ldr r2, .L_081260dc
	str r3, [r0]
	movs r1, #32
	movs r0, #2
	bl Func_08013438
	movs r0, #1
	b .L_081260e0
.L_081260cc:
	.4byte 0x000000ff
.L_081260d0:
	.4byte gIoWriteQueue
.L_081260d4:
	.4byte 0x04000208
.L_081260d8:
	.4byte 0x00000064
.L_081260dc:
	.4byte Func_08125d68
.L_081260e0:
	bl WaitFrames
	movs r0, #20
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r5, #128
	adds r3, #65
	lsls r5, r5, #19
	ldrb r0, [r3]
	adds r5, #8
	bl Func_08038128
	adds r0, r5, #0
	movs r1, #2
	bl Func_08013d0c
	adds r0, r5, #0
	movs r1, #0
	bl Func_08013c58
	ldr r3, .L_08126148
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	add r1, sp, #12
	strh r3, [r2]
	movs r0, #3
	mov r8, r1
	bl BattleParty_ListActorIds
	adds r7, r0, #0
	lsls r3, r7, #1
	mov r2, r8
	mov r0, r10
	strh r0, [r2, r3]
	movs r1, #0
	mov r0, r8
	bl BattleActor_SpawnObjectsForList
	movs r0, #1
	mov r1, r8
	bl BattleParty_ListActorIds
	adds r7, r0, #0
	movs r6, #0
	cmp r7, #0
	beq .L_0812615e
	b .L_0812614c
	.2byte 0x0000
.L_08126148:
	.4byte 0x00003f40
.L_0812614c:
	mov r5, r8
.L_0812614e:
	ldrh r0, [r5]
	movs r1, #1
	adds r6, #1
	adds r5, #2
	bl BattlePres_SetActorRecordMode
	cmp r6, r7
	bne .L_0812614e
.L_0812615e:
	movs r1, #128
	lsls r1, r1, #19
	ldr r5, .L_08126188
	adds r1, #82
	movs r6, #0
	mov r10, r1
.L_0812616a:
	adds r3, r6, #0
	orrs r3, r5
	mov r2, r10
	strh r3, [r2]
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #16
	bne .L_0812616a
	movs r6, #0
	cmp r7, #0
	beq .L_0812619c
	mov r5, r8
	b .L_0812618c
.L_08126188:
	.4byte 0x00001000
.L_0812618c:
	ldrh r0, [r5]
	movs r1, #0
	adds r6, #1
	adds r5, #2
	bl BattlePres_SetActorRecordMode
	cmp r6, r7
	bne .L_0812618c
.L_0812619c:
	ldr r0, [sp, #8]
	bl Func_08118bcc
	ldr r2, .L_081261fc
	movs r3, #0
	strh r3, [r2, #2]
	movs r0, #1
	bl WaitFrames
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Func_08013438
.L_081261b8:
	ldr r5, .L_081261f4
	movs r6, #128
	lsls r6, r6, #19
	movs r1, #0
	movs r2, #0
	adds r6, #10
	movs r0, #2
	bl Func_08013438
	strh r5, [r6]
	movs r0, #1
	bl WaitFrames
	strh r5, [r6]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #8
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #253
	ands r3, r2
	ldr r2, .L_081261fc
	strh r3, [r1]
	movs r3, #8
	strh r3, [r2, #4]
	ldr r3, .L_081261f8
	movs r2, #128
	b .L_08126200
	.2byte 0x0000
.L_081261f4:
	.4byte 0x00001f83
.L_081261f8:
	.4byte 0x00001541
.L_081261fc:
	.4byte Data_03001120
.L_08126200:
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #168
	bl Runtime_ReleaseHeapBlock
	add sp, #156
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
