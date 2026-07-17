@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080cd508
	.thumb_func
Func_080cd508:
	push	{lr}
	ldr	r3, [pc, #20]
	ldr	r0, [r3, #0]
	ldr	r3, [pc, #20]
	movs	r1, #8
	adds	r0, r0, r3
	ldr	r3, [pc, #16]
	bl	Func_080072f0
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007818
	.4byte 0x03000164
