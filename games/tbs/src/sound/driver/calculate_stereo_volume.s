@ C2: MusicPlayer2000 library (pret's m4a_1.s: ChnVolSetAsm), with live channel/track in r4/r5.
.syntax unified
	.thumb
	.global SoundDriver_CalculateStereoVolume
	.thumb_func
SoundDriver_CalculateStereoVolume:
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
	bls.n	.L0
	movs	r0, #255
.L0:
	strb	r0, [r4, #2]
	movs	r3, #127
	subs	r3, r3, r2
	muls	r3, r1
	ldrb	r0, [r5, #17]
	muls	r0, r3
	asrs	r0, r0, #14
	cmp	r0, #255
	bls.n	.L1
	movs	r0, #255
.L1:
	strb	r0, [r4, #3]
	bx	lr
