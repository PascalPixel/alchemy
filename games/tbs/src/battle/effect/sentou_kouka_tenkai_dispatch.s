@ C2: ldr pc, [pc, ip, lsl #2] dispatches directly into the following ARM
@ command code, rather than returning or calling through a procedure entry.
.syntax unified
	.arm
	.global SentouKouka_TenkaiYomi
SentouKouka_TenkaiYomi:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_080f0100:
	nop
AlchemyUncreditedEnd_080f0100:
