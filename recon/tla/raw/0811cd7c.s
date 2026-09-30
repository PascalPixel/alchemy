.syntax unified
	.thumb
	.global Camera_ConfigureScene
	.thumb_func
Camera_ConfigureScene:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #48]
	adds r3, #176
	ldr r3, [r3]
	movs r6, #0
	mov r8, r3
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r5, #16]
	movs r3, #128
	mov r2, r8
	lsls r3, r3, #7
	str r6, [r5, #12]
	str r6, [r5, #20]
	str r3, [r2]
	strh r3, [r5, #54]
	movs r3, #244
	lsls r3, r3, #8
	strh r3, [r5, #52]
	ldr r3, .L_0811ce38
	str r6, [r5, #28]
	str r3, [r5, #32]
	str r6, [r5, #24]
	sub sp, #16
	mov r10, r0
	bl Func_08014de4
	adds r0, r5, #0
	adds r0, #12
	bl Func_08015128
	movs r3, #54
	ldrsh r0, [r5, r3]
	bl Func_08015068
	movs r2, #52
	ldrsh r0, [r5, r2]
	bl Func_08015024
	add r0, sp, #4
	str r6, [r0]
	str r6, [r0, #4]
	adds r1, r5, #0
	ldr r3, [r5, #32]
	movs r5, #1
	str r3, [r0, #8]
	ldr r3, .L_0811ce3c
	mov lr, r3
	.2byte 0xf800
	movs r1, #192
	ldr r3, .L_0811ce40
	lsls r1, r1, #8
	ldr r0, .L_0811ce44
	mov lr, r3
	.2byte 0xf800
	ldr r2, .L_0811ce48
	adds r1, r0, #0
	movs r0, #0
	bl Camera_StoreSceneParameters
	ldr r2, .L_0811ce4c
	mov r3, r10
	adds r3, #120
	str r3, [r2, #16]
	movs r1, #118
	mov r2, r10
	mov r3, r8
	subs r1, r1, r2
	movs r2, #128
	str r5, [r3, #16]
	lsls r2, r2, #10
	movs r0, #240
	movs r3, #128
	str r2, [sp, #0]
	lsls r0, r0, #15
	lsls r1, r1, #16
	lsls r3, r3, #4
	movs r2, #0
	bl Func_08126548
	mov r3, r8
	str r5, [r3, #20]
	str r6, [r3, #16]
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0811ce38:
	.4byte 0x02ee0000
.L_0811ce3c:
	.4byte IwramTransformVector
.L_0811ce40:
	.4byte IwramRatioMulQ14
.L_0811ce44:
	.4byte 0x03c90000
.L_0811ce48:
	.4byte 0x07920000
.L_0811ce4c:
	.4byte gCameraSceneParameters
