@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08019854
	.thumb_func
Func_08019854:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	ldr	r3, [r5, #0]
	ldrh	r1, [r3, #12]
	mov	r9, r1
	ldrh	r1, [r3, #14]
	mov	fp, r1
	ldrh	r1, [r3, #8]
	sub	sp, #8
	ldrh	r2, [r3, #18]
	str	r1, [sp, #4]
	ldrh	r3, [r3, #10]
	str	r3, [sp, #0]
	cmp	r2, #4
	bne.n	.L0
	adds	r1, #2
	mov	r6, r9
	mov	r7, fp
	mov	r8, r1
	subs	r6, #1
	subs	r7, #1
	adds	r3, #2
	mov	r2, r8
	adds	r0, r6, #0
	adds	r1, r7, #0
	mov	sl, r3
	bl	Func_080170f8
	ldrh	r3, [r5, #20]
	ldr	r2, [pc, #60]
	adds	r3, r3, r2
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	lsrs	r2, r3, #16
	cmp	r2, #0
	bne.n	.L0
	ldr	r3, [r5, #0]
	adds	r0, r6, #0
	strh	r2, [r3, #18]
	adds	r1, r7, #0
	mov	r2, r8
	mov	r3, sl
	bl	Func_08016178
	mov	r0, r9
	mov	r1, fp
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #0]
	bl	Func_080170f8
.L0:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000ffff
