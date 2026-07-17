@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0809397c
	.thumb_func
Func_0809397c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldr	r1, [r6, #104]
	cmp	r1, #0
	beq.n	.L0
	ldr	r2, [r1, #8]
	ldr	r3, [r6, #8]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L1
	ldr	r2, [pc, #116]
	adds	r0, r0, r2
.L1:
	ldr	r2, [r1, #16]
	ldr	r3, [r6, #16]
	asrs	r5, r0, #16
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L2
	ldr	r3, [pc, #100]
	adds	r0, r0, r3
.L2:
	asrs	r0, r0, #16
	mov	r8, r0
	mov	r2, r8
	mov	r3, r8
	muls	r3, r2
	adds	r0, r5, #0
	muls	r0, r5
	adds	r0, r0, r3
	ldr	r3, [pc, #84]
	bl	Func_080072f0
	adds	r3, r6, #0
	adds	r3, #100
	movs	r2, #0
	ldrsh	r7, [r3, r2]
	cmp	r0, r7
	blt.n	.L3
	lsls	r0, r5, #20
	adds	r1, r7, #0
	bl	Func_080022ec
	ldr	r5, [r6, #8]
	mov	r3, r8
	adds	r5, r5, r0
	adds	r1, r7, #0
	lsls	r0, r3, #20
	bl	Func_080022ec
	ldr	r3, [r6, #16]
	adds	r1, r5, #0
	adds	r3, r3, r0
	ldr	r2, [r6, #12]
	adds	r0, r6, #0
	bl	Func_08009150
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_08009080
	b.n	.L0
.L3:
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08009080
.L0:
	movs	r0, #1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0000ffff
	.4byte 0x030001d8
