@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08021360
	.thumb_func
Func_08021360:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #0
	cmp	r5, #8
	bhi.n	.L0
	movs	r0, #32
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L1
	ldr	r3, [pc, #16]
	lsls	r2, r5, #1
	ldrsh	r0, [r3, r2]
	b.n	.L0
.L1:
	ldr	r3, [pc, #12]
	lsls	r2, r5, #1
	ldrsh	r0, [r3, r2]
.L0:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x08037206
	.4byte 0x08037216
