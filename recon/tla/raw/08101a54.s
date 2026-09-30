.syntax unified
	.thumb
	.global Func_08101a54
	.thumb_func
Func_08101a54:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r2, #0
	movs r5, #224
	adds r2, r3, #0
	ands r5, r2
	sub sp, #4
	mov r8, r0
	adds r6, r1, #0
	cmp r5, #0
	bge .L_08101a6e
	adds r5, #31
.L_08101a6e:
	asrs r5, r5, #5
	lsls r0, r5, #2
	adds r0, r0, r5
	movs r3, #31
	ands r3, r2
	lsls r0, r0, #2
	adds r0, r0, r3
	ldr r3, .L_08101ac4
	adds r2, r6, #0
	adds r0, r0, r3
	mov r1, r8
	adds r3, r7, #0
	adds r2, #8
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #160
	lsls r3, r3, #7
	adds r3, #1
	adds r1, r6, #0
	adds r5, r5, r3
	cmp r1, #0
	bge .L_08101a9c
	adds r1, #7
.L_08101a9c:
	adds r2, r7, #0
	asrs r4, r1, #3
	cmp r2, #0
	bge .L_08101aa6
	adds r2, #7
.L_08101aa6:
	asrs r3, r2, #3
	movs r2, #0
	str r2, [sp, #0]
	mov r0, r8
	adds r1, r5, #0
	adds r2, r4, #0
	bl UiWindow_SetTilemapEntryFar
	movs r0, #15
	bl Func_080380b8
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_08101ac4:
	.4byte 0x000006d3
