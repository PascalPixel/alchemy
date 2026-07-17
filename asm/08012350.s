@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08012350
	.thumb_func
Func_08012350:
	push	{r5, r6, lr}
	ldr	r3, [pc, #48]
	ldr	r5, [r3, #0]
	ldr	r3, [r5, #4]
	movs	r6, #0
	b.n	.L0
.L2:
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #150
	adds	r6, #1
	lsls	r3, r3, #1
	cmp	r6, r3
	bge.n	.L1
	ldr	r3, [r5, #4]
.L0:
	cmp	r3, #255
	bgt.n	.L2
	ldr	r3, [r5, #8]
	cmp	r3, #255
	bgt.n	.L2
.L1:
	movs	r3, #0
	str	r3, [r5, #12]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
