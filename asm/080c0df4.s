@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c0df4
	.thumb_func
Func_080c0df4:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r6, r1, #0
	mov	r8, r2
	bl	Func_080b7dd0
	ldr	r5, [r0, #0]
	adds	r0, r6, #0
	bl	Func_080b7dd0
	ldr	r3, [r0, #0]
	ldr	r1, [r5, #8]
	ldr	r0, [r3, #8]
	ldr	r4, [r5, #16]
	ldr	r2, [r3, #16]
	adds	r0, r0, r1
	adds	r2, r2, r4
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r0, r0, #1
	asrs	r2, r2, #1
	movs	r1, #0
	mov	r3, r8
	bl	Func_080c0cec
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
