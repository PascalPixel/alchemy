@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800e9dc
	.thumb_func
Func_0800e9dc:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r3, #4
	ldrsh	r2, [r5, r3]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r1, r3, #4
	ldr	r3, [r1, #0]
	ldr	r2, [pc, #36]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	ldrh	r0, [r5, #4]
	cmp	r3, #0
	beq.n	.L0
	ldr	r2, [r1, #4]
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_080072f0
	ldrh	r0, [r5, #4]
.L0:
	adds	r3, r0, #3
	strh	r3, [r5, #4]
	movs	r0, #1
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080136e0
