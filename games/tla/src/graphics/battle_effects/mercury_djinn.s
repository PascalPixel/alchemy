	.section .rodata
	.global BattleEffect_MercuryDjinnSheet
BattleEffect_MercuryDjinnSheet:
	.incbin "games/tla/src/graphics/battle_effects/mercury_djinn.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/mercury_djinn.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
