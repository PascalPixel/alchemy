	.section .rodata
	.global BattleFx_SlashSheet
BattleFx_SlashSheet:
	.incbin "games/common/src/graphics/battle_fx/slash_00.gbapal"
	.incbin "games/common/src/graphics/battle_fx/slash.parts.lz"
	.byte 0xf1, 0x59	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0xd12
