.syntax unified
	.thumb
	.section .text.x020080c0,"ax",%progbits
	.global Func_020000c0
	.thumb_func
Func_020000c0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	adds r5, r0, #0
	mov lr, r2
	mov r12, r3
	movs r6, #0
	cmp r7, #0
	beq .L_02008132
.L_020080d4:
	ldrh r2, [r5]
	ldr r1, .L_02008104
	movs r3, #31
	ands r3, r2
	mov r4, lr
	lsls r2, r2, #16
	adds r0, r3, r4
	lsrs r3, r2, #21
	mov r8, r2
	ands r3, r1
	mov r2, r12
	adds r4, r3, r2
	mov r3, r8
	lsrs r2, r3, #26
	ldr r3, [sp, #20]
	ands r2, r1
	adds r2, r2, r3
	cmp r0, #31
	ble .L_020080fc
	movs r0, #31
.L_020080fc:
	cmp r0, #0
	bge .L_02008108
	movs r0, #0
	b .L_02008108
.L_02008104:
	.4byte 0x0000001f
.L_02008108:
	cmp r4, #31
	ble .L_0200810e
	movs r4, #31
.L_0200810e:
	cmp r4, #0
	bge .L_02008114
	movs r4, #0
.L_02008114:
	cmp r2, #31
	ble .L_0200811a
	movs r2, #31
.L_0200811a:
	cmp r2, #0
	bge .L_02008120
	movs r2, #0
.L_02008120:
	lsls r3, r2, #10
	lsls r2, r4, #5
	orrs r3, r2
	orrs r3, r0
	adds r6, #1
	strh r3, [r5]
	adds r5, #2
	cmp r6, r7
	bne .L_020080d4
.L_02008132:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x02008138,"ax",%progbits
	.global Func_02000138
	.thumb_func
Func_02000138:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	mov r11, r1
	movs r1, #128
	str r2, [sp, #0]
	ldr r3, .L_02008204
	ldr r2, .L_02008208
	lsls r1, r1, #5
	mov r8, r0
	mov lr, r3
	.2byte 0xf800
	mov r2, r11
	movs r7, #0
	cmp r2, #1
	beq .L_020081f4
.L_02008162:
	movs r4, #1
	negs r4, r4
	add r4, r11
	mov r9, r4
	movs r3, #0
	mov r1, r9
	lsls r0, r7, #14
	mov r10, r3
	bl __divsi3
	bl Math_Cosine
	lsls r3, r0, #4
	ldr r2, [sp, #0]
	subs r3, r3, r0
	asrs r1, r3, #16
	adds r3, r2, #0
	muls r3, r1
	adds r5, r7, #0
	adds r6, r7, #0
	cmp r3, #0
	bge .L_02008196
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
.L_02008196:
	asrs r1, r3, #16
	cmp r1, #15
	ble .L_0200819e
	movs r1, #15
.L_0200819e:
	cmp r1, #0
	bgt .L_020081a4
	movs r1, #1
.L_020081a4:
	cmp r7, #0
	blt .L_020081ee
.L_020081a8:
	movs r2, #63
	mov r3, r10
	subs r0, r2, r5
	subs r2, r2, r3
	lsls r3, r2, #6
	adds r3, r3, r0
	add r3, r8
	strb r1, [r3]
	cmp r0, #63
	bge .L_020081be
	strb r1, [r3, #1]
.L_020081be:
	movs r4, #63
	adds r0, r2, #0
	subs r2, r4, r5
	lsls r3, r2, #6
	adds r3, r3, r0
	add r3, r8
	strb r1, [r3]
	cmp r0, #63
	bge .L_020081d2
	strb r1, [r3, #1]
.L_020081d2:
	mov r2, r10
	lsls r3, r2, #1
	subs r3, r6, r3
	subs r6, r3, #1
	cmp r6, #0
	bge .L_020081e6
	lsls r3, r5, #1
	adds r3, r6, r3
	subs r6, r3, #2
	subs r5, #1
.L_020081e6:
	movs r3, #1
	add r10, r3
	cmp r5, r10
	bge .L_020081a8
.L_020081ee:
	adds r7, #1
	cmp r7, r9
	bne .L_02008162
.L_020081f4:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008204:
	.4byte IwramFillWords
.L_02008208:
	.4byte 0x01010101
	.section .text.x02008d64,"ax",%progbits
	.global Func_02000d64
	.thumb_func
Func_02000d64:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #24
	bl Func_02002094
	mov r8, r0
	bl Func_0200209c
	bl Actor_GetById
	ldr r3, [r0, #80]
	ldrh r2, [r0, #32]
	ldr r3, [r3, #12]
	mov r10, r0
	adds r0, r2, #0
	muls r0, r3
	str r0, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r2, #230
	mov r9, r1
	lsls r2, r2, #1
	add r2, r9
	ldr r3, [r3, #32]
	ldr r6, [r2]
	movs r2, #150
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r5, r8
	ldr r3, [r3]
	ldrb r2, [r5]
	movs r5, #128
	lsls r5, r5, #9
	ldr r1, [r6, #8]
	ldr r0, [r6, #16]
	ldr r4, .L_02008f80
	cmp r3, r5
	bgt .L_02008dda
	ldr r3, .L_02008f84
	movs r5, #160
	lsls r5, r5, #16
	adds r3, r1, r3
	adds r5, r1, r5
	ldr r1, .L_02008f88
	str r3, [sp, #12]
	movs r3, #200
	lsls r3, r3, #16
	adds r1, r0, r1
	adds r3, r0, r3
	str r5, [sp, #8]
	str r1, [sp, #4]
	str r3, [sp, #0]
	b .L_02008df6
.L_02008dda:
	ldr r5, .L_02008f8c
	movs r3, #240
	adds r5, r1, r5
	str r5, [sp, #12]
	lsls r3, r3, #16
	ldr r5, .L_02008f90
	adds r3, r1, r3
	movs r1, #150
	lsls r1, r1, #17
	adds r5, r0, r5
	adds r1, r0, r1
	str r3, [sp, #8]
	str r5, [sp, #4]
	str r1, [sp, #0]
.L_02008df6:
	movs r3, #0
	str r3, [sp, #16]
	adds r3, r2, #0
	mov r11, r4
	cmp r3, #0
	bne .L_02008e04
	b .L_02008f72
.L_02008e04:
	mov r5, r8
	ldrb r3, [r5, #1]
	cmp r3, #15
	bne .L_02008e0e
	b .L_02008f56
.L_02008e0e:
	movs r1, #4
	ldrsh r3, [r5, r1]
	mov r0, r11
	ldr r6, [r0]
	mov r0, r8
	lsls r5, r3, #16
	ldr r1, [sp, #12]
	movs r2, #6
	ldrsh r3, [r0, r2]
	lsls r7, r3, #16
	cmp r5, r1
	bgt .L_02008e28
	b .L_02008f46
.L_02008e28:
	ldr r2, [sp, #8]
	cmp r5, r2
	blt .L_02008e30
	b .L_02008f46
.L_02008e30:
	ldr r3, [sp, #4]
	cmp r7, r3
	bgt .L_02008e38
	b .L_02008f46
.L_02008e38:
	ldr r0, [sp, #0]
	cmp r7, r0
	blt .L_02008e40
	b .L_02008f46
.L_02008e40:
	mov r2, r8
	movs r1, #10
	ldrsh r0, [r2, r1]
	movs r3, #1
	negs r3, r3
	movs r2, #1
	cmp r0, r3
	beq .L_02008e70
	movs r3, #128
	lsls r3, r3, #5
	ands r3, r0
	cmp r3, #0
	beq .L_02008e62
	bl GameFlag_Test
	adds r2, r0, #0
	b .L_02008e70
.L_02008e62:
	bl GameFlag_Test
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	movs r2, #1
	subs r2, r2, r3
.L_02008e70:
	cmp r2, #0
	beq .L_02008f46
	cmp r6, #0
	bne .L_02008ec4
	mov r2, r8
	movs r1, #2
	ldrsh r0, [r2, r1]
	adds r3, r7, #0
	adds r1, r5, #0
	movs r2, #0
	bl Func_02001fcc
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008f56
	ldr r1, .L_02008f94
	bl Func_02001fc4
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	mov r5, r8
	adds r2, #4
	movs r3, #1
	strb r3, [r2]
	ldrh r3, [r5, #8]
	adds r0, r6, #0
	strh r3, [r6, #6]
	movs r1, #1
	bl Func_02001fbc
	ldr r5, [r6, #80]
	movs r1, #192
	ldr r0, [r5, #12]
	ldr r3, .L_02008f98
	lsls r1, r1, #8
	mov lr, r3
	.2byte 0xf800
	str r0, [r5, #12]
	mov r0, r11
	str r6, [r0]
.L_02008ec4:
	ldr r3, .L_02008f9c
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #118
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008f56
	ldr r3, [r6, #80]
	mov r5, r10
	ldr r0, [r3, #12]
	ldr r2, [r6, #8]
	ldr r3, [r5, #8]
	subs r1, r2, r3
	cmp r1, #0
	bge .L_02008ee8
	subs r1, r3, r2
.L_02008ee8:
	ldrh r3, [r6, #32]
	muls r3, r0
	ldr r0, [sp, #20]
	adds r2, r0, r3
	mov r3, r10
	ldr r0, [r6, #16]
	ldr r4, [r3, #16]
	subs r3, r0, r4
	cmp r3, #0
	blt .L_02008f04
	adds r3, r1, r3
	cmp r3, r2
	blt .L_02008f0c
	b .L_02008f56
.L_02008f04:
	subs r3, r4, r0
	adds r3, r1, r3
	cmp r3, r2
	bge .L_02008f56
.L_02008f0c:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008f56
	ldr r3, .L_02008f9c
	movs r5, #128
	lsls r5, r5, #2
	adds r5, #18
	adds r3, r3, r5
	ldrb r3, [r3]
	cmp r3, #9
	bne .L_02008f38
	movs r2, #179
	movs r3, #128
	lsls r2, r2, #1
	lsls r3, r3, #6
	add r2, r9
	adds r3, #139
	strh r3, [r2]
	b .L_02008f56
.L_02008f38:
	ldr r2, [sp, #16]
	movs r3, #170
	lsls r3, r3, #1
	adds r2, #100
	add r3, r9
	strh r2, [r3]
	b .L_02008f56
.L_02008f46:
	cmp r6, #0
	beq .L_02008f56
	adds r0, r6, #0
	bl Func_02001fd4
	movs r3, #0
	mov r0, r11
	str r3, [r0]
.L_02008f56:
	ldr r1, [sp, #16]
	movs r2, #4
	adds r1, #1
	movs r3, #20
	str r1, [sp, #16]
	add r11, r2
	add r8, r3
	cmp r1, #127
	bhi .L_02008f72
	mov r5, r8
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_02008f72
	b .L_02008e04
.L_02008f72:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008f80:
	.4byte gData_02003dc0
.L_02008f84:
	.4byte 0xff600000
.L_02008f88:
	.4byte 0xfed40000
.L_02008f8c:
	.4byte 0xff100000
.L_02008f90:
	.4byte 0xfe3e0000
.L_02008f94:
	.4byte gData_02003da8
.L_02008f98:
	.4byte IwramMulQ16
.L_02008f9c:
	.4byte gMainRoutineWork
	.section .text.x02009ad0,"ax",%progbits
	.balign 4
	.global Func_02001ad0
	.thumb_func
Func_02001ad0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	sub sp, #8
	adds r6, r0, #0
	mov r9, r3
	movs r7, #0
.L_02009aea:
	movs r0, #16
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	ldr r2, [r6, #12]
	adds r0, #255
	bl Func_02001fcc
	mov r8, sp
	lsls r3, r7, #2
	mov r1, r8
	str r0, [r3, r1]
	cmp r0, #0
	beq .L_02009b94
	ldr r3, [r6, #20]
	ldr r5, [r0, #80]
	str r3, [r0, #20]
	ldr r1, .L_02009b24
	adds r3, r0, #0
	adds r3, #85
	movs r2, #0
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	mov r10, r1
	str r6, [r0, #104]
	cmp r5, #0
	beq .L_02009b94
	b .L_02009b28
	.2byte 0x0000
.L_02009b24:
	.4byte 0x00000000
.L_02009b28:
	movs r1, #0
	adds r0, r5, #0
	bl Animation_ApplyChildArgument
	mov r2, r10
	strb r2, [r5, #26]
	ldrb r0, [r5, #16]
	bl Resource_ResetEntry
	movs r3, #248
	lsls r3, r3, #3
	add r3, r9
	ldrh r3, [r3]
	movs r2, #1
	strb r3, [r5, #16]
	ldrb r3, [r5, #17]
	ldr r1, .L_02009b84
	orrs r3, r2
	strb r3, [r5, #17]
	ldrb r3, [r5, #16]
	ldr r2, .L_02009b88
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r2, [r3, #2]
	ldrh r3, [r5, #8]
	lsls r2, r2, #17
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
	movs r1, #33
	ldrb r3, [r5, #5]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #63
	ands r3, r2
	movs r1, #64
	orrs r3, r1
	strb r3, [r5, #5]
	ldrb r3, [r5, #7]
	ands r2, r3
	movs r3, #128
	orrs r2, r3
	b .L_02009b8c
	.2byte 0x0000
.L_02009b84:
	.4byte 0xfffffc00
.L_02009b88:
	.4byte gResourceTableEntries
.L_02009b8c:
	ldr r3, [r5, #40]
	strb r2, [r5, #7]
	mov r2, r10
	strb r2, [r3, #22]
.L_02009b94:
	adds r7, #1
	cmp r7, #1
	ble .L_02009aea
	ldr r2, [sp, #0]
	ldr r3, .L_02009bd8
	ldr r0, [r2, #80]
	str r3, [r2, #108]
	ldrb r1, [r0, #9]
	movs r2, #13
	negs r2, r2
	adds r3, r2, #0
	movs r4, #4
	ands r3, r1
	orrs r3, r4
	strb r3, [r0, #9]
	mov r3, r8
	ldr r1, [r3, #4]
	add sp, #8
	ldr r0, [r1, #80]
	ldrb r3, [r0, #9]
	ands r2, r3
	ldr r3, .L_02009bdc
	orrs r2, r4
	str r3, [r1, #108]
	adds r1, #35
	movs r3, #2
	strb r2, [r0, #9]
	strb r3, [r1]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009bd8:
	.4byte Func_02001a80
.L_02009bdc:
	.4byte Func_02001a34
	.section .text.x02009be0,"ax",%progbits
	.global Func_02001be0
	.thumb_func
Func_02001be0:
	push {r5, r6, r7, lr}
	lsls r3, r0, #4
	subs r3, r3, r0
	movs r2, #128
	lsls r3, r3, #1
	lsls r2, r2, #8
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r6, r3, #1
	movs r7, #0
	cmp r0, #0
	beq .L_02009bfa
	movs r7, #100
.L_02009bfa:
	cmp r6, #15
	bgt .L_02009c0c
	ldr r3, .L_02009d4c
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #1
	bhi .L_02009c0c
	movs r7, #200
.L_02009c0c:
	cmp r6, #7
	bgt .L_02009c1e
	ldr r3, .L_02009d4c
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #1
	bhi .L_02009c1e
	movs r7, #200
.L_02009c1e:
	ldr r1, .L_02009d50
	adds r2, r7, r6
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r2, r3
	bne .L_02009c2c
	b .L_02009d4a
.L_02009c2c:
	ldr r5, .L_02009d54
	strh r2, [r1]
	movs r1, #128
	ldr r3, .L_02009d58
	adds r0, r5, #0
	lsls r1, r1, #1
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_02009d5c
	cmp r7, #0
	bne .L_02009c4e
	ldr r1, .L_02009d60
	adds r0, r5, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_02009d3a
.L_02009c4e:
	cmp r7, #100
	bne .L_02009c5e
	ldr r1, .L_02009d64
	adds r0, r5, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_02009c68
.L_02009c5e:
	ldr r1, .L_02009d60
	adds r0, r5, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_02009c68:
	movs r2, #0
	adds r5, r6, #0
	mov r12, r2
	cmp r5, #7
	ble .L_02009c84
	ldr r3, .L_02009d5c
	ldr r0, .L_02009d54
	ldr r1, .L_02009d68
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	movs r3, #8
	mov r12, r3
	subs r5, #8
.L_02009c84:
	cmp r6, #15
	ble .L_02009c9c
	ldr r0, .L_02009d6c
	ldr r1, .L_02009d70
	ldr r3, .L_02009d5c
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	adds r5, r6, #0
	movs r0, #16
	mov r12, r0
	subs r5, #16
.L_02009c9c:
	cmp r6, #23
	ble .L_02009cb4
	movs r2, #32
	ldr r0, .L_02009d74
	ldr r1, .L_02009d78
	ldr r3, .L_02009d5c
	mov lr, r3
	.2byte 0xf800
	adds r5, r6, #0
	movs r2, #24
	mov r12, r2
	subs r5, #24
.L_02009cb4:
	cmp r6, #30
	ble .L_02009cca
	ldr r3, .L_02009d5c
	ldr r0, .L_02009d7c
	ldr r1, .L_02009d80
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	movs r3, #0
	mov r12, r3
	movs r5, #0
.L_02009cca:
	cmp r5, #0
	beq .L_02009d3a
	movs r7, #0
	cmp r7, r5
	bge .L_02009d3a
.L_02009cd4:
	mov r3, r12
	cmp r3, #0
	bge .L_02009cdc
	adds r3, #7
.L_02009cdc:
	asrs r3, r3, #3
	lsls r1, r3, #5
	lsrs r3, r7, #31
	adds r3, r7, r3
	asrs r0, r3, #1
	ldr r2, .L_02009d84
	ldr r3, .L_02009d88
	adds r2, r1, r2
	adds r3, r1, r3
	adds r6, r2, r0
	adds r0, r3, r0
	movs r3, #1
	ands r3, r7
	movs r4, #2
	cmp r3, #0
	bne .L_02009d1a
	movs r4, #1
.L_02009cfe:
	ldrb r3, [r0]
	ldrb r1, [r6]
	movs r2, #240
	ands r2, r3
	movs r3, #15
	ands r3, r1
	orrs r2, r3
	adds r4, #1
	strb r2, [r0]
	adds r6, #4
	adds r0, #4
	cmp r4, #3
	ble .L_02009cfe
	b .L_02009d34
.L_02009d1a:
	ldrb r3, [r0]
	ldrb r1, [r6]
	movs r2, #15
	ands r2, r3
	movs r3, #240
	ands r3, r1
	orrs r2, r3
	subs r4, #1
	strb r2, [r0]
	adds r6, #4
	adds r0, #4
	cmp r4, #0
	bge .L_02009d1a
.L_02009d34:
	adds r7, #1
	cmp r7, r5
	blt .L_02009cd4
.L_02009d3a:
	ldr r3, .L_02009d8c
	movs r1, #128
	movs r2, #0
	ldrsh r0, [r3, r2]
	lsls r1, r1, #1
	ldr r2, .L_02009d54
	bl VramCache_Load
.L_02009d4a:
	pop {r5, r6, r7, pc}
.L_02009d4c:
	.4byte gFrameCount
.L_02009d50:
	.4byte gData_02003fc4
.L_02009d54:
	.4byte gData_02003fd8
.L_02009d58:
	.4byte IwramClearWords
.L_02009d5c:
	.4byte IwramCopyWords
.L_02009d60:
	.4byte Data_020041d8
.L_02009d64:
	.4byte Data_02004158
.L_02009d68:
	.4byte gData_020040d8
.L_02009d6c:
	.4byte Data_02003ff8
.L_02009d70:
	.4byte Data_020040f8
.L_02009d74:
	.4byte gData_02004018
.L_02009d78:
	.4byte Data_02004118
.L_02009d7c:
	.4byte Data_02004038
.L_02009d80:
	.4byte Data_02004138
.L_02009d84:
	.4byte gData_020040dc
.L_02009d88:
	.4byte Data_02003fdc
.L_02009d8c:
	.4byte gData_02003fc6
	.section .text.x02009d90,"ax",%progbits
	.global Func_02001d90
	.thumb_func
Func_02001d90:
	push {r5, r6, r7, lr}
	ldr r0, .L_02009e2c
	sub sp, #16
	str r0, [sp, #0]
	ldr r3, .L_02009e30
	ldr r2, .L_02009e34
	movs r1, #0
	ldrsh r3, [r3, r1]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r6, r3, #5
	bl Func_0200209c
	bl Actor_GetById
	ldr r3, .L_02009e38
	movs r2, #3
	ldr r3, [r3]
	adds r7, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_02009dec
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009dec
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #11
	cmp r3, r2
	ble .L_02009dec
	ldr r5, .L_02009e3c
	ldrh r3, [r5]
	subs r3, #1
	strh r3, [r5]
	lsls r3, r3, #16
	cmp r3, #0
	bgt .L_02009dec
	movs r0, #1
	bl Func_0200200c
	ldr r3, .L_02009e28
	strh r3, [r5]
.L_02009dec:
	bl Func_02002014
	bl Func_02001be0
	adds r0, r7, #0
	add r1, sp, #4
	adds r0, #8
	bl Render_ProjectPoint
	add r3, sp, #4
	ldr r0, [sp, #0]
	ldr r1, [r3]
	ldr r3, [r3, #4]
	movs r2, #0
	stmia r0!, {r2}
	ldr r5, .L_02009e40
	subs r1, #16
	lsls r1, r1, #16
	subs r3, #32
	orrs r3, r1
	adds r4, r0, #0
	orrs r3, r5
	str r4, [sp, #0]
	stmia r0!, {r3}
	movs r3, #128
	lsls r3, r3, #3
	adds r1, r0, #0
	orrs r6, r3
	b .L_02009e44
	.2byte 0x0000
.L_02009e28:
	.4byte 0x0000000a
.L_02009e2c:
	.4byte Data_02003fcc
.L_02009e30:
	.4byte gData_02003fc6
.L_02009e34:
	.4byte gResourceTableEntries
.L_02009e38:
	.4byte gInput
.L_02009e3c:
	.4byte gData_02003fc8
.L_02009e40:
	.4byte 0x80004000
.L_02009e44:
	str r1, [sp, #0]
	str r6, [r0]
	movs r1, #255
	ldr r0, .L_02009e54
	bl Func_02001f7c
	add sp, #16
	pop {r5, r6, r7, pc}
.L_02009e54:
	.4byte Data_02003fcc
	.section .bss,"aw",%nobits
	.global gData_02003dbc
gData_02003dbc:
	.space 0x00000002
	.global gData_02003dbe
gData_02003dbe:
	.space 0x00000002
	.global gData_02003dc0
gData_02003dc0:
	.space 0x00000200
	.global gData_02003fc0
gData_02003fc0:
	.space 0x00000004
	.global gData_02003fc4
gData_02003fc4:
	.space 0x00000002
	.global gData_02003fc6
gData_02003fc6:
	.space 0x00000002
	.global gData_02003fc8
gData_02003fc8:
	.space 0x00000004
	.global Data_02003fcc
Data_02003fcc:
	.space 0x0000000c
	.global gData_02003fd8
gData_02003fd8:
	.space 0x00000004
	.global Data_02003fdc
Data_02003fdc:
	.space 0x0000001c
	.global Data_02003ff8
Data_02003ff8:
	.space 0x00000020
	.global gData_02004018
gData_02004018:
	.space 0x00000020
	.global Data_02004038
Data_02004038:
	.space 0x000000a0
	.global gData_020040d8
gData_020040d8:
	.space 0x00000004
	.global gData_020040dc
gData_020040dc:
	.space 0x0000001c
	.global Data_020040f8
Data_020040f8:
	.space 0x00000020
	.global Data_02004118
Data_02004118:
	.space 0x00000020
	.global Data_02004138
Data_02004138:
	.space 0x00000020
	.global Data_02004158
Data_02004158:
	.space 0x00000080
	.global Data_020041d8
Data_020041d8:
