.syntax unified
	.thumb
	.section .text.x020082da,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008b5c,"ax",%progbits
	.global Func_02000b5c
	.thumb_func
Func_02000b5c:
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
	ldr r3, .L_02008cf0
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02008cf4
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02008cf8
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02008cfc
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02008d00
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_02008bae
	b .L_02008ce2
.L_02008bae:
	ldr r2, .L_02008d04
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_02008bbc
	b .L_02008cd2
.L_02008bbc:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_02008bc4
	b .L_02008cd2
.L_02008bc4:
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
	ldr r3, .L_02008d08
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02008c3a
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
	bhi .L_02008cd2
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_02008cd2
	cmp r4, #239
	bgt .L_02008cd2
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
	ldr r3, .L_02008d0c
	b .L_02008c76
.L_02008c3a:
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
	bhi .L_02008cd2
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_02008cd2
	cmp r4, #175
	bgt .L_02008cd2
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
	ldr r3, .L_02008d10
.L_02008c76:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02008d14
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_02008cb4
	adds r0, r5, #0
	bl Func_02001bd4
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
	b .L_02008cc8
.L_02008cb4:
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
.L_02008cc8:
	adds r0, r6, #0
	mov r1, r11
	bl Func_02001a84
	adds r6, #12
.L_02008cd2:
	ldr r3, .L_02008d00
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02008ce2
	b .L_02008bae
.L_02008ce2:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008cf0:
	.4byte 0xffff0000
.L_02008cf4:
	.4byte gData_02002428
.L_02008cf8:
	.4byte gResourceTableEntries
.L_02008cfc:
	.4byte Data_0200246c
.L_02008d00:
	.4byte gData_0200242a
.L_02008d04:
	.4byte gData_0200242c
.L_02008d08:
	.4byte gData_0200252c
.L_02008d0c:
	.4byte 0x40002000
.L_02008d10:
	.4byte 0xc000a000
.L_02008d14:
	.4byte gData_0200252e
	.section .text.x02008f22,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008f5a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009722,"ax",%progbits
	.2byte 0x0000
	.section .text.x020098bc,"ax",%progbits
	.global Func_020018bc
	.thumb_func
Func_020018bc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009964
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_02009968
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
	bge .L_02009958
.L_020098f0:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200994c
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200994c
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009920
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0200151c
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_02009958
.L_02009920:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_02009958
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_0200158c
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
	b .L_0200995a
.L_0200994c:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_020098f0
.L_02009958:
	movs r0, #0
.L_0200995a:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009964:
	.4byte gMainRoutineWork
.L_02009968:
	.4byte gData_02002530
	.section .bss,"aw",%nobits
	.global Data_020023e4
Data_020023e4:
	.space 0x00000004
	.global gData_020023e8
gData_020023e8:
	.space 0x00000020
	.global gData_02002408
gData_02002408:
	.space 0x00000004
	.global gData_0200240c
gData_0200240c:
	.space 0x0000001c
	.global gData_02002428
gData_02002428:
	.space 0x00000002
	.global gData_0200242a
gData_0200242a:
	.space 0x00000002
	.global gData_0200242c
gData_0200242c:
	.space 0x00000040
	.global Data_0200246c
Data_0200246c:
	.space 0x000000c0
	.global gData_0200252c
gData_0200252c:
	.space 0x00000002
	.global gData_0200252e
gData_0200252e:
	.space 0x00000002
	.global gData_02002530
gData_02002530:
