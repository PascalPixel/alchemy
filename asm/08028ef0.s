@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08028ef0
	.thumb_func
Func_08028ef0:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	adds	r5, r1, #0
	lsls	r5, r5, #16
	asrs	r5, r5, #16
	adds	r6, r0, #0
	movs	r3, #0
	ldrsh	r1, [r2, r3]
	adds	r0, r5, #0
	sub	sp, #4
	mov	r9, r2
	bl	Func_0808a5d0
	ldr	r3, [pc, #120]
	mov	sl, r0
	adds	r0, r6, #0
	add	sl, r3
	bl	Func_08016478
	movs	r2, #14
	str	r2, [sp, #0]
	mov	r8, r2
	adds	r0, r5, #0
	adds	r2, r6, #0
	movs	r3, #0
	movs	r1, #3
	bl	Func_0801e9a0
	mov	r2, r9
	movs	r3, #0
	ldrsh	r0, [r2, r3]
	mov	r3, r8
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r1, #3
	movs	r3, #82
	bl	Func_0801e9a0
	ldr	r2, [pc, #72]
	mov	r8, r2
	mov	r0, r8
	adds	r1, r6, #0
	movs	r2, #74
	movs	r3, #0
	bl	Func_0801e858
	ldr	r3, [pc, #60]
	adds	r5, r5, r3
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_0801e74c
	mov	r0, r8
	adds	r1, r6, #0
	movs	r2, #74
	movs	r3, #14
	bl	Func_0801e858
	mov	r0, sl
	adds	r1, r6, #0
	movs	r2, #82
	movs	r3, #0
	bl	Func_0801e74c
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x0000099b
	.4byte 0x08037428
	.4byte 0x00000a07
