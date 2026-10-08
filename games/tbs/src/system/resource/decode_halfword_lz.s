.syntax unified
	.arm
	.global Resource_DecodeHalfwordLz
Resource_DecodeHalfwordLz:
	mov	r3, #0x80000000
	b	.L_080022a4
.L_080022a0:
	strh	r2, [r1], #2
.L_080022a4:
	lsls	r3, r3, #1
.L_080022a8:
	ldrh	r2, [r0], #2
	bcc	.L_080022a0
	bne	.L_080022c0
	adc	r3, r2, r2
	lsls	r3, r3, #16
	b	.L_080022a8
.L_080022c0:
	cmp	r2, #0
	bxeq	lr
	sub	ip, r1, r2, lsr #5
	sub	ip, ip, r2, lsr #5
	and	r2, r2, #31
	add	r2, r2, #2
.L_080022d8:
	ldrh	r4, [ip], #2
	strh	r4, [r1], #2
	subs	r2, r2, #1
	bne	.L_080022d8
	b	.L_080022a4
	.global Resource_DecompressHalfwordsCodeSize
	.set Resource_DecompressHalfwordsCodeSize, . - Resource_DecodeHalfwordLz
