@ The dispatcher's jump table in the SENTOU_KOUKA_TENKAI_2.S unit: 32 zero
@ bytes (0x081a0380) that the decoder fills at run time (add r4, pc, #196;
@ str r6, [r4], #4) with the eight case addresses that follow (0x081a03a0;
@ add r3, pc, #236), each moved by the distance the unit was copied. A
@ table rewritten at run time and the addresses it is built from are data
@ and carry no credit, as SENTOU_KOUKA_TENKAI_TABLES.S.
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
