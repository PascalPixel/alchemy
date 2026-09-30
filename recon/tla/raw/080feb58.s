.syntax unified
	.thumb
	.global Func_080feb58
	.thumb_func
Func_080feb58:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	sub sp, #4
	movs r2, #28
	ldrsb r2, [r7, r2]
	movs r3, #2
	str r2, [sp, #0]
	mov r11, r3
	movs r1, #30
	ldrsb r1, [r7, r1]
	lsls r2, r2, #1
	mov r10, r1
	movs r1, #135
	lsls r1, r1, #2
	adds r3, r7, r1
	ldrh r3, [r3]
	adds r6, r7, #0
	mov r9, r3
	movs r3, #129
	lsls r3, r3, #2
	mov r8, r3
	add r2, r8
	adds r6, #240
	ldrh r0, [r7, r2]
	bl Owner_GetState
	ldr r0, [r6]
	bl RenderOutput_RedrawSavedRectFar
	ldr r5, .L_080fecf4
	ldr r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r1, #226
	lsls r1, r1, #1
	mov r6, r8
	adds r5, r7, r1
.L_080febc8:
	mov r2, r11
	cmp r2, #0
	beq .L_080fec64
	ldr r0, [sp, #0]
	mov r1, r10
	add r0, r10
	bl __modsi3
	str r0, [sp, #0]
	lsls r0, r0, #1
	adds r0, r0, r6
	ldrh r0, [r7, r0]
	bl Owner_GetState
	mov r0, r9
	movs r1, #3
	adds r0, #3
	bl __modsi3
	ldr r3, [sp, #0]
	mov r9, r0
	lsls r3, r3, #1
	adds r3, r3, r6
	ldrh r0, [r7, r3]
	mov r1, r9
	bl Func_080fee40
	ldr r3, [sp, #0]
	adds r0, r7, #0
	lsls r3, r3, #1
	adds r3, r3, r6
	ldrh r1, [r7, r3]
	bl Func_080f88c4
	ldr r3, [sp, #0]
	lsls r3, r3, #1
	adds r3, r3, r6
	ldrh r0, [r7, r3]
	bl Owner_GetState
	movs r2, #0
	adds r1, r5, #0
	bl Func_080fd6f0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r7, r1
	strb r0, [r3]
	adds r0, r5, #0
	bl Func_080fd6b0
	movs r2, #8
	movs r0, #96
	movs r1, #96
	bl Func_081005e4
	adds r0, r5, #0
	bl Func_080facd8
	mov r2, r11
	cmp r2, #2
	bne .L_080fec5a
	ldr r0, [sp, #0]
	cmp r0, #0
	bge .L_080fec4c
	adds r0, #3
.L_080fec4c:
	asrs r0, r0, #2
	lsls r0, r0, #2
	bl Func_08104ef8
	movs r0, #1
	bl WaitFrames
.L_080fec5a:
	ldr r0, [r7, #16]
	ldr r1, [sp, #0]
	mov r2, r10
	bl Func_08104d5c
.L_080fec64:
	ldr r2, [sp, #0]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080fec6e
	adds r3, r2, #3
.L_080fec6e:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #3
	movs r1, #16
	subs r0, #10
	bl Func_080f8a44
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080fecf8
	movs r2, #1
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_080fec9e
	movs r0, #112
	bl Audio_PlayCue
	movs r0, #1
	b .L_080fecc2
.L_080fec9e:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080fecb4
	movs r0, #113
	bl Audio_PlayCue
	movs r0, #1
	negs r0, r0
	b .L_080fecc2
.L_080fecb4:
	mov r0, sp
	mov r1, r10
	movs r2, #4
	bl Func_08104c00
	mov r11, r0
	b .L_080febc8
.L_080fecc2:
	ldr r3, [sp, #0]
	movs r2, #129
	strb r3, [r7, #28]
	ldr r3, [sp, #0]
	lsls r2, r2, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r3, [r7, r3]
	movs r1, #128
	str r3, [r7, #8]
	ldr r3, [sp, #0]
	lsls r1, r1, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r2, [r7, r3]
	adds r1, #22
	adds r3, r7, r1
	strb r2, [r3]
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080fecf4:
	.4byte 0x00001136
.L_080fecf8:
	.4byte gInput
