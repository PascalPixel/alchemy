@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08006af8
	.thumb_func
Func_08006af8:
	push	{r4, r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r4, r0, #0
	adds	r5, r1, #0
	lsls	r4, r4, #24
	lsrs	r4, r4, #24
	lsls	r2, r2, #24
	lsrs	r6, r2, #24
	movs	r0, #0
	mov	r8, r0
	adds	r0, r4, #0
	bl	Func_08006a00
	ldr	r7, [pc, #8]
	movs	r0, #192
	lsls	r0, r0, #8
	orrs	r4, r0
	lsls	r4, r4, #16
	b.n	.L0
	.4byte 0x02004c1c
.L3:
	ldr	r0, [pc, #20]
	ldr	r1, [r0, #0]
	ldr	r0, [pc, #20]
	ldrh	r1, [r1, #20]
	cmp	r1, r0
	bne.n	.L1
	ldr	r1, [pc, #16]
	movs	r0, #240
	strb	r0, [r1, #0]
.L1:
	lsrs	r4, r4, #16
	mov	r8, r4
	b.n	.L2
	.4byte 0x02004c08
	.4byte 0x00001cc2
	.4byte 0x0e005555
.L0:
	ldr	r1, [r7, #0]
	adds	r0, r5, #0
	bl	Func_080072e8
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	cmp	r0, r6
	beq.n	.L2
	ldr	r0, [pc, #36]
	ldrb	r0, [r0, #0]
	cmp	r0, #0
	beq.n	.L0
	ldr	r1, [r7, #0]
	adds	r0, r5, #0
	bl	Func_080072e8
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	cmp	r0, r6
	bne.n	.L3
.L2:
	bl	Func_08006a78
	mov	r0, r8
	pop	{r3}
	mov	r8, r3
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x02004c24
