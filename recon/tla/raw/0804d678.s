.syntax unified
	.thumb
	.global Menu_DrawModeLabel
	.thumb_func
Menu_DrawModeLabel:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #232
	ldr r6, [r3]
	sub sp, #4
	adds r0, r6, #0
	adds r5, r6, #0
	adds r5, #140
	adds r0, #150
	movs r3, #0
	ldrsh r2, [r0, r3]
	movs r4, #0
	ldrsh r3, [r5, r4]
	ldrh r1, [r5]
	cmp r2, r3
	beq .L_0804d74c
	strh r1, [r0]
	movs r3, #80
	ldr r0, [r6, #124]
	movs r2, #40
	str r3, [sp, #0]
	movs r1, #8
	movs r3, #128
	bl UiWindow_ClearInteriorTiles
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #1
	beq .L_0804d706
	cmp r3, #1
	bgt .L_0804d6be
	cmp r3, #0
	beq .L_0804d6c4
	b .L_0804d74c
.L_0804d6be:
	cmp r3, #2
	beq .L_0804d730
	b .L_0804d74c
.L_0804d6c4:
	ldr r5, .L_0804d750
	ldr r1, [r6, #124]
	adds r0, r5, #0
	movs r2, #8
	movs r3, #40
	bl UiText_DrawCharacterAtOffset
	adds r0, r5, #1
	ldr r1, [r6, #124]
	movs r2, #8
	movs r3, #48
	bl UiText_DrawCharacterAtOffset
	adds r0, r5, #2
	ldr r1, [r6, #124]
	movs r2, #8
	movs r3, #56
	bl UiText_DrawCharacterAtOffset
	adds r0, r5, #3
	ldr r1, [r6, #124]
	movs r2, #8
	movs r3, #64
	adds r5, #4
	bl UiText_DrawCharacterAtOffset
	ldr r1, [r6, #124]
	adds r0, r5, #0
	movs r2, #8
	movs r3, #72
	bl UiText_DrawCharacterAtOffset
	b .L_0804d74c
.L_0804d706:
	ldr r5, .L_0804d750
	ldr r1, [r6, #124]
	adds r0, r5, #0
	movs r2, #8
	movs r3, #40
	bl UiText_DrawCharacterAtOffset
	adds r0, r5, #1
	ldr r1, [r6, #124]
	movs r2, #8
	movs r3, #48
	adds r5, #2
	bl UiText_DrawCharacterAtOffset
	ldr r1, [r6, #124]
	adds r0, r5, #0
	movs r2, #8
	movs r3, #56
	bl UiText_DrawCharacterAtOffset
	b .L_0804d74c
.L_0804d730:
	ldr r5, .L_0804d750
	ldr r1, [r6, #124]
	adds r0, r5, #0
	movs r2, #8
	movs r3, #40
	adds r5, #1
	bl UiText_DrawCharacterAtOffset
	ldr r1, [r6, #124]
	adds r0, r5, #0
	movs r2, #8
	movs r3, #48
	bl UiText_DrawCharacterAtOffset
.L_0804d74c:
	add sp, #4
	pop {r5, r6, pc}
.L_0804d750:
	.4byte 0x0000117c
