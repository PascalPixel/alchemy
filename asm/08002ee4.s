@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08002ee4
	.thumb_func
Func_08002ee4:
	push	{r5, lr}
	ldr	r5, [pc, #12]
.L0:
	movs	r0, #1
	ldr	r3, [r5, #0]
	bl	Func_080030f8
	b.n	.L0
	movs	r0, r0
	.4byte 0x03001c94
