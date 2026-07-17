@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08091858
	.thumb_func
Func_08091858:
	push	{r5, r6, lr}
	ldr	r6, [pc, #44]
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r5, r6, r3
	ldrh	r0, [r5, #0]
	bl	Func_08091814
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #0
	strh	r3, [r5, #0]
.L0:
	ldr	r3, [pc, #24]
	adds	r5, r6, r3
	ldrh	r0, [r5, #0]
	bl	Func_08091814
	cmp	r0, #0
	beq.n	.L1
	movs	r3, #0
	strh	r3, [r5, #0]
.L1:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x00000222
