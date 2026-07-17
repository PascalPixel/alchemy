@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08016594
	.thumb_func
Func_08016594:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	Func_08015ec0
	ldrb	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L0
	ldrb	r0, [r5, #14]
	bl	Func_08003f3c
	ldrb	r3, [r5, #4]
	cmp	r3, #2
	bne.n	.L0
	ldr	r3, [pc, #32]
	ldr	r1, [r3, #0]
	ldrb	r3, [r5, #25]
	ldr	r2, [pc, #28]
	lsrs	r3, r3, #4
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #12]
	strh	r2, [r1, r3]
.L0:
	movs	r3, #0
	strb	r3, [r5, #5]
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000003e7
	.4byte 0x03001e8c
	.4byte 0x000012d0
