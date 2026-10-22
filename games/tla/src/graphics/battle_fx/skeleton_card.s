	.section .rodata
	.global BattleEffect_SkeletonCardSheet
BattleEffect_SkeletonCardSheet:
	.incbin "games/tla/src/graphics/battle_fx/skeleton_card.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/skeleton_card.bitmap.lz"
	.byte 0x7c, 0x04	@ PACKER: left in the buffer by BattleEffect_SheepCardSheet+0x6e6
