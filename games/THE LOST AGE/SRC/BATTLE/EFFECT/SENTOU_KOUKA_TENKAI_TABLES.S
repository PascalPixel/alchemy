@ The dispatcher's jump table in the SENTOU_KOUKA_TENKAI.S unit: 32 zero
@ bytes (0x081a0130) that the decoder fills at run time (add r4, pc, #196;
@ str r6, [r4], #4) with the eight case addresses that follow (0x081a0150;
@ add r3, pc, #236), each moved by the distance the unit was copied. A
@ table rewritten at run time and the addresses it is built from are data
@ and carry no credit, as TBS BATTLE/EFFECT/SENTOU_KOUKA_TENKAI_TABLES.S.
.syntax unified
	.set	.L_081a0170, 0x081a0170
	.set	.L_081a018c, 0x081a018c
	.set	.L_081a022c, 0x081a022c
	.set	.L_081a01dc, 0x081a01dc
	.set	.L_081a01b4, 0x081a01b4
	.set	.L_081a025c, 0x081a025c
	.set	.L_081a0204, 0x081a0204
	.arm
	.set AlchemyLoadAddress, 0x081a0130
	.4byte	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
	.4byte	.L_081a0170
	.4byte	.L_081a018c
	.4byte	.L_081a022c
	.4byte	.L_081a01dc
	.4byte	.L_081a0170
	.4byte	.L_081a01b4
	.4byte	.L_081a025c
	.4byte	.L_081a0204
