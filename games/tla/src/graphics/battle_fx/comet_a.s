	.section .rodata
	.global BattleEffect_CometSheetA
BattleEffect_CometSheetA:
	.incbin "games/tla/src/graphics/battle_fx/comet_a.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/comet_a.bitmap.lz"
	.byte 0x2a	@ PACKER: left in the buffer by BattleEffect_ThornSheet+0x21b
