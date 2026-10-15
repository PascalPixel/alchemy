	.section .rodata
	.global BattleFx_CornerGlowSheet
BattleFx_CornerGlowSheet:
	.incbin "games/tla/src/graphics/battle_fx/corner_glow.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/corner_glow.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
