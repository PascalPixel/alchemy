	.section .rodata
	.global BattleEffect_VioletCometSheet
BattleEffect_VioletCometSheet:
	.incbin "games/tla/src/graphics/battle_effects/violet_comet_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/violet_comet.parts.lz"
	.byte 0x5e, 0xda, 0x33	@ PACKER: left in the buffer by BattleEffect_FirePillarSheetA+0x3e5
