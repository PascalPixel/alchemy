@ C2: push saves r5/r6/r7 without lr.
.syntax unified
	.arm

	.global IwramDecompress
IwramDecompress:
	ldrb	r2, [r0], #1
	ldrb	r3, [r0], #1
	orrs	r2, r2, r3, lsl #8
	bxeq	lr
	push	{r5, r6, r7}
	mov	r6, r0
	add	r2, r2, r0
	sub	r2, r2, #2
.L_decompress_flag_reload:
	ldrb	r3, [r2], #1
	orr	r3, r3, #0x100
.L_decompress_flag:
	lsrs	r3, r3, #1
	bcc	.L_decompress_backref
	beq	.L_decompress_flag_reload
	ldrb	r4, [r0], #1
	subs	r5, r4, #0xdf
	addhi	r1, r1, r5
	strbls	r4, [r1], #1
	b	.L_decompress_flag
.L_decompress_backref:
	ldrb	r4, [r2], #1
	ldrb	ip, [r2], #1
	orrs	r4, ip, r4, lsl #8
	beq	.L_decompress_done
	bic	ip, r4, #0xf000
	sub	ip, r6, ip
	lsrs	r4, r4, #12
	ldrbeq	r4, [r2], #1
	addeq	r4, r4, #16
	add	r4, r4, #1
.L_decompress_copy:
	ldrb	r5, [ip], #1
	subs	r7, r5, #0xdf
	addhi	r1, r1, r7
	strbls	r5, [r1], #1
	subs	r4, r4, #1
	bpl	.L_decompress_copy
	b	.L_decompress_flag
.L_decompress_done:
	pop	{r5, r6, r7}
	mov	r0, #0
	bx	lr
