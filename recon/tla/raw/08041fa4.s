.syntax unified
	.thumb
	.global UiText_DrawResource
	.thumb_func
UiText_DrawResource:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	mov r11, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #60]
	mov r9, r2
	movs r2, #152
	movs r3, #0
	lsls r2, r2, #5
	mov r8, r3
	adds r2, #66
	adds r6, r5, r2
	mov r2, r8
	mov r10, r1
	strh r2, [r6]
	movs r1, #1
	bl UiText_BuildRenderEntries
	ldrh r3, [r6]
	movs r1, #244
	lsls r1, r1, #4
	lsls r3, r3, #1
	adds r3, r3, r1
	mov r2, r8
	strh r2, [r5, r3]
	ldr r2, .L_08041ffc
	ldrh r3, [r6]
	adds r5, r5, r1
	adds r3, #1
	ands r3, r2
	strh r3, [r6]
	adds r0, r5, #0
	mov r1, r10
	mov r2, r9
	mov r3, r11
	bl UiText_RenderWideStringAtOffset
	b .L_08042000
.L_08041ffc:
	.4byte 0x000001ff
.L_08042000:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
	.2byte 0x0000
