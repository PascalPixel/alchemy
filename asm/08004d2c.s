@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08004d2c
	.thumb_func
Func_08004d2c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	adds	r7, r1, #0
	sub	sp, #48
	bl	Func_08002322
	mov	sl, r0
	ldr	r0, [r5, #0]
	bl	Func_0800231c
	mov	r9, r0
	ldr	r0, [r5, #4]
	bl	Func_08002322
	mov	r8, r0
	ldr	r0, [r5, #4]
	bl	Func_0800231c
	mov	fp, r0
	ldr	r0, [r5, #8]
	bl	Func_08002322
	adds	r6, r0, #0
	ldr	r0, [r5, #8]
	bl	Func_0800231c
	mov	lr, r0
	ldr	r3, [pc, #216]
	mov	r0, fp
	mov	r1, lr
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r5, sp
	str	r0, [r5, #0]
	adds	r1, r6, #0
	mov	r0, fp
	mov	ip, pc
	bx	r3
	mov	r1, r8
	negs	r2, r1
	str	r0, [r5, #4]
	str	r2, [r5, #8]
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r1, lr
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	adds	r1, r6, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r3
	subs	r4, r4, r0
	str	r4, [r5, #12]
	mov	r0, sl
	mov	r1, r8
	mov	ip, pc
	bx	r3
	adds	r1, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	mov	r1, lr
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r4, r0
	str	r4, [r5, #16]
	mov	r0, sl
	mov	r1, fp
	mov	ip, pc
	bx	r3
	str	r0, [r5, #20]
	mov	r1, r8
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r1, lr
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	adds	r1, r6, #0
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r4, r0
	str	r4, [r5, #24]
	mov	r0, r9
	mov	r1, r8
	mov	ip, pc
	bx	r3
	adds	r1, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	mov	r1, lr
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r3
	subs	r4, r4, r0
	str	r4, [r5, #28]
	mov	r0, r9
	mov	r1, fp
	mov	ip, pc
	bx	r3
	str	r0, [r5, #32]
	ldr	r3, [r7, #0]
	str	r3, [r5, #36]
	ldr	r3, [r7, #4]
	str	r3, [r5, #40]
	ldr	r3, [r7, #8]
	adds	r0, r5, #0
	str	r3, [r5, #44]
	ldr	r3, [pc, #24]
	bl	Func_080072f0
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03000118
	.4byte 0x030002c0
