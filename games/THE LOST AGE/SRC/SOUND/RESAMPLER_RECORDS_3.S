@ Third 76-byte record of RESAMPLER_RECORDS.S up to its branch to the next
@ record's boundary: flag-setting asrs and rors feeding conditional mulne
@ and mlane, with no entry or return of its own. Its zero padding
@ (0x08000544, 12 bytes) is RESAMPLER_RECORDS_PAD_2.S and carries no
@ credit.
@ credit: handwritten — TLA fixed-size ARM sound resampler record
.syntax unified
	.set	.L_08000550, 0x08000550
	.arm
	.set AlchemyLoadAddress, 0x08000504
.L_08000504:
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
	b	.L_08000550
