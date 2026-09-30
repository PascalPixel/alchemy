.syntax unified
	.thumb
	.global Func_080e37e0
	.thumb_func
Func_080e37e0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	bl Object_GetById
	ldr r3, .L_080e38c4
	adds r7, r0, #0
	ldrh r2, [r7, #6]
	str r3, [r7, #108]
	movs r3, #0
	str r3, [r7, #24]
	mov r10, r2
	bl Func_080d22a8
	movs r0, #0
	bl Func_080cded4
	mov r0, r8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	bl Func_080d2a3c
	bl Func_080d2a8c
	movs r0, #140
	ldr r3, [r7, #16]
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	lsls r0, r0, #1
	bl Object_Spawn
	movs r1, #2
	adds r5, r0, #0
	bl Object_SetMode
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	movs r3, #192
	lsls r3, r3, #8
	str r3, [r5, #28]
	movs r6, #31
.L_080e383e:
	ldr r3, [r5, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	movs r2, #128
	lsls r2, r2, #2
	adds r3, r3, r2
	str r3, [r5, #28]
	movs r0, #1
	subs r6, #1
	bl WaitFrames
	cmp r6, #0
	bge .L_080e383e
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #24]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #28]
	movs r0, #136
	bl Audio_PlayCue
	adds r0, r5, #0
	movs r1, #6
	bl Object_SetMode
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r6, #15
.L_080e3882:
	ldr r3, [r7, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r0, #1
	subs r6, #1
	bl WaitFrames
	cmp r6, #0
	bge .L_080e3882
	mov r0, r8
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26Far
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Func_080200c8
	movs r3, #0
	str r3, [r7, #108]
	mov r3, r10
	strh r3, [r7, #6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e38c4:
	.4byte Func_080e3060
