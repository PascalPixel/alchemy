.syntax unified
	.arm
	.global BattleEffect_ExpandRead2
BattleEffect_ExpandRead2:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_081a037c:
	nop
AlchemyUncreditedEnd_081a037c:
