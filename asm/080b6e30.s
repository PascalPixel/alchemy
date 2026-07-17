@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b6e30
	.thumb_func
Func_080b6e30:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #36]
	ldr	r7, [r3, #0]
	mov	r8, r0
	movs	r6, #0
	mov	sl, r2
	movs	r5, #4
.L1:
	ldrsh	r3, [r5, r7]
	cmp	r3, r8
	bne.n	.L0
	movs	r3, #0
	adds	r0, r6, #0
	movs	r1, #0
	movs	r2, #0
	bl	Func_08009270
	mov	r3, sl
	strh	r3, [r5, r7]
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001e74
.L0:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, #5
	ble.n	.L1
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
