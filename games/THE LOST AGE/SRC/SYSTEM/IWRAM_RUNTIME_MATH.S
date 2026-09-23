@ Word clearing and filling, integer square root and bit-run expansion of
@ TLA's resident runtime, each routine behind a 4-byte Thumb entry (bx pc).
@ The clearer sets the fill value to zero and runs into the fill through the
@ fill's entry, which ARM executes as andeq r4, r0, #0x1e00000; the fill
@ jumps into its unrolled stmia run with add pc, pc, ip, lsr #3 and saves
@ r5-r9 without lr; the square root returns early with bxmi lr; the
@ expander stores r2 at the bytes that the set bits of 14 halfwords select,
@ 16 bytes per halfword. GCC never jumps into unrolled code. Apart from the
@ entries these 236 bytes equal TBS SYSTEM/RUNTIME_MATH.S (0x080008d4) byte
@ for byte. The entries are TLA's own and carry no credit.
@ credit: library — TBS/TLA common IWRAM runtime fill, square root and bit-run helpers
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08000810
	.set AlchemyRunAddress, 0x03000258
AlchemyUncredited_08000810:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08000810:
	mov	r2, #0
AlchemyUncredited_08000818:
	andeq	r4, r0, #120, 14
AlchemyUncreditedEnd_08000818:
	push	{r5, r6, r7, r8, r9}
	mov	r3, r2
	mov	r4, r2
	mov	r5, r2
	mov	r6, r2
	mov	r7, r2
	mov	r8, r2
	mov	r9, r2
	ands	ip, r1, #224
	rsb	ip, ip, #224
	add	pc, pc, ip, lsr #3
.L_03000290:
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	subs	r1, r1, #0x100
	bpl	.L_03000290
	ands	r1, r1, #28
	beq	.L_030002cc
.L_030002c0:
	stmia	r0!, {r2}
	subs	r1, r1, #4
	bgt	.L_030002c0
.L_030002cc:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
	.thumb
AlchemyUncredited_0800088c:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_0800088c:
	movs	r1, r0
	mov	r0, #0
	bxmi	lr
	mov	r2, #0x8000
.L_030002e8:
	add	r0, r0, r2
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2
	add	r0, r0, r2, lsr #1
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2, lsr #1
	lsrs	r2, r2, #2
	bne	.L_030002e8
	bx	lr
	.thumb
AlchemyUncredited_080008cc:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080008cc:
	add	ip, r0, #28
.L_0300031c:
	mov	r4, r1
	ldrh	r3, [r0], #2
	bic	r3, r3, #3
	lsls	r3, r3, #17
	bcc	.L_0300033c
.L_03000330:
	strb	r2, [r4], #1
.L_03000334:
	lsls	r3, r3, #1
	bcs	.L_03000330
.L_0300033c:
	add	r4, r4, #1
	bne	.L_03000334
	add	r1, r1, #16
	cmp	r0, ip
	bne	.L_0300031c
	bx	lr
