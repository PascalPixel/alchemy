	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_LimeColumnSheet
BattleFx_LimeColumnSheet:
	.incbin "GRAPHICS/BATTLE_FX/LIME_COLUMN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/LIME_COLUMN.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_RevealMaskF at +0x3cf */
	.byte 0x02
	.endif
