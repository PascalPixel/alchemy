@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f3844
	.thumb_func
Func_080f3844:
	push	{lr}
	ldr	r3, [pc, #12]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	strh	r0, [r3, #0]
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001ed0
