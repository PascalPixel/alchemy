@ Second 76-byte record of RESAMPLER_RECORDS.S up to its branch to the next
@ record's boundary: flag-setting asrs and rors feeding conditional mulne
@ and mlane, with no entry or return of its own. Its zero padding
@ (0x080004e4, 32 bytes) is RESAMPLER_RECORDS_PAD_1.S and carries no
@ credit.
@ credit: handwritten — TLA fixed-size ARM sound resampler record
.syntax unified
	.set	.L_08000504, 0x08000504
	.arm
	.set AlchemyLoadAddress, 0x080004b8
.L_080004b8:
	asrs	r4, ip, #24
	mulne	r4, r6, r4
	rors	ip, ip, #16
	asrs	r2, ip, #24
	mulne	r2, r6, r2
	asrs	ip, fp, #24
	mlane	r4, r1, ip, r4
	rors	fp, fp, #16
	asrs	ip, fp, #24
	mlane	r2, r1, ip, r2
	b	.L_08000504
