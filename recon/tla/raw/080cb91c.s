.syntax unified
	.thumb
	.global Func_080cb91c
	.thumb_func
Func_080cb91c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #213
	lsls r5, r5, #4
	adds r1, r5, #0
	movs r0, #108
	sub sp, #12
	bl Runtime_AllocateBlock
	adds r1, r5, #0
	ldr r3, .L_080cb9bc
	mov r8, r0
	mov lr, r3
	.2byte 0xf800
	movs r0, #4
	adds r0, #255
	bl GameFlag_ClearBit
	ldr r5, .L_080cb9c0
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r5, r0
	ldrh r2, [r3]
	movs r1, #244
	lsls r1, r1, #1
	adds r3, r5, r1
	strh r2, [r3]
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r3, [r3]
	movs r4, #245
	lsls r4, r4, #1
	adds r2, r5, r4
	strh r3, [r2]
	movs r3, #255
	adds r0, #14
	lsls r3, r3, #8
	adds r2, r5, r0
	adds r3, #255
	strh r3, [r2]
	movs r3, #248
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #1
	negs r3, r3
	adds r4, #8
	strh r3, [r2]
	adds r0, #6
	adds r2, r5, r4
	strh r3, [r2]
	adds r2, r5, r0
	strh r3, [r2]
	movs r2, #226
	ldr r1, .L_080cb9b8
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	strb r1, [r3]
	bl Scheduler_ResetTaskTable
	movs r0, #0
	bl Func_080d793c
	movs r4, #253
	lsls r4, r4, #1
	adds r3, r5, r4
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldr r3, .L_080cb9c4
	cmp r0, r3
	bne .L_080cb9e0
	b .L_080cb9c8
.L_080cb9b8:
	.4byte 0x00000000
.L_080cb9bc:
	.4byte IwramClearWords
.L_080cb9c0:
	.4byte gPartyState
.L_080cb9c4:
	.4byte 0x00000001
.L_080cb9c8:
	bl Func_08020110
	movs r3, #145
	lsls r3, r3, #2
	adds r2, r5, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #0
	movs r7, #3
	bl Func_080c9e48
	b .L_080cb9ec
.L_080cb9e0:
	bl Func_08020108
	movs r0, #1
	movs r7, #2
	bl Func_080c9e48
.L_080cb9ec:
	movs r3, #197
	lsls r3, r3, #1
	add r3, r8
	movs r0, #10
	strb r7, [r3]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080cba0e
	ldr r3, .L_080cba90
	movs r4, #200
	lsls r4, r4, #5
	adds r4, #76
	adds r3, r3, r4
	strb r0, [r3]
	b .L_080cba12
.L_080cba0e:
	bl Func_080e2718
