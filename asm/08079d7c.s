@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08079d7c
	.thumb_func
Func_08079d7c:
	push	{lr}
	subs	r0, #8
	cmp	r0, #49
	bls.n	.L0
	b.n	.L1
.L0:
	ldr	r2, [pc, #272]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08079e8c
	.4byte 0x08079e8c
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e58
	.4byte 0x08079e58
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e5c
	.4byte 0x08079e5c
	.4byte 0x08079e6c
	.4byte 0x08079e6c
	.4byte 0x08079e78
	.4byte 0x08079e7c
	.4byte 0x08079e60
	.4byte 0x08079e64
	.4byte 0x08079e68
	.4byte 0x08079e6c
	.4byte 0x08079e70
	.4byte 0x08079e74
	.4byte 0x08079e8c
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e78
	.4byte 0x08079e8c
	.4byte 0x08079e90
	.4byte 0x08079e7c
	.4byte 0x08079e80
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e90
	.4byte 0x08079e84
	.4byte 0x08079e88
	movs	r0, #70
	b.n	.L2
	movs	r0, #75
	b.n	.L2
	movs	r0, #30
	b.n	.L2
	movs	r0, #40
	b.n	.L2
	movs	r0, #45
	b.n	.L2
	movs	r0, #55
	b.n	.L2
	movs	r0, #25
	b.n	.L2
	movs	r0, #20
	b.n	.L2
	movs	r0, #65
	b.n	.L2
	movs	r0, #35
	b.n	.L2
	movs	r0, #50
	b.n	.L2
	movs	r0, #60
	b.n	.L3
	movs	r0, #90
	b.n	.L3
	movs	r0, #60
	b.n	.L2
.L1:
	movs	r0, #100
.L3:
	negs	r0, r0
.L2:
	pop	{r1}
	bx	r1
	.4byte 0x08079d90
