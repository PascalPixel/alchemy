@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800bc48
	.thumb_func
Func_0800bc48:
	push	{lr}
	adds	r1, r0, #0
	sub	sp, #4
	cmp	r1, #0
	beq.n	.L0
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	ldr	r2, [pc, #12]
	ldr	r3, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L0:
	add	sp, #4
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x85000006
	.4byte 0x040000d4
