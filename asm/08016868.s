@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08016868
	.thumb_func
Func_08016868:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #128]
	movs	r2, #196
	ldr	r3, [r3, #0]
	lsls	r2, r2, #3
	adds	r5, r3, r2
	movs	r3, #1
	movs	r7, #0
	mov	r8, r3
.L6:
	ldr	r2, [r5, #0]
	cmp	r2, #0
	beq.n	.L0
	ldr	r3, [r2, #24]
	cmp	r3, #0
	bne.n	.L0
	ldrh	r3, [r2, #22]
	cmp	r3, #0
	bne.n	.L1
	str	r3, [r5, #0]
	b.n	.L0
.L1:
	ldrh	r6, [r2, #18]
	cmp	r6, #0
	beq.n	.L2
	adds	r0, r5, #0
	bl	Func_08019854
	b.n	.L0
.L2:
	adds	r0, r5, #0
	bl	Func_080168f4
	cmp	r0, #8
	beq.n	.L3
	cmp	r0, #9
	beq.n	.L4
	b.n	.L0
.L3:
	ldr	r3, [r5, #0]
	mov	r2, r8
	b.n	.L5
.L4:
	ldr	r0, [r5, #0]
	ldrh	r3, [r0, #22]
	movs	r1, #2
	ands	r1, r3
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	bl	Func_08016418
	ldr	r3, [r5, #0]
	mov	r2, r8
	strh	r6, [r5, #4]
	strh	r6, [r5, #6]
	strh	r6, [r5, #18]
	strh	r6, [r5, #20]
	strh	r6, [r5, #22]
	strh	r6, [r5, #24]
	strh	r6, [r5, #26]
.L5:
	strh	r2, [r3, #20]
.L0:
	adds	r7, #1
	adds	r5, #40
	cmp	r7, #3
	bne.n	.L6
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
