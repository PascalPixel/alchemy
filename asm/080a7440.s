@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a7440
	.thumb_func
Func_080a7440:
	push	{r5, lr}
	ldr	r3, [pc, #44]
	ldr	r5, [r3, #0]
	movs	r2, #0
	movs	r1, #186
	lsls	r1, r1, #1
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r0, #0
	bl	Func_080a77a4
	movs	r3, #1
	negs	r3, r3
	adds	r2, r0, #0
	cmp	r0, r3
	beq.n	.L0
	ldr	r1, [pc, #16]
	adds	r3, r5, r1
	ldrb	r2, [r3, #0]
.L0:
	adds	r0, r2, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x0000021a
