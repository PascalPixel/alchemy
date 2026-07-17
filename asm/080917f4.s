@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080917f4
	.thumb_func
Func_080917f4:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	bl	Func_08077150
	adds	r0, r6, #0
	bl	Func_08077150
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_08015428
	pop	{r5, r6}
	pop	{r0}
	bx	r0
