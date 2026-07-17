@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a68a8
	.thumb_func
Func_080a68a8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #56]
	ldr	r3, [r3, #0]
	mov	r8, r0
	adds	r6, r3, #0
	adds	r6, #72
	mov	r5, r8
	movs	r7, #31
.L1:
	ldrh	r1, [r5, #0]
	adds	r5, #2
	cmp	r1, #0
	beq.n	.L0
	ldr	r3, [r6, #0]
	movs	r0, #4
	ldrb	r2, [r3, #14]
	movs	r3, #0
	bl	Func_08015298
.L0:
	subs	r7, #1
	adds	r6, #4
	cmp	r7, #0
	bge.n	.L1
	mov	r0, r8
	bl	Func_080a3d24
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
