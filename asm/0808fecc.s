@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808fecc
	.thumb_func
Func_0808fecc:
	push	{lr}
	movs	r1, #168
	lsls	r1, r1, #3
	movs	r0, #31
	sub	sp, #4
	bl	Func_080048f4
	movs	r3, #0
	adds	r4, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	adds	r1, r4, #0
	ldr	r3, [pc, #12]
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r4, #0
	add	sp, #4
	pop	{r1}
	bx	r1
	.4byte 0x040000d4
	.4byte 0x85000150
