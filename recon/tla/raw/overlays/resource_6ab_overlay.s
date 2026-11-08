.syntax unified
	.thumb
	.section .text.x0200a148,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02002148
	.thumb_func
Func_02002148:
	push {r5, r6, r7, lr}
	mov r7, sl
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_ja_0200a0fc
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3, #0]
	sub sp, #12
	cmp r3, #0
	beq .L_ja_02009f8c
	b .L_ja_0200a0ec
.L_ja_02009f8c:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02001fcc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3, #0]
	bl Actor_GetById
	ldr r2, .L_ja_0200a100
	adds r5, r0, #0
	mov sl, r2
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	ldr r3, .L_ja_0200a104
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r1, #49
	bl Func_020041cc
.L_ja_02009fc4:
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldrb r0, [r3, #0]
	ldr r2, [r5, #16]
	bl Func_0200422c
	mov r6, sl
	mov r7, sl
	adds r6, #104
	adds r7, #106
	mov r1, sp
	cmp r0, #7
	bne .L_ja_0200a03c
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
.L_ja_02009ffa:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1, #0]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_0200424c
	cmp r0, #0
	beq .L_ja_0200a02a
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	b .L_ja_0200a0e0
.L_ja_0200a02a:
	movs r0, #1
	bl wait
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	ble .L_ja_02009ffa
	b .L_ja_0200a0e0
.L_ja_0200a03c:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #0]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_0200424c
	cmp r0, #0
	bgt .L_ja_0200a0e0
	cmp r0, #0
	bge .L_ja_0200a082
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	ldr r3, .L_ja_0200a0fc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3, #0]
	movs r1, #6
	negs r2, r2
	bl Func_0200436c
	b .L_ja_0200a0e0
.L_ja_0200a082:
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r5, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldr r7, .L_ja_0200a108
	lsls r3, r3, #17
	ldr r0, .L_ja_0200a10c
	adds r6, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r6, [r5, #16]
	cmp r2, r4
	ble .L_ja_0200a0ae
	adds r2, r4, #0
.L_ja_0200a0ae:
	ldr r0, .L_ja_0200a110
	cmp r2, r0
	bge .L_ja_0200a0b6
	adds r2, r0, #0
.L_ja_0200a0b6:
	subs r3, r1, r2
	ldr r1, .L_ja_0200a10c
	str r3, [r5, #8]
	adds r3, r6, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_ja_0200a0c8
	adds r2, r4, #0
.L_ja_0200a0c8:
	cmp r2, r0
	bge .L_ja_0200a0ce
	adds r2, r0, #0
.L_ja_0200a0ce:
	subs r3, r6, r2
	str r3, [r5, #16]
	mov r2, sl
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_ja_02009fc4
.L_ja_0200a0e0:
	movs r3, #0
	str r3, [r5, #108]
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_ja_0200a0ec:
	mov r0, sl
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov sl, r5
	pop {r5, r6, r7, pc}
.L_ja_0200a0fc:
	.4byte gMainRoutineWork
.L_ja_0200a100:
	.4byte 0x0200254c
.L_ja_0200a104:
	.4byte Func_02002120
.L_ja_0200a108:
	.4byte 0x000fffff
.L_ja_0200a10c:
	.4byte 0xfff80000
.L_ja_0200a110:
	.4byte 0xffff0000
	.endif
	.section .bss,"aw",%nobits
	.global Data_02004cec
Data_02004cec:
	.space 0x00000004
	.global gData_02004cf0
gData_02004cf0:
	.space 0x00000004
	.global gData_02004cf4
gData_02004cf4:
