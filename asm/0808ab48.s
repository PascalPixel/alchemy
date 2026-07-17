@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808ab48
	.thumb_func
Func_0808ab48:
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r2, [pc, #20]
	lsls	r3, r3, #3
	ldrsh	r0, [r3, r2]
	ldr	r1, [pc, #16]
	bl	Func_08002fb0
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x0809f1a8
	.4byte 0x02008000
