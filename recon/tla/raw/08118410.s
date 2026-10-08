.syntax unified
	.thumb_func
	.arm
	.thumb
	.global ColorBuffer_Scale
	.thumb_func
ColorBuffer_Scale:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #124
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	mov	lr, sp
	.inst.n	0xf800
	add	sp, #124
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_ScaleKernelCode
	.4byte	0x8400001f
