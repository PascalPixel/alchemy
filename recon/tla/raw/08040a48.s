.syntax unified
	.thumb
	.global Func_08040a48
	.thumb_func
Func_08040a48:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_08040c48
	sub sp, #24
	movs r2, #8
	movs r1, #0
	add r2, sp
	movs r0, #1
	mov r8, r1
	mov r9, r0
	mov r11, r2
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldr r3, [r3]
	mov r10, r11
	str r3, [r2]
	bl Func_080409a4
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r3, [r3]
	str r3, [sp, #4]
	bl Func_080409f0
	adds r7, r0, #0
	movs r0, #1
	bl WaitFrames
.L_08040a8c:
	mov r2, r9
	cmp r2, #0
	beq .L_08040b42
	mov r0, r8
	movs r3, #0
	movs r1, #4
	adds r0, #4
	mov r9, r3
	bl Math_Mod
	mov r4, r10
	mov r8, r0
	ldr r0, [r4, #12]
	movs r1, #5
	adds r0, #5
	bl Math_Mod
	mov r1, r10
	str r0, [r1, #12]
	adds r0, r7, #0
	bl RenderOutput_RedrawSavedRect
	movs r6, #2
	movs r5, #2
.L_08040abc:
	adds r2, r6, #0
	adds r0, r7, #0
	movs r1, #0
	movs r3, #14
	subs r5, #1
	str r6, [sp, #0]
	bl UiWindow_DrawDividerLine
	adds r6, #2
	cmp r5, #0
	bge .L_08040abc
	movs r5, #0
	mov r6, r11
.L_08040ad6:
	lsls r3, r5, #4
	ldmia r6!, {r0}
	movs r1, #0
	str r3, [sp, #0]
	adds r2, r7, #0
	movs r3, #72
	adds r5, #1
	bl UiText_DrawNumberInWindow
	cmp r5, #3
	ble .L_08040ad6
	ldr r0, .L_08040c4c
	adds r1, r7, #0
	movs r2, #8
	movs r3, #0
	bl UiText_DrawStringInWindow
	ldr r0, .L_08040c50
	adds r1, r7, #0
	movs r2, #8
	movs r3, #16
	bl UiText_DrawStringInWindow
	ldr r0, .L_08040c54
	adds r1, r7, #0
	movs r2, #8
	movs r3, #32
	bl UiText_DrawStringInWindow
	movs r2, #8
	movs r3, #48
	ldr r0, .L_08040c58
	adds r1, r7, #0
	bl UiText_DrawStringInWindow
	movs r2, #12
	ldrsh r1, [r7, r2]
	movs r3, #14
	ldrsh r2, [r7, r3]
	mov r4, r8
	lsls r3, r4, #4
	lsls r2, r2, #3
	adds r2, r2, r3
	movs r4, #160
	ldr r3, [sp, #4]
	lsls r4, r4, #3
	lsls r1, r1, #3
	adds r4, #164
	adds r0, r3, r4
	subs r1, #4
	adds r2, #12
	movs r3, #3
	bl Func_08108040
.L_08040b42:
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_08040c5c
	movs r2, #2
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_08040c26
	ldr r3, [r5, #4]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08040b7a
	mov r0, r8
	cmp r0, #3
	bne .L_08040b6e
	mov r1, r11
	ldr r0, [r1, #12]
	bl Sound_LoadPresetParameters
	b .L_08040b7a
.L_08040b6e:
	mov r2, r8
	lsls r3, r2, #2
	mov r4, r11
	ldr r0, [r4, r3]
	bl Audio_PlayCue
.L_08040b7a:
	ldr r3, [r5, #4]
	movs r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L_08040b92
	movs r0, #195
	lsls r0, r0, #1
	bl Audio_PlayCue
	movs r0, #0
	bl Audio_PlayCue
.L_08040b92:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08040bae
	mov r0, r8
	lsls r2, r0, #2
	mov r1, r10
	ldr r3, [r1, r2]
	adds r3, #10
	str r3, [r1, r2]
	movs r2, #1
	mov r9, r2
.L_08040bae:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08040bca
	mov r3, r8
	lsls r2, r3, #2
	mov r4, r10
	ldr r3, [r4, r2]
	movs r0, #1
	subs r3, #10
	str r3, [r4, r2]
	mov r9, r0
.L_08040bca:
	ldr r3, [r5, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_08040be4
	mov r1, r8
	lsls r2, r1, #2
	mov r4, r10
	ldr r3, [r4, r2]
	movs r0, #1
	adds r3, #1
	str r3, [r4, r2]
	mov r9, r0
.L_08040be4:
	ldr r3, [r5, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_08040bfe
	mov r1, r8
	lsls r2, r1, #2
	mov r4, r10
	ldr r3, [r4, r2]
	movs r0, #1
	subs r3, #1
	str r3, [r4, r2]
	mov r9, r0
.L_08040bfe:
	ldr r3, [r5, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_08040c12
	movs r1, #1
	negs r1, r1
	movs r2, #1
	add r8, r1
	mov r9, r2
.L_08040c12:
	ldr r3, [r5, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	bne .L_08040c1e
	b .L_08040a8c
.L_08040c1e:
	movs r3, #1
	add r8, r3
	mov r9, r3
	b .L_08040a8c
.L_08040c26:
	adds r0, r7, #0
	movs r1, #2
	bl UiWork_Finalize
	bl Func_080409dc
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08040c48:
	.4byte Data_0805ea98
.L_08040c4c:
	.4byte Data_0805eaa8
.L_08040c50:
	.4byte Data_0805eab0
.L_08040c54:
	.4byte Data_0805eab8
.L_08040c58:
	.4byte Data_0805eabc
.L_08040c5c:
	.4byte gInput
