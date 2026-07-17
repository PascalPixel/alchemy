@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801c188
	.thumb_func
Func_0801c188:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #128]
	ldr	r5, [r3, #0]
	adds	r0, r5, #0
	bl	Func_0801b36c
	adds	r6, r0, #0
	ldrh	r3, [r6, #10]
	cmp	r3, #1
	beq.n	.L0
	cmp	r3, #6
	bne.n	.L1
.L0:
	movs	r1, #193
	lsls	r1, r1, #3
	movs	r0, #17
	bl	Func_080048b0
	movs	r3, #195
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldrh	r3, [r6, #8]
	adds	r7, r0, #0
	ldr	r0, [pc, #88]
	mov	r8, r3
	bl	Func_08002f40
	ldr	r3, [pc, #84]
	adds	r2, r7, r3
	ldrh	r3, [r6, #8]
	lsls	r3, r3, #1
	ldrh	r3, [r3, r0]
	adds	r0, r0, r3
	str	r0, [r2, #0]
	adds	r1, r7, #0
	bl	Func_080053e8
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	bne.n	.L2
	bl	Func_08004080
	strh	r0, [r5, #12]
.L2:
	movs	r1, #128
	ldrh	r0, [r5, #12]
	lsls	r1, r1, #3
	adds	r2, r7, #0
	bl	Func_08003fa4
	movs	r3, #1
	strh	r3, [r5, #10]
	mov	r3, r8
	strh	r3, [r5, #8]
	movs	r3, #40
	strh	r3, [r5, #34]
	strh	r3, [r5, #36]
	movs	r3, #240
	strh	r0, [r5, #14]
	strh	r3, [r5, #38]
	movs	r0, #17
	bl	Func_08002dd8
.L1:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e98
	.4byte 0x000000f1
	.4byte 0x00000604
