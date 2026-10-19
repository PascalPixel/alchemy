	.section .rodata
	.global BattleFx_ThornSheet
BattleFx_ThornSheet:
	.incbin "games/tla/src/graphics/battle_fx/thorn_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/thorn.parts.lz"
	.byte 0x78	@ PACKER: left in the buffer by BattleFx_FlashBurstSheet+0xd9b
