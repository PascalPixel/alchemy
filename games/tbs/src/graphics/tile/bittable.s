	.section .data,"aw",%progbits
	.balign 4
@ IWRAM の写しで実行時に埋まる分岐先
	.space 32, 0
	.global Model_DispatchHandlers
Model_DispatchHandlers:
	.4byte Model_BitCommands, Model_AddShortDelta, Model_AddSignedDelta, Model_AddLongDelta
	.4byte Model_BitCommands, Model_SubtractShortDelta, Model_SetBitValue, Model_SubtractLongDelta
