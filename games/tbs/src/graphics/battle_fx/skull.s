	.section .rodata
	.global BattleAnim_SkullSheet
BattleAnim_SkullSheet:
	.incbin "games/tbs/src/graphics/battle_fx/skull.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/skull.bitmap.lz"
	.byte 0x98, 0x43, 0x4a	@ PACKER: left in the buffer by BattleAnim_SheepSheet+0x421
