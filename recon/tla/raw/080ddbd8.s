.syntax unified
	.thumb
	.global Func_080ddbd8
	.thumb_func
Func_080ddbd8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	sub sp, #16
	ldr r0, [r3, #16]
	mov r9, r3
	mov r8, r0
	bl BattleEffect_InitializeSharedScene
	movs r2, #4
	movs r3, #23
	add r2, sp
	str r3, [sp, #0]
	mov r10, r2
	mov r11, r10
.L_080ddc06:
	mov r0, r9
	ldrh r3, [r0]
	movs r2, #128
	lsls r2, r2, #7
	cmp r3, r2
	bne .L_080ddc22
	mov r0, r8
	ldr r3, [r0, #8]
	mov r2, r11
	str r3, [r2]
	ldr r3, [r0, #12]
	movs r0, #160
	lsls r0, r0, #12
	b .L_080ddc38
.L_080ddc22:
	movs r2, #192
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_080ddc46
	mov r0, r8
	ldr r3, [r0, #8]
	mov r2, r11
	str r3, [r2]
	ldr r3, [r0, #12]
	movs r0, #192
	lsls r0, r0, #13
.L_080ddc38:
	adds r3, r3, r0
	str r3, [r2, #4]
	mov r2, r8
	ldr r3, [r2, #16]
	mov r0, r11
	str r3, [r0, #8]
	b .L_080ddc6a
.L_080ddc46:
	mov r2, r8
	ldr r3, [r2, #8]
	mov r0, r11
	str r3, [r0]
	movs r0, #160
	ldr r3, [r2, #12]
	lsls r0, r0, #12
	adds r3, r3, r0
	mov r2, r11
	str r3, [r2, #4]
	mov r2, r8
	ldr r3, [r2, #16]
	mov r2, r11
	str r3, [r2, #8]
	mov r3, r9
	ldrh r1, [r3]
	bl Vector_AddPolarOffset
.L_080ddc6a:
	mov r0, r11
	ldr r1, [r0]
	ldr r2, [r0, #4]
	ldr r3, [r0, #8]
	movs r0, #168
	lsls r0, r0, #2
	bl Object_Spawn
	adds r7, r0, #0
	ldr r6, [r7, #80]
	movs r0, #33
	adds r4, r6, #0
	adds r4, #28
	ldrb r3, [r6, #5]
	ldrb r1, [r4, #5]
	negs r0, r0
	movs r2, #32
	ands r2, r3
	adds r3, r0, #0
	ands r1, r3
	orrs r1, r2
	strb r1, [r4, #5]
	ldrb r0, [r4, #7]
	ldrb r2, [r6, #7]
	movs r5, #63
	lsrs r2, r2, #6
	adds r3, r5, #0
	lsls r2, r2, #6
	ands r3, r0
	orrs r3, r2
	strb r3, [r4, #7]
	ands r1, r5
	ldrb r3, [r6, #5]
	ldr r2, .L_080ddcdc
	lsrs r3, r3, #6
	lsls r3, r3, #6
	orrs r1, r3
	strb r1, [r4, #5]
	ldrh r3, [r4, #8]
	ldrh r1, [r6, #8]
	ands r3, r2
	lsls r1, r1, #22
	lsrs r1, r1, #22
	orrs r3, r1
	strh r3, [r4, #8]
	movs r3, #15
	ldrb r2, [r6, #9]
	ldrb r1, [r4, #9]
	lsrs r2, r2, #4
	lsls r2, r2, #4
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	cmp r7, #0
	beq .L_080ddd70
	b .L_080ddce0
	.2byte 0x0000
.L_080ddcdc:
	.4byte 0xfffffc00
.L_080ddce0:
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r7, #28]
	str r3, [r7, #24]
	movs r3, #192
	lsls r3, r3, #9
	adds r2, r7, #0
	adds r2, #85
	str r3, [r7, #52]
	str r3, [r7, #48]
	movs r3, #0
	strb r3, [r2]
	adds r0, r7, #0
	movs r1, #11
	bl Animation_ApplyChildValuesFar
	adds r0, r7, #0
	movs r1, #7
	bl Object_SetMode
	adds r0, r7, #0
	ldr r1, .L_080ddd9c
	bl Object_SetCallback
	adds r0, r7, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26Far
	mov r2, r9
	ldr r3, [r2, #4]
	mov r0, r10
	str r3, [r0]
	ldr r3, [r2, #8]
	str r3, [r0, #4]
	ldr r3, [r2, #12]
	str r3, [r0, #8]
	ldrh r3, [r2]
	movs r2, #192
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_080ddd42
	mov r3, r9
	movs r0, #224
	ldrh r1, [r3]
	lsls r0, r0, #12
	mov r2, r10
	bl Vector_AddPolarOffset
.L_080ddd42:
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	lsls r5, r5, #1
	adds r5, r5, r0
	bl Random16
	mov r2, r10
	adds r1, r0, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	mov r2, r10
	mov r0, r10
	ldr r1, [r2]
	ldr r3, [r0, #8]
	ldr r2, [r2, #4]
	adds r0, r7, #0
	bl Object_SetPosition
.L_080ddd70:
	movs r0, #131
	bl Audio_PlayCue
	movs r0, #2
	bl WaitFrames
	ldr r2, [sp, #0]
	subs r2, #1
	str r2, [sp, #0]
	cmp r2, #0
	blt .L_080ddd88
	b .L_080ddc06
.L_080ddd88:
	movs r0, #8
	bl WaitFrames
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ddd9c:
	.4byte Data_080f0e58
