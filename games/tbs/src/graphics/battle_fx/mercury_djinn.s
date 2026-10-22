	.section .rodata
	.global BattleEffect_MercuryDjinnSheet
BattleEffect_MercuryDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_fx/mercury_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/mercury_djinn.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
