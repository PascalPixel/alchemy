.syntax unified
	.thumb
	.global Func_080394e8
	.thumb_func
Func_080394e8:
	push	{lr}
	ldr	r3, [r0, #0]
	adds	r2, r0, #0
	cmp	r3, #0
	beq.n	.L_080394fa
.L_080394f2:
	adds	r2, r3, #0
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080394f2
.L_080394fa:
	str	r2, [r0, #4]
	pop	{pc}
	.2byte 0x0000
