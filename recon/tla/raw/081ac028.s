.syntax unified
	.thumb
	.global Func_081ac028
	.thumb_func
Func_081ac028:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #48]
	movs r3, #152
	lsls r3, r3, #8
	strh r3, [r5, #52]
	movs r3, #255
	lsls r3, r3, #17
	str r3, [r5, #32]
	ldr r3, .L_081ac0a0
	movs r6, #0
	str r6, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #20]
	strh r6, [r5, #54]
	str r6, [r5, #28]
	str r6, [r3, #12]
	str r6, [r3, #16]
	str r6, [r5, #24]
	sub sp, #12
	bl Func_08014de4
	adds r0, r5, #0
	adds r0, #12
	bl SceneTransform_ApplyPosition
	movs r3, #54
	ldrsh r0, [r5, r3]
	bl Func_08015068
	movs r3, #52
	ldrsh r0, [r5, r3]
	bl SceneTransform_ApplyPitch
	mov r0, sp
	str r6, [r0]
	str r6, [r0, #4]
	adds r1, r5, #0
	ldr r3, [r5, #32]
	str r3, [r0, #8]
	ldr r3, .L_081ac0a4
	mov lr, r3
	.2byte 0xf800
	movs r0, #250
	movs r1, #192
	ldr r3, .L_081ac0a8
	lsls r1, r1, #8
	lsls r0, r0, #16
	mov lr, r3
	.2byte 0xf800
	adds r1, r0, #0
	movs r0, #250
	lsls r0, r0, #16
	ldr r2, .L_081ac0ac
	bl Camera_StoreSceneParameters
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_081ac0a0:
	.4byte gCameraSceneParameters
.L_081ac0a4:
	.4byte IwramTransformVector
.L_081ac0a8:
	.4byte IwramRatioMulQ14
.L_081ac0ac:
	.4byte 0x7fff0000
