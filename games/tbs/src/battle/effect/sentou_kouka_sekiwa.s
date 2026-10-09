@ C2: Both multiply halves remain live in nonconsecutive ip/r0 through smull
@ and smlal; compiler DI multiply patterns require consecutive halves, and
@ highpart patterns discard the low half.
.syntax unified
.text
	.arm
	.global SentouKouka_SekiWa
SentouKouka_SekiWa:
	smull	ip, r0, r1, r0
	smlal	ip, r0, r2, r3
	ldm	sp, {r2, r3}
	smlal	ip, r0, r2, r3
	lsl	r0, r0, #16
	orr	r0, r0, ip, lsr #16
	bx	lr
