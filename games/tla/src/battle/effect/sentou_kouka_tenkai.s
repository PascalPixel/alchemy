.syntax unified
	.thumb
	.global SentouKouka_TenkaiCode
SentouKouka_TenkaiCode:
	.global SentouKouka_Tenkai
	.thumb_func
SentouKouka_Tenkai:
	bx	pc
AlchemyUncredited_081a004e:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_081a004e:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	mov	sl, r2
	ldr	r2, [pc, #192]
	add	r3, pc, #236
	sub	r2, r3, r2
	add	r4, pc, #196
	mov	r5, #8
.L_081a006c:
	ldr	r6, [r3], #4
	add	r6, r6, r2
	str	r6, [r4], #4
	subs	r5, r5, #1
	bne	.L_081a006c
	mov	fp, #7
	ldrh	r3, [r0], #2
	mov	r2, #0
	mov	r5, #0
	mov	r9, #15
.L_081a0094:
	mov	r8, #8
.L_081a0098:
	mov	r7, #32
.L_081a009c:
	bl	SentouKouka_TenkaiYomi
	lsr	r6, r5, #25
	bl	SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #17
	bl	SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #9
	bl	SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #4
	bl	SentouKouka_TenkaiYomi
	lsr	r6, r5, #25
	bl	SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #17
	bl	SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #9
	bl	SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #60
	subs	r7, r7, #1
	bne	.L_081a009c
	sub	r1, r1, #0x800
	add	r1, r1, #8
	subs	r8, r8, #1
	bne	.L_081a0098
	sub	r1, r1, #64
	add	r1, r1, #0x800
	subs	r9, r9, #1
	bne	.L_081a0094
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
