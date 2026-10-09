@ C2: private decoder linkage: ldr pc dispatches live r0/r2/r3/fp state
@ to commands returning their value in unsaved r5, which the caller reads.
.syntax unified
	.arm
	.global SentouKouka_TenkaiYomi
SentouKouka_TenkaiYomi:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_080f0100:
	nop
AlchemyUncreditedEnd_080f0100:
