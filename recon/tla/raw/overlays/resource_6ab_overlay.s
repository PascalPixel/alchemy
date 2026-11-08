.syntax unified
	.thumb
	.section .text.x020082f4,"ax",%progbits
	.global Func_020002f4
	.thumb_func
Func_020002f4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_02008548
	movs r3, #240
	mov r8, r1
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r5, [r3, r2]
	movs r3, #241
	lsls r3, r3, #1
	movs r2, #192
	lsls r2, r2, #18
	add r3, r8
	movs r1, #0
	ldrsh r7, [r3, r1]
	ldr r3, [r2, #108]
	movs r1, #214
	lsls r1, r1, #1
	mov r10, r2
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	ldr r2, .L_0200854c
	movs r3, #0
	movs r0, #137
	str r3, [r2]
	lsls r0, r0, #1
	sub sp, #8
	bl GameFlag_SetBit
	bl Func_02001d7c
	movs r3, #88
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02001df0
	ldr r3, .L_02008550
	cmp r5, r3
	beq .L_02008350
	b .L_0200853a
.L_02008350:
	movs r0, #0
	bl Func_0200443c
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083ac
	movs r5, #38
	movs r6, #24
	movs r0, #65
	movs r1, #65
	movs r2, #9
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_0200423c
	movs r0, #65
	movs r1, #65
	movs r2, #9
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004244
	movs r3, #87
	str r3, [sp, #4]
	movs r0, #76
	movs r1, #64
	movs r2, #9
	movs r3, #22
	str r5, [sp, #0]
	bl Func_02004244
	movs r3, #102
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #76
	movs r1, #64
	movs r2, #9
	movs r3, #22
	bl Func_02004244
.L_020083ac:
	cmp r7, #11
	bne .L_0200840a
	movs r3, #133
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	bl Actor_GetById
	mov r8, r0
	movs r0, #208
	mov r2, r10
	lsls r0, r0, #2
	ldr r6, [r2, #108]
	bl GameFlag_GetByte
	adds r5, r0, #0
	movs r0, #210
	lsls r0, r0, #2
	bl GameFlag_GetByte
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #20
	adds r5, r5, r3
	ldr r2, .L_02008554
	movs r3, #230
	lsls r3, r3, #1
	adds r6, r6, r3
	lsls r0, r0, #20
	adds r0, r0, r2
	ldr r2, [r6]
	mov r1, r8
	str r0, [r1, #16]
	str r5, [r1, #8]
	str r5, [r2, #8]
	ldr r3, [r1, #16]
	str r3, [r2, #16]
	bl Func_020041fc
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200840a
	bl Func_02003e04
.L_0200840a:
	adds r3, r7, #0
	subs r3, #9
	cmp r3, #1
	bhi .L_02008422
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008422
	bl Func_02003e04
.L_02008422:
	cmp r7, #1
	beq .L_0200842c
	cmp r7, #20
	beq .L_0200842c
	b .L_0200853a
.L_0200842c:
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008452
	movs r0, #11
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r0, #12
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r0, #12
	bl Actor_GetById
	ldr r3, .L_02008558
	str r3, [r0, #24]
.L_02008452:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #75
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008470
	bl Func_02000d88
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200432c
	b .L_02008494
.L_02008470:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #74
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200848a
	movs r0, #154
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008494
.L_0200848a:
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200432c
.L_02008494:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #59
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008532
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084c2
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_0200432c
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_0200432c
.L_020084c2:
	bl Func_02000d88
	movs r0, #10
	bl wait
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #58
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084f8
	movs r0, #0
	bl Func_02000ae4
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084f8
	movs r0, #13
	bl Func_02000db8
	movs r0, #14
	bl Func_02000db8
.L_020084f8:
	movs r0, #13
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r1, #200
	lsls r1, r1, #17
	cmp r3, r1
	ble .L_0200853a
	movs r3, #43
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #39
	movs r1, #22
	movs r2, #2
	movs r3, #1
	bl Func_0200423c
	movs r3, #40
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #31
	movs r1, #21
	movs r2, #2
	movs r3, #2
	bl Func_0200423c
	b .L_0200853a
.L_02008532:
	cmp r7, #20
	bne .L_0200853a
	bl Func_02000714
.L_0200853a:
	movs r0, #0
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008548:
	.4byte gMainRoutineWork
.L_0200854c:
	.4byte Data_02004cec
.L_02008550:
	.4byte 0x0000011e
.L_02008554:
	.4byte 0xfed80000
.L_02008558:
	.4byte 0xffff0000
	.section .text.x0200a148,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02002148
	.thumb_func
Func_02002148:
	push {r5, r6, r7, lr}
	mov r7, sl
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_ja_0200a0fc
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3, #0]
	sub sp, #12
	cmp r3, #0
	beq .L_ja_02009f8c
	b .L_ja_0200a0ec
.L_ja_02009f8c:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02001fcc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3, #0]
	bl Actor_GetById
	ldr r2, .L_ja_0200a100
	adds r5, r0, #0
	mov sl, r2
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	ldr r3, .L_ja_0200a104
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r1, #49
	bl Func_020041cc
.L_ja_02009fc4:
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldrb r0, [r3, #0]
	ldr r2, [r5, #16]
	bl Func_0200422c
	mov r6, sl
	mov r7, sl
	adds r6, #104
	adds r7, #106
	mov r1, sp
	cmp r0, #7
	bne .L_ja_0200a03c
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
.L_ja_02009ffa:
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
	bl Func_0200424c
	cmp r0, #0
	beq .L_ja_0200a02a
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	b .L_ja_0200a0e0
.L_ja_0200a02a:
	movs r0, #1
	bl wait
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	ble .L_ja_02009ffa
	b .L_ja_0200a0e0
.L_ja_0200a03c:
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
	bl Func_0200424c
	cmp r0, #0
	bgt .L_ja_0200a0e0
	cmp r0, #0
	bge .L_ja_0200a082
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	ldr r3, .L_ja_0200a0fc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3, #0]
	movs r1, #6
	negs r2, r2
	bl Func_0200436c
	b .L_ja_0200a0e0
.L_ja_0200a082:
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r5, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldr r7, .L_ja_0200a108
	lsls r3, r3, #17
	ldr r0, .L_ja_0200a10c
	adds r6, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r6, [r5, #16]
	cmp r2, r4
	ble .L_ja_0200a0ae
	adds r2, r4, #0
.L_ja_0200a0ae:
	ldr r0, .L_ja_0200a110
	cmp r2, r0
	bge .L_ja_0200a0b6
	adds r2, r0, #0
.L_ja_0200a0b6:
	subs r3, r1, r2
	ldr r1, .L_ja_0200a10c
	str r3, [r5, #8]
	adds r3, r6, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_ja_0200a0c8
	adds r2, r4, #0
.L_ja_0200a0c8:
	cmp r2, r0
	bge .L_ja_0200a0ce
	adds r2, r0, #0
.L_ja_0200a0ce:
	subs r3, r6, r2
	str r3, [r5, #16]
	mov r2, sl
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_ja_02009fc4
.L_ja_0200a0e0:
	movs r3, #0
	str r3, [r5, #108]
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_ja_0200a0ec:
	mov r0, sl
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov sl, r5
	pop {r5, r6, r7, pc}
.L_ja_0200a0fc:
	.4byte gMainRoutineWork
.L_ja_0200a100:
	.4byte 0x0200254c
.L_ja_0200a104:
	.4byte Func_02002120
.L_ja_0200a108:
	.4byte 0x000fffff
.L_ja_0200a10c:
	.4byte 0xfff80000
.L_ja_0200a110:
	.4byte 0xffff0000
	.endif
	.section .text.x0200a4d4,"ax",%progbits
	.global Func_020024d4
	.thumb_func
Func_020024d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200a63c
	sub sp, #4
	ldr r3, [r1, #112]
	mov r11, r0
	cmp r3, #0
	bne .L_0200a4f0
	b .L_0200a648
.L_0200a4f0:
	movs r2, #0
	str r2, [sp, #0]
.L_0200a4f4:
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
	bl Func_020041e4
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200a628
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
	ldr r1, .L_0200a640
	adds r0, r7, #0
	mov r9, r2
	bl Func_020041dc
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
	ldr r3, .L_0200a644
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
	ldr r2, .L_0200a63c
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
	ldr r1, .L_0200a63c
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
	ldr r1, .L_0200a638
	ldrh r3, [r5, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
.L_0200a628:
	ldr r1, [sp, #0]
	subs r1, #1
	str r1, [sp, #0]
	cmp r1, #0
	blt .L_0200a634
	b .L_0200a4f4
.L_0200a634:
	b .L_0200a648
	.2byte 0x0000
.L_0200a638:
	.4byte 0xfffffc00
.L_0200a63c:
	.4byte gData_020023c4 + 0x188
.L_0200a640:
	.4byte gData_02004774
.L_0200a644:
	.4byte IwramMulQ16
.L_0200a648:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global Data_02004cec
Data_02004cec:
	.space 0x00000004
	.global gData_02004cf0
gData_02004cf0:
	.space 0x00000004
	.global gData_02004cf4
gData_02004cf4:
