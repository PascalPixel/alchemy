	.section .rodata
	.global BattleFx_BlueFlameSheet
BattleFx_BlueFlameSheet:
	.incbin "games/tbs/src/graphics/battle_fx/blue_flame_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/blue_flame.parts.lz"
	.byte 0xb7, 0x7c	@ PACKER: left in the buffer by BattleFx_WaterSpraySheet+0xaee
