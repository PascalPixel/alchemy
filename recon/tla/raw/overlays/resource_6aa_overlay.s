.syntax unified
	.thumb
	.section .text.x02008e3c,"ax",%progbits
	.global Func_02000e3c
	.thumb_func
Func_02000e3c:
	push {r5, r6, r7, lr}
	ldr r5, .L_02008ecc
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	adds r7, r0, #0
	ldr r0, [r5]
	bl Actor_GetById
	adds r6, r0, #0
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	ldr r0, [r5]
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r3, #0
	movs r1, #128
	str r3, [r6, #108]
	movs r2, #0
	lsls r1, r1, #1
	ldr r0, [r5]
	bl Func_02004d00
	adds r0, r6, #0
	movs r1, #27
	bl Func_02004bc0
	movs r1, #48
	adds r0, r6, #0
	bl Actor_SetAnimationSpeed
	movs r0, #30
	bl wait
	movs r3, #128
	ldr r2, .L_02008ec8
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	ldr r2, .L_02008ed0
	ldr r3, [r6, #8]
	asrs r4, r3, #20
	ldr r3, [r6, #16]
	asrs r0, r3, #20
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	cmp r1, #0
	beq .L_02008ed4
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	movs r2, #3
	ands r2, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [r1, r3]
	b .L_02008ed4
.L_02008ec8:
	.4byte 0x00000000
.L_02008ecc:
	.4byte gMainRoutineWork
.L_02008ed0:
	.4byte gMapCellBuffer
.L_02008ed4:
	lsls r3, r0, #7
	adds r3, r4, r3
	lsls r3, r3, #2
	movs r1, #128
	adds r3, r2, r3
	lsls r1, r1, #2
	adds r2, r3, r1
	ldrb r2, [r2, #3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	bne .L_02008f0a
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_02008f38
	orrs r3, r2
	strh r3, [r1]
	bl Func_02000df0
.L_02008f0a:
	ldr r3, .L_02008f3c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #28
	bl Actor_SetModeById
	movs r0, #16
	bl FieldEvent_Wait
	movs r5, #0
.L_02008f22:
	cmp r5, #5
	bne .L_02008f2c
	movs r0, #204
	bl Func_02004de0
.L_02008f2c:
	ldr r3, [r6, #24]
	ldr r1, .L_02008f40
	ldr r2, .L_02008f44
	adds r3, r3, r1
	str r3, [r6, #24]
	b .L_02008f48
.L_02008f38:
	.4byte 0x00008000
.L_02008f3c:
	.4byte gMainRoutineWork
.L_02008f40:
	.4byte 0xfffffc00
.L_02008f44:
	.4byte 0xfffffd00
.L_02008f48:
	ldr r3, [r6, #28]
	ldr r1, .L_02008f9c
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r5, #1
	bl wait
	cmp r5, #39
	ble .L_02008f22
	ldr r3, .L_02008fa0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	adds r0, r7, #0
	bl Func_02004d30
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008f9c:
	.4byte 0xffff6667
.L_02008fa0:
	.4byte gMainRoutineWork
	.section .text.x0200a4c0,"ax",%progbits
	.balign 4
	.global Func_020024c0
	.thumb_func
Func_020024c0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #16
	ldr r6, [r3, #108]
	bl Func_02004dc0
	mov r8, r0
	bl Actor_GetById
	bl Party_CountActiveOwners
	movs r5, #0
	adds r7, r0, #0
	cmp r5, r7
	bge .L_0200a502
.L_0200a4e6:
	ldr r2, .L_0200a5a8
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r5, r1
	ldrb r0, [r2, r3]
	bl Unit_Get
	ldrh r3, [r0, #56]
	lsls r2, r5, #1
	mov r1, sp
	adds r5, #1
	strh r3, [r1, r2]
	cmp r5, r7
	blt .L_0200a4e6
.L_0200a502:
	movs r0, #10
	negs r0, r0
	movs r1, #0
	bl Func_02004da8
	movs r2, #182
	lsls r2, r2, #1
	movs r4, #183
	adds r3, r6, r2
	lsls r4, r4, #1
	movs r2, #0
	strh r2, [r3]
	movs r1, #129
	adds r3, r6, r4
	strh r2, [r3]
	mov r0, r8
	lsls r1, r1, #1
	movs r5, #0
	bl Func_02004d08
	cmp r5, r7
	bge .L_0200a59c
.L_0200a52e:
	ldr r1, .L_0200a5a8
	movs r2, #134
	lsls r2, r2, #2
	adds r2, r2, r5
	ldrb r0, [r1, r2]
	mov r10, r1
	mov r8, r2
	bl Unit_Get
	movs r4, #56
	ldrsh r3, [r0, r4]
	cmp r3, #0
	ble .L_0200a556
	movs r1, #183
	lsls r1, r1, #1
	adds r2, r6, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a596
.L_0200a556:
	mov r3, sp
	lsls r2, r5, #1
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a596
	movs r2, #182
	lsls r2, r2, #1
	adds r1, r6, r2
	ldrh r3, [r1]
	movs r4, #184
	adds r2, r3, #1
	lsls r3, r3, #16
	lsls r4, r4, #1
	asrs r3, r3, #15
	strh r2, [r1]
	adds r3, r3, r4
	mov r1, r10
	mov r4, r8
	ldrb r2, [r1, r4]
	movs r1, #181
	strh r2, [r6, r3]
	movs r3, #255
	lsls r1, r1, #1
	lsls r3, r3, #8
	adds r2, r6, r1
	adds r3, #255
	strh r3, [r2]
	movs r3, #50
	adds r3, #255
	adds r2, r0, r3
	movs r3, #0
	strb r3, [r2]
.L_0200a596:
	adds r5, #1
	cmp r5, r7
	blt .L_0200a52e
.L_0200a59c:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a5a8:
	.4byte gMainRoutineWork
	.section .text.x0200a63c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_0200263c
	.thumb_func
Func_0200263c:
	push {r5, r6, r7, lr}
	mov r7, sl
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_ja_0200a7d0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3, #0]
	sub sp, #12
	cmp r3, #0
	beq .L_ja_0200a660
	b .L_ja_0200a7c0
.L_ja_0200a660:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_020024c0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3, #0]
	bl Actor_GetById
	ldr r2, .L_ja_0200a7d4
	adds r5, r0, #0
	mov sl, r2
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	ldr r3, .L_ja_0200a7d8
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r1, #49
	bl Func_02004bc0
.L_ja_0200a698:
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldrb r0, [r3, #0]
	ldr r2, [r5, #16]
	bl Func_02004c18
	mov r6, sl
	mov r7, sl
	adds r6, #104
	adds r7, #106
	mov r1, sp
	cmp r0, #7
	bne .L_ja_0200a710
	movs r0, #0
	ldrsh r3, [r6, r0]
	movs r2, #0
	lsls r3, r3, #17
	str r3, [r5, #36]
	mov r8, r2
	movs r1, #0
	ldrsh r3, [r7, r1]
	lsls r3, r3, #17
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r5, #52]
.L_ja_0200a6ce:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1, #0]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_02004c38
	cmp r0, #0
	beq .L_ja_0200a6fe
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	b .L_ja_0200a7b4
.L_ja_0200a6fe:
	movs r0, #1
	bl wait
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	ble .L_ja_0200a6ce
	b .L_ja_0200a7b4
.L_ja_0200a710:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #0]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_02004c38
	cmp r0, #0
	bgt .L_ja_0200a7b4
	cmp r0, #0
	bge .L_ja_0200a756
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	ldr r3, .L_ja_0200a7d0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3, #0]
	movs r1, #6
	negs r2, r2
	bl Func_02004ce8
	b .L_ja_0200a7b4
.L_ja_0200a756:
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r5, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldr r7, .L_ja_0200a7dc
	lsls r3, r3, #17
	ldr r0, .L_ja_0200a7e0
	adds r6, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r6, [r5, #16]
	cmp r2, r4
	ble .L_ja_0200a782
	adds r2, r4, #0
.L_ja_0200a782:
	ldr r0, .L_ja_0200a7e4
	cmp r2, r0
	bge .L_ja_0200a78a
	adds r2, r0, #0
.L_ja_0200a78a:
	subs r3, r1, r2
	ldr r1, .L_ja_0200a7e0
	str r3, [r5, #8]
	adds r3, r6, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_ja_0200a79c
	adds r2, r4, #0
.L_ja_0200a79c:
	cmp r2, r0
	bge .L_ja_0200a7a2
	adds r2, r0, #0
.L_ja_0200a7a2:
	subs r3, r6, r2
	str r3, [r5, #16]
	mov r2, sl
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_ja_0200a698
.L_ja_0200a7b4:
	movs r3, #0
	str r3, [r5, #108]
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_ja_0200a7c0:
	mov r0, sl
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov sl, r5
	pop {r5, r6, r7, pc}
.L_ja_0200a7d0:
	.4byte gMainRoutineWork
.L_ja_0200a7d4:
	.4byte 0x0200254c
.L_ja_0200a7d8:
	.4byte Func_02002614
.L_ja_0200a7dc:
	.4byte 0x000fffff
.L_ja_0200a7e0:
	.4byte 0xfff80000
.L_ja_0200a7e4:
	.4byte 0xffff0000
	.endif
	.section .text.x0200a9c8,"ax",%progbits
	.balign 4
	.global Func_020029c8
	.thumb_func
Func_020029c8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200ab30
	sub sp, #4
	ldr r3, [r1, #112]
	mov r11, r0
	cmp r3, #0
	bne .L_0200a9e4
	b .L_0200ab3c
.L_0200a9e4:
	movs r2, #0
	str r2, [sp, #0]
.L_0200a9e8:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r3, r11
	ldr r3, [r3, #8]
	lsls r5, r5, #4
	mov r8, r3
	add r8, r5
	lsls r0, r0, #4
	mov r1, r8
	subs r1, r1, r0
	mov r8, r1
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r11
	ldr r3, [r2, #16]
	ldr r2, [r2, #12]
	lsls r5, r5, #4
	lsls r6, r6, #3
	movs r1, #128
	lsls r0, r0, #4
	adds r6, r6, r2
	lsls r1, r1, #11
	adds r3, r3, r5
	subs r3, r3, r0
	adds r6, r6, r1
	movs r0, #234
	adds r0, #255
	mov r1, r8
	adds r2, r6, #0
	bl Func_02004bd8
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200ab1c
	bl Random16Far
	mov r10, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #8
	adds r5, r5, r0
	ldr r1, .L_0200ab34
	adds r0, r7, #0
	mov r9, r2
	bl Func_02004bd0
	adds r0, r7, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	mov r1, r10
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r7, #40]
	mov r0, r10
	bl Math_Cosine
	ldr r3, .L_0200ab38
	lsls r6, r6, #3
	mov r8, r3
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r10
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, .L_0200ab30
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	lsrs r5, r5, #2
	ldr r3, [r2, #112]
	add r5, r9
	movs r6, #0
	mov r1, r9
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r1, [r7, #68]
	ldr r5, [r7, #80]
	str r0, [r7, #36]
	str r6, [r7, #52]
	ldr r3, [r3, #80]
	ldrb r0, [r5, #16]
	mov r8, r3
	bl Resource_ResetEntry
	ldrb r3, [r5, #17]
	ldr r1, .L_0200ab30
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #17]
	ldrh r3, [r1, #12]
	ldr r0, [r5, #40]
	strb r3, [r5, #16]
	bl ResourceMetadata_ClearRecord
	str r6, [r5, #40]
	strb r6, [r5, #27]
	mov r2, r8
	ldrb r3, [r2, #20]
	ldrb r0, [r5, #5]
	strb r3, [r5, #20]
	ldrb r3, [r2, #21]
	strb r3, [r5, #21]
	ldrb r1, [r2, #5]
	movs r2, #63
	adds r3, r2, #0
	lsrs r1, r1, #6
	lsls r1, r1, #6
	ands r3, r0
	orrs r3, r1
	strb r3, [r5, #5]
	mov r1, r8
	ldrb r3, [r1, #7]
	ldrb r1, [r5, #7]
	lsrs r3, r3, #6
	lsls r3, r3, #6
	ands r2, r1
	orrs r2, r3
	strb r2, [r5, #7]
	mov r3, r8
	ldrh r2, [r3, #8]
	ldr r1, .L_0200ab2c
	ldrh r3, [r5, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
.L_0200ab1c:
	ldr r1, [sp, #0]
	subs r1, #1
	str r1, [sp, #0]
	cmp r1, #0
	blt .L_0200ab28
	b .L_0200a9e8
.L_0200ab28:
	b .L_0200ab3c
	.2byte 0x0000
.L_0200ab2c:
	.4byte 0xfffffc00
.L_0200ab30:
	.4byte gData_020023c4 + 0x188
.L_0200ab34:
	.4byte gData_020056d0
.L_0200ab38:
	.4byte IwramMulQ16
.L_0200ab3c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200afc4,"ax",%progbits
	.balign 4
	.global Func_02002fc4
	.thumb_func
Func_02002fc4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r3, r11
	adds r3, #98
	ldrb r0, [r3]
	sub sp, #12
	bl Actor_GetById
	mov r3, r11
	adds r7, r0, #0
	adds r3, #102
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, [r7, #80]
	mov r2, r11
	mov r9, r3
	ldr r3, [r2, #8]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r2, #12]
	ldr r2, .L_0200b054
	adds r1, r6, #0
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r2, r11
	ldr r3, [r2, #16]
	lsls r0, r0, #14
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	ldr r3, .L_0200b050
	movs r2, #0
	mov r10, r3
	adds r3, r7, #0
	mov r8, r2
	adds r3, #85
	mov r2, r10
	strh r6, [r7, #6]
	strb r2, [r3]
	adds r0, r7, #0
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Actor_SetPositionAndResetMotion
	ldr r3, .L_0200b058
	mov r2, r10
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #90
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r7, #52]
	ldr r3, .L_0200b05c
	mov r2, r9
	adds r6, r6, r3
	b .L_0200b060
	.2byte 0x0000
.L_0200b050:
	.4byte 0x00000000
.L_0200b054:
	.4byte 0xfff40000
.L_0200b058:
	.4byte Func_02002c40
.L_0200b05c:
	.4byte 0xffffc000
.L_0200b060:
	mov r3, r8
	strh r6, [r2, #18]
	str r3, [r7, #24]
	str r3, [r7, #28]
	adds r3, r7, #0
	mov r2, r8
	adds r3, #100
	strh r2, [r3]
	mov r2, r11
	ldr r3, [r2, #104]
	movs r0, #104
	str r3, [r7, #104]
	bl Func_02004de0
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b564,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_02003564
	.thumb_func
Func_02003564:
	push {r5, r6, r7, lr}
	mov r7, fp
	mov r6, sl
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_ja_0200b564
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3, #0]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_ja_0200b568
	ldr r3, .L_ja_0200b560
	adds r7, r0, #0
	strh r3, [r2, #0]
.L_ja_0200b52a:
	bl Func_02003284
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_ja_0200b56c
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov sl, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, sl
	str r3, [sp, #12]
	str r3, [r0, #0]
	b .L_ja_0200b570
.L_ja_0200b560:
	.4byte 0x00000000
.L_ja_0200b564:
	.4byte gMainRoutineWork
.L_ja_0200b568:
	.4byte gData_020068b0
.L_ja_0200b56c:
	.4byte 0xfff00000
.L_ja_0200b570:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3, #0]
	ldr r2, [sp, #16]
	bl Func_02004c10
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_ja_0200b5dc
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_ja_0200b5ec
	ldr r3, .L_ja_0200b5e0
	movs r2, #15
	ldr r3, [r3, #0]
	ldr r1, .L_ja_0200b5e4
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_ja_0200b5e8
	cmp r3, r2
	bne .L_ja_0200b618
	b .L_ja_0200b7e0
.L_ja_0200b5dc:
	.4byte IwramMulQ16
.L_ja_0200b5e0:
	.4byte gInput
.L_ja_0200b5e4:
	.4byte gData_02005798
.L_ja_0200b5e8:
	.4byte 0xffff0000
.L_ja_0200b5ec:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_ja_0200b614
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_ja_0200b618
.L_ja_0200b614:
	.4byte 0xffffc000
.L_ja_0200b618:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, sl
	ldr r1, [r2, #0]
	ldrb r0, [r3, #0]
	ldr r2, [r2, #8]
	bl Func_02004c10
	mov fp, r0
	cmp r0, #255
	beq .L_ja_0200b69a
	ldr r3, [sp, #4]
	mov r2, sl
	ldrb r0, [r3, #0]
	ldr r1, [r2, #0]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_0200b69a
	ldr r0, [sp, #12]
	mov r2, sl
	str r0, [r2, #0]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2, #0]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_02004bf8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004bc0
	adds r0, r7, #0
	movs r1, #48
	bl Actor_SetAnimationSpeed
	adds r0, r7, #0
	bl Func_02004c00
	ldr r3, .L_ja_0200b7f0
	str r3, [r7, #108]
	b .L_ja_0200b776
.L_ja_0200b69a:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_ja_0200b7c2
.L_ja_0200b6ae:
	ldr r2, [sp, #4]
	ldr r1, [r6, #0]
	ldrb r0, [r2, #0]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_0200b796
	ldr r3, .L_ja_0200b7f4
	ldrh r3, [r3, #0]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200b796
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov sl, r0
	mov r8, r2
.L_ja_0200b6e6:
	ldr r3, [r5, #0]
	cmp r3, #0
	beq .L_ja_0200b710
	mov r3, r8
	ldrb r2, [r3, #0]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200b710
	cmp r5, r7
	beq .L_ja_0200b710
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004c78
	cmp r0, #0
	bge .L_ja_0200b796
.L_ja_0200b710:
	movs r0, #1
	add sl, r0
	movs r2, #128
	mov r3, sl
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_ja_0200b6e6
	ldr r0, [r6, #0]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6, #0]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_02004bf8
	b .L_ja_0200b74e
.L_ja_0200b746:
	movs r0, #1
	bl wait
	adds r5, #1
.L_ja_0200b74e:
	cmp r5, #179
	bgt .L_ja_0200b766
	adds r0, r7, #0
	bl Func_02004c70
	cmp r0, #0
	bne .L_ja_0200b766
	ldr r3, .L_ja_0200b7f4
	ldrh r3, [r3, #0]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_ja_0200b746
.L_ja_0200b766:
	ldr r3, .L_ja_0200b7f4
	ldrh r3, [r3, #0]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200b7c2
	ldr r3, [sp, #24]
	cmp fp, r3
	bne .L_ja_0200b7bc
.L_ja_0200b776:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2, #0]
	ldr r1, [r6, #0]
	ldr r2, [r6, #8]
	bl Func_02004c10
	mov fp, r0
	cmp r0, #255
	bne .L_ja_0200b6ae
.L_ja_0200b796:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02004bf8
	adds r0, r7, #0
	bl Func_02004c00
	movs r0, #2
	bl wait
	b .L_ja_0200b52a
.L_ja_0200b7bc:
	movs r0, #10
	bl wait
.L_ja_0200b7c2:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1, #0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1, #0]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02004bc0
.L_ja_0200b7e0:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov sl, r6
	mov fp, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200b7f0:
	.4byte Func_020031f8
.L_ja_0200b7f4:
	.4byte gData_020068b0
	.endif
	.section .bss,"aw",%nobits
	.global Data_02006878
Data_02006878:
	.space 0x00000004
	.global gData_0200687c
gData_0200687c:
	.space 0x00000004
	.global Data_02006880
Data_02006880:
	.space 0x00000004
	.global gData_02006884
gData_02006884:
	.space 0x00000004
	.global gData_0200e888
gData_0200e888:
	.space 0x00000004
	.global gData_0200e88c
gData_0200e88c:
	.space 0x00000004
	.global gData_02006890
gData_02006890:
	.space 0x00000004
	.global Data_02006894
Data_02006894:
	.space 0x00000004
	.global gData_02006898
gData_02006898:
	.space 0x00000004
	.global gData_0200689c
gData_0200689c:
	.space 0x00000004
	.global gData_020068a0
gData_020068a0:
	.space 0x00000002
	.global gData_020068a2
gData_020068a2:
	.space 0x00000002
	.global Data_020068a4
Data_020068a4:
	.space 0x0000000c
	.global gData_020068b0
gData_020068b0:
	.space 0x00000004
	.global Data_020068b4
Data_020068b4:
