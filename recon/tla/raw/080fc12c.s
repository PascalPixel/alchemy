.syntax unified
	.thumb
	.global Func_080fc12c
	.thumb_func
Func_080fc12c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r3, #134
	lsls r3, r3, #2
	adds r5, r6, r3
	ldr r3, [r5]
	movs r2, #182
	lsls r2, r2, #1
	adds r2, r2, r6
	ldrh r1, [r2]
	mov r8, r2
	movs r0, #2
	ldrb r2, [r3, #14]
	movs r3, #0
	bl UiWindow_SetTilemapEntryFar + 0x18
	ldr r2, [r5]
	movs r3, #1
	strb r3, [r2, #5]
	movs r3, #112
	ldr r2, [r5]
	strh r3, [r2, #6]
	movs r3, #8
	ldr r2, [r5]
	strh r3, [r2, #8]
	ldr r0, [r5]
	bl UiIcon_PrepareObject
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #22
	adds r3, r6, r2
	ldrb r0, [r3]
	adds r6, #240
	bl Owner_GetState
	ldr r1, [r6]
	movs r2, #16
	movs r3, #0
	bl UiText_DrawStringAtOffsetFar
	mov r2, r8
	ldrh r3, [r2]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	ldr r3, .L_080fc1a8
	ldr r1, [r6]
	adds r0, r0, r3
	movs r2, #16
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_080fc1a8:
	.4byte 0x0000025f
