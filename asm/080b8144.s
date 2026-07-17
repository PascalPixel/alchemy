@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b8144
	.thumb_func
Func_080b8144:
	push	{lr}
	adds	r2, r0, #0
	ldr	r0, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r0, #52]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r0, #48]
	ldr	r3, [pc, #28]
	adds	r1, r0, #0
	str	r3, [r0, #72]
	adds	r1, #90
	movs	r3, #0
	str	r3, [r0, #68]
	strb	r3, [r1, #0]
	ldr	r1, [r2, #12]
	ldr	r3, [r2, #16]
	movs	r2, #0
	bl	Func_08009150
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000ab85
