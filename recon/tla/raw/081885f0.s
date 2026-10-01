.syntax unified
	.thumb
	.global Func_081885f0
	.thumb_func
Func_081885f0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #28
	movs r3, #15
	str r1, [sp, #20]
	movs r1, #0
	str r1, [sp, #16]
	mov r11, r3
	subs r1, #36
	movs r3, #0
	str r0, [sp, #24]
	str r1, [sp, #12]
	str r3, [sp, #8]
	adds r4, r2, #0
.L_08188616:
	movs r1, #0
	mov r10, r1
	cmp r4, #0
	beq .L_081886de
	ldr r3, [sp, #8]
	mov r9, r1
	mov r8, r3
.L_08188624:
	movs r0, #128
	adds r1, r4, #0
	lsls r0, r0, #9
	str r4, [sp, #0]
	bl __divsi3
	mov r6, r10
	muls r6, r0
	adds r0, r6, #0
	bl Trig_Sin
	mov r7, r8
	ldr r1, [sp, #24]
	lsls r3, r0, #1
	add r7, r10
	adds r3, r3, r0
	lsls r5, r7, #2
	adds r5, r5, r1
	lsrs r3, r3, #11
	strb r3, [r5]
	add r3, sp, #12
	ldrb r3, [r3]
	adds r0, r6, #0
	strb r3, [r5, #1]
	bl Trig_Cos
	lsls r3, r0, #1
	adds r3, r3, r0
	lsrs r3, r3, #11
	strb r3, [r5, #2]
	ldr r1, [sp, #16]
	ldr r4, [sp, #0]
	cmp r1, #0
	bgt .L_081886d4
	ldr r3, [sp, #20]
	lsls r5, r7, #1
	adds r5, r5, r7
	lsls r5, r5, #3
	adds r5, r5, r3
	mov r1, r10
	lsls r2, r1, #3
	adds r6, r5, #0
	mov r1, r9
	adds r6, #12
	strb r1, [r5, #5]
	adds r3, r2, #7
	mov r1, r11
	mov r7, r10
	strb r3, [r5, #4]
	strb r3, [r5, #6]
	strb r1, [r5, #7]
	strb r2, [r5, #8]
	strb r1, [r5, #9]
	adds r7, #1
	mov r1, r9
	strb r3, [r6, #6]
	mov r3, r11
	strb r3, [r6, #9]
	strb r2, [r6, #4]
	strb r1, [r6, #5]
	strb r1, [r6, #7]
	strb r2, [r6, #8]
	adds r1, r4, #0
	adds r0, r7, #0
	bl __modsi3
	ldr r4, [sp, #0]
	mov r1, r8
	adds r2, r0, r1
	adds r0, r0, r4
	add r0, r8
	strb r2, [r5]
	strb r0, [r5, #1]
	adds r1, r4, #0
	mov r0, r10
	str r2, [sp, #4]
	bl __modsi3
	ldr r4, [sp, #0]
	ldr r2, [sp, #4]
	adds r3, r0, r4
	add r3, r8
	add r0, r8
	strb r3, [r5, #2]
	strb r0, [r6]
	strb r2, [r6, #1]
	strb r3, [r6, #2]
	b .L_081886d8
.L_081886d4:
	mov r7, r10
	adds r7, #1
.L_081886d8:
	mov r10, r7
	cmp r10, r4
	bne .L_08188624
.L_081886de:
	ldr r3, [sp, #12]
	ldr r1, [sp, #8]
	adds r3, #18
	str r3, [sp, #12]
	ldr r3, [sp, #16]
	adds r1, r1, r4
	adds r3, #1
	str r1, [sp, #8]
	str r3, [sp, #16]
	cmp r3, #2
	bne .L_08188616
	ldr r1, [sp, #20]
	lsls r3, r4, #1
	adds r3, r3, r4
	lsls r3, r3, #3
	movs r2, #0
	adds r3, r3, r1
	strb r2, [r3]
	strb r2, [r3, #1]
	strb r2, [r3, #2]
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
