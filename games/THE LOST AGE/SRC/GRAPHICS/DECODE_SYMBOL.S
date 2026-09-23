@ Symbol decoder, copied and run as one 324-byte unit (size and source {0x144,
@ 0x08038438} in the literal pools at 0x0803b1e0 and 0x0803cb04). Behind a
@ 4-byte Thumb entry (bx pc) the ARM body reads tree and message bits with
@ lsrs and bcs or bcc, refills each bit word through the carry with ldreq
@ and rrxseq, and saves r5 and r6 without lr. GCC never branches on the carry
@ out of a shift, never emits rrx and saves lr with any register. After that
@ entry its 320-byte body equals TBS GRAPHICS/TEXT/DECODE_SYMBOL.S
@ (0x08015430) apart from one relocated literal word.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: library — TBS/TLA common ARM text and tile kernel bank, symbol decoder
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08038438
	.global Text_DecodeSymbol
	.thumb_func
Text_DecodeSymbol:
AlchemyUncredited_08038438:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08038438:
	push	{r5, r6}
	ldm	r0, {r1, r2, r3}
	ldr	ip, [pc, #300]
	lsr	r4, r1, #8
	add	ip, ip, r4, lsl #3
	ldm	ip, {r4, r5}
	and	ip, r1, #255
	add	ip, ip, ip
	ldrh	r5, [r5, ip]
	add	r4, r4, r5
	mov	r5, r4
	mov	ip, #1
	ands	r6, r4, #3
	beq	.L_0803848c
	rsbs	r6, ip, r6, lsl #3
	bic	r4, r4, #3
	ldr	ip, [r4], #4
	rrx	ip, ip
	lsr	ip, ip, r6
	mov	r6, #0
.L_0803848c:
	lsrs	ip, ip, #1
	ldreq	ip, [r4], #4
	rrxseq	ip, ip
	bcs	.L_08038544
	lsrs	r3, r3, #1
	bcc	.L_0803848c
	ldreq	r3, [r2], #4
	rrxseq	r3, r3
	bcc	.L_0803848c
	mov	r1, #0
.L_080384b4:
	lsrs	ip, ip, #1
	bcs	.L_0803851c
	lsrs	ip, ip, #1
	bcs	.L_080384e0
	lsrs	ip, ip, #1
	bcs	.L_080384dc
	lsrs	ip, ip, #1
	bcs	.L_080384fc
	add	r1, r1, #4
	b	.L_080384b4
.L_080384dc:
	add	r1, r1, #1
.L_080384e0:
	addne	r6, r6, #1
	bne	.L_080384b4
	ldr	ip, [r4], #4
	rrxs	ip, ip
	addcc	r1, r1, #2
	addcs	r6, r6, #1
	b	.L_080384b4
.L_080384fc:
	add	r1, r1, #2
	addne	r6, r6, #1
	bne	.L_080384b4
	ldr	ip, [r4], #4
	rrxs	ip, ip
	addcc	r1, r1, #2
	addcs	r6, r6, #1
	b	.L_080384b4
.L_0803851c:
	beq	.L_08038530
.L_08038520:
	add	r6, r6, #1
	subs	r1, r1, #1
	bge	.L_080384b4
	b	.L_0803848c
.L_08038530:
	ldr	ip, [r4], #4
	rrxs	ip, ip
	bcs	.L_08038520
	add	r1, r1, #1
	b	.L_080384b4
.L_08038544:
	lsrs	r1, r6, #1
	add	r6, r6, r1
	sub	r6, r5, r6
	ldrb	r5, [r6, #-1]
	ldrb	r6, [r6, #-2]
	andcs	r1, r5, #15
	orrcs	r1, r6, r1, lsl #8
	lslcc	r1, r5, #4
	orrcc	r1, r1, r6, lsr #4
	stm	r0, {r1, r2, r3}
	movs	r0, r1
	pop	{r5, r6}
	bx	lr
	.4byte	0x08060c30
