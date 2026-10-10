@ C2: smull r2, r0, r1, r0 puts the low half in r2 and the high half in r0.
@ mulsidi3 writes a DI register pair, RdHi = RdLo + 1, earlyclobbered and so
@ apart from both inputs; here RdHi is r0, not r3, and is also an input. So
@ this multiply did not come from mulsidi3. smulsi3_highpart can emit this
@ same smull, but its low half is a dead scratch, and orr r0, r0, r2, lsr #16
@ reads it.
@ This proves a hand-written multiply sequence, not by itself a hand-written
@ file. The routine has no other tell: its four instructions are a frameless
@ leaf that reuses the argument registers, as compiled code also may.
.syntax unified
	.arm

	.global IwramMulQ16
IwramMulQ16:
	smull	r2, r0, r1, r0
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	bx	lr
