	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_SlashSheet
BattleFx_SlashSheet:
	.incbin "GRAPHICS/BATTLE_FX/SLASH_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SLASH.parts.lz"
	.ifeq GAME_TLA
	.byte 0xf1, 0x59	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0xd12
	.endif
