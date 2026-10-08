	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_DaggerSheet
BattleFx_DaggerSheet:
	.incbin "GRAPHICS/BATTLE_FX/DAGGER.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/DAGGER.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_ShieldSheet at +0xb45 */
	.byte 0x0a, 0x77, 0x19
	.endif
