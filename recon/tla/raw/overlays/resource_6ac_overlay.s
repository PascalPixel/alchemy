.syntax unified
	.thumb
	.section .text.x020080c0,"ax",%progbits
	.global Func_020000c0
	.thumb_func
Func_020000c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_020082a0
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r3, #192
	lsls r3, r3, #18
	mov r11, r2
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_020080f2
	b .L_02008292
.L_020080f2:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r10, r3
	cmp r3, #0
	beq .L_02008108
	b .L_02008292
.L_02008108:
	ldr r7, .L_020082a4
	movs r2, #1
	ldr r3, [r7]
	negs r2, r2
	cmp r3, r2
	bne .L_02008140
	ldr r1, .L_020082a8
	movs r2, #32
	ldr r3, .L_020082ac
	ldr r0, .L_020082b0
	mov lr, r3
	.2byte 0xf800
	ldr r1, .L_020082b4
	movs r2, #64
	mov r8, r1
	ldr r3, .L_020082ac
	ldr r1, .L_020082b8
	mov r0, r8
	mov lr, r3
	.2byte 0xf800
	mov r1, r8
	ldr r0, .L_020082bc
	movs r2, #64
	ldr r3, .L_020082ac
	mov lr, r3
	.2byte 0xf800
	mov r1, r10
	str r1, [r7]
.L_02008140:
	ldr r0, [r7]
	cmp r0, #0
	bge .L_02008148
	adds r0, #7
.L_02008148:
	movs r1, #6
	asrs r0, r0, #3
	bl __modsi3
	movs r2, #32
	mov r9, r0
	ldr r1, .L_020082b0
	ldr r0, .L_020082bc
	ldr r3, .L_020082ac
	mov lr, r3
	.2byte 0xf800
	mov r2, r11
	cmp r2, #1
	bne .L_0200819e
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200819e
	movs r3, #20
	movs r7, #0
	mov r8, r3
.L_02008178:
	ldr r1, .L_020082b4
	mov r2, r9
	adds r0, r2, r7
	mov r10, r1
	movs r1, #6
	bl __modsi3
	lsls r0, r0, #1
	mov r1, r10
	adds r0, #20
	ldrh r3, [r1, r0]
	ldr r2, .L_020082bc
	mov r1, r8
	strh r3, [r2, r1]
	adds r7, #1
	movs r2, #2
	add r8, r2
	cmp r7, #5
	ble .L_02008178
.L_0200819e:
	mov r3, r11
	cmp r3, #2
	bne .L_020081de
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #51
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081de
	movs r1, #20
	movs r7, #0
	mov r8, r1
.L_020081b8:
	ldr r2, .L_020082b4
	mov r3, r9
	adds r0, r3, r7
	movs r1, #6
	mov r10, r2
	bl __modsi3
	lsls r0, r0, #1
	mov r1, r10
	adds r0, #40
	ldrh r3, [r1, r0]
	ldr r2, .L_020082bc
	mov r1, r8
	strh r3, [r2, r1]
	adds r7, #1
	movs r2, #2
	add r8, r2
	cmp r7, #5
	ble .L_020081b8
.L_020081de:
	mov r3, r11
	cmp r3, #3
	bne .L_0200821c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #52
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200821c
	movs r7, #0
	movs r5, #20
.L_020081f6:
	ldr r1, .L_020082bc
	ldr r2, .L_020082b4
	mov r3, r9
	adds r0, r3, r7
	mov r10, r1
	movs r1, #6
	mov r8, r2
	bl __modsi3
	lsls r0, r0, #1
	adds r0, #52
	mov r1, r8
	ldrh r3, [r1, r0]
	mov r2, r10
	adds r7, #1
	strh r3, [r2, r5]
	adds r5, #2
	cmp r7, #5
	ble .L_020081f6
.L_0200821c:
	mov r3, r11
	cmp r3, #4
	bne .L_0200825a
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #53
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200825a
	movs r7, #0
	movs r6, #20
.L_02008234:
	ldr r1, .L_020082bc
	ldr r2, .L_020082b4
	mov r3, r9
	adds r0, r3, r7
	mov r10, r1
	movs r1, #6
	mov r8, r2
	bl __modsi3
	lsls r0, r0, #1
	adds r0, #8
	mov r1, r8
	ldrh r3, [r1, r0]
	mov r2, r10
	adds r7, #1
	strh r3, [r2, r6]
	adds r6, #2
	cmp r7, #5
	ble .L_02008234
.L_0200825a:
	ldr r1, .L_020082c0
	ldr r0, .L_020082c4
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02008288
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	ldr r2, .L_020082bc
	adds r3, r3, r1
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, .L_020082a8
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008288:
	strh r4, [r0]
	ldr r2, .L_020082a4
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_02008292:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020082a0:
	.4byte gMainRoutineWork
.L_020082a4:
	.4byte gData_020036c4
.L_020082a8:
	.4byte 0x050001a0
.L_020082ac:
	.4byte IwramCopyWords
.L_020082b0:
	.4byte Data_020039bc
.L_020082b4:
	.4byte gData_0200393c
.L_020082b8:
	.4byte 0x05000100
.L_020082bc:
	.4byte gData_0200397c
.L_020082c0:
	.4byte gIoWriteQueue
.L_020082c4:
	.4byte 0x04000208
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
	.section .text.x02008934,"ax",%progbits
	.balign 4
	.global Func_02000934
	.thumb_func
Func_02000934:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r6, #1
	sub sp, #8
	negs r6, r6
	bl Func_02003158
	ldr r1, .L_0200897c
	cmp r5, #8
	bne .L_0200894a
	movs r6, #20
.L_0200894a:
	cmp r5, #10
	bne .L_02008950
	movs r6, #22
.L_02008950:
	cmp r5, #12
	bne .L_02008956
	movs r6, #24
.L_02008956:
	cmp r5, #14
	bne .L_0200895c
	movs r6, #26
.L_0200895c:
	movs r2, #1
	negs r2, r2
	cmp r6, r2
	bne .L_0200898c
	ldr r2, .L_02008978
	ldr r3, .L_02008980
	strh r2, [r1]
	strh r2, [r3]
	ldr r3, .L_02008984
	strh r6, [r3]
	ldr r3, .L_02008988
	strh r6, [r3]
	b .L_02008a2c
	.2byte 0x0000
