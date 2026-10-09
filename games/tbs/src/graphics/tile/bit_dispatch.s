@ C2: private decoder linkage: ldr pc dispatches live r0/r2/r3/fp state
@ to commands returning their value in unsaved r5, which the caller reads.
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
