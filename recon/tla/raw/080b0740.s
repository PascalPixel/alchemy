.syntax unified
	.thumb
	.global Func_080b0740
	.thumb_func
Func_080b0740:
	push	{lr}
	cmp	r0, #5
	beq.n	.L_080b0752
	cmp	r0, #56
	beq.n	.L_080b0752
	cmp	r0, #57
	beq.n	.L_080b0752
	cmp	r0, #73
	bne.n	.L_080b0756
.L_080b0752:
	movs	r0, #1
	b.n	.L_080b0758
.L_080b0756:
	movs	r0, #0
.L_080b0758:
	pop	{pc}
	.2byte 0x0000
