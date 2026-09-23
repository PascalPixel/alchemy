@ Third to fifth 152-byte records of RESAMPLER_KERNELS.S. The third and
@ fourth fill the whole slot with shifted sums over the loop's live
@ registers; the fifth clears r1-r4 and branches to the slot's end over 132
@ bytes of leftover instructions, which never run and are
@ RESAMPLER_KERNELS_PAD_2.S. None has an entry or return of its own. TBS
@ only.
@ credit: handwritten — ARM sound-buffer records patched into the IWRAM runtime
.syntax unified
	.set	.Lrecords_end, 0x080006fc
	.arm
	.set AlchemyLoadAddress, 0x08000534
.Lrecord_2:
	asrs	r6, r4, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40
	ror	r4, r4, #16
	asrs	r7, r4, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40
	adds	r4, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r2, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40
	ror	r2, r2, #16
	asrs	r7, r2, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40
	adds	r2, r7, r6, lsl #16
	ror	fp, fp, #8
	ror	ip, ip, #8
	asrs	r6, r3, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40
	ror	r3, r3, #16
	asrs	r7, r3, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40
	adds	r3, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r1, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40
	ror	r1, r1, #16
	asrs	r7, r1, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40
	adds	r1, r7, r6, lsl #16
.Lrecord_3:
	asrs	r6, r4, #19
	adds	r6, r6, fp, asr #18
	addmi	r6, r6, #72
	ror	r4, r4, #16
	asrs	r7, r4, #19
	adds	r7, r7, ip, asr #18
	addmi	r7, r7, #72
	adds	r4, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r2, #19
	adds	r6, r6, fp, asr #18
	addmi	r6, r6, #72
	ror	r2, r2, #16
	asrs	r7, r2, #19
	adds	r7, r7, ip, asr #18
	addmi	r7, r7, #72
	adds	r2, r7, r6, lsl #16
	ror	fp, fp, #8
	ror	ip, ip, #8
	asrs	r6, r3, #19
	adds	r6, r6, fp, asr #18
	addmi	r6, r6, #72
	ror	r3, r3, #16
	asrs	r7, r3, #19
	adds	r7, r7, ip, asr #18
	addmi	r7, r7, #72
	adds	r3, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r1, #19
	adds	r6, r6, fp, asr #18
	addmi	r6, r6, #72
	ror	r1, r1, #16
	asrs	r7, r1, #19
	adds	r7, r7, ip, asr #18
	addmi	r7, r7, #72
	adds	r1, r7, r6, lsl #16
.Lrecord_4:
	mov	r1, #0
	mov	r2, #0
	mov	r3, #0
	mov	r4, #0
	b	.Lrecords_end
