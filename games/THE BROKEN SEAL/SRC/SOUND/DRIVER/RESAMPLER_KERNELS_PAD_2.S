@ The 132 bytes that fill the fifth record of RESAMPLER_KERNELS.S (in
@ RESAMPLER_KERNELS_2.S) to 152 bytes after its branch to the slot's end:
@ shifted sums with conditional corrections and rotations (asrs; adds;
@ addmi; ror) left from another record. The selector copies them with the
@ record, but that branch always skips them, so they
@ never run. Filler that never runs carries no credit, as
@ RESAMPLER_KERNELS_PAD.S and RESAMPLER_KERNELS_PAD_1.S.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08000678
	adds	r7, r7, fp, asr #18
	addmi	r7, r7, #64
	adds	r4, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r2, #19
	adds	r6, r6, ip, asr #18
	addmi	r6, r6, #64
	ror	r2, r2, #16
	asrs	r7, r2, #19
	adds	r7, r7, fp, asr #18
	addmi	r7, r7, #64
	adds	r2, r7, r6, lsl #16
	ror	fp, fp, #8
	ror	ip, ip, #8
	asrs	r6, r3, #19
	adds	r6, r6, ip, asr #18
	addmi	r6, r6, #64
	ror	r3, r3, #16
	asrs	r7, r3, #19
	adds	r7, r7, fp, asr #18
	addmi	r7, r7, #64
	adds	r3, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r1, #19
	adds	r6, r6, ip, asr #18
	addmi	r6, r6, #64
	ror	r1, r1, #16
	asrs	r7, r1, #19
	adds	r7, r7, fp, asr #18
	addmi	r7, r7, #64
	adds	r1, r7, r6, lsl #16
