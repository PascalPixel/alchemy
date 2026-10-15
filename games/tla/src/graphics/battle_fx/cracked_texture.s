	.section .rodata
	.global BattleFx_CrackedTextureSheet
BattleFx_CrackedTextureSheet:
	.incbin "games/tla/src/graphics/battle_fx/cracked_texture.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/cracked_texture.bitmap.lz"
	.byte 0x0e, 0x30, 0x00	@ PACKER: left in the buffer by BattleFx_CurseSealSheet+0x43d
