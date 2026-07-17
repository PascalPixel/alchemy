@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801c930
	.thumb_func
Func_0801c930:
	push	{lr}
	ldr	r1, [pc, #24]
	movs	r0, #19
	bl	Func_080048f4
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #70
	strh	r3, [r2, #0]
	ldr	r2, [pc, #12]
	adds	r0, r0, r2
	strh	r3, [r0, #0]
	pop	{r0}
	bx	r0
	.4byte 0x00001004
	.4byte 0x00000352
