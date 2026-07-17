@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c1054
	.thumb_func
Func_080c1054:
	push	{r5, r6, r7, lr}
	sub	sp, #28
	mov	r5, sp
	movs	r0, #3
	adds	r1, r5, #0
	bl	Func_080b6c08
	cmp	r0, #0
	ble.n	.L0
	adds	r7, r5, #0
	movs	r6, #0
	adds	r5, r0, #0
.L1:
	ldrsh	r0, [r6, r7]
	movs	r1, #0
	subs	r5, #1
	bl	Func_080c0f98
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L1
.L0:
	add	sp, #28
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
