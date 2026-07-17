@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080da24c
	.thumb_func
Func_080da24c:
	push	{r5, r6, r7, lr}
	mov	lr, r1
	ldr	r1, [r0, #16]
	lsls	r3, r1, #1
	movs	r6, #0
	adds	r5, r3, #1
	mov	ip, r6
	movs	r4, #0
	cmp	r5, #0
	beq.n	.L0
	movs	r2, #36
	ldrsh	r3, [r0, r2]
	ldr	r2, [r0, #12]
	cmp	r2, r3
	beq.n	.L1
.L2:
	adds	r4, #1
	cmp	r4, r5
	beq.n	.L1
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r3, [r0, r3]
	cmp	r2, r3
	bne.n	.L2
	mov	ip, r4
.L1:
	movs	r4, #0
	cmp	r5, #0
	beq.n	.L0
	b.n	.L3
.L5:
	ldr	r1, [r0, #16]
.L3:
	mov	r2, ip
	adds	r3, r2, r4
	subs	r2, r3, r1
	cmp	r2, #0
	blt.n	.L4
	ldr	r3, [r0, #20]
	cmp	r2, r3
	bge.n	.L4
	lsls	r3, r6, #1
	mov	r7, lr
	strh	r2, [r3, r7]
	adds	r6, #1
.L4:
	adds	r4, #1
	cmp	r4, r5
	bne.n	.L5
.L0:
	adds	r0, r6, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
