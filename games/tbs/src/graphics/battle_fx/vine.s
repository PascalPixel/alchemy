	.section .rodata
	.global BattleEffect_VineSheet
BattleEffect_VineSheet:
	.incbin "games/tbs/src/graphics/battle_fx/vine_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/vine.parts.lz"
	.byte 0xbe	@ PACKER: left in the buffer by BattleEffect_WhirlwindSheet+0xdcb
