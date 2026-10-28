	.section .rodata
	.global BattleEffect_PinkStarSheet
BattleEffect_PinkStarSheet:
	.incbin "games/tla/src/graphics/battle_effects/pink_star.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/pink_star.bitmap.lz"
	.byte 0x80	@ PACKER: left in the buffer by BattleEffect_TargetBurstImage+0x38f
