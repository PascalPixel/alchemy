.syntax unified
	.thumb
	.section .text.x020081a4,"ax",%progbits
	.global Func_020001a4
	.thumb_func
Func_020001a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020081c6
	ldr r3, .L_020081e8
	movs r1, #3
	ldr r0, [r3]
	bl __umodsi3
	cmp r0, #0
	bne .L_0200828c
.L_020081c6:
	movs r0, #23
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081ec
	bl Random16Far
	adds r2, r0, #0
	ldr r3, [r5, #12]
	lsls r2, r2, #8
	b .L_020081f6
.L_020081e8:
	.4byte gFrameCount
.L_020081ec:
	bl Random16Far
	adds r2, r0, #0
	ldr r3, [r5, #12]
	lsls r2, r2, #6
.L_020081f6:
	lsrs r2, r2, #16
	lsls r2, r2, #16
	adds r2, r2, r3
	ldr r3, .L_0200827c
	movs r0, #168
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	lsls r0, r0, #2
	bl Func_02002828
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200828c
	ldr r1, .L_02008280
	adds r0, r7, #0
	ldr r6, [r7, #80]
	bl Func_02002820
	movs r1, #1
	adds r0, r7, #0
	bl Actor_SetPartAttribute
	adds r3, r7, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	bl Random16Far
	ldr r3, .L_02008284
	adds r2, r7, #0
	adds r2, #100
	ands r3, r0
	strh r3, [r2]
	adds r3, r7, #0
	adds r3, #102
	strh r5, [r3]
	ldr r3, .L_02008288
	ldr r1, .L_02008278
	str r3, [r7, #108]
	mov r8, r1
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #16
	subs r0, r0, r3
	lsrs r0, r0, #20
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r7, #48]
	mov r3, r8
	ldrb r2, [r6, #9]
	strb r3, [r6, #26]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #9]
	b .L_0200828c
	.2byte 0x0000
.L_02008278:
	.4byte 0x00000000
.L_0200827c:
	.4byte 0xffe40000
.L_02008280:
	.4byte gData_02003214
.L_02008284:
	.4byte 0x0ffff000
.L_02008288:
	.4byte Func_02000148
.L_0200828c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008cb6,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009538,"ax",%progbits
	.global Func_02001538
	.thumb_func
Func_02001538:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #16
	mov r10, r3
	movs r2, #232
	movs r3, #95
	movs r7, #128
	lsls r2, r2, #4
	str r3, [sp, #0]
	lsls r7, r7, #3
	add r2, r10
	add r7, r10
	mov r11, r2
.L_02009564:
	ldr r2, [r7, #20]
	ldr r0, [r7, #24]
	mov r9, r2
	cmp r0, #0
	blt .L_02009632
	cmp r0, r9
	bgt .L_02009632
	mov r1, r9
	lsls r0, r0, #15
	bl __divsi3
	bl Math_Sine
	movs r3, #232
	movs r2, #236
	lsls r3, r3, #5
	lsls r2, r2, #5
	adds r3, #140
	add r2, r10
	add r3, r10
	ldr r5, [r2]
	ldr r3, [r3]
	ldr r2, [r7, #24]
	subs r3, r3, r5
	mov r8, r0
	mov r1, r9
	adds r0, r2, #0
	muls r0, r3
	add r6, sp, #4
	bl __divsi3
	ldr r3, [r7, #12]
	adds r5, r5, r0
	mov r2, r8
	muls r2, r3
	adds r3, r2, #0
	adds r5, r5, r3
	str r5, [r6]
	ldr r0, [r7, #24]
	mov r1, r9
	lsls r0, r0, #15
	bl __divsi3
	bl Math_Sine
	movs r2, #232
	movs r3, #232
	lsls r2, r2, #5
	lsls r3, r3, #5
	adds r2, #132
	adds r3, #144
	add r2, r10
	add r3, r10
	ldr r5, [r2]
	ldr r3, [r3]
	ldr r2, [r7, #24]
	subs r3, r3, r5
	mov r8, r0
	mov r1, r9
	adds r0, r2, #0
	muls r0, r3
	bl __divsi3
	ldr r3, [r7, #16]
	adds r5, r5, r0
	mov r2, r8
	muls r2, r3
	adds r3, r2, #0
	adds r5, r5, r3
	movs r2, #232
	movs r3, #232
	str r5, [r6, #4]
	lsls r2, r2, #5
	lsls r3, r3, #5
	adds r2, #136
	adds r3, #148
	add r2, r10
	add r3, r10
	ldr r5, [r2]
	ldr r3, [r3]
	ldr r2, [r7, #24]
	subs r3, r3, r5
	adds r0, r2, #0
	muls r0, r3
	mov r1, r9
	bl __divsi3
	adds r5, r5, r0
	adds r0, r6, #0
	str r5, [r6, #8]
	bl Func_020029b0
	ldr r3, [r6]
	mov r2, r11
	str r3, [r2, #12]
	mov r0, r11
	ldr r3, [r6, #8]
	str r3, [r2, #16]
	bl Func_020029e8
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_02009632:
	ldr r2, [sp, #0]
	movs r3, #40
	subs r2, #1
	add r11, r3
	str r2, [sp, #0]
	adds r7, #28
	cmp r2, #0
	bge .L_02009564
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x02009718,"ax",%progbits
	.global Func_02001718
	.thumb_func
Func_02001718:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	movs r1, #237
	adds r5, r0, #0
	lsls r1, r1, #5
	movs r0, #220
	sub sp, #4
	bl HeapSlot_AllocFar
	ldr r3, .L_02009850
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r7, r0, #0
	ldr r0, [r3]
	bl Actor_GetById
	movs r3, #236
	lsls r3, r3, #5
	adds r2, r7, r3
	ldr r3, [r5]
	movs r1, #232
	str r3, [r2]
	lsls r1, r1, #5
	ldr r3, [r5, #4]
	adds r1, #132
	adds r2, r7, r1
	str r3, [r2]
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #136
	adds r2, r7, r3
	ldr r3, [r5, #8]
	adds r1, #8
	str r3, [r2]
	adds r2, r7, r1
	ldr r3, [r6]
	adds r1, #8
	str r3, [r2]
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #144
	adds r2, r7, r3
	ldr r3, [r6, #4]
	mov r11, r0
	str r3, [r2]
	adds r2, r7, r1
	ldr r3, [r6, #8]
	ldr r0, .L_02009854
	str r3, [r2]
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_020027b8
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #3
	adds r2, r7, #0
	adds r5, r0, #0
	bl VramCache_Load
	movs r2, #232
	lsls r2, r2, #5
	adds r2, #154
	mov r9, r0
	adds r3, r7, r2
	mov r1, r9
	strh r1, [r3]
	adds r2, #2
	movs r1, #128
	adds r3, r7, r2
	lsls r1, r1, #3
	movs r2, #232
	strh r5, [r3]
	adds r1, r1, r7
	movs r3, #0
	lsls r2, r2, #4
	mov r10, r3
	mov r8, r1
	adds r6, r7, r2
.L_020097c8:
	movs r5, #15
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #1
	add r3, r9
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #4
	movs r2, #4
	movs r3, #0
	bl Func_020029e0
	ldrb r3, [r6, #5]
	movs r2, #32
	orrs r3, r2
	strb r3, [r6, #5]
	ldrb r3, [r6, #9]
	mov r0, r11
	ands r5, r3
	strb r5, [r6, #9]
	bl Func_020029f0
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	lsls r0, r0, #2
	orrs r3, r0
	strb r3, [r6, #9]
	mov r0, r11
	bl Func_020029f8
	movs r3, #1
	mov r2, r8
	negs r3, r3
	subs r0, #1
	strh r0, [r6, #30]
	str r3, [r2, #24]
	movs r3, #1
	add r10, r3
	movs r1, #28
	mov r2, r10
	adds r6, #40
	add r8, r1
	cmp r2, #95
	ble .L_020097c8
	movs r1, #232
	lsls r1, r1, #5
	adds r1, #152
	adds r2, r7, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_02009858
	bl Task_Add
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009850:
	.4byte gMainRoutineWork
.L_02009854:
	.4byte 0x000001ef
.L_02009858:
	.4byte Func_02001538
	.section .text.x02009888,"ax",%progbits
	.balign 4
	.global Func_02001888
	.thumb_func
Func_02001888:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_02009a1c
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02009a20
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02009a24
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02009a28
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02009a2c
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_020098da
	b .L_02009a0e
.L_020098da:
	ldr r2, .L_02009a30
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_020098e8
	b .L_020099fe
.L_020098e8:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_020098f0
	b .L_020099fe
.L_020098f0:
	mov r1, r10
	subs r0, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r5, #12]
	movs r1, #128
	subs r3, r3, r2
	ldr r2, [r5, #16]
	lsls r1, r1, #12
	adds r3, r3, r1
	mov r1, r8
	subs r2, r2, r1
	ldr r1, [sp, #0]
	subs r2, r2, r1
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_02009a34
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02009966
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #16
	cmp r3, r1
	bhi .L_020099fe
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_020099fe
	cmp r4, #239
	bgt .L_020099fe
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02009a38
	b .L_020099a2
.L_02009966:
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_020099fe
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_020099fe
	cmp r4, #175
	bgt .L_020099fe
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02009a3c
.L_020099a2:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02009a40
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_020099e0
	adds r0, r5, #0
	bl Func_020029f0
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r6, #9]
	b .L_020099f4
.L_020099e0:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r6, #9]
	negs r0, r0
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r6, #9]
.L_020099f4:
	adds r0, r6, #0
	mov r1, r11
	bl Func_020027d8
	adds r6, #12
.L_020099fe:
	ldr r3, .L_02009a2c
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02009a0e
	b .L_020098da
.L_02009a0e:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009a1c:
	.4byte 0xffff0000
.L_02009a20:
	.4byte gData_02003444
.L_02009a24:
	.4byte gResourceTableEntries
.L_02009a28:
	.4byte gData_02003488
.L_02009a2c:
	.4byte gData_02003446
.L_02009a30:
	.4byte gData_02003448
.L_02009a34:
	.4byte gData_02003548
.L_02009a38:
	.4byte 0x40002000
.L_02009a3c:
	.4byte 0xc000a000
.L_02009a40:
	.4byte gData_0200354a
	.section .text.x02009c4e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009c86,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a44e,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a5e8,"ax",%progbits
	.global Func_020025e8
	.thumb_func
Func_020025e8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a690
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200a694
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_0200a684
.L_0200a61c:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200a678
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200a678
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a64c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02002248
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200a684
.L_0200a64c:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200a684
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_020022b8
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_0200a686
.L_0200a678:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200a61c
.L_0200a684:
	movs r0, #0
.L_0200a686:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a690:
	.4byte gMainRoutineWork
.L_0200a694:
	.4byte Data_0200354c
	.section .bss,"aw",%nobits
	.global gData_02003400
gData_02003400:
	.space 0x00000004
	.global gData_02003404
gData_02003404:
	.space 0x00000020
	.global gData_02003424
gData_02003424:
	.space 0x00000004
	.global gData_02003428
gData_02003428:
	.space 0x0000001c
	.global gData_02003444
gData_02003444:
	.space 0x00000002
	.global gData_02003446
gData_02003446:
	.space 0x00000002
	.global gData_02003448
gData_02003448:
	.space 0x00000040
	.global gData_02003488
gData_02003488:
	.space 0x000000c0
	.global gData_02003548
gData_02003548:
	.space 0x00000002
	.global gData_0200354a
gData_0200354a:
	.space 0x00000002
	.global Data_0200354c
Data_0200354c:
