.syntax unified
	.thumb
	.global Battle_SelectAbility
	.thumb_func
Battle_SelectAbility:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #224
	str r0, [sp, #76]
	ldr r5, .L_08024184
	movs r3, #1
	ldr r1, [r5]
	movs r2, #0
	negs r3, r3
	movs r0, #128
	str r1, [sp, #72]
	str r2, [sp, #68]
	str r3, [sp, #64]
	bl Resource_LoadIntoFreeSlot
	str r0, [sp, #60]
	movs r0, #168
	lsls r0, r0, #1
	bl Runtime_BumpAllocateAlternatePool
	ldr r2, [sp, #64]
	movs r1, #0
	movs r3, #42
	movs r6, #0
	str r0, [sp, #56]
	str r1, [sp, #52]
	str r2, [sp, #48]
	str r3, [sp, #0]
	movs r1, #4
	movs r2, #30
	movs r3, #4
	movs r0, #0
	str r6, [sp, #40]
	str r6, [sp, #32]
	str r6, [sp, #80]
	str r6, [sp, #28]
	str r6, [sp, #24]
	bl UiWindow_Create
	str r0, [sp, #44]
	movs r0, #1
	bl UiWork_SetAltFlagAndClearTable
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #9
	movs r2, #9
	movs r3, #11
	movs r0, #21
	bl UiWindow_Create
	mov r9, r0
	adds r5, #168
	ldr r3, [r5]
	ldr r1, [r3, #52]
	ldr r2, [r3, #48]
	ldr r3, [r3, #56]
	mov r11, r1
	mov r10, r2
	str r3, [sp, #36]
	ldr r0, [sp, #76]
	bl Func_08077008
	adds r0, #248
	movs r7, #0
	mov r8, r0
.L_08023efe:
	ldr r1, [sp, #52]
	ldr r2, [sp, #56]
	lsls r3, r1, #2
	movs r6, #0
	adds r5, r3, r2
.L_08023f08:
	mov r1, r8
	movs r2, #1
	ldr r3, [r1, #16]
	lsls r2, r6
	ands r3, r2
	cmp r3, #0
	beq .L_08023f24
	lsls r3, r7, #8
	orrs r3, r6
	stmia r5!, {r3}
	ldr r2, [sp, #52]
	adds r2, #1
	str r2, [sp, #52]
	b .L_08023fba
.L_08023f24:
	mov r1, r8
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	beq .L_08023fba
	ldr r2, [sp, #76]
	movs r0, #0
	cmp r2, #7
	bls .L_08023f38
	movs r0, #1
.L_08023f38:
	bl Func_08077000
	movs r2, #132
	adds r3, r0, #0
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r3, [r3]
	movs r1, #0
	adds r0, #8
	movs r4, #0
	cmp r1, r3
	bge .L_08023f94
	ldrb r3, [r0, #2]
	ldr r2, [sp, #76]
	cmp r3, r2
	bne .L_08023f64
	ldrb r3, [r0]
	cmp r3, r7
	bne .L_08023f64
	ldrb r3, [r0, #1]
	cmp r3, r6
	beq .L_08023f8e
.L_08023f64:
	movs r2, #128
	lsls r2, r2, #1
	adds r3, r0, r2
	ldr r3, [r3]
	adds r1, #1
	cmp r1, r3
	bge .L_08023f92
	lsls r4, r1, #2
	adds r2, r0, r4
	ldrb r3, [r2, #2]
	mov r12, r3
	ldr r3, [sp, #76]
	cmp r12, r3
	bne .L_08023f64
	ldrb r3, [r2]
	cmp r3, r7
	bne .L_08023f64
	ldrb r3, [r2, #1]
	cmp r3, r6
	bne .L_08023f64
	b .L_08023f94
.L_08023f8e:
	movs r4, #0
	b .L_08023f94
.L_08023f92:
	lsls r4, r1, #2
.L_08023f94:
	lsls r2, r7, #8
	movs r3, #128
	lsls r3, r3, #9
	orrs r2, r6
	orrs r2, r3
	str r2, [r5]
	adds r3, r0, r4
	ldrb r3, [r3, #3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_08023fb2
	lsls r3, r3, #17
	orrs r2, r3
	str r2, [r5]
.L_08023fb2:
	ldr r1, [sp, #52]
	adds r1, #1
	str r1, [sp, #52]
	adds r5, #4
.L_08023fba:
	adds r6, #1
	cmp r6, #19
	ble .L_08023f08
	movs r2, #4
	adds r7, #1
	add r8, r2
	cmp r7, #3
	ble .L_08023efe
	ldr r3, [sp, #52]
	ldr r1, [sp, #56]
	lsls r2, r3, #2
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r2, r1]
	ldr r3, [sp, #72]
	ldr r1, .L_08024188
	adds r2, r3, r1
	movs r3, #1
	strb r3, [r2]
	mov r2, sp
	adds r2, #212
	str r2, [sp, #4]
	ldr r1, [sp, #72]
	ldr r2, .L_08024188
	mov r3, sp
	adds r3, #84
	adds r2, r1, r2
	str r3, [sp, #12]
	str r2, [sp, #8]
.L_08023ff4:
	ldr r3, [sp, #48]
	cmp r11, r3
	bne .L_08024008
	ldr r1, [sp, #64]
	cmp r10, r1
	bne .L_08024008
	ldr r2, [sp, #28]
	cmp r2, #0
	bne .L_08024008
	b .L_08024328
.L_08024008:
	mov r3, r11
	ldr r1, [sp, #56]
	add r3, r10
	lsls r3, r3, #2
	ldr r5, [r3, r1]
	ldr r3, [sp, #72]
	ldr r1, .L_0802418c
	movs r2, #0
	str r2, [sp, #40]
	adds r2, r3, r1
	movs r3, #1
	strb r3, [r2]
	mov r2, r9
	ldrh r0, [r2, #12]
	ldrh r1, [r2, #14]
	ldr r2, [sp, #64]
	lsls r3, r2, #1
	adds r1, r1, r3
	mov r3, r9
	ldrh r2, [r3, #8]
	movs r3, #15
	adds r1, #1
	str r3, [sp, #0]
	adds r0, #1
	subs r2, #2
	movs r3, #1
	bl Ui_SetRectHighlight
	ldr r1, [sp, #32]
	cmp r1, #0
	beq .L_08024064
	ldr r0, [sp, #44]
	movs r1, #1
	bl UiWork_Finalize
	movs r3, #42
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #4
	movs r2, #30
	movs r3, #4
	bl UiWindow_Create
	str r0, [sp, #44]
	bl Ui_FillVramBlockPattern
.L_08024064:
	ldr r3, [sp, #52]
	movs r2, #0
	str r2, [sp, #28]
	cmp r3, #0
	bne .L_08024070
	b .L_080241a0
.L_08024070:
	bl UiWork_ClearValueNameTables
	movs r1, #0
	str r1, [sp, #24]
	ldr r0, .L_08024190
	ldr r3, [r0]
	movs r2, #228
	ldr r3, [r3, r2]
	ldr r1, [sp, #28]
	cmp r3, r5
	bne .L_0802408c
	movs r2, #1
	str r2, [sp, #24]
	b .L_080240a2
.L_0802408c:
	adds r1, #1
	cmp r1, #7
	bgt .L_080240a2
	ldr r3, [r0]
	lsls r2, r1, #2
	adds r2, #228
	ldr r3, [r3, r2]
	cmp r3, r5
	bne .L_0802408c
	movs r3, #1
	str r3, [sp, #24]
.L_080240a2:
	ldr r1, [sp, #24]
	cmp r1, #0
	beq .L_080240ca
	ldr r6, [sp, #12]
	movs r2, #52
	ldr r0, .L_08024194
	adds r1, r6, #0
	bl UiText_CopyMessageString
	ldr r2, [sp, #68]
	cmp r2, #0
	beq .L_080241ac
	adds r0, r2, #0
	movs r1, #1
	bl UiWork_Finalize
	movs r3, #0
	str r3, [sp, #68]
	str r3, [sp, #32]
	b .L_080241ac
.L_080240ca:
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r5
	cmp r3, #0
	beq .L_0802414c
	movs r0, #248
	lsls r0, r0, #14
	ands r0, r5
	cmp r0, #0
	beq .L_0802411e
	lsrs r0, r0, #17
	movs r1, #5
	bl UiWork_PushValueSlot
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r5
	lsrs r3, r3, #8
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r3, #255
	ands r5, r3
	ldr r6, [sp, #12]
	ldr r3, .L_08024198
	lsls r0, r0, #2
	adds r0, r0, r5
	adds r1, r6, #0
	adds r0, r0, r3
	movs r2, #52
	bl UiText_CopyMessageString
	ldr r1, [sp, #68]
	cmp r1, #0
	beq .L_080241ac
	adds r0, r1, #0
	movs r1, #1
	bl UiWork_Finalize
	movs r2, #0
	str r2, [sp, #68]
	str r2, [sp, #32]
	b .L_080241ac
.L_0802411e:
	add r3, sp, #80
	str r3, [sp, #0]
	ldr r1, [sp, #76]
	adds r2, r5, #0
	ldr r3, [sp, #32]
	ldr r0, [sp, #68]
	bl DjinnMenu_ShowChangePreview
	ldr r6, [sp, #12]
	str r0, [sp, #68]
	adds r1, r6, #0
	ldr r0, .L_0802419c
	movs r2, #52
	bl UiText_CopyMessageString
	movs r3, #240
	lsls r3, r3, #4
	ands r5, r3
	lsrs r3, r5, #8
	movs r1, #1
	lsls r1, r3
	str r1, [sp, #40]
	b .L_080241ac
.L_0802414c:
	add r3, sp, #80
	adds r2, r5, #0
	str r3, [sp, #0]
	ldr r1, [sp, #76]
	ldr r3, [sp, #32]
	ldr r0, [sp, #68]
	bl DjinnMenu_ShowChangePreview
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r5
	lsrs r3, r3, #8
	str r0, [sp, #68]
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r3, #255
	ands r5, r3
	lsls r0, r0, #2
	ldr r3, .L_08024198
	ldr r6, [sp, #12]
	adds r0, r0, r5
	adds r0, r0, r3
	adds r1, r6, #0
	movs r2, #52
	bl UiText_CopyMessageString
	b .L_080241ac
	.2byte 0x0000
.L_08024184:
	.4byte Data_03001e8c
.L_08024188:
	.4byte 0x00000ea3
.L_0802418c:
	.4byte 0x00000ea6
.L_08024190:
	.4byte gLinkCountdownWork
.L_08024194:
	.4byte 0x000008ef
.L_08024198:
	.4byte 0x00000666
.L_0802419c:
	.4byte 0x00000899
.L_080241a0:
	ldr r6, [sp, #12]
	ldr r0, .L_080244dc
	adds r1, r6, #0
	movs r2, #52
	bl UiText_CopyMessageString
.L_080241ac:
	ldr r2, [sp, #72]
	ldr r3, .L_080244e0
	movs r1, #0
	adds r5, r2, r3
	strb r1, [r5]
	ldr r2, [sp, #32]
	cmp r2, #0
	bne .L_080241e0
	movs r3, #1
	strb r3, [r5]
	ldr r0, [sp, #44]
	movs r1, #1
	bl UiWork_Finalize
	movs r3, #42
	str r3, [sp, #0]
	movs r0, #0
	movs r3, #4
	movs r1, #4
	movs r2, #30
	bl UiWindow_Create
	str r0, [sp, #44]
	add r3, sp, #32
	ldrb r3, [r3]
	strb r3, [r5]
.L_080241e0:
	ldr r1, [sp, #44]
	movs r2, #0
	adds r0, r6, #0
	movs r3, #4
	bl UiText_RenderWideStringAtOffset
	ldr r2, [sp, #48]
	mov r1, r10
	str r1, [sp, #64]
	cmp r11, r2
	beq .L_080242ae
	mov r0, r9
	bl RenderOutput_RedrawSavedRect
	mov r1, r11
	ldr r2, [sp, #56]
	lsls r3, r1, #2
	adds r3, r3, r2
	movs r1, #128
	ldr r6, [r3]
	lsls r1, r1, #24
	movs r7, #0
	cmp r6, r1
	beq .L_080242a8
	mov r8, r3
.L_08024212:
	movs r2, #240
	lsls r2, r2, #4
	adds r1, r6, #0
	ands r1, r2
	ldr r3, .L_080244e4
	lsrs r1, r1, #8
	adds r1, r1, r3
	movs r2, #0
	lsls r3, r7, #1
	mov r0, r9
	str r2, [sp, #0]
	bl UiWindow_SetTilemapEntry
	movs r3, #248
	lsls r3, r3, #14
	ands r3, r6
	cmp r3, #0
	beq .L_0802423e
	movs r0, #4
	bl UiWork_SetParamNibble
	b .L_0802424e
.L_0802423e:
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r6
	cmp r3, #0
	beq .L_0802424e
	movs r0, #2
	bl UiWork_SetParamNibble
.L_0802424e:
	movs r1, #240
	lsls r1, r1, #4
	adds r3, r6, #0
	ands r3, r1
	lsrs r3, r3, #8
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r3, #255
	ands r3, r6
	lsls r0, r0, #2
	adds r0, r0, r3
	ldr r3, .L_080244e8
	lsls r5, r7, #4
	adds r0, r0, r3
	mov r1, r9
	movs r2, #8
	adds r3, r5, #0
	bl UiText_DrawCharacterAtOffset
	movs r0, #248
	lsls r0, r0, #14
	ands r0, r6
	cmp r0, #0
	beq .L_0802428c
	lsrs r0, r0, #17
	movs r1, #1
	mov r2, r9
	movs r3, #48
	str r5, [sp, #0]
	bl UiText_DrawNumberAtOffset
.L_0802428c:
	movs r0, #15
	adds r7, #1
	bl UiWork_SetParamNibble
	cmp r7, #4
	bgt .L_080242a8
	movs r2, #4
	add r8, r2
	mov r3, r8
	movs r1, #128
	ldr r6, [r3]
	lsls r1, r1, #24
	cmp r6, r1
	bne .L_08024212
.L_080242a8:
	mov r12, r11
	mov r2, r12
	str r2, [sp, #48]
.L_080242ae:
	ldr r1, [sp, #52]
	cmp r1, #5
	ble .L_080242f8
	movs r7, #0
	adds r1, #4
	mov r8, r1
	b .L_080242ea
.L_080242bc:
	ldr r2, .L_080244ec
	mov r0, r11
	movs r1, #5
	adds r6, r7, r2
	bl FixedPoint_Ratio
	cmp r7, r0
	bne .L_080242d0
	ldr r3, .L_080244f0
	adds r6, r7, r3
.L_080242d0:
	mov r1, r9
	ldrh r2, [r1, #8]
	subs r2, r2, r5
	adds r2, r2, r7
	movs r3, #0
	str r3, [sp, #0]
	subs r2, #2
	mov r0, r9
	adds r1, r6, #0
	subs r3, #1
	bl UiWindow_SetTilemapEntry
	adds r7, #1
.L_080242ea:
	mov r0, r8
	movs r1, #5
	bl FixedPoint_Ratio
	adds r5, r0, #0
	cmp r7, r5
	blt .L_080242bc
.L_080242f8:
	mov r1, r9
	ldrh r0, [r1, #12]
	mov r2, r10
	ldrh r1, [r1, #14]
	lsls r3, r2, #1
	adds r1, r1, r3
	mov r3, r9
	ldrh r2, [r3, #8]
	movs r3, #14
	adds r1, #1
	subs r2, #2
	str r3, [sp, #0]
	adds r0, #1
	movs r3, #1
	bl Ui_SetRectHighlight
	ldr r1, [sp, #8]
	movs r3, #1
	strb r3, [r1]
	ldr r2, [sp, #72]
	ldr r1, .L_080244e0
	adds r3, r2, r1
	movs r2, #0
	strb r2, [r3]
.L_08024328:
	ldr r3, [sp, #52]
	cmp r3, #5
	ble .L_08024412
	movs r7, #0
	adds r3, #4
	mov r8, r3
	b .L_0802438a
.L_08024336:
	ldr r3, .L_080244f4
	movs r2, #128
	ldr r3, [r3]
	ldr r1, .L_080244ec
	lsls r2, r2, #1
	ands r3, r2
	adds r6, r7, r1
	cmp r3, #0
	bne .L_08024354
	ldr r3, .L_080244f8
	ldr r3, [r3]
	movs r2, #15
	ands r3, r2
	cmp r3, #11
	bhi .L_08024364
.L_08024354:
	mov r0, r11
	movs r1, #5
	bl FixedPoint_Ratio
	cmp r7, r0
	bne .L_08024364
	ldr r2, .L_080244f0
	adds r6, r7, r2
.L_08024364:
	mov r3, r9
	movs r1, #5
	mov r0, r8
	ldrh r5, [r3, #8]
	bl FixedPoint_Ratio
	subs r5, r5, r0
	adds r5, r5, r7
	movs r1, #0
	subs r5, #2
	movs r3, #1
	str r1, [sp, #0]
	mov r0, r9
	adds r1, r6, #0
	adds r2, r5, #0
	negs r3, r3
	bl UiWindow_SetTilemapEntry
	adds r7, #1
.L_0802438a:
	mov r0, r8
	movs r1, #5
	bl FixedPoint_Ratio
	cmp r7, r0
	blt .L_08024336
	ldr r3, .L_080244f4
	ldr r5, [r3]
	movs r3, #128
	lsls r3, r3, #1
	ands r5, r3
	cmp r5, #0
	bne .L_080243d0
	mov r3, r9
	ldrh r2, [r3, #8]
	movs r3, #1
	subs r2, r2, r0
	subs r2, #3
	mov r0, r9
	ldr r1, .L_080244fc
	negs r3, r3
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntry
	mov r1, r9
	ldrh r2, [r1, #8]
	movs r3, #1
	subs r2, #2
	mov r0, r9
	ldr r1, .L_08024500
	negs r3, r3
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntry
	b .L_080243fe
.L_080243d0:
	mov r3, r9
	ldrh r2, [r3, #8]
	movs r1, #0
	subs r2, r2, r0
	movs r3, #1
	subs r2, #3
	str r1, [sp, #0]
	mov r0, r9
	ldr r1, .L_08024504
	negs r3, r3
	bl UiWindow_SetTilemapEntry
	mov r3, r9
	ldrh r2, [r3, #8]
	movs r1, #0
	movs r3, #1
	str r1, [sp, #0]
	subs r2, #2
	mov r0, r9
	ldr r1, .L_08024508
	negs r3, r3
	bl UiWindow_SetTilemapEntry
.L_080243fe:
	mov r2, r9
	ldrh r3, [r2, #14]
	ldr r1, [sp, #8]
	subs r3, #1
	lsrs r3, r3, #2
	movs r2, #2
	lsls r2, r3
	ldrb r3, [r1]
	orrs r2, r3
	strb r2, [r1]
.L_08024412:
	ldr r3, .L_0802450c
	ldr r1, [r3]
	ldr r3, .L_08024510
	ldr r0, .L_08024514
	ldr r7, [r3]
	ldr r3, .L_080244f4
	ldr r2, [r0]
	ldr r3, [r3]
	mov r8, r3
	adds r3, r2, #0
	adds r3, #216
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0802444a
	adds r2, #220
	ldr r3, [r2]
	movs r1, #0
	movs r7, #0
	mov r8, r1
	cmp r3, #0
	bne .L_08024446
	movs r3, #60
	str r3, [r2]
	movs r7, #1
	movs r1, #1
	b .L_0802444a
.L_08024446:
	subs r3, #1
	str r3, [r2]
.L_0802444a:
	ldr r2, [r0]
	ldr r3, [r2, #76]
	cmp r3, #0
	beq .L_0802445a
	movs r3, #2
	ands r3, r1
	cmp r3, #0
	beq .L_08024466
.L_0802445a:
	movs r0, #113
	movs r6, #1
	bl Func_080f9010
	negs r6, r6
	b .L_080248a0
.L_08024466:
	movs r3, #1
	ands r3, r1
	cmp r3, #0
	beq .L_0802451c
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_080244d6
	mov r3, r11
	add r3, r10
	ldr r1, [sp, #56]
	lsls r3, r3, #2
	ldr r0, [r3, r1]
	movs r6, #248
	lsls r6, r6, #14
	adds r5, r0, #0
	ands r5, r6
	cmp r5, #0
	bne .L_080244a0
	ldr r3, [sp, #24]
	cmp r3, #0
	bne .L_080244ce
	mov r1, r11
	mov r3, r10
	str r1, [r2, #52]
	str r3, [r2, #48]
	ldr r1, [sp, #36]
	adds r6, r0, #0
	str r1, [r2, #56]
	b .L_080248a0
.L_080244a0:
	ldr r2, [sp, #24]
	cmp r2, #0
	bne .L_080244ce
	ands r5, r6
	bl Ui_FillVramBlockPattern
	bl UiWork_ClearValueNameTables
	lsrs r0, r5, #17
	movs r1, #5
	bl UiWork_PushValueSlot
	movs r2, #52
	ldr r1, [sp, #12]
	ldr r0, .L_08024518
	bl UiText_CopyMessageString
	movs r2, #0
	ldr r0, [sp, #12]
	ldr r1, [sp, #44]
	movs r3, #4
	bl UiText_RenderWideStringAtOffset
.L_080244ce:
	movs r0, #114
	bl Func_080f9010
	b .L_0802451c
.L_080244d6:
	movs r6, #1
	negs r6, r6
	b .L_080248a0
.L_080244dc:
	.4byte 0x000008ed
.L_080244e0:
	.4byte 0x00000ea6
.L_080244e4:
	.4byte 0x00005001
.L_080244e8:
	.4byte 0x0000045f
.L_080244ec:
	.4byte 0x0000f301
.L_080244f0:
	.4byte 0x0000f30b
.L_080244f4:
	.4byte Data_03001ae8
.L_080244f8:
	.4byte gFrameCount
.L_080244fc:
	.4byte 0x0000f334
.L_08024500:
	.4byte 0x0000f335
.L_08024504:
	.4byte 0x0000f011
.L_08024508:
	.4byte 0x0000f012
.L_0802450c:
	.4byte gKeyState
.L_08024510:
	.4byte gKeysRepeat
.L_08024514:
	.4byte gLinkCountdownWork
.L_08024518:
	.4byte 0x00000898
.L_0802451c:
	ldr r3, [sp, #52]
	cmp r3, #0
	bne .L_08024524
	b .L_08024766
.L_08024524:
	movs r3, #128
	ands r3, r7
	cmp r3, #0
	beq .L_08024550
	movs r0, #111
	bl Func_080f9010
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #5
	beq .L_08024546
	mov r3, r11
	ldr r1, [sp, #52]
	add r3, r10
	cmp r3, r1
	bne .L_0802454a
.L_08024546:
	movs r2, #0
	mov r10, r2
.L_0802454a:
	mov r3, r10
	str r3, [sp, #36]
	b .L_08024766
.L_08024550:
	movs r3, #64
	ands r3, r7
	cmp r3, #0
	beq .L_08024590
	movs r0, #111
	bl Func_080f9010
	movs r1, #1
	negs r1, r1
	add r10, r1
	mov r2, r10
	cmp r2, #0
	bge .L_0802458a
	ldr r0, [sp, #52]
	movs r1, #5
	subs r0, #1
	bl FixedPoint_Ratio
	lsls r3, r0, #2
	adds r3, r3, r0
	cmp r11, r3
	bne .L_08024586
	ldr r1, [sp, #52]
	mov r2, r11
	subs r3, r1, r2
	subs r3, #1
	b .L_08024588
.L_08024586:
	movs r3, #4
.L_08024588:
	mov r10, r3
.L_0802458a:
	mov r1, r10
	str r1, [sp, #36]
	b .L_08024766
.L_08024590:
	movs r3, #128
	lsls r3, r3, #1
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_08024694
	ldr r3, [sp, #68]
	cmp r3, #0
	beq .L_08024622
	ldr r0, [sp, #80]
	movs r5, #0
	cmp r5, r0
	bge .L_080245e6
.L_080245aa:
	ldr r3, .L_0802472c
	ldr r2, .L_08024730
	ldr r3, [r3]
	adds r1, r5, r2
	movs r2, #15
	ands r3, r2
	cmp r3, #11
	bhi .L_080245c8
	ldr r3, [sp, #32]
	subs r3, #1
	cmp r5, r3
	bne .L_080245c8
	ldr r3, [sp, #32]
	ldr r2, .L_08024734
	adds r1, r3, r2
.L_080245c8:
	ldr r3, [sp, #68]
	ldrh r2, [r3, #8]
	subs r2, r2, r0
	adds r2, r2, r5
	movs r3, #0
	str r3, [sp, #0]
	ldr r0, [sp, #68]
	subs r2, #2
	subs r3, #1
	bl UiWindow_SetTilemapEntry
	ldr r0, [sp, #80]
	adds r5, #1
	cmp r5, r0
	blt .L_080245aa
.L_080245e6:
	ldr r1, [sp, #68]
	ldrh r2, [r1, #8]
	movs r3, #0
	subs r2, r2, r0
	str r3, [sp, #0]
	adds r0, r1, #0
	subs r2, #3
	ldr r1, .L_08024738
	subs r3, #1
	bl UiWindow_SetTilemapEntry
	ldr r1, [sp, #68]
	ldrh r2, [r1, #8]
	movs r3, #0
	str r3, [sp, #0]
	adds r0, r1, #0
	subs r2, #2
	ldr r1, .L_0802473c
	subs r3, #1
	bl UiWindow_SetTilemapEntry
	ldr r1, [sp, #68]
	ldrh r2, [r1, #14]
	ldr r1, [sp, #8]
	lsrs r2, r2, #2
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	orrs r3, r2
	strb r3, [r1]
.L_08024622:
	ldr r2, [sp, #32]
	cmp r2, #0
	bne .L_08024642
	ldr r0, [sp, #80]
	cmp r0, #0
	beq .L_08024644
	ldr r3, [sp, #68]
	cmp r3, #0
	beq .L_0802463a
	adds r0, r3, #0
	bl RenderOutput_ClearList
.L_0802463a:
	movs r1, #1
	str r1, [sp, #32]
	str r1, [sp, #28]
	b .L_08024766
.L_08024642:
	ldr r0, [sp, #80]
.L_08024644:
	ldr r2, [sp, #32]
	cmp r2, r0
	ble .L_0802464c
	str r0, [sp, #32]
.L_0802464c:
	ldr r3, [sp, #32]
	cmp r3, #0
	bne .L_08024654
	b .L_08024766
.L_08024654:
	movs r3, #16
	ands r3, r7
	cmp r3, #0
	beq .L_08024672
	movs r0, #111
	bl Func_080f9010
	ldr r1, [sp, #32]
	ldr r3, [sp, #80]
	adds r1, #1
	str r1, [sp, #32]
	cmp r1, r3
	ble .L_0802468e
	movs r2, #1
	b .L_0802468c
.L_08024672:
	movs r3, #32
	ands r3, r7
	cmp r3, #0
	beq .L_08024766
	movs r0, #111
	bl Func_080f9010
	ldr r1, [sp, #32]
	subs r1, #1
	str r1, [sp, #32]
	cmp r1, #0
	bgt .L_0802468e
	ldr r2, [sp, #80]
.L_0802468c:
	str r2, [sp, #32]
.L_0802468e:
	movs r3, #1
	str r3, [sp, #28]
	b .L_08024766
.L_08024694:
	ldr r1, [sp, #32]
	cmp r1, #0
	beq .L_080246b0
	ldr r2, [sp, #68]
	cmp r2, #0
	beq .L_080246a6
	adds r0, r2, #0
	bl RenderOutput_ClearList
.L_080246a6:
	movs r3, #0
	movs r1, #1
	str r3, [sp, #32]
	str r1, [sp, #28]
	b .L_08024766
.L_080246b0:
	movs r3, #16
	ands r3, r7
	cmp r3, #0
	beq .L_08024708
	movs r0, #111
	bl Func_080f9010
	bl Runtime_SetMainState19
	mov r3, r11
	ldr r2, [sp, #52]
	adds r3, #5
	cmp r3, r2
	blt .L_080246dc
	mov r3, r11
	cmp r3, #0
	beq .L_08024766
	ldr r2, [sp, #36]
	movs r1, #0
	mov r11, r1
	mov r10, r2
	b .L_08024766
.L_080246dc:
	ldr r0, [sp, #52]
	mov r11, r3
	ldr r3, [sp, #36]
	subs r0, #1
	movs r1, #5
	mov r10, r3
	bl FixedPoint_Ratio
	lsls r3, r0, #2
	adds r3, r3, r0
	cmp r11, r3
	bne .L_08024766
	ldr r1, [sp, #52]
	mov r2, r11
	subs r3, r1, r2
	subs r3, #1
	mov r10, r3
	ldr r3, [sp, #36]
	cmp r10, r3
	ble .L_08024766
	mov r10, r3
	b .L_08024766
.L_08024708:
	movs r3, #32
	ands r3, r7
	cmp r3, #0
	beq .L_08024766
	movs r0, #111
	bl Func_080f9010
	bl Runtime_SetMainState19
	mov r1, r11
	cmp r1, #0
	beq .L_08024740
	movs r2, #5
	ldr r3, [sp, #36]
	negs r2, r2
	add r11, r2
	mov r10, r3
	b .L_08024766
.L_0802472c:
	.4byte gFrameCount
.L_08024730:
	.4byte 0x0000f301
.L_08024734:
	.4byte 0x0000f30a
.L_08024738:
	.4byte 0x0000f334
.L_0802473c:
	.4byte 0x0000f335
.L_08024740:
	ldr r0, [sp, #52]
	movs r1, #5
	subs r0, #1
	bl FixedPoint_Ratio
	lsls r3, r0, #2
	ldr r1, [sp, #36]
	adds r3, r3, r0
	mov r11, r3
	mov r10, r1
	cmp r3, #0
	beq .L_08024766
	ldr r2, [sp, #52]
	subs r3, r2, r3
	subs r3, #1
	mov r10, r3
	cmp r10, r1
	ble .L_08024766
	mov r10, r1
.L_08024766:
	mov r1, r9
	ldrh r3, [r1, #12]
	lsls r3, r3, #3
	mov r2, r10
	subs r3, #2
	str r3, [sp, #16]
	lsls r3, r2, #1
	ldrh r2, [r1, #14]
	adds r3, r3, r2
	lsls r3, r3, #3
	adds r3, #20
	ldr r1, [sp, #4]
	str r3, [sp, #20]
	movs r3, #128
	lsls r3, r3, #23
	movs r2, #0
	str r3, [r1, #4]
	str r2, [r1, #8]
	ldr r0, [sp, #60]
	ldr r1, .L_080247d8
	bl Resource_GetBuffer
	ldr r3, .L_080247c8
	ldr r1, [sp, #4]
	ands r0, r3
	ldr r2, .L_080247cc
	ldrh r3, [r1, #8]
	ldr r6, .L_080247dc
	ands r3, r2
	orrs r3, r0
	adds r2, r1, #0
	strh r3, [r2, #8]
	ldr r2, [r6]
	movs r5, #4
	ldr r3, [sp, #16]
	ands r2, r5
	ldr r1, .L_080247e0
	lsrs r2, r2, #1
	adds r2, r3, r2
	adds r2, r2, r1
	ldr r3, .L_080247d0
	ldr r1, [sp, #4]
	ands r2, r3
	ldrh r3, [r1, #6]
	ldr r1, .L_080247d4
	ands r3, r1
	orrs r3, r2
	b .L_080247e4
	.2byte 0x0000
.L_080247c8:
	.4byte 0x000003ff
.L_080247cc:
	.4byte 0xfffffc00
.L_080247d0:
	.4byte 0x000001ff
.L_080247d4:
	.4byte 0xfffffe00
.L_080247d8:
	.4byte Resource_FixedBlockBTiles
.L_080247dc:
	.4byte gFrameCount
.L_080247e0:
	.4byte 0x0000fffa
.L_080247e4:
	ldr r2, [sp, #4]
	strh r3, [r2, #6]
	ldr r3, [r6]
	ldr r1, [sp, #20]
	ands r3, r5
	lsrs r3, r3, #2
	subs r3, r1, r3
	adds r3, #248
	strb r3, [r2, #4]
	ldr r2, [sp, #52]
	cmp r2, #0
	beq .L_08024804
	ldr r0, [sp, #4]
	movs r1, #242
	bl Runtime_PushSlotEntry
.L_08024804:
	ldr r3, .L_0802490c
	ldr r3, [r3]
	ldrh r2, [r3, #12]
	ldr r6, [r6]
	ldr r7, [r3]
	movs r3, #2
	ands r3, r2
	ands r6, r5
	cmp r3, #0
	beq .L_08024858
	movs r5, #0
.L_0802481a:
	negs r3, r6
	orrs r3, r6
	lsrs r3, r3, #31
	adds r2, r3, #0
	movs r3, #15
	subs r2, r3, r2
	ldr r1, [sp, #40]
	movs r3, #1
	lsls r3, r5
	ands r3, r1
	cmp r3, #0
	bne .L_08024834
	movs r2, #15
.L_08024834:
	ldr r3, .L_08024910
	ldrh r0, [r7, #12]
	ldrb r3, [r3, r5]
	adds r0, r0, r3
	ldr r3, .L_08024914
	ldrh r1, [r7, #14]
	ldrb r3, [r3, r5]
	adds r1, r1, r3
	str r2, [sp, #0]
	adds r0, #1
	adds r1, #1
	movs r2, #2
	movs r3, #2
	adds r5, #1
	bl Ui_SetRectHighlight
	cmp r5, #3
	ble .L_0802481a
.L_08024858:
	ldr r3, .L_08024918
	ldr r3, [r3]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	beq .L_0802487e
	ldr r5, .L_0802491c
	movs r2, #32
	adds r1, r5, #0
	ldr r6, .L_08024920
	ldr r0, .L_08024924
	bl _call_via_r6
	ldr r0, .L_08024928
	adds r1, r5, #0
	movs r2, #32
	bl _call_via_r6
	b .L_08024896
.L_0802487e:
	ldr r3, .L_0802492c
	movs r1, #32
	ldr r2, .L_08024930
	ldr r0, .L_08024924
	bl _call_via_r3
	ldr r3, .L_08024920
	ldr r0, .L_08024928
	ldr r1, .L_0802491c
	movs r2, #32
	bl _call_via_r3
.L_08024896:
	movs r0, #1
	bl WaitFrames
	bl .L_08023ff4
.L_080248a0:
	ldr r3, .L_0802490c
	ldr r1, [r3]
	ldrh r2, [r1, #12]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080248c4
	ldr r3, [r1]
	ldrh r0, [r3, #12]
	ldrh r1, [r3, #14]
	movs r3, #15
	str r3, [sp, #0]
	adds r0, #1
	adds r1, #1
	movs r2, #4
	movs r3, #4
	bl Ui_SetRectHighlight
.L_080248c4:
	ldr r0, [sp, #60]
	bl Resource_ResetEntry
	movs r1, #1
	ldr r0, [sp, #44]
	bl UiWork_Finalize
	movs r1, #1
	ldr r0, [sp, #68]
	bl UiWork_Finalize
	movs r1, #1
	mov r0, r9
	bl UiWork_Finalize
	bl UiWindow_MarkVisibleTileAttributes
	movs r0, #0
	bl UiWork_SetAltFlagAndClearTable
	ldr r0, [sp, #56]
	bl Runtime_BumpFree
	movs r0, #1
	bl WaitFrames
	adds r0, r6, #0
	add sp, #224
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_0802490c:
	.4byte Data_03001e90
.L_08024910:
	.4byte Data_080373e7
.L_08024914:
	.4byte Data_080373eb
.L_08024918:
	.4byte gFrameCount
.L_0802491c:
	.4byte Data_08037308
.L_08024920:
	.4byte IwramCopyWords
.L_08024924:
	.4byte 0x06006500
.L_08024928:
	.4byte 0x06006520
.L_0802492c:
	.4byte IwramFillWords
.L_08024930:
	.4byte 0x44444444
