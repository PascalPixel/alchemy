.syntax unified
	.thumb
	.global Func_08101ac8
	.thumb_func
Func_08101ac8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #220
	ldr r2, [r2, #60]
	ldr r7, [r3]
	sub sp, #28
	str r2, [sp, #24]
	adds r6, r1, #0
	ldr r1, [r7, #52]
	movs r3, #1
	str r1, [sp, #20]
	strb r3, [r2, #6]
	mov r11, r0
	ldr r0, [sp, #20]
	bl RenderOutput_RedrawSavedRectFar
	bl Party_CountActiveOwnersFar
	adds r5, r0, #0
	movs r0, #1
	negs r0, r0
	bl Party_SumDjinnCountsFar
	cmp r5, #0
	beq .L_08101b32
	subs r0, #1
	adds r1, r5, #0
	bl Math_Div
	cmp r0, #6
	bgt .L_08101b32
	ldr r0, .L_08101c08
	ldr r1, [sp, #20]
	movs r2, #0
	movs r3, #80
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #10
	str r3, [sp, #0]
	ldr r0, [sp, #20]
	movs r1, #0
	movs r2, #10
	movs r3, #28
	bl UiWindow_DrawDividerLineFar
.L_08101b32:
	mov r0, r11
	bl Func_081019a4
	movs r3, #8
	str r3, [sp, #12]
	adds r1, r6, #0
	lsls r3, r6, #2
	movs r2, #0
	adds r1, #160
	adds r7, #248
	adds r3, r3, r6
	lsls r3, r3, #2
	str r2, [sp, #16]
	str r1, [sp, #8]
	str r7, [sp, #4]
	add r3, r11
	mov r10, r3
.L_08101b54:
	movs r2, #16
	mov r9, r2
	ldr r2, [sp, #4]
	ldmia r2!, {r3}
	adds r1, r2, #0
	str r1, [sp, #4]
	cmp r3, #0
	beq .L_08101bce
	ldr r7, [sp, #8]
	movs r3, #0
	mov r8, r3
.L_08101b6a:
	mov r1, r11
	ldrsb r3, [r1, r7]
	movs r6, #0
	cmp r6, r3
	bge .L_08101bc4
	mov r5, r10
.L_08101b76:
	ldrh r2, [r5]
	movs r3, #224
	ands r3, r2
	lsrs r3, r3, #5
	cmp r8, r3
	bne .L_08101bb8
	ldrh r0, [r5]
	bl Func_08101a04
	cmp r0, #0
	beq .L_08101b92
	cmp r0, #1
	beq .L_08101b9a
	b .L_08101ba2
.L_08101b92:
	movs r0, #2
	bl UiWork_SetParamNibbleFar
	b .L_08101ba8
.L_08101b9a:
	movs r0, #15
	bl UiWork_SetParamNibbleFar
	b .L_08101ba8
.L_08101ba2:
	movs r0, #4
	bl UiWork_SetParamNibbleFar
.L_08101ba8:
	mov r2, r9
	ldrh r3, [r5]
	ldr r0, [sp, #20]
	ldr r1, [sp, #12]
	bl Func_08101a54
	movs r2, #8
	add r9, r2
.L_08101bb8:
	mov r1, r11
	ldrsb r3, [r1, r7]
	adds r6, #1
	adds r5, #2
	cmp r6, r3
	blt .L_08101b76
.L_08101bc4:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #3
	ble .L_08101b6a
.L_08101bce:
	ldr r1, [sp, #8]
	ldr r3, [sp, #16]
	adds r1, #1
	str r1, [sp, #8]
	ldr r1, [sp, #12]
	movs r2, #20
	adds r3, #1
	adds r1, #56
	add r10, r2
	str r3, [sp, #16]
	str r1, [sp, #12]
	cmp r3, #3
	ble .L_08101b54
	ldr r1, [sp, #24]
	movs r2, #0
	movs r3, #1
	strb r3, [r1, #3]
	strb r2, [r1, #6]
	movs r0, #3
	bl WaitFrames
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08101c08:
	.4byte 0x000010de
