.syntax unified
	.thumb
	.global Func_080fd294
	.thumb_func
Func_080fd294:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	bl Owner_GetState
	movs r3, #226
	lsls r3, r3, #1
	adds r6, r7, r3
	adds r1, r6, #0
	movs r2, #2
	bl Func_080fd6f0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r7, r3
	strb r0, [r5]
	ldr r0, [r7, #36]
	bl RenderOutput_RedrawSavedRectFar
	movs r0, #108
	movs r1, #32
	movs r2, #8
	bl Func_080f8bcc
	adds r0, r6, #0
	bl Func_080fd6b0
	ldrb r3, [r5]
	cmp r3, #0
	bne .L_080fd2e0
	ldr r0, .L_080fd2e4
	ldr r1, [r7, #36]
	movs r2, #0
	movs r3, #24
	bl UiText_DrawCharacterAtOffsetFar
.L_080fd2e0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080fd2e4:
	.4byte 0x00001021
