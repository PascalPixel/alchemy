@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a3354
	.thumb_func
Func_080a3354:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #92]
	ldr	r7, [r3, #0]
	adds	r0, r7, #0
	sub	sp, #4
	bl	Func_080a1814
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #2
	movs	r2, #2
	movs	r3, #8
	bl	Func_080a1870
	movs	r0, #165
	lsls	r0, r0, #1
	ldr	r1, [pc, #56]
	movs	r2, #3
	adds	r3, r7, r0
.L0:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L0
	movs	r5, #0
	str	r5, [r7, #40]
	str	r5, [r7, #36]
	movs	r6, #2
	movs	r1, #17
	movs	r2, #30
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	bl	Func_08015010
	movs	r2, #136
	str	r0, [r7, #44]
	lsls	r2, r2, #1
	ldr	r0, [pc, #20]
	adds	r3, r7, r2
	str	r5, [r7, #32]
	strb	r5, [r3, #0]
	adds	r3, r7, r0
	strb	r5, [r3, #0]
	b.n	.L1
	movs	r0, r0
	.4byte 0x0000001e
	.4byte 0x03001f2c
	.4byte 0x00000111
.L1:
	movs	r3, #137
	lsls	r3, r3, #1
	adds	r2, r7, r3
	adds	r0, #2
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r3, r7, r0
	strb	r6, [r3, #0]
	add	sp, #4
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
