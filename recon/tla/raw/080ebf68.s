.syntax unified
	.thumb
	.global Func_080ebf68
	.thumb_func
Func_080ebf68:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, [r5]
	sub sp, #4
	cmp r0, #0
	beq .L_080ebf78
	bl Func_08020048
.L_080ebf78:
	mov r0, sp
	movs r3, #0
	str r3, [r0]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r5, #0
	adds r2, #18
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	add sp, #4
	pop {r5, pc}
