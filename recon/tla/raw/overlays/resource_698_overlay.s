.syntax unified
	.thumb
	.section .text.x0200822a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008aac,"ax",%progbits
	.global Func_02000aac
	.thumb_func
Func_02000aac:
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
	ldr r3, .L_02008c40
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02008c44
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02008c48
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02008c4c
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02008c50
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_02008afe
	b .L_02008c32
.L_02008afe:
	ldr r2, .L_02008c54
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_02008b0c
	b .L_02008c22
.L_02008b0c:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_02008b14
	b .L_02008c22
.L_02008b14:
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
	ldr r3, .L_02008c58
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02008b8a
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
	bhi .L_02008c22
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_02008c22
	cmp r4, #239
	bgt .L_02008c22
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
	ldr r3, .L_02008c5c
	b .L_02008bc6
.L_02008b8a:
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
	bhi .L_02008c22
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_02008c22
	cmp r4, #175
	bgt .L_02008c22
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
	ldr r3, .L_02008c60
.L_02008bc6:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02008c64
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_02008c04
	adds r0, r5, #0
	bl Func_02001b24
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
	b .L_02008c18
.L_02008c04:
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
.L_02008c18:
	adds r0, r6, #0
	mov r1, r11
	bl Func_020019d4
	adds r6, #12
.L_02008c22:
	ldr r3, .L_02008c50
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02008c32
	b .L_02008afe
.L_02008c32:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008c40:
	.4byte 0xffff0000
.L_02008c44:
	.4byte gData_02002200
.L_02008c48:
	.4byte gResourceTableEntries
.L_02008c4c:
	.4byte Data_02002244
.L_02008c50:
	.4byte gData_02002202
.L_02008c54:
	.4byte gData_02002204
.L_02008c58:
	.4byte gData_02002304
.L_02008c5c:
	.4byte 0x40002000
.L_02008c60:
	.4byte 0xc000a000
.L_02008c64:
	.4byte gData_02002306
	.section .text.x02008e72,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008eaa,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009672,"ax",%progbits
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global gData_020021bc
gData_020021bc:
	.space 0x00000004
	.global gData_020021c0
gData_020021c0:
	.space 0x00000020
	.global gData_020021e0
gData_020021e0:
	.space 0x00000004
	.global gData_020021e4
gData_020021e4:
	.space 0x0000001c
	.global gData_02002200
gData_02002200:
	.space 0x00000002
	.global gData_02002202
gData_02002202:
	.space 0x00000002
	.global gData_02002204
gData_02002204:
	.space 0x00000040
	.global Data_02002244
Data_02002244:
	.space 0x000000c0
	.global gData_02002304
gData_02002304:
	.space 0x00000002
	.global gData_02002306
gData_02002306:
	.space 0x00000002
	.global gData_02002308
gData_02002308:
