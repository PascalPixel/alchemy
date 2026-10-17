.syntax unified
	.section .data,"aw",%progbits
	.arm
	@ The table number last loaded, then the second colour-table copy's address less
	@ 224 bytes (unused) and less 256 bytes: table n is the 256 bytes at
	@ that base + n * 256.
	.4byte	0x00000000
	.4byte	Data_080c89cc - 0xe0
	.4byte	Data_080c89cc - 0x100
	.global BattleFx_DecodeFrameCopyEnd
BattleFx_DecodeFrameCopyEnd:
	.4byte	0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0
	.global BattleFx_DecodeFrameEnd
BattleFx_DecodeFrameEnd:
