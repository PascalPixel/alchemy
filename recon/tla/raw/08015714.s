.syntax unified
	.thumb
	.global Func_08015714
	.thumb_func
Func_08015714:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	lsrs r3, r5, #31
	adds r5, r5, r3
	asrs r5, r5, #1
	adds r0, r5, #0
	mov r8, r1
	mov r10, r2
	bl Trig_Sin
	movs r2, #128
	lsls r2, r2, #7
	adds r5, r5, r2
	adds r6, r0, #0
	adds r0, r5, #0
	bl Trig_Sin
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #4
	ldr r3, .L_08015760
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_08015764
	mov r2, r8
	str r2, [r3, #4]
	mov r2, r10
	str r0, [r3]
	str r2, [r3, #8]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08015760:
	.4byte IwramRatioMulQ14
.L_08015764:
	.4byte gCameraSceneParameters
