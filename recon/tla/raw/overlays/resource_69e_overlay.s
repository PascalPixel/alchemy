.syntax unified
	.thumb
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
