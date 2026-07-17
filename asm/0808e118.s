@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808e118
	.thumb_func
Func_0808e118:
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #32]
	ldr	r3, [r3, #0]
	adds	r1, r3, r2
	movs	r2, #0
	strh	r2, [r1, #0]
	ldr	r2, [pc, #28]
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [pc, #20]
	bl	Func_0808e5d8
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x00000cb6
	.4byte 0x00000cb8
	.4byte 0x00002090
