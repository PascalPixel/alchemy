	.section .rodata
	.global BattleEffect_BlueFlameSheet
BattleEffect_BlueFlameSheet:
	.incbin "games/tla/src/graphics/battle_fx/blue_flame_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/blue_flame.parts.lz"
	.byte 0xb7, 0x7c	@ PACKER: left in the buffer by BattleEffect_WaterSpraySheet+0xaee
