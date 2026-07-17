@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0809a65c
	.thumb_func
Func_0809a65c:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r2, [r6, #72]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r7, [r6, #76]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl	Func_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L0
	adds	r3, #15
.L0:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r1, [r6, #80]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #30]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #30]
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
