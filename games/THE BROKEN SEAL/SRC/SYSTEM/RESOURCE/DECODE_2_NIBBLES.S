@ Nibble order of the stream type 2 decoder: the words 0xfedcba98 and
@ 0x76543210 hold the sixteen nibbles 0 to 15, and DECODE_2.S loads them
@ into sl and fp with add ip, pc, #36; ldm ip, {sl, fp}. A data table
@ carries no credit; TLA's copy of the decoder leaves the same 8 bytes
@ outside its credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08002848
@ ニブル選択に用いる並べ替え定数。
.L_08002848:
	.4byte 0xfedcba98
	.4byte 0x76543210
