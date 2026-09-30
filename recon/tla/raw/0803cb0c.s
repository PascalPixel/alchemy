.syntax unified
	.thumb
	.global Func_0803cb0c
	.thumb_func
Func_0803cb0c:
	push {lr}
	ldr r3, [r0]
	cmp r3, #0
	beq .L_0803cb18
	movs r3, #0
	str r3, [r0]
.L_0803cb18:
	pop {pc}
	.2byte 0x0000
