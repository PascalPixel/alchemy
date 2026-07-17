@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08011bc8
	.thumb_func
Func_08011bc8:
	push	{lr}
	ldr	r0, [pc, #16]
	bl	Func_08004278
	movs	r0, #28
	bl	Func_08002dd8
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x08011bf5
