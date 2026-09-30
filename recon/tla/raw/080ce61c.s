.syntax unified
	.thumb
	.global Func_080ce61c
	.thumb_func
Func_080ce61c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r0
	movs r0, #192
	movs r3, #192
	lsls r0, r0, #2
	adds r0, #255
	lsls r3, r3, #18
	mov r1, r8
	ands r1, r0
	ldr r3, [r3, #108]
	mov r9, r1
	sub sp, #16
	mov r0, r9
	str r3, [sp, #12]
	bl BattleAction_Get
	mov r2, r8
	ldrb r0, [r0, #6]
	lsrs r7, r2, #10
	movs r3, #15
	ands r7, r3
	movs r3, #0
	str r0, [sp, #4]
	str r3, [sp, #0]
	bl Func_080d2260
	ldr r0, [sp, #12]
	movs r1, #211
	lsls r1, r1, #4
	adds r2, r0, r1
	movs r3, #255
	strb r3, [r2]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #72
	adds r3, r0, r2
	mov r0, sp
	ldrh r0, [r0]
	strh r0, [r3]
	movs r0, #70
	adds r0, #255
	bl GameFlag_ClearBit
	cmp r7, #15
	bne .L_080ce684
	movs r7, #0
.L_080ce684:
	movs r0, #191
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ce6aa
	adds r0, r7, #0
	movs r1, #1
	bl UiText_DrawQuantity
	mov r0, r9
	movs r1, #4
	bl UiText_DrawQuantity
	ldr r0, .L_080ce900
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080ceaec
.L_080ce6aa:
	ldr r1, [sp, #12]
	movs r2, #197
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080ce736
	mov r3, r9
	cmp r3, #144
	beq .L_080ce6c6
	cmp r3, #155
	bne .L_080ce6e0
.L_080ce6c6:
	adds r0, r7, #0
	movs r1, #1
	bl UiText_DrawQuantity
	mov r0, r9
	movs r1, #4
	bl UiText_DrawQuantity
	ldr r0, .L_080ce900
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080ceaec
.L_080ce6e0:
	mov r0, r9
	cmp r0, #153
	bne .L_080ce736
	ldr r3, .L_080ce904
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #118
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_080ce71c
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ce71c
	movs r0, #72
	adds r0, #255
	bl GameFlag_SetBit
	ldr r3, [sp, #12]
	movs r0, #181
	lsls r0, r0, #1
	adds r2, r3, r0
	movs r3, #253
	strh r3, [r2]
	b .L_080ceaec
.L_080ce71c:
	adds r0, r7, #0
	movs r1, #1
	bl UiText_DrawQuantity
	mov r0, r9
	movs r1, #4
	bl UiText_DrawQuantity
	ldr r0, .L_080ce900
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080ceaec
.L_080ce736:
	ldr r3, .L_080ce904
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #118
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_080ce768
	mov r3, r9
	cmp r3, #139
	bne .L_080ce768
	adds r0, r7, #0
	movs r1, #1
	bl UiText_DrawQuantity
	movs r0, #139
	movs r1, #4
	bl UiText_DrawQuantity
	ldr r0, .L_080ce900
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080ceaec
.L_080ce768:
	mov r0, r9
	cmp r0, #149
	bne .L_080ce7ee
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ce794
	adds r0, r7, #0
	movs r1, #1
	bl UiText_DrawQuantity
	movs r0, #149
	movs r1, #4
	bl UiText_DrawQuantity
	ldr r0, .L_080ce908
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080ceaec
.L_080ce794:
	movs r0, #149
	movs r1, #4
	bl UiText_DrawQuantity
	movs r1, #13
	ldr r0, .L_080ce90c
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r0, #1
	bl Func_080d295c
	adds r5, r0, #0
	bl UiWork_FinalizePendingCoreFar
	movs r0, #0
	cmp r5, #0
	beq .L_080ce7b8
	b .L_080ceaee
.L_080ce7b8:
	ldr r1, .L_080ce904
	movs r2, #152
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	strh r2, [r3]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #98
	adds r3, r1, r2
	ldrh r3, [r3]
	adds r0, #2
	adds r2, r1, r0
	strh r3, [r2]
	ldr r1, [sp, #12]
	movs r3, #172
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	movs r0, #1
	strh r3, [r2]
	str r0, [sp, #0]
.L_080ce7ee:
	ldr r0, [sp, #4]
	bl Func_080ce31c
	adds r6, r0, #0
	movs r0, #128
	lsls r0, r0, #24
	adds r0, #5
	ldr r1, [sp, #4]
	adds r2, r6, #0
	bl Func_080ce458
	cmp r0, #0
	beq .L_080ce82e
	movs r1, #1
	negs r1, r1
	mov r10, r1
	cmp r6, r10
	beq .L_080ce826
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_080ce826
	movs r2, #255
	mov r10, r2
	mov r3, r10
	ands r3, r6
	mov r10, r3
.L_080ce826:
	adds r1, r7, #0
	mov r2, r10
	bl Func_080ceafc
.L_080ce82e:
	movs r3, #128
	lsls r3, r3, #6
	mov r0, r8
	ands r3, r0
	cmp r3, #0
	beq .L_080ce840
	bl Func_080ce574
	b .L_080ceaee
.L_080ce840:
	ldr r0, [sp, #4]
	bl Func_080ce31c
	adds r6, r0, #0
	movs r0, #128
	lsls r0, r0, #21
	ldr r1, [sp, #4]
	adds r2, r6, #0
	adds r0, #5
	bl Func_080ce458
	adds r2, r6, #0
	str r0, [sp, #8]
	ldr r1, [sp, #4]
	movs r0, #5
	bl Func_080ce458
	mov r8, r0
	movs r0, #160
	lsls r0, r0, #23
	ldr r1, [sp, #4]
	adds r0, #5
	adds r2, r6, #0
	bl Func_080ce458
	mov r1, r9
	mov r11, r0
	cmp r1, #156
	bne .L_080ce8b8
	ldr r3, .L_080ce904
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_080ce910
	cmp r2, r3
	beq .L_080ce8b8
	ldr r1, [sp, #8]
	cmp r1, #0
	bne .L_080ce8b8
	mov r2, r8
	cmp r2, #0
	bne .L_080ce8b8
	mov r3, r11
	cmp r3, #0
	bne .L_080ce8b8
	adds r0, r7, #0
	movs r1, #1
	bl UiText_DrawQuantity
	movs r0, #156
	movs r1, #4
	bl UiText_DrawQuantity
	ldr r0, .L_080ce908
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080ceaec
.L_080ce8b8:
	cmp r7, #7
	bgt .L_080ce920
	mov r0, r9
	bl BattleAction_Get
	adds r5, r0, #0
	adds r0, r7, #0
	bl Owner_GetState
	ldrb r1, [r5, #9]
	movs r2, #58
	ldrsh r3, [r0, r2]
	cmp r3, r1
	bge .L_080ce918
	ldr r3, [sp, #0]
	cmp r3, #0
	beq .L_080ce8e6
	ldr r0, [sp, #12]
	movs r1, #172
	lsls r1, r1, #1
	adds r2, r0, r1
	movs r3, #0
	strh r3, [r2]
.L_080ce8e6:
	adds r0, r7, #0
	movs r1, #1
	bl UiText_DrawQuantity
	mov r0, r9
	movs r1, #4
	bl UiText_DrawQuantity
	ldr r0, .L_080ce914
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080ceaec
.L_080ce900:
	.4byte 0x00000dbb
.L_080ce904:
	.4byte gPartyState
.L_080ce908:
	.4byte 0x00000dbd
.L_080ce90c:
	.4byte 0x00000dbc
.L_080ce910:
	.4byte 0x00000002
.L_080ce914:
	.4byte 0x00000dba
.L_080ce918:
	negs r1, r1
	adds r0, r7, #0
	bl Owner_AdjustSecondValueFar
.L_080ce920:
	ldr r2, [sp, #4]
	cmp r2, #23
	bne .L_080ce948
	bl Func_080cdf5c
	bl ObjectTable_Get
	ldr r3, [r0, #8]
	ldr r1, [r0, #16]
	adds r0, #34
	ldrb r2, [r0]
	adds r0, r3, #0
	bl Func_080dbcd8
	cmp r0, #0
	bne .L_080ce948
	movs r3, #0
	str r3, [sp, #8]
	mov r8, r3
	mov r11, r3
.L_080ce948:
	movs r0, #160
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #66
	adds r0, #255
	bl GameFlag_ClearBit
	ldr r1, [sp, #8]
	movs r0, #1
	negs r0, r0
	mov r10, r0
	cmp r1, #0
	bne .L_080ce970
	mov r2, r8
	cmp r2, #0
	bne .L_080ce970
	mov r3, r11
	cmp r3, #0
	beq .L_080ce9b6
.L_080ce970:
	movs r0, #1
	negs r0, r0
	cmp r6, r0
	beq .L_080ce98c
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_080ce98c
	movs r1, #255
	mov r10, r1
	mov r2, r10
	ands r2, r6
	mov r10, r2
.L_080ce98c:
	movs r5, #66
	adds r5, #255
	adds r0, r5, #0
	bl GameFlag_SetBit
	mov r3, r8
	cmp r3, #0
	beq .L_080ce9b6
	ldrh r2, [r3, #4]
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080ce9b6
	movs r0, #160
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	adds r0, r5, #0
	bl GameFlag_ClearBit
.L_080ce9b6:
	mov r0, r9
	movs r1, #0
	bl Func_080dc410
	movs r1, #192
	ldr r0, [sp, #12]
	lsls r1, r1, #4
	adds r1, #182
	adds r2, r0, r1
	movs r3, #1
	strb r3, [r2]
	ldr r2, [sp, #8]
	movs r1, #0
	cmp r2, #0
	beq .L_080ce9f2
	ldrh r2, [r2, #4]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080ce9f2
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	adds r2, r3, #0
	adds r2, #34
	adds r3, #35
	strb r1, [r2]
	strb r1, [r3]
.L_080ce9f2:
	mov r3, r8
	cmp r3, #0
	beq .L_080cea18
	ldrh r2, [r3, #4]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080cea18
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #0
	adds r1, r3, #0
	adds r1, #34
	adds r3, #35
	strb r2, [r1]
	strb r2, [r3]
.L_080cea18:
	mov r0, r11
	cmp r0, #0
	beq .L_080cea3e
	ldrh r2, [r0, #4]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080cea3e
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #0
	adds r1, r3, #0
	adds r1, #34
	adds r3, #35
	strb r2, [r1]
	strb r2, [r3]
.L_080cea3e:
	bl Func_080cdf5c
	mov r1, r10
	bl Func_080dc62c
	bl Func_080dc6d8
	adds r1, r7, #0
	ldr r0, [sp, #8]
	mov r2, r10
	bl Func_080ceafc
	movs r0, #160
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080cea78
	movs r0, #66
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080cea74
	bl BattleFx_DispatchRequestKind
	b .L_080cea78
.L_080cea74:
	bl BattleFx_Run
.L_080cea78:
	bl Func_080dc7cc
	ldr r1, [sp, #12]
	movs r2, #211
	lsls r2, r2, #4
	adds r5, r1, r2
	movs r2, #0
	ldrsb r2, [r5, r2]
	movs r3, #1
	negs r3, r3
	cmp r2, r3
	beq .L_080ceab0
	movs r3, #128
	movs r0, #128
	lsls r3, r3, #1
	lsls r0, r0, #22
	orrs r2, r3
	adds r0, #5
	ldr r1, [sp, #4]
	bl Func_080ce458
	cmp r0, #0
	beq .L_080ceab0
	movs r2, #0
	ldrsb r2, [r5, r2]
	adds r1, r7, #0
	bl Func_080ceafc
.L_080ceab0:
	movs r5, #160
	mov r0, r8
	adds r1, r7, #0
	mov r2, r10
	lsls r5, r5, #1
	bl Func_080ceafc
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ceacc
	bl BattleFx_ClearChildValueOnMismatch
.L_080ceacc:
	ldr r0, [sp, #12]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #182
	adds r3, r0, r1
	movs r2, #0
	strb r2, [r3]
	bl Func_080dc7e8
	adds r0, r5, #0
	bl GameFlag_ClearBit
	movs r0, #66
	adds r0, #255
	bl GameFlag_ClearBit
.L_080ceaec:
	movs r0, #0
.L_080ceaee:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
