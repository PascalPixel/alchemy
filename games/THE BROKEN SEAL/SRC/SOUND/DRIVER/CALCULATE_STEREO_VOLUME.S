@ Sets a channel's two output volume bytes from its velocity and pan and the
@ track's two volumes, clamped to 255. Its inputs arrive in r4 (the channel)
@ and r5 (the track), not in argument registers, and it uses r5 without saving
@ it; code from either approved compiler takes arguments in r0-r3 and saves
@ any callee-saved register it touches. MusicTrack_HandleNote and the driver
@ update call it. Every TBS and TLA edition has these bytes.
@ credit: library — TBS/TLA common sound driver stereo-volume helper
.syntax unified
	.thumb
	.global Func_080f9f3c
	.thumb_func
Func_080f9f3c:
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
