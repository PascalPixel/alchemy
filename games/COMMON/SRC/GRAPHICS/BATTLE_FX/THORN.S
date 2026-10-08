	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_ThornSheet
BattleFx_ThornSheet:
	.incbin "GRAPHICS/BATTLE_FX/THORN_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/THORN.parts.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_FlashBurstSheet at +0xd9b */
	.byte 0x78
	.endif