.L_02008978:
	.4byte 0x00000000
.L_0200897c:
	.4byte Data_02003a7c
.L_02008980:
	.4byte gData_02003a7e
.L_02008984:
	.4byte gData_02003a80
.L_02008988:
	.4byte gData_02003a82
.L_0200898c:
	ldr r3, .L_020089c4
	ldr r2, .L_020089cc
	strh r3, [r1]
	ldr r3, .L_020089c8
	ldr r5, .L_020089d0
	strh r3, [r2]
	adds r3, r6, #0
	subs r3, #20
	lsrs r2, r3, #31
	adds r3, r3, r2
	ldr r2, .L_020089d4
	asrs r3, r3, #1
	strh r3, [r5]
	strh r6, [r2]
	cmp r3, #0
	bne .L_020089d8
	movs r3, #17
	movs r2, #67
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
	b .L_020089d8
	.2byte 0x0000
.L_020089c4:
	.4byte 0x0000000f
.L_020089c8:
	.4byte 0x00000010
.L_020089cc:
	.4byte gData_02003a7e
.L_020089d0:
	.4byte gData_02003a80
.L_020089d4:
	.4byte gData_02003a82
.L_020089d8:
	movs r6, #0
	ldrsh r3, [r5, r6]
	cmp r3, #1
	bne .L_020089f4
	movs r3, #8
	movs r2, #72
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_020089f4:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #2
	bne .L_02008a10
	movs r3, #17
	movs r2, #72
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_02008a10:
	movs r6, #0
	ldrsh r3, [r5, r6]
	cmp r3, #3
	bne .L_02008a2c
	movs r3, #8
	movs r2, #67
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_02008a2c:
	ldr r4, .L_02008aac
	ldr r0, .L_02008ab0
	ldrh r3, [r0]
	adds r5, r3, #0
	strh r0, [r0]
	ldrh r3, [r4]
	cmp r3, #31
	bgt .L_02008a68
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r4]
	ldr r3, .L_02008ab4
	lsls r2, r2, #2
	movs r6, #0
	ldrsh r1, [r3, r6]
	movs r3, #16
	subs r3, r3, r1
	lsls r3, r3, #8
	adds r2, r2, r4
	adds r2, #4
	orrs r1, r3
	stmia r2!, {r1}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_02008a68:
	strh r5, [r0]
	ldrh r3, [r0]
	adds r1, r3, #0
	strh r0, [r0]
	ldrh r2, [r4]
	cmp r2, #31
	bgt .L_02008a9a
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r4]
	movs r2, #252
	adds r3, r3, r4
	lsls r2, r2, #6
	adds r3, #4
	adds r2, #66
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02008a9a:
	strh r1, [r0]
	ldr r0, .L_02008ab8
	movs r1, #144
	lsls r1, r1, #3
	bl Task_Add
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008aac:
	.4byte gIoWriteQueue
.L_02008ab0:
	.4byte 0x04000208
.L_02008ab4:
	.4byte Data_02003a7c
.L_02008ab8:
	.4byte Func_0200055c
	.section .text.x02008b64,"ax",%progbits
	.global Func_02000b64
	.thumb_func
