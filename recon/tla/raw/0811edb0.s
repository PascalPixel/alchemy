.syntax unified
	.thumb
	.global Func_0811edb0
	.thumb_func
Func_0811edb0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r1, [r3]
	ldrb r3, [r0]
	mov r8, r2
	sub sp, #92
	mov r10, r0
	ldr r2, .L_0811f024
	cmp r3, #4
	bhi .L_0811edd8
	movs r2, #128
	lsls r2, r2, #6
.L_0811edd8:
	ldr r3, [r1]
	cmp r3, r2
	beq .L_0811ede0
	str r2, [r1]
.L_0811ede0:
	add r5, sp, #4
	mov r0, r10
	adds r1, r5, #0
	bl Func_0811ddd8
	movs r1, #0
	movs r0, #0
	bl BattlePres_SetActorModes
	ldr r0, [r5, #8]
	bl GetBattleObjectSlot
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r1, #128
	ldr r0, [r0]
	lsls r1, r1, #4
	adds r1, #105
	adds r3, r3, r1
	ldrb r1, [r3]
	mov r9, r0
	bl Object_SetMode
	mov r0, r9
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	mov r2, r10
	ldrb r3, [r2, #3]
	cmp r3, #7
	bhi .L_0811ee24
	movs r3, #1
	b .L_0811ee26
.L_0811ee24:
	movs r3, #0
.L_0811ee26:
	str r3, [r5, #4]
	ldr r3, [r5, #20]
	movs r7, #0
	adds r2, r5, #0
	cmp r3, #0
	beq .L_0811ee76
	movs r6, #0
.L_0811ee34:
	lsls r3, r7, #1
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
	beq .L_0811ee6a
	add r2, sp, #92
	mov r12, r3
	adds r3, r2, r6
	adds r2, r3, #0
	subs r2, #34
	adds r0, #40
.L_0811ee5c:
	ldmia r0!, {r3}
	adds r1, #1
	ldrb r3, [r3, #5]
	strb r3, [r2]
	adds r2, #1
	cmp r1, r12
	bne .L_0811ee5c
.L_0811ee6a:
	ldr r3, [r5, #20]
	adds r7, #1
	adds r6, #4
	adds r2, r5, #0
	cmp r7, r3
	bne .L_0811ee34
.L_0811ee76:
	movs r1, #144
	ldr r0, .L_0811f028
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0811eeee
	movs r3, #192
	lsls r3, r3, #18
	movs r7, #0
	mov r11, r3
	movs r6, #0
.L_0811ee90:
	mov r1, r11
	ldr r3, [r1, #36]
	cmp r7, #19
	bgt .L_0811eebc
	movs r2, #160
	lsls r2, r2, #3
	movs r1, #192
	adds r2, #108
	lsls r1, r1, #3
	adds r0, r3, r2
	adds r1, #108
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #9
	movs r1, #160
	subs r2, r2, r6
	lsls r1, r1, #19
	str r2, [r3]
	adds r1, #192
	movs r3, #128
	bl ColorBuffer_Scale
.L_0811eebc:
	movs r0, #1
	bl WaitFrames
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #68
	adds r7, #1
	adds r6, r6, r2
	cmp r7, #19
	ble .L_0811ee90
	mov r6, r10
	ldr r3, [r6, #88]
	movs r2, #128
	lsls r2, r2, #7
	ands r3, r2
	cmp r3, #0
	beq .L_0811eee6
	adds r0, r5, #0
	bl Func_08138008
	b .L_0811eef4
.L_0811eee6:
	adds r0, r5, #0
	bl Func_08138018
	b .L_0811eef4
.L_0811eeee:
	movs r0, #60
	bl WaitFrames
.L_0811eef4:
	bl Func_081234f0
	adds r6, r5, #0
	mov r0, r9
	movs r1, #1
	bl Object_SetMode
	ldr r3, [r6, #20]
	movs r7, #0
	cmp r3, #0
	beq .L_0811ef22
	movs r2, #36
.L_0811ef0c:
	ldrsh r0, [r6, r2]
	str r2, [sp, #0]
	bl Actor_ResetMotionAtAnchor
	adds r5, r6, #0
	ldr r2, [sp, #0]
	ldr r3, [r5, #20]
	adds r7, #1
	adds r2, #2
	cmp r7, r3
	bne .L_0811ef0c
.L_0811ef22:
	mov r3, r8
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Owner_GetState
	mov r1, r8
	movs r7, #8
	ldrsh r3, [r1, r7]
	adds r6, r0, #0
	lsls r3, r3, #1
	adds r3, #216
	ldrh r5, [r6, r3]
	adds r0, r5, #0
	bl Item_Get
	ldrb r2, [r0, #12]
	adds r3, r2, #0
	cmp r3, #1
	bne .L_0811efb2
	mov r3, r8
	movs r6, #8
	ldrsh r1, [r3, r6]
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Inventory_RemoveFar
	mov r1, r8
	movs r7, #8
	ldrsh r5, [r1, r7]
	cmp r0, #2
	bne .L_0811f012
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #36]
	movs r4, #0
.L_0811ef68:
	movs r2, #188
	lsls r1, r4, #4
	lsls r2, r2, #2
	adds r3, r1, r2
	adds r3, r0, r3
	movs r6, #2
	ldrsh r3, [r3, r6]
	cmp r3, #2
	bne .L_0811efaa
	movs r7, #187
	lsls r7, r7, #2
	adds r3, r1, r7
	ldrsh r2, [r0, r3]
	mov r6, r8
	movs r7, #0
	ldrsh r3, [r6, r7]
	cmp r2, r3
	bne .L_0811efaa
	movs r7, #189
	lsls r7, r7, #2
	adds r1, r1, r7
	ldrsh r2, [r0, r1]
	ldrh r3, [r0, r1]
	cmp r2, r5
	bne .L_0811efa2
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	b .L_0811efa8
.L_0811efa2:
	cmp r2, r5
	ble .L_0811efaa
	subs r3, #1
.L_0811efa8:
	strh r3, [r0, r1]
.L_0811efaa:
	adds r4, #1
	cmp r4, #19
	bls .L_0811ef68
	b .L_0811f012
.L_0811efb2:
	lsls r3, r2, #24
	lsrs r3, r3, #24
	cmp r3, #2
	bne .L_0811eff4
	bl BattleRandom16Far
	movs r3, #7
	ands r0, r3
	cmp r0, #0
	bne .L_0811f012
	mov r1, r8
	movs r7, #8
	ldrsh r3, [r1, r7]
	movs r0, #2
	lsls r3, r3, #1
	adds r3, #216
	ldrh r1, [r6, r3]
	bl BattleEv_Push
	ldr r1, .L_0811f02c
	movs r0, #4
	bl BattleEv_Push
	mov r3, r8
	movs r2, #0
	ldrsh r0, [r3, r2]
	movs r6, #8
	ldrsh r1, [r3, r6]
	bl Inventory_BreakFar
	bl BattleEv_DispatchQueued
	b .L_0811f012
.L_0811eff4:
	cmp r3, #4
	bne .L_0811f012
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r3, r5
	cmp r3, #184
	bne .L_0811f006
	movs r5, #185
.L_0811f006:
	mov r1, r8
	movs r7, #8
	ldrsh r3, [r1, r7]
	lsls r3, r3, #1
	adds r3, #216
	strh r5, [r6, r3]
.L_0811f012:
	movs r0, #0
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811f024:
	.4byte 0xffffe000
.L_0811f028:
	.4byte Func_08122d10
.L_0811f02c:
	.4byte 0x00000c68
