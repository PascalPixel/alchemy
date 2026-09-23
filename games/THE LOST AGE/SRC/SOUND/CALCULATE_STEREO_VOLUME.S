@ Stereo-volume helper that reads its inputs through r4 and r5, which its
@ callers set, outside the registers a C call passes arguments in, and clamps
@ each side's volume to 255. UPDATE.S (0x081c1b0c) and
@ HANDLE_NOTE_UNREFERENCED.S call it through that private contract. These 48
@ bytes equal TBS SOUND/DRIVER/CALCULATE_STEREO_VOLUME.S (0x080f9f3c).
@ credit: library — TBS/TLA common sound driver stereo-volume helper
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c1bd8
	.global SoundDriver_CalculateStereoVolumeTwin
	.thumb_func
SoundDriver_CalculateStereoVolumeTwin:
	ldrb	r1, [r4, #18]
	movs	r0, #20
	ldrsb	r2, [r4, r0]
	movs	r3, #128
	adds	r3, r3, r2
	muls	r3, r1
	ldrb	r0, [r5, #16]
	muls	r0, r3
	asrs	r0, r0, #14
	cmp	r0, #255
	bls.n	.L_081c1bf0
	movs	r0, #255
.L_081c1bf0:
	strb	r0, [r4, #2]
	movs	r3, #127
	subs	r3, r3, r2
	muls	r3, r1
	ldrb	r0, [r5, #17]
	muls	r0, r3
	asrs	r0, r0, #14
	cmp	r0, #255
	bls.n	.L_081c1c04
	movs	r0, #255
.L_081c1c04:
	strb	r0, [r4, #3]
	bx	lr
