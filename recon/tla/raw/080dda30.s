.syntax unified
	.thumb
	.global Func_080dda30
	.thumb_func
Func_080dda30:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r7, [r3]
	sub sp, #12
	ldr r5, [r7, #20]
	movs r6, #0
	cmp r5, #0
	beq .L_080ddb30
	bl Func_080ddbd8
	adds r0, r5, #0
	bl Func_08020330
	movs r3, #186
	lsls r3, r3, #1
	cmp r0, r3
	bne .L_080dda5c
	movs r6, #1
.L_080dda5c:
	adds r0, r5, #0
	bl Func_08020330
	movs r3, #197
	lsls r3, r3, #1
	cmp r0, r3
	bne .L_080dda6c
	movs r6, #2
.L_080dda6c:
	cmp r6, #0
	bne .L_080ddaa6
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetMode
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	adds r3, r5, #0
	adds r3, #89
	strb r6, [r3]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r0, #10
	bl WaitFrames
	movs r0, #126
	bl Audio_PlayCue
	movs r0, #40
	bl WaitFrames
	b .L_080ddb2c
.L_080ddaa6:
	cmp r6, #1
	bne .L_080ddb20
	movs r3, #4
	mov r6, sp
	mov r8, r3
.L_080ddab0:
	ldr r3, [r7, #4]
	str r3, [r6]
	ldr r3, [r7, #12]
	str r3, [r6, #8]
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r3, #128
	lsls r3, r3, #11
	lsls r5, r5, #1
	adds r5, r5, r3
	bl Random16
	adds r2, r6, #0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	ldr r2, [r7, #8]
	movs r0, #26
	str r2, [r6, #4]
	ldr r1, [r6]
	ldr r3, [r6, #8]
	adds r0, #255
	bl Object_Spawn
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080ddafa
	ldr r1, .L_080ddb38
	bl ObjectDispatch_InitializeFar
	adds r2, r5, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
.L_080ddafa:
	bl Random16
	lsls r0, r0, #1
	lsrs r0, r0, #16
	adds r0, #2
	bl WaitFrames
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r3, r8
	cmp r3, #0
	bge .L_080ddab0
	movs r3, #26
	ldrsh r0, [r7, r3]
	movs r1, #1
	bl Func_080daecc
	b .L_080ddb2c
.L_080ddb20:
	movs r0, #10
	bl WaitFrames
	movs r0, #126
	bl Audio_PlayCue
.L_080ddb2c:
	bl BattleFx_PrepareBufferInterpolation
.L_080ddb30:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080ddb38:
	.4byte Data_080f0ee8
