	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FlashBurstSheet
BattleFx_FlashBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLASH_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLASH_BURST.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_FireBurstSheet at +0x144b */
	.byte 0x60
	.endif
