.syntax unified
	.thumb
	.global Func_080fae2c
	.thumb_func
Func_080fae2c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	mov r8, r1
	adds r6, r0, #0
	bl Owner_GetState
	movs r2, #226
	lsls r2, r2, #1
	adds r5, r7, r2
	adds r1, r5, #0
	movs r2, #0
	bl ItemMenu_Collect
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	strb r0, [r3]
	ldr r0, [r7, #36]
	bl RenderOutput_RedrawSavedRectFar
	mov r0, r8
	bl ItemMenu_RefreshEntry
	adds r0, r5, #0
	movs r1, #0
	bl ItemMenu_DrawIcons
	adds r0, r6, #0
	bl Func_080fad1c
	cmp r0, #0
	bne .L_080fae82
	ldr r0, .L_080fae88
	ldr r1, [r7, #36]
	movs r2, #8
	movs r3, #24
	bl UiText_DrawCharacterAtOffsetFar
.L_080fae82:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080fae88:
	.4byte 0x00001006
