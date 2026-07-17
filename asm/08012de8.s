@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08012de8
	.thumb_func
Func_08012de8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #52]
	adds	r7, r1, #0
	adds	r5, r0, #0
	adds	r0, r7, #0
	ldr	r6, [r3, #0]
	bl	Func_08185000
	movs	r3, #3
	ands	r3, r5
	lsls	r3, r3, #2
	adds	r3, #40
	mov	r8, r3
	movs	r5, #9
.L0:
	mov	r3, r8
	ldr	r0, [r6, r3]
	subs	r5, #1
	strh	r7, [r0, #0]
	bl	Func_0800b868
	adds	r6, #56
	cmp	r5, #0
	bge.n	.L0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e60
