@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b28d4
	.thumb_func
Func_080b28d4:
	push	{r5, r6, lr}
	ldr	r3, [pc, #76]
	movs	r2, #233
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldrh	r0, [r3, #0]
	bl	Func_0808a540
	adds	r5, r0, #0
	bl	Func_08015140
	adds	r0, r6, #0
	bl	Func_080b2884
	lsls	r5, r5, #16
	movs	r3, #34
	orrs	r5, r3
	movs	r1, #5
	movs	r2, #0
	adds	r3, r5, #0
	adds	r6, r0, #0
	bl	Func_08015038
	b.n	.L0
.L1:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L1
	movs	r0, #1
	bl	Func_080030f8
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
