	.section .rodata
	.global BattleAnim_BlueFlameSheet
BattleAnim_BlueFlameSheet:
	.incbin "games/tbs/src/graphics/battle_anims/blue_flame_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_flame.parts.lz"
	.byte 0xb7, 0x7c	@ PACKER: left in the buffer by BattleAnim_WaterSpraySheet+0xaee
