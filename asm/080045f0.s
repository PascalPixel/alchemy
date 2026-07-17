@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080045f0
	.thumb_func
Func_080045f0:
	push	{lr}
	ldr	r3, [pc, #36]
	ldr	r4, [pc, #36]
	movs	r1, #15
	adds	r2, r3, #7
	mov	ip, r3
.L0:
	adds	r3, r0, #0
	ands	r3, r1
	ldrb	r3, [r4, r3]
	strb	r3, [r2, #0]
	subs	r2, #1
	lsrs	r0, r0, #4
	cmp	r2, ip
	bge.n	.L0
	ldr	r2, [pc, #8]
	movs	r3, #0
	strb	r3, [r2, #8]
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f70
	.4byte 0x0800795c
