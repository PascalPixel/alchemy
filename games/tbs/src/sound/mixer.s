@ C2: Thumb entry switches to ARM within its body; mixed-mode blocks also use
@ fp as a volume accumulator.
.syntax unified
	.thumb
	.global SoundDriver_Mixer
	.thumb_func
SoundDriver_Mixer:
	ldrb	r3, [r0, #5]
	cmp	r3, #0
	beq .Lclear
	add	r1, pc, #4
	bx	r1
AlchemyUncredited_mixer_1:
	.arm
AlchemyUncreditedEnd_mixer_1:
	cmp	r4, #2
	addeq	r7, r0, #848
	addne	r7, r5, r8
	mov	r4, r8
.Lreverb_sample:
	ldrsb	r0, [r5, r6]
	ldrsb	r1, [r5]
	add	r0, r0, r1
	ldrsb	r1, [r7, r6]
	add	r0, r0, r1
	ldrsb	r1, [r7], #1
	add	r0, r0, r1
	mul	r1, r0, r3
	asr	r0, r1, #9
	tst	r0, #128
	addne	r0, r0, #1
	strb	r0, [r5, r6]
	strb	r0, [r5], #1
	subs	r4, r4, #1
	bgt .Lreverb_sample
	add	r0, pc, #47
	bx	r0
	.thumb
.Lclear:
	movs	r0, #0
	mov	r1, r8
	adds	r6, r6, r5
	lsrs	r1, r1, #3
	bcc .Lclear_two
	stmia	r5!, {r0}
	stmia	r6!, {r0}
.Lclear_two:
	lsrs	r1, r1, #1
	bcc .Lclear_four
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	stmia	r5!, {r0}
	stmia	r6!, {r0}
.Lclear_four:
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	subs	r1, #1
	bgt .Lclear_four
	ldr	r4, [sp, #24]
	ldr	r0, [r4, #24]
	mov	ip, r0
	ldrb	r0, [r4, #6]
	adds	r4, #80
.Lchannel:
	str	r0, [sp, #4]
	ldr	r3, [r4, #36]
	ldr	r0, [sp, #20]
	cmp	r0, #0
	beq .Lchannel_status
	ldr	r1, [pc, #16]
	ldrb	r1, [r1, #0]
	cmp	r1, #160
	bcs .Lline_known
	adds	r1, #228
.Lline_known:
	cmp	r1, r0
	bcc .Lchannel_status
	b .Lexit
AlchemyUncredited_mixer_2:
	.2byte	0x0000
AlchemyUncreditedEnd_mixer_2:
	.4byte	0x04000006
.Lchannel_status:
	ldrb	r6, [r4, #0]
	movs	r0, #199
	tst	r0, r6
	bne .Lchannel_on
	b .Lnext_channel
.Lchannel_on:
	movs	r0, #128
	tst	r0, r6
	beq .Lenvelope
	movs	r0, #64
	tst	r0, r6
	bne .Lchannel_stop
	movs	r6, #3
	strb	r6, [r4, #0]
	adds	r0, r3, #0
	adds	r0, #16
	str	r0, [r4, #40]
	ldr	r0, [r3, #12]
	str	r0, [r4, #24]
	movs	r5, #0
	strb	r5, [r4, #9]
	str	r5, [r4, #28]
	ldrb	r2, [r3, #3]
	movs	r0, #192
	tst	r0, r2
	beq .Lattack
	movs	r0, #16
	orrs	r6, r0
	strb	r6, [r4, #0]
	b .Lattack
.Lenvelope:
	ldrb	r5, [r4, #9]
	movs	r0, #4
	tst	r0, r6
	beq .Lrelease
	ldrb	r0, [r4, #13]
	subs	r0, #1
	strb	r0, [r4, #13]
	bhi .Lvolume
.Lchannel_stop:
	movs	r0, #0
	strb	r0, [r4, #0]
	b .Lnext_channel
.Lrelease:
	movs	r0, #64
	tst	r0, r6
	beq .Ldecay
	ldrb	r0, [r4, #7]
	muls	r5, r0
	lsrs	r5, r5, #8
	ldrb	r0, [r4, #12]
	cmp	r5, r0
	bhi .Lvolume
.Lecho:
	ldrb	r5, [r4, #12]
	cmp	r5, #0
	beq .Lchannel_stop
	movs	r0, #4
	orrs	r6, r0
	strb	r6, [r4, #0]
	b .Lvolume
.Ldecay:
	movs	r2, #3
	ands	r2, r6
	cmp	r2, #2
	bne .Lattack_check
	ldrb	r0, [r4, #5]
	muls	r5, r0
	lsrs	r5, r5, #8
	ldrb	r0, [r4, #6]
	cmp	r5, r0
	bhi .Lvolume
	adds	r5, r0, #0
	beq .Lecho
	subs	r6, #1
	strb	r6, [r4, #0]
	b .Lvolume
.Lattack_check:
	cmp	r2, #3
	bne .Lvolume
.Lattack:
	ldrb	r0, [r4, #4]
	adds	r5, r5, r0
	cmp	r5, #255
	bcc .Lvolume
	movs	r5, #255
	subs	r6, #1
	strb	r6, [r4, #0]
.Lvolume:
	strb	r5, [r4, #9]
	ldr	r0, [sp, #24]
	ldrb	r0, [r0, #7]
	adds	r0, #1
	muls	r0, r5
	lsrs	r5, r0, #4
	ldrb	r0, [r4, #2]
	muls	r0, r5
	lsrs	r0, r0, #8
	strb	r0, [r4, #10]
	ldrb	r0, [r4, #3]
	muls	r0, r5
	lsrs	r0, r0, #8
	strb	r0, [r4, #11]
	movs	r0, #16
	ands	r0, r6
	str	r0, [sp, #16]
	beq .Lmix
	adds	r0, r3, #0
	adds	r0, #16
	ldr	r1, [r3, #8]
	adds	r0, r0, r1
	str	r0, [sp, #12]
	ldr	r0, [r3, #12]
	subs	r0, r0, r1
	str	r0, [sp, #16]
.Lmix:
	ldr	r5, [sp, #8]
	ldr	r2, [r4, #24]
	ldr	r3, [r4, #40]
	add	r0, pc, #4
	bx	r0
AlchemyUncredited_mixer_3:
	.arm
AlchemyUncreditedEnd_mixer_3:
	str	r8, [sp]
	ldrb	sl, [r4, #10]
	ldrb	fp, [r4, #11]
	lsl	sl, sl, #16
	lsl	fp, fp, #16
	ldrb	r0, [r4, #1]
	tst	r0, #8
	beq .Lpitch
.Lfixed_block:
	cmp	r2, #4
	ble .Lfixed_tail_word
	subs	r2, r2, r8
	movgt	lr, #0
	bgt .Lfixed_word
	mov	lr, r8
	add	r2, r2, r8
	sub	r8, r2, #4
	sub	lr, lr, r8
	ands	r2, r2, #3
	moveq	r2, #4
.Lfixed_word:
	ldr	r6, [r5]
	ldr	r7, [r5, #1584]
.Lfixed_sample:
	ldrsb	r0, [r3], #1
	mul	r1, sl, r0
	bic	r1, r1, #16711680
	add	r6, r1, r6, ror #8
	mul	r1, fp, r0
	bic	r1, r1, #16711680
	add	r7, r1, r7, ror #8
	adds	r5, r5, #1073741824
	bcc .Lfixed_sample
	str	r7, [r5, #1584]
	str	r6, [r5], #4
	subs	r8, r8, #4
	bgt .Lfixed_word
	adds	r8, r8, lr
	beq .Lsave_position
.Lfixed_tail_word:
	ldr	r6, [r5]
	ldr	r7, [r5, #1584]
.Lfixed_tail_sample:
	ldrsb	r0, [r3], #1
	mul	r1, sl, r0
	bic	r1, r1, #16711680
	add	r6, r1, r6, ror #8
	mul	r1, fp, r0
	bic	r1, r1, #16711680
	add	r7, r1, r7, ror #8
	subs	r2, r2, #1
	beq .Lfixed_end
.Lfixed_tail_next:
	adds	r5, r5, #1073741824
	bcc .Lfixed_tail_sample
	str	r7, [r5, #1584]
	str	r6, [r5], #4
	subs	r8, r8, #4
	bgt .Lfixed_block
	b .Lsave_position
.Lpitch_end:
	ldr	r0, [sp, #24]
	cmp	r0, #0
	beq .Lpitch_stop
	ldr	r3, [sp, #20]
	rsb	r9, r2, #0
.Lpitch_wrap:
	adds	r2, r0, r2
	bgt .Lpitch_advance
	sub	r9, r9, r0
	b .Lpitch_wrap
.Lpitch_stop:
	pop	{r4, ip}
	mov	r2, #0
	b .Lflush
.Lfixed_end:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	ldrne	r3, [sp, #12]
	bne .Lfixed_tail_next
.Lflush:
	strb	r2, [r4]
	lsr	r0, r5, #30
	bic	r5, r5, #-1073741824
	rsb	r0, r0, #3
	lsl	r0, r0, #3
	ror	r6, r6, r0
	ror	r7, r7, r0
	str	r7, [r5, #1584]
	str	r6, [r5], #4
	b .Lmix_done
.Lpitch:
	push	{r4, ip}
	ldr	lr, [r4, #28]
	ldr	r1, [r4, #32]
	mul	r4, ip, r1
	ldrsb	r0, [r3]
	ldrsb	r1, [r3, #1]!
	sub	r1, r1, r0
.Lpitch_word:
	ldr	r6, [r5]
	ldr	r7, [r5, #1584]
.Lpitch_sample:
	mul	r9, lr, r1
	add	r9, r0, r9, asr #23
	mul	ip, sl, r9
	bic	ip, ip, #16711680
	add	r6, ip, r6, ror #8
	mul	ip, fp, r9
	bic	ip, ip, #16711680
	add	r7, ip, r7, ror #8
	add	lr, lr, r4
	lsrs	r9, lr, #23
	beq .Lpitch_next
	bic	lr, lr, #1065353216
	subs	r2, r2, r9
	ble .Lpitch_end
	subs	r9, r9, #1
	addeq	r0, r0, r1
.Lpitch_advance:
	ldrsbne	r0, [r3, r9]!
	ldrsb	r1, [r3, #1]!
	sub	r1, r1, r0
.Lpitch_next:
	adds	r5, r5, #1073741824
	bcc .Lpitch_sample
	str	r7, [r5, #1584]
	str	r6, [r5], #4
	subs	r8, r8, #4
	bgt .Lpitch_word
	sub	r3, r3, #1
	pop	{r4, ip}
	str	lr, [r4, #28]
.Lsave_position:
	str	r2, [r4, #24]
	str	r3, [r4, #40]
.Lmix_done:
	ldr	r8, [sp]
	add	r0, pc, #1
	bx	r0
	.thumb
.Lnext_channel:
	ldr	r0, [sp, #4]
	subs	r0, #1
	ble .Lexit
	adds	r4, #64
	b .Lchannel
.Lexit:
	ldr	r0, [sp, #24]
	ldr	r3, [pc, #16]
	str	r3, [r0, #0]
	add	sp, #28
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r3}
	.global Mixer_CallViaR3
	.thumb_func
Mixer_CallViaR3:
	bx	r3
	.align 2
.Lsignature:
	.4byte 0x68736d53
