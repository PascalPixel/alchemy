.syntax unified
	.thumb
	.global DjinnMenu_DrawStatPreview
	.thumb_func
DjinnMenu_DrawStatPreview:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #172
	mov r9, r0
	adds r0, r3, #0
	ldr r5, [sp, #204]
	ldr r6, [sp, #208]
	str r1, [sp, #64]
	str r2, [sp, #60]
	str r3, [sp, #56]
	bl Func_08077008
	ldr r3, .L_080ace18
	str r0, [sp, #52]
	movs r1, #149
	ldr r4, [r3]
	lsls r1, r1, #2
	adds r0, r4, #2
	adds r3, r5, r1
	ldrb r2, [r0, r3]
	str r2, [sp, #44]
	ldrb r3, [r4, r3]
	str r3, [sp, #40]
	movs r3, #188
	lsls r3, r3, #1
	mov r12, r3
	lsls r5, r5, #1
	add r5, r12
	movs r2, #128
	mov lr, r0
	lsls r2, r2, #8
	ldrh r0, [r4, r5]
	adds r3, r2, #0
	ands r3, r0
	lsls r3, r3, #16
	lsrs r3, r3, #16
	str r3, [sp, #36]
	adds r1, r6, r1
	mov r0, lr
	ldrb r0, [r0, r1]
	str r0, [sp, #32]
	ldrb r1, [r4, r1]
	lsls r6, r6, #1
	str r1, [sp, #28]
	add r6, r12
	ldrh r3, [r4, r6]
	movs r5, #166
	ands r2, r3
	lsls r5, r5, #1
	lsls r2, r2, #16
	lsrs r2, r2, #16
	adds r0, r5, #0
	str r2, [sp, #24]
	bl Runtime_BumpAllocate
	ldr r3, .L_080ace1c
	ldr r1, [sp, #52]
	str r0, [sp, #48]
	adds r2, r5, #0
	bl _call_via_r3
	ldr r1, [sp, #216]
	cmp r1, #0
	beq .L_080acb44
	b .L_080acc66
.L_080acb44:
	ldr r2, [sp, #212]
	cmp r2, #3
	bne .L_080acbd2
	ldr r1, [sp, #52]
	ldr r2, [sp, #64]
	movs r3, #52
	ldrsh r0, [r1, r3]
	lsls r2, r2, #3
	ldr r3, [sp, #60]
	mov r11, r2
	lsls r7, r3, #3
	mov r5, r11
	movs r1, #56
	adds r5, #80
	adds r1, r1, r7
	str r1, [sp, #0]
	mov r10, r1
	mov r2, r9
	adds r3, r5, #0
	movs r1, #3
	bl UiNumber_DrawAt
	movs r1, #64
	ldr r3, [sp, #52]
	adds r1, r1, r7
	movs r2, #54
	ldrsh r0, [r3, r2]
	mov r8, r1
	str r1, [sp, #0]
	mov r2, r9
	adds r3, r5, #0
	movs r1, #3
	bl UiNumber_DrawAt
	ldr r3, [sp, #52]
	subs r5, #32
	mov r1, r10
	movs r2, #56
	ldrsh r0, [r3, r2]
	str r1, [sp, #0]
	mov r2, r9
	adds r3, r5, #0
	movs r1, #3
	bl UiNumber_DrawAt
	ldr r3, [sp, #52]
	mov r1, r8
	movs r2, #58
	ldrsh r0, [r3, r2]
	str r1, [sp, #0]
	adds r3, r5, #0
	movs r1, #3
	mov r2, r9
	bl UiNumber_DrawAt
	mov r6, r11
	ldr r5, .L_080ace20
	adds r6, #72
	adds r0, r5, #0
	mov r1, r9
	adds r2, r6, #0
	mov r3, r10
	bl Func_08015090
	adds r0, r5, #0
	mov r1, r9
	adds r2, r6, #0
	mov r3, r8
	bl Func_08015090
	b .L_080acc0c
.L_080acbd2:
	ldr r3, [sp, #52]
	ldr r1, [sp, #64]
	movs r2, #56
	ldrsh r0, [r3, r2]
	ldr r2, [sp, #60]
	lsls r1, r1, #3
	lsls r7, r2, #3
	mov r11, r1
	mov r5, r11
	adds r3, r7, #0
	adds r5, #48
	adds r3, #56
	str r3, [sp, #0]
	movs r1, #3
	adds r3, r5, #0
	mov r2, r9
	bl UiNumber_DrawAt
	ldr r1, [sp, #52]
	movs r3, #58
	ldrsh r0, [r1, r3]
	adds r3, r7, #0
	adds r3, #64
	str r3, [sp, #0]
	movs r1, #3
	mov r2, r9
	adds r3, r5, #0
	bl UiNumber_DrawAt
.L_080acc0c:
	ldr r2, [sp, #52]
	mov r5, r11
	adds r3, r7, #0
	adds r5, #48
	adds r3, #72
	ldrh r0, [r2, #60]
	movs r1, #3
	str r3, [sp, #0]
	mov r2, r9
	adds r3, r5, #0
	bl UiNumber_DrawAt
	ldr r3, [sp, #52]
	ldrh r0, [r3, #62]
	adds r3, r7, #0
	adds r3, #80
	str r3, [sp, #0]
	movs r1, #3
	mov r2, r9
	adds r3, r5, #0
	bl UiNumber_DrawAt
	ldr r3, [sp, #52]
	adds r3, #64
	ldrh r0, [r3]
	adds r3, r7, #0
	adds r3, #88
	str r3, [sp, #0]
	movs r1, #3
	mov r2, r9
	adds r3, r5, #0
	bl UiNumber_DrawAt
	ldr r3, [sp, #52]
	adds r2, r7, #0
	adds r3, #66
	ldrb r0, [r3]
	adds r2, #96
	mov r3, r11
	str r2, [sp, #0]
	adds r3, #56
	movs r1, #2
	mov r2, r9
	bl UiNumber_DrawAt
.L_080acc66:
	ldr r0, [sp, #212]
	cmp r0, #1
	beq .L_080acc9e
	cmp r0, #1
	bgt .L_080acc76
	cmp r0, #0
	beq .L_080acc82
	b .L_080acd0a
.L_080acc76:
	ldr r1, [sp, #212]
	cmp r1, #2
	beq .L_080accb2
	cmp r1, #4
	beq .L_080accea
	b .L_080acd0a
.L_080acc82:
	ldr r2, [sp, #28]
	movs r5, #31
	ands r5, r2
	ldr r1, [sp, #32]
	adds r2, r5, #0
	ldr r0, [sp, #56]
	bl Djinn_AddToOwnerFar
	adds r2, r5, #0
	ldr r0, [sp, #56]
	ldr r1, [sp, #32]
	bl Djinn_ActivateFar
	b .L_080acd0a
.L_080acc9e:
	ldr r0, [sp, #40]
	movs r3, #31
	ands r0, r3
	str r0, [sp, #40]
	ldr r1, [sp, #44]
	ldr r0, [sp, #56]
	ldr r2, [sp, #40]
	bl Func_080771b8
	b .L_080acd0a
.L_080accb2:
	ldr r1, [sp, #36]
	cmp r1, #0
	beq .L_080accc8
	ldr r2, [sp, #40]
	movs r3, #31
	ands r2, r3
	ldr r0, [sp, #56]
	ldr r1, [sp, #44]
	str r2, [sp, #40]
	bl Func_080771b8
.L_080accc8:
	ldr r3, [sp, #28]
	movs r5, #31
	ands r5, r3
	ldr r0, [sp, #56]
	ldr r1, [sp, #32]
	adds r2, r5, #0
	bl Djinn_AddToOwnerFar
	ldr r0, [sp, #24]
	cmp r0, #0
	beq .L_080acd0a
	ldr r0, [sp, #56]
	ldr r1, [sp, #32]
	adds r2, r5, #0
	bl Djinn_ActivateFar
	b .L_080acd0a
.L_080accea:
	ldr r1, [sp, #28]
	movs r5, #31
	ands r5, r1
	adds r2, r5, #0
	ldr r0, [sp, #56]
	ldr r1, [sp, #32]
	bl Djinn_AddToOwnerFar
	ldr r2, [sp, #24]
	cmp r2, #0
	beq .L_080acd0a
	ldr r0, [sp, #56]
	ldr r1, [sp, #32]
	adds r2, r5, #0
	bl Djinn_ActivateFar
.L_080acd0a:
	ldr r0, [sp, #56]
	bl Owner_RecalculateStatsFar
	ldr r0, [sp, #56]
	bl Func_08077008
	ldr r3, [sp, #216]
	str r0, [sp, #52]
	cmp r3, #0
	bne .L_080acdca
	ldr r0, [sp, #64]
	ldr r1, [sp, #60]
	lsls r0, r0, #3
	mov r11, r0
	lsls r7, r1, #3
	mov r6, r11
	adds r6, #40
	adds r5, r7, #0
	ldr r0, [sp, #52]
	adds r5, #16
	mov r1, r9
	adds r2, r6, #0
	adds r3, r7, #0
	bl Func_08015090
	adds r3, r5, #0
	ldr r0, .L_080ace24
	mov r1, r9
	adds r2, r6, #0
	bl Func_08015090
	ldr r2, [sp, #52]
	mov r3, r11
	ldrb r0, [r2, #15]
	adds r3, #88
	movs r1, #2
	mov r2, r9
	str r5, [sp, #0]
	bl UiNumber_DrawAt
	ldr r5, .L_080ace28
	adds r3, r7, #0
	adds r0, r5, #0
	adds r3, #56
	mov r1, r9
	mov r2, r11
	bl UiText_DrawCharacterAtOffsetFar
	adds r3, r7, #0
	adds r0, r5, #1
	adds r3, #64
	mov r1, r9
	mov r2, r11
	bl UiText_DrawCharacterAtOffsetFar
	adds r3, r7, #0
	adds r0, r5, #2
	adds r3, #72
	mov r1, r9
	mov r2, r11
	bl UiText_DrawCharacterAtOffsetFar
	adds r3, r7, #0
	adds r0, r5, #3
	adds r3, #80
	mov r1, r9
	mov r2, r11
	bl UiText_DrawCharacterAtOffsetFar
	adds r3, r7, #0
	adds r0, r5, #4
	adds r3, #88
	mov r1, r9
	mov r2, r11
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #5
	adds r3, r7, #0
	adds r3, #96
	adds r0, r5, #0
	mov r1, r9
	mov r2, r11
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, .L_080ace2c
	ldr r0, [sp, #48]
	adds r3, r0, r1
	ldrb r0, [r3]
	ldr r3, .L_080ace30
	adds r0, r0, r3
	adds r3, r7, #0
	adds r3, #32
	mov r1, r9
	mov r2, r11
	bl UiText_DrawCharacterAtOffsetFar
.L_080acdca:
	ldr r2, [sp, #216]
	cmp r2, #0
	beq .L_080acdd2
	b .L_080ad062
.L_080acdd2:
	ldr r3, .L_080ace2c
	ldr r0, [sp, #48]
	ldr r1, [sp, #52]
	adds r6, r0, r3
	adds r5, r1, r3
	ldrb r1, [r6]
	ldrb r3, [r5]
	mov r12, r1
	cmp r12, r3
	beq .L_080ace38
	ldr r1, [sp, #60]
	ldr r3, .L_080ace30
	ldrb r0, [r5]
	ldr r2, [sp, #64]
	adds r0, r0, r3
	lsls r3, r1, #3
	lsls r2, r2, #3
	adds r3, #48
	mov r1, r9
	mov r11, r2
	bl UiText_DrawCharacterAtOffsetFar
	ldr r3, [sp, #216]
	ldr r2, [sp, #64]
	ldr r1, .L_080ace34
	str r3, [sp, #0]
	adds r2, #2
	movs r3, #5
	mov r0, r9
	bl UiWindow_SetTilemapEntryFar
	ldrb r1, [r6]
	ldrb r3, [r5]
	b .L_080ace3e
	.2byte 0x0000
.L_080ace18:
	.4byte Data_03001f2c
.L_080ace1c:
	.4byte IwramCopyWords
.L_080ace20:
	.4byte Data_080af290
.L_080ace24:
	.4byte Data_080af28c
.L_080ace28:
	.4byte 0x000008ae
.L_080ace2c:
	.4byte 0x00000129
.L_080ace30:
	.4byte 0x00000741
.L_080ace34:
	.4byte 0x0000f296
.L_080ace38:
	ldr r0, [sp, #64]
	lsls r0, r0, #3
	mov r11, r0
.L_080ace3e:
	mov r12, r1
	ldr r2, [sp, #64]
	cmp r12, r3
	beq .L_080ace48
	adds r2, #5
.L_080ace48:
	ldr r1, [sp, #60]
	ldr r3, [sp, #52]
	movs r0, #142
	movs r4, #0
	adds r1, #5
	lsls r0, r0, #1
	mov r8, r1
	mov r10, r4
	adds r7, r2, #1
	adds r6, r3, r0
	adds r5, r2, #0
.L_080ace5e:
	ldr r2, .L_080ad108
	mov r3, r10
	adds r1, r4, r2
	str r3, [sp, #0]
	adds r2, r5, #0
	mov r0, r9
	mov r3, r8
	str r4, [sp, #8]
	bl UiWindow_SetTilemapEntryFar
	ldrb r1, [r6]
	ldr r0, .L_080ad10c
	mov r2, r10
	adds r1, r1, r0
	str r2, [sp, #0]
	mov r0, r9
	adds r2, r7, #0
	mov r3, r8
	bl UiWindow_SetTilemapEntryFar
	ldr r4, [sp, #8]
	adds r4, #1
	adds r6, #1
	adds r7, #2
	adds r5, #2
	cmp r4, #3
	ble .L_080ace5e
	ldr r2, [sp, #52]
	movs r3, #70
	movs r1, #56
	ldrsh r0, [r2, r1]
	ldr r2, [sp, #48]
	add r3, r11
	mov r8, r3
	movs r1, #56
	ldrsh r3, [r2, r1]
	cmp r0, r3
	beq .L_080aceea
	ldr r1, [sp, #60]
	lsls r2, r1, #3
	adds r5, r2, #0
	mov r3, r11
	adds r3, #72
	movs r1, #4
	mov r2, r9
	adds r5, #56
	str r5, [sp, #0]
	bl UiNumber_DrawAt
	ldr r0, [sp, #52]
	movs r3, #56
	ldrsh r2, [r0, r3]
	ldr r0, [sp, #48]
	movs r1, #56
	ldrsh r3, [r0, r1]
	cmp r2, r3
	ble .L_080acede
	mov r0, r9
	mov r1, r8
	adds r2, r5, #0
	movs r3, #0
	bl UiIcon_DrawVariantWithTileOffset
	b .L_080aceea
.L_080acede:
	mov r0, r9
	mov r1, r8
	adds r2, r5, #0
	movs r3, #1
	bl UiIcon_DrawVariantWithTileOffset
.L_080aceea:
	ldr r2, [sp, #52]
	movs r1, #58
	ldrsh r0, [r2, r1]
	ldr r2, [sp, #48]
	movs r1, #58
	ldrsh r3, [r2, r1]
	cmp r0, r3
	beq .L_080acf3a
	ldr r1, [sp, #60]
	lsls r2, r1, #3
	adds r5, r2, #0
	mov r3, r11
	adds r3, #72
	movs r1, #4
	mov r2, r9
	adds r5, #64
	str r5, [sp, #0]
	bl UiNumber_DrawAt
	ldr r0, [sp, #52]
	movs r3, #58
	ldrsh r2, [r0, r3]
	ldr r0, [sp, #48]
	movs r1, #58
	ldrsh r3, [r0, r1]
	cmp r2, r3
	ble .L_080acf2e
	mov r0, r9
	mov r1, r8
	adds r2, r5, #0
	movs r3, #0
	bl UiIcon_DrawVariantWithTileOffset
	b .L_080acf3a
.L_080acf2e:
	mov r0, r9
	mov r1, r8
	adds r2, r5, #0
	movs r3, #1
	bl UiIcon_DrawVariantWithTileOffset
.L_080acf3a:
	ldr r1, [sp, #52]
	ldr r0, [sp, #48]
	ldrh r2, [r1, #60]
	ldrh r3, [r0, #60]
	cmp r2, r3
	beq .L_080acf84
	ldr r1, [sp, #60]
	adds r0, r2, #0
	lsls r2, r1, #3
	adds r5, r2, #0
	mov r3, r11
	adds r3, #72
	mov r2, r9
	adds r5, #72
	movs r1, #4
	str r5, [sp, #0]
	bl UiNumber_DrawAt
	ldr r3, [sp, #52]
	ldr r0, [sp, #48]
	ldrh r2, [r3, #60]
	ldrh r3, [r0, #60]
	cmp r2, r3
	bls .L_080acf78
	mov r0, r9
	mov r1, r8
	adds r2, r5, #0
	movs r3, #0
	bl UiIcon_DrawVariantWithTileOffset
	b .L_080acf84
.L_080acf78:
	mov r0, r9
	mov r1, r8
	adds r2, r5, #0
	movs r3, #1
	bl UiIcon_DrawVariantWithTileOffset
.L_080acf84:
	ldr r1, [sp, #52]
	ldr r0, [sp, #48]
	ldrh r2, [r1, #62]
	ldrh r3, [r0, #62]
	cmp r2, r3
	beq .L_080acfce
	ldr r1, [sp, #60]
	adds r0, r2, #0
	lsls r2, r1, #3
	adds r5, r2, #0
	mov r3, r11
	adds r3, #72
	mov r2, r9
	adds r5, #80
	movs r1, #4
	str r5, [sp, #0]
	bl UiNumber_DrawAt
	ldr r3, [sp, #52]
	ldr r0, [sp, #48]
	ldrh r2, [r3, #62]
	ldrh r3, [r0, #62]
	cmp r2, r3
	bls .L_080acfc2
	mov r0, r9
	mov r1, r8
	adds r2, r5, #0
	movs r3, #0
	bl UiIcon_DrawVariantWithTileOffset
	b .L_080acfce
.L_080acfc2:
	mov r0, r9
	mov r1, r8
	adds r2, r5, #0
	movs r3, #1
	bl UiIcon_DrawVariantWithTileOffset
.L_080acfce:
	ldr r5, [sp, #52]
	ldr r7, [sp, #48]
	adds r5, #64
	adds r7, #64
	ldrh r2, [r5]
	ldrh r3, [r7]
	cmp r2, r3
	beq .L_080ad018
	ldr r1, [sp, #60]
	adds r0, r2, #0
	lsls r2, r1, #3
	adds r6, r2, #0
	mov r3, r11
	adds r3, #72
	mov r2, r9
	adds r6, #88
	movs r1, #4
	str r6, [sp, #0]
	bl UiNumber_DrawAt
	ldrh r2, [r5]
	ldrh r3, [r7]
	cmp r2, r3
	bls .L_080ad00c
	mov r0, r9
	mov r1, r8
	adds r2, r6, #0
	movs r3, #0
	bl UiIcon_DrawVariantWithTileOffset
	b .L_080ad018
.L_080ad00c:
	mov r0, r9
	mov r1, r8
	adds r2, r6, #0
	movs r3, #1
	bl UiIcon_DrawVariantWithTileOffset
.L_080ad018:
	ldr r7, [sp, #52]
	ldr r6, [sp, #48]
	adds r7, #66
	adds r6, #66
	ldrb r2, [r7]
	ldrb r3, [r6]
	cmp r2, r3
	beq .L_080ad062
	ldr r1, [sp, #60]
	adds r0, r2, #0
	lsls r2, r1, #3
	adds r5, r2, #0
	mov r3, r11
	adds r3, #88
	mov r2, r9
	adds r5, #96
	movs r1, #2
	str r5, [sp, #0]
	bl UiNumber_DrawAt
	ldrb r2, [r7]
	ldrb r3, [r6]
	cmp r2, r3
	bls .L_080ad056
	mov r0, r9
	mov r1, r8
	adds r2, r5, #0
	movs r3, #0
	bl UiIcon_DrawVariantWithTileOffset
	b .L_080ad062
.L_080ad056:
	mov r0, r9
	mov r1, r8
	adds r2, r5, #0
	movs r3, #1
	bl UiIcon_DrawVariantWithTileOffset
.L_080ad062:
	ldr r2, [sp, #216]
	cmp r2, #0
	bgt .L_080ad06a
	b .L_080ad212
.L_080ad06a:
	ldr r3, [sp, #212]
	movs r2, #3
	eors r2, r3
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	mov r10, r3
	mov r0, r10
	movs r3, #6
	subs r0, r3, r0
	ldr r3, [sp, #216]
	mov r10, r0
	subs r3, #1
	mov r1, r10
	muls r1, r3
	ldr r0, [sp, #48]
	mov r8, r1
	ldr r1, [sp, #52]
	add r2, sp, #68
	add r5, sp, #76
	adds r1, #88
	add r3, sp, #72
	str r2, [sp, #0]
	adds r0, #88
	adds r2, r5, #0
	bl OwnerAction_DiffSlots
	lsls r0, r0, #24
	str r0, [sp, #16]
	asrs r3, r0, #24
	ldr r0, [sp, #64]
	lsls r0, r0, #3
	movs r2, #0
	movs r1, #0
	mov r11, r0
	cmp r8, r3
	bge .L_080ad182
	cmp r1, r10
	bge .L_080ad17c
	movs r3, #0
	str r0, [sp, #20]
	mov r0, r8
	str r3, [sp, #12]
	lsls r3, r0, #1
	adds r7, r3, r5
.L_080ad0c4:
	ldr r1, [sp, #60]
	lsls r6, r2, #24
	asrs r2, r6, #23
	adds r2, r1, r2
	ldrh r3, [r7]
	ldr r0, .L_080ad110
	lsls r2, r2, #3
	ands r3, r0
	adds r2, #4
	mov r0, r9
	mov r1, r11
	bl UiIcon_CreateWithLoadedResource
	ldrh r2, [r7]
	ldr r3, .L_080ad100
	ands r3, r2
	cmp r3, #0
	beq .L_080ad0f0
	movs r0, #4
	bl UiWork_SetParamNibbleFar
	b .L_080ad11a
.L_080ad0f0:
	ldr r3, .L_080ad104
	ands r3, r2
	cmp r3, #0
	beq .L_080ad114
	movs r0, #2
	bl UiWork_SetParamNibbleFar
	b .L_080ad11a
.L_080ad100:
	.4byte 0x00008000
.L_080ad104:
	.4byte 0x00004000
.L_080ad108:
	.4byte 0x00005001
.L_080ad10c:
	.4byte 0x0000f030
.L_080ad110:
	.4byte 0x00003fff
.L_080ad114:
	movs r0, #15
	bl UiWork_SetParamNibbleFar
.L_080ad11a:
	ldr r1, [sp, #60]
	asrs r6, r6, #24
	ldrh r3, [r7]
	lsls r5, r6, #1
	ldr r0, .L_080ad250
	adds r5, r1, r5
	ldr r2, [sp, #20]
	ands r0, r3
	lsls r5, r5, #3
	ldr r3, .L_080ad254
	adds r5, #8
	adds r0, r0, r3
	mov r1, r9
	adds r2, #16
	adds r3, r5, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldrh r0, [r7]
	bl Func_08077080
	ldr r3, [sp, #20]
	ldrb r0, [r0, #9]
	movs r1, #2
	mov r2, r9
	adds r3, #88
	str r5, [sp, #0]
	bl Func_080150a8
	movs r1, #128
	ldr r0, [sp, #12]
	lsls r1, r1, #17
	adds r3, r0, r1
	ldr r0, [sp, #16]
	adds r6, #1
	lsrs r1, r3, #24
	movs r3, #1
	lsls r6, r6, #24
	add r8, r3
	asrs r3, r0, #24
	lsrs r2, r6, #24
	adds r7, #2
	cmp r8, r3
	bge .L_080ad182
	lsls r3, r1, #24
	str r3, [sp, #12]
	asrs r3, r3, #24
	cmp r3, r10
	blt .L_080ad0c4
	b .L_080ad182
.L_080ad17c:
	ldr r1, [sp, #64]
	lsls r1, r1, #3
	mov r11, r1
.L_080ad182:
	movs r0, #15
	bl UiWork_SetParamNibbleFar
	ldr r3, [sp, #60]
	mov r2, r11
	lsls r6, r3, #3
	ldr r0, .L_080ad258
	adds r2, #88
	mov r1, r9
	adds r3, r6, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, [sp, #212]
	cmp r0, #3
	beq .L_080ad206
	ldr r3, [sp, #72]
	movs r5, #0
	cmp r3, #0
	beq .L_080ad1be
	movs r0, #4
	bl UiWork_SetParamNibbleFar
	adds r3, r6, #0
	ldr r0, .L_080ad25c
	adds r3, #88
	mov r1, r9
	mov r2, r11
	bl UiText_DrawCharacterAtOffsetFar
	movs r5, #1
.L_080ad1be:
	ldr r3, [sp, #68]
	cmp r3, #0
	beq .L_080ad1de
	movs r0, #2
	bl UiWork_SetParamNibbleFar
	ldr r1, [sp, #60]
	adds r3, r1, r5
	lsls r3, r3, #3
	ldr r0, .L_080ad260
	adds r3, #88
	mov r1, r9
	mov r2, r11
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #1
.L_080ad1de:
	cmp r5, #0
	bne .L_080ad1f0
	adds r3, r6, #0
	ldr r0, .L_080ad264
	adds r3, #88
	mov r1, r9
	mov r2, r11
	bl UiText_DrawCharacterAtOffsetFar
.L_080ad1f0:
	movs r0, #15
	bl UiWork_SetParamNibbleFar
	movs r3, #11
	str r3, [sp, #0]
	mov r0, r9
	movs r1, #0
	movs r2, #11
	movs r3, #13
	bl UiWindow_DrawDividerLineFar
.L_080ad206:
	ldr r3, .L_080ad268
	ldr r2, .L_080ad26c
	ldr r3, [r3]
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
.L_080ad212:
	ldr r3, [sp, #216]
	cmp r3, #0
	bne .L_080ad228
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r0, [sp, #56]
	movs r1, #0
	ldr r2, [sp, #220]
	mov r3, r9
	bl SideObject_CreateFar
.L_080ad228:
	movs r2, #166
	ldr r1, [sp, #48]
	ldr r3, .L_080ad270
	ldr r0, [sp, #52]
	lsls r2, r2, #1
	bl _call_via_r3
	ldr r0, [sp, #48]
	bl Runtime_BumpFree
	movs r0, #1
	add sp, #172
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080ad250:
	.4byte 0x00003fff
.L_080ad254:
	.4byte 0x00000333
.L_080ad258:
	.4byte 0x00000aed
.L_080ad25c:
	.4byte 0x00000ba2
.L_080ad260:
	.4byte 0x00000ba3
.L_080ad264:
	.4byte 0x00000ba8
.L_080ad268:
	.4byte Data_03001e8c
.L_080ad26c:
	.4byte 0x00000ea3
.L_080ad270:
	.4byte IwramCopyWords
