	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_RuneSheet
BattleFx_RuneSheet:
	.incbin "GRAPHICS/BATTLE_FX/RUNE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RUNE.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x43, 0x21, 0xbe	@ PACKER: left in the buffer by BattleFx_SwordSlashSheet+0x3d9
	.endif
