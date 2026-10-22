	.section .rodata
	.global BattleEffect_FlameballSheet
BattleEffect_FlameballSheet:
	.incbin "games/tbs/src/graphics/battle_fx/flameball.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/flameball.bitmap.lz"
	.byte 0x83	@ PACKER: left in the buffer by BattleEffect_FirePillarSheetA+0x557
