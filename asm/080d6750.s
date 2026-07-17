@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080d6750
	.thumb_func
Func_080d6750:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r1, #36
	ldrsh	r3, [r0, r1]
	sub	sp, #32
	movs	r2, #0
	cmp	r3, #127
	ble.n	.L0
	add	r3, sp, #4
	mov	r8, r3
	movs	r6, #0
	mov	r7, r8
.L2:
	adds	r5, r6, #0
	adds	r5, #128
	adds	r0, r5, #0
	str	r2, [sp, #0]
	bl	Func_08077008
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r2, [sp, #0]
	cmp	r3, #0
	ble.n	.L1
	strh	r5, [r7, #0]
	adds	r2, #1
	adds	r7, #2
.L1:
	adds	r6, #1
	cmp	r6, #6
	bne.n	.L2
	b.n	.L3
.L0:
	add	r3, sp, #4
	mov	r8, r3
	mov	r1, r8
	lsls	r3, r2, #1
	movs	r6, #0
	adds	r5, r3, r1
.L5:
	adds	r0, r6, #0
	str	r2, [sp, #0]
	bl	Func_08077008
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r2, [sp, #0]
	cmp	r3, #0
	ble.n	.L4
	strh	r6, [r5, #0]
	adds	r2, #1
	adds	r5, #2
.L4:
	adds	r6, #1
	cmp	r6, #8
	bne.n	.L5
.L3:
	ldr	r3, [pc, #16]
	lsls	r2, r2, #1
	mov	r1, r8
	strh	r3, [r1, r2]
	mov	r0, r8
	movs	r1, #0
	bl	Func_080b5010
	add	sp, #32
	b.n	.L6
	.4byte 0x000000ff
.L6:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
