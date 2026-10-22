	.section .rodata
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0BA
Resource_Data0BA:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0ba.bitmap.lz"
	.byte 0x20	@ PACKER: left in the buffer by BattleEffect_WhirlBeamSheet+0x24b
