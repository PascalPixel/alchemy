@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b2ed8
	.thumb_func
Func_080b2ed8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #92]
	ldr	r2, [pc, #92]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	adds	r7, r1, #0
	adds	r6, r0, #0
	adds	r1, r5, #0
	adds	r0, r7, #0
	bl	Func_080b2778
	mov	r8, r0
	cmp	r6, #0
	beq.n	.L0
	adds	r0, r6, #0
	bl	Func_08015060
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	Func_080b27b0
	cmp	r0, #0
	beq.n	.L1
	ldr	r5, [pc, #52]
	b.n	.L2
.L1:
	ldr	r5, [pc, #52]
.L2:
	adds	r0, r5, #0
	bl	Func_080b2884
	movs	r1, #5
	adds	r5, r0, #0
	mov	r0, r8
	bl	Func_08015120
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015078
.L0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000003aa
	.4byte 0x00000d2c
	.4byte 0x00000d2d
