@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808e96c
	.thumb_func
Func_0808e96c:
	push	{lr}
	adds	r1, r0, #0
	sub	sp, #4
	lsls	r1, r1, #16
	mov	r2, sp
	lsrs	r1, r1, #16
	ldr	r0, [pc, #16]
	bl	Func_0808e4b4
	adds	r3, r0, #0
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	add	sp, #4
	pop	{r1}
	bx	r1
	.4byte 0x70000005
