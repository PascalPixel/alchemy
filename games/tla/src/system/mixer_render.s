@ Why assembly: its Thumb entry's bx pc switches to the ARM code that follows,
@ a mode switch inside a function; the halfword after bx pc is never run.
@ ldrne and ldreq pick one of the two bgt words kept after its return, and
@ str r1, [pc, #252] writes it over the ldmgt at .L_03000e60, a store into
@ code; movs r9, r9 copies and tests without a shift; fp (r11) holds packed
@ samples, not a frame pointer.
@ Why assembly: push {r2, r3, r9, ip} saves without lr; fp (r11) holds the
@ packed volumes, an operand of add, sub and mla, not a frame pointer.
	.section .data,"aw",%progbits
	.section .text.resident_mixer_render,"ax",%progbits
.syntax unified
	.section .data,"aw",%progbits
	.arm
	.4byte	0x00000000, 0x00000000, 0x00000000, 0x00000000, MixerPcmBuffer, 0x000000b0, 0x00000000, 0xfff80035
	.4byte	0x0034fff8

	.section .text.resident_mixer_render,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global IwramSoundRenderVBlank
IwramSoundRenderVBlank:
	bx	pc
AlchemyUncredited080012fa:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd080012fa:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	r1, [pc, #-52]
	tst	r1, #12
	lsl	r1, r1, #4
	str	r1, [pc, #-64]
	ldrne	r1, [pc, #296]
	ldreq	r1, [pc, #296]
	str	r1, [pc, #252]
	ldr	r0, [pc, #380]
	ldr	r1, [pc, #-80]
	ldr	r9, [pc, #-68]
	ldr	r5, [pc, #-84]
	mov	r8, #0xff000000
	orr	r8, r8, r8, lsr #16
	and	lr, r8, r8, lsl #7
	mov	r3, #0xc60
	ldr	sl, [pc, #-96]
	str	r5, [pc, #-100]
	rsb	r2, r9, r1
	movs	r9, r9
	moveq	r2, #0
	moveq	r9, r1
	add	sl, sl, r2, lsl #1
	str	r2, [pc, #-116]
	ldr	r1, [pc, #-116]
	ldr	r6, [pc, #-116]
	ldm	r0, {r2, r4}
.L_03000db0:
	add	r2, r2, lr, lsr #9
	add	r4, r4, lr, lsr #9
.L_03000db8:
	eor	ip, r2, r2, lsl #1
	ands	ip, ip, lr
	eoreq	ip, r4, r4, lsl #1
	andseq	ip, ip, lr
	bne	.L_03000ec0
.L_03000dcc:
	and	ip, r2, lr
	add	ip, r2, ip, lsr #9
	sub	r2, ip, lr, lsr #9
	and	ip, r8, ip, ror #15
	and	r7, r8, r2, ror #15
	orr	ip, r7, ip, lsr #8
	and	fp, r4, lr
	add	fp, r4, fp, lsr #9
	sub	r4, fp, lr, lsr #9
	and	fp, r8, fp, ror #15
	and	r7, r8, r4, ror #15
	orr	r7, r7, fp, lsr #8
	ldrsb	r2, [r5]
	mul	r2, r1, r2
	ldrsb	r4, [r5, #2]
	mul	r4, r1, r4
	ldrsb	fp, [r5, r3]!
	mla	r2, r6, fp, r2
	ldrsb	fp, [r5, #2]!
	mla	r4, r6, fp, r4
	strh	r7, [r5], #-2
	strh	ip, [r5], -r3
	bic	r7, r7, #255
	bic	r7, r7, #0xff00
	orr	r7, r7, ip, lsr #16
	str	r7, [r5], #4
	and	r7, r7, r8
	add	r7, r7, r7, lsr #7
	add	r4, r4, r7, asr #19
	rors	r7, r7, #16
	add	r2, r2, r7, asr #19
	ldrsb	fp, [sl], #2
	add	r2, r2, fp, lsl #21
	ldrsb	fp, [sl], #2
	add	r4, r4, fp, lsl #21
	stmia	r0!, {r2, r4}
	subs	r9, r9, #2
.L_03000e60:
	ldmgt	r0, {r2, r4}
.L_03000e64:
	bgt	.L_03000db0
	ldr	r2, [pc, #-316]
	cmp	r2, #0
	strgt	r9, [pc, #-324]
	ldrgt	sl, [pc, #-336]
	movgt	r9, r2
	bgt	.L_03000e60
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
	.4byte	0xca000000 | (((.L_03000db8 - (.L_03000e64 + 8)) >> 2) & 0x00ffffff)
	.4byte	0xca000000 | (((.L_03000db0 - (.L_03000e64 + 8)) >> 2) & 0x00ffffff)
.L_03000e90:
	ror	r7, ip, #16
	adds	r7, r7, r7
	bvc	.L_03000ea8
	orr	ip, ip, #0x4000
	bic	ip, ip, #0x3fc0
	submi	ip, ip, #64
.L_03000ea8:
	adds	r7, ip, ip
	bxvc	lr
	orr	ip, ip, #0x40000000
	bic	ip, ip, #0x3fc00000
	submi	ip, ip, #0x400000
	bx	lr
.L_03000ec0:
	mov	ip, r2
	bl	.L_03000e90
	mov	r2, ip
	mov	ip, r4
	bl	.L_03000e90
	mov	r4, ip
	and	lr, r8, r8, lsl #7
	str	lr, [pc]
	b	.L_03000dcc
	.global SoundMixerClipped
SoundMixerClipped:
	.4byte	0
	.4byte	IwramSoundMixWorkspace

	.section .text.resident_mixer_render,"ax",%progbits
	.balign 4
.syntax unified
	.arm
	.global IwramSoundSynthWave
IwramSoundSynthWave:
	cmp	ip, #0
	bne	.L_03000fb8
	ldrb	r6, [r3, #2]
	add	r2, r2, r6, lsl #24
	ldrb	r6, [r3, #4]
	adds	r6, r2, r6, lsl #24
	mvnmi	r6, r6
	lsr	sl, r6, #8
	ldrb	r1, [r3, #3]
	ldrb	r0, [r3, #1]
	lsl	r0, r0, #24
	mla	r6, sl, r1, r0
	push	{r2, r3, r9, ip}
.L_03000f20:
	ldm	r5, {r0, r1, r2, r3, r9, sl, ip, lr}
	cmp	r7, r6
	addcc	r0, r0, fp, lsl #6
	subcs	r0, r0, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	r1, r1, fp, lsl #6
	subcs	r1, r1, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	r2, r2, fp, lsl #6
	subcs	r2, r2, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	r3, r3, fp, lsl #6
	subcs	r3, r3, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	r9, r9, fp, lsl #6
	subcs	r9, r9, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	sl, sl, fp, lsl #6
	subcs	sl, sl, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	ip, ip, fp, lsl #6
	subcs	ip, ip, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	lr, lr, fp, lsl #6
	subcs	lr, lr, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	stmia	r5!, {r0, r1, r2, r3, r9, sl, ip, lr}
	subs	r8, r8, #8
	bgt	.L_03000f20
	pop	{r2, r3, r9, ip}
	b	IwramSoundMixDone
.L_03000fb8:
	subs	ip, ip, #1
	bne	.L_03001044
	mov	r6, #0x300
	lsr	fp, fp, #1
	bic	fp, fp, #0xff00
	mov	ip, #112
.L_03000fd0:
	ldm	r5, {r0, r1, sl, lr}
	adds	r7, r7, r4, lsl #3
	rsb	r9, ip, r7, lsr #24
	lsl	r6, r7, #1
	sub	r9, r9, r6, lsr #27
	adds	r2, r9, r2, asr #1
	mlane	r0, fp, r2, r0
	adds	r7, r7, r4, lsl #3
	rsb	r9, ip, r7, lsr #24
	lsl	r6, r7, #1
	sub	r9, r9, r6, lsr #27
	adds	r2, r9, r2, asr #1
	mlane	r1, fp, r2, r1
	adds	r7, r7, r4, lsl #3
	rsb	r9, ip, r7, lsr #24
	lsl	r6, r7, #1
	sub	r9, r9, r6, lsr #27
	adds	r2, r9, r2, asr #1
	mlane	sl, fp, r2, sl
	adds	r7, r7, r4, lsl #3
	rsb	r9, ip, r7, lsr #24
	lsl	r6, r7, #1
	sub	r9, r9, r6, lsr #27
	adds	r2, r9, r2, asr #1
	mlane	lr, fp, r2, lr
	stmia	r5!, {r0, r1, sl, lr}
	subs	r8, r8, #4
	bgt	.L_03000fd0
	b	IwramSoundMixDone
.L_03001044:
	mov	r6, #128
	mov	ip, #0x180
.L_0300104c:
	ldm	r5, {r0, r1, sl, lr}
	adds	r7, r7, r4, lsl #3
	rsbpl	r9, r6, r7, asr #23
	submi	r9, ip, r7, lsr #23
	mla	r0, fp, r9, r0
	adds	r7, r7, r4, lsl #3
	rsbpl	r9, r6, r7, asr #23
	submi	r9, ip, r7, lsr #23
	mla	r1, fp, r9, r1
	adds	r7, r7, r4, lsl #3
	rsbpl	r9, r6, r7, asr #23
	submi	r9, ip, r7, lsr #23
	mla	sl, fp, r9, sl
	adds	r7, r7, r4, lsl #3
	rsbpl	r9, r6, r7, asr #23
	submi	r9, ip, r7, lsr #23
	mla	lr, fp, r9, lr
	stmia	r5!, {r0, r1, sl, lr}
	subs	r8, r8, #4
	bgt	.L_0300104c
	b	IwramSoundMixDone
