@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801ebd8
	.thumb_func
Func_0801ebd8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	adds	r5, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	adds	r6, r3, #0
	bl	Func_08004080
	str	r0, [sp, #8]
	cmp	r0, #96
	bne.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	add	r2, sp, #8
	add	r3, sp, #4
	movs	r1, #1
	adds	r0, r5, #0
	str	r1, [sp, #0]
	bl	Func_08019ee4
	movs	r1, #128
	mov	r3, r8
	ldr	r0, [sp, #8]
	lsls	r1, r1, #23
	adds	r2, r7, #0
	str	r6, [sp, #0]
	bl	Func_0801eadc
	movs	r3, #251
	strb	r3, [r0, #15]
.L1:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
