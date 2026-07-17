@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c0eb8
	.thumb_func
Func_080c0eb8:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldr	r6, [r5, #0]
	adds	r0, r5, #0
	movs	r1, #128
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	lsls	r1, r1, #9
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	ldr	r3, [r5, #0]
	adds	r6, r6, r3
	str	r6, [r5, #4]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
