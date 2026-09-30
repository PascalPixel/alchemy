.syntax unified
	.thumb
	.global Func_0811e830
	.thumb_func
Func_0811e830:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r2, r5, #0
	sub sp, #40
	adds r2, #48
	str r2, [sp, #4]
	movs r7, #128
	ldrb r3, [r2]
	lsls r7, r7, #19
	adds r0, r3, #0
	str r3, [sp, #8]
	bl GetBattleObjectSlot
	ldr r0, [r0]
	movs r1, #253
	mov r10, r0
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #80
	lsls r1, r1, #6
	bl Func_08013ba4
	movs r1, #1
	ldr r0, [sp, #8]
	bl BattlePres_SetActorRecordMode
	movs r6, #0
	adds r7, #82
.L_0811e874:
	mov r2, r10
	lsls r3, r6, #18
	str r3, [r2, #12]
	ldrh r3, [r2, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	mov r2, r10
	strh r3, [r2, #6]
	lsls r3, r6, #1
	adds r1, r3, #0
	subs r1, #16
	cmp r1, #0
	bge .L_0811e892
	movs r1, #0
.L_0811e892:
	cmp r1, #15
	bgt .L_0811e8a0
	ldr r3, .L_0811e8cc
	lsls r2, r1, #8
	subs r3, r3, r1
	orrs r2, r3
	strh r2, [r7]
.L_0811e8a0:
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #16
	bne .L_0811e874
	ldr r0, [sp, #8]
	bl Func_0811b724
	ldr r3, [sp, #4]
	ldrb r0, [r3]
	bl GetBattleObjectSlot
	ldr r3, .L_0811e8d0
	adds r1, r5, #0
	movs r2, #44
	mov lr, r3
	.2byte 0xf800
	adds r3, r5, #0
	adds r3, #50
	ldrb r3, [r3]
	b .L_0811e8d4
.L_0811e8cc:
	.4byte 0x00000010
.L_0811e8d0:
	.4byte IwramCopyWords
.L_0811e8d4:
	cmp r3, #0
	beq .L_0811e8e2
	ldr r0, [sp, #8]
	bl Func_0811bc64
	bl Func_0811bc98
.L_0811e8e2:
	movs r3, #0
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ResourceMetadata_SumCommandLengthsFar + 0x10
	adds r5, #49
	str r5, [sp, #0]
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_0811e91a
	add r3, sp, #8
	ldrh r3, [r3]
	add r2, sp, #12
	strh r3, [r2]
	mov r9, r2
	movs r3, #255
	strh r3, [r2, #2]
	mov r0, r9
	movs r1, #1
	movs r2, #0
	bl Func_0811b75c
	movs r1, #1
	ldr r0, [sp, #8]
	bl BattlePres_SetActorRecordMode
	b .L_0811e980
.L_0811e91a:
	ldr r2, [sp, #4]
	ldrb r3, [r2]
	negs r0, r3
	orrs r0, r3
	add r3, sp, #12
	mov r9, r3
	lsrs r0, r0, #31
	adds r0, #1
	mov r1, r9
	bl BattleParty_ListActorIds
	mov r11, r9
	mov r8, r0
	cmp r0, #0
	ble .L_0811e95c
	movs r7, #0
	mov r6, r8
.L_0811e93c:
	mov r2, r11
	ldrh r0, [r7, r2]
	bl Func_0811a4e0
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0811e954
	mov r3, r11
	ldrh r0, [r7, r3]
	bl GetBattleObjectSlot
	strh r5, [r0, #4]
.L_0811e954:
	subs r6, #1
	adds r7, #2
	cmp r6, #0
	bne .L_0811e93c
.L_0811e95c:
	movs r2, #0
	mov r0, r11
	movs r1, #1
	bl Func_0811b75c
	mov r2, r8
	cmp r2, #0
	ble .L_0811e980
	mov r5, r11
	mov r6, r8
.L_0811e970:
	ldrh r0, [r5]
	movs r1, #1
	subs r6, #1
	adds r5, #2
	bl BattlePres_SetActorRecordMode
	cmp r6, #0
	bne .L_0811e970
.L_0811e980:
	ldr r0, [sp, #8]
	bl Actor_ResetMotionAtAnchor
	mov r2, r10
	ldr r3, [r2, #56]
	movs r7, #128
	str r3, [r2, #8]
	ldr r3, [r2, #60]
	movs r6, #0
	str r3, [r2, #12]
	ldr r3, [r2, #64]
	lsls r7, r7, #19
	str r3, [r2, #16]
	movs r5, #128
	mov r8, r6
	adds r7, #82
	lsls r5, r5, #5
.L_0811e9a2:
	mov r3, r8
	mov r2, r10
	str r3, [r2, #12]
	adds r3, r5, #0
	orrs r3, r6
	strh r3, [r7]
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0811ea08
	adds r6, #1
	adds r5, r5, r3
	cmp r6, #15
	ble .L_0811e9a2
	ldr r2, [sp, #0]
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_0811e9d0
	ldr r0, [sp, #8]
	movs r1, #0
	bl BattlePres_SetActorRecordMode
	b .L_0811e9fa
.L_0811e9d0:
	ldr r2, [sp, #4]
	mov r1, r9
	ldrb r3, [r2]
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	adds r0, #1
	bl BattleParty_ListActorIds
	cmp r0, #0
	ble .L_0811e9fa
	mov r5, r9
	adds r6, r0, #0
.L_0811e9ea:
	ldrh r0, [r5]
	movs r1, #0
	subs r6, #1
	adds r5, #2
	bl BattlePres_SetActorRecordMode
	cmp r6, #0
	bne .L_0811e9ea
.L_0811e9fa:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0811ea08:
	.4byte 0xffffff00
