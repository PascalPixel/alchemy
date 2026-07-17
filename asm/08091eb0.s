@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08091eb0
	.thumb_func
Func_08091eb0:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #84]
	adds	r6, r1, #0
	ldr	r7, [r3, #0]
	adds	r5, r0, #0
	bl	Func_0808b05c
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r7, r1
	strh	r0, [r3, #0]
	cmp	r5, #98
	bne.n	.L0
	cmp	r6, #0
	bne.n	.L0
	ldr	r3, [pc, #60]
	ldr	r2, [pc, #60]
	adds	r1, #90
	adds	r3, r3, r1
	strh	r2, [r3, #0]
.L0:
	movs	r2, #207
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #3
	bne.n	.L1
	ldr	r3, [pc, #36]
	adds	r2, #86
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	adds	r0, #8
	bl	Func_0808adf0
.L1:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_0808b320
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000021
