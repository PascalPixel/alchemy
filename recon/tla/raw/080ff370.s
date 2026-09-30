.syntax unified
	.thumb
	.global Func_080ff370
	.thumb_func
Func_080ff370:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	str r2, [sp, #24]
	str r1, [sp, #28]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	adds r7, r0, #0
	movs r0, #1
	negs r0, r0
	mov r8, r3
	bl Party_SumDjinnCountsFar
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	ldr r0, [sp, #28]
	str r3, [sp, #8]
	bl Owner_GetState
	ldr r2, [sp, #24]
	movs r3, #255
	ands r3, r2
	movs r2, #7
	str r0, [sp, #20]
	str r2, [sp, #12]
	cmp r3, #1
	beq .L_080ff3ba
	movs r3, #10
	str r3, [sp, #12]
.L_080ff3ba:
	movs r3, #184
	lsls r3, r3, #1
	add r3, r8
	ldr r2, [r3]
	movs r3, #1
	strb r3, [r2, #5]
	ldr r1, [sp, #28]
	ldr r2, [sp, #24]
	adds r0, r7, #0
	bl Func_080ff5bc
	add r6, sp, #32
	ldr r2, [sp, #28]
	adds r0, r6, #0
	movs r1, #1
	bl CharacterMenu_BuildAvailability
	ldr r2, [sp, #24]
	movs r5, #128
	lsls r5, r5, #1
	ands r5, r2
	cmp r5, #0
	bne .L_080ff3f6
	movs r3, #96
	adds r0, r7, #0
	movs r1, #0
	movs r2, #40
	str r3, [sp, #0]
	bl UiWindow_ClearInteriorTilesFar
.L_080ff3f6:
	adds r3, r6, #0
	adds r0, r7, #0
	movs r1, #0
	movs r2, #40
	bl Func_081054e0
	movs r3, #135
	lsls r3, r3, #2
	add r3, r8
	ldrh r3, [r3]
	cmp r3, #3
	bne .L_080ff410
	b .L_080ff59c
.L_080ff410:
	cmp r5, #0
	bne .L_080ff42a
	movs r0, #1
	bl WaitFrames
	movs r3, #96
	str r3, [sp, #0]
	adds r0, r7, #0
	movs r1, #64
	movs r2, #56
	movs r3, #224
	bl UiWindow_ClearInteriorTilesFar
.L_080ff42a:
	movs r0, #15
	bl UiWork_SetParamNibbleFar
	ldr r3, [sp, #24]
	cmp r3, #1
	beq .L_080ff43c
	ldr r2, [sp, #8]
	cmp r2, #1
	bne .L_080ff476
.L_080ff43c:
	movs r5, #4
	ldr r3, [sp, #12]
	adds r0, r7, #0
	movs r1, #1
	movs r2, #15
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntryFar
	adds r0, r7, #0
	ldr r3, [sp, #12]
	movs r1, #2
	movs r2, #19
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntryFar
	adds r0, r7, #0
	movs r1, #3
	movs r2, #23
	ldr r3, [sp, #12]
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntryFar
	adds r0, r7, #0
	movs r1, #4
	movs r2, #27
	ldr r3, [sp, #12]
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntryFar
.L_080ff476:
	ldr r3, [sp, #8]
	cmp r3, #0
	beq .L_080ff48e
	ldr r2, [sp, #12]
	ldr r0, .L_080ff5ac
	lsls r6, r2, #3
	adds r3, r6, #0
	adds r3, #8
	adds r1, r7, #0
	movs r2, #64
	bl UiText_DrawCharacterAtOffsetFar
.L_080ff48e:
	ldr r3, [sp, #24]
	cmp r3, #1
	bne .L_080ff4d2
	ldr r2, [sp, #8]
	cmp r2, #0
	bne .L_080ff4a0
	ldr r3, [sp, #12]
	subs r3, #1
	str r3, [sp, #12]
.L_080ff4a0:
	ldr r2, [sp, #12]
	ldr r0, .L_080ff5b0
	lsls r6, r2, #3
	adds r3, r6, #0
	adds r3, #16
	adds r1, r7, #0
	movs r2, #64
	bl UiText_DrawStringAtOffsetFar
	ldr r5, .L_080ff5b4
	adds r3, r6, #0
	adds r0, r5, #0
	adds r3, #24
	adds r1, r7, #0
	movs r2, #64
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #1
	adds r3, r6, #0
	adds r3, #32
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #64
	bl UiText_DrawCharacterAtOffsetFar
.L_080ff4d2:
	ldr r2, [sp, #12]
	movs r3, #0
	lsls r2, r2, #3
	str r3, [sp, #16]
	ldr r3, [sp, #20]
	str r2, [sp, #4]
	adds r2, #8
	mov r11, r2
	movs r2, #104
	adds r3, #72
	mov r9, r2
	ldr r2, [sp, #20]
	mov r8, r3
	movs r3, #120
	mov r10, r3
	adds r3, #160
	adds r5, r2, r3
.L_080ff4f4:
	ldr r2, [sp, #8]
	cmp r2, #0
	beq .L_080ff50a
	mov r3, r11
	ldrb r0, [r5]
	movs r1, #1
	str r3, [sp, #0]
	adds r2, r7, #0
	mov r3, r10
	bl UiText_DrawNumberInWindowFar
.L_080ff50a:
	ldr r2, [sp, #24]
	movs r3, #255
	ands r3, r2
	cmp r3, #1
	bne .L_080ff586
	ldr r3, [sp, #8]
	cmp r3, #0
	beq .L_080ff53c
	mov r2, r11
	ldrb r0, [r5, #4]
	movs r1, #1
	str r2, [sp, #0]
	mov r3, r9
	adds r2, r7, #0
	ldr r6, [sp, #4]
	bl UiText_DrawNumberInWindowFar
	mov r2, r10
	subs r2, #8
	ldr r0, .L_080ff5b8
	adds r1, r7, #0
	mov r3, r11
	bl UiText_DrawStringInWindowFar
	b .L_080ff540
.L_080ff53c:
	ldr r3, [sp, #12]
	lsls r6, r3, #3
.L_080ff540:
	ldr r1, [sp, #16]
	ldr r0, [sp, #28]
	bl Func_080ad1a0
	adds r2, r6, #0
	adds r2, #16
	mov r3, r10
	str r2, [sp, #0]
	subs r3, #8
	adds r2, r7, #0
	movs r1, #2
	bl UiText_DrawNumberInWindowFar
	mov r3, r8
	movs r2, #0
	ldrsh r0, [r3, r2]
	adds r3, r6, #0
	adds r3, #24
	adds r2, r7, #0
	str r3, [sp, #0]
	movs r1, #3
	mov r3, r9
	bl UiText_DrawNumberInWindowFar
	mov r3, r8
	movs r2, #2
	ldrsh r0, [r3, r2]
	adds r3, r6, #0
	adds r3, #32
	str r3, [sp, #0]
	movs r1, #3
	adds r2, r7, #0
	mov r3, r9
	bl UiText_DrawNumberInWindowFar
.L_080ff586:
	movs r2, #4
	add r8, r2
	ldr r2, [sp, #16]
	movs r3, #32
	adds r2, #1
	add r9, r3
	add r10, r3
	adds r5, #1
	str r2, [sp, #16]
	cmp r2, #3
	ble .L_080ff4f4
.L_080ff59c:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ff5ac:
	.4byte 0x0000102c
.L_080ff5b0:
	.4byte Data_08105974
.L_080ff5b4:
	.4byte 0x0000102d
.L_080ff5b8:
	.4byte Data_08105978
