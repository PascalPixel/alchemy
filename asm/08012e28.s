@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08012e28
	.thumb_func
Func_08012e28:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #212]
	ldr	r1, [r2, #0]
	ldr	r3, [r1, #40]
	ldrb	r0, [r3, #4]
	ldr	r3, [pc, #208]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	sub	sp, #12
	mov	r8, r1
	movs	r6, #0
	str	r3, [sp, #4]
	str	r4, [sp, #8]
	ldr	r1, [r2, #20]
	cmp	r0, #6
	beq.n	.L0
	cmp	r0, #6
	bhi.n	.L1
	cmp	r0, #4
	beq.n	.L0
	cmp	r0, #4
	bhi.n	.L2
	movs	r5, #0
	ldr	r3, [pc, #176]
	movs	r4, #6
	cmp	r0, #3
	beq.n	.L3
	b.n	.L4
.L1:
	cmp	r0, #20
	beq.n	.L5
	cmp	r0, #20
	bhi.n	.L6
	cmp	r0, #8
	beq.n	.L2
	b.n	.L4
.L6:
	cmp	r0, #44
	beq.n	.L2
	cmp	r0, #88
	bne.n	.L4
.L2:
	movs	r3, #128
	movs	r5, #0
	lsls	r3, r3, #6
	movs	r4, #8
	b.n	.L3
.L0:
	movs	r5, #0
	ldr	r3, [pc, #136]
	movs	r4, #10
	b.n	.L3
.L5:
	movs	r5, #0
	movs	r3, #0
	ldr	r6, [pc, #128]
	b.n	.L7
.L4:
	movs	r5, #128
	movs	r3, #128
	lsls	r5, r5, #6
	lsls	r3, r3, #7
.L7:
	movs	r4, #4
.L3:
	movs	r7, #0
	cmp	r7, r4
	bcs.n	.L8
	lsls	r3, r3, #16
	movs	r2, #4
	mov	r9, r3
	lsls	r3, r6, #16
	add	r2, sp
	lsrs	r3, r3, #16
	mov	fp, r2
	mov	sl, r3
	adds	r6, r1, #0
.L10:
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r3, r5, #0
	mov	r0, r8
	adds	r1, r6, #0
	mov	r2, fp
	str	r4, [sp, #0]
	bl	Func_0800b168
	movs	r3, #56
	mov	r1, r9
	add	r8, r3
	lsrs	r3, r1, #16
	adds	r5, r5, r3
	movs	r3, #1
	lsls	r5, r5, #16
	ands	r3, r7
	asrs	r5, r5, #16
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L9
	lsls	r3, r5, #16
	lsrs	r3, r3, #16
	add	r3, sl
	lsls	r3, r3, #16
	asrs	r5, r3, #16
.L9:
	adds	r7, #1
	adds	r6, #16
	cmp	r7, r4
	bcc.n	.L10
.L8:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e60
	.4byte 0x08013584
	.4byte 0x00002aaa
	.4byte 0x00001999
	.4byte 0xffff8000
