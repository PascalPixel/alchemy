@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08011b00
	.thumb_func
Func_08011b00:
	push	{r5, r6, lr}
	movs	r1, #180
	movs	r0, #28
	bl	Func_080048f4
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r0, #0
	movs	r4, #0
.L1:
	str	r4, [r1, #0]
	strh	r4, [r1, #4]
	strh	r4, [r1, #6]
	strh	r4, [r1, #8]
	strh	r4, [r1, #10]
	movs	r3, #0
.L0:
	lsrs	r2, r3, #16
	lsls	r3, r2, #1
	movs	r6, #128
	adds	r2, #1
	adds	r3, #12
	lsls	r2, r2, #16
	lsls	r6, r6, #13
	strh	r4, [r1, r3]
	adds	r3, r2, #0
	cmp	r2, r6
	bne.n	.L0
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r6, #128
	adds	r3, r0, r2
	lsls	r6, r6, #11
	adds	r1, #44
	adds	r0, r3, #0
	cmp	r3, r6
	bne.n	.L1
	adds	r2, r5, #0
	adds	r2, #176
	movs	r3, #0
	strh	r3, [r2, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
