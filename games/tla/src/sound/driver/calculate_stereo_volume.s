@ Why assembly: caller supplies the channel in r4 and track in r5; the routine
@ reads them (ldrb r1, [r4, #18]; ldrb r0, [r5, #16]) without an argument move
@ or save, a linkage no procedure-call standard produces.
.syntax unified
	.thumb
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
