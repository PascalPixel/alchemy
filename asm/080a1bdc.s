@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a1bdc
	.thumb_func
Func_080a1bdc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #64]
	ldr	r3, [r3, #0]
	adds	r5, r3, #0
	adds	r5, #72
	sub	sp, #4
	mov	r9, r0
	mov	sl, r1
	mov	r8, r2
	movs	r6, #0
	adds	r7, r5, #0
.L1:
	ldmia	r7!, {r3}
	cmp	r3, #0
	beq.n	.L0
	mov	r3, r8
	str	r3, [sp, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	Func_080a1c2c
.L0:
	adds	r6, #1
	adds	r5, #4
	cmp	r6, #31
	ble.n	.L1
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
