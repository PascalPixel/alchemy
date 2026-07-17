@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080198dc
	.thumb_func
Func_080198dc:
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r4, [pc, #32]
	ldr	r3, [r3, #0]
	adds	r2, r3, r4
	subs	r4, #32
	movs	r1, #0
	movs	r0, #0
	adds	r3, r3, r4
.L0:
	adds	r1, #1
	stmia	r3!, {r0}
	strh	r0, [r2, #0]
	adds	r2, #2
	cmp	r1, #8
	bne.n	.L0
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x000012dc
