@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b63b0
	.thumb_func
Func_080b63b0:
	push	{lr}
	ldr	r3, [pc, #12]
	movs	r1, #16
	ldr	r0, [pc, #12]
	bl	Func_080072f0
	pop	{r1}
	bx	r1
	.4byte 0x03000164
	.4byte 0x02002224
