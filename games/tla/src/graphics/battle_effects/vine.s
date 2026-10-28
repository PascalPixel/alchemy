	.section .rodata
	.global BattleEffect_VineSheet
BattleEffect_VineSheet:
	.incbin "games/tla/src/graphics/battle_effects/vine_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/vine.parts.lz"
	.byte 0xbe	@ PACKER: left in the buffer by BattleEffect_WhirlwindSheet+0xdcb
