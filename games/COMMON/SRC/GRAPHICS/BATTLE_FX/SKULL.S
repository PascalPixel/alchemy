	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_SkullSheet
BattleFx_SkullSheet:
	.incbin "GRAPHICS/BATTLE_FX/SKULL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SKULL.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_SheepSheet at +0x421 */
	.byte 0x98, 0x43, 0x4a
	.endif
