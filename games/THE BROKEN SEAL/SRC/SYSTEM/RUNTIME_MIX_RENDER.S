@ Output stage of the resident mixer and its synthesised-channel loops.
@ IwramSoundRenderFrame reads the mixed words of RUNTIME_MIX_WORKSPACE.S
@ (add r0, pc, #656), branches into IwramSoundNormalizeChannels
@ (RUNTIME_MIX_CHANNEL.S) when a mixed halfword overflows, which rejoins its
@ loop with b, and ends with pop {r8, r9, sl, fp}; pop {r5, r6, r7, lr};
@ bx lr, releasing a frame it did not build. Its loop body at
@ 0x03000bd8..0x03000c70 holds the third record of
@ SOUND/DRIVER/RESAMPLER_KERNELS.S (in RESAMPLER_KERNELS_2.S), which the
@ selector at 0x080037d4 overwrites with the record it is given. The wave,
@ noise and square loops after it are entered from RUNTIME_MIX.S and return
@ into RUNTIME_MIX_INTERPOLATE.S with b. Branching between routine bodies and
@ unwinding another routine's frame are beyond both approved compilers.
@ credit: handwritten — IWRAM runtime frame renderer
.syntax unified
	.set	IwramSoundNormalizeChannels, 0x03000b1c
	.set	.L_audio_096c, 0x0300096c
	.arm
	.set AlchemyLoadAddress, 0x080012d0
	.set AlchemyRunAddress, 0x03000b60
IwramSoundRenderFrame:
	add	r0, pc, #656
	mov	r9, #352
	mov	r8, #-16777216
	orr	r8, r8, r8, lsr #16
	mov	sl, #-2147483648
	orr	lr, sl, sl, lsr #16
.L_audio_0b78:
	ldm	r0, {r1, r2, r3, r4}
	eor	r6, r1, r1, lsl #1
	ands	r6, r6, lr
	eoreq	r6, r2, r2, lsl #1
	andseq	r6, r6, lr
	eoreq	r6, r3, r3, lsl #1
	andseq	r6, r6, lr
	eoreq	r6, r4, r4, lsl #1
	andseq	r6, r6, lr
	bne	IwramSoundNormalizeChannels
.L_audio_0ba0:
	and	r6, r8, r1, ror #15
	and	r7, r8, r2, ror #15
	orr	r6, r7, r6, lsr #8
	and	fp, r8, r3, ror #15
	and	r7, r8, r4, ror #15
	orr	r7, r7, fp, lsr #8
	bic	fp, r6, sl, asr #15
	orr	fp, fp, r7, lsl #16
	and	r7, r7, sl, asr #15
	orr	r7, r7, r6, lsr #16
	ldr	ip, [r5, #1584]
	str	fp, [r5, #1584]
	ldr	fp, [r5]
	str	r7, [r5], #4
	asrs	r6, r4, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40, 0
	ror	r4, r4, #16
	asrs	r7, r4, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40, 0
	adds	r4, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r2, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40, 0
	ror	r2, r2, #16
	asrs	r7, r2, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40, 0
	adds	r2, r7, r6, lsl #16
	ror	fp, fp, #8
	ror	ip, ip, #8
	asrs	r6, r3, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40, 0
	ror	r3, r3, #16
	asrs	r7, r3, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40, 0
	adds	r3, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r1, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40, 0
	ror	r1, r1, #16
	asrs	r7, r1, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40, 0
	adds	r1, r7, r6, lsl #16
	stmia	r0!, {r1, r2, r3, r4}
	subs	r9, r9, #4, 0
	bgt	.L_audio_0b78
	add	sp, sp, #28, 0
	pop	{r8, r9, sl, fp}
	pop	{r5, r6, r7, lr}
	bx	lr
.L_audio_wave_mode:
	cmp	ip, #0, 0
	bne	.L_audio_noise_mode
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
.L_audio_0cbc:
	ldm	r5, {r0, r1, sl, lr}
	cmp	r7, r6
	addcc	r0, r0, fp, lsl #6
	subcs	r0, r0, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	r1, r1, fp, lsl #6
	subcs	r1, r1, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	sl, sl, fp, lsl #6
	subcs	sl, sl, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	lr, lr, fp, lsl #6
	subcs	lr, lr, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	stmia	r5!, {r0, r1, sl, lr}
	subs	r8, r8, #4, 0
	bgt	.L_audio_0cbc
	b	.L_audio_096c
.L_audio_noise_mode:
	subs	ip, ip, #1, 0
	bne	.L_audio_square_mode
	mov	r6, #768
	lsr	fp, fp, #1
	bic	fp, fp, #65280
	mov	ip, #112, 0
.L_audio_0d28:
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
	subs	r8, r8, #4, 0
	bgt	.L_audio_0d28
	b	.L_audio_096c
.L_audio_square_mode:
	mov	r6, #128, 0
	mov	ip, #384
.L_audio_0da4:
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
	subs	r8, r8, #4, 0
	bgt	.L_audio_0da4
	b	.L_audio_096c
