	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FlameColumnSheet
BattleFx_FlameColumnSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLAME_COLUMN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAME_COLUMN.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x41	@ PACKER: left in the buffer by BattleFx_RuneSheet+0x3ab
	.endif
