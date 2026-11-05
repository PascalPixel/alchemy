.syntax unified
	.thumb
	.section .text.x0200813c,"ax",%progbits
	.balign 4
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
	bl Actor_GetById
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
	bl wait
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
	bl wait
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
	bl wait
	movs r1, #1
	negs r1, r1
	add r9, r1
	mov r2, r9
	cmp r2, #0
	bge .L_0200825e
	movs r0, #30
	bl FieldEvent_Wait
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
	.4byte gData_02002ae8
	.section .text.x020084c8,"ax",%progbits
	.balign 4
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
	bl Actor_GetById
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
	bl wait
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
	.4byte gMainRoutineWork
.L_02008638:
	.4byte 0x00000093
.L_0200863c:
	.4byte gData_020029b8
.L_02008640:
	.4byte 0x00000095
.L_02008644:
	.4byte gData_020029c0 + 0x1
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
	.section .text.x02009ff0,"ax",%progbits
	.balign 4
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
	bl wait
	pop {r5, r6, r7, pc}
.L_0200a078:
	.4byte gMapCellBuffer
.L_0200a07c:
	.4byte gMapShapeGrid
.L_0200a080:
	.4byte IwramMulQ16
	.section .text.x0200a144,"ax",%progbits
	.balign 4
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
	bl Actor_GetById
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
	.4byte gData_02002ae8
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
	bl wait
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	cmp r2, #0
	bge .L_0200a1f2
	movs r0, #30
	bl FieldEvent_Wait
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
