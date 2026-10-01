.syntax unified
	.thumb
	.global BattleEffect_RunDitherDissolveScene
	.thumb_func
BattleEffect_RunDitherDissolveScene:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080d6a48
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #100
	str r3, [sp, #64]
	ldr r3, .L_080d6a4c
	mov r9, r1
	add r3, r9
	str r0, [r3]
	ldr r1, .L_080d6a50
	movs r0, #8
	movs r2, #1
	bl BattleFx_SpawnObjects
	movs r2, #0
	mov r10, r2
	movs r1, #128
	ldr r2, .L_080d6a54
	movs r0, #127
	lsls r1, r1, #3
.L_080d69a6:
	mov r3, r10
	ands r3, r0
	strb r3, [r2]
	movs r3, #1
	add r10, r3
	adds r2, #1
	cmp r10, r1
	bne .L_080d69a6
	movs r4, #0
	movs r0, #127
	mov r10, r4
	mov r8, r0
	mov r11, r4
.L_080d69c0:
	movs r7, #0
	mov r6, r11
.L_080d69c4:
	bl Random16
	mov r1, r8
	adds r5, r0, #0
	ands r5, r1
	bl Random16
	mov r2, r8
	ldr r3, .L_080d6a54
	ands r0, r2
	adds r0, r6, r0
	adds r5, r6, r5
	adds r0, r0, r3
	adds r5, r5, r3
	ldrb r2, [r0]
	ldrb r3, [r5]
	adds r7, #1
	strb r3, [r0]
	strb r2, [r5]
	cmp r7, #128
	bne .L_080d69c4
	movs r0, #1
	add r10, r0
	movs r4, #128
	mov r1, r10
	add r11, r4
	cmp r1, #8
	bne .L_080d69c0
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080d6a58
	ldr r3, .L_080d6a40
	strh r3, [r2]
	ldr r3, .L_080d6a44
	adds r2, #48
	strh r3, [r2]
	ldr r0, .L_080d6a5c
	mov r1, r9
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r6, #1
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r6, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r5, .L_080d6a60
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	movs r1, #7
	str r3, [sp, #68]
	movs r2, #7
	movs r3, #7
	movs r0, #47
	str r6, [sp, #0]
	b .L_080d6a64
.L_080d6a40:
	.4byte 0x00000100
.L_080d6a44:
	.4byte 0x00000000
.L_080d6a48:
	.4byte gBattleFxWork
.L_080d6a4c:
	.4byte 0x00007828
.L_080d6a50:
	.4byte 0x00000177
.L_080d6a54:
	.4byte gMapCellBuffer
.L_080d6a58:
	.4byte 0x04000020
.L_080d6a5c:
	.4byte 0x000000b2
.L_080d6a60:
	.4byte gWorkSlot
.L_080d6a64:
	bl BattleEffect_LoadWork
	adds r5, #188
	ldr r3, [r5]
	mov r2, sp
	adds r2, #68
	str r2, [sp, #36]
	str r3, [r2, #4]
	movs r3, #239
	lsls r3, r3, #7
	ldr r2, .L_080d6d6c
	add r3, r9
	str r6, [r3]
	add r2, r9
	movs r3, #0
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080d6d70
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_080d6d74
	ldr r2, .L_080d6d78
	movs r4, #90
	movs r5, #225
	str r3, [r2]
	negs r4, r4
	movs r3, #0
	lsls r5, r5, #7
	mov r10, r3
	movs r7, #7
	mov r8, r4
	movs r6, #0
	add r5, r9
.L_080d6aa8:
	mov r0, r10
	cmp r0, #4
	bgt .L_080d6aba
	str r6, [r5]
	bl Random16
	ands r0, r7
	adds r0, #104
	b .L_080d6ac6
.L_080d6aba:
	mov r1, r8
	str r1, [r5]
	bl Random16
	ands r0, r7
	adds r0, #108
.L_080d6ac6:
	str r0, [r5, #4]
	bl Random16
	ands r0, r7
	adds r0, #4
	str r0, [r5, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #16
	str r3, [r5, #24]
	movs r3, #1
	add r10, r3
	movs r2, #20
	mov r4, r10
	add r8, r2
	adds r6, #20
	adds r5, #28
	cmp r4, #16
	bne .L_080d6aa8
	ldr r5, .L_080d6d7c
	movs r0, #0
	mov r10, r0
	add r5, r9
.L_080d6af8:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	str r3, [r5]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #64
	str r3, [r5, #4]
	bl Random16
	movs r3, #7
	ands r3, r0
	movs r1, #1
	negs r3, r3
	add r10, r1
	subs r3, #8
	mov r2, r10
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #16
	bne .L_080d6af8
	movs r5, #232
	movs r3, #0
	lsls r5, r5, #7
	mov r10, r3
	movs r6, #0
	add r5, r9
.L_080d6b36:
	movs r3, #128
	lsls r3, r3, #16
	str r3, [r5]
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r5, #4]
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #200
	movs r4, #1
	negs r3, r3
	add r10, r4
	lsls r3, r3, #9
	mov r0, r10
	str r3, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #24]
	adds r5, #28
	cmp r0, #16
	bne .L_080d6b36
	ldr r5, .L_080d6d80
	mov r1, r9
	ldr r0, [r1, r5]
	bl BattleFx_SelectLivingTargets
	ldr r2, .L_080d6d84
	movs r3, #0
	str r2, [sp, #56]
	str r3, [sp, #52]
	str r3, [sp, #60]
.L_080d6b76:
	ldr r3, .L_080d6d88
	ldr r5, [r3]
	ldr r3, .L_080d6d8c
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080d6ba4
	ldr r4, [sp, #60]
	cmp r4, #190
	ble .L_080d6ba4
	ldr r0, .L_080d6d90
	cmp r4, r0
	bgt .L_080d6ba4
	movs r1, #128
	lsls r1, r1, #7
	ldr r3, .L_080d6d94
	ldr r0, [sp, #64]
	bl _call_via_r3
	movs r1, #143
	lsls r1, r1, #1
	str r1, [sp, #60]
.L_080d6ba4:
	ldr r2, [sp, #60]
	cmp r2, #224
	bne .L_080d6bb4
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #0
	str r3, [r2]
.L_080d6bb4:
	bl Render_ResetTransformState
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [sp, #60]
	cmp r3, #31
	bne .L_080d6c04
	ldr r2, .L_080d6d98
	movs r3, #8
	add r2, r9
	str r3, [r2]
	movs r0, #157
	bl AudioCommand_PlayFar
	ldr r1, .L_080d6d80
	mov r0, r9
	ldr r3, [r0, r1]
	ldr r3, [r3, #20]
	movs r4, #0
	mov r10, r4
	cmp r3, #0
	beq .L_080d6c04
	ldr r5, .L_080d6d80
	movs r6, #36
	add r5, r9
.L_080d6bec:
	ldr r3, [r5]
	movs r1, #6
	ldrsh r0, [r3, r6]
	bl BattleMotion_ApplyVariantMotionFar
	movs r3, #1
	add r10, r3
	ldr r3, [r5]
	ldr r3, [r3, #20]
	adds r6, #2
	cmp r10, r3
	bne .L_080d6bec
.L_080d6c04:
	ldr r4, [sp, #60]
	cmp r4, #72
	bne .L_080d6c10
	movs r0, #136
	bl AudioCommand_PlayFar
.L_080d6c10:
	ldr r0, [sp, #60]
	cmp r0, #140
	bne .L_080d6c1c
	movs r0, #156
	bl AudioCommand_PlayFar
.L_080d6c1c:
	ldr r1, [sp, #52]
	movs r2, #128
	ldr r3, [sp, #56]
	lsls r2, r2, #7
	adds r1, r1, r2
	movs r4, #128
	adds r3, r3, r1
	lsls r4, r4, #15
	str r1, [sp, #52]
	str r3, [sp, #56]
	cmp r3, r4
	ble .L_080d6c36
	str r4, [sp, #56]
.L_080d6c36:
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #16
	ldr r2, [sp, #56]
	bl BattleFx_PlaceFormationObjects
	ldr r0, [sp, #60]
	subs r0, #48
	cmp r0, #48
	bhi .L_080d6c76
	movs r1, #24
	bl __divsi3
	movs r1, #3
	bl __modsi3
	ldr r3, .L_080d6d9c
	ldr r6, .L_080d6da0
	adds r5, r0, #0
	add r3, r9
	lsls r5, r5, #1
	ldrb r1, [r6, r5]
	ldr r0, [r3]
	bl Object_InitializeMode
	ldr r3, .L_080d6da4
	adds r5, #1
	add r3, r9
	ldr r0, [r3]
	ldrb r1, [r6, r5]
	bl Object_InitializeMode
.L_080d6c76:
	ldr r3, [sp, #60]
	subs r3, #72
	cmp r3, #55
	bhi .L_080d6cee
	movs r6, #232
	movs r0, #0
	lsls r6, r6, #7
	mov r10, r0
	add r6, r9
.L_080d6c88:
	mov r3, r10
	ldr r1, [sp, #60]
	adds r3, #72
	cmp r1, r3
	blt .L_080d6ce2
	ldr r5, [r6, #4]
	ldr r2, .L_080d6da8
	cmp r5, r2
	bgt .L_080d6ce2
	adds r0, r1, #0
	add r0, r10
	cmp r0, #0
	bge .L_080d6ca4
	adds r0, #3
.L_080d6ca4:
	movs r1, #5
	asrs r0, r0, #2
	bl __modsi3
	ldr r4, .L_080d6dac
	lsls r1, r0, #1
	ldrh r1, [r4, r1]
	ldr r4, .L_080d6db0
	movs r3, #2
	ldrsh r2, [r6, r3]
	asrs r3, r5, #16
	ldrb r5, [r4, r0]
	lsrs r4, r5, #1
	subs r2, r2, r4
	ldr r4, .L_080d6db4
	ldrb r4, [r4, r0]
	lsrs r0, r4, #1
	add r1, r9
	subs r3, r3, r0
	str r4, [sp, #4]
	str r5, [sp, #0]
	ldr r4, [sp, #68]
	ldr r0, [sp, #64]
	bl _call_via_r4
	movs r2, #128
	adds r0, r6, #0
	movs r1, #64
	lsls r2, r2, #5
	bl EffectStep_AdvanceWithGravity2D
.L_080d6ce2:
	movs r4, #1
	add r10, r4
	mov r0, r10
	adds r6, #28
	cmp r0, #16
	bne .L_080d6c88
.L_080d6cee:
	ldr r1, [sp, #60]
	cmp r1, #128
	bne .L_080d6d52
	ldr r5, .L_080d6d7c
	movs r2, #0
	mov r10, r2
	movs r6, #255
	add r5, r9
.L_080d6cfe:
	bl Random16
	movs r1, #96
	bl __umodsi3
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #88
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #128
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	negs r0, r0
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	negs r3, r3
	subs r3, #16
	str r3, [r5, #24]
	movs r3, #1
	add r10, r3
	mov r4, r10
	adds r5, #28
	cmp r4, #48
	bne .L_080d6cfe
	ldr r2, .L_080d6d78
	movs r3, #0
	str r3, [r2]
.L_080d6d52:
	ldr r0, [sp, #60]
	subs r0, #128
	str r0, [sp, #48]
	cmp r0, #96
	bls .L_080d6d5e
	b .L_080d6eee
.L_080d6d5e:
	str r0, [sp, #44]
	cmp r0, #80
	ble .L_080d6db8
	movs r1, #80
	str r1, [sp, #44]
	b .L_080d6dc0
	.2byte 0x0000
.L_080d6d6c:
	.4byte 0x00007784
.L_080d6d70:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d6d74:
	.4byte 0xffffe000
.L_080d6d78:
	.4byte 0x04000028
.L_080d6d7c:
	.4byte 0x00007240
.L_080d6d80:
	.4byte 0x00007828
.L_080d6d84:
	.4byte 0xffc00000
.L_080d6d88:
	.4byte gCameraWork
.L_080d6d8c:
	.4byte gKeysRepeat
.L_080d6d90:
	.4byte 0x0000011d
.L_080d6d94:
	.4byte IwramClearWords
.L_080d6d98:
	.4byte 0x000077a8
.L_080d6d9c:
	.4byte 0x000077e4
.L_080d6da0:
	.4byte Data_080ee910
.L_080d6da4:
	.4byte 0x000077e8
.L_080d6da8:
	.4byte 0x0067ffff
.L_080d6dac:
	.4byte Data_080ee916
.L_080d6db0:
	.4byte Data_080ee920
.L_080d6db4:
	.4byte Data_080ee925
.L_080d6db8:
	ldr r2, .L_080d6fe8
	movs r3, #2
	add r2, r9
	str r3, [r2]
.L_080d6dc0:
	movs r7, #225
	movs r2, #0
	lsls r7, r7, #7
	mov r10, r2
	add r7, r9
.L_080d6dca:
	ldr r3, [r7, #24]
	ldr r4, [sp, #44]
	cmp r4, r3
	bgt .L_080d6dd4
	b .L_080d6ee0
.L_080d6dd4:
	mov r0, r10
	mov r11, r9
	cmp r0, #5
	ble .L_080d6de4
	movs r1, #216
	lsls r1, r1, #3
	add r1, r9
	mov r11, r1
.L_080d6de4:
	ldr r4, [sp, #44]
	subs r2, r4, r3
	ldr r3, [r7, #16]
	adds r0, r3, #0
	muls r0, r2
	str r0, [sp, #40]
	adds r6, r0, #0
	cmp r0, #184
	ble .L_080d6dfc
.L_080d6df6:
	subs r6, #64
	cmp r6, #184
	bgt .L_080d6df6
.L_080d6dfc:
	cmp r6, #119
	bgt .L_080d6e24
	movs r1, #1
	mov r0, r10
	ands r0, r1
	movs r1, #24
	ldr r3, [r7, #4]
	ldr r2, [r7]
	str r1, [sp, #0]
	movs r1, #8
	str r1, [sp, #4]
	ldr r1, [sp, #36]
	lsls r0, r0, #2
	subs r3, r3, r6
	ldr r4, [r0, r1]
	subs r3, #8
	ldr r0, [sp, #64]
	mov r1, r11
	bl _call_via_r4
.L_080d6e24:
	mov r3, r10
	movs r4, #1
	ands r3, r4
	lsls r3, r3, #2
	movs r2, #0
	str r3, [sp, #32]
	mov r8, r2
.L_080d6e32:
	ldr r1, [r7, #4]
	mov r0, r8
	lsls r3, r0, #6
	subs r2, r1, r6
	adds r5, r2, r3
	movs r3, #64
	negs r3, r3
	movs r2, #0
	movs r0, #64
	cmp r5, r3
	blt .L_080d6e80
	cmp r5, #0
	bge .L_080d6e5a
	negs r2, r5
	lsls r3, r2, #1
	adds r0, r5, #0
	adds r3, r3, r2
	lsls r2, r3, #3
	adds r0, #64
	movs r5, #0
.L_080d6e5a:
	adds r3, r5, r0
	cmp r3, r1
	ble .L_080d6e64
	subs r3, r3, r1
	subs r0, r0, r3
.L_080d6e64:
	mov r4, r11
	movs r3, #24
	adds r1, r4, r2
	ldr r2, [r7]
	str r3, [sp, #0]
	str r0, [sp, #4]
	ldr r3, [sp, #36]
	ldr r0, [sp, #32]
	adds r1, #192
	ldr r4, [r0, r3]
	ldr r0, [sp, #64]
	adds r3, r5, #0
	bl _call_via_r4
.L_080d6e80:
	movs r4, #1
	add r8, r4
	mov r0, r8
	cmp r0, #3
	bne .L_080d6e32
	mov r6, r10
	ands r6, r4
	cmp r6, #0
	beq .L_080d6ee0
	ldr r1, [sp, #40]
	ldr r5, [r7, #4]
	movs r2, #127
	subs r3, r5, r1
	ands r3, r2
	subs r3, #16
	movs r1, #3
	mov r0, r10
	mov r8, r3
	bl __modsi3
	ldr r3, .L_080d6fec
	adds r1, r0, #0
	ldrb r4, [r3, r1]
	mov r2, r8
	adds r3, r2, r4
	mov r12, r4
	cmp r3, r5
	ble .L_080d6ebc
	subs r3, r3, r5
	subs r4, r4, r3
.L_080d6ebc:
	cmp r4, #0
	ble .L_080d6ee0
	ldr r2, .L_080d6ff0
	lsls r3, r1, #1
	ldrh r1, [r2, r3]
	mov r3, r12
	ldr r2, [r7]
	str r3, [sp, #0]
	str r4, [sp, #4]
	ldr r3, [sp, #36]
	lsls r0, r6, #2
	ldr r4, [r0, r3]
	add r1, r9
	adds r2, #8
	ldr r0, [sp, #64]
	mov r3, r8
	bl _call_via_r4
.L_080d6ee0:
	movs r4, #1
	add r10, r4
	mov r0, r10
	adds r7, #28
	cmp r0, #10
	beq .L_080d6eee
	b .L_080d6dca
.L_080d6eee:
	ldr r1, [sp, #48]
	cmp r1, #95
	bhi .L_080d6f9e
	movs r5, #232
	movs r2, #0
	lsls r5, r5, #7
	mov r10, r2
	movs r6, #255
	add r5, r9
.L_080d6f00:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_080d6f8e
	movs r1, #5
	mov r0, r10
	bl __modsi3
	ldr r2, .L_080d6ff4
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #6
	ldrsh r3, [r5, r4]
	ldr r4, .L_080d6ff8
	ldrb r4, [r4, r0]
	str r4, [sp, #0]
	ldr r4, .L_080d6ffc
	ldrb r0, [r4, r0]
	add r1, r9
	str r0, [sp, #4]
	ldr r4, [sp, #68]
	ldr r0, [sp, #64]
	bl _call_via_r4
	ldr r3, [r5]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #16]
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #240
	adds r2, r2, r3
	lsls r1, r1, #15
	adds r3, r3, r0
	str r2, [r5, #4]
	str r3, [r5, #16]
	cmp r2, r1
	bls .L_080d6f8c
	ldr r2, [sp, #60]
	cmp r2, #159
	bgt .L_080d6f8c
	bl Random16
	movs r1, #96
	bl __umodsi3
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #88
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #128
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	negs r0, r0
	lsls r0, r0, #11
	str r0, [r5, #16]
.L_080d6f8c:
	ldr r3, [r5, #24]
.L_080d6f8e:
	adds r3, #1
	str r3, [r5, #24]
	movs r3, #1
	add r10, r3
	mov r4, r10
	adds r5, #28
	cmp r4, #32
	bne .L_080d6f00
.L_080d6f9e:
	ldr r3, [sp, #60]
	subs r3, #224
	cmp r3, #23
	bhi .L_080d7034
	ldr r0, [sp, #60]
	movs r3, #3
	ands r3, r0
	cmp r3, #0
	bne .L_080d7034
	ldr r2, .L_080d6fe4
	movs r4, #160
	movs r1, #0
	lsls r4, r4, #19
	mov r10, r1
	mov r8, r2
.L_080d6fbc:
	ldrh r3, [r4]
	movs r7, #31
	ands r7, r3
	lsls r3, r3, #16
	mov r0, r8
	lsrs r6, r3, #21
	lsrs r5, r3, #26
	ands r6, r0
	ands r5, r0
	adds r0, r7, r6
	adds r0, r0, r5
	movs r1, #3
	str r4, [sp, #8]
	bl __divsi3
	ldr r4, [sp, #8]
	cmp r7, r0
	ble .L_080d7000
	subs r7, #1
	b .L_080d7000
.L_080d6fe4:
	.4byte 0x0000001f
.L_080d6fe8:
	.4byte 0x000077a8
.L_080d6fec:
	.4byte Data_080ee930
.L_080d6ff0:
	.4byte Data_080ee92a
.L_080d6ff4:
	.4byte Data_080ee934
.L_080d6ff8:
	.4byte Data_080ee93e
.L_080d6ffc:
	.4byte Data_080ee943
.L_080d7000:
	cmp r7, r0
	bge .L_080d7006
	adds r7, #1
.L_080d7006:
	cmp r6, r0
	ble .L_080d700c
	subs r6, #1
.L_080d700c:
	cmp r6, r0
	bge .L_080d7012
	adds r6, #1
.L_080d7012:
	cmp r5, r0
	ble .L_080d7018
	subs r5, #1
.L_080d7018:
	cmp r5, r0
	bge .L_080d701e
	adds r5, #1
.L_080d701e:
	lsls r2, r6, #5
	lsls r3, r5, #10
	movs r1, #1
	orrs r3, r2
	add r10, r1
	orrs r3, r7
	mov r2, r10
	strh r3, [r4]
	adds r4, #2
	cmp r2, #64
	bne .L_080d6fbc
.L_080d7034:
	ldr r3, [sp, #48]
	cmp r3, #172
	bhi .L_080d710e
	ldr r2, .L_080d7230
	mov r0, r9
	ldr r3, [r0, r2]
	ldr r3, [r3, #20]
	movs r4, #0
	mov r10, r4
	cmp r3, #0
	beq .L_080d710e
	add r1, sp, #76
	mov r8, r1
	add r6, sp, #88
	mov r11, r4
.L_080d7052:
	mov r4, r10
	mov r3, r9
	ldr r2, [r3, r2]
	lsls r3, r4, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlotFar
	ldr r2, [r0]
	ldr r3, [r2, #8]
	str r3, [r6]
	ldr r3, [r2, #12]
	str r3, [r6, #4]
	ldr r3, [r2, #16]
	adds r0, r6, #0
	str r3, [r6, #8]
	mov r1, r8
	bl EffectPosition_ApplyBaseAndYOffset
	mov r2, r11
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	ldr r4, .L_080d7234
	add r3, r9
	movs r7, #0
	adds r5, r3, r4
.L_080d7088:
	ldr r0, [r5, #24]
	cmp r0, #0
	bne .L_080d70b0
	bl Random16
	movs r1, #15
	ldr r3, [sp, #76]
	ands r0, r1
	adds r3, r3, r0
	subs r3, #8
	str r3, [r5]
	bl Random16
	movs r2, #15
	ldr r3, [sp, #80]
	ands r0, r2
	adds r3, r3, r0
	subs r3, #40
	str r3, [r5, #4]
	ldr r0, [r5, #24]
.L_080d70b0:
	cmp r0, #4
	bhi .L_080d70d8
	ldr r2, .L_080d7238
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_080d723c
	ldrb r0, [r3, r0]
	ldr r2, [r5]
	ldr r3, [r5, #4]
	lsrs r4, r0, #1
	subs r2, r2, r4
	subs r3, r3, r4
	str r0, [sp, #0]
	str r0, [sp, #4]
	add r1, r9
	ldr r4, [sp, #68]
	ldr r0, [sp, #64]
	bl _call_via_r4
	ldr r0, [r5, #24]
.L_080d70d8:
	adds r3, r0, #1
	str r3, [r5, #24]
	ldr r4, [sp, #60]
	cmp r4, #199
	bgt .L_080d70f2
	cmp r3, #5
	bne .L_080d70f2
	bl Random16
	movs r3, #7
	ands r3, r0
	negs r3, r3
	str r3, [r5, #24]
.L_080d70f2:
	adds r7, #1
	adds r5, #28
	cmp r7, #6
	bne .L_080d7088
	ldr r2, .L_080d7230
	mov r4, r9
	ldr r3, [r4, r2]
	movs r1, #1
	ldr r3, [r3, #20]
	movs r0, #6
	add r10, r1
	add r11, r0
	cmp r10, r3
	bne .L_080d7052
.L_080d710e:
	ldr r0, [sp, #60]
	cmp r0, #232
	ble .L_080d71dc
	ldr r1, .L_080d7240
	lsls r3, r0, #1
	adds r6, r3, r1
	movs r2, #0
	movs r3, #0
	mov r10, r2
	mov r12, r3
	adds r4, r6, #0
.L_080d7124:
	movs r7, #0
.L_080d7126:
	cmp r4, #127
	bhi .L_080d7168
	movs r5, #7
	adds r0, r4, #0
	ands r0, r5
	lsls r3, r0, #5
	add r3, r10
	ldr r1, .L_080d7244
	lsls r3, r3, #2
	adds r3, r3, r7
	adds r3, r3, r1
	ldrb r1, [r3]
	adds r3, r4, #0
	cmp r4, #0
	bge .L_080d7146
	adds r3, r4, #7
.L_080d7146:
	asrs r3, r3, #3
	adds r2, r1, #0
	cmp r1, #0
	bge .L_080d7150
	adds r2, r1, #7
.L_080d7150:
	asrs r2, r2, #3
	lsls r3, r3, #4
	adds r3, r3, r2
	lsls r3, r3, #3
	adds r3, r3, r0
	ldr r2, [sp, #64]
	ands r1, r5
	lsls r3, r3, #3
	adds r3, r3, r1
	adds r3, r2, r3
	mov r0, r12
	strb r0, [r3]
.L_080d7168:
	adds r7, #1
	cmp r7, #4
	bne .L_080d7126
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r4, #1
	cmp r2, #32
	bne .L_080d7124
	movs r4, #0
	movs r3, #0
	mov lr, r4
	mov r10, r3
	adds r4, r6, #1
.L_080d7184:
	movs r7, #0
	mov r12, r4
.L_080d7188:
	cmp r4, #127
	bhi .L_080d71ca
	movs r5, #7
	mov r0, r12
	ands r0, r5
	lsls r3, r0, #5
	add r3, r10
	ldr r1, .L_080d7244
	lsls r3, r3, #2
	adds r3, r3, r7
	adds r3, r3, r1
	ldrb r1, [r3]
	mov r3, r12
	cmp r3, #0
	bge .L_080d71a8
	adds r3, #7
.L_080d71a8:
	asrs r3, r3, #3
	adds r2, r1, #0
	cmp r1, #0
	bge .L_080d71b2
	adds r2, r1, #7
.L_080d71b2:
	asrs r2, r2, #3
	lsls r3, r3, #4
	adds r3, r3, r2
	lsls r3, r3, #3
	adds r3, r3, r0
	ldr r2, [sp, #64]
	ands r1, r5
	lsls r3, r3, #3
	adds r3, r3, r1
	adds r3, r2, r3
	mov r0, lr
	strb r0, [r3]
.L_080d71ca:
	adds r7, #1
	cmp r7, #4
	bne .L_080d7188
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r4, #1
	cmp r2, #32
	bne .L_080d7184
.L_080d71dc:
	ldr r3, [sp, #60]
	subs r3, #161
	cmp r3, #62
	bhi .L_080d726c
	ldr r2, .L_080d7230
	movs r3, #0
	mov r4, r9
	mov r10, r3
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080d726c
.L_080d71f4:
	mov r0, r10
	lsls r3, r0, #3
	ldr r1, [sp, #60]
	adds r3, #160
	cmp r1, r3
	ble .L_080d725c
	mov r3, r9
	ldr r2, [r3, r2]
	lsls r3, r0, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlotFar
	adds r6, r0, #0
	ldr r2, [r6]
	movs r0, #128
	ldr r3, [r2, #12]
	lsls r0, r0, #12
	movs r1, #128
	adds r3, r3, r0
	lsls r1, r1, #16
	str r3, [r2, #12]
	cmp r3, r1
	ble .L_080d7226
	str r1, [r2, #12]
.L_080d7226:
	movs r3, #0
	movs r5, #0
	str r3, [r2, #72]
	b .L_080d7250
	.2byte 0x0000
.L_080d7230:
	.4byte 0x00007828
.L_080d7234:
	.4byte 0x00007240
.L_080d7238:
	.4byte Data_080ee948
.L_080d723c:
	.4byte Data_080ee952
.L_080d7240:
	.4byte 0xfffffe10
.L_080d7244:
	.4byte gMapCellBuffer
.L_080d7248:
	movs r1, #5
	bl Object_InitializeMode
	adds r5, #1
.L_080d7250:
	ldr r0, [r6]
	adds r1, r5, #0
	bl GetMotionRecordFar
	cmp r0, #0
	bne .L_080d7248
.L_080d725c:
	ldr r2, .L_080d7388
	mov r4, r9
	ldr r3, [r4, r2]
	movs r1, #1
	ldr r3, [r3, #20]
	add r10, r1
	cmp r10, r3
	bne .L_080d71f4
.L_080d726c:
	ldr r2, .L_080d7388
	mov r1, r9
	ldr r3, [r1, r2]
	ldr r3, [r3, #20]
	movs r0, #0
	mov r10, r0
	cmp r3, #0
	beq .L_080d72e0
	movs r5, #143
	movs r7, #8
	movs r6, #36
	lsls r5, r5, #1
.L_080d7284:
	ldr r3, [sp, #60]
	cmp r3, r5
	bne .L_080d72a0
	mov r4, r9
	ldr r3, [r4, r2]
	ldrsh r0, [r3, r6]
	bl GetBattleObjectSlotFar
	movs r3, #192
	ldr r2, [r0]
	lsls r3, r3, #15
	str r3, [r2, #12]
	ldr r3, .L_080d738c
	str r3, [r2, #72]
.L_080d72a0:
	adds r3, r5, #0
	ldr r2, [sp, #60]
	adds r3, #16
	cmp r2, r3
	bne .L_080d72cc
	ldr r3, .L_080d7388
	add r3, r9
	ldr r3, [r3]
	movs r2, #1
	ldrsh r0, [r3, r6]
	movs r1, #7
	mov r3, r10
	negs r2, r2
	str r7, [sp, #0]
	bl ObjectGroup_UpdateMembers
	movs r0, #134
	bl AudioCommand_PlayFar
	ldr r3, .L_080d7390
	add r3, r9
	str r7, [r3]
.L_080d72cc:
	ldr r2, .L_080d7388
	mov r1, r9
	ldr r3, [r1, r2]
	movs r0, #1
	ldr r3, [r3, #20]
	add r10, r0
	adds r6, #2
	adds r5, #5
	cmp r10, r3
	bne .L_080d7284
.L_080d72e0:
	movs r3, #151
	ldr r2, [sp, #60]
	lsls r3, r3, #1
	cmp r2, r3
	beq .L_080d72ec
	b .L_080d7430
.L_080d72ec:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_080d7394
	mov r1, r9
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #180
	lsls r1, r1, #5
	ldr r0, .L_080d7398
	add r1, r9
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r6, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r6, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r5, .L_080d739c
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	movs r1, #7
	str r3, [sp, #68]
	movs r2, #7
	movs r3, #7
	movs r0, #47
	str r6, [sp, #0]
	bl BattleEffect_LoadWork
	adds r5, #188
	ldr r4, [sp, #36]
	ldr r3, [r5]
	ldr r2, .L_080d73a0
	str r3, [r4, #4]
	ldr r3, .L_080d7380
	strh r3, [r2]
	ldr r3, .L_080d7384
	subs r2, #48
	strh r3, [r2]
	adds r2, #8
	movs r3, #0
	str r3, [r2]
	movs r3, #239
	lsls r3, r3, #7
	ldr r2, .L_080d73a4
	add r3, r9
	str r6, [r3]
	add r2, r9
	movs r3, #75
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080d73a8
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080d7388
	mov r1, r9
	ldr r3, [r1, r2]
	ldr r3, [r3, #20]
	movs r0, #0
	mov r10, r0
	b .L_080d73ac
	.2byte 0x0000
.L_080d7380:
	.4byte 0x00003f46
.L_080d7384:
	.4byte 0x00000080
.L_080d7388:
	.4byte 0x00007828
.L_080d738c:
	.4byte 0x0000ab85
.L_080d7390:
	.4byte 0x000077a8
.L_080d7394:
	.4byte 0x00000098
.L_080d7398:
	.4byte 0x000000c0
.L_080d739c:
	.4byte gWorkSlot
.L_080d73a0:
	.4byte 0x04000050
.L_080d73a4:
	.4byte 0x00007784
.L_080d73a8:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d73ac:
	cmp r3, #0
	beq .L_080d7430
.L_080d73b0:
	mov r4, r10
	mov r3, r9
	ldr r2, [r3, r2]
	lsls r3, r4, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlotFar
	mov r3, r10
	lsls r2, r3, #2
	add r2, r10
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r0, [r0]
	lsls r3, r3, #3
	movs r4, #225
	add r3, r9
	lsls r4, r4, #7
	mov r8, r0
	movs r7, #0
	adds r6, r3, r4
.L_080d73da:
	mov r0, r8
	ldr r3, [r0, #8]
	str r3, [r6]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r6, #4]
	ldr r3, [r0, #16]
	str r3, [r6, #8]
	ldr r3, .L_080d7634
	adds r5, r7, #0
	muls r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	lsls r0, r0, #2
	str r0, [r6, #12]
	bl Random16
	ldr r3, .L_080d7638
	movs r1, #128
	ands r3, r0
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r6, #16]
	adds r0, r5, #0
	bl Trig_Cos
	adds r7, #1
	lsls r0, r0, #2
	movs r3, #0
	str r0, [r6, #20]
	str r3, [r6, #24]
	adds r6, #28
	cmp r7, #10
	bne .L_080d73da
	movs r2, #1
	add r10, r2
	ldr r2, .L_080d763c
	mov r4, r9
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r10, r3
	bne .L_080d73b0
.L_080d7430:
	ldr r0, [sp, #60]
	ldr r1, .L_080d7640
	cmp r0, r1
	bgt .L_080d743a
	b .L_080d7594
.L_080d743a:
	movs r2, #0
	mov r10, r2
	ldr r2, .L_080d763c
	mov r4, r9
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	bne .L_080d744c
	b .L_080d7594
.L_080d744c:
	ldr r1, [sp, #60]
	ldr r3, .L_080d7644
	mov r0, sp
	adds r0, #76
	adds r3, r1, r3
	movs r4, #151
	mov r1, sp
	str r0, [sp, #24]
	lsls r4, r4, #1
	movs r0, #0
	adds r1, #88
	str r3, [sp, #20]
	str r4, [sp, #16]
	str r0, [sp, #12]
	str r1, [sp, #28]
.L_080d746a:
	ldr r3, [sp, #60]
	ldr r4, [sp, #16]
	cmp r3, r4
	blt .L_080d74f8
	ldr r0, [sp, #12]
	movs r1, #157
	lsls r1, r1, #1
	ldr r4, [sp, #60]
	adds r3, r0, r1
	cmp r4, r3
	bge .L_080d74f8
	ldr r0, [sp, #20]
	mov r1, r9
	mov r4, r10
	ldr r2, [r1, r2]
	lsls r3, r4, #1
	adds r3, #36
	lsrs r6, r0, #31
	adds r6, r0, r6
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlotFar
	ldr r2, [r0]
	ldr r4, [sp, #28]
	ldr r3, [r2, #8]
	str r3, [r4]
	movs r3, #0
	str r3, [r4, #4]
	ldr r3, [r2, #16]
	str r3, [r4, #8]
	ldr r0, [sp, #24]
	mov r8, r0
	mov r1, r8
	ldr r0, [sp, #28]
	bl EffectPosition_ApplyBaseAndYOffset
	asrs r6, r6, #1
	mov r1, r8
	ldr r2, [r1]
	lsls r5, r6, #4
	subs r5, r5, r6
	ldr r3, [r1, #4]
	asrs r2, r2, #1
	lsls r5, r5, #5
	str r2, [r1]
	add r5, r9
	movs r4, #20
	movs r0, #24
	subs r2, #20
	subs r3, #24
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #68]
	adds r1, r5, #0
	ldr r0, [sp, #64]
	bl _call_via_r4
	mov r1, r8
	movs r4, #20
	movs r0, #24
	ldr r3, [r1, #4]
	ldr r2, [r1]
	str r0, [sp, #4]
	ldr r1, [sp, #36]
	str r4, [sp, #0]
	subs r3, #24
	ldr r4, [r1, #4]
	ldr r0, [sp, #64]
	adds r1, r5, #0
	bl _call_via_r4
.L_080d74f8:
	ldr r3, [sp, #16]
	ldr r2, [sp, #60]
	adds r3, #6
	cmp r2, r3
	blt .L_080d7570
	ldr r2, [sp, #12]
	add r3, sp, #76
	add r2, r10
	mov r8, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #3
	movs r4, #225
	add r3, r9
	lsls r4, r4, #7
	movs r0, #12
	movs r7, #0
	mov r6, r8
	adds r5, r3, r4
	mov r11, r0
.L_080d7520:
	adds r0, r5, #0
	adds r1, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r6]
	asrs r2, r3, #1
	str r2, [r6]
	ldr r3, [r5, #24]
	cmp r3, #26
	bhi .L_080d7556
	ldr r3, .L_080d7648
	mov r4, r11
	ldrh r1, [r3, r4]
	ldr r3, .L_080d764c
	ldrh r4, [r3, r4]
	mov r3, r8
	ldr r3, [r3, #4]
	lsrs r0, r4, #1
	subs r2, r2, r0
	subs r3, r3, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	add r1, r9
	ldr r4, [sp, #68]
	ldr r0, [sp, #64]
	bl _call_via_r4
.L_080d7556:
	movs r2, #128
	adds r0, r5, #0
	movs r1, #60
	lsls r2, r2, #5
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r5, #24]
	adds r7, #1
	adds r3, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #5
	bne .L_080d7520
.L_080d7570:
	ldr r4, [sp, #20]
	ldr r0, [sp, #16]
	ldr r1, [sp, #12]
	subs r4, #4
	movs r2, #1
	adds r0, #4
	adds r1, #4
	str r4, [sp, #20]
	str r0, [sp, #16]
	str r1, [sp, #12]
	add r10, r2
	ldr r2, .L_080d763c
	mov r4, r9
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r10, r3
	beq .L_080d7594
	b .L_080d746a
.L_080d7594:
	ldr r0, [sp, #60]
	cmp r0, #127
	bgt .L_080d75a4
	movs r0, #4
	movs r1, #16
	bl Camera_ApplyShake
	b .L_080d75be
.L_080d75a4:
	ldr r1, [sp, #60]
	ldr r2, .L_080d7640
	cmp r1, r2
	bgt .L_080d75b6
	movs r0, #2
	movs r1, #2
	bl Camera_ApplyShake
	b .L_080d75be
.L_080d75b6:
	movs r0, #4
	movs r1, #8
	bl Camera_ApplyShake
.L_080d75be:
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080d7650
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #60]
	movs r4, #183
	adds r3, #1
	lsls r4, r4, #1
	str r3, [sp, #60]
	cmp r3, r4
	beq .L_080d75e2
	bl .L_080d6b76
.L_080d75e2:
	ldr r0, .L_080d7654
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #16
	ldr r2, [sp, #56]
	ldr r5, .L_080d7658
	bl BattleEffect_RunImpactBurst
	movs r0, #0
	mov r10, r0
	add r5, r9
.L_080d760e:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #8
	bne .L_080d760e
	bl BattleFx_EndCanvasLayer
	add sp, #100
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080d7634:
	.4byte 0x00003334
.L_080d7638:
	.4byte 0x00007fff
.L_080d763c:
	.4byte 0x00007828
.L_080d7640:
	.4byte 0x0000012d
.L_080d7644:
	.4byte 0xfffffed2
.L_080d7648:
	.4byte Data_080ee958
.L_080d764c:
	.4byte Data_080ee966
.L_080d7650:
	.4byte 0x00007824
.L_080d7654:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d7658:
	.4byte 0x000077d8
