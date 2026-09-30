.syntax unified
	.thumb
	.global PsynergyMenu_DrawListPage
	.thumb_func
PsynergyMenu_DrawListPage:
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
	mov r8, r0
	mov r11, r3
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r11
	ldrb r0, [r3]
	adds r5, r2, #0
	sub sp, #8
	bl Owner_GetState
	str r0, [sp, #4]
	mov r0, r8
	bl RenderOutput_RedrawSavedRectFar
	ldr r2, [r5, #8]
	lsls r3, r2, #2
	adds r6, r3, r2
	ldr r3, [r5, #20]
	subs r3, r3, r6
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r9, r3
	cmp r3, #5
	bls .L_080ffba8
	movs r1, #5
	mov r9, r1
.L_080ffba8:
	movs r3, #58
	str r3, [sp, #0]
	movs r0, #5
	adds r1, r6, #0
	mov r2, r8
	movs r3, #80
	bl Menu_SetPageIcons
	movs r2, #28
	ldr r1, [r5, #20]
	ldr r3, [r5, #8]
	mov r0, r8
	str r2, [sp, #0]
	movs r2, #5
	bl Menu_DrawPageIndicator
	movs r2, #176
	movs r3, #0
	ldr r0, .L_080ffcc0
	mov r1, r8
	bl UiText_DrawCharacterAtOffsetFar
	movs r2, #0
	mov r3, r9
	mov r10, r2
	cmp r3, #0
	bls .L_080ffc56
	movs r1, #226
	lsls r3, r6, #1
	lsls r1, r1, #1
	adds r7, r3, r1
.L_080ffbe6:
	mov r2, r11
	ldrh r3, [r7, r2]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	bl BattleAction_Get
	mov r1, r11
	ldrh r3, [r7, r1]
	adds r6, r0, #0
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	mov r2, r10
	ldr r3, .L_080ffcc4
	lsls r5, r2, #4
	adds r5, #16
	adds r0, r0, r3
	mov r1, r8
	movs r2, #88
	adds r3, r5, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldrb r0, [r6, #9]
	movs r1, #2
	mov r2, r8
	movs r3, #176
	str r5, [sp, #0]
	bl UiText_DrawNumberAtOffsetFar
	ldrb r4, [r6, #8]
	cmp r4, #255
	bne .L_080ffc30
	movs r4, #11
	b .L_080ffc32
.L_080ffc30:
	subs r4, #1
.L_080ffc32:
	mov r3, r10
	lsls r2, r3, #1
	movs r3, #0
	str r3, [sp, #0]
	adds r2, #2
	adds r3, r4, #0
	mov r0, r8
	movs r1, #25
	bl Func_080ff8e0
	mov r3, r10
	adds r3, #1
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r10, r3
	adds r7, #2
	cmp r9, r10
	bhi .L_080ffbe6
.L_080ffc56:
	movs r3, #133
	lsls r3, r3, #2
	add r3, r11
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080ffc6e
	ldr r0, .L_080ffcc8
	mov r1, r8
	movs r2, #96
	movs r3, #17
	bl UiText_DrawCharacterAtOffsetFar
.L_080ffc6e:
	ldr r0, [sp, #4]
	mov r1, r8
	movs r2, #40
	movs r3, #0
	bl UiText_DrawStringAtOffsetFar
	ldr r1, [sp, #4]
	movs r2, #42
	adds r2, #255
	adds r3, r1, r2
	ldrb r0, [r3]
	ldr r3, .L_080ffccc
	mov r1, r8
	adds r0, r0, r3
	movs r2, #0
	movs r3, #32
	bl UiText_DrawCharacterAtOffsetFar
	mov r1, r8
	ldr r0, .L_080ffcd0
	movs r2, #0
	movs r3, #48
	bl UiText_DrawStringInWindowFar
	ldr r3, [sp, #4]
	movs r1, #2
	ldrb r0, [r3, #15]
	movs r3, #48
	str r3, [sp, #0]
	mov r2, r8
	movs r3, #24
	bl UiText_DrawNumberInWindowFar
	movs r0, #1
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ffcc0:
	.4byte 0x0000101c
.L_080ffcc4:
	.4byte 0x000005a7
.L_080ffcc8:
	.4byte 0x0000101e
.L_080ffccc:
	.4byte 0x00000b63
.L_080ffcd0:
	.4byte Data_08105974
