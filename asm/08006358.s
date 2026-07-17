@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08006358
	.thumb_func
Func_08006358:
	push	{lr}
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #24]
	movs	r0, #7
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl	Func_0800307c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl	Func_0800307c
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001cb0
.L0:
	pop	{r0}
	bx	r0
