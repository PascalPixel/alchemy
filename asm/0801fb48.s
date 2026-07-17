@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801fb48
	.thumb_func
Func_0801fb48:
	push	{r5, lr}
	movs	r1, #8
	movs	r2, #12
	movs	r3, #2
	ldr	r0, [pc, #76]
	bl	Func_08017658
	b.n	.L0
.L1:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	bl	Func_08017364
	cmp	r0, #0
	beq.n	.L1
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	movs	r3, #1
	bl	Func_08028df4
	cmp	r0, #0
	beq.n	.L2
	bl	Func_08019a54
	b.n	.L3
.L2:
	bl	Func_08019a54
	movs	r0, #85
	bl	Func_080f9010
	bl	Func_0801faa8
	adds	r5, r0, #0
	cmp	r5, #0
	blt.n	.L3
	ldr	r0, [pc, #16]
	movs	r1, #1
	bl	Func_0801776c
.L3:
	adds	r0, r5, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x00000014
	.4byte 0x00000017
