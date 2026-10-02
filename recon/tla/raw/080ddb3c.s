.syntax unified
	.thumb
	.global Func_080ddb3c
	.thumb_func
Func_080ddb3c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	sub sp, #12
	ldr r7, [r3]
	bl Func_080ddbd8
	movs r0, #134
	bl Audio_PlayCue
	movs r3, #4
	mov r6, sp
	mov r8, r3
.L_080ddb5c:
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
	ldr r1, [r6]
	ldr r3, [r6, #8]
	adds r0, #255
	str r2, [r6, #4]
	bl Object_Spawn
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080ddba6
	ldr r1, .L_080ddbd4
	bl ObjectDispatch_InitializeFar
	adds r2, r5, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
.L_080ddba6:
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
	bge .L_080ddb5c
	movs r0, #30
	bl WaitFrames
	bl BattleFx_PrepareBufferInterpolation
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ddbd4:
	.4byte Data_080f0ee8
