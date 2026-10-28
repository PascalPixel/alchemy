@ Why Tile_Decompress4bpp is assembly:
@ push saves r5/r6 without lr and rrx consumes reader carry.
@ Why Tile_Decompress4bppLoop is assembly:
@ Conditional rrxs consumes the shift carry in the bit reader.
@ Why Tile_ExpandMasked is assembly:
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
.Lpush_front:
	orr	r2, r4, r3, lsr #28
	orr	r3, r6, r3, lsl #4
.Lfront:
	and	r6, r3, r5
.Lput:
	strb	r6, [r1], #1
	lsrs	ip, ip, #1
	bcs .Lcode
	strb	r6, [r1], #1
	.global Tile_Decompress4bppLoop
Tile_Decompress4bppLoop:
	lsrs	ip, ip, #1
	bcc .Lfront
.Lcode:
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcc .Lfront
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcc .Lcode_1_3
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcc .Lcode_4_5
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .Lcode_7_up
	lsr	r4, r3, #28
	orr	r4, r4, r3, lsl #8
	and	r6, r5, r3, lsr #24
	orr	r3, r6, r4, ror #4
	b .Lput
.Lcode_7_up:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .Lcode_9_up
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .Ltake_8
	ror	r3, r3, #28
	and	r6, r5, r3
	b .Lput
.Ltake_8:
	and	r6, r5, r2
	bic	r4, r2, #15
	b .Lpush_front
.Lcode_9_up:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .Lcode_10_up
	and	r6, r5, r2, lsr #4
	lsr	r4, r2, #8
	orr	r4, r4, r2, lsl #28
	ror	r4, r4, #24
	b .Lpush_front
.Lcode_10_up:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .Lcode_12_up
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .Ltake_11
	and	r6, r5, r2, lsr #8
	lsr	r4, r2, #12
	orr	r4, r4, r2, lsl #24
	ror	r4, r4, #20
	b .Lpush_front
.Ltake_11:
	and	r6, r5, r2, lsr #12
	lsr	r4, r2, #16
	orr	r4, r4, r2, lsl #20
	ror	r4, r4, #16
	b .Lpush_front
.Lcode_12_up:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .Lcode_13_up
	and	r6, r5, r2, lsr #16
	lsr	r4, r2, #20
	orr	r4, r4, r2, lsl #16
	ror	r4, r4, #12
	b .Lpush_front
.Lcode_13_up:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .Lcode_15_end
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .Ltake_14
	and	r6, r5, r2, lsr #20
	lsr	r4, r2, #24
	orr	r4, r4, r2, lsl #12
	ror	r4, r4, #8
	b .Lpush_front
.Ltake_14:
	and	r6, r5, r2, lsr #24
	lsr	r4, r2, #28
	orr	r4, r4, r2, lsl #8
	ror	r4, r4, #4
	b .Lpush_front
.Lcode_15_end:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .Ldone
	and	r6, r5, r2, lsr #28
	lsl	r4, r2, #4
	b .Lpush_front
.Lcode_4_5:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcc .Ltake_4
	lsr	r4, r3, #24
	orr	r4, r4, r3, lsl #12
	and	r6, r5, r3, lsr #20
	orr	r3, r6, r4, ror #8
	b .Lput
.Ltake_4:
	lsr	r4, r3, #20
	orr	r4, r4, r3, lsl #16
	and	r6, r5, r3, lsr #16
	orr	r3, r6, r4, ror #12
	b .Lput
.Lcode_1_3:
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcc .Ltake_1
	lsrs	ip, ip, #1
	ldreq	ip, [r0], #4
	rrxseq	ip, ip
	bcs .Ltake_3
	lsr	r4, r3, #12
	orr	r4, r4, r3, lsl #24
	and	r6, r5, r3, lsr #8
	orr	r3, r6, r4, ror #20
	b .Lput
.Ltake_3:
	lsr	r4, r3, #16
	orr	r4, r4, r3, lsl #20
	and	r6, r5, r3, lsr #12
	orr	r3, r6, r4, ror #16
	b .Lput
.Ltake_1:
	lsr	r4, r3, #8
	orr	r4, r4, r3, lsl #28
	and	r6, r5, r3, lsr #4
	orr	r3, r6, r4, ror #24
	b .Lput
.Ldone:
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
.Lline:
	add	sl, r0, r2, lsl #3
.Ltile:
	add	r9, r1, #32
.Lrow:
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
	bne .Lrow
	sub	r0, r0, r2, lsl #6
	add	r0, r0, #8
	cmp	r0, sl
	bne .Ltile
	add	r0, r0, r2, lsl #6
	sub	r0, r0, r2, lsl #3
	subs	r3, r3, #1
	bne .Lline
	pop	{r5, r6, r7, r8, r9, sl}
	bx	lr
	.4byte 0x00ff00ff
	.4byte 0x0000ffff
	.4byte 0x11111111
