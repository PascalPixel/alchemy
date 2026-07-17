@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808b074
	.thumb_func
Func_0808b074:
	push	{lr}
	ldr	r3, [pc, #20]
	lsls	r0, r0, #2
	adds	r0, r0, r3
	ldrh	r3, [r0, #0]
	ldrh	r1, [r0, #2]
	adds	r0, r3, #0
	bl	Func_0808b05c
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0809d8b0
