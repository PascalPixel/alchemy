.syntax unified
	.thumb
	.global Func_080237e8
	.thumb_func
Func_080237e8:
	push {lr}
	cmp r0, #0
	beq .L_080237f4
	ldr r0, [r0, #80]
	bl Func_08022f24
.L_080237f4:
	pop {pc}
	.2byte 0x0000
