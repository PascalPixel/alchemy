	.section .rodata
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0D8
Resource_Data0D8:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0d8.bitmap.lz"
	.byte 0x05	@ PACKER: left in the buffer by BattleEffect_LightBurstSheet+0x57b
