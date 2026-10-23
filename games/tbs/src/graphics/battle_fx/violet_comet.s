	.section .rodata
	.global BattleAnim_VioletCometSheet
BattleAnim_VioletCometSheet:
	.incbin "games/tbs/src/graphics/battle_fx/violet_comet_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/violet_comet.parts.lz"
	.byte 0x5e, 0xda, 0x33	@ PACKER: left in the buffer by BattleAnim_FirePillarSheetA+0x3e5
