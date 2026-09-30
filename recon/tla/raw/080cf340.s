.syntax unified
	.thumb
	.global Func_080cf340
	.thumb_func
Func_080cf340:
	push {lr}
	cmp r0, #0
	beq .L_080cf34a
	bl Func_080200c8
.L_080cf34a:
	pop {pc}
	.4byte 0x00004770
