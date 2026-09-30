.syntax unified
	.thumb
	.global Func_0804e1d8
	.thumb_func
Func_0804e1d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	movs r3, #1
	str r3, [sp, #4]
	movs r2, #0
	movs r3, #192
	mov r10, r2
	mov r11, r2
	mov r9, r2
	lsls r3, r3, #18
	add r2, sp, #4
	ldr r3, [r3, #24]
	ldrh r2, [r2]
	movs r0, #1
	strh r2, [r3, #4]
	bl WaitFrames
.L_0804e206:
	ldr r2, .L_0804e3dc
	ldr r3, [r2, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0804e21c
	movs r2, #1
	movs r3, #1
	str r2, [sp, #4]
	negs r3, r3
	add r11, r3
.L_0804e21c:
	ldr r2, .L_0804e3dc
	ldr r3, [r2, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_0804e22e
	movs r3, #1
	str r3, [sp, #4]
	add r11, r3
.L_0804e22e:
	ldr r2, .L_0804e3dc
	ldr r3, [r2, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0804e246
	movs r2, #1
	movs r3, #1
	str r2, [sp, #4]
	negs r3, r3
	add r9, r3
.L_0804e246:
	ldr r2, .L_0804e3dc
	ldr r3, [r2, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0804e25a
	movs r3, #1
	str r3, [sp, #4]
	add r9, r3
.L_0804e25a:
	ldr r2, .L_0804e3dc
	ldr r3, [r2, #12]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0804e268
	b .L_0804e3ba
.L_0804e268:
	ldr r2, .L_0804e3dc
	movs r5, #2
	ldr r3, [r2, #12]
	ands r3, r5
	cmp r3, #0
	beq .L_0804e276
	b .L_0804e3ba
.L_0804e276:
	ldr r3, [sp, #4]
	cmp r3, #0
	bne .L_0804e27e
	b .L_0804e3b2
.L_0804e27e:
	mov r0, r11
	movs r2, #0
	movs r1, #12
	adds r0, #12
	str r2, [sp, #4]
	bl Math_Mod
	mov r11, r0
	mov r0, r9
	movs r1, #3
	adds r0, #3
	bl Math_Mod
	movs r1, #2
	mov r9, r0
	mov r0, r10
	bl UiWork_Finalize
	movs r3, #12
	movs r0, #10
	movs r1, #0
	movs r2, #18
	str r5, [sp, #0]
	bl UiWindow_Create
	mov r3, r9
	mov r10, r0
	cmp r3, #0
	bne .L_0804e2bc
	ldr r0, .L_0804e3e0
	b .L_0804e2c4
.L_0804e2bc:
	mov r2, r9
	cmp r2, #1
	bne .L_0804e2d0
	ldr r0, .L_0804e3e4
.L_0804e2c4:
	mov r1, r10
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringInWindow
	b .L_0804e2dc
.L_0804e2d0:
	ldr r0, .L_0804e3e8
	mov r1, r10
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringInWindow
.L_0804e2dc:
	ldr r0, .L_0804e3ec
	mov r1, r10
	movs r2, #0
	movs r3, #8
	bl UiText_DrawStringInWindow
	movs r3, #8
	str r3, [sp, #0]
	mov r0, r11
	movs r1, #0
	mov r2, r10
	movs r3, #40
	bl UiText_DrawNumberInWindow
	mov r2, r11
	lsls r2, r2, #5
	mov r8, r2
	movs r3, #8
	str r3, [sp, #0]
	mov r0, r8
	movs r1, #3
	mov r2, r10
	movs r3, #64
	bl UiText_DrawNumberInWindow
	ldr r0, .L_0804e3f0
	mov r1, r10
	movs r2, #88
	movs r3, #8
	bl UiText_DrawStringInWindow
	movs r2, #8
	mov r0, r8
	str r2, [sp, #0]
	adds r0, #31
	movs r1, #3
	mov r2, r10
	movs r3, #96
	bl UiText_DrawNumberInWindow
	movs r5, #0
.L_0804e32e:
	movs r3, #1
	negs r3, r3
	str r3, [sp, #12]
	adds r2, r5, #0
	cmp r5, #0
	bge .L_0804e33c
	adds r2, r5, #7
.L_0804e33c:
	asrs r2, r2, #3
	lsls r3, r2, #3
	subs r3, r5, r3
	lsls r2, r2, #4
	lsls r7, r3, #4
	adds r6, r2, #0
	mov r3, r9
	adds r6, #16
	cmp r3, #0
	bne .L_0804e362
	mov r2, r8
	adds r0, r2, r5
	str r3, [sp, #0]
	movs r1, #1
	add r2, sp, #12
	add r3, sp, #8
	bl Func_0803d5c4
	b .L_0804e37a
.L_0804e362:
	mov r3, r9
	cmp r3, #1
	bne .L_0804e38c
	mov r2, r8
	movs r3, #0
	adds r0, r2, r5
	str r3, [sp, #0]
	movs r1, #1
	add r2, sp, #12
	add r3, sp, #8
	bl Func_0803d9bc
.L_0804e37a:
	movs r1, #128
	ldr r0, [sp, #12]
	lsls r1, r1, #23
	mov r2, r10
	adds r3, r7, #0
	str r6, [sp, #0]
	bl RenderOutput_Create
	b .L_0804e3ac
.L_0804e38c:
	bl Resource_FindFreeEntry
	movs r1, #0
	adds r2, r0, #0
	adds r0, r5, #0
	str r2, [sp, #12]
	bl Ui_BuildPatternToSlot
	movs r1, #128
	ldr r0, [sp, #12]
	lsls r1, r1, #23
	mov r2, r10
	adds r3, r7, #0
	str r6, [sp, #0]
	bl RenderOutput_Create
.L_0804e3ac:
	adds r5, #1
	cmp r5, #31
	ble .L_0804e32e
.L_0804e3b2:
	movs r0, #1
	bl WaitFrames
	b .L_0804e206
.L_0804e3ba:
	mov r0, r10
	movs r1, #2
	bl UiWork_Finalize
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #24]
	movs r3, #0
	movs r0, #0
	strh r3, [r2, #4]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0804e3dc:
	.4byte gInput
.L_0804e3e0:
	.4byte Data_0805f8f0
.L_0804e3e4:
	.4byte Data_0805f8f8
.L_0804e3e8:
	.4byte Data_0805f900
.L_0804e3ec:
	.4byte Data_0805f908
.L_0804e3f0:
	.4byte Data_0805f910
