@ C2: Tile_Decompress4bpp:
@ push saves r5/r6 without lr and rrx consumes reader carry.
@ C2: Tile_Decompress4bppLoop:
@ Conditional rrxs consumes the shift carry in the bit reader.
@ C2: Tile_ExpandMasked:
@ push saves r5-r9/sl without lr.
.syntax unified
.text
	.arm
	.global Tile_Decompress4bpp
Tile_Decompress4bpp:
	push	{r5, r6}
	add	r5, pc, #40
	ldm	r5, {r2, r3}
	mov	r5, #15
	mov	ip, #1
	ands	r6, r0, #3
	beq Tile_Decompress4bppLoop
	rsbs	r6, ip, r6, lsl #3
	bic	r0, r0, #3
	ldr	ip, [r0], #4
	rrx	ip, ip
	lsr	ip, ip, r6
	b Tile_Decompress4bppLoop
.syntax unified
	.section .rodata,"a",%progbits
	.arm
	.4byte 0xfedcba98
	.4byte 0x76543210
.syntax unified
	.section .text.decompress_4bpp_loop,"ax",%progbits
	.arm
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
	.global Tile_Decompress4bppLoop
Tile_Decompress4bppLoop:
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
.syntax unified
	.section .text.decompress_4bpp_loop,"ax",%progbits
	.arm
	.global Tile_ExpandMasked
Tile_ExpandMasked:
	push	{r5, r6, r7, r8, r9, sl}
	add	sl, pc, #132
	ldm	sl, {r4, r6, r7}
.L_08015d80:
	add	sl, r0, r2, lsl #3
.L_08015d84:
	add	r9, r1, #32
.L_08015d88:
	ldm	r0, {r5, ip}
	orr	r5, r5, r5, lsr #4
	and	r5, r5, r4
	orr	r5, r5, r5, lsr #8
	and	r5, r5, r6
	orr	ip, ip, ip, lsr #4
	and	ip, ip, r4
	orr	ip, ip, ip, lsr #8
	orr	ip, r5, ip, lsl #16
	orr	r5, ip, ip, lsr #1
	orr	r5, r5, r5, lsr #2
	and	r5, r5, r7
	rsbs	r5, r5, r5, lsl #4
	mvns	r8, r5
	ldrne	r8, [r1]
	bicne	r8, r8, r5
	orrne	ip, ip, r8
	str	ip, [r1], #4
	add	r0, r0, r2, lsl #3
	cmp	r1, r9
	bne .L_08015d88
	sub	r0, r0, r2, lsl #6
	add	r0, r0, #8
	cmp	r0, sl
	bne .L_08015d84
	add	r0, r0, r2, lsl #6
	sub	r0, r0, r2, lsl #3
	subs	r3, r3, #1
	bne .L_08015d80
	pop	{r5, r6, r7, r8, r9, sl}
	bx	lr
