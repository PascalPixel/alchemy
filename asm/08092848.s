@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08092848
	.thumb_func
Func_08092848:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r2, #0
	bl	Func_0808ba1c
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Func_0808ba1c
	adds	r1, r0, #0
	cmp	r6, #0
	beq.n	.L0
	cmp	r1, #0
	beq.n	.L0
	adds	r0, r6, #0
	bl	Func_08092878
	adds	r0, r7, #0
	bl	Func_0809163c
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
