	.section .rodata
	.global BattleEffect_ThornSheet
BattleEffect_ThornSheet:
	.incbin "games/tla/src/graphics/battle_effects/thorn_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/thorn.parts.lz"
	.byte 0x78	@ PACKER: left in the buffer by BattleEffect_FlashBurstSheet+0xd9b
