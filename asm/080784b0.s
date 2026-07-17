@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080784b0
	.thumb_func
Func_080784b0:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	Func_08077394
	lsls	r5, r5, #1
	adds	r5, #216
	ldrh	r0, [r0, r5]
	ldr	r3, [pc, #20]
	ands	r3, r0
	lsrs	r0, r0, #11
	adds	r0, #1
	cmp	r3, #0
	bne.n	.L0
	movs	r0, #0
.L0:
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000001ff
