@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080207c4
	.thumb_func
Func_080207c4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	mov	r8, r2
	bl	Func_080056cc
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L0
	ldr	r0, [pc, #228]
	movs	r1, #1
	bl	Func_0801776c
	movs	r3, #9
	negs	r3, r3
	mov	r8, r3
	b.n	.L1
.L0:
	bl	Func_08005c68
	ldr	r3, [pc, #212]
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #212]
	movs	r1, #0
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	Func_08020244
	movs	r3, #1
	adds	r7, r0, #0
	negs	r3, r3
	cmp	r7, r3
	bne.n	.L2
	mov	r8, r7
	b.n	.L1
.L2:
	ldr	r2, [pc, #192]
	lsls	r3, r7, #6
	adds	r3, r3, r2
	ldrb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L3
	ldr	r0, [pc, #184]
	movs	r1, #13
	bl	Func_0801776c
	b.n	.L4
.L5:
	movs	r0, #1
	bl	Func_080030f8
.L4:
	bl	Func_08017364
	cmp	r0, #0
	beq.n	.L5
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	movs	r3, #1
	bl	Func_08028df4
	cmp	r0, #0
	beq.n	.L6
	bl	Func_08019a54
	b.n	.L1
.L6:
	bl	Func_08019a54
.L3:
	ldr	r3, [pc, #124]
	movs	r0, #85
	strh	r7, [r3, #0]
	bl	Func_080f9010
	ldr	r0, [pc, #128]
	movs	r1, #13
	bl	Func_0801776c
	b.n	.L7
.L8:
	movs	r0, #1
	bl	Func_080030f8
.L7:
	bl	Func_08017364
	cmp	r0, #0
	beq.n	.L8
	bl	Func_0801f818
	bl	Func_0808a5b0
	ldr	r5, [pc, #100]
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	Func_08005920
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r5, r5, r3
	adds	r6, r0, #0
	adds	r1, r5, #0
	adds	r0, r7, #3
	bl	Func_08005920
	orrs	r6, r0
	bl	Func_08019a54
	cmp	r6, #0
	beq.n	.L9
	ldr	r0, [pc, #68]
	movs	r1, #1
	bl	Func_0801776c
	movs	r2, #3
	negs	r2, r2
	mov	r8, r2
	b.n	.L1
.L9:
	ldr	r0, [pc, #56]
	movs	r1, #9
	bl	Func_0801776c
.L1:
	bl	Func_08005cf8
	mov	r0, r8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000000a
	.4byte 0x03001f1c
	.4byte 0x02002004
	.4byte 0x0000105c
	.4byte 0x00000014
	.4byte 0x0000001a
	.4byte 0x02000000
	.4byte 0x0000000b
	.4byte 0x00000017
