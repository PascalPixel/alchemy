.syntax unified
.text
	.arm
	.global Model_DispatchBitCommand
Model_DispatchBitCommand:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_080b5214:
	nop
AlchemyUncreditedEnd_080b5214:
