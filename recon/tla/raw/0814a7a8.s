.syntax unified
	.thumb
	.global Func_0814a7a8
	.thumb_func
Func_0814a7a8:
	push {lr}
	ldr r3, [r0, #24]
	cmp r3, #0
	bne .L_0814a7b8
	movs r1, #0
	bl Func_0814a814
	b .L_0814a7be
.L_0814a7b8:
	movs r1, #1
	bl Func_0814a814
.L_0814a7be:
	pop {pc}
