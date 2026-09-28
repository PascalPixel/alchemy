@ The dispatcher's jump table in the SENTOU_KOUKA_TENKAI.S unit: 32 zero
@ bytes (0x081a0130) that the decoder fills at run time (add r4, pc, #196;
@ str r6, [r4], #4) with the eight case addresses that follow (0x081a0150;
@ add r3, pc, #236), each moved by the distance the unit was copied. A
@ table rewritten at run time and the addresses it is built from are data
@ and carry no credit, as TBS BATTLE/EFFECT/SENTOU_KOUKA_TENKAI_TABLES.S.
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
