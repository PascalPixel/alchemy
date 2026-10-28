.syntax unified
	.arm
	.global Model_DispatchBitCommand
Model_DispatchBitCommand:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_08118240:
	nop
AlchemyUncreditedEnd_08118240:
