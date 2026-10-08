	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_BlueCrescentSheet
BattleFx_BlueCrescentSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_CRESCENT.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_CRESCENT.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_FireBurstSheet at +0x21e */
	.byte 0xe0, 0x02
	.endif
