@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080cef64
	.thumb_func
Func_080cef64:
	push	{r5, r6, lr}
	sub	sp, #4
	adds	r6, r1, #0
	cmp	r0, #0
	bne.n	.L0
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	Func_080ed408
	ldr	r5, [pc, #80]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r0, #47
	str	r3, [r6, #0]
	movs	r1, #7
	movs	r3, #3
	movs	r2, #7
	adds	r5, #188
	str	r3, [sp, #0]
	b.n	.L1
.L0:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #46
	bl	Func_080ed408
	ldr	r5, [pc, #40]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [r6, #0]
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r3, #7
	movs	r1, #7
	movs	r2, #7
	adds	r5, #188
.L1:
	bl	Func_080ed408
	ldr	r3, [r5, #0]
	str	r3, [r6, #4]
	add	sp, #4
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e50
