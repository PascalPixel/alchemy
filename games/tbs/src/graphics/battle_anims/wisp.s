	.section .rodata
	.global BattleAnim_WispSheet
BattleAnim_WispSheet:
	.incbin "games/tbs/src/graphics/battle_anims/wisp.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/wisp.bitmap.lz"
	.byte 0x2e, 0x06	@ PACKER: left in the buffer by BattleAnim_SmokeSheet+0x7ae
