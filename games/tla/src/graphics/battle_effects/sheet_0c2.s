	.section .rodata
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0C2
Resource_Data0C2:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0c2.bitmap.lz"
	.byte 0xb0, 0x3d	@ PACKER: left in the buffer by BattleEffect_CurseSealSheet+0x10ca
