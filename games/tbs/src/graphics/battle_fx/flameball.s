	.section .rodata
	.global BattleFx_FlameballSheet
BattleFx_FlameballSheet:
	.incbin "games/tbs/src/graphics/battle_fx/flameball.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/flameball.bitmap.lz"
	.byte 0x83	@ PACKER: left in the buffer by BattleFx_FirePillarSheetA+0x557
