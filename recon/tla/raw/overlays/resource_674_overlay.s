.syntax unified
	.thumb
	.section .text.x0200b7d8,"ax",%progbits
	.global Func_020037d8
	.thumb_func
Func_020037d8:
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
	bl Func_02003940
	adds r4, r0, #0
	cmp r4, #0
	bne .L_0200b802
	b .L_0200b922
.L_0200b802:
	ldr r5, [r5]
	ldr r0, .L_0200b934
	str r5, [sp, #20]
	lsls r1, r5, #2
	ldrsb r2, [r0, r1]
	cmp r2, #0
	bge .L_0200b812
	negs r2, r2
.L_0200b812:
	adds r3, r1, #2
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_0200b81c
	negs r3, r3
.L_0200b81c:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #16]
	adds r3, r1, #1
	ldrsb r2, [r0, r3]
	cmp r2, #0
	bge .L_0200b82c
	negs r2, r2
.L_0200b82c:
	adds r3, r1, #3
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_0200b836
	negs r3, r3
.L_0200b836:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #12]
	ldr r3, [sp, #24]
	ldr r2, .L_0200b938
	ldr r1, .L_0200b93c
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
.L_0200b874:
	ldr r3, [sp, #20]
	ldr r2, .L_0200b934
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
	bge .L_0200b8e2
.L_0200b892:
	ldr r3, .L_0200b934
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
	bge .L_0200b8cc
.L_0200b8aa:
	adds r0, r4, #0
	add r1, sp, #28
	str r4, [sp, #0]
	bl Func_02003e2c
	ldr r4, [sp, #0]
	cmp r0, #2
	beq .L_0200b8f4
	ldr r3, [r5]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5]
	ldr r2, [sp, #16]
	adds r7, #1
	cmp r7, r2
	blt .L_0200b8aa
.L_0200b8cc:
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
	blt .L_0200b892
.L_0200b8e2:
	ldr r3, [r6]
	movs r1, #1
	add r3, r9
	str r3, [r6]
	ldr r3, [r6, #8]
	add r11, r1
	add r3, r10
	str r3, [r6, #8]
	b .L_0200b874
.L_0200b8f4:
	ldr r2, [sp, #8]
	movs r3, #0
	strb r3, [r2]
	mov r3, r11
	movs r0, #0
	cmp r3, #0
	beq .L_0200b924
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
	b .L_0200b924
.L_0200b922:
	movs r0, #0
.L_0200b924:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b934:
	.4byte gData_0200407c
.L_0200b938:
	.4byte gData_02004094
.L_0200b93c:
	.4byte 0xffff0000
	.section .bss,"aw",%nobits
	.global gData_02004f04
gData_02004f04:
	.space 0x0000000c
	.ifdef TLA_EDITION_JA
	.space 0x00000004 @ the next object sits on 16 bytes, and the Japanese image ends 4 short of that
	.endif
	.global gData_02004f10
gData_02004f10:
	.space 0x00000280
	.global gData_02005190
gData_02005190:
	.space 0x00000004
	.global gData_02005194
gData_02005194:
	.space 0x00000004
	.global gData_02005198
gData_02005198:
	.space 0x00000004
	.global gData_0200519c
gData_0200519c:
