@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a1778
	.thumb_func
Func_080a1778:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	mov	r8, r0
	mov	sl, r1
	adds	r7, r2, #0
	bl	Func_08004080
	adds	r5, r0, #0
	movs	r6, #0
	cmp	r5, #0
	beq.n	.L0
	ldr	r2, [pc, #40]
	movs	r1, #128
	bl	Func_08003fa4
	movs	r1, #128
	lsls	r1, r1, #23
	adds	r0, r5, #0
	mov	r2, r8
	mov	r3, sl
	str	r7, [sp, #0]
	bl	Func_080150c8
	adds	r6, r0, #0
.L0:
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080aea4c
