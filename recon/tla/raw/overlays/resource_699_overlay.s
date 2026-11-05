.syntax unified
	.thumb
	.section .text.x02008298,"ax",%progbits
	.balign 4
	.global Func_02000298
	.thumb_func
Func_02000298:
	push {r5, r6, r7, lr}
	movs r6, #0
.L_0200829c:
	movs r0, #168
	movs r1, #0
	movs r2, #0
	movs r3, #0
	lsls r0, r0, #2
	bl Func_02001c04
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #85
	movs r7, #0
	strb r7, [r3]
	movs r0, #128
	subs r3, #50
	strb r7, [r3]
	lsls r0, r0, #8
	lsls r3, r6, #12
	adds r3, r3, r0
	str r3, [r5, #48]
	str r3, [r5, #52]
	ldr r3, .L_02008320
	adds r2, r5, #0
	str r3, [r5, #108]
	adds r2, #99
	lsls r3, r6, #3
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #2
	bl Actor_SetPartAttribute
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r0, r5, #0
	strb r3, [r1, #9]
	movs r1, #1
	bl Animation_SetStateFlags
	movs r2, #1
	adds r1, r5, #0
	ands r2, r6
	adds r1, #100
	adds r5, #102
	adds r0, r6, #1
	cmp r2, #0
	beq .L_02008312
	movs r3, #212
	strh r3, [r1]
	strh r7, [r5]
	b .L_02008318
.L_02008312:
	movs r3, #116
	strh r3, [r1]
	strh r2, [r5]
.L_02008318:
	adds r6, r0, #0
	cmp r6, #16
	bne .L_0200829c
	pop {r5, r6, r7, pc}
.L_02008320:
	.4byte Func_020001d8
	.section .text.x02008c94,"ax",%progbits
	.balign 4
	.global Func_02000c94
	.thumb_func
Func_02000c94:
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
	ldr r3, .L_02008e28
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02008e2c
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02008e30
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02008e34
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02008e38
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_02008ce6
	b .L_02008e1a
.L_02008ce6:
	ldr r2, .L_02008e3c
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_02008cf4
	b .L_02008e0a
.L_02008cf4:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_02008cfc
	b .L_02008e0a
.L_02008cfc:
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
	ldr r3, .L_02008e40
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02008d72
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
	bhi .L_02008e0a
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_02008e0a
	cmp r4, #239
	bgt .L_02008e0a
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
	ldr r3, .L_02008e44
	b .L_02008dae
.L_02008d72:
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
	bhi .L_02008e0a
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_02008e0a
	cmp r4, #175
	bgt .L_02008e0a
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
	ldr r3, .L_02008e48
.L_02008dae:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02008e4c
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_02008dec
	adds r0, r5, #0
	bl Func_02001d24
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
	b .L_02008e00
.L_02008dec:
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
.L_02008e00:
	adds r0, r6, #0
	mov r1, r11
	bl Func_02001bc4
	adds r6, #12
.L_02008e0a:
	ldr r3, .L_02008e38
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02008e1a
	b .L_02008ce6
.L_02008e1a:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008e28:
	.4byte 0xffff0000
.L_02008e2c:
	.4byte gData_020023f0
.L_02008e30:
	.4byte gResourceTableEntries
.L_02008e34:
	.4byte Data_02002434
.L_02008e38:
	.4byte gData_020023f2
.L_02008e3c:
	.4byte gData_020023f4
.L_02008e40:
	.4byte gData_020024f4
.L_02008e44:
	.4byte 0x40002000
.L_02008e48:
	.4byte 0xc000a000
.L_02008e4c:
	.4byte gData_020024f6
	.section .text.x0200905a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009092,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009388,"ax",%progbits
	.balign 4
	.global Func_02001388
	.thumb_func
Func_02001388:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_020095b8
	str r1, [sp, #40]
	mov r8, r0
	movs r1, #32
	add r1, r8
	mov r9, r1
	mov r12, r9
	adds r5, r2, #0
	mov r2, r12
	adds r6, r3, #0
	str r2, [sp, #8]
	ldr r3, .L_020095bc
	movs r1, #4
	ldr r7, [sp, #80]
	mov lr, r3
	.2byte 0xf800
	add r0, sp, #44
	ldrh r0, [r0]
	mov r1, r8
	strh r0, [r1, #4]
	add r1, sp, #40
	ldrh r1, [r1]
	mov r3, r8
	strh r1, [r3]
	strh r5, [r3, #2]
	movs r3, #255
	lsls r3, r3, #8
	mov r5, r8
	mov r0, r8
	adds r3, #255
	mov r1, r8
	strh r6, [r5, #6]
	movs r2, #0
	strh r7, [r0, #8]
	strh r3, [r1, #12]
	mov r3, r8
	strh r2, [r3, #10]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	mov r12, r3
	lsls r2, r2, #1
	mov r1, r12
	add r2, r12
	adds r1, #236
	ldr r0, [r1]
	ldr r3, [r2, #8]
	ldr r5, [r2, #48]
	adds r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #32]
	adds r1, #4
	ldr r3, [r2, #12]
	ldr r2, [r1]
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #28]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #24]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r2, r2, #20
	lsls r2, r2, #7
	adds r2, r2, r0
	lsls r2, r2, #2
	asrs r3, r3, #20
	adds r5, r5, r2
	movs r0, #0
	str r3, [sp, #20]
	str r5, [sp, #36]
	str r0, [sp, #12]
	cmp r0, r3
	bge .L_0200950c
.L_0200943c:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_02009500
.L_02009450:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_020094f0
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_020094f0
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_020094f0
	ldr r2, [sp, #16]
	ldr r3, [sp, #32]
	mov r0, r9
	adds r7, r2, r3
	strh r7, [r0]
	ldr r1, [sp, #12]
	ldr r2, [sp, #28]
	add r0, sp, #40
	ldrh r0, [r0]
	adds r6, r1, r2
	mov r3, r9
	mov r1, r9
	strh r6, [r3, #2]
	strh r0, [r1, #4]
	ldr r1, [sp, #40]
	movs r0, #10
	adds r1, #1
	adds r0, #255
	str r1, [sp, #40]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020094a4
	cmp r5, r10
	bne .L_020094e2
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_020094e2
.L_020094a4:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094e2
	mov r2, r8
	ldrh r4, [r2, #6]
	ldrh r5, [r2, #8]
	movs r3, #8
	ldrsh r1, [r2, r3]
	movs r3, #6
	ldrsh r0, [r2, r3]
	movs r2, #64
	adds r3, r2, #0
	ands r3, r4
	ands r2, r5
	lsls r3, r3, #16
	lsls r2, r2, #16
	asrs r3, r3, #16
	asrs r2, r2, #16
	orrs r7, r3
	orrs r6, r2
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl Func_02001c34
.L_020094e2:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_020094f0:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_02009450
.L_02009500:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200943c
.L_0200950c:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009564
	ldr r3, .L_020095c0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r2, #0
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	mov r0, r8
	asrs r1, r3, #20
	ldr r3, [sp, #8]
	mov r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	cmp r2, r3
	bge .L_02009564
.L_0200953e:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_02009554
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_02009554
	mov r0, r8
	strh r2, [r0, #12]
.L_02009554:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200953e
.L_02009564:
	movs r0, #128
	lsls r0, r0, #1
	bl Heap_Alloc
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_02009572:
	ldr r3, .L_020095c4
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_02009572
	bl Resource_FindFreeEntry
	mov r1, r8
	strh r0, [r1, #16]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #1
	asrs r0, r0, #16
	bl VramCache_Load
	adds r0, r5, #0
	bl Heap_Release
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020095c8
	bl Task_Add
	mov r3, r8
	movs r2, #10
	ldrsh r0, [r3, r2]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020095b8:
	.4byte gData_020024f8
.L_020095bc:
	.4byte IwramClearWords
.L_020095c0:
	.4byte gMainRoutineWork
.L_020095c4:
	.4byte 0x11111111
.L_020095c8:
	.4byte Func_0200125c
	.section .text.x0200985a,"ax",%progbits
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global Data_020023ac
Data_020023ac:
	.space 0x00000004
	.global gData_020023b0
gData_020023b0:
	.space 0x00000020
	.global gData_020023d0
gData_020023d0:
	.space 0x00000004
	.global gData_020023d4
gData_020023d4:
	.space 0x0000001c
	.global gData_020023f0
gData_020023f0:
	.space 0x00000002
	.global gData_020023f2
gData_020023f2:
	.space 0x00000002
	.global gData_020023f4
gData_020023f4:
	.space 0x00000040
	.global Data_02002434
Data_02002434:
	.space 0x000000c0
	.global gData_020024f4
gData_020024f4:
	.space 0x00000002
	.global gData_020024f6
gData_020024f6:
	.space 0x00000002
	.global gData_020024f8
gData_020024f8:
