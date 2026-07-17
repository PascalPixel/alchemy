@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808b64c
	.thumb_func
Func_0808b64c:
	push	{r5, r6, lr}
	ldr	r3, [pc, #32]
	adds	r5, r0, #0
	ldr	r6, [r3, #0]
	bl	Func_0808ba1c
	cmp	r0, #0
	beq.n	.L0
	bl	Func_080090d0
	lsls	r3, r5, #2
	adds	r3, #20
	movs	r2, #0
	str	r2, [r6, r3]
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
