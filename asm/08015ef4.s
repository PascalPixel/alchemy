@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08015ef4
	.thumb_func
Func_08015ef4:
	push	{lr}
	ldr	r3, [pc, #44]
	movs	r1, #211
	ldr	r0, [r3, #0]
	lsls	r1, r1, #3
	adds	r2, r0, r1
	ldr	r1, [pc, #36]
	adds	r3, r0, r1
	str	r2, [r3, #0]
	movs	r3, #62
.L0:
	adds	r1, r2, #0
	adds	r1, #28
	subs	r3, #1
	str	r1, [r2, #0]
	adds	r2, r1, #0
	cmp	r3, #0
	bge.n	.L0
	ldr	r2, [pc, #20]
	movs	r3, #0
	str	r3, [r1, #0]
	adds	r3, r0, r2
	str	r1, [r3, #0]
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x00000d98
	.4byte 0x00000d9c
