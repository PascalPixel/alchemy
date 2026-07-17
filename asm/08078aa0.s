@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08078aa0
	.thumb_func
Func_08078aa0:
	push	{lr}
	adds	r2, r0, #0
	ldr	r4, [pc, #36]
	movs	r0, #0
	cmp	r2, #127
	bgt.n	.L0
	ldrb	r3, [r4, r2]
	adds	r3, r3, r1
	cmp	r3, #0
	bge.n	.L1
	movs	r3, #0
	b.n	.L2
.L1:
	cmp	r3, #99
	ble.n	.L3
	movs	r3, #99
	movs	r0, #99
	b.n	.L2
.L3:
	adds	r0, r3, #0
.L2:
	strb	r3, [r4, r2]
.L0:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000380
