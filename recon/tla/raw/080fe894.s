.syntax unified
	.thumb
	.global Func_080fe894
	.thumb_func
Func_080fe894:
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
	sub sp, #8
	movs r2, #28
	ldrsb r2, [r7, r2]
	movs r3, #2
	str r2, [sp, #4]
	mov r9, r3
	movs r1, #30
	ldrsb r1, [r7, r1]
	lsls r2, r2, #1
	mov r8, r1
	movs r1, #0
	str r1, [sp, #0]
	movs r1, #135
	lsls r1, r1, #2
	adds r3, r7, r1
	ldrh r3, [r3]
	mov r10, r3
	movs r3, #129
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrh r0, [r7, r2]
	bl Owner_GetState
	movs r1, #140
	ldr r0, .L_080fe910
	lsls r1, r1, #2
	adds r3, r7, r1
	movs r2, #3
	movs r1, #130
.L_080fe8e4:
	subs r2, #1
	strh r1, [r3]
	strh r0, [r3, #8]
	adds r1, #32
	adds r3, #2
	cmp r2, #0
	bge .L_080fe8e4
	movs r0, #14
	bl Func_080f9108
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_080fe914
	lsls r1, r1, #19
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_080fe918
.L_080fe910:
	.4byte 0x00000080
.L_080fe914:
	.4byte 0x05000200
.L_080fe918:
	movs r2, #128
	lsls r2, r2, #24
	ldr r0, .L_080feb40
	adds r1, #28
	adds r2, #1
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #24
	ldr r0, .L_080feb44
	adds r1, #4
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #24
	ldr r0, .L_080feb48
	adds r1, #28
	adds r2, #1
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #129
	lsls r2, r2, #2
	mov r11, r2
	b .L_080feae8
.L_080fe94a:
	mov r3, r9
	cmp r3, #0
	beq .L_080fe9f2
	adds r5, r7, #0
	adds r5, #240
	ldr r0, [r5]
	bl RenderOutput_RedrawSavedRectFar
	ldr r6, .L_080feb4c
	ldr r1, [r5]
	adds r0, r6, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #55
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080fe97e
	ldr r1, [r5]
	ldr r0, .L_080feb50
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
.L_080fe97e:
	movs r2, #0
	movs r3, #8
	ldr r1, [r5]
	subs r0, r6, #3
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, [sp, #4]
	mov r1, r8
	add r0, r8
	bl Math_Mod
	str r0, [sp, #4]
	lsls r0, r0, #1
	add r0, r11
	ldrh r0, [r7, r0]
	bl Owner_GetState
	mov r0, r10
	movs r1, #3
	adds r0, #3
	bl Math_Mod
	ldr r3, [sp, #4]
	mov r10, r0
	lsls r3, r3, #1
	add r3, r11
	ldrh r0, [r7, r3]
	mov r1, r10
	bl Func_080fee40
	ldr r3, [sp, #4]
	adds r0, r7, #0
	lsls r3, r3, #1
	add r3, r11
	ldrh r1, [r7, r3]
	bl Func_080f88c4
	mov r1, r9
	cmp r1, #2
	bne .L_080fe9e4
	ldr r0, [sp, #4]
	cmp r0, #0
	bge .L_080fe9d6
	adds r0, #3
.L_080fe9d6:
	asrs r0, r0, #2
	lsls r0, r0, #2
	bl Func_08104ef8
	movs r0, #1
	bl WaitFrames
.L_080fe9e4:
	mov r2, r8
	ldr r0, [r7, #16]
	ldr r1, [sp, #4]
	bl Func_08104d5c
	movs r2, #0
	mov r9, r2
.L_080fe9f2:
	ldr r2, [sp, #4]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080fe9fc
	adds r3, r2, #3
.L_080fe9fc:
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
	ldr r1, .L_080feb54
	movs r3, #1
	ldr r2, [r1, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_080fea2e
	movs r0, #112
	bl Audio_PlayCue
	movs r3, #1
	str r3, [sp, #0]
	b .L_080feaf6
.L_080fea2e:
	ldr r2, [r1, #4]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080fea46
	movs r0, #113
	bl Audio_PlayCue
	movs r1, #1
	negs r1, r1
	str r1, [sp, #0]
	b .L_080feaf6
.L_080fea46:
	ldr r2, [r1, #12]
	movs r3, #128
	lsls r3, r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_080fea72
	ldr r0, [sp, #4]
	movs r1, #1
	bl Func_080fecfc
	cmp r0, #0
	beq .L_080feab0
	movs r0, #112
	bl Audio_PlayCue
	ldr r2, [sp, #4]
	adds r3, r2, #1
	str r3, [sp, #4]
	cmp r3, #0
	bge .L_080fea9c
	adds r3, r2, #4
	b .L_080fea9c
.L_080fea72:
	ldr r2, [r1, #12]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080feabe
	ldr r0, [sp, #4]
	movs r1, #0
	bl Func_080fecfc
	cmp r0, #0
	beq .L_080feab0
	movs r0, #112
	bl Audio_PlayCue
	ldr r2, [sp, #4]
	subs r3, r2, #1
	str r3, [sp, #4]
	cmp r3, #0
	bge .L_080fea9c
	adds r3, r2, #2
.L_080fea9c:
	asrs r0, r3, #2
	lsls r0, r0, #2
	bl Func_08104ef8
	ldr r0, [r7, #16]
	ldr r1, [sp, #4]
	mov r2, r8
	bl Func_08104d5c
	b .L_080feab6
.L_080feab0:
	movs r0, #114
	bl Audio_PlayCue
.L_080feab6:
	movs r0, #1
	bl WaitFrames
	b .L_080feae8
.L_080feabe:
	ldr r3, [r1, #4]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	beq .L_080feadc
	movs r0, #55
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080feadc
	bl Func_080fe638
	movs r2, #1
	mov r9, r2
	b .L_080feae8
.L_080feadc:
	add r0, sp, #4
	mov r1, r8
	movs r2, #4
	bl Func_08104c00
	mov r9, r0
.L_080feae8:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080feaf6
	b .L_080fe94a
.L_080feaf6:
	ldr r3, [sp, #4]
	movs r2, #129
	strb r3, [r7, #28]
	ldr r3, [sp, #4]
	lsls r2, r2, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r3, [r7, r3]
	movs r1, #128
	str r3, [r7, #8]
	ldr r3, [sp, #4]
	lsls r1, r1, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r2, [r7, r3]
	adds r1, #22
	adds r3, r7, r1
	strb r2, [r3]
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r3, [r3]
	movs r2, #13
	subs r1, #154
	strb r2, [r3, #5]
	adds r3, r7, r1
	ldr r3, [r3]
	strb r2, [r3, #5]
	ldr r0, [sp, #0]
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080feb40:
	.4byte 0x050001c8
.L_080feb44:
	.4byte 0x05000200
.L_080feb48:
	.4byte 0x050001e8
.L_080feb4c:
	.4byte 0x0000103c
.L_080feb50:
	.4byte 0x00001045
.L_080feb54:
	.4byte gInput
