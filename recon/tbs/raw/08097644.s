.syntax unified
	.thumb
	.global Func_08097644
	.thumb_func
Func_08097644:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0809766c
	movs r0, #165
	ldr r7, [r3]
	lsls r0, r0, #2
	adds r1, r7, r0
	ldrb r2, [r1]
	adds r3, r2, #0
	sub sp, #24
	cmp r3, #0
	beq .L_08097670
	adds r3, #255
	strb r3, [r1]
	b .L_08097856
.L_0809766c:
	.4byte gBattleBgFxWork
.L_08097670:
	ldr r1, .L_08097818
	adds r3, r7, r1
	ldrb r3, [r3]
	movs r2, #1
	eors r2, r3
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #4
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r6, r7, r3
	movs r5, #0
.L_08097688:
	movs r2, #162
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrh r0, [r3]
	lsls r3, r5, #3
	adds r0, r0, r3
	movs r1, #160
	lsls r0, r0, #16
	bl __udivsi3
	bl Trig_Sin
	adds r5, #1
	asrs r0, r0, #14
	strh r0, [r6]
	adds r6, #2
	cmp r5, #159
	bls .L_08097688
	movs r3, #162
	lsls r3, r3, #2
	adds r2, r7, r3
	ldrh r3, [r2]
	ldr r4, .L_08097818
	adds r3, #4
	strh r3, [r2]
	adds r1, r7, r4
	ldrb r3, [r1]
	movs r2, #1
	eors r3, r2
	strb r3, [r1]
	cmp r3, #0
	beq .L_08097704
	ldr r0, .L_0809781c
	movs r1, #163
	adds r3, r7, r0
	lsls r1, r1, #2
	movs r0, #0
	ldrsb r0, [r3, r0]
	adds r3, r7, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	ldr r2, .L_08097820
	lsls r3, r3, #5
	lsls r0, r0, #10
	orrs r0, r3
	adds r3, r7, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	orrs r0, r3
	movs r3, #128
	lsls r3, r3, #14
	orrs r0, r3
	movs r1, #1
	bl BattleFx_ApplyColorToTargetBuffer
	movs r0, #1
	bl BattleFx_StartBufferInterpolation
	bl BattleFx_AdvanceHueCycle
.L_08097704:
	movs r4, #164
	lsls r4, r4, #2
	adds r3, r7, r4
	ldrh r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl Animation_ApplyChildPalette
	ldr r0, .L_08097824
	adds r3, r7, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08097728
	cmp r3, #8
	beq .L_08097728
	cmp r3, #16
	bne .L_08097806
.L_08097728:
	movs r1, #164
	lsls r1, r1, #2
	adds r5, r7, r1
	ldrh r0, [r5]
	bl Object_GetById
	ldr r2, .L_08097828
	adds r2, r2, r7
	adds r6, r0, #0
	ldrh r0, [r2]
	mov r11, r2
	bl Object_GetById
	mov r9, r0
	cmp r6, #0
	beq .L_08097806
	cmp r0, #0
	beq .L_08097806
	add r3, sp, #12
	mov r10, r3
	ldr r3, [r6, #8]
	mov r4, r10
	str r3, [r4]
	ldrh r0, [r5]
	bl BattleAction_FindDescriptor
	movs r1, #0
	ldrsh r0, [r0, r1]
	bl Resource_GetMetadataRecordFar
	movs r2, #8
	ldrsb r2, [r0, r2]
	ldr r3, [r6, #12]
	lsls r2, r2, #16
	ldr r5, .L_0809782c
	adds r3, r3, r2
	adds r3, r3, r5
	mov r2, r10
	str r3, [r2, #4]
	ldr r3, [r6, #16]
	str r3, [r2, #8]
	mov r4, r9
	ldr r3, [r4, #8]
	mov r1, r11
	ldrh r0, [r1]
	mov r8, sp
	str r3, [sp, #0]
	bl BattleAction_FindDescriptor
	movs r2, #0
	ldrsh r0, [r0, r2]
	bl Resource_GetMetadataRecordFar
	mov r4, r9
	movs r3, #8
	ldrsb r3, [r0, r3]
	ldr r2, [r4, #12]
	lsls r3, r3, #16
	adds r2, r2, r3
	mov r0, r8
	adds r2, r2, r5
	str r2, [r0, #4]
	ldr r3, [r4, #16]
	str r3, [r0, #8]
	mov r4, r8
	ldr r0, .L_08097830
	ldr r1, [r4]
	bl Object_CreateFar
	adds r6, r0, #0
	cmp r6, #0
	beq .L_08097806
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	ldr r5, [r6, #80]
	strb r3, [r2]
	ldr r3, .L_08097834
	str r3, [r6, #48]
	str r3, [r6, #52]
	mov r2, r8
	mov r1, r10
	ldr r3, [r2, #8]
	ldr r0, [r1, #8]
	ldr r1, [r1]
	subs r0, r0, r3
	ldr r3, [r2]
	subs r1, r1, r3
	bl ArcTan2
	ldr r3, .L_08097838
	ldr r2, .L_08097814
	str r3, [r6, #108]
	adds r3, r5, #0
	adds r3, #38
	strb r2, [r3]
	movs r3, #13
	ldrb r2, [r5, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strh r0, [r6, #6]
	strb r3, [r5, #9]
	mov r3, r10
	ldr r1, [r3]
	ldr r2, [r3, #4]
	adds r0, r6, #0
	ldr r3, [r3, #8]
	bl Object_SetMoveTargetFar
.L_08097806:
	ldr r4, .L_08097824
	adds r5, r7, r4
	ldrb r2, [r5]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_08097844
	b .L_0809783c
.L_08097814:
	.4byte 0x00000000
.L_08097818:
	.4byte 0x0000028a
.L_0809781c:
	.4byte 0x0000028d
.L_08097820:
	.4byte 0x0000028b
.L_08097824:
	.4byte 0x00000295
.L_08097828:
	.4byte 0x00000292
.L_0809782c:
	.4byte 0xfffe0000
.L_08097830:
	.4byte 0x00000119
.L_08097834:
	.4byte 0x0000a3d7
.L_08097838:
	.4byte BattleFx_SetCallbackWhenTargetUnset
.L_0809783c:
	movs r0, #130
	bl AudioCommand_PlayFar
	ldrb r2, [r5]
.L_08097844:
	adds r3, r2, #1
	movs r0, #240
	strb r3, [r5]
	lsls r0, r0, #22
	lsls r3, r3, #24
	cmp r3, r0
	bls .L_08097856
	movs r3, #0
	strb r3, [r5]
.L_08097856:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
