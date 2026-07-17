@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080bb588
	.thumb_func
Func_080bb588:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	Func_08077008
	ldr	r4, [pc, #192]
	adds	r1, r0, #0
	movs	r2, #3
	movs	r0, #0
	adds	r3, r1, r4
.L0:
	subs	r2, #1
	strb	r0, [r3, #0]
	subs	r3, #1
	cmp	r2, #0
	bge.n	.L0
	ldr	r0, [pc, #176]
	movs	r4, #153
	movs	r3, #0
	adds	r2, r1, r0
	lsls	r4, r4, #1
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r4, #2
	adds	r2, r1, r0
	strb	r3, [r2, #0]
	adds	r2, r1, r4
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	bl	Func_08077010
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000012f
	.4byte 0x00000131
