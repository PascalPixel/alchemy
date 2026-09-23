@ Bit-command dispatcher of the battle-effect model decoder: and ip, r3, #7
@ then ldr pc, [pc, ip, lsl #2] jumps through the table that follows
@ (SENTOU_KOUKA_TENKAI_TABLES.S) without a range check; GCC's switch jumps
@ always compare against the table size first. These 12 bytes equal TBS
@ GRAPHICS/TILE/BIT_DISPATCH.S (0x080b520c) and TLA
@ BATTLE/EFFECT/SENTOU_KOUKA_TENKAI_DISPATCH.S (0x081a0124).
@ The nop after its jump (0x080f0100), which the jump always skips, carries
@ no credit.
@ credit: library — TBS/TLA common ARM model/bit-stream decoder bank, bit-command dispatcher
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x080f00f8
SentouKouka_TenkaiYomi:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_080f0100:
	nop
AlchemyUncreditedEnd_080f0100:
