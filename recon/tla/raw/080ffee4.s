.syntax unified
	.thumb
	.global ItemMenu_DrawEquipPage
	.thumb_func
ItemMenu_DrawEquipPage:
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
	mov r10, r2
	ldr r3, [r3]
	ldr r2, [r2, #8]
	mov r8, r3
	lsls r3, r2, #2
	mov r1, r10
	adds r3, r3, r2
	mov r9, r3
	ldr r3, [r1, #16]
	mov r2, r8
	add r3, r9
	str r3, [r1, #24]
	adds r6, r0, #0
	ldr r0, [r2, #48]
	sub sp, #8
	bl RenderOutput_RedrawSavedRectFar
	movs r0, #1
	bl WaitFrames
	mov r1, r10
	ldr r3, [r1, #24]
	movs r2, #226
	lsls r2, r2, #1
	lsls r3, r3, #1
	adds r3, r3, r2
	mov r1, r8
	ldrh r2, [r1, r3]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080fff4c
	movs r0, #128
	ldr r3, .L_081000dc
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r2
	adds r0, r0, r3
	ldr r1, [r1, #48]
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_080fff4c:
	movs r2, #1
	mov r1, r9
	mov r11, r2
	lsls r3, r1, #1
	movs r2, #226
	add r3, r8
	lsls r2, r2, #1
	adds r2, r2, r3
	movs r7, #0
	movs r5, #1
	mov r9, r2
.L_080fff62:
	mov r1, r10
	ldr r3, [r1, #16]
	cmp r7, r3
	bne .L_080fffb4
	ldr r3, [r1, #24]
	movs r2, #226
	lsls r2, r2, #1
	lsls r3, r3, #1
	adds r3, r3, r2
	mov r1, r8
	ldrh r3, [r1, r3]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	bl Item_Get
	ldr r0, [r0, #20]
	cmp r0, #4
	beq .L_080fffa0
	movs r3, #0
	adds r1, r0, #1
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r2, #27
	adds r3, r5, #0
	bl UiWindow_SetTilemapEntryFar
	mov r2, r11
	movs r3, #14
	b .L_080fffde
.L_080fffa0:
	mov r3, r11
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r3, #14
	movs r1, #14
	adds r2, r5, #0
	str r3, [sp, #4]
	bl Func_080f9224
	b .L_08100004
.L_080fffb4:
	mov r1, r9
	ldrh r3, [r1]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	bl Item_Get
	ldr r0, [r0, #20]
	cmp r0, #4
	beq .L_080ffff0
	movs r3, #4
	adds r1, r0, #1
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r2, #27
	adds r3, r5, #0
	bl UiWindow_SetTilemapEntryFar
	mov r2, r11
	movs r3, #15
.L_080fffde:
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r6, #0
	movs r1, #14
	adds r2, r5, #0
	movs r3, #13
	bl Func_080f9224
	b .L_08100004
.L_080ffff0:
	mov r3, r11
	str r3, [sp, #0]
	movs r3, #15
	str r3, [sp, #4]
	adds r0, r6, #0
	movs r1, #14
	adds r2, r5, #0
	movs r3, #14
	bl Func_080f9224
.L_08100004:
	movs r1, #2
	adds r7, #1
	adds r5, #2
	add r9, r1
	cmp r7, #4
	ble .L_080fff62
	movs r2, #1
	mov r9, r2
	movs r5, #1
	movs r6, #15
	movs r7, #3
.L_0810001a:
	mov r3, r8
	mov r1, r9
	ldr r0, [r3, #52]
	adds r2, r5, #0
	str r1, [sp, #0]
	movs r3, #9
	movs r1, #2
	subs r7, #1
	str r6, [sp, #4]
	adds r5, #2
	bl Func_080f9224
	cmp r7, #0
	bge .L_0810001a
	mov r2, r10
	ldr r3, [r2, #24]
	movs r1, #226
	lsls r1, r1, #1
	lsls r3, r3, #1
	adds r3, r3, r1
	mov r1, r8
	ldrh r2, [r1, r3]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_081000c6
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r2
	bl Item_Get
	ldrb r1, [r0, #2]
	cmp r1, #2
	beq .L_081000b0
	cmp r1, #2
	bgt .L_0810006c
	cmp r1, #1
	beq .L_08100076
	b .L_081000c6
.L_0810006c:
	cmp r1, #3
	beq .L_08100098
	cmp r1, #4
	beq .L_08100086
	b .L_081000c6
.L_08100076:
	mov r2, r8
	movs r3, #14
	ldr r0, [r2, #52]
	str r1, [sp, #0]
	str r3, [sp, #4]
	movs r1, #2
	movs r2, #1
	b .L_081000a8
.L_08100086:
	mov r3, r8
	ldr r0, [r3, #52]
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #14
	str r3, [sp, #4]
	movs r1, #2
	movs r2, #3
	b .L_081000a8
.L_08100098:
	mov r1, r8
	movs r3, #1
	ldr r0, [r1, #52]
	str r3, [sp, #0]
	movs r3, #14
	str r3, [sp, #4]
	movs r1, #2
	movs r2, #5
.L_081000a8:
	movs r3, #9
	bl Func_080f9224
	b .L_081000c6
.L_081000b0:
	mov r2, r8
	movs r3, #1
	ldr r0, [r2, #52]
	str r3, [sp, #0]
	movs r3, #14
	str r3, [sp, #4]
	movs r1, #2
	movs r2, #7
	movs r3, #9
	bl Func_080f9224
.L_081000c6:
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081000dc:
	.4byte 0x00000092
