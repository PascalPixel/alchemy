	.section .rodata
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0BB
Resource_Data0BB:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0bb.bitmap.lz"
	.byte 0x63	@ PACKER: left in the buffer by Resource_Data0BA+0x183
