	.section .rodata
	.global BattleEffect_SlashSheet
BattleEffect_SlashSheet:
	.incbin "games/tla/src/graphics/battle_effects/slash_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/slash.parts.lz"
	.byte 0xf1, 0x59	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0xd12
