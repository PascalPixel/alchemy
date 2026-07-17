@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a345c
	.thumb_func
Func_080a345c:
	push	{lr}
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	movs	r0, #13
	adds	r3, #72
	movs	r1, #31
.L1:
	ldmia	r3!, {r2}
	cmp	r2, #0
	beq.n	.L0
	strb	r0, [r2, #5]
.L0:
	subs	r1, #1
	cmp	r1, #0
	bge.n	.L1
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
