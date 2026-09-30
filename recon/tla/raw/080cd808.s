.syntax unified
	.thumb
	.global Func_080cd808
	.thumb_func
Func_080cd808:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	subs r6, #242
	cmp r6, #5
	bhi .L_080cd832
	bl Func_080d2260
	ldr r3, .L_080cd8fc
	ldr r0, .L_080cd900
	ldrb r5, [r3, r6]
	movs r1, #1
	adds r0, r5, r0
	bl UiText_ShowPositionedMessageAndWaitFar
	ldr r0, .L_080cd904
	movs r1, #1
	adds r0, r5, r0
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080cd8f8
.L_080cd832:
	adds r1, r5, #0
	movs r0, #3
	bl Func_080ccd78
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080cd8e0
	ldr r3, [r5]
	movs r2, #6
	ldrsh r7, [r5, r2]
	asrs r6, r3, #4
	ldrh r2, [r5, #4]
	movs r3, #31
	ands r6, r3
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r2
	cmp r3, #0
	bne .L_080cd874
	cmp r6, #0
	beq .L_080cd874
	bl Func_080d2260
	ldr r0, .L_080cd900
	movs r1, #1
	adds r0, r6, r0
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r0, #161
	lsls r0, r0, #1
	bl GameFlag_SetBit
	b .L_080cd87c
.L_080cd874:
	movs r0, #161
	lsls r0, r0, #1
	bl GameFlag_ClearBit
.L_080cd87c:
	ldr r2, [r5, #8]
	movs r3, #240
	lsls r3, r3, #20
	ands r3, r2
	cmp r3, #0
	bne .L_080cd896
	ldr r3, .L_080cd908
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #15
	cmp r3, r2
	bne .L_080cd8f0
	b .L_080cd8c8
.L_080cd896:
	adds r0, r7, #0
	bl GameFlag_IsConditionActive
	cmp r0, #0
	beq .L_080cd8b0
	ldr r3, .L_080cd90c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	ldr r3, [r5, #8]
	mov lr, r3
	.2byte 0xf800
.L_080cd8b0:
	movs r0, #161
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080cd8f0
	ldr r0, .L_080cd904
	movs r1, #1
	adds r0, r6, r0
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080cd8f0
.L_080cd8c8:
	adds r0, r7, #0
	bl GameFlag_IsConditionActive
	cmp r0, #0
	beq .L_080cd8d6
	ldrh r0, [r5, #8]
	b .L_080cd8d8
.L_080cd8d6:
	ldr r0, .L_080cd910
.L_080cd8d8:
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080cd8f0
.L_080cd8e0:
	ldr r0, .L_080cd914
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	ldr r0, .L_080cd918
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080cd8f0:
	movs r0, #161
	lsls r0, r0, #1
	bl GameFlag_ClearBit
.L_080cd8f8:
	movs r0, #0
	pop {r5, r6, r7, pc}
.L_080cd8fc:
	.4byte Data_080eff22
.L_080cd900:
	.4byte 0x00000dc4
.L_080cd904:
	.4byte 0x00000def
.L_080cd908:
	.4byte 0xfff00000
.L_080cd90c:
	.4byte gPartyState
.L_080cd910:
	.4byte 0x00000e1f
.L_080cd914:
	.4byte 0x00000dc9
.L_080cd918:
	.4byte 0x00000df4
