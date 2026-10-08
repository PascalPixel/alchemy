	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_PinkBurstSheet
BattleFx_PinkBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/PINK_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/PINK_BURST.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x57	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x36b
	.endif
