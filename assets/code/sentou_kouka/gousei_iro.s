.syntax unified
.text
	.arm
	.global SentouKouka_IroJun
	.global SentouKouka_IroGyaku
SentouKouka_IroJun:
	ldr	r8, [r9]
	ldrb	r5, [r1], #1
	add	r8, r8, r5
	ldrb	r5, [r1], #1
	add	r8, r8, r5, lsl #8
	ldrb	r5, [r1], #1
	add	r8, r8, r5, lsl #16
	ldrb	r5, [r1], #1
	add	r8, r8, r5, lsl #24
	and	r5, r8, sl
	bic	r8, r8, sl
	sub	r5, r5, r5, lsr #6
	orr	r8, r8, r5
	str	r8, [r9], #4
SentouKouka_IroGyaku:
	ldr	r8, [r9]
	ldrb	r5, [r1], #-1
	add	r8, r8, r5
	ldrb	r5, [r1], #-1
	add	r8, r8, r5, lsl #8
	ldrb	r5, [r1], #-1
	add	r8, r8, r5, lsl #16
	ldrb	r5, [r1], #-1
	add	r8, r8, r5, lsl #24
	and	r5, r8, sl
	bic	r8, r8, sl
	sub	r5, r5, r5, lsr #6
	orr	r8, r8, r5
	str	r8, [r9], #4
