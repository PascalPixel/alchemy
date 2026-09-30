.syntax unified
	.thumb
	.global Func_080227a0
	.thumb_func
Func_080227a0:
	push {lr}
	cmp r0, #0
	beq .L_080227ae
	ldr r3, [r0, #40]
	strb r1, [r3, #5]
	movs r3, #1
	strb r3, [r0, #25]
.L_080227ae:
	pop {pc}
