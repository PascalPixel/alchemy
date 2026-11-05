.syntax unified
	.thumb
	.section .text.x020089da,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200925c,"ax",%progbits
	.global Func_0200125c
	.thumb_func
Func_0200125c:
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
	ldr r3, .L_020093f0
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_020093f4
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_020093f8
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_020093fc
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02009400
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_020092ae
	b .L_020093e2
.L_020092ae:
	ldr r2, .L_02009404
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_020092bc
	b .L_020093d2
.L_020092bc:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_020092c4
	b .L_020093d2
.L_020092c4:
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
	ldr r3, .L_02009408
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_0200933a
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
	bhi .L_020093d2
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_020093d2
	cmp r4, #239
	bgt .L_020093d2
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
	ldr r3, .L_0200940c
	b .L_02009376
.L_0200933a:
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
	bhi .L_020093d2
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_020093d2
	cmp r4, #175
	bgt .L_020093d2
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
	ldr r3, .L_02009410
.L_02009376:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02009414
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_020093b4
	adds r0, r5, #0
	bl Func_020022f4
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
	b .L_020093c8
.L_020093b4:
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
.L_020093c8:
	adds r0, r6, #0
	mov r1, r11
	bl Func_0200218c
	adds r6, #12
.L_020093d2:
	ldr r3, .L_02009400
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_020093e2
	b .L_020092ae
.L_020093e2:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020093f0:
	.4byte 0xffff0000
.L_020093f4:
	.4byte gData_02002bd8
.L_020093f8:
	.4byte gResourceTableEntries
.L_020093fc:
	.4byte Data_02002c1c
.L_02009400:
	.4byte gData_02002bda
.L_02009404:
	.4byte gData_02002bdc
.L_02009408:
	.4byte gData_02002cdc
.L_0200940c:
	.4byte 0x40002000
.L_02009410:
	.4byte 0xc000a000
.L_02009414:
	.4byte gData_02002cde
	.section .text.x02009622,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200965a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009e22,"ax",%progbits
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global gData_02002b60
gData_02002b60:
	.space 0x00000028
	.global gData_02002b88
gData_02002b88:
	.space 0x0000000c
	.global Data_02002b94
Data_02002b94:
	.space 0x00000004
	.global gData_02002b98
gData_02002b98:
	.space 0x00000020
	.global gData_02002bb8
gData_02002bb8:
	.space 0x00000004
	.global gData_02002bbc
gData_02002bbc:
	.space 0x0000001c
	.global gData_02002bd8
gData_02002bd8:
	.space 0x00000002
	.global gData_02002bda
gData_02002bda:
	.space 0x00000002
	.global gData_02002bdc
gData_02002bdc:
	.space 0x00000040
	.global Data_02002c1c
Data_02002c1c:
	.space 0x000000c0
	.global gData_02002cdc
gData_02002cdc:
	.space 0x00000002
	.global gData_02002cde
gData_02002cde:
	.space 0x00000002
	.global gData_02002ce0
gData_02002ce0:
