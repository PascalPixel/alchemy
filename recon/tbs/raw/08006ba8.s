@ Flash library code whose C (recon/tbs/en/main/08006ba8.c) matches, kept as its
@ compiler's assembly until the library's original translation units and their
@ constant blocks are reconstructed.
	.code	16
.gcc2_compiled.:
.text
	.align	2, 0
	.globl	Func_08006ba8
	.type	 Func_08006ba8,function
	.thumb_func
Func_08006ba8:
	push	{r4, r5, r6, r7, lr}
	add	sp, sp, #-128
	add	r5, r1, #0
	add	r6, r2, #0
	add	r7, r3, #0
	lsl	r0, r0, #16
	lsr	r4, r0, #16
	ldr	r2, .L8
	ldrh	r0, [r2]
	ldr	r1, .L8+4
	and	r0, r0, r1
	mov	r1, #3
	orr	r0, r0, r1
	strh	r0, [r2]
	ldr	r3, .L8+8
	mov	r0, #1
	eor	r3, r3, r0
	mov	r2, sp
	ldr	r0, .L8+12
	ldr	r1, .L8+8
	sub	r0, r0, r1
	lsl	r0, r0, #15
	b	.L7
.L9:
	.align	2, 0
.L8:
	.word	67109380
	.word	65532
	.word	ReadFlashCore
	.word	Func_08006ba8
.L5:
	ldrh	r0, [r3]
	strh	r0, [r2]
	add	r3, r3, #2
	add	r2, r2, #2
	sub	r0, r1, #1
	lsl	r0, r0, #16
.L7:
	lsr	r1, r0, #16
	cmp	r1, #0
	bne	.L5	@cond_branch
	mov	r3, sp
	add	r3, r3, #1
	ldr	r0, .L10
	ldrb	r0, [r0, #28]
	lsl	r4, r4, r0
	add	r0, r4, #0
	mov	r2, #224
	lsl	r2, r2, #20
	add	r1, r5, r2
	add	r0, r0, r1
	add	r1, r6, #0
	add	r2, r7, #0
	bl	_call_via_r3
	add	sp, sp, #128
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
.L11:
	.align	2, 0
.L10:
	.word	Data_08007abc
.Lfe1:
	.size	 Func_08006ba8,.Lfe1-Func_08006ba8

