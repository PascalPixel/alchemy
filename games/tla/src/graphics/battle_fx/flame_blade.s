	.section .rodata
	.global BattleEffect_FlameBladeSheet
BattleEffect_FlameBladeSheet:
	.incbin "games/tla/src/graphics/battle_fx/flame_blade.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/flame_blade.bitmap.lz"
	.byte 0xff	@ PACKER: left in the buffer by BattleEffect_GreyCrescentSheet+0xc83
