@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08092208
	.thumb_func
Func_08092208:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	mov	r8, r1
	mov	sl, r2
	bl	Func_0808ba1c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L0
	movs	r2, #10
	ldrsh	r3, [r6, r2]
	adds	r5, r3, #0
	cmp	r3, #0
	bge.n	.L1
	adds	r5, #15
.L1:
	asrs	r5, r5, #4
	lsls	r5, r5, #4
	adds	r2, r6, #0
	adds	r2, #91
	subs	r5, r3, r5
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	bl	Func_08009140
	lsls	r5, r5, #16
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_08009080
	asrs	r5, r5, #16
	movs	r3, #8
	ldr	r1, [r6, #8]
	subs	r3, r3, r5
	lsls	r3, r3, #16
	adds	r1, r1, r3
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r6, #0
	bl	Func_08009150
	adds	r0, r6, #0
	bl	Func_08009158
	adds	r0, r7, #0
	mov	r1, r8
	bl	Func_08092b08
	mov	r3, sl
	lsls	r0, r3, #16
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #8]
	adds	r3, r3, r0
	ldr	r2, [r6, #12]
	adds	r0, r6, #0
	bl	Func_08009150
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
