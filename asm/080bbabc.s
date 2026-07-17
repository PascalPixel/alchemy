@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080bbabc
	.thumb_func
Func_080bbabc:
	push	{r5, lr}
	ldr	r3, [pc, #32]
	ldr	r4, [r3, #0]
	movs	r3, #215
	lsls	r3, r3, #3
	adds	r5, r4, r3
	ldr	r3, [pc, #24]
	adds	r4, r4, r3
	ldr	r2, [r4, #0]
	lsls	r3, r2, #2
	strb	r0, [r5, r2]
	adds	r3, #64
	adds	r2, #1
	str	r1, [r5, r3]
	str	r2, [r4, #0]
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x03001e74
	.4byte 0x000007fc
