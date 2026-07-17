@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c1f50
	.thumb_func
Func_080c1f50:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	movs	r5, #0
	movs	r7, #49
.L3:
	adds	r0, r5, #0
	adds	r0, #128
	bl	Func_08077008
	adds	r2, r0, #0
	movs	r0, #149
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrb	r1, [r3, #0]
	cmp	r1, #1
	bne.n	.L0
	subs	r0, #2
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, r6
	bne.n	.L0
	ldrb	r3, [r2, #0]
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L1
	strb	r7, [r2, #0]
	strb	r0, [r2, r1]
	b.n	.L2
.L1:
	adds	r0, #1
	cmp	r0, #13
	bgt.n	.L2
	ldrb	r1, [r2, r0]
	cmp	r1, #0
	bne.n	.L1
	adds	r3, r0, #1
	strb	r7, [r2, r0]
	strb	r1, [r2, r3]
	b.n	.L2
.L0:
	adds	r5, #1
	cmp	r5, #5
	ble.n	.L3
.L2:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
