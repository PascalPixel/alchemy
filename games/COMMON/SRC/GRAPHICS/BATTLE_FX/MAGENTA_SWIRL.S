	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_MagentaSwirlSheet
BattleFx_MagentaSwirlSheet:
	.incbin "GRAPHICS/BATTLE_FX/MAGENTA_SWIRL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MAGENTA_SWIRL.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_ExplosionSheet at +0x82a */
	.byte 0x1c, 0x4c
	.endif
