.syntax unified
	.thumb
	.global Func_080fd8a0
	.thumb_func
Func_080fd8a0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r8, r2
	movs r3, #192
	ldr r2, [r2, #8]
	lsls r3, r3, #18
	adds r3, #220
	mov r1, r8
	ldr r7, [r3]
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, [r1, #16]
	movs r0, #82
	adds r3, r3, r2
	str r3, [r1, #24]
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fd906
	ldr r0, [r7, #48]
	bl RenderOutput_RedrawSavedRectFar
	movs r0, #1
	bl WaitFrames
	mov r2, r8
	ldr r3, [r2, #24]
	movs r1, #226
	lsls r3, r3, #1
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r2, [r7, r3]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080fd910
	movs r0, #128
	ldr r3, .L_080fd964
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r2
	adds r0, r0, r3
	ldr r1, [r7, #48]
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080fd910
.L_080fd906:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_ClearBit
.L_080fd910:
	movs r2, #1
	movs r6, #0
	mov r10, r2
	movs r5, #1
.L_080fd918:
	mov r1, r8
	ldr r3, [r1, #16]
	cmp r6, r3
	bne .L_080fd934
	mov r2, r10
	ldr r0, [r7, #36]
	movs r3, #14
	str r2, [sp, #0]
	movs r1, #1
	adds r2, r5, #0
	str r3, [sp, #4]
	bl Render_SetTilemapFlagRect
	b .L_080fd948
.L_080fd934:
	mov r3, r10
	ldr r0, [r7, #36]
	str r3, [sp, #0]
	movs r3, #15
	str r3, [sp, #4]
	movs r1, #1
	adds r2, r5, #0
	movs r3, #14
	bl Render_SetTilemapFlagRect
.L_080fd948:
	adds r6, #1
	adds r5, #2
	cmp r6, #4
	ble .L_080fd918
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080fd964:
	.4byte 0x00000885
