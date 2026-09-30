.syntax unified
	.thumb
	.global Func_08178680
	.thumb_func
Func_08178680:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r9, r0
	mov r12, r1
	mov lr, r3
	movs r7, #0
	cmp r3, #0
	beq .L_081786f2
	subs r5, r2, #1
	movs r2, #3
	movs r3, #1
	mov r8, r2
	mov r2, r12
	mov r10, r3
	movs r6, #2
	movs r0, #1
	subs r2, #1
	movs r1, #0
	mov r4, r9
.L_081786b0:
	lsls r3, r7, #1
	mov r11, r3
	mov r3, r10
	strb r3, [r4, #5]
	strb r3, [r4, #9]
	strb r3, [r4, #21]
	mov r3, r11
	strb r3, [r4]
	mov r3, r8
	strb r3, [r4, #13]
	adds r7, #1
	movs r3, #2
	strb r1, [r4, #4]
	strb r1, [r4, #6]
	strb r2, [r4, #8]
	strb r1, [r4, #16]
	strb r2, [r4, #18]
	strb r2, [r4, #20]
	strb r0, [r4, #1]
	strb r6, [r4, #2]
	strb r0, [r4, #12]
	strb r6, [r4, #14]
	strb r5, [r4, #7]
	strb r5, [r4, #17]
	strb r5, [r4, #19]
	add r8, r3
	adds r6, #2
	adds r0, #2
	add r2, r12
	add r1, r12
	adds r4, #24
	cmp r7, lr
	bne .L_081786b0
.L_081786f2:
	mov r2, lr
	lsls r3, r2, #1
	add r3, lr
	lsls r3, r3, #3
	add r3, r9
	movs r2, #0
	strb r2, [r3]
	strb r2, [r3, #1]
	strb r2, [r3, #2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
