@ Flash library code whose C (recon/tbs/en/main/08006e24.c) matches, kept as its
@ compiler's assembly until the library's original translation units and their
@ constant blocks are reconstructed.
	.code	16
.gcc2_compiled.:
.text
	.align	2, 0
	.globl	Func_08006e24
	.type	 Func_08006e24,function
	.thumb_func
Func_08006e24:
	push	{r4, r5, r6, r7, lr}
	mov	r7, r9
	mov	r6, r8
	push	{r6, r7}
	add	sp, sp, #-96
	mov	r9, r1
	lsl	r0, r0, #16
	lsr	r0, r0, #16
	mov	r8, r0
	cmp	r0, #15
	bls	.L3	@cond_branch
	ldr	r0, .L29
	b	.L25
.L30:
	.align	2, 0
.L29:
	.word	33023
.L3:
	ldr	r0, .L31
	ldr	r0, [r0]
	ldrb	r0, [r0, #8]
	mov	r7, r8
	lsl	r7, r7, r0
	mov	r0, #224
	lsl	r0, r0, #20
	add	r7, r7, r0
	ldr	r1, .L31+4
	mov	r0, #1
	add	r3, r1, #0
	eor	r3, r3, r0
	mov	r2, sp
	ldr	r0, .L31+8
	sub	r0, r0, r1
	b	.L28
.L32:
	.align	2, 0
.L31:
	.word	33573896
	.word	Func_08006f48
	.word	Func_08006f6c
.L6:
	ldrh	r0, [r3]
	strh	r0, [r2]
	add	r3, r3, #2
	add	r2, r2, #2
	sub	r0, r1, #2
.L28:
	lsl	r0, r0, #16
	lsr	r1, r0, #16
	cmp	r1, #0
	bne	.L6	@cond_branch
	mov	r4, #0
	b	.L8
.L11:
	add	r0, r4, #1
	lsl	r0, r0, #24
	lsr	r4, r0, #24
	cmp	r4, #81
	beq	.L27	@cond_branch
.L8:
	mov	r0, r8
	bl	Func_08006d50
	lsl	r0, r0, #16
	lsr	r5, r0, #16
	cmp	r5, #0
	bne	.L11	@cond_branch
	add	r0, r7, #0
	mov	r1, sp
	add	r1, r1, #1
	bl	Func_08006f6c
	lsl	r0, r0, #16
	lsr	r5, r0, #16
	cmp	r5, #0
	bne	.L11	@cond_branch
	mov	r6, #1
	cmp	r4, #0
	beq	.L15	@cond_branch
	mov	r6, #6
.L15:
	mov	r4, #1
	cmp	r4, r6
	bhi	.L17	@cond_branch
.L18:
	mov	r0, r8
	bl	Func_08006d50
	add	r0, r4, #1
	lsl	r0, r0, #24
	lsr	r4, r0, #24
	cmp	r4, r6
	bls	.L18	@cond_branch
.L17:
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r3, .L33
	ldrh	r1, [r3]
	ldr	r0, .L33+4
	and	r1, r1, r0
	ldr	r0, .L33+8
	ldr	r2, [r0]
	ldrh	r0, [r2, #16]
	orr	r0, r0, r1
	strh	r0, [r3]
	ldr	r1, .L33+12
	ldr	r0, [r2, #4]
	strh	r0, [r1]
	add	r4, r1, #0
	b	.L20
.L34:
	.align	2, 0
.L33:
	.word	67109380
	.word	65532
	.word	33573896
	.word	33573900
.L23:
	ldrh	r0, [r4]
	sub	r0, r0, #1
	strh	r0, [r4]
	mov	r0, #1
	add	r9, r9, r0
	add	r7, r7, #1
.L20:
	ldrh	r0, [r4]
	cmp	r0, #0
	beq	.L21	@cond_branch
	mov	r0, r9
	add	r1, r7, #0
	bl	Func_08006dec
	lsl	r0, r0, #16
	lsr	r5, r0, #16
	cmp	r5, #0
	beq	.L23	@cond_branch
.L21:
	ldr	r2, .L35
	ldrh	r0, [r2]
	ldr	r1, .L35+4
	and	r0, r0, r1
	mov	r1, #3
	orr	r0, r0, r1
	strh	r0, [r2]
.L27:
	add	r0, r5, #0
.L25:
	add	sp, sp, #96
	pop	{r3, r4}
	mov	r8, r3
	mov	r9, r4
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
.L36:
	.align	2, 0
.L35:
	.word	67109380
	.word	65532
.Lfe1:
	.size	 Func_08006e24,.Lfe1-Func_08006e24

