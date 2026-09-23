@ Bit-command dispatcher of the model decoder unit: and ip, r3, #7 then
@ ldr pc, [pc, ip, lsl #2] jumps through the runtime table that follows at
@ 0x08118244, which the decoder fills with relocated case addresses, without
@ a range check; GCC's switch jumps always compare against the table size
@ first. That table and the case addresses after it are data and stay
@ outside this module. These 12 bytes equal TBS GRAPHICS/TILE/BIT_DISPATCH.S
@ (0x080b520c).
@ The nop after its jump (0x08118240), which the jump always skips, carries
@ no credit.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank, model bit-command dispatcher
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08118238
	.global Model_DispatchBitCommand
Model_DispatchBitCommand:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_08118240:
	nop
AlchemyUncreditedEnd_08118240:
