.syntax unified
	.thumb
	.global Func_0812381c
	.thumb_func
Func_0812381c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	add r3, sp, #36
	add r2, sp, #28
	str r1, [r3]
	mov r11, r3
	adds r3, r0, #0
	str r0, [r2]
	movs r4, #0
	ldrsh r0, [r3, r4]
	mov r10, r2
	bl Owner_GetState
	mov r1, sp
	mov r2, sp
	adds r1, #20
	adds r2, #24
	str r1, [sp, #12]
	str r0, [r1]
	str r2, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	mov r4, r10
	str r3, [r2]
	ldr r3, [r4]
	movs r6, #4
	movs r1, #10
	ldrsh r0, [r3, r1]
	bl Battle_GetTaggedSlotValue
	add r5, sp, #32
	str r0, [r5]
	bl Func_081234a4
	mov r2, r11
	mov r4, r10
	ldr r3, [r2]
	ldr r2, [r4]
	movs r1, #0
	ldrh r2, [r2]
	str r1, [r3, #100]
	str r1, [r3, #96]
	strb r1, [r3, #1]
	str r1, [r3, #88]
	str r1, [r3, #92]
	strb r2, [r3]
	strb r2, [r3, #2]
	str r6, [r3, #80]
	bl UiWork_ClearValueNameTablesFar
	ldr r0, [sp, #12]
	ldr r3, [r0]
	movs r1, #56
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0812389e
	bl .L_0812417c
.L_0812389e:
	ldr r3, .L_08123be4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08123918
	movs r0, #110
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08123918
	ldr r1, .L_08123be8
	movs r2, #128
	ldr r3, [r1]
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08123918
	ldr r3, [r1]
	movs r7, #1
	ands r3, r6
	cmp r3, #0
	beq .L_081238cc
	movs r7, #0
.L_081238cc:
	movs r6, #100
	b .L_081238f4
.L_081238d0:
	cmp r5, #254
	beq .L_081238f2
	movs r1, #192
	adds r0, r5, #0
	lsls r1, r1, #24
	bl Owner_AdjustFirstValueFar
	cmp r0, #0
	bne .L_081238f2
	movs r0, #8
	adds r1, r5, #0
	bl BattleEv_Push
	movs r0, #9
	adds r1, r5, #0
	bl BattleEv_Push
.L_081238f2:
	adds r6, #2
.L_081238f4:
	cmp r7, #0
	beq .L_08123902
	ldr r2, [sp, #8]
	ldr r3, [r2]
	adds r3, #2
	ldrsh r5, [r3, r6]
	b .L_0812390c
.L_08123902:
	ldr r0, [sp, #8]
	adds r3, r6, #0
	ldr r2, [r0]
	subs r3, #12
	ldrsh r5, [r2, r3]
.L_0812390c:
	cmp r5, #255
	bne .L_081238d0
	bl BattleEv_DispatchQueued
	bl .L_0812417c
.L_08123918:
	bl UiWork_ClearValueNameTablesFar
	mov r2, r10
	ldr r0, [r2]
	movs r3, #6
	ldrsh r4, [r0, r3]
	cmp r4, #9
	beq .L_081239c8
	ldr r1, [sp, #12]
	movs r3, #70
	ldr r2, [r1]
	adds r3, #255
	adds r1, r2, r3
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_08123950
	movs r3, #0
	strb r3, [r1]
	movs r4, #0
	ldrsh r0, [r0, r4]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08123bec
	bl UiText_ShowMessageAndWaitCoreFar
	bl .L_081241aa
.L_08123950:
	movs r1, #158
	lsls r1, r1, #1
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08123970
	movs r2, #0
	ldrsh r0, [r0, r2]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08123bf0
	bl UiText_ShowMessageAndWaitCoreFar
	bl .L_081241aa
.L_08123970:
	movs r1, #60
	adds r1, #255
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08123990
	movs r2, #0
	ldrsh r0, [r0, r2]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08123bf4
	bl UiText_ShowMessageAndWaitCoreFar
	bl .L_081241aa
.L_08123990:
	movs r0, #152
	lsls r0, r0, #1
	adds r3, r2, r0
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_081239c8
	cmp r4, #3
	beq .L_081239c8
	bl BattleRandom16Far
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .L_081239c8
	mov r1, r10
	ldr r3, [r1]
	movs r1, #1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl UiText_DrawQuantity
	ldr r0, .L_08123bf8
	bl UiText_ShowMessageAndWaitCoreFar
	bl .L_081241aa
.L_081239c8:
	mov r4, r10
	ldr r3, [r4]
	movs r0, #6
	ldrsh r3, [r3, r0]
	cmp r3, #8
	bne .L_081239d6
	b .L_0812417c
.L_081239d6:
	mov r1, r11
	ldr r3, [r1]
	movs r7, #1
	movs r1, #0
	adds r3, #45
	movs r2, #13
.L_081239e2:
	subs r2, #1
	strb r1, [r3]
	adds r3, #1
	cmp r2, #0
	bge .L_081239e2
	mov r2, r11
	ldr r3, [r2]
	movs r4, #1
	negs r4, r4
	adds r1, r4, #0
	adds r3, #59
	movs r2, #13
.L_081239fa:
	subs r2, #1
	strb r1, [r3]
	adds r3, #1
	cmp r2, #0
	bge .L_081239fa
	mov r0, r10
	ldr r3, [r0]
	movs r1, #6
	ldrsh r3, [r3, r1]
	cmp r3, #99
	bls .L_08123a14
	bl .L_08124376
.L_08123a14:
	ldr r2, .L_08123bfc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08123a1c:
	.4byte .L_08123c08
	.4byte .L_08123ca8
	.4byte .L_08123d96
	.4byte .L_08123e7a
	.4byte .L_08123e90
	.4byte .L_08124028
	.4byte .L_0812420a
	.4byte .L_08123e7a
	.4byte .L_0812417c
	.4byte .L_08123c68
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08124376
	.4byte .L_08123bac
.L_08123bac:
	mov r2, r10
	ldr r3, [r2]
	ldrh r3, [r3]
	lsls r0, r3, #16
	movs r3, #224
	lsls r3, r3, #11
	cmp r0, r3
	bhi .L_08123bc4
	ldr r0, .L_08123c00
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_08123bd2
.L_08123bc4:
	asrs r0, r0, #16
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08123c04
	bl UiText_ShowMessageAndWaitCoreFar
.L_08123bd2:
	bl BattlePresentation_WaitForAdvance
	mov r4, r11
	ldr r2, [r4]
	movs r3, #7
	str r3, [r2, #84]
	bl .L_081247ea
	.2byte 0x0000
.L_08123be4:
	.4byte Data_03001238
.L_08123be8:
	.4byte gInput
.L_08123bec:
	.4byte 0x00000cdc
.L_08123bf0:
	.4byte 0x00000caf
.L_08123bf4:
	.4byte 0x00000cae
.L_08123bf8:
	.4byte 0x00000cb0
.L_08123bfc:
	.4byte .L_08123a1c
.L_08123c00:
	.4byte 0x00000c98
.L_08123c04:
	.4byte 0x00000c9b
.L_08123c08:
	ldr r1, [sp, #12]
	ldr r0, [r1]
	bl Func_080ad108
	add r2, sp, #40
	mov r9, r2
	adds r7, r0, #0
	bl Func_08123648
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_08123c26
	bl .L_081247ec
.L_08123c26:
	cmp r7, #1
	bne .L_08123c2c
	b .L_0812437c
.L_08123c2c:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r2, [sp, #12]
	movs r1, #1
	ldr r0, [r2]
	bl Inventory_GetEquippedItemFar
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r5, .L_08123f60
	adds r0, r5, #0
	bl UiText_ShowMessageAndWaitCoreFar
	adds r5, #1
	bl Func_08120158
	adds r0, r7, #0
	movs r1, #4
	bl UiText_DrawQuantity
	adds r0, r5, #0
.L_08123c62:
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_08124376
.L_08123c68:
	ldr r4, [sp, #12]
	movs r0, #165
	ldr r3, [r4]
	lsls r0, r0, #1
	adds r3, r3, r0
	ldrh r3, [r3]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #155
	adds r3, r3, r1
	movs r2, #128
	movs r7, #44
	lsls r3, r3, #16
	lsls r2, r2, #10
	adds r7, #255
	cmp r3, r2
	bhi .L_08123c8c
	movs r7, #1
.L_08123c8c:
	ldr r3, [r5]
	cmp r3, #255
	bne .L_08123c94
	b .L_0812417c
.L_08123c94:
	mov r3, r11
	ldr r2, [r3]
	movs r1, #1
	movs r3, #0
	strb r1, [r2, #1]
	strb r3, [r2, #17]
	ldr r3, [r5]
	strb r1, [r2, #31]
	strb r3, [r2, #3]
	b .L_08124376
.L_08123ca8:
	mov r4, r10
	ldr r3, [r4]
	movs r5, #1
	movs r0, #8
	ldrsh r7, [r3, r0]
	adds r0, r7, #0
	bl BattleAction_Get
	add r1, sp, #40
	adds r6, r0, #0
	mov r9, r1
	adds r0, r7, #0
	bl Func_08123648
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_08123cd0
	bl .L_081247ec
.L_08123cd0:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	adds r0, r7, #0
	movs r1, #4
	bl UiText_DrawQuantity
	movs r2, #167
	lsls r2, r2, #1
	adds r2, #255
	cmp r7, r2
	ble .L_08123d1a
	movs r3, #171
	lsls r3, r3, #1
	adds r3, #255
	cmp r7, r3
	ble .L_08123d12
	movs r4, #157
	lsls r4, r4, #2
	cmp r7, r4
	bge .L_08123d1a
	movs r0, #150
	lsls r0, r0, #2
	cmp r7, r0
	blt .L_08123d1a
	ldr r0, .L_08123f64
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_08123d20
.L_08123d12:
	ldr r0, .L_08123f68
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_08123d20
.L_08123d1a:
	ldr r0, .L_08123f6c
	bl UiText_ShowMessageAndWaitCoreFar
.L_08123d20:
	ldr r2, [sp, #12]
	ldr r1, [r2]
	movs r3, #58
	ldrsh r2, [r1, r3]
	ldrb r3, [r6, #9]
	cmp r2, r3
	bge .L_08123d38
	mov r4, r11
	ldr r2, [r4]
	movs r3, #2
	str r3, [r2, #92]
	movs r5, #0
.L_08123d38:
	movs r0, #62
	adds r0, #255
	adds r3, r1, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08123d4e
	mov r1, r11
	ldr r2, [r1]
	movs r3, #1
	str r3, [r2, #92]
	movs r5, #0
.L_08123d4e:
	cmp r5, #0
	bne .L_08123d54
	b .L_08124376
.L_08123d54:
	mov r2, r11
	ldr r3, [r2]
	movs r5, #0
	str r5, [r3, #92]
	ldr r3, [sp, #12]
	ldrb r2, [r6, #9]
	ldr r1, [r3]
	mov r4, r10
	ldrh r3, [r1, #58]
	subs r3, r3, r2
	strh r3, [r1, #58]
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Owner_RecalculateRatiosFar
	ldr r2, [sp, #12]
	ldr r1, [r2]
	movs r4, #58
	ldrsh r3, [r1, r4]
	cmp r3, #0
	bge .L_08123d82
	strh r5, [r1, #58]
.L_08123d82:
	movs r0, #58
	ldrsh r2, [r1, r0]
	movs r4, #54
	ldrsh r3, [r1, r4]
	ldrh r0, [r1, #54]
	cmp r2, r3
	bgt .L_08123d92
	b .L_08124376
.L_08123d92:
	strh r0, [r1, #58]
	b .L_08124376
.L_08123d96:
	mov r0, r10
	ldr r3, [r0]
	movs r1, #8
	ldrsh r2, [r3, r1]
	cmp r2, #0
	bge .L_08123db4
	movs r2, #0
	ldrsh r0, [r3, r2]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08123f70
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_081241aa
.L_08123db4:
	ldr r4, [sp, #12]
	lsls r2, r2, #1
	ldr r3, [r4]
	adds r2, #216
	ldrh r0, [r3, r2]
	bl Item_Get
	adds r5, r0, #0
	ldrh r7, [r5, #40]
	cmp r7, #0
	beq .L_08123de8
	mov r3, r10
	ldr r1, [r3]
	ldr r0, [sp, #12]
	movs r4, #8
	ldrsh r3, [r1, r4]
	ldr r2, [r0]
	lsls r3, r3, #1
	adds r3, #216
	ldrh r2, [r2, r3]
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_08123e14
	b .L_08123dec
.L_08123de8:
	mov r0, r10
	ldr r1, [r0]
.L_08123dec:
	movs r2, #0
	ldrsh r0, [r1, r2]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08123f74
	bl UiText_ShowMessageAndWaitCoreFar
	ldr r4, [sp, #12]
	movs r0, #44
	ldr r3, [r4]
	adds r0, #255
	adds r2, r3, r0
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08123e0e
	b .L_081241aa
.L_08123e0e:
	movs r3, #1
	strb r3, [r2]
	b .L_081241aa
.L_08123e14:
	add r1, sp, #40
	mov r9, r1
	adds r0, r7, #0
	bl Func_08123648
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_08123e2a
	bl .L_081247ec
.L_08123e2a:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r3, [sp, #12]
	mov r4, r10
	ldr r2, [r3]
	ldr r3, [r4]
	movs r1, #2
	movs r0, #8
	ldrsh r3, [r3, r0]
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r2, r3]
	bl UiText_DrawQuantity
	ldrb r3, [r5, #12]
	cmp r3, #2
	beq .L_08123e5a
	cmp r3, #0
	bne .L_08123e76
.L_08123e5a:
	ldrb r0, [r5, #2]
	cmp r0, #3
	beq .L_08123e72
	cmp r0, #3
	bgt .L_08123e6a
	cmp r0, #1
	beq .L_08123e72
	b .L_08123e76
.L_08123e6a:
	cmp r0, #8
	bgt .L_08123e76
	cmp r0, #6
	blt .L_08123e76
.L_08123e72:
	ldr r0, .L_08123f78
	b .L_08123c62
.L_08123e76:
	ldr r0, .L_08123f7c
	b .L_08123c62
.L_08123e7a:
	mov r1, r10
	ldr r3, [r1]
	movs r1, #1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl UiText_DrawQuantity
	ldr r0, .L_08123f74
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_081241aa
.L_08123e90:
	mov r4, r10
	ldr r3, [r4]
	add r1, sp, #40
	movs r0, #8
	ldrsh r7, [r3, r0]
	mov r9, r1
	adds r0, r7, #0
	bl Func_08123648
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_08123eae
	bl .L_081247ec
.L_08123eae:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	adds r0, r7, #0
	movs r1, #4
	bl UiText_DrawQuantity
	adds r0, r7, #0
	bl BattleAction_Get
	ldrb r2, [r0, #1]
	movs r3, #15
	ands r3, r2
	cmp r3, #6
	bne .L_08123ed8
	ldr r0, .L_08123f80
	b .L_08123eda
.L_08123ed8:
	ldr r0, .L_08123f84
.L_08123eda:
	movs r3, #246
	lsls r3, r3, #1
	cmp r7, r3
	beq .L_08123fd6
	cmp r7, r3
	bgt .L_08123f30
	movs r2, #186
	adds r2, #255
	cmp r7, r2
	bgt .L_08123f0c
	subs r3, #56
	cmp r7, r3
	bgt .L_08123fca
	cmp r7, #224
	beq .L_08123fb6
	cmp r7, #224
	bge .L_08123efe
	b .L_08123c62
.L_08123efe:
	movs r4, #217
	lsls r4, r4, #1
	cmp r7, r4
	bgt .L_08123f08
	b .L_08123c62
.L_08123f08:
	ldr r0, .L_08123f88
	b .L_08123c62
.L_08123f0c:
	movs r3, #236
	lsls r3, r3, #1
	cmp r7, r3
	beq .L_08123fce
	cmp r7, r3
	bgt .L_08123f26
	movs r1, #222
	lsls r1, r1, #1
	cmp r7, r1
	ble .L_08123f22
	b .L_08123c62
.L_08123f22:
	ldr r0, .L_08123f84
	b .L_08123c62
.L_08123f26:
	movs r2, #244
	lsls r2, r2, #1
	cmp r7, r2
	beq .L_08123fd2
	b .L_08123c62
.L_08123f30:
	movs r3, #246
	adds r3, #255
	cmp r7, r3
	beq .L_08123fbe
	cmp r7, r3
	bgt .L_08123f8c
	subs r3, #6
	cmp r7, r3
	beq .L_08123fda
	cmp r7, r3
	bgt .L_08123f4e
	subs r3, #1
	cmp r7, r3
	beq .L_08123fc6
	b .L_08123c62
.L_08123f4e:
	movs r4, #244
	adds r4, #255
	cmp r7, r4
	beq .L_08123fc2
	movs r1, #250
	lsls r1, r1, #1
	cmp r7, r1
	beq .L_08123fba
	b .L_08123c62
.L_08123f60:
	.4byte 0x00000c65
.L_08123f64:
	.4byte 0x00000c8b
.L_08123f68:
	.4byte 0x00000c8c
.L_08123f6c:
	.4byte 0x00000c8a
.L_08123f70:
	.4byte 0x00000c67
.L_08123f74:
	.4byte 0x00000c62
.L_08123f78:
	.4byte 0x00000c64
.L_08123f7c:
	.4byte 0x00000c63
.L_08123f80:
	.4byte 0x00000d52
.L_08123f84:
	.4byte 0x00000d51
.L_08123f88:
	.4byte 0x00000d53
.L_08123f8c:
	movs r3, #252
	lsls r3, r3, #1
	cmp r7, r3
	beq .L_08123fe2
	cmp r7, r3
	bgt .L_08123fa2
	movs r2, #248
	adds r2, #255
	cmp r7, r2
	beq .L_08123fde
	b .L_08123c62
.L_08123fa2:
	movs r3, #254
	lsls r3, r3, #1
	cmp r7, r3
	beq .L_08123fe6
	movs r4, #239
	lsls r4, r4, #1
	adds r4, #255
	cmp r7, r4
	beq .L_08123fea
	b .L_08123c62
.L_08123fb6:
	ldr r0, .L_08123ff0
	b .L_08123c62
.L_08123fba:
	ldr r0, .L_08123ff4
	b .L_08123c62
.L_08123fbe:
	ldr r0, .L_08123ff8
	b .L_08123c62
.L_08123fc2:
	ldr r0, .L_08123ffc
	b .L_08123c62
.L_08123fc6:
	ldr r0, .L_08124000
	b .L_08123c62
.L_08123fca:
	ldr r0, .L_08124004
	b .L_08123c62
.L_08123fce:
	ldr r0, .L_08124008
	b .L_08123c62
.L_08123fd2:
	ldr r0, .L_0812400c
	b .L_08123c62
.L_08123fd6:
	ldr r0, .L_08124010
	b .L_08123c62
.L_08123fda:
	ldr r0, .L_08124014
	b .L_08123c62
.L_08123fde:
	ldr r0, .L_08124018
	b .L_08123c62
.L_08123fe2:
	ldr r0, .L_0812401c
	b .L_08123c62
.L_08123fe6:
	ldr r0, .L_08124020
	b .L_08123c62
.L_08123fea:
	ldr r0, .L_08124024
	b .L_08123c62
	.2byte 0x0000
.L_08123ff0:
	.4byte 0x00000c8a
.L_08123ff4:
	.4byte 0x00000d58
.L_08123ff8:
	.4byte 0x00000d59
.L_08123ffc:
	.4byte 0x00000d5a
.L_08124000:
	.4byte 0x00000d5b
.L_08124004:
	.4byte 0x00000d5c
.L_08124008:
	.4byte 0x00000d5d
.L_0812400c:
	.4byte 0x00000d5e
.L_08124010:
	.4byte 0x00000d60
.L_08124014:
	.4byte 0x00000d5f
.L_08124018:
	.4byte 0x00000d61
.L_0812401c:
	.4byte 0x00000d62
.L_08124020:
	.4byte 0x00000d63
.L_08124024:
	.4byte 0x00000d65
.L_08124028:
	mov r0, r10
	ldr r3, [r0]
	ldr r6, .L_08124064
	ldrh r3, [r3, #8]
	movs r5, #255
	lsls r0, r3, #16
	asrs r0, r0, #24
	adds r1, r5, #0
	ands r1, r3
	ands r0, r6
	bl Djinn_GetDefinitionHeaderFar
	mov r1, r10
	ldr r3, [r1]
	adds r7, r0, #0
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldrh r3, [r3, #8]
	adds r2, r5, #0
	lsls r1, r3, #16
	asrs r1, r1, #24
	ands r1, r6
	ands r2, r3
	bl Djinn_IsActiveFar
	cmp r0, #0
	beq .L_08124060
	b .L_081241b0
.L_08124060:
	b .L_08124068
	.2byte 0x0000
.L_08124064:
	.4byte 0x0000000f
.L_08124068:
	mov r4, r10
	ldr r3, [r4]
	adds r2, r5, #0
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	ands r1, r6
	ands r2, r3
	bl Trade_CanOfferDjinnFar
	cmp r0, #0
	bne .L_08124086
	b .L_08124182
.L_08124086:
	adds r0, r7, #0
	bl BattleAction_Get
	movs r0, #0
	movs r1, #0
	bl BattlePres_SetActorModes
	mov r2, r10
	ldr r3, [r2]
	adds r2, r5, #0
	movs r4, #0
	ldrsh r0, [r3, r4]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	ands r1, r6
	ands r2, r3
	bl Djinn_ActivateFar
	mov r0, r10
	ldr r3, [r0]
	adds r2, r5, #0
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	ands r1, r6
	ands r2, r3
	bl Trade_RemoveOfferFar
	mov r2, r10
	ldr r3, [r2]
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl Owner_RecalculateStatsFar
	bl Func_081234a4
	movs r0, #30
	bl Func_08122c88
	mov r0, r10
	ldr r3, [r0]
	movs r0, #0
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl BattleEv_Push
	mov r4, r10
	ldr r3, [r4]
	movs r0, #150
	ldrh r2, [r3, #8]
	lsls r0, r0, #1
	lsls r3, r2, #16
	asrs r3, r3, #24
	ands r3, r6
	lsls r1, r3, #2
	adds r1, r1, r3
	adds r3, r5, #0
	ands r3, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	adds r1, r1, r0
	movs r0, #3
	bl BattleEv_Push
	movs r0, #14
	movs r1, #175
	bl BattleEv_Push
	movs r0, #10
	movs r1, #0
	bl BattleEv_Push
	ldr r1, .L_08124434
	movs r0, #4
	bl BattleEv_Push
	mov r1, r10
	ldr r3, [r1]
	movs r0, #11
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl BattleEv_Push
	movs r0, #212
	bl Audio_PlayCue
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlot
	movs r1, #3
	ldr r0, [r0]
	bl Object_SetMode
	mov r2, r10
	ldr r3, [r2]
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl GetBattleObjectSlot
	movs r1, #32
	ldr r0, [r0]
	bl ObjectDispatch_ApplyValueToChildrenFar
	mov r0, r10
	ldr r3, [r0]
	movs r2, #3
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrh r1, [r3, #8]
	movs r3, #0
	lsls r1, r1, #16
	asrs r1, r1, #24
	ands r1, r6
	bl Func_08127308
	bl Func_081234f0
.L_0812417c:
	movs r0, #2
	negs r0, r0
	b .L_081247ec
.L_08124182:
	mov r2, r10
	ldr r3, [r2]
	movs r1, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl UiText_DrawQuantity
	movs r1, #4
	adds r0, r7, #0
	bl UiText_DrawQuantity
	movs r0, #114
	bl Audio_PlayCue
	ldr r0, .L_08124438
	bl UiText_ShowMessageAndWaitCoreFar
	movs r0, #60
	bl WaitFrames
.L_081241aa:
	movs r0, #1
	negs r0, r0
	b .L_081247ec
.L_081241b0:
	add r0, sp, #40
	mov r9, r0
	adds r0, r7, #0
	bl Func_08123648
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_081241c4
	b .L_081247ec
.L_081241c4:
	mov r2, r10
	ldr r3, [r2]
	adds r2, r5, #0
	movs r4, #0
	ldrsh r0, [r3, r4]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	ands r2, r3
	ands r1, r6
	bl Trade_AddOfferFar
	adds r0, r7, #0
	bl BattleAction_Get
	adds r5, r0, #0
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	adds r0, r7, #0
	movs r1, #4
	bl UiText_DrawQuantity
	ldr r0, .L_0812443c
	bl UiText_ShowMessageAndWaitCoreFar
	mov r3, r11
	ldr r2, [r3]
	ldrb r3, [r5, #2]
	str r3, [r2, #80]
	b .L_08124376
.L_0812420a:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #8
	ldrsh r0, [r3, r1]
	bl SummonDefinition_Get
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r7, r0, #0
	adds r3, #68
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08124230
	mov r2, r10
	ldr r3, [r2]
	movs r4, #0
	ldrsh r0, [r3, r4]
	b .L_08124232
.L_08124230:
	movs r0, #0
.L_08124232:
	add r1, sp, #16
	mov r8, r1
	bl Func_08123574
	mov r2, r10
	ldr r3, [r2]
	movs r0, #0
	ldrh r3, [r3]
	cmp r3, #7
	bls .L_08124256
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #68
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08124256
	movs r0, #1
.L_08124256:
	bl Trade_GetOfferStateFar
	adds r0, #8
	str r0, [sp, #4]
	mov r3, r8
	adds r0, r7, #4
	ldrb r2, [r3]
	ldrb r3, [r0]
	movs r6, #0
	cmp r2, r3
	bcc .L_0812428c
	movs r4, #4
	movs r5, #4
	mov r1, r8
.L_08124272:
	ldrb r3, [r7, r4]
	adds r6, #1
	strb r3, [r1]
	adds r5, #1
	adds r1, #1
	cmp r6, #3
	bgt .L_0812428c
	adds r0, #1
	ldrb r2, [r1]
	ldrb r3, [r0]
	adds r4, r5, #0
	cmp r2, r3
	bcs .L_08124272
.L_0812428c:
	ldrh r7, [r7]
	add r4, sp, #40
	mov r9, r4
	adds r0, r7, #0
	bl Func_08123648
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	bne .L_081242a2
	b .L_081247ec
.L_081242a2:
	cmp r6, #4
	beq .L_081242c6
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	adds r0, r7, #0
	movs r1, #4
	bl UiText_DrawQuantity
	ldr r0, .L_08124440
	bl UiText_ShowMessageAndWaitCoreFar
	adds r0, r5, #0
	b .L_081247ec
.L_081242c6:
	mov r2, r10
	ldr r3, [r2]
	movs r1, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl UiText_DrawQuantity
	movs r1, #4
	adds r0, r7, #0
	bl UiText_DrawQuantity
	ldr r0, .L_08124444
	bl UiText_ShowMessageAndWaitCoreFar
	ldr r0, [sp, #4]
	movs r1, #144
	lsls r1, r1, #1
	adds r3, r0, r1
	ldr r3, [r3]
	movs r6, #0
	cmp r3, #0
	beq .L_08124332
	mov r9, r5
	adds r5, r0, #0
.L_081242f6:
	movs r3, #3
	ldrsb r3, [r5, r3]
	cmp r3, r9
	bne .L_08124320
	ldrb r0, [r5, #2]
	bl BattleParty_IsUnitListed
	cmp r0, #0
	beq .L_08124320
	ldrb r1, [r5]
	mov r3, r8
	ldrb r2, [r3, r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_08124320
	movs r3, #254
	strb r3, [r5, #3]
	adds r3, r2, #0
	adds r3, #255
	mov r4, r8
	strb r3, [r4, r1]
.L_08124320:
	ldr r0, [sp, #4]
	movs r1, #144
	lsls r1, r1, #1
	adds r3, r0, r1
	ldr r3, [r3]
	adds r6, #1
	adds r5, #4
	cmp r6, r3
	bne .L_081242f6
.L_08124332:
	movs r2, #201
	lsls r2, r2, #1
	cmp r7, r2
	bne .L_08124376
	mov r4, r10
	ldr r3, [r4]
	ldrh r3, [r3]
	cmp r3, #7
	bls .L_08124364
	ldr r0, [sp, #8]
	movs r2, #128
	ldr r3, [r0]
	movs r4, #128
	lsls r2, r2, #4
	lsls r4, r4, #4
	adds r2, #98
	adds r4, #101
	adds r1, r3, r2
	adds r3, r3, r4
	ldrb r2, [r3]
	movs r3, #1
	lsls r3, r2
	ldrh r2, [r1]
	orrs r3, r2
	strh r3, [r1]
.L_08124364:
	ldr r0, [sp, #8]
	movs r1, #128
	ldr r2, [r0]
	lsls r1, r1, #4
	adds r1, #101
	adds r2, r2, r1
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
.L_08124376:
	cmp r7, #1
	beq .L_0812437c
	b .L_081244e4
.L_0812437c:
	mov r2, r11
	ldr r3, [r2]
	ldrb r0, [r3, #3]
	bl Owner_GetState
	mov r3, r11
	ldr r2, [r3]
	mov r4, r10
	movs r3, #1
	str r3, [r2, #76]
	ldr r3, [r4]
	adds r6, r0, #0
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Func_080ad118
	mov r2, r11
	ldr r1, [r2]
	movs r3, #2
	str r0, [r1, #80]
	str r3, [r1, #84]
	ldr r3, [sp, #12]
	movs r4, #42
	ldr r2, [r3]
	adds r4, #255
	adds r3, r2, r4
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_081243d0
	movs r0, #165
	lsls r0, r0, #1
	adds r3, r2, r0
	ldrh r0, [r3]
	bl Func_08128124
	mov r1, r11
	ldr r2, [r1]
	movs r3, #128
	lsls r3, r3, #7
	orrs r3, r0
	str r3, [r2, #88]
	b .L_081243e6
.L_081243d0:
	movs r3, #0
	str r3, [r1, #88]
	movs r4, #165
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrh r0, [r3]
	bl Func_0811d79c
	mov r1, r11
	ldr r3, [r1]
	str r0, [r3, #88]
.L_081243e6:
	mov r2, r10
	ldr r3, [r2]
	movs r1, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl UiText_DrawQuantity
	ldr r0, .L_08124448
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_0812444c
.L_081243fc:
	ldr r0, [sp, #12]
	movs r1, #156
	ldr r3, [r0]
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0812441e
	bl BattleRandom16Far
	movs r3, #255
	ands r0, r3
	cmp r0, #152
	bgt .L_0812441e
	mov r2, r11
	ldr r3, [r2]
	strb r5, [r3, #31]
.L_0812441e:
	bl BattleRandom16Far
	movs r3, #31
	ands r0, r3
	cmp r0, #0
	bne .L_08124484
	mov r4, r11
	ldr r3, [r4]
	strb r0, [r3, #31]
	b .L_08124484
	.2byte 0x0000
.L_08124434:
	.4byte 0x00000cf7
.L_08124438:
	.4byte 0x00000cb2
.L_0812443c:
	.4byte 0x00000c90
.L_08124440:
	.4byte 0x00000c93
.L_08124444:
	.4byte 0x00000c92
.L_08124448:
	.4byte 0x00000c60
.L_0812444c:
	movs r0, #56
	ldrsh r3, [r6, r0]
	cmp r3, #0
	beq .L_08124484
	movs r1, #158
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_08124484
	movs r2, #60
	adds r2, #255
	adds r3, r6, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_08124484
	movs r4, #70
	adds r4, #255
	adds r3, r6, r4
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_08124484
	movs r0, #157
	lsls r0, r0, #1
	adds r3, r6, r0
	ldrb r5, [r3]
	cmp r5, #0
	beq .L_081243fc
.L_08124484:
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08124498
	mov r1, r11
	ldr r2, [r1]
	movs r3, #0
	strb r3, [r2, #31]
.L_08124498:
	movs r2, #56
	ldrsh r3, [r6, r2]
	cmp r3, #0
	bne .L_081244a2
	b .L_081247be
.L_081244a2:
	bl BattleRandom16Far
	movs r3, #31
	ands r0, r3
	cmp r0, #0
	bne .L_081244b4
	mov r4, r11
	ldr r3, [r4]
	b .L_081244dc
.L_081244b4:
	ldr r1, [sp, #12]
	ldr r0, [r1]
	bl Equipment_GetUnleashRateBonusFar
	movs r1, #200
	lsls r0, r0, #16
	bl __divsi3
	adds r5, r0, #0
	bl BattleRandom16Far
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r0, r3
	cmp r5, r0
	bgt .L_081244d8
	b .L_081247be
.L_081244d8:
	mov r2, r11
	ldr r3, [r2]
.L_081244dc:
	movs r2, #1
	adds r3, #45
	strb r2, [r3]
	b .L_081247be
.L_081244e4:
	adds r0, r7, #0
	bl BattleAction_Get
	mov r4, r11
	ldr r3, [r4]
	ldrb r2, [r0, #2]
	str r7, [r3, #76]
	str r2, [r3, #80]
	movs r2, #0
	str r2, [r3, #88]
	ldrb r2, [r0, #3]
	mov r8, r0
	adds r3, r2, #0
	cmp r3, #65
	beq .L_0812451a
	cmp r3, #41
	beq .L_0812451a
	cmp r3, #42
	beq .L_0812451a
	cmp r3, #43
	beq .L_0812451a
	cmp r3, #44
	beq .L_0812451a
	cmp r3, #68
	beq .L_0812451a
	cmp r3, #89
	bne .L_081245c4
.L_0812451a:
	adds r3, r2, #0
	cmp r3, #89
	bne .L_0812455e
	bl BattleRandom16Far
	movs r3, #255
	ands r0, r3
	cmp r0, #178
	ble .L_0812452e
	b .L_081246b0
.L_0812452e:
	movs r5, #1
	cmp r0, #127
	bgt .L_08124536
	movs r5, #2
.L_08124536:
	mov r0, r11
	ldr r2, [r0]
	movs r1, #0
	movs r3, #1
	ldrsb r3, [r2, r3]
	cmp r1, r3
	blt .L_08124546
	b .L_081246b0
.L_08124546:
	adds r0, r2, #0
	adds r2, #31
.L_0812454a:
	ldrb r3, [r2]
	adds r1, #1
	adds r3, r3, r5
	strb r3, [r2]
	adds r2, #1
	movs r3, #1
	ldrsb r3, [r0, r3]
	cmp r1, r3
	blt .L_0812454a
	b .L_081246b0
.L_0812455e:
	lsls r3, r2, #24
	lsrs r3, r3, #24
	cmp r3, #65
	beq .L_0812456a
	cmp r3, #68
	bne .L_0812456e
.L_0812456a:
	movs r6, #153
	b .L_0812457a
.L_0812456e:
	cmp r3, #41
	beq .L_08124578
	movs r6, #64
	cmp r3, #43
	bne .L_0812457a
.L_08124578:
	movs r6, #32
.L_0812457a:
	lsls r3, r2, #24
	lsrs r3, r3, #24
	cmp r3, #65
	beq .L_0812458c
	cmp r3, #41
	beq .L_0812458c
	movs r5, #2
	cmp r3, #42
	bne .L_0812458e
.L_0812458c:
	movs r5, #1
.L_0812458e:
	bl BattleRandom16Far
	movs r3, #255
	ands r0, r3
	cmp r0, r6
	blt .L_0812459c
	b .L_081246b0
.L_0812459c:
	mov r3, r11
	ldr r2, [r3]
	movs r1, #0
	movs r3, #1
	ldrsb r3, [r2, r3]
	cmp r1, r3
	blt .L_081245ac
	b .L_081246b0
.L_081245ac:
	adds r0, r2, #0
	adds r2, #31
.L_081245b0:
	ldrb r3, [r2]
	adds r1, #1
	adds r3, r3, r5
	strb r3, [r2]
	adds r2, #1
	movs r3, #1
	ldrsb r3, [r0, r3]
	cmp r1, r3
	blt .L_081245b0
	b .L_081246b0
.L_081245c4:
	adds r3, r2, #0
	adds r3, #220
	movs r4, #128
	lsls r3, r3, #24
	lsls r4, r4, #19
	cmp r3, r4
	bhi .L_08124638
	mov r0, r8
	ldrb r3, [r0, #3]
	subs r3, #36
	cmp r3, #4
	bhi .L_08124608
	ldr r2, .L_081247fc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_081245e4:
	.4byte .L_081245f8
	.4byte .L_081245fc
	.4byte .L_08124600
	.4byte .L_08124604
	.4byte .L_08124608
.L_081245f8:
	movs r5, #63
	b .L_0812460a
.L_081245fc:
	movs r5, #31
	b .L_0812460a
.L_08124600:
	movs r5, #15
	b .L_0812460a
.L_08124604:
	movs r5, #7
	b .L_0812460a
.L_08124608:
	movs r5, #3
.L_0812460a:
	bl BattleRandom16Far
	ands r0, r5
	cmp r0, #0
	bne .L_081246b0
	mov r3, r11
	ldr r2, [r3]
	movs r1, #0
	movs r3, #1
	ldrsb r3, [r2, r3]
	cmp r1, r3
	bge .L_081246b0
	adds r0, r2, #1
	movs r4, #2
	adds r2, #45
.L_08124628:
	strb r4, [r2]
	adds r1, #1
	movs r3, #0
	ldrsb r3, [r0, r3]
	adds r2, #1
	cmp r1, r3
	blt .L_08124628
	b .L_081246b0
.L_08124638:
	cmp r7, #178
	beq .L_0812464e
	movs r4, #187
	lsls r4, r4, #1
	cmp r7, r4
	beq .L_0812464e
	movs r0, #164
	lsls r3, r2, #24
	lsls r0, r0, #23
	cmp r3, r0
	bne .L_081246b0
.L_0812464e:
	mov r2, r11
	ldr r3, [r2]
	movs r5, #0
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r5, r3
	bge .L_081246b0
	mov r6, r11
.L_08124660:
	mov r1, r8
	ldrb r3, [r1]
	cmp r3, #1
	bne .L_0812467e
	ldr r3, [r2]
	ldr r1, .L_08124800
	adds r3, r3, r5
	ldrb r3, [r3, #17]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bge .L_0812467a
	negs r3, r3
.L_0812467a:
	ldrb r4, [r1, r3]
	b .L_08124680
.L_0812467e:
	movs r4, #100
.L_08124680:
	mov r2, r10
	ldr r3, [r2]
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldr r3, [r6]
	adds r3, #3
	ldrb r1, [r3, r5]
	mov r3, r8
	ldrb r2, [r3, #2]
	ldrb r3, [r3, #3]
	str r4, [sp, #0]
	bl Battle_HitCheck
	ldr r1, [r6]
	adds r2, r5, #0
	adds r2, #56
	adds r3, r1, #3
	strb r0, [r3, r2]
	adds r5, #1
	movs r3, #1
	ldrsb r3, [r1, r3]
	adds r2, r6, #0
	cmp r5, r3
	blt .L_08124660
.L_081246b0:
	movs r4, #156
	lsls r4, r4, #1
	cmp r7, r4
	bne .L_081246ca
	mov r0, r11
	ldr r3, [r0]
	ldrb r0, [r3]
	bl Func_0811f5a4
	mov r1, r11
	ldr r3, [r1]
	strb r0, [r3, #2]
	b .L_08124708
.L_081246ca:
	movs r2, #158
	lsls r2, r2, #1
	cmp r7, r2
	bne .L_08124708
	mov r3, r11
	ldr r2, [r3]
	movs r5, #0
	ldrb r3, [r2, #3]
	strb r3, [r2, #2]
.L_081246dc:
	mov r4, r11
	ldr r3, [r4]
	movs r0, #0
	ldrb r3, [r3, #2]
	cmp r3, #7
	bhi .L_081246ea
	movs r0, #1
.L_081246ea:
	bl Func_0811f4d4
	bl Battle_GetTaggedSlotValue
	mov r1, r11
	ldr r3, [r1]
	strb r0, [r3, #3]
	ldrb r3, [r3, #2]
	lsls r0, r0, #24
	lsrs r0, r0, #24
	cmp r0, r3
	bne .L_08124708
	adds r5, #1
	cmp r5, #9
	ble .L_081246dc
.L_08124708:
	movs r2, #183
	lsls r2, r2, #2
	cmp r7, r2
	bhi .L_0812473c
	mov r3, r11
	ldr r1, [r3]
	ldr r2, .L_08124804
	lsls r3, r7, #2
	ldr r2, [r2, r3]
	movs r3, #31
	ldrsb r3, [r1, r3]
	str r2, [r1, #88]
	cmp r3, #1
	ble .L_0812472e
	ldr r4, .L_08124808
	lsls r3, r3, #12
	adds r3, r2, r3
	adds r3, r3, r4
	str r3, [r1, #88]
.L_0812472e:
	ldr r1, .L_0812480c
	ldrb r3, [r1, r7]
	cmp r3, #0
	beq .L_0812473c
	mov r0, r11
	ldr r2, [r0]
	b .L_08124772
.L_0812473c:
	adds r0, r7, #0
	bl Ability_CheckStatusOrSpecialId
	cmp r0, #0
	beq .L_0812474e
	mov r1, r11
	ldr r2, [r1]
	movs r3, #3
	b .L_08124772
.L_0812474e:
	mov r3, r11
	ldr r2, [r3]
	ldr r3, [r2, #88]
	cmp r3, #0
	beq .L_08124770
	ldr r4, [sp, #12]
	movs r0, #42
	ldr r3, [r4]
	adds r0, #255
	adds r3, r3, r0
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0812476c
	movs r3, #8
	b .L_08124772
.L_0812476c:
	movs r3, #3
	b .L_08124772
.L_08124770:
	movs r3, #1
.L_08124772:
	str r3, [r2, #84]
	mov r1, r8
	ldrb r0, [r1, #3]
	bl BattleFx_IsReviveFar
	cmp r0, #0
	beq .L_0812478e
	mov r2, r11
	ldr r3, [r2]
	movs r1, #128
	ldr r2, [r3, #88]
	lsls r1, r1, #9
	orrs r2, r1
	str r2, [r3, #88]
.L_0812478e:
	cmp r7, #178
	beq .L_081247a2
	movs r3, #187
	lsls r3, r3, #1
	cmp r7, r3
	beq .L_081247a2
	mov r4, r8
	ldrb r3, [r4, #3]
	cmp r3, #82
	bne .L_081247be
.L_081247a2:
	mov r0, r11
	ldr r1, [r0]
	adds r3, r1, #0
	adds r3, #59
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_081247be
	ldr r3, [r1, #88]
	movs r2, #128
	lsls r2, r2, #5
	orrs r3, r2
	str r3, [r1, #88]
.L_081247be:
	mov r1, r10
	ldr r3, [r1]
	movs r2, #6
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_081247dc
	mov r3, r11
	ldr r2, [r3]
	ldr r3, [r2, #84]
	cmp r3, #5
	beq .L_081247dc
	cmp r3, #9
	beq .L_081247dc
	movs r3, #4
	str r3, [r2, #84]
.L_081247dc:
	mov r0, r10
	ldr r2, [r0]
	mov r4, r11
	ldr r3, [r4]
	ldrh r2, [r2, #6]
	adds r3, #74
	strh r2, [r3]
.L_081247ea:
	movs r0, #0
.L_081247ec:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081247fc:
	.4byte .L_081245e4
.L_08124800:
	.4byte Data_0812896c
.L_08124804:
	.4byte Data_08128c50
.L_08124808:
	.4byte 0xfffff000
.L_0812480c:
	.4byte Data_08128972
