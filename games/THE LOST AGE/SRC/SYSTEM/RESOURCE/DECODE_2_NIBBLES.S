@ Nibble order of the stream type 2 decoder: the words 0xfedcba98 and
@ 0x76543210 hold the sixteen nibbles 0 to 15, and DECODE_2.S loads them
@ into sl and fp with add ip, pc, #36; ldm ip, {sl, fp}. A data table
@ carries no credit, as TBS SYSTEM/RESOURCE/DECODE_2_NIBBLES.S.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08012bb4
	.4byte	0xfedcba98
	.4byte	0x76543210
