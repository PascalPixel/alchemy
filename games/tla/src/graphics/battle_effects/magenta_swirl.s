	.section .rodata
	.global BattleEffect_MagentaSwirlSheet
BattleEffect_MagentaSwirlSheet:
	.incbin "games/tla/src/graphics/battle_effects/magenta_swirl.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/magenta_swirl.bitmap.lz"
	.byte 0x1c, 0x4c	@ PACKER: left in the buffer by BattleEffect_ExplosionSheet+0x82a
