@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801c244
	.thumb_func
Func_0801c244:
	push	{r5, r6, lr}
	ldr	r3, [pc, #128]
	ldr	r6, [r3, #0]
	movs	r5, #0
.L2:
	bl	Func_0801c2d0
	adds	r0, r5, #0
	bl	Func_08028920
	adds	r5, r0, #0
	bl	Func_0801c2e4
	cmp	r5, #4
	bhi.n	.L0
	ldr	r2, [pc, #104]
	lsls	r3, r5, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0801c27c
	.4byte 0x0801c290
	.4byte 0x0801c29e
	.4byte 0x0801c2a8
	.4byte 0x0801c2b6
	bl	Func_0808a280
	cmp	r0, #0
	bne.n	.L1
	movs	r0, #255
.L1:
	movs	r2, #189
	lsls	r2, r2, #1
	adds	r3, r6, r2
	strh	r0, [r3, #0]
	b.n	.L0
	bl	Func_080a1008
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L0
	b.n	.L2
	bl	Func_080a1040
	cmp	r0, #0
	beq.n	.L0
	b.n	.L2
	bl	Func_080a1000
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L0
	b.n	.L2
	bl	Func_080a1010
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L2
.L0:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001ebc
	.4byte 0x0801c268
