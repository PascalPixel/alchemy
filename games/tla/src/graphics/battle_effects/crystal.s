	.section .rodata
	.global BattleEffect_CrystalBitmapSheet
BattleEffect_CrystalBitmapSheet:
	.incbin "games/tla/src/graphics/battle_effects/crystal.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/crystal.bitmap.lz"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
