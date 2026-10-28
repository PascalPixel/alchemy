	.section .rodata
	.global BattleEffect_ThornVineSheet
BattleEffect_ThornVineSheet:
	.incbin "games/tla/src/graphics/battle_effects/thorn_vine_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/thorn_vine.parts.lz"
	.byte 0x1e, 0x99, 0x03	@ PACKER: left in the buffer by BattleEffect_VioletStreakSheet+0x691
