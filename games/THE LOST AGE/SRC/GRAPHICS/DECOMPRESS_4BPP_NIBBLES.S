@ Starting order of the move-to-front nibble table of DECOMPRESS_4BPP.S:
@ the words 0xfedcba98 and 0x76543210 hold the sixteen nibbles 0 to 15, and
@ the entry loads them into r2 and r3 with add r5, pc, #40;
@ ldm r5, {r2, r3}. A data table carries no credit, as TBS
@ GRAPHICS/TILE/DECOMPRESS_4BPP_NIBBLES.S.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08038b4c
	.4byte	0xfedcba98
	.4byte	0x76543210
