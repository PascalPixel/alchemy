.syntax unified
	.thumb
	.global Func_0814eec4
	.thumb_func
Func_0814eec4:
	push {lr}
	ldr r3, [r0, #24]
	cmp r3, #0
	bne .L_0814eed4
	movs r1, #3
	bl Func_0814ef44
	b .L_0814eeda
.L_0814eed4:
	movs r1, #4
	bl Func_0814ef44
.L_0814eeda:
	pop {pc}
