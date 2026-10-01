.syntax unified
	.thumb
	.global Func_08191b10
	.thumb_func
Func_08191b10:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r1, #0
	lsls r1, r5, #1
	adds r7, r0, #0
	mov r8, r2
	movs r6, #0
	cmp r1, #63
	bls .L_08191b26
	movs r1, #63
.L_08191b26:
	movs r0, #63
	bl __udivsi3
	adds r1, r0, #0
	cmp r1, #5
	bhi .L_08191b34
	movs r1, #6
.L_08191b34:
	movs r0, #0
	cmp r5, #0
	beq .L_08191b54
	adds r2, r7, #0
.L_08191b3c:
	adds r6, r6, r1
	mov r3, r8
	muls r3, r6
	asrs r3, r3, #16
	cmp r3, #63
	ble .L_08191b4a
	movs r3, #63
.L_08191b4a:
	adds r0, #1
	strb r3, [r2]
	adds r2, #1
	cmp r0, r5
	bne .L_08191b3c
.L_08191b54:
	movs r6, #1
	adds r4, r5, #0
.L_08191b58:
	movs r0, #0
	cmp r5, #0
	beq .L_08191b6e
	adds r1, r4, #0
	adds r2, r7, #0
.L_08191b62:
	ldrb r3, [r2]
	adds r0, #1
	strb r3, [r2, r1]
	adds r2, #1
	cmp r0, r5
	bne .L_08191b62
.L_08191b6e:
	adds r6, #1
	adds r4, r4, r5
	cmp r6, #8
	bne .L_08191b58
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
