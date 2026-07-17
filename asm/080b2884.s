@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b2884
	.thumb_func
Func_080b2884:
	push	{lr}
	ldr	r3, [pc, #52]
	ldr	r2, [pc, #52]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	cmp	r1, #1
	bne.n	.L0
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #44]
	subs	r3, r3, r2
	adds	r0, r0, r3
.L0:
	cmp	r1, #2
	bne.n	.L1
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #32]
	subs	r3, r3, r2
	adds	r0, r0, r3
.L1:
	cmp	r1, #3
	bne.n	.L2
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #20]
	subs	r3, r3, r2
	adds	r0, r0, r3
.L2:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000003aa
	.4byte 0x00000d2e
	.4byte 0x00000d24
	.4byte 0x00000d38
	.4byte 0x00000d42
