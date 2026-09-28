@ Flash library code whose C (recon/tbs/en/main/08006f84.c) matches, kept as its
@ compiler's assembly until the library's original translation units and their
@ constant blocks are reconstructed.
	.code	16
.gcc2_compiled.:
.text
	.align	2, 0
	.globl	Func_08006f84
	.type	 Func_08006f84,function
	.thumb_func
Func_08006f84:
	push	{r4, r5, r6, r7, lr}
	add	sp, sp, #-64
	add	r7, r1, #0
	lsl	r0, r0, #16
	lsr	r4, r0, #16
	cmp	r4, #15
	bls	.L3	@cond_branch
	ldr	r0, .L12
	b	.L10
.L13:
	.align	2, 0
.L12:
	.word	33023
.L3:
	add	r0, r4, #0
	bl	Func_08006d50
	lsl	r0, r0, #16
	lsr	r5, r0, #16
	cmp	r5, #0
	bne	.L11	@cond_branch
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r3, .L14
	ldrh	r1, [r3]
	ldr	r0, .L14+4
	and	r1, r1, r0
	ldr	r0, .L14+8
	ldr	r2, [r0]
	ldrh	r0, [r2, #16]
	orr	r0, r0, r1
	strh	r0, [r3]
	ldr	r1, .L14+12
	ldr	r0, [r2, #4]
	strh	r0, [r1]
	ldrb	r0, [r2, #8]
	lsl	r4, r4, r0
	mov	r0, #224
	lsl	r0, r0, #20
	add	r4, r4, r0
	add	r6, r1, #0
	b	.L5
.L15:
	.align	2, 0
.L14:
	.word	67109380
	.word	65532
	.word	33573896
	.word	Data_02004c0c
.L8:
	ldrh	r0, [r6]
	sub	r0, r0, #1
	strh	r0, [r6]
	add	r7, r7, #1
	add	r4, r4, #1
.L5:
	ldrh	r0, [r6]
	cmp	r0, #0
	beq	.L6	@cond_branch
	add	r0, r7, #0
	add	r1, r4, #0
	bl	Func_08006dec
	lsl	r0, r0, #16
	lsr	r5, r0, #16
	cmp	r5, #0
	beq	.L8	@cond_branch
.L6:
	ldr	r2, .L16
	ldrh	r0, [r2]
	ldr	r1, .L16+4
	and	r0, r0, r1
	mov	r1, #3
	orr	r0, r0, r1
	strh	r0, [r2]
.L11:
	add	r0, r5, #0
.L10:
	add	sp, sp, #64
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
.L17:
	.align	2, 0
.L16:
	.word	67109380
	.word	65532
.Lfe1:
	.size	 Func_08006f84,.Lfe1-Func_08006f84

