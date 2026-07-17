@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08006d50
	.thumb_func
Func_08006d50:
	push	{r4, r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #64
	lsls	r0, r0, #16
	lsrs	r3, r0, #16
	cmp	r3, #15
	bhi.n	.L0
	ldr	r6, [pc, #92]
	ldrh	r1, [r6, #0]
	ldr	r0, [pc, #92]
	mov	r8, r0
	ands	r1, r0
	ldr	r0, [pc, #92]
	ldr	r2, [r0, #0]
	ldrh	r0, [r2, #16]
	orrs	r0, r1
	strh	r0, [r6, #0]
	ldrb	r4, [r2, #8]
	lsls	r3, r4
	adds	r4, r3, #0
	movs	r2, #224
	lsls	r2, r2, #20
	adds	r4, r4, r2
	ldr	r1, [pc, #72]
	movs	r5, #170
	strb	r5, [r1, #0]
	ldr	r3, [pc, #72]
	movs	r2, #85
	strb	r2, [r3, #0]
	movs	r0, #128
	strb	r0, [r1, #0]
	strb	r5, [r1, #0]
	strb	r2, [r3, #0]
	movs	r0, #48
	strb	r0, [r4, #0]
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r0, [pc, #52]
	ldr	r3, [r0, #0]
	movs	r0, #2
	adds	r1, r4, #0
	movs	r2, #255
	bl	Func_080072f0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	ldrh	r1, [r6, #0]
	mov	r2, r8
	ands	r1, r2
	movs	r2, #3
	orrs	r1, r2
	strh	r1, [r6, #0]
	b.n	.L1
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x02004c08
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.4byte 0x02004c00
.L0:
	ldr	r0, [pc, #12]
.L1:
	add	sp, #64
	pop	{r3}
	mov	r8, r3
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000080ff
