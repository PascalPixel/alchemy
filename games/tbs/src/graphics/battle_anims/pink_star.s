	.section .rodata
	.global BattleAnim_PinkStarSheet
BattleAnim_PinkStarSheet:
	.incbin "games/tbs/src/graphics/battle_anims/pink_star.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/pink_star.bitmap.lz"
	.byte 0x80	@ PACKER: left in the buffer by BattleAnim_TargetBurstImage+0x38f
