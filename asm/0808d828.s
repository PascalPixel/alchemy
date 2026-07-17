@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808d828
	.thumb_func
Func_0808d828:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	adds	r1, r6, #0
	movs	r0, #2
	bl	Func_0808d48c
	ldr	r3, [pc, #176]
	ldr	r3, [r3, #0]
	movs	r7, #1
	adds	r5, r0, #0
	negs	r7, r7
	mov	r8, r3
	cmp	r5, #0
	beq.n	.L0
	ldr	r1, [r5, #8]
	cmp	r1, #0
	beq.n	.L0
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	subs	r2, #100
	add	r2, r8
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [r5, #8]
.L1:
	movs	r3, #128
	lsls	r3, r3, #9
	cmp	r1, r3
	bge.n	.L2
	bl	Func_080916b0
	ldr	r0, [r5, #8]
	bl	Func_08092b94
	adds	r0, r7, #0
	movs	r1, #0
	bl	Func_08092f84
	movs	r7, #0
	bl	Func_08091750
	b.n	.L3
.L2:
	adds	r0, r6, #0
	bl	Func_080072e8
	b.n	.L4
.L0:
	movs	r0, #1
	adds	r1, r6, #0
	bl	Func_0808d48c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L3
	ldr	r2, [r5, #0]
	movs	r3, #48
	ands	r2, r3
	cmp	r2, #48
	beq.n	.L5
	cmp	r2, #48
	bgt.n	.L6
	cmp	r2, #0
	beq.n	.L7
	cmp	r2, #32
	beq.n	.L8
	b.n	.L6
.L7:
	movs	r0, #123
	bl	Func_080f9010
	b.n	.L6
.L8:
	movs	r0, #128
	bl	Func_080f9010
	bl	Func_08094354
	b.n	.L6
.L5:
	movs	r0, #129
	bl	Func_080f9010
	bl	Func_08094368
.L6:
	movs	r3, #184
	ldr	r2, [r5, #8]
	lsls	r3, r3, #1
	add	r3, r8
	strh	r2, [r3, #0]
.L4:
	movs	r7, #0
.L3:
	adds	r0, r7, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ebc
