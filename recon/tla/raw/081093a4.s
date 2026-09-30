.syntax unified
	.thumb
	.global Func_081093a4
	.thumb_func
Func_081093a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r1, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r10, r0
	adds r0, r1, #0
	mov r9, r2
	str r3, [sp, #16]
	bl Owner_GetState
	adds r7, r0, #0
	mov r0, r9
	bl Item_Get
	adds r5, r0, #0
	movs r0, #1
	negs r0, r0
	mov r1, r10
	str r0, [sp, #12]
	cmp r1, #0
	bne .L_081093e2
	b .L_0810959a
.L_081093e2:
	mov r0, r10
	bl RenderOutput_PrepareForRedrawFar
	mov r1, r9
	ldr r0, [sp, #20]
	bl Item_CanOwnerEquip
	cmp r0, #0
	bne .L_08109402
	ldr r0, .L_08109464
	mov r1, r10
	movs r2, #8
	movs r3, #24
	bl UiText_DrawResourceFar
	b .L_0810959a
.L_08109402:
	ldrb r1, [r5, #2]
	ldr r0, [sp, #20]
	bl Inventory_FindEquippedFar
	ldr r2, [sp, #12]
	cmp r0, r2
	bne .L_08109468
	movs r3, #216
	ldrh r2, [r7, r3]
	movs r1, #128
	lsls r1, r1, #2
	adds r3, r1, #0
	ands r3, r2
	movs r5, #0
	cmp r3, #0
	beq .L_0810943a
	mov r12, r1
	adds r1, r7, #0
	adds r1, #216
.L_08109428:
	adds r5, #1
	cmp r5, #14
	bgt .L_0810943a
	adds r1, #2
	ldrh r2, [r1]
	mov r3, r12
	ands r3, r2
	cmp r3, #0
	bne .L_08109428
.L_0810943a:
	cmp r5, #15
	bne .L_08109460
	adds r6, r7, #0
	movs r5, #0
	adds r6, #216
	b .L_08109448
.L_08109446:
	adds r5, #1
.L_08109448:
	cmp r5, #14
	bgt .L_0810945a
	ldrh r0, [r6]
	bl Item_Get
	ldrb r3, [r0, #2]
	adds r6, #2
	cmp r3, #6
	bne .L_08109446
.L_0810945a:
	cmp r5, #15
	bne .L_08109460
	movs r5, #0
.L_08109460:
	lsls r0, r5, #1
	b .L_0810947a
.L_08109464:
	.4byte 0x0000123b
.L_08109468:
	lsls r0, r0, #1
	adds r3, r0, #0
	adds r3, #216
	ldrh r3, [r7, r3]
	movs r1, #128
	lsls r1, r1, #1
	adds r1, #255
	ands r1, r3
	str r1, [sp, #12]
.L_0810947a:
	ldr r3, .L_081094b0
	adds r5, r0, #0
	mov r0, r9
	orrs r0, r3
	adds r5, #216
	mov r9, r0
	ldrh r2, [r7, r5]
	mov r1, r9
	strh r1, [r7, r5]
	ldr r0, [sp, #20]
	mov r8, r2
	bl Owner_RecalculateStatsFar
	ldrh r3, [r7, #60]
	add r2, sp, #24
	str r3, [r2]
	adds r6, r7, #0
	ldrh r3, [r7, #62]
	adds r6, #64
	str r3, [r2, #4]
	mov r0, r8
	ldrh r3, [r6]
	mov r11, r2
	str r3, [r2, #8]
	adds r3, r7, #0
	b .L_081094b4
	.2byte 0x0000
.L_081094b0:
	.4byte 0x00000200
.L_081094b4:
	adds r3, #66
	str r3, [sp, #8]
	ldrb r3, [r3]
	str r3, [r2, #12]
	strh r0, [r7, r5]
	ldr r0, [sp, #20]
	bl Owner_RecalculateStatsFar
	ldrh r3, [r7, #60]
	add r1, sp, #40
	str r3, [r1]
	movs r5, #0
	ldrh r3, [r7, #62]
	mov r9, r1
	str r3, [r1, #4]
	mov r8, r5
	ldrh r3, [r6]
	movs r7, #0
	str r3, [r1, #8]
	ldr r2, [sp, #8]
	ldrb r3, [r2]
	str r3, [r1, #12]
	movs r3, #2
	str r3, [sp, #4]
.L_081094e4:
	mov r0, r8
	mov r1, r9
	ldr r2, [r0, r1]
	mov r1, r11
	ldr r3, [r0, r1]
	cmp r2, r3
	ble .L_081094fe
	ldr r2, [sp, #16]
	movs r0, #128
	lsls r0, r0, #3
	adds r0, #246
	adds r3, r2, r0
	b .L_0810950c
.L_081094fe:
	cmp r2, r3
	bge .L_08109526
	ldr r1, [sp, #16]
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #244
	adds r3, r1, r2
.L_0810950c:
	ldrh r0, [r3]
	movs r1, #128
	subs r3, r7, #4
	str r3, [sp, #0]
	lsls r1, r1, #23
	movs r3, #56
	mov r2, r10
	bl RenderOutput_CreateFar
	movs r3, #0
	adds r6, r7, #0
	strb r3, [r0, #4]
	b .L_08109528
.L_08109526:
	lsls r6, r5, #4
.L_08109528:
	add r1, sp, #40
	mov r3, r8
	ldr r0, [r3, r1]
	mov r2, r10
	movs r1, #3
	movs r3, #32
	str r7, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	mov r2, r8
	add r0, sp, #40
	mov r1, r11
	ldr r3, [r2, r0]
	ldr r0, [r2, r1]
	cmp r3, r0
	beq .L_08109554
	movs r1, #3
	mov r2, r10
	movs r3, #72
	str r6, [sp, #0]
	bl UiText_DrawNumberInWindowFar
.L_08109554:
	ldr r0, .L_081095a8
	mov r1, r10
	adds r0, r5, r0
	movs r2, #0
	adds r3, r6, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r2, [sp, #4]
	mov r0, r10
	movs r3, #13
	movs r1, #0
	str r2, [sp, #0]
	bl UiWindow_DrawDividerLineFar
	ldr r3, [sp, #4]
	movs r0, #4
	adds r3, #2
	adds r5, #1
	str r3, [sp, #4]
	add r8, r0
	adds r7, #16
	cmp r5, #2
	ble .L_081094e4
	ldr r1, [sp, #12]
	movs r2, #1
	negs r2, r2
	cmp r1, r2
	beq .L_0810959a
	ldr r0, .L_081095ac
	movs r2, #0
	adds r0, r1, r0
	movs r3, #48
	mov r1, r10
	bl UiText_DrawCharacterAtOffsetFar
.L_0810959a:
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081095a8:
	.4byte 0x00001245
.L_081095ac:
	.4byte 0x0000025f
