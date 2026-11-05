.syntax unified
	.thumb
	.section .text.x020085a0,"ax",%progbits
	.global Func_020005a0
	.thumb_func
Func_020005a0:
	push {r5, lr}
	ldr r3, .L_020085e4
	adds r5, r0, #0
	adds r4, r3, #0
	movs r2, #0
	ldrsh r3, [r4, r2]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	beq .L_020085de
	movs r0, #2
	movs r1, #0
.L_020085b8:
	ldrsh r2, [r4, r1]
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020085d0
	ldrsh r2, [r0, r4]
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020085d0
	movs r0, #1
	b .L_020085e0
.L_020085d0:
	adds r1, #4
	ldrsh r3, [r4, r1]
	movs r2, #1
	negs r2, r2
	adds r0, #4
	cmp r3, r2
	bne .L_020085b8
.L_020085de:
	movs r0, #0
.L_020085e0:
	pop {r5, pc}
	.2byte 0x0000
.L_020085e4:
	.4byte gData_02002c64
	.section .text.x0200915a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009b74,"ax",%progbits
	.global Func_02001b74
	.thumb_func
Func_02001b74:
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
	ldr r3, .L_02009d08
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02009d0c
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02009d10
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02009d14
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02009d18
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_02009bc6
	b .L_02009cfa
.L_02009bc6:
	ldr r2, .L_02009d1c
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_02009bd4
	b .L_02009cea
.L_02009bd4:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_02009bdc
	b .L_02009cea
.L_02009bdc:
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
	ldr r3, .L_02009d20
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02009c52
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
	bhi .L_02009cea
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_02009cea
	cmp r4, #239
	bgt .L_02009cea
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
	ldr r3, .L_02009d24
	b .L_02009c8e
.L_02009c52:
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
	bhi .L_02009cea
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_02009cea
	cmp r4, #175
	bgt .L_02009cea
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
	ldr r3, .L_02009d28
.L_02009c8e:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02009d2c
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_02009ccc
	adds r0, r5, #0
	bl Func_02002c2c
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
	b .L_02009ce0
.L_02009ccc:
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
.L_02009ce0:
	adds r0, r6, #0
	mov r1, r11
	bl Func_02002aa4
	adds r6, #12
.L_02009cea:
	ldr r3, .L_02009d18
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02009cfa
	b .L_02009bc6
.L_02009cfa:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009d08:
	.4byte 0xffff0000
.L_02009d0c:
	.4byte gData_020035fc
.L_02009d10:
	.4byte gResourceTableEntries
.L_02009d14:
	.4byte Data_02003640
.L_02009d18:
	.4byte gData_020035fe
.L_02009d1c:
	.4byte gData_02003600
.L_02009d20:
	.4byte gData_02003700
.L_02009d24:
	.4byte 0x40002000
.L_02009d28:
	.4byte 0xc000a000
.L_02009d2c:
	.4byte gData_02003702
	.section .text.x02009f3a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009f72,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a73a,"ax",%progbits
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global Data_020035b8
Data_020035b8:
	.space 0x00000004
	.global gData_020035bc
gData_020035bc:
	.space 0x00000020
	.global gData_020035dc
gData_020035dc:
	.space 0x00000004
	.global gData_020035e0
gData_020035e0:
	.space 0x0000001c
	.global gData_020035fc
gData_020035fc:
	.space 0x00000002
	.global gData_020035fe
gData_020035fe:
	.space 0x00000002
	.global gData_02003600
gData_02003600:
	.space 0x00000040
	.global Data_02003640
Data_02003640:
	.space 0x000000c0
	.global gData_02003700
gData_02003700:
	.space 0x00000002
	.global gData_02003702
gData_02003702:
	.space 0x00000002
	.global gData_02003704
gData_02003704:
