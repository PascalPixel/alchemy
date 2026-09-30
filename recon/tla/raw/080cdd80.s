.syntax unified
	.thumb
	.global Func_080cdd80
	.thumb_func
Func_080cdd80:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_080cdea0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	movs r2, #12
	lsrs r6, r3, #12
	adds r3, r6, #2
	ands r3, r2
	lsls r6, r3, #12
	ldr r3, [r5, #8]
	mov r7, sp
	str r3, [r7]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #11
	adds r3, r3, r2
	ldr r2, .L_080cdea4
	adds r1, r6, #0
	ands r3, r2
	str r3, [r7, #4]
	adds r2, r7, #0
	ldr r3, [r5, #16]
	str r3, [r7, #8]
	movs r3, #128
	lsls r3, r3, #13
	mov r8, r3
	mov r0, r8
	bl Vector_AddPolarOffset
	movs r1, #1
	adds r0, r7, #0
	bl Func_080eaf28
	ldr r3, [r0, #8]
	mov r10, r0
	str r3, [r7]
	adds r1, r6, #0
	ldr r3, [r0, #12]
	mov r6, r10
	str r3, [r7, #4]
	adds r2, r7, #0
	ldr r3, [r0, #16]
	adds r6, #34
	mov r0, r8
	str r3, [r7, #8]
	bl Vector_AddPolarOffset
	ldrb r0, [r6]
	movs r3, #2
	mov r8, r0
	strb r3, [r6]
	mov r0, r10
	adds r1, r7, #0
	bl Func_08020210
	mov r2, r8
	strb r2, [r6]
	cmp r0, #0
	bgt .L_080cde94
	movs r1, #8
	adds r0, r5, #0
	bl Object_SetMode
	movs r0, #15
	bl WaitFrames
	movs r0, #185
	bl Audio_PlayCue
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	mov r0, r10
	str r3, [r0, #48]
	str r3, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	mov r9, r3
	ldr r3, [r7, #8]
	bl Object_SetPosition
	movs r0, #2
	bl WaitFrames
	ldr r2, [r5, #52]
	mov r3, r9
	str r3, [r5, #52]
	ldr r6, [r5, #48]
	str r3, [r5, #48]
	ldr r1, [r7]
	ldr r3, [r7, #8]
	mov r8, r2
	adds r0, r5, #0
	ldr r2, [r7, #4]
	bl Object_SetPosition
	mov r0, r10
	bl Object_CommitPosition
	movs r0, #1
	bl WaitFrames
	bl Func_080d2c98
	mov r0, r10
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Object_SetPositionAndResetMotionFar
	movs r0, #10
	ldrsh r1, [r5, r0]
	movs r0, #18
	ldrsh r3, [r5, r0]
	ldr r2, [r5, #12]
	lsls r1, r1, #16
	lsls r3, r3, #16
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotionFar
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetMode
	mov r2, r8
	str r6, [r5, #48]
	str r2, [r5, #52]
.L_080cde94:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080cdea0:
	.4byte gPartyState
.L_080cdea4:
	.4byte 0xfff80000
