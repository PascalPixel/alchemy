.syntax unified
.text
	.arm
	.global SentouKouka_YomiGyaku
	.global SentouKouka_YomiJun
	.global SentouKouka_NuriJun
	.global SentouKouka_NuriGyaku
	.global SentouKouka_Nuri8Jun
	.global SentouKouka_Nuri8Gyaku
	.global SentouKouka_HikakuJun
	.global SentouKouka_HikakuGyaku
	.global SentouKouka_Hikaku4Jun
	.global SentouKouka_Hikaku4Gyaku
	.global SentouKouka_KasanJun
	.global SentouKouka_KasanGyaku
	.global SentouKouka_Mask
	.global SentouKouka_Gousei
SentouKouka_YomiGyaku:
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
SentouKouka_YomiJun:
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
SentouKouka_NuriJun:
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9]
	add	r9, r9, #1
SentouKouka_NuriGyaku:
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9]
	add	r9, r9, #1
SentouKouka_Nuri8Jun:
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #1]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #2]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #3]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #4]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #5]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #6]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #7]
	add	r9, r9, #8
SentouKouka_Nuri8Gyaku:
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #1]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #2]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #3]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #4]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #5]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #6]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #7]
	add	r9, r9, #8
SentouKouka_HikakuJun:
	ldrb	r5, [r1], #1
	ldrb	r8, [r9], #1
	cmp	r5, r8
	strbgt	r5, [r9, #-1]
SentouKouka_HikakuGyaku:
	ldrb	r5, [r1], #-1
	ldrb	r8, [r9], #1
	cmp	r5, r8
	strbgt	r5, [r9, #-1]
SentouKouka_Hikaku4Jun:
	ldrb	r5, [r1], #1
	ldr	r8, [r9], #4
	and	sl, r8, #255
	cmp	r5, sl
	strbgt	r5, [r9, #-4]
	ldrb	r5, [r1], #1
	and	sl, r8, #65280
	cmp	r5, sl, lsr #8
	strbgt	r5, [r9, #-3]
	ldrb	r5, [r1], #1
	and	sl, r8, #16711680
	cmp	r5, sl, lsr #16
	strbgt	r5, [r9, #-2]
	ldrb	r5, [r1], #1
	cmp	r5, r8, lsr #24
	strbgt	r5, [r9, #-1]
SentouKouka_Hikaku4Gyaku:
	ldrb	r5, [r1], #-1
	ldr	r8, [r9], #4
	and	sl, r8, #255
	cmp	r5, sl
	strbgt	r5, [r9, #-4]
	ldrb	r5, [r1], #-1
	and	sl, r8, #65280
	cmp	r5, sl, lsr #8
	strbgt	r5, [r9, #-3]
	ldrb	r5, [r1], #-1
	and	sl, r8, #16711680
	cmp	r5, sl, lsr #16
	strbgt	r5, [r9, #-2]
	ldrb	r5, [r1], #-1
	cmp	r5, r8, lsr #24
	strbgt	r5, [r9, #-1]
SentouKouka_KasanJun:
	ldrb	r5, [r1], #1
	ldrb	r8, [r9], #1
	add	r5, r5, r8
	cmp	r5, #63
	movcs	r5, #63
	strb	r5, [r9, #-1]
SentouKouka_KasanGyaku:
	ldrb	r5, [r1], #-1
	ldrb	r8, [r9], #1
	add	r5, r5, r8
	cmp	r5, #63
	movcs	r5, #63
	strb	r5, [r9, #-1]
SentouKouka_Mask:
	mov	sl, #64
	orr	sl, sl, sl, lsl #8
	orr	sl, sl, sl, lsl #16
SentouKouka_Gousei:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	add	r4, sp, #32
	ldm	r4, {r4, r5}
	add	r6, r1, r4
	sub	r1, r6, #1
	lsl	r6, r4, #1
	sub	r6, r5, #1
	mla	r1, r6, r4, r1
	rsb	r6, r4, #0
	lsl	r6, r6, #1
	mla	r1, r4, r5, r1
	sub	r1, r1, #1
	mov	r6, #0
	mov	r6, #0
	rsbs	ip, r3, #0
	addpl	r5, r5, r3
	mlapl	r1, r3, r4, r1
	mlapl	r1, ip, r4, r1
	movpl	r3, #0
	add	ip, r3, r5
	cmp	ip, #0
	rsbgt	r5, r3, #0
	cmp	r5, #0
	ble .L_080edd28
	and	r2, r2, #0
.L_080edd28:
	rsbs	ip, r2, #0
	addpl	r4, r4, r2
	addpl	r6, r6, r2
	addpl	r1, r1, r2
	subpl	r6, r6, r2
	subpl	r1, r1, r2
	movpl	r2, #0
	add	ip, r2, r4
	subs	ip, ip, #0
	subgt	r6, r6, ip
	addgt	r6, r6, ip
	rsbgt	r4, r2, #0
	cmp	r4, #0
	ble .L_080edd64
	ands	r7, r2, #7
.L_080edd64:
	beq .L_080edd78
	rsb	r7, r7, #8
	subs	r4, r4, r7
	addlt	r7, r4, r7
	movlt	r4, #0
.L_080edd78:
	lsr	ip, r2, #3
	rsb	fp, ip, #16
	and	r2, r2, #7
	add	r2, r2, ip, lsl #6
	add	r0, r0, r2
	lsl	r3, r3, #3
	add	r2, r3, r5, lsl #3
	and	ip, r3, #56
	and	lr, r3, #0, 26
	orr	ip, ip, lr
	add	r9, r0, ip
	mov	lr, fp
	cmp	r7, #0
	beq .L_080eddb4
	sub	ip, r1, r7
.L_080eddb4:
	add	ip, r1, r7
	ldrb	r5, [r1], #-1
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strb	r5, [r9], #1
	cmp	r1, ip
	bne .L_080eddd4
	add	r9, r9, #56
.L_080eddd4:
	subs	lr, lr, #1
	subeq	r9, r9, #1024
	lsrs	ip, r4, #3
	beq .L_080edde8
	ldrb	r5, [r1], #-1
.L_080edde8:
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strb	r5, [r9], #1
	add	r9, r9, #56
	subs	lr, lr, #1
	subeq	r9, r9, #1024
	subs	ip, ip, #1
	bne .L_080ede0c
	ands	ip, r4, #7
.L_080ede0c:
	beq .L_080ede14
	sub	ip, r1, ip
.L_080ede14:
	add	ip, r1, ip
	ldrb	r5, [r1], #-1
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strb	r5, [r9], #1
	cmp	r1, ip
	bne .L_080ede34
	add	r1, r1, r6
.L_080ede34:
	add	r3, r3, #8
	cmp	r3, r2
	bne .L_080ede44
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
.L_080ede44:
	bx	lr
