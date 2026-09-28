@ Flash library code whose C (recon/tbs/en/main/08007220.c) matches, kept as its
@ compiler's assembly until the library's original translation units and their
@ constant blocks are reconstructed.
	.code	16
.gcc2_compiled.:
.text
	.align	2, 0
	.globl	Func_08007220
	.type	 Func_08007220,function
	.thumb_func
Func_08007220:
	push	{r4, r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	add	sp, sp, #-64
	add	r7, r1, #0
	lsl	r0, r0, #16
	lsr	r4, r0, #16
	cmp	r4, #15
	bls	.L3	@cond_branch
	ldr	r0, .L18
	b	.L17
.L19:
	.align	2, 0
.L18:
	.word	33023
.L3:
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r2, .L20
	ldrh	r0, [r2]
	ldr	r1, .L20+4
	and	r0, r0, r1
	ldr	r1, .L20+8
	ldrh	r1, [r1, #36]
	orr	r0, r0, r1
	strh	r0, [r2]
	lsl	r0, r4, #21
	lsr	r5, r0, #16
	ldr	r1, .L20+12
	ldr	r0, .L20+16
	ldr	r0, [r0, #24]
	strh	r0, [r1]
	add	r0, r1, #0
	mov	r8, r0
	b	.L4
.L21:
	.align	2, 0
.L20:
	.word	67109380
	.word	65532
	.word	Data_08007c10
	.word	Data_02004c0c
	.word	Data_08007be4
.L15:
	ldr	r0, .L22
	ldr	r1, [r0, #24]
	mov	r2, r8
	ldrh	r2, [r2]
	sub	r0, r2, r1
	mov	r3, r8
	strh	r0, [r3]
	add	r7, r7, r1
	add	r0, r5, #1
	lsl	r0, r0, #16
	lsr	r5, r0, #16
.L4:
	mov	r1, r8
	ldrh	r0, [r1]
	cmp	r0, #0
	beq	.L5	@cond_branch
	mov	r4, #2
	b	.L7
.L23:
	.align	2, 0
.L22:
	.word	Data_08007c10
.L11:
	sub	r0, r4, #1
	lsl	r0, r0, #16
	lsr	r4, r0, #16
	cmp	r4, #0
	beq	.L9	@cond_branch
.L7:
	add	r0, r5, #0
	add	r1, r7, #0
	bl	Func_080071a8
	lsl	r0, r0, #16
	lsr	r6, r0, #16
	cmp	r6, #0
	bne	.L11	@cond_branch
.L9:
	cmp	r6, #0
	beq	.L15	@cond_branch
.L5:
	ldr	r2, .L24
	ldrh	r0, [r2]
	ldr	r1, .L24+4
	and	r0, r0, r1
	mov	r1, #3
	orr	r0, r0, r1
	strh	r0, [r2]
	add	r0, r6, #0
.L17:
	add	sp, sp, #64
	pop	{r3}
	mov	r8, r3
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
.L25:
	.align	2, 0
.L24:
	.word	67109380
	.word	65532
.Lfe1:
	.size	 Func_08007220,.Lfe1-Func_08007220

