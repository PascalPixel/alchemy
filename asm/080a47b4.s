@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a47b4
	.thumb_func
Func_080a47b4:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #68]
	ldr	r6, [r3, #0]
	sub	sp, #8
	movs	r3, #10
	adds	r7, r6, #0
	str	r3, [sp, #0]
	adds	r7, #48
	movs	r3, #2
	str	r3, [sp, #4]
	adds	r5, r0, #0
	movs	r3, #13
	movs	r1, #0
	movs	r2, #0
	adds	r0, r7, #0
	bl	Func_080a10d0
	bl	Func_080a22f4
	movs	r3, #188
	lsls	r3, r3, #1
	lsls	r5, r5, #1
	adds	r5, r5, r3
	ldrh	r3, [r6, r5]
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [r7, #0]
	adds	r1, r3, #0
	bl	Func_080a4924
.L0:
	movs	r0, #1
	add	sp, #8
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
