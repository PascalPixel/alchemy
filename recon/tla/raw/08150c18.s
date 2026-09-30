.syntax unified
	.thumb
	.global Func_08150c18
	.thumb_func
Func_08150c18:
	push {r5, r6, r7, lr}
	mov lr, r1
	ldr r1, [r0, #16]
	movs r6, #0
	lsls r3, r1, #1
	adds r5, r3, #1
	mov r12, r6
	movs r4, #0
	cmp r5, #0
	beq .L_08150c70
	movs r2, #36
	ldrsh r3, [r0, r2]
	ldr r2, [r0, #12]
	cmp r2, r3
	beq .L_08150c48
.L_08150c36:
	adds r4, #1
	cmp r4, r5
	beq .L_08150c48
	lsls r3, r4, #1
	adds r3, #36
	ldrsh r3, [r0, r3]
	cmp r2, r3
	bne .L_08150c36
	mov r12, r4
.L_08150c48:
	movs r4, #0
	cmp r5, #0
	beq .L_08150c70
	b .L_08150c52
.L_08150c50:
	ldr r1, [r0, #16]
.L_08150c52:
	mov r2, r12
	adds r3, r2, r4
	subs r2, r3, r1
	cmp r2, #0
	blt .L_08150c6a
	ldr r3, [r0, #20]
	cmp r2, r3
	bge .L_08150c6a
	lsls r3, r6, #1
	mov r7, lr
	strh r2, [r3, r7]
	adds r6, #1
.L_08150c6a:
	adds r4, #1
	cmp r4, r5
	bne .L_08150c50
.L_08150c70:
	adds r0, r6, #0
	pop {r5, r6, r7, pc}
