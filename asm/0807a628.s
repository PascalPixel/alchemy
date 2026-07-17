@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0807a628
	.thumb_func
Func_0807a628:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	mov	r8, r1
	bl	Func_08077394
	mov	r1, r8
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	Func_08078588
	movs	r6, #0
	adds	r5, #216
.L1:
	ldrh	r3, [r5, #0]
	adds	r5, #2
	cmp	r3, r8
	bne.n	.L0
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	Func_08078708
.L0:
	adds	r6, #1
	cmp	r6, #14
	ble.n	.L1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
