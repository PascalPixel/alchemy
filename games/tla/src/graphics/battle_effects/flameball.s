	.section .rodata
	.global BattleEffect_FlameballSheet
BattleEffect_FlameballSheet:
	.incbin "games/tla/src/graphics/battle_effects/flameball.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/flameball.bitmap.lz"
	.byte 0x83	@ PACKER: left in the buffer by BattleEffect_FirePillarSheetA+0x557
