	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FireBlobSheet
BattleFx_FireBlobSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_BLOB.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_BLOB.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_RockWallSheet at +0xb87 */
	.byte 0x1a
	.endif
