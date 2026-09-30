.syntax unified
	.thumb
	.global Func_08022d1c
	.thumb_func
Func_08022d1c:
	push {lr}
	adds r1, r0, #0
	sub sp, #4
	cmp r1, #0
	beq .L_08022d3c
	mov r0, sp
	movs r3, #0
	str r3, [r0]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r2, #6
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08022d3c:
	add sp, #4
	pop {pc}
