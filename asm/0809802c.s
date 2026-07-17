@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0809802c
	.thumb_func
Func_0809802c:
	push	{r5, lr}
	ldr	r3, [pc, #60]
	ldr	r3, [r3, #0]
	sub	sp, #12
	ldr	r5, [r3, #16]
	bl	Func_08097384
	adds	r0, r5, #0
	bl	Func_08098070
	adds	r5, r0, #0
	bl	Func_08098184
	cmp	r5, #0
	beq.n	.L0
	adds	r0, r5, #0
	movs	r1, #4
	bl	Func_08009080
	movs	r0, #30
	bl	Func_080030f8
.L0:
	bl	Func_0809748c
	adds	r0, r5, #0
	bl	Func_080981b0
	add	sp, #12
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
