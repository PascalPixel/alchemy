@ Flash library code whose C (recon/tbs/en/main/08007098.c) matches, kept as its
@ compiler's assembly until the library's original translation units and their
@ constant blocks are reconstructed.
	.code	16
.gcc2_compiled.:
.text
	.align	2, 0
	.globl	Func_08007098
	.type	 Func_08007098,function
	.thumb_func
Func_08007098:
	push	{r4, r5, lr}
	lsl	r0, r0, #16
	ldr	r3, .L8
	lsr	r0, r0, #16
	add	r4, r0, #0
	ldrb	r1, [r3, #28]
	lsl	r4, r4, r1
	mov	r0, #224
	lsl	r0, r0, #20
	add	r4, r4, r0
	ldr	r1, .L8+4
	ldrh	r0, [r1]
	add	r5, r0, #0
	mov	r0, #0
	strh	r0, [r1]
	ldr	r2, .L8+8
	mov	r0, #170
	strb	r0, [r2]
	ldr	r1, .L8+12
	mov	r0, #85
	strb	r0, [r1]
	mov	r0, #160
	strb	r0, [r2]
	ldr	r0, [r3, #24]
	cmp	r0, #0
	beq	.L4	@cond_branch
	mov	r1, #255
.L5:
	strb	r1, [r4]
	add	r4, r4, #1
	sub	r0, r0, #1
	cmp	r0, #0
	bne	.L5	@cond_branch
.L4:
	sub	r4, r4, #1
	ldr	r0, .L8+4
	strh	r5, [r0]
	ldr	r0, .L8+16
	ldr	r3, [r0]
	mov	r0, #1
	add	r1, r4, #0
	mov	r2, #255
	bl	_call_via_r3
	lsl	r0, r0, #16
	lsr	r1, r0, #16
	cmp	r1, #0
	beq	.L7	@cond_branch
	mov	r0, #255
	lsl	r0, r0, #8
	and	r1, r1, r0
	mov	r0, #2
	orr	r1, r1, r0
.L7:
	add	r0, r1, #0
	pop	{r4, r5}
	pop	{r1}
	bx	r1
.L9:
	.align	2, 0
.L8:
	.word	Data_08007c10
	.word	67109384
	.word	234902869
	.word	234891946
	.word	33573888
.Lfe1:
	.size	 Func_08007098,.Lfe1-Func_08007098

