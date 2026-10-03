.syntax unified
	.thumb
	.global Func_080d4d08
	.thumb_func
Func_080d4d08:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_080d5064
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	sub sp, #24
	bl Object_GetById
	movs r3, #1
	negs r3, r3
	str r3, [sp, #8]
	adds r6, r0, #0
	ldrh r2, [r6, #6]
	movs r3, #85
	adds r3, r3, r6
	mov r10, r2
	ldrb r2, [r3]
	mov r9, r3
	str r2, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #128
	mov r11, r3
	movs r3, #1
	str r3, [sp, #0]
	lsls r2, r2, #2
	adds r2, #18
	adds r5, r5, r2
	ldrb r3, [r5]
	cmp r3, #2
	bne .L_080d4d58
	b .L_080d50e2
.L_080d4d58:
	cmp r3, #3
	bne .L_080d4d5e
	b .L_080d50e2
.L_080d4d5e:
	movs r3, #128
	lsls r3, r3, #5
	movs r2, #128
	add r3, r10
	lsls r2, r2, #6
	ands r3, r2
	cmp r3, #0
	beq .L_080d4d70
	b .L_080d50e2
.L_080d4d70:
	movs r3, #128
	lsls r3, r3, #6
	add r10, r3
	movs r3, #192
	mov r5, r10
	lsls r3, r3, #8
	ands r5, r3
	mov r10, r5
	add r7, sp, #12
.L_080d4d82:
	ldr r3, [r6, #8]
	ldr r5, .L_080d5068
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r5
	adds r3, r3, r2
	str r3, [r7]
	movs r0, #128
	ldr r3, [r6, #12]
	lsls r0, r0, #13
	str r3, [r7, #4]
	mov r1, r10
	ldr r3, [r6, #16]
	ands r3, r5
	adds r3, r3, r2
	str r3, [r7, #8]
	adds r2, r7, #0
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r7, #0
	bl Func_08020210
	cmp r0, #1
	bne .L_080d4dba
	movs r0, #1
	negs r0, r0
	b .L_080d50e4
.L_080d4dba:
	ldr r3, [r6, #8]
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r5
	adds r3, r3, r2
	str r3, [r7]
	movs r0, #128
	ldr r3, [r6, #12]
	lsls r0, r0, #14
	str r3, [r7, #4]
	mov r1, r10
	ldr r3, [r6, #16]
	ands r3, r5
	adds r3, r3, r2
	str r3, [r7, #8]
	adds r2, r7, #0
	bl Vector_AddPolarOffset
	movs r3, #0
	adds r0, r6, #0
	adds r1, r7, #0
	mov r8, r3
	bl Func_080d4ccc
	cmp r0, #0
	bne .L_080d4dfe
	adds r0, r6, #0
	adds r1, r7, #0
	bl Func_08020210
	cmp r0, #0
	bne .L_080d4dfe
	movs r5, #1
	mov r8, r5
.L_080d4dfe:
	adds r3, r6, #0
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	beq .L_080d4e82
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #20]
	ldr r2, .L_080d506c
	adds r1, r0, #0
	movs r4, #0
	mov r12, r2
	adds r1, #86
.L_080d4e1c:
	ldr r3, [r0]
	cmp r3, #0
	beq .L_080d4e78
	adds r3, r0, #0
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080d4e78
	ldrb r3, [r1]
	cmp r3, #0
	bne .L_080d4e78
	cmp r0, r6
	beq .L_080d4e78
	ldr r3, [r7]
	ldr r2, [r0, #8]
	movs r5, #128
	subs r3, r3, r2
	lsls r5, r5, #12
	adds r3, r3, r5
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_080d4e78
	ldr r3, [r7, #8]
	ldr r2, [r0, #16]
	subs r3, r3, r2
	adds r3, r3, r5
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_080d4e78
	ldr r3, [r0, #12]
	ldr r2, [r7, #4]
	ldr r5, .L_080d5068
	subs r2, r2, r3
	adds r3, r2, r5
	cmp r3, #0
	bge .L_080d4e6e
	movs r3, #128
	lsls r3, r3, #13
	subs r3, r3, r2
.L_080d4e6e:
	cmp r3, r12
	bgt .L_080d4e78
	movs r2, #1
	mov r8, r2
	b .L_080d4e82
.L_080d4e78:
	adds r4, #1
	adds r1, #128
	adds r0, #128
	cmp r4, #63
	ble .L_080d4e1c
.L_080d4e82:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #20]
	ldr r5, .L_080d506c
	movs r3, #89
	adds r3, r3, r0
	movs r4, #0
	mov r12, r3
.L_080d4e92:
	ldr r3, [r0]
	cmp r3, #0
	beq .L_080d4ef2
	adds r3, r0, #0
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	bne .L_080d4ef2
	cmp r0, r6
	beq .L_080d4ef2
	mov r2, r12
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_080d4ef2
	ldr r2, [r0, #8]
	ldr r3, [r7]
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_080d4ef2
	ldr r2, [r0, #16]
	ldr r3, [r7, #8]
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_080d4ef2
	ldr r1, [r7, #4]
	ldr r3, [r0, #12]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_080d4ee6
	cmp r2, r5
	bgt .L_080d4ef2
	b .L_080d4eec
.L_080d4ee6:
	subs r3, r3, r1
	cmp r3, r5
	bgt .L_080d4ef2
.L_080d4eec:
	movs r3, #0
	mov r8, r3
	b .L_080d4efe
.L_080d4ef2:
	movs r2, #128
	adds r4, #1
	add r12, r2
	adds r0, #128
	cmp r4, #63
	ble .L_080d4e92
.L_080d4efe:
	mov r3, r8
	cmp r3, #0
	bne .L_080d4f06
	b .L_080d50d2
.L_080d4f06:
	adds r3, r6, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080d4f16
	ldr r3, [r6, #80]
	ldrb r3, [r3, #26]
	str r3, [sp, #0]
.L_080d4f16:
	bl EventRuntime_Begin
	movs r1, #6
	adds r0, r6, #0
	bl Object_SetMode
	movs r0, #6
	bl WaitFrames
	movs r0, #152
	bl Audio_PlayCue
	adds r0, r6, #0
	movs r1, #7
	bl Object_SetMode
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r6, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r6, #40]
	mov r5, r9
	ldrb r2, [r5]
	movs r3, #126
	ands r3, r2
	strb r3, [r5]
	ldr r2, [sp, #0]
	movs r1, #254
	ands r1, r2
	adds r0, r6, #0
	bl ObjectDispatch_SetSingleChildField26Far
	ldr r3, .L_080d5064
	movs r5, #133
	lsls r5, r5, #2
	adds r3, r3, r5
	ldr r0, [r3]
	movs r2, #2
	ldrsh r1, [r7, r2]
	movs r3, #10
	ldrsh r2, [r7, r3]
	bl ObjectMotion_SetPositionAndCommit
	adds r0, r6, #0
	movs r1, #6
	bl Object_SetMode
	adds r0, r6, #0
	ldr r1, [sp, #0]
	bl ObjectDispatch_SetSingleChildField26Far
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	movs r3, #12
	ldrb r1, [r0, #23]
	ands r3, r1
	cmp r3, #0
	beq .L_080d4fe2
	ldr r3, [r6, #8]
	asrs r5, r3, #20
	ldr r3, [r6, #16]
	asrs r4, r3, #20
	cmp r0, #0
	bne .L_080d4faa
	movs r3, #34
	adds r3, r3, r6
	ldr r2, .L_080d5070
	mov r8, r3
	b .L_080d4fc4
.L_080d4faa:
	movs r2, #34
	adds r2, r2, r6
	ldrb r3, [r2]
	mov r8, r2
	movs r2, #3
	ands r2, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [r0, r3]
.L_080d4fc4:
	lsls r3, r4, #7
	adds r3, r5, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r2, [r2, #3]
	movs r3, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080d4fe8
	lsls r1, r1, #28
	lsrs r1, r1, #30
	adds r0, r6, #0
	bl Func_080203a0
	b .L_080d4fe8
.L_080d4fe2:
	movs r3, #34
	adds r3, r3, r6
	mov r8, r3
.L_080d4fe8:
	movs r1, #129
	lsls r1, r1, #1
	adds r0, r6, #0
	bl Object_FindNearestFacingTarget
	cmp r0, #0
	bne .L_080d5004
	movs r1, #4
	adds r1, #255
	adds r0, r6, #0
	bl Object_FindNearestFacingTarget
	cmp r0, #0
	beq .L_080d5078
.L_080d5004:
	adds r3, r0, #0
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080d5078
	movs r1, #7
	bl Object_SetMode
	ldr r5, .L_080d5074
	ldr r3, [r6, #12]
	movs r0, #2
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	bl WaitFrames
	ldr r3, [r6, #12]
	movs r0, #10
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	bl WaitFrames
	ldr r3, [r6, #12]
	movs r5, #128
	lsls r5, r5, #9
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	movs r0, #4
	adds r3, r3, r5
	str r3, [r6, #20]
	bl WaitFrames
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	b .L_080d507e
	.2byte 0x0000
.L_080d5064:
	.4byte gPartyState
.L_080d5068:
	.4byte 0xfff00000
.L_080d506c:
	.4byte 0x0007ffff
.L_080d5070:
	.4byte gMapCellBuffer
.L_080d5074:
	.4byte 0xffff0000
.L_080d5078:
	movs r0, #6
	bl WaitFrames
.L_080d507e:
	add r5, sp, #4
	ldrb r5, [r5]
	mov r2, r9
	strb r5, [r2]
	bl EventRuntime_End
	mov r2, r11
	cmp r2, #0
	beq .L_080d50ae
	movs r3, #206
	lsls r3, r3, #1
	add r3, r11
	movs r1, #128
	ldr r0, [r3]
	lsls r1, r1, #14
	ldr r3, .L_080d50f4
	mov lr, r3
	.2byte 0xf800
	movs r2, #208
	lsls r2, r2, #1
	add r2, r11
	ldr r3, [r2]
	adds r3, r3, r0
	str r3, [r2]
.L_080d50ae:
	mov r3, r8
	ldrb r0, [r3]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Func_080201c8
	cmp r0, #254
	bne .L_080d50ce
	adds r0, r6, #0
	movs r1, #1
	bl Object_SetMode
	movs r0, #6
	bl WaitFrames
	b .L_080d4d82
.L_080d50ce:
	movs r5, #0
	str r5, [sp, #8]
.L_080d50d2:
	adds r2, r6, #0
	movs r3, #8
	adds r2, #100
	strh r3, [r2]
	adds r3, r6, #0
	movs r1, #0
	adds r3, #102
	strh r1, [r3]
.L_080d50e2:
	ldr r0, [sp, #8]
.L_080d50e4:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d50f4:
	.4byte IwramMulQ16
