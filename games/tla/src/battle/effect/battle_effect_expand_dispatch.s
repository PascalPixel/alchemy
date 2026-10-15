.syntax unified
	.arm
	.global SentouKouka_TenkaiYomi
SentouKouka_TenkaiYomi:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_081a012c:
	nop
AlchemyUncreditedEnd_081a012c:
