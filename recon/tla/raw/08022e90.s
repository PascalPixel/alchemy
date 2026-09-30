.syntax unified
	.thumb
	.global Func_08022e90
	.thumb_func
Func_08022e90:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	sub sp, #4
	cmp r7, #0
	beq .L_08022ed4
	ldrb r2, [r7, #17]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_08022eaa
	ldrb r0, [r7, #16]
	bl Func_08014274
.L_08022eaa:
	adds r5, r7, #0
	adds r5, #40
	movs r6, #3
.L_08022eb0:
	ldmia r5!, {r0}
	subs r6, #1
	bl Func_08022d1c
	cmp r6, #0
	bge .L_08022eb0
	mov r0, sp
	movs r3, #0
	str r3, [r0]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r7, #0
	adds r2, #14
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08022ed4:
	add sp, #4
	pop {r5, r6, r7, pc}
