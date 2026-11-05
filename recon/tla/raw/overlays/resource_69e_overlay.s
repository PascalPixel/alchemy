.syntax unified
	.thumb
	.section .text.x02008cb6,"ax",%progbits
	.2byte 0x0000
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
