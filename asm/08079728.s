@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08079728
	.thumb_func
Func_08079728:
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #140
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [r3, #0]
	ldr	r1, [pc, #24]
	adds	r2, r2, r0
	cmp	r2, r1
	ble.n	.L0
	adds	r2, r1, #0
.L0:
	cmp	r2, #0
	bge.n	.L1
	movs	r2, #0
.L1:
	str	r2, [r3, #0]
	adds	r0, r2, #0
	pop	{r1}
	bx	r1
	.4byte 0x02000240
	.4byte 0x000f423f
