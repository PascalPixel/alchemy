@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08092adc
	.thumb_func
Func_08092adc:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r6, r2, #0
	bl	Func_0808ba1c
	cmp	r0, #0
	beq.n	.L0
	adds	r3, r0, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	ldr	r1, [pc, #16]
	bl	Func_08009098
	adds	r0, r6, #0
	bl	Func_0809163c
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0809fc1c
