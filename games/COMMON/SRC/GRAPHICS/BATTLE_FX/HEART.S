	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_HeartSheet
BattleFx_HeartSheet:
	.incbin "GRAPHICS/BATTLE_FX/HEART.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/HEART.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x09	@ PACKER: left in the buffer by BattleFx_SkullSheet+0x297
	.endif
