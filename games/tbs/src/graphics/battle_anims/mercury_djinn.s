	.section .rodata
	.global BattleAnim_MercuryDjinnSheet
BattleAnim_MercuryDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_anims/mercury_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/mercury_djinn.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
