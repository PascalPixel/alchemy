.syntax unified
	.thumb
	.global Func_0804257c
	.thumb_func
Func_0804257c:
	push {lr}
	cmp r0, #0
	beq .L_08042586
	mvns r3, r1
	strb r3, [r0, #15]
.L_08042586:
	pop {pc}
