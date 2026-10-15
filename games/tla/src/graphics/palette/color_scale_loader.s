@ ColorBuffer_Scale(source, destination, scale, count): copies the ARM kernel
@ ColorBuffer_ScaleKernelCode by DMA into a buffer on the stack and calls it
@ there, through lr with a lone BL suffix, as color_backup_loaders.s does.
@ Handwritten (S1): it reads its three DMA words, the channel's address, the
@ kernel and the control, with one pc-relative block load.

.syntax unified
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
