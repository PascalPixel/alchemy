	.section .rodata
	.global BattleAnim_RuneSheet
BattleAnim_RuneSheet:
	.incbin "games/tbs/src/graphics/battle_anims/rune.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/rune.bitmap.lz"
	.byte 0x43, 0x21, 0xbe	@ PACKER: left in the buffer by BattleAnim_SwordSlashSheet+0x3d9
