	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FlameColumnSheet
BattleFx_FlameColumnSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLAME_COLUMN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAME_COLUMN.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_RuneSheet at +0x3ab */
	.byte 0x41
	.endif
