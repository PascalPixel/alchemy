	.section .rodata
	.global BattleEffect_YellowOrbSheet
BattleEffect_YellowOrbSheet:
	.incbin "games/tla/src/graphics/battle_effects/yellow_orb.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/yellow_orb.bitmap.lz"
	.byte 0x01, 0x9d	@ PACKER: left in the buffer by BattleEffect_LavaOrbSheet+0xa12
