.syntax unified
	.thumb
	.section .text.x020083c4,"ax",%progbits
	.global Func_020003c4
	.thumb_func
Func_020003c4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	sub sp, #4
	cmp r3, #0
	beq .L_020083e2
	b .L_0200852e
.L_020083e2:
	movs r0, #192
	lsls r0, r0, #4
	adds r0, #164
	adds r3, r2, r0
	movs r5, #0
	ldrsb r5, [r3, r5]
	cmp r5, #0
	beq .L_020083f4
	b .L_0200852e
.L_020083f4:
	ldr r7, .L_02008538
	movs r1, #1
	ldr r3, [r7]
	negs r1, r1
	cmp r3, r1
	bne .L_0200842c
	movs r1, #160
	lsls r1, r1, #19
	adds r1, #64
	movs r2, #32
	ldr r3, .L_0200853c
	ldr r0, .L_02008540
	mov lr, r3
	.2byte 0xf800
	ldr r6, .L_02008544
	ldr r1, .L_02008548
	movs r2, #64
	ldr r3, .L_0200853c
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	ldr r0, .L_0200854c
	adds r1, r6, #0
	movs r2, #64
	ldr r3, .L_0200853c
	mov lr, r3
	.2byte 0xf800
	str r5, [r7]
.L_0200842c:
	ldr r0, [r7]
	cmp r0, #0
	bge .L_02008434
	adds r0, #7
.L_02008434:
	asrs r0, r0, #3
	movs r1, #6
	bl __modsi3
	movs r7, #8
	mov r8, r0
	movs r4, #0
