@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800bf34
	.thumb_func
Func_0800bf34:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	str	r0, [sp, #0]
	mov	fp, r1
	movs	r7, #0
.L2:
	mov	r3, fp
	cmp	r3, #0
	ble.n	.L0
	adds	r3, r7, #1
	mov	r9, r3
	adds	r3, r7, #2
	mov	sl, r3
	ldr	r5, [sp, #0]
	adds	r3, r7, #3
	mov	r8, r3
	mov	r6, fp
.L1:
	ldr	r0, [r5, #0]
	adds	r1, r7, #0
	bl	Func_0800be70
	ldr	r0, [r5, #0]
	mov	r1, r9
	bl	Func_0800be70
	ldr	r0, [r5, #0]
	mov	r1, sl
	bl	Func_0800be70
	subs	r6, #1
	ldmia	r5!, {r0}
	mov	r1, r8
	bl	Func_0800be70
	cmp	r6, #0
	bne.n	.L1
.L0:
	movs	r0, #1
	adds	r7, #4
	bl	Func_080030f8
	cmp	r7, #127
	bls.n	.L2
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
