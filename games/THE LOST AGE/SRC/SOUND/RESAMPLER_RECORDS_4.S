@ Fourth and fifth 76-byte records of RESAMPLER_RECORDS.S, which fill
@ their slots: flag-setting asrs and rors feeding conditional mulne and
@ mlane, then ldrsb sample reads with post-increment, with no entry or
@ return. Fixed-size instruction records are not compiled functions.
@ credit: handwritten — TLA fixed-size ARM sound resampler records
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08000550
.L_08000550:
	and	r7, r7, r8
	add	r7, r7, r7, lsr #7
	asrs	r4, ip, #24
	mulne	r4, r6, r4
	add	r4, r4, r7, asr #19
	rors	ip, ip, #16
	asrs	r2, ip, #24
	mulne	r2, r6, r2
	rors	r7, r7, #16
	add	r2, r2, r7, asr #19
	asrs	ip, fp, #24
	mlane	r4, r1, ip, r4
	rors	fp, fp, #16
	asrs	ip, fp, #24
	mlane	r2, r1, ip, r2
	ldrsb	ip, [sl], #2
	add	r2, r2, ip, lsl #21
	ldrsb	ip, [sl], #2
	add	r4, r4, ip, lsl #21
	and	r7, r7, r8
	add	r7, r7, r7, lsr #7
	asrs	r4, ip, #24
	mulne	r4, r6, r4
	add	r4, r4, r7, asr #19
	rors	ip, ip, #16
	asrs	r2, ip, #24
	mulne	r2, r6, r2
	rors	r7, r7, #16
	add	r2, r2, r7, asr #19
	asrs	ip, fp, #24
	mlane	r4, r1, ip, r4
	rors	fp, fp, #16
	asrs	ip, fp, #24
	mlane	r2, r1, ip, r2
	ldrsb	ip, [sl], #2
	add	r2, r2, ip, lsl #21
	ldrsb	ip, [sl], #2
	add	r4, r4, ip, lsl #21
