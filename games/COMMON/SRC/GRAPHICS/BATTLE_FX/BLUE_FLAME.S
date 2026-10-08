	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_BlueFlameSheet
BattleFx_BlueFlameSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_FLAME_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_FLAME.parts.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_WaterSpraySheet at +0xaee */
	.byte 0xb7, 0x7c
	.endif
