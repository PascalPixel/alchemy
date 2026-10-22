	.section .rodata
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0F8
Resource_Data0F8:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0f8.bitmap.lz"
	.byte 0xad	@ PACKER: left in the buffer by Resource_Data0F5+0xc23
