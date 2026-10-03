.syntax unified
	.thumb
	.global Func_080ce0ac
	.thumb_func
Func_080ce0ac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	movs r2, #1
	sub sp, #8
	mov r11, r3
	negs r2, r2
	str r2, [sp, #0]
	mov r2, r11
	ands r2, r0
	movs r3, #15
	asrs r0, r0, #10
	ands r0, r3
	mov r11, r2
	mov r10, r0
	str r1, [sp, #4]
	bl Party_CountActiveOwnersFar
	mov r2, r10
	movs r7, #0
	mov r8, r0
	cmp r2, #15
	bne .L_080ce136
	movs r3, #0
	mov r10, r3
	movs r6, #0
	cmp r10, r8
	bge .L_080ce15a
	ldr r3, .L_080ce30c
	movs r0, #128
	lsls r0, r0, #1
	movs r2, #134
	adds r0, #255
	lsls r2, r2, #2
	mov r9, r0
	adds r5, r3, r2
.L_080ce102:
	ldrb r0, [r5]
	bl Owner_GetState
	movs r4, #0
	adds r0, #216
	movs r1, #14
.L_080ce10e:
	ldrh r2, [r0]
	mov r3, r9
	ands r3, r2
	adds r0, #2
	cmp r3, r11
	bne .L_080ce11c
	adds r4, #1
.L_080ce11c:
	subs r1, #1
	cmp r1, #0
	bge .L_080ce10e
	cmp r7, r4
	bge .L_080ce12c
	ldrb r3, [r5]
	adds r7, r4, #0
	mov r10, r3
.L_080ce12c:
	adds r6, #1
	adds r5, #1
	cmp r6, r8
	blt .L_080ce102
	b .L_080ce15a
.L_080ce136:
	mov r0, r10
	bl Owner_GetState
	movs r4, #128
	lsls r4, r4, #1
	adds r4, #255
	adds r0, #216
	movs r1, #14
.L_080ce146:
	ldrh r2, [r0]
	adds r3, r4, #0
	ands r3, r2
	adds r0, #2
	cmp r3, r11
	bne .L_080ce154
	adds r7, #1
.L_080ce154:
	subs r1, #1
	cmp r1, #0
	bge .L_080ce146
.L_080ce15a:
	cmp r7, #0
	bne .L_080ce16c
	ldr r0, .L_080ce310
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r0, #1
	negs r0, r0
	b .L_080ce2fe
.L_080ce16c:
	mov r0, r11
	bl Event_FindFacingTrigger
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080ce1ee
	ldr r3, [r6, #8]
	cmp r3, #0
	beq .L_080ce1ee
	movs r0, #68
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #161
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	ldrh r2, [r6, #4]
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r2
	cmp r3, #0
	bne .L_080ce1b2
	mov r0, r10
	movs r1, #1
	bl UiText_DrawQuantity
	mov r0, r11
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r0, .L_080ce314
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080ce1b2:
	ldr r3, [r6, #8]
	movs r0, #128
	lsls r0, r0, #9
	cmp r3, r0
	bge .L_080ce1de
	bl EventRuntime_GetControlledOwner
	bl Func_080cd91c
	adds r5, r0, #0
	bl EventRuntime_Begin
	ldr r0, [r6, #8]
	bl EventRuntime_SetMessage
	adds r0, r5, #0
	movs r1, #0
	bl EventRuntime_ShowMessageAndWait
	bl EventRuntime_End
	b .L_080ce1e8
.L_080ce1de:
	mov r0, r11
	mov r1, r10
	ldr r2, [sp, #4]
	mov lr, r3
	.2byte 0xf800
.L_080ce1e8:
	movs r2, #0
	str r2, [sp, #0]
	b .L_080ce2d4
.L_080ce1ee:
	movs r0, #68
	movs r7, #161
	adds r0, #255
	lsls r7, r7, #1
	bl GameFlag_ClearBit
	adds r0, r7, #0
	bl GameFlag_SetBit
	mov r0, r11
	bl Item_Get
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldrh r5, [r0, #40]
	mov r8, r3
	cmp r5, #0
	beq .L_080ce2d4
	movs r0, #70
	adds r0, #255
	bl GameFlag_SetBit
	adds r0, r7, #0
	bl GameFlag_ClearBit
	cmp r5, #149
	bne .L_080ce282
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ce282
	mov r0, r11
	movs r1, #2
	bl UiText_DrawQuantity
	movs r1, #13
	ldr r0, .L_080ce318
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r0, #1
	bl Func_080d295c
	adds r6, r0, #0
	bl UiWork_FinalizePendingCoreFar
	movs r0, #0
	cmp r6, #0
	bne .L_080ce2fe
	ldr r1, .L_080ce30c
	movs r0, #152
	lsls r0, r0, #2
	adds r3, r1, r0
	ldrh r2, [r3]
	subs r0, #128
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
	movs r2, #172
	movs r3, #186
	lsls r2, r2, #1
	lsls r3, r3, #2
	add r2, r8
	adds r3, #255
	strh r3, [r2]
.L_080ce282:
	mov r0, r10
	movs r1, #1
	bl UiText_DrawQuantity
	movs r6, #192
	mov r0, r11
	movs r1, #2
	bl UiText_DrawQuantity
	lsls r6, r6, #4
	ldr r0, .L_080ce314
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	adds r6, #182
	adds r0, r5, #0
	movs r1, #0
	bl Func_080dc410
	add r6, r8
	movs r2, #0
	movs r5, #1
	mov r8, r2
	strb r5, [r6]
	bl BattleFx_Run
	mov r3, r8
	strb r3, [r6]
	bl Func_080dc7e8
	mov r0, r11
	bl Item_Get
	ldrb r3, [r0, #12]
	ands r5, r3
	cmp r5, #0
	beq .L_080ce2d4
	movs r0, #68
	adds r0, #255
	bl GameFlag_SetBit
.L_080ce2d4:
	movs r0, #161
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ce2e8
	ldr r0, .L_080ce310
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080ce2e8:
	movs r0, #68
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ce2fc
	mov r0, r10
	ldr r1, [sp, #4]
	bl Inventory_RemoveFar
.L_080ce2fc:
	ldr r0, [sp, #0]
.L_080ce2fe:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ce30c:
	.4byte gPartyState
.L_080ce310:
	.4byte 0x00000dc3
.L_080ce314:
	.4byte 0x00000d94
.L_080ce318:
	.4byte 0x00000dc0
