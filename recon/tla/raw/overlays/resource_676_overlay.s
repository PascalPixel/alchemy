.syntax unified
	.thumb
	.section .text.x02008088,"ax",%progbits
	.global Func_02000088
	.thumb_func
Func_02000088:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	mov r11, r3
	adds r3, r5, #0
	adds r3, #99
	ldrb r3, [r3]
	mov r10, r2
	movs r2, #0
	mov r8, r1
	mov r9, r2
	cmp r3, #0
	beq .L_020080b8
	adds r2, r5, #0
	adds r2, #91
	movs r3, #1
	strb r3, [r2]
	movs r0, #1
	b .L_0200813a
.L_020080b8:
	mov r6, r8
	adds r7, r5, #0
	adds r6, #8
	adds r7, #8
	adds r0, r6, #0
	adds r1, r7, #0
	bl SceneActor_CalculateFixedPointDistance
	cmp r0, r10
	blt .L_020080d2
	mov r3, r11
	cmp r3, #0
	beq .L_02008128
.L_020080d2:
	mov r2, r8
	ldr r0, [r2, #16]
	ldr r3, [r5, #16]
	ldr r1, [r6]
	subs r0, r0, r3
	ldr r3, [r7]
	subs r1, r1, r3
	bl ArcTan2
	ldr r3, .L_02008148
	lsls r0, r0, #16
	movs r2, #128
	lsrs r0, r0, #16
	lsls r2, r2, #5
	adds r1, r0, r2
	ldrh r2, [r5, #6]
	adds r4, r0, r3
	movs r3, #240
	lsls r3, r3, #8
	ands r4, r3
	ands r1, r3
	ands r0, r3
	ands r3, r2
	cmp r0, r3
	beq .L_02008112
	cmp r1, r3
	beq .L_02008112
	cmp r4, r3
	beq .L_02008112
	mov r3, r11
	cmp r3, #0
	beq .L_02008138
.L_02008112:
	adds r2, r5, #0
	adds r2, #91
	movs r3, #1
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #1
	bl Func_02002448
	movs r2, #1
	mov r9, r2
	b .L_02008138
.L_02008128:
	adds r3, r5, #0
	adds r3, #91
	mov r2, r9
	strb r2, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl Func_02002448
.L_02008138:
	mov r0, r9
.L_0200813a:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008148:
	.4byte 0xfffff000
	.section .text.x0200814c,"ax",%progbits
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #60]
	ldr r3, [r3, #108]
	mov r10, r2
	mov r8, r3
	ldr r3, .L_02008200
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	adds r1, r5, #0
	adds r0, #8
	adds r1, #8
	movs r7, #0
	bl SceneActor_CalculateFixedPointDistance
	cmp r0, #11
	bgt .L_02008190
	adds r3, r5, #0
	adds r3, #91
	adds r0, r5, #0
	strb r7, [r3]
	movs r1, #2
	bl Func_02002448
	b .L_020081f4
.L_02008190:
	adds r6, r5, #0
	adds r6, #100
	ldrh r2, [r6]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020081a2
	movs r0, #10
	b .L_020081a4
.L_020081a2:
	movs r0, #9
.L_020081a4:
	bl Object_GetById
	adds r1, r0, #0
	adds r0, r5, #0
	movs r2, #24
	movs r3, #0
	bl Func_02000088
	cmp r0, #0
	bne .L_020081f4
	ldr r3, .L_02008200
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #176
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r1, r0, #0
	cmp r3, #0
	bne .L_020081de
	mov r2, r10
	ldrb r3, [r2, #4]
	cmp r3, #0
	beq .L_020081ea
.L_020081de:
	ldrh r2, [r6]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_020081ea
	movs r7, #1
.L_020081ea:
	adds r0, r5, #0
	movs r2, #24
	adds r3, r7, #0
	bl Func_02000088
.L_020081f4:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008200:
	.4byte gPartyState
	.section .text.x02008270,"ax",%progbits
	.global Func_02000270
	.thumb_func
Func_02000270:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	bl Func_02002488
	movs r0, #0
	bl Func_02002590
	movs r5, #8
.L_02008284:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008296
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008296:
	adds r5, #1
	cmp r5, #63
	bls .L_02008284
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r6, [r3, r2]
	movs r0, #158
	bl Func_020025a0
	subs r6, #1
	ldr r0, .L_02008320
	lsls r4, r6, #3
	adds r3, r4, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r4]
	bl Func_02002460
	ldr r5, .L_02008324
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	cmp r6, #7
	bne .L_020082f4
	movs r2, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	b .L_020082fe
.L_020082f4:
	movs r2, #4
	movs r1, #2
	negs r2, r2
	bl ObjectMotion_CenterOnColumn
.L_020082fe:
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02002578
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02002490
	pop {r5, r6, r7, pc}
.L_02008320:
	.4byte Data_02002ce4
.L_02008324:
	.4byte gPartyState
	.section .text.x020086bc,"ax",%progbits
	.balign 4
	.global Func_020006bc
	.thumb_func
Func_020006bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	bl Object_GetById
	movs r2, #98
	adds r5, r0, #0
	adds r2, r2, r5
	ldrb r3, [r2]
	mov r8, r2
	cmp r3, #1
	bne .L_020086da
	bl .L_0200967e
.L_020086da:
	adds r7, r5, #0
	adds r7, #100
	cmp r3, #1
	bgt .L_020086e8
	cmp r3, #0
	beq .L_020086f2
	b .L_02008724
.L_020086e8:
	cmp r3, #2
	beq .L_02008704
	cmp r3, #3
	beq .L_020086fc
	b .L_02008724
.L_020086f2:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02002448
	b .L_02008724
.L_020086fc:
	adds r0, r5, #0
	movs r1, #1
	bl Func_02002448
.L_02008704:
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	ldr r1, [r5, #8]
	adds r0, r5, #0
	bl Func_02002450
	ldrh r2, [r7]
	movs r3, #2
	negs r3, r3
	ands r3, r2
	strh r3, [r7]
	mov r2, r8
	movs r3, #1
	strb r3, [r2]
	bl .L_0200967e
.L_02008724:
	movs r2, #0
	ldrsh r3, [r7, r2]
	movs r2, #162
	lsls r2, r2, #1
	cmp r3, r2
	bcc .L_02008734
	bl .L_0200967e
.L_02008734:
	ldr r2, .L_0200873c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	b .L_02008740
.L_0200873c:
	.4byte .L_02008744
.L_02008740:
	mov pc, r3
	.2byte 0x0000
.L_02008744:
	.4byte .L_02008c54
	.4byte .L_0200966e
	.4byte .L_02008c66
	.4byte .L_0200966e
	.4byte .L_02008c78
	.4byte .L_0200966e
	.4byte .L_02008c8a
	.4byte .L_0200966e
	.4byte .L_02008c9c
	.4byte .L_0200966e
	.4byte .L_02008cae
	.4byte .L_0200966e
	.4byte .L_02008cc0
	.4byte .L_0200966e
	.4byte .L_02008cd2
	.4byte .L_0200966e
	.4byte .L_02008ce4
	.4byte .L_0200966e
	.4byte .L_02008cf6
	.4byte .L_0200966e
	.4byte .L_02008d08
	.4byte .L_0200966e
	.4byte .L_02008d1a
	.4byte .L_0200966e
	.4byte .L_02008d2c
	.4byte .L_0200966e
	.4byte .L_02008d3e
	.4byte .L_0200966e
	.4byte .L_02008d50
	.4byte .L_0200966e
	.4byte .L_02008d62
	.4byte .L_0200966e
	.4byte .L_02008d74
	.4byte .L_0200966e
	.4byte .L_02008d86
	.4byte .L_0200966e
	.4byte .L_02008daa
	.4byte .L_0200966e
	.4byte .L_02008dbc
	.4byte .L_0200966e
	.4byte .L_02008dce
	.4byte .L_0200966e
	.4byte .L_02008de0
	.4byte .L_0200966e
	.4byte .L_02008dfc
	.4byte .L_0200966e
	.4byte .L_02008e0e
	.4byte .L_0200966e
	.4byte .L_02008e20
	.4byte .L_0200966e
	.4byte .L_02008e32
	.4byte .L_0200966e
	.4byte .L_02008e44
	.4byte .L_0200966e
	.4byte .L_02008e56
	.4byte .L_0200966e
	.4byte .L_02008e68
	.4byte .L_0200966e
	.4byte .L_02008e7a
	.4byte .L_0200966e
	.4byte .L_02008e8a
	.4byte .L_0200966e
	.4byte .L_02008e9a
	.4byte .L_0200966e
	.4byte .L_02008eaa
	.4byte .L_0200966e
	.4byte .L_02008eba
	.4byte .L_0200966e
	.4byte .L_02008eca
	.4byte .L_0200966e
	.4byte .L_02008eda
	.4byte .L_0200966e
	.4byte .L_02008eea
	.4byte .L_0200966e
	.4byte .L_02008efa
	.4byte .L_0200966e
	.4byte .L_02008f0a
	.4byte .L_0200966e
	.4byte .L_02008f1a
	.4byte .L_0200966e
	.4byte .L_02008f2a
	.4byte .L_0200966e
	.4byte .L_02008f3a
	.4byte .L_0200966e
	.4byte .L_02008f4a
	.4byte .L_0200966e
	.4byte .L_02008f5a
	.4byte .L_0200966e
	.4byte .L_02008f6a
	.4byte .L_0200966e
	.4byte .L_02008f7a
	.4byte .L_0200966e
	.4byte .L_02008f8a
	.4byte .L_0200966e
	.4byte .L_02008f9a
	.4byte .L_0200966e
	.4byte .L_02008faa
	.4byte .L_0200966e
	.4byte .L_02008fba
	.4byte .L_0200966e
	.4byte .L_02008fca
	.4byte .L_0200966e
	.4byte .L_02008fda
	.4byte .L_0200966e
	.4byte .L_02008fea
	.4byte .L_0200966e
	.4byte .L_02008ffa
	.4byte .L_0200966e
	.4byte .L_0200900a
	.4byte .L_0200966e
	.4byte .L_0200901a
	.4byte .L_0200966e
	.4byte .L_0200902a
	.4byte .L_0200966e
	.4byte .L_0200903a
	.4byte .L_0200966e
	.4byte .L_0200904a
	.4byte .L_0200966e
	.4byte .L_0200905a
	.4byte .L_0200966e
	.4byte .L_0200906a
	.4byte .L_0200966e
	.4byte .L_0200907a
	.4byte .L_0200966e
	.4byte .L_0200908a
	.4byte .L_0200966e
	.4byte .L_0200909a
	.4byte .L_0200966e
	.4byte .L_020090aa
	.4byte .L_0200966e
	.4byte .L_020090ba
	.4byte .L_0200966e
	.4byte .L_020090ca
	.4byte .L_0200966e
	.4byte .L_020090da
	.4byte .L_0200966e
	.4byte .L_020090ea
	.4byte .L_0200966e
	.4byte .L_020090fa
	.4byte .L_0200966e
	.4byte .L_0200910a
	.4byte .L_0200966e
	.4byte .L_0200911a
	.4byte .L_0200966e
	.4byte .L_0200912a
	.4byte .L_0200966e
	.4byte .L_0200913a
	.4byte .L_0200966e
	.4byte .L_0200914a
	.4byte .L_0200966e
	.4byte .L_0200915a
	.4byte .L_0200966e
	.4byte .L_0200916a
	.4byte .L_0200966e
	.4byte .L_02009180
	.4byte .L_0200966e
	.4byte .L_02009190
	.4byte .L_0200966e
	.4byte .L_0200919e
	.4byte .L_0200966e
	.4byte .L_020091ac
	.4byte .L_0200966e
	.4byte .L_020091ba
	.4byte .L_0200966e
	.4byte .L_020091c8
	.4byte .L_0200966e
	.4byte .L_020091d6
	.4byte .L_0200966e
	.4byte .L_020091e6
	.4byte .L_0200966e
	.4byte .L_020091f6
	.4byte .L_0200966e
	.4byte .L_02009206
	.4byte .L_0200966e
	.4byte .L_02009214
	.4byte .L_0200966e
	.4byte .L_02009222
	.4byte .L_0200966e
	.4byte .L_02009230
	.4byte .L_0200966e
	.4byte .L_0200923e
	.4byte .L_0200966e
	.4byte .L_0200924c
	.4byte .L_0200966e
	.4byte .L_0200925c
	.4byte .L_0200966e
	.4byte .L_0200926c
	.4byte .L_0200966e
	.4byte .L_0200927c
	.4byte .L_0200966e
	.4byte .L_0200928a
	.4byte .L_0200966e
	.4byte .L_02009298
	.4byte .L_0200966e
	.4byte .L_020092a6
	.4byte .L_0200966e
	.4byte .L_020092b4
	.4byte .L_0200966e
	.4byte .L_020092c2
	.4byte .L_0200966e
	.4byte .L_020092d2
	.4byte .L_0200966e
	.4byte .L_020092e2
	.4byte .L_0200966e
	.4byte .L_020092f2
	.4byte .L_0200966e
	.4byte .L_02009300
	.4byte .L_0200966e
	.4byte .L_0200930e
	.4byte .L_0200966e
	.4byte .L_0200931c
	.4byte .L_0200966e
	.4byte .L_0200932a
	.4byte .L_0200966e
	.4byte .L_02009338
	.4byte .L_0200966e
	.4byte .L_02009346
	.4byte .L_0200966e
	.4byte .L_02009354
	.4byte .L_0200966e
	.4byte .L_02009362
	.4byte .L_0200966e
	.4byte .L_02009370
	.4byte .L_020093e6
	.4byte .L_020093f6
	.4byte .L_0200966e
	.4byte .L_0200937e
	.4byte .L_0200966e
	.4byte .L_0200938e
	.4byte .L_0200966e
	.4byte .L_0200939e
	.4byte .L_0200966e
	.4byte .L_020093ae
	.4byte .L_0200966e
	.4byte .L_020093bc
	.4byte .L_0200966e
	.4byte .L_020093ca
	.4byte .L_0200966e
	.4byte .L_020093d8
	.4byte .L_020093e6
	.4byte .L_020093f6
	.4byte .L_0200966e
	.4byte .L_0200940e
	.4byte .L_0200966e
	.4byte .L_0200941e
	.4byte .L_0200966e
	.4byte .L_0200942e
	.4byte .L_0200966e
	.4byte .L_0200943e
	.4byte .L_0200966e
	.4byte .L_0200944c
	.4byte .L_0200966e
	.4byte .L_0200945a
	.4byte .L_0200966e
	.4byte .L_02009468
	.4byte .L_0200966e
	.4byte .L_02009488
	.4byte .L_0200966e
	.4byte .L_02009496
	.4byte .L_0200966e
	.4byte .L_020094ae
	.4byte .L_0200966e
	.4byte .L_020094bc
	.4byte .L_0200966e
	.4byte .L_020094ca
	.4byte .L_0200966e
	.4byte .L_020094d8
	.4byte .L_0200966e
	.4byte .L_020094e6
	.4byte .L_0200966e
	.4byte .L_020094f4
	.4byte .L_0200966e
	.4byte .L_02009502
	.4byte .L_0200966e
	.4byte .L_02009510
	.4byte .L_0200966e
	.4byte .L_0200951e
	.4byte .L_0200966e
	.4byte .L_0200952c
	.4byte .L_0200966e
	.4byte .L_0200953a
	.4byte .L_0200966e
	.4byte .L_02009548
	.4byte .L_0200966e
	.4byte .L_02009556
	.4byte .L_0200966e
	.4byte .L_02009564
	.4byte .L_0200966e
	.4byte .L_02009572
	.4byte .L_0200966e
	.4byte .L_02009580
	.4byte .L_0200966e
	.4byte .L_0200958e
	.4byte .L_0200966e
	.4byte .L_0200959c
	.4byte .L_0200966e
	.4byte .L_020095aa
	.4byte .L_0200966e
	.4byte .L_020095b8
	.4byte .L_0200966e
	.4byte .L_020095c6
	.4byte .L_0200966e
	.4byte .L_020095d4
	.4byte .L_0200966e
	.4byte .L_020095e2
	.4byte .L_0200966e
	.4byte .L_020095f0
	.4byte .L_0200966e
	.4byte .L_020095fe
	.4byte .L_0200966e
	.4byte .L_0200960c
	.4byte .L_0200966e
	.4byte .L_0200961a
	.4byte .L_0200966e
	.4byte .L_02009628
	.4byte .L_0200966e
	.4byte .L_02009636
	.4byte .L_0200966e
	.4byte .L_02009644
	.4byte .L_0200966e
	.4byte .L_02009652
	.4byte .L_0200966e
	.4byte .L_02009660
	.4byte .L_0200966e
.L_02008c54:
	movs r1, #172
	movs r2, #206
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008c66:
	movs r1, #162
	movs r2, #205
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008c78:
	movs r1, #160
	movs r2, #202
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008c8a:
	movs r1, #162
	movs r2, #199
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008c9c:
	movs r1, #172
	movs r2, #198
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008cae:
	movs r1, #182
	movs r2, #199
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008cc0:
	movs r1, #184
	movs r2, #202
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008cd2:
	movs r1, #182
	movs r2, #205
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008ce4:
	movs r1, #172
	movs r2, #206
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008cf6:
	movs r1, #162
	movs r2, #205
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008d08:
	movs r1, #160
	movs r2, #202
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008d1a:
	movs r1, #162
	movs r2, #199
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008d2c:
	movs r1, #172
	movs r2, #198
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008d3e:
	movs r1, #182
	movs r2, #199
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008d50:
	movs r1, #184
	movs r2, #202
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008d62:
	movs r1, #182
	movs r2, #205
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008d74:
	movs r1, #172
	movs r2, #206
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008d86:
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #9
	adds r0, r6, #0
	adds r1, #204
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #182
	movs r2, #205
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008daa:
	movs r1, #184
	movs r2, #202
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008dbc:
	movs r1, #196
	movs r2, #202
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008dce:
	movs r1, #196
	movs r2, #186
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008de0:
	adds r0, r6, #0
	ldr r1, .L_0200917c
	ldr r2, .L_0200917c
	bl ObjectMotion_SetSpeedParameters
	movs r1, #186
	movs r2, #185
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008dfc:
	movs r1, #184
	movs r2, #182
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008e0e:
	movs r1, #186
	movs r2, #179
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008e20:
	movs r1, #196
	movs r2, #178
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008e32:
	movs r1, #206
	movs r2, #179
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008e44:
	movs r1, #208
	movs r2, #182
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008e56:
	movs r1, #206
	movs r2, #185
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008e68:
	movs r1, #196
	movs r2, #186
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
.L_02008e7a:
	movs r1, #186
	movs r2, #185
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008e8a:
	movs r1, #184
	movs r2, #182
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008e9a:
	movs r1, #186
	movs r2, #179
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008eaa:
	movs r1, #196
	movs r2, #178
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008eba:
	movs r1, #206
	movs r2, #179
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008eca:
	movs r1, #208
	movs r2, #182
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008eda:
	movs r1, #206
	movs r2, #185
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008eea:
	movs r1, #196
	movs r2, #186
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008efa:
	movs r1, #186
	movs r2, #185
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008f0a:
	movs r1, #184
	movs r2, #182
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008f1a:
	movs r1, #186
	movs r2, #179
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008f2a:
	movs r1, #196
	movs r2, #178
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008f3a:
	movs r1, #206
	movs r2, #179
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008f4a:
	movs r1, #208
	movs r2, #182
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008f5a:
	movs r1, #206
	movs r2, #185
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008f6a:
	movs r1, #196
	movs r2, #186
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008f7a:
	movs r1, #186
	movs r2, #185
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008f8a:
	movs r1, #184
	movs r2, #182
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008f9a:
	movs r1, #186
	movs r2, #179
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008faa:
	movs r1, #196
	movs r2, #178
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008fba:
	movs r1, #196
	movs r2, #174
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008fca:
	movs r1, #206
	movs r2, #173
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008fda:
	movs r1, #208
	movs r2, #170
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008fea:
	movs r1, #206
	movs r2, #167
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02008ffa:
	movs r1, #196
	movs r2, #166
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200900a:
	movs r1, #186
	movs r2, #167
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200901a:
	movs r1, #184
	movs r2, #170
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200902a:
	movs r1, #160
	movs r2, #170
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200903a:
	movs r1, #158
	movs r2, #173
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200904a:
	movs r1, #148
	movs r2, #174
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200905a:
	movs r1, #138
	movs r2, #173
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200906a:
	movs r1, #136
	movs r2, #170
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200907a:
	movs r1, #138
	movs r2, #167
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200908a:
	movs r1, #148
	movs r2, #166
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200909a:
	movs r1, #158
	movs r2, #167
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020090aa:
	movs r1, #160
	movs r2, #170
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020090ba:
	movs r1, #158
	movs r2, #173
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020090ca:
	movs r1, #148
	movs r2, #174
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020090da:
	movs r1, #138
	movs r2, #173
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020090ea:
	movs r1, #136
	movs r2, #170
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020090fa:
	movs r1, #138
	movs r2, #167
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200910a:
	movs r1, #148
	movs r2, #166
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200911a:
	movs r1, #158
	movs r2, #167
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200912a:
	movs r1, #160
	movs r2, #170
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200913a:
	movs r1, #158
	movs r2, #173
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200914a:
	movs r1, #148
	movs r2, #174
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200915a:
	movs r1, #148
	movs r2, #182
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200916a:
	movs r1, #136
	movs r2, #182
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
	.2byte 0x0000
.L_0200917c:
	.4byte 0x00013333
.L_02009180:
	movs r1, #134
	movs r2, #185
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009190:
	movs r2, #186
	adds r0, r6, #0
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200919e:
	movs r2, #185
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020091ac:
	movs r2, #182
	adds r0, r6, #0
	movs r1, #224
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020091ba:
	movs r2, #179
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020091c8:
	movs r2, #178
	adds r0, r6, #0
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020091d6:
	movs r1, #134
	movs r2, #179
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020091e6:
	movs r1, #136
	movs r2, #182
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020091f6:
	movs r1, #134
	movs r2, #185
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009206:
	movs r2, #186
	adds r0, r6, #0
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009214:
	movs r2, #185
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009222:
	movs r2, #182
	adds r0, r6, #0
	movs r1, #224
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009230:
	movs r2, #179
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200923e:
	movs r2, #178
	adds r0, r6, #0
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200924c:
	movs r1, #134
	movs r2, #179
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200925c:
	movs r1, #136
	movs r2, #182
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200926c:
	movs r1, #134
	movs r2, #185
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200927c:
	movs r2, #186
	adds r0, r6, #0
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200928a:
	movs r2, #185
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009298:
	movs r2, #182
	adds r0, r6, #0
	movs r1, #224
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020092a6:
	movs r2, #179
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020092b4:
	movs r2, #178
	adds r0, r6, #0
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020092c2:
	movs r1, #134
	movs r2, #179
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020092d2:
	movs r1, #136
	movs r2, #182
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020092e2:
	movs r1, #134
	movs r2, #185
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020092f2:
	movs r2, #186
	adds r0, r6, #0
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009300:
	movs r2, #190
	adds r0, r6, #0
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200930e:
	movs r2, #191
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200931c:
	movs r2, #194
	adds r0, r6, #0
	movs r1, #224
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200932a:
	movs r2, #197
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009338:
	movs r2, #198
	adds r0, r6, #0
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009346:
	movs r2, #210
	adds r0, r6, #0
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009354:
	movs r2, #211
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009362:
	movs r2, #214
	adds r0, r6, #0
	movs r1, #224
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009370:
	movs r2, #217
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200937e:
	movs r1, #134
	movs r2, #217
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200938e:
	movs r1, #136
	movs r2, #214
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200939e:
	movs r1, #134
	movs r2, #211
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020093ae:
	movs r2, #210
	adds r0, r6, #0
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020093bc:
	movs r2, #211
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020093ca:
	movs r2, #214
	adds r0, r6, #0
	movs r1, #224
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020093d8:
	movs r2, #217
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020093e6:
	adds r0, r5, #0
	bl Func_020006a4
	cmp r0, #0
	beq .L_020093f6
	ldrh r3, [r7]
	adds r3, #1
	strh r3, [r7]
.L_020093f6:
	movs r2, #218
	lsls r2, r2, #2
	adds r0, r6, #0
	movs r1, #248
	bl ObjectMotion_ResetAndSetPosition
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200967e
.L_0200940e:
	movs r1, #134
	movs r2, #217
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200941e:
	movs r1, #136
	movs r2, #214
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200942e:
	movs r1, #134
	movs r2, #211
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200943e:
	movs r2, #210
	adds r0, r6, #0
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200944c:
	movs r2, #211
	adds r0, r6, #0
	movs r1, #228
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200945a:
	movs r2, #214
	adds r0, r6, #0
	movs r1, #224
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009468:
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #9
	adds r0, r6, #0
	adds r1, #204
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #214
	adds r0, r6, #0
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009488:
	movs r2, #186
	adds r0, r6, #0
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009496:
	adds r0, r6, #0
	ldr r1, .L_02009684
	ldr r2, .L_02009684
	bl ObjectMotion_SetSpeedParameters
	movs r2, #185
	adds r0, r6, #0
	movs r1, #180
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020094ae:
	movs r2, #182
	adds r0, r6, #0
	movs r1, #176
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020094bc:
	movs r2, #179
	adds r0, r6, #0
	movs r1, #180
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020094ca:
	movs r2, #178
	adds r0, r6, #0
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020094d8:
	movs r2, #179
	adds r0, r6, #0
	movs r1, #220
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020094e6:
	movs r2, #182
	adds r0, r6, #0
	movs r1, #224
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020094f4:
	movs r2, #185
	adds r0, r6, #0
	movs r1, #220
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009502:
	movs r2, #186
	adds r0, r6, #0
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009510:
	movs r2, #185
	adds r0, r6, #0
	movs r1, #180
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200951e:
	movs r2, #182
	adds r0, r6, #0
	movs r1, #176
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200952c:
	movs r2, #179
	adds r0, r6, #0
	movs r1, #180
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200953a:
	movs r2, #178
	adds r0, r6, #0
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009548:
	movs r2, #179
	adds r0, r6, #0
	movs r1, #220
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009556:
	movs r2, #182
	adds r0, r6, #0
	movs r1, #224
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009564:
	movs r2, #185
	adds r0, r6, #0
	movs r1, #220
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009572:
	movs r2, #186
	adds r0, r6, #0
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009580:
	movs r2, #185
	adds r0, r6, #0
	movs r1, #180
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200958e:
	movs r2, #182
	adds r0, r6, #0
	movs r1, #176
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200959c:
	movs r2, #179
	adds r0, r6, #0
	movs r1, #180
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020095aa:
	movs r2, #178
	adds r0, r6, #0
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020095b8:
	movs r2, #179
	adds r0, r6, #0
	movs r1, #220
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020095c6:
	movs r2, #182
	adds r0, r6, #0
	movs r1, #224
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020095d4:
	movs r2, #185
	adds r0, r6, #0
	movs r1, #220
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020095e2:
	movs r2, #186
	adds r0, r6, #0
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020095f0:
	movs r2, #185
	adds r0, r6, #0
	movs r1, #180
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_020095fe:
	movs r2, #182
	adds r0, r6, #0
	movs r1, #176
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200960c:
	movs r2, #179
	adds r0, r6, #0
	movs r1, #180
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200961a:
	movs r2, #178
	adds r0, r6, #0
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009628:
	movs r2, #170
	adds r0, r6, #0
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009636:
	movs r2, #169
	adds r0, r6, #0
	movs r1, #212
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009644:
	movs r2, #166
	adds r0, r6, #0
	movs r1, #216
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009652:
	movs r2, #163
	adds r0, r6, #0
	movs r1, #212
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_02009660:
	movs r2, #162
	adds r0, r6, #0
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009678
.L_0200966e:
	adds r0, r5, #0
	bl Func_020006a4
	cmp r0, #0
	beq .L_0200967e
.L_02009678:
	ldrh r3, [r7]
	adds r3, #1
	strh r3, [r7]
.L_0200967e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009684:
	.4byte 0x00013333
	.section .text.x0200982c,"ax",%progbits
	.balign 4
	.global Func_0200182c
	.thumb_func
Func_0200182c:
	push {r5, lr}
	movs r0, #8
	bl Object_GetById
	ldr r5, .L_020098f8
	adds r1, r0, #0
	ldr r2, [r5]
	adds r3, r1, #0
	adds r3, #100
	strh r2, [r3]
	adds r2, r1, #0
	ldr r3, [r5, #4]
	adds r2, #102
	strh r3, [r2]
	subs r2, #4
	ldr r3, [r5, #8]
	movs r0, #10
	strb r3, [r2]
	adds r2, #1
	ldr r3, [r5, #12]
	strb r3, [r2]
	bl Object_GetById
	ldr r2, [r5, #16]
	adds r1, r0, #0
	adds r3, r1, #0
	adds r3, #100
	strh r2, [r3]
	adds r2, r1, #0
	ldr r3, [r5, #20]
	adds r2, #102
	strh r3, [r2]
	subs r2, #4
	ldr r3, [r5, #24]
	movs r0, #9
	strb r3, [r2]
	adds r2, #1
	ldr r3, [r5, #28]
	strb r3, [r2]
	bl Object_GetById
	ldr r2, [r5, #32]
	adds r1, r0, #0
	adds r3, r1, #0
	adds r3, #100
	strh r2, [r3]
	adds r2, r1, #0
	ldr r3, [r5, #36]
	adds r2, #102
	strh r3, [r2]
	subs r2, #4
	ldr r3, [r5, #40]
	movs r0, #11
	strb r3, [r2]
	adds r2, #1
	ldr r3, [r5, #44]
	strb r3, [r2]
	bl Object_GetById
	ldr r2, [r5, #48]
	adds r1, r0, #0
	adds r3, r1, #0
	adds r3, #100
	strh r2, [r3]
	adds r2, r1, #0
	ldr r3, [r5, #52]
	adds r2, #102
	strh r3, [r2]
	subs r2, #4
	ldr r3, [r5, #56]
	strb r3, [r2]
	adds r2, #1
	ldr r3, [r5, #60]
	strb r3, [r2]
	ldr r3, .L_020098fc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r2, [r5, #64]
	adds r1, r0, #0
	adds r3, r1, #0
	adds r3, #100
	strh r2, [r3]
	adds r2, r1, #0
	ldr r3, [r5, #68]
	adds r2, #102
	strh r3, [r2]
	subs r2, #4
	ldr r3, [r5, #72]
	movs r1, #144
	strb r3, [r2]
	adds r2, #1
	ldr r3, [r5, #76]
	lsls r1, r1, #3
	strb r3, [r2]
	ldr r0, .L_02009900
	bl Scheduler_AddOrUpdateCallback
	pop {r5, pc}
.L_020098f8:
	.4byte Data_02002fc0
.L_020098fc:
	.4byte gPartyState
.L_02009900:
	.4byte Func_02001688
	.section .text.x02009904,"ax",%progbits
	.global Func_02001904
	.thumb_func
Func_02001904:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Object_GetById
	ldr r2, .L_02009940
	adds r6, r0, #0
	ldr r1, .L_02009940
	adds r0, r5, #0
	bl ObjectMotion_SetSpeedParameters
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetModeById
	adds r2, r6, #0
	movs r3, #0
	adds r2, #100
	strh r3, [r2]
	adds r2, #2
	strh r3, [r2]
	ldr r1, .L_0200993c
	subs r2, #4
	movs r3, #1
	strb r3, [r2]
	adds r3, r6, #0
	adds r3, #99
	strb r1, [r3]
	pop {r5, r6, pc}
.L_0200993c:
	.4byte 0x00000000
.L_02009940:
	.4byte 0x00013333
	.section .text.x02009944,"ax",%progbits
	.global Func_02001944
	.thumb_func
Func_02001944:
	push {r5, r6, r7, lr}
	movs r0, #8
	bl Object_GetById
	adds r7, r0, #0
	ldr r0, .L_02009cd8
	bl Func_02002520
	movs r1, #0
	movs r0, #8
	bl Func_02002538
	movs r0, #9
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #10
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #9
	bl Object_GetById
	movs r5, #0
	str r5, [r0, #108]
	movs r0, #10
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #1
	bl WaitFrames
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009cdc
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #10
	ldr r1, .L_02009cdc
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02009ce0
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009ce4
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	bl Func_02002570
	adds r0, #85
	strb r5, [r0]
	movs r1, #200
	movs r0, #204
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02002560
	movs r0, #152
	movs r1, #1
	lsls r0, r0, #17
	negs r1, r1
	ldr r2, .L_02009ce8
	movs r3, #1
	bl Motion_CamBounds
	ldr r3, .L_02009cec
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #148
	movs r2, #218
	lsls r2, r2, #2
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	ldr r0, [r5]
	bl Func_02002548
	movs r0, #78
	bl Func_020025a0
	movs r1, #176
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	bl Func_02002548
	movs r0, #8
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #8
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_02009cdc
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #11
	ldr r1, .L_02009cdc
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02009cf0
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009cf4
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009cf8
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009cfc
	movs r0, #11
	bl Object_SetActionCallbackAndRefreshById
	movs r1, #148
	movs r2, #218
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r0, #5
	bl Func_020024f8
	movs r0, #1
	bl WaitFrames
	movs r1, #224
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02009d00
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #192
	movs r1, #156
	lsls r2, r2, #2
	adds r2, #86
	movs r0, #5
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	bl Func_02002548
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009af0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_02009af0:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020024f8
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02009cdc
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #158
	movs r2, #224
	ldr r0, [r5]
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #172
	movs r2, #228
	lsls r2, r2, #2
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_02002548
	movs r0, #17
	bl Func_020025a0
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	bl Func_02002548
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r0, #8
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	ldr r1, .L_02009d04
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009d08
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009d0c
	movs r0, #9
	bl Object_SetActionCallbackAndRefreshById
	movs r2, #192
	movs r1, #172
	lsls r2, r2, #2
	adds r2, #122
	movs r0, #11
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02002548
	movs r0, #8
	bl Func_02001904
	movs r0, #10
	bl Func_02001904
	movs r0, #9
	bl Func_02001904
	movs r0, #11
	bl Func_02001904
	ldr r0, [r5]
	bl Func_02001904
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009d10
	bl Scheduler_AddOrUpdateCallback
	adds r5, r7, #0
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	adds r5, #102
	movs r3, #1
	strh r3, [r5]
.L_02009bbe:
	adds r6, r7, #0
	movs r0, #1
	adds r6, #100
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #34
	bne .L_02009bbe
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009be6:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #42
	bne .L_02009be6
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009c0a:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #108
	bne .L_02009c0a
	movs r3, #3
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	bl Func_02001730
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #4
	movs r2, #40
	bl ObjectMotion_Launch
	ldr r5, .L_02009cec
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02002548
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009cb2
	bl Func_02001d14
	b .L_02009cd6
.L_02009cb2:
	movs r1, #0
	movs r2, #20
	movs r0, #8
	bl Func_02002530
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #123
	bl Func_020025a0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #15
	bl Func_02002578
.L_02009cd6:
	pop {r5, r6, r7, pc}
.L_02009cd8:
	.4byte MsgHereLetMe
.L_02009cdc:
	.4byte 0x00019999
.L_02009ce0:
	.4byte Data_0200277c
.L_02009ce4:
	.4byte Data_020027c0
.L_02009ce8:
	.4byte 0x036e0000
.L_02009cec:
	.4byte gPartyState
.L_02009cf0:
	.4byte Data_02002804
.L_02009cf4:
	.4byte Data_02002870
.L_02009cf8:
	.4byte Data_020028dc
.L_02009cfc:
	.4byte Data_02002948
.L_02009d00:
	.4byte 0x00013333
.L_02009d04:
	.4byte Data_02002998
.L_02009d08:
	.4byte Data_020029c0
.L_02009d0c:
	.4byte Data_02002a04
.L_02009d10:
	.4byte Func_02001688
	.section .text.x02009d14,"ax",%progbits
	.global Func_02001d14
	.thumb_func
Func_02001d14:
	push {r5, r6, r7, lr}
	movs r0, #8
	bl Object_GetById
	movs r1, #128
	adds r7, r0, #0
	lsls r1, r1, #8
	movs r0, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r3, .L_02009eb8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Func_02002548
	ldr r0, .L_02009ebc
	bl Func_02002520
	adds r5, r7, #0
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	adds r5, #102
	bl Func_0200182c
	movs r3, #1
	strh r3, [r5]
.L_02009d78:
	adds r6, r7, #0
	movs r0, #1
	adds r6, #100
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #114
	bne .L_02009d78
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009da0:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #150
	bne .L_02009da0
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009dc4:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #154
	bne .L_02009dc4
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009de8:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #216
	bne .L_02009de8
	movs r3, #3
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	bl Func_02001730
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #4
	movs r2, #40
	bl ObjectMotion_Launch
	ldr r5, .L_02009eb8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02002548
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009e90
	bl Func_02001ec0
	b .L_02009eb4
.L_02009e90:
	movs r1, #0
	movs r2, #20
	movs r0, #8
	bl Func_02002530
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #123
	bl Func_020025a0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #15
	bl Func_02002578
.L_02009eb4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009eb8:
	.4byte gPartyState
.L_02009ebc:
	.4byte MsgIfYouWant
	.section .text.x02009ec0,"ax",%progbits
	.global Func_02001ec0
	.thumb_func
Func_02001ec0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #8
	bl Object_GetById
	movs r1, #192
	adds r6, r0, #0
	lsls r1, r1, #6
	movs r0, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r3, .L_0200a234
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #192
	ldr r0, [r3]
	lsls r1, r1, #7
	bl Func_02002548
	ldr r0, .L_0200a238
	bl Func_02002520
	adds r5, r6, #0
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	adds r5, #102
	bl Func_0200182c
	movs r3, #1
	strh r3, [r5]
.L_02009f28:
	adds r7, r6, #0
	movs r0, #1
	adds r7, #100
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #218
	bne .L_02009f28
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009f50:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #254
	bne .L_02009f50
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009f74:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r7, r2]
	movs r2, #129
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_02009f74
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009f9c:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r7, r2]
	movs r2, #162
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_02009f9c
	movs r3, #3
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	bl Func_02001730
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #8
	bl Func_02002548
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #16]
	movs r0, #8
	ldr r1, .L_0200a23c
	ldr r2, .L_0200a240
	mov r8, r3
	ldr r7, [r6, #8]
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	lsls r2, r2, #2
	movs r0, #8
	movs r1, #200
	adds r2, #158
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	mov r3, r8
	movs r2, #0
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02002450
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #10
	bl Func_02002548
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r6, #16]
	movs r0, #10
	mov r8, r2
	ldr r1, .L_0200a23c
	ldr r2, .L_0200a240
	ldr r7, [r6, #8]
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	lsls r2, r2, #2
	movs r0, #10
	movs r1, #200
	adds r2, #158
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #10
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	mov r3, r8
	movs r2, #0
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02002450
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02002548
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #16]
	movs r0, #9
	ldr r1, .L_0200a23c
	ldr r2, .L_0200a240
	mov r8, r3
	ldr r7, [r6, #8]
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	lsls r2, r2, #2
	movs r0, #9
	movs r1, #200
	adds r2, #158
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #0
	mov r3, r8
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02002450
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02002548
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r0, #11
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r6, #16]
	movs r0, #11
	mov r8, r2
	ldr r1, .L_0200a23c
	ldr r2, .L_0200a240
	ldr r7, [r6, #8]
	bl ObjectMotion_SetSpeedParameters
	movs r0, #11
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	lsls r2, r2, #2
	movs r0, #11
	movs r1, #200
	adds r2, #158
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #11
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	adds r1, r7, #0
	mov r3, r8
	adds r0, r6, #0
	movs r2, #0
	bl Func_02002450
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r5, .L_0200a234
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #128
	lsls r2, r2, #2
	ldr r0, [r5]
	movs r1, #184
	adds r2, #174
	bl ObjectMotion_SetPositionAndReset
	movs r2, #128
	lsls r2, r2, #2
	ldr r0, [r5]
	movs r1, #186
	adds r2, #162
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #160
	orrs r5, r3
	strb r5, [r0]
	lsls r1, r1, #7
	movs r0, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #20
	movs r0, #8
	bl Func_02002530
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #123
	bl Func_020025a0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #15
	bl Func_02002578
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a234:
	.4byte gPartyState
.L_0200a238:
	.4byte MsgIfYouWantTo
.L_0200a23c:
	.4byte 0x00026666
.L_0200a240:
	.4byte 0x00013333
	.section .text.x0200a2f8,"ax",%progbits
	.balign 4
	.global Func_020022f8
	.thumb_func
Func_020022f8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r6, #80]
	movs r0, #192
	lsls r0, r0, #2
	mov r8, r2
	bl GameFlag_SetBit
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #52]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r6, #48]
	movs r0, #227
	bl Func_020025a0
	movs r1, #176
	movs r2, #160
	movs r3, #184
	adds r0, r6, #0
	lsls r1, r1, #16
	lsls r2, r2, #14
	lsls r3, r3, #16
	bl Func_02002450
	movs r7, #0
	movs r5, #15
.L_0200a33c:
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r0, #1
	subs r3, r3, r7
	strh r3, [r2, #18]
	subs r5, #1
	bl WaitFrames
	adds r7, #40
	cmp r5, #0
	bge .L_0200a33c
	movs r1, #168
	movs r2, #128
	movs r3, #184
	adds r0, r6, #0
	lsls r1, r1, #16
	lsls r2, r2, #13
	lsls r3, r3, #16
	bl Func_02002450
	movs r7, #200
	lsls r7, r7, #1
	movs r5, #25
.L_0200a36a:
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r0, #1
	subs r3, r3, r7
	strh r3, [r2, #18]
	subs r5, #1
	bl WaitFrames
	adds r7, #40
	cmp r5, #0
	bge .L_0200a36a
	adds r0, r6, #0
	bl Func_02002458
	movs r0, #2
	bl Battle_WaitMode0
	movs r0, #240
	bl Func_020025a0
	movs r5, #0
	mov r3, r8
	strh r5, [r3, #18]
	movs r3, #168
	lsls r3, r3, #16
	str r3, [r6, #8]
	movs r3, #192
	lsls r3, r3, #16
	str r3, [r6, #16]
	movs r2, #12
	movs r3, #10
	str r5, [r6, #12]
	str r5, [r6, #40]
	str r5, [r6, #36]
	movs r0, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl Func_02002470
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a3c8,"ax",%progbits
	.global Func_020023c8
	.thumb_func
Func_020023c8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #9
	bl Object_GetById
	adds r1, r0, #0
	adds r2, r1, #0
	movs r3, #0
	ldr r5, .L_0200a40c
	adds r2, #100
	ldr r6, .L_0200a408
	mov r8, r3
	movs r3, #1
	strh r3, [r2]
	adds r3, r1, #0
	adds r3, #99
	strb r6, [r3]
	movs r0, #10
	str r5, [r1, #108]
	bl Object_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #100
	mov r1, r8
	strh r1, [r3]
	subs r3, #1
	strb r6, [r3]
	str r5, [r2, #108]
	b .L_0200a410
	.2byte 0x0000
.L_0200a408:
	.4byte 0x00000000
.L_0200a40c:
	.4byte Func_0200014c
.L_0200a410:
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.space 0x00000008
	.global Data_02002fc0
Data_02002fc0:
