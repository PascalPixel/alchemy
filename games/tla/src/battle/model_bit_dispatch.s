.syntax unified
	.arm
	.global Model_DispatchBitCommand
Model_DispatchBitCommand:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_08118240:
	nop
AlchemyUncreditedEnd_08118240:
@ Model_Decode writes the handlers' addresses here, relocated to where it runs,
@ from the table after them.
	.space 32, 0
	.global Model_BitCommandTable
Model_BitCommandTable:
	.4byte Model_BitCommands, Model_AddShortDelta, Model_AddSignedDelta, Model_AddLongDelta
	.4byte Model_BitCommands, Model_SubtractShortDelta, Model_SetBitValue, Model_SubtractLongDelta
