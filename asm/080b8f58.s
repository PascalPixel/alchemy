@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b8f58
	.thumb_func
Func_080b8f58:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r1, #31
	ldr	r6, [pc, #100]
	mov	r8, r1
	movs	r7, #15
.L3:
	ldr	r3, [pc, #100]
	ldr	r3, [r3, #0]
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #10
	ldrh	r5, [r6, #32]
	bl	Func_0800231c
	adds	r3, r0, #0
	movs	r0, #128
	lsls	r0, r0, #9
	subs	r0, r0, r3
	ldr	r1, [pc, #80]
	bl	Func_080022ec
	mov	r2, r8
	lsrs	r3, r5, #10
	ands	r3, r2
	mov	r1, r8
	lsrs	r2, r5, #5
	ands	r2, r1
	adds	r3, r3, r0
	ands	r1, r5
	adds	r2, r2, r0
	adds	r1, r1, r0
	cmp	r3, #31
	bls.n	.L0
	movs	r3, #31
.L0:
	cmp	r2, #31
	bls.n	.L1
	movs	r2, #31
.L1:
	cmp	r1, #31
	bls.n	.L2
	movs	r1, #31
.L2:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	subs	r7, #1
	strh	r3, [r6, #0]
	adds	r6, #2
	cmp	r7, #0
	bge.n	.L3
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x050001c0
	.4byte 0x03001e40
	.4byte 0x00002aaa
