.syntax unified
	.thumb
	.global Func_080fb104
	.thumb_func
Func_080fb104:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #0
	sub sp, #16
	movs r3, #192
	mov r8, r1
	add r1, sp, #8
	lsls r3, r3, #18
	adds r3, #220
	mov r11, r1
	ldr r6, [r3]
	movs r2, #0
	movs r3, #1
	mov r0, r11
	mov r10, r2
	mov r9, r3
	bl Func_080fb410
	movs r2, #135
	lsls r2, r2, #2
	adds r2, r6, r2
	str r2, [sp, #4]
	movs r7, #0
	ldrh r3, [r2]
	cmp r3, #1
	beq .L_080fb178
	bl Func_080fa458
	ldr r0, [r6, #56]
	bl RenderOutput_RedrawSavedRectFar
	adds r3, r6, #0
	adds r3, #240
	ldr r5, [r3]
	bl Func_080fbe24
	adds r0, r5, #0
	bl RenderOutput_RedrawSavedRectFar
	movs r3, #3
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #0
	movs r2, #3
	movs r3, #16
	bl UiWindow_DrawDividerLineFar
	bl Func_080fc12c
	mov r0, r11
	adds r1, r5, #0
	bl Func_080fb554
.L_080fb178:
	ldr r1, [sp, #4]
	mov r3, r8
	movs r2, #177
	strh r3, [r1]
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r6, r2
	movs r5, #0
	ldrsb r5, [r3, r5]
	movs r3, #1
	negs r3, r3
	cmp r5, r3
	bne .L_080fb1f6
	mov r1, r11
	movs r3, #2
	ldrsb r3, [r1, r3]
	cmp r3, #1
	bne .L_080fb1a2
	movs r2, #0
	movs r7, #2
	mov r10, r2
.L_080fb1a2:
	mov r1, r11
	movs r3, #3
	ldrsb r3, [r1, r3]
	cmp r3, #1
	bne .L_080fb1b2
	movs r2, #1
	movs r7, #0
	mov r10, r2
.L_080fb1b2:
	mov r1, r11
	movs r3, #1
	ldrsb r3, [r1, r3]
	cmp r3, #1
	bne .L_080fb1c2
	movs r2, #0
	movs r7, #1
	mov r10, r2
.L_080fb1c2:
	mov r1, r11
	movs r3, #4
	ldrsb r3, [r1, r3]
	cmp r3, #1
	bne .L_080fb1d2
	movs r2, #1
	movs r7, #1
	mov r10, r2
.L_080fb1d2:
	mov r1, r11
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #1
	bne .L_080fb218
	movs r2, #0
	movs r7, #0
	mov r10, r2
	b .L_080fb218
.L_080fb1e4:
	movs r0, #113
	bl Audio_PlayCue
	movs r3, #1
	movs r1, #177
	negs r3, r3
	lsls r1, r1, #1
	mov r8, r3
	b .L_080fb35e
.L_080fb1f6:
	movs r1, #3
	adds r0, r5, #0
	bl __modsi3
	lsls r0, r0, #24
	asrs r7, r0, #24
	movs r1, #3
	adds r0, r5, #0
	bl Math_Div
	lsls r0, r0, #24
	asrs r0, r0, #24
	mov r10, r0
	lsls r3, r0, #1
	add r3, r10
	adds r3, r3, r7
	mov r8, r3
.L_080fb218:
	lsls r0, r7, #2
	adds r0, r0, r7
	mov r3, r10
	lsls r1, r3, #3
	lsls r0, r0, #3
	adds r0, #86
	adds r1, #30
	bl Func_080f8ab4
	b .L_080fb3d4
.L_080fb22c:
	mov r1, r9
	cmp r1, #0
	beq .L_080fb2d2
	movs r2, #0
	adds r0, r7, #3
	movs r1, #3
	mov r9, r2
	bl __modsi3
	mov r2, r10
	adds r2, #2
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r3, r3, #1
	subs r2, r2, r3
	mov r10, r2
	lsls r3, r2, #1
	adds r7, r0, #0
	add r3, r10
	adds r3, r3, r7
	mov r8, r3
	bl Func_080fac58
	mov r3, r8
	cmp r3, #2
	ble .L_080fb296
	movs r1, #152
	lsls r1, r1, #2
	adds r2, r6, r1
	movs r3, #1
	strb r3, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #22
	adds r3, r6, r2
	ldrb r3, [r3]
	subs r1, #248
	adds r2, r6, r1
	ldrh r1, [r2]
	adds r0, r3, #0
	movs r2, #0
	bl Func_080fae8c
	mov r2, r8
	cmp r2, #3
	bne .L_080fb2d2
	movs r1, #144
	ldr r0, .L_080fb404
	lsls r1, r1, #3
	bl Func_080145a8
	b .L_080fb2d2
.L_080fb296:
	mov r3, r8
	cmp r3, #0
	beq .L_080fb2be
	movs r1, #152
	lsls r1, r1, #2
	adds r3, r6, r1
	movs r2, #128
	strb r5, [r3]
	lsls r2, r2, #2
	adds r2, #22
	adds r3, r6, r2
	ldrb r3, [r3]
	subs r1, #248
	adds r2, r6, r1
	ldrh r1, [r2]
	adds r0, r3, #0
	movs r2, #0
	bl Func_080fae8c
	b .L_080fb2d2
.L_080fb2be:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #22
	adds r3, r6, r2
	ldrb r1, [r3]
	ldr r0, [r6, #40]
	movs r2, #0
	movs r3, #0
	bl Func_080f8170
.L_080fb2d2:
	lsls r0, r7, #2
	mov r3, r10
	adds r0, r0, r7
	lsls r1, r3, #3
	lsls r0, r0, #3
	adds r0, #86
	adds r1, #30
	bl Func_080f8a44
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_080fb408
	movs r3, #1
	ldr r2, [r5, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_080fb368
	mov r1, r11
	mov r2, r8
	ldrsb r3, [r1, r2]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	bne .L_080fb30c
	movs r0, #114
	bl Audio_PlayCue
	b .L_080fb368
.L_080fb30c:
	mov r2, r8
	cmp r2, #5
	bhi .L_080fb354
	lsls r3, r2, #2
	ldr r2, .L_080fb40c
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080fb31c:
	.4byte .L_080fb334
	.4byte .L_080fb33c
	.4byte .L_080fb344
	.4byte .L_080fb344
	.4byte .L_080fb34c
	.4byte .L_080fb344
.L_080fb334:
	movs r0, #174
	bl Audio_PlayCue
	b .L_080fb35a
.L_080fb33c:
	movs r0, #175
	bl Audio_PlayCue
	b .L_080fb35a
.L_080fb344:
	movs r0, #112
	bl Audio_PlayCue
	b .L_080fb35a
.L_080fb34c:
	movs r0, #117
	bl Audio_PlayCue
	b .L_080fb35a
.L_080fb354:
	movs r0, #112
	bl Audio_PlayCue
.L_080fb35a:
	movs r1, #177
	lsls r1, r1, #1
.L_080fb35e:
	adds r1, #255
	adds r3, r6, r1
	mov r2, r8
	strb r2, [r3]
	b .L_080fb3e4
.L_080fb368:
	ldr r2, [r5, #4]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080fb374
	b .L_080fb1e4
.L_080fb374:
	ldr r2, [r5, #12]
	movs r3, #64
	ands r2, r3
	cmp r2, #0
	beq .L_080fb38e
	subs r3, #65
	movs r1, #1
	movs r0, #111
	add r10, r3
	mov r9, r1
	bl Audio_PlayCue
	b .L_080fb3d4
.L_080fb38e:
	ldr r2, [r5, #12]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_080fb3a6
	movs r2, #1
	movs r0, #111
	add r10, r2
	mov r9, r2
	bl Audio_PlayCue
	b .L_080fb3d4
.L_080fb3a6:
	ldr r2, [r5, #12]
	movs r3, #16
	ands r2, r3
	cmp r2, #0
	beq .L_080fb3be
	movs r3, #1
	movs r0, #111
	adds r7, #1
	mov r9, r3
	bl Audio_PlayCue
	b .L_080fb3d4
.L_080fb3be:
	ldr r3, [r5, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_080fb3d4
	movs r1, #1
	movs r0, #111
	subs r7, #1
	mov r9, r1
	bl Audio_PlayCue
.L_080fb3d4:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_080fb3e4
	b .L_080fb22c
.L_080fb3e4:
	movs r3, #152
	lsls r3, r3, #2
	adds r2, r6, r3
	movs r3, #0
	strb r3, [r2]
	bl Func_080fac58
	mov r0, r8
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080fb404:
	.4byte Func_080fabe0
.L_080fb408:
	.4byte gInput
.L_080fb40c:
	.4byte .L_080fb31c
