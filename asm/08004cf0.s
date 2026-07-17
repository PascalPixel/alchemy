@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08004cf0
	.thumb_func
Func_08004cf0:
	push	{r5, r6, lr}
	sub	sp, #48
	adds	r6, r0, #0
	mov	r5, sp
	adds	r0, r5, #0
	movs	r1, #128
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	lsls	r1, r1, #9
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	ldr	r3, [r6, #0]
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #16]
	ldr	r3, [r6, #8]
	adds	r0, r5, #0
	str	r3, [r5, #32]
	ldr	r3, [pc, #12]
	bl	Func_080072f0
	add	sp, #48
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x030002c0
