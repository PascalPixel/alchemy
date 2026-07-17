@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f3804
	.thumb_func
Func_080f3804:
	push	{lr}
	ldr	r3, [pc, #24]
	adds	r4, r1, #0
	ldr	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L0
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r2, r1, r3
	adds	r3, r4, #0
	bl	Func_080f3078
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001ed0
