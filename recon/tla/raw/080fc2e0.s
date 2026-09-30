.syntax unified
	.thumb
	.global Func_080fc2e0
	.thumb_func
Func_080fc2e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #0
	sub sp, #12
	mov r8, r3
	movs r3, #1
	str r3, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r6, #140
	lsls r6, r6, #1
	mov r9, r3
	adds r6, #255
	add r6, r9
	ldrb r0, [r6]
	bl Owner_GetState
	movs r3, #181
	str r0, [sp, #4]
	lsls r3, r3, #1
	add r3, r9
	ldrh r1, [r3]
	mov r10, r3
	ldrb r3, [r6]
	movs r5, #166
	adds r0, r3, #0
	movs r2, #0
	lsls r5, r5, #1
	bl Func_080fae8c
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	ldr r3, .L_080fc470
	ldr r1, [sp, #4]
	adds r2, r5, #0
	mov r11, r0
	mov lr, r3
	.2byte 0xf800
	mov r3, r9
	adds r3, #240
	ldr r7, [r3]
	mov r3, r10
	ldrb r0, [r6]
	ldrh r1, [r3]
	bl Func_080ad048
	adds r0, #2
	cmp r0, #1
	bhi .L_080fc35c
	b .L_080fc41e
.L_080fc354:
	movs r0, #175
	bl Audio_PlayCue
	b .L_080fc422
.L_080fc35c:
	ldr r5, .L_080fc474
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #96
	movs r3, #0
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #96
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #24
	str r3, [sp, #0]
	adds r0, r7, #0
	movs r1, #16
	movs r2, #16
	movs r3, #96
	bl RenderOutput_PrepareForRedrawFar + 0x8
	adds r1, r7, #0
	ldr r0, .L_080fc478
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #184
	movs r1, #5
	bl Func_080f8ab4
	b .L_080fc3e0
.L_080fc39e:
	mov r3, r8
	lsls r1, r3, #4
	adds r1, #5
	movs r0, #184
	bl Func_080f8a44
	ldr r3, [r5, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_080fc3c4
	movs r3, #1
	negs r3, r3
	add r8, r3
	movs r0, #111
	movs r3, #1
	str r3, [sp, #8]
	bl Audio_PlayCue
.L_080fc3c4:
	ldr r3, [r5, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080fc3da
	movs r3, #1
	movs r0, #111
	add r8, r3
	str r3, [sp, #8]
	bl Audio_PlayCue
.L_080fc3da:
	movs r0, #1
	bl WaitFrames
.L_080fc3e0:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fc422
	ldr r3, [sp, #8]
	cmp r3, #0
	beq .L_080fc402
	mov r0, r8
	movs r3, #0
	adds r0, #2
	movs r1, #2
	str r3, [sp, #8]
	bl Math_Mod
	mov r8, r0
.L_080fc402:
	ldr r5, .L_080fc47c
	movs r2, #1
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_080fc354
	ldr r3, [r5, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080fc39e
	movs r0, #113
	bl Audio_PlayCue
.L_080fc41e:
	movs r3, #1
	mov r8, r3
.L_080fc422:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080fc432
	movs r3, #1
	mov r8, r3
.L_080fc432:
	mov r3, r8
	cmp r3, #1
	bne .L_080fc446
	movs r2, #166
	ldr r3, .L_080fc470
	ldr r0, [sp, #4]
	mov r1, r11
	lsls r2, r2, #1
	mov lr, r3
	.2byte 0xf800
.L_080fc446:
	movs r5, #140
	lsls r5, r5, #1
	adds r5, #255
	mov r0, r11
	add r5, r9
	bl Sys_Free
	ldrb r0, [r5]
	bl Func_080ad288
	ldrb r0, [r5]
	bl BattleUnit_Recalculate
	mov r0, r8
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080fc470:
	.4byte IwramCopyWords
.L_080fc474:
	.4byte 0x0000105b
.L_080fc478:
	.4byte 0x00001005
.L_080fc47c:
	.4byte gInput
