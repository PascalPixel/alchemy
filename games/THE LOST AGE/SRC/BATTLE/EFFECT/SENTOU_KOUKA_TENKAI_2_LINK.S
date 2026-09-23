@ Two data words of the SENTOU_KOUKA_TENKAI_2.S unit: 0x80808080, which
@ nothing in the unit reads, and the address of the case-address table
@ (0x081a03a0), which the decoder loads with ldr r2, [pc, #192] to find how
@ far the unit was moved. They sit between two routines and carry no
@ credit, as SENTOU_KOUKA_TENKAI_LINK.S.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x081a036c
	.4byte	0x80808080
	.4byte	0x081a03a0
