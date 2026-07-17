@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a9d84
	.thumb_func
Func_080a9d84:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #52]
	ldr	r3, [r3, #0]
	movs	r2, #248
	adds	r5, r3, #0
	mov	r8, r2
	adds	r5, #200
	movs	r7, #168
	movs	r6, #4
.L1:
	ldmia	r5!, {r0}
	cmp	r0, #0
	beq.n	.L0
	mov	r3, r8
	strh	r3, [r0, #6]
	movs	r3, #240
	strh	r7, [r0, #8]
	strb	r3, [r0, #15]
	bl	Func_080a17c4
.L0:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