Func_02000b64:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r2, .L_02008cdc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r6, [r3, r1]
	movs r1, #241
	lsls r1, r1, #1
	movs r7, #192
	adds r3, r2, r1
	lsls r7, r7, #18
	movs r2, #0
	ldrsh r5, [r3, r2]
	ldr r3, [r7, #108]
	subs r1, #54
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #1
	movs r0, #137
	str r2, [r3]
	lsls r0, r0, #1
	sub sp, #8
	bl GameFlag_SetBit
	ldr r3, .L_02008ce0
	ldr r0, .L_02008ce4
	ldr r1, .L_02008ce8
	ldr r2, .L_02008cec
	bl Func_0200154c
	ldr r3, .L_02008cf0
	cmp r6, r3
	bne .L_02008bb0
	b .L_02008e76
.L_02008bb0:
	ldr r3, .L_02008cf4
	cmp r6, r3
	beq .L_02008bb8
	b .L_02008e76
.L_02008bb8:
	movs r0, #0
	bl Func_02003278
	ldr r3, [r7, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	subs r3, r5, #1
	cmp r3, #1
	bls .L_02008be2
	cmp r5, #8
	beq .L_02008be2
	cmp r5, #10
	beq .L_02008be2
	cmp r5, #12
	beq .L_02008be2
	cmp r5, #14
	beq .L_02008be2
	b .L_02008e76
.L_02008be2:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #49
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008bf4
	b .L_02008e76
.L_02008bf4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008c04
	b .L_02008e76
.L_02008c04:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #51
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008c14
	b .L_02008e76
.L_02008c14:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #52
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008c24
	b .L_02008e76
.L_02008c24:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #53
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008c34
	b .L_02008e76
.L_02008c34:
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #78
	bl Func_020032b8
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02003218
	movs r0, #224
	movs r1, #128
	movs r2, #216
	lsls r2, r2, #16
	movs r3, #1
	lsls r1, r1, #15
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	bl Func_02003228
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003250
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl wait
	ldr r5, .L_02008cf8
	ldr r3, .L_02008cd8
	movs r0, #1
	strh r3, [r5]
	bl wait
	movs r2, #4
	ldr r0, .L_02008cfc
	movs r1, #0
	bl Func_020031c0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl wait
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02003218
	movs r0, #224
	movs r1, #128
	movs r2, #216
	b .L_02008d00
	.2byte 0x0000
.L_02008cd8:
	.4byte 0x00007fff
.L_02008cdc:
	.4byte gMainRoutineWork
.L_02008ce0:
	.4byte gData_020033f4
.L_02008ce4:
	.4byte gData_0200338c
.L_02008ce8:
	.4byte gData_0200339c
.L_02008cec:
	.4byte gData_020033c8
.L_02008cf0:
	.4byte 0x00000128
.L_02008cf4:
	.4byte 0x0000011f
.L_02008cf8:
	.4byte 0x0500021e
.L_02008cfc:
	.4byte MsgYouHaveProven
.L_02008d00:
	lsls r1, r1, #13
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	bl Func_02003228
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #16
	bl Actor_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #85
	strb r6, [r3]
	movs r3, #128
	adds r1, r2, #0
	lsls r3, r3, #13
	str r3, [r2, #12]
	str r3, [r2, #20]
	adds r1, #98
	movs r3, #18
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	adds r1, #4
	strh r3, [r1]
	str r6, [r2, #104]
	bl Func_02002e6c
	movs r0, #40
	bl wait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02003198
	movs r3, #13
	str r3, [sp, #0]
	str r3, [sp, #4]
	mov r8, r3
	movs r0, #216
	movs r1, #216
	movs r2, #65
	movs r3, #65
	bl Func_02001324
	mov r1, r8
	str r1, [sp, #4]
	movs r6, #14
	movs r0, #232
	movs r1, #216
	movs r2, #66
	movs r3, #65
	str r6, [sp, #0]
	bl Func_02001324
	movs r0, #232
	movs r1, #232
	movs r2, #66
	movs r3, #66
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Func_02001324
	mov r2, r8
	str r2, [sp, #0]
	movs r5, #12
	movs r0, #216
	movs r1, #200
	movs r2, #65
	movs r3, #64
	str r5, [sp, #4]
	bl Func_02001324
	movs r3, #15
	str r3, [sp, #0]
	mov r10, r3
	movs r0, #248
	movs r1, #200
	movs r2, #67
	movs r3, #64
	str r5, [sp, #4]
	bl Func_02001324
	mov r1, r10
	str r1, [sp, #0]
	movs r0, #248
	movs r1, #232
	movs r2, #67
	movs r3, #66
	str r6, [sp, #4]
	bl Func_02001324
	mov r2, r8
	str r2, [sp, #0]
	movs r0, #216
	movs r1, #232
	movs r2, #65
	movs r3, #66
	str r6, [sp, #4]
	bl Func_02001324
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #200
	movs r1, #216
	movs r2, #64
	movs r3, #65
	str r5, [sp, #0]
	bl Func_02001324
	movs r0, #232
	movs r1, #200
	movs r2, #66
	movs r3, #64
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001324
	mov r1, r10
	mov r2, r8
	str r1, [sp, #0]
	str r2, [sp, #4]
	movs r0, #248
	movs r1, #216
	movs r2, #67
	movs r3, #65
	bl Func_02001324
	movs r0, #200
	movs r1, #232
	movs r2, #64
	movs r3, #66
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02001324
	movs r3, #64
	movs r0, #200
	movs r1, #200
	movs r2, #64
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001324
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02003198
	movs r1, #12
	movs r2, #4
	movs r3, #3
	movs r0, #30
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003170
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #49
	bl GameFlag_SetBit
	movs r0, #80
	bl Func_020032b8
	bl Sound_WaitForBgmRestore
	bl Func_02003290
	bl Func_020031e8
.L_02008e76:
	movs r0, #0
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008e84,"ax",%progbits
	.global Func_02000e84
	.thumb_func
Func_02000e84:
	push {r5, r6, lr}
	ldr r2, .L_020090b8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r5, [r3, r1]
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, .L_020090bc
	sub sp, #12
	cmp r5, r3
	beq .L_02008ebc
	ldr r3, .L_020090c0
	cmp r5, r3
	beq .L_02008ebc
	ldr r3, .L_020090c4
	cmp r5, r3
	beq .L_02008ebc
	ldr r3, .L_020090c8
	cmp r5, r3
	beq .L_02008ebc
	ldr r3, .L_020090cc
	cmp r5, r3
	bne .L_02008ee2
.L_02008ebc:
	movs r0, #1
	bl Func_020032b0
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #8
	lsls r2, r2, #7
	movs r1, #128
	str r3, [sp, #4]
	str r2, [sp, #8]
	movs r3, #128
	movs r2, #128
	lsls r1, r1, #9
	movs r0, #1
	lsls r2, r2, #11
	lsls r3, r3, #10
	str r1, [sp, #0]
	bl Func_02003270
.L_02008ee2:
	ldr r3, .L_020090bc
	cmp r5, r3
	bne .L_02008fa6
	cmp r6, #2
	beq .L_02008f22
	cmp r6, #2
	bgt .L_02008ef6
	cmp r6, #1
	beq .L_02008f00
	b .L_02008f86
.L_02008ef6:
	cmp r6, #3
	beq .L_02008f44
	cmp r6, #4
	beq .L_02008f66
	b .L_02008f86
.L_02008f00:
	movs r0, #8
	bl Actor_GetById
	movs r1, #1
	bl Actor_SetPartAttribute
	movs r1, #4
	movs r0, #9
	bl Func_02000058
	movs r0, #4
	bl Func_02003150
	movs r0, #5
	bl Func_02003150
	b .L_02008f86
.L_02008f22:
	movs r0, #8
	bl Actor_GetById
	movs r1, #3
	bl Actor_SetPartAttribute
	movs r1, #5
	movs r0, #9
	bl Func_02000058
	movs r0, #6
	bl Func_02003150
	movs r0, #7
	bl Func_02003150
	b .L_02008f86
.L_02008f44:
	movs r0, #8
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetPartAttribute
	movs r1, #2
	movs r0, #9
	bl Func_02000058
	movs r0, #0
	bl Func_02003150
	movs r0, #1
	bl Func_02003150
	b .L_02008f86
.L_02008f66:
	movs r0, #8
	bl Actor_GetById
	movs r1, #4
	bl Actor_SetPartAttribute
	movs r1, #3
	movs r0, #9
	bl Func_02000058
	movs r0, #2
	bl Func_02003150
	movs r0, #3
	bl Func_02003150
.L_02008f86:
	movs r0, #10
	bl Actor_GetById
	adds r2, r0, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r3, #208
	lsls r3, r3, #15
	movs r1, #144
	str r3, [r0, #16]
	lsls r1, r1, #3
	ldr r0, .L_020090d0
	bl Task_Add
	b .L_02009088
.L_02008fa6:
	ldr r3, .L_020090d4
	cmp r5, r3
	bne .L_02009088
	subs r3, r6, #1
	cmp r3, #1
	bls .L_02008fc2
	cmp r6, #8
	beq .L_02008fc2
	cmp r6, #10
	beq .L_02008fc2
	cmp r6, #12
	beq .L_02008fc2
	cmp r6, #14
	bne .L_02009088
.L_02008fc2:
	bl Func_02002118
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #49
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ff8
	movs r5, #12
	movs r0, #30
	movs r1, #12
	movs r2, #4
	movs r3, #3
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003170
	movs r3, #76
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #64
	movs r2, #4
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02003178
.L_02008ff8:
	adds r0, r6, #0
	bl Func_02000934
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020090d8
	bl Task_Add
	movs r1, #4
	movs r0, #12
	bl Func_02000058
	movs r1, #5
	movs r0, #13
	bl Func_02000058
	movs r1, #2
	movs r0, #14
	bl Func_02000058
	movs r0, #15
	movs r1, #3
	bl Func_02000058
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009040
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_020031f8
.L_02009040:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #51
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009058
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_020031f8
.L_02009058:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #52
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009070
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_020031f8
.L_02009070:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #53
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009088
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_020031f8
.L_02009088:
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090b0
	ldr r3, .L_020090b8
	movs r1, #253
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r3, .L_020090dc
	ldr r2, .L_020090e0
	movs r1, #160
	subs r3, r3, r2
	adds r0, r0, r3
	lsls r1, r1, #19
	bl Func_020031b8
.L_020090b0:
	movs r0, #0
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020090b8:
	.4byte gMainRoutineWork
.L_020090bc:
	.4byte 0x00000128
.L_020090c0:
	.4byte 0x00000120
.L_020090c4:
	.4byte 0x00000121
.L_020090c8:
	.4byte 0x00000122
.L_020090cc:
	.4byte 0x00000123
.L_020090d0:
	.4byte Func_020000c0
.L_020090d4:
	.4byte 0x0000011f
.L_020090d8:
	.4byte Func_020003c4
.L_020090dc:
	.4byte 0x00000121
.L_020090e0:
	.4byte 0x0000010e
	.section .text.x0200954c,"ax",%progbits
	.balign 4
	.global Func_0200154c
	.thumb_func
Func_0200154c:
	push {r5, r6, lr}
	adds r4, r1, #0
	movs r1, #192
	lsls r1, r1, #18
	adds r1, #128
	ldr r6, [r1]
	ldr r1, .L_020095f0
	adds r5, r0, #0
	str r5, [r1]
	ldr r1, .L_020095f4
	str r4, [r1]
	ldr r1, .L_020095f8
	str r2, [r1]
	ldr r2, .L_020095fc
	str r3, [r2]
	movs r2, #255
	ldrh r3, [r5]
	b .L_02009596
.L_02009570:
	ldrh r0, [r4]
	adds r4, #2
	ldrh r2, [r4]
	adds r4, #2
	ldrh r1, [r5]
	ldrh r3, [r4]
	adds r5, #2
	adds r4, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	movs r2, #160
	lsls r2, r2, #19
	lsls r1, r1, #1
	orrs r3, r0
	adds r1, r1, r2
	strh r3, [r1]
	ldrh r3, [r5]
	movs r2, #255
.L_02009596:
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_020095a4
	ldrh r3, [r4]
	cmp r3, r2
	bne .L_02009570
.L_020095a4:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r0, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r0, r0, #19
	adds r1, r6, #0
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r6, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, .L_02009600
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003248
	ldr r3, .L_02009604
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_020095ee
	bl Func_0200183c
.L_020095ee:
	pop {r5, r6, pc}
.L_020095f0:
	.4byte gData_02003928
.L_020095f4:
	.4byte Data_0200392c
.L_020095f8:
	.4byte Data_02003930
.L_020095fc:
	.4byte Data_0200391c
.L_02009600:
	.4byte 0x05000200
.L_02009604:
	.4byte gMainRoutineWork
	.section .text.x020096e8,"ax",%progbits
	.balign 4
	.global Func_020016e8
	.thumb_func
Func_020016e8:
	push {lr}
	ldr r1, .L_02009700
	ldrh r3, [r0]
	movs r2, #0
	cmp r3, r1
	beq .L_02009704
.L_020096f4:
	adds r0, #2
	ldrh r3, [r0]
	adds r2, #1
	cmp r3, r1
	bne .L_020096f4
	b .L_02009704
.L_02009700:
	.4byte 0x0000ffff
.L_02009704:
	subs r2, #1
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x0200970c,"ax",%progbits
	.global Func_0200170c
	.thumb_func
Func_0200170c:
	push {r5, r6, r7, lr}
	ldr r1, .L_020097bc
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_02009746
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02009746
	ldr r0, .L_020097c0
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_02009746
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_02009746:
	cmp r4, #0
	bne .L_0200974c
	b .L_02009838
.L_0200974c:
	ldr r3, .L_020097c4
	ldr r6, .L_020097c8
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_0200979a
	ldr r3, .L_020097cc
	ldr r2, .L_020097d0
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_02009764:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r3, [r6]
	movs r0, #160
	muls r3, r2
	adds r3, r3, r5
	lsls r3, r3, #1
	ldrh r3, [r3, r7]
	lsls r0, r0, #19
	lsls r3, r3, #1
	adds r4, r3, r0
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	adds r1, #2
	ldrh r3, [r1]
	adds r1, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	adds r5, #1
	mov r2, r12
	ldrh r3, [r2]
	cmp r5, r3
	bcc .L_02009764
.L_0200979a:
	ldr r3, .L_020097c8
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_020097d4
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_020097d8
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_020097e0
	ldr r3, .L_020097dc
	b .L_020097e2
.L_020097bc:
	.4byte gData_02003920
.L_020097c0:
	.4byte gData_02003924
.L_020097c4:
	.4byte Data_02003930
.L_020097c8:
	.4byte gData_02003934
.L_020097cc:
	.4byte Data_0200391c
.L_020097d0:
	.4byte gData_02003938
.L_020097d4:
	.4byte gData_02003928
.L_020097d8:
	.4byte gData_02003918
.L_020097dc:
	.4byte Data_0200392c
.L_020097e0:
	ldr r3, .L_02009828
.L_020097e2:
	lsls r2, r2, #1
	ldr r3, [r3]
	adds r1, r3, r2
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	ldrh r3, [r1, #2]
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	ldr r1, .L_0200982c
	ldr r2, .L_02009820
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_02009830
	ldr r2, .L_02009834
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_02009838
	ldr r3, .L_02009824
	strh r3, [r1]
	b .L_02009838
	.2byte 0x0000
.L_02009820:
	.4byte 0x00000001
.L_02009824:
	.4byte 0x00000000
.L_02009828:
	.4byte Data_02003930
.L_0200982c:
	.4byte gData_02003918
.L_02009830:
	.4byte gData_02003938
.L_02009834:
	.4byte gData_02003934
.L_02009838:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009988,"ax",%progbits
	.global Func_02001988
	.thumb_func
Func_02001988:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009ae4
	ldr r2, .L_02009ae8
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
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
	bge .L_020099e4
	adds r3, #15
.L_020099e4:
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
	bne .L_02009a76
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_020018c4
	cmp r0, #0
	beq .L_02009a76
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r2, [r3]
	cmp r2, #0
	bne .L_02009a76
	ldr r1, [r6, #76]
	cmp r1, #0
	beq .L_02009a4a
	mov r3, r8
	adds r3, #104
	strh r2, [r3]
	mov r2, r8
	adds r2, #106
	cmp r1, #0
	ble .L_02009a42
	movs r3, #1
	b .L_02009a48
.L_02009a42:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_02009a48:
	strh r3, [r2]
.L_02009a4a:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_02009a76:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_02009ad6
	mov r5, r8
	adds r5, #84
.L_02009a86:
	bl Actor_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009ac8
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001928
	cmp r0, #0
	beq .L_02009ac8
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009ac2
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_02009ac8
.L_02009ac2:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_02009ac8:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_02009ad6
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_02009a86
.L_02009ad6:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009ae4:
	.4byte gMainRoutineWork
.L_02009ae8:
	.4byte gData_020023c4 + 0x188
	.section .text.x02009b64,"ax",%progbits
	.global Func_02001b64
	.thumb_func
Func_02001b64:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009cbc
	ldr r2, .L_02009cc0
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #16]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #16]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #8]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #8]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_02009bc0
	adds r3, #15
.L_02009bc0:
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
	bne .L_02009c50
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_020018c4
	cmp r0, #0
	beq .L_02009c50
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02009c50
	ldr r2, [r6, #76]
	cmp r2, #0
	beq .L_02009c24
	mov r1, r8
	adds r1, #106
	strh r3, [r1]
	subs r1, #2
	cmp r2, #0
	ble .L_02009c1c
	movs r3, #1
	b .L_02009c22
.L_02009c1c:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_02009c22:
	strh r3, [r1]
.L_02009c24:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r3, [r6, #16]
	ldr r2, [r6, #68]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_02009c50:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_02009cb0
	mov r5, r8
	adds r5, #84
.L_02009c60:
	bl Actor_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009ca2
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001928
	cmp r0, #0
	beq .L_02009ca2
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009c9c
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_02009ca2
.L_02009c9c:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_02009ca2:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_02009cb0
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_02009c60
.L_02009cb0:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009cbc:
	.4byte gMainRoutineWork
.L_02009cc0:
	.4byte gData_020023c4 + 0x188
	.section .text.x02009f94,"ax",%progbits
	.global Func_02001f94
	.thumb_func
Func_02001f94:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_0200a110
	sub sp, #8
	mov r8, r0
	movs r0, #192
	lsls r0, r0, #18
	ldr r1, [r0, #32]
	mov r7, r8
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	ldr r2, [r2, #4]
	mov r9, r3
	ldr r3, .L_0200a114
	mov r4, r9
	ands r4, r3
	ands r2, r3
	ldr r3, [r1]
	mov r9, r4
	ldr r3, [r3, #4]
	adds r7, #20
	str r3, [sp, #4]
	mov r10, r2
	ldr r0, [r0, #108]
	str r0, [sp, #0]
	mov r0, r8
	movs r4, #6
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .L_02009fee
	movs r1, #8
	ldrsh r3, [r0, r1]
	cmp r3, #0
	bne .L_02009fee
	ldr r3, [r0, #16]
	cmp r3, #0
	beq .L_02009fee
	mov lr, r3
	.2byte 0xf800
.L_02009fee:
	mov r2, r8
	ldrh r3, [r2, #6]
	mov r4, r8
	movs r2, #0
	mov r0, r8
	strh r3, [r4, #8]
	strh r2, [r0, #6]
	movs r1, #3
	mov r11, r1
.L_0200a000:
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #0
	beq .L_0200a0f0
	ldr r5, [r7, #8]
	cmp r5, #0
	beq .L_0200a0f0
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	ldr r4, [sp, #0]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r4, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a02a
	movs r3, #1
	orrs r0, r3
.L_0200a02a:
	adds r6, r5, #0
	adds r6, #91
	strb r0, [r6]
	mov r0, r8
	movs r4, #14
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .L_0200a044
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	strb r0, [r6]
.L_0200a044:
	ldr r2, [r5, #8]
	mov r1, r9
	ldr r3, [r5, #16]
	subs r2, r2, r1
	ldr r1, [r5, #12]
	mov r4, r10
	subs r3, r3, r4
	subs r1, r3, r1
	movs r0, #6
	ldrsh r4, [r7, r0]
	asrs r3, r1, #16
	adds r1, r3, #0
	asrs r2, r2, #16
	subs r1, #8
	cmp r4, #0
	bne .L_0200a07a
	adds r3, r2, #7
	movs r2, #167
	lsls r2, r2, #1
	cmp r3, r2
	bhi .L_0200a0f0
	movs r3, #48
	negs r3, r3
	cmp r1, r3
	ble .L_0200a0f0
	cmp r1, #239
	bgt .L_0200a0f0
.L_0200a07a:
	movs r0, #2
	ldrsh r3, [r7, r0]
	ldrh r1, [r7, #2]
	cmp r3, #0
	bgt .L_0200a0ec
	ldrh r3, [r7, #4]
	movs r1, #240
	ands r1, r3
	cmp r1, #32
	beq .L_0200a0c0
	cmp r1, #32
	bgt .L_0200a09c
	cmp r1, #0
	beq .L_0200a0dc
	cmp r1, #16
	beq .L_0200a0ce
	b .L_0200a0e8
.L_0200a09c:
	cmp r1, #48
	beq .L_0200a0b2
	cmp r1, #128
	bne .L_0200a0e8
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001f14
	b .L_0200a0e8
.L_0200a0b2:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001d08
	b .L_0200a0e8
.L_0200a0c0:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001d98
	b .L_0200a0e8
.L_0200a0ce:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001d50
	b .L_0200a0e8
.L_0200a0dc:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001b1c
.L_0200a0e8:
	movs r3, #8
	b .L_0200a0ee
.L_0200a0ec:
	subs r3, r1, #1
.L_0200a0ee:
	strh r3, [r7, #2]
.L_0200a0f0:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	adds r7, #16
	cmp r2, #0
	blt .L_0200a100
	b .L_0200a000
.L_0200a100:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a110:
	.4byte gData_020023c4 + 0x188
.L_0200a114:
	.4byte 0xffff0000
	.section .text.x0200a18c,"ax",%progbits
	.global Func_0200218c
	.thumb_func
Func_0200218c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r1
	ldr r1, .L_0200a2b8
	sub sp, #16
	adds r6, r0, #0
	movs r0, #10
	str r1, [sp, #4]
	adds r0, #255
	adds r1, #20
	str r2, [sp, #12]
	str r3, [sp, #8]
	mov r9, r1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a270
	ldrh r3, [r6]
	movs r2, #0
	mov r11, r2
	mov r10, r3
	adds r6, #2
	cmp r3, #0
	ble .L_0200a22e
.L_0200a1c6:
	ldrh r7, [r6]
	movs r1, #15
	ands r1, r7
	movs r3, #240
	mov r0, r10
	str r1, [sp, #0]
	ands r7, r3
	bl Actor_GetById
	adds r5, r0, #0
	adds r6, #2
	cmp r5, #0
	beq .L_0200a21a
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r3, r5, #0
	movs r2, #128
	adds r3, #98
	movs r1, #1
	ands r2, r7
	strb r1, [r3]
	cmp r2, #0
	bne .L_0200a1fa
	subs r3, #9
	strb r2, [r3]
.L_0200a1fa:
	mov r2, r9
	mov r3, r9
	strh r1, [r2]
	mov r0, r10
	strh r7, [r3, #4]
	bl Actor_GetById
	mov r1, r9
	str r0, [r1, #8]
	ldr r2, [sp, #0]
	lsls r3, r2, #16
	str r3, [r1, #12]
	mov r3, r11
	strh r3, [r1, #2]
	movs r2, #16
	add r9, r2
.L_0200a21a:
	movs r3, #1
	add r11, r3
	mov r1, r11
	cmp r1, #3
	bgt .L_0200a22e
	ldrh r2, [r6]
	adds r6, #2
	mov r10, r2
	cmp r2, #0
	bgt .L_0200a1c6
.L_0200a22e:
	mov r3, r8
	cmp r3, #0
	beq .L_0200a270
	movs r1, #0
	ldrh r2, [r3]
	mov r11, r1
	ldr r1, [sp, #4]
	movs r3, #2
	add r8, r3
	movs r3, #84
	strh r2, [r1, r3]
	cmp r2, #0
	ble .L_0200a270
	adds r2, r1, #0
	adds r2, #84
.L_0200a24c:
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #1
	strh r3, [r2, #2]
	add r11, r1
	movs r3, #2
	add r8, r3
	mov r3, r11
	adds r2, #4
	cmp r3, #3
	bgt .L_0200a270
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #2
	add r8, r1
	strh r3, [r2]
	cmp r3, #0
	bgt .L_0200a24c
.L_0200a270:
	ldr r2, [sp, #12]
	ldr r3, [sp, #4]
	add r1, sp, #8
	str r2, [r3, #16]
	ldrh r1, [r1]
	ldr r2, [sp, #4]
	strh r1, [r2, #10]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #80
	ldrh r3, [r1]
	cmp r3, #0
	bne .L_0200a2a0
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
.L_0200a2a0:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a2bc
	bl Task_Add
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a2b8:
	.4byte gData_020023c4 + 0x188
.L_0200a2bc:
	.4byte Func_02001f94
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
	.else
	.global Func_020024e4
	.thumb_func
Func_020024e4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200a6d8
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #16
	cmp r3, #0
	beq .L_0200a506
	b .L_0200a6c4
.L_0200a506:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02002368
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r2, .L_0200a6dc
	adds r6, r0, #0
	str r2, [sp, #0]
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	ldr r3, .L_0200a6e0
	adds r0, r6, #0
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	movs r1, #49
	bl Func_02003118
.L_0200a53e:
	adds r3, r6, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Func_02003168
	ldr r3, [sp, #0]
	ldr r1, [sp, #0]
	adds r3, #104
	adds r1, #106
	mov r9, r1
	mov r10, r3
	add r1, sp, #4
	cmp r0, #7
	bne .L_0200a5ba
	movs r2, #0
	ldrsh r3, [r3, r2]
	mov r1, r9
	lsls r3, r3, #17
	str r3, [r6, #36]
	movs r5, #0
	movs r0, #0
	ldrsh r3, [r1, r0]
	lsls r3, r3, #17
	str r3, [r6, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r6, #52]
.L_0200a578:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	add r1, sp, #4
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_02003180
	cmp r0, #0
	beq .L_0200a5ac
	movs r3, #0
	str r3, [r6, #36]
	str r3, [r6, #44]
	b .L_0200a6b8
.L_0200a5ac:
	movs r0, #1
	adds r5, #1
	bl wait
	cmp r5, #9
	ble .L_0200a578
	b .L_0200a6b8
.L_0200a5ba:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_02003180
	cmp r0, #0
	bgt .L_0200a6b8
	cmp r0, #0
	bge .L_0200a604
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	ldr r3, .L_0200a6d8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_02003200
	b .L_0200a6b8
.L_0200a604:
	ldrh r3, [r6, #32]
	movs r2, #0
	subs r3, #2
	mov r11, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	mov r8, r2
	adds r7, r5, #0
	adds r7, #89
.L_0200a618:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200a648
	ldrb r2, [r7]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200a648
	cmp r5, r6
	beq .L_0200a648
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	mov r1, r11
	add r2, sp, #4
	bl Func_020031b0
	cmp r0, #0
	blt .L_0200a648
	movs r0, #1
	bl wait
	b .L_0200a6b8
.L_0200a648:
	movs r3, #1
	add r8, r3
	mov r0, r8
	adds r7, #128
	adds r5, #128
	cmp r0, #63
	ble .L_0200a618
	mov r2, r10
	movs r1, #0
	ldrsh r3, [r2, r1]
	ldr r2, [r6, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r6, #8]
	mov r2, r9
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldr r2, [r6, #16]
	ldr r7, .L_0200a6e4
	lsls r3, r3, #17
	ldr r0, .L_0200a6e8
	adds r5, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r5, [r6, #16]
	cmp r2, r4
	ble .L_0200a686
	adds r2, r4, #0
.L_0200a686:
	ldr r0, .L_0200a6ec
	cmp r2, r0
	bge .L_0200a68e
	adds r2, r0, #0
.L_0200a68e:
	subs r3, r1, r2
	ldr r1, .L_0200a6e8
	str r3, [r6, #8]
	adds r3, r5, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_0200a6a0
	adds r2, r4, #0
.L_0200a6a0:
	cmp r2, r0
	bge .L_0200a6a6
	adds r2, r0, #0
.L_0200a6a6:
	subs r3, r5, r2
	str r3, [r6, #16]
	ldr r2, [sp, #0]
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_0200a53e
.L_0200a6b8:
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200a6c4:
	ldr r0, [sp, #0]
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a6d8:
	.4byte gMainRoutineWork
.L_0200a6dc:
	.4byte gData_020023c4 + 0x188
.L_0200a6e0:
	.4byte Func_020024bc
.L_0200a6e4:
	.4byte 0x000fffff
.L_0200a6e8:
	.4byte 0xfff80000
.L_0200a6ec:
	.4byte 0xffff0000
	.endif
	.section .text.x0200a708,"ax",%progbits
	.global Func_02002708
	.thumb_func
Func_02002708:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r1, #217
	lsls r1, r1, #1
	adds r6, r5, r1
	ldrh r3, [r6]
	sub sp, #12
	cmp r3, #0
	bne .L_0200a734
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r5, r2
	adds r1, #2
	ldr r0, [r3]
	adds r3, r5, r1
	ldr r1, [r3]
	bl Func_02003240
	movs r3, #1
	strh r3, [r6]
.L_0200a734:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r2, #179
	lsls r2, r2, #1
	movs r1, #173
	adds r3, r5, r2
	lsls r1, r1, #1
	movs r2, #0
	strh r2, [r3]
	adds r3, r5, r1
	adds r1, #4
	strh r2, [r3]
	adds r3, r5, r1
	subs r1, #2
	strh r2, [r3]
	adds r3, r5, r1
	adds r1, #8
	strh r2, [r3]
	movs r0, #10
	adds r3, r5, r1
	strh r2, [r3]
	movs r1, #0
	negs r0, r0
	bl Func_02002368
	movs r0, #224
	movs r1, #224
	lsls r1, r1, #8
	lsls r0, r0, #11
	bl Func_02003218
	ldr r3, .L_0200a838
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r6, r0, #0
	movs r0, #131
	lsls r0, r0, #1
	ldr r7, .L_0200a83c
	bl GameFlag_SetBit
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	ldr r3, .L_0200a840
	movs r1, #49
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r0, r6, #0
	bl Func_02003118
	ldr r3, [r7, #108]
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200a820
.L_0200a7c0:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #0
	bl Func_02003168
	ldr r1, [r7, #108]
	cmp r0, #7
	beq .L_0200a7e0
	ldr r2, [r1, #112]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r3, [r6, #16]
	ldr r2, [r1, #120]
	adds r3, r3, r2
	b .L_0200a808
.L_0200a7e0:
	ldr r3, [r1, #112]
	cmp r3, #0
	beq .L_0200a7f4
	ldr r3, [r6, #8]
	ldr r2, .L_0200a844
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #8]
.L_0200a7f4:
	ldr r3, [r7, #108]
	ldr r3, [r3, #120]
	cmp r3, #0
	beq .L_0200a80a
	ldr r3, [r6, #16]
	ldr r2, .L_0200a844
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #12
	adds r3, r3, r1
.L_0200a808:
	str r3, [r6, #16]
.L_0200a80a:
	movs r3, #0
	strh r3, [r7, #4]
	movs r0, #1
	bl wait
	ldr r3, [r7, #108]
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a7c0
.L_0200a820:
	movs r5, #0
	movs r0, #30
	bl FieldEvent_Wait
	adds r0, r6, #0
	str r5, [r6, #108]
	movs r1, #0
	bl Animation_ApplyChildValues
	strh r5, [r7, #4]
	add sp, #12
	pop {r5, r6, r7, pc}
.L_0200a838:
	.4byte gMainRoutineWork
.L_0200a83c:
	.4byte gData_020023c4 + 0x188
.L_0200a840:
	.4byte Func_020024bc
.L_0200a844:
	.4byte 0xfff00000
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
	.section .text.x0200aae8,"ax",%progbits
	.balign 4
	.global Func_02002ae8
	.thumb_func
Func_02002ae8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r3, #192
	movs r0, #100
	lsls r3, r3, #18
	adds r0, r0, r5
	ldr r6, [r3, #108]
	movs r1, #0
	ldrsh r3, [r0, r1]
	sub sp, #56
	mov r8, r0
	cmp r3, #0
	beq .L_0200ab10
	b .L_0200ad9c
.L_0200ab10:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_0200ab2a
	movs r3, #1
	orrs r0, r3
.L_0200ab2a:
	adds r3, r5, #0
	adds r3, #91
	strb r0, [r3]
	add r7, sp, #44
	ldr r3, [r5, #8]
	movs r0, #128
	str r3, [r7]
	lsls r0, r0, #12
	ldr r3, [r5, #12]
	adds r2, r7, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	str r3, [r7, #8]
	ldrh r1, [r5, #6]
	bl Vector_AddPolarOffsetFar
	ldr r1, [r7]
	ldr r2, [r7, #8]
	movs r0, #0
	bl Func_02003168
	cmp r0, #7
	bne .L_0200aba0
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #64]
	str r3, [r5, #60]
	str r3, [r5, #56]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	mov r0, r8
	movs r3, #1
	strh r3, [r0]
	movs r0, #145
	bl Func_020032b8
	movs r0, #160
	lsls r0, r0, #11
	movs r2, #128
	adds r1, r0, #0
	lsls r2, r2, #9
	bl Func_02003198
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02003198
	ldr r3, [r5, #104]
	cmp r3, #0
	beq .L_0200aba0
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
.L_0200aba0:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	ldr r3, [r5, #8]
	ldr r1, [r5, #16]
	asrs r3, r3, #20
	str r3, [sp, #32]
	movs r3, #184
	lsls r3, r3, #1
	ldr r4, [sp, #32]
	asrs r1, r1, #20
	adds r2, r0, r3
	ldr r2, [r2]
	lsls r3, r1, #7
	adds r3, r4, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	str r2, [sp, #28]
	movs r4, #212
	lsls r4, r4, #1
	adds r2, r0, r4
	ldr r2, [r2]
	subs r4, #92
	adds r2, r2, r3
	str r2, [sp, #24]
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r0, r2
	ldr r3, [r3]
	asrs r3, r3, #20
	str r3, [sp, #20]
	adds r3, r0, r4
	adds r4, #52
	ldr r2, [r3]
	adds r3, r0, r4
	ldr r3, [r3]
	adds r4, #4
	asrs r3, r3, #20
	str r3, [sp, #16]
	adds r3, r0, r4
	ldr r3, [r3]
	movs r0, #1
	asrs r3, r3, #20
	adds r3, r1, r3
	subs r3, #2
	asrs r2, r2, #20
	negs r0, r0
	str r3, [sp, #8]
	str r0, [sp, #40]
	subs r3, r1, #1
	adds r1, r1, r2
	subs r1, #1
	mov r8, r3
	mov r11, r1
.L_0200ac0c:
	ldr r0, [sp, #32]
	ldr r1, [sp, #16]
	ldr r2, [sp, #20]
	movs r4, #1
	adds r3, r0, r1
	subs r3, #1
	negs r4, r4
	mov r9, r3
	str r4, [sp, #36]
	adds r3, r0, r2
	adds r6, r0, #0
	subs r3, #1
	subs r6, #1
	mov r10, r3
.L_0200ac28:
	ldr r4, [sp, #40]
	ldr r0, [sp, #36]
	lsls r3, r4, #7
	adds r3, r3, r0
	lsls r3, r3, #2
	ldr r1, [sp, #28]
	str r3, [sp, #12]
	adds r2, r3, r1
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200ac88
	movs r3, #0
	strb r3, [r2, #2]
	mov r2, r10
	mov r3, r11
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #1
	bl Func_02003178
	mov r4, r8
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r4, [sp, #4]
	str r6, [sp, #0]
	bl Func_02003170
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020032b8
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_020029f4
.L_0200ac88:
	ldr r4, [sp, #12]
	ldr r0, [sp, #24]
	adds r2, r4, r0
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200acdc
	movs r3, #0
	strb r3, [r2, #2]
	ldr r2, [sp, #8]
	mov r1, r9
	str r1, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #2
	bl Func_02003178
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r6, [sp, #0]
	bl Func_02003170
	movs r0, #143
	lsls r0, r0, #2
	bl Func_020032b8
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_020029f4
.L_0200acdc:
	ldr r0, [sp, #36]
	movs r4, #1
	adds r0, #1
	add r9, r4
	adds r6, #1
	add r10, r4
	str r0, [sp, #36]
	cmp r0, #1
	ble .L_0200ac28
	ldr r1, [sp, #8]
	ldr r2, [sp, #40]
	adds r1, #1
	adds r2, #1
	str r1, [sp, #8]
	add r8, r4
	add r11, r4
	str r2, [sp, #40]
	cmp r2, #1
	ble .L_0200ac0c
	ldr r3, [r5, #24]
	movs r4, #128
	lsls r4, r4, #9
	cmp r3, r4
	bge .L_0200ad1a
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
.L_0200ad1a:
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r5, #0
	bl Func_02003140
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	ldr r3, .L_0200ade0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	ldr r6, .L_0200ade4
	bl Actor_GetById
	ldr r1, [r5, #8]
	ldr r3, [r0, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_0200ad50
	movs r1, #160
	lsls r1, r1, #13
	cmp r2, r1
	blt .L_0200ad5a
	b .L_0200add0
.L_0200ad50:
	movs r2, #160
	subs r3, r3, r1
	lsls r2, r2, #13
	cmp r3, r2
	bge .L_0200add0
.L_0200ad5a:
	ldr r3, [r5, #12]
	ldr r2, [r0, #12]
	ldr r4, .L_0200ade8
	ldr r1, .L_0200adec
	subs r3, r3, r2
	adds r3, r3, r4
	cmp r3, r1
	bhi .L_0200add0
	ldr r3, [r5, #16]
	ldr r0, [r0, #16]
	subs r2, r3, r0
	cmp r2, #0
	blt .L_0200ad7e
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	blt .L_0200ad88
	b .L_0200add0
.L_0200ad7e:
	movs r4, #160
	subs r3, r0, r3
	lsls r4, r4, #13
	cmp r3, r4
	bge .L_0200add0
.L_0200ad88:
	movs r3, #2
	strh r3, [r6, #4]
	ldrh r3, [r6, #10]
	movs r0, #170
	lsls r0, r0, #1
	adds r3, #2
	adds r2, r7, r0
	str r5, [r6, #108]
	strh r3, [r2]
	b .L_0200add0
.L_0200ad9c:
	cmp r3, #1
	bne .L_0200add0
	adds r3, r5, #0
	adds r3, #91
	movs r2, #0
	strb r2, [r3]
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_0200adc2
	ldr r2, .L_0200adf0
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
	bl Func_02002870
	b .L_0200add0
.L_0200adc2:
	str r2, [r5, #16]
	str r2, [r5, #12]
	str r2, [r5, #8]
	str r2, [r5, #44]
	str r2, [r5, #40]
	str r2, [r5, #36]
	str r2, [r5, #108]
.L_0200add0:
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ade0:
	.4byte gMainRoutineWork
.L_0200ade4:
	.4byte gData_020023c4 + 0x188
.L_0200ade8:
	.4byte 0x0007ffff
.L_0200adec:
	.4byte 0x001ffffe
.L_0200adf0:
	.4byte 0xfffff000
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
