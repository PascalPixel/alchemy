.syntax unified
	.thumb
	.global Func_080fd968
	.thumb_func
Func_080fd968:
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
	sub sp, #4
	mov r10, r3
	mov r8, r0
	adds r6, r2, #0
	bl RenderOutput_RedrawSavedRectFar
	movs r3, #11
	str r3, [sp, #0]
	movs r2, #11
	movs r3, #16
	mov r0, r8
	movs r1, #0
	bl UiWindow_DrawDividerLineFar
	movs r3, #135
	lsls r3, r3, #2
	add r3, r10
	ldrh r2, [r3]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080fd9b8
	ldr r0, .L_080fdac4
	mov r1, r8
	movs r2, #0
	movs r3, #88
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080fd9c4
.L_080fd9b8:
	ldr r0, .L_080fdac8
	mov r1, r8
	movs r2, #0
	movs r3, #88
	bl UiText_DrawCharacterAtOffsetFar
.L_080fd9c4:
	ldr r2, [r6, #8]
	lsls r3, r2, #2
	adds r5, r3, r2
	ldr r3, [r6, #20]
	subs r3, r3, r5
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r11, r3
	cmp r3, #5
	bls .L_080fd9dc
	movs r1, #5
	mov r11, r1
.L_080fd9dc:
	movs r3, #34
	str r3, [sp, #0]
	movs r0, #5
	adds r1, r5, #0
	mov r2, r8
	movs r3, #112
	bl Menu_SetPageIcons
	movs r2, #15
	ldr r1, [r6, #20]
	ldr r3, [r6, #8]
	mov r0, r8
	str r2, [sp, #0]
	movs r2, #5
	bl Menu_DrawPageIndicator
	movs r2, #96
	movs r3, #0
	ldr r0, .L_080fdacc
	mov r1, r8
	bl UiText_DrawCharacterAtOffsetFar
	movs r2, #0
	mov r3, r11
	mov r9, r2
	cmp r3, #0
	bls .L_080fdab4
	movs r1, #226
	lsls r3, r5, #1
	lsls r1, r1, #1
	adds r6, r3, r1
.L_080fda1a:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r10
	ldrb r0, [r3]
	bl Owner_GetState
	mov r2, r10
	ldrh r3, [r6, r2]
	adds r5, r0, #0
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	bl BattleAction_Get
	adds r7, r0, #0
	ldrb r2, [r7, #9]
	movs r1, #58
	ldrsh r3, [r5, r1]
	cmp r2, r3
	ble .L_080fda4e
	movs r0, #2
	bl UiWork_SetParamNibbleFar
	b .L_080fda70
.L_080fda4e:
	mov r2, r10
	ldrh r3, [r6, r2]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	bl Func_080fe164
	cmp r0, #0
	beq .L_080fda6a
	movs r0, #4
	bl UiWork_SetParamNibbleFar
	b .L_080fda70
.L_080fda6a:
	movs r0, #15
	bl UiWork_SetParamNibbleFar
.L_080fda70:
	mov r1, r10
	ldrh r3, [r6, r1]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	mov r2, r9
	ands r0, r3
	ldr r3, .L_080fdad0
	lsls r5, r2, #4
	adds r5, #8
	adds r0, r0, r3
	mov r1, r8
	movs r2, #16
	adds r3, r5, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldrb r0, [r7, #9]
	movs r3, #104
	movs r1, #2
	mov r2, r8
	str r5, [sp, #0]
	bl UiText_DrawNumberAtOffsetFar
	movs r0, #15
	bl UiWork_SetParamNibbleFar
	mov r3, r9
	adds r3, #1
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r9, r3
	adds r6, #2
	cmp r11, r9
	bhi .L_080fda1a
.L_080fdab4:
	movs r0, #1
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080fdac4:
	.4byte 0x00001010
.L_080fdac8:
	.4byte 0x000010ba
.L_080fdacc:
	.4byte 0x0000101c
.L_080fdad0:
	.4byte 0x000005a7
