	.section .rodata
	.global BattleEffect_FlareSheet
BattleEffect_FlareSheet:
	.incbin "games/tla/src/graphics/battle_fx/flare_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/flare.parts.lz"
	.byte 0x26, 0x20	@ PACKER: left in the buffer by Resource_Data0F3+0x1d86
