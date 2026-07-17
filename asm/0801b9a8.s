@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801b9a8
	.thumb_func
Func_0801b9a8:
	push	{lr}
	movs	r3, #210
	lsls	r3, r3, #2
	adds	r0, r0, r3
	sub	sp, #12
	ldr	r2, [r0, #0]
	cmp	r1, #0
	beq.n	.L0
.L1:
	subs	r1, #1
	ldr	r2, [r2, #4]
	cmp	r1, #0
	bne.n	.L1
.L0:
	ldrh	r3, [r2, #10]
	cmp	r3, #1
	beq.n	.L2
	cmp	r3, #6
	bne.n	.L3
.L2:
	ldrh	r0, [r2, #32]
	ldr	r3, [pc, #24]
	subs	r0, r0, r3
	ldrh	r3, [r2, #12]
	movs	r1, #1
	str	r3, [sp, #8]
	str	r1, [sp, #0]
	add	r2, sp, #8
	add	r3, sp, #4
	movs	r1, #0
	bl	Func_08019ee4
.L3:
	add	sp, #12
	pop	{r0}
	bx	r0
	.4byte 0x0000001f
