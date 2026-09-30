.syntax unified
	.thumb
	.global Func_08191b7c
	.thumb_func
Func_08191b7c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r1, #0
	mov r9, r3
	lsls r3, r5, #1
	mov r8, r3
	adds r7, r0, #0
	mov r10, r2
	movs r6, #0
	mov r1, r8
	cmp r3, #63
	bls .L_08191b9c
	movs r1, #63
.L_08191b9c:
	movs r0, #63
	bl Math_DivU
	adds r1, r0, #0
	cmp r1, #5
	bhi .L_08191baa
	movs r1, #6
.L_08191baa:
	movs r0, #0
	cmp r5, #0
	beq .L_08191be2
	adds r2, r7, #0
.L_08191bb2:
	adds r6, r6, r1
	mov r3, r9
	muls r3, r6
	asrs r3, r3, #16
	cmp r3, #63
	ble .L_08191bc0
	movs r3, #63
.L_08191bc0:
	adds r0, #1
	strb r3, [r2]
	adds r2, #1
	cmp r0, r5
	bne .L_08191bb2
	movs r0, #0
	cmp r5, #0
	beq .L_08191be2
	adds r2, r5, r7
	subs r1, r2, #1
.L_08191bd4:
	ldrb r3, [r1]
	adds r0, #1
	strb r3, [r2]
	subs r1, #1
	adds r2, #1
	cmp r0, r5
	bne .L_08191bd4
.L_08191be2:
	mov r3, r10
	movs r6, #1
	cmp r3, #1
	beq .L_08191c14
	mov lr, r8
	adds r4, r5, #0
.L_08191bee:
	mov r3, lr
	movs r0, #0
	cmp r3, #0
	beq .L_08191c0c
	lsls r3, r4, #1
	mov r12, r8
	adds r1, r3, r7
	adds r2, r7, #0
.L_08191bfe:
	ldrb r3, [r2]
	adds r0, #1
	strb r3, [r1]
	adds r2, #1
	adds r1, #1
	cmp r0, r12
	bne .L_08191bfe
.L_08191c0c:
	adds r6, #1
	adds r4, r4, r5
	cmp r6, r10
	bne .L_08191bee
.L_08191c14:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