.L_02008442:
	mov r1, r8
	adds r0, r1, r4
	movs r1, #6
	str r4, [sp, #0]
	bl __modsi3
	ldr r2, .L_02008544
	lsls r1, r0, #1
	adds r3, r1, #0
	adds r3, #8
	ldrh r3, [r2, r3]
	ldr r6, .L_0200854c
	ldr r0, .L_02008544
	strh r3, [r6, r7]
	adds r3, r1, #0
	adds r3, #20
	ldrh r3, [r0, r3]
	adds r2, r7, #0
	adds r2, #12
	strh r3, [r6, r2]
	adds r3, r1, #0
	adds r3, #40
	ldrh r3, [r0, r3]
	adds r2, #20
	strh r3, [r6, r2]
	adds r3, r1, #0
	adds r3, #52
	ldr r4, [sp, #0]
	ldrh r3, [r0, r3]
	adds r2, #12
	adds r4, #1
	strh r3, [r6, r2]
	adds r7, #2
	cmp r4, #5
	ble .L_02008442
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084a4
	adds r0, r6, #0
	adds r0, #20
	ldr r1, .L_02008550
	ldr r3, .L_0200853c
	movs r2, #12
	mov lr, r3
	.2byte 0xf800
.L_020084a4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #51
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084c0
	adds r0, r6, #0
	adds r0, #40
	ldr r1, .L_02008550
	ldr r3, .L_0200853c
	movs r2, #12
	mov lr, r3
	.2byte 0xf800
.L_020084c0:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #52
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084dc
	adds r0, r6, #0
	adds r0, #52
	ldr r1, .L_02008550
	ldr r3, .L_0200853c
	movs r2, #12
	mov lr, r3
	.2byte 0xf800
.L_020084dc:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #53
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084f8
	adds r0, r6, #0
	adds r0, #8
	ldr r1, .L_02008550
	ldr r3, .L_0200853c
	movs r2, #12
	mov lr, r3
	.2byte 0xf800
.L_020084f8:
	ldr r1, .L_02008554
	ldr r0, .L_02008558
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02008524
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r1
	adds r3, #4
	adds r2, #1
	stmia r3!, {r6}
	strh r2, [r1]
	ldr r2, .L_02008548
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	str r2, [r3]
.L_02008524:
	strh r4, [r0]
	ldr r2, .L_02008538
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200852e:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008538:
	.4byte gData_02003704
.L_0200853c:
	.4byte IwramCopyWords
.L_02008540:
	.4byte gData_02003a5c
.L_02008544:
	.4byte Data_020039dc
.L_02008548:
	.4byte 0x05000100
.L_0200854c:
	.4byte Data_02003a1c
.L_02008550:
	.4byte Data_0200ba70
.L_02008554:
	.4byte gIoWriteQueue
.L_02008558:
	.4byte 0x04000208
	.section .text.x0200a368,"ax",%progbits
	.global Func_02002368
	.thumb_func
Func_02002368:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #16
	ldr r6, [r3, #108]
	bl Func_02003298
	mov r8, r0
	bl Actor_GetById
	bl Party_CountActiveOwners
	movs r5, #0
	adds r7, r0, #0
	cmp r5, r7
	bge .L_0200a3aa
.L_0200a38e:
	ldr r2, .L_0200a450
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
	blt .L_0200a38e
.L_0200a3aa:
	movs r0, #10
	negs r0, r0
	movs r1, #0
	bl Func_02003288
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
	bl Func_02003210
	cmp r5, r7
	bge .L_0200a444
.L_0200a3d6:
	ldr r1, .L_0200a450
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
	ble .L_0200a3fe
	movs r1, #183
	lsls r1, r1, #1
	adds r2, r6, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a43e
.L_0200a3fe:
	mov r3, sp
	lsls r2, r5, #1
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a43e
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
.L_0200a43e:
	adds r5, #1
	cmp r5, r7
	blt .L_0200a3d6
.L_0200a444:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a450:
	.4byte gMainRoutineWork
	.section .text.x0200a4e4,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_020024e4
	.thumb_func
Func_020024e4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_ja_0200a664
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #12
	cmp r3, #0
	beq .L_ja_0200a4f4
	b .L_ja_0200a654
.L_ja_0200a4f4:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02002368
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r2, .L_ja_0200a668
	adds r5, r0, #0
	mov r10, r2
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	ldr r3, .L_ja_0200a66c
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r1, #49
	bl Func_02003118
.L_ja_0200a52c:
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldrb r0, [r3]
	ldr r2, [r5, #16]
	bl Func_02003168
	mov r6, r10
	mov r7, r10
	adds r6, #104
	adds r7, #106
	mov r1, sp
	cmp r0, #7
	bne .L_ja_0200a5a4
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
.L_ja_0200a562:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_02003180
	cmp r0, #0
	beq .L_ja_0200a592
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	b .L_ja_0200a648
.L_ja_0200a592:
	movs r0, #1
	bl wait
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	ble .L_ja_0200a562
	b .L_ja_0200a648
.L_ja_0200a5a4:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_02003180
	cmp r0, #0
	bgt .L_ja_0200a648
	cmp r0, #0
	bge .L_ja_0200a5ea
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	ldr r3, .L_ja_0200a664
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_02003200
	b .L_ja_0200a648
.L_ja_0200a5ea:
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r5, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldr r7, .L_ja_0200a670
	lsls r3, r3, #17
	ldr r0, .L_ja_0200a674
	adds r6, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r6, [r5, #16]
	cmp r2, r4
	ble .L_ja_0200a616
	adds r2, r4, #0
.L_ja_0200a616:
	ldr r0, .L_ja_0200a678
	cmp r2, r0
	bge .L_ja_0200a61e
	adds r2, r0, #0
.L_ja_0200a61e:
	subs r3, r1, r2
	ldr r1, .L_ja_0200a674
	str r3, [r5, #8]
	adds r3, r6, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_ja_0200a630
	adds r2, r4, #0
.L_ja_0200a630:
	cmp r2, r0
	bge .L_ja_0200a636
	adds r2, r0, #0
.L_ja_0200a636:
	subs r3, r6, r2
	str r3, [r5, #16]
	mov r2, r10
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_ja_0200a52c
.L_ja_0200a648:
	movs r3, #0
	str r3, [r5, #108]
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_ja_0200a654:
	mov r0, r10
	movs r3, #0
	strh r3, [r0, #4]
	sub sp, #-12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_ja_0200a664:
	.4byte gMainRoutineWork
.L_ja_0200a668:
	.4byte gData_020023c4 + 0x188
.L_ja_0200a66c:
	.4byte Func_020024bc
.L_ja_0200a670:
	.4byte 0x000fffff
.L_ja_0200a674:
	.4byte 0xfff80000
.L_ja_0200a678:
	.4byte 0xffff0000
	.endif
	.section .text.x0200a870,"ax",%progbits
	.global Func_02002870
	.thumb_func
Func_02002870:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200a9d8
	sub sp, #4
	ldr r3, [r1, #112]
	mov r11, r0
	cmp r3, #0
	bne .L_0200a88c
	b .L_0200a9e4
.L_0200a88c:
	movs r2, #0
	str r2, [sp, #0]
.L_0200a890:
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
	bl Func_02003128
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200a9c4
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
	ldr r1, .L_0200a9dc
	adds r0, r7, #0
	mov r9, r2
	bl Func_02003120
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
	ldr r3, .L_0200a9e0
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
	ldr r2, .L_0200a9d8
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
	ldr r1, .L_0200a9d8
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
	ldr r1, .L_0200a9d4
	ldrh r3, [r5, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
.L_0200a9c4:
	ldr r1, [sp, #0]
	subs r1, #1
	str r1, [sp, #0]
	cmp r1, #0
	blt .L_0200a9d0
	b .L_0200a890
.L_0200a9d0:
	b .L_0200a9e4
	.2byte 0x0000
.L_0200a9d4:
	.4byte 0xfffffc00
.L_0200a9d8:
	.4byte gData_020023c4 + 0x188
.L_0200a9dc:
	.4byte gData_02003304
.L_0200a9e0:
	.4byte IwramMulQ16
.L_0200a9e4:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200ae6c,"ax",%progbits
	.global Func_02002e6c
	.thumb_func
Func_02002e6c:
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
	ldr r2, .L_0200aefc
	adds r1, r6, #0
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r2, r11
	ldr r3, [r2, #16]
	lsls r0, r0, #14
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	ldr r3, .L_0200aef8
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
	ldr r3, .L_0200af00
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
	ldr r3, .L_0200af04
	mov r2, r9
	adds r6, r6, r3
	b .L_0200af08
	.2byte 0x0000
.L_0200aef8:
	.4byte 0x00000000
.L_0200aefc:
	.4byte 0xfff40000
.L_0200af00:
	.4byte Func_02002ae8
.L_0200af04:
	.4byte 0xffffc000
.L_0200af08:
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
	bl Func_020032b8
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global gData_02003918
gData_02003918:
	.space 0x00000004
	.global Data_0200391c
Data_0200391c:
	.space 0x00000004
	.global gData_02003920
gData_02003920:
	.space 0x00000004
	.global gData_02003924
gData_02003924:
	.space 0x00000004
	.global gData_02003928
gData_02003928:
	.space 0x00000004
	.global Data_0200392c
Data_0200392c:
	.space 0x00000004
	.global Data_02003930
Data_02003930:
	.space 0x00000004
	.global gData_02003934
gData_02003934:
	.space 0x00000004
	.global gData_02003938
gData_02003938:
	.space 0x00000004
	.global gData_0200393c
gData_0200393c:
	.space 0x00000040
	.global gData_0200397c
gData_0200397c:
	.space 0x00000040
	.global Data_020039bc
Data_020039bc:
	.space 0x00000020
	.global Data_020039dc
Data_020039dc:
	.space 0x00000040
	.global Data_02003a1c
Data_02003a1c:
	.space 0x00000040
	.global gData_02003a5c
gData_02003a5c:
	.space 0x00000014
	.global Data_0200ba70
Data_0200ba70:
	.space 0x0000000c
	.global Data_02003a7c
Data_02003a7c:
	.space 0x00000002
	.global gData_02003a7e
gData_02003a7e:
	.space 0x00000002
	.global gData_02003a80
gData_02003a80:
	.space 0x00000002
	.global gData_02003a82
gData_02003a82:
