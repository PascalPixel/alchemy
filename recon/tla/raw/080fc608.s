.syntax unified
	.thumb
	.global Func_080fc608
	.thumb_func
Func_080fc608:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	adds r5, r2, #0
	sub sp, #4
	adds r6, r0, #0
	mov r10, r3
	bl RenderOutput_RedrawSavedRectFar
	movs r3, #11
	str r3, [sp, #0]
	movs r2, #11
	movs r3, #16
	adds r0, r6, #0
	movs r1, #0
	bl UiWindow_DrawDividerLineFar
	ldr r2, [r5, #8]
	lsls r3, r2, #2
	adds r7, r3, r2
	ldr r3, [r5, #20]
	subs r3, r3, r7
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r8, r3
	cmp r3, #5
	bls .L_080fc64c
	movs r2, #5
	mov r8, r2
.L_080fc64c:
	movs r3, #34
	str r3, [sp, #0]
	adds r2, r6, #0
	movs r0, #5
	adds r1, r7, #0
	movs r3, #116
	bl Menu_SetPageIcons
	movs r2, #15
	ldr r3, [r5, #8]
	ldr r1, [r5, #20]
	adds r0, r6, #0
	str r2, [sp, #0]
	movs r2, #5
	bl Menu_DrawPageIndicator
	mov r3, r8
	movs r6, #0
	cmp r3, #0
	bls .L_080fc6b0
	lsls r3, r7, #1
	movs r2, #226
	ldr r7, .L_080fc6a8
	add r3, r10
	lsls r2, r2, #1
	adds r5, r3, r2
.L_080fc680:
	ldrh r3, [r5]
	adds r0, r7, #0
	ands r0, r3
	ldr r3, .L_080fc6ac
	movs r2, #24
	adds r0, r0, r3
	mov r3, r10
	ldr r1, [r3, #36]
	lsls r3, r6, #4
	adds r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	adds r3, r6, #1
	lsls r3, r3, #24
	lsrs r6, r3, #24
	adds r5, #2
	cmp r8, r6
	bhi .L_080fc680
	b .L_080fc6b0
	.2byte 0x0000
.L_080fc6a8:
	.4byte 0x000001ff
.L_080fc6ac:
	.4byte 0x0000025f
.L_080fc6b0:
	movs r0, #1
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
