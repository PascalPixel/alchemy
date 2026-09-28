@ Flash library code whose C (recon/tbs/en/main/0800711c.c) matches, kept as its
@ compiler's assembly until the library's original translation units and their
@ constant blocks are reconstructed.
	.code	16
.gcc2_compiled.:
.text
	.align	2, 0
	.globl	Func_0800711c
	.type	 Func_0800711c,function
	.thumb_func
Func_0800711c:
	push	{r4, r5, r6, lr}
	add	sp, sp, #-64
	lsl	r0, r0, #16
	lsr	r4, r0, #16
	cmp	r4, #15
	bls	.L3	@cond_branch
	ldr	r0, .L14
	b	.L13
.L15:
	.align	2, 0
.L14:
	.word	33023
.L3:
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r2, .L16
	ldrh	r0, [r2]
	ldr	r1, .L16+4
	and	r0, r0, r1
	ldr	r1, .L16+8
	ldrh	r1, [r1, #36]
	orr	r0, r0, r1
	strh	r0, [r2]
	lsl	r0, r4, #21
	lsr	r5, r0, #16
	mov	r6, #0
.L4:
	mov	r4, #2
	b	.L5
.L17:
	.align	2, 0
.L16:
	.word	67109380
	.word	65532
	.word	Data_08007c10
.L6:
	sub	r0, r4, #1
	lsl	r0, r0, #16
	lsr	r4, r0, #16
	cmp	r4, #0
	beq	.L8	@cond_branch
.L5:
	add	r0, r5, #0
	bl	Func_08007098
	lsl	r0, r0, #16
	lsr	r3, r0, #16
	cmp	r3, #0
	bne	.L6	@cond_branch
.L8:
	add	r0, r5, #1
	lsl	r0, r0, #16
	lsr	r5, r0, #16
	cmp	r3, #0
	bne	.L11	@cond_branch
	add	r0, r6, #1
	lsl	r0, r0, #16
	lsr	r6, r0, #16
	cmp	r6, #31
	bls	.L4	@cond_branch
.L11:
	ldr	r2, .L18
	ldrh	r0, [r2]
	ldr	r1, .L18+4
	and	r0, r0, r1
	mov	r1, #3
	orr	r0, r0, r1
	strh	r0, [r2]
	add	r0, r3, #0
.L13:
	add	sp, sp, #64
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
.L19:
	.align	2, 0
.L18:
	.word	67109380
	.word	65532
.Lfe1:
	.size	 Func_0800711c,.Lfe1-Func_0800711c

