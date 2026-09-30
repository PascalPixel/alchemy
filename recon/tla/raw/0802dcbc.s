.syntax unified
	.thumb
	.global Func_0802dcbc
	.thumb_func
Func_0802dcbc:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	cmp r0, #0
	blt .L_0802dcca
	str r0, [r3, #4]
.L_0802dcca:
	cmp r1, #0
	blt .L_0802dcd0
	str r1, [r3, #8]
.L_0802dcd0:
	cmp r2, #0
	blt .L_0802dcd6
	str r2, [r3, #12]
.L_0802dcd6:
	pop {pc}
