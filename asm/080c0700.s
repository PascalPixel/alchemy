@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c0700
	.thumb_func
Func_080c0700:
	push	{r5, r6, lr}
	ldr	r3, [pc, #84]
	ldr	r0, [r3, #0]
	ldr	r3, [pc, #84]
	sub	sp, #4
	adds	r4, r1, #0
	adds	r5, r0, r3
	mov	r6, sp
	ldr	r3, [pc, #76]
	ldrh	r2, [r3, #0]
	str	r2, [r6, #0]
	strh	r3, [r3, #0]
	cmp	r4, #0
	bne.n	.L0
	ldr	r3, [pc, #68]
	adds	r0, r5, #0
	ldr	r1, [pc, #68]
	ldr	r2, [pc, #72]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L1
.L0:
	ldr	r3, [pc, #68]
	adds	r0, r0, r3
	lsls	r3, r4, #4
	adds	r3, r3, r4
	lsls	r3, r3, #4
	adds	r3, r3, r4
	movs	r2, #128
	lsls	r3, r3, #2
	lsls	r2, r2, #9
	subs	r2, r2, r3
	str	r2, [r0, #0]
	ldr	r1, [pc, #36]
	adds	r0, r5, #0
	movs	r3, #128
	bl	Func_080c1724
.L1:
	ldr	r2, [r6, #0]
	ldr	r3, [pc, #16]
	add	sp, #4
	strh	r2, [r3, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001e74
	.4byte 0x00000544
	.4byte 0x04000208
	.4byte 0x040000d4
	.4byte 0x050000c0
	.4byte 0x80000080
	.4byte 0x00000644
