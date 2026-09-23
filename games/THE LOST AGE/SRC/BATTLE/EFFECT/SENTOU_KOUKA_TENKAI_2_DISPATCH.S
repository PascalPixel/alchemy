@ Bit-command dispatcher of the second model decoder: and ip, r3, #7 then
@ ldr pc, [pc, ip, lsl #2] jumps through the runtime table that follows at
@ 0x081a0380, which the decoder fills with relocated case addresses, without
@ a range check; GCC's switch jumps always compare against the table size
@ first. That table and the case addresses after it are data and stay
@ outside this module. These 12 bytes equal TBS GRAPHICS/TILE/BIT_DISPATCH.S
@ (0x080b520c) and BATTLE/EFFECT/SENTOU_KOUKA_TENKAI_DISPATCH.S (0x080f00f8).
@ The nop after its jump (0x081a037c), which the jump always skips, carries
@ no credit.
@ credit: library — TBS/TLA common ARM model/bit-stream decoder bank, bit-command dispatcher
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x081a0374
	.global SentouKouka_TenkaiYomi2
SentouKouka_TenkaiYomi2:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_081a037c:
	nop
AlchemyUncreditedEnd_081a037c:
