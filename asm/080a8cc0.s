@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a8cc0
	.thumb_func
Func_080a8cc0:
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	lsls	r5, r3, #1
	ldr	r3, [pc, #88]
	mov	r8, r1
	movs	r1, #128
	mov	r9, r2
	adds	r3, r3, r5
	lsls	r1, r1, #3
	sub	sp, #4
	movs	r6, #0
	mov	fp, r3
	orrs	r1, r3
	mov	r2, r8
	mov	r3, r9
	str	r6, [sp, #0]
	mov	sl, r0
	bl	Func_08015280
	ldr	r3, [pc, #60]
	mov	r2, r8
	adds	r5, r5, r3
	mov	r0, sl
	adds	r1, r5, #0
	mov	r3, r9
	adds	r2, #1
	str	r6, [sp, #0]
	bl	Func_08015280
	movs	r3, #2
	add	r8, r3
	mov	r0, sl
	mov	r1, fp
	mov	r2, r8
	mov	r3, r9
	str	r6, [sp, #0]
	bl	Func_08015280
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000f281
	.4byte 0x0000f280
