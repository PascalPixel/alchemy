.syntax unified
	.thumb
	.section .text.x02008c44,"ax",%progbits
	.global Func_02000c44
	.thumb_func
Func_02000c44:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r5, r6, #0
	sub sp, #40
	adds r1, r6, #0
	adds r5, #12
	add r0, sp, #24
	adds r1, #16
	adds r2, r5, #0
	bl Func_02000dac
	adds r4, r0, #0
	cmp r4, #0
	bne .L_02008c6e
	b .L_02008d8e
.L_02008c6e:
	ldr r5, [r5]
	ldr r0, .L_02008da0
	str r5, [sp, #20]
	lsls r1, r5, #2
	ldrsb r2, [r0, r1]
	cmp r2, #0
	bge .L_02008c7e
	negs r2, r2
.L_02008c7e:
	adds r3, r1, #2
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_02008c88
	negs r3, r3
.L_02008c88:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #16]
	adds r3, r1, #1
	ldrsb r2, [r0, r3]
	cmp r2, #0
	bge .L_02008c98
	negs r2, r2
.L_02008c98:
	adds r3, r1, #3
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_02008ca2
	negs r3, r3
.L_02008ca2:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #12]
	ldr r3, [sp, #24]
	ldr r2, .L_02008da4
	ldr r1, .L_02008da8
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	mov r9, r1
	mov r2, r9
	ands r2, r3
	lsls r3, r3, #16
	mov r10, r3
	movs r3, #0
	str r3, [r6, #20]
	mov r11, r3
	adds r3, r4, #0
	adds r3, #34
	str r3, [sp, #8]
	ldr r1, [sp, #8]
	movs r3, #2
	strb r3, [r1]
	mov r9, r2
	ldr r3, [r4, #8]
	add r3, r9
	str r3, [r6]
	ldr r3, [r4, #16]
	add r3, r10
	str r3, [r6, #8]
	ldr r3, [r4, #12]
	str r3, [sp, #32]
.L_02008ce0:
	ldr r3, [sp, #20]
	ldr r2, .L_02008da0
	lsls r3, r3, #2
	str r3, [sp, #4]
	adds r3, #1
	ldrsb r2, [r2, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	ldr r2, [sp, #12]
	movs r1, #0
	mov r8, r1
	str r3, [sp, #36]
	cmp r8, r2
	bge .L_02008d4e
.L_02008cfe:
	ldr r3, .L_02008da0
	ldr r1, [sp, #4]
	add r5, sp, #28
	ldrsb r2, [r3, r1]
	ldr r3, [r6]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [sp, #16]
	movs r7, #0
	cmp r7, r2
	bge .L_02008d38
.L_02008d16:
	adds r0, r4, #0
	add r1, sp, #28
	str r4, [sp, #0]
	bl Func_020021b8
	ldr r4, [sp, #0]
	cmp r0, #2
	beq .L_02008d60
	ldr r3, [r5]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5]
	ldr r2, [sp, #16]
	adds r7, #1
	cmp r7, r2
	blt .L_02008d16
.L_02008d38:
	add r2, sp, #28
	ldr r3, [r2, #8]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r2, #8]
	ldr r3, [sp, #12]
	movs r2, #1
	add r8, r2
	cmp r8, r3
	blt .L_02008cfe
.L_02008d4e:
	ldr r3, [r6]
	movs r1, #1
	add r3, r9
	str r3, [r6]
	ldr r3, [r6, #8]
	add r11, r1
	add r3, r10
	str r3, [r6, #8]
	b .L_02008ce0
.L_02008d60:
	ldr r2, [sp, #8]
	movs r3, #0
	strb r3, [r2]
	mov r3, r11
	movs r0, #0
	cmp r3, #0
	beq .L_02008d90
	mov r1, r9
	ldr r3, [r4, #8]
	mov r2, r11
	muls r2, r1
	adds r3, r3, r2
	str r3, [r6]
	movs r0, #1
	ldr r3, [r4, #12]
	str r3, [r6, #4]
	mov r3, r10
	mov r2, r11
	muls r2, r3
	ldr r3, [r4, #16]
	adds r3, r3, r2
	str r3, [r6, #8]
	b .L_02008d90
.L_02008d8e:
	movs r0, #0
.L_02008d90:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008da0:
	.4byte gData_020022dc
.L_02008da4:
	.4byte gData_020022f4
.L_02008da8:
	.4byte 0xffff0000
	.section .text.x020095c6,"ax",%progbits
	.2byte 0x0000
	.section .text.x020095fe,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009dc6,"ax",%progbits
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global Data_02002aac
Data_02002aac:
	.space 0x00000004
	.global gData_02002ab0
gData_02002ab0:
	.space 0x00000020
	.global gData_02002ad0
gData_02002ad0:
	.space 0x00000004
	.global gData_02002ad4
gData_02002ad4:
	.space 0x0000001c
	.global gData_02002af0
gData_02002af0:
	.space 0x00000002
	.global gData_02002af2
gData_02002af2:
	.space 0x00000002
	.global gData_02002af4
gData_02002af4:
	.space 0x00000040
	.global gData_02002b34
gData_02002b34:
	.space 0x000000c0
	.global gData_02002bf4
gData_02002bf4:
	.space 0x00000002
	.global gData_02002bf6
gData_02002bf6:
	.space 0x00000002
	.global gData_02002bf8
gData_02002bf8:
