	.section .rodata
	.global BattleAnim_FlameballSheet
BattleAnim_FlameballSheet:
	.incbin "games/tbs/src/graphics/battle_anims/flameball.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/flameball.bitmap.lz"
	.byte 0x83	@ PACKER: left in the buffer by BattleAnim_FirePillarSheetA+0x557
