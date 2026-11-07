.syntax unified
	.thumb
	.section .text.x02008e44,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02000e44
	.thumb_func
Func_02000e44:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #124
	bl Func_02002208
	ldr r3, .L_ja_02008e8c
	movs r0, #147
	movs r1, #128
	lsls r0, r0, #1
	lsls r1, r1, #2
	adds r0, #255
	adds r1, #38
	adds r2, r3, r0
	adds r3, r3, r1
	ldrb r0, [r2]
	ldrb r1, [r3]
	bl Func_02002310
	ldr r4, .L_ja_02008e90
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_ja_02008e94
	adds r1, r4, #0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_ja_02008e88
	movs r0, #1
	strh r3, [r4, #8]
	bl Func_020022b0
	movs r2, #1
	negs r2, r2
	str r0, [sp, #64]
	cmp r0, r2
	bne .L_ja_02008e60
	.2byte 0xf000, 0xfd1a @ bl 0x02009894
.L_ja_02008e60:
	movs r0, #71
	bl Func_02002408
	movs r0, #238
	movs r3, #0
	lsls r0, r0, #1
	str r3, [sp, #60]
	str r3, [sp, #52]
	str r3, [sp, #48]
	bl Heap_AllocEwram
	movs r4, #0
	str r0, [sp, #36]
	ldr r0, .L_ja_02008e98
	str r4, [sp, #8]
	bl Func_02002320
	ldr r1, [sp, #64]
	b .L_ja_02008e9c
	.2byte 0x0000
.L_ja_02008e88:
	.4byte 0x00006318
.L_ja_02008e8c:
	.4byte gMainRoutineWork
.L_ja_02008e90:
	.4byte 0x050001c0
.L_ja_02008e94:
	.4byte 0x050001e0
.L_ja_02008e98:
	.4byte 0x06006000
.L_ja_02008e9c:
	movs r0, #130
	lsls r0, r0, #1
	str r0, [sp, #44]
	cmp r1, #0
	beq .L_ja_02008eb4
	ldr r3, [sp, #64]
	movs r2, #61
	str r2, [sp, #44]
	cmp r3, #1
	beq .L_ja_02008eb4
	movs r4, #16
	str r4, [sp, #44]
.L_ja_02008eb4:
	ldr r0, [sp, #44]
	movs r1, #50
	bl __divsi3
	adds r0, #1
	str r0, [sp, #40]
	movs r5, #2
	movs r1, #13
	movs r2, #29
	movs r3, #6
	movs r0, #1
	str r5, [sp, #0]
	bl Window_Create
	ldr r6, .L_ja_02009190
	mov r10, r0
	mov r1, r10
	adds r0, r6, #0
	movs r2, #0
	adds r6, #1
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #0
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #0
	movs r3, #16
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #0
	movs r3, #24
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #16
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #24
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #16
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #24
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #144
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	movs r2, #144
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	movs r3, #5
	str r3, [sp, #0]
	adds r6, #1
	mov r0, r10
	movs r1, #24
	movs r2, #0
	movs r3, #24
	bl Window_DrawDividerLine
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #192
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #192
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	movs r2, #192
	movs r3, #24
	bl Text_DrawCharacterAtOffset
	movs r0, #5
	movs r1, #1
	movs r2, #20
	movs r3, #12
	str r5, [sp, #0]
	bl Window_Create
	str r0, [sp, #56]
	movs r6, #0
.L_ja_02008fe2:
	ldr r2, [sp, #36]
	movs r0, #224
	lsls r0, r0, #1
	adds r1, r6, r0
	adds r2, #2
	movs r3, #0
	adds r6, #1
	strb r3, [r2, r1]
	cmp r6, #26
	bne .L_ja_02008fe2
	ldr r2, .L_ja_02009194
	movs r6, #0
.L_ja_02008ffa:
	movs r1, #130
	movs r3, #99
	adds r6, #1
	lsls r1, r1, #1
	strb r3, [r2]
	adds r2, #1
	cmp r6, r1
	bne .L_ja_02008ffa
	ldr r2, [sp, #36]
	movs r3, #200
	lsls r3, r3, #1
	movs r6, #0
	adds r2, r2, r3
	mov r9, r2
	mov r11, r6
.L_ja_02009018:
	movs r1, #10
	adds r0, r6, #0
	bl __modsi3
	adds r5, r0, #0
	lsls r3, r5, #1
	adds r3, r3, r5
	lsls r7, r3, #2
	movs r4, #8
	adds r4, r4, r7
	adds r0, r6, #0
	movs r1, #10
	mov r8, r4
	bl __divsi3
	lsls r0, r0, #4
	adds r3, r0, #2
	cmp r5, #4
	ble .L_ja_02009042
	adds r7, #18
	mov r8, r7
.L_ja_02009042:
	ldr r2, [sp, #36]
	mov r0, r11
	ldr r1, [r0, r2]
	ldr r0, [sp, #56]
	mov r2, r8
	bl Func_02002330
	ldr r3, [sp, #36]
	mov r2, r11
	adds r2, #200
	str r0, [r2, r3]
	mov r4, r9
	movs r3, #99
	strb r3, [r4]
	ldr r1, [sp, #36]
	movs r0, #1
	add r9, r0
	ldr r3, .L_ja_02009198
	ldr r0, [r2, r1]
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
	adds r6, #1
	movs r2, #4
	add r11, r2
	cmp r6, #50
	bne .L_ja_02009018
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_ja_020090b2
	bl Func_02002300
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	mov r2, r10
	adds r0, r5, #0
	str r3, [sp, #0]
	bl WindowSprite_Create
	adds r1, r0, #0
	add r0, sp, #108
	str r1, [r0]
	mov r4, r10
	movs r3, #12
	ldrsh r1, [r4, r3]
	movs r3, #14
	ldrsh r2, [r4, r3]
	lsls r1, r1, #3
	lsls r2, r2, #3
	subs r1, #2
	adds r2, #12
	bl Func_02002400
.L_ja_020090b2:
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_ja_020090fa
	bl Func_02002308
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	mov r2, r10
	adds r0, r5, #0
	str r3, [sp, #0]
	bl WindowSprite_Create
	adds r1, r0, #0
	add r0, sp, #92
	str r1, [r0]
	movs r3, #255
	ldrb r2, [r1, #25]
	strb r3, [r1, #15]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	strb r3, [r1, #25]
	ldr r2, [sp, #56]
	movs r4, #12
	ldrsh r1, [r2, r4]
	movs r3, #14
	ldrsh r2, [r2, r3]
	lsls r1, r1, #3
	lsls r2, r2, #3
	subs r1, #4
	adds r2, #12
	bl Func_02002400
.L_ja_020090fa:
	movs r4, #0
	movs r0, #1
	str r0, [sp, #24]
	str r4, [sp, #32]
	str r4, [sp, #28]
	mov r2, r10
	movs r1, #12
	ldrsh r3, [r2, r1]
	movs r0, #0
	lsls r3, r3, #3
	subs r3, #8
	str r3, [sp, #20]
	movs r4, #14
	ldrsh r3, [r2, r4]
	str r0, [sp, #12]
	lsls r3, r3, #3
	adds r3, #11
	str r3, [sp, #16]
.L_ja_0200911e:
	ldr r2, [sp, #12]
	movs r1, #1
	negs r1, r1
	mov r9, r1
	cmp r2, #2
	bne .L_ja_0200912e
	movs r3, #1
	str r3, [sp, #12]
.L_ja_0200912e:
	movs r3, #15
	movs r4, #1
	str r3, [sp, #4]
	mov r0, r10
	ldr r1, [sp, #32]
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	str r4, [sp, #0]
	bl Func_02000db4
	ldr r0, [sp, #12]
	cmp r0, #0
	bne .L_ja_0200921c
	ldr r3, .L_ja_0200919c
	movs r2, #16
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_ja_02009170
	movs r0, #111
	bl Func_02002408
	ldr r0, [sp, #52]
	ldr r1, [sp, #48]
	adds r0, #1
	cmp r1, #2
	bne .L_ja_02009168
	movs r1, #20
	b .L_ja_0200916a
.L_ja_02009168:
	movs r1, #21
.L_ja_0200916a:
	bl __modsi3
	str r0, [sp, #52]
.L_ja_02009170:
	ldr r3, .L_ja_0200919c
	movs r2, #32
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_ja_020091ac
	movs r0, #111
	bl Func_02002408
	ldr r2, [sp, #48]
	cmp r2, #2
	bne .L_ja_020091a0
	ldr r0, [sp, #52]
	movs r1, #20
	adds r0, #19
	b .L_ja_020091a6
.L_ja_02009190:
	.4byte 0x00000d80
.L_ja_02009194:
	.4byte Data_0200274a
.L_ja_02009198:
	.4byte IwramClearWords
.L_ja_0200919c:
	.4byte gInput
.L_ja_020091a0:
	ldr r0, [sp, #52]
	movs r1, #21
	adds r0, #20
.L_ja_020091a6:
	bl __modsi3
	str r0, [sp, #52]
.L_ja_020091ac:
	ldr r3, .L_ja_02009424
	movs r2, #128
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_ja_020091e4
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #48]
	adds r1, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_ja_020091cc
	ldr r3, [sp, #48]
	adds r3, #4
.L_ja_020091cc:
	asrs r3, r3, #2
	str r3, [sp, #48]
	lsls r3, r3, #2
	subs r1, r1, r3
	ldr r3, [sp, #52]
	str r1, [sp, #48]
	cmp r3, #20
	bne .L_ja_020091e4
	cmp r1, #2
	bne .L_ja_020091e4
	movs r4, #3
	str r4, [sp, #48]
.L_ja_020091e4:
	ldr r3, .L_ja_02009424
	movs r2, #64
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200921c
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #48]
	adds r1, #3
	adds r3, r1, #0
	cmp r1, #0
	bge .L_ja_02009204
	ldr r3, [sp, #48]
	adds r3, #6
.L_ja_02009204:
	asrs r3, r3, #2
	ldr r0, [sp, #52]
	str r3, [sp, #48]
	lsls r3, r3, #2
	subs r1, r1, r3
	str r1, [sp, #48]
	cmp r0, #20
	bne .L_ja_02009226
	cmp r1, #2
	bne .L_ja_0200921c
	movs r1, #1
	str r1, [sp, #48]
.L_ja_0200921c:
	ldr r3, [sp, #52]
	movs r2, #3
	str r2, [sp, #24]
	cmp r3, #20
	beq .L_ja_0200922a
.L_ja_02009226:
	movs r4, #1
	str r4, [sp, #24]
.L_ja_0200922a:
	ldr r0, [sp, #52]
	ldr r1, [sp, #48]
	str r0, [sp, #32]
	str r1, [sp, #28]
	cmp r0, #4
	ble .L_ja_0200923c
	adds r2, r0, #0
	adds r2, #1
	str r2, [sp, #32]
.L_ja_0200923c:
	ldr r3, [sp, #52]
	cmp r3, #9
	ble .L_ja_02009248
	ldr r4, [sp, #32]
	adds r4, #1
	str r4, [sp, #32]
.L_ja_02009248:
	ldr r0, [sp, #52]
	cmp r0, #14
	ble .L_ja_02009254
	ldr r1, [sp, #32]
	adds r1, #1
	str r1, [sp, #32]
.L_ja_02009254:
	ldr r2, [sp, #52]
	cmp r2, #19
	ble .L_ja_02009260
	ldr r3, [sp, #32]
	adds r3, #1
	str r3, [sp, #32]
.L_ja_02009260:
	movs r3, #14
	movs r4, #1
	mov r0, r10
	str r3, [sp, #4]
	ldr r1, [sp, #32]
	ldr r2, [sp, #48]
	ldr r3, [sp, #24]
	str r4, [sp, #0]
	bl Func_02000db4
	mov r1, r10
	movs r0, #12
	ldrsh r3, [r1, r0]
	ldr r2, [sp, #52]
	mov r0, r10
	adds r3, r3, r2
	lsls r1, r3, #3
	movs r4, #14
	ldrsh r3, [r0, r4]
	ldr r4, [sp, #48]
	adds r2, r1, #0
	adds r3, r3, r4
	lsls r3, r3, #3
	adds r0, r3, #0
	ldr r3, [sp, #52]
	subs r2, #8
	adds r0, #11
	cmp r3, #4
	ble .L_ja_0200929c
	adds r2, r1, #0
.L_ja_0200929c:
	ldr r4, [sp, #52]
	cmp r4, #9
	ble .L_ja_020092a4
	adds r2, #8
.L_ja_020092a4:
	ldr r1, [sp, #52]
	cmp r1, #14
	ble .L_ja_020092ac
	adds r2, #8
.L_ja_020092ac:
	ldr r3, [sp, #52]
	cmp r3, #20
	bne .L_ja_020092b4
	adds r2, #8
.L_ja_020092b4:
	ldr r4, [sp, #20]
	ldr r1, [sp, #16]
	subs r3, r2, r4
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r4, r4, r3
	subs r3, r0, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r1, r1, r3
	str r1, [sp, #16]
	ldr r2, [sp, #16]
	add r0, sp, #108
	adds r1, r4, #0
	movs r3, #1
	str r4, [sp, #20]
	bl Func_020023f8
	add r0, sp, #108
	bl Func_020023f0
	movs r1, #50
	ldr r0, [sp, #60]
	bl __modsi3
	movs r1, #10
	adds r5, r0, #0
	bl __divsi3
	ldr r4, [sp, #56]
	lsls r0, r0, #1
	movs r2, #14
	ldrsh r3, [r4, r2]
	movs r1, #10
	adds r0, r0, r3
	ldr r3, .L_ja_02009428
	lsls r0, r0, #3
	ldr r3, [r3]
	subs r7, r0, #2
	lsrs r6, r3, #1
	movs r0, #7
	mov r8, r0
	ands r6, r0
	adds r0, r5, #0
	bl __modsi3
	ldr r4, [sp, #56]
	lsls r2, r0, #1
	movs r1, #12
	ldrsh r3, [r4, r1]
	adds r2, r2, r0
	lsls r2, r2, #2
	lsls r3, r3, #3
	adds r2, r2, r3
	adds r1, r2, #0
	adds r1, #16
	cmp r0, #4
	ble .L_ja_0200932e
	adds r1, #10
.L_ja_0200932e:
	ldr r2, .L_ja_0200942c
	mov r0, r8
	ldrsb r3, [r2, r6]
	adds r1, r1, r3
	adds r3, r6, #5
	ands r3, r0
	ldrsb r3, [r2, r3]
	add r0, sp, #92
	adds r7, r7, r3
	adds r2, r7, #0
	movs r3, #1
	bl Func_020023f8
	add r0, sp, #92
	bl Func_020023f0
	ldr r1, [sp, #8]
	cmp r1, #0
	ble .L_ja_02009366
	adds r1, #1
	str r1, [sp, #8]
	cmp r1, #8
	bne .L_ja_02009366
	ldr r3, [sp, #60]
	movs r2, #0
	adds r3, #1
	str r2, [sp, #8]
	str r3, [sp, #60]
.L_ja_02009366:
	ldr r4, [sp, #8]
	cmp r4, #0
	beq .L_ja_0200936e
	b .L_ja_0200974a
.L_ja_0200936e:
	ldr r0, [sp, #52]
	ldr r3, .L_ja_02009424
	cmp r0, #20
	bne .L_ja_0200938a
	ldr r1, [sp, #48]
	cmp r1, #0
	bne .L_ja_02009380
	ldr r3, [r3, #12]
	b .L_ja_0200938c
.L_ja_02009380:
	ldr r2, [sp, #48]
	cmp r2, #1
	bne .L_ja_0200938a
	ldr r3, [r3, #12]
	b .L_ja_0200938c
.L_ja_0200938a:
	ldr r3, [r3, #4]
.L_ja_0200938c:
	lsls r3, r3, #16
	asrs r2, r3, #16
	movs r3, #128
	lsls r2, r2, #16
	lsls r3, r3, #9
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200943c
	ldr r3, [sp, #12]
	cmp r3, #0
	bne .L_ja_0200943c
	ldr r0, [sp, #52]
	movs r4, #0
	mov r9, r4
	cmp r0, #20
	bne .L_ja_020093d0
	ldr r1, [sp, #48]
	cmp r1, #0
	bne .L_ja_020093b8
	movs r2, #1
	mov r9, r2
	b .L_ja_0200943c
.L_ja_020093b8:
	ldr r3, [sp, #48]
	cmp r3, #1
	bne .L_ja_020093c4
	movs r4, #2
	mov r9, r4
	b .L_ja_0200943c
.L_ja_020093c4:
	ldr r0, [sp, #48]
	cmp r0, #3
	bne .L_ja_0200943c
	movs r1, #3
	mov r9, r1
	b .L_ja_0200943c
.L_ja_020093d0:
	ldr r2, [sp, #60]
	ldr r3, [sp, #44]
	cmp r2, r3
	bge .L_ja_0200943c
	ldr r4, [sp, #48]
	ldr r0, [sp, #52]
	lsls r3, r4, #2
	ldr r2, .L_ja_02009430
	adds r3, r3, r4
	lsls r3, r3, #2
	adds r3, r0, r3
	ldrb r5, [r2, r3]
	cmp r5, #63
	bgt .L_ja_0200943c
	movs r0, #112
	bl Func_02002408
	ldr r3, .L_ja_02009434
	ldr r1, [sp, #60]
	strb r5, [r3, r1]
	ldr r0, [sp, #60]
	movs r1, #10
	bl __modsi3
	ldr r3, [sp, #44]
	ldr r2, [sp, #60]
	subs r3, #1
	cmp r2, r3
	bge .L_ja_02009438
	adds r0, r2, #0
	movs r1, #50
	bl __modsi3
	cmp r0, #49
	bne .L_ja_0200941c
	movs r3, #1
	str r3, [sp, #8]
	b .L_ja_0200943c
.L_ja_0200941c:
	ldr r4, [sp, #60]
	adds r4, #1
	str r4, [sp, #60]
	b .L_ja_0200943c
.L_ja_02009424:
	.4byte gInput
.L_ja_02009428:
	.4byte gVBlankCount
.L_ja_0200942c:
	.4byte gData_02002438
.L_ja_02009430:
	.4byte gData_0200265e
.L_ja_02009434:
	.4byte Data_0200274a
.L_ja_02009438:
	movs r0, #2
	str r0, [sp, #12]
.L_ja_0200943c:
	mov r1, r9
	cmp r1, #0
	bne .L_ja_0200944a
	ldr r2, [sp, #12]
	cmp r2, #1
	beq .L_ja_0200944a
	b .L_ja_0200974a
.L_ja_0200944a:
	ldr r1, .L_ja_020097cc
	movs r2, #8
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_ja_02009464
	mov r3, r9
	cmp r3, #3
	beq .L_ja_02009464
	ldr r4, [sp, #12]
	cmp r4, #1
	beq .L_ja_02009464
	b .L_ja_020096ba
.L_ja_02009464:
	ldr r7, .L_ja_020097d0
	ldr r1, [sp, #44]
	adds r2, r7, #0
	ldr r0, .L_ja_020097d4
	bl Func_02002068
	mov r9, r0
	mov r6, r9
	subs r6, #2
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_0200217c
	movs r3, #2
	adds r5, r0, #0
	movs r0, #0
	str r0, [sp, #12]
	str r3, [sp, #0]
	movs r1, #12
	movs r3, #3
	movs r0, #8
	movs r2, #14
	bl Window_Create
	lsls r5, r5, #16
	asrs r5, r5, #16
	ldrb r3, [r7, r6]
	lsls r5, r5, #16
	lsrs r1, r5, #16
	lsrs r5, r5, #24
	mov r8, r0
	cmp r3, r5
	bne .L_ja_0200957e
	mov r3, r9
	subs r3, #1
	ldrb r3, [r7, r3]
	movs r2, #255
	ands r2, r1
	cmp r3, r2
	bne .L_ja_0200957e
	ldr r0, .L_ja_020097d8
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	movs r0, #112
	bl Func_02002408
.L_ja_020094c6:
	movs r0, #1
	bl wait
	ldr r3, .L_ja_020097cc
	movs r2, #1
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_ja_020094c6
	movs r0, #112
	bl Func_02002408
	bl Func_02002350
	ldr r5, .L_ja_020097dc
	movs r1, #133
	lsls r1, r1, #2
	adds r2, r5, r1
	movs r3, #0
	str r3, [r2]
	movs r0, #4
	bl Party_RemoveActiveOwner
	movs r0, #5
	bl Party_RemoveActiveOwner
	movs r0, #6
	bl Party_RemoveActiveOwner
	movs r0, #7
	bl Party_RemoveActiveOwner
	movs r0, #0
	bl Party_AddActiveOwner
	movs r0, #1
	bl Party_AddActiveOwner
	movs r0, #2
	bl Party_AddActiveOwner
	movs r0, #3
	bl Party_AddActiveOwner
	ldr r1, .L_ja_020097d0
	ldr r0, [sp, #64]
	bl Func_020018e0
	mov r0, r8
	movs r1, #2
	bl Window_Close
	movs r1, #2
	ldr r0, [sp, #56]
	bl Window_Close
	mov r0, r10
	movs r1, #2
	bl Window_Close
	movs r2, #147
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	ldrb r0, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #38
	adds r5, r5, r3
	ldrb r1, [r5]
	bl Func_02002310
	movs r0, #74
	bl Func_02002408
	movs r0, #6
	bl wait
	movs r0, #0
	bl Func_020022f8
	bl Func_020023e8
	movs r2, #244
	ldr r3, .L_ja_020097e0
	ldr r0, .L_ja_020097e4
	ldr r1, .L_ja_020097e8
	lsls r2, r2, #4
	mov lr, r3
	.2byte 0xf800
	movs r0, #0
	b .L_ja_02009894
.L_ja_0200957e:
	movs r0, #113
	bl Func_02002408
	ldr r0, .L_ja_020097ec
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl Text_DrawCharacterAtOffset
.L_ja_02009590:
	movs r0, #1
	bl wait
	ldr r1, .L_ja_020097cc
	movs r2, #1
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_ja_020095ac
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_ja_02009590
.L_ja_020095ac:
	movs r1, #2
	mov r0, r8
	bl Window_Close
	mov r0, r10
	bl WindowSprite_RedrawSavedRect
	ldr r6, .L_ja_020097f0
	mov r1, r10
	adds r0, r6, #0
	movs r2, #0
	adds r6, #1
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #0
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #0
	movs r3, #16
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #0
	movs r3, #24
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #16
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #24
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #16
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #24
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #144
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	movs r2, #144
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	movs r3, #5
	str r3, [sp, #0]
	adds r6, #1
	mov r0, r10
	movs r1, #24
	movs r2, #0
	movs r3, #24
	bl Window_DrawDividerLine
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #192
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	movs r2, #192
	movs r3, #8
	adds r6, #1
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	movs r2, #192
	movs r3, #24
	bl Text_DrawCharacterAtOffset
	b .L_ja_0200974a
.L_ja_020096ba:
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_ja_020096cc
	mov r4, r9
	cmp r4, #2
	bne .L_ja_020096e0
.L_ja_020096cc:
	ldr r0, [sp, #60]
	cmp r0, #0
	ble .L_ja_0200974a
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #60]
	subs r1, #1
	str r1, [sp, #60]
	b .L_ja_0200974a
.L_ja_020096e0:
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_ja_020096f2
	mov r2, r9
	cmp r2, #1
	bne .L_ja_0200970a
.L_ja_020096f2:
	ldr r3, [sp, #44]
	ldr r4, [sp, #60]
	subs r3, #1
	cmp r4, r3
	bge .L_ja_0200974a
	movs r0, #111
	bl Func_02002408
	ldr r0, [sp, #60]
	adds r0, #1
	str r0, [sp, #60]
	b .L_ja_0200974a
.L_ja_0200970a:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200974a
	ldr r1, [sp, #60]
	cmp r1, #0
	ble .L_ja_02009728
	movs r0, #111
	bl Func_02002408
	ldr r2, [sp, #60]
	subs r2, #1
	str r2, [sp, #60]
	b .L_ja_0200974a
.L_ja_02009728:
	movs r0, #113
	bl Func_02002408
	movs r1, #2
	ldr r0, [sp, #56]
	bl Window_Close
	mov r0, r10
	movs r1, #2
	bl Window_Close
	movs r0, #74
	bl Func_02002408
	movs r0, #1
	negs r0, r0
	b .L_ja_02009894
.L_ja_0200974a:
	ldr r0, [sp, #60]
	movs r1, #50
	bl __divsi3
	mov r11, r0
	ldr r0, [sp, #56]
	bl WindowSprite_RedrawSavedRect
	movs r6, #0
	movs r5, #2
.L_ja_0200975e:
	adds r2, r5, #0
	ldr r0, [sp, #56]
	movs r1, #0
	movs r3, #18
	adds r6, #1
	str r5, [sp, #0]
	bl Window_DrawDividerLine
	adds r5, #2
	cmp r6, #4
	bne .L_ja_0200975e
	ldr r3, [sp, #40]
	movs r6, #0
	cmp r3, #0
	beq .L_ja_020097ae
	negs r3, r3
	adds r5, r3, #0
	adds r5, #18
.L_ja_02009782:
	movs r4, #243
	lsls r4, r4, #8
	adds r4, #1
	adds r1, r6, r4
	cmp r6, r11
	bne .L_ja_02009796
	movs r0, #243
	lsls r0, r0, #8
	adds r0, #11
	adds r1, r6, r0
.L_ja_02009796:
	movs r3, #0
	str r3, [sp, #0]
	adds r2, r5, #0
	ldr r0, [sp, #56]
	movs r3, #10
	bl Window_SetTilemapEntry
	ldr r1, [sp, #40]
	adds r6, #1
	adds r5, #1
	cmp r6, r1
	bne .L_ja_02009782
.L_ja_020097ae:
	ldr r2, [sp, #44]
	mov r9, r2
	cmp r2, #49
	ble .L_ja_020097f8
	ldr r3, [sp, #40]
	subs r3, #1
	cmp r3, r11
	bne .L_ja_020097f4
	ldr r0, [sp, #44]
	movs r1, #50
	bl __modsi3
	mov r9, r0
	b .L_ja_020097f8
	.2byte 0x0000
.L_ja_020097cc:
	.4byte gInput
.L_ja_020097d0:
	.4byte Data_0200274a + 0x104
.L_ja_020097d4:
	.4byte Data_0200274a
.L_ja_020097d8:
	.4byte 0x00000d92
.L_ja_020097dc:
	.4byte gMainRoutineWork
.L_ja_020097e0:
	.4byte IwramCopyWords
.L_ja_020097e4:
	.4byte Data_02012f94 + 0xac
.L_ja_020097e8:
	.4byte gGameFlagBytes
.L_ja_020097ec:
	.4byte 0x00000d91
.L_ja_020097f0:
	.4byte 0x00000d80
.L_ja_020097f4:
	movs r3, #50
	mov r9, r3
.L_ja_020097f8:
	mov r4, r9
	movs r6, #0
	cmp r4, #0
	beq .L_ja_02009858
	ldr r0, [sp, #36]
	ldr r1, [sp, #36]
	movs r2, #200
	adds r0, #200
	lsls r2, r2, #1
	mov r8, r0
	adds r7, r1, r2
.L_ja_0200980e:
	movs r3, #50
	mov r0, r11
	muls r0, r3
	ldr r4, .L_ja_020098a4
	adds r3, r0, #0
	adds r0, r6, r3
	ldrb r2, [r4, r0]
	ldrb r3, [r7]
	adds r1, r2, #0
	cmp r1, r3
	beq .L_ja_0200984c
	strb r2, [r7]
	cmp r1, #99
	bne .L_ja_02009838
	mov r1, r8
	ldr r0, [r1]
	ldr r3, .L_ja_020098a8
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
	b .L_ja_0200984c
.L_ja_02009838:
	add r5, sp, #68
	ldrb r0, [r4, r0]
	adds r1, r5, #0
	bl Func_02000a70
	mov r2, r8
	ldrh r0, [r5]
	ldr r1, [r2]
	bl Func_02002328
.L_ja_0200984c:
	movs r3, #4
	adds r6, #1
	add r8, r3
	adds r7, #1
	cmp r6, r9
	bne .L_ja_0200980e
.L_ja_02009858:
	cmp r6, #50
	beq .L_ja_0200988c
	ldr r4, [sp, #36]
	lsls r3, r6, #2
	movs r0, #200
	adds r3, r3, r4
	lsls r0, r0, #1
	adds r7, r3, #0
	adds r3, r6, r0
	adds r7, #200
	adds r5, r3, r4
.L_ja_0200986e:
	ldrb r3, [r5]
	cmp r3, #99
	beq .L_ja_02009882
	movs r3, #99
	strb r3, [r5]
	ldr r0, [r7]
	ldr r3, .L_ja_020098a8
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
.L_ja_02009882:
	adds r6, #1
	adds r7, #4
	adds r5, #1
	cmp r6, #50
	bne .L_ja_0200986e
.L_ja_0200988c:
	movs r0, #1
	bl wait
	b .L_ja_0200911e
.L_ja_02009894:
	add sp, #124
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_020098a4:
	.4byte Data_0200274a
.L_ja_020098a8:
	.4byte IwramClearWords
	.endif
	.ifdef TLA_EDITION_FR
	.global Func_02000e44
	.thumb_func
Func_02000e44:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #124
	bl Func_02002208
	ldr r3, .L_fr_02008ec4
	movs r0, #147
	movs r1, #128
	lsls r0, r0, #1
	lsls r1, r1, #2
	adds r0, #255
	adds r1, #38
	adds r2, r3, r0
	adds r3, r3, r1
	ldrb r0, [r2]
	ldrb r1, [r3]
	bl Func_02002310
	ldr r4, .L_fr_02008ec8
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_fr_02008ecc
	adds r1, r4, #0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_fr_02008ec0
	movs r0, #1
	strh r3, [r4, #8]
	bl Func_020022b0
	movs r2, #1
	negs r2, r2
	str r0, [sp, #64]
	cmp r0, r2
	bne .L_fr_02008e98
	.2byte 0xf000, 0xfca8 @ bl 0x020097e8
.L_fr_02008e98:
	movs r0, #71
	bl Func_02002408
	movs r0, #238
	movs r3, #0
	lsls r0, r0, #1
	str r3, [sp, #60]
	str r3, [sp, #52]
	str r3, [sp, #48]
	bl Heap_AllocEwram
	movs r4, #0
	str r0, [sp, #36]
	ldr r0, .L_fr_02008ed0
	str r4, [sp, #8]
	bl Func_02002320
	ldr r1, [sp, #64]
	b .L_fr_02008ed4
	.2byte 0x0000
.L_fr_02008ec0:
	.4byte 0x00006318
.L_fr_02008ec4:
	.4byte gMainRoutineWork
.L_fr_02008ec8:
	.4byte 0x050001c0
.L_fr_02008ecc:
	.4byte 0x050001e0
.L_fr_02008ed0:
	.4byte 0x06006000
.L_fr_02008ed4:
	movs r0, #130
	lsls r0, r0, #1
	str r0, [sp, #44]
	cmp r1, #0
	beq .L_fr_02008eec
	ldr r3, [sp, #64]
	movs r2, #61
	str r2, [sp, #44]
	cmp r3, #1
	beq .L_fr_02008eec
	movs r4, #16
	str r4, [sp, #44]
.L_fr_02008eec:
	ldr r0, [sp, #44]
	movs r1, #50
	bl __divsi3
	adds r0, #1
	str r0, [sp, #40]
	movs r5, #2
	movs r1, #13
	movs r2, #29
	movs r3, #6
	movs r0, #1
	str r5, [sp, #0]
	bl Window_Create
	mov r9, r0
	mov r1, r9
	ldr r0, .L_fr_02009158
	movs r2, #0
	movs r3, #0
	bl Text_DrawStringInWindow
	ldr r0, .L_fr_0200915c
	mov r1, r9
	movs r2, #0
	movs r3, #8
	bl Text_DrawStringInWindow
	ldr r0, .L_fr_02009160
	mov r1, r9
	movs r2, #0
	movs r3, #16
	bl Text_DrawStringInWindow
	ldr r0, .L_fr_02009164
	mov r1, r9
	movs r2, #0
	movs r3, #24
	bl Text_DrawStringInWindow
	ldr r6, .L_fr_02009168
	movs r3, #5
	str r3, [sp, #0]
	mov r0, r9
	movs r1, #20
	movs r2, #0
	movs r3, #20
	bl Window_DrawDividerLine
	adds r0, r6, #0
	mov r1, r9
	adds r6, #1
	movs r2, #160
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r9
	adds r6, #1
	movs r2, #160
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r9
	movs r2, #160
	movs r3, #24
	bl Text_DrawCharacterAtOffset
	movs r0, #5
	movs r1, #1
	movs r2, #20
	movs r3, #12
	str r5, [sp, #0]
	bl Window_Create
	str r0, [sp, #56]
	movs r6, #0
.L_fr_02008f86:
	ldr r2, [sp, #36]
	movs r0, #224
	lsls r0, r0, #1
	adds r1, r6, r0
	adds r2, #2
	movs r3, #0
	adds r6, #1
	strb r3, [r2, r1]
	cmp r6, #26
	bne .L_fr_02008f86
	ldr r2, .L_fr_0200916c
	movs r6, #0
.L_fr_02008f9e:
	movs r1, #130
	movs r3, #99
	adds r6, #1
	lsls r1, r1, #1
	strb r3, [r2]
	adds r2, #1
	cmp r6, r1
	bne .L_fr_02008f9e
	ldr r2, [sp, #36]
	movs r3, #200
	lsls r3, r3, #1
	movs r6, #0
	adds r2, r2, r3
	mov r10, r2
	mov r11, r6
.L_fr_02008fbc:
	movs r1, #10
	adds r0, r6, #0
	bl __modsi3
	adds r5, r0, #0
	lsls r3, r5, #1
	adds r3, r3, r5
	lsls r7, r3, #2
	movs r4, #8
	adds r4, r4, r7
	adds r0, r6, #0
	movs r1, #10
	mov r8, r4
	bl __divsi3
	lsls r0, r0, #4
	adds r3, r0, #2
	cmp r5, #4
	ble .L_fr_02008fe6
	adds r7, #18
	mov r8, r7
.L_fr_02008fe6:
	ldr r2, [sp, #36]
	mov r0, r11
	ldr r1, [r0, r2]
	ldr r0, [sp, #56]
	mov r2, r8
	bl Func_02002330
	ldr r3, [sp, #36]
	mov r2, r11
	adds r2, #200
	str r0, [r2, r3]
	mov r4, r10
	movs r3, #99
	strb r3, [r4]
	ldr r1, [sp, #36]
	movs r0, #1
	add r10, r0
	ldr r3, .L_fr_02009170
	ldr r0, [r2, r1]
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
	adds r6, #1
	movs r2, #4
	add r11, r2
	cmp r6, #50
	bne .L_fr_02008fbc
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_fr_02009056
	bl Func_02002300
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	mov r2, r9
	adds r0, r5, #0
	str r3, [sp, #0]
	bl WindowSprite_Create
	adds r1, r0, #0
	add r0, sp, #108
	str r1, [r0]
	mov r4, r9
	movs r3, #12
	ldrsh r1, [r4, r3]
	movs r3, #14
	ldrsh r2, [r4, r3]
	lsls r1, r1, #3
	lsls r2, r2, #3
	subs r1, #2
	adds r2, #12
	bl Func_02002400
.L_fr_02009056:
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_fr_0200909e
	bl Func_02002308
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	mov r2, r9
	adds r0, r5, #0
	str r3, [sp, #0]
	bl WindowSprite_Create
	adds r1, r0, #0
	add r0, sp, #92
	str r1, [r0]
	movs r3, #255
	ldrb r2, [r1, #25]
	strb r3, [r1, #15]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	strb r3, [r1, #25]
	ldr r2, [sp, #56]
	movs r4, #12
	ldrsh r1, [r2, r4]
	movs r3, #14
	ldrsh r2, [r2, r3]
	lsls r1, r1, #3
	lsls r2, r2, #3
	subs r1, #4
	adds r2, #12
	bl Func_02002400
.L_fr_0200909e:
	movs r4, #0
	movs r0, #1
	str r0, [sp, #24]
	str r4, [sp, #32]
	str r4, [sp, #28]
	mov r2, r9
	movs r1, #12
	ldrsh r3, [r2, r1]
	movs r0, #0
	lsls r3, r3, #3
	subs r3, #8
	str r3, [sp, #20]
	movs r4, #14
	ldrsh r3, [r2, r4]
	str r0, [sp, #12]
	lsls r3, r3, #3
	adds r3, #11
	str r3, [sp, #16]
.L_fr_020090c2:
	ldr r2, [sp, #12]
	movs r1, #1
	negs r1, r1
	mov r10, r1
	cmp r2, #2
	bne .L_fr_020090d2
	movs r3, #1
	str r3, [sp, #12]
.L_fr_020090d2:
	movs r3, #15
	movs r4, #1
	str r3, [sp, #4]
	mov r0, r9
	ldr r1, [sp, #32]
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	str r4, [sp, #0]
	bl Func_02000db4
	ldr r0, [sp, #12]
	cmp r0, #0
	beq .L_fr_020090ee
	b .L_fr_020091f4
.L_fr_020090ee:
	ldr r3, .L_fr_02009174
	movs r2, #16
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_fr_02009126
	movs r0, #111
	bl Func_02002408
	ldr r0, [sp, #52]
	ldr r1, [sp, #48]
	adds r0, #1
	cmp r1, #2
	bne .L_fr_0200911e
	adds r3, r0, #0
	cmp r0, #0
	bge .L_fr_02009114
	ldr r3, [sp, #52]
	adds r3, #16
.L_fr_02009114:
	asrs r3, r3, #4
	str r3, [sp, #52]
	lsls r3, r3, #4
	subs r0, r0, r3
	b .L_fr_02009124
.L_fr_0200911e:
	movs r1, #17
	bl __modsi3
.L_fr_02009124:
	str r0, [sp, #52]
.L_fr_02009126:
	ldr r3, .L_fr_02009174
	movs r2, #32
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_fr_02009184
	movs r0, #111
	bl Func_02002408
	ldr r2, [sp, #48]
	cmp r2, #2
	bne .L_fr_02009178
	ldr r2, [sp, #52]
	adds r2, #15
	adds r3, r2, #0
	cmp r2, #0
	bge .L_fr_0200914c
	ldr r3, [sp, #52]
	adds r3, #30
.L_fr_0200914c:
	asrs r3, r3, #4
	str r3, [sp, #52]
	lsls r3, r3, #4
	subs r2, r2, r3
	str r2, [sp, #52]
	b .L_fr_02009184
.L_fr_02009158:
	.4byte gData_02002440
.L_fr_0200915c:
	.4byte gData_02002454
.L_fr_02009160:
	.4byte gData_02002468
.L_fr_02009164:
	.4byte gData_0200247c
.L_fr_02009168:
	.4byte 0x00000f5a
.L_fr_0200916c:
	.4byte Data_0200274a
.L_fr_02009170:
	.4byte IwramClearWords
.L_fr_02009174:
	.4byte gInput
.L_fr_02009178:
	ldr r0, [sp, #52]
	movs r1, #17
	adds r0, #16
	bl __modsi3
	str r0, [sp, #52]
.L_fr_02009184:
	ldr r3, .L_fr_020093f8
	movs r2, #128
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_fr_020091bc
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #48]
	adds r1, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_fr_020091a4
	ldr r3, [sp, #48]
	adds r3, #4
.L_fr_020091a4:
	asrs r3, r3, #2
	str r3, [sp, #48]
	lsls r3, r3, #2
	subs r1, r1, r3
	ldr r3, [sp, #52]
	str r1, [sp, #48]
	cmp r3, #16
	bne .L_fr_020091bc
	cmp r1, #2
	bne .L_fr_020091bc
	movs r4, #3
	str r4, [sp, #48]
.L_fr_020091bc:
	ldr r3, .L_fr_020093f8
	movs r2, #64
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_fr_020091f4
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #48]
	adds r1, #3
	adds r3, r1, #0
	cmp r1, #0
	bge .L_fr_020091dc
	ldr r3, [sp, #48]
	adds r3, #6
.L_fr_020091dc:
	asrs r3, r3, #2
	ldr r0, [sp, #52]
	str r3, [sp, #48]
	lsls r3, r3, #2
	subs r1, r1, r3
	str r1, [sp, #48]
	cmp r0, #16
	bne .L_fr_020091fe
	cmp r1, #2
	bne .L_fr_020091f4
	movs r1, #1
	str r1, [sp, #48]
.L_fr_020091f4:
	ldr r3, [sp, #52]
	movs r2, #6
	str r2, [sp, #24]
	cmp r3, #16
	beq .L_fr_02009202
.L_fr_020091fe:
	movs r4, #1
	str r4, [sp, #24]
.L_fr_02009202:
	ldr r0, [sp, #52]
	ldr r1, [sp, #48]
	str r0, [sp, #32]
	str r1, [sp, #28]
	cmp r0, #4
	ble .L_fr_02009214
	adds r2, r0, #0
	adds r2, #1
	str r2, [sp, #32]
.L_fr_02009214:
	ldr r3, [sp, #52]
	cmp r3, #9
	ble .L_fr_02009220
	ldr r4, [sp, #32]
	adds r4, #1
	str r4, [sp, #32]
.L_fr_02009220:
	ldr r0, [sp, #52]
	cmp r0, #14
	ble .L_fr_0200922c
	ldr r1, [sp, #32]
	adds r1, #1
	str r1, [sp, #32]
.L_fr_0200922c:
	ldr r2, [sp, #52]
	cmp r2, #15
	ble .L_fr_02009238
	ldr r3, [sp, #32]
	adds r3, #1
	str r3, [sp, #32]
.L_fr_02009238:
	movs r3, #14
	movs r4, #1
	mov r0, r9
	str r3, [sp, #4]
	ldr r1, [sp, #32]
	ldr r2, [sp, #48]
	ldr r3, [sp, #24]
	str r4, [sp, #0]
	bl Func_02000db4
	mov r1, r9
	movs r0, #12
	ldrsh r3, [r1, r0]
	ldr r2, [sp, #52]
	mov r0, r9
	adds r3, r3, r2
	lsls r1, r3, #3
	movs r4, #14
	ldrsh r3, [r0, r4]
	ldr r4, [sp, #48]
	adds r2, r1, #0
	adds r3, r3, r4
	lsls r3, r3, #3
	adds r0, r3, #0
	ldr r3, [sp, #52]
	subs r2, #8
	adds r0, #11
	cmp r3, #4
	ble .L_fr_02009274
	adds r2, r1, #0
.L_fr_02009274:
	ldr r4, [sp, #52]
	cmp r4, #9
	ble .L_fr_0200927c
	adds r2, #8
.L_fr_0200927c:
	ldr r1, [sp, #52]
	cmp r1, #14
	ble .L_fr_02009284
	adds r2, #8
.L_fr_02009284:
	ldr r3, [sp, #52]
	cmp r3, #15
	ble .L_fr_0200928c
	adds r2, #8
.L_fr_0200928c:
	ldr r4, [sp, #20]
	ldr r1, [sp, #16]
	subs r3, r2, r4
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r4, r4, r3
	subs r3, r0, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r1, r1, r3
	str r1, [sp, #16]
	ldr r2, [sp, #16]
	add r0, sp, #108
	adds r1, r4, #0
	movs r3, #1
	str r4, [sp, #20]
	bl Func_020023f8
	add r0, sp, #108
	bl Func_020023f0
	movs r1, #50
	ldr r0, [sp, #60]
	bl __modsi3
	movs r1, #10
	adds r5, r0, #0
	bl __divsi3
	ldr r4, [sp, #56]
	lsls r0, r0, #1
	movs r2, #14
	ldrsh r3, [r4, r2]
	movs r1, #10
	adds r0, r0, r3
	ldr r3, .L_fr_020093fc
	lsls r0, r0, #3
	ldr r3, [r3]
	subs r7, r0, #2
	lsrs r6, r3, #1
	movs r0, #7
	mov r8, r0
	ands r6, r0
	adds r0, r5, #0
	bl __modsi3
	ldr r4, [sp, #56]
	lsls r2, r0, #1
	movs r1, #12
	ldrsh r3, [r4, r1]
	adds r2, r2, r0
	lsls r2, r2, #2
	lsls r3, r3, #3
	adds r2, r2, r3
	adds r1, r2, #0
	adds r1, #16
	cmp r0, #4
	ble .L_fr_02009306
	adds r1, #10
.L_fr_02009306:
	ldr r2, .L_fr_02009400
	mov r0, r8
	ldrsb r3, [r2, r6]
	adds r1, r1, r3
	adds r3, r6, #5
	ands r3, r0
	ldrsb r3, [r2, r3]
	add r0, sp, #92
	adds r7, r7, r3
	adds r2, r7, #0
	movs r3, #1
	bl Func_020023f8
	add r0, sp, #92
	bl Func_020023f0
	ldr r1, [sp, #8]
	cmp r1, #0
	ble .L_fr_0200933e
	adds r1, #1
	str r1, [sp, #8]
	cmp r1, #8
	bne .L_fr_0200933e
	ldr r3, [sp, #60]
	movs r2, #0
	adds r3, #1
	str r2, [sp, #8]
	str r3, [sp, #60]
.L_fr_0200933e:
	ldr r4, [sp, #8]
	cmp r4, #0
	beq .L_fr_02009346
	b .L_fr_0200968a
.L_fr_02009346:
	ldr r0, [sp, #52]
	ldr r3, .L_fr_020093f8
	cmp r0, #16
	bne .L_fr_02009362
	ldr r1, [sp, #48]
	cmp r1, #0
	bne .L_fr_02009358
	ldr r3, [r3, #12]
	b .L_fr_02009364
.L_fr_02009358:
	ldr r2, [sp, #48]
	cmp r2, #1
	bne .L_fr_02009362
	ldr r3, [r3, #12]
	b .L_fr_02009364
.L_fr_02009362:
	ldr r3, [r3, #4]
.L_fr_02009364:
	lsls r3, r3, #16
	asrs r2, r3, #16
	movs r3, #128
	lsls r2, r2, #16
	lsls r3, r3, #9
	ands r3, r2
	cmp r3, #0
	beq .L_fr_02009410
	ldr r3, [sp, #12]
	cmp r3, #0
	bne .L_fr_02009410
	ldr r0, [sp, #52]
	movs r4, #0
	mov r10, r4
	cmp r0, #16
	bne .L_fr_020093a8
	ldr r1, [sp, #48]
	cmp r1, #0
	bne .L_fr_02009390
	movs r2, #1
	mov r10, r2
	b .L_fr_02009410
.L_fr_02009390:
	ldr r3, [sp, #48]
	cmp r3, #1
	bne .L_fr_0200939c
	movs r4, #2
	mov r10, r4
	b .L_fr_02009410
.L_fr_0200939c:
	ldr r0, [sp, #48]
	cmp r0, #3
	bne .L_fr_02009410
	movs r1, #3
	mov r10, r1
	b .L_fr_02009410
.L_fr_020093a8:
	ldr r2, [sp, #60]
	ldr r3, [sp, #44]
	cmp r2, r3
	bge .L_fr_02009410
	ldr r4, [sp, #48]
	ldr r0, [sp, #52]
	ldr r2, .L_fr_02009404
	lsls r3, r4, #4
	adds r3, r0, r3
	ldrb r5, [r2, r3]
	cmp r5, #63
	bgt .L_fr_02009410
	movs r0, #112
	bl Func_02002408
	ldr r3, .L_fr_02009408
	ldr r1, [sp, #60]
	strb r5, [r3, r1]
	ldr r0, [sp, #60]
	movs r1, #10
	bl __modsi3
	ldr r3, [sp, #44]
	ldr r2, [sp, #60]
	subs r3, #1
	cmp r2, r3
	bge .L_fr_0200940c
	adds r0, r2, #0
	movs r1, #50
	bl __modsi3
	cmp r0, #49
	bne .L_fr_020093f0
	movs r3, #1
	str r3, [sp, #8]
	b .L_fr_02009410
.L_fr_020093f0:
	ldr r4, [sp, #60]
	adds r4, #1
	str r4, [sp, #60]
	b .L_fr_02009410
.L_fr_020093f8:
	.4byte gInput
.L_fr_020093fc:
	.4byte gVBlankCount
.L_fr_02009400:
	.4byte gData_02002438
.L_fr_02009404:
	.4byte gData_0200265e
.L_fr_02009408:
	.4byte Data_0200274a
.L_fr_0200940c:
	movs r0, #2
	str r0, [sp, #12]
.L_fr_02009410:
	mov r1, r10
	cmp r1, #0
	bne .L_fr_0200941e
	ldr r2, [sp, #12]
	cmp r2, #1
	beq .L_fr_0200941e
	b .L_fr_0200968a
.L_fr_0200941e:
	ldr r1, .L_fr_02009750
	movs r2, #8
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_fr_02009438
	mov r3, r10
	cmp r3, #3
	beq .L_fr_02009438
	ldr r4, [sp, #12]
	cmp r4, #1
	beq .L_fr_02009438
	b .L_fr_020095fa
.L_fr_02009438:
	ldr r7, .L_fr_02009754
	ldr r1, [sp, #44]
	adds r2, r7, #0
	ldr r0, .L_fr_02009758
	bl Func_02002068
	mov r10, r0
	mov r6, r10
	subs r6, #2
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_0200217c
	movs r3, #2
	adds r5, r0, #0
	movs r0, #0
	str r0, [sp, #12]
	str r3, [sp, #0]
	movs r1, #12
	movs r3, #3
	movs r0, #5
	movs r2, #20
	bl Window_Create
	lsls r5, r5, #16
	asrs r5, r5, #16
	ldrb r3, [r7, r6]
	lsls r5, r5, #16
	lsrs r1, r5, #16
	lsrs r5, r5, #24
	mov r8, r0
	cmp r3, r5
	bne .L_fr_02009552
	mov r3, r10
	subs r3, #1
	ldrb r3, [r7, r3]
	movs r2, #255
	ands r2, r1
	cmp r3, r2
	bne .L_fr_02009552
	ldr r0, .L_fr_0200975c
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	movs r0, #112
	bl Func_02002408
.L_fr_0200949a:
	movs r0, #1
	bl wait
	ldr r3, .L_fr_02009750
	movs r2, #1
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_fr_0200949a
	movs r0, #112
	bl Func_02002408
	bl Func_02002350
	ldr r5, .L_fr_02009760
	movs r1, #133
	lsls r1, r1, #2
	adds r2, r5, r1
	movs r3, #0
	str r3, [r2]
	movs r0, #4
	bl Party_RemoveActiveOwner
	movs r0, #5
	bl Party_RemoveActiveOwner
	movs r0, #6
	bl Party_RemoveActiveOwner
	movs r0, #7
	bl Party_RemoveActiveOwner
	movs r0, #0
	bl Party_AddActiveOwner
	movs r0, #1
	bl Party_AddActiveOwner
	movs r0, #2
	bl Party_AddActiveOwner
	movs r0, #3
	bl Party_AddActiveOwner
	ldr r1, .L_fr_02009754
	ldr r0, [sp, #64]
	bl Func_020018e0
	mov r0, r8
	movs r1, #2
	bl Window_Close
	movs r1, #2
	ldr r0, [sp, #56]
	bl Window_Close
	mov r0, r9
	movs r1, #2
	bl Window_Close
	movs r2, #147
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	ldrb r0, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #38
	adds r5, r5, r3
	ldrb r1, [r5]
	bl Func_02002310
	movs r0, #74
	bl Func_02002408
	movs r0, #6
	bl wait
	movs r0, #0
	bl Func_020022f8
	bl Func_020023e8
	movs r2, #244
	ldr r3, .L_fr_02009764
	ldr r0, .L_fr_02009768
	ldr r1, .L_fr_0200976c
	lsls r2, r2, #4
	mov lr, r3
	.2byte 0xf800
	movs r0, #0
	b .L_fr_020097e8
.L_fr_02009552:
	movs r0, #113
	bl Func_02002408
	ldr r0, .L_fr_02009770
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl Text_DrawCharacterAtOffset
.L_fr_02009564:
	movs r0, #1
	bl wait
	ldr r1, .L_fr_02009750
	movs r2, #1
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_fr_02009580
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_fr_02009564
.L_fr_02009580:
	movs r1, #2
	mov r0, r8
	bl Window_Close
	mov r0, r9
	bl WindowSprite_RedrawSavedRect
	ldr r0, .L_fr_02009774
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl Text_DrawStringInWindow
	ldr r0, .L_fr_02009778
	mov r1, r9
	movs r2, #0
	movs r3, #8
	bl Text_DrawStringInWindow
	ldr r0, .L_fr_0200977c
	mov r1, r9
	movs r2, #0
	movs r3, #16
	bl Text_DrawStringInWindow
	ldr r0, .L_fr_02009780
	mov r1, r9
	movs r2, #0
	movs r3, #24
	bl Text_DrawStringInWindow
	ldr r6, .L_fr_02009784
	movs r3, #5
	str r3, [sp, #0]
	mov r0, r9
	movs r1, #20
	movs r2, #0
	movs r3, #20
	bl Window_DrawDividerLine
	adds r0, r6, #0
	mov r1, r9
	adds r6, #1
	movs r2, #160
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r9
	movs r2, #160
	movs r3, #8
	adds r6, #1
	bl Text_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r9
	movs r2, #160
	movs r3, #24
	bl Text_DrawCharacterAtOffset
	b .L_fr_0200968a
.L_fr_020095fa:
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_fr_0200960c
	mov r4, r10
	cmp r4, #2
	bne .L_fr_02009620
.L_fr_0200960c:
	ldr r0, [sp, #60]
	cmp r0, #0
	ble .L_fr_0200968a
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #60]
	subs r1, #1
	str r1, [sp, #60]
	b .L_fr_0200968a
.L_fr_02009620:
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_fr_02009632
	mov r2, r10
	cmp r2, #1
	bne .L_fr_0200964a
.L_fr_02009632:
	ldr r3, [sp, #44]
	ldr r4, [sp, #60]
	subs r3, #1
	cmp r4, r3
	bge .L_fr_0200968a
	movs r0, #111
	bl Func_02002408
	ldr r0, [sp, #60]
	adds r0, #1
	str r0, [sp, #60]
	b .L_fr_0200968a
.L_fr_0200964a:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_fr_0200968a
	ldr r1, [sp, #60]
	cmp r1, #0
	ble .L_fr_02009668
	movs r0, #111
	bl Func_02002408
	ldr r2, [sp, #60]
	subs r2, #1
	str r2, [sp, #60]
	b .L_fr_0200968a
.L_fr_02009668:
	movs r0, #113
	bl Func_02002408
	movs r1, #2
	ldr r0, [sp, #56]
	bl Window_Close
	mov r0, r9
	movs r1, #2
	bl Window_Close
	movs r0, #74
	bl Func_02002408
	movs r0, #1
	negs r0, r0
	b .L_fr_020097e8
.L_fr_0200968a:
	ldr r0, [sp, #60]
	movs r1, #50
	bl __divsi3
	mov r11, r0
	ldr r0, [sp, #56]
	bl WindowSprite_RedrawSavedRect
	movs r6, #0
	movs r5, #2
.L_fr_0200969e:
	adds r2, r5, #0
	ldr r0, [sp, #56]
	movs r1, #0
	movs r3, #18
	adds r6, #1
	str r5, [sp, #0]
	bl Window_DrawDividerLine
	adds r5, #2
	cmp r6, #4
	bne .L_fr_0200969e
	ldr r3, [sp, #40]
	movs r6, #0
	cmp r3, #0
	beq .L_fr_020096ee
	negs r3, r3
	adds r5, r3, #0
	adds r5, #18
.L_fr_020096c2:
	movs r4, #243
	lsls r4, r4, #8
	adds r4, #1
	adds r1, r6, r4
	cmp r6, r11
	bne .L_fr_020096d6
	movs r0, #243
	lsls r0, r0, #8
	adds r0, #11
	adds r1, r6, r0
.L_fr_020096d6:
	movs r3, #0
	str r3, [sp, #0]
	adds r2, r5, #0
	ldr r0, [sp, #56]
	movs r3, #10
	bl Window_SetTilemapEntry
	ldr r1, [sp, #40]
	adds r6, #1
	adds r5, #1
	cmp r6, r1
	bne .L_fr_020096c2
.L_fr_020096ee:
	ldr r2, [sp, #44]
	mov r10, r2
	cmp r2, #49
	ble .L_fr_0200970e
	ldr r3, [sp, #40]
	subs r3, #1
	cmp r3, r11
	bne .L_fr_0200970a
	ldr r0, [sp, #44]
	movs r1, #50
	bl __modsi3
	mov r10, r0
	b .L_fr_0200970e
.L_fr_0200970a:
	movs r3, #50
	mov r10, r3
.L_fr_0200970e:
	mov r4, r10
	movs r6, #0
	cmp r4, #0
	beq .L_fr_020097ac
	ldr r0, [sp, #36]
	ldr r1, [sp, #36]
	movs r2, #200
	adds r0, #200
	lsls r2, r2, #1
	mov r8, r0
	adds r7, r1, r2
.L_fr_02009724:
	movs r3, #50
	mov r0, r11
	muls r0, r3
	ldr r4, .L_fr_02009758
	adds r3, r0, #0
	adds r0, r6, r3
	ldrb r2, [r4, r0]
	ldrb r3, [r7]
	adds r1, r2, #0
	cmp r1, r3
	beq .L_fr_020097a0
	strb r2, [r7]
	cmp r1, #99
	bne .L_fr_0200978c
	mov r1, r8
	ldr r0, [r1]
	ldr r3, .L_fr_02009788
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
	b .L_fr_020097a0
	.2byte 0x0000
.L_fr_02009750:
	.4byte gInput
.L_fr_02009754:
	.4byte Data_0200288a
.L_fr_02009758:
	.4byte Data_0200274a
.L_fr_0200975c:
	.4byte 0x00000f5e
.L_fr_02009760:
	.4byte gMainRoutineWork
.L_fr_02009764:
	.4byte IwramCopyWords
.L_fr_02009768:
	.4byte 0x02013040
.L_fr_0200976c:
	.4byte gGameFlagBytes
.L_fr_02009770:
	.4byte 0x00000f5d
.L_fr_02009774:
	.4byte gData_02002440
.L_fr_02009778:
	.4byte gData_02002454
.L_fr_0200977c:
	.4byte gData_02002468
.L_fr_02009780:
	.4byte gData_0200247c
.L_fr_02009784:
	.4byte 0x00000f5a
.L_fr_02009788:
	.4byte IwramClearWords
.L_fr_0200978c:
	add r5, sp, #68
	ldrb r0, [r4, r0]
	adds r1, r5, #0
	bl Func_02000a70
	mov r2, r8
	ldrh r0, [r5]
	ldr r1, [r2]
	bl Func_02002328
.L_fr_020097a0:
	movs r3, #4
	adds r6, #1
	add r8, r3
	adds r7, #1
	cmp r6, r10
	bne .L_fr_02009724
.L_fr_020097ac:
	cmp r6, #50
	beq .L_fr_020097e0
	ldr r4, [sp, #36]
	lsls r3, r6, #2
	movs r0, #200
	adds r3, r3, r4
	lsls r0, r0, #1
	adds r7, r3, #0
	adds r3, r6, r0
	adds r7, #200
	adds r5, r3, r4
.L_fr_020097c2:
	ldrb r3, [r5]
	cmp r3, #99
	beq .L_fr_020097d6
	movs r3, #99
	strb r3, [r5]
	ldr r0, [r7]
	ldr r3, .L_fr_020097f8
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
.L_fr_020097d6:
	adds r6, #1
	adds r7, #4
	adds r5, #1
	cmp r6, #50
	bne .L_fr_020097c2
.L_fr_020097e0:
	movs r0, #1
	bl wait
	b .L_fr_020090c2
.L_fr_020097e8:
	sub sp, #-124
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_fr_020097f8:
	.4byte IwramClearWords
	.endif
	.section .bss,"aw",%nobits
	.global Data_0200274a
Data_0200274a:
	.space 0x00000140
	.global Data_0200288a
Data_0200288a:
