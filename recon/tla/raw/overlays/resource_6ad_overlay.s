.syntax unified
	.thumb
	.section .text.x0200a97c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_0200297c
	.thumb_func
Func_0200297c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_ja_0200a9bc
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #12
	cmp r3, #0
	beq .L_ja_0200a84c
	b .L_ja_0200a9ac
.L_ja_0200a84c:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02002800
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r2, .L_ja_0200a9c0
	adds r5, r0, #0
	mov r10, r2
	bl Func_02005560
	movs r0, #0
	bl Func_02005648
	ldr r3, .L_ja_0200a9c4
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r1, #49
	bl Func_02005480
.L_ja_0200a884:
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldrb r0, [r3]
	ldr r2, [r5, #16]
	bl Func_020054d8
	mov r6, r10
	mov r7, r10
	adds r6, #104
	adds r7, #106
	mov r1, sp
	cmp r0, #7
	bne .L_ja_0200a8fc
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
.L_ja_0200a8ba:
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
	bl Func_020054f8
	cmp r0, #0
	beq .L_ja_0200a8ea
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	b .L_ja_0200a9a0
.L_ja_0200a8ea:
	movs r0, #1
	bl wait
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	ble .L_ja_0200a8ba
	b .L_ja_0200a9a0
.L_ja_0200a8fc:
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
	bl Func_020054f8
	cmp r0, #0
	bgt .L_ja_0200a9a0
	cmp r0, #0
	bge .L_ja_0200a942
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	ldr r3, .L_ja_0200a9bc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_020055a8
	b .L_ja_0200a9a0
.L_ja_0200a942:
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r5, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldr r7, .L_ja_0200a9c8
	lsls r3, r3, #17
	ldr r0, .L_ja_0200a9cc
	adds r6, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r6, [r5, #16]
	cmp r2, r4
	ble .L_ja_0200a96e
	adds r2, r4, #0
.L_ja_0200a96e:
	ldr r0, .L_ja_0200a9d0
	cmp r2, r0
	bge .L_ja_0200a976
	adds r2, r0, #0
.L_ja_0200a976:
	subs r3, r1, r2
	ldr r1, .L_ja_0200a9cc
	str r3, [r5, #8]
	adds r3, r6, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_ja_0200a988
	adds r2, r4, #0
.L_ja_0200a988:
	cmp r2, r0
	bge .L_ja_0200a98e
	adds r2, r0, #0
.L_ja_0200a98e:
	subs r3, r6, r2
	str r3, [r5, #16]
	mov r2, r10
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_ja_0200a884
.L_ja_0200a9a0:
	movs r3, #0
	str r3, [r5, #108]
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_ja_0200a9ac:
	mov r0, r10
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_ja_0200a9bc:
	.4byte gMainRoutineWork
.L_ja_0200a9c0:
	.4byte gData_020023c4 + 0x188
.L_ja_0200a9c4:
	.4byte Func_02002954
.L_ja_0200a9c8:
	.4byte 0x000fffff
.L_ja_0200a9cc:
	.4byte 0xfff80000
.L_ja_0200a9d0:
	.4byte 0xffff0000
	.endif
	.section .bss,"aw",%nobits
	.global Data_02007064
Data_02007064:
	.space 0x00000004
	.global gData_02007068
gData_02007068:
	.space 0x00000020
	.global gData_02007088
gData_02007088:
	.space 0x00000004
	.global gData_0200708c
gData_0200708c:
	.space 0x0000001c
	.global gData_020070a8
gData_020070a8:
	.space 0x00000420
	.global gData_020074c8
gData_020074c8:
	.space 0x00000004
	.global gData_020074cc
gData_020074cc:
	.space 0x00000002
	.global gData_020074ce
gData_020074ce:
	.space 0x00000002
	.global gData_020074d0
gData_020074d0:
	.space 0x00000040
	.global Data_0200f510
Data_0200f510:
	.space 0x000000c0
	.global gData_020075d0
gData_020075d0:
	.space 0x00000002
	.global gData_020075d2
gData_020075d2:
