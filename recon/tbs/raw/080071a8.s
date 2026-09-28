@ Flash library code whose C (recon/tbs/en/main/080071a8.c) matches, kept as its
@ compiler's assembly until the library's original translation units and their
@ constant blocks are reconstructed.
	.code	16
.gcc2_compiled.:
.text
	.align	2, 0
	.globl	Func_080071a8
	.type	 Func_080071a8,function
	.thumb_func
Func_080071a8:
	push	{r4, r5, r6, lr}
	add	r5, r1, #0
	lsl	r0, r0, #16
	ldr	r3, .L9
	lsr	r0, r0, #16
	add	r4, r0, #0
	ldrb	r1, [r3, #28]
	lsl	r4, r4, r1
	mov	r0, #224
	lsl	r0, r0, #20
	add	r4, r4, r0
	ldr	r1, .L9+4
	ldrh	r0, [r1]
	add	r6, r0, #0
	mov	r0, #0
	strh	r0, [r1]
	ldr	r2, .L9+8
	mov	r0, #170
	strb	r0, [r2]
	ldr	r1, .L9+12
	mov	r0, #85
	strb	r0, [r1]
	mov	r0, #160
	strb	r0, [r2]
	ldr	r1, [r3, #24]
	cmp	r1, #0
	beq	.L3	@cond_branch
.L4:
	ldrb	r0, [r5]
	strb	r0, [r4]
	add	r5, r5, #1
	add	r4, r4, #1
	sub	r1, r1, #1
	cmp	r1, #0
	bne	.L4	@cond_branch
.L3:
	sub	r4, r4, #1
	sub	r5, r5, #1
	ldr	r0, .L9+4
	strh	r6, [r0]
	ldr	r0, .L9+16
	ldrb	r2, [r5]
	ldr	r3, [r0]
	mov	r0, #1
	add	r1, r4, #0
	bl	_call_via_r3
	lsl	r0, r0, #16
	lsr	r0, r0, #16
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
.L10:
	.align	2, 0
.L9:
	.word	Data_08007c10
	.word	67109384
	.word	234902869
	.word	234891946
	.word	33573888
.Lfe1:
	.size	 Func_080071a8,.Lfe1-Func_080071a8

