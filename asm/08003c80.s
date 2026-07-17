@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08003c80
	.thumb_func
Func_08003c80:
	push	{r5, lr}
	adds	r5, r3, #0
	ldr	r3, [pc, #28]
	strb	r0, [r3, #0]
	ldr	r3, [pc, #20]
	adds	r4, r2, #0
	ldr	r2, [pc, #24]
	ands	r1, r3
	strh	r1, [r2, #0]
	cmp	r4, #16
	bls.n	.L0
	ldr	r2, [pc, #20]
	ldr	r3, [pc, #20]
	ldrb	r2, [r2, #0]
	strb	r2, [r3, #0]
	b.n	.L1
	.4byte 0x0000003f
	.4byte 0x03001cd4
	.4byte 0x03001cf8
	.4byte 0x03001ca8
	.4byte 0x03001aec
.L0:
	ldr	r3, [pc, #24]
	strb	r4, [r3, #0]
.L1:
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #28]
	strb	r5, [r3, #0]
	ldr	r3, [sp, #8]
	strb	r3, [r2, #0]
	ldr	r1, [pc, #24]
	ldrb	r3, [r2, #0]
	strb	r3, [r1, #0]
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001aec
	.4byte 0x03001ca8
	.4byte 0x03001c98
	.4byte 0x03001ac0
