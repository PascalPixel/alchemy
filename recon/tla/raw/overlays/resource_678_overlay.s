.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	movs r0, #8
	movs r1, #11
	bl Func_02002940
	pop {pc}
	.section .text.x02008058,"ax",%progbits
	.global Func_02000058
	.thumb_func
Func_02000058:
	push {lr}
	ldr r3, .L_020080bc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080c0
	cmp r2, r3
	bne .L_02008070
	ldr r0, .L_020080c4
	b .L_020080b8
.L_02008070:
	ldr r3, .L_020080c8
	cmp r2, r3
	bne .L_0200807a
	ldr r0, .L_020080cc
	b .L_020080b8
.L_0200807a:
	ldr r3, .L_020080d0
	cmp r2, r3
	bne .L_02008084
	ldr r0, .L_020080d4
	b .L_020080b8
.L_02008084:
	ldr r3, .L_020080d8
	cmp r2, r3
	bne .L_0200808e
	ldr r0, .L_020080dc
	b .L_020080b8
.L_0200808e:
	ldr r3, .L_020080e0
	cmp r2, r3
	bne .L_02008098
	ldr r0, .L_020080e4
	b .L_020080b8
.L_02008098:
	ldr r3, .L_020080e8
	cmp r2, r3
	bne .L_020080a2
	ldr r0, .L_020080ec
	b .L_020080b8
.L_020080a2:
	ldr r3, .L_020080f0
	cmp r2, r3
	bne .L_020080ac
	ldr r0, .L_020080f4
	b .L_020080b8
.L_020080ac:
	ldr r3, .L_020080f8
	cmp r2, r3
	bne .L_020080b6
	ldr r0, .L_020080fc
	b .L_020080b8
.L_020080b6:
	ldr r0, .L_02008100
.L_020080b8:
	pop {pc}
	.2byte 0x0000
.L_020080bc:
	.4byte gPartyState
.L_020080c0:
	.4byte 0x00000090
.L_020080c4:
	.4byte Data_02002c6c
.L_020080c8:
	.4byte 0x00000092
.L_020080cc:
	.4byte Data_02002d2c
.L_020080d0:
	.4byte 0x00000093
.L_020080d4:
	.4byte Data_02002d5c
.L_020080d8:
	.4byte 0x00000095
.L_020080dc:
	.4byte Data_02002dbc
.L_020080e0:
	.4byte 0x00000096
.L_020080e4:
	.4byte Data_02002e04
.L_020080e8:
	.4byte 0x00000097
.L_020080ec:
	.4byte Data_02002f9c
.L_020080f0:
	.4byte 0x00000098
.L_020080f4:
	.4byte Data_02002e64
.L_020080f8:
	.4byte 0x00000099
.L_020080fc:
	.4byte Data_02002fe4
.L_02008100:
	.4byte Data_02002c54
	.section .text.x02008104,"ax",%progbits
	.global Func_02000104
	.thumb_func
Func_02000104:
	push {lr}
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #216
	adds r1, r1, r3
	adds r0, r1, #0
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008118,"ax",%progbits
	.global Func_02000118
	.thumb_func
Func_02000118:
	push {lr}
	sub sp, #8
	movs r3, #47
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #16
	movs r2, #1
	movs r3, #2
	movs r0, #50
	bl Func_02002840
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x0200813c,"ax",%progbits
	.global Func_0200013c
	.thumb_func
Func_0200013c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r1
	mov r0, r11
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #12
	beq .L_0200815e
	b .L_020082e4
