@ Halfword LZ decoder that Resource_DecompressHalfwords (0x080052f4) copies
@ onto the stack, exactly these 84 bytes, and calls there. It is ARM-state
@ code whose flag word carries a sentinel bit, is tested with lsls and bcc
@ and is refilled through the carry (adc r3, r2, r2), with no frame and a
@ conditional bxeq return. GCC never branches on the carry out of a shift and
@ uses adc only for 64-bit sums. Every TBS and TLA edition has these bytes.
@ credit: library — TBS/TLA common ARM halfword-LZ decoder copied to the stack
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08002298
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
