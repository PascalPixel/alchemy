@ C2: The dispatcher uses the decoder's live r0/r2/r3 and branches to shared
@ command code with ldr pc, [pc, ip, lsl #2]. tenkai.s and tenkai_commands.s
@ document that non-procedure linkage.
.syntax unified
	.arm
	.global SentouKouka_TenkaiYomi
SentouKouka_TenkaiYomi:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_080f0100:
	nop
AlchemyUncreditedEnd_080f0100:
