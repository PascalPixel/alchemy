@ Second 152-byte record of RESAMPLER_KERNELS.S up to its branch to the
@ slot's end: shifted sums with conditional corrections (asrs; adds;
@ addmi) and rotations over the loop's live registers, with no entry or
@ return of its own. Its three nop words of padding are
@ RESAMPLER_KERNELS_PAD_1.S. TBS only.
@ credit: handwritten — ARM sound-buffer record patched into the IWRAM runtime
.syntax unified
	.set	.Lrecord_2, 0x08000534
	.arm
	.set AlchemyLoadAddress, 0x0800049c
.Lrecord_1:
	asrs	r7, fp, #19
	adds	r7, r7, ip, asr #20
	addmi	r7, r7, #48
	asrs	r6, ip, #19
	adds	r6, r6, fp, asr #20
	addmi	r6, r6, #48
	adds	r4, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r7, fp, #19
	adds	r7, r7, ip, asr #20
	addmi	r7, r7, #48
	asrs	r6, ip, #19
	adds	r6, r6, fp, asr #20
	addmi	r6, r6, #48
	adds	r2, r7, r6, lsl #16
	ror	fp, fp, #8
	ror	ip, ip, #8
	asrs	r7, fp, #19
	adds	r7, r7, ip, asr #20
	addmi	r7, r7, #48
	asrs	r6, ip, #19
	adds	r6, r6, fp, asr #20
	addmi	r6, r6, #48
	adds	r3, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r7, fp, #19
	adds	r7, r7, ip, asr #20
	addmi	r7, r7, #48
	asrs	r6, ip, #19
	adds	r6, r6, fp, asr #20
	addmi	r6, r6, #48
	adds	r1, r7, r6, lsl #16
	b	.Lrecord_2
