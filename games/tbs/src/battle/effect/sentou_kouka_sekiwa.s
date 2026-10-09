@ C2: smull and both smlal instructions retain the low/high product halves
@ in ip/r0, a nonconsecutive pair the approved compiler cannot allocate.
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
