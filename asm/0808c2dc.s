@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808c2dc
	.thumb_func
Func_0808c2dc:
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	bl	Func_08077148
	cmp	r0, #0
	ble.n	.L0
	ldr	r3, [pc, #28]
	movs	r2, #252
	lsls	r2, r2, #1
	adds	r6, r3, r2
	adds	r5, r0, #0
.L1:
	ldrb	r0, [r6, #0]
	adds	r1, r7, #0
	subs	r5, #1
	adds	r6, #1
	bl	Func_08077120
	cmp	r5, #0
	bne.n	.L1
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
