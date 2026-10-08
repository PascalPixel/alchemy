	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_SlashSheet
BattleFx_SlashSheet:
	.incbin "GRAPHICS/BATTLE_FX/SLASH_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SLASH.parts.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_FireBurstSheet at +0xd12 */
	.byte 0xf1, 0x59
	.endif
