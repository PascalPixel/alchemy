@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08011be0
	.thumb_func
Func_08011be0:
	push	{lr}
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #8]
	bl	Func_080041d8
	pop	{r0}
	bx	r0
	.4byte 0x08011bf5
