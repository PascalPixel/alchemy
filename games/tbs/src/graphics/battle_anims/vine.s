	.section .rodata
	.global BattleAnim_VineSheet
BattleAnim_VineSheet:
	.incbin "games/tbs/src/graphics/battle_anims/vine_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/vine.bitmaps.lz"
	.byte 0xbe	@ PACKER: left in the buffer by BattleAnim_WhirlwindSheet+0xdcb
