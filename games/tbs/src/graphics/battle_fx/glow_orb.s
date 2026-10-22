	.section .rodata
	.global BattleEffect_GlowOrbSheet
BattleEffect_GlowOrbSheet:
	.incbin "games/tbs/src/graphics/battle_fx/glow_orb.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/glow_orb.bitmap.lz"
	.byte 0xe0	@ PACKER: left in the buffer by BattleEffect_EmberStreakSheet+0xb3b
