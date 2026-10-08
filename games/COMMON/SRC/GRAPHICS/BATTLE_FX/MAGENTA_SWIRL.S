	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_MagentaSwirlSheet
BattleFx_MagentaSwirlSheet:
	.incbin "GRAPHICS/BATTLE_FX/MAGENTA_SWIRL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MAGENTA_SWIRL.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x1c, 0x4c	@ PACKER: left in the buffer by BattleFx_ExplosionSheet+0x82a
	.endif
