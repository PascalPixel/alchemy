@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b26cc
	.thumb_func
Func_080b26cc:
	push	{r5, r6, lr}
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #3
	adds	r6, r5, r3
	adds	r0, r6, #0
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L0
	adds	r0, r6, #0
	bl	Func_080770c8
	lsls	r3, r5, #5
	adds	r3, r3, r5
	lsls	r2, r3, #1
	ldr	r1, [pc, #44]
	adds	r3, r2, #0
	adds	r3, #48
	ldrsh	r0, [r1, r3]
	movs	r6, #0
	cmp	r0, #0
	beq.n	.L0
	adds	r3, r2, r1
	adds	r5, r3, #0
	adds	r5, #48
.L1:
	movs	r1, #1
	adds	r6, #1
	bl	Func_08077240
	cmp	r6, #7
	bgt.n	.L0
	adds	r5, #2
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	cmp	r0, #0
	bne.n	.L1
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x080b41ac
