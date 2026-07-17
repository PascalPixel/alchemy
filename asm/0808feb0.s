@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808feb0
	.thumb_func
Func_0808feb0:
	push	{lr}
	ldr	r0, [pc, #16]
	bl	Func_080042c8
	ldr	r0, [pc, #12]
	bl	Func_080042c8
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0808f52d
	.4byte 0x0808f499
