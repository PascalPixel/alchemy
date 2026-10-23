	.section .rodata
	.global BattleAnim_YellowOrbSheet
BattleAnim_YellowOrbSheet:
	.incbin "games/tbs/src/graphics/battle_fx/yellow_orb.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/yellow_orb.bitmap.lz"
	.byte 0x01, 0x9d	@ PACKER: left in the buffer by BattleAnim_LavaOrbSheet+0xa12
