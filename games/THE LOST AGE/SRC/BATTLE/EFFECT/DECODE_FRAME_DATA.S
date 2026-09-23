@ Data after the final return of the battle-effect bank's DECODE_FRAME.S:
@ the mode word that the decoder reads with ldr r4, [pc, #136] and rewrites
@ with str r2, [pc, #124] (0x080c95a8), the table addresses 0x080c88ec,
@ which the decoder never reads, and 0x080c88cc, the base of its 256-byte
@ translation tables (ldr r3, [pc, #128]), and 224 zero bytes at the place
@ of the buffer that the decoder fills with the chosen table and translates
@ through. Its loader copies only up to 0x080c95b4, so the copy takes the
@ first three words and the decoder fills the buffer in its heap block. A
@ state word, two addresses and a run-time buffer are data and carry no
@ credit, as TBS GRAPHICS/RENDER/DECODE_DATA.S.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x080c95a8
	.4byte	0x00000000
	.4byte	0x080c88ec
	.4byte	0x080c88cc, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0
