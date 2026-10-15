.syntax unified
	.section .data,"aw",%progbits
	.arm
	.4byte	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
	.global SentouKouka_TenkaiCaseTable
SentouKouka_TenkaiCaseTable:
	.4byte	SentouKouka_TenkaiCase2
	.4byte	SentouKouka_TenkaiCase4Plus
	.4byte	SentouKouka_TenkaiCase8
	.4byte	SentouKouka_TenkaiCase6Plus
	.4byte	SentouKouka_TenkaiCase2
	.4byte	SentouKouka_TenkaiCase4Minus
	.4byte	SentouKouka_TenkaiCase10
	.4byte	SentouKouka_TenkaiCase6Minus
