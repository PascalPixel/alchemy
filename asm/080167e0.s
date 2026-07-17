@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080167e0
	.thumb_func
Func_080167e0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	lsls	r3, r0, #1
	adds	r3, r3, r0
	ldr	r6, [pc, #100]
	lsls	r1, r3, #1
	lsls	r3, r3, #3
	mov	r8, r3
	adds	r5, r3, r6
	ldr	r3, [pc, #92]
	sub	sp, #8
	str	r3, [sp, #0]
	movs	r3, #32
	movs	r2, #24
	movs	r4, #132
	subs	r3, r3, r1
	subs	r2, r2, r1
	lsls	r4, r4, #24
	lsls	r3, r3, #2
	mov	r9, r2
	mov	fp, r4
	mov	sl, r3
	movs	r7, #29
.L0:
	mov	r2, r9
	mov	r4, fp
	ldr	r3, [pc, #64]
	adds	r0, r5, #0
	adds	r1, r6, #0
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [sp, #0]
	ldr	r3, [pc, #56]
	add	r0, sl
	mov	r1, r8
	movs	r2, #0
	bl	Func_080072f0
	ldr	r3, [sp, #0]
	subs	r7, #1
	adds	r3, #128
	adds	r6, #128
	adds	r5, #128
	str	r3, [sp, #0]
	cmp	r7, #0
	bge.n	.L0
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x06002520
	.4byte 0x06002500
	.4byte 0x040000d4
	.4byte 0x03000168
