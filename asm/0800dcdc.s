@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800dcdc
	.thumb_func
Func_0800dcdc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	ldr	r2, [r6, #104]
	ldr	r3, [r2, #48]
	str	r3, [r6, #48]
	ldr	r3, [r2, #52]
	str	r3, [r6, #52]
	ldr	r3, [r6, #8]
	ldr	r1, [r2, #8]
	ldr	r2, [r2, #16]
	subs	r1, r1, r3
	ldr	r3, [r6, #16]
	subs	r2, r2, r3
	mov	sl, r2
	asrs	r3, r1, #16
	asrs	r2, r2, #16
	adds	r0, r3, #0
	muls	r0, r3
	adds	r3, r2, #0
	muls	r3, r2
	adds	r0, r0, r3
	ldr	r3, [pc, #92]
	mov	r8, r1
	bl	Func_080072f0
	adds	r7, r0, #0
	cmp	r7, #16
	ble.n	.L0
	adds	r5, r7, #0
	subs	r5, #16
	mov	r0, r8
	muls	r0, r5
	adds	r1, r7, #0
	bl	Func_080022ec
	adds	r1, r7, #0
	mov	r8, r0
	mov	r0, sl
	muls	r0, r5
	bl	Func_080022ec
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	add	r1, r8
	adds	r3, r3, r0
	ldr	r2, [r6, #12]
	adds	r0, r6, #0
	bl	Func_0800d14c
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_0800c300
	ldrh	r3, [r6, #4]
	adds	r3, #1
	strh	r3, [r6, #4]
	movs	r0, #1
	b.n	.L1
.L0:
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_0800c300
	movs	r0, #0
.L1:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x030001d8
