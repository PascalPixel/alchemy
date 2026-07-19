.syntax unified
.text
	.thumb
	.global Fragment_08021dfa
	.thumb_func
Fragment_08021dfa:
	movs	r0, r0
	ldr	r1, [pc, #16]
	movs	r3, #4
	ldrh	r2, [r1, #0]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #3
	orrs	r3, r2
	strh	r3, [r1, #0]
	bx	lr
	.inst.n 0x0000
	.4byte 0x0400000a
	.inst.n 0x4903
	.inst.n 0x2304
	.inst.n 0x880a
	.inst.n 0x425b
	.inst.n 0x4013
	.inst.n 0x800b
	.inst.n 0x4770
	.inst.n 0x0000
	.inst.n 0x000a
	.inst.n 0x0400
