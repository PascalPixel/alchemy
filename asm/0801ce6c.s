@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801ce6c
	.thumb_func
Func_0801ce6c:
	push	{lr}
	ldr	r2, [pc, #28]
	adds	r0, r0, r2
	ldrh	r3, [r0, #0]
	movs	r2, #128
	adds	r3, #1
	strh	r3, [r0, #0]
	lsls	r2, r2, #10
	lsls	r3, r3, #16
	cmp	r3, r2
	bls.n	.L0
	movs	r3, #0
	strh	r3, [r0, #0]
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000574
