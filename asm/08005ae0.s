@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08005ae0
	.thumb_func
Func_08005ae0:
	push	{lr}
	ldr	r3, [pc, #56]
	ldr	r3, [r3, #0]
	ldr	r4, [pc, #56]
	adds	r2, r3, #0
	movs	r0, #0
	movs	r1, #0
	adds	r2, #80
.L0:
	ldrb	r3, [r2, #0]
	adds	r0, r0, r3
	ldrb	r3, [r2, #1]
	adds	r0, r0, r3
	ldrb	r3, [r2, #2]
	adds	r0, r0, r3
	ldrb	r3, [r2, #3]
	adds	r0, r0, r3
	ldrb	r3, [r2, #4]
	adds	r0, r0, r3
	ldrb	r3, [r2, #5]
	adds	r0, r0, r3
	ldrb	r3, [r2, #6]
	adds	r0, r0, r3
	ldrb	r3, [r2, #7]
	adds	r1, #8
	adds	r0, r0, r3
	adds	r2, #8
	cmp	r1, r4
	bls.n	.L0
	pop	{r1}
	bx	r1
	.4byte 0x03001f1c
	.4byte 0x00000fe7
