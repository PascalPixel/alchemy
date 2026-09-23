@ Data after the final return of DECODE.S, inside the 708-byte heap copies
@ its loaders make: the mode word that the decoder reads with
@ ldr r4, [pc, #136] and rewrites with str r2, [pc, #124] (0x08009d90), the
@ table addresses 0x080091d8, which the decoder never reads, and 0x080091b8,
@ the base of its 256-byte translation tables (ldr r3, [pc, #128]), and the
@ 224-byte buffer that it fills with the chosen table (add r3, pc, #124)
@ and translates through (add r2, pc, #48). A state word, two addresses and
@ a run-time buffer are data, so this module carries no credit; TLA leaves
@ the same 236 bytes after its DECODE_FRAME.S links outside their credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08009d90
	.4byte 0x00000000
	.4byte 0x080091d8
	.4byte 0x080091b8
@ 転送先で置換表として更新する初期化領域。
	.space 224, 0
