.syntax unified
	.thumb
	.global Func_0816ae40
	.thumb_func
Func_0816ae40:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r7, r1, #0
	movs r1, #0
	str r0, [sp, #4]
	adds r6, r2, #0
	mov r10, r3
	mov r11, r1
	cmp r3, #0
	beq .L_0816aec4
	lsls r2, r6, #1
	lsls r3, r3, #2
	str r2, [sp, #0]
	subs r3, #2
	mov r9, r1
	mov r8, r1
	mov lr, r3
.L_0816ae6e:
	movs r3, #0
	mov r12, r3
	cmp r6, #0
	beq .L_0816aeb0
	mov r3, lr
	muls r3, r6
	mov r1, r8
	lsls r2, r1, #1
	adds r0, r2, r7
	ldr r1, [sp, #4]
	lsls r2, r6, #1
	adds r5, r3, r7
	adds r3, r2, r3
	add r2, r9
	adds r3, r3, r7
	adds r2, r2, r7
	add r1, r8
	subs r4, r3, #1
	subs r2, #1
.L_0816ae94:
	ldrb r3, [r1]
	adds r1, #1
	strb r3, [r0]
	strb r3, [r2]
	strb r3, [r5]
	strb r3, [r4]
	movs r3, #1
	add r12, r3
	adds r0, #1
	subs r2, #1
	adds r5, #1
	subs r4, #1
	cmp r12, r6
	bne .L_0816ae94
.L_0816aeb0:
	ldr r1, [sp, #0]
	movs r2, #2
	movs r3, #1
	negs r2, r2
	add r11, r3
	add r9, r1
	add r8, r6
	add lr, r2
	cmp r11, r10
	bne .L_0816ae6e
.L_0816aec4:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
