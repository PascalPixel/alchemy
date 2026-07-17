@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083b11e4
	.thumb_func
Func_083b11e4:
	push	{r1, r3, r6, lr}
	ldr	r3, [sp, #504]
	ldrh	r6, [r1, #2]
	adds	r4, #207
	mov	pc, sp
