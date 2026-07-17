@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08004698
	.thumb_func
Func_08004698:
	push	{lr}
	ldr	r1, [pc, #28]
	movs	r2, #0
	ldr	r3, [r1, #0]
	cmp	r2, r0
	bcs.n	.L0
	ldr	r1, [pc, #12]
.L1:
	adds	r2, #1
	strh	r1, [r3, #0]
	adds	r3, #2
	cmp	r2, r0
	bcc.n	.L1
	ldr	r1, [pc, #4]
	b.n	.L0
	.4byte 0x0000f000
	.4byte 0x03001cbc
.L0:
	str	r3, [r1, #0]
	pop	{r0}
	bx	r0
