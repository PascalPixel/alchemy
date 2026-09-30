.syntax unified
	.thumb
	.global Func_080dd748
	.thumb_func
Func_080dd748:
	push {lr}
	movs r3, #63
.L_080dd74c:
	subs r3, #1
	cmp r3, #0
	bge .L_080dd74c
	pop {pc}
