.syntax unified
	.thumb
	.global Menu_DrawModeIndicator
	.thumb_func
Menu_DrawModeIndicator:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #232
	ldr r6, [r3]
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
	beq .L_0804d7ee
	strh r1, [r0]
	ldr r0, [r6, #124]
	bl RenderOutput_PrepareForRedraw
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_0804d78a
	cmp r3, #1
	beq .L_0804d7a8
	b .L_0804d7d2
.L_0804d78a:
	ldr r5, .L_0804d7f0
	ldr r1, [r6, #124]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #4
	adds r5, #1
	bl UiText_DrawResource
	ldr r1, [r6, #124]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #16
	bl UiText_DrawResource
	b .L_0804d7ee
.L_0804d7a8:
	ldr r5, .L_0804d7f4
	ldr r1, [r6, #124]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #4
	bl UiText_DrawResource
	adds r0, r5, #1
	ldr r1, [r6, #124]
	movs r2, #0
	movs r3, #16
	adds r5, #2
	bl UiText_DrawResource
	ldr r1, [r6, #124]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #28
	bl UiText_DrawResource
	b .L_0804d7ee
.L_0804d7d2:
	ldr r5, .L_0804d7f8
	ldr r1, [r6, #124]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #4
	adds r5, #1
	bl UiText_DrawResource
	ldr r1, [r6, #124]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #16
	bl UiText_DrawResource
.L_0804d7ee:
	pop {r5, r6, pc}
.L_0804d7f0:
	.4byte 0x00001171
.L_0804d7f4:
	.4byte 0x00001173
.L_0804d7f8:
	.4byte 0x00001176
