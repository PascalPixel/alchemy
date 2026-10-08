	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_HeartSheet
BattleFx_HeartSheet:
	.incbin "GRAPHICS/BATTLE_FX/HEART.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/HEART.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_SkullSheet at +0x297 */
	.byte 0x09
	.endif
