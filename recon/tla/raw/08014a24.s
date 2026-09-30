.syntax unified
	.thumb
	.global Func_08014a24
	.thumb_func
Func_08014a24:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #0
	adds r5, r0, #0
	mov r8, r3
	cmp r5, #0
	bge .L_08014a3a
	movs r3, #1
	negs r5, r5
	mov r8, r3
.L_08014a3a:
	ldr r2, .L_08014aa0
	movs r3, #0
	strb r3, [r2, #11]
	movs r6, #10
	cmp r5, #0
	beq .L_08014a6a
	adds r7, r2, #0
	adds r7, #10
.L_08014a4a:
	adds r0, r5, #0
	ldr r1, .L_08014aa4
	bl Math_UnsignedMulHigh
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	subs r5, r5, r3
	adds r3, r5, #0
	adds r3, #48
	adds r5, r0, #0
	strb r3, [r7]
	subs r6, #1
	subs r7, #1
	cmp r5, #0
	bne .L_08014a4a
.L_08014a6a:
	cmp r6, #10
	bne .L_08014a76
	ldr r2, .L_08014aa0
	movs r3, #48
	strb r3, [r2, r6]
	movs r6, #9
.L_08014a76:
	mov r3, r8
	cmp r3, #0
	beq .L_08014a84
	ldr r2, .L_08014aa0
	movs r3, #45
	strb r3, [r2, r6]
	subs r6, #1
.L_08014a84:
	cmp r6, #0
	blt .L_08014a98
	ldr r3, .L_08014aa0
	movs r1, #32
	adds r2, r6, r3
	mov r12, r3
.L_08014a90:
	strb r1, [r2]
	subs r2, #1
	cmp r2, r12
	bge .L_08014a90
.L_08014a98:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08014aa0:
	.4byte Data_03001250
.L_08014aa4:
	.4byte 0x1999999a
