@ Decode loop of DECOMPRESS_4BPP.S. Code bits are read with lsrs and bcs
@ or bcc, each bit word is refilled through the carry (ldreq; rrxseq), the
@ nibbles come from the move-to-front table in r2 and r3, and it returns
@ for the entry with pop {r5, r6}; bx lr. GCC never branches on the carry
@ out of a shift and never emits rrx. Every TBS and TLA edition has these
@ bytes.
@ credit: library — TBS/TLA common ARM text and tile kernel bank
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08015b38
.L_08015b38:
	orr	r2, r4, r3, lsr #28
	orr	r3, r6, r3, lsl #4
.L_08015b40:
	and	r6, r3, r5
.L_08015b44:
	strb	r6, [r1], #1
	lsrs	ip, ip, #1
	bcs .L_08015b5c
	strb	r6, [r1], #1
.L_08015b54:
	lsrs	ip, ip, #1
	bcc .L_08015b40
.L_08015b5c:
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcc .L_08015b40
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcc .L_08015d10
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcc .L_08015cd8
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .L_08015bac
	lsr	r4, r3, #28
	orr	r4, r4, r3, lsl #8
	and	r6, r5, r3, lsr #24
	orr	r3, r6, r4, ror #4
	b .L_08015b44
.L_08015bac:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .L_08015be4
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .L_08015bd8
	ror	r3, r3, #28
	and	r6, r5, r3
	b .L_08015b44
.L_08015bd8:
	and	r6, r5, r2
	bic	r4, r2, #15
	b .L_08015b38
.L_08015be4:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .L_08015c08
	and	r6, r5, r2, lsr #4
	lsr	r4, r2, #8
	orr	r4, r4, r2, lsl #28
	ror	r4, r4, #24
	b .L_08015b38
.L_08015c08:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .L_08015c50
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .L_08015c3c
	and	r6, r5, r2, lsr #8
	lsr	r4, r2, #12
	orr	r4, r4, r2, lsl #24
	ror	r4, r4, #20
	b .L_08015b38
.L_08015c3c:
	and	r6, r5, r2, lsr #12
	lsr	r4, r2, #16
	orr	r4, r4, r2, lsl #20
	ror	r4, r4, #16
	b .L_08015b38
.L_08015c50:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .L_08015c74
	and	r6, r5, r2, lsr #16
	lsr	r4, r2, #20
	orr	r4, r4, r2, lsl #16
	ror	r4, r4, #12
	b .L_08015b38
.L_08015c74:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .L_08015cbc
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .L_08015ca8
	and	r6, r5, r2, lsr #20
	lsr	r4, r2, #24
	orr	r4, r4, r2, lsl #12
	ror	r4, r4, #8
	b .L_08015b38
.L_08015ca8:
	and	r6, r5, r2, lsr #24
	lsr	r4, r2, #28
	orr	r4, r4, r2, lsl #8
	ror	r4, r4, #4
	b .L_08015b38
.L_08015cbc:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .L_08015d6c
	and	r6, r5, r2, lsr #28
	lsl	r4, r2, #4
	b .L_08015b38
.L_08015cd8:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcc .L_08015cfc
	lsr	r4, r3, #24
	orr	r4, r4, r3, lsl #12
	and	r6, r5, r3, lsr #20
	orr	r3, r6, r4, ror #8
	b .L_08015b44
.L_08015cfc:
	lsr	r4, r3, #20
	orr	r4, r4, r3, lsl #16
	and	r6, r5, r3, lsr #16
	orr	r3, r6, r4, ror #12
	b .L_08015b44
.L_08015d10:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcc .L_08015d58
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .L_08015d44
	lsr	r4, r3, #12
	orr	r4, r4, r3, lsl #24
	and	r6, r5, r3, lsr #8
	orr	r3, r6, r4, ror #20
	b .L_08015b44
.L_08015d44:
	lsr	r4, r3, #16
	orr	r4, r4, r3, lsl #20
	and	r6, r5, r3, lsr #12
	orr	r3, r6, r4, ror #16
	b .L_08015b44
.L_08015d58:
	lsr	r4, r3, #8
	orr	r4, r4, r3, lsl #28
	and	r6, r5, r3, lsr #4
	orr	r3, r6, r4, ror #24
	b .L_08015b44
.L_08015d6c:
	pop	{r5, r6}
	bx	lr
