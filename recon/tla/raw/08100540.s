.syntax unified
	.thumb
	.global ItemMenu_DrawEquippedItemNames
	.thumb_func
ItemMenu_DrawEquippedItemNames:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_08100584
	adds r6, r0, #0
	mov r8, r3
	movs r3, #14
	adds r7, r1, #0
	mov r10, r3
.L_08100554:
	ldrh r0, [r7]
	ldr r3, .L_0810057c
	adds r7, #2
	ands r3, r0
	cmp r3, #0
	beq .L_081005d0
	ldr r3, .L_08100580
	adds r5, r3, #0
	ands r5, r0
	adds r0, r5, #0
	bl Item_Get
	ldrb r3, [r0, #2]
	cmp r3, #2
	beq .L_081005a2
	cmp r3, #2
	bgt .L_08100588
	cmp r3, #1
	beq .L_08100592
	b .L_081005d0
.L_0810057c:
	.4byte 0x00000200
.L_08100580:
	.4byte 0x000001ff
.L_08100584:
	.4byte 0x0000025f
.L_08100588:
	cmp r3, #3
	beq .L_081005b2
	cmp r3, #4
	beq .L_081005c2
	b .L_081005d0
.L_08100592:
	mov r3, r8
	adds r0, r5, r3
	adds r1, r6, #0
	movs r2, #16
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	b .L_081005d0
.L_081005a2:
	mov r3, r8
	adds r0, r5, r3
	adds r1, r6, #0
	movs r2, #16
	movs r3, #56
	bl UiText_DrawCharacterAtOffsetFar
	b .L_081005d0
.L_081005b2:
	mov r3, r8
	adds r0, r5, r3
	adds r1, r6, #0
	movs r2, #16
	movs r3, #40
	bl UiText_DrawCharacterAtOffsetFar
	b .L_081005d0
.L_081005c2:
	mov r3, r8
	adds r0, r5, r3
	adds r1, r6, #0
	movs r2, #16
	movs r3, #24
	bl UiText_DrawCharacterAtOffsetFar
.L_081005d0:
	movs r3, #1
	negs r3, r3
	add r10, r3
	mov r3, r10
	cmp r3, #0
	bge .L_08100554
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
