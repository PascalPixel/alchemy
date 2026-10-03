.syntax unified
	.thumb
	.global Camera_InitDefaultTransform
	.thumb_func
Camera_InitDefaultTransform:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #48]
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r5, #54]
	movs r3, #254
	lsls r3, r3, #8
	strh r3, [r5, #52]
	movs r3, #255
	movs r6, #0
	lsls r3, r3, #17
	str r3, [r5, #32]
	str r6, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #20]
	str r6, [r5, #28]
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
	ldr r3, .L_0811be38
	mov lr, r3
	.2byte 0xf800
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0811be38:
	.4byte IwramTransformVector
