.syntax unified
.text
	.set Func_080f03f0, 0x080f03f0
	.thumb
	.global SentouKouka_Tobikomi
	.thumb_func
SentouKouka_Tobikomi:
	ldr r4, [pc, #0]
	bx r4
	.4byte Func_080f03f0 + 1
	.arm
	.global SentouKouka_SekiWa
	.global SentouKouka_Tenkai
	.global SentouKouka_TenkaiYomi
SentouKouka_SekiWa:
	smull	ip, r0, r1, r0
	smlal	ip, r0, r2, r3
	ldm	sp, {r2, r3}
	smlal	ip, r0, r2, r3
	lsl	r0, r0, #16
	orr	r0, r0, ip, lsr #16
	bx	lr
SentouKouka_Tenkai:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	mov	sl, r2
	ldr	r2, [pc, #192]
	add	r3, pc, #236
	sub	r2, r3, r2
	add	r4, pc, #196
	mov	r5, #8
.L_080f0040:
	ldr	r6, [r3], #4
	add	r6, r6, r2
	str	r6, [r4], #4
	subs	r5, r5, #1
	bne .L_080f0040
	mov	fp, #7
	ldrh	r3, [r0], #2
	mov	r2, #0
	mov	r5, #0
	mov	r9, #15
.L_080f0068:
	mov	r8, #8
.L_080f006c:
	mov	r7, #32
.L_080f0070:
	bl SentouKouka_TenkaiYomi
	lsr	r6, r5, #25
	bl SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #17
	bl SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #9
	bl SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #4
	bl SentouKouka_TenkaiYomi
	lsr	r6, r5, #25
	bl SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #17
	bl SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #9
	bl SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #60
	subs	r7, r7, #1
	bne .L_080f0070
	sub	r1, r1, #2048
	add	r1, r1, #8
	subs	r8, r8, #1
	bne .L_080f006c
	sub	r1, r1, #64
	add	r1, r1, #2048
	subs	r9, r9, #1
	bne .L_080f0068
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L_bias:
	.4byte 0x80808080
.L_dispatch_base:
	.4byte .L_dispatch_source
SentouKouka_TenkaiYomi:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
	nop
.L_dispatch_runtime:
	.space 32, 0
.L_dispatch_source:
	.4byte .L_case_2
	.4byte .L_case_4_plus
	.4byte .L_case_8
	.4byte .L_case_6_plus
	.4byte .L_case_2
	.4byte .L_case_4_minus
	.4byte .L_case_10
	.4byte .L_case_6_minus
.L_case_2:
	lsr	r3, r3, #2
	subs	r2, r2, #2
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
.L_case_4_plus:
	and	ip, r3, #8
	lsrs	r3, r3, #4
	add	ip, ip, #8
	add	r5, r5, ip, lsl #22
	subs	r2, r2, #4
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
.L_case_4_minus:
	and	ip, r3, #8
	lsrs	r3, r3, #4
	add	ip, ip, #8
	sub	r5, r5, ip, lsl #22
	subs	r2, r2, #4
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
.L_case_6_plus:
	and	ip, fp, r3, lsr #3
	lsr	r3, r3, #6
	add	ip, ip, #3
	add	r5, r5, ip, lsl #25
	subs	r2, r2, #6
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
.L_case_6_minus:
	and	ip, fp, r3, lsr #3
	lsr	r3, r3, #6
	add	ip, ip, #3
	sub	r5, r5, ip, lsl #25
	subs	r2, r2, #6
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
.L_case_8:
	mov	ip, #15
	ands	ip, ip, r3, lsr #4
	add	ip, ip, #11
	rsbcs	ip, ip, #0
	add	r5, r5, ip, lsl #25
	lsr	r3, r3, #8
	subs	r2, r2, #8
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
.L_case_10:
	ror	r5, r3, #10
	and	r5, r5, #-33554432
	lsr	r3, r3, #10
	subs	r2, r2, #10
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
