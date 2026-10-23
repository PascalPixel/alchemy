	.section .rodata
	.global BattleAnim_RockWallSheet
BattleAnim_RockWallSheet:
	.incbin "games/tbs/src/graphics/battle_anims/rock_wall.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/rock_wall.bitmap.lz"
	.byte 0x07, 0x43, 0x0b	@ PACKER: left in the buffer by BattleAnim_VineSheet+0xcd1
