@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b6cb0
	.thumb_func
Func_080b6cb0:
	push	{r5, lr}
	sub	sp, #28
	mov	r5, sp
	adds	r1, r5, #0
	movs	r0, #3
	bl	Func_080b6c08
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_080b7b6c
	add	sp, #28
	pop	{r5}
	pop	{r0}
	bx	r0
