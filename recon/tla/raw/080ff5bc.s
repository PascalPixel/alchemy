.syntax unified
	.thumb
	.global Func_080ff5bc
	.thumb_func
Func_080ff5bc:
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
	adds r6, r0, #0
	adds r0, r1, #0
	mov r8, r2
	ldr r5, [r3]
	sub sp, #4
	bl Owner_GetState
	movs r2, #184
	lsls r2, r2, #1
	adds r5, r5, r2
	ldr r2, [r5]
	movs r3, #1
	strb r3, [r2, #5]
	mov r2, r8
	adds r3, #255
	ands r2, r3
	adds r7, r0, #0
	mov r8, r2
	cmp r2, #0
	bne .L_080ff608
	movs r3, #40
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	movs r3, #128
	bl UiWindow_ClearInteriorTilesFar
.L_080ff608:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #40
	movs r3, #0
	bl UiText_DrawStringAtOffsetFar
	movs r2, #42
	adds r2, #255
	adds r3, r7, r2
	ldrb r0, [r3]
	ldr r3, .L_080ff79c
	adds r1, r6, #0
	adds r0, r0, r3
	movs r2, #0
	movs r3, #32
	bl UiText_DrawCharacterAtOffsetFar
	adds r1, r6, #0
	movs r2, #104
	movs r3, #0
	ldr r0, .L_080ff7a0
	bl UiText_DrawStringAtOffsetFar
	movs r0, #15
	bl Func_080380b8
	movs r3, #0
	ldrb r0, [r7, #15]
	movs r1, #2
	str r3, [sp, #0]
	adds r2, r6, #0
	movs r3, #128
	bl UiText_DrawNumberInWindowFar
	movs r2, #40
	ldr r0, .L_080ff7a4
	adds r1, r6, #0
	movs r3, #16
	bl UiText_DrawStringAtOffsetFar
	movs r3, #16
	movs r2, #56
	ldrsh r0, [r7, r2]
	mov r9, r3
	str r3, [sp, #0]
	adds r2, r6, #0
	movs r1, #4
	movs r3, #72
	bl UiText_DrawNumberInWindowFar
	mov r3, r9
	movs r2, #52
	ldrsh r0, [r7, r2]
	movs r1, #4
	str r3, [sp, #0]
	adds r2, r6, #0
	movs r3, #112
	bl UiText_DrawNumberInWindowFar
	ldr r5, .L_080ff7a8
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #104
	movs r3, #16
	bl UiText_DrawStringInWindowFar
	movs r2, #40
	ldr r0, .L_080ff7ac
	adds r1, r6, #0
	movs r3, #24
	bl UiText_DrawStringAtOffsetFar
	movs r3, #24
	movs r2, #58
	ldrsh r0, [r7, r2]
	mov r10, r3
	str r3, [sp, #0]
	adds r2, r6, #0
	movs r1, #4
	movs r3, #72
	bl UiText_DrawNumberInWindowFar
	mov r3, r10
	movs r2, #54
	ldrsh r0, [r7, r2]
	movs r1, #4
	str r3, [sp, #0]
	adds r2, r6, #0
	movs r3, #112
	bl UiText_DrawNumberInWindowFar
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #104
	movs r3, #24
	bl UiText_DrawStringInWindowFar
	ldr r5, .L_080ff7b0
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #40
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	movs r2, #146
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r0, [r3]
	movs r3, #8
	str r3, [sp, #0]
	adds r2, r6, #0
	mov r11, r3
	movs r1, #8
	movs r3, #80
	bl UiText_DrawNumberInWindowFar
	mov r2, r8
	cmp r2, #0
	bne .L_080ff70c
	movs r0, #1
	bl WaitFrames
	movs r3, #40
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #144
	movs r2, #0
	movs r3, #224
	bl UiWindow_ClearInteriorTilesFar
.L_080ff70c:
	adds r0, r5, #0
	adds r1, r6, #0
	subs r0, #23
	movs r2, #152
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	adds r1, r6, #0
	subs r0, #22
	movs r2, #152
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	adds r1, r6, #0
	subs r0, #21
	movs r2, #152
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	adds r1, r6, #0
	subs r0, #20
	movs r2, #152
	movs r3, #24
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #0
	ldrh r0, [r7, #60]
	adds r2, r6, #0
	str r3, [sp, #0]
	movs r1, #3
	movs r3, #200
	bl UiText_DrawNumberInWindowFar
	mov r2, r11
	ldrh r0, [r7, #62]
	movs r1, #3
	str r2, [sp, #0]
	movs r3, #200
	adds r2, r6, #0
	bl UiText_DrawNumberInWindowFar
	adds r3, r7, #0
	adds r3, #64
	ldrh r0, [r3]
	mov r3, r9
	str r3, [sp, #0]
	adds r2, r6, #0
	movs r1, #3
	movs r3, #200
	bl UiText_DrawNumberInWindowFar
	adds r3, r7, #0
	mov r2, r10
	adds r3, #66
	ldrb r0, [r3]
	movs r1, #3
	str r2, [sp, #0]
	movs r3, #200
	adds r2, r6, #0
	bl UiText_DrawNumberInWindowFar
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ff79c:
	.4byte 0x00000b63
.L_080ff7a0:
	.4byte Data_08105974
.L_080ff7a4:
	.4byte Data_0810597c
.L_080ff7a8:
	.4byte Data_08105978
.L_080ff7ac:
	.4byte Data_08105980
.L_080ff7b0:
	.4byte 0x0000103d
