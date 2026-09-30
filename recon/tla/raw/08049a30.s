.syntax unified
	.thumb
	.global Func_08049a30
	.thumb_func
Func_08049a30:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #304
	str r0, [sp, #80]
	str r2, [sp, #72]
	str r1, [sp, #76]
	movs r5, #192
	lsls r5, r5, #18
	ldr r1, [r5, #60]
	movs r2, #1
	negs r2, r2
	movs r0, #128
	str r1, [sp, #68]
	str r2, [sp, #64]
	str r2, [sp, #60]
	bl Resource_LoadIntoFreeSlot
	movs r3, #42
	str r0, [sp, #56]
	str r3, [sp, #0]
	movs r1, #5
	movs r2, #30
	movs r3, #4
	movs r0, #0
	bl UiWindow_Create
	movs r3, #0
	str r0, [sp, #52]
	str r3, [sp, #48]
	adds r5, #228
	ldr r3, [r5]
	ldr r1, [r3, #52]
	ldr r2, [r3, #48]
	ldr r3, [r3, #56]
	mov r9, r1
	mov r8, r2
	str r3, [sp, #44]
	movs r3, #6
	str r3, [sp, #0]
	movs r0, #15
	movs r3, #11
	movs r1, #9
	movs r2, #15
	bl UiWindow_Create
	mov r3, sp
	adds r3, #84
	str r3, [sp, #28]
	ldr r6, .L_08049af0
	movs r5, #128
	mov r11, r0
	movs r7, #0
	adds r4, r3, #0
	lsls r5, r5, #23
.L_08049aa6:
	movs r3, #0
	lsls r0, r7, #1
	str r5, [r4, #4]
	str r3, [r4, #8]
	mov r3, r11
	movs r1, #12
	ldrsh r2, [r3, r1]
	ldr r3, .L_08049aec
	ldrh r1, [r4, #6]
	lsls r2, r2, #3
	adds r2, #8
	ands r2, r3
	adds r3, r6, #0
	ands r3, r1
	orrs r3, r2
	mov r2, r11
	strh r3, [r4, #6]
	movs r1, #14
	ldrsh r3, [r2, r1]
	adds r7, #1
	adds r0, r0, r3
	lsls r0, r0, #3
	adds r0, #4
	strb r0, [r4, #4]
	adds r4, #12
	cmp r7, #4
	ble .L_08049aa6
	mov r3, sp
	adds r3, #144
	ldr r1, .L_08049af4
	str r3, [sp, #24]
	ldr r6, [sp, #28]
	str r3, [sp, #8]
	movs r5, #8
	b .L_08049af8
.L_08049aec:
	.4byte 0x000001ff
.L_08049af0:
	.4byte 0xfffffe00
.L_08049af4:
	.4byte 0xfffffc00
.L_08049af8:
	mov r10, r1
	movs r7, #4
.L_08049afc:
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	ldr r3, [sp, #8]
	movs r1, #1
	stmia r3!, {r0}
	negs r1, r1
	adds r2, r3, #0
	str r2, [sp, #8]
	bl Resource_GetBuffer
	ldr r3, .L_08049b2c
	mov r1, r10
	ands r0, r3
	ldrh r3, [r5, r6]
	subs r7, #1
	ands r3, r1
	orrs r3, r0
	strh r3, [r5, r6]
	adds r5, #12
	cmp r7, #0
	bge .L_08049afc
	b .L_08049b30
	.2byte 0x0000
.L_08049b2c:
	.4byte 0x000003ff
.L_08049b30:
	movs r5, #240
	lsls r5, r5, #8
	adds r5, #24
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #2
	bl Func_08049a04
	movs r1, #129
	lsls r1, r1, #1
	adds r0, r5, #0
	adds r1, #255
	bl Func_08049a04
	adds r5, #1
	movs r1, #132
	lsls r1, r1, #2
	adds r0, r5, #0
	bl Func_08049a04
	movs r1, #137
	lsls r1, r1, #1
	adds r1, #255
	adds r0, r5, #0
	bl Func_08049a04
	movs r2, #146
	lsls r2, r2, #1
	mov r3, r8
	mov r1, sp
	add r2, sp
	lsls r3, r3, #1
	adds r1, #164
	str r2, [sp, #20]
	str r3, [sp, #16]
	str r1, [sp, #32]
.L_08049b78:
	ldr r2, [sp, #64]
	cmp r9, r2
	bne .L_08049b86
	ldr r3, [sp, #60]
	cmp r8, r3
	bne .L_08049b86
	b .L_08049d5c
.L_08049b86:
	ldr r1, [sp, #68]
	movs r3, #1
	strb r3, [r1, #6]
	mov r3, r11
	movs r2, #12
	ldrsh r0, [r3, r2]
	movs r2, #14
	ldrsh r1, [r3, r2]
	ldr r2, [sp, #60]
	adds r0, #1
	lsls r3, r2, #1
	adds r1, r1, r3
	mov r3, r11
	ldrh r2, [r3, #8]
	movs r3, #15
	adds r1, #1
	str r3, [sp, #0]
	subs r2, #2
	movs r3, #1
	bl Func_08046134
	bl Ui_FillVramBlockPattern
	ldr r1, [sp, #72]
	cmp r1, #0
	beq .L_08049bfc
	mov r3, r9
	ldr r2, [sp, #76]
	add r3, r8
	lsls r3, r3, #1
	adds r5, r3, r2
	ldrh r1, [r5]
	ldr r0, [sp, #80]
	bl Item_ClassifyUseAbility
	cmp r0, #2
	bne .L_08049bd8
	ldr r5, [sp, #32]
	ldr r0, .L_08049bf4
	adds r1, r5, #0
	b .L_08049be6
.L_08049bd8:
	ldrh r3, [r5]
	ldr r0, .L_08049bf0
	ldr r5, [sp, #32]
	ands r0, r3
	ldr r3, .L_08049bf8
	adds r1, r5, #0
	adds r0, r0, r3
.L_08049be6:
	movs r2, #52
	bl UiText_CopyMessageString
	b .L_08049c08
	.2byte 0x0000
.L_08049bf0:
	.4byte 0x000001ff
.L_08049bf4:
	.4byte 0x00000d4f
.L_08049bf8:
	.4byte 0x00000092
.L_08049bfc:
	ldr r5, [sp, #32]
	ldr r0, .L_08049c64
	adds r1, r5, #0
	movs r2, #52
	bl UiText_CopyMessageString
.L_08049c08:
	ldr r1, [sp, #52]
	movs r3, #4
	adds r0, r5, #0
	movs r2, #0
	bl UiText_RenderWideStringAtOffset
	ldr r1, [sp, #64]
	mov r3, r8
	str r3, [sp, #60]
	cmp r9, r1
	beq .L_08049cde
	mov r0, r11
	bl RenderOutput_RedrawSavedRect
	ldr r1, [sp, #76]
	mov r2, r9
	lsls r3, r2, #1
	adds r3, r3, r1
	ldrh r5, [r3]
	movs r7, #0
	cmp r5, #0
	beq .L_08049cd8
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	adds r6, r3, #0
	movs r3, #8
	str r3, [sp, #12]
	str r1, [sp, #4]
	mov r10, r2
.L_08049c42:
	adds r0, r5, #0
	bl Item_Get
	movs r0, #15
	bl Func_08041f70
	adds r1, r5, #0
	ldr r0, [sp, #80]
	bl Item_ClassifyUseAbility
	cmp r0, #0
	beq .L_08049c68
	movs r0, #4
	bl Func_08041f70
	b .L_08049c78
	.2byte 0x0000
.L_08049c64:
	.4byte 0x00000d46
.L_08049c68:
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r5
	cmp r3, #0
	beq .L_08049c78
	movs r0, #2
	bl Func_08041f70
.L_08049c78:
	movs r0, #128
	ldr r3, .L_08049cd4
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r5
	adds r0, r0, r3
	mov r1, r11
	lsls r3, r7, #4
	movs r2, #16
	bl UiText_DrawCharacterAtOffset
	movs r0, #15
	bl Func_08041f70
	ldr r3, [sp, #4]
	adds r0, r5, #0
	ldmia r3!, {r1}
	adds r7, #1
	adds r2, r3, #0
	str r2, [sp, #4]
	bl Resource_LoadKind26EntryToBuffer
	ldr r3, .L_08049ccc
	ldr r1, [sp, #12]
	mov r2, r10
	ands r0, r3
	ldrh r3, [r1, r2]
	ldr r2, .L_08049cd0
	ands r3, r2
	orrs r3, r0
	mov r2, r10
	strh r3, [r1, r2]
	adds r1, #12
	str r1, [sp, #12]
	cmp r7, #4
	bgt .L_08049cd8
	adds r6, #2
	ldrh r5, [r6]
	cmp r5, #0
	bne .L_08049c42
	b .L_08049cd8
	.2byte 0x0000
.L_08049ccc:
	.4byte 0x000003ff
.L_08049cd0:
	.4byte 0xfffffc00
.L_08049cd4:
	.4byte 0x0000025f
.L_08049cd8:
	mov r3, r9
	str r7, [sp, #48]
	str r3, [sp, #64]
.L_08049cde:
	ldr r1, [sp, #72]
	cmp r1, #5
	ble .L_08049d30
	movs r7, #0
	adds r1, #4
	mov r10, r1
	b .L_08049d22
.L_08049cec:
	movs r2, #243
	lsls r2, r2, #8
	adds r2, #1
	mov r0, r9
	movs r1, #5
	adds r6, r7, r2
	bl Math_Div
	cmp r7, r0
	bne .L_08049d08
	movs r3, #243
	lsls r3, r3, #8
	adds r3, #11
	adds r6, r7, r3
.L_08049d08:
	mov r1, r11
	ldrh r2, [r1, #8]
	movs r3, #0
	subs r2, r2, r5
	adds r2, r2, r7
	str r3, [sp, #0]
	subs r2, #2
	mov r0, r11
	adds r1, r6, #0
	subs r3, #1
	bl UiWindow_SetTilemapEntry
	adds r7, #1
.L_08049d22:
	mov r0, r10
	movs r1, #5
	bl Math_Div
	adds r5, r0, #0
	cmp r7, r5
	blt .L_08049cec
.L_08049d30:
	mov r2, r11
	movs r1, #12
	ldrsh r0, [r2, r1]
	movs r3, #14
	ldrsh r1, [r2, r3]
	ldr r2, [sp, #16]
	mov r3, r11
	adds r1, r1, r2
	ldrh r2, [r3, #8]
	movs r3, #14
	adds r1, #1
	subs r2, #2
	str r3, [sp, #0]
	adds r0, #1
	movs r3, #1
	bl Func_08046134
	ldr r1, [sp, #68]
	movs r3, #1
	movs r2, #0
	strb r3, [r1, #3]
	strb r2, [r1, #6]
.L_08049d5c:
	ldr r3, [sp, #72]
	cmp r3, #5
	ble .L_08049e10
	movs r7, #0
	adds r3, #4
	mov r10, r3
	b .L_08049db8
.L_08049d6a:
	ldr r3, .L_08049e9c
	movs r1, #243
	ldr r3, [r3]
	lsls r1, r1, #8
	movs r2, #15
	adds r1, #1
	ands r3, r2
	adds r6, r7, r1
	cmp r3, #11
	bhi .L_08049d92
	mov r0, r9
	movs r1, #5
	bl Math_Div
	cmp r7, r0
	bne .L_08049d92
	movs r2, #243
	lsls r2, r2, #8
	adds r2, #11
	adds r6, r7, r2
.L_08049d92:
	mov r3, r11
	movs r1, #5
	mov r0, r10
	ldrh r5, [r3, #8]
	bl Math_Div
	subs r5, r5, r0
	adds r5, r5, r7
	movs r1, #0
	subs r5, #2
	movs r3, #1
	str r1, [sp, #0]
	mov r0, r11
	adds r1, r6, #0
	adds r2, r5, #0
	negs r3, r3
	bl UiWindow_SetTilemapEntry
	adds r7, #1
.L_08049db8:
	mov r0, r10
	movs r1, #5
	bl Math_Div
	cmp r7, r0
	blt .L_08049d6a
	mov r3, r11
	ldrh r2, [r3, #8]
	movs r1, #0
	str r1, [sp, #0]
	movs r5, #1
	movs r1, #243
	negs r5, r5
	subs r2, r2, r0
	lsls r1, r1, #8
	mov r0, r11
	adds r3, r5, #0
	subs r2, #3
	adds r1, #52
	bl UiWindow_SetTilemapEntry
	mov r3, r11
	ldrh r2, [r3, #8]
	movs r1, #0
	str r1, [sp, #0]
	movs r1, #243
	lsls r1, r1, #8
	subs r2, #2
	mov r0, r11
	adds r1, #53
	adds r3, r5, #0
	bl UiWindow_SetTilemapEntry
	mov r1, r11
	movs r2, #14
	ldrsh r3, [r1, r2]
	ldr r1, [sp, #68]
	subs r3, #1
	lsrs r3, r3, #2
	movs r2, #2
	lsls r2, r3
	ldrb r3, [r1, #3]
	orrs r2, r3
	strb r2, [r1, #3]
.L_08049e10:
	ldr r2, [sp, #48]
	cmp r2, #0
	ble .L_08049e2a
	ldr r5, [sp, #28]
	adds r7, r2, #0
.L_08049e1a:
	adds r0, r5, #0
	movs r1, #240
	subs r7, #1
	bl Runtime_PushSlotEntry
	adds r5, #12
	cmp r7, #0
	bne .L_08049e1a
.L_08049e2a:
	mov r2, r11
	movs r1, #12
	ldrsh r3, [r2, r1]
	lsls r3, r3, #3
	subs r3, #2
	str r3, [sp, #36]
	movs r1, #14
	ldrsh r3, [r2, r1]
	ldr r2, [sp, #16]
	ldr r1, [sp, #20]
	adds r3, r2, r3
	lsls r3, r3, #3
	adds r3, #20
	str r3, [sp, #40]
	movs r3, #128
	lsls r3, r3, #23
	movs r2, #0
	str r3, [r1, #4]
	str r2, [r1, #8]
	ldr r0, [sp, #56]
	ldr r1, .L_08049ea0
	bl Resource_GetBuffer
	ldr r3, .L_08049e8c
	ldr r1, [sp, #20]
	ands r0, r3
	ldr r2, .L_08049e90
	ldrh r3, [r1, #8]
	ands r3, r2
	orrs r3, r0
	adds r2, r1, #0
	strh r3, [r2, #8]
	ldr r3, .L_08049e9c
	movs r1, #255
	ldr r0, [r3]
	movs r3, #4
	ands r0, r3
	ldr r3, [sp, #36]
	lsrs r2, r0, #1
	lsls r1, r1, #8
	adds r2, r3, r2
	adds r1, #252
	adds r2, r2, r1
	ldr r3, .L_08049e94
	ldr r1, [sp, #20]
	ands r2, r3
	ldrh r3, [r1, #6]
	ldr r1, .L_08049e98
	b .L_08049ea4
.L_08049e8c:
	.4byte 0x000003ff
.L_08049e90:
	.4byte 0xfffffc00
.L_08049e94:
	.4byte 0x000001ff
.L_08049e98:
	.4byte 0xfffffe00
.L_08049e9c:
	.4byte Data_0300122c
.L_08049ea0:
	.4byte Data_080597f8
.L_08049ea4:
	lsrs r0, r0, #2
	ands r3, r1
	orrs r3, r2
	ldr r2, [sp, #20]
	strh r3, [r2, #6]
	ldr r3, [sp, #40]
	subs r0, r3, r0
	adds r0, #248
	strb r0, [r2, #4]
	ldr r1, [sp, #72]
	cmp r1, #0
	beq .L_08049ec4
	ldr r0, [sp, #20]
	movs r1, #242
	bl Runtime_PushSlotEntry
.L_08049ec4:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r1, [r3]
	mov r2, r9
	mov r3, r8
	str r2, [r1, #52]
	str r3, [r1, #48]
	ldr r2, [sp, #44]
	str r2, [r1, #56]
	ldr r0, .L_0804a124
	movs r2, #1
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_08049f6a
	ldr r3, [sp, #72]
	cmp r3, #0
	beq .L_08049f62
	ldr r2, [sp, #76]
	mov r1, r9
	add r1, r8
	lsls r3, r1, #1
	adds r5, r3, r2
	ldrh r0, [r5]
	mov r10, r1
	movs r7, #128
	bl Item_Get
	ldrh r2, [r5]
	lsls r7, r7, #3
	adds r3, r7, #0
	ands r3, r2
	movs r6, #0
	cmp r3, #0
	bne .L_08049f1c
	ldrh r1, [r5]
	ldr r0, [sp, #80]
	bl Item_ClassifyUseAbility
	adds r6, r0, #0
	cmp r6, #0
	bne .L_08049f1c
	b .L_0804a0e0
.L_08049f1c:
	movs r0, #114
	bl Audio_PlayCue
	cmp r6, #2
	bne .L_08049f2c
	ldr r5, [sp, #32]
	ldr r0, .L_0804a128
	b .L_08049f3a
.L_08049f2c:
	ldrh r2, [r5]
	adds r3, r7, #0
	ands r3, r2
	cmp r3, #0
	beq .L_08049f44
	ldr r5, [sp, #32]
	ldr r0, .L_0804a12c
.L_08049f3a:
	adds r1, r5, #0
	movs r2, #32
	bl UiText_CopyMessageString
	b .L_08049f50
.L_08049f44:
	ldr r5, [sp, #32]
	ldr r0, .L_0804a130
	adds r1, r5, #0
	movs r2, #32
	bl UiText_CopyMessageString
.L_08049f50:
	bl Ui_FillVramBlockPattern
	adds r0, r5, #0
	ldr r1, [sp, #52]
	movs r2, #0
	movs r3, #4
	bl UiText_RenderWideStringAtOffset
	b .L_08049f88
.L_08049f62:
	movs r3, #1
	negs r3, r3
	mov r10, r3
	b .L_0804a0e0
.L_08049f6a:
	ldr r3, [r1, #76]
	cmp r3, #0
	beq .L_08049f7a
	ldr r3, [r0, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08049f88
.L_08049f7a:
	movs r0, #113
	bl Audio_PlayCue
	movs r1, #1
	negs r1, r1
	mov r10, r1
	b .L_0804a0e0
.L_08049f88:
	ldr r2, [sp, #72]
	cmp r2, #0
	bne .L_08049f90
	b .L_0804a0d8
.L_08049f90:
	ldr r1, .L_0804a124
	movs r2, #128
	ldr r3, [r1, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08049fc6
	movs r0, #111
	bl Audio_PlayCue
	movs r3, #1
	add r8, r3
	mov r1, r8
	cmp r1, #5
	beq .L_08049fb6
	ldr r2, [sp, #72]
	mov r3, r9
	add r3, r8
	cmp r3, r2
	bne .L_08049fba
.L_08049fb6:
	movs r3, #0
	mov r8, r3
.L_08049fba:
	mov r2, r8
	mov r1, r8
	lsls r2, r2, #1
	str r1, [sp, #44]
	str r2, [sp, #16]
	b .L_0804a0d8
.L_08049fc6:
	ldr r3, [r1, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_0804a010
	movs r0, #111
	bl Audio_PlayCue
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r1, r8
	cmp r1, #0
	bge .L_0804a004
	ldr r0, [sp, #72]
	movs r1, #5
	subs r0, #1
	bl Math_Div
	lsls r3, r0, #2
	adds r3, r3, r0
	cmp r9, r3
	bne .L_0804a000
	ldr r2, [sp, #72]
	mov r1, r9
	subs r3, r2, r1
	subs r3, #1
	mov r8, r3
	b .L_0804a004
.L_0804a000:
	movs r2, #4
	mov r8, r2
.L_0804a004:
	mov r1, r8
	mov r3, r8
	lsls r1, r1, #1
	str r3, [sp, #44]
	str r1, [sp, #16]
	b .L_0804a0d8
.L_0804a010:
	ldr r3, [r1, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_0804a06e
	movs r0, #111
	bl Audio_PlayCue
	bl Runtime_SetMainState19
	ldr r2, [sp, #72]
	mov r3, r9
	adds r3, #5
	cmp r3, r2
	blt .L_0804a042
	mov r3, r9
	cmp r3, #0
	beq .L_0804a0d8
	ldr r2, [sp, #44]
	movs r1, #0
	mov r8, r2
	mov r3, r8
	lsls r3, r3, #1
	mov r9, r1
	b .L_0804a0d6
.L_0804a042:
	ldr r1, [sp, #44]
	ldr r0, [sp, #72]
	mov r8, r1
	subs r0, #1
	movs r1, #5
	mov r9, r3
	bl Math_Div
	lsls r3, r0, #2
	adds r3, r3, r0
	cmp r9, r3
	bne .L_0804a0d2
	ldr r2, [sp, #72]
	mov r1, r9
	subs r3, r2, r1
	ldr r2, [sp, #44]
	subs r3, #1
	mov r8, r3
	cmp r8, r2
	ble .L_0804a0c2
	mov r8, r2
	b .L_0804a0d2
.L_0804a06e:
	ldr r3, [r1, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0804a0d8
	movs r0, #111
	bl Audio_PlayCue
	bl Runtime_SetMainState19
	mov r1, r9
	cmp r1, #0
	beq .L_0804a09a
	ldr r3, [sp, #44]
	movs r2, #5
	mov r8, r3
	mov r1, r8
	negs r2, r2
	lsls r1, r1, #1
	add r9, r2
	str r1, [sp, #16]
	b .L_0804a0d8
.L_0804a09a:
	ldr r0, [sp, #72]
	movs r1, #5
	subs r0, #1
	bl Math_Div
	ldr r2, [sp, #44]
	lsls r3, r0, #2
	adds r3, r3, r0
	mov r9, r3
	mov r8, r2
	cmp r3, #0
	beq .L_0804a0ca
	ldr r1, [sp, #72]
	subs r3, r1, r3
	subs r3, #1
	mov r8, r3
	cmp r8, r2
	ble .L_0804a0d2
	mov r8, r2
	b .L_0804a0cc
.L_0804a0c2:
	mov r1, r8
	lsls r1, r1, #1
	str r1, [sp, #16]
	b .L_0804a0d8
.L_0804a0ca:
	mov r2, r8
.L_0804a0cc:
	lsls r2, r2, #1
	str r2, [sp, #16]
	b .L_0804a0d8
.L_0804a0d2:
	mov r3, r8
	lsls r3, r3, #1
.L_0804a0d6:
	str r3, [sp, #16]
.L_0804a0d8:
	movs r0, #1
	bl WaitFrames
	b .L_08049b78
.L_0804a0e0:
	ldr r0, [sp, #52]
	movs r1, #1
	bl UiWork_Finalize
	movs r1, #1
	mov r0, r11
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #56]
	bl Resource_ResetEntry
	ldr r5, [sp, #24]
	movs r7, #4
.L_0804a100:
	ldmia r5!, {r0}
	subs r7, #1
	bl Resource_ResetEntry
	cmp r7, #0
	bge .L_0804a100
	movs r0, #1
	bl WaitFrames
	mov r0, r10
	add sp, #304
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0804a124:
	.4byte gInput
.L_0804a128:
	.4byte 0x00000d4f
.L_0804a12c:
	.4byte 0x00000d4d
.L_0804a130:
	.4byte 0x00000d4c
