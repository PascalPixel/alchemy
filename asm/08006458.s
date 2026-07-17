@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08006458
	.thumb_func
Func_08006458:
	push	{r5, r6, lr}
	ldr	r2, [pc, #36]
	ldr	r3, [r2, #0]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r6, r2, #0
.L1:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [pc, #20]
	adds	r5, #1
	cmp	r5, r3
	bhi.n	.L0
	ldr	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L1
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x02002080
	.4byte 0x000927bf
