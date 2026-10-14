	.section .rodata
	.global Data_0812a16c
Data_0812a16c:
	.byte 4, 7, 5, 0x80, 0x81
	.balign 4, 0
@ デバッグ用の仲間 (owner -1 で区切り)
@ The Broken Seal's Debug_PartyPresets, with six item columns for its four
	.global Data_0812a174
Data_0812a174:
	.incbin "games/tla/src/debug/presets.table"
