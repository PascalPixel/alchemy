@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080797fc
	.thumb_func
Func_080797fc:
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	adds	r6, r2, #0
	cmp	r7, #7
	ble.n	.L0
	bl	Func_080773d8
	adds	r0, #52
	ldrb	r1, [r0, #0]
	cmp	r1, #43
	bls.n	.L1
	movs	r1, #0
.L1:
	lsls	r3, r1, #1
	adds	r3, r3, r1
	ldr	r2, [pc, #92]
	lsls	r3, r3, #3
	adds	r3, r3, r2
	movs	r5, #0
	adds	r0, r6, #0
	adds	r1, r3, #4
.L2:
	ldrb	r2, [r1, #0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r5, #1
	adds	r1, #1
	stmia	r0!, {r3}
	cmp	r5, #3
	ble.n	.L2
	b.n	.L3
.L0:
	adds	r0, r6, #0
	adds	r1, #36
	movs	r5, #3
.L4:
	ldrb	r2, [r1, #0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	subs	r5, #1
	adds	r1, #1
	stmia	r0!, {r3}
	cmp	r5, #0
	bge.n	.L4
	cmp	r7, #7
	bgt.n	.L3
	movs	r5, #0
.L5:
	adds	r0, r7, #0
	bl	Func_08078ed8
	adds	r3, r5, #0
	adds	r3, #144
	adds	r0, #2
	ldrb	r2, [r0, r3]
	ldr	r3, [r6, #0]
	adds	r5, #1
	adds	r3, r3, r2
	stmia	r6!, {r3}
	cmp	r5, #3
	ble.n	.L5
.L3:
	movs	r0, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x08088e38
