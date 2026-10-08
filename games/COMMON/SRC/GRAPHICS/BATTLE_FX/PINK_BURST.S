	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_PinkBurstSheet
BattleFx_PinkBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/PINK_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/PINK_BURST.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_IceBlockSheet at +0x36b */
	.byte 0x57
	.endif
