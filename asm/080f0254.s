@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f0254
	.thumb_func
Func_080f0254:
	push	{r5, lr}
	sub	sp, #4
	cmp	r0, #0
	bne.n	.L0
	movs	r1, #192
	movs	r5, #160
	ldr	r3, [pc, #48]
	lsls	r1, r1, #19
	lsls	r5, r5, #19
	b.n	.L1
.L0:
	ldr	r3, [pc, #44]
	ldr	r1, [pc, #48]
	ldr	r5, [pc, #48]
.L1:
	mov	r4, sp
	str	r3, [r4, #0]
	adds	r0, r4, #0
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #0
	str	r3, [r4, #0]
	adds	r0, r4, #0
	ldr	r3, [pc, #32]
	adds	r1, r5, #0
	ldr	r2, [pc, #36]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #4
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x01010101
	.4byte 0x81818181
	.4byte 0x06008000
	.4byte 0x05000100
	.4byte 0x040000d4
	.4byte 0x85001e00
	.4byte 0x85000040
