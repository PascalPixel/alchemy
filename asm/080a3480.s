@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a3480
	.thumb_func
Func_080a3480:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #52]
	ldr	r3, [r3, #0]
	movs	r2, #13
	adds	r7, r3, #0
	movs	r6, #0
	mov	r8, r2
	adds	r7, #72
.L1:
	ldmia	r7!, {r5}
	cmp	r5, #0
	beq.n	.L0
	adds	r0, r6, #0
	movs	r1, #5
	bl	Func_080022fc
	cmp	r0, #0
	bne.n	.L0
	mov	r3, r8
	strb	r3, [r5, #5]
.L0:
	adds	r6, #1
	cmp	r6, #31
	ble.n	.L1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
