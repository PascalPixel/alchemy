	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_LimeColumnSheet
BattleFx_LimeColumnSheet:
	.incbin "GRAPHICS/BATTLE_FX/LIME_COLUMN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/LIME_COLUMN.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x02	@ PACKER: left in the buffer by BattleFx_RevealMaskF+0x3cf
	.endif
