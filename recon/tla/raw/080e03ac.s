.syntax unified
	.thumb
	.global Func_080e03ac
	.thumb_func
Func_080e03ac:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_080d1ed8
	cmp r0, #255
	bne .L_080e03be
	movs r0, #1
	negs r0, r0
	b .L_080e03c0
.L_080e03be:
	adds r0, r5, #0
.L_080e03c0:
	pop {r5, pc}
	.2byte 0x0000
