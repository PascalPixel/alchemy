.syntax unified
.text
	.arm
	.global Func_08015430
Func_08015430:
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
	beq .L_08015480
	rsbs	r6, ip, r6, lsl #3
	bic	r4, r4, #3
	ldr	ip, [r4], #4
	rrx	ip, ip
	lsr	ip, ip, r6
	mov	r6, #0
.L_08015480:
	lsrs	ip, ip, #1
	ldreq	ip, [r4], #4
	rrxseq	ip, ip
	bcs .L_08015538
	lsrs	r3, r3, #1
	bcc .L_08015480
	ldreq	r3, [r2], #4
	rrxseq	r3, r3
	bcc .L_08015480
	mov	r1, #0
.L_080154a8:
	lsrs	ip, ip, #1
	bcs .L_08015510
	lsrs	ip, ip, #1
	bcs .L_080154d4
	lsrs	ip, ip, #1
	bcs .L_080154d0
	lsrs	ip, ip, #1
	bcs .L_080154f0
	add	r1, r1, #4
	b .L_080154a8
.L_080154d0:
	add	r1, r1, #1
.L_080154d4:
	addne	r6, r6, #1
	bne .L_080154a8
	ldr	ip, [r4], #4
	rrxs	ip, ip
	addcc	r1, r1, #2
	addcs	r6, r6, #1
	b .L_080154a8
.L_080154f0:
	add	r1, r1, #2
	addne	r6, r6, #1
	bne .L_080154a8
	ldr	ip, [r4], #4
	rrxs	ip, ip
	addcc	r1, r1, #2
	addcs	r6, r6, #1
	b .L_080154a8
.L_08015510:
	beq .L_08015524
.L_08015514:
	add	r6, r6, #1
	subs	r1, r1, #1
	bge .L_080154a8
	b .L_08015480
.L_08015524:
	ldr	ip, [r4], #4
	rrxs	ip, ip
	bcs .L_08015514
	add	r1, r1, #1
	b .L_080154a8
.L_08015538:
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
	.4byte 0x0803842c
