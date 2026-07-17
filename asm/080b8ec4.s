@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b8ec4
	.thumb_func
Func_080b8ec4:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	Func_08077008
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bgt.n	.L0
	adds	r0, r6, #0
	bl	Func_080b7dd0
	ldr	r3, [r0, #0]
	ldr	r5, [r3, #80]
	movs	r1, #5
	adds	r0, r5, #0
	bl	Func_08009020
	ldr	r2, [r5, #40]
	movs	r3, #6
	strb	r3, [r2, #5]
	movs	r3, #255
	strb	r3, [r2, #22]
	movs	r0, #4
	bl	Func_080030f8
	adds	r0, r5, #0
	bl	Func_08009100
	adds	r0, r6, #0
	bl	Func_080b7e60
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
