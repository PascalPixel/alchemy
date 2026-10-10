@ C2: smull ip, r0, r1, r0 and both smlal ip, r0, r2, r3 keep the 64-bit sum
@ with its low half in ip (r12) and its high half in r0. mulsidi3 and
@ *mulsidi3adddi write a DI register pair, RdHi = RdLo + 1, earlyclobbered
@ and so apart from the inputs; a pair whose low half is r12 would put its
@ high half in sp, and r0 is also an input of the smull. So these multiplies
@ did not come from the DI patterns. smulsi3_highpart leaves its low half a
@ dead scratch, and orr r0, r0, ip, lsr #16 reads it.
@ This proves a hand-written multiply sequence, not by itself a hand-written
@ file. Nothing else here is a tell: the routine is a frameless leaf, and
@ ldm sp, {r2, r3}, which reloads the two stack arguments into argument
@ registers already consumed, has no writeback and is within the compiler's
@ load-multiple patterns.
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
