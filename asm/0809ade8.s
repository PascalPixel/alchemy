@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0809ade8
	.thumb_func
Func_0809ade8:
	push	{r5, lr}
	bl	Func_0808ba1c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	ldr	r2, [r5, #108]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L1
	ldr	r2, [pc, #52]
	movs	r3, #148
	lsls	r3, r3, #2
	adds	r1, r2, r3
	ldr	r3, [r1, #0]
	str	r3, [r5, #108]
	movs	r3, #0
	str	r3, [r1, #0]
	ldr	r3, [pc, #40]
	adds	r2, r2, r3
	movs	r1, #0
	ldrsb	r1, [r2, r1]
	bl	Func_08009240
.L1:
	adds	r2, r5, #0
	adds	r2, #91
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #16
	bl	Func_08009088
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0809ad71
	.4byte 0x02000240
	.4byte 0x00000249
