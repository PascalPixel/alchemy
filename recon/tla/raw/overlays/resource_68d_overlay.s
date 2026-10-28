.syntax unified
	.thumb
	.section .text.x020082b4,"ax",%progbits
	.global Func_020002b4
	.thumb_func
Func_020002b4:
	push {r5, r6, lr}
	ldr r3, .L_02008380
	sub sp, #12
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #1
	bne .L_020082e8
	ldr r4, .L_02008384
	ldr r1, .L_02008388
	ldr r0, [r4]
	movs r2, #15
	asrs r3, r0, #3
	ands r3, r2
	ldrb r1, [r1, r3]
	lsls r2, r1, #8
	movs r3, #16
	subs r3, r3, r1
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	adds r0, #1
	str r0, [r4]
	b .L_02008338
.L_020082e8:
	cmp r3, #2
	bne .L_02008338
	ldr r6, .L_02008384
	movs r1, #28
	ldr r0, [r6]
	asrs r0, r0, #2
	bl __modsi3
	ldr r2, .L_0200838c
	adds r5, r0, #0
	lsls r3, r5, #1
	ldrh r2, [r2, r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	cmp r5, #0
	bne .L_0200831c
	ldr r3, .L_02008390
	movs r2, #1
	str r2, [r3]
	ldr r3, .L_02008394
	movs r0, #153
	str r5, [r3]
	bl Func_02002124
.L_0200831c:
	cmp r5, #14
	bne .L_02008332
	ldr r2, .L_02008390
	movs r3, #0
	str r3, [r2]
	ldr r2, .L_02008394
	movs r3, #1
	str r3, [r2]
	movs r0, #153
	bl Func_02002124
.L_02008332:
	ldr r3, [r6]
	adds r3, #1
	str r3, [r6]
.L_02008338:
	movs r3, #208
	mov r5, sp
	lsls r3, r3, #18
	str r3, [r5]
	movs r3, #0
	str r3, [r5, #4]
	movs r3, #224
	lsls r3, r3, #17
	str r3, [r5, #8]
	adds r0, r5, #0
	bl Func_020020d4
	ldr r3, .L_02008390
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008366
	ldr r3, [r5]
	ldr r0, .L_02008398
	str r3, [r0, #12]
	ldr r3, [r5, #8]
	str r3, [r0, #16]
	bl Func_0200210c
.L_02008366:
	ldr r3, .L_02008394
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200837c
	ldr r3, [r5]
	ldr r0, .L_0200839c
	str r3, [r0, #12]
	ldr r3, [r5, #8]
	str r3, [r0, #16]
	bl Func_0200210c
.L_0200837c:
	add sp, #12
	pop {r5, r6, pc}
.L_02008380:
	.4byte gData_0200308c
.L_02008384:
	.4byte Data_02003098
.L_02008388:
	.4byte Data_02002fd7
.L_0200838c:
	.4byte gData_02002fe8
.L_02008390:
	.4byte Data_02003090
.L_02008394:
	.4byte Data_02003024
.L_02008398:
	.4byte Data_02003060
.L_0200839c:
	.4byte Data_02003030
	.section .text.x020083a0,"ax",%progbits
	.global Func_020003a0
	.thumb_func
Func_020003a0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #128
	lsls r0, r0, #5
	sub sp, #4
	bl Heap_Alloc
	mov r9, r0
	movs r2, #253
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #6
	adds r3, #80
	strh r2, [r3]
	adds r3, #2
	movs r1, #128
	lsls r1, r1, #5
	strh r1, [r3]
	ldr r0, .L_020084b4
	movs r7, #32
	mov r1, r9
	bl Func_02001f6c
	ldr r5, .L_020084b8
	bl Resource_FindFreeEntry
	movs r1, #128
	mov r2, r9
	str r0, [r5]
	lsls r1, r1, #5
	ldr r5, .L_020084bc
	bl VramCache_Load
	ldr r6, .L_020084c0
	movs r3, #192
	str r0, [r5]
	movs r1, #0
	str r0, [sp, #0]
	movs r2, #0
	adds r0, r6, #0
	lsls r3, r3, #24
	bl Func_02002104
	movs r3, #1
	strh r3, [r6, #30]
	movs r5, #13
	ldrb r3, [r6, #9]
	negs r5, r5
	adds r2, r5, #0
	ands r2, r3
	movs r3, #8
	mov r8, r3
	mov r1, r8
	orrs r2, r1
	ldrb r1, [r6, #5]
	adds r3, r5, #0
	ands r3, r1
	movs r1, #4
	mov r10, r1
	mov r1, r10
	orrs r3, r1
	orrs r3, r7
	strb r3, [r6, #5]
	movs r3, #15
	ands r2, r3
	strb r2, [r6, #9]
	mov r1, r9
	ldr r0, .L_020084c4
	mov r11, r3
	bl Func_02001f6c
	ldr r6, .L_020084c8
	bl Resource_FindFreeEntry
	movs r1, #128
	mov r2, r9
	str r0, [r6]
	lsls r1, r1, #5
	ldr r6, .L_020084cc
	bl VramCache_Load
	str r0, [r6]
	ldr r6, .L_020084d0
	movs r3, #192
	str r0, [sp, #0]
	movs r1, #0
	adds r0, r6, #0
	movs r2, #0
	lsls r3, r3, #24
	bl Func_02002104
	ldrb r2, [r6, #9]
	movs r3, #2
	strh r3, [r6, #30]
	adds r3, r5, #0
	mov r1, r8
	ands r3, r2
	ldrb r2, [r6, #5]
	orrs r3, r1
	mov r1, r11
	ands r3, r1
	strb r3, [r6, #9]
	ands r5, r2
	ldr r3, .L_020084d4
	mov r2, r10
	orrs r5, r2
	orrs r5, r7
	movs r2, #0
	strb r5, [r6, #5]
	str r2, [r3]
	ldr r3, .L_020084d8
	movs r1, #0
	str r2, [r3]
	ldr r3, .L_020084dc
	ldr r0, .L_020084e0
	str r2, [r3]
	ldr r3, .L_020084e4
	strb r1, [r3]
	movs r1, #144
	lsls r1, r1, #3
	bl Task_Add
	mov r0, r9
	bl Heap_Release
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020084b4:
	.4byte Data_02002620
.L_020084b8:
	.4byte gData_02003020
.L_020084bc:
	.4byte Data_02003088
.L_020084c0:
	.4byte Data_02003060
.L_020084c4:
	.4byte Data_02002af8
.L_020084c8:
	.4byte gData_02003094
.L_020084cc:
	.4byte Data_02003058
.L_020084d0:
	.4byte Data_02003030
.L_020084d4:
	.4byte Data_02003090
.L_020084d8:
	.4byte Data_02003024
.L_020084dc:
	.4byte Data_02003098
.L_020084e0:
	.4byte Func_020002b4
.L_020084e4:
	.4byte gData_0200308c
	.section .bss,"aw",%nobits
	.global gData_02003020
gData_02003020:
	.space 0x00000004
	.global Data_02003024
Data_02003024:
	.space 0x0000000c
	.global Data_02003030
Data_02003030:
	.space 0x00000028
	.global Data_02003058
Data_02003058:
	.space 0x00000008
	.global Data_02003060
Data_02003060:
	.space 0x00000028
	.global Data_02003088
Data_02003088:
	.space 0x00000004
	.global gData_0200308c
gData_0200308c:
	.space 0x00000004
	.global Data_02003090
Data_02003090:
	.space 0x00000004
	.global gData_02003094
gData_02003094:
	.space 0x00000004
	.global Data_02003098
Data_02003098:
