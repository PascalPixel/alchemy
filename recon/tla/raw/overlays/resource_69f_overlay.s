.syntax unified
	.thumb
	.section .text.x020082da,"ax",%progbits
	.2byte 0x0000
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
