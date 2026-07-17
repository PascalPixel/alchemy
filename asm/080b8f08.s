@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b8f08
	.thumb_func
Func_080b8f08:
	push	{r5, r6, lr}
	movs	r2, #10
	ldrsh	r5, [r0, r2]
	adds	r0, r5, #0
	sub	sp, #28
	bl	Func_08077008
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	adds	r0, r5, #0
	cmp	r3, #0
	bne.n	.L0
	cmp	r5, #127
	ble.n	.L1
	mov	r6, sp
	movs	r0, #2
	b.n	.L2
.L1:
	mov	r6, sp
	movs	r0, #1
.L2:
	adds	r1, r6, #0
	bl	Func_080b6b40
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L3
	movs	r0, #128
	lsls	r0, r0, #1
	b.n	.L0
.L3:
	bl	Func_08004458
	adds	r3, r5, #0
	muls	r3, r0
	lsrs	r3, r3, #16
	lsls	r3, r3, #1
	ldrsh	r0, [r6, r3]
.L0:
	add	sp, #28
	pop	{r5, r6}
	pop	{r1}
	bx	r1
