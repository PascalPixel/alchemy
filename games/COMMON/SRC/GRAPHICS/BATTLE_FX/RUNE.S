	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_RuneSheet
BattleFx_RuneSheet:
	.incbin "GRAPHICS/BATTLE_FX/RUNE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RUNE.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_SwordSlashSheet at +0x3d9 */
	.byte 0x43, 0x21, 0xbe
	.endif
