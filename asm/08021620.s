@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08021620
	.thumb_func
Func_08021620:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	adds	r5, r0, #0
	mov	r9, r1
	adds	r6, r2, #0
	mov	sl, r3
	bl	Func_08004080
	adds	r7, r0, #0
	movs	r0, #0
	cmp	r7, #95
	bgt.n	.L0
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	Func_080215e0
	ldr	r2, [pc, #72]
	mov	r3, sl
	mov	fp, r2
	str	r3, [sp, #0]
	mov	r1, fp
	mov	r2, r9
	adds	r3, r6, #0
	adds	r0, r7, #0
	bl	Func_0801eadc
	movs	r5, #253
	mov	r2, sl
	adds	r3, r6, #0
	strb	r5, [r0, #15]
	mov	r8, r0
	adds	r3, #32
	str	r2, [sp, #0]
	mov	r1, fp
	mov	r2, r9
	adds	r0, r7, #0
	bl	Func_0801eadc
	ldrh	r1, [r0, #24]
	lsls	r2, r1, #22
	ldr	r3, [pc, #20]
	lsrs	r2, r2, #22
	adds	r2, #8
	ands	r2, r3
	ldr	r3, [pc, #20]
	ands	r3, r1
	orrs	r3, r2
	strb	r5, [r0, #15]
	strh	r3, [r0, #24]
	mov	r0, r8
	b.n	.L0
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x80004000
	.4byte 0xfffffc00
.L0:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
