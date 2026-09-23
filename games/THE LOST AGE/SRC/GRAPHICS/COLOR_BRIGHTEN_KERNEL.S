@ Saturating colour-buffer brighten that saves a backup copy, run from the
@ stack by its loader in COLOR_FADE_LOADERS.S (0x84000022: 34 words). Behind
@ its 4-byte Thumb entry (bx pc) the ARM body saves r5-r9 without lr
@ (push {r5, r6, r7, r8, r9} ... bx lr); GCC saves lr with any register.
@ After that entry its 132-byte body equals the brighten kernel of TBS
@ GRAPHICS/PALETTE/COLOR_KERNELS.S (0x08001fb8) byte for byte; TLA links it
@ apart from the other four.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: library — TBS/TLA common ARM colour-buffer kernels copied to the stack, brighten
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08001c08
	.global ColorBuffer_BrightenKernel
	.thumb_func
ColorBuffer_BrightenKernel:
.L_08001c08:
AlchemyUncredited_08001c08:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001c08:
	push	{r5, r6, r7, r8, r9}
	mov	r8, #64
	orr	r8, r8, r8, lsl #8
	orr	r8, r8, r8, lsl #16
	sub	ip, r8, r8, lsr #6
	add	r3, r0, r3
.L_08001c24:
	ldm	r0, {r4, r5, r6, r7}
	stmia	r2!, {r4, r5, r6, r7}
	add	r4, r4, r1
	and	r9, r4, r8
	bic	r4, r4, r9
	sub	r9, r9, r9, lsr #6
	orr	r4, r4, r9
	add	r5, r5, r1
	and	r9, r5, r8
	bic	r5, r5, r9
	sub	r9, r9, r9, lsr #6
	orr	r5, r5, r9
	add	r6, r6, r1
	and	r9, r6, r8
	bic	r6, r6, r9
	sub	r9, r9, r9, lsr #6
	orr	r6, r6, r9
	add	r7, r7, r1
	and	r9, r7, r8
	bic	r7, r7, r9
	sub	r9, r9, r9, lsr #6
	orr	r7, r7, r9
	stmia	r0!, {r4, r5, r6, r7}
	cmp	r0, r3
	bne	.L_08001c24
	pop	{r5, r6, r7, r8, r9}
	bx	lr
