	.section .rodata
	.global BattleFx_PinkStarSheet
BattleFx_PinkStarSheet:
	.incbin "games/tbs/src/graphics/battle_fx/pink_star.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/pink_star.bitmap.lz"
	.byte 0x80	@ PACKER: left in the buffer by BattleFx_TargetBurstImage+0x38f
