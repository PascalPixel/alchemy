	.section .rodata
	.global BattleEffect_MagentaSwirlSheet
BattleEffect_MagentaSwirlSheet:
	.incbin "games/tbs/src/graphics/battle_fx/magenta_swirl.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/magenta_swirl.bitmap.lz"
	.byte 0x1c, 0x4c	@ PACKER: left in the buffer by BattleEffect_ExplosionSheet+0x82a
