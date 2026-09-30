.syntax unified
	.thumb
	.global Func_080d7f80
	.thumb_func
Func_080d7f80:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #12
	adds r6, r0, #0
	bl Object_GetById
	ldr r3, .L_080d8130
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	mov r8, r0
	cmp r5, #0
	bne .L_080d7fa8
	b .L_080d8126
.L_080d7fa8:
	bl BattleFx_InitializeSlots
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	ldr r0, .L_080d8134
	mov r10, r3
	bl Unnamed_080b0840Far
	movs r0, #30
	bl WaitFrames
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	adds r0, r6, #0
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl WaitFrames
	movs r0, #173
	bl Audio_PlayCue
	adds r0, r6, #0
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #174
	bl Audio_PlayCue
	adds r0, r6, #0
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #175
	bl Audio_PlayCue
	movs r1, #1
	adds r0, r6, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl WaitFrames
	movs r0, #140
	bl Audio_PlayCue
	ldr r3, .L_080d8138
	adds r2, r5, #0
	str r3, [r5, #108]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	movs r0, #80
	bl WaitFrames
	ldr r3, .L_080d813c
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #3
	bl Object_SetMode
	ldr r3, [r5, #8]
	mov r7, sp
	str r3, [r7]
	adds r0, r7, #0
	ldr r3, [r5, #12]
	movs r6, #23
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	mov r5, r10
	str r3, [r7, #8]
	bl Camera_WorldToScreen
	adds r5, #80
.L_080d8042:
	movs r1, #168
	ldr r2, [r7]
	ldr r3, [r7, #8]
	adds r0, r5, #0
	lsls r1, r1, #2
	bl Func_080ebec8
	adds r0, r5, #0
	ldr r1, .L_080d8140
	bl Func_080ebeb4
	adds r0, r5, #0
	movs r1, #7
	bl Func_080ebea8
	ldr r0, [r5]
	movs r1, #10
	bl Animation_ApplyChildValuesToRecordFar
	bl Random16
	movs r1, #3
	bl Math_DivU
	movs r3, #128
	lsls r3, r3, #9
	adds r0, r0, r3
	str r0, [r5, #44]
	str r0, [r5, #40]
	subs r6, #1
	movs r0, #1
	bl WaitFrames
	adds r5, #72
	cmp r6, #0
	bge .L_080d8042
	movs r0, #60
	bl WaitFrames
	ldr r5, .L_080d8130
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl WaitFrames
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #28
	bl Object_SetMode
	movs r0, #20
	bl WaitFrames
	mov r2, r10
	movs r1, #2
	adds r2, #144
	movs r6, #23
.L_080d80c4:
	movs r3, #5
	ldrsb r3, [r2, r3]
	cmp r3, #0
	beq .L_080d80ce
	strb r1, [r2]
.L_080d80ce:
	subs r6, #1
	adds r2, #72
	cmp r6, #0
	bge .L_080d80c4
	movs r0, #60
	bl WaitFrames
	ldr r3, .L_080d8144
	mov r2, r8
	str r3, [r2, #108]
	movs r0, #100
	bl WaitFrames
	mov r2, r10
	movs r1, #5
	adds r2, #144
	movs r6, #23
.L_080d80f0:
	movs r3, #5
	ldrsb r3, [r2, r3]
	cmp r3, #0
	beq .L_080d80fa
	strb r1, [r2]
.L_080d80fa:
	subs r6, #1
	adds r2, #72
	cmp r6, #0
	bge .L_080d80f0
	movs r0, #10
	bl WaitFrames
	movs r5, #0
	mov r3, r8
	str r5, [r3, #108]
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r8
	str r3, [r2, #24]
	str r3, [r2, #28]
	movs r0, #30
	bl WaitFrames
	bl Func_08108060
	bl Func_080d7ab4
.L_080d8126:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080d8130:
	.4byte gPartyState
.L_080d8134:
	.4byte 0x00201090
.L_080d8138:
	.4byte Func_080d7d68
.L_080d813c:
	.4byte Func_080d7d90
.L_080d8140:
	.4byte Func_080d7dbc
.L_080d8144:
	.4byte Func_080d7d48
