.syntax unified
	.thumb
	.global Func_080f9644
	.thumb_func
Func_080f9644:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	str r1, [sp, #32]
	movs r1, #0
	str r0, [sp, #36]
	str r2, [sp, #28]
	str r1, [sp, #24]
	str r1, [sp, #20]
	str r1, [sp, #16]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r8, r1
	mov r9, r3
	bl .L_080fa280
.L_080f9672:
	mov r2, r8
	cmp r2, #12
	bls .L_080f967c
	bl .L_080fa27c
.L_080f967c:
	lsls r3, r2, #2
	ldr r2, .L_080f9a00
	ldr r3, [r3, r2]
	mov pc, r3
.L_080f9684:
	.4byte .L_080f96b8
	.4byte .L_080f9718
	.4byte .L_080f98c8
	.4byte .L_080fa0b0
	.4byte .L_080f9b96
	.4byte .L_080f9a18
	.4byte .L_080f99a4
	.4byte .L_080f9dd6
	.4byte .L_080fa280
	.4byte .L_080f9782
	.4byte .L_080fa228
	.4byte .L_080fa18a
	.4byte .L_080fa26c
.L_080f96b8:
	movs r2, #180
	lsls r2, r2, #1
	add r2, r9
	movs r3, #0
	strh r3, [r2]
	bl Func_080fbe48
	bl Func_080fbdbc
	movs r3, #134
	lsls r3, r3, #2
	add r3, r9
	ldr r2, [r3]
	movs r3, #13
	strb r3, [r2, #5]
	ldr r1, .L_080f9a04
	movs r0, #0
	bl Func_080facb4
	mov r3, r9
	ldr r0, [r3, #48]
	bl RenderOutput_RedrawSavedRectFar
	mov r1, r9
	ldr r0, [r1, #48]
	bl UiText_DrawWorkValueWithLabel
	movs r0, #0
	bl Func_080fa50c
	movs r3, #1
	adds r7, r0, #0
	negs r3, r3
	cmp r7, r3
	bne .L_080f9708
	movs r2, #0
	str r3, [sp, #16]
	movs r3, #1
	str r2, [sp, #20]
	str r3, [sp, #24]
.L_080f9708:
	mov r1, r9
	ldr r0, [r1, #48]
	bl RenderOutput_RedrawSavedRectFar
	bl ItemMenu_HideAllIcons
	bl .L_080fa276
.L_080f9718:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r0, [r3]
	bl Func_080fad1c
	movs r3, #0
	mov r8, r3
	cmp r0, #0
	bne .L_080f9732
	bl .L_080fa280
.L_080f9732:
	bl Func_080fbe48
	bl Func_080fbdbc
	movs r3, #134
	lsls r3, r3, #2
	add r3, r9
	ldr r2, [r3]
	movs r3, #13
	strb r3, [r2, #5]
	mov r1, r9
	ldr r2, [r1, #20]
	movs r3, #1
	strb r3, [r2, #5]
	ldr r1, .L_080f9a08
	movs r0, #0
	bl Func_080facb4
	movs r0, #0
	bl Func_080fc6bc
	movs r3, #1
	movs r2, #0
	negs r3, r3
	str r0, [sp, #20]
	mov r8, r2
	cmp r0, r3
	bne .L_080f976e
	bl .L_080fa280
.L_080f976e:
	movs r2, #177
	lsls r2, r2, #1
	adds r2, #255
	add r2, r9
	movs r3, #255
	movs r1, #9
	strb r3, [r2]
	mov r8, r1
	bl .L_080fa280
.L_080f9782:
	bl Func_080fb104
	movs r5, #1
	adds r7, r0, #0
	negs r5, r5
	cmp r7, r5
	bne .L_080f97a0
	movs r3, #128
	movs r2, #1
	lsls r3, r3, #2
	mov r8, r2
	adds r3, #30
	add r3, r9
	mov r1, r8
	strh r1, [r3]
.L_080f97a0:
	cmp r7, #0
	bne .L_080f9898
	movs r2, #182
	lsls r2, r2, #1
	add r2, r9
	ldrh r3, [r2]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	mov r10, r2
	bl BattleFx_HasTriggerFar
	cmp r0, #0
	beq .L_080f97ec
	movs r3, #1
	str r3, [sp, #24]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r3, [r3]
	ldr r1, [sp, #36]
	str r3, [r1]
	ldr r2, [sp, #32]
	mov r3, r10
	str r7, [r2]
	ldr r1, [sp, #28]
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r3, r2
	movs r2, #1
	str r3, [r1]
	str r2, [sp, #16]
	bl .L_080fa280
.L_080f97ec:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	mov r2, r10
	ldrb r0, [r3]
	ldrh r1, [r2]
	mov r11, r3
	bl Func_080fb638
	adds r6, r0, #0
	cmp r6, #1
	bne .L_080f980a
	movs r3, #2
	mov r8, r3
.L_080f980a:
	cmp r6, #2
	bne .L_080f9864
	bl Func_080fa2d0
	mov r1, r9
	ldr r0, [r1, #48]
	bl RenderOutput_ClearListFar
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #94
	add r3, r9
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r3, .L_080f9a0c
	adds r2, r5, #0
	adds r0, r0, r3
	movs r1, #0
	bl Func_080f8ce8
	mov r3, r9
	ldr r2, [r3, #20]
	movs r5, #226
	movs r3, #13
	mov r1, r11
	lsls r5, r5, #1
	strb r3, [r2, #5]
	ldrb r0, [r1]
	add r5, r9
	bl Owner_GetState
	movs r2, #0
	adds r1, r5, #0
	bl ItemMenu_Collect
	movs r3, #133
	lsls r3, r3, #2
	add r3, r9
	strb r0, [r3]
	movs r1, #0
	adds r0, r5, #0
	bl Func_080fadd0
	movs r2, #0
	mov r8, r2
.L_080f9864:
	adds r3, r6, #1
	cmp r3, #1
	bhi .L_080f9898
	movs r3, #1
	str r3, [sp, #24]
	mov r1, r11
	ldrb r3, [r1]
	ldr r2, [sp, #36]
	str r3, [r2]
	movs r3, #140
	lsls r3, r3, #1
	adds r3, #255
	add r3, r9
	ldrb r3, [r3]
	ldr r1, [sp, #32]
	str r3, [r1]
	mov r3, r10
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #1
	ldr r1, [sp, #28]
	adds r3, #255
	ands r3, r2
	movs r2, #1
	str r3, [r1]
	str r2, [sp, #16]
.L_080f9898:
	cmp r7, #1
	bne .L_080f98a0
	movs r3, #3
	mov r8, r3
.L_080f98a0:
	cmp r7, #3
	bne .L_080f98a8
	movs r1, #6
	mov r8, r1
.L_080f98a8:
	cmp r7, #5
	bne .L_080f98b0
	movs r2, #5
	mov r8, r2
.L_080f98b0:
	cmp r7, #4
	bne .L_080f98b8
	movs r3, #11
	mov r8, r3
.L_080f98b8:
	cmp r7, #2
	beq .L_080f98c0
	bl .L_080fa280
.L_080f98c0:
	movs r1, #10
	mov r8, r1
	bl .L_080fa280
.L_080f98c8:
	mov r5, r9
	adds r5, #240
	bl ItemMenu_HideAllIcons
	bl Func_080fbddc
	bl Func_080fbd9c
	ldr r0, [r5]
	bl RenderOutput_RedrawSavedRectFar
	bl Func_080fc12c
	ldr r1, [r5]
	ldr r0, .L_080f9a10
	movs r2, #16
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #0
	bl Func_080fa870
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	beq .L_080f99f6
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	movs r7, #0
	bl ItemMenu_IsSpecial
	cmp r0, #0
	beq .L_080f9918
	movs r7, #8
.L_080f9918:
	bl Func_080fa2d0
	movs r3, #140
	lsls r3, r3, #1
	adds r3, #255
	mov r2, r9
	add r3, r9
	adds r6, r0, #0
	ldrb r1, [r3]
	ldr r0, [r2, #40]
	adds r3, r7, #0
	movs r2, #0
	bl Func_080f8170
	cmp r6, r5
	beq .L_080f9968
	mov r3, r9
	ldr r0, [r3, #48]
	bl RenderOutput_ClearListFar
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #94
	add r3, r9
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldr r3, .L_080f9a0c
	movs r1, #0
	adds r0, r0, r3
	adds r2, r5, #0
	bl Func_080f8ce8
	mov r3, r9
	ldr r2, [r3, #20]
	movs r3, #13
	strb r3, [r2, #5]
	bl Func_080fb6d4
	movs r1, #1
	mov r8, r1
.L_080f9968:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	movs r5, #226
	add r3, r9
	lsls r5, r5, #1
	ldrb r0, [r3]
	add r5, r9
	bl Owner_GetState
	movs r2, #0
	adds r1, r5, #0
	bl ItemMenu_Collect
	movs r3, #133
	lsls r3, r3, #2
	add r3, r9
	strb r0, [r3]
	movs r1, #0
	adds r0, r5, #0
	bl Func_080fadd0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #30
	add r2, r9
	movs r3, #1
	strh r3, [r2]
	bl .L_080fa280
.L_080f99a4:
	mov r5, r9
	adds r5, #240
	bl Func_080fbddc
	bl Func_080fbd9c
	ldr r0, [r5]
	bl RenderOutput_RedrawSavedRectFar
	bl Func_080fc12c
	ldr r1, [r5]
	movs r3, #16
	ldr r0, .L_080f9a14
	movs r2, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #1
	bl Func_080fa870
	movs r1, #1
	movs r3, #4
	negs r1, r1
	mov r8, r3
	cmp r0, r1
	beq .L_080f99dc
	bl .L_080fa280
.L_080f99dc:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r3, [r3]
	movs r2, #180
	lsls r2, r2, #1
	add r2, r9
	ldrh r1, [r2]
	adds r0, r3, #0
	movs r2, #0
	bl Func_080fae8c
.L_080f99f6:
	movs r2, #9
	mov r8, r2
	bl .L_080fa280
	.2byte 0x0000
.L_080f9a00:
	.4byte .L_080f9684
.L_080f9a04:
	.4byte 0x00001007
.L_080f9a08:
	.4byte 0x00001008
.L_080f9a0c:
	.4byte 0x00001120
.L_080f9a10:
	.4byte 0x0000100a
.L_080f9a14:
	.4byte 0x0000100b
.L_080f9a18:
	movs r5, #182
	lsls r5, r5, #1
	add r5, r9
	bl ItemMenu_HideAllIcons
	ldrh r3, [r5]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	bl Item_Get
	ldrb r2, [r0, #3]
	movs r3, #16
	ands r3, r2
	movs r6, #0
	cmp r3, #0
	beq .L_080f9a56
	ldrh r3, [r5]
	lsrs r3, r3, #11
	adds r5, r3, #1
	cmp r5, #1
	ble .L_080f9a56
	bl Func_080fc12c
	movs r0, #0
	adds r1, r5, #0
	movs r2, #1
	bl Func_080fbe6c
	adds r6, r0, #0
.L_080f9a56:
	movs r1, #1
	movs r3, #9
	negs r1, r1
	mov r8, r3
	cmp r6, r1
	bne .L_080f9a66
	bl .L_080fa280
.L_080f9a66:
	movs r2, #140
	lsls r2, r2, #1
	adds r2, #255
	add r2, r9
	movs r3, #0
	strb r3, [r2]
	movs r3, #182
	lsls r3, r3, #1
	movs r2, #128
	add r3, r9
	lsls r2, r2, #1
	ldrh r3, [r3]
	adds r2, #255
	mov r8, r2
	movs r5, #134
	mov r1, r8
	lsls r5, r5, #2
	add r5, r9
	ands r1, r3
	lsls r3, r6, #11
	orrs r1, r3
	ldr r3, [r5]
	movs r0, #2
	ldrb r2, [r3, #14]
	movs r3, #0
	bl Func_08038288
	ldr r2, [r5]
	movs r3, #1
	strb r3, [r2, #5]
	movs r3, #120
	ldr r2, [r5]
	strh r3, [r2, #6]
	movs r3, #28
	ldr r2, [r5]
	strh r3, [r2, #8]
	ldr r0, [r5]
	bl UiIcon_PrepareObject
	mov r3, r9
	ldr r0, [r3, #56]
	movs r3, #96
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #72
	movs r3, #120
	bl UiWindow_ClearInteriorTilesFar
	mov r3, r9
	adds r3, #240
	ldr r0, [r3]
	bl RenderOutput_RedrawSavedRectFar
	ldr r0, [sp, #20]
	bl Func_080fc1ac
	cmp r0, #0
	bne .L_080f9b72
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r7, [r3]
	adds r0, r7, #0
	bl Owner_GetState
	adds r0, r6, #1
	cmp r0, #0
	ble .L_080f9b1a
	adds r5, r0, #0
	mov r6, r8
.L_080f9af4:
	movs r3, #180
	lsls r3, r3, #1
	add r3, r9
	ldrh r1, [r3]
	adds r0, r7, #0
	bl Inventory_RemoveFar
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	adds r0, r6, #0
	ands r0, r3
	movs r1, #1
	subs r5, #1
	bl Item_AdjustCounterFar
	cmp r5, #0
	bne .L_080f9af4
.L_080f9b1a:
	adds r0, r7, #0
	bl Owner_RefreshClassActionsFar
	adds r0, r7, #0
	bl Owner_RecalculateStatsFar
	bl Func_080fbdbc
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r0, [r3]
	movs r1, #0
	bl Func_080fae2c
	movs r3, #134
	lsls r3, r3, #2
	add r3, r9
	ldr r3, [r3]
	movs r2, #13
	strb r2, [r3, #5]
	mov r1, r9
	ldr r3, [r1, #20]
	movs r0, #1
	strb r2, [r3, #5]
	bl WaitFrames
	mov r2, r9
	ldr r0, [r2, #48]
	bl RenderOutput_ClearListFar
	movs r2, #13
	ldr r0, .L_080f9e58
	movs r1, #15
	bl Func_080f8ce8
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #30
	movs r3, #1
	add r2, r9
	strh r3, [r2]
	b .L_080f9b74
.L_080f9b72:
	movs r3, #9
.L_080f9b74:
	mov r8, r3
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r0, [r3]
	bl Owner_RecalculateStatsFar
	movs r3, #134
	lsls r3, r3, #2
	add r3, r9
	ldr r2, [r3]
	movs r3, #13
	strb r3, [r2, #5]
	bl Event_ClearInvalidPackedValuesFar
	b .L_080fa280
.L_080f9b96:
	movs r5, #182
	lsls r5, r5, #1
	add r5, r9
	movs r7, #128
	ldrh r3, [r5]
	lsls r7, r7, #1
	adds r7, #255
	adds r0, r7, #0
	movs r1, #0
	ands r0, r3
	mov r10, r1
	bl Item_Get
	ldrb r2, [r0, #3]
	movs r3, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080f9c86
	movs r6, #140
	ldrh r3, [r5]
	lsls r6, r6, #1
	adds r6, #255
	add r6, r9
	adds r1, r7, #0
	ldrb r0, [r6]
	ands r1, r3
	bl Func_080fad48
	adds r5, r0, #0
	cmp r5, #30
	bne .L_080f9bd8
	movs r2, #1
	mov r10, r2
.L_080f9bd8:
	ldrb r0, [r6]
	bl Func_080fad1c
	cmp r0, #15
	bne .L_080f9be6
	cmp r5, #0
	beq .L_080f9cae
.L_080f9be6:
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	mov r1, r10
	lsrs r3, r3, #11
	adds r3, #1
	cmp r1, #0
	bne .L_080f9cdc
	lsls r2, r3, #24
	asrs r1, r2, #24
	adds r3, r5, r1
	cmp r3, #30
	ble .L_080f9c06
	movs r3, #30
	subs r1, r3, r5
.L_080f9c06:
	movs r3, #128
	lsls r3, r3, #17
	cmp r2, r3
	ble .L_080f9c1a
	movs r0, #0
	movs r2, #0
	bl Func_080fbe6c
	adds r6, r0, #0
	b .L_080f9c1c
.L_080f9c1a:
	movs r6, #0
.L_080f9c1c:
	movs r1, #1
	negs r1, r1
	cmp r6, r1
	bne .L_080f9c26
	b .L_080f9df8
.L_080f9c26:
	movs r7, #0
	adds r6, #1
	cmp r7, r6
	bge .L_080f9cdc
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #255
	mov r11, r3
.L_080f9c36:
	movs r3, #140
	lsls r3, r3, #1
	adds r3, #255
	add r3, r9
	ldrb r0, [r3]
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	mov r1, r11
	ands r1, r3
	bl Inventory_AddItemFar
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	beq .L_080f9c7a
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r0, [r3]
	movs r3, #180
	lsls r3, r3, #1
	add r3, r9
	ldrh r1, [r3]
	bl Inventory_RemoveFar
	movs r3, #181
	lsls r3, r3, #1
	add r3, r9
	strh r5, [r3]
	b .L_080f9c7e
.L_080f9c7a:
	movs r2, #1
	mov r10, r2
.L_080f9c7e:
	adds r7, #1
	cmp r7, r6
	blt .L_080f9c36
	b .L_080f9cdc
.L_080f9c86:
	movs r3, #140
	lsls r3, r3, #1
	adds r3, #255
	add r3, r9
	ldrb r0, [r3]
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #255
	ands r1, r3
	bl Inventory_AddItemFar
	movs r5, #1
	adds r6, r0, #0
	negs r5, r5
	cmp r6, r5
	bne .L_080f9cb4
.L_080f9cae:
	movs r3, #7
	mov r8, r3
	b .L_080fa280
.L_080f9cb4:
	movs r3, #181
	lsls r3, r3, #1
	add r3, r9
	strh r6, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r0, [r3]
	movs r3, #180
	lsls r3, r3, #1
	add r3, r9
	ldrh r1, [r3]
	bl Inventory_RemoveFar
	adds r6, r0, #0
	cmp r6, r5
	bne .L_080f9cdc
	movs r1, #1
	mov r10, r1
.L_080f9cdc:
	movs r5, #128
	lsls r5, r5, #2
	movs r7, #140
	adds r5, #22
	lsls r7, r7, #1
	add r5, r9
	adds r7, #255
	add r7, r9
	ldrb r0, [r5]
	bl Owner_RefreshClassActionsFar
	ldrb r0, [r7]
	bl Owner_RefreshClassActionsFar
	ldrb r0, [r5]
	bl Owner_RecalculateStatsFar
	ldrb r0, [r7]
	bl Owner_RecalculateStatsFar
	mov r2, r10
	movs r6, #1
	cmp r2, #0
	bne .L_080f9d38
	ldrb r3, [r7]
	movs r2, #182
	strb r3, [r5]
	lsls r2, r2, #1
	add r2, r9
	ldrh r1, [r2]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r3, r1
	strh r3, [r2]
	mov r3, r9
	adds r3, #240
	ldr r0, [r3]
	bl RenderOutput_RedrawSavedRectFar
	bl Func_080fc12c
	movs r0, #0
	bl Func_080fc2e0
	adds r6, r0, #0
.L_080f9d38:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080f9d46
	b .L_080fa280
.L_080f9d46:
	movs r1, #1
	ldrb r0, [r7]
	bl Func_080fae2c
	mov r3, r9
	ldr r2, [r3, #20]
	movs r3, #13
	strb r3, [r2, #5]
	movs r0, #1
	bl WaitFrames
	mov r1, r10
	cmp r1, #1
	bne .L_080f9d6e
	mov r2, r9
	ldr r0, [r2, #48]
	bl RenderOutput_ClearListFar
	ldr r0, .L_080f9e5c
	b .L_080f9d7c
.L_080f9d6e:
	mov r3, r9
	ldr r0, [r3, #48]
	bl RenderOutput_ClearListFar
	cmp r6, #1
	bne .L_080f9d86
	ldr r0, .L_080f9e60
.L_080f9d7c:
	movs r1, #15
	movs r2, #14
	bl Func_080f8ce8
	b .L_080fa0a6
.L_080f9d86:
	ldrb r3, [r7]
	movs r2, #181
	lsls r2, r2, #1
	add r2, r9
	adds r0, r3, #0
	ldrh r1, [r2]
	movs r2, #0
	bl Func_080fae8c
	ldr r5, .L_080f9e64
	movs r2, #14
	adds r0, r5, #0
	movs r1, #15
	bl Func_080f8ce8
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r0, [r3]
	bl Item_Get
	ldrb r2, [r0, #3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080f9dbc
	b .L_080fa0a6
.L_080f9dbc:
	movs r0, #103
	bl Audio_PlayCue
	mov r1, r9
	ldr r0, [r1, #48]
	bl RenderOutput_ClearListFar
	adds r0, r5, #7
	movs r1, #14
	movs r2, #14
	bl Func_080f8ce8
	b .L_080fa0a6
.L_080f9dd6:
	movs r3, #0
	mov r10, r3
	bl Func_080fbe48
	bl Func_080fbdbc
	ldr r1, .L_080f9e68
	movs r0, #0
	bl Func_080facb4
	movs r0, #1
	bl Func_080fc6bc
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_080f9e04
.L_080f9df8:
	movs r2, #6
	mov r8, r2
	b .L_080fa280
.L_080f9dfe:
	movs r3, #1
	mov r10, r3
	b .L_080f9f04
.L_080f9e04:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r0, [r3]
	bl Owner_GetState
	movs r3, #140
	str r0, [sp, #12]
	lsls r3, r3, #1
	adds r3, #255
	add r3, r9
	ldrb r0, [r3]
	bl Owner_GetState
	movs r5, #166
	lsls r5, r5, #1
	str r0, [sp, #8]
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	mov r11, r0
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	adds r2, r5, #0
	ldr r1, [sp, #12]
	mov r8, r0
	ldr r6, .L_080f9e6c
	mov r0, r11
	mov lr, r6
	.2byte 0xf800
	adds r2, r5, #0
	mov r0, r8
	ldr r1, [sp, #8]
	mov lr, r6
	.2byte 0xf800
	adds r5, #202
	movs r7, #0
	add r5, r9
	b .L_080f9e76
	.2byte 0x0000
.L_080f9e58:
	.4byte 0x000010ae
.L_080f9e5c:
	.4byte 0x000010b6
.L_080f9e60:
	.4byte 0x000010b0
.L_080f9e64:
	.4byte 0x000010ad
.L_080f9e68:
	.4byte 0x0000100c
.L_080f9e6c:
	.4byte IwramCopyWords
.L_080f9e70:
	adds r3, r7, #1
	lsls r3, r3, #24
	lsrs r7, r3, #24
.L_080f9e76:
	cmp r7, #29
	bhi .L_080f9e9a
	movs r3, #180
	lsls r3, r3, #1
	add r3, r9
	ldrb r0, [r5]
	ldrh r1, [r3]
	bl Inventory_RemoveFar
	adds r6, r0, #0
	cmp r6, #2
	beq .L_080f9e9a
	movs r1, #1
	negs r1, r1
	cmp r6, r1
	bne .L_080f9e70
	movs r2, #1
	mov r10, r2
.L_080f9e9a:
	adds r3, r7, #1
	lsls r3, r3, #24
	lsrs r7, r3, #24
	movs r5, #0
.L_080f9ea2:
	movs r3, #183
	lsls r3, r3, #1
	add r3, r9
	ldrh r2, [r3]
	ldr r3, .L_080f9ecc
	ands r3, r2
	cmp r3, #0
	beq .L_080f9ed4
	ldr r0, .L_080f9ed0
	ands r0, r2
	bl Item_Get
	ldrb r2, [r0, #3]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080f9ed4
	movs r3, #1
	mov r10, r3
	b .L_080f9ed4
	.2byte 0x0000
.L_080f9ecc:
	.4byte 0x00000200
.L_080f9ed0:
	.4byte 0x000001ff
.L_080f9ed4:
	movs r3, #140
	lsls r3, r3, #1
	adds r3, #255
	add r3, r9
	ldrb r0, [r3]
	movs r3, #181
	lsls r3, r3, #1
	add r3, r9
	ldrh r1, [r3]
	bl Inventory_RemoveFar
	adds r6, r0, #0
	cmp r6, #2
	beq .L_080f9f04
	movs r1, #1
	negs r1, r1
	cmp r6, r1
	bne .L_080f9efa
	b .L_080f9dfe
.L_080f9efa:
	adds r3, r5, #1
	lsls r3, r3, #24
	lsrs r5, r3, #24
	cmp r5, #29
	bls .L_080f9ea2
.L_080f9f04:
	adds r3, r5, #1
	ldr r2, .L_080f9f10
	lsls r3, r3, #24
	lsrs r5, r3, #24
	b .L_080f9f24
	.2byte 0x0000
.L_080f9f10:
	.4byte 0x000005ff
.L_080f9f14:
	movs r3, #181
	lsls r3, r3, #1
	add r3, r9
	strh r6, [r3]
	adds r3, r7, #0
	adds r3, #255
	lsls r3, r3, #24
	lsrs r7, r3, #24
.L_080f9f24:
	cmp r7, #0
	beq .L_080f9f54
	movs r3, #140
	lsls r3, r3, #1
	adds r3, #255
	add r3, r9
	ldrb r0, [r3]
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	adds r1, r2, #0
	ands r1, r3
	str r2, [sp, #4]
	bl Inventory_AddItemFar
	movs r3, #1
	adds r6, r0, #0
	negs r3, r3
	ldr r2, [sp, #4]
	cmp r6, r3
	bne .L_080f9f14
	movs r1, #1
	mov r10, r1
.L_080f9f54:
	ldr r7, .L_080f9f58
	b .L_080f9f6c
.L_080f9f58:
	.4byte 0x000005ff
.L_080f9f5c:
	movs r3, #180
	lsls r3, r3, #1
	add r3, r9
	strh r6, [r3]
	adds r3, r5, #0
	adds r3, #255
	lsls r3, r3, #24
	lsrs r5, r3, #24
.L_080f9f6c:
	cmp r5, #0
	beq .L_080f9f98
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r0, [r3]
	movs r3, #183
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	adds r1, r7, #0
	ands r1, r3
	bl Inventory_AddItemFar
	movs r2, #1
	adds r6, r0, #0
	negs r2, r2
	cmp r6, r2
	bne .L_080f9f5c
	movs r3, #1
	mov r10, r3
.L_080f9f98:
	movs r0, #1
	bl WaitFrames
	mov r1, r10
	cmp r1, #1
	bne .L_080f9fcc
	movs r2, #166
	ldr r0, [sp, #12]
	ldr r5, .L_080fa2b8
	mov r1, r11
	lsls r2, r2, #1
	mov lr, r5
	.2byte 0xf800
	movs r2, #166
	mov r1, r8
	ldr r0, [sp, #8]
	lsls r2, r2, #1
	mov lr, r5
	.2byte 0xf800
	mov r2, r9
	ldr r0, [r2, #48]
	bl RenderOutput_ClearListFar
	ldr r0, .L_080fa2bc
	movs r1, #15
	b .L_080fa088
.L_080f9fcc:
	movs r5, #128
	lsls r5, r5, #2
	movs r7, #140
	adds r5, #22
	lsls r7, r7, #1
	add r5, r9
	adds r7, #255
	add r7, r9
	ldrb r0, [r5]
	bl Owner_RefreshClassActionsFar
	ldrb r0, [r7]
	bl Owner_RefreshClassActionsFar
	ldrb r0, [r5]
	bl Owner_RecalculateStatsFar
	ldrb r0, [r7]
	bl Owner_RecalculateStatsFar
	bl Func_080fbddc
	mov r3, r9
	adds r3, #240
	ldr r0, [r3]
	bl RenderOutput_RedrawSavedRectFar
	ldrb r3, [r7]
	strb r3, [r5]
	movs r5, #182
	lsls r5, r5, #1
	add r5, r9
	ldrh r2, [r5]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r3, r2
	strh r3, [r5]
	bl Func_080fc12c
	movs r0, #0
	bl Func_080fc2e0
	adds r6, r0, #0
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fa09a
	mov r3, r9
	ldr r0, [r3, #48]
	bl RenderOutput_ClearListFar
	bl Func_080fbd9c
	ldrb r0, [r7]
	movs r1, #1
	bl Func_080fae2c
	cmp r6, #0
	bne .L_080fa090
	ldrb r3, [r7]
	movs r2, #181
	lsls r2, r2, #1
	add r2, r9
	adds r0, r3, #0
	ldrh r1, [r2]
	movs r2, #0
	bl Func_080fae8c
	ldr r6, .L_080fa2c0
	movs r2, #14
	adds r0, r6, #0
	movs r1, #15
	bl Func_080f8ce8
	ldrh r0, [r5]
	bl Item_Get
	ldrb r2, [r0, #3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080fa09a
	movs r0, #103
	bl Audio_PlayCue
	mov r1, r9
	ldr r0, [r1, #48]
	bl RenderOutput_ClearListFar
	adds r0, r6, #7
	movs r1, #14
.L_080fa088:
	movs r2, #14
	bl Func_080f8ce8
	b .L_080fa09a
.L_080fa090:
	ldr r0, .L_080fa2c4
	movs r1, #15
	movs r2, #14
	bl Func_080f8ce8
.L_080fa09a:
	mov r0, r8
	bl Sys_Free
	mov r0, r11
	bl Sys_Free
.L_080fa0a6:
	bl Event_ClearInvalidPackedValuesFar
	movs r2, #0
	mov r8, r2
	b .L_080fa280
.L_080fa0b0:
	movs r7, #128
	lsls r7, r7, #2
	movs r3, #180
	adds r7, #22
	lsls r3, r3, #1
	add r3, r9
	add r7, r9
	ldrh r1, [r3]
	ldrb r0, [r7]
	mov r10, r3
	bl Func_080ad048
	movs r5, #1
	movs r1, #1
	adds r6, r0, #0
	negs r5, r5
	mov r8, r1
	cmp r6, r5
	bne .L_080fa0d8
	b .L_080fa280
.L_080fa0d8:
	movs r2, #2
	negs r2, r2
	cmp r6, r2
	bne .L_080fa0f8
	mov r3, r9
	ldr r0, [r3, #48]
	bl RenderOutput_ClearListFar
	movs r1, #0
	ldr r0, .L_080fa2c8
	adds r2, r5, #0
	bl Func_080f8ce8
	movs r1, #1
	mov r8, r1
	b .L_080fa280
.L_080fa0f8:
	ldrb r0, [r7]
	bl Owner_RefreshClassActionsFar
	ldrb r0, [r7]
	bl Owner_RecalculateStatsFar
	mov r3, r9
	ldr r2, [r3, #20]
	movs r5, #226
	movs r3, #13
	lsls r5, r5, #1
	strb r3, [r2, #5]
	add r5, r9
	ldrb r0, [r7]
	bl Owner_GetState
	movs r2, #0
	adds r1, r5, #0
	bl ItemMenu_Collect
	movs r3, #133
	lsls r3, r3, #2
	add r3, r9
	strb r0, [r3]
	movs r1, #0
	adds r0, r5, #0
	bl Func_080fadd0
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r7]
	mov r2, r10
	ldrh r1, [r2]
	adds r0, r3, #0
	movs r2, #0
	bl Func_080fae8c
	mov r3, r9
	ldr r0, [r3, #48]
	bl RenderOutput_ClearListFar
	ldr r5, .L_080fa2c0
	movs r2, #8
	adds r0, r5, #0
	movs r1, #15
	bl Func_080f8ce8
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r0, [r3]
	bl Item_Get
	ldrb r2, [r0, #3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080fa170
	b .L_080fa276
.L_080fa170:
	movs r0, #103
	bl Audio_PlayCue
	mov r1, r9
	ldr r0, [r1, #48]
	bl RenderOutput_ClearListFar
	adds r0, r5, #7
	movs r1, #14
	movs r2, #8
	bl Func_080f8ce8
	b .L_080fa276
.L_080fa18a:
	movs r6, #128
	lsls r6, r6, #2
	adds r6, #22
	add r6, r9
	ldrb r0, [r6]
	bl Owner_GetState
	movs r3, #180
	lsls r3, r3, #1
	add r3, r9
	ldrh r2, [r3]
	mov r8, r3
	lsls r2, r2, #1
	adds r2, #216
	ldrh r1, [r0, r2]
	movs r3, #253
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r1
	strh r3, [r0, r2]
	ldrb r0, [r6]
	bl Owner_RefreshClassActionsFar
	ldrb r0, [r6]
	bl Owner_RecalculateStatsFar
	mov r1, r9
	ldr r2, [r1, #20]
	movs r3, #0
	movs r5, #226
	mov r10, r3
	lsls r5, r5, #1
	movs r3, #13
	strb r3, [r2, #5]
	add r5, r9
	ldrb r0, [r6]
	bl Owner_GetState
	movs r2, #0
	adds r1, r5, #0
	bl ItemMenu_Collect
	movs r3, #133
	lsls r3, r3, #2
	add r3, r9
	strb r0, [r3]
	movs r1, #0
	adds r0, r5, #0
	movs r5, #152
	bl Func_080fadd0
	lsls r5, r5, #2
	movs r0, #1
	bl WaitFrames
	add r5, r9
	movs r3, #1
	strb r3, [r5]
	mov r2, r8
	ldrb r3, [r6]
	ldrh r1, [r2]
	adds r0, r3, #0
	movs r2, #0
	bl Func_080fae8c
	mov r3, r10
	strb r3, [r5]
	mov r1, r9
	ldr r0, [r1, #48]
	bl RenderOutput_ClearListFar
	movs r2, #8
	ldr r0, .L_080fa2cc
	movs r1, #14
	bl Func_080f8ce8
	bl Event_ClearInvalidPackedValuesFar
	b .L_080fa276
.L_080fa228:
	mov r3, r9
	ldr r2, [r3, #20]
	movs r3, #13
	strb r3, [r2, #5]
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r0, [r3]
	bl Func_080fb780
	mov r1, r9
	ldr r0, [r1, #40]
	bl RenderOutput_RedrawSavedRectFar
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r3, [r3]
	movs r2, #180
	lsls r2, r2, #1
	add r2, r9
	ldrh r1, [r2]
	adds r0, r3, #0
	movs r2, #0
	bl Func_080fae8c
	mov r3, r9
	ldr r2, [r3, #20]
	movs r1, #9
	movs r3, #1
	strb r3, [r2, #5]
	mov r8, r1
	b .L_080fa280
.L_080fa26c:
	movs r2, #0
	movs r0, #0
	movs r1, #30
	bl Func_080fbe6c
.L_080fa276:
	movs r2, #1
	mov r8, r2
	b .L_080fa280
.L_080fa27c:
	movs r3, #1
	str r3, [sp, #24]
.L_080fa280:
	ldr r1, [sp, #24]
	cmp r1, #0
	bne .L_080fa296
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fa296
	bl .L_080f9672
.L_080fa296:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080fa2a8
	movs r2, #1
	negs r2, r2
	str r2, [sp, #16]
.L_080fa2a8:
	ldr r0, [sp, #16]
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080fa2b8:
	.4byte IwramCopyWords
.L_080fa2bc:
	.4byte 0x000010b5
.L_080fa2c0:
	.4byte 0x000010ad
.L_080fa2c4:
	.4byte 0x000010b2
.L_080fa2c8:
	.4byte 0x000010b3
.L_080fa2cc:
	.4byte 0x000010b1