.L_0200815e:
	bl Func_02002898
	adds r6, r7, #0
	movs r0, #0
	bl Func_02002968
	adds r6, #85
	movs r3, #3
	movs r5, #0
	strb r3, [r6]
	movs r0, #5
	bl WaitFrames
	strb r5, [r6]
	movs r3, #128
	lsls r3, r3, #11
	ldr r0, [r7, #80]
	str r5, [r7, #40]
	str r3, [r7, #12]
	mov r10, r0
	mov r9, r5
.L_02008188:
	mov r2, r9
	mov r3, r10
	lsls r1, r2, #2
	ldrb r2, [r3, #17]
	movs r0, #3
	adds r3, r0, #0
	ands r3, r2
	orrs r3, r1
	mov r1, r10
	ldrb r2, [r1, #26]
	strb r3, [r1, #17]
	movs r3, #8
	orrs r3, r2
	mov r8, r9
	movs r2, #254
	ands r3, r2
	mov r2, r8
	strb r3, [r1, #26]
	ands r2, r0
	movs r3, #1
	strb r3, [r1, #25]
	mov r8, r2
	cmp r2, #0
	bne .L_0200823e
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	ldr r6, [r7, #8]
	lsls r5, r5, #1
	adds r6, r6, r5
	lsls r0, r0, #1
	subs r6, r6, r0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	ldr r3, [r7, #16]
	lsls r5, r5, #1
	adds r3, r3, r5
	lsls r0, r0, #1
	subs r3, r3, r0
	ldr r2, [r7, #12]
	movs r0, #14
	adds r1, r6, #0
	bl Func_02002808
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200823e
	ldr r3, [r7, #20]
	ldr r6, [r5, #80]
	str r3, [r5, #20]
	ldr r1, .L_020082f0
	bl Func_02002800
	adds r3, r5, #0
	adds r3, #85
	mov r0, r8
	strb r0, [r3]
	cmp r6, #0
	beq .L_0200823e
	movs r1, #1
	adds r0, r6, #0
	bl Animation_ApplyChildArgument
	mov r1, r8
	strb r1, [r6, #26]
	mov r2, r10
	ldrb r3, [r2, #9]
	movs r0, #3
	lsls r3, r3, #28
	ldrb r2, [r6, #9]
	lsrs r3, r3, #30
	ands r3, r0
	subs r0, #16
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	adds r0, r6, #0
	movs r1, #13
	strb r2, [r6, #9]
	bl Animation_ApplyChildValuesToRecord
	adds r0, r6, #0
	movs r1, #8
	bl Animation_ApplyChildValue
.L_0200823e:
	movs r0, #3
	bl WaitFrames
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #24
	ble .L_02008188
	movs r3, #84
	adds r3, r3, r7
	mov r10, r3
	mov r0, r10
	movs r3, #0
	strb r3, [r0]
	str r3, [r7, #12]
	mov r9, r1
.L_0200825e:
	movs r0, #14
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	adds r0, #255
	bl Func_02002808
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020082ae
	ldr r1, .L_020082f0
	ldr r6, [r5, #80]
	bl Func_02002800
	movs r1, #0
	adds r3, r5, #0
	mov r8, r1
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	adds r2, r5, #0
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	cmp r6, #0
	beq .L_020082ae
	adds r0, r6, #0
	movs r1, #2
	bl Animation_ApplyChildArgument
	mov r3, r8
	strb r3, [r6, #26]
	movs r0, #13
	ldrb r3, [r6, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r6, #9]
.L_020082ae:
	movs r0, #15
	bl WaitFrames
	movs r1, #1
	negs r1, r1
	add r9, r1
	mov r2, r9
	cmp r2, #0
	bge .L_0200825e
	movs r0, #30
	bl Battle_WaitMode0
	movs r3, #1
	mov r0, r10
	strb r3, [r0]
	movs r1, #0
	mov r0, r11
	movs r2, #0
	bl Func_020028d0
	bl Func_020028a0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #229
	bl GameFlag_SetBit
.L_020082e4:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020082f0:
	.4byte Data_02002ae8
	.section .text.x020082f4,"ax",%progbits
	.global Func_020002f4
	.thumb_func
Func_020002f4:
	push {lr}
	movs r2, #192
	lsls r2, r2, #2
	movs r1, #65
	adds r2, #1
	bl Func_02002958
	pop {pc}
	.section .text.x02008304,"ax",%progbits
	.global Func_02000304
	.thumb_func
Func_02000304:
	push {lr}
	movs r2, #192
	movs r1, #64
	lsls r2, r2, #2
	bl Func_02002958
	pop {pc}
	.2byte 0x0000
	.section .text.x02008314,"ax",%progbits
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {lr}
	sub sp, #12
	movs r3, #14
	movs r2, #37
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #76
	movs r1, #70
	movs r2, #14
	movs r3, #8
	bl Func_02002990
	add sp, #12
	pop {pc}
	.section .text.x02008334,"ax",%progbits
	.global Func_02000334
	.thumb_func
Func_02000334:
	push {r5, r6, lr}
	ldr r3, .L_020083c0
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #14
	movs r2, #37
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #70
	movs r0, #76
	movs r2, #14
	movs r3, #8
	bl Func_02002990
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083bc
	movs r1, #132
	movs r2, #166
	movs r0, #64
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_020028d0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #16
	bne .L_020083bc
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #41
	bne .L_020083bc
	bl Func_02002898
	movs r0, #0
	bl Func_02002968
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02002910
	movs r2, #16
	ldr r0, [r6]
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_020028a0
.L_020083bc:
	add sp, #12
	pop {r5, r6, pc}
.L_020083c0:
	.4byte gPartyState
	.section .text.x020083c4,"ax",%progbits
	.global Func_020003c4
	.thumb_func
Func_020003c4:
	push {lr}
	sub sp, #12
	movs r3, #14
	movs r2, #37
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #76
	movs r1, #70
	movs r2, #14
	movs r3, #8
	bl Func_02002990
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #182
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008420
	movs r1, #172
	movs r2, #162
	movs r0, #249
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl Func_020029a8
	ldr r2, .L_02008424
	movs r3, #149
	lsls r3, r3, #2
	adds r1, r2, r3
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #182
	strh r3, [r1]
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r2, r3
	movs r3, #2
	strb r3, [r2]
	movs r0, #106
	movs r1, #1
	bl Func_02002938
.L_02008420:
	add sp, #12
	pop {pc}
.L_02008424:
	.4byte gPartyState
	.section .text.x02008428,"ax",%progbits
	.global Func_02000428
	.thumb_func
Func_02000428:
	push {r5, r6, lr}
	ldr r5, .L_02008460
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	adds r6, r0, #0
	movs r1, #3
	ldr r0, [r5]
	bl ObjectMotion_SetActionVariant
	movs r2, #128
	movs r1, #6
	lsls r2, r2, #5
	ldr r0, [r5]
	bl Func_020028e0
	movs r0, #155
	lsls r0, r0, #1
	bl Func_020029b0
	movs r0, #145
	lsls r0, r0, #1
	bl GameFlag_SetBit
	adds r0, r6, #0
	bl Func_02002930
	pop {r5, r6, pc}
.L_02008460:
	.4byte gPartyState
	.section .text.x02008464,"ax",%progbits
	.global Func_02000464
	.thumb_func
Func_02000464:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	bl Object_GetById
	cmp r5, #9
	bne .L_0200849a
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #17
	bne .L_020084c4
	ldr r3, [r0, #16]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	movs r1, #128
	str r3, [r0, #16]
	lsls r1, r1, #12
	movs r0, #2
	bl Func_02002764
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_SetBit
	b .L_020084c4
.L_0200849a:
	cmp r5, #10
	bne .L_020084c4
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #29
	bne .L_020084c4
	ldr r3, [r0, #16]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	movs r1, #128
	str r3, [r0, #16]
	lsls r1, r1, #12
	movs r0, #8
	bl Func_02002764
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_SetBit
.L_020084c4:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020084c8,"ax",%progbits
	.global Func_020004c8
	.thumb_func
Func_020004c8:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008634
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008638
	sub sp, #12
	adds r6, r0, #0
	cmp r2, r3
	bne .L_02008516
	ldr r3, .L_0200863c
	adds r2, r6, #0
	subs r2, #10
	ldrb r0, [r3, r2]
	bl Func_02002708
	adds r7, r0, #0
	cmp r6, #12
	bne .L_02008502
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008502
	b .L_0200862e
.L_02008502:
	cmp r6, #18
	bne .L_02008516
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008516
	b .L_0200862e
.L_02008516:
	ldr r3, .L_02008634
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008640
	cmp r2, r3
	bne .L_0200855a
	ldr r3, .L_02008644
	adds r2, r6, #0
	subs r2, #10
	ldrb r0, [r3, r2]
	bl Func_02002708
	adds r7, r0, #0
	cmp r6, #10
	bne .L_02008548
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200862e
.L_02008548:
	cmp r6, #12
	bne .L_0200855a
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200862e
.L_0200855a:
	ldr r3, .L_02008634
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008648
	cmp r2, r3
	bne .L_02008588
	adds r0, r6, #0
	subs r0, #10
	bl Func_02002708
	adds r7, r0, #0
	cmp r6, #10
	bne .L_02008588
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200862e
.L_02008588:
	ldr r5, .L_02008634
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200864c
	cmp r2, r3
	bne .L_020085a4
	adds r0, r6, #0
	subs r0, #10
	bl Func_02002708
	adds r7, r0, #0
.L_020085a4:
	cmp r7, #0
	beq .L_0200862e
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02002898
	movs r0, #0
	bl Func_02002968
	b .L_02008610
.L_020085c2:
	cmp r0, #0
	bge .L_020085d4
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	adds r0, r6, #0
	bl Func_02000428
	b .L_0200862e
.L_020085d4:
	ldr r3, [r5, #16]
	movs r1, #128
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r5, #16]
	ldr r2, [r5, #8]
	ldr r3, .L_02008650
	ldr r1, .L_02008654
	ands r3, r2
	adds r3, r3, r1
	movs r1, #128
	lsls r1, r1, #9
	cmp r3, r1
	ble .L_020085f4
	movs r3, #128
	lsls r3, r3, #9
.L_020085f4:
	ldr r1, .L_02008658
	cmp r3, r1
	bge .L_020085fc
	ldr r3, .L_02008658
.L_020085fc:
	subs r3, r2, r3
	str r3, [r5, #8]
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r0, #1
	bl WaitFrames
.L_02008610:
	ldr r3, [r5, #8]
	mov r1, sp
	str r3, [r1]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #12
	str r3, [r1, #4]
	adds r0, r5, #0
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r1, #8]
	bl Func_02002850
	cmp r0, #0
	ble .L_020085c2
.L_0200862e:
	add sp, #12
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008634:
	.4byte gPartyState
.L_02008638:
	.4byte 0x00000093
.L_0200863c:
	.4byte Data_020029b8
.L_02008640:
	.4byte 0x00000095
.L_02008644:
	.4byte Data_020029c0 + 0x1
.L_02008648:
	.4byte 0x00000096
.L_0200864c:
	.4byte 0x00000097
.L_02008650:
	.4byte 0x000fffff
.L_02008654:
	.4byte 0xfff80000
.L_02008658:
	.4byte 0xffff0000
	.section .text.x0200865c,"ax",%progbits
	.global Func_0200065c
	.thumb_func
Func_0200065c:
	push {r5, r6, r7, lr}
	movs r0, #8
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02002898
	movs r0, #0
	bl Func_02002968
	ldr r5, .L_0200870c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #8
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #30
	bl Func_02002908
	movs r0, #204
	movs r1, #1
	movs r2, #156
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02002928
	movs r1, #2
	movs r0, #8
	adds r1, #255
	bl Func_02002910
	movs r5, #128
	lsls r5, r5, #8
	movs r6, #2
.L_020086b4:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #40]
	movs r0, #152
	bl Func_020029b0
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	str r5, [r7, #48]
	str r3, [r7, #52]
	movs r0, #8
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r3, #128
	lsls r3, r3, #7
	subs r6, #1
	adds r5, r5, r3
	cmp r6, #0
	bge .L_020086b4
	movs r0, #155
	lsls r0, r0, #1
	bl Func_020029b0
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #40]
	movs r1, #0
	movs r2, #8
	movs r0, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_020028a0
	pop {r5, r6, r7, pc}
.L_0200870c:
	.4byte gPartyState
	.section .text.x02008710,"ax",%progbits
	.global Func_02000710
	.thumb_func
Func_02000710:
	push {r5, r6, r7, lr}
	bl Object_GetById
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #85
	movs r3, #3
	movs r6, #60
	strb r3, [r7]
	b .L_02008726
.L_02008724:
	subs r6, #1
.L_02008726:
	cmp r6, #0
	beq .L_02008736
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #40]
	cmp r3, #0
	bne .L_02008724
.L_02008736:
	movs r0, #10
	bl WaitFrames
	movs r3, #0
	strb r3, [r7]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008744,"ax",%progbits
	.global Func_02000744
	.thumb_func
Func_02000744:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	sub sp, #8
	bl Object_GetById
	cmp r5, #9
	bne .L_0200877e
	ldr r3, [r0, #8]
	asrs r5, r3, #20
	cmp r5, #16
	bne .L_0200877e
	movs r0, #9
	bl Func_02000710
	movs r3, #26
	str r3, [sp, #4]
	movs r0, #15
	movs r1, #26
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002840
	movs r0, #151
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
.L_0200877e:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008784,"ax",%progbits
	.global Func_02000784
	.thumb_func
Func_02000784:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	adds r0, r5, #0
	bl Object_SetModeById
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_SetBit
	pop {r5, pc}
	.section .text.x020087bc,"ax",%progbits
	.global Func_020007bc
	.thumb_func
Func_020007bc:
	push {r5, lr}
	sub sp, #8
	movs r3, #26
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r5, r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #28
	movs r1, #8
	bl Func_02002840
	adds r0, r5, #0
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #253
	ands r3, r2
	strb r3, [r0]
	adds r1, r5, #0
	movs r0, #0
	bl Func_02002778
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_ClearBit
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008810,"ax",%progbits
	.global Func_02000810
	.thumb_func
Func_02000810:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	movs r1, #8
	sub sp, #8
	bl Object_SetModeById
	movs r1, #3
	adds r0, r5, #0
	bl ObjectMotion_SetActionVariant
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	adds r0, r5, #0
	bl Object_GetById
	movs r6, #0
	movs r1, #0
	str r6, [r0, #108]
	movs r0, #0
	bl Func_02002778
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_ClearBit
	ldr r3, .L_020088d4
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	ldr r3, [r6, #8]
	ldr r2, [r0, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	cmp r2, r3
	bne .L_020088bc
	ldr r2, [r0, #16]
	ldr r3, [r6, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020088bc
	movs r1, #129
	ldr r0, [r7]
	lsls r1, r1, #1
	bl Func_02002910
	movs r1, #0
	movs r2, #16
	ldr r0, [r7]
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #2
	bl Battle_WaitMode0
	movs r1, #192
	ldr r0, [r7]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
.L_020088bc:
	movs r3, #26
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #22
	movs r1, #8
	movs r2, #1
	movs r3, #1
	bl Func_02002840
	add sp, #8
	pop {r5, r6, r7, pc}
.L_020088d4:
	.4byte gPartyState
	.section .text.x020088d8,"ax",%progbits
	.global Func_020008d8
	.thumb_func
Func_020008d8:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r1, #7
	bl Object_SetModeById
	movs r1, #3
	adds r0, r5, #0
	bl ObjectMotion_SetActionVariant
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	adds r0, r5, #0
	bl Object_GetById
	movs r6, #0
	str r6, [r0, #108]
	movs r1, #0
	movs r0, #0
	bl Func_02002778
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_ClearBit
	pop {r5, r6, pc}
	.section .text.x02008920,"ax",%progbits
	.global Func_02000920
	.thumb_func
Func_02000920:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r6, r0, #0
	movs r1, #3
	adds r0, r5, #0
	bl ObjectMotion_SetActionVariant
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	adds r0, r5, #0
	bl Object_SetModeById
	cmp r6, #5
	beq .L_02008956
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_GetByte
	cmp r0, #3
	ble .L_02008968
.L_02008956:
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_SetBit
.L_02008968:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200896c,"ax",%progbits
	.global Func_0200096c
	.thumb_func
Func_0200096c:
	push {r5, lr}
	sub sp, #8
	movs r3, #25
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r5, r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #23
	movs r1, #27
	bl Func_02002840
	adds r0, r5, #0
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #253
	ands r3, r2
	strb r3, [r0]
	adds r1, r5, #0
	movs r0, #0
	bl Func_02002778
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_ClearBit
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020089c0,"ax",%progbits
	.global Func_020009c0
	.thumb_func
Func_020009c0:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	movs r1, #8
	sub sp, #8
	bl Object_SetModeById
	movs r1, #3
	adds r0, r6, #0
	bl ObjectMotion_SetActionVariant
	adds r0, r6, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	adds r0, r6, #0
	bl Object_GetById
	movs r5, #0
	movs r1, #0
	str r5, [r0, #108]
	movs r0, #0
	bl Func_02002778
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_ClearBit
	movs r0, #140
	movs r1, #0
	lsls r0, r0, #2
	bl GameFlag_SetByte
	ldr r3, .L_02008a90
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	bl Object_GetById
	adds r5, r0, #0
	adds r0, r6, #0
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r2, [r0, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	cmp r2, r3
	bne .L_02008a76
	ldr r2, [r0, #16]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008a76
	movs r1, #129
	ldr r0, [r7]
	lsls r1, r1, #1
	bl Func_02002910
	movs r1, #0
	movs r2, #16
	ldr r0, [r7]
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #2
	bl Battle_WaitMode0
	movs r1, #192
	ldr r0, [r7]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
.L_02008a76:
	movs r3, #25
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #1
	bl Func_02002840
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a90:
	.4byte gPartyState
	.section .text.x02008a94,"ax",%progbits
	.global Func_02000a94
	.thumb_func
Func_02000a94:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r1, #7
	bl Object_SetModeById
	movs r1, #3
	adds r0, r5, #0
	bl ObjectMotion_SetActionVariant
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	adds r0, r5, #0
	bl Object_GetById
	movs r6, #0
	str r6, [r0, #108]
	movs r1, #0
	movs r0, #0
	bl Func_02002778
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #140
	lsls r0, r0, #2
	movs r1, #0
	bl GameFlag_SetByte
	pop {r5, r6, pc}
	.section .text.x02008adc,"ax",%progbits
	.global Func_02000adc
	.thumb_func
Func_02000adc:
	push {lr}
	adds r0, r1, #0
	movs r1, #2
	bl Object_SetModeById
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #49
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #50
	bl GameFlag_ClearBit
	pop {pc}
	.section .text.x02008afc,"ax",%progbits
	.global Func_02000afc
	.thumb_func
Func_02000afc:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r0, r6, #0
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r7, r3, #20
	cmp r7, #7
	bne .L_02008b54
	adds r0, r6, #0
	movs r1, #3
	bl Object_SetModeById
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	movs r0, #192
	orrs r3, r2
	lsls r0, r0, #2
	strb r3, [r1]
	adds r0, #50
	bl GameFlag_SetBit
	movs r0, #141
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	movs r3, #15
	str r3, [sp, #4]
	movs r0, #7
	movs r1, #17
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	bl Func_02002840
.L_02008b54:
	add sp, #8
	pop {r5, r6, r7, pc}
	.section .text.x02008b58,"ax",%progbits
	.global Func_02000b58
	.thumb_func
Func_02000b58:
	push {lr}
	adds r0, r1, #0
	movs r1, #2
	sub sp, #8
	bl Object_SetModeById
	movs r0, #205
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #53
	bl GameFlag_ClearBit
	movs r3, #27
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #27
	movs r1, #14
	movs r2, #1
	movs r3, #1
	bl Func_02002840
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008b90,"ax",%progbits
	.global Func_02000b90
	.thumb_func
Func_02000b90:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r0, r6, #0
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	asrs r7, r3, #20
	cmp r7, #16
	bne .L_02008c10
	adds r0, r6, #0
	movs r1, #3
	bl Object_SetModeById
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	movs r0, #192
	orrs r3, r2
	lsls r0, r0, #2
	strb r3, [r1]
	adds r0, #53
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #54
	bl GameFlag_SetBit
	movs r1, #128
	movs r2, #128
	adds r0, r6, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #32
	movs r2, #0
	negs r1, r1
	adds r0, r6, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	adds r0, r6, #0
	bl ObjectMotion_CommitCurrentPositionAndActivate
	adds r0, r6, #0
	movs r1, #3
	bl Object_SetModeById
	movs r3, #25
	str r3, [sp, #0]
	movs r0, #25
	movs r1, #18
	movs r2, #1
	movs r3, #1
	str r7, [sp, #4]
	bl Func_02002840
.L_02008c10:
	add sp, #8
	pop {r5, r6, r7, pc}
	.section .text.x02008c14,"ax",%progbits
	.global Func_02000c14
	.thumb_func
Func_02000c14:
	ldr r2, [r0, #80]
	ldr r1, .L_02008c20
	ldrh r3, [r2, #18]
	adds r3, r3, r1
	strh r3, [r2, #18]
	bx lr
.L_02008c20:
	.4byte 0xfffff800
	.section .text.x02008c24,"ax",%progbits
	.global Func_02000c24
	.thumb_func
Func_02000c24:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r2, .L_02008c94
	ldr r3, [r2]
	mov r8, r2
	subs r3, #1
	str r3, [r2]
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	lsls r6, r6, #5
	lsls r5, r5, #4
	movs r3, #150
	lsls r3, r3, #17
	adds r5, r5, r6
	adds r5, r5, r3
	bl Random16Far
	adds r3, r0, #0
	lsls r3, r3, #4
	movs r2, #130
	lsls r2, r2, #17
	adds r3, r3, r6
	movs r0, #30
	adds r3, r3, r2
	adds r1, r5, #0
	movs r2, #0
	adds r0, #255
	bl Func_02002808
	ldr r1, .L_02008c98
	adds r5, r0, #0
	bl Func_02002800
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	mov r2, r8
	ldr r3, [r2]
	cmp r3, #0
	bne .L_02008c8e
	ldr r0, .L_02008c9c
	bl Scheduler_RemoveCallbackFar
.L_02008c8e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02008c94:
	.4byte Data_02003458
.L_02008c98:
	.4byte Data_020029c8
.L_02008c9c:
	.4byte Func_02000c24
	.section .text.x02008ca0,"ax",%progbits
	.global Func_02000ca0
	.thumb_func
Func_02000ca0:
	push {r5, lr}
	ldr r5, .L_02008ce4
	sub sp, #8
	ldr r2, [r5]
	subs r1, r2, #1
	str r1, [r5]
	adds r3, r1, #0
	cmp r1, #0
	bge .L_02008cb4
	adds r3, r2, #6
.L_02008cb4:
	movs r2, #1
	ands r1, r2
	lsls r2, r1, #1
	movs r0, #47
	asrs r3, r3, #3
	subs r0, r0, r2
	movs r2, #15
	subs r2, r2, r3
	movs r1, #64
	subs r1, r1, r3
	movs r4, #29
	str r2, [sp, #4]
	movs r2, #2
	str r4, [sp, #0]
	bl Func_02002838
	ldr r3, [r5]
	cmp r3, #0
	bne .L_02008ce0
	ldr r0, .L_02008ce8
	bl Scheduler_RemoveCallbackFar
.L_02008ce0:
	add sp, #8
	pop {r5, pc}
.L_02008ce4:
	.4byte Data_0200345c
.L_02008ce8:
	.4byte Func_02000ca0
	.section .text.x02008cec,"ax",%progbits
	.global Func_02000cec
	.thumb_func
Func_02000cec:
	push {r5, lr}
	ldr r5, .L_02008d24
	movs r3, #1
	ldr r2, [r5]
	sub sp, #8
	subs r2, #1
	ands r3, r2
	lsls r1, r3, #2
	str r2, [r5]
	adds r1, r1, r3
	movs r2, #15
	movs r3, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #5
	adds r1, #59
	movs r0, #50
	movs r2, #13
	bl Func_02002838
	ldr r3, [r5]
	cmp r3, #0
	bne .L_02008d20
	ldr r0, .L_02008d28
	bl Scheduler_RemoveCallbackFar
.L_02008d20:
	add sp, #8
	pop {r5, pc}
.L_02008d24:
	.4byte Data_02003460
.L_02008d28:
	.4byte Func_02000cec
	.section .text.x02008d2c,"ax",%progbits
	.global Func_02000d2c
	.thumb_func
Func_02000d2c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #19
	sub sp, #8
	bl Object_GetById
	mov r9, r0
	bl Func_02002898
	movs r0, #0
	bl Func_02002968
	movs r0, #9
	mov r11, r0
.L_02008d52:
	movs r0, #1
	bl WaitFrames
	mov r1, r9
	ldr r2, [r1, #80]
	ldr r0, .L_02008f80
	ldrh r3, [r2, #18]
	adds r3, r3, r0
	strh r3, [r2, #18]
	ldr r3, [r1, #80]
	ldrh r0, [r3, #18]
	bl Math_Cosine
	mov r1, r9
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [r1, #8]
	asrs r0, r0, #1
	subs r3, r3, r0
	movs r2, #1
	str r3, [r1, #8]
	negs r2, r2
	movs r3, #128
	lsls r3, r3, #24
	add r11, r2
	str r3, [r1, #56]
	mov r3, r11
	cmp r3, #0
	bge .L_02008d52
	ldr r3, .L_02008f84
	movs r2, #128
	str r3, [r1, #108]
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #246
	lsls r1, r1, #1
	movs r0, #19
	movs r2, #246
	bl ObjectMotion_ResetAndSetPosition
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	mov r0, r9
	mov r2, r9
	str r3, [r0, #72]
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	mov r3, r9
	movs r5, #0
	adds r3, #34
	strb r5, [r3]
	movs r0, #19
	bl ObjectMotion_CommitCurrentPositionAndActivate
	mov r1, r9
	ldr r3, [r1, #20]
	str r5, [r1, #40]
	str r3, [r1, #12]
	movs r2, #248
	movs r1, #240
	lsls r1, r1, #17
	movs r0, #19
	lsls r2, r2, #16
	bl Func_020028d0
	mov r2, r9
	ldr r3, [r2, #80]
	movs r0, #188
	strh r5, [r3, #18]
	bl Func_020029b0
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl Func_02002868
	movs r0, #141
	bl Func_020029b0
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02002868
	movs r1, #240
	movs r2, #248
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_020028d0
	movs r1, #220
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #232
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #10
	bl Object_GetById
	movs r3, #160
	lsls r3, r3, #12
	str r3, [r0, #40]
	mov r3, r9
	str r5, [r3, #108]
	movs r0, #15
	mov r11, r0
.L_02008e42:
	mov r2, r9
	ldr r1, [r2, #8]
	ldr r2, [r2, #12]
	movs r3, #128
	lsls r3, r3, #13
	mov r0, r9
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #255
	bl Func_02002808
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02008ed0
	bl Random16Far
	mov r8, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	movs r1, #128
	adds r5, r0, #0
	lsls r1, r1, #5
	lsrs r5, r5, #2
	adds r5, r5, r1
	adds r0, r7, #0
	ldr r1, .L_02008f88
	bl Func_02002800
	movs r1, #0
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r2, r8
	movs r0, #128
	lsls r3, r2, #2
	lsls r0, r0, #10
	adds r3, r3, r0
	str r3, [r7, #40]
	mov r0, r8
	bl Math_Cosine
	ldr r2, .L_02008f8c
	lsls r6, r6, #3
	adds r1, r0, #0
	mov r10, r2
	adds r0, r6, #0
	mov lr, r10
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r8
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r10
	.2byte 0xf800
	movs r3, #0
	str r3, [r7, #52]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #8
	str r0, [r7, #36]
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r3, [r7, #68]
.L_02008ed0:
	movs r3, #1
	negs r3, r3
	add r11, r3
	mov r0, r11
	cmp r0, #0
	bge .L_02008e42
	bl Func_02002870
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008f90
	bl Scheduler_AddOrUpdateCallback
	movs r0, #40
	bl WaitFrames
	movs r0, #230
	movs r1, #224
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #102
	adds r1, #204
	bl Func_02002918
	movs r0, #168
	movs r1, #1
	movs r2, #140
	negs r1, r1
	lsls r0, r0, #17
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	movs r1, #11
	mov r11, r1
	movs r5, #114
.L_02008f18:
	movs r3, #18
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r1, r5, #0
	movs r2, #13
	movs r3, #5
	movs r0, #50
	bl Func_02002838
	movs r0, #8
	bl WaitFrames
	movs r2, #1
	negs r2, r2
	add r11, r2
	mov r3, r11
	subs r5, #5
	cmp r3, #0
	bgt .L_02008f18
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008f94
	bl Scheduler_AddOrUpdateCallback
	movs r0, #60
	bl WaitFrames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008f98
	bl Scheduler_AddOrUpdateCallback
	movs r0, #24
	bl WaitFrames
	movs r0, #120
	bl Battle_WaitMode0
	bl Func_020028a0
	movs r0, #204
	lsls r0, r0, #2
	bl GameFlag_SetBit
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008f80:
	.4byte 0xffffff00
.L_02008f84:
	.4byte Func_02000c14
.L_02008f88:
	.4byte Data_02002a04
.L_02008f8c:
	.4byte IwramMulQ16
.L_02008f90:
	.4byte Func_02000ca0
.L_02008f94:
	.4byte Func_02000c24
.L_02008f98:
	.4byte Func_02000cec
	.section .text.x02008f9c,"ax",%progbits
	.global Func_02000f9c
	.thumb_func
Func_02000f9c:
	push {lr}
	adds r0, r1, #0
	movs r1, #2
	bl Object_SetModeById
	movs r0, #206
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #57
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008fbc,"ax",%progbits
	.global Func_02000fbc
	.thumb_func
Func_02000fbc:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r0, r6, #0
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	asrs r7, r3, #20
	cmp r7, #21
	bne .L_02009014
	adds r0, r6, #0
	movs r1, #3
	bl Object_SetModeById
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	movs r0, #192
	orrs r3, r2
	lsls r0, r0, #2
	strb r3, [r1]
	adds r0, #57
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #58
	bl GameFlag_SetBit
	movs r3, #11
	str r3, [sp, #0]
	movs r0, #11
	movs r1, #19
	movs r2, #1
	movs r3, #1
	str r7, [sp, #4]
	bl Func_02002840
.L_02009014:
	add sp, #8
	pop {r5, r6, r7, pc}
	.section .text.x02009018,"ax",%progbits
	.global Func_02001018
	.thumb_func
Func_02001018:
	push {lr}
	adds r0, r1, #0
	movs r1, #2
	bl Object_SetModeById
	movs r0, #143
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #207
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02009038,"ax",%progbits
	.global Func_02001038
	.thumb_func
Func_02001038:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r0, r6, #0
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r7, r3, #20
	cmp r7, #31
	bne .L_020090b6
	adds r0, r6, #0
	movs r1, #3
	bl Object_SetModeById
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	movs r0, #207
	strb r3, [r1]
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #61
	bl GameFlag_SetBit
	movs r1, #128
	movs r2, #128
	adds r0, r6, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r2, #32
	negs r2, r2
	movs r1, #0
	adds r0, r6, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	adds r0, r6, #0
	bl ObjectMotion_CommitCurrentPositionAndActivate
	adds r0, r6, #0
	movs r1, #3
	bl Object_SetModeById
	movs r3, #11
	str r3, [sp, #4]
	movs r0, #30
	movs r1, #13
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	bl Func_02002840
.L_020090b6:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020090bc,"ax",%progbits
	.global Func_020010bc
	.thumb_func
Func_020010bc:
	push {lr}
	bl Func_02002948
	pop {pc}
	.section .text.x020090c4,"ax",%progbits
	.global Func_020010c4
	.thumb_func
Func_020010c4:
	push {lr}
	adds r0, r1, #0
	movs r1, #2
	sub sp, #8
	bl Object_SetModeById
	movs r0, #208
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #65
	bl GameFlag_ClearBit
	movs r3, #7
	movs r2, #38
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #39
	movs r2, #1
	movs r3, #1
	movs r0, #7
	bl Func_02002840
	movs r0, #1
	bl Func_02002084
	add sp, #8
	pop {pc}
	.section .text.x02009100,"ax",%progbits
	.global Func_02001100
	.thumb_func
Func_02001100:
	push {lr}
	adds r0, r1, #0
	movs r1, #2
	bl Object_SetModeById
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #66
	bl GameFlag_SetBit
	movs r0, #145
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #1
	bl Func_02002084
	pop {pc}
	.2byte 0x0000
	.section .text.x02009128,"ax",%progbits
	.global Func_02001128
	.thumb_func
Func_02001128:
	push {lr}
	movs r2, #210
	movs r1, #64
	lsls r2, r2, #2
	bl Func_02002958
	pop {pc}
	.2byte 0x0000
	.section .text.x02009138,"ax",%progbits
	.global Func_02001138
	.thumb_func
Func_02001138:
	push {lr}
	ldr r0, .L_02009144
	bl Func_020029a0
	pop {pc}
	.2byte 0x0000
.L_02009144:
	.4byte 0x0000224a
	.section .text.x02009148,"ax",%progbits
	.global Func_02001148
	.thumb_func
Func_02001148:
	push {lr}
	ldr r3, .L_020091c8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020091cc
	cmp r2, r3
	bne .L_02009160
	ldr r0, .L_020091d0
	b .L_020091c6
.L_02009160:
	ldr r3, .L_020091d4
	cmp r2, r3
	bne .L_0200916a
	ldr r0, .L_020091d8
	b .L_020091c6
.L_0200916a:
	ldr r3, .L_020091dc
	cmp r2, r3
	bne .L_02009174
	ldr r0, .L_020091e0
	b .L_020091c6
.L_02009174:
	ldr r3, .L_020091e4
	cmp r2, r3
	bne .L_0200917e
	ldr r0, .L_020091e8
	b .L_020091c6
.L_0200917e:
	ldr r3, .L_020091ec
	cmp r2, r3
	bne .L_02009188
	ldr r0, .L_020091f0
	b .L_020091c6
.L_02009188:
	ldr r3, .L_020091f4
	cmp r2, r3
	bne .L_02009192
	ldr r0, .L_020091f8
	b .L_020091c6
.L_02009192:
	ldr r3, .L_020091fc
	cmp r2, r3
	bne .L_0200919c
	ldr r0, .L_02009200
	b .L_020091c6
.L_0200919c:
	ldr r3, .L_02009204
	cmp r2, r3
	bne .L_020091a6
	ldr r0, .L_02009208
	b .L_020091c6
.L_020091a6:
	ldr r3, .L_0200920c
	cmp r2, r3
	bne .L_020091b0
	ldr r0, .L_02009210
	b .L_020091c6
.L_020091b0:
	ldr r3, .L_02009214
	cmp r2, r3
	bne .L_020091ba
	ldr r0, .L_02009218
	b .L_020091c6
.L_020091ba:
	ldr r3, .L_0200921c
	cmp r2, r3
	bne .L_020091c4
	ldr r0, .L_02009220
	b .L_020091c6
.L_020091c4:
	ldr r0, .L_02009224
.L_020091c6:
	pop {pc}
.L_020091c8:
	.4byte gPartyState
.L_020091cc:
	.4byte 0x0000008f
.L_020091d0:
	.4byte Data_02003038
.L_020091d4:
	.4byte 0x00000090
.L_020091d8:
	.4byte Data_02003068
.L_020091dc:
	.4byte 0x00000091
.L_020091e0:
	.4byte Data_02003128
.L_020091e4:
	.4byte 0x00000092
.L_020091e8:
	.4byte Data_02003194
.L_020091ec:
	.4byte 0x00000093
.L_020091f0:
	.4byte Data_020031e8
.L_020091f4:
	.4byte 0x00000094
.L_020091f8:
	.4byte Data_02003314
.L_020091fc:
	.4byte 0x00000095
.L_02009200:
	.4byte Data_02003338
.L_02009204:
	.4byte 0x00000096
.L_02009208:
	.4byte Data_020033e0
.L_0200920c:
	.4byte 0x00000097
.L_02009210:
	.4byte Data_020034e8
.L_02009214:
	.4byte 0x00000098
.L_02009218:
	.4byte Data_02003464
.L_0200921c:
	.4byte 0x00000099
.L_02009220:
	.4byte Data_02003560
.L_02009224:
	.4byte Data_0200302c
	.section .text.x02009228,"ax",%progbits
	.global Func_02001228
	.thumb_func
Func_02001228:
	push {r5, r6, lr}
	adds r6, r0, #0
	ldr r4, [r6, #12]
	ldr r3, [r6, #40]
	ldr r2, [r6, #20]
	adds r3, r4, r3
	cmp r3, r2
	bge .L_0200927e
	movs r0, #14
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	adds r0, #255
	adds r2, r4, #0
	bl Func_02002808
	movs r3, #140
	lsls r3, r3, #8
	adds r5, r0, #0
	adds r3, #204
	cmp r5, #0
	beq .L_0200926c
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldr r1, .L_02009284
	bl Func_02002800
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Func_020027f8
.L_0200926c:
	movs r0, #106
	bl Func_020029b0
	ldr r3, [r6, #20]
	movs r0, #0
	str r3, [r6, #12]
	movs r3, #0
	str r3, [r6, #40]
	b .L_02009280
.L_0200927e:
	movs r0, #1
.L_02009280:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009284:
	.4byte Data_02002a48
	.section .text.x02009288,"ax",%progbits
	.global Func_02001288
	.thumb_func
Func_02001288:
	push {r5, lr}
	ldr r3, .L_020092fc
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_020092f8
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bcs .L_020092f8
	bl Random16Far
	movs r3, #130
	adds r5, r0, #0
	lsls r3, r3, #18
	lsls r5, r5, #5
	adds r5, r5, r3
	bl Random16Far
	movs r3, #176
	lsls r0, r0, #3
	lsls r3, r3, #16
	subs r3, r3, r0
	movs r2, #160
	movs r0, #46
	adds r1, r5, #0
	lsls r2, r2, #16
	adds r0, #255
	bl Func_02002808
	movs r3, #168
	lsls r3, r3, #7
	adds r5, r0, #0
	adds r3, #122
	cmp r5, #0
	beq .L_020092f8
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #230
	lsls r3, r3, #7
	adds r3, #51
	str r3, [r5, #72]
	ldr r1, .L_02009300
	bl Func_02002800
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #3
	bl Func_02002880
.L_020092f8:
	pop {r5, pc}
	.2byte 0x0000
.L_020092fc:
	.4byte Data_0300122c
.L_02009300:
	.4byte Data_02002a54
	.section .text.x02009304,"ax",%progbits
	.global Func_02001304
	.thumb_func
Func_02001304:
	push {r5, lr}
	bl Func_02002730
	adds r5, r0, #0
	cmp r5, #180
	beq .L_02009344
	cmp r5, #180
	bgt .L_0200931a
	cmp r5, #0
	beq .L_02009328
	b .L_020093de
.L_0200931a:
	cmp r5, #240
	beq .L_0200936e
	movs r1, #210
	lsls r1, r1, #1
	cmp r5, r1
	beq .L_020093a6
	b .L_020093de
.L_02009328:
	movs r1, #1
	movs r0, #0
	bl Func_0200271c
	movs r1, #1
	movs r0, #1
	bl Func_0200271c
	movs r1, #1
	movs r0, #4
	bl Func_0200271c
	movs r0, #5
	b .L_02009398
.L_02009344:
	movs r1, #0
	movs r0, #0
	bl Func_0200271c
	movs r1, #0
	movs r0, #1
	bl Func_0200271c
	movs r1, #0
	movs r0, #4
	bl Func_0200271c
	movs r0, #5
	movs r1, #0
	bl Func_0200271c
	movs r0, #1
	negs r0, r0
	bl Func_02002978
	b .L_020093de
.L_0200936e:
	movs r1, #1
	movs r0, #2
	bl Func_0200271c
	movs r1, #1
	movs r0, #3
	bl Func_0200271c
	movs r1, #1
	movs r0, #6
	bl Func_0200271c
	movs r1, #1
	movs r0, #7
	bl Func_0200271c
	movs r1, #1
	movs r0, #8
	bl Func_0200271c
	movs r0, #9
.L_02009398:
	movs r1, #1
	bl Func_0200271c
	movs r0, #170
	bl Func_02002978
	b .L_020093de
.L_020093a6:
	movs r1, #0
	movs r0, #2
	bl Func_0200271c
	movs r1, #0
	movs r0, #3
	bl Func_0200271c
	movs r1, #0
	movs r0, #6
	bl Func_0200271c
	movs r1, #0
	movs r0, #7
	bl Func_0200271c
	movs r1, #0
	movs r0, #8
	bl Func_0200271c
	movs r0, #9
	movs r1, #0
	bl Func_0200271c
	movs r0, #1
	negs r0, r0
	bl Func_02002978
.L_020093de:
	adds r3, r5, #0
	subs r3, #251
	cmp r3, #178
	bhi .L_0200941a
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200941a
	movs r0, #59
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200941a
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200941a
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #100
	strh r3, [r2]
.L_0200941a:
	movs r0, #240
	lsls r0, r0, #1
	bl Func_0200273c
	pop {r5, pc}
	.section .text.x02009424,"ax",%progbits
	.global Func_02001424
	.thumb_func
Func_02001424:
	push {r5, lr}
	bl Func_02002730
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009436
	cmp r5, #120
	beq .L_0200944e
	b .L_02009466
.L_02009436:
	movs r1, #1
	movs r0, #0
	bl Func_0200271c
	movs r0, #1
	movs r1, #1
	bl Func_0200271c
	movs r0, #170
	bl Func_02002978
	b .L_02009466
.L_0200944e:
	movs r1, #0
	movs r0, #0
	bl Func_0200271c
	movs r0, #1
	movs r1, #0
	bl Func_0200271c
	movs r0, #1
	negs r0, r0
	bl Func_02002978
.L_02009466:
	adds r3, r5, #0
	subs r3, #11
	cmp r3, #118
	bhi .L_02009496
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009484
	movs r0, #8
	bl Func_02000810
	b .L_02009496
.L_02009484:
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009496
	movs r0, #8
	bl Func_020008d8
.L_02009496:
	movs r0, #240
	lsls r0, r0, #1
	bl Func_0200273c
	pop {r5, pc}
	.section .text.x020094a0,"ax",%progbits
	.global Func_020014a0
	.thumb_func
Func_020014a0:
	push {r5, lr}
	bl Func_02002730
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020094b2
	cmp r5, #120
	beq .L_020094ca
	b .L_020094e2
.L_020094b2:
	movs r1, #1
	movs r0, #0
	bl Func_0200271c
	movs r0, #1
	movs r1, #1
	bl Func_0200271c
	movs r0, #170
	bl Func_02002978
	b .L_020094e2
.L_020094ca:
	movs r1, #0
	movs r0, #0
	bl Func_0200271c
	movs r0, #1
	movs r1, #0
	bl Func_0200271c
	movs r0, #1
	negs r0, r0
	bl Func_02002978
.L_020094e2:
	adds r3, r5, #0
	subs r3, #11
	cmp r3, #118
	bhi .L_02009512
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009500
	movs r0, #10
	bl Func_020009c0
	b .L_02009512
.L_02009500:
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009512
	movs r0, #10
	bl Func_02000a94
.L_02009512:
	movs r0, #240
	lsls r0, r0, #1
	bl Func_0200273c
	pop {r5, pc}
	.section .text.x0200951c,"ax",%progbits
	.global Func_0200151c
	.thumb_func
Func_0200151c:
	push {lr}
	bl Func_02002730
	cmp r0, #0
	beq .L_0200952c
	cmp r0, #180
	beq .L_02009544
	b .L_0200955c
.L_0200952c:
	movs r1, #1
	movs r0, #0
	bl Func_0200271c
	movs r0, #1
	movs r1, #1
	bl Func_0200271c
	movs r0, #170
	bl Func_02002978
	b .L_0200955c
.L_02009544:
	movs r1, #0
	movs r0, #0
	bl Func_0200271c
	movs r0, #1
	movs r1, #0
	bl Func_0200271c
	movs r0, #1
	negs r0, r0
	bl Func_02002978
.L_0200955c:
	movs r0, #240
	bl Func_0200273c
	pop {pc}
	.section .text.x02009564,"ax",%progbits
	.global Func_02001564
	.thumb_func
Func_02001564:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r2, [r6, #40]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	ldr r2, [r6, #20]
	cmp r3, r2
	bge .L_02009610
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200958e
	movs r0, #106
	bl Func_020029b0
.L_0200958a:
	movs r0, #0
	b .L_02009612
.L_0200958e:
	movs r0, #14
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	adds r0, #255
	bl Func_02002808
	movs r3, #140
	lsls r3, r3, #8
	adds r5, r0, #0
	adds r3, #204
	cmp r5, #0
	beq .L_020095c2
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldr r1, .L_02009614
	bl Func_02002800
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Func_020027f8
.L_020095c2:
	movs r0, #106
	bl Func_020029b0
	ldr r3, [r6, #20]
	movs r0, #130
	str r3, [r6, #12]
	str r7, [r6, #40]
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200958a
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_GetByte
	adds r5, r0, #0
	cmp r5, #3
	bgt .L_020095f4
	adds r5, #1
	movs r0, #140
	lsls r0, r0, #2
	adds r1, r5, #0
	bl GameFlag_SetByte
.L_020095f4:
	cmp r5, #0
	beq .L_02009600
	movs r0, #0
	movs r1, #10
	bl Func_02000920
.L_02009600:
	movs r0, #10
	bl Object_GetById
	lsls r3, r5, #14
	adds r6, r0, #0
	str r3, [r6, #24]
	str r3, [r6, #28]
	b .L_0200958a
.L_02009610:
	movs r0, #1
.L_02009612:
	pop {r5, r6, r7, pc}
.L_02009614:
	.4byte Data_02002a48
	.section .text.x02009618,"ax",%progbits
	.global Func_02001618
	.thumb_func
Func_02001618:
	push {r5, lr}
	ldr r3, .L_02009664
	movs r2, #63
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009660
	movs r1, #204
	movs r3, #220
	movs r2, #160
	movs r0, #46
	lsls r3, r3, #17
	lsls r1, r1, #17
	lsls r2, r2, #16
	adds r0, #255
	bl Func_02002808
	movs r3, #168
	lsls r3, r3, #7
	adds r5, r0, #0
	adds r3, #122
	cmp r5, #0
	beq .L_02009660
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #230
	lsls r3, r3, #7
	adds r3, #51
	ldr r1, .L_02009668
	str r3, [r5, #72]
	bl Func_02002800
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_02009660:
	pop {r5, pc}
	.2byte 0x0000
.L_02009664:
	.4byte Data_0300122c
.L_02009668:
	.4byte Data_02002a60
	.section .text.x0200966c,"ax",%progbits
	.global Func_0200166c
	.thumb_func
Func_0200166c:
	push {lr}
	ldr r2, [r0, #40]
	ldr r3, [r0, #12]
	movs r0, #0
	adds r3, r3, r2
	ldr r2, .L_02009680
	cmp r3, r2
	blt .L_0200967e
	movs r0, #1
.L_0200967e:
	pop {pc}
.L_02009680:
	.4byte 0xffd00000
	.section .text.x02009684,"ax",%progbits
	.global Func_02001684
	.thumb_func
Func_02001684:
	push {r5, lr}
	ldr r3, .L_020096f0
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_020096ee
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bcs .L_020096ee
	bl Random16Far
	movs r3, #192
	lsls r0, r0, #5
	lsls r3, r3, #17
	adds r1, r0, r3
	movs r3, #200
	lsls r3, r3, #17
	subs r3, r3, r0
	movs r2, #160
	movs r0, #46
	lsls r2, r2, #16
	adds r0, #255
	bl Func_02002808
	movs r3, #168
	lsls r3, r3, #7
	adds r5, r0, #0
	adds r3, #122
	cmp r5, #0
	beq .L_020096ee
	adds r2, r5, #0
	str r3, [r5, #24]
	str r3, [r5, #28]
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	ldr r3, .L_020096f4
	ldr r1, .L_020096f8
	str r3, [r5, #20]
	movs r3, #230
	lsls r3, r3, #7
	adds r3, #51
	str r3, [r5, #72]
	bl Func_02002800
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_020096ee:
	pop {r5, pc}
.L_020096f0:
	.4byte Data_0300122c
.L_020096f4:
	.4byte 0xffc00000
.L_020096f8:
	.4byte Data_02002a6c
	.section .text.x020096fc,"ax",%progbits
	.global Func_020016fc
	.thumb_func
Func_020016fc:
	push {r5, lr}
	ldr r3, .L_02009768
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009766
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bcs .L_02009766
	bl Random16Far
	movs r3, #208
	lsls r0, r0, #5
	lsls r3, r3, #17
	adds r1, r0, r3
	movs r3, #176
	lsls r3, r3, #17
	subs r3, r3, r0
	movs r2, #160
	movs r0, #46
	lsls r2, r2, #16
	adds r0, #255
	bl Func_02002808
	movs r3, #168
	lsls r3, r3, #7
	adds r5, r0, #0
	adds r3, #122
	cmp r5, #0
	beq .L_02009766
	adds r2, r5, #0
	str r3, [r5, #24]
	str r3, [r5, #28]
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	ldr r3, .L_0200976c
	ldr r1, .L_02009770
	str r3, [r5, #20]
	movs r3, #230
	lsls r3, r3, #7
	adds r3, #51
	str r3, [r5, #72]
	bl Func_02002800
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_02009766:
	pop {r5, pc}
.L_02009768:
	.4byte Data_0300122c
.L_0200976c:
	.4byte 0xffc00000
.L_02009770:
	.4byte Data_02002a6c
	.section .text.x02009774,"ax",%progbits
	.global Func_02001774
	.thumb_func
Func_02001774:
	push {r5, r6, r7, lr}
	ldr r5, .L_02009ac4
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #32]
	ldr r3, [r3, #108]
	subs r0, #52
	adds r1, r3, r0
	ldr r3, .L_02009ac8
	movs r7, #0
	sub sp, #8
	str r7, [r1]
	cmp r2, r3
	bne .L_02009850
	ldr r2, .L_02009acc
	movs r1, #152
	lsls r1, r1, #2
	adds r3, r5, r1
	strh r2, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #98
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
	bl Func_02002120
	movs r5, #8
.L_020097b6:
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #216
	adds r0, r5, r1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020097d2
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl Func_020028d0
	b .L_020097de
.L_020097d2:
	adds r0, r5, #0
	bl Object_GetById
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r0, #28]
.L_020097de:
	adds r5, #1
	cmp r5, #12
	ble .L_020097b6
	movs r0, #13
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #13
	bl Object_GetById
	movs r5, #0
	adds r0, #89
	strb r5, [r0]
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009818
	movs r3, #47
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #16
	movs r2, #1
	movs r3, #2
	bl Func_02002840
.L_02009818:
	movs r0, #14
	bl Object_GetById
	cmp r0, #0
	beq .L_02009830
	movs r3, #192
	lsls r3, r3, #8
	str r3, [r0, #24]
	str r3, [r0, #28]
	adds r3, r0, #0
	adds r3, #85
	strb r5, [r3]
.L_02009830:
	movs r1, #144
	ldr r0, .L_02009ad0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldrb r2, [r6, #23]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	ldr r2, .L_02009ad4
	strb r3, [r6, #23]
	movs r3, #93
	strb r3, [r2]
	b .L_02009e96
.L_02009850:
	ldr r3, .L_02009ad8
	cmp r2, r3
	bne .L_0200988a
	movs r3, #129
	lsls r3, r3, #2
	movs r0, #192
	str r3, [r1]
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009870
	movs r0, #64
	movs r1, #1
	bl Object_SetWideSprite
.L_02009870:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009880
	b .L_02009e96
.L_02009880:
	movs r0, #65
	movs r1, #1
	bl Object_SetWideSprite
	b .L_02009e96
.L_0200988a:
	ldr r3, .L_02009adc
	cmp r2, r3
	bne .L_020098b6
	movs r3, #129
	lsls r3, r3, #2
	movs r0, #0
	str r3, [r1]
	bl Func_02002960
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020098aa
	b .L_02009e96
.L_020098aa:
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020028d0
	b .L_02009e96
.L_020098b6:
	ldr r3, .L_02009ae0
	cmp r2, r3
	bne .L_02009922
	movs r3, #129
	lsls r3, r3, #2
	movs r0, #0
	str r3, [r1]
	bl Func_02002960
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020098de
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020028d0
.L_020098de:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020098fa
	movs r1, #140
	movs r2, #232
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_020028d0
.L_020098fa:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009916
	movs r1, #236
	movs r2, #248
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_020028d0
.L_02009916:
	ldr r0, .L_02009ae4
	ldr r1, .L_02009ae8
	ldr r2, .L_02009aec
	bl Func_02002614
	b .L_02009e96
.L_02009922:
	ldr r3, .L_02009af0
	cmp r2, r3
	bne .L_02009930
	movs r3, #129
	lsls r3, r3, #2
	str r3, [r1]
	b .L_02009e96
.L_02009930:
	ldr r3, .L_02009af4
	cmp r2, r3
	bne .L_02009a26
	movs r3, #129
	lsls r3, r3, #2
	str r3, [r1]
	movs r0, #0
	bl Func_02002960
	movs r1, #144
	ldr r0, .L_02009af8
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	cmp r2, #3
	beq .L_0200995e
	cmp r2, #5
	bne .L_02009968
.L_0200995e:
	ldr r0, .L_02009afc
	ldr r2, .L_02009b00
	movs r1, #0
	bl Func_02002614
.L_02009968:
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020099aa
	movs r3, #26
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #28
	movs r1, #8
	movs r2, #1
	movs r3, #1
	bl Func_02002840
	adds r0, r5, #0
	movs r1, #0
	bl Func_020027f8
	b .L_020099e6
.L_020099aa:
	movs r3, #26
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #22
	movs r1, #8
	bl Func_02002840
	movs r0, #8
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	adds r1, r5, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #2
	orrs r3, r2
	movs r0, #136
	strb r3, [r1]
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020099e6
	adds r0, r5, #0
	movs r1, #7
	bl Func_020027f8
.L_020099e6:
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	movs r0, #151
	adds r3, #85
	movs r6, #0
	lsls r0, r0, #4
	strb r6, [r3]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009a06
	b .L_02009e96
.L_02009a06:
	movs r1, #132
	movs r2, #212
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_020028d0
	movs r3, #16
	movs r2, #26
	str r6, [r5, #20]
	str r6, [r5, #12]
	movs r0, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #26
	b .L_02009db6
.L_02009a26:
	ldr r3, .L_02009b04
	cmp r2, r3
	beq .L_02009a2e
	b .L_02009b56
.L_02009a2e:
	movs r3, #129
	lsls r3, r3, #2
	str r3, [r1]
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	cmp r2, #2
	beq .L_02009a5e
	cmp r2, #4
	beq .L_02009a5e
	cmp r2, #9
	beq .L_02009a5e
	bl Func_02002980
	movs r1, #194
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #8
	movs r3, #9
	bl Func_02002988
	b .L_02009e96
.L_02009a5e:
	ldr r2, .L_02009b08
	movs r1, #0
	ldr r0, .L_02009b0c
	bl Func_02002614
	ldr r3, .L_02009ac4
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r1, #144
	strb r3, [r0]
	lsls r1, r1, #3
	ldr r0, .L_02009b10
	bl Scheduler_AddOrUpdateCallback
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009b14
	bl Scheduler_AddOrUpdateCallback
	movs r0, #10
	bl Object_GetById
	adds r6, r0, #0
	adds r3, r6, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009b18
	adds r0, r6, #0
	movs r1, #0
	bl Func_020027f8
	b .L_02009b40
	.2byte 0x0000
.L_02009ac4:
	.4byte gPartyState
.L_02009ac8:
	.4byte 0x00000090
.L_02009acc:
	.4byte 0x0000008f
.L_02009ad0:
	.4byte Func_02001288
.L_02009ad4:
	.4byte gDecodeFillByte
.L_02009ad8:
	.4byte 0x00000091
.L_02009adc:
	.4byte 0x00000092
.L_02009ae0:
	.4byte 0x00000093
.L_02009ae4:
	.4byte Data_02002a78
.L_02009ae8:
	.4byte Data_02002ab6
.L_02009aec:
	.4byte Func_02001304
.L_02009af0:
	.4byte 0x00000094
.L_02009af4:
	.4byte 0x00000095
.L_02009af8:
	.4byte Func_02001684
.L_02009afc:
	.4byte Data_02002abc
.L_02009b00:
	.4byte Func_02001424
.L_02009b04:
	.4byte 0x00000096
.L_02009b08:
	.4byte Func_020014a0
.L_02009b0c:
	.4byte Data_02002aca
.L_02009b10:
	.4byte Func_020016fc
.L_02009b14:
	.4byte Func_02001618
.L_02009b18:
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #2
	orrs r3, r2
	movs r0, #136
	strb r3, [r1]
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009b40
	adds r0, r6, #0
	movs r1, #7
	bl Func_020027f8
.L_02009b40:
	movs r3, #60
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #36
	movs r2, #13
	movs r3, #13
	bl Func_02002838
	b .L_02009e96
.L_02009b56:
	ldr r3, .L_02009e9c
	cmp r2, r3
	bne .L_02009c46
	movs r3, #129
	lsls r3, r3, #2
	str r3, [r1]
	ldr r0, .L_02009ea0
	ldr r2, .L_02009ea4
	movs r1, #0
	bl Func_02002614
	movs r0, #206
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009b84
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #57
	bl GameFlag_SetBit
	b .L_02009bde
.L_02009b84:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #58
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009bde
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r1, #184
	movs r2, #172
	lsls r1, r1, #16
	lsls r2, r2, #17
	movs r0, #8
	bl Func_020028d0
	movs r0, #8
	bl Object_GetById
	adds r0, #89
	strb r7, [r0]
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r3, #11
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #11
	movs r1, #19
	movs r2, #1
	movs r3, #1
	bl Func_02002840
.L_02009bde:
	movs r0, #143
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009bf6
	movs r0, #207
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02009e96
.L_02009bf6:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #61
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009c06
	b .L_02009e96
.L_02009c06:
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r1, #252
	movs r2, #184
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #9
	bl Func_020028d0
	movs r0, #9
	bl Object_GetById
	movs r3, #0
	adds r0, #89
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r2, #11
	movs r3, #31
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #13
	b .L_02009db6
.L_02009c46:
	ldr r3, .L_02009ea8
	cmp r2, r3
	beq .L_02009c4e
	b .L_02009dca
.L_02009c4e:
	movs r3, #129
	lsls r3, r3, #2
	str r3, [r1]
	movs r0, #19
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #204
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009cba
	movs r1, #240
	movs r2, #248
	movs r0, #19
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_020028d0
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009c90
	movs r1, #220
	movs r2, #232
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_020028d0
.L_02009c90:
	movs r3, #29
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #50
	movs r2, #2
	movs r3, #14
	bl Func_02002838
	movs r3, #18
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #59
	movs r2, #13
	movs r3, #5
	bl Func_02002838
	b .L_02009cc6
.L_02009cba:
	adds r3, r5, #0
	adds r3, #85
	strb r0, [r3]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #12]
.L_02009cc6:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #49
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009ce0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #50
	bl GameFlag_SetBit
	b .L_02009d3e
.L_02009ce0:
	movs r0, #141
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d36
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r1, #240
	movs r2, #248
	lsls r1, r1, #15
	lsls r2, r2, #16
	movs r0, #9
	bl Func_020028d0
	movs r0, #9
	bl Object_GetById
	movs r3, #0
	adds r0, #89
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r2, #15
	movs r3, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #7
	movs r1, #17
	movs r2, #1
	movs r3, #1
	bl Func_02002840
	b .L_02009d3e
.L_02009d36:
	movs r0, #9
	movs r1, #0
	bl Object_SetModeById
.L_02009d3e:
	movs r0, #205
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009d56
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #53
	bl GameFlag_SetBit
	b .L_02009e96
.L_02009d56:
	movs r3, #27
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #27
	movs r1, #14
	movs r2, #1
	movs r3, #1
	bl Func_02002840
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #54
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009dc0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r1, #204
	movs r2, #132
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #10
	bl Func_020028d0
	movs r0, #10
	bl Object_GetById
	movs r3, #0
	adds r0, #89
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r2, #16
	movs r3, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #18
.L_02009db6:
	movs r2, #1
	movs r3, #1
	bl Func_02002840
	b .L_02009e96
.L_02009dc0:
	movs r0, #10
	movs r1, #0
	bl Object_SetModeById
	b .L_02009e96
.L_02009dca:
	ldr r3, .L_02009eac
	cmp r2, r3
	bne .L_02009e96
	movs r3, #129
	lsls r3, r3, #2
	movs r0, #208
	str r3, [r1]
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009dee
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #65
	bl GameFlag_SetBit
	b .L_02009e0a
.L_02009dee:
	movs r0, #8
	movs r1, #0
	bl Object_SetModeById
	movs r3, #7
	movs r2, #38
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #7
	movs r1, #39
	movs r2, #1
	movs r3, #1
	bl Func_02002840
.L_02009e0a:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #66
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009e24
	movs r0, #145
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02009e2c
.L_02009e24:
	movs r0, #9
	movs r1, #0
	bl Object_SetModeById
.L_02009e2c:
	movs r0, #210
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009e40
	movs r0, #64
	movs r1, #0
	bl Object_SetWideSprite
.L_02009e40:
	ldr r3, .L_02009eb0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r5, [r3, r0]
	subs r3, r5, #2
	cmp r3, #1
	bhi .L_02009e6c
	movs r0, #208
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009e66
	movs r0, #0
	bl Func_02002084
	b .L_02009e6c
.L_02009e66:
	movs r0, #0
	bl Func_02002114
.L_02009e6c:
	cmp r5, #6
	bne .L_02009e8c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #66
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009e86
	movs r0, #0
	bl Func_02002084
	b .L_02009e8c
.L_02009e86:
	movs r0, #0
	bl Func_02002114
.L_02009e8c:
	subs r3, r5, #4
	cmp r3, #1
	bhi .L_02009e96
	bl Func_02002828
.L_02009e96:
	movs r0, #0
	add sp, #8
	pop {r5, r6, r7, pc}
.L_02009e9c:
	.4byte 0x00000097
.L_02009ea0:
	.4byte Data_02002ad8
.L_02009ea4:
	.4byte Func_0200151c
.L_02009ea8:
	.4byte 0x00000098
.L_02009eac:
	.4byte 0x00000099
.L_02009eb0:
	.4byte gPartyState
	.section .text.x02009eb8,"ax",%progbits
	.global Func_02001eb8
	.thumb_func
Func_02001eb8:
	push {r5, lr}
	movs r0, #210
	movs r5, #1
	lsls r0, r0, #2
	sub sp, #8
	negs r5, r5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009ed0
	movs r5, #62
	b .L_02009fc8
.L_02009ed0:
	ldr r1, .L_02009fe8
	ldr r3, [r1]
	cmp r3, #50
	bhi .L_02009fba
	ldr r2, .L_02009fec
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02009ee0:
	.4byte .L_02009fac
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fb0
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fb4
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fb8
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fb4
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fba
	.4byte .L_02009fb0
.L_02009fac:
	movs r5, #59
	b .L_02009fba
.L_02009fb0:
	movs r5, #56
	b .L_02009fba
.L_02009fb4:
	movs r5, #53
	b .L_02009fba
.L_02009fb8:
	movs r5, #50
.L_02009fba:
	ldr r3, [r1]
	adds r3, #1
	str r3, [r1]
	cmp r3, #59
	ble .L_02009fc8
	movs r3, #0
	str r3, [r1]
.L_02009fc8:
	movs r3, #1
	negs r3, r3
	cmp r5, r3
	beq .L_02009fe4
	movs r3, #76
	movs r2, #61
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	adds r1, r5, #0
	movs r2, #3
	movs r3, #3
	bl Func_02002848
.L_02009fe4:
	add sp, #8
	pop {r5, pc}
.L_02009fe8:
	.4byte Data_020035f0
.L_02009fec:
	.4byte .L_02009ee0
	.section .text.x02009ff0,"ax",%progbits
	.global Func_02001ff0
	.thumb_func
Func_02001ff0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #32]
	movs r2, #188
	lsls r2, r2, #1
	adds r5, r7, r2
	lsls r3, r0, #19
	str r3, [r5, #8]
	lsls r3, r1, #19
	str r3, [r5, #12]
	movs r3, #127
	strh r3, [r5, #40]
	movs r3, #254
	lsls r3, r3, #6
	strh r3, [r5, #42]
	lsls r3, r1, #7
	strh r3, [r5, #46]
	lsrs r3, r1, #31
	adds r1, r1, r3
	lsrs r3, r0, #31
	movs r2, #0
	strh r0, [r5, #44]
	asrs r1, r1, #1
	adds r0, r0, r3
	str r2, [r5, #24]
	str r2, [r5, #28]
	str r2, [r5, #32]
	str r2, [r5, #36]
	asrs r0, r0, #1
	ldr r2, .L_0200a078
	lsls r1, r1, #7
	adds r1, r1, r0
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r5, #48]
	movs r4, #128
	ldr r3, .L_0200a07c
	lsls r4, r4, #9
	str r4, [r5, #16]
	str r4, [r5, #20]
	adds r1, r1, r3
	adds r3, r7, #0
	adds r3, #228
	str r1, [r5, #52]
	ldr r6, .L_0200a080
	adds r1, r4, #0
	ldr r0, [r3]
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	ldr r1, [r5, #20]
	adds r0, r0, r3
	str r0, [r5]
	adds r3, r7, #0
	adds r3, #232
	ldr r0, [r3]
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #12]
	adds r0, r0, r3
	str r0, [r5, #4]
	bl Func_02002810
	movs r0, #1
	bl WaitFrames
	pop {r5, r6, r7, pc}
.L_0200a078:
	.4byte gMapCellBuffer
.L_0200a07c:
	.4byte gMapShapeGrid
.L_0200a080:
	.4byte IwramMulQ16
	.section .text.x0200a084,"ax",%progbits
	.global Func_02002084
	.thumb_func
Func_02002084:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02002828
	cmp r5, #0
	beq .L_0200a0da
	movs r3, #224
	movs r5, #128
	lsls r3, r3, #4
	lsls r5, r5, #19
	adds r3, #6
	adds r5, #82
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #4
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #2
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #1
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #4
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
.L_0200a0da:
	movs r0, #100
	movs r1, #50
	bl Func_02001ff0
	movs r2, #192
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #7
	adds r3, #82
	strh r2, [r3]
	ldr r3, .L_0200a10c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #6
	bne .L_0200a10a
	movs r1, #144
	ldr r0, .L_0200a110
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_0200a10a:
	pop {r5, pc}
.L_0200a10c:
	.4byte gPartyState
.L_0200a110:
	.4byte Func_02001eb8
	.section .text.x0200a114,"ax",%progbits
	.global Func_02002114
	.thumb_func
Func_02002114:
	push {lr}
	movs r0, #100
	movs r1, #100
	bl Func_02001ff0
	pop {pc}
	.section .text.x0200a120,"ax",%progbits
	.global Func_02002120
	.thumb_func
Func_02002120:
	push {r5, lr}
	movs r0, #10
	adds r0, #255
	ldr r5, .L_0200a13c
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a13a
	ldr r3, .L_0200a140
	adds r0, r5, #0
	movs r1, #12
	mov lr, r3
	.2byte 0xf800
.L_0200a13a:
	pop {r5, pc}
.L_0200a13c:
	.4byte gSceneState
.L_0200a140:
	.4byte IwramClearWords
	.section .text.x0200a144,"ax",%progbits
	.global Func_02002144
	.thumb_func
Func_02002144:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_0200a224
	sub sp, #4
	mov r9, r0
	bl Func_02002998
	bl Object_GetById
	ldr r7, [r0, #80]
	mov r8, r0
	ldr r2, [r7, #44]
	cmp r2, #0
	beq .L_0200a16e
	movs r3, #1
	strb r3, [r2, #6]
.L_0200a16e:
	ldrb r2, [r7, #26]
	movs r3, #8
	orrs r3, r2
	movs r2, #254
	ands r3, r2
	strb r3, [r7, #26]
	movs r3, #1
	strb r3, [r7, #25]
	ldr r1, .L_0200a228
	movs r2, #240
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	beq .L_0200a1b0
	ldr r3, [r1]
	movs r2, #2
	movs r0, #160
	ands r3, r2
	lsls r0, r0, #5
	cmp r3, #0
	beq .L_0200a19e
	lsls r3, r0, #1
	adds r3, r3, r0
	asrs r0, r3, #1
.L_0200a19e:
	movs r1, #96
	bl __divsi3
	mov r1, r9
	ldrh r3, [r1, #2]
	mov r2, r9
	adds r3, r3, r0
	adds r3, #1
	strh r3, [r2, #2]
.L_0200a1b0:
	mov r1, r9
	movs r3, #2
	ldrsh r0, [r1, r3]
	adds r2, r0, #0
	cmp r0, #0
	bge .L_0200a1be
	adds r2, #255
.L_0200a1be:
	ldrb r1, [r7, #17]
	asrs r2, r2, #8
	movs r3, #3
	lsls r2, r2, #2
	ands r3, r1
	orrs r3, r2
	strb r3, [r7, #17]
	mov r2, r9
	movs r3, #1
	strh r3, [r2]
	movs r3, #160
	lsls r3, r3, #5
	cmp r0, r3
	blt .L_0200a29c
	bl Func_02002898
	movs r0, #0
	bl Func_02002968
	ldr r3, .L_0200a220
	mov r0, r8
	adds r0, #84
	str r0, [sp, #0]
	strb r3, [r0]
	movs r1, #1
	mov r11, r1
.L_0200a1f2:
	mov r0, r8
	mov r2, r8
	ldr r3, [r0, #16]
	movs r0, #14
	ldr r1, [r2, #8]
	adds r0, #255
	ldr r2, [r2, #12]
	bl Func_02002808
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200a25a
	ldr r1, .L_0200a22c
	ldr r6, [r5, #80]
	bl Func_02002800
	movs r1, #0
	adds r3, r5, #0
	mov r10, r1
	adds r3, #85
	mov r2, r10
	b .L_0200a230
	.2byte 0x0000
.L_0200a220:
	.4byte 0x00000000
.L_0200a224:
	.4byte gSceneState
.L_0200a228:
	.4byte gInput
.L_0200a22c:
	.4byte Data_02002ae8
.L_0200a230:
	strb r2, [r3]
	adds r2, r5, #0
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	cmp r6, #0
	beq .L_0200a25a
	adds r0, r6, #0
	movs r1, #2
	bl Animation_ApplyChildArgument
	mov r3, r10
	strb r3, [r6, #26]
	movs r0, #13
	ldrb r3, [r6, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r6, #9]
.L_0200a25a:
	movs r0, #15
	bl WaitFrames
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	cmp r2, #0
	bge .L_0200a1f2
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, [sp, #0]
	ldrb r2, [r7, #17]
	movs r3, #1
	strb r3, [r0]
	movs r3, #3
	ands r3, r2
	movs r5, #0
	strb r3, [r7, #17]
	strb r5, [r7, #26]
	bl Func_020028a0
	mov r1, r9
	ldr r3, [r1, #4]
	mov r2, r8
	str r3, [r2, #8]
	mov r0, r9
	ldr r3, [r1, #8]
	str r3, [r2, #16]
	mov r3, r9
	strh r5, [r3]
	strh r5, [r0, #2]
.L_0200a29c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a2ac,"ax",%progbits
	.global Func_020022ac
	.thumb_func
Func_020022ac:
	push {r5, r6, r7, lr}
	ldr r7, .L_0200a32c
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #2
	beq .L_0200a32a
	bl Func_02002998
	bl Object_GetById
	adds r5, r0, #0
	ldr r6, [r5, #80]
	bl Func_02002898
	movs r0, #0
	bl Func_02002968
	ldr r2, .L_0200a330
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	movs r0, #128
	lsls r0, r0, #12
	ands r1, r2
	ands r3, r2
	adds r1, r1, r0
	adds r3, r3, r0
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_02002818
	adds r0, r5, #0
	bl Func_02002820
	b .L_0200a308
.L_0200a2f0:
	lsrs r2, r1, #2
	adds r2, #255
	movs r3, #3
	ands r3, r1
	lsls r2, r2, #2
	orrs r3, r2
	strb r3, [r6, #17]
	movs r3, #1
	strb r3, [r6, #25]
	movs r0, #1
	bl WaitFrames
.L_0200a308:
	ldrb r1, [r6, #17]
	movs r3, #252
	ands r3, r1
	cmp r3, #0
	bne .L_0200a2f0
	bl Func_020028a0
	ldrb r2, [r6, #26]
	movs r3, #8
	orrs r3, r2
	movs r2, #254
	ands r3, r2
	movs r1, #0
	strb r3, [r6, #26]
	movs r3, #2
	strh r3, [r7]
	strh r1, [r7, #2]
.L_0200a32a:
	pop {r5, r6, r7, pc}
.L_0200a32c:
	.4byte gSceneState
.L_0200a330:
	.4byte 0xfff00000
	.section .text.x0200a334,"ax",%progbits
	.global Func_02002334
	.thumb_func
Func_02002334:
	push {r5, lr}
	ldr r5, .L_0200a378
	bl Func_0200237c
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_0200a376
	bl Func_02002998
	bl Object_GetById
	ldr r0, [r0, #80]
	ldr r2, [r0, #44]
	cmp r2, #0
	beq .L_0200a358
	movs r3, #9
	strb r3, [r2, #6]
.L_0200a358:
	ldrb r2, [r0, #26]
	movs r3, #247
	ands r3, r2
	movs r2, #1
	orrs r3, r2
	ldrb r2, [r0, #17]
	strb r3, [r0, #26]
	movs r3, #3
	ands r3, r2
	movs r1, #0
	strb r3, [r0, #17]
	movs r3, #1
	strb r3, [r0, #25]
	strh r1, [r5]
	strh r1, [r5, #2]
.L_0200a376:
	pop {r5, pc}
.L_0200a378:
	.4byte gSceneState
	.section .text.x0200a37c,"ax",%progbits
	.global Func_0200237c
	.thumb_func
Func_0200237c:
	push {r5, lr}
	ldr r5, .L_0200a3a0
	bl Func_02002998
	bl Object_GetById
	ldr r1, .L_0200a3a4
	ldr r3, [r0, #8]
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [r5, #8]
	pop {r5, pc}
.L_0200a3a0:
	.4byte gSceneState
.L_0200a3a4:
	.4byte 0xfff00000
	.section .text.x0200a3a8,"ax",%progbits
	.global Func_020023a8
	.thumb_func
Func_020023a8:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_0200a3be
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_0200a3c8
	b .L_0200a3f8
.L_0200a3be:
	movs r2, #192
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_0200a3f8
.L_0200a3c8:
	ldr r2, [r1, #12]
	ldr r3, [r0, #12]
	subs r3, r3, r2
	ldr r2, .L_0200a3fc
	subs r3, #1
	cmp r3, r2
	bhi .L_0200a3f8
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_0200a3ea
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_0200a3f4
	b .L_0200a3f8
.L_0200a3ea:
	movs r2, #128
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_0200a3f8
.L_0200a3f4:
	movs r0, #1
	b .L_0200a3fa
.L_0200a3f8:
	movs r0, #0
.L_0200a3fa:
	pop {pc}
.L_0200a3fc:
	.4byte 0x000ffffe
	.section .text.x0200a400,"ax",%progbits
	.global Func_02002400
	.thumb_func
Func_02002400:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, .L_0200a4f0
	ldr r3, .L_0200a4f4
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	bl Object_GetById
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_0200a44e
	adds r3, #15
.L_0200a44e:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_0200a49a
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_020023a8
	cmp r0, #0
	beq .L_0200a49a
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	movs r3, #0
	str r3, [r6, #76]
.L_0200a49a:
	movs r3, #164
	lsls r3, r3, #1
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_0200a4e6
	adds r5, r2, r3
.L_0200a4aa:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_0200a4d8
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_020023a8
	cmp r0, #0
	beq .L_0200a4d8
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	movs r3, #0
	str r3, [r6, #76]
.L_0200a4d8:
	adds r7, #1
	cmp r7, #3
	bgt .L_0200a4e6
	adds r5, #2
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_0200a4aa
.L_0200a4e6:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a4f0:
	.4byte Data_020023c4 + 0x188
.L_0200a4f4:
	.4byte gPartyState
	.section .text.x0200a4f8,"ax",%progbits
	.global Func_020024f8
	.thumb_func
Func_020024f8:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r2, r5, #0
	adds r3, #4
	strb r6, [r3]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r3, #3
	ldr r0, [r5, #80]
	ands r1, r3
	ldrb r2, [r0, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	lsls r1, r1, #2
	orrs r3, r1
	strb r3, [r0, #9]
	movs r1, #0
	adds r0, r5, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Func_020027f8
	adds r0, r5, #0
	ldr r1, .L_0200a550
	bl Func_02002800
	adds r0, r5, #0
	movs r1, #10
	bl Object_SetPartAttribute
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
.L_0200a550:
	.4byte Data_02002af4
	.section .text.x0200a554,"ax",%progbits
	.global Func_02002554
	.thumb_func
Func_02002554:
	push {r5, r6, lr}
	ldr r3, [r0, #8]
	ldr r2, [r0, #4]
	adds r6, r1, #0
	ldr r1, [r0]
	ldr r0, .L_0200a5c8
	adds r3, r3, r0
	movs r0, #30
	adds r0, #255
	bl Func_02002808
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200a5c6
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #12
	movs r3, #192
	lsls r3, r3, #6
	lsrs r0, r0, #16
	adds r0, r0, r3
	bl Math_Cosine
	str r0, [r5, #68]
	bl Random16Far
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r0, [r5, #72]
	bl Random16Far
	lsls r0, r0, #17
	lsrs r0, r0, #16
	adds r0, r0, r6
	str r0, [r5, #76]
	bl Random16Far
	ldr r3, .L_0200a5cc
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	movs r1, #3
	adds r0, r5, #0
	bl Func_020024f8
	ldr r3, .L_0200a5d0
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #1
	bl Animation_SetStateFlags
.L_0200a5c6:
	pop {r5, r6, pc}
.L_0200a5c8:
	.4byte 0xfffe0000
.L_0200a5cc:
	.4byte 0xffff8000
.L_0200a5d0:
	.4byte Func_02002400
	.section .text.x0200a5d4,"ax",%progbits
	.global Func_020025d4
	.thumb_func
Func_020025d4:
	push {r5, r6, lr}
	ldr r3, .L_0200a610
	movs r6, #15
	ldr r0, [r3, #4]
	adds r5, r3, #0
	mov lr, r0
	.2byte 0xf800
	adds r5, #8
.L_0200a5e4:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_0200a606
	movs r1, #2
	ldrsh r3, [r5, r1]
	ldrh r2, [r5, #2]
	cmp r3, #0
	bgt .L_0200a602
	adds r0, r5, #4
	ldr r1, [r5, #16]
	bl Func_02002554
	movs r3, #9
	b .L_0200a604
.L_0200a602:
	subs r3, r2, #1
.L_0200a604:
	strh r3, [r5, #2]
.L_0200a606:
	subs r6, #1
	adds r5, #20
	cmp r6, #0
	bge .L_0200a5e4
	pop {r5, r6, pc}
.L_0200a610:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a614,"ax",%progbits
	.global Func_02002614
	.thumb_func
Func_02002614:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	ldr r0, .L_0200a6fc
	mov r9, r2
	mov r10, r0
	movs r2, #8
	movs r0, #10
	add r2, r10
	adds r0, #255
	sub sp, #4
	adds r7, r1, #0
	mov r8, r2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a6bc
	movs r1, #168
	lsls r1, r1, #1
	ldr r3, .L_0200a700
	mov r0, r10
	mov lr, r3
	.2byte 0xf800
	ldrh r1, [r6]
	movs r4, #0
	adds r6, #2
	cmp r1, #0
	ble .L_0200a694
.L_0200a652:
	ldrh r2, [r6]
	mov r0, r8
	movs r3, #0
	ldrh r5, [r6, #2]
	strh r3, [r0]
	movs r3, #128
	lsls r3, r3, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	str r2, [r0, #12]
	str r1, [r0, #4]
	adds r2, r2, r3
	movs r0, #0
	str r4, [sp, #0]
	bl Map_GetTerrainHeight
	ldr r4, [sp, #0]
	mov r2, r8
	mov r3, r8
	lsls r5, r5, #16
	str r0, [r2, #8]
	str r5, [r2, #16]
	movs r0, #20
	strh r4, [r3, #2]
	adds r4, #1
	adds r6, #4
	add r8, r0
	cmp r4, #15
	bgt .L_0200a694
	ldrh r1, [r6]
	adds r6, #2
	cmp r1, #0
	bgt .L_0200a652
.L_0200a694:
	cmp r7, #0
	beq .L_0200a6bc
	ldrh r1, [r7]
	movs r4, #0
	adds r7, #2
	cmp r1, #0
	ble .L_0200a6bc
.L_0200a6a2:
	movs r2, #164
	lsls r3, r4, #1
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r0, r10
	adds r4, #1
	strh r1, [r0, r3]
	cmp r4, #3
	bgt .L_0200a6bc
	ldrh r1, [r7]
	adds r7, #2
	cmp r1, #0
	bgt .L_0200a6a2
.L_0200a6bc:
	movs r1, #128
	lsls r1, r1, #19
	mov r3, r10
	mov r2, r9
	adds r1, #80
	str r2, [r3, #4]
	ldrh r3, [r1]
	cmp r3, #0
	bne .L_0200a6e4
	movs r3, #192
	movs r2, #128
	lsls r3, r3, #4
	lsls r2, r2, #19
	adds r3, #8
	adds r2, #82
	strh r3, [r2]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #16
	strh r3, [r1]
.L_0200a6e4:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a704
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a6fc:
	.4byte Data_020023c4 + 0x188
.L_0200a700:
	.4byte IwramClearWords
.L_0200a704:
	.4byte Func_020025d4
	.section .text.x0200a708,"ax",%progbits
	.global Func_02002708
	.thumb_func
Func_02002708:
	ldr r2, .L_0200a718
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r3, r3, r2
	movs r2, #8
	ldrsh r0, [r3, r2]
	bx lr
.L_0200a718:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a71c,"ax",%progbits
	.global Func_0200271c
	.thumb_func
Func_0200271c:
	ldr r2, .L_0200a72c
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r3, r3, r2
	strh r1, [r3, #8]
	bx lr
	.2byte 0x0000
.L_0200a72c:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a730,"ax",%progbits
	.global Func_02002730
	.thumb_func
Func_02002730:
	ldr r3, .L_0200a738
	ldr r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200a738:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a73c,"ax",%progbits
	.global Func_0200273c
	.thumb_func
Func_0200273c:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	ldr r5, .L_0200a760
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a75a
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	cmp r3, r6
	blt .L_0200a75a
	str r0, [r5]
.L_0200a75a:
	ldr r0, [r5]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a760:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a764,"ax",%progbits
	.global Func_02002764
	.thumb_func
Func_02002764:
	ldr r2, .L_0200a774
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r3, r3, r2
	str r1, [r3, #24]
	bx lr
	.2byte 0x0000
.L_0200a774:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a778,"ax",%progbits
	.global Func_02002778
	.thumb_func
Func_02002778:
	push {lr}
	ldr r2, .L_0200a78c
	cmp r0, #3
	bhi .L_0200a78a
	lsls r3, r0, #1
	movs r0, #164
	lsls r0, r0, #1
	adds r3, r3, r0
	strh r1, [r2, r3]
.L_0200a78a:
	pop {pc}
.L_0200a78c:
	.4byte Data_020023c4 + 0x188
	.section .rodata.x0200a9b8,"a",%progbits
	.global Data_020029b8
Data_020029b8:
	.4byte 0x06090200
	.4byte 0x01050604
	.global Data_020029c0
Data_020029c0:
	.4byte 0x00010003
	.4byte 0x00000001
	.global Data_020029c8
Data_020029c8:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_02002a04
Data_02002a04:
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_02002a48
Data_02002a48:
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000026
	.global Data_02002a54
Data_02002a54:
	.4byte 0x0000002e
	.4byte Func_02001228
	.4byte 0x00000026
	.global Data_02002a60
Data_02002a60:
	.4byte 0x0000002e
	.4byte Func_02001564
	.4byte 0x00000026
	.global Data_02002a6c
Data_02002a6c:
	.4byte 0x0000002e
	.4byte Func_0200166c
	.4byte 0x00000026
	.global Data_02002a78
Data_02002a78:
	.4byte 0x00c800c8
	.4byte 0x01080004
	.4byte 0x000400c8
	.4byte 0x00d800e8
	.4byte 0x01180004
	.4byte 0x000400d8
	.4byte 0x00c80238
	.4byte 0x02780004
	.4byte 0x000400c8
	.4byte 0x00d80218
	.4byte 0x02580004
	.4byte 0x000400d8
	.4byte 0x00e80198
	.4byte 0x01d80004
	.4byte 0x000400e8
	.2byte 0x0000
	.global Data_02002ab6
Data_02002ab6:
	.2byte 0x0009
	.4byte 0x0000000a
	.global Data_02002abc
Data_02002abc:
	.4byte 0x006801a8
	.4byte 0x02180004
	.4byte 0x00040088
	.2byte 0x0000
	.global Data_02002aca
Data_02002aca:
	.2byte 0x0198
	.4byte 0x00040198
	.4byte 0x016801d8
	.4byte 0x00000004
	.global Data_02002ad8
Data_02002ad8:
	.4byte 0x00b800d8
	.4byte 0x01380004
	.4byte 0x00040088
	.4byte 0x00000000
	.global Data_02002ae8
Data_02002ae8:
	.4byte 0x00000000
	.4byte 0x00000020
	.4byte 0x00000026
	.global Data_02002af4
Data_02002af4:
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x0000008f
	.4byte 0x00123002
	.4byte 0x00201090
	.4byte 0x00302090
	.4byte 0x00000090
	.4byte 0x0010208f
	.4byte 0x0020308f
	.4byte 0x00303091
	.4byte 0x00404094
	.4byte 0x00505091
	.4byte 0x00000091
	.4byte 0x00303090
	.4byte 0x00505090
	.4byte 0x00602093
	.4byte 0x00703093
	.4byte 0x00809091
	.4byte 0x00908091
	.4byte 0x00000092
	.4byte 0x00101093
	.4byte 0x00204093
	.4byte 0x00000093
	.4byte 0x00101092
	.4byte 0x00206091
	.4byte 0x00307091
	.4byte 0x00402092
	.4byte 0x00a0a092
	.4byte 0x00b0b092
	.4byte 0x00c0c092
	.4byte 0x00d0d092
	.4byte 0x00e0e092
	.4byte 0x00f0f092
	.4byte 0x01010092
	.4byte 0x00000094
	.4byte 0x00101095
	.4byte 0x00404090
	.4byte 0x00000095
	.4byte 0x00101094
	.4byte 0x00202096
	.4byte 0x00303096
	.4byte 0x00405095
	.4byte 0x00504095
	.4byte 0x00a09096
	.4byte 0x00c09096
	.4byte 0x00000096
	.4byte 0x00101098
	.4byte 0x00202095
	.4byte 0x00303095
	.4byte 0x00405096
	.4byte 0x00504096
	.4byte 0x00000098
	.4byte 0x00101096
	.4byte 0x00202099
	.4byte 0x00303097
	.4byte 0x00404097
	.4byte 0x00000097
	.4byte 0x00303098
	.4byte 0x00404098
	.4byte 0x00000099
	.4byte 0x00202098
	.4byte 0x00304099
	.4byte 0x00403099
	.4byte 0x00506099
	.4byte 0x00605099
	.4byte 0x000001ff
	.global Data_02002c54
Data_02002c54:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002c6c
Data_02002c6c:
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x0002c000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x0002c000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x0002c000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x0002c000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x0002c000
	.4byte 0xffff012b
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x0002c000
	.4byte 0x08e50111
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002d2c
Data_02002d2c:
	.4byte 0x003b00f3
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002d5c
Data_02002d5c:
	.4byte 0x003b00f3
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00004000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002dbc
Data_02002dbc:
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002e04
Data_02002e04:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002e64
Data_02002e64:
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0102c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0102c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0102c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0102c000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x0002c000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x0102c000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x0102c000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002f9c
Data_02002f9c:
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0002c000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002fe4
Data_02002fe4:
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x0002c000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200302c
Data_0200302c:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003038
Data_02003038:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003068
Data_02003068:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000ce01
	.4byte 0x18e00003
	.4byte 0x00000003
	.4byte 0x0000ce01
	.4byte 0x18e30004
	.4byte 0x00000004
	.4byte 0x0000ce01
	.4byte 0x18e40005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0xffff003c
	.4byte Func_02002144
	.4byte 0x00000002
	.4byte 0xffff003d
	.4byte Func_020022ac
	.4byte 0x00000002
	.4byte 0xffff003e
	.4byte Func_02002334
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte Func_02000104
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte Func_02000104
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte Func_02000104
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte Func_02000104
	.4byte 0x00004e15
	.4byte 0xffff000c
	.4byte Func_02000104
	.4byte 0x00000c15
	.4byte 0xffff000d
	.4byte Func_02000118
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte Func_0200013c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003128
Data_02003128:
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x50008805
	.4byte 0x03010033
	.4byte Func_020002f4
	.4byte 0x50008805
	.4byte 0x03000032
	.4byte Func_02000304
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003194
Data_02003194:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000038
	.4byte 0x50008905
	.4byte 0xffff001e
	.4byte Func_02000314
	.4byte 0x50008905
	.4byte 0xffff001f
	.4byte Func_02000334
	.4byte 0x50008905
	.4byte 0xffff0020
	.4byte Func_020003c4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020031e8
Data_020031e8:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte Func_02000428
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte Func_02000428
	.4byte 0x00004602
	.4byte 0xffff000c
	.4byte Func_02000428
	.4byte 0x00004602
	.4byte 0xffff000d
	.4byte Func_02000428
	.4byte 0x00004602
	.4byte 0xffff000e
	.4byte Func_02000428
	.4byte 0x00004602
	.4byte 0xffff000f
	.4byte Func_02000428
	.4byte 0x00004602
	.4byte 0xffff0010
	.4byte Func_02000428
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_020004c8
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte Func_020004c8
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte Func_020004c8
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte Func_020004c8
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte Func_020004c8
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte Func_020004c8
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte Func_020004c8
	.4byte 0x00000002
	.4byte 0xffff0011
	.4byte Func_020004c8
	.4byte 0x00000002
	.4byte 0xffff0012
	.4byte Func_020004c8
	.4byte 0x00008c15
	.4byte 0x0a6d0009
	.4byte Func_02000464
	.4byte 0x00008c15
	.4byte 0x0a6e000a
	.4byte Func_02000464
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000464
	.4byte 0x00000006
	.4byte 0x03100064
	.4byte Func_0200065c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003314
Data_02003314:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003338
Data_02003338:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte Func_02000428
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_020004c8
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte Func_020004c8
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte Func_020004c8
	.4byte 0x00008c15
	.4byte 0x0a6f0009
	.4byte Func_02000744
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000744
	.4byte 0x50002115
	.4byte 0x02200008
	.4byte Func_02000784
	.4byte 0x00001815
	.4byte 0x12220008
	.4byte Func_020007bc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020033e0
Data_020033e0:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_020004c8
	.4byte 0x00008515
	.4byte 0x03080008
	.4byte 0x00000000
	.4byte 0x50002115
	.4byte 0x0220000a
	.4byte Func_02000920
	.4byte 0x00001815
	.4byte 0x1222000a
	.4byte Func_0200096c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003458
Data_02003458:
	.4byte 0x00000070
	.global Data_0200345c
Data_0200345c:
	.4byte 0x00000078
	.global Data_02003460
Data_02003460:
	.4byte 0x00000018
	.global Data_02003464
Data_02003464:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00002115
	.4byte 0x03310009
	.4byte Func_02000adc
	.4byte 0x00002115
	.4byte 0x0334000a
	.4byte Func_02000b58
	.4byte 0x00008c15
	.4byte 0x03320009
	.4byte Func_02000afc
	.4byte 0x00008c15
	.4byte 0x0335000a
	.4byte Func_02000b90
	.4byte 0x00000009
	.4byte 0x03350000
	.4byte Func_02000b90
	.4byte 0x00008715
	.4byte 0x0330000b
	.4byte Func_02000d2c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020034e8
Data_020034e8:
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_020004c8
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte Func_020004c8
	.4byte 0x00000602
	.4byte 0xffff0040
	.4byte Func_020010bc
	.4byte 0x00002115
	.4byte 0x03380008
	.4byte Func_02000f9c
	.4byte 0x00002115
	.4byte 0x033b0009
	.4byte Func_02001018
	.4byte 0x00008c15
	.4byte 0x03390008
	.4byte Func_02000fbc
	.4byte 0x00008c15
	.4byte 0x033c0009
	.4byte Func_02001038
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003560
Data_02003560:
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000053
	.4byte 0x03480032
	.4byte Func_02001138
	.4byte 0x00002115
	.4byte 0x03400008
	.4byte Func_020010c4
	.4byte 0x00002115
	.4byte 0x03420009
	.4byte Func_02001100
	.4byte 0x00008c15
	.4byte 0x03410008
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0x03430009
	.4byte 0x00000000
	.4byte 0x50008805
	.4byte 0x03480032
	.4byte Func_02001128
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020035f0
Data_020035f0:
	.4byte 0x00000000
