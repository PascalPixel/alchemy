@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801bc34
	.thumb_func
Func_0801bc34:
	push	{r5, lr}
	movs	r3, #1
	sub	sp, #12
	negs	r3, r3
	subs	r0, #1
	adds	r5, r1, #0
	str	r3, [sp, #8]
	cmp	r0, #8
	bhi.n	.L0
	ldr	r2, [pc, #136]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0801bc74
	.4byte 0x0801bc84
	.4byte 0x0801bcc6
	.4byte 0x0801bcb6
	.4byte 0x0801bcc6
	.4byte 0x0801bc74
	.4byte 0x0801bcc6
	.4byte 0x0801bcc6
	.4byte 0x0801bc9a
	movs	r1, #0
	add	r2, sp, #8
	add	r3, sp, #4
	adds	r0, r5, #0
	str	r1, [sp, #0]
	bl	Func_08019ee4
	b.n	.L0
	bl	Func_08004080
	adds	r2, r0, #0
	str	r2, [sp, #8]
	cmp	r2, #96
	beq.n	.L1
	adds	r0, r5, #0
	movs	r1, #26
	bl	Func_0801a2a4
	b.n	.L0
	bl	Func_08004080
	adds	r2, r0, #0
	str	r2, [sp, #8]
	cmp	r2, #96
	bne.n	.L2
.L1:
	movs	r0, #1
	negs	r0, r0
	b.n	.L3
.L2:
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_0801a32c
	b.n	.L0
	movs	r1, #0
	str	r1, [sp, #0]
	add	r2, sp, #8
	add	r3, sp, #4
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_0801a3d0
.L0:
	ldr	r0, [sp, #8]
.L3:
	add	sp, #12
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x0801bc50
