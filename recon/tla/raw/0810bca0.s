.syntax unified
	.thumb
	.global Func_0810bca0
	.thumb_func
Func_0810bca0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	adds r6, r1, #0
	adds r7, r0, #0
	adds r0, r6, #0
	mov r8, r2
	sub sp, #8
	mov r11, r3
	bl Owner_GetState
	mov r1, r8
	lsls r1, r1, #1
	mov r9, r1
	mov r2, r9
	adds r2, #216
	ldrh r3, [r0, r2]
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	ands r5, r3
	ldrh r3, [r0, r2]
	mov r10, r0
	lsrs r3, r3, #11
	adds r3, #1
	str r3, [sp, #4]
	cmp r7, #0
	beq .L_0810bd92
	adds r0, r7, #0
	bl RenderOutput_RedrawSavedRectFar
	ldr r0, .L_0810bda0
	movs r3, #0
	adds r0, r5, r0
	adds r1, r7, #0
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #11
	add r3, r11
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0810bd18
	adds r0, r6, #0
	mov r1, r8
	bl Func_080ad268
	b .L_0810bd20
.L_0810bd18:
	adds r0, r6, #0
	mov r1, r8
	bl Func_080ad270
.L_0810bd20:
	movs r3, #2
	ands r3, r0
	cmp r3, #0
	beq .L_0810bd2c
	ldr r0, .L_0810bda4
	b .L_0810bd36
.L_0810bd2c:
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_0810bd42
	ldr r0, .L_0810bda8
.L_0810bd36:
	adds r1, r7, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	b .L_0810bd92
.L_0810bd42:
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #11
	add r3, r11
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0810bd92
	mov r3, r9
	adds r3, #216
	mov r2, r10
	ldrh r0, [r2, r3]
	bl Shop_GetSellPrice
	ldr r5, .L_0810bdac
	ldr r3, [sp, #4]
	adds r1, r7, #0
	adds r6, r3, #0
	muls r6, r0
	movs r2, #8
	adds r0, r5, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #8
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #5
	adds r2, r7, #0
	movs r3, #40
	subs r5, #5
	bl UiText_DrawNumberInWindowFar
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #80
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
.L_0810bd92:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0810bda0:
	.4byte 0x0000025f
.L_0810bda4:
	.4byte 0x00001241
.L_0810bda8:
	.4byte 0x00001242
.L_0810bdac:
	.4byte 0x0000123a
