	.section .rodata
	.global BattleAnim_GreenPillarSheet
BattleAnim_GreenPillarSheet:
	.incbin "games/tbs/src/graphics/battle_fx/green_pillar_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/green_pillar.parts.lz"
	.byte 0x21, 0xf0, 0x28	@ PACKER: left in the buffer by BattleAnim_WhirlwindSheet+0x75d
