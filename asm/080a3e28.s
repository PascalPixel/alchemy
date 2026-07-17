@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a3e28
	.thumb_func
Func_080a3e28:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #80]
	ldr	r3, [r3, #0]
	mov	sl, r0
	adds	r5, r3, #0
	mov	r8, r1
	adds	r5, #72
	mov	r6, sl
	movs	r7, #14
.L2:
	ldrh	r1, [r6, #0]
	adds	r6, #2
	cmp	r1, #0
	beq.n	.L0
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L1
	ldr	r3, [r5, #0]
	movs	r0, #2
	ldrb	r2, [r3, #14]
	movs	r3, #0
	bl	Func_08015298
	b.n	.L0
.L1:
	ldr	r3, [r5, #0]
	movs	r0, #7
	ldrb	r2, [r3, #14]
	movs	r3, #0
	bl	Func_08015298
.L0:
	subs	r7, #1
	adds	r5, #4
	cmp	r7, #0
	bge.n	.L2
	mov	r0, sl
	bl	Func_080a3d24
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
