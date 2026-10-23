	.section .rodata
	.global BattleAnim_ThornSheet
BattleAnim_ThornSheet:
	.incbin "games/tbs/src/graphics/battle_anims/thorn_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/thorn.parts.lz"
	.byte 0x78	@ PACKER: left in the buffer by BattleAnim_FlashBurstSheet+0xd9b