.L_080cba12:
	bl UiWork_InitializeWithResourceCountersFar
	adds r0, r7, #0
	bl Func_08020088
	bl BattleFx_ResetCounters
	ldr r6, .L_080cba94
	ldr r0, [r6, #36]
	mov lr, r0
	.2byte 0xf800
	mov r1, r8
	str r0, [r1, #16]
	bl Func_080cc9c8
	ldr r0, [r6, #28]
	mov lr, r0
	.2byte 0xf800
	bl Func_080caa4c
	bl Func_080cec34
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080cba4e
	bl Func_080cae5c
.L_080cba4e:
	ldr r5, .L_080cba98
	movs r2, #149
	lsls r2, r2, #2
	adds r3, r5, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_080cba62
	bl Func_080d784c
.L_080cba62:
	movs r2, #192
	lsls r2, r2, #4
	movs r3, #255
	adds r2, #184
	lsls r3, r3, #8
	add r2, r8
	adds r3, #255
	strh r3, [r2]
	cmp r7, #3
	bne .L_080cba9c
	bl Func_08020128
	movs r0, #254
	lsls r0, r0, #1
	movs r1, #129
	adds r3, r5, r0
	lsls r1, r1, #2
	ldr r0, [r3]
	adds r3, r5, r1
	ldr r1, [r3]
	bl Func_08020130
	b .L_080cbaaa
.L_080cba90:
	.4byte Data_02001000
.L_080cba94:
	.4byte gOverlayArea
.L_080cba98:
	.4byte gPartyState
.L_080cba9c:
	bl Func_08020180
	ldr r0, [r6, #52]
	mov lr, r0
	.2byte 0xf800
	bl Func_08020120
.L_080cbaaa:
	movs r0, #32
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_080d1684
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_080d170c
	movs r2, #214
	lsls r2, r2, #1
	movs r3, #128
	add r2, r8
	lsls r3, r3, #1
	movs r4, #217
	str r3, [r2]
	lsls r4, r4, #1
	adds r3, #180
	add r3, r8
	add r4, r8
	mov r11, r2
	movs r5, #16
	movs r2, #206
	str r5, [r3]
	mov r10, r4
	mov r9, r3
	lsls r2, r2, #1
	movs r3, #154
	movs r7, #0
	adds r3, #255
	mov r0, r10
	add r2, r8
	strh r7, [r0]
	str r3, [r2]
	adds r3, #7
	add r3, r8
	ldr r6, .L_080cbb68
	movs r1, #128
	str r7, [r3]
	lsls r1, r1, #2
	adds r1, #94
	adds r3, r6, r1
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #2
	bne .L_080cbb14
	movs r0, #162
	str r7, [r2]
	lsls r0, r0, #1
	bl GameFlag_SetBit
.L_080cbb14:
	movs r3, #212
	ldr r2, .L_080cbb6c
	lsls r3, r3, #1
	add r3, r8
	movs r0, #10
	str r2, [r3]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080cbb70
	bl Func_080ca2fc
	movs r0, #147
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r3, r6, r0
	adds r1, #78
	strh r5, [r3]
	adds r3, r6, r1
	strh r7, [r3]
	movs r3, #148
	lsls r3, r3, #2
	adds r2, r6, r3
	movs r4, #128
	movs r3, #1
	strh r3, [r2]
	lsls r4, r4, #2
	movs r3, #255
	adds r4, #106
	lsls r3, r3, #8
	adds r2, r6, r4
	adds r3, #255
	strh r3, [r2]
	ldr r3, .L_080cbb64
	adds r0, #33
	adds r2, r6, r0
	strb r3, [r2]
	b .L_080cbb70
.L_080cbb64:
	.4byte 0x00000000
.L_080cbb68:
	.4byte gPartyState
.L_080cbb6c:
	.4byte gMapCellBuffer
.L_080cbb70:
	bl Func_080dec8c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_080c9934
	movs r2, #208
	adds r5, r0, #0
	lsls r2, r2, #4
	lsrs r3, r5, #4
	movs r1, #15
	adds r2, #55
	ands r3, r1
	add r2, r8
	strb r3, [r2]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #54
	add r3, r8
	ands r5, r1
	strb r5, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #194
	adds r6, r6, r3
	movs r3, #0
	ldrsb r3, [r6, r3]
	cmp r3, #2
	bne .L_080cbbc8
	bl Func_080cdf5c
	bl Object_GetById
	str r7, [r0, #24]
	bl Func_080cdf5c
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
.L_080cbbc8:
	bl Func_080cb05c
	movs r0, #2
	bl WaitFrames
	ldr r3, .L_080cbddc
	ldr r0, [r3, #4]
	mov lr, r0
	.2byte 0xf800
	movs r3, #172
	lsls r3, r3, #1
	add r3, r8
	movs r4, #0
	ldrsh r0, [r3, r4]
	cmp r0, #0
	beq .L_080cbbf0
	adds r6, r0, #0
	strh r7, [r3]
	bl .L_080cc4fa
.L_080cbbf0:
	movs r0, #10
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_080cafc4
	cmp r0, #0
	beq .L_080cbc02
	b .L_080cbd7c
.L_080cbc02:
	mov r0, r10
	ldrh r5, [r0]
	cmp r5, #0
	bne .L_080cbc7a
	movs r3, #0
	ldrsb r3, [r6, r3]
	cmp r3, #1
	bne .L_080cbc2a
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #184
	movs r0, #92
	bl Runtime_AllocateHeapBlock
	movs r0, #20
	bl WaitFrames
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
.L_080cbc2a:
	mov r1, r11
	mov r2, r9
	ldr r0, [r1]
	ldr r1, [r2]
	bl Func_080d01cc
	movs r3, #1
	mov r4, r10
	strh r3, [r4]
	movs r3, #0
	ldrsb r3, [r6, r3]
	cmp r3, #1
	bne .L_080cbc5c
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #184
	movs r0, #92
	bl Runtime_AllocateHeapBlock
	movs r0, #20
	bl WaitFrames
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
.L_080cbc5c:
	movs r3, #160
	lsls r3, r3, #19
	strh r5, [r3]
	bl ObjectEffect_RunPendingFlagEvent
	cmp r0, #0
	bne .L_080cbc7a
	mov r1, r9
	ldr r0, [r1]
	adds r0, #1
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	bl WaitFrames
.L_080cbc7a:
	ldr r7, .L_080cbde0
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #194
	adds r2, r7, r3
	movs r6, #0
	ldrsb r6, [r2, r6]
	cmp r6, #1
	bne .L_080cbd08
	movs r3, #2
	strb r3, [r2]
	movs r4, #0
	mov r10, r4
	bl Func_080e7804
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #190
	adds r3, r7, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_080cbde4
	cmp r2, r3
	bne .L_080cbcc2
	movs r0, #176
	lsls r0, r0, #2
	adds r3, r7, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Func_080c9e1c
	movs r3, #16
	adds r5, r0, #0
	ands r3, r5
	cmp r3, #0
	beq .L_080cbcd0
.L_080cbcc2:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r7, r2
	mov r4, r10
	strh r4, [r3]
	b .L_080cbce2
.L_080cbcd0:
	movs r3, #32
	ands r3, r5
	cmp r3, #0
	beq .L_080cbce2
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #118
	adds r3, r7, r0
	strh r6, [r3]
.L_080cbce2:
	ldr r3, .L_080cbde0
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #190
	adds r2, r3, r1
	adds r1, #2
	adds r3, r3, r1
	movs r4, #0
	ldrsh r0, [r2, r4]
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Func_080d2aa4
	b .L_080cbd14
.L_080cbcfe:
	movs r3, #0
	adds r6, r0, #0
	strh r3, [r1]
	bl .L_080cc4fa
.L_080cbd08:
	cmp r6, #2
	bne .L_080cbd14
	movs r3, #0
	strb r3, [r2]
	bl Func_080e7238
.L_080cbd14:
	movs r5, #48
	adds r5, #255
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080cbd38
	adds r0, r5, #0
	bl GameFlag_ClearBit
	ldr r3, .L_080cbde0
	movs r4, #240
	lsls r4, r4, #1
	adds r3, r3, r4
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Func_08038200
.L_080cbd38:
	ldr r5, .L_080cbde0
	movs r2, #149
	lsls r2, r2, #2
	adds r6, r5, r2
	movs r3, #0
	ldrsh r0, [r6, r3]
	cmp r0, #0
	beq .L_080cbd52
	movs r1, #1
	bl Func_080d793c
	movs r3, #0
	strh r3, [r6]
.L_080cbd52:
	movs r4, #151
	lsls r4, r4, #2
	adds r5, r5, r4
	ldrh r3, [r5]
	cmp r3, #0
	beq .L_080cbd7c
	bl Func_080cb82c
	ldrh r0, [r5]
	bl Func_080d25c8
	cmp r0, #0
	bne .L_080cbd74
	ldrh r0, [r5]
	movs r1, #0
	bl PartyInventory_GiveItem
.L_080cbd74:
	bl Func_080cb8a4
	movs r3, #0
	strh r3, [r5]
.L_080cbd7c:
	ldr r0, .L_080cbde0
	movs r2, #173
	lsls r2, r2, #1
	str r0, [sp, #8]
	movs r1, #0
	add r2, r8
	mov r10, r1
	mov r11, r2
.L_080cbd8c:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_080cb8e8
	mov r9, r0
	bl Object_ResetMotion
	ldr r3, [sp, #8]
	movs r4, #254
	lsls r4, r4, #1
	mov r0, r9
	adds r2, r3, r4
	ldr r3, [r0, #8]
	mov r4, r10
	str r3, [r2]
	ldr r1, [sp, #8]
	movs r2, #128
	lsls r2, r2, #2
	adds r3, r1, r2
	str r4, [r3]
	movs r0, #129
	lsls r0, r0, #2
	adds r2, r1, r0
	mov r1, r9
	ldr r3, [r1, #16]
	movs r4, #130
	str r3, [r2]
	ldr r3, [sp, #8]
	lsls r4, r4, #2
	adds r2, r3, r4
	ldrh r3, [r1, #6]
	ldr r0, .L_080cbde8
	str r3, [r2]
	mov r3, r9
	adds r3, #34
	ldrb r3, [r3]
	strh r3, [r0]
	b .L_080cc47e
.L_080cbddc:
	.4byte gOverlayArea
.L_080cbde0:
	.4byte gPartyState
.L_080cbde4:
	.4byte 0x00000002
.L_080cbde8:
	.4byte Data_0200044c
.L_080cbdec:
	movs r1, #172
	lsls r1, r1, #1
	add r1, r8
	movs r2, #0
	ldrsh r0, [r1, r2]
	cmp r0, #0
	beq .L_080cbdfc
	b .L_080cbcfe
.L_080cbdfc:
	movs r2, #181
	lsls r2, r2, #1
	add r2, r8
	movs r4, #0
	ldrsh r3, [r2, r4]
	cmp r3, #0
	bne .L_080cbe0c
	b .L_080cc0c6
.L_080cbe0c:
	movs r3, #192
	lsls r3, r3, #4
	ldr r0, .L_080cbe48
	adds r3, #162
	add r3, r8
	strh r0, [r3]
	movs r5, #1
	movs r1, #0
	ldrsh r3, [r2, r1]
	negs r5, r5
	ldr r6, .L_080cbe48
	cmp r3, r5
	beq .L_080cbe28
	b .L_080cbf84
.L_080cbe28:
	bl Func_080d2260
	bl Func_080cb82c
	movs r3, #182
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r5, #0
	cmp r5, r3
	bcs .L_080cbe78
	movs r6, #184
	lsls r6, r6, #1
	b .L_080cbe4c
	.2byte 0x0000
.L_080cbe48:
	.4byte 0x00000001
.L_080cbe4c:
	mov r0, r9
	movs r1, #22
	bl Object_SetMode
	mov r3, r8
	ldrsh r0, [r6, r3]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_080cbe9c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r3, #182
	lsls r3, r3, #1
	add r3, r8
	movs r0, #0
	ldrsh r3, [r3, r0]
	adds r5, #1
	adds r6, #2
	cmp r5, r3
	bcc .L_080cbe4c
.L_080cbe78:
	movs r3, #183
	lsls r3, r3, #1
	add r3, r8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_080cbe88
	b .L_080cbf7e
.L_080cbe88:
	ldr r2, .L_080cbea0
	ldr r3, [r2]
	cmp r3, #4
	bne .L_080cbea4
	mov r0, r9
	movs r1, #36
	bl Object_SetMode
	b .L_080cbeac
	.2byte 0x0000
.L_080cbe9c:
	.4byte 0x00000d92
.L_080cbea0:
	.4byte Data_02000454
.L_080cbea4:
	mov r0, r9
	movs r1, #19
	bl Object_SetMode
.L_080cbeac:
	movs r0, #59
	bl Audio_PlayCue
	movs r1, #1
	ldr r0, .L_080cbeec
	bl UiText_ShowPositionedMessageAndWaitFar
	ldr r3, .L_080cbef0
	ldr r0, [r3]
	bl Owner_GetState
	ldr r4, .L_080cbee8
	adds r6, r0, #0
	strh r4, [r6, #56]
	movs r0, #1
	lsls r5, r0, #14
	movs r2, #52
	ldrsh r1, [r6, r2]
	adds r0, r5, #0
	bl __divsi3
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_080cbef4
	movs r5, #0
	cmp r0, #0
	blt .L_080cbef4
	adds r5, r0, #0
	b .L_080cbef4
.L_080cbee8:
	.4byte 0x00000001
.L_080cbeec:
	.4byte 0x00000d93
.L_080cbef0:
	.4byte Data_02000454
.L_080cbef4:
	lsls r3, r5, #16
	strh r5, [r6, #20]
	cmp r3, #0
	bne .L_080cbf08
	movs r4, #56
	ldrsh r3, [r6, r4]
	cmp r3, #0
	beq .L_080cbf08
	ldr r0, .L_080cbf28
	strh r0, [r6, #20]
.L_080cbf08:
	movs r1, #58
	ldrsh r0, [r6, r1]
	movs r2, #54
	ldrsh r1, [r6, r2]
	lsls r0, r0, #14
	bl __divsi3
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_080cbf2c
	movs r3, #0
	cmp r0, #0
	blt .L_080cbf2c
	adds r3, r0, #0
	b .L_080cbf2c
.L_080cbf28:
	.4byte 0x00000001
.L_080cbf2c:
	strh r3, [r6, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_080cbf40
	movs r4, #58
	ldrsh r3, [r6, r4]
	cmp r3, #0
	beq .L_080cbf40
	ldr r0, .L_080cbf74
	strh r0, [r6, #22]
.L_080cbf40:
	ldr r1, [sp, #8]
	movs r2, #242
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r0, [r3]
	movs r4, #240
	lsls r4, r4, #1
	adds r3, r1, r4
	strh r0, [r3]
	adds r2, #2
	adds r3, r1, r2
	ldrh r1, [r3]
	ldr r3, .L_080cbf78
	lsls r0, r0, #16
	strh r1, [r3]
	movs r6, #186
	lsls r1, r1, #16
	asrs r0, r0, #16
	asrs r1, r1, #16
	lsls r6, r6, #2
	bl Func_080ca3f4
	adds r6, #255
	bl Func_080cb8a4
	b .L_080cbf7c
.L_080cbf74:
	.4byte 0x00000001
.L_080cbf78:
	.4byte Data_02000422
.L_080cbf7c:
	b .L_080cc4fa
.L_080cbf7e:
	bl Func_080cb8a4
	b .L_080cc0b6
.L_080cbf84:
	ldr r4, .L_080cc110
	cmp r3, r4
	bne .L_080cbfb4
	bl Func_080d2260
	movs r0, #1
	bl Func_080ed2a4
	ldr r2, .L_080cc114
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #190
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, r5
	bne .L_080cbfa8
	b .L_080cc0b6
.L_080cbfa8:
	movs r4, #128
	lsls r4, r4, #2
	adds r4, #194
	adds r3, r2, r4
	strb r6, [r3]
	b .L_080cc0b6
.L_080cbfb4:
	ldr r0, .L_080cc118
	cmp r3, r0
	bne .L_080cbfd0
	bl Func_080d2260
	movs r5, #0
	movs r6, #1
.L_080cbfc2:
	adds r0, r5, #0
	bl Func_080ed2a4
	eors r5, r6
	cmp r0, #0
	bne .L_080cbfc2
	b .L_080cc0b6
.L_080cbfd0:
	movs r3, #181
	lsls r3, r3, #1
	add r3, r8
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldr r2, .L_080cc11c
	cmp r0, r2
	bne .L_080cbfea
	bl Func_080d2260
	bl Func_080ed788
	b .L_080cc0b6
.L_080cbfea:
	ldr r3, .L_080cc120
	cmp r0, r3
	bne .L_080cc006
	movs r3, #195
	ldr r2, .L_080cc124
	lsls r3, r3, #1
	add r3, r8
	ldrh r3, [r3]
	ldr r2, [r2]
	cmp r3, r2
	bne .L_080cc0b6
	bl Func_080d5720
	b .L_080cc03c
.L_080cc006:
	ldr r1, .L_080cc128
	cmp r0, r1
	bne .L_080cc022
	movs r3, #195
	ldr r2, .L_080cc124
	lsls r3, r3, #1
	add r3, r8
	ldrh r3, [r3]
	ldr r2, [r2]
	cmp r3, r2
	bne .L_080cc0b6
	bl Func_080d57e4
	b .L_080cc084
.L_080cc022:
	ldr r3, .L_080cc12c
	cmp r0, r3
	bne .L_080cc06a
	movs r3, #195
	ldr r2, .L_080cc124
	lsls r3, r3, #1
	add r3, r8
	ldrh r3, [r3]
	ldr r2, [r2]
	cmp r3, r2
	bne .L_080cc0b6
	bl Func_080d59dc
.L_080cc03c:
	movs r3, #175
	lsls r3, r3, #1
	mov r4, r10
	mov r0, r11
	add r3, r8
	mov r1, r10
	strh r4, [r0]
	strh r1, [r3]
	movs r3, #174
	lsls r3, r3, #1
	add r3, r8
	mov r2, r10
	strh r2, [r3]
	movs r3, #178
	lsls r3, r3, #1
	add r3, r8
	strh r4, [r3]
	movs r3, #179
	lsls r3, r3, #1
	add r3, r8
	mov r0, r10
	strh r0, [r3]
	b .L_080cc0b6
.L_080cc06a:
	ldr r1, .L_080cc130
	cmp r0, r1
	bne .L_080cc0b2
	movs r3, #195
	ldr r2, .L_080cc124
	lsls r3, r3, #1
	add r3, r8
	ldrh r3, [r3]
	ldr r2, [r2]
	cmp r3, r2
	bne .L_080cc0b6
	bl Func_080d5aa0
.L_080cc084:
	mov r2, r10
	mov r3, r11
	strh r2, [r3]
	movs r3, #175
	lsls r3, r3, #1
	add r3, r8
	mov r4, r10
	strh r4, [r3]
	movs r3, #174
	lsls r3, r3, #1
	add r3, r8
	mov r0, r10
	strh r0, [r3]
	movs r3, #178
	lsls r3, r3, #1
	add r3, r8
	mov r1, r10
	strh r1, [r3]
	movs r3, #179
	lsls r3, r3, #1
	add r3, r8
	strh r2, [r3]
	b .L_080cc0b6
.L_080cc0b2:
	bl Func_080cd584
.L_080cc0b6:
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #162
	add r3, r8
	mov r4, r10
	strh r4, [r3]
	movs r3, #181
	b .L_080cc476
.L_080cc0c6:
	movs r6, #170
	lsls r6, r6, #1
	add r6, r8
	movs r2, #0
	ldrsh r7, [r6, r2]
	cmp r7, #0
	beq .L_080cc0f2
	movs r5, #192
	ldr r4, .L_080cc10c
	lsls r5, r5, #4
	adds r5, #162
	add r5, r8
	strh r4, [r5]
	str r3, [sp, #0]
	movs r1, #0
	ldrsh r0, [r6, r1]
	bl Func_080cd5bc
	ldr r3, [sp, #0]
	strh r3, [r5]
	strh r3, [r6]
	b .L_080cc47e
.L_080cc0f2:
	movs r5, #171
	lsls r5, r5, #1
	add r5, r8
	movs r2, #0
	ldrsh r6, [r5, r2]
	cmp r6, #0
	beq .L_080cc134
	adds r0, r6, #0
	bl Func_080cd680
.L_080cc106:
	strh r7, [r5]
	b .L_080cc47e
	.2byte 0x0000
.L_080cc10c:
	.4byte 0x00000001
.L_080cc110:
	.4byte 0xfffffc89
.L_080cc114:
	.4byte gPartyState
.L_080cc118:
	.4byte 0xfffffc88
.L_080cc11c:
	.4byte 0xfffffc87
.L_080cc120:
	.4byte 0xfffffc85
.L_080cc124:
	.4byte gInput
.L_080cc128:
	.4byte 0xfffffc86
.L_080cc12c:
	.4byte 0xfffffc83
.L_080cc130:
	.4byte 0xfffffc84
.L_080cc134:
	movs r5, #178
	lsls r5, r5, #1
	add r5, r8
	movs r4, #0
	ldrsh r3, [r5, r4]
	cmp r3, #0
	beq .L_080cc192
	str r1, [sp, #4]
	bl Func_080d2260
	bl Func_080cad9c
	movs r3, #128
	ldr r0, [sp, #8]
	lsls r3, r3, #2
	adds r3, #62
	adds r2, r0, r3
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	movs r4, #240
	strh r3, [r2]
	lsls r4, r4, #1
	movs r3, #255
	adds r2, r0, r4
	lsls r3, r3, #1
	strh r3, [r2]
	ldr r0, .L_080cc218
	ldrh r3, [r5]
	ldr r1, [sp, #4]
	strh r3, [r0]
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	strh r3, [r1]
	movs r1, #0
	ldrsh r0, [r5, r1]
	bl Func_080d5c70
	movs r3, #202
	ldr r2, .L_080cc21c
	lsls r3, r3, #1
	add r3, r8
	str r6, [r3]
	strh r6, [r5]
	str r6, [r2]
	b .L_080cc47e
.L_080cc192:
	movs r3, #174
	lsls r3, r3, #1
	add r3, r8
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_080cc238
	bl Func_08038208
	ldr r3, .L_080cc220
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #118
	adds r3, r3, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r6, #0
	cmp r3, #0
	bne .L_080cc1be
	ldr r2, .L_080cc224
	ldr r0, [r2]
	b .L_080cc1c0
.L_080cc1be:
	movs r0, #8
.L_080cc1c0:
	bl Func_080cd91c
	adds r5, r0, #0
	movs r3, #1
	negs r3, r3
	cmp r5, r3
	beq .L_080cc1e2
	movs r6, #1
	cmp r5, #63
	bgt .L_080cc1e2
	adds r0, r5, #0
	bl Func_080cce94
	cmp r0, #0
	beq .L_080cc1e0
	movs r0, #1
.L_080cc1e0:
	adds r6, r0, #0
.L_080cc1e2:
	cmp r6, #0
	beq .L_080cc1fa
	ldr r3, .L_080cc214
	mov r4, r10
	orrs r5, r3
	movs r3, #176
	lsls r3, r3, #1
	add r3, r8
	mov r0, r11
	strh r5, [r3]
	strh r4, [r0]
	b .L_080cc22e
.L_080cc1fa:
	bl Func_080cc54c
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080cc228
	movs r3, #177
	lsls r3, r3, #1
	add r3, r8
	mov r1, r10
	mov r2, r11
	strh r6, [r3]
	strh r1, [r2]
	b .L_080cc22e
.L_080cc214:
	.4byte 0x00004000
.L_080cc218:
	.4byte Data_02000422
.L_080cc21c:
	.4byte Data_02000498
.L_080cc220:
	.4byte gPartyState
.L_080cc224:
	.4byte Data_02000454
.L_080cc228:
	ldr r3, .L_080cc234
	mov r4, r11
	strh r3, [r4]
.L_080cc22e:
	movs r3, #174
	b .L_080cc476
	.2byte 0x0000
.L_080cc234:
	.4byte 0x00000001
.L_080cc238:
	mov r2, r11
	movs r1, #0
	ldrsh r7, [r2, r1]
	cmp r7, #0
	beq .L_080cc2c6
	bl Func_08038208
	bl Func_080d2260
	movs r0, #111
	bl Audio_PlayCue
	bl Func_080cb82c
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_SetBit
	ldr r3, .L_080cc308
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080cc280
	ldr r1, .L_080cc30c
	movs r2, #2
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	beq .L_080cc280
	ldr r3, [r1]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	beq .L_080cc280
	bl Menu_RunSelectionWithCursorObjectFar
	b .L_080cc2ae
.L_080cc280:
	movs r0, #8
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080cc298
	movs r2, #181
	lsls r2, r2, #1
	add r2, r8
	movs r3, #250
	strh r3, [r2]
	b .L_080cc2ae
.L_080cc298:
	movs r5, #192
	lsls r5, r5, #4
	adds r5, #165
	add r5, r8
	strb r0, [r5]
	bl Func_080381e8
	movs r3, #1
	strb r3, [r5]
	bl Func_080cb05c
.L_080cc2ae:
	bl Func_080cb8a4
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	bl Func_080ad2b8
	mov r4, r10
	mov r0, r11
	strh r4, [r0]
	b .L_080cc47e
.L_080cc2c6:
	movs r5, #176
	lsls r5, r5, #1
	add r5, r8
	movs r1, #0
	ldrsh r6, [r5, r1]
	cmp r6, #0
	beq .L_080cc2e8
	bl Func_080cb82c
	ldrh r3, [r5]
	ldr r0, .L_080cc304
	ands r0, r3
	bl Func_080ccec8
	bl Func_080cb8a4
	b .L_080cc106
.L_080cc2e8:
	movs r5, #177
	lsls r5, r5, #1
	add r5, r8
	movs r2, #0
	ldrsh r7, [r5, r2]
	cmp r7, #0
	beq .L_080cc310
	bl Func_080cb82c
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Func_080cd808
	b .L_080cc378
.L_080cc304:
	.4byte 0x00003fff
.L_080cc308:
	.4byte gDebugMode
.L_080cc30c:
	.4byte gInput
.L_080cc310:
	movs r5, #179
	lsls r5, r5, #1
	add r5, r8
	movs r4, #0
	ldrsh r6, [r5, r4]
	ldrh r2, [r5]
	cmp r6, #0
	beq .L_080cc354
	ldr r3, .L_080cc350
	ands r3, r2
	lsls r3, r3, #16
	asrs r6, r3, #16
	bl Func_08038208
	cmp r6, #0
	bne .L_080cc33a
	bl Func_080cb82c
	movs r0, #1
	bl WaitFrames
.L_080cc33a:
	movs r1, #0
	ldrsh r0, [r5, r1]
	bl Func_080ce61c
	cmp r6, #0
	beq .L_080cc348
	b .L_080cc106
.L_080cc348:
	bl Func_080cb8a4
	b .L_080cc106
	.2byte 0x0000
.L_080cc350:
	.4byte 0x00002000
.L_080cc354:
	movs r5, #180
	lsls r5, r5, #1
	add r5, r8
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_080cc380
	bl Func_080cb82c
	movs r3, #0
	ldrsh r0, [r5, r3]
	movs r3, #193
	lsls r3, r3, #1
	add r3, r8
	movs r4, #0
	ldrsh r1, [r3, r4]
	bl Func_080ce0ac
.L_080cc378:
	bl Func_080cb8a4
	strh r6, [r5]
	b .L_080cc47e
.L_080cc380:
	movs r3, #175
	lsls r3, r3, #1
	add r3, r8
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_080cc47e
	movs r0, #111
	bl Audio_PlayCue
	bl Func_080d2260
	bl Func_080cb82c
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_SetBit
	ldr r1, .L_080cc454
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_080cc3be
	ldr r3, .L_080cc458
	movs r2, #2
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_080cc3be
	bl Func_08038278
	b .L_080cc468
.L_080cc3be:
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_080cc3d8
	ldr r3, .L_080cc458
	movs r2, #128
	ldr r3, [r3]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080cc3d8
	bl Func_080cc9fc
	b .L_080cc468
.L_080cc3d8:
	movs r0, #8
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080cc3f0
	movs r2, #181
	lsls r2, r2, #1
	add r2, r8
	movs r3, #250
	strh r3, [r2]
	b .L_080cc468
.L_080cc3f0:
	bl Func_080cdec8
	bl Func_08038208
	bl Func_081c0070
	movs r2, #128
	ldr r1, [sp, #8]
	lsls r2, r2, #2
	adds r2, #62
	adds r3, r1, r2
	strh r0, [r3]
	movs r0, #191
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080cc460
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #24]
	ldr r1, .L_080cc45c
	movs r2, #128
	movs r0, #0
	lsls r2, r2, #2
.L_080cc422:
	ldrb r3, [r1]
	adds r1, #1
	cmp r3, #255
	bne .L_080cc42c
	adds r0, #1
.L_080cc42c:
	subs r2, #1
	cmp r2, #0
	bne .L_080cc422
	adds r3, r0, #0
	subs r3, #136
	cmp r3, #0
	bge .L_080cc444
	ldr r3, .L_080cc450
	movs r0, #1
	strh r3, [r5, #4]
	bl WaitFrames
.L_080cc444:
	movs r0, #0
	bl Func_08038358
	mov r4, r10
	strh r4, [r5, #4]
	b .L_080cc468
.L_080cc450:
	.4byte 0x00000001
.L_080cc454:
	.4byte gDebugMode
.L_080cc458:
	.4byte gInput
.L_080cc45c:
	.4byte Data_02003410
.L_080cc460:
	ldr r0, .L_080cc540
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080cc468:
	bl Func_080cb8a4
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r3, #175
.L_080cc476:
	lsls r3, r3, #1
	add r3, r8
	mov r0, r10
	strh r0, [r3]
.L_080cc47e:
	bl Func_080cafc4
	cmp r0, #0
	beq .L_080cc488
	b .L_080cbdec
.L_080cc488:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	bl Func_080cb8e8
	cmp r0, #0
	beq .L_080cc49c
	bl Func_080200b0
.L_080cc49c:
	ldr r5, .L_080cc544
.L_080cc49e:
	movs r0, #1
	bl WaitFrames
	bl Func_080cb8e8
	ldr r3, .L_080cc548
	mov r9, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080cc4be
	movs r0, #100
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080cc4f0
.L_080cc4be:
	movs r3, #0
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_080cc4de
	mov r3, r9
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_080cc4f0
	mov r3, r9
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_080cc4f0
.L_080cc4de:
	mov r3, r9
	adds r3, #34
	ldrb r0, [r3]
	mov r3, r9
	ldr r1, [r3, #8]
	ldr r2, [r3, #12]
	ldr r3, [r3, #16]
	bl Func_080cb2b8
.L_080cc4f0:
	bl Func_080cafc4
	cmp r0, #0
	beq .L_080cc49e
	b .L_080cbd8c
.L_080cc4fa:
	movs r7, #217
	lsls r7, r7, #1
	add r7, r8
	ldrh r3, [r7]
	cmp r3, #0
	beq .L_080cc524
	movs r3, #214
	movs r5, #218
	lsls r3, r3, #1
	lsls r5, r5, #1
	add r3, r8
	add r5, r8
	ldr r0, [r3]
	ldr r1, [r5]
	bl Func_080d0520
	movs r3, #0
	strh r3, [r7]
	ldr r0, [r5]
	bl WaitFrames
.L_080cc524:
	movs r0, #1
	bl WaitFrames
	movs r0, #108
	bl Runtime_ReleaseHeapBlock
	adds r0, r6, #0
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080cc540:
	.4byte 0x00001167
.L_080cc544:
	.4byte Data_020004ac
.L_080cc548:
	.4byte gDebugMode
