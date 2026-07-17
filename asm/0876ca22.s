@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0876ca22
	.thumb_func
Func_0876ca22:
	push	{r3, r4, r6, lr}
	movs	r3, #175
.L0:
	strh	r0, [r7, #16]
	lsls	r6, r6, #16
	b.n	.L0
