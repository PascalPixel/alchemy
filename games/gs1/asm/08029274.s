@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x08029274 (80 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_08029274
	.thumb_func
Func_08029274:
	push	{r5, r6, lr}
	sub	sp, #8
	adds	r5, r2, #0
	cmp	r1, #5
	bls.n	.L_08029280
	movs	r1, #5
.L_08029280:
	movs	r2, #0
	cmp	r1, #0
	beq.n	.L_080292a4
	movs	r6, #15
	mov	r4, sp
.L_0802928a:
	adds	r3, r0, #0
	ands	r3, r6
	cmp	r3, #9
	bhi.n	.L_08029296
	adds	r3, #48	@ 0x30
	b.n	.L_08029298
.L_08029296:
	adds	r3, #55	@ 0x37
.L_08029298:
	strb	r3, [r4, #0]
	adds	r2, #1
	lsrs	r0, r0, #4
	adds	r4, #1
	cmp	r2, r1
	bne.n	.L_0802928a
.L_080292a4:
	subs	r2, r1, #1
	cmp	r2, #0
	blt.n	.L_080292bc
	mov	r3, sp
	adds	r1, r2, r3
	mov	ip, r3
.L_080292b0:
	ldrb	r3, [r1, #0]
	subs	r1, #1
	strb	r3, [r5, #0]
	adds	r5, #1
	cmp	r1, ip
	bge.n	.L_080292b0
.L_080292bc:
	add	sp, #8
	pop	{r5, r6}
	pop	{r0}
	bx	r0
