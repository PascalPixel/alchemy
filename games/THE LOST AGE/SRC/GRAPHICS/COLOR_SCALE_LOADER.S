@ Loader that runs the colour-scale kernel of COLOR_SCALE_KERNEL.S from the
@ stack. It reads {DMA3 register base, 0x08118394, 0x8400001f} from its own
@ PC-relative block with add r4, pc, #16 and ldmia r4, {r4, r5, r7},
@ reserves 124 bytes of stack, has DMA3 copy the kernel there with
@ stmia r4!, {r5, r6, r7} and calls the copy (mov lr, sp; bl suffix).
@ Neither approved compiler emits the Thumb add r4, pc, #16, and neither
@ loads a block into its own base register. TBS has no such loader.
@ credit: handwritten — Thumb stack-copy loader of the colour-scale kernel
.syntax unified
	.thumb_func
	.set	.L_08118394, 0x08118394
	.arm
	.set AlchemyLoadAddress, 0x08118410
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
	.4byte	.L_08118394
	.4byte	0x8400001f
