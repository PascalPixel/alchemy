@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0809b0dc
	.thumb_func
Func_0809b0dc:
	push	{lr}
	ldr	r1, [pc, #56]
	ldr	r3, [r0, #28]
	ldr	r2, [r0, #24]
	adds	r3, r3, r1
	adds	r2, r2, r1
	str	r3, [r0, #28]
	movs	r1, #128
	ldrh	r3, [r0, #6]
	lsls	r1, r1, #6
	adds	r3, r3, r1
	strh	r3, [r0, #6]
	movs	r1, #128
	ldr	r3, [r0, #12]
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r0, #12]
	movs	r3, #192
	lsls	r3, r3, #6
	str	r2, [r0, #24]
	cmp	r2, r3
	bge.n	.L0
	adds	r2, r0, #0
	ldr	r3, [pc, #8]
	adds	r2, #84
	strb	r3, [r2, #0]
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x00000000
	.4byte 0xfffffd80
