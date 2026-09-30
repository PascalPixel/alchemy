.syntax unified
	.thumb
	.global Func_08103218
	.thumb_func
Func_08103218:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #168
	mov r9, r0
	adds r0, r3, #0
	ldr r5, [sp, #200]
	ldr r6, [sp, #204]
	str r1, [sp, #60]
	str r2, [sp, #56]
	str r3, [sp, #52]
	bl Owner_GetState
	movs r3, #192
	str r0, [sp, #48]
	lsls r3, r3, #18
	adds r3, #220
	ldr r4, [r3]
	movs r1, #150
	lsls r1, r1, #2
	adds r0, r4, #2
	adds r3, r5, r1
	ldrb r2, [r0, r3]
	lsls r5, r5, #1
	str r2, [sp, #40]
	mov lr, r0
	ldrb r3, [r4, r3]
	movs r2, #128
	str r3, [sp, #36]
	movs r3, #182
	lsls r3, r3, #1
	mov r12, r3
	add r5, r12
	ldrh r0, [r4, r5]
	lsls r2, r2, #8
	adds r3, r2, #0
	ands r3, r0
	lsls r3, r3, #16
	lsrs r3, r3, #16
	str r3, [sp, #32]
	adds r1, r6, r1
	mov r0, lr
	ldrb r0, [r0, r1]
	lsls r6, r6, #1
	str r0, [sp, #28]
	add r6, r12
	ldrb r1, [r4, r1]
	movs r5, #166
	str r1, [sp, #24]
	lsls r5, r5, #1
	ldrh r3, [r4, r6]
	adds r0, r5, #0
	ands r2, r3
	lsls r2, r2, #16
	lsrs r2, r2, #16
	str r2, [sp, #20]
	bl Runtime_BumpAllocate
	ldr r3, .L_0810362c
	ldr r1, [sp, #48]
	str r0, [sp, #44]
	adds r2, r5, #0
	mov lr, r3
	.2byte 0xf800
	ldr r1, [sp, #212]
	cmp r1, #0
	beq .L_081032a8
	b .L_081033ca
.L_081032a8:
	ldr r2, [sp, #208]
	cmp r2, #3
	bne .L_08103336
	ldr r1, [sp, #48]
	ldr r2, [sp, #60]
	movs r3, #52
	ldrsh r0, [r1, r3]
	ldr r3, [sp, #56]
	lsls r7, r2, #3
	lsls r3, r3, #3
	mov r11, r3
	adds r5, r7, #0
	movs r1, #56
	adds r5, #72
	add r1, r11
	str r1, [sp, #0]
	mov r10, r1
	mov r2, r9
	adds r3, r5, #0
	movs r1, #4
	bl UiText_DrawNumberInWindowFar
	ldr r3, [sp, #48]
	movs r1, #64
	add r1, r11
	movs r2, #54
	ldrsh r0, [r3, r2]
	mov r8, r1
	str r1, [sp, #0]
	mov r2, r9
	adds r3, r5, #0
	movs r1, #4
	bl UiText_DrawNumberInWindowFar
	ldr r3, [sp, #48]
	subs r5, #40
	mov r1, r10
	movs r2, #56
	ldrsh r0, [r3, r2]
	str r1, [sp, #0]
	mov r2, r9
	adds r3, r5, #0
	movs r1, #4
	bl UiText_DrawNumberInWindowFar
	ldr r3, [sp, #48]
	mov r1, r8
	movs r2, #58
	ldrsh r0, [r3, r2]
	str r1, [sp, #0]
	adds r3, r5, #0
	movs r1, #4
	mov r2, r9
	bl UiText_DrawNumberInWindowFar
	ldr r5, .L_08103630
	adds r6, r7, #0
	adds r6, #64
	adds r0, r5, #0
	mov r1, r9
	adds r2, r6, #0
	mov r3, r10
	bl UiText_DrawStringAtOffsetFar
	adds r0, r5, #0
	mov r1, r9
	adds r2, r6, #0
	mov r3, r8
	bl UiText_DrawStringAtOffsetFar
	b .L_08103370
.L_08103336:
	ldr r3, [sp, #48]
	ldr r1, [sp, #60]
	movs r2, #56
	ldrsh r0, [r3, r2]
	ldr r2, [sp, #56]
	lsls r7, r1, #3
	lsls r2, r2, #3
	mov r11, r2
	adds r5, r7, #0
	mov r3, r11
	adds r5, #32
	adds r3, #56
	str r3, [sp, #0]
	movs r1, #4
	adds r3, r5, #0
	mov r2, r9
	bl UiText_DrawNumberInWindowFar
	ldr r1, [sp, #48]
	mov r2, r9
	movs r3, #58
	ldrsh r0, [r1, r3]
	mov r3, r11
	adds r3, #64
	str r3, [sp, #0]
	movs r1, #4
	adds r3, r5, #0
	bl UiText_DrawNumberInWindowFar
.L_08103370:
	ldr r2, [sp, #48]
	adds r5, r7, #0
	mov r3, r11
	adds r5, #48
	adds r3, #72
	ldrh r0, [r2, #60]
	movs r1, #3
	str r3, [sp, #0]
	mov r2, r9
	adds r3, r5, #0
	bl UiText_DrawNumberInWindowFar
	ldr r3, [sp, #48]
	movs r1, #3
	ldrh r0, [r3, #62]
	mov r3, r11
	adds r3, #80
	str r3, [sp, #0]
	mov r2, r9
	adds r3, r5, #0
	bl UiText_DrawNumberInWindowFar
	ldr r3, [sp, #48]
	movs r1, #3
	adds r3, #64
	ldrh r0, [r3]
	mov r3, r11
	adds r3, #88
	str r3, [sp, #0]
	mov r2, r9
	adds r3, r5, #0
	bl UiText_DrawNumberInWindowFar
	ldr r3, [sp, #48]
	mov r2, r11
	adds r3, #66
	ldrb r0, [r3]
	adds r2, #96
	adds r3, r7, #0
	str r2, [sp, #0]
	adds r3, #56
	movs r1, #2
	mov r2, r9
	bl UiText_DrawNumberInWindowFar
.L_081033ca:
	ldr r0, [sp, #208]
	cmp r0, #1
	beq .L_08103402
	cmp r0, #1
	bgt .L_081033da
	cmp r0, #0
	beq .L_081033e6
	b .L_0810346e
.L_081033da:
	ldr r1, [sp, #208]
	cmp r1, #2
	beq .L_08103416
	cmp r1, #4
	beq .L_0810344e
	b .L_0810346e
.L_081033e6:
	ldr r2, [sp, #24]
	movs r5, #31
	ands r5, r2
	ldr r1, [sp, #28]
	adds r2, r5, #0
	ldr r0, [sp, #52]
	bl Djinn_AddToOwnerFar
	adds r2, r5, #0
	ldr r0, [sp, #52]
	ldr r1, [sp, #28]
	bl Djinn_ActivateFar
	b .L_0810346e
.L_08103402:
	ldr r0, [sp, #36]
	movs r3, #31
	ands r0, r3
	str r0, [sp, #36]
	ldr r1, [sp, #40]
	ldr r0, [sp, #52]
	ldr r2, [sp, #36]
	bl Djinn_ActivateFar + 0x8
	b .L_0810346e
.L_08103416:
	ldr r1, [sp, #32]
	cmp r1, #0
	beq .L_0810342c
	ldr r2, [sp, #36]
	movs r3, #31
	ands r2, r3
	ldr r0, [sp, #52]
	ldr r1, [sp, #40]
	str r2, [sp, #36]
	bl Djinn_ActivateFar + 0x8
.L_0810342c:
	ldr r3, [sp, #24]
	movs r5, #31
	ands r5, r3
	ldr r0, [sp, #52]
	ldr r1, [sp, #28]
	adds r2, r5, #0
	bl Djinn_AddToOwnerFar
	ldr r0, [sp, #20]
	cmp r0, #0
	beq .L_0810346e
	ldr r0, [sp, #52]
	ldr r1, [sp, #28]
	adds r2, r5, #0
	bl Djinn_ActivateFar
	b .L_0810346e
.L_0810344e:
	ldr r1, [sp, #24]
	movs r5, #31
	ands r5, r1
	adds r2, r5, #0
	ldr r0, [sp, #52]
	ldr r1, [sp, #28]
	bl Djinn_AddToOwnerFar
	ldr r2, [sp, #20]
	cmp r2, #0
	beq .L_0810346e
	ldr r0, [sp, #52]
	ldr r1, [sp, #28]
	adds r2, r5, #0
	bl Djinn_ActivateFar
.L_0810346e:
	ldr r0, [sp, #52]
	bl BattleUnit_Recalculate
	ldr r0, [sp, #52]
	bl Owner_GetState
	ldr r3, [sp, #212]
	str r0, [sp, #48]
	cmp r3, #0
	bne .L_08103530
	ldr r0, [sp, #60]
	ldr r1, [sp, #56]
	lsls r7, r0, #3
	lsls r1, r1, #3
	mov r8, r1
	adds r6, r7, #0
	adds r6, #40
	mov r5, r8
	ldr r0, [sp, #48]
	adds r5, #16
	mov r1, r9
	adds r2, r6, #0
	mov r3, r8
	bl UiText_DrawStringAtOffsetFar
	adds r3, r5, #0
	ldr r0, .L_08103634
	mov r1, r9
	adds r2, r6, #0
	bl UiText_DrawStringAtOffsetFar
	ldr r2, [sp, #48]
	adds r3, r7, #0
	ldrb r0, [r2, #15]
	adds r3, #88
	movs r1, #2
	mov r2, r9
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r5, .L_08103638
	mov r3, r8
	adds r0, r5, #0
	adds r3, #56
	mov r1, r9
	adds r2, r7, #0
	bl UiText_DrawCharacterAtOffsetFar
	mov r3, r8
	adds r0, r5, #1
	adds r3, #64
	mov r1, r9
	adds r2, r7, #0
	bl UiText_DrawCharacterAtOffsetFar
	mov r3, r8
	adds r0, r5, #2
	adds r3, #72
	mov r1, r9
	adds r2, r7, #0
	bl UiText_DrawCharacterAtOffsetFar
	mov r3, r8
	adds r0, r5, #3
	adds r3, #80
	mov r1, r9
	adds r2, r7, #0
	bl UiText_DrawCharacterAtOffsetFar
	mov r3, r8
	adds r0, r5, #4
	adds r3, #88
	mov r1, r9
	adds r2, r7, #0
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #5
	mov r3, r8
	adds r3, #96
	adds r0, r5, #0
	mov r1, r9
	adds r2, r7, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, [sp, #44]
	movs r1, #42
	adds r1, #255
	adds r3, r0, r1
	ldrb r0, [r3]
	ldr r3, .L_0810363c
	mov r1, r9
	adds r0, r0, r3
	mov r3, r8
	adds r3, #32
	adds r2, r7, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_08103530:
	ldr r2, [sp, #212]
	cmp r2, #0
	beq .L_08103538
	b .L_081037f6
.L_08103538:
	ldr r0, [sp, #44]
	movs r3, #42
	ldr r1, [sp, #48]
	adds r3, #255
	adds r6, r0, r3
	adds r5, r1, r3
	ldrb r2, [r6]
	ldrb r3, [r5]
	mov r12, r2
	cmp r12, r3
	beq .L_08103580
	ldr r2, [sp, #60]
	ldr r1, [sp, #56]
	ldr r3, .L_0810363c
	ldrb r0, [r5]
	lsls r7, r2, #3
	adds r0, r0, r3
	lsls r3, r1, #3
	adds r3, #48
	mov r1, r9
	adds r2, r7, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r3, [sp, #212]
	ldr r2, [sp, #60]
	movs r1, #242
	lsls r1, r1, #8
	adds r2, #2
	str r3, [sp, #0]
	adds r1, #150
	movs r3, #5
	mov r0, r9
	bl UiWindow_SetTilemapEntryFar
	ldrb r2, [r6]
	ldrb r3, [r5]
.L_08103580:
	ldr r0, [sp, #60]
	mov r12, r2
	mov r8, r0
	cmp r12, r3
	beq .L_0810358e
	adds r0, #5
	mov r8, r0
.L_0810358e:
	ldr r7, [sp, #56]
	movs r6, #0
	adds r7, #5
	mov r10, r6
.L_08103596:
	asrs r6, r6, #24
	movs r2, #160
	lsls r5, r6, #1
	lsls r2, r2, #7
	adds r2, #1
	add r5, r8
	mov r3, r10
	adds r1, r6, r2
	str r3, [sp, #0]
	adds r2, r5, #0
	mov r0, r9
	adds r3, r7, #0
	bl UiWindow_SetTilemapEntryFar
	movs r0, #142
	ldr r2, [sp, #48]
	lsls r0, r0, #1
	adds r3, r6, r0
	ldrb r1, [r2, r3]
	movs r3, #240
	lsls r3, r3, #8
	adds r3, #48
	mov r0, r10
	adds r5, #1
	adds r1, r1, r3
	str r0, [sp, #0]
	adds r2, r5, #0
	mov r0, r9
	adds r3, r7, #0
	bl UiWindow_SetTilemapEntryFar
	adds r6, #1
	movs r1, #192
	lsls r6, r6, #24
	lsls r1, r1, #18
	cmp r6, r1
	ble .L_08103596
	ldr r3, [sp, #48]
	movs r2, #56
	ldrsh r0, [r3, r2]
	ldr r2, [sp, #44]
	movs r1, #56
	ldrsh r3, [r2, r1]
	cmp r0, r3
	beq .L_0810364e
	ldr r3, [sp, #60]
	ldr r1, [sp, #56]
	lsls r7, r3, #3
	lsls r2, r1, #3
	adds r5, r2, #0
	adds r3, r7, #0
	adds r3, #72
	movs r1, #4
	mov r2, r9
	adds r5, #56
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r0, [sp, #48]
	movs r3, #56
	ldrsh r2, [r0, r3]
	ldr r0, [sp, #44]
	movs r1, #56
	ldrsh r3, [r0, r1]
	cmp r2, r3
	ble .L_08103640
	adds r1, r7, #0
	adds r1, #62
	mov r0, r9
	adds r2, r5, #0
	movs r3, #0
	bl Func_08104ba8
	b .L_0810364e
	.2byte 0x0000
.L_0810362c:
	.4byte IwramCopyWords
.L_08103630:
	.4byte Data_081059d8
.L_08103634:
	.4byte Data_081059d4
.L_08103638:
	.4byte 0x00000d0e
.L_0810363c:
	.4byte 0x00000b63
.L_08103640:
	adds r1, r7, #0
	adds r1, #62
	mov r0, r9
	adds r2, r5, #0
	movs r3, #1
	bl Func_08104ba8
.L_0810364e:
	ldr r2, [sp, #48]
	movs r1, #58
	ldrsh r0, [r2, r1]
	ldr r2, [sp, #44]
	movs r1, #58
	ldrsh r3, [r2, r1]
	cmp r0, r3
	beq .L_081036a6
	ldr r3, [sp, #60]
	ldr r1, [sp, #56]
	lsls r7, r3, #3
	lsls r2, r1, #3
	adds r5, r2, #0
	adds r3, r7, #0
	adds r3, #72
	movs r1, #4
	mov r2, r9
	adds r5, #64
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r0, [sp, #48]
	movs r3, #58
	ldrsh r2, [r0, r3]
	ldr r0, [sp, #44]
	movs r1, #58
	ldrsh r3, [r0, r1]
	cmp r2, r3
	ble .L_08103698
	adds r1, r7, #0
	adds r1, #62
	mov r0, r9
	adds r2, r5, #0
	movs r3, #0
	bl Func_08104ba8
	b .L_081036a6
.L_08103698:
	adds r1, r7, #0
	adds r1, #62
	mov r0, r9
	adds r2, r5, #0
	movs r3, #1
	bl Func_08104ba8
.L_081036a6:
	ldr r1, [sp, #48]
	ldr r0, [sp, #44]
	ldrh r2, [r1, #60]
	ldrh r3, [r0, #60]
	cmp r2, r3
	beq .L_081036f8
	ldr r1, [sp, #60]
	adds r0, r2, #0
	lsls r7, r1, #3
	ldr r1, [sp, #56]
	adds r3, r7, #0
	lsls r2, r1, #3
	adds r5, r2, #0
	adds r3, #72
	mov r2, r9
	adds r5, #72
	movs r1, #4
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r3, [sp, #48]
	ldr r0, [sp, #44]
	ldrh r2, [r3, #60]
	ldrh r3, [r0, #60]
	cmp r2, r3
	bls .L_081036ea
	adds r1, r7, #0
	adds r1, #70
	mov r0, r9
	adds r2, r5, #0
	movs r3, #0
	bl Func_08104ba8
	b .L_081036f8
.L_081036ea:
	adds r1, r7, #0
	adds r1, #70
	mov r0, r9
	adds r2, r5, #0
	movs r3, #1
	bl Func_08104ba8
.L_081036f8:
	ldr r1, [sp, #48]
	ldr r0, [sp, #44]
	ldrh r2, [r1, #62]
	ldrh r3, [r0, #62]
	cmp r2, r3
	beq .L_0810374a
	ldr r1, [sp, #60]
	adds r0, r2, #0
	lsls r7, r1, #3
	ldr r1, [sp, #56]
	adds r3, r7, #0
	lsls r2, r1, #3
	adds r5, r2, #0
	adds r3, #72
	mov r2, r9
	adds r5, #80
	movs r1, #4
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r3, [sp, #48]
	ldr r0, [sp, #44]
	ldrh r2, [r3, #62]
	ldrh r3, [r0, #62]
	cmp r2, r3
	bls .L_0810373c
	adds r1, r7, #0
	adds r1, #70
	mov r0, r9
	adds r2, r5, #0
	movs r3, #0
	bl Func_08104ba8
	b .L_0810374a
.L_0810373c:
	adds r1, r7, #0
	adds r1, #70
	mov r0, r9
	adds r2, r5, #0
	movs r3, #1
	bl Func_08104ba8
.L_0810374a:
	ldr r5, [sp, #48]
	ldr r1, [sp, #44]
	adds r5, #64
	adds r1, #64
	ldrh r2, [r5]
	ldrh r3, [r1]
	mov r8, r1
	cmp r2, r3
	beq .L_081037a0
	adds r0, r2, #0
	ldr r1, [sp, #56]
	ldr r2, [sp, #60]
	lsls r7, r2, #3
	lsls r2, r1, #3
	adds r6, r2, #0
	adds r3, r7, #0
	adds r3, #72
	mov r2, r9
	adds r6, #88
	movs r1, #4
	str r6, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	mov r0, r8
	ldrh r2, [r5]
	ldrh r3, [r0]
	cmp r2, r3
	bls .L_08103792
	adds r1, r7, #0
	adds r1, #70
	mov r0, r9
	adds r2, r6, #0
	movs r3, #0
	bl Func_08104ba8
	b .L_081037a0
.L_08103792:
	adds r1, r7, #0
	adds r1, #70
	mov r0, r9
	adds r2, r6, #0
	movs r3, #1
	bl Func_08104ba8
.L_081037a0:
	ldr r5, [sp, #48]
	ldr r1, [sp, #44]
	adds r5, #66
	adds r1, #66
	ldrb r2, [r5]
	ldrb r3, [r1]
	mov r8, r1
	cmp r2, r3
	beq .L_081037f6
	adds r0, r2, #0
	ldr r1, [sp, #56]
	ldr r2, [sp, #60]
	lsls r7, r2, #3
	lsls r2, r1, #3
	adds r6, r2, #0
	adds r3, r7, #0
	adds r3, #88
	mov r2, r9
	adds r6, #96
	movs r1, #2
	str r6, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	mov r0, r8
	ldrb r2, [r5]
	ldrb r3, [r0]
	cmp r2, r3
	bls .L_081037e8
	adds r1, r7, #0
	adds r1, #70
	mov r0, r9
	adds r2, r6, #0
	movs r3, #0
	bl Func_08104ba8
	b .L_081037f6
.L_081037e8:
	adds r1, r7, #0
	adds r1, #70
	mov r0, r9
	adds r2, r6, #0
	movs r3, #1
	bl Func_08104ba8
.L_081037f6:
	ldr r1, [sp, #212]
	cmp r1, #0
	bgt .L_081037fe
	b .L_081039a8
.L_081037fe:
	ldr r3, [sp, #208]
	movs r2, #3
	eors r2, r3
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	mov r11, r3
	mov r0, r11
	movs r3, #6
	subs r0, r3, r0
	adds r3, r1, #0
	mov r11, r0
	subs r3, #1
	mov r1, r11
	muls r1, r3
	ldr r0, [sp, #44]
	mov r10, r1
	ldr r1, [sp, #48]
	add r2, sp, #64
	add r5, sp, #72
	adds r1, #88
	add r3, sp, #68
	str r2, [sp, #0]
	adds r0, #88
	adds r2, r5, #0
	bl Func_08101860
	lsls r0, r0, #24
	str r0, [sp, #12]
	asrs r3, r0, #24
	ldr r0, [sp, #60]
	movs r2, #0
	movs r1, #0
	lsls r7, r0, #3
	cmp r10, r3
	bge .L_0810391a
	cmp r1, r11
	bge .L_08103916
	str r1, [sp, #8]
	mov r1, r10
	lsls r3, r1, #1
	str r7, [sp, #16]
	adds r3, r3, r5
	mov r8, r3
.L_08103856:
	ldr r3, [sp, #56]
	lsls r6, r2, #24
	mov r0, r8
	asrs r2, r6, #23
	adds r2, r3, r2
	movs r1, #252
	ldrh r3, [r0]
	lsls r1, r1, #6
	adds r1, #255
	lsls r2, r2, #3
	ands r3, r1
	adds r2, #4
	mov r0, r9
	adds r1, r7, #0
	bl Func_08104b18
	mov r3, r8
	ldrh r2, [r3]
	ldr r3, .L_0810389c
	ands r3, r2
	cmp r3, #0
	beq .L_0810388a
	movs r0, #4
	bl UiText_DrawNumberInWindowFar + 0x8
	b .L_081038aa
.L_0810388a:
	ldr r3, .L_081038a0
	ands r3, r2
	cmp r3, #0
	beq .L_081038a4
	movs r0, #2
	bl UiText_DrawNumberInWindowFar + 0x8
	b .L_081038aa
	.2byte 0x0000
.L_0810389c:
	.4byte 0x00008000
.L_081038a0:
	.4byte 0x00004000
.L_081038a4:
	movs r0, #15
	bl UiText_DrawNumberInWindowFar + 0x8
.L_081038aa:
	mov r0, r8
	ldr r1, [sp, #56]
	ldrh r3, [r0]
	movs r0, #252
	asrs r6, r6, #24
	lsls r0, r0, #6
	lsls r5, r6, #1
	adds r0, #255
	ldr r2, [sp, #16]
	ands r0, r3
	adds r5, r1, r5
	ldr r3, .L_081039e4
	lsls r5, r5, #3
	adds r5, #8
	adds r0, r0, r3
	mov r1, r9
	adds r3, r5, #0
	adds r2, #16
	bl UiText_DrawCharacterAtOffsetFar
	mov r2, r8
	ldrh r0, [r2]
	bl BattleAction_Get
	ldr r3, [sp, #16]
	ldrb r0, [r0, #9]
	movs r1, #2
	mov r2, r9
	adds r3, #88
	str r5, [sp, #0]
	bl UiText_DrawNumberAtOffsetFar
	ldr r0, [sp, #8]
	movs r1, #128
	lsls r1, r1, #17
	adds r3, r0, r1
	movs r0, #1
	add r10, r0
	ldr r0, [sp, #12]
	adds r6, #1
	lsrs r1, r3, #24
	movs r3, #2
	lsls r6, r6, #24
	add r8, r3
	asrs r3, r0, #24
	lsrs r2, r6, #24
	cmp r10, r3
	bge .L_0810391a
	lsls r3, r1, #24
	str r3, [sp, #8]
	asrs r3, r3, #24
	cmp r3, r11
	blt .L_08103856
	b .L_0810391a
.L_08103916:
	ldr r1, [sp, #60]
	lsls r7, r1, #3
.L_0810391a:
	movs r0, #15
	bl UiText_DrawNumberInWindowFar + 0x8
	ldr r3, [sp, #56]
	adds r2, r7, #0
	lsls r6, r3, #3
	ldr r0, .L_081039e8
	adds r2, #88
	mov r1, r9
	adds r3, r6, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, [sp, #208]
	cmp r0, #3
	beq .L_0810399e
	ldr r3, [sp, #68]
	movs r5, #0
	cmp r3, #0
	beq .L_08103956
	movs r0, #4
	bl UiText_DrawNumberInWindowFar + 0x8
	adds r3, r6, #0
	ldr r0, .L_081039ec
	adds r3, #88
	mov r1, r9
	adds r2, r7, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r5, #1
.L_08103956:
	ldr r3, [sp, #64]
	cmp r3, #0
	beq .L_08103976
	movs r0, #2
	bl UiText_DrawNumberInWindowFar + 0x8
	ldr r1, [sp, #56]
	ldr r0, .L_081039f0
	adds r3, r1, r5
	lsls r3, r3, #3
	adds r3, #88
	mov r1, r9
	adds r2, r7, #0
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #1
.L_08103976:
	cmp r5, #0
	bne .L_08103988
	adds r3, r6, #0
	ldr r0, .L_081039f4
	adds r3, #88
	mov r1, r9
	adds r2, r7, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_08103988:
	movs r0, #15
	bl UiText_DrawNumberInWindowFar + 0x8
	movs r3, #11
	str r3, [sp, #0]
	mov r0, r9
	movs r1, #0
	movs r2, #11
	movs r3, #13
	bl UiWindow_DrawDividerLineFar
.L_0810399e:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #60]
	movs r3, #1
	strb r3, [r2, #3]
.L_081039a8:
	ldr r2, [sp, #212]
	cmp r2, #0
	bne .L_081039be
	str r2, [sp, #0]
	str r2, [sp, #4]
	ldr r0, [sp, #52]
	movs r1, #0
	ldr r2, [sp, #216]
	mov r3, r9
	bl RenderOutput_CreateFar + 0x10
.L_081039be:
	movs r2, #166
	ldr r1, [sp, #44]
	ldr r3, .L_081039f8
	ldr r0, [sp, #48]
	lsls r2, r2, #1
	mov lr, r3
	.2byte 0xf800
	ldr r0, [sp, #44]
	bl Sys_Free
	movs r0, #1
	add sp, #168
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081039e4:
	.4byte 0x000005a7
.L_081039e8:
	.4byte 0x0000101c
.L_081039ec:
	.4byte 0x000010d3
.L_081039f0:
	.4byte 0x000010d4
.L_081039f4:
	.4byte 0x000010d9
.L_081039f8:
	.4byte IwramCopyWords
