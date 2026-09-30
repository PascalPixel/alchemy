.syntax unified
	.thumb
	.global Func_081000e0
	.thumb_func
Func_081000e0:
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
	ldr r3, [r3]
	adds r7, r0, #0
	mov r9, r3
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r0, [r3]
	adds r5, r2, #0
	sub sp, #4
	bl Owner_GetState
	movs r3, #96
	str r3, [sp, #0]
	movs r2, #8
	movs r3, #224
	mov r11, r0
	movs r1, #128
	adds r0, r7, #0
	bl RenderOutput_PrepareForRedrawFar + 0x8
	ldr r2, [r5, #8]
	lsls r3, r2, #2
	adds r3, r3, r2
	mov r8, r3
	ldr r3, [r5, #20]
	mov r2, r8
	subs r3, r3, r2
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r10, r3
	cmp r3, #5
	bls .L_0810013a
	movs r3, #5
	mov r10, r3
.L_0810013a:
	movs r3, #52
	str r3, [sp, #0]
	movs r0, #5
	mov r1, r8
	adds r2, r7, #0
	movs r3, #123
	bl Func_080f92dc
	movs r2, #28
	ldr r3, [r5, #8]
	ldr r1, [r5, #20]
	adds r0, r7, #0
	str r2, [sp, #0]
	movs r2, #5
	bl Menu_DrawPageIndicator
	movs r3, #133
	lsls r3, r3, #2
	add r3, r9
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_08100174
	ldr r0, .L_08100210
	adds r1, r7, #0
	movs r2, #120
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	b .L_081001b2
.L_08100174:
	mov r2, r10
	movs r6, #0
	cmp r2, #0
	bls .L_081001b2
	mov r2, r8
	lsls r3, r2, #1
	movs r2, #226
	add r3, r9
	lsls r2, r2, #1
	adds r5, r3, r2
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	mov r8, r3
.L_08100190:
	ldrh r3, [r5]
	mov r0, r8
	ands r0, r3
	ldr r3, .L_08100214
	adds r1, r7, #0
	adds r0, r0, r3
	lsls r3, r6, #4
	adds r3, #8
	movs r2, #136
	bl UiText_DrawCharacterAtOffsetFar
	adds r3, r6, #1
	lsls r3, r3, #24
	lsrs r6, r3, #24
	adds r5, #2
	cmp r10, r6
	bhi .L_08100190
.L_081001b2:
	mov r0, r11
	adds r1, r7, #0
	movs r2, #40
	movs r3, #0
	bl UiText_DrawStringAtOffsetFar
	ldr r5, .L_08100218
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #32
	movs r3, #16
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #32
	movs r3, #24
	bl UiText_DrawCharacterAtOffsetFar
	mov r2, r11
	movs r3, #16
	ldrh r0, [r2, #60]
	movs r1, #3
	str r3, [sp, #0]
	adds r2, r7, #0
	movs r3, #72
	bl UiText_DrawNumberInWindowFar
	mov r3, r11
	ldrh r0, [r3, #62]
	movs r3, #24
	str r3, [sp, #0]
	movs r1, #3
	adds r2, r7, #0
	movs r3, #72
	bl UiText_DrawNumberInWindowFar
	movs r0, #1
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08100210:
	.4byte 0x00001006
.L_08100214:
	.4byte 0x0000025f
.L_08100218:
	.4byte 0x00001026
