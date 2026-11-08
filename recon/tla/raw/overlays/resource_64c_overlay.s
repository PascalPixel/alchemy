.syntax unified
	.thumb
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
