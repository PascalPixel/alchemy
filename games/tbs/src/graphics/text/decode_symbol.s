@ C2: Text_DecodeSymbol:
@ rrx and conditional rrxs consume the bit reader's shift carry.
@ C2: Message_FindArm:
@ rrx r3, r3 rotates through carry before the final bit shift.
.syntax unified
.text
	.arm
	.global Text_DecodeSymbol
Text_DecodeSymbol:
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
	beq .Lnext_node
	rsbs	r6, ip, r6, lsl #3
	bic	r4, r4, #3
	ldr	ip, [r4], #4
	rrx	ip, ip
	lsr	ip, ip, r6
	mov	r6, #0
.Lnext_node:
	lsrs	ip, ip, #1
	ldreq	ip, [r4], #4
	rrxseq	ip, ip
	bcs .Lleaf
	lsrs	r3, r3, #1
	bcc .Lnext_node
	ldreq	r3, [r2], #4
	rrxseq	r3, r3
	bcc .Lnext_node
	mov	r1, #0
.Lskip:
	lsrs	ip, ip, #1
	bcs .Lskip_leaf_1
	lsrs	ip, ip, #1
	bcs .Lskip_leaf_2
	lsrs	ip, ip, #1
	bcs .Lskip_leaf_3
	lsrs	ip, ip, #1
	bcs .Lskip_leaf_4
	add	r1, r1, #4
	b .Lskip
.Lskip_leaf_3:
	add	r1, r1, #1
.Lskip_leaf_2:
	addne	r6, r6, #1
	bne .Lskip
	ldr	ip, [r4], #4
	rrxs	ip, ip
	addcc	r1, r1, #2
	addcs	r6, r6, #1
	b .Lskip
.Lskip_leaf_4:
	add	r1, r1, #2
	addne	r6, r6, #1
	bne .Lskip
	ldr	ip, [r4], #4
	rrxs	ip, ip
	addcc	r1, r1, #2
	addcs	r6, r6, #1
	b .Lskip
.Lskip_leaf_1:
	beq .Lskip_refill
.Lskip_leaf:
	add	r6, r6, #1
	subs	r1, r1, #1
	bge .Lskip
	b .Lnext_node
.Lskip_refill:
	ldr	ip, [r4], #4
	rrxs	ip, ip
	bcs .Lskip_leaf
	add	r1, r1, #1
	b .Lskip
.Lleaf:
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
	.4byte Text_MessageContexts
	.global Text_DecodeSymbolCodeSize
	.set Text_DecodeSymbolCodeSize, . - Text_DecodeSymbol
.syntax unified
.text
	.arm
	.global Message_FindArm
Message_FindArm:
	lsr	r3, r1, #8
	ldr	ip, [pc, #80]
	add	ip, ip, r3, lsl #3
	ldm	ip, {r2, r4}
	ands	r1, r1, #255
	beq .Lalign
.Lsum_lengths:
	ldrb	r3, [r4], #1
	add	r2, r2, r3
	cmp	r3, #255
	beq .Lsum_lengths
	subs	r1, r1, #1
	bne .Lsum_lengths
.Lalign:
	mov	r3, #1
	ands	r4, r2, #3
	beq .Lstore
	rsbs	r4, r3, r4, lsl #3
	bic	r2, r2, #3
	ldr	r3, [r2], #4
	rrx	r3, r3
	lsr	r3, r3, r4
.Lstore:
	mov	r1, #0
	stm	r0, {r1, r2, r3}
	bx	lr
	.4byte Text_MessageBanks
	.global Message_FindCodeSize
	.set Message_FindCodeSize, . - Message_FindArm
