	.section .rodata
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0E9
Resource_Data0E9:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0e9.bitmap.lz"
	.byte 0x1f, 0x50	@ PACKER: left in the buffer by BattleEffect_ClawSlashSheet+0x2ca
