	.section .rodata
	.global BattleEffect_CrystalBitmapSheet
BattleEffect_CrystalBitmapSheet:
	.incbin "games/tla/src/graphics/battle_fx/crystal.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/crystal.bitmap.lz"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
