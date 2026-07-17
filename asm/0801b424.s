@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801b424
	.thumb_func
Func_0801b424:
	push	{r5, r6, lr}
	ldr	r3, [pc, #180]
	ldr	r5, [r3, #0]
	adds	r6, r0, #0
.L0:
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #232
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L0
	ldr	r2, [pc, #160]
	cmp	r6, r2
	beq.n	.L1
	ldr	r1, [pc, #156]
	ldr	r3, [r1, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	movs	r0, #111
	bl	Func_080f9010
	adds	r0, r5, #0
	bl	Func_0801b664
	b.n	.L3
.L2:
	ldr	r3, [r1, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	movs	r0, #111
	bl	Func_080f9010
	adds	r0, r5, #0
	bl	Func_0801b810
.L3:
	ldr	r3, [pc, #112]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r1, #231
	lsls	r1, r1, #2
	adds	r3, r5, r1
	adds	r1, #2
	ldrh	r2, [r3, #0]
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r6, r2, r3
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	ldrh	r3, [r3, #10]
	cmp	r3, #6
	bne.n	.L4
	cmp	r6, #0
	bne.n	.L5
	movs	r0, #112
	bl	Func_080f9010
	b.n	.L6
.L5:
	movs	r0, #113
	bl	Func_080f9010
	b.n	.L6
.L4:
	movs	r0, #112
	bl	Func_080f9010
.L6:
	adds	r0, r6, #0
	b.n	.L7
.L1:
	cmp	r6, #0
	beq.n	.L0
	ldr	r3, [pc, #36]
	ldr	r3, [r3, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	movs	r0, #113
	bl	Func_080f9010
	movs	r0, #1
	negs	r0, r0
.L7:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001e98
	.4byte 0x000003e7
	.4byte 0x03001b04
	.4byte 0x03001c94
