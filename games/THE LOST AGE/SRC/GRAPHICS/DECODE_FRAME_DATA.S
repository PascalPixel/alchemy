@ Data after the final return of DECODE_FRAME.S: the mode word that the
@ decoder reads with ldr r4, [pc, #136] and rewrites with str r2, [pc, #124]
@ (0x08020f84), the table addresses 0x080202c8, which the decoder never
@ reads, and 0x080202a8, the base of its 256-byte translation tables
@ (ldr r3, [pc, #128]), and 224 zero bytes at the place of the buffer that
@ the decoder fills with the chosen table (add r3, pc, #124) and translates
@ through (add r2, pc, #48). Its loaders copy only up to 0x08020f90, so the
@ copies take the first three words and the decoder fills the buffer in its
@ heap block. A state word, two addresses and a run-time buffer are data and
@ carry no credit, as TBS GRAPHICS/RENDER/DECODE_DATA.S.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08020f84
	.4byte	0x00000000
	.4byte	0x080202c8
	.4byte	0x080202a8, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0
