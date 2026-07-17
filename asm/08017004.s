@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08017004
	.thumb_func
Func_08017004:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r1, [sp, #0]
	adds	r7, r0, #0
	movs	r3, #24
	ldrsh	r6, [r7, r3]
	movs	r3, #26
	ldrsh	r0, [r7, r3]
	subs	r3, r0, r6
	mov	sl, r3
	ldrh	r3, [r7, #8]
	adds	r1, r3, #0
	muls	r1, r6
	ldr	r3, [pc, #148]
	add	r5, sp, #4
	lsls	r1, r1, #16
	lsls	r0, r0, #17
	mov	r8, r3
	str	r1, [r5, #0]
	str	r0, [r5, #4]
	bl	Func_08007304
	ldrh	r3, [r7, #12]
	str	r0, [r5, #8]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	ldrh	r3, [r7, #8]
	mov	r1, sl
	muls	r1, r3
	lsls	r1, r1, #16
	str	r1, [r5, #0]
	mov	r9, r0
	ldr	r0, [r5, #4]
	bl	Func_08007304
	ldrh	r3, [r7, #10]
	adds	r1, r3, #0
	muls	r1, r6
	str	r0, [r5, #8]
	asrs	r0, r0, #15
	mov	fp, r0
	movs	r3, #26
	ldrsh	r0, [r7, r3]
	lsls	r1, r1, #16
	lsls	r0, r0, #17
	str	r1, [r5, #0]
	str	r0, [r5, #4]
	bl	Func_08007304
	ldrh	r3, [r7, #14]
	str	r0, [r5, #8]
	asrs	r0, r0, #16
	adds	r6, r0, r3
	ldrh	r3, [r7, #10]
	mov	r1, sl
	muls	r1, r3
	lsls	r1, r1, #16
	str	r1, [r5, #0]
	ldr	r0, [r5, #4]
	bl	Func_08007304
	str	r0, [r5, #8]
	asrs	r5, r0, #15
	adds	r3, r5, #0
	mov	r0, r9
	adds	r1, r6, #0
	mov	r2, fp
	bl	Func_080170f8
	ldr	r3, [sp, #0]
	cmp	r3, #0
	beq.n	.L0
	mov	r3, r9
	strh	r3, [r7, #28]
	mov	r3, fp
	strh	r6, [r7, #30]
	strh	r3, [r7, #32]
	strh	r5, [r7, #34]
.L0:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0300013c
