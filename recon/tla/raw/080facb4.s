.syntax unified
	.thumb
	.global Func_080facb4
	.thumb_func
Func_080facb4:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	adds r6, r1, #0
	adds r5, #240
	ldr r0, [r5]
	bl RenderOutput_RedrawSavedRectFar
	ldr r1, [r5]
	adds r0, r6, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	pop {r5, r6, pc}
	.2byte 0x0000
