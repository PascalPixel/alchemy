	.section .rodata
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0FA
Resource_Data0FA:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0fa.bitmap.lz"
	.byte 0xb2	@ PACKER: left in the buffer by Resource_Data0F9+0x1c7
