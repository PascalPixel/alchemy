@ Interpolation templates and sample fetch of the resident mixer. The three
@ 24-byte templates at its start are copied by RUNTIME_MIX.S into its
@ sample loop and are not run here. The rest fetches samples and returns
@ to the mixer two instructions past the call with addne pc, lr, #8, a
@ return that no compiler emits.
@ credit: handwritten — IWRAM runtime interpolation templates and sample fetch
.syntax unified
	.set	.L_audio_0acc, 0x03000acc
	.set	.L_audio_09bc, 0x030009bc
	.arm
	.set AlchemyLoadAddress, 0x08001044
	.set AlchemyRunAddress, 0x030008d4
	asr	r9, r9, #22
	adds	r9, r9, r6, lsl #1
	adds	r7, r7, r4
	addpl	r6, ip, r6
	ldrsbpl	ip, [r3, #1]!
	subspl	ip, ip, r6
	adds	r9, r6, r9, asr #23
	add	r6, ip, r6
	adds	r7, r7, r4
	ldrsbpl	r6, [r3, #1]!
	ldrsb	ip, [r3, #1]!
	subs	ip, ip, r6
	adds	r9, r6, r9, asr #23
	add	r7, r7, r4
	lsr	ip, r7, #23
	ldrsb	r6, [r3, ip]!
	ldrsb	ip, [r3, #1]!
	subs	ip, ip, r6
.L_audio_091c:
	add	r5, r5, r8, lsl #2
.L_audio_0920:
	mul	r9, r7, ip
	asr	r9, r9, #22
	adds	r9, r9, r6, lsl #1
	ldrne	r0, [r5, -r8, lsl #2]
	mlane	r0, fp, r9, r0
	strne	r0, [r5, -r8, lsl #2]
	add	r7, r7, r4
	lsrs	r9, r7, #23
	beq	.L_audio_0964
	subs	r2, r2, r7, lsr #23
	blle	.L_audio_refill_source
	subs	r9, r9, #1, 0
	addeq	r6, ip, r6
	ldrsbne	r6, [r3, r9]!
	ldrsb	ip, [r3, #1]!
	sub	ip, ip, r6
	bic	r7, r7, #1065353216
.L_audio_0964:
	subs	r8, r8, #1, 0
	bgt	.L_audio_0920
.L_audio_096c:
	sub	r3, r3, #1, 0
	pop	{r4, r9, ip}
	str	r7, [r4, #28]
	b	.L_audio_0acc
.L_audio_refill_source:
	add	r3, sp, #24, 0
	ldm	r3, {r3, r6}
	cmp	r6, #0, 0
	rsbne	r9, r2, #0, 0
	addne	r2, r6, r2
	addne	pc, lr, #8, 0
	pop	{r4, r9, ip}
	b	.L_audio_09bc
