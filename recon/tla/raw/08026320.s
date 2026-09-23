.syntax unified
	.thumb
	.global Func_08026320
	.thumb_func
Func_08026320:
	push	{lr}
	cmp	r0, #0
	beq.n	.L_08026328
	str	r1, [r0, #108]
.L_08026328:
	pop	{pc}
	.2byte 0x0000
