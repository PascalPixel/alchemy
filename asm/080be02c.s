@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080be02c
	.thumb_func
Func_080be02c:
	push	{r5, lr}
	ldr	r3, [pc, #56]
	ldr	r1, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L0
	movs	r3, #1
	str	r3, [r2, #0]
.L0:
	cmp	r3, #4
	beq.n	.L1
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r5, r1, r3
.L2:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r5, #0]
	cmp	r3, #4
	bne.n	.L2
.L1:
	ldr	r0, [pc, #16]
	bl	Func_08004278
	bl	Func_080bdfec
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x03001e74
	.4byte 0x080bd899
