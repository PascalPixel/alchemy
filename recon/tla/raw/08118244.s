.syntax unified
	.thumb
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Model_BitCommandTable
Model_BitCommandTable:
	.4byte Model_BitCommands
	.4byte Model_AddShortDelta
	.4byte Model_AddSignedDelta
	.4byte Model_AddLongDelta
	.4byte Model_BitCommands
	.4byte Model_SubtractShortDelta
	.4byte Model_SetBitValue
	.4byte Model_SubtractLongDelta
