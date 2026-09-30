.syntax unified
	.thumb
	.global Func_0802c240
	.thumb_func
Func_0802c240:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #48]
	sub sp, #48
	str r1, [sp, #32]
	adds r1, #12
	ldr r2, [r3, #28]
	mov r9, r1
	str r2, [sp, #28]
	movs r1, #144
	ldr r7, [r3, #32]
	movs r3, #200
	lsls r3, r3, #4
	adds r3, r2, r3
	ldr r5, [r7]
	lsls r1, r1, #4
	str r3, [sp, #24]
	adds r1, #92
	adds r3, r7, r1
	ldr r3, [r3]
	movs r2, #150
	str r3, [sp, #20]
	lsls r2, r2, #4
	adds r3, r7, r2
	ldr r3, [r3]
	adds r1, #22
	str r3, [sp, #16]
	movs r3, #0
	str r3, [sp, #8]
	str r3, [sp, #4]
	adds r3, r7, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_0802c2a4
	ldr r3, [r7, #4]
	cmp r3, #0
	bne .L_0802c2a8
	ldr r3, [r7, #8]
	cmp r3, #0
	bne .L_0802c2a8
	b .L_0802c4a6
.L_0802c2a4:
	bl Func_0802c58c
.L_0802c2a8:
	cmp r5, #0
	beq .L_0802c38c
	ldr r2, [r5]
	ldr r3, [r7, #4]
	ldr r5, [r5, #8]
	mov r8, r2
	mov r10, r5
	cmp r3, #0
	beq .L_0802c2de
	bl Random16
	adds r5, r0, #0
	bl Random16
	subs r5, r5, r0
	ldr r6, .L_0802c4b4
	adds r1, r5, #0
	ldr r0, [r7, #4]
	mov lr, r6
	.2byte 0xf800
	str r0, [sp, #8]
	add r8, r0
	ldr r1, [r7, #12]
	ldr r0, [r7, #4]
	mov lr, r6
	.2byte 0xf800
	str r0, [r7, #4]
.L_0802c2de:
	ldr r3, [r7, #8]
	cmp r3, #0
	beq .L_0802c308
	bl Random16
	adds r5, r0, #0
	bl Random16
	subs r5, r5, r0
	ldr r6, .L_0802c4b4
	adds r1, r5, #0
	ldr r0, [r7, #8]
	mov lr, r6
	.2byte 0xf800
	str r0, [sp, #4]
	add r10, r0
	ldr r1, [r7, #12]
	ldr r0, [r7, #8]
	mov lr, r6
	.2byte 0xf800
	str r0, [r7, #8]
.L_0802c308:
	mov r0, r8
	cmp r0, #0
	bge .L_0802c312
	ldr r0, .L_0802c4b8
	add r0, r8
.L_0802c312:
	asrs r4, r0, #20
	mov r0, r10
	cmp r0, #0
	bge .L_0802c31e
	ldr r0, .L_0802c4b8
	add r0, r10
.L_0802c31e:
	adds r6, r7, #0
	adds r6, #228
	asrs r0, r0, #20
	str r0, [sp, #12]
	str r6, [sp, #0]
	mov r2, r8
	ldr r1, [r6]
	adds r3, r1, #0
	eors r3, r2
	movs r2, #128
	lsls r2, r2, #13
	ands r3, r2
	cmp r3, #0
	beq .L_0802c350
	cmp r1, r8
	bge .L_0802c348
	adds r0, r4, #0
	adds r0, #16
	bl Func_0802c1f4
	b .L_0802c350
.L_0802c348:
	adds r0, r4, #0
	subs r0, #16
	bl Func_0802c1f4
.L_0802c350:
	adds r5, r7, #0
	adds r5, #232
	ldr r1, [r5]
	mov r2, r10
	adds r3, r1, #0
	eors r3, r2
	movs r2, #128
	lsls r2, r2, #13
	ands r3, r2
	mov r11, r5
	cmp r3, #0
	beq .L_0802c37e
	cmp r1, r10
	bge .L_0802c376
	ldr r0, [sp, #12]
	adds r0, #12
	bl Func_0802c174
	b .L_0802c37e
.L_0802c376:
	ldr r0, [sp, #12]
	subs r0, #18
	bl Func_0802c174
.L_0802c37e:
	ldr r1, [sp, #0]
	mov r3, r8
	str r3, [r1]
	mov r2, r10
	mov r3, r11
	str r2, [r3]
	b .L_0802c394
.L_0802c38c:
	adds r6, r7, #0
	adds r5, r7, #0
	adds r6, #228
	adds r5, #232
.L_0802c394:
	ldr r3, [r6]
	mov r1, r9
	str r3, [r1]
	movs r3, #0
	str r3, [r1, #4]
	movs r2, #144
	ldr r3, [r5]
	lsls r2, r2, #4
	str r3, [r1, #8]
	adds r2, #113
	adds r3, r7, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r8, r3
	cmp r3, #0
	beq .L_0802c3de
	ldr r0, [sp, #8]
	ldr r2, .L_0802c4bc
	cmp r0, #0
	bge .L_0802c3c6
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_0802c3c6:
	asrs r3, r0, #16
	strh r3, [r2, #4]
	ldr r0, [sp, #4]
	cmp r0, #0
	bge .L_0802c3d8
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r0, r0, r1
.L_0802c3d8:
	asrs r3, r0, #16
	strh r3, [r2, #6]
	b .L_0802c4a6
.L_0802c3de:
	ldr r2, .L_0802c4c0
	movs r3, #120
	str r3, [r2, #12]
	movs r3, #96
	str r3, [r2, #16]
	ldr r2, [sp, #16]
	ldr r0, [sp, #20]
	lsrs r1, r2, #31
	adds r1, r2, r1
	asrs r1, r1, #1
	lsls r2, r2, #1
	bl Camera_StoreSceneParameters
	bl Func_08014de4
	mov r0, r9
	bl SceneTransform_ApplyPosition
	movs r1, #143
	lsls r1, r1, #1
	adds r3, r7, r1
	ldrh r0, [r3]
	bl Func_08015068
	movs r2, #142
	lsls r2, r2, #1
	adds r6, r7, r2
	ldrh r0, [r6]
	bl SceneTransform_ApplyPitch
	movs r1, #144
	lsls r1, r1, #4
	adds r1, #118
	adds r3, r7, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0802c446
	add r0, sp, #36
	mov r2, r8
	str r2, [r0]
	str r2, [r0, #4]
	ldr r1, [sp, #16]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r1, r2
	str r3, [r0, #8]
	ldr r1, [sp, #32]
	ldr r3, .L_0802c4c4
	mov lr, r3
	.2byte 0xf800
.L_0802c446:
	bl Func_08014de4
	mov r1, r9
	ldr r0, [sp, #32]
	bl Func_080156e8
	ldr r7, .L_0802c4c8
	ldrh r0, [r6]
	ldr r3, [r7]
	cmp r3, r0
	beq .L_0802c484
	bl Trig_Cos
	adds r5, r0, #0
	ldrh r0, [r6]
	bl Trig_Sin
	ldr r3, .L_0802c4cc
	adds r1, r0, #0
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	ldr r2, [sp, #28]
	bl Func_0802dd70
	ldr r3, .L_0802c4d0
	mov r1, r8
	str r1, [r3]
	ldrh r3, [r6]
	str r3, [r7]
.L_0802c484:
	ldr r3, .L_0802c4d4
	movs r1, #192
	ldr r2, [r3]
	movs r3, #1
	ands r2, r3
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, [sp, #24]
	lsls r1, r1, #18
	lsls r3, r3, #10
	adds r3, r2, r3
	ldr r4, [r1, #104]
	ldr r0, [sp, #32]
	mov r1, r9
	ldr r2, [sp, #28]
	mov lr, r4
	.2byte 0xf800
.L_0802c4a6:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0802c4b4:
	.4byte IwramMulQ16
.L_0802c4b8:
	.4byte 0x000fffff
.L_0802c4bc:
	.4byte Data_03001120
.L_0802c4c0:
	.4byte gCameraSceneParameters
.L_0802c4c4:
	.4byte IwramTransformVector
.L_0802c4c8:
	.4byte Data_03001144
.L_0802c4cc:
	.4byte IwramRatioMulQ14
.L_0802c4d0:
	.4byte Data_03001244
.L_0802c4d4:
	.4byte Data_0300122c
