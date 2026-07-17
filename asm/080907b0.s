@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080907b0
	.thumb_func
Func_080907b0:
	push	{r5, lr}
	ldr	r3, [pc, #88]
	sub	sp, #4
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #84]
	adds	r4, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	ldr	r1, [pc, #80]
	ldr	r3, [pc, #84]
	ldr	r2, [pc, #84]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #1
	negs	r2, r2
	cmp	r4, r2
	beq.n	.L0
	movs	r1, #0
	movs	r3, #7
.L1:
	lsls	r1, r1, #4
	subs	r3, #1
	orrs	r1, r4
	cmp	r3, #0
	bge.n	.L1
	movs	r3, #161
	lsls	r3, r3, #3
	adds	r2, r5, r3
	movs	r3, #7
.L2:
	subs	r3, #1
	stmia	r2!, {r1}
	cmp	r3, #0
	bge.n	.L2
	movs	r2, #161
	lsls	r2, r2, #3
	movs	r1, #192
	adds	r0, r5, r2
	ldr	r3, [pc, #28]
	lsls	r1, r1, #19
	ldr	r2, [pc, #32]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L0:
	add	sp, #4
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ecc
	.4byte 0xf000f000
	.4byte 0x06002000
	.4byte 0x040000d4
	.4byte 0x85000140
	.4byte 0x84000008
