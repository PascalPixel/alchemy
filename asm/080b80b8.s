@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b80b8
	.thumb_func
Func_080b80b8:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r5, [r0, #0]
	ldr	r6, [r1, #0]
	mov	sl, r2
	ldr	r3, [r6, #8]
	ldr	r2, [r5, #8]
	subs	r3, r3, r2
	mov	r0, sl
	muls	r0, r3
	movs	r1, #100
	mov	r8, r2
	bl	Func_080022ec
	ldr	r3, [r6, #16]
	ldr	r6, [r5, #16]
	subs	r3, r3, r6
	add	r8, r0
	movs	r1, #100
	mov	r0, sl
	muls	r0, r3
	bl	Func_080022ec
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #60]
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r3, [pc, #48]
	adds	r2, r5, #0
	str	r3, [r5, #72]
	movs	r3, #0
	str	r3, [r5, #68]
	adds	r6, r6, r0
	adds	r2, #90
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #0
	adds	r3, r6, #0
	bl	Func_08009150
	adds	r0, r5, #0
	movs	r1, #2
	bl	Func_08009080
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x04000050
	.4byte 0x0000ab85
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
