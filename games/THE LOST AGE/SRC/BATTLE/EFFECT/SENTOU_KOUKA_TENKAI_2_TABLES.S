@ The dispatcher's jump table in the SENTOU_KOUKA_TENKAI_2.S unit: 32 zero
@ bytes (0x081a0380) that the decoder fills at run time (add r4, pc, #196;
@ str r6, [r4], #4) with the eight case addresses that follow (0x081a03a0;
@ add r3, pc, #236), each moved by the distance the unit was copied. A
@ table rewritten at run time and the addresses it is built from are data
@ and carry no credit, as SENTOU_KOUKA_TENKAI_TABLES.S.
.syntax unified
	.set	.L_081a03c0, 0x081a03c0
	.set	.L_081a03dc, 0x081a03dc
	.set	.L_081a047c, 0x081a047c
	.set	.L_081a042c, 0x081a042c
	.set	.L_081a0404, 0x081a0404
	.set	.L_081a04ac, 0x081a04ac
	.set	.L_081a0454, 0x081a0454
	.arm
	.set AlchemyLoadAddress, 0x081a0380
	.4byte	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
	.4byte	.L_081a03c0
	.4byte	.L_081a03dc
	.4byte	.L_081a047c
	.4byte	.L_081a042c
	.4byte	.L_081a03c0
	.4byte	.L_081a0404
	.4byte	.L_081a04ac
	.4byte	.L_081a0454
