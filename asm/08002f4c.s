@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08002f4c
	.thumb_func
Func_08002f4c:
	push	{r5, r6, r7, lr}
	asrs	r7, r1, #1
	movs	r6, #1
	adds	r0, #2
	cmp	r6, r7
	bge.n	.L0
	movs	r3, #248
	lsls	r3, r3, #8
	mov	ip, r3
	movs	r3, #240
	lsls	r3, r3, #8
	ldr	r5, [pc, #72]
	mov	lr, r3
	subs	r0, #4
.L2:
	ldrh	r2, [r0, #4]
	mov	r3, ip
	adds	r1, r2, #0
	ands	r1, r3
	adds	r0, #2
	cmp	r1, ip
	bne.n	.L1
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	ands	r4, r1
	cmp	r4, lr
	bne.n	.L1
	ands	r3, r5
	ands	r2, r5
	lsls	r2, r2, #1
	lsls	r3, r3, #12
	orrs	r3, r2
	lsls	r2, r6, #1
	subs	r3, r3, r2
	lsrs	r2, r3, #12
	lsrs	r3, r3, #1
	ands	r2, r5
	ands	r3, r5
	orrs	r2, r4
	orrs	r3, r1
	strh	r2, [r0, #0]
	strh	r3, [r0, #2]
.L1:
	adds	r6, #1
	cmp	r6, r7
	blt.n	.L2
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000007ff
