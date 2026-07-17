@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801a7c0
	.thumb_func
Func_0801a7c0:
	push	{lr}
	ldr	r3, [pc, #44]
	ldr	r2, [r3, #0]
	movs	r3, #229
	lsls	r3, r3, #2
	adds	r4, r2, r3
	ldrh	r3, [r4, #0]
	cmp	r3, #16
	beq.n	.L0
	lsls	r3, r3, #1
	mov	ip, r3
	movs	r3, #213
	lsls	r3, r3, #2
	add	r3, ip
	strh	r0, [r2, r3]
	movs	r3, #221
	lsls	r3, r3, #2
	add	r3, ip
	strh	r1, [r2, r3]
	ldrh	r3, [r4, #0]
	adds	r3, #1
	strh	r3, [r4, #0]
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001e98
