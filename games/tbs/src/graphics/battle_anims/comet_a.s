	.section .rodata
	.global BattleAnim_CometSheetA
BattleAnim_CometSheetA:
	.incbin "games/tbs/src/graphics/battle_anims/comet_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/comet_a.bitmap.lz"
	.byte 0x2a	@ PACKER: left in the buffer by BattleAnim_ThornSheet+0x21b
