	.section .rodata
	.global BattleAnim_SlashSheet
BattleAnim_SlashSheet:
	.incbin "games/tbs/src/graphics/battle_fx/slash_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/slash.parts.lz"
	.byte 0xf1, 0x59	@ PACKER: left in the buffer by BattleAnim_FireBurstSheet+0xd12
