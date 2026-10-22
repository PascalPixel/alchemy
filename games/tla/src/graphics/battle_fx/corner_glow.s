	.section .rodata
	.global BattleEffect_CornerGlowSheet
BattleEffect_CornerGlowSheet:
	.incbin "games/tla/src/graphics/battle_fx/corner_glow.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/corner_glow.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
