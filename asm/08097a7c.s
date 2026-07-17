@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08097a7c
	.thumb_func
Func_08097a7c:
	push	{lr}
	ldr	r3, [pc, #72]
	ldr	r2, [pc, #72]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	movs	r2, #1
	strb	r2, [r3, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #64]
	strh	r2, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r1, #144
	adds	r3, #2
	strh	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl	Func_080041d8
	b.n	.L0
	.4byte 0x0000739c
	.4byte 0x03001e8c
	.4byte 0x00000ea4
	.4byte 0x050001e2
	.4byte 0x08097869
.L0:
	pop	{r0}
	bx	r0
