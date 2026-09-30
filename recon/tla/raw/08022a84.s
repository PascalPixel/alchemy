.syntax unified
	.thumb
	.global Func_08022a84
	.thumb_func
Func_08022a84:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r1, #0
	cmp r5, #0
	beq .L_08022aca
	cmp r7, #3
	bhi .L_08022aca
	lsls r3, r7, #2
	adds r6, r3, #0
	adds r6, #40
	ldr r0, [r5, r6]
	cmp r0, #0
	beq .L_08022aca
	bl Func_08022d1c
	movs r3, #0
	adds r2, r7, #1
	str r3, [r5, r6]
	movs r0, #0
	cmp r2, #3
	bhi .L_08022ac4
	lsls r3, r2, #2
	adds r3, r3, r5
	adds r1, r3, #0
	adds r1, #40
.L_08022ab6:
	ldmia r1!, {r3}
	cmp r3, #0
	beq .L_08022abe
	adds r0, #1
.L_08022abe:
	adds r2, #1
	cmp r2, #3
	bls .L_08022ab6
.L_08022ac4:
	cmp r0, #0
	bne .L_08022aca
	strb r7, [r5, #27]
.L_08022aca:
	pop {r5, r6, r7, pc}
