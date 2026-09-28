@ Flash library code whose C (recon/tbs/en/main/08007028.c) matches, kept as its
@ compiler's assembly until the library's original translation units and their
@ constant blocks are reconstructed.
	.code	16
.gcc2_compiled.:
.text
	.align	2, 0
	.globl	Func_08007028
	.type	 Func_08007028,function
	.thumb_func
Func_08007028:
	push	{r4, r5, r6, lr}
	add	sp, sp, #-64
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r5, .L3
	ldrh	r0, [r5]
	ldr	r6, .L3+4
	and	r0, r0, r6
	ldr	r1, .L3+8
	ldrh	r1, [r1, #36]
	orr	r0, r0, r1
	strh	r0, [r5]
	ldr	r1, .L3+12
	mov	r4, #170
	strb	r4, [r1]
	ldr	r3, .L3+16
	mov	r2, #85
	strb	r2, [r3]
	mov	r0, #128
	strb	r0, [r1]
	strb	r4, [r1]
	strb	r2, [r3]
	mov	r0, #16
	strb	r0, [r1]
	ldr	r0, .L3+20
	mov	r1, #224
	lsl	r1, r1, #20
	ldr	r3, [r0]
	mov	r0, #3
	mov	r2, #255
	bl	_call_via_r3
	lsl	r0, r0, #16
	lsr	r0, r0, #16
	ldrh	r1, [r5]
	and	r1, r1, r6
	mov	r2, #3
	orr	r1, r1, r2
	strh	r1, [r5]
	add	sp, sp, #64
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
.L4:
	.align	2, 0
.L3:
	.word	67109380
	.word	65532
	.word	Data_08007c10
	.word	234902869
	.word	234891946
	.word	33573888
.Lfe1:
	.size	 Func_08007028,.Lfe1-Func_08007028

