@ Flash library code whose C (recon/tbs/en/main/08006c68.c) matches, kept as its
@ compiler's assembly until the library's original translation units and their
@ constant blocks are reconstructed.
	.code	16
.gcc2_compiled.:
.text
	.align	2, 0
	.globl	Flash_VerifySector
	.type	 Flash_VerifySector,function
	.thumb_func
Flash_VerifySector:
	push	{r4, r5, lr}
	add	sp, sp, #-256
	add	r5, r1, #0
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
	.word	Func_08006c24
	.word	Flash_VerifySector
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
	ldrb	r1, [r0, #28]
	lsl	r4, r4, r1
	add	r1, r4, #0
	mov	r2, #224
	lsl	r2, r2, #20
	add	r1, r1, r2
	ldrh	r2, [r0, #24]
	add	r0, r5, #0
	bl	_call_via_r3
	add	sp, sp, #256
	pop	{r4, r5}
	pop	{r1}
	bx	r1
.L11:
	.align	2, 0
.L10:
	.word	Data_08007abc
.Lfe1:
	.size	 Flash_VerifySector,.Lfe1-Flash_VerifySector

