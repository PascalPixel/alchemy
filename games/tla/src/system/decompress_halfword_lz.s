.syntax unified
	.thumb
	.global Resource_DecodeHalfwordLzCode
Resource_DecodeHalfwordLzCode:
	.global Resource_DecodeHalfwordLz
	.thumb_func
Resource_DecodeHalfwordLz:
	bx	pc
AlchemyUncredited_08001ffe:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001ffe:
	mov	r3, #0x80000000
	b	.L_0800200c
.L_08002008:
	strh	r2, [r1], #2
.L_0800200c:
	lsls	r3, r3, #1
.L_08002010:
	ldrh	r2, [r0], #2
	bcc	.L_08002008
	bne	.L_08002028
	adc	r3, r2, r2
	lsls	r3, r3, #16
	b	.L_08002010
.L_08002028:
	cmp	r2, #0
	bxeq	lr
	sub	ip, r1, r2, lsr #5
	sub	ip, ip, r2, lsr #5
	and	r2, r2, #31
	add	r2, r2, #2
.L_08002040:
	ldrh	r4, [ip], #2
	strh	r4, [r1], #2
	subs	r2, r2, #1
	bne	.L_08002040
	b	.L_0800200c
	.global Resource_DecompressHalfwordsCodeSize
	.set Resource_DecompressHalfwordsCodeSize, . - Resource_DecodeHalfwordLzCode
