	.section .rodata
	.global BattleEffect_FlareSheet
BattleEffect_FlareSheet:
	.incbin "games/tla/src/graphics/battle_effects/flare_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/flare.parts.lz"
	.byte 0x26, 0x20	@ PACKER: left in the buffer by Resource_Data0F3+0x1d86
