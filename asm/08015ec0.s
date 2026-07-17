@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08015ec0
	.thumb_func
Func_08015ec0:
	push	{lr}
	ldr	r3, [pc, #40]
	movs	r1, #211
	ldr	r2, [r3, #0]
	lsls	r1, r1, #3
	adds	r3, r2, r1
	cmp	r0, r3
	bcc.n	.L0
	ldr	r1, [pc, #28]
	adds	r3, r2, r1
	cmp	r0, r3
	bcs.n	.L0
	adds	r1, #4
	adds	r3, r2, r1
	ldr	r2, [r3, #0]
	str	r0, [r3, #0]
	movs	r3, #0
	str	r0, [r2, #0]
	str	r3, [r0, #0]
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000d98
