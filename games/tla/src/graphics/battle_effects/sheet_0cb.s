	.section .rodata
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0CB
Resource_Data0CB:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0cb.bitmap.lz"
	.byte 0x40, 0xe7, 0x09	@ PACKER: left in the buffer by Resource_Data0C8+0x7bd
