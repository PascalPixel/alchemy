@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08092924
	.thumb_func
Func_08092924:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl	Func_0808ba1c
	ldr	r3, [pc, #24]
	ldr	r6, [r3, #0]
	cmp	r0, #0
	beq.n	.L0
	bl	Func_080090d0
	lsls	r3, r5, #2
	adds	r3, #20
	movs	r2, #0
	str	r2, [r6, r3]
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
	bx	lr
