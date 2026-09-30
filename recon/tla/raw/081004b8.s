.syntax unified
	.thumb
	.global Func_081004b8
	.thumb_func
Func_081004b8:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	adds r6, r0, #0
	mov r10, r2
	mov r8, r3
	bl ItemMenu_PosCategory
	bl ItemMenu_HideAllIcons
	ldr r5, .L_0810053c
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #1
	adds r1, r6, #0
	movs r2, #0
	movs r3, #32
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #2
	adds r1, r6, #0
	movs r2, #0
	movs r3, #16
	adds r5, #3
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	movs r5, #226
	lsls r5, r5, #1
	movs r3, #48
	adds r1, r6, #0
	movs r2, #0
	add r5, r8
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_08100540
	mov r3, r10
	cmp r3, #0
	bne .L_08100532
	movs r0, #1
	bl WaitFrames
	adds r0, r5, #0
	movs r1, #1
	bl Func_080fadd0
	adds r0, r5, #0
	bl ItemMenu_ArrangeCategoryItemIcons
.L_08100532:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0810053c:
	.4byte 0x00001053
