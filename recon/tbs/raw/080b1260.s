.syntax unified
	.thumb
	.global Shop_DrawEquipComparison
	.thumb_func
Shop_DrawEquipComparison:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r1, [sp, #20]
	ldr r3, .L_080b131c
	ldr r3, [r3]
	mov r10, r0
	adds r0, r1, #0
	mov r9, r2
	str r3, [sp, #16]
	bl Func_08077008
	adds r7, r0, #0
	mov r0, r9
	bl Item_Get
	adds r5, r0, #0
	movs r0, #1
	negs r0, r0
	mov r1, r10
	str r0, [sp, #12]
	cmp r1, #0
	bne .L_080b129a
	b .L_080b1450
.L_080b129a:
	mov r0, r10
	bl RenderOutput_PrepareForRedrawFar
	mov r1, r9
	ldr r0, [sp, #20]
	bl Item_CanOwnerEquip
	cmp r0, #0
	bne .L_080b12ba
	ldr r0, .L_080b1320
	mov r1, r10
	movs r2, #8
	movs r3, #24
	bl UiText_DrawResourceFar
	b .L_080b1450
.L_080b12ba:
	ldrb r1, [r5, #2]
	ldr r0, [sp, #20]
	bl Inventory_FindEquippedFar
	ldr r2, [sp, #12]
	cmp r0, r2
	bne .L_080b1324
	movs r3, #216
	movs r1, #128
	ldrh r2, [r7, r3]
	lsls r1, r1, #2
	adds r3, r1, #0
	ands r3, r2
	movs r5, #0
	cmp r3, #0
	beq .L_080b12f2
	mov r12, r1
	adds r1, r7, #0
	adds r1, #216
.L_080b12e0:
	adds r5, #1
	cmp r5, #14
	bgt .L_080b12f2
	adds r1, #2
	ldrh r2, [r1]
	mov r3, r12
	ands r3, r2
	cmp r3, #0
	bne .L_080b12e0
.L_080b12f2:
	cmp r5, #15
	bne .L_080b1318
	adds r6, r7, #0
	movs r5, #0
	adds r6, #216
	b .L_080b1300
.L_080b12fe:
	adds r5, #1
.L_080b1300:
	cmp r5, #14
	bgt .L_080b1312
	ldrh r0, [r6]
	bl Item_Get
	ldrb r3, [r0, #2]
	adds r6, #2
	cmp r3, #6
	bne .L_080b12fe
.L_080b1312:
	cmp r5, #15
	bne .L_080b1318
	movs r5, #0
.L_080b1318:
	lsls r0, r5, #1
	b .L_080b1332
.L_080b131c:
	.4byte Data_03001f2c
.L_080b1320:
	.4byte 0x00000c8e
.L_080b1324:
	lsls r0, r0, #1
	adds r3, r0, #0
	adds r3, #216
	ldrh r3, [r7, r3]
	ldr r1, .L_080b136c
	ands r1, r3
	str r1, [sp, #12]
.L_080b1332:
	ldr r3, .L_080b1368
	adds r5, r0, #0
	mov r0, r9
	orrs r0, r3
	mov r9, r0
	mov r1, r9
	adds r5, #216
	ldrh r2, [r7, r5]
	strh r1, [r7, r5]
	ldr r0, [sp, #20]
	mov r8, r2
	bl Owner_RecalculateStatsFar
	ldrh r3, [r7, #60]
	add r2, sp, #24
	str r3, [r2]
	ldrh r3, [r7, #62]
	adds r6, r7, #0
	str r3, [r2, #4]
	adds r6, #64
	ldrh r3, [r6]
	str r3, [r2, #8]
	adds r3, r7, #0
	adds r3, #66
	str r3, [sp, #8]
	b .L_080b1370
	.2byte 0x0000
.L_080b1368:
	.4byte 0x00000200
.L_080b136c:
	.4byte 0x000001ff
.L_080b1370:
	ldrb r3, [r3]
	mov r0, r8
	str r3, [r2, #12]
	strh r0, [r7, r5]
	ldr r0, [sp, #20]
	mov r11, r2
	bl Owner_RecalculateStatsFar
	ldrh r3, [r7, #60]
	add r1, sp, #40
	str r3, [r1]
	ldrh r3, [r7, #62]
	str r3, [r1, #4]
	ldrh r3, [r6]
	str r3, [r1, #8]
	ldr r2, [sp, #8]
	ldrb r3, [r2]
	str r3, [r1, #12]
	movs r3, #2
	movs r5, #0
	str r3, [sp, #4]
	mov r9, r1
	mov r8, r5
	movs r7, #0
.L_080b13a0:
	mov r0, r8
	mov r1, r9
	ldr r2, [r0, r1]
	mov r1, r11
	ldr r3, [r0, r1]
	cmp r2, r3
	ble .L_080b13b6
	ldr r2, [sp, #16]
	ldr r0, .L_080b1464
	adds r3, r2, r0
	b .L_080b13c2
.L_080b13b6:
	cmp r2, r3
	bge .L_080b13dc
	ldr r1, [sp, #16]
	movs r2, #230
	lsls r2, r2, #2
	adds r3, r1, r2
.L_080b13c2:
	ldrh r0, [r3]
	movs r1, #128
	subs r3, r7, #4
	str r3, [sp, #0]
	lsls r1, r1, #23
	movs r3, #56
	mov r2, r10
	bl Func_080150c8
	movs r3, #0
	adds r6, r7, #0
	strb r3, [r0, #4]
	b .L_080b13de
.L_080b13dc:
	lsls r6, r5, #4
.L_080b13de:
	add r1, sp, #40
	mov r3, r8
	ldr r0, [r3, r1]
	mov r2, r10
	movs r1, #3
	movs r3, #32
	str r7, [sp, #0]
	bl UiNumber_DrawAt
	mov r2, r8
	add r0, sp, #40
	mov r1, r11
	ldr r3, [r2, r0]
	ldr r0, [r2, r1]
	cmp r3, r0
	beq .L_080b140a
	movs r1, #3
	mov r2, r10
	movs r3, #72
	str r6, [sp, #0]
	bl UiNumber_DrawAt
.L_080b140a:
	ldr r0, .L_080b1468
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
	ble .L_080b13a0
	movs r2, #1
	ldr r1, [sp, #12]
	negs r2, r2
	cmp r1, r2
	beq .L_080b1450
	ldr r0, .L_080b146c
	movs r2, #0
	adds r0, r1, r0
	movs r3, #48
	mov r1, r10
	bl UiText_DrawCharacterAtOffsetFar
.L_080b1450:
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080b1464:
	.4byte 0x0000039a
.L_080b1468:
	.4byte 0x00000c98
.L_080b146c:
	.4byte 0x00000182
