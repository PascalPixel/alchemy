@ Loaders that run the backup fade kernels and the brighten kernel from the
@ stack. Each reads {DMA3 register base, kernel address, control} from its
@ own PC-relative block with add r4, pc and ldmia r4, {r4, r5, r7}, reserves
@ exactly the kernel's size on the stack, has DMA3 copy the kernel there with
@ stmia r4!, {r5, r6, r7} and calls the copy. Neither approved compiler emits
@ an add rd, pc address form; both load constants one ldr at a time.
@ credit: handwritten — Thumb stack-copy loaders of the colour-buffer fade kernels
.syntax unified
	.set ColorBuffer_ScaleThreeQuartersKernel, 0x08001a14
	.set ColorBuffer_HalveKernel, 0x08001ad4
	.set ColorBuffer_DarkenKernel, 0x08001b54
	.set ColorBuffer_BrightenKernel, 0x08001c08
	.thumb
	.set AlchemyLoadAddress, 0x08001c90
	.global ColorBuffer_ScaleThreeQuarters
	.thumb_func
ColorBuffer_ScaleThreeQuarters:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #192
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	mov	lr, sp
	.inst.n	0xf800
	add	sp, #192
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_ScaleThreeQuartersKernel
	.4byte	0x84000030
	.global ColorBuffer_Halve
	.thumb_func
ColorBuffer_Halve:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #128
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	mov	lr, sp
	.inst.n	0xf800
	add	sp, #128
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_HalveKernel
	.4byte	0x84000020
	.global ColorBuffer_Brighten
	.thumb_func
ColorBuffer_Brighten:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #136
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	mov	lr, sp
	.inst.n	0xf800
	add	sp, #136
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_BrightenKernel
	.4byte	0x84000022
	.global ColorBuffer_Darken
	.thumb_func
ColorBuffer_Darken:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #180
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	mov	lr, sp
	.inst.n	0xf800
	add	sp, #180
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_DarkenKernel
	.4byte	0x8400002d
