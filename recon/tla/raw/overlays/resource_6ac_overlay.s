.syntax unified
	.thumb
	.section .text.x0200a4e4,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_020024e4
	.thumb_func
Func_020024e4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_ja_0200a664
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #12
	cmp r3, #0
	beq .L_ja_0200a4f4
	b .L_ja_0200a654
.L_ja_0200a4f4:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02002368
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r2, .L_ja_0200a668
	adds r5, r0, #0
	mov r10, r2
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	ldr r3, .L_ja_0200a66c
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r1, #49
	bl Func_02003118
.L_ja_0200a52c:
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldrb r0, [r3]
	ldr r2, [r5, #16]
	bl Func_02003168
	mov r6, r10
	mov r7, r10
	adds r6, #104
	adds r7, #106
	mov r1, sp
	cmp r0, #7
	bne .L_ja_0200a5a4
	movs r0, #0
	ldrsh r3, [r6, r0]
	movs r2, #0
	lsls r3, r3, #17
	str r3, [r5, #36]
	mov r8, r2
	movs r1, #0
	ldrsh r3, [r7, r1]
	lsls r3, r3, #17
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r5, #52]
.L_ja_0200a562:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_02003180
	cmp r0, #0
	beq .L_ja_0200a592
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	b .L_ja_0200a648
.L_ja_0200a592:
	movs r0, #1
	bl wait
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	ble .L_ja_0200a562
	b .L_ja_0200a648
.L_ja_0200a5a4:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_02003180
	cmp r0, #0
	bgt .L_ja_0200a648
	cmp r0, #0
	bge .L_ja_0200a5ea
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	ldr r3, .L_ja_0200a664
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_02003200
	b .L_ja_0200a648
.L_ja_0200a5ea:
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r5, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldr r7, .L_ja_0200a670
	lsls r3, r3, #17
	ldr r0, .L_ja_0200a674
	adds r6, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r6, [r5, #16]
	cmp r2, r4
	ble .L_ja_0200a616
	adds r2, r4, #0
.L_ja_0200a616:
	ldr r0, .L_ja_0200a678
	cmp r2, r0
	bge .L_ja_0200a61e
	adds r2, r0, #0
.L_ja_0200a61e:
	subs r3, r1, r2
	ldr r1, .L_ja_0200a674
	str r3, [r5, #8]
	adds r3, r6, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_ja_0200a630
	adds r2, r4, #0
.L_ja_0200a630:
	cmp r2, r0
	bge .L_ja_0200a636
	adds r2, r0, #0
.L_ja_0200a636:
	subs r3, r6, r2
	str r3, [r5, #16]
	mov r2, r10
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_ja_0200a52c
.L_ja_0200a648:
	movs r3, #0
	str r3, [r5, #108]
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_ja_0200a654:
	mov r0, r10
	movs r3, #0
	strh r3, [r0, #4]
	sub sp, #-12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_ja_0200a664:
	.4byte gMainRoutineWork
.L_ja_0200a668:
	.4byte gData_020023c4 + 0x188
.L_ja_0200a66c:
	.4byte Func_020024bc
.L_ja_0200a670:
	.4byte 0x000fffff
.L_ja_0200a674:
	.4byte 0xfff80000
.L_ja_0200a678:
	.4byte 0xffff0000
	.endif
	.section .bss,"aw",%nobits
	.global gData_02003918
gData_02003918:
	.space 0x00000004
	.global Data_0200391c
Data_0200391c:
	.space 0x00000004
	.global gData_02003920
gData_02003920:
	.space 0x00000004
	.global gData_02003924
gData_02003924:
	.space 0x00000004
	.global gData_02003928
gData_02003928:
	.space 0x00000004
	.global Data_0200392c
Data_0200392c:
	.space 0x00000004
	.global Data_02003930
Data_02003930:
	.space 0x00000004
	.global gData_02003934
gData_02003934:
	.space 0x00000004
	.global gData_02003938
gData_02003938:
	.space 0x00000004
	.global gData_0200393c
gData_0200393c:
	.space 0x00000040
	.global gData_0200397c
gData_0200397c:
	.space 0x00000040
	.global Data_020039bc
Data_020039bc:
	.space 0x00000020
	.global Data_020039dc
Data_020039dc:
	.space 0x00000040
	.global Data_02003a1c
Data_02003a1c:
	.space 0x00000040
	.global gData_02003a5c
gData_02003a5c:
	.space 0x00000014
	.global Data_0200ba70
Data_0200ba70:
	.space 0x0000000c
	.global Data_02003a7c
Data_02003a7c:
	.space 0x00000002
	.global gData_02003a7e
gData_02003a7e:
	.space 0x00000002
	.global gData_02003a80
gData_02003a80:
	.space 0x00000002
	.global gData_02003a82
gData_02003a82:
