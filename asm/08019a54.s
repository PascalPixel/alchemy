@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08019a54
	.thumb_func
Func_08019a54:
	push	{r5, r6, lr}
	ldr	r3, [pc, #68]
	movs	r2, #196
	ldr	r3, [r3, #0]
	lsls	r2, r2, #3
	adds	r5, r3, r2
	movs	r6, #0
.L1:
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L0
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bne.n	.L0
	ldrh	r2, [r0, #22]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	ldrh	r3, [r0, #20]
	cmp	r3, #0
	beq.n	.L0
	movs	r1, #2
	ands	r1, r2
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	bl	Func_08016418
.L0:
	adds	r6, #1
	adds	r5, #40
	cmp	r6, #3
	bne.n	.L1
	movs	r0, #10
	bl	Func_080030f8
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
