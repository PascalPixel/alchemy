	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FireSwirlSheet
BattleFx_FireSwirlSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_SWIRL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_SWIRL.parts.lz"
	.ifeq GAME_TLA
	.byte 0x12, 0x21	@ PACKER: left in the buffer by BattleFx_FlareSheet+0x442
	.endif
