@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080216e8
	.thumb_func
Func_080216e8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #128
	lsls	r3, r3, #3
	mov	sl, r3
	adds	r6, r0, #0
	adds	r7, r1, #0
	movs	r0, #14
	mov	r1, sl
	mov	r8, r2
	bl	Func_080048f4
	adds	r5, r0, #0
	ldr	r0, [pc, #68]
	bl	Func_08002f40
	adds	r2, r0, #0
	cmp	r7, #95
	bgt.n	.L0
	lsls	r3, r6, #1
	ldrh	r0, [r3, r2]
	adds	r1, r5, #0
	adds	r0, r2, r0
	bl	Func_080053e8
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L1
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r0, r5, #0
	bl	Func_08009288
.L1:
	adds	r0, r7, #0
	mov	r1, sl
	adds	r2, r5, #0
	bl	Func_08003fa4
	movs	r0, #14
	bl	Func_08002dd8
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000000f1
