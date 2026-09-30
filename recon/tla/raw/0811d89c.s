.syntax unified
	.thumb
	.global Func_0811d89c
	.thumb_func
Func_0811d89c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	ldrb r3, [r5]
	sub sp, #20
	movs r0, #1
	cmp r3, #7
	bls .L_0811d8b2
	movs r0, #2
.L_0811d8b2:
	mov r8, sp
	mov r1, r8
	bl Func_0811a39c
	mov r10, r0
	cmp r0, #0
	ble .L_0811d8fa
	movs r0, #31
	adds r0, r0, r5
	adds r2, r5, #0
	adds r3, r5, #3
	movs r7, #0
	movs r6, #1
	mov r12, r0
	adds r2, #17
	mov lr, r3
	mov r1, r8
	mov r4, r10
.L_0811d8d6:
	ldrh r0, [r1]
	subs r4, #1
	mov r8, r0
	mov r3, r8
	mov r0, lr
	strb r3, [r0]
	mov r0, r12
	movs r3, #1
	strb r7, [r2]
	adds r1, #2
	strb r6, [r0]
	add lr, r3
	strb r7, [r2, #28]
	add r12, r3
	strb r6, [r0, #28]
	adds r2, #1
	cmp r4, #0
	bne .L_0811d8d6
.L_0811d8fa:
	mov r3, r10
	strb r3, [r5, #1]
	movs r3, #103
	str r3, [r5, #76]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #181
	str r3, [r5, #88]
	add sp, #20
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
