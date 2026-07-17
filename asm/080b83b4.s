@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b83b4
	.thumb_func
Func_080b83b4:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	sub	sp, #12
	bl	Func_080b7dd0
	ldr	r6, [r0, #0]
	adds	r0, r5, #0
	bl	Func_080b7dd0
	movs	r4, #128
	ldr	r2, [r6, #56]
	lsls	r4, r4, #24
	ldr	r0, [r0, #0]
	cmp	r2, r4
	bne.n	.L0
	ldr	r2, [r6, #8]
.L0:
	ldr	r5, [r6, #64]
	cmp	r5, r4
	bne.n	.L1
	ldr	r5, [r6, #16]
.L1:
	ldr	r3, [r0, #56]
	cmp	r3, r4
	bne.n	.L2
	ldr	r3, [r0, #8]
.L2:
	ldr	r1, [r0, #64]
	cmp	r1, r4
	bne.n	.L3
	ldr	r1, [r0, #16]
.L3:
	adds	r3, r2, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	mov	r0, sp
	asrs	r3, r3, #1
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	adds	r3, r5, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	movs	r1, #128
	str	r3, [r0, #8]
	lsls	r1, r1, #5
	bl	Func_080b83b0
	add	sp, #12
	pop	{r5, r6}
	pop	{r0}
	bx	r0
