.syntax unified
	.section .data,"aw",%progbits
	.arm
	.4byte	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
	.global SentouKouka_Tenkai2CaseTable
SentouKouka_Tenkai2CaseTable:
	.4byte	SentouKouka_Tenkai2Case2
	.4byte	SentouKouka_Tenkai2Case4Plus
	.4byte	SentouKouka_Tenkai2Case8
	.4byte	SentouKouka_Tenkai2Case6Plus
	.4byte	SentouKouka_Tenkai2Case2
	.4byte	SentouKouka_Tenkai2Case4Minus
	.4byte	SentouKouka_Tenkai2Case10
	.4byte	SentouKouka_Tenkai2Case6Minus
