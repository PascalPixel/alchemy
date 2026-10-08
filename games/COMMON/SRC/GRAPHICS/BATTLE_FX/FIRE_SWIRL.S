	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FireSwirlSheet
BattleFx_FireSwirlSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_SWIRL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_SWIRL.parts.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_FlareSheet at +0x442 */
	.byte 0x12, 0x21
	.endif
