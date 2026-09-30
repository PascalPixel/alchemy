.syntax unified
	.thumb
	.global Func_080fc1ac
	.thumb_func
Func_080fc1ac:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #3
	movs r2, #17
	adds r5, r0, #0
	movs r3, #10
	movs r0, #13
	bl UiWindow_CreateFar
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r5, r3
	adds r7, r0, #0
	adds r0, r5, #0
	bl Item_Get
	ldr r3, .L_080fc2d0
	adds r1, r7, #0
	adds r5, r5, r3
	adds r0, r5, #0
	movs r2, #24
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r5, .L_080fc2d4
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #8
	movs r3, #16
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #8
	movs r3, #24
	bl UiText_DrawCharacterAtOffsetFar
	ldr r5, .L_080fc2d8
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #24
	movs r3, #40
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #24
	movs r3, #56
	bl UiText_DrawCharacterAtOffsetFar
	movs r6, #1
	movs r0, #104
	movs r1, #86
	mov r8, r6
	bl Func_080f8ab4
	b .L_080fc268
.L_080fc22c:
	lsls r1, r6, #4
	adds r1, #70
	movs r0, #104
	bl Func_080f8a44
	ldr r3, [r5, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_080fc24c
	movs r2, #1
	movs r0, #111
	subs r6, #1
	mov r8, r2
	bl Audio_PlayCue
.L_080fc24c:
	ldr r3, [r5, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080fc262
	movs r3, #1
	movs r0, #111
	adds r6, #1
	mov r8, r3
	bl Audio_PlayCue
.L_080fc262:
	movs r0, #1
	bl WaitFrames
.L_080fc268:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fc2ae
	mov r2, r8
	cmp r2, #0
	beq .L_080fc288
	movs r3, #0
	adds r0, r6, #2
	movs r1, #2
	mov r8, r3
	bl Math_Mod
	adds r6, r0, #0
.L_080fc288:
	ldr r5, .L_080fc2dc
	movs r2, #1
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_080fc29c
	movs r0, #112
	bl Audio_PlayCue
	b .L_080fc2ae
.L_080fc29c:
	ldr r3, [r5, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080fc22c
	movs r0, #113
	bl Audio_PlayCue
	movs r6, #1
.L_080fc2ae:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080fc2bc
	movs r6, #1
.L_080fc2bc:
	adds r0, r7, #0
	movs r1, #1
	bl UiWork_FinalizeFar
	adds r0, r6, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080fc2d0:
	.4byte 0x0000025f
.L_080fc2d4:
	.4byte 0x00001003
.L_080fc2d8:
	.4byte 0x0000105b
.L_080fc2dc:
	.4byte gInput
