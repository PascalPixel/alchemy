@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0807882c
	.thumb_func
Func_0807882c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r6, r0, #0
	mov	r8, r1
	movs	r7, #0
	movs	r5, #216
	mov	sl, r2
.L2:
	ldrh	r3, [r5, r6]
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	ldrh	r0, [r5, r6]
	bl	Func_08078414
	ldrb	r3, [r0, #2]
	cmp	r3, r8
	beq.n	.L1
.L0:
	adds	r7, #1
	adds	r5, #2
	cmp	r7, #14
	ble.n	.L2
	movs	r0, #0
.L1:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
