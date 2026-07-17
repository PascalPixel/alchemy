@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08021e28
	.thumb_func
Func_08021e28:
	push	{lr}
	ldr	r2, [pc, #20]
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r2, [pc, #16]
	movs	r0, #2
	movs	r1, #136
	bl	Func_0800307c
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x04000012
	.4byte 0x08021e15
