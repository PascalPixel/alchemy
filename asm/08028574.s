@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08028574
	.thumb_func
Func_08028574:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #268]
	ldr	r3, [r3, #0]
	movs	r1, #140
	mov	r8, r3
	add	r1, r8
	mov	sl, r1
	mov	r2, sl
	movs	r3, #146
	add	r3, r8
	strh	r0, [r2, #0]
	mov	fp, r3
.L7:
	mov	r1, r8
	ldr	r0, [r1, #120]
	bl	Func_08016478
	mov	r3, fp
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L0
	mov	r2, sl
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	adds	r0, r0, r3
	b.n	.L1
.L4:
	movs	r0, #113
	bl	Func_080f9010
	movs	r0, #1
	negs	r0, r0
	b.n	.L2
.L0:
	mov	r0, sl
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	mov	r1, r8
	adds	r3, #132
	ldrb	r2, [r1, r3]
	ldr	r3, [pc, #196]
	adds	r0, r2, r3
.L1:
	mov	r2, r8
	ldr	r1, [r2, #120]
	movs	r3, #0
	movs	r2, #0
	bl	Func_0801e7c0
	movs	r3, #142
	add	r3, r8
	ldr	r7, [pc, #180]
	mov	r6, sl
	mov	r9, r3
.L9:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [r7, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L3
	ldr	r2, [r7, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L4
	ldr	r2, [r7, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L4
	ldr	r1, [pc, #144]
	ldr	r2, [r1, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L5
	ldr	r5, [r1, #0]
	movs	r3, #64
	ands	r5, r3
	cmp	r5, #0
	beq.n	.L6
.L5:
	movs	r0, #111
	bl	Func_080f9010
	ldrh	r3, [r6, #0]
	subs	r3, #1
	strh	r3, [r6, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bge.n	.L7
	mov	r0, r9
	ldrh	r3, [r0, #0]
	mov	r1, sl
	subs	r3, #1
	strh	r3, [r1, #0]
	b.n	.L7
.L6:
	ldr	r2, [r1, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L8
	ldr	r2, [r1, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L9
.L8:
	movs	r0, #111
	bl	Func_080f9010
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
	mov	r1, r9
	lsls	r3, r3, #16
	movs	r0, #0
	ldrsh	r2, [r1, r0]
	asrs	r3, r3, #16
	cmp	r3, r2
	blt.n	.L7
	mov	r2, sl
	strh	r5, [r2, #0]
	b.n	.L7
.L3:
	movs	r0, #112
	bl	Func_080f9010
	mov	r1, sl
	movs	r3, #0
	ldrsh	r0, [r1, r3]
.L2:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f38
	.4byte 0x0000001f
	.4byte 0x03001c94
	.4byte 0x03001b04
