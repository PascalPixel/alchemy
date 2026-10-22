	.section .rodata
	.global BattleEffect_CometSheetB
BattleEffect_CometSheetB:
	.incbin "games/tbs/src/graphics/battle_fx/comet_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/comet_b.bitmap.lz"
	.byte 0xaf	@ PACKER: left in the buffer by BattleEffect_ThornSheet+0x2fb
