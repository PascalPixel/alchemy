	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FlashBurstSheet
BattleFx_FlashBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLASH_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLASH_BURST.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x60	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x144b
	.endif
