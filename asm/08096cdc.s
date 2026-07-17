@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08096cdc
	.thumb_func
Func_08096cdc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r1, #0
	ldr	r1, [pc, #64]
	mov	r8, r0
	adds	r6, r2, #0
	movs	r5, #0
	mov	sl, r1
.L1:
	adds	r0, r5, #0
	bl	Func_0808ba1c
	mov	r1, sl
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	cmp	r5, r3
	beq.n	.L0
	cmp	r0, #0
	beq.n	.L0
	cmp	r0, r8
	beq.n	.L0
	adds	r3, r0, #0
	adds	r3, #91
	strb	r7, [r3, #0]
	adds	r1, r6, #0
	bl	Func_08009088
.L0:
	adds	r5, #1
	cmp	r5, #66
	ble.n	.L1
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0200048a
