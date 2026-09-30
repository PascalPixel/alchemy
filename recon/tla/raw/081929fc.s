.syntax unified
	.thumb
	.global Func_081929fc
	.thumb_func
Func_081929fc:
	push {lr}
	mov r12, r3
	mov r3, r9
	push {r3}
	mov r3, r12
	sub sp, #4
	mov r3, sp
	mov r2, r9
	str r2, [r3]
	adds r3, r2, #0
	adds r4, r3, #0
	subs r4, #16
	ldr r3, [r4]
	movs r0, #32
	ldr r3, [r3, #24]
	movs r1, #0
	cmp r3, #0
	bne .L_08192a24
	movs r0, #0
	b .L_08192a3c
.L_08192a24:
	adds r1, #1
	cmp r1, #32
	beq .L_08192a3c
	lsls r2, r1, #3
	ldr r3, [r4]
	subs r2, r2, r1
	lsls r2, r2, #2
	adds r2, #24
	ldr r3, [r3, r2]
	cmp r3, #0
	bne .L_08192a24
	adds r0, r1, #0
.L_08192a3c:
	cmp r0, #32
	bne .L_08192a44
	movs r0, #1
	negs r0, r0
.L_08192a44:
	add sp, #4
	pop {r3}
	mov r9, r3
	pop {pc}
