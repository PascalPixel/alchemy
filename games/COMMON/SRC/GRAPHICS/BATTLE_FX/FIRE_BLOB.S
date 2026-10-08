	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FireBlobSheet
BattleFx_FireBlobSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_BLOB.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_BLOB.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x1a	@ PACKER: left in the buffer by BattleFx_RockWallSheet+0xb87
	.endif
