	.section .rodata
	.global BattleAnim_MagentaSwirlSheet
BattleAnim_MagentaSwirlSheet:
	.incbin "games/tbs/src/graphics/battle_fx/magenta_swirl.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/magenta_swirl.bitmap.lz"
	.byte 0x1c, 0x4c	@ PACKER: left in the buffer by BattleAnim_ExplosionSheet+0x82a
