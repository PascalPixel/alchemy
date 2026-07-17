@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080fa928
	.thumb_func
Func_080fa928:
	push	{lr}
	sub	sp, #4
	ldr	r0, [pc, #88]
	ldr	r2, [r0, #0]
	ldr	r1, [r2, #0]
	ldr	r3, [pc, #88]
	adds	r0, r1, r3
	cmp	r0, #1
	bhi.n	.L0
	adds	r0, r1, #0
	adds	r0, #10
	str	r0, [r2, #0]
	ldr	r1, [pc, #76]
	ldr	r0, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #18
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L1
	ldr	r0, [pc, #68]
	str	r0, [r1, #0]
.L1:
	ldr	r1, [pc, #68]
	ldr	r0, [r1, #0]
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L2
	ldr	r0, [pc, #52]
	str	r0, [r1, #0]
.L2:
	ldr	r0, [pc, #56]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r1, r3, #0
	strh	r1, [r0, #0]
	adds	r0, #12
	strh	r1, [r0, #0]
	movs	r0, #0
	str	r0, [sp, #0]
	movs	r0, #212
	lsls	r0, r0, #2
	adds	r1, r2, r0
	ldr	r2, [pc, #36]
	mov	r0, sp
	bl	Func_08006864
.L0:
	add	sp, #4
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03007ff0
	.4byte 0x978c92ad
	.4byte 0x040000c4
	.4byte 0x84400004
	.4byte 0x040000d0
	.4byte 0x040000c6
	.4byte 0x05000318
