@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a33d4
	.thumb_func
Func_080a33d4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r0
	movs	r3, #168
	mov	r6, r8
	sub	sp, #4
	adds	r7, r1, #0
	movs	r5, #0
	mov	sl, r3
	adds	r6, #72
.L0:
	mov	r3, sl
	str	r3, [sp, #0]
	adds	r1, r5, #0
	movs	r0, #2
	adds	r2, r7, #0
	movs	r3, #248
	bl	Func_080150d0
	adds	r5, #1
	stmia	r6!, {r0}
	cmp	r5, #7
	ble.n	.L0
	movs	r3, #168
	mov	r6, r8
	movs	r5, #8
	mov	sl, r3
	adds	r6, #104
.L1:
	mov	r3, sl
	str	r3, [sp, #0]
	movs	r3, #128
	adds	r1, r5, #0
	movs	r0, #2
	adds	r2, r7, #0
	lsls	r3, r3, #1
	bl	Func_080150d0
	adds	r5, #1
	stmia	r6!, {r0}
	cmp	r5, #15
	ble.n	.L1
	movs	r3, #168
	mov	r6, r8
	movs	r5, #16
	mov	sl, r3
	adds	r6, #136
.L2:
	mov	r3, sl
	str	r3, [sp, #0]
	movs	r3, #128
	adds	r1, r5, #0
	movs	r0, #2
	adds	r2, r7, #0
	lsls	r3, r3, #1
	bl	Func_080150d0
	adds	r5, #1
	stmia	r6!, {r0}
	cmp	r5, #31
	ble.n	.L2
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
