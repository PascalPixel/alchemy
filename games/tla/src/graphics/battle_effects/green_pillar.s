	.section .rodata
	.global BattleEffect_GreenPillarSheet
BattleEffect_GreenPillarSheet:
	.incbin "games/tla/src/graphics/battle_effects/green_pillar_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/green_pillar.parts.lz"
	.byte 0x21, 0xf0, 0x28	@ PACKER: left in the buffer by BattleEffect_WhirlwindSheet+0x75d
