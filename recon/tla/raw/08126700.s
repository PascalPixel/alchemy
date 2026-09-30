.syntax unified
	.thumb
	.global Func_08126700
	.thumb_func
Func_08126700:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	mov r11, r2
	mov r8, r3
	movs r2, #12
	add r2, r8
	adds r5, r0, #0
	mov r9, r1
	lsls r0, r4, #16
	movs r1, #100
	sub sp, #40
	mov r10, r2
	bl Math_Div
	mov r3, r10
	mov r2, r9
	str r2, [r3, #4]
	mov r2, r11
	str r2, [r3, #8]
	ldr r2, .L_081267f8
	str r5, [r3]
	movs r6, #255
	add r3, sp, #4
	movs r5, #0
	lsls r6, r6, #17
	movs r1, #192
	str r5, [r3]
	str r5, [r3, #4]
	str r5, [r3, #8]
	adds r7, r0, #0
	mov r11, r2
	adds r0, r6, #0
	lsls r1, r1, #8
	mov r9, r3
	mov lr, r11
	.2byte 0xf800
	lsls r2, r6, #1
	adds r1, r0, #0
	adds r0, r6, #0
	bl Camera_StoreSceneParameters
	bl Func_08014de4
	mov r0, r10
	bl SceneTransform_ApplyPosition
	mov r2, r8
	movs r3, #54
	ldrsh r0, [r2, r3]
	bl Func_08015068
	mov r2, r8
	movs r3, #52
	ldrsh r0, [r2, r3]
	bl SceneTransform_ApplyPitch
	add r0, sp, #28
	str r5, [r0]
	str r5, [r0, #4]
	mov r2, r8
	ldr r3, [r2, #32]
	mov r1, r8
	str r3, [r0, #8]
	ldr r3, .L_081267fc
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_08126800
	movs r5, #120
	str r5, [r3, #12]
	str r5, [r3, #16]
	bl Func_08014de4
	mov r0, r8
	mov r1, r10
	bl Func_080156e8
	add r6, sp, #16
	adds r1, r6, #0
	mov r0, r9
	bl Func_08015778
	ldr r3, [r6, #4]
	ldr r2, [r6]
	movs r1, #240
	subs r2, r5, r2
	subs r5, r5, r3
	lsls r5, r5, #8
	adds r3, r5, #0
	lsls r1, r1, #15
	lsls r5, r7, #8
	lsls r2, r2, #8
	adds r0, r1, #0
	subs r5, r5, r7
	str r7, [sp, #0]
	lsls r6, r5, #1
	bl BattleCamera_SetRange
	movs r1, #192
	adds r0, r6, #0
	lsls r1, r1, #8
	mov lr, r11
	.2byte 0xf800
	lsls r5, r5, #2
	adds r1, r0, #0
	adds r2, r5, #0
	adds r0, r6, #0
	bl Camera_StoreSceneParameters
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081267f8:
	.4byte IwramRatioMulQ14
.L_081267fc:
	.4byte IwramTransformVector
.L_08126800:
	.4byte gCameraSceneParameters
