.syntax unified
	.thumb
	.global Func_080e3698
	.thumb_func
Func_080e3698:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	str r0, [sp, #20]
	bl Object_GetById
	adds r7, r0, #0
	ldrh r1, [r7, #6]
	adds r2, r7, #0
	adds r2, #85
	str r1, [sp, #16]
	str r2, [sp, #8]
	ldrb r3, [r2]
	movs r2, #1
	negs r2, r2
	str r3, [sp, #12]
	adds r0, r2, #0
	adds r1, r2, #0
	movs r3, #0
	bl Motion_CamBounds
	ldr r1, [sp, #8]
	movs r3, #0
	strb r3, [r1]
	ldr r3, .L_080e37d8
	movs r0, #140
	ldr r2, [r7, #12]
	str r3, [r7, #108]
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	lsls r0, r0, #1
	bl Object_Spawn
	movs r1, #2
	mov r8, r0
	bl Object_SetMode
	movs r3, #192
	mov r2, r8
	lsls r3, r3, #9
	str r3, [r2, #24]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r2, #28]
	ldr r3, [r7, #8]
	ldr r1, [r7, #12]
	ldr r2, [r7, #16]
	mov r9, r3
	movs r3, #0
	mov r11, r1
	str r2, [sp, #4]
	str r3, [sp, #0]
	bl Func_080d22a8
	movs r0, #0
	bl Func_080cded4
	bl Func_080d2a3c
	movs r1, #0
	mov r10, r1
	add r6, sp, #24
.L_080e371e:
	mov r3, r10
	lsls r5, r3, #7
	mov r2, r9
	adds r0, r5, #0
	str r2, [r6]
	bl Trig_Cos
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #5
	add r3, r11
	str r3, [r6, #4]
	ldr r1, [sp, #4]
	adds r0, r5, #0
	str r1, [r6, #8]
	bl Trig_Cos
	adds r2, r6, #0
	ldr r1, [sp, #0]
	lsls r0, r0, #5
	bl Vector_AddPolarOffset
	ldr r2, [r6]
	movs r1, #128
	str r2, [r7, #8]
	lsls r1, r1, #3
	ldr r3, [r6, #4]
	movs r0, #1
	str r3, [r7, #12]
	ldr r3, [r6, #8]
	str r3, [r7, #16]
	ldr r3, [sp, #0]
	adds r3, r3, r1
	str r3, [sp, #0]
	mov r3, r8
	str r2, [r3, #8]
	ldr r3, [r7, #12]
	ldr r1, .L_080e37dc
	mov r2, r8
	adds r3, r3, r1
	str r3, [r2, #12]
	ldr r3, [r7, #16]
	str r3, [r2, #16]
	bl WaitFrames
	movs r3, #1
	add r10, r3
	mov r1, r10
	cmp r1, #127
	ble .L_080e371e
	ldr r3, [sp, #4]
	mov r2, r11
	mov r1, r9
	adds r0, r7, #0
	bl Object_SetPositionAndResetMotionFar
	movs r0, #136
	bl Audio_PlayCue
	movs r1, #6
	mov r0, r8
	bl Object_SetMode
	movs r0, #15
	bl Battle_WaitMode0
	mov r0, r8
	bl Object_Destroy
	add r2, sp, #16
	ldrh r2, [r2]
	movs r3, #0
	str r3, [r7, #108]
	strh r2, [r7, #6]
	add r3, sp, #12
	ldr r1, [sp, #8]
	ldrb r3, [r3]
	movs r0, #1
	strb r3, [r1]
	bl WaitFrames
	movs r1, #1
	ldr r0, [sp, #20]
	bl Object_AttachWorkTargetToObject
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e37d8:
	.4byte Func_080e3060
.L_080e37dc:
	.4byte 0xfffa0000
