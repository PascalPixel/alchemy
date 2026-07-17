@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080793f8
	.thumb_func
Func_080793f8:
	push	{lr}
	lsls	r3, r0, #20
	ldr	r1, [pc, #20]
	lsrs	r0, r3, #23
	ldrb	r2, [r1, r0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r3, #255
	strb	r3, [r1, r0]
.L0:
	ldrb	r0, [r1, r0]
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000040
